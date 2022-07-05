// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI_BloomanEd.h"
#include "PropertyEditorModule.h"
#include "BloomWidgetBaseCustomization.h"
#include "BloomWidgetBase.h"
#include "UMGEditorProjectSettings.h"

#define LOCTEXT_NAMESPACE "FUI_BloomanEdModule"

void FUI_BloomanEdModule::StartupModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    
    PropertyModule.RegisterCustomClassLayout(
        UBloomWidgetBase::StaticClass()->GetFName(),
        FOnGetDetailCustomizationInstance::CreateStatic(&FBloomWidgetBaseCustomization::MakeInstance));

    PropertyModule.NotifyCustomizationModuleChanged();
}

void FUI_BloomanEdModule::ShutdownModule()
{
    FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
    PropertyModule.UnregisterCustomClassLayout(UBloomWidgetBase::StaticClass()->GetFName());
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUI_BloomanEdModule, UI_BloomanEd)