// Copyright seiko_dev. All Rights Reserved.
#include "SPseudoBloom.h"
#include "PseudoBloom.h"


SLATE_IMPLEMENT_WIDGET(SPseudoBloom)
void SPseudoBloom::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SPseudoBloom::SPseudoBloom()
    : Driver(nullptr)
{
}

void SPseudoBloom::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            InArgs._Content.Widget
        ];
}

void SPseudoBloom::SetContent(const TSharedRef<SWidget>& InContent, class UPseudoBloomDriver* InDriver)
{
    ChildSlot.AttachWidget(InContent);
    Driver = InDriver;
}

int32 SPseudoBloom::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // 先にコンテンツを描いてから
    LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    // 上にBloomを描く
    if (Driver) {
        const_cast<SPseudoBloom*>(this)->Driver->OnPaintPreProcess(AllottedGeometry);

        FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
        Driver->OnPaint(Context, AllottedGeometry);

        LayerId = FMath::Max(LayerId, Context.MaxLayer);       
    }
    
    return LayerId;
}
