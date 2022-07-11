// Copyright seiko_dev. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

class UI_BLOOMAN_API SPseudoBloom : public SCompoundWidget
{
    SLATE_DECLARE_WIDGET(SPseudoBloom, SCompoundWidget)
public:
    SLATE_BEGIN_ARGS(SPseudoBloom)
    {
        _Visibility = EVisibility::SelfHitTestInvisible;
    }

    SLATE_DEFAULT_SLOT(FArguments, Content)
    SLATE_END_ARGS()

public:
    SPseudoBloom();
    void Construct(const FArguments& InArgs);
    void SetContent(const TSharedRef<SWidget>& InContent);
    void SetDriver(class UPseudoBloomDriver* InDriver) { Driver = InDriver; }
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
    UPseudoBloomDriver* Driver;
};
