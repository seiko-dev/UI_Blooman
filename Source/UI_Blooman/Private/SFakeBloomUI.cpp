// Copyright seiko_dev. All Rights Reserved.
#include "SFakeBloom.h"
#include "FakeBloom.h"


SLATE_IMPLEMENT_WIDGET(SFakeBloom)
void SFakeBloom::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SFakeBloom::SFakeBloom()
    : Driver(nullptr)
{
}

void SFakeBloom::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            InArgs._Content.Widget
        ];

    SetCanTick(false);
}

void SFakeBloom::SetContent(const TSharedRef<SWidget>& InContent, class UFakeBloomDriver* InDriver)
{
    ChildSlot.AttachWidget(InContent);
    Driver = InDriver;
}

int32 SFakeBloom::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // 前処理
    if (Driver) {
        const_cast<SFakeBloom*>(this)->Driver->OnPaintPreProcess(FFakeBloomPreProcessArgs(AllottedGeometry, MyCullingRect));
    }
    
    // 先にコンテンツを描いてから
    LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    // 上にBloomを描く
    if (Driver) {
        FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
        Driver->OnPaint(Context, AllottedGeometry);

        LayerId = FMath::Max(LayerId, Context.MaxLayer);
    }
    
    return LayerId;
}
