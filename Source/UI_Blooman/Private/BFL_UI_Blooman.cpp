// Copyright seiko_dev. All Rights Reserved.

#include "BFL_UI_Blooman.h"
#include "Slate/WidgetRenderer.h"
#include "Components/Widget.h"
#include "Engine/TextureRenderTarget2D.h"

bool UBFL_UI_Blooman::DrawWidgetToTarget(UTextureRenderTarget2D* Target,
                                         UWidget* WidgetToRender,
                                         FVector2D DrawSize,
                                         FVector2D DrawOffset,
                                         bool UseGamma,
                                         bool UpdateImmediate,
                                         int32& NumMips)
{
    if (!WidgetToRender)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : WidgetToRender is empty!"));
        return false;
    }
    if (DrawSize.X < 0 || DrawSize.Y < 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : DrawSize is 0 or less!"));
        return false;
    }
    if (!Target)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawWidgetToTarget Fail : Target is empty!"));
        return false;
    }

    FWidgetRenderer* WidgetRenderer = new FWidgetRenderer(UseGamma, false);
    check(WidgetRenderer);

    {
        WidgetRenderer->ViewOffset = DrawOffset;
        WidgetRenderer->DrawWidget(Target, WidgetToRender->TakeWidget(), DrawSize, 0.0);
        FlushRenderingCommands();

        BeginCleanup(WidgetRenderer);

        NumMips = 0;

        if (UpdateImmediate) {
            Target->UpdateResourceImmediate(false);
            NumMips = Target->GetNumMips();
        }
    }

    return true;
}

int32 UBFL_UI_Blooman::GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target)
{
    if (Target) {
        return Target->GetNumMips();
    }
    return -1;
}
