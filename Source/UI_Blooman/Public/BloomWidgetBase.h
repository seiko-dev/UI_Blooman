// Copyright seiko_dev. All Rights Reserved.
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

public:
    UBloomWidgetBase(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void PaintPreProcess(const FGeometry& MyGeometry);

    int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void OpenSaveTextureDialog(const FString& InBasePath, bool& IsSuccess, FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void NotifyCreateTextureFinished();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Create Texture")
    void RequestCreateNewTexture();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Create Texture")
    void RequestOverwriteTexture();

    bool HasShowOutlineFlag();

#if WITH_EDITOR
    void RequestTextureCreateCommand(uint8 Cmd);
#endif


public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="BloomDraw")
    UTexture2D* BloomTexture;

#if WITH_EDITOR
    DECLARE_DELEGATE(FCreateTextureCallBackForEditor);
    FCreateTextureCallBackForEditor CreateTextureCallBackForEditor;

    TSharedPtr<IPropertyHandle> TexPropHandle;
#endif
};
