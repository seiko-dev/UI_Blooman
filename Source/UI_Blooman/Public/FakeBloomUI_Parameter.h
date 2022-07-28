// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureDefines.h"
#include "FakeBloomUI_Parameter.generated.h"

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_PreProcessArgs
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "PreProcessArgs")
    FGeometry Geometry;

    FSlateRect CullingRect;

    FFakeBloomUI_PreProcessArgs()
    {}

    FFakeBloomUI_PreProcessArgs(const FGeometry& InGeometry, const FSlateRect& InCullingRect)
        : Geometry(InGeometry)
        , CullingRect(InCullingRect)
    {}
};

// UObject化する等して分割したいが、Sequencer対応の都合で今はStruct
USTRUCT(BlueprintType)
struct UI_BLOOMAN_API FFakeBloomUI_BaseParameter
{
    GENERATED_BODY()
public:
    // Amount of bloom to draw outside the Widget.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0", ClampMax = "2048", UIMin = "0", UIMax = "256"))
    int32 OverhangX;

    // Amount of bloom to draw outside the Widget.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (ClampMin = "0", ClampMax = "2048", UIMin = "0", UIMax = "256"))
    int32 OverhangY;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Common")
    bool bUseTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Common", meta = (EditCondition = "bUseTexture"))
    TObjectPtr<UTexture2D> BloomTexture;

    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Paint")
    FSlateBrush Brush;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint")
    FLinearColor TintColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Paint")
    FVector2D SizeScale;

    FFakeBloomUI_BaseParameter()
        : OverhangX(0)
        , OverhangY(0)
        , bUseTexture(false)
        , BloomTexture(nullptr)
        , Brush()
        , TintColor(FLinearColor::White)
        , SizeScale(1.0f, 1.0f)
    {}

    bool IsUsingValidTexture() const
    {
        return bUseTexture && BloomTexture;
    }
};
