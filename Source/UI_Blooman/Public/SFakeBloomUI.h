// Copyright seiko_dev. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

class UI_BLOOMAN_API SFakeBloom : public SCompoundWidget
{
    SLATE_DECLARE_WIDGET(SFakeBloom, SCompoundWidget)
public:
    SLATE_BEGIN_ARGS(SFakeBloom)
    {
        _Visibility = EVisibility::SelfHitTestInvisible;
    }

    SLATE_DEFAULT_SLOT(FArguments, Content)
    SLATE_END_ARGS()

public:
    SFakeBloom();
    void Construct(const FArguments& InArgs);
    void SetContent(const TSharedRef<SWidget>& InContent, class UFakeBloomDriver* InDriver);
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
    UFakeBloomDriver* Driver;
};
