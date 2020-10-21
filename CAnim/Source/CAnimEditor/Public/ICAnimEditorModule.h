#pragma once

#include "CoreMinimal.h"

#include "Framework/Commands/UICommandList.h"
#include "Framework/MultiBox/MultiBoxExtender.h"

#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"

#include "Modules/ModuleInterface.h"
#include "ICAnimEditor.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCAnimBlueprintEditor, Log, All);

class IToolkitHost;
class UCAnimBlueprintBase;

class ICAnimEditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility {
public:
	/**
	 * Creates an instance of an CAnim Blueprint editor.
	 *
	 * Note: This function should not be called directly, use one of the following instead:
	 *	- FKismetEditorUtilities::BringKismetToFocusAttentionOnObject
	 *  - FAssetEditorManager::Get().OpenEditorForAsset
	 *
	 * @param	Mode					Mode that this editor should operate in
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	CAnimBlueprint			The blueprint object to start editing.  If specified, Skeleton and AnimationAsset must be NULL.
	 *
	 * @return	Interface to the new Animation Blueprint editor
	 */
	virtual TSharedRef<ICAnimEditor> CreateCAnimEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UCAnimBlueprintBase* Blueprint) = 0;

	/** Get all toolbar extenders */
	DECLARE_DELEGATE_RetVal_TwoParams(TSharedRef<FExtender>, FCAnimEditorToolbarExtender, const TSharedRef<FUICommandList> /*InCommandList*/, TSharedRef<ICAnimEditor> /*InAnimationBlueprintEditor*/);
	virtual TArray<FCAnimEditorToolbarExtender>& GetAllCAnimEditorToolbarExtenders() = 0;
};
