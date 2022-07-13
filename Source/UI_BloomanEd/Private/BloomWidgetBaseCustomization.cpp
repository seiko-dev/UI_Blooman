// Copyright seiko_dev. All Rights Reserved.

#include "BloomWidgetBaseCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "BloomWidgetBase.h"
#include "Widgets/Docking/SDockTab.h"
#include "UI_BloomanEdSubsystem.h"

FBloomWidgetBaseCustomization::FBloomWidgetBaseCustomization()
    : RestoreOutlineCounter(0)
{

}

TSharedRef<IDetailCustomization> FBloomWidgetBaseCustomization::MakeInstance()
{
    return MakeShareable(new FBloomWidgetBaseCustomization());
}

void FBloomWidgetBaseCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    // UI構築
    {
        IDetailCategoryBuilder& SettingsCategory = DetailLayout.EditCategory("BloomSettings", 
                                                                             FText::GetEmpty(),
                                                                             ECategoryPriority::TypeSpecific);

        TSharedRef<SHorizontalBox> ButtonBox = SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Create New Texture")))
               .OnClicked(this, &FBloomWidgetBaseCustomization::OnCreateNewTextureClicked)
            ]
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Overwrite Texture")))
                .OnClicked(this, &FBloomWidgetBaseCustomization::OnOverwriteTextureClicked)
            ];

        FDetailWidgetRow& ButtonRow = SettingsCategory.AddCustomRow(FText::GetEmpty());
        ButtonRow.WholeRowContent()
            [
                ButtonBox
            ];
        
        TSharedRef<FGlobalTabmanager> GlobalTabManager = FGlobalTabmanager::Get();
        TSharedPtr<SDockTab> ActiveTab = GlobalTabManager->GetActiveTab();
        if (ActiveTab.IsValid()){
            TSharedPtr<SWidget> AssetEditor = SearchNearestParentAssetEditor(ActiveTab);
            DesinerView = SearchNearestChildDesignerView(AssetEditor);
        }

        IDetailCategoryBuilder& DrawCategory = DetailLayout.EditCategory("BloomDraw",
                                                                             FText::GetEmpty(),
                                                                             ECategoryPriority::TypeSpecific);
    }

    TexHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UBloomWidgetBase, BloomTexture));
    SelectedObjects = DetailLayout.GetSelectedObjects();

    // 遅延されていた命令があるなら、ここで実行
    TriggerCreateTextureCommand();
}

bool FBloomWidgetBaseCustomization::HideOutline()
{
    // まず現状Outline表示状態なのか確認
    bool IsShowOutline(false);
    for (auto& Obj : SelectedObjects) {
        if (UBloomWidgetBase* Widget = Cast<UBloomWidgetBase>(Obj.Get())) {
            if(Widget->HasShowOutlineFlag()){
                IsShowOutline = true;
                break;
            }
        }
    }
    if (!IsShowOutline) {
        return false;
    }

    // 後で元に戻すフラグ
    if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
        SubSys->SetRequestRestoreShowOutline(true);
    }

    ToggleOutline();

    return true;
}

void FBloomWidgetBaseCustomization::ToggleOutline()
{
    if (!DesinerView.IsValid()) {
        return;
    }

    FInputChord Chord(EKeys::G);
    const uint32* CharacterCodePtr;
    const uint32* KeyCodePtr;
    FInputKeyManager::Get().GetCodesFromKey(Chord.Key, CharacterCodePtr, KeyCodePtr);
    uint32 CharacterCode = (CharacterCodePtr != nullptr ? *CharacterCodePtr : 0);
    uint32 KeyCode = (KeyCodePtr != nullptr ? *KeyCodePtr : 0);

    FModifierKeysState ModifierKeys(
        Chord.bShift, Chord.bShift,
        Chord.bCtrl, Chord.bCtrl,
        Chord.bAlt, Chord.bAlt,
        Chord.bCmd, Chord.bCmd,
        false
    );

    FKeyEvent KeyEvent(Chord.Key, ModifierKeys, FSlateApplication::Get().GetUserIndexForKeyboard(), false, CharacterCode, KeyCode);

    // これにFocusしないとショトカキーが反応しない
    FSlateApplication::Get().SetAllUserFocus(DesinerView);

    // 成功すると、Widget本体やDetails欄含めが作り直しになる
    FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

void FBloomWidgetBaseCustomization::RestoreOutline()
{
    --RestoreOutlineCounter;

    if (RestoreOutlineCounter <= 0 ) {
        ToggleOutline();
        RestoreOutlineCounter = 0;
    }
}

void FBloomWidgetBaseCustomization::TriggerCreateTextureCommand()
{
    if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
        const auto Cmd = SubSys->GetTextureCreateCommand();

        if (Cmd == UUI_BloomanEdSubsystem::ETexCreateCmd::None) {
            return;
        }

        for (auto& Obj : SelectedObjects) {
            if (!Obj.IsValid()) continue;

            if (UBloomWidgetBase* Widget = Cast<UBloomWidgetBase>(Obj.Get())) {
                Widget->TexPropHandle = TexHandle;
                Widget->RequestTextureCreateCommand((uint8)Cmd);
                UE_LOG(LogTemp, Log, TEXT("%s: requested."), UTF8_TO_TCHAR(__func__));

                // 自動Outline消しが発動していたら、全員の作業後に元に戻す予約
                if (SubSys->GetRequestRestoreShowOutline()) {
                    ++RestoreOutlineCounter;
                    Widget->CreateTextureCallBackForEditor.BindRaw(this, &FBloomWidgetBaseCustomization::RestoreOutline);
                }
            }
        }

        // 各Widgetに指示出しは終わったのでSubsystemへの予約は解除
        SubSys->SetTextureCreateCommand(UUI_BloomanEdSubsystem::ETexCreateCmd::None);
        SubSys->SetRequestRestoreShowOutline(false);
    }
}

// Slate階層を上りながらAssetEditorを探す
TSharedPtr<SWidget> FBloomWidgetBaseCustomization::SearchNearestParentAssetEditor(TSharedPtr<SWidget> CurrentWidget)
{
    if (!CurrentWidget.IsValid()) {
        return nullptr;
    }

    TSharedPtr<SWidget> Parent = CurrentWidget->GetParentWidget();

    if (!Parent.IsValid()) {
        return nullptr;
    }

    // ClassがPrivateなので文字列直打ち
    const FName ClassName(Parent->GetType());
    if (ClassName == FName("SStandaloneAssetEditorToolkitHost")) {
        return Parent;
    }

    return SearchNearestParentAssetEditor(Parent);
}

// Slate階層を下りながらDesignerViewを探す
TSharedPtr<SWidget> FBloomWidgetBaseCustomization::SearchNearestChildDesignerView(TSharedPtr<SWidget> CurrentWidget)
{
    if (!CurrentWidget.IsValid()) {
        return nullptr;
    }

    FChildren* Children = CurrentWidget->GetChildren();
    if (Children == nullptr) {
        return nullptr;
    }

    for (int32 Index = 0; Index < Children->Num(); Index++){
        TSharedPtr<SWidget> ChildWidget = Children->GetChildAt(Index);

        if (!ChildWidget.IsValid()) {
            return nullptr;
        }

        // ClassがPrivateなので文字列直打ち
        const FName ClassName(ChildWidget->GetType());
        if (ClassName == FName("SDesignerView")) {
            return ChildWidget;
        }

        TSharedPtr<SWidget> GrandChildWidget = SearchNearestChildDesignerView(ChildWidget);
        if(GrandChildWidget.IsValid()) {
            return GrandChildWidget;
        }
    }

    return nullptr;
}

FReply FBloomWidgetBaseCustomization::OnCreateNewTextureClicked()
{
    // アウトライン非表示にするためにWidgetが再構築されるので、
    // Subsystemに予約して命令を遅延する
    if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
        SubSys->SetTextureCreateCommand(UUI_BloomanEdSubsystem::ETexCreateCmd::CreateNew);
    }

    if (!HideOutline()) {
        // 既にOutline非表示であれば、すぐに命令を実行できる
        TriggerCreateTextureCommand();
    }

    return FReply::Handled();
}

FReply FBloomWidgetBaseCustomization::OnOverwriteTextureClicked()
{
    if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
        SubSys->SetTextureCreateCommand(UUI_BloomanEdSubsystem::ETexCreateCmd::Overwrite);
    }

    if (!HideOutline()) {
        TriggerCreateTextureCommand();
    }

    return FReply::Handled();
}
