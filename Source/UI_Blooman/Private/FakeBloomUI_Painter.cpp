// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomUI_Painter.h"
#include "FakeBloomUI_Parameter.h"

UFakeBloomUI_Painter::UFakeBloomUI_Painter()
    : CommonParameter(nullptr)
    , TintColor(FLinearColor::White)
    , SizeScale(1.0f, 1.0f)
{
}

void UFakeBloomUI_Painter::DrawImageToCenter(FPaintContext& Context,
                                             UObject* Image,
                                             int32 Overhang,
                                             const FVector2D& InSizeScale,
                                             const FLinearColor& InTintColor)
{
    FVector2D Size = (Context.AllottedGeometry.GetLocalSize() + 2*Overhang)* InSizeScale;
    FVector2D Position = (Context.AllottedGeometry.GetLocalSize() - Size) * 0.5f;
    FSlateBrush Brush;
    Brush.SetResourceObject(Image);

    FSlateDrawElement::MakeBox(
        Context.OutDrawElements,
        Context.MaxLayer,
        Context.AllottedGeometry.ToPaintGeometry(Position, Size),
        &Brush,
        ESlateDrawEffect::None,
        InTintColor);

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
