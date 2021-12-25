// Copyright Epic Games, Inc. All Rights Reserved.

#include "MoveCoreEditorModule.h"

#include "AssetToolsModule.h"
#include "ATActions/ATActions_FightAction.h"
#include "ATActions/ATActions_MoveSequence.h"

#include "PropertyEditorModule.h"
#include "JoystickInput.h"
#include "Customization/PTCustomization_JoystickInput.h"
#include "Graph/MSeqGraphNode_Regular.h"
#include "Customization/DCustomization_MSeqGraphNode_Regular.h"

#define LOCTEXT_NAMESPACE "FMoveCoreEditorModule"

void FMoveCoreEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(FJoystickInput::StaticStruct()->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FPTCustomization_JoystickInput::MakeInstance));
	PropertyModule.RegisterCustomClassLayout(UMSeqGraphNode_Regular::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FDCustomization_MSeqGraphNode_Regular::MakeInstance));

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
		PropertyModule.UnregisterCustomClassLayout(UMSeqGraphNode_Regular::StaticClass()->GetFName());
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