// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloom.h"
#include "SFakeBloom.h"
#include "Slate/WidgetRenderer.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Input/HittestGrid.h"

#define LOCTEXT_NAMESPACE "UI_BLOOMAN"

bool UFakeBloomDriver::DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                                            class UWidget* WidgetToRender,
                                            const FFakeBloomPreProcessArgs& PreProcessArgs,
                                            float Overhang,
                                            bool UseGamma,
                                            bool UpdateImmediate,
                                            int32& NumMips)
{
    const FVector2D& LocalSize = PreProcessArgs.Geometry.GetLocalSize();
    
    if (!WidgetToRender)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : WidgetToRender is empty!"));
        return false;
    }
    if (LocalSize.X < 0 || LocalSize.Y < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : LocalSize is 0 or less!"));
        return false;
    }
    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : Target is empty!"));
        return false;
    }

    FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(UseGamma, false);
    check(WidgetRenderer);

    {
        const FVector2D DrawOffset(Overhang, Overhang);
        WidgetRenderer->SetIsPrepassNeeded(false); // Paintで先に描画済み(Layout計算済み)なのでPrepass不要

#if 1 /* Niagara UI Renderer対応実装 */
        // やってる事のメモ
        // https://github.com/seiko-dev/UI_Blooman/issues/29

        const float Scale = PreProcessArgs.Geometry.GetAccumulatedLayoutTransform().GetScale();
        const FVector2D RectLT(PreProcessArgs.CullingRect.Left, PreProcessArgs.CullingRect.Top);
        const FVector2D AbsolutePos = PreProcessArgs.Geometry.GetAccumulatedRenderTransform().GetTranslation();

        // ContentPos部分の距離は、Widget DesingerがZoomしている場合に拡縮がかかるので、打ち消しておく。
        FVector2D ContentPos = (AbsolutePos - RectLT)/Scale;

        // テクスチャ自体は等倍で描く必要があるので、Scaleは常に1.0。
        FGeometry WindowGeometry = FGeometry::MakeRoot(ContentPos + LocalSize, FSlateLayoutTransform(1.0));

        // Geometryと一致する大きさ
        FSlateRect WindowClipRect = WindowGeometry.GetLayoutBoundingRect();

        // 一時的に付け替えるので親を覚えておく
        const TSharedRef<SWidget>& Content = WidgetToRender->TakeWidget();
        TSharedPtr<SWidget> OldParent = Content->GetParentWidget();

        TSharedRef<SConstraintCanvas> Canvas = SNew(SConstraintCanvas)
            + SConstraintCanvas::Slot()
            .Anchors(FAnchors(0, 0, 1, 1))
            .Offset(FMargin(ContentPos.X, ContentPos.Y, 0, 0))
            .Alignment(FVector2D(0, 0))
            [
                Content
            ];

        TSharedRef<SVirtualWindow> Window = SNew(SVirtualWindow).Size(WindowGeometry.GetLocalSize());
        Window->SetContent(Canvas);

        WidgetRenderer->ViewOffset = DrawOffset - ContentPos;

        WidgetRenderer->DrawWindow(
            Target->GameThread_GetRenderTargetResource(),
            *MakeUnique<FHittestGrid>(),
            Window,
            WindowGeometry,
            WindowClipRect,
            0.0f);

#if 0
        // https://github.com/seiko-dev/UI_Blooman/issues/32
        // ↑に取り組む時に使いそうなので残しておく
        UE_LOG(LogTemp, Log, TEXT("%s:     a %s"), UTF8_TO_TCHAR(__func__), *AbsolutePos.ToString());
        UE_LOG(LogTemp, Log, TEXT("%s:     c %s"), UTF8_TO_TCHAR(__func__), *ContentPos.ToString());
        UE_LOG(LogTemp, Log, TEXT("%s:     L %s"), UTF8_TO_TCHAR(__func__), *LocalSize.ToString());
        UE_LOG(LogTemp, Log, TEXT("%s:     w %s"), UTF8_TO_TCHAR(__func__), *WindowGeometry.GetLocalSize().ToString());
        UE_LOG(LogTemp, Log, TEXT("%s: "), UTF8_TO_TCHAR(__func__));
#endif

        // 付け替えた親戻し
        if (OldParent.IsValid())
        {
            Content->AssignParentWidget(OldParent);
        }

        FlushRenderingCommands();
        BeginCleanup(WidgetRenderer);


#else /* シンプル版 */
        WidgetRenderer->ViewOffset = DrawOffset;
        WidgetRenderer->DrawWidget(Target, WidgetToRender->TakeWidget(), LocalSize, 0.0);

        FlushRenderingCommands();
        BeginCleanup(WidgetRenderer);
#endif

        NumMips = 0;

        if (UpdateImmediate) {
            Target->UpdateResourceImmediate(false);
            NumMips = Target->GetNumMips();
        }
    }

    return true;
}

// Editor用テクスチャ生成処理終了時の呼出
void UFakeBloomDriver::NotifyCreateTextureFinished()
{
#if WITH_EDITOR
    Widget->NotifyCreateTextureFinished();
#endif
}


void UFakeBloomDriver::DrawSlateBrush(UPARAM(ref) FPaintContext& Context, const FSlateBrush& Brush)
{
    Context.MaxLayer++;

    FVector2D Position = (Context.AllottedGeometry.GetLocalSize() - Brush.ImageSize) * 0.5f;

    FSlateDrawElement::MakeBox(
        Context.OutDrawElements,
        Context.MaxLayer,
        Context.AllottedGeometry.ToPaintGeometry(Position, Brush.ImageSize),
        &Brush,
        ESlateDrawEffect::None,
        Brush.TintColor.GetSpecifiedColor());
}

UFakeBloom::UFakeBloom(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    Visibility = ESlateVisibility::SelfHitTestInvisible;
}

UWidget* UFakeBloom::GetChildContent() const
{
    return GetContentSlot()->Content;
}

void UFakeBloom::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyFakeBloom.Reset();
}

#if WITH_EDITOR
void UFakeBloom::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        TSharedPtr<SWidget> SafeWidget = GetCachedWidget();
        if (SafeWidget.IsValid())
        {
            // プロパティ変えたら再構築
            GetDriver()->OnRebuild();
        }
    }
}

const FText UFakeBloom::GetPaletteCategory()
{
    return LOCTEXT("SpecialFX", "Special Effects");
}

void UFakeBloom::RequestCreateNewTexture(const FString& Path)
{
    // 同じインスタンスに対して、RebuildWidgetより先に来てしまうのでここでDriverを作る場合もある
    GetDriver()->RequestCreateNewTexture(Path);
}

void UFakeBloom::RequestOverwriteTexture()
{
    // 同じインスタンスに対して、RebuildWidgetより先に来てしまうのでここでDriverを作る場合もある
    GetDriver()->RequestOverwriteTexture();
}

void UFakeBloom::NotifyCreateTextureFinished()
{
    // プロパティ変化をSlateに通知
    if (PaintParamHandle.IsValid()) {
        UObject* Prop(nullptr);
        if (PaintParamHandle->GetValue(Prop) == FPropertyAccess::Result::Success) {
            PaintParamHandle->SetValue(Prop);
        } else {
            UE_LOG(LogTemp, Log, TEXT("%s: valueget failed."), UTF8_TO_TCHAR(__func__) );
        }
    }

    // DetailsCustomizationが何かBindしてるかもしれないので通知(Outline制御の後始末とか
    CreateTextureCallBack.ExecuteIfBound();
    CreateTextureCallBack.Unbind();
}
#endif

TSharedRef<SWidget> UFakeBloom::RebuildWidget()
{
    GetDriver()->OnRebuild();

    MyFakeBloom = SNew(SFakeBloom);

    // Add any existing content to the new slate box
    if (GetChildrenCount() > 0)
    {
        UPanelSlot* ContentSlot = GetContentSlot();
        if (ContentSlot->Content)
        {
            MyFakeBloom->SetContent(ContentSlot->Content->TakeWidget(), Driver);
        }
    }

    return MyFakeBloom.ToSharedRef();
}

void UFakeBloom::OnSlotAdded(UPanelSlot* InSlot)
{
    // Add the child to the live slot if it already exists
    if (MyFakeBloom.IsValid() && InSlot->Content)
    {
        MyFakeBloom->SetContent(InSlot->Content->TakeWidget(), Driver);
    }
}

void UFakeBloom::OnSlotRemoved(UPanelSlot* InSlot)
{
    // Remove the widget from the live slot if it exists.
    if (MyFakeBloom.IsValid())
    {
        MyFakeBloom->SetContent(SNullWidget::NullWidget, nullptr);
    }
}

UFakeBloomDriver* UFakeBloom::GetDriver()
{
    if (!Driver) {
        if (!DriverClass) {
            FString Path = "/UI_Blooman/B_FakeBloomDriver.B_FakeBloomDriver_C";
            DriverClass = TSoftClassPtr<UFakeBloomDriver>(FSoftObjectPath(*Path)).LoadSynchronous();
        }
        if (DriverClass) {
            Driver = NewObject<UFakeBloomDriver>(this, DriverClass, TEXT("UFakeBloomDriver"));
            Driver->SetWidget(this);
        }
    }
    return Driver;
}

bool UFakeBloom::IsDesignTime() const
{
#if WITH_EDITOR
    return UWidget::IsDesignTime();
#else
    return false;
#endif
}

#undef LOCTEXT_NAMESPACE
