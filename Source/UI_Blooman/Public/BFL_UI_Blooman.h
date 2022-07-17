// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "BFL_UI_Blooman.generated.h"

/**
 * 
 */
UCLASS()
class UI_BLOOMAN_API UBFL_UI_Blooman : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    UFUNCTION(Category = "UMG", BlueprintCallable)
    static bool DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                                   class UWidget* WidgetToRender,
                                   FVector2D DrawSize,
                                   FVector2D DrawOffset,
                                   bool UseGamma,
                                   bool UpdateImmediate,
                                   int32& NumMips);


    UFUNCTION(Category = "UMG", BlueprintPure)
    static int32 GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target);
};
