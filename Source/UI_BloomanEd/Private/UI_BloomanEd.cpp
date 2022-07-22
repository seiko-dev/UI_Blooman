// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI_BloomanEd.h"
#include "PropertyEditorModule.h"
#include "FakeBloomUI.h"
#include "FakeBloomUI_Customization.h"

#define LOCTEXT_NAMESPACE "FUI_BloomanEdModule"

void FUI_BloomanEdModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomClassLayout(
        UFakeBloomUI::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FFakeBloomUI_Customization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FUI_BloomanEdModule::ShutdownModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.UnregisterCustomClassLayout(UFakeBloomUI::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUI_BloomanEdModule, UI_BloomanEd)