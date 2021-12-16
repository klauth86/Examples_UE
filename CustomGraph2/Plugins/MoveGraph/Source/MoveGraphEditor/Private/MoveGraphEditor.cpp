// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveGraphEditor.h"

#include "AssetToolsModule.h"
#include "ATActions/ATActions_Move.h"

#include "PropertyEditorModule.h"
#include "JoystickInput.h"
#include "PTCustomization/PTCustomization_JoystickInput.h"

#define LOCTEXT_NAMESPACE "FMoveGraphEditorModule"

void FMoveGraphEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPTCustomization_JoystickInput::MakeInstance));

	RegisteredAssetTypeActions.Add(MakeShared<FATActions_Move>());

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (auto& registeredAssetTypeAction : RegisteredAssetTypeActions)
	{
		if (registeredAssetTypeAction.IsValid())
		{
			AssetTools.RegisterAssetTypeActions(registeredAssetTypeAction.ToSharedRef());
		}
	}
}

void FMoveGraphEditorModule::ShutdownModule()
{
	if (!UObjectInitialized()) return;

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName());
	}

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (auto& registeredAssetTypeAction : RegisteredAssetTypeActions)
	{
		if (registeredAssetTypeAction.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(registeredAssetTypeAction.ToSharedRef());
			registeredAssetTypeAction.Reset();
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMoveGraphEditorModule, MoveGraphEditor)