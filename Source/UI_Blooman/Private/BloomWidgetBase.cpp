// Fill out your copyright notice in the Description page of Project Settings.


#include "BloomWidgetBase.h"

#define LOCTEXT_NAMESPACE "UI_Blooman"

int32 UBloomWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const_cast<UBloomWidgetBase*>(this)->PaintPreProcess(AllottedGeometry);

    return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

#if WITH_EDITOR
#include "EditorAssetLibrary.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "../Private/SPathPicker.h"
#include "MainFrame/Public/Interfaces/IMainFrameModule.h"

// ref: https://qiita.com/Rinderon/items/059463ea4e44fc54f121
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
        FString basePath(InBasePath);

        if (UEditorAssetLibrary::DoesDirectoryExist(InBasePath)) {
            basePath = InBasePath;

        } else {
            UE_LOG(LogTemp, Warning, TEXT("InBasePath(%s) is not exist, \"/Game\" is used instead."), *InBasePath);
            basePath = TEXT("/Game");
        }

        FPathPickerConfig config;
        config.DefaultPath = basePath;
        config.OnPathSelected = FOnPathSelected::CreateRaw(this, &SDirectoryDialog::OnPathSelected);

        FContentBrowserModule& contentBrowserModule
            = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

        TSharedPtr<SPathPicker> pathPicker = StaticCastSharedRef<SPathPicker>(
            contentBrowserModule.Get().CreatePathPicker(config));

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

        TSharedRef<SVerticalBox> mainBox = SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            .Padding(0.0f, 0.0f, 0.0f, 4.0f)
            [
                pathPicker.ToSharedRef()
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
    bool HasValidResult() const {
        return ConfirmedFlag && (!SelectedPath.IsEmpty());
    }
    const FString& GetPath() const { return SelectedPath; }

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

    void OnPathSelected(const FString& InCurrentPath) { SelectedPath = InCurrentPath; }

private: // internal properties
    FString SelectedPath;
    bool ConfirmedFlag = false;
};
#endif // WITH_EDITOR


void UBloomWidgetBase::OpenSaveTextureDialog(bool& IsSuccess, FString& Path, FString& Name)
{
#if WITH_EDITOR
    TSharedRef<SWindow> window = SNew(SWindow)
        .Title(LOCTEXT("BrowseForFolderTitle", "Browse Folders"))
        .ClientSize(FVector2D(320.0f, 320.0f))
        .SizingRule(ESizingRule::UserSized)
        .SupportsMaximize(false)
        .SupportsMinimize(false);

    const FString InBasePath("/Game/");
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
    Name.Empty();
    IsSuccess = false;
}

#undef LOCTEXT_NAMESPACE 
