// Copyright seiko_dev. All Rights Reserved.

#include "BFL_UI_Blooman.h"
#include "Engine/TextureRenderTarget2D.h"


int32 UBFL_UI_Blooman::GetRenderTargetMipMapNum(UTextureRenderTarget2D* Target)
{
    if (Target) {
        return Target->GetNumMips();
    }
    return -1;
}
