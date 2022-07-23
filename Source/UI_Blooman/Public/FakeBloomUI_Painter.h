// Copyright seiko_dev. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "FakeBloomUI_Painter.generated.h"

// Paint the Image Object specified by the parameter.
// PainterがUIに塗り、WriterがStatic Textureに書き出す。
UCLASS(Abstract, Blueprintable)
class UI_BLOOMAN_API UFakeBloomUI_Painter : public UObject
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void OnRebuild();

    void SetParameters(const FFakeBloomUI_BuildParameter* InBuildParameter, const FFakeBloomUI_PaintParameter* InPaintParameter);

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void SetRenderTexture(UTextureRenderTarget2D* InRenderTexture);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Painter")
    void OnPaint(UPARAM(ref) FPaintContext& Context) const;

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Painter")
    void DrawImageToCenter(UPARAM(ref) FPaintContext& Context,
                           UObject* Image,
                           int32 Overhang,
                           const FVector2D& Scale,
                           const FLinearColor& TintColor ) const;

    UFUNCTION(BlueprintCallable, Category = "Painter")
    const FFakeBloomUI_BuildParameter& GetBuildParameter() const;

    UFUNCTION(BlueprintCallable, Category = "Painter")
    const FFakeBloomUI_PaintParameter& GetPaintParameter() const;

protected:
    virtual class UWorld* GetWorld() const;

    TObjectPtr<const FFakeBloomUI_BuildParameter> BuildParameter; // Overhangが欲しい
    TObjectPtr<const FFakeBloomUI_PaintParameter> PaintParameter;
};
