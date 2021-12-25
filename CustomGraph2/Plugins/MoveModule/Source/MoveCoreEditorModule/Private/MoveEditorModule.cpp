// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveCoreEditorModule.h"

#include "AssetToolsModule.h"
#include "ATActions/ATActions_FightAction.h"
#include "ATActions/ATActions_MoveSequence.h"

#include "PropertyEditorModule.h"
#include "JoystickInput.h"
#include "PTCustomization/PTCustomization_JoystickInput.h"
#include "Graph/MSeqGraphNode_Regular.h"
#include "PTCustomization/PTCustomization_MSeqGraphNode_Regular.h"

#define LOCTEXT_NAMESPACE "FMoveCoreEditorModule"

void FMoveCoreEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPTCustomization_JoystickInput::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(UMSeqGraphNode_Regular::StaticClass()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPTCustomization_MSeqGraphNode_Regular::MakeInstance));

	RegisteredAssetTypeActions.Add(MakeShared<FATActions_FightAction>());
	RegisteredAssetTypeActions.Add(MakeShared<FATActions_MoveSequence>());
	
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	for (auto& registeredAssetTypeAction : RegisteredAssetTypeActions)
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
		PropertyModule.UnregisterCustomPropertyTypeLayout(UMSeqGraphNode_Regular::StaticClass()->GetFName());
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
	
IMPLEMENT_MODULE(FMoveCoreEditorModule, MoveCoreEditorModule)