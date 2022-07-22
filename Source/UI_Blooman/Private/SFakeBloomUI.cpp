// Copyright seiko_dev. All Rights Reserved.
#include "SFakeBloomUI.h"
#include "FakeBloomUI.h"


SLATE_IMPLEMENT_WIDGET(SFakeBloomUI)
void SFakeBloomUI::PrivateRegisterAttributes(FSlateAttributeInitializer& AttributeInitializer)
{
}

SFakeBloomUI::SFakeBloomUI()
    : Builder(nullptr)
    , Painter(nullptr)
{
}

void SFakeBloomUI::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            InArgs._Content.Widget
        ];

    SetCanTick(false);
}

void SFakeBloomUI::SetContent(const TSharedRef<SWidget>& InContent)
{
    ChildSlot.AttachWidget(InContent);
}

void SFakeBloomUI::SetDrivers(UFakeBloomUI_Builder* InBuilder, UFakeBloomUI_Painter* InPainter)
{
    Builder = InBuilder;
    Painter = InPainter;
}

int32 SFakeBloomUI::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // 前処理
    if (Builder) {
        const_cast<SFakeBloomUI*>(this)->Builder->OnPaintPreProcess(FFakeBloomUI_PreProcessArgs(AllottedGeometry, MyCullingRect));
    }
    
    // 先にコンテンツを描いてから
    LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    // 上にBloomを描く
    if (Painter) {
        FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
        Painter->OnPaint(Context);

        LayerId = FMath::Max(LayerId, Context.MaxLayer);
    }
    
    return LayerId;
}
