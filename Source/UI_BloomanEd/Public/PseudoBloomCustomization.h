// Copyright seiko_dev. All Rights Reserved.

#pragma once

#include "IDetailCustomization.h"

class FPseudoBloomCustomization : public IDetailCustomization
{
public:
    FPseudoBloomCustomization();

    // Makes a new instance of this detail layout class for a specific detail view requesting it
    static TSharedRef<IDetailCustomization> MakeInstance();
    
    // IDetailCustomization interface
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
    // End of IDetailCustomizationinterface

private:
    FReply OnCreateNewTextureClicked();
    FReply OnOverwriteTextureClicked();

    bool HideOutline();
    void ToggleOutline();
    void RestoreOutline();
    void TriggerCreateTextureCommand();
    
    TSharedPtr<SWidget> SearchNearestParentAssetEditor(TSharedPtr<SWidget> CurrentWidget);
    TSharedPtr<SWidget> SearchNearestChildDesignerView(TSharedPtr<SWidget> CurrentWidget);

private:
    TSharedPtr<IPropertyHandle> DrawParamHandle;
    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
    TSharedPtr<class SWidget> DesignerView;
    int32_t RestoreOutlineCounter;
};
