// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomUI.h"
#include "SFakeBloomUI.h"
#include "FakeBloomUI_Builder.h"
#include "FakeBloomUI_Painter.h"

#if WITH_EDITOR
#include "Kismet/KismetRenderingLibrary.h"
#endif

#define LOCTEXT_NAMESPACE "UI_BLOOMAN"

//------------------------------------------------------------------------------------------------

UFakeBloomUI::UFakeBloomUI(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    Visibility = ESlateVisibility::SelfHitTestInvisible;
}

UWidget* UFakeBloomUI::GetChildContent() const
{
    return GetContentSlot()->Content;
}

void UFakeBloomUI::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyFakeBloomUI.Reset();
}

#if WITH_EDITOR
void UFakeBloomUI::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        TSharedPtr<SWidget> SafeWidget = GetCachedWidget();
        if (SafeWidget.IsValid())
        {
            // 変更したプロパティ値に対応してRender Targetを作り直させたいので再構築
            if (Builder) {
                Builder->OnRequestRedraw();
            }

            // PainterもbUseTextureの状態を反映させたいので作り直し
            if (Painter) {
                Painter->OnRebuild();
            }
        }
    }
}

const FText UFakeBloomUI::GetPaletteCategory()
{
    return LOCTEXT("SpecialFX", "Special Effects");
}

void UFakeBloomUI::CreateNewTexture(UTextureRenderTarget2D* InRenderTarget)
{
    Builder->OnFinishBuild.RemoveDynamic(this, &UFakeBloomUI::CreateNewTexture);

    if (!Builder) {
        // ここで生成できてないとおかしい
        ensure(0);
        return;
    }

    UTexture2D* Tex = UKismetRenderingLibrary::RenderTargetCreateStaticTexture2DEditorOnly(
        InRenderTarget,
        TextureSavePath,
        WriteParameter.Format,
        TextureMipGenSettings::TMGS_NoMipmaps);

    if (Tex) {
        PaintParameter.bUseTexture = true;
        PaintParameter.BloomTexture = Tex;
    }

    OnFinishWriteJob();
}

void UFakeBloomUI::OverwriteTexture(UTextureRenderTarget2D* InRenderTarget)
{
    Builder->OnFinishBuild.RemoveDynamic(this, &UFakeBloomUI::OverwriteTexture);

    UKismetRenderingLibrary::ConvertRenderTargetToTexture2DEditorOnly(
        GetWorld(),
        InRenderTarget,
        PaintParameter.BloomTexture);

    // 圧縮設定も上書き
    PaintParameter.BloomTexture->CompressionSettings = WriteParameter.Format;
    PaintParameter.BloomTexture->PostEditChange();

    OnFinishWriteJob();
}

void UFakeBloomUI::OnFinishWriteJob()
{
    // DetailsCustomizationへの通知用
    FinishEditorCommand.ExecuteIfBound();
    FinishEditorCommand.Unbind();
}
#endif

TSharedRef<SWidget> UFakeBloomUI::RebuildWidget()
{
    // Widget DesignerへD&DでFakeBloomを含むUserWidgetを配置した際、
    // Drag時に表示されるWidgetのコピーを生成するっぽいので流用するとバグる。
    // (Drag中の半透明UFakeBloomUIがOuterだとGetWorldに失敗し各種関数が動かない)
    // https://github.com/seiko-dev/UI_Blooman/issues/36
    Builder = GetBuilder(true);
    Painter = GetPainter(true);

#if WITH_EDITOR
    // 何かしら予約があれば実行
    CheckEditorCommand.ExecuteIfBound(this);
    CheckEditorCommand.Unbind();
#endif

    if (Builder && Painter) {
        Builder->OnRebuild();
        Painter->OnRebuild();

        // Builderの成果をPainterに渡す予約
        // PainterがTObjectPtrなので、Getで生ポインタに変換しないとコンパイルが通らない
        Builder->OnFinishBuild.AddDynamic(Painter.Get(), &UFakeBloomUI_Painter::SetRenderTexture);
    }

    MyFakeBloomUI = SNew(SFakeBloomUI);
    MyFakeBloomUI->SetDrivers(Builder, Painter);

    // Add any existing content to the new slate box
    if (GetChildrenCount() > 0)
    {
        UPanelSlot* ContentSlot = GetContentSlot();
        if (ContentSlot->Content)
        {
            MyFakeBloomUI->SetContent(ContentSlot->Content->TakeWidget());

            if (Builder) {
                Builder->TargetWidget = ContentSlot->Content;
            }
        }
    }

    return MyFakeBloomUI.ToSharedRef();
}

void UFakeBloomUI::OnSlotAdded(UPanelSlot* InSlot)
{
    // Add the child to the live slot if it already exists
    if (MyFakeBloomUI.IsValid() && InSlot->Content)
    {
        MyFakeBloomUI->SetContent(InSlot->Content->TakeWidget());
    }
}

void UFakeBloomUI::OnSlotRemoved(UPanelSlot* InSlot)
{
    // Remove the widget from the live slot if it exists.
    if (MyFakeBloomUI.IsValid())
    {
        MyFakeBloomUI->SetContent(SNullWidget::NullWidget);
    }
}

UFakeBloomUI_Builder* UFakeBloomUI::GetBuilder(bool ForceRebuild)
{
    if (!Builder || ForceRebuild) {
        if (!BuilderClass) {
            // TODO: Project Settingsでの指定
            FString Path = "/UI_Blooman/B_FakeBloomUI_Builder.B_FakeBloomUI_Builder_C";
            BuilderClass = TSoftClassPtr<UFakeBloomUI_Builder>(FSoftObjectPath(*Path)).LoadSynchronous();
        }
        if (BuilderClass) {
            Builder = NewObject<UFakeBloomUI_Builder>(this, BuilderClass, BuilderClass->GetFName());
            Builder->SetParameters(&BuildParameter, &PaintParameter);

        } else {
            check(0);
        }
    }
    return Builder;
}

UFakeBloomUI_Painter* UFakeBloomUI::GetPainter(bool ForceRebuild)
{
    if (!Painter || ForceRebuild) {
        if (!PainterClass) {
            // TODO: Project Settingsでの指定
            // B_FakeBloom_Builder
            FString Path = "/UI_Blooman/B_FakeBloomUI_Painter.B_FakeBloomUI_Painter_C";
            PainterClass = TSoftClassPtr<UFakeBloomUI_Painter>(FSoftObjectPath(*Path)).LoadSynchronous();
        }
        if (PainterClass) {
            Painter = NewObject<UFakeBloomUI_Painter>(this, PainterClass, PainterClass->GetFName());
            Painter->SetParameters(&BuildParameter, &PaintParameter);

        } else {
            check(0);
        }
    }
    return Painter;
}

bool UFakeBloomUI::IsDesignTime() const
{
#if WITH_EDITOR
    return UWidget::IsDesignTime();
#else
    return false;
#endif
}

#undef LOCTEXT_NAMESPACE
