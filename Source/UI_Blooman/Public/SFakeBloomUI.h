// Copyright seiko_dev. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "Widgets/SPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWidget.h"

class UI_BLOOMAN_API SFakeBloomUI : public SCompoundWidget
{
    SLATE_DECLARE_WIDGET(SFakeBloomUI, SCompoundWidget)
public:
    SLATE_BEGIN_ARGS(SFakeBloomUI)
    {
        _Visibility = EVisibility::SelfHitTestInvisible;
    }

    SLATE_DEFAULT_SLOT(FArguments, Content)
    SLATE_END_ARGS()

public:
    SFakeBloomUI();
    void Construct(const FArguments& InArgs);
    void SetContent(const TSharedRef<SWidget>& InContent);
    void SetWidget(UFakeBloomUI* InWidget);
    virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:
    TObjectPtr<class UFakeBloomUI> Widget;
};
