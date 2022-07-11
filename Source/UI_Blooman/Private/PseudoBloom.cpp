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
    //MyBox.Reset();
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

        UE_LOG(LogTemp, Log, TEXT("%s: recreate."), UTF8_TO_TCHAR(__func__) );
    }

    //MyBox = SNew(SBox);
    MyPseudoBloom = SNew(SPseudoBloom);

    //if (IsDesignTime())
    //{
    //    MyBox->SetContent(
    //        SNew(SBox)
    //        .HAlign(HAlign_Center)
    //        .VAlign(VAlign_Center)
    //        [
    //            SNew(STextBlock)
    //            .Text(FText::FromName(GetFName()))
    //        ]
    //    );
    //}

    // Add any existing content to the new slate box
    if (GetChildrenCount() > 0)
    {
        UPanelSlot* ContentSlot = GetContentSlot();
        if (ContentSlot->Content)
        {
            //MyBox->SetContent(ContentSlot->Content->TakeWidget());
            MyPseudoBloom->SetContent(ContentSlot->Content->TakeWidget());
        }
    }

    return MyPseudoBloom.ToSharedRef();
}

void UPseudoBloom::OnSlotAdded(UPanelSlot* InSlot)
{
    // Add the child to the live slot if it already exists
    //if (MyBox.IsValid() && InSlot->Content)
    //{
    //    MyBox->SetContent(InSlot->Content->TakeWidget());
    //}
    if (MyPseudoBloom.IsValid() && InSlot->Content)
    {
        MyPseudoBloom->SetContent(InSlot->Content->TakeWidget());
    }
}

void UPseudoBloom::OnSlotRemoved(UPanelSlot* InSlot)
{
    // Remove the widget from the live slot if it exists.
    if (MyPseudoBloom.IsValid())
    {
        MyPseudoBloom->SetContent(SNullWidget::NullWidget);

        //if (IsDesignTime())
        //{
        //    MyBox->SetContent(
        //        SNew(SBox)
        //        .HAlign(HAlign_Center)
        //        .VAlign(VAlign_Center)
        //        [
        //            SNew(STextBlock)
        //            .Text(FText::FromName(GetFName()))
        //        ]
        //    );
        //}
    }
}

#undef LOCTEXT_NAMESPACE
