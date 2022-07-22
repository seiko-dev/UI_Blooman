// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomUI_Painter.h"
#include "FakeBloomUI_Parameter.h"

void UFakeBloomUI_Painter::SetParameters(const FFakeBloomUI_BuildParameter* InBuildParameter,
                                         const FFakeBloomUI_PaintParameter* InPaintParameter)
{
    BuildParameter = InBuildParameter;
    PaintParameter = InPaintParameter;
}

void UFakeBloomUI_Painter::DrawImageToCenter(FPaintContext& Context,
                                             UObject* Image,
                                             float Overhang,
                                             const FVector2D& Scale,
                                             const FLinearColor& TintColor) const
{
    FVector2D Size = (Context.AllottedGeometry.GetLocalSize() + 2*Overhang)*Scale;
    FVector2D Position = (Context.AllottedGeometry.GetLocalSize() - Size) * 0.5f;
    FSlateBrush Brush;
    Brush.SetResourceObject(Image);

    FSlateDrawElement::MakeBox(
        Context.OutDrawElements,
        Context.MaxLayer,
        Context.AllottedGeometry.ToPaintGeometry(Position, Size),
        &Brush,
        ESlateDrawEffect::None,
        TintColor);

    Context.MaxLayer++;
}

class UWorld* UFakeBloomUI_Painter::GetWorld() const
{
    if (!HasAnyFlags(RF_ClassDefaultObject) // CDOでは無効
        && ensureMsgf(GetOuter(), TEXT("%s has a null OuterPrivate in GetWorld()"), *GetFullName()) // Outerあるよね？
        && !GetOuter()->HasAnyFlags(RF_BeginDestroyed)  // Outer死にかけてたら無効
        && !GetOuter()->IsUnreachable()) // Outerない事になってたら無効
    {
        return GetOuter()->GetWorld();
    }
    return nullptr;
}

const FFakeBloomUI_BuildParameter& UFakeBloomUI_Painter::GetBuildParameter() const
{
    if (BuildParameter) {
        return *BuildParameter;
    }

    ensure(0);
    static FFakeBloomUI_BuildParameter Dummy;
    return Dummy;
}

const FFakeBloomUI_PaintParameter& UFakeBloomUI_Painter::GetPaintParameter() const
{
    if (PaintParameter) {
        return *PaintParameter;
    }

    ensure(0);
    static FFakeBloomUI_PaintParameter Dummy;
    return Dummy;
}
