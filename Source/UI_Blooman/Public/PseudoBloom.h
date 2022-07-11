// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "PseudoBloom.generated.h"


USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FUI_BloomBuildParameter 
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float build_param;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FUI_BloomDrawParameter
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
        float draw_param;
};

UCLASS(Abstract, Blueprintable)
class UI_BLOOMAN_API UPseudoBloomDriver : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly)
    UPseudoBloom* Widget;

    void SetWidget(UPseudoBloom* InWidget) {
        Widget = InWidget;
    }

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
    void PaintPreProcess(const FGeometry& MyGeometry);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
    void OnRebuildWidget(bool IsDesignTime);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void OnPaint(UPARAM(ref) FPaintContext& Context, const FGeometry& Geometry) const;
};

/**
 * 
 */
UCLASS()
class UI_BLOOMAN_API UPseudoBloom : public UContentWidget
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FUI_BloomBuildParameter BuildParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FUI_BloomDrawParameter DrawParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear)
    TSubclassOf<UPseudoBloomDriver> DriverClass;

    UPseudoBloom(const FObjectInitializer& ObjectInitializer);

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
    virtual const FText GetPaletteCategory() override;
#endif

protected:
    /** UWidget interface */
    virtual TSharedRef<SWidget> RebuildWidget() override;

    /** UPanelWidget interface */
    virtual void OnSlotAdded(UPanelSlot* InSlot) override;
    virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

private:
    UPROPERTY()
    UPseudoBloomDriver* Driver;

protected:
    TSharedPtr<class SPseudoBloom> MyPseudoBloom;
};
