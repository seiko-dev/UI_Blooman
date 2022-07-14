// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI_BloomanEd.h"
#include "PropertyEditorModule.h"
#include "PseudoBloom.h"
#include "PseudoBloomCustomization.h"

#define LOCTEXT_NAMESPACE "FUI_BloomanEdModule"

void FUI_BloomanEdModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomClassLayout(
        UPseudoBloom::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FPseudoBloomCustomization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FUI_BloomanEdModule::ShutdownModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.UnregisterCustomClassLayout(UPseudoBloom::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUI_BloomanEdModule, UI_BloomanEd)