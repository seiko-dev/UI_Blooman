// Copyright seiko_dev. All Rights Reserved.
#include "UI_BloomanEdSubsystem.h"

void UUI_BloomanEdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Reset();
}

void UUI_BloomanEdSubsystem::Reset()
{
    TextureCreateCommand = ETexCreateCmd::None;
    bNeedRestoreShowOutline = false;
    SavePath.Empty();
}
