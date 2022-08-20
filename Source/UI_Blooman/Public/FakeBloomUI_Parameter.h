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

USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_OverhangAmount
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = 0, ClampMax = 2048, UIMin = 0, UIMax = 256))
    int32 Left;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = 0, ClampMax = 2048, UIMin = 0, UIMax = 256))
    int32 Top;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = 0, ClampMax = 2048, UIMin = 0, UIMax = 256))
    int32 Right;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (ClampMin = 0, ClampMax = 2048, UIMin = 0, UIMax = 256))
    int32 Bottom;

public:
    FFakeBloomUI_OverhangAmount()
        : Left(0), Top(0), Right(0), Bottom(0)
    {}

    FFakeBloomUI_OverhangAmount(int32 Value)
        : Left(Value), Top(Value), Right(Value), Bottom(Value)
    {}

    int32 GetSizeX() const
    {
        return Left + Right;
    }
    int32 GetSizeY() const
    {
        return Top + Bottom;
    }
};

// UObject化する等して分割したいが、Sequencer対応の都合で今はStruct
USTRUCT(BlueprintType)
struct UI_BLOOMAN_API FFakeBloomUI_BaseParameter
{
    GENERATED_BODY()
public:
    // Amount of bloom to draw outside the Widget.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter")
    FFakeBloomUI_OverhangAmount Overhang;

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
        : Overhang(0)
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
