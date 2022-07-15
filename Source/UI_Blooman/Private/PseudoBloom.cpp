// Copyright seiko_dev. All Rights Reserved.

#include "PseudoBloom.h"
#include "SPseudoBloom.h"

#define LOCTEXT_NAMESPACE "UI_BLOOMAN"

// Editor用テクスチャ生成処理終了時の呼出
void UPseudoBloomDriver::NotifyCreateTextureFinished()
{
#if WITH_EDITOR
    Widget->NotifyCreateTextureFinished();
#endif
}


UPseudoBloom::UPseudoBloom(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    Visibility = ESlateVisibility::SelfHitTestInvisible;
}

UWidget* UPseudoBloom::GetChildContent() const
{
    return GetContentSlot()->Content;
}

void UPseudoBloom::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyPseudoBloom.Reset();
}

#if WITH_EDITOR
void UPseudoBloom::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
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

const FText UPseudoBloom::GetPaletteCategory()
{
    return LOCTEXT("SpecialFX", "Special Effects");
}

void UPseudoBloom::RequestCreateNewTexture(const FString& Path)
{
    // 同じインスタンスに対して、RebuildWidgetより先に来てしまうのでここでDriverを作る場合もある
    GetDriver()->RequestCreateNewTexture(Path);
}

void UPseudoBloom::RequestOverwriteTexture()
{
    // 同じインスタンスに対して、RebuildWidgetより先に来てしまうのでここでDriverを作る場合もある
    GetDriver()->RequestOverwriteTexture();
}

void UPseudoBloom::NotifyCreateTextureFinished()
{
    // プロパティ変化をSlateに通知
    if (PaintTexHandle.IsValid()) {
        UObject* Prop(nullptr);
        if (PaintTexHandle->GetValue(Prop) == FPropertyAccess::Result::Success) {
            PaintTexHandle->SetValue(Prop);
        } else {
            UE_LOG(LogTemp, Log, TEXT("%s: valueget failed."), UTF8_TO_TCHAR(__func__) );
        }
    }

    // DetailsCustomizationが何かBindしてるかもしれないので通知(Outline制御の後始末とか
    CreateTextureCallBack.ExecuteIfBound();
    CreateTextureCallBack.Unbind();
}
#endif

TSharedRef<SWidget> UPseudoBloom::RebuildWidget()
{
    GetDriver()->OnRebuild();

    MyPseudoBloom = SNew(SPseudoBloom);

    // Add any existing content to the new slate box
    if (GetChildrenCount() > 0)
    {
        UPanelSlot* ContentSlot = GetContentSlot();
        if (ContentSlot->Content)
        {
            MyPseudoBloom->SetContent(ContentSlot->Content->TakeWidget(), Driver);
        }
    }

    return MyPseudoBloom.ToSharedRef();
}

void UPseudoBloom::OnSlotAdded(UPanelSlot* InSlot)
{
    // Add the child to the live slot if it already exists
    if (MyPseudoBloom.IsValid() && InSlot->Content)
    {
        MyPseudoBloom->SetContent(InSlot->Content->TakeWidget(), Driver);
    }
}

void UPseudoBloom::OnSlotRemoved(UPanelSlot* InSlot)
{
    // Remove the widget from the live slot if it exists.
    if (MyPseudoBloom.IsValid())
    {
        MyPseudoBloom->SetContent(SNullWidget::NullWidget, nullptr);
    }
}

UPseudoBloomDriver* UPseudoBloom::GetDriver()
{
    if (!Driver) {
        if (!DriverClass) {
            FString Path = "/UI_Blooman/B_PseudoBloomDriver.B_PseudoBloomDriver_C";
            DriverClass = TSoftClassPtr<UPseudoBloomDriver>(FSoftObjectPath(*Path)).LoadSynchronous();
        }
        if (DriverClass) {
            Driver = NewObject<UPseudoBloomDriver>(this, DriverClass, TEXT("UPseudoBloomDriver"));
            Driver->SetWidget(this);
        }
    }
    return Driver;
}

#undef LOCTEXT_NAMESPACE
