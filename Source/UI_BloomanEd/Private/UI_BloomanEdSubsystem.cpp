// Copyright seiko_dev. All Rights Reserved.

#include "UI_BloomanEdSubsystem.h"
#include "FakeBloomUI.h"
#include "FakeBloomUI_Builder.h"

void UUI_BloomanEdSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    ResetTextureSaveParam();
}

void UUI_BloomanEdSubsystem::ResetTextureSaveParam()
{
    TextureCreateCommand = ETexCreateCmd::None;
    bNeedRestoreShowOutline = false;
    TextureSavePath.Empty();
}

void UUI_BloomanEdSubsystem::ExecuteReservedCommand(UFakeBloomUI* Widget)
{
    if (!(Widget && Widget->Builder)) {
        // ここで生成できてないとおかしい
        ensure(0);
        return;
    }

    switch (TextureCreateCommand) {
    case ETexCreateCmd::CreateNew:
        Widget->TextureSavePath = TextureSavePath;
        Widget->Builder->OnFinishBuild.AddDynamic(Widget, &UFakeBloomUI::CreateNewTexture);
        break;

    case ETexCreateCmd::Overwrite:
        Widget->Builder->OnFinishBuild.AddDynamic(Widget, &UFakeBloomUI::OverwriteTexture);
        break;

    default:
        break;
    }

    // もう一度Render Targetを描画(Outline非表示だった場合のために明示的にリクエストが必要)
    Widget->Builder->OnRequestRedraw();
}
