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
        : Overhang(16.0f)
        , AlphaToLuminance(1.0f)
        , LuminanceThreshold(0.0f)
        , Strength(1.0f)
        , Spead(1.0f)
        , MaxMipLevel(5)
        , Compression(1)
        , BuildEveryFrame(false)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build")
    float Overhang;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float AlphaToLuminance;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float LuminanceThreshold;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build")
    float Strength;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build")
    float Spead;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build", meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxMipLevel;

    // Free SizeでTexture保存できるようになるまで0は封印
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build", meta = (ClampMin = "1", UIMin = "1"))
    int32 Compression;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Build")
    bool BuildEveryFrame;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FUI_BloomPaintParameter
{
    GENERATED_BODY()
public:
    FUI_BloomPaintParameter()
        : TintColor(FLinearColor::White)
        , SizeScale(1.0f, 1.0f)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint")
    FLinearColor TintColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint")
    FVector2D SizeScale;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint")
    bool bUseTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint", meta = (EditCondition = "bUseTexture"))
    UTexture2D* BloomTexture;
};

UCLASS(Abstract, Blueprintable)
class UI_BLOOMAN_API UPseudoBloomDriver : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "Pseudo Bloom")
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

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Create Texture")
    void RequestCreateNewTexture(const FString& Path);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Create Texture")
    void RequestOverwriteTexture();

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void NotifyCreateTextureFinished();

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

    UFUNCTION(BlueprintCallable, Category = "Pseudo Bloom")
    UWidget* GetChildContent() const;

    UFUNCTION(BlueprintCallable, Category = "Pseudo Bloom")
    UPseudoBloomDriver* GetDriver() const { return Driver; }

    UFUNCTION(BlueprintCallable, Category = "Pseudo Bloom")
    bool IsDesignTime() const;

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pseudo Bloom")
    FUI_BloomBuildParameter BuildParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Pseudo Bloom")
    FUI_BloomPaintParameter PaintParameter;

    //UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint")
    //    UTexture2D* sampleTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear, AdvancedDisplay, Category = "Pseudo Bloom")
    TSubclassOf<UPseudoBloomDriver> DriverClass;


#if WITH_EDITOR
    DECLARE_DELEGATE(FCreateTextureCallBack);
    FCreateTextureCallBack CreateTextureCallBack;

    TSharedPtr<IPropertyHandle> PaintParamHandle;
#endif

public:
    // UPanelWidget interface
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
    // UObject interface
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    // UWidget interface
    virtual const FText GetPaletteCategory() override;

    void RequestCreateNewTexture(const FString& Path);
    void RequestOverwriteTexture();
    void NotifyCreateTextureFinished();
#endif

protected:
    // UWidget interface 
    virtual TSharedRef<SWidget> RebuildWidget() override;

    // UPanelWidget interface
    virtual void OnSlotAdded(UPanelSlot* InSlot) override;
    virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

    UPseudoBloomDriver* GetDriver();

private:
    UPROPERTY()
    UPseudoBloomDriver* Driver;

protected:
    TSharedPtr<class SPseudoBloom> MyPseudoBloom;
};
