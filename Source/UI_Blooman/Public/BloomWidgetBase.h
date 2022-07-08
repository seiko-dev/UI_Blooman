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
    // Editor用テクスチャ生成指示の種類
    enum class ETexCreateCmd : uint8
    {
        None,
        CreateNew,
        Overwrite,
    };

public:
    UBloomWidgetBase(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface | Painting")
    void PaintPreProcess(const FGeometry& MyGeometry);

    int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void OpenSaveTextureDialog(const FString& InBasePath, bool& IsSuccess, FString& Path);

    UFUNCTION(BlueprintCallable, Category = "Create Texture")
    void NotifyCreateTextureFinished();

    void RequestTextureCreateCommand(ETexCreateCmd Cmd);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Create Texture")
    void RequestCreateNewTexture();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Create Texture")
    void RequestOverwriteTexture();

    bool HasShowOutlineFlag();

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="BloomDraw")
    UTexture2D* BloomTexture;

#if WITH_EDITOR
    DECLARE_DELEGATE(FCreateTextureCallBackForEditor);
    FCreateTextureCallBackForEditor CreateTextureCallBackForEditor;

    TSharedPtr<IPropertyHandle> TexPropHandle;
#endif
};
