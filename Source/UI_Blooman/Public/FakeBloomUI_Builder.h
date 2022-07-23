// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FakeBloomUI_Builder.generated.h"


USTRUCT(Blueprintable)
struct UI_BLOOMAN_API FFakeBloomUI_PreProcessArgs
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "PreProcessArgs")
    FGeometry Geometry;

    FSlateRect CullingRect;

    FFakeBloomUI_PreProcessArgs()
    {}

    FFakeBloomUI_PreProcessArgs(const FGeometry& InGeometry, const FSlateRect& InCullingRect)
        : Geometry(InGeometry)
        , CullingRect(InCullingRect)
    {}
};


// 
UCLASS(Abstract, Blueprintable)
class UI_BLOOMAN_API UFakeBloomUI_Builder : public UObject
{
    GENERATED_BODY()
public:
    UFakeBloomUI_Builder();

    void SetParameter(FFakeBloomUI_BuildParameter* Param);
    
    UPROPERTY(BlueprintReadOnly, Category = "Builder")
    TObjectPtr<UWidget> TargetWidget;

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnRebuild();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnRequestRedraw();

    UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "Builder")
    void OnPaintPreProcess(const FFakeBloomUI_PreProcessArgs& PreProcessArgs);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Builder")
    bool DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                            class UWidget* WidgetToRender,
                            const FFakeBloomUI_PreProcessArgs& PreProcessArgs,
                            int32 Overhang,
                            bool UseGamma,
                            bool UpdateImmediate) const;

    UFUNCTION(BlueprintCallable, Category = "Builder")
    const FFakeBloomUI_BuildParameter& GetBuildParameter() const;

    UFUNCTION(BlueprintPure, Category = "Builder")
    static int32 GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target);

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBuild, UTextureRenderTarget2D*, ResultRT);
    UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "Builder")
    FOnFinishBuild OnFinishBuild;

protected:
    virtual class UWorld* GetWorld() const;

protected:
    TObjectPtr<FFakeBloomUI_BuildParameter> BuildParameter;
};
