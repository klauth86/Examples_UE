// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "ICAnimEditorModule.h"

struct FCAnimGraphNodeFactory;
struct FCAnimGraphPinFactory;
struct FCAnimGraphPinConnectionFactory;

/**
 * Animation Blueprint Editor module allows editing of Animation Blueprints
 */
class FCAnimEditorModule : public ICAnimEditorModule
{
public:
	/**
	 * Called right after the module's DLL has been loaded and the module object has been created
	 */
	virtual void StartupModule();

	/**
	 * Called before the module is unloaded, right before the module object is destroyed.
	 */
	virtual void ShutdownModule();

	/** IAnimationBlueprintEditorModule interface */
	virtual TSharedRef<class ICAnimEditor> CreateCAnimEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UCAnimBlueprintBase* Blueprint) override;
	virtual TArray<FCAnimEditorToolbarExtender>& GetAllCAnimEditorToolbarExtenders() { return CAnimEditorToolbarExtenders; }

	/** Gets the extensibility managers for outside entities to extend this editor's menus and toolbars */
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }

private:
	/** When a new AnimBlueprint is created, this will handle post creation work such as adding non-event default nodes */
	void OnNewBlueprintCreated(class UBlueprint* InBlueprint);

private:
	TSharedPtr<class FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<class FExtensibilityManager> ToolBarExtensibilityManager;

	TArray<FCAnimEditorToolbarExtender> CAnimEditorToolbarExtenders;

	TSharedPtr<FCAnimGraphNodeFactory> AnimGraphNodeFactory;
	TSharedPtr<FCAnimGraphPinFactory> AnimGraphPinFactory;
	TSharedPtr<FCAnimGraphPinConnectionFactory> AnimGraphPinConnectionFactory;
};
