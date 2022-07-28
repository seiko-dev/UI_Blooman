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

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    void ResetTextureSaveParam();

    void SetSavePath(const FString& Path) {
        TextureSavePath = Path;
    }

    const FString& GetTextureSavePath() const {
        return TextureSavePath;
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

    void ExecuteReservedCommand(class UFakeBloomUI* Widget);

private:
    ETexCreateCmd TextureCreateCommand;
    bool bNeedRestoreShowOutline;
    FString TextureSavePath;
};
