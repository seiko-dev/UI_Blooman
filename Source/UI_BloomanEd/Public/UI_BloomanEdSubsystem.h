// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "UI_BloomanEdSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UI_BLOOMANED_API UUI_BloomanEdSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
public:
    // Editor用テクスチャ生成指示の種類
    enum class ETexCreateCmd : uint8
    {
        None = 0,
        CreateNew,
        Overwrite,
    };

    virtual void Initialize(FSubsystemCollectionBase& Collection);

    void Reset();

    void SetSavePath(const FString& Path) {
        SavePath = Path;
    }

    const FString& GetSavePath() const {
        return SavePath;
    }

    void SetTextureCreateCommand(ETexCreateCmd Cmd) {
        TextureCreateCommand = Cmd;
    }

    ETexCreateCmd GetTextureCreateCommand() const {
        return TextureCreateCommand;
    }

    void SetRequestRestoreShowOutline(bool Active) {
        bNeedRestoreShowOutline = Active;
    }

    bool GetRequestRestoreShowOutline() const {
        return bNeedRestoreShowOutline;
    }


private:
    ETexCreateCmd TextureCreateCommand;
    bool bNeedRestoreShowOutline;
    FString SavePath;
};
