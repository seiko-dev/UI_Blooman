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
    FUI_BloomBuildParameter()
        : Overhang(10.0f)
        , AlphaToLuminance(1.0f)
        , LuminanceThreshold(0.0f)
        , Strength(1.0f)
        , Spead(1.0f)
        , MaxMipLevel(5)
        , BuildEveryFrame(false)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Overhang;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float AlphaToLuminance;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float LuminanceThreshold;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Strength;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Spead;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxMipLevel;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool BuildEveryFrame;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FUI_BloomDrawParameter
{
    GENERATED_BODY()
public:
    FUI_BloomDrawParameter()
        : TintColor(FLinearColor::White)
        , SizeScale(1.0f, 1.0f)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FLinearColor TintColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector2D SizeScale;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UTexture2D* BloomTexture;
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
    void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
    void OnPaintPreProcess(const FGeometry& MyGeometry);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void OnPaint(UPARAM(ref) FPaintContext& Context, const FGeometry& Geometry) const;

private:
    virtual class UWorld* GetWorld() const
    {
        if (!HasAnyFlags(RF_ClassDefaultObject) // CDOでは無効
            && ensureMsgf(GetOuter(), TEXT("%s has a null OuterPrivate in GetWorld()"), *GetFullName()) // Outerあるよね？
            && !GetOuter()->HasAnyFlags(RF_BeginDestroyed)  // Outer死にかけてたら無効
            && !GetOuter()->IsUnreachable()) // Outerない事になってたら無効
        {
            return GetOuter()->GetWorld();
        }
        return nullptr;
    }
};

/**
 * 
 */
UCLASS()
class UI_BLOOMAN_API UPseudoBloom : public UContentWidget
{
    GENERATED_BODY()
public:
    UPseudoBloom(const FObjectInitializer& ObjectInitializer);


    UFUNCTION(BlueprintCallable)
    UWidget* GetChildContent() const;

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FUI_BloomBuildParameter BuildParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FUI_BloomDrawParameter DrawParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear)
    TSubclassOf<UPseudoBloomDriver> DriverClass;


    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
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
