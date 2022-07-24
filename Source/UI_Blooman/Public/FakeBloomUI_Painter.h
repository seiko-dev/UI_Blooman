// Copyright seiko_dev. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "FakeBloomUI_Painter.generated.h"

// Paint the Image Object specified by the parameter.
// PainterがUIに塗り、WriterがStatic Textureに書き出す。
UCLASS(Abstract, Blueprintable, EditInlineNew)
class UI_BLOOMAN_API UFakeBloomUI_Painter : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "Parameter")
         TObjectPtr<const UFakeBloomUI_CommonParameter> CommonParameter;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Parameter")
        FLinearColor TintColor;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Interp, Category = "Parameter")
        FVector2D SizeScale;

public:
    UFakeBloomUI_Painter();

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, Category = "Painter")
    void SetRenderTexture(UTextureRenderTarget2D* InRenderTexture);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Painter")
    void OnPaint(UPARAM(ref) FPaintContext& Context) const;

    UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Painter")
    static void DrawImageToCenter(UPARAM(ref) FPaintContext& Context,
                                  UObject* Image,
                                  int32 Overhang,
                                  const FVector2D& InSizeScale,
                                  const FLinearColor& InTintColor);


protected:
    virtual class UWorld* GetWorld() const;
};
