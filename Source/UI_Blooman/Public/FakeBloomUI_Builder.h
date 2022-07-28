// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FakeBloomUI_Builder.generated.h"

UENUM(BlueprintType)
enum class EFakeBloomUI_BuildPhase : uint8
{
    // If no bloom texture use, build only once when creating the widget.
    AtCreate,

    // Same as "AtCreate" in game, but always builds in Widget Designer. 
    AtDesignTime,

    // Always build in game. *Note the impact on performance!*
    Always,
};

// 
UCLASS(Abstract, Blueprintable, EditInlineNew)
class UI_BLOOMAN_API UFakeBloomUI_Builder : public UObject
{
    GENERATED_BODY()
public:
    // The closer to 1.0, the more only the brightest pixels bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float AlphaToLuminance;

    // Blooming transparency threshold.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float LuminanceThreshold;

    // Adjust the strength of the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float Strength;

    // Fine-tune the bloom spread.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float Spread;

    // The larger this is, the higher level MipMap is used, and the wider the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxMipLevel;

    // Final Texture Compression Strength
    // 0 is resereved for free texture size (not pad to power of 2) mode.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0", UIMin = "1"))
    int32 Compression;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter")
    EFakeBloomUI_BuildPhase BuildPhase;

    UPROPERTY(BlueprintReadOnly, Category = "Builder")
    TObjectPtr<UWidget> TargetWidget;

public:
    FFakeBloomUI_BaseParameter BaseParameter;

    UFUNCTION(BlueprintPure, Category = "Parameter")
    bool IsUsingValidTexture() const
    {
        return BaseParameter.bUseTexture && BaseParameter.BloomTexture;
    }

    UFUNCTION(BlueprintPure, Category = "Parameter")
    const FFakeBloomUI_BaseParameter& GetBaseParameter() const
    {
        return BaseParameter;
    }

public:
    UFakeBloomUI_Builder();
    
public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnRequestRedraw();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnPaintPreProcess(const FFakeBloomUI_PreProcessArgs& PreProcessArgs);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Builder")
    bool DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                            class UWidget* WidgetToRender,
                            const FFakeBloomUI_PreProcessArgs& PreProcessArgs,
                            int32 OverhangX,
                            int32 OverhangY,
                            bool UseGamma,
                            bool UpdateImmediate) const;

    UFUNCTION(BlueprintPure, Category = "Builder")
    static int32 GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target);

    UFUNCTION(BlueprintPure, Category = "Builder")
    bool IsDesignTime() const;

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBuild, UTextureRenderTarget2D*, ResultRT);
    UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Builder")
    FOnFinishBuild OnFinishBuild;

protected:
    virtual class UWorld* GetWorld() const;
};
