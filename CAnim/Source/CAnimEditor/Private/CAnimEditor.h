// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Stats/Stats.h"
#include "Widgets/SWidget.h"
#include "Toolkits/IToolkitHost.h"
#include "GraphEditor.h"
#include "BlueprintEditor.h"
#include "ICAnimEditor.h"
#include "Containers/ArrayView.h"

struct FCAnimNode_Base;
class UCAnimBlueprintBase;
class UCAnimGraphNode_Base;

class UAnimationBlueprintEditorOptions;
class IPersonaToolkit;
class IPersonaViewport;
class ISkeletonTree;
class UEdGraph;
class USkeletalMesh;
class ISkeletonTreeItem;

struct FAnimationBlueprintEditorModes;

class FCAnimEditor : public ICAnimEditor {
	
	friend class FAnimationBlueprintEditorMode;

public:
	/**
	 * Edits the specified character asset(s)
	 *
	 * @param	Mode					Mode that this editor should operate in
	 * @param	InitToolkitHost			When Mode is WorldCentric, this is the level editor instance to spawn this editor within
	 * @param	InitSkeleton			The skeleton to edit.  If specified, Blueprint must be NULL.
	 * @param	InitAnimBlueprint		The blueprint object to start editing.  If specified, Skeleton and AnimationAsset must be NULL.
	 * @param	InitAnimationAsset		The animation asset to edit.  If specified, Blueprint must be NULL.
	 * @param	InitMesh				The mesh asset to edit.  If specified, Blueprint must be NULL.	 
	 */
	void InitCAnimEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UCAnimBlueprintBase* InAnimBlueprint) {}

public:
	FCAnimEditor();

	virtual ~FCAnimEditor();

	/** Update the inspector that displays information about the current selection*/
	void SetDetailObjects(const TArray<UObject*>& InObjects);
	void SetDetailObject(UObject* Obj);

	/** IAnimationBlueprintEditor interface */
	virtual const FEdGraphPinType& GetLastGraphPinTypeUsed() const override { return LastGraphPinType; }
	virtual void SetLastGraphPinTypeUsed(const FEdGraphPinType& InType) override { LastGraphPinType = InType; }

	/** IHasPersonaToolkit interface */
	virtual TSharedRef<class IPersonaToolkit> GetPersonaToolkit() const { return PersonaToolkit.ToSharedRef(); }

	/** FBlueprintEdi1tor interface */
	virtual void OnActiveTabChanged(TSharedPtr<SDockTab> PreviouslyActive, TSharedPtr<SDockTab> NewlyActivated) override;
	virtual void OnSelectedNodesChangedImpl(const TSet<class UObject*>& NewSelection) override;
	virtual void HandleSetObjectBeingDebugged(UObject* InObject) override;

	// Gets the Anim Blueprint being edited/viewed by this Persona instance
	UCAnimBlueprintBase* GetCAnimBlueprint() const;

	// Sets the current preview mesh
	void SetPreviewMesh(USkeletalMesh* NewPreviewMesh);

	/** Clears the selected actor */
	void ClearSelectedActor();

	/** Clears the selected anim graph node */
	void ClearSelectedAnimGraphNode();

	/** Clears the selection (both sockets and bones). Also broadcasts this */
	void DeselectAll();

	/** Returns the editors preview scene */
	TSharedRef<class IPersonaPreviewScene> GetPreviewScene() const;

	/** Handle general object selection */
	void HandleObjectsSelected(const TArray<UObject*>& InObjects);
	void HandleObjectSelected(UObject* InObject);
	void HandleSelectionChanged(const TArrayView<TSharedPtr<ISkeletonTreeItem>>& InSelectedItems, ESelectInfo::Type InSelectInfo);

	/** Get the object to be displayed in the asset properties */
	UObject* HandleGetObject();

	/** Handle opening a new asset from the asset browser */
	void HandleOpenNewAsset(UObject* InNewAsset);


public:

	/** @return the documentation location for this editor */
	virtual FString GetDocumentationLink() const override {	return FString(TEXT("Engine/Animation/Persona")); }
	
	/** Returns a pointer to the Blueprint object we are currently editing, as long as we are editing exactly one */
	virtual UBlueprint* GetBlueprintObj() const override;

	TSharedRef<SWidget> GetPreviewEditor() { return PreviewEditor.ToSharedRef(); }
	
	/** Refresh Preview Instance Track Curves **/
	void RefreshPreviewInstanceTrackCurves();

	void RecompileAnimBlueprintIfDirty();

	/** Get the skeleton tree this Persona editor is hosting */
	TSharedRef<class ISkeletonTree> GetSkeletonTree() const { return SkeletonTree.ToSharedRef(); }

	/** Viewport widget */
	TWeakPtr<class IPersonaViewport> Viewport;

	/** holding this pointer to refresh persona mesh detials tab when LOD is changed **/
	class IDetailLayoutBuilder* PersonaMeshDetailLayout;

	// Called after an undo is performed to give child widgets a chance to refresh
	typedef FSimpleMulticastDelegate::FDelegate FOnPostUndo;

	/** Registers a delegate to be called after an Undo operation */
	void RegisterOnPostUndo(const FOnPostUndo& Delegate) {
		OnPostUndo.Add(Delegate);
	}

	/** Unregisters a delegate to be called after an Undo operation */
	void UnregisterOnPostUndo(SWidget* Widget) {
		OnPostUndo.RemoveAll(Widget);
	}

	/** Delegate called after an undo operation for child widgets to refresh */
	FSimpleMulticastDelegate OnPostUndo;

protected:

	// Toggle pose watch on selected nodes
	void OnTogglePoseWatch();

	void BindCommands();

	/** Clear up Preview Mesh's AnimNotifyStates. Called when undo or redo**/
	void ClearupPreviewMeshAnimNotifyStates();

	void UndoAction();

	void RedoAction();

private:

	/** Extend menu */
	void ExtendMenu();

	/** Extend toolbar */
	void ExtendToolbar();

	/** Called immediately prior to a blueprint compilation */
	void OnBlueprintPreCompile(UBlueprint* BlueprintToCompile);

	/** Called post compile to copy node data */
	void OnPostCompile();

	/** Helper function used to keep skeletal controls in preview & instance in sync */
	FCAnimNode_Base* FindAnimNode(class UCAnimGraphNode_Base* AnimGraphNode) const;

	/** Handle a pin's default value changing be propagating it to the preview */
	void HandlePinDefaultValueChanged(UEdGraphPin* InPinThatChanged);

	/** Handle the preview mesh changing (so we can re-hook debug anim links etc.) */
	void HandlePreviewMeshChanged(USkeletalMesh* OldPreviewMesh, USkeletalMesh* NewPreviewMesh);

	/** Handle the viewport being created */
	void HandleViewportCreated(const TSharedRef<IPersonaViewport>& InPersonaViewport);

	/**
	 * Load editor settings from disk (docking state, window pos/size, option state, etc).
	 */
	virtual void LoadEditorSettings();

	/**
	 * Saves editor settings to disk (docking state, window pos/size, option state, etc).
	 */
	virtual void SaveEditorSettings();

	/** The extender to pass to the level editor to extend it's window menu */
	TSharedPtr<FExtender> MenuExtender;

	/** Toolbar extender */
	TSharedPtr<FExtender> ToolbarExtender;

	/** Preview instance inspector widget */
	TSharedPtr<class SWidget> PreviewEditor;

	/** Persona toolkit */
	TSharedPtr<class IPersonaToolkit> PersonaToolkit;

	/** Skeleton tree */
	TSharedPtr<class ISkeletonTree> SkeletonTree;

	// selected anim graph node 
	TWeakObjectPtr<UCAnimGraphNode_Base> SelectedAnimGraphNode;

	/** Delegate handle registered for when pin default values change */
	FDelegateHandle OnPinDefaultValueChangedHandle;

	/** The last pin type we added to a graph's inputs */
	FEdGraphPinType LastGraphPinType;

	/** Configuration class used to store editor settings across sessions. */
	UAnimationBlueprintEditorOptions* EditorOptions;

#pragma region FGCObject Interface

public:
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

#pragma endregion // End FGCObject Interface

#pragma region IToolkit Interface

public:
	virtual FName GetToolkitFName() const override { return FName("CAnimBlueprintEditor"); }
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;	

#pragma endregion // End IToolkit Interface

#pragma region FTickableEditorObject Interface

public:
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

#pragma endregion // End FTickableEditorObject Interface

#pragma region FBlueprintEditor Interface

protected:
	virtual void CreateDefaultCommands() override;
	virtual void OnCreateGraphEditorCommands(TSharedPtr<FUICommandList> GraphEditorCommandsList);
	virtual bool CanSelectBone() const override { return true; }
	virtual void OnAddPosePin() override;
	virtual bool CanAddPosePin() const override { return true; }
	virtual void OnRemovePosePin() override;
	virtual bool CanRemovePosePin() const override { return true; }
	virtual void Compile() override;
	virtual void OnGraphEditorFocused(const TSharedRef<class SGraphEditor>& InGraphEditor) override;
	virtual void OnGraphEditorBackgrounded(const TSharedRef<SGraphEditor>& InGraphEditor) override;
	virtual void OnConvertToSequenceEvaluator() override;
	virtual void OnConvertToSequencePlayer() override;
	virtual void OnConvertToBlendSpaceEvaluator() override;
	virtual void OnConvertToBlendSpacePlayer() override;
	virtual void OnConvertToPoseBlender() override;
	virtual void OnConvertToPoseByName() override;
	virtual void OnConvertToAimOffsetLookAt() override;
	virtual void OnConvertToAimOffsetSimple() override;
	virtual bool IsInAScriptingMode() const override { return true; }
	virtual void OnOpenRelatedAsset() override;
	virtual void GetCustomDebugObjects(TArray<FCustomDebugObject>& DebugList) const override;
	virtual void CreateDefaultTabContents(const TArray<UBlueprint*>& InBlueprints) override;
	virtual FGraphAppearanceInfo GetGraphAppearance(class UEdGraph* InGraph) const override;
	virtual bool IsEditable(UEdGraph* InGraph) const override;
	virtual FText GetGraphDecorationString(UEdGraph* InGraph) const override;
	virtual void OnBlueprintChangedImpl(UBlueprint* InBlueprint, bool bIsJustBeingCompiled = false) override;	

#pragma endregion // End FBlueprintEditor Interface

#pragma region FEditorUndoClient Interface

protected:
	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

#pragma endregion // End FEditorUndoClient Interface

#pragma region FNotifyHook Interface

protected:
	virtual void NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, UProperty* PropertyThatChanged) override;

#pragma endregion // End FNotifyHook Interface
};
