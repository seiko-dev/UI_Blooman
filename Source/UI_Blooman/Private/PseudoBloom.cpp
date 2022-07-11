// Copyright seiko_dev. All Rights Reserved.

#include "PseudoBloom.h"
#include "SPseudoBloom.h"

#define LOCTEXT_NAMESPACE "UI_BLOOMAN"

UPseudoBloom::UPseudoBloom(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    Visibility = ESlateVisibility::SelfHitTestInvisible;
}

void UPseudoBloom::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyPseudoBloom.Reset();
}

#if WITH_EDITOR
const FText UPseudoBloom::GetPaletteCategory()
{
    return LOCTEXT("SpecialFX", "Special Effects");
}
#endif

TSharedRef<SWidget> UPseudoBloom::RebuildWidget()
{
    if (!DriverClass) {
        FString Path = "/UI_Blooman/B_PseudoBloomDriver.B_PseudoBloomDriver_C";
        DriverClass = TSoftClassPtr<UPseudoBloomDriver>(FSoftObjectPath(*Path)).LoadSynchronous();
    }
    if (DriverClass) {
        Driver = NewObject<UPseudoBloomDriver>(this, DriverClass, TEXT("UPseudoBloomDriver"));
        Driver->SetWidget(this);
        Driver->OnRebuildWidget(IsDesignTime());
    }

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

#undef LOCTEXT_NAMESPACE
