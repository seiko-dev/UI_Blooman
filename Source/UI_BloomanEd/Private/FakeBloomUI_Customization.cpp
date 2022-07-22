// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "FakeBloom.h"
#include "Widgets/Docking/SDockTab.h"
#include "UI_BloomanEdSubsystem.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "IDetailGroup.h"

#define LOCTEXT_NAMESPACE "UI_BloomanEd"

// 新規テクスチャ生成時の保存先質問用ダイアログ
class SDirectoryDialog : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SDirectoryDialog) {}

    SLATE_END_ARGS()

public:
    SDirectoryDialog() {}
    virtual ~SDirectoryDialog() {}

    virtual void Construct(const FArguments& InArgs, const FString& InBasePath)
    {
        FString basePath(FPackageName::GetLongPackagePath(InBasePath));

        if (basePath.IsEmpty())
        {
            UE_LOG(LogTemp, Warning, TEXT("InBasePath(%s) is not exist, \"/Game/\" is used instead."), *InBasePath)
                basePath = TEXT("/Game/");
        }

        SelectedPath = InBasePath;

        FContentBrowserModule& contentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

        FPathPickerConfig config;
        config.DefaultPath = basePath;
        config.OnPathSelected = FOnPathSelected::CreateRaw(this, &SDirectoryDialog::OnPathSelected);

        AssetName = TEXT("T_Bloom");

        // Texture名入力ボックス
        TSharedRef<SHorizontalBox> nameBox = SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .Padding(0, 0, 10, 0)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Text(LOCTEXT("TextureNameLabel", "Texture Name"))
            ]
        + SHorizontalBox::Slot()
            [
                SNew(SEditableTextBox)
                .Text(FText::FromString(AssetName))
            .OnTextCommitted(this, &SDirectoryDialog::OnNameChange)
            .MinDesiredWidth(250)
            ];

        // OK / Cancelボタンパーツ
        TSharedRef<SHorizontalBox> buttonsBox = SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Bottom)
            .Padding(4.0f, 3.0f)
            [
                SNew(SButton)
                .Text(LOCTEXT("DirectoryPickerConfirmButton", "OK"))
            .ContentPadding(FMargin(8.0f, 2.0f))
            .IsEnabled(this, &SDirectoryDialog::IsConfirmButtonEnabled)
            .OnClicked(this, &SDirectoryDialog::OnConfirmButtonClicked)
            ]
        + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Bottom)
            .Padding(4.0f, 3.0f)
            [
                SNew(SButton)
                .Text(LOCTEXT("DirectoryPickerCancelButton", "Cancel"))
            .ContentPadding(FMargin(8.0f, 2.0f))
            .OnClicked(this, &SDirectoryDialog::OnCancelButtonClicked)
            ];


        // パーツ配置
        TSharedRef<SVerticalBox> mainBox = SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(0.0f, 0.0f, 0.0f, 4.0f)
            [
                contentBrowserModule.Get().CreatePathPicker(config)
            ]
        + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 0.0f, 0.0f, 4.0f)
            [
                nameBox
            ]
        + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Fill)
            .Padding(0.0f)
            [
                buttonsBox
            ];

        ChildSlot
            [
                mainBox
            ];
    }

public: // public methods
    bool HasValidResult() const
    {
        return ConfirmedFlag && (!SelectedPath.IsEmpty());
    }

    FString GetPath() const
    {
        return SelectedPath + TEXT("/") + AssetName;
    }

private: // internal methods: utilities
    void CloseDialog()
    {
        TSharedPtr<SWindow> window = FSlateApplication::Get().FindWidgetWindow(AsShared());
        if (window.IsValid())
        {
            window->RequestDestroyWindow();
        }
    }

private: // internal methods: delegates
    bool IsConfirmButtonEnabled() const { return true; }

    FReply OnConfirmButtonClicked()
    {
        CloseDialog();
        ConfirmedFlag = true;
        return FReply::Handled();
    }

    FReply OnCancelButtonClicked()
    {
        CloseDialog();
        ConfirmedFlag = false;
        return FReply::Handled();
    }

    void OnPathSelected(const FString& InCurrentPath)
    {
        SelectedPath = InCurrentPath;
    }

    void OnNameChange(const FText& NewName, ETextCommit::Type CommitInfo)
    {
        if (!NewName.IsEmpty()) {
            AssetName = NewName.ToString();
        }
    }

private: // internal properties
    FString SelectedPath;
    FString AssetName;
    bool ConfirmedFlag = false;
};


FFakeBloomCustomization::FFakeBloomCustomization()
    : RestoreOutlineCounter(0)
{

}

TSharedRef<IDetailCustomization> FFakeBloomCustomization::MakeInstance()
{
    return MakeShareable(new FFakeBloomCustomization());
}

void FFakeBloomCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    // UI構築
    IDetailCategoryBuilder& Category = DetailLayout.EditCategory(TEXT("Fake Bloom"),
                                                                    FText::GetEmpty(),
                                                                    ECategoryPriority::TypeSpecific);
    // Build
    {
        Category.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, BuildParameter)));

        TSharedRef<SHorizontalBox> ButtonBox = SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Create New Texture")))
            .OnClicked(this, &FFakeBloomCustomization::OnCreateNewTextureClicked)
            ]
        + SHorizontalBox::Slot()
            .AutoWidth()
            .VAlign(VAlign_Center)
            [
                SNew(SButton)
                .Text(FText::FromString(TEXT("Overwrite Texture")))
            .OnClicked(this, &FFakeBloomCustomization::OnOverwriteTextureClicked)
            ];

        FDetailWidgetRow& ButtonRow = Category.AddCustomRow(FText::GetEmpty());
        ButtonRow.WholeRowContent()
            [
                ButtonBox
            ];
    }

    // Paint
    {
        // 「プロパティ活性化チェックボックス + プロパティ実体」のセット。
        // なんかもっと簡単に作れそうだけど…
        Category.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter.bUseTexture))).CustomWidget()
        .NameContent()
        [
            SNew(SHorizontalBox)
            + SHorizontalBox::Slot()
            .AutoWidth()
            [
                DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter.bUseTexture))->CreatePropertyValueWidget()
            ]
            + SHorizontalBox::Slot()
            .Padding(4, 0)
            .AutoWidth()
            [
                DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter.bUseTexture))->CreatePropertyNameWidget()
            ]
        ]
        .ValueContent()
        .VAlign(VAlign_Center)
        [
            DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter.BloomTexture))->CreatePropertyValueWidget()
        ];
        DetailLayout.HideProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter.BloomTexture)));

        // 残りのプロパティを追加
        Category.AddProperty(DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter)));
    }

    // Outline消しショトカ用にDesignerViewを探しておく
    {
        TSharedRef<FGlobalTabmanager> GlobalTabManager = FGlobalTabmanager::Get();
        TSharedPtr<SDockTab> ActiveTab = GlobalTabManager->GetActiveTab();
        if (ActiveTab.IsValid()){
            TSharedPtr<SWidget> AssetEditor = SearchNearestParentAssetEditor(ActiveTab);
            DesignerView = SearchNearestChildDesignerView(AssetEditor);
        }

    }

    // 
    PaintParamHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UFakeBloom, PaintParameter));
    SelectedObjects = DetailLayout.GetSelectedObjects();

    // 遅延されていた命令があるなら、ここで実行
    TriggerCreateTextureCommand();
}

bool FFakeBloomCustomization::HideOutline()
{
    // まず現状Outline表示状態なのか確認
    bool IsShowOutline(false);
    for (auto& Obj : SelectedObjects) {
        if (UFakeBloom* Widget = Cast<UFakeBloom>(Obj.Get())) {
            if(EnumHasAnyFlags(Widget->GetDesignerFlags(), EWidgetDesignFlags::ShowOutline)){
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

void FFakeBloomCustomization::ToggleOutline()
{
    if (!DesignerView.IsValid()) {
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
    FSlateApplication::Get().SetAllUserFocus(DesignerView);

    // 成功すると、Widget本体やDetails欄含めが作り直しになる
    FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
}

void FFakeBloomCustomization::RestoreOutline()
{
    --RestoreOutlineCounter;

    if (RestoreOutlineCounter <= 0 ) {
        ToggleOutline();
        RestoreOutlineCounter = 0;
    }
}

void FFakeBloomCustomization::TriggerCreateTextureCommand()
{
    UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>();
    if (!SubSys) {
        return;
    }
    const auto Cmd = SubSys->GetTextureCreateCommand();

    if (Cmd == UUI_BloomanEdSubsystem::ETexCreateCmd::None) {
        return;
    }

    for (auto& Obj : SelectedObjects) {
        if (!Obj.IsValid()) continue;

        UFakeBloom* Widget = Cast<UFakeBloom>(Obj.Get());
        if (!Widget) continue;

        Widget->PaintParamHandle = PaintParamHandle;

        switch (Cmd) {
        case UUI_BloomanEdSubsystem::ETexCreateCmd::CreateNew:
            if (SelectedObjects.Num()==1) {
                Widget->RequestCreateNewTexture(SubSys->GetSavePath());
            }
            break;

        case UUI_BloomanEdSubsystem::ETexCreateCmd::Overwrite:
            Widget->RequestOverwriteTexture();
            break;

        default:
            check(0);
            return;
        }

        // 自動Outline消しが発動していたら、全員の作業後に元に戻す予約
        if (SubSys->GetRequestRestoreShowOutline()) {
            ++RestoreOutlineCounter;
            Widget->CreateTextureCallBack.BindRaw(this, &FFakeBloomCustomization::RestoreOutline);
        }
    }

    // 各Widgetに指示出しは終わったのでSubSystemへの予約は解除
    SubSys->Reset();
}

// Slate階層を上りながらAssetEditorを探す
TSharedPtr<SWidget> FFakeBloomCustomization::SearchNearestParentAssetEditor(TSharedPtr<SWidget> CurrentWidget)
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
TSharedPtr<SWidget> FFakeBloomCustomization::SearchNearestChildDesignerView(TSharedPtr<SWidget> CurrentWidget)
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

FReply FFakeBloomCustomization::OnCreateNewTextureClicked()
{
    // 保存先確認Dialog
    TSharedRef<SWindow> window = SNew(SWindow)
        .Title(LOCTEXT("BrowseForFolderTitle", "Save Static Texture"))
        .ClientSize(FVector2D(320.0f, 320.0f))
        .SizingRule(ESizingRule::UserSized)
        .SupportsMaximize(false)
        .SupportsMinimize(false);

    TSharedRef<SDirectoryDialog> Dialog = SNew(SDirectoryDialog, TEXT("/Game/"));
    window->SetContent(Dialog);

    do {
        IMainFrameModule& mainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
        const TSharedPtr<SWindow>& parentWindow = mainFrameModule.GetParentWindow();
        if (!parentWindow.IsValid()) {
            UE_LOG(LogTemp, Log, TEXT("%s: Open SaveTexDialog failed"), UTF8_TO_TCHAR(__func__) );
            break;
        }

        FSlateApplication::Get().AddModalWindow(window, parentWindow.ToSharedRef());
        if (!Dialog->HasValidResult()) {
            UE_LOG(LogTemp, Log, TEXT("%s: SaveTexDialog has not valid result.(%s)"), UTF8_TO_TCHAR(__func__), *Dialog->GetPath());
            break;
        }

        // アウトライン非表示にするためにWidgetが再構築される場合がある。
        // いったんSubSystemに命令を予約
        if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
            SubSys->SetSavePath(Dialog->GetPath());
            SubSys->SetTextureCreateCommand(UUI_BloomanEdSubsystem::ETexCreateCmd::CreateNew);
        }

        if (!HideOutline()) {
            // 既にOutline非表示であれば、すぐに命令を実行できる
            TriggerCreateTextureCommand();
        }
    } while (0);

    return FReply::Handled();
}

FReply FFakeBloomCustomization::OnOverwriteTextureClicked()
{

    if (UUI_BloomanEdSubsystem* SubSys = GEditor->GetEditorSubsystem<UUI_BloomanEdSubsystem>()) {
        SubSys->SetTextureCreateCommand(UUI_BloomanEdSubsystem::ETexCreateCmd::Overwrite);
    }

    if (!HideOutline()) {
        TriggerCreateTextureCommand();
    }

    return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE 
