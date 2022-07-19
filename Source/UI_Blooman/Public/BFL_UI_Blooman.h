// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BFL_UI_Blooman.generated.h"

/**
 * 
 */
UCLASS()
class UI_BLOOMAN_API UBFL_UI_Blooman : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(Category = "UMG", BlueprintPure)
    static int32 GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target);
};
