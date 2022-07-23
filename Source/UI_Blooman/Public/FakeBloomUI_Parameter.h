// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureDefines.h"
#include "FakeBloomUI_Parameter.generated.h"

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_BuildParameter
{
    GENERATED_BODY()
public:
    FFakeBloomUI_BuildParameter()
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
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0", ClampMax = "2048", UIMin = "0", UIMax = "256"))
    int32 Overhang;

    // The closer to 1.0, the more only the brightest pixels bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float AlphaToLuminance;

    // Blooming transparency threshold.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
    float LuminanceThreshold;

    // Adjust the strength of the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float Strength;

    // Fine-tune the bloom spread.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float Spead;

    // The larger this is, the higher level MipMap is used, and the wider the bloom.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "1", UIMin = "1"))
    int32 MaxMipLevel;

    // Final Texture Compression Strength
    // 0 is resereved for free texture size (not pad to power of 2) mode.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build", meta = (ClampMin = "0", UIMin = "1"))
    int32 Compression;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Build")
    bool BuildEveryFrame;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_PaintParameter
{
    GENERATED_BODY()
public:
    FFakeBloomUI_PaintParameter()
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

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint")
    bool bUseTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Paint", meta = (EditCondition = "bUseTexture"))
    UTexture2D* BloomTexture;
};

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_WriteParameter
{
    GENERATED_BODY()
public:
    FFakeBloomUI_WriteParameter()
        : Format(TextureCompressionSettings::TC_Default)
    {
    }

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Write")
    TEnumAsByte<enum TextureCompressionSettings> Format;
};
