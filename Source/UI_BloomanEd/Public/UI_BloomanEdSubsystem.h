// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "BloomWidgetBase.h"
#include "UI_BloomanEdSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UI_BLOOMANED_API UUI_BloomanEdSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
    virtual void Initialize(FSubsystemCollectionBase& Collection);

    void SetTextureCreateCommand(UBloomWidgetBase::ETexCreateCmd Cmd) {
        TextureCreateCommand = Cmd;
    }

    UBloomWidgetBase::ETexCreateCmd GetTextureCreateCommand() const {
        return TextureCreateCommand;
    }

    void SetRequestRestoreShowOutline(bool Active) {
        bNeedRestoreShowOutline = Active;
    }

    bool GetRequestRestoreShowOutline() const {
        return bNeedRestoreShowOutline;
    }


private:
    UBloomWidgetBase::ETexCreateCmd TextureCreateCommand;
    bool bNeedRestoreShowOutline;
};
