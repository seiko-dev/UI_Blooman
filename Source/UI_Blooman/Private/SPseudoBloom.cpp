// Copyright seiko_dev. All Rights Reserved.
#include "SPseudoBloom.h"
#include "PseudoBloom.h"

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

void SPseudoBloom::SetContent(const TSharedRef<SWidget>& InContent)
{
    ChildSlot.AttachWidget(InContent);
}

SLATE_IMPLEMENT_WIDGET(SPseudoBloom)

int32 SPseudoBloom::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    if (Driver) {
        const_cast<SPseudoBloom*>(this)->Driver->PaintPreProcess(AllottedGeometry);

        FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
        Driver->OnPaint(Context);

        LayerId = FMath::Max(LayerId, Context.MaxLayer);
    }

    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void SPseudoBloom::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}
