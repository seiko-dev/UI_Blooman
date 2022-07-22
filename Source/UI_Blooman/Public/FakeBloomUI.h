// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ContentWidget.h"
#include "FakeBloomUI_Parameter.h"
#include "FakeBloomUI.generated.h"

class UFakeBloomUI_Builder;
class UFakeBloomUI_Painter;

/**
 * 
 */
UCLASS(DisplayName = "Fake Bloom")
class UI_BLOOMAN_API UFakeBloomUI : public UContentWidget
{
    GENERATED_BODY()
public:
    UFakeBloomUI(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "Fake Bloom")
    UWidget* GetChildContent() const;

    UFUNCTION(BlueprintCallable, Category = "Fake Bloom")
    bool IsDesignTime() const;

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Fake Bloom")
    FFakeBloomUI_BuildParameter BuildParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Fake Bloom")
    FFakeBloomUI_PaintParameter PaintParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Fake Bloom")
    FFakeBloomUI_WriteParameter WriteParameter;

public:
    //-------------------------------------------------
    UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear, AdvancedDisplay, Category = "Fake Bloom")
    TSubclassOf<UFakeBloomUI_Builder> BuilderClass;

    UPROPERTY(BlueprintReadOnly, Category = "Fake Bloom")
    TObjectPtr<UFakeBloomUI_Builder> Builder;

    UFakeBloomUI_Builder* GetBuilder(bool ForceRebuild = false);

    //-------------------------------------------------
    UPROPERTY(BlueprintReadWrite, EditAnywhere, NoClear, AdvancedDisplay, Category = "Fake Bloom")
    TSubclassOf<UFakeBloomUI_Painter> PainterClass;

    UPROPERTY(BlueprintReadOnly, Category = "Fake Bloom")
    TObjectPtr<UFakeBloomUI_Painter> Painter;

    UFakeBloomUI_Painter* GetPainter(bool ForceRebuild = false);

#if WITH_EDITOR
public:
    // UObject interface
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    // UWidget interface
    virtual const FText GetPaletteCategory() override;

public:
    // 本ClassにEditorSubSystemを意識させない為のDelegate
    DECLARE_DELEGATE_OneParam(FCheckEditorCommand, UFakeBloomUI*);
    FCheckEditorCommand CheckEditorCommand;

    DECLARE_DELEGATE(FFinishEditorCommand);
    FFinishEditorCommand FinishEditorCommand;

    UFUNCTION() // AddDynamicする為に修飾が必要
    void CreateNewTexture(UTextureRenderTarget2D* InRenderTarget);

    UFUNCTION() // AddDynamicする為に修飾が必要
    void OverwriteTexture(UTextureRenderTarget2D* InRenderTarget);

    void OnFinishWriteJob();

    FString TextureSavePath;
#endif

protected:
    // UWidget interface 
    virtual TSharedRef<SWidget> RebuildWidget() override;

    // UPanelWidget interface
    virtual void ReleaseSlateResources(bool bReleaseChildren) override;
    virtual void OnSlotAdded(UPanelSlot* InSlot) override;
    virtual void OnSlotRemoved(UPanelSlot* InSlot) override;

protected:
    TSharedPtr<class SFakeBloomUI> MyFakeBloomUI;
};
