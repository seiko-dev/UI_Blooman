// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class FFakeBloomUI_Customization : public IDetailCustomization
{
public:
    FFakeBloomUI_Customization();

    // Makes a new instance of this detail layout class for a specific detail view requesting it
    static TSharedRef<IDetailCustomization> MakeInstance();
    
    // IDetailCustomization interface
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
    virtual void PendingDelete() override;
    // End of IDetailCustomizationinterface

private:
    FReply OnCreateNewTextureClicked();
    FReply OnOverwriteTextureClicked();

    bool HideOutline();
    void ToggleOutline();
    void FinishCreateTexture();
    void TriggerCreateTextureCommand();

    void OnUpdateStats(class UFakeBloomUI* Widget);

    TSharedPtr<SWidget> SearchNearestParentAssetEditor(TSharedPtr<SWidget> CurrentWidget);
    TSharedPtr<SWidget> SearchNearestChildDesignerView(TSharedPtr<SWidget> CurrentWidget);

private:
    TSharedPtr<IPropertyHandle> UseTexHandle;
    TSharedPtr<IPropertyHandle> TextureHandle;
    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    TSharedPtr<class SWidget> DesignerView;
    int32_t TextureCreateWorkerNum;
    TSharedPtr<STextBlock> BloomTextureStatText;
};
