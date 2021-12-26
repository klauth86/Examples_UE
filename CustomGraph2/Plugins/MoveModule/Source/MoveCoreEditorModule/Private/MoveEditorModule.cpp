// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveCoreEditorModule.h"

#include "AssetToolsModule.h"
#include "ATActions/ATActions_FightAction.h"
#include "ATActions/ATActions_MoveSequence.h"

#include "PropertyEditorModule.h"
#include "JoystickInput.h"
#include "Customization/PTCustomization_JoystickInput.h"

#define LOCTEXT_NAMESPACE "FMoveCoreEditorModule"

void FMoveCoreEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPTCustomization_JoystickInput::MakeInstance));

	RegisteredAssetTypeActions.Add(MakeShared<FATActions_FightAction>());
	RegisteredAssetTypeActions.Add(MakeShared<FATActions_MoveSequence>());
	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (TSharedPtr<FAssetTypeActions_Base>& registeredAssetTypeAction : RegisteredAssetTypeActions)
	{
		if (registeredAssetTypeAction.IsValid())
		{
			AssetTools.RegisterAssetTypeActions(registeredAssetTypeAction.ToSharedRef());
		}
	}
}

void FMoveCoreEditorModule::ShutdownModule()
{
	if (!UObjectInitialized()) return;

	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName());
	}

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (TSharedPtr<FAssetTypeActions_Base>& registeredAssetTypeAction : RegisteredAssetTypeActions)
	{
		if (registeredAssetTypeAction.IsValid())
		{
			AssetTools.UnregisterAssetTypeActions(registeredAssetTypeAction.ToSharedRef());
			registeredAssetTypeAction.Reset();
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMoveCoreEditorModule, MoveCoreEditorModule)