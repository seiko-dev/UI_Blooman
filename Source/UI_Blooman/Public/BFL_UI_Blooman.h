// Fill out your copyright notice in the Description page of Project Settings.

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

    UFUNCTION(Category = "UMG", BlueprintCallable)
    static void DrawSlateBrush(UPARAM(ref) FPaintContext& Context, 
                               FVector2D Position, 
                               FVector2D Size,
                               const FSlateBrush& Brush, 
                               const FLinearColor& InTint);

    UFUNCTION(Category = "UMG", BlueprintPure)
    static int32 GetNumMipMap(UTextureRenderTarget2D* Target);
};
