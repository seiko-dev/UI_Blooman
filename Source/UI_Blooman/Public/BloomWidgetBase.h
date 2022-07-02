// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloomWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class UI_BLOOMAN_API UBloomWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void PaintPreProcess(const FGeometry& MyGeometry);

    int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    UFUNCTION(BlueprintCallable, Category = "Bloom")
    void OpenSaveTextureDialog(bool& IsSuccess, FString& Path, FString& Name);
};
