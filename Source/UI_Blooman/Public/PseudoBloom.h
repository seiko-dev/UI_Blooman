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

    // Amount of bloom to draw outside the Widget.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build")
    float Overhang;

    // The closer to 1.0, the more only the brightest pixels bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float AlphaToLuminance;

    // Blooming transparency threshold.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float LuminanceThreshold;

    // Adjust the strength of the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build")
    float Strength;

    // Fine-tune the bloom spread.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build")
    float Spead;

    // The larger this is, the higher level MipMap is used, and the wider the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxMipLevel;

    // Final Texture Compression Strength
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "1", UIMin = "1"))
    int32 Compression;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build")
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
        , bUseTexture(false)
        , BloomTexture(nullptr)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint")
    FLinearColor TintColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint")
    FVector2D SizeScale;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint")
    bool bUseTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint", meta = (EditCondition = "bUseTexture"))
    UTexture2D* BloomTexture;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FPseudoBloomPreProcessArgs
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly)
    FGeometry Geometry;

    FSlateRect CullingRect;

    FPseudoBloomPreProcessArgs()
    {
    }

    FPseudoBloomPreProcessArgs(const FGeometry& InGeometry, const FSlateRect& InCullingRect)
        : Geometry(InGeometry)
        , CullingRect(InCullingRect)
    {}
};


UCLASS(Abstract, Blueprintable)
class UI_BLOOMAN_API UPseudoBloomDriver : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
        void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
        void OnPaintPreProcess(const FPseudoBloomPreProcessArgs& PreProcessArgs);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
        void OnPaint(UPARAM(ref) FPaintContext& Context, const FGeometry& Geometry) const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Create Texture")
        void RequestCreateNewTexture(const FString& Path);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Create Texture")
        void RequestOverwriteTexture();

public:
    UFUNCTION(BlueprintCallable)
    bool DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                            class UWidget* WidgetToRender,
                            const FPseudoBloomPreProcessArgs& PreProcessArgs,
                            float Overhang,
                            bool UseGamma,
                            bool UpdateImmediate,
                            int32& NumMips);

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void NotifyCreateTextureFinished();

    UFUNCTION(Category = "Painting", BlueprintCallable)
    static void DrawSlateBrush(UPARAM(ref) FPaintContext& Context, const FSlateBrush& Brush);

public:
    UPROPERTY(BlueprintReadOnly, Category = "Pseudo Bloom")
    UPseudoBloom* Widget;

public:
    void SetWidget(UPseudoBloom* InWidget) {
        Widget = InWidget;
    }

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
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Pseudo Bloom")
    FUI_BloomBuildParameter BuildParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Pseudo Bloom")
    FUI_BloomPaintParameter PaintParameter;

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
