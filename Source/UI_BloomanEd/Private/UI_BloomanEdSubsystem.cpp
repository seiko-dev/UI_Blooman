// Copyright seiko_dev. All Rights Reserved.
#include "UI_BloomanEdSubsystem.h"

void UUI_BloomanEdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    TextureCreateCommand = UBloomWidgetBase::ETexCreateCmd::None;
    bNeedRestoreShowOutline = false;
}
