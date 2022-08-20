// Copyright seiko_dev. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "FakeBloomUI_Parameter.h"
#include "FakeBloomUI_Painter.generated.h"

// Paint the Image Object specified by the parameter.
// PainterがUIに塗り、WriterがStatic Textureに書き出す。
UCLASS(Abstract, Blueprintable, EditInlineNew)
class UI_BLOOMAN_API UFakeBloomUI_Painter : public UObject
{
    GENERATED_BODY()
public:
    UFakeBloomUI_Painter();

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void OnPaintPreProcess();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Painter")
    void OnPaint(UPARAM(ref) FPaintContext& Context) const;

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Painter")
    static void DrawImageToCenter(UPARAM(ref) FPaintContext& Context,
                                  const FFakeBloomUI_OverhangAmount& Overhang,
                                  const FVector2D& InSizeScale,
                                  const FSlateBrush& Brush);

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void SetRenderTexture(UTextureRenderTarget2D* InRenderTexture);

    UFUNCTION(BlueprintPure, Category = "Painter")
    float GetOpacity() const { return Opacity; }

public:
    void SetOpacity(float InOpacity);

public:
    TObjectPtr<UFakeBloomUI> FakeBloomUI;

    UFUNCTION(BlueprintPure, Category = "Parameter")
    bool IsUsingValidTexture() const;

    UFUNCTION(BlueprintPure, Category = "Parameter")
    const FFakeBloomUI_BaseParameter& GetBaseParameter() const;

protected:
    virtual class UWorld* GetWorld() const;

private:
    float Opacity;
};
