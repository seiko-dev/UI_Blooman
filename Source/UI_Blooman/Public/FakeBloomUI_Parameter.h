// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TextureDefines.h"
#include "FakeBloomUI_Parameter.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class UI_BLOOMAN_API UFakeBloomUI_CommonParameter : public UObject
{
    GENERATED_BODY()
public:
    // Amount of bloom to draw outside the Widget.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Parameter", meta = (ClampMin = "0", ClampMax = "2048", UIMin = "0", UIMax = "256"))
        int32 Overhang;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter")
        bool bUseTexture;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Parameter", meta = (EditCondition = "bUseTexture"))
        TObjectPtr<UTexture2D> BloomTexture;

public:
    UFUNCTION(BlueprintPure, Category = "Common")
    bool UseValidTexture() const
    {
        return bUseTexture && BloomTexture;
    }
};
