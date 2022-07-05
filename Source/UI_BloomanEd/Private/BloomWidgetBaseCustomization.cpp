#include "BloomWidgetBaseCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "BloomWidgetBase.h"

TSharedRef<IDetailCustomization> FBloomWidgetBaseCustomization::MakeInstance()
{
    return MakeShareable(new FBloomWidgetBaseCustomization());
}

void FBloomWidgetBaseCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    SelectedObjects = DetailLayout.GetSelectedObjects();

    IDetailCategoryBuilder& SettingsCategory = DetailLayout.EditCategory("Settings", 
                                                                         FText::GetEmpty(),
                                                                         ECategoryPriority::Important);
    
    TexHandle = DetailLayout.GetProperty(GET_MEMBER_NAME_CHECKED(UBloomWidgetBase, BloomTexture));

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

    {
        FDetailWidgetRow& ButtonRow = SettingsCategory.AddCustomRow(FText::GetEmpty());
        // 中身の設定 (ボタンのラベルとクリック時の動作を設定)
        ButtonRow.WholeRowContent()
            [
                ButtonBox
            ];
    }
}


// ボタンクリック時に呼び出されるメソッド
FReply FBloomWidgetBaseCustomization::OnCreateNewTextureClicked()
{
    for (auto& Obj : SelectedObjects) {
        if (Obj.IsValid()) {
            if (UBloomWidgetBase* Widget = Cast<UBloomWidgetBase>(Obj.Get())) {
                Widget->TexPropHandle = TexHandle;
                Widget->RequestCreateNewTexture();
            }
        }
    }

    return FReply::Handled();
}

FReply FBloomWidgetBaseCustomization::OnOverwriteTextureClicked()
{
    for (auto& Obj : SelectedObjects) {
        if (Obj.IsValid()) {
            if (UBloomWidgetBase* Widget = Cast<UBloomWidgetBase>(Obj.Get())) {
                Widget->TexPropHandle = TexHandle;
                Widget->RequestOverwriteTexture();
            }
        }
    }

    return FReply::Handled();
}

