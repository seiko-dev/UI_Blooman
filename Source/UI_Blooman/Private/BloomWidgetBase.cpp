// Copyright seiko_dev. All Rights Reserved.


#include "BloomWidgetBase.h"
#include "UObject/Field.h"

#define LOCTEXT_NAMESPACE "UI_Blooman"

UBloomWidgetBase::UBloomWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

int32 UBloomWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const_cast<UBloomWidgetBase*>(this)->PaintPreProcess(AllottedGeometry);

    return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

#if WITH_EDITOR
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Widgets/Input/SEditableTextBox.h"

// directory selection dialog (slate)
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
            UE_LOG(LogTemp, Warning, TEXT("InBasePath(%s) is not exist, \"/Game\" is used instead."), *InBasePath)
            basePath = TEXT("/Game");
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
        if(!NewName.IsEmpty()){
            AssetName = NewName.ToString();
        }
    }

private: // internal properties
    FString SelectedPath;
    FString AssetName;
    bool ConfirmedFlag = false;
};
#endif // WITH_EDITOR


void UBloomWidgetBase::OpenSaveTextureDialog(const FString& InBasePath, bool& IsSuccess, FString& Path)
{
#if WITH_EDITOR
    TSharedRef<SWindow> window = SNew(SWindow)
        .Title(LOCTEXT("BrowseForFolderTitle", "Save Static Texture"))
        .ClientSize(FVector2D(320.0f, 320.0f))
        .SizingRule(ESizingRule::UserSized)
        .SupportsMaximize(false)
        .SupportsMinimize(false);
        
    TSharedRef<SDirectoryDialog> dialog = SNew(SDirectoryDialog, InBasePath);
    window->SetContent(dialog);

    IMainFrameModule& mainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>("MainFrame");
    const TSharedPtr<SWindow>& parentWindow = mainFrameModule.GetParentWindow();
    if (parentWindow.IsValid())
    {
        FSlateApplication::Get().AddModalWindow(window, parentWindow.ToSharedRef());
        if (dialog->HasValidResult())
        {
            Path = dialog->GetPath();
            IsSuccess = true;
            return;
        }
    }
#endif // WITH_EDITOR

    Path.Empty();
    IsSuccess = false;
}

void UBloomWidgetBase::NotifyCreateTextureFinished()
{
#if WITH_EDITOR
    // プロパティ変化をSlateに通知
    if (TexPropHandle.IsValid()) {
        const UObject* Obj(nullptr);
        if (TexPropHandle->GetValue(Obj) == FPropertyAccess::Result::Success) {
            TexPropHandle->SetValue(Obj);
        }
    }

    // Outline制御の後始末が必要なら実行
    CreateTextureCallBackForEditor.ExecuteIfBound();
    CreateTextureCallBackForEditor.Unbind();
#endif
}

void UBloomWidgetBase::RequestTextureCreateCommand(ETexCreateCmd Cmd)
{
    switch (Cmd) {
    default: ensure(0);
    case ETexCreateCmd::None:
        break;

    case ETexCreateCmd::CreateNew:
        RequestCreateNewTexture();
        break;

    case ETexCreateCmd::Overwrite:
        RequestOverwriteTexture();
        break;
    }
}

bool UBloomWidgetBase::HasShowOutlineFlag()
{
#if WITH_EDITOR
    return EnumHasAnyFlags(GetDesignerFlags(), EWidgetDesignFlags::ShowOutline);
#else
    return false;
#endif
}

#undef LOCTEXT_NAMESPACE 
