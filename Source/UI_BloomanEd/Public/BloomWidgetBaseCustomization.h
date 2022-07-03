#pragma once

#include "IDetailCustomization.h"

class FBloomWidgetBaseCustomization : public IDetailCustomization
{
public:
    // Makes a new instance of this detail layout class for a specific detail view requesting it
    static TSharedRef<IDetailCustomization> MakeInstance();
    
    // IDetailCustomization interface
    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
    // End of IDetailCustomizationinterface


private:
    void NotifyTexChange(class UBloomWidgetBase* Widget);
    FReply OnCreateNewTextureClicked();
    FReply OnOverwriteTextureClicked();

private:
    TSharedPtr<IPropertyHandle> TexHandle;
    TArray<TWeakObjectPtr<UObject>> SelectedObjects;
};
