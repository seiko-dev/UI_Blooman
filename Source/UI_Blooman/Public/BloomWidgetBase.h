// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BloomWidgetBase.generated.h"

/**
 * 疑似発光Widgetの基底
 */
UCLASS(Abstract)
class UI_BLOOMAN_API UBloomWidgetBase : public UUserWidget
{
	GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void PaintPreProcess(const FGeometry& MyGeometry);

    int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void OpenSaveTextureDialog(const FString& InBasePath, bool& IsSuccess, FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void NotifyTexPropertyChanged();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
    void RequestCreateNewTexture();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic)
    void RequestOverwriteTexture();

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="Settings")
    UTexture2D* BloomTexture;

#if WITH_EDITOR
    TSharedPtr<IPropertyHandle> TexPropHandle;
#endif
};