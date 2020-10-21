#pragma once

#include "UObject/ObjectMacros.h"
#include "K2Node.h"
#include "CAnimGraphNode_Base.generated.h"

enum class EBlueprintUsage : uint8;
enum class EAnimAssetHandlerType : uint8;

class USkeleton;
class UAnimationAsset;
class IDetailLayoutBuilder;
class FPrimitiveDrawInterface;
class FViewport;
class FCanvas;
class FSceneView;

typedef FName FEditorModeID;
struct FPoseLinkMappingRecord;

// CAnim
class UCAnimMeshComponentBase;
struct FCAnimNode_Base;
class UCAnimBlueprintBase;
class UCAnimBlueprintGeneratedClassBase;

UCLASS(Abstract)
class UCAnimGraphNode_Base : public UK2Node {

	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = PinOptions, EditFixedSize)
	TArray<FOptionalPinFromProperty> ShowPinForProperties;

	UPROPERTY(Transient)
	EBlueprintUsage BlueprintUsage;


#pragma region UObject interface

		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
		virtual void PreEditChange(UProperty* PropertyAboutToChange) override;

#pragma endregion // End of UObject interface
	
#pragma region UEdGraphNode interface

	virtual void AllocateDefaultPins() override;
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Black; }
	virtual FString GetDocumentationLink() const override { return TEXT("Shared/GraphNodes/Animation"); }
	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;
	virtual bool ShowPaletteIconOnNode() const override { return false; }
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FString GetPinMetaData(FName InPinName, FName InKey) override;

#pragma endregion // End of UEdGraphNode interface
	
#pragma region UK2Node interface

	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual bool ShouldShowNodeProperties() const override { return true; }
	virtual bool CanPlaceBreakpoints() const override { return false; }
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;
	virtual void GetNodeAttributes(TArray<TKeyValuePair<FString, FString>>& OutNodeAttributes) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override { return FText::FromString(GetNodeCategory()); }
	
	// By default return any animation assets we have
	virtual UObject* GetJumpTargetForDoubleClick() const override { return GetAnimationAsset(); }
	virtual bool CanJumpToDefinition() const override;
	virtual void JumpToDefinition() const override;

#pragma endregion // End of UK2Node interface

		// Gets the menu category this node belongs in
	virtual FString GetNodeCategory() const { return TEXT("Misc."); }

	// Is this node a sink that has no pose outputs?
	virtual bool IsSinkNode() const { return false; }

	// Create any output pins necessary for this node
	virtual void CreateOutputPins();

	// customize pin data based on the input
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex) const {}

	// Gives each visual node a chance to do final validation before it's node is harvested for use at runtime
	virtual void ValidateAnimNodeDuringCompilation(USkeleton* ForSkeleton, FCompilerResultsLog& MessageLog);

	// Gives each visual node a chance to validate that they are still valid in the context of the compiled class, giving a last shot at error or warning generation after primary compilation is finished
	virtual void ValidateAnimNodePostCompile(FCompilerResultsLog& MessageLog, UCAnimBlueprintGeneratedClassBase* CompiledClass, int32 CompiledNodeIndex) {}

	// If using CopyPoseFromMesh, the AnimBlueprint Compiler will cache this off for optimizations. 
	virtual bool UsingCopyPoseFromMesh() { return false; }

	// Gives each visual node a chance to update the node template before it is inserted in the compiled class
	virtual void BakeDataDuringCompilation(FCompilerResultsLog& MessageLog) {}

	// Give the node a chance to change the display name of a pin
	virtual void PostProcessPinName(const UEdGraphPin* Pin, FString& DisplayName) const;

	/** Get the animation blueprint to which this node belongs */
	UCAnimBlueprintBase* GetCAnimBlueprint() const;

	// Populate the supplied arrays with the currently reffered to animation assets 
	virtual void GetAllAnimationSequencesReferred(TArray<UAnimationAsset*>& AnimAssets) const {}

	// Replace references to animations that exist in the supplied maps 	
	virtual void ReplaceReferredAnimations(const TMap<UAnimationAsset*, UAnimationAsset*>& AnimAssetReplacementMap) {};

	// Helper function for GetAllAnimationSequencesReferred
	void HandleAnimReferenceCollection(UAnimationAsset* AnimAsset, TArray<UAnimationAsset*>& AnimationAssets) const;

	// Helper function for ReplaceReferredAnimations	
	template<class AssetType>
	void HandleAnimReferenceReplacement(AssetType*& OriginalAsset, const TMap<UAnimationAsset*, UAnimationAsset*>& AnimAssetReplacementMap);

	/**
	 * Selection notification callback.
	 * If a node needs to handle viewport input etc. then it should push an editor mode here.
	 * @param	bInIsSelected	Whether we selected or deselected the node
	 * @param	InModeTools		The mode tools. Use this to push the editor mode if required.
	 * @param	InRuntimeNode	The runtime node to go with this skeletal control. This may be NULL in some cases when bInIsSelected is false.
	 */
	virtual void OnNodeSelected(bool bInIsSelected, class FEditorModeTools& InModeTools, struct FCAnimNode_Base* InRuntimeNode);

	/**
	 * Override this function to push an editor mode when this node is selected
	 * @return the editor mode to use when this node is selected
	 */
	virtual FEditorModeID GetEditorMode() const;

	// Draw function for supporting visualization
	virtual void Draw(FPrimitiveDrawInterface* PDI, UCAnimMeshComponentBase * PreviewSkelMeshComp) const {}
	// Canvas draw function to draw to viewport
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas, UCAnimMeshComponentBase * PreviewSkelMeshComp) const {}
	// Function to collect strings from nodes to display in the viewport.
	// Use this rather than DrawCanvas when adding general text to the viewport.
	virtual void GetOnScreenDebugInfo(TArray<FText>& DebugInfo, FCAnimNode_Base* RuntimeAnimNode, UCAnimMeshComponentBase* PreviewSkelMeshComp) const {}

	/** Called after editing a default value to update internal node from pin defaults. This is needed for forwarding code to propagate values to preview. */
	virtual void CopyPinDefaultsToNodeData(UEdGraphPin* InPin) {}

	/** Called to propagate data from the internal node to the preview in Persona. */
	virtual void CopyNodeDataToPreviewNode(FCAnimNode_Base* InPreviewNode) {}

#pragma region Interface to support transition getter

	// if you return true for DoesSupportExposeTimeForTransitionGetter
	// you should implement all below functions
	virtual bool DoesSupportTimeForTransitionGetter() const { return false; }
	virtual UAnimationAsset* GetAnimationAsset() const { return nullptr; }
	virtual const TCHAR* GetTimePropertyName() const { return nullptr; }
	virtual UScriptStruct* GetTimePropertyStruct() const { return nullptr; }

#pragma endregion // END Interface to support transition getter

	// can customize details tab 
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {}

	/** Try to find the preview node instance for this anim graph node */
	FCAnimNode_Base* FindDebugAnimNode(UCAnimMeshComponentBase * PreviewSkelMeshComp) const;

	template<typename NodeType>
	NodeType* GetActiveInstanceNode(UObject* AnimInstanceObject) const {
		if (!AnimInstanceObject) {
			return nullptr;
		}

		if (UCAnimBlueprintGeneratedClassBase* AnimClass = Cast<UCAnimBlueprintGeneratedClassBase>(AnimInstanceObject->GetClass())) {
			return AnimClass->GetPropertyInstance<NodeType>(AnimInstanceObject, NodeGuid);
		}

		return nullptr;
	}

	/**
	 *	Returns whether this node supports the supplied asset class
	 *	@param	bPrimaryAssetHandler	Is this the 'primary' handler for this asset (the node that should be created when asset is dropped)
	 */
	virtual EAnimAssetHandlerType SupportsAssetClass(const UClass* AssetClass) const;

	// Event that observers can bind to so that they are notified about changes
	// made to this node through the property system
	DECLARE_EVENT_OneParam(UCAnimGraphNode_Base, FOnNodePropertyChangedEvent, FPropertyChangedEvent&);
	FOnNodePropertyChangedEvent& OnNodePropertyChanged() { return PropertyChangeEvent; }

	/**
	 * Helper function to check whether a pin is valid and linked to something else in the graph
	 * @param	InPinName		The name of the pin @see UEdGraphNode::FindPin
	 * @param	InPinDirection	The direction of the pin we are looking for. If this is EGPD_MAX, all directions are considered
	 * @return true if the pin is present and connected
	 */
	bool IsPinExposedAndLinked(const FString& InPinName, const EEdGraphPinDirection Direction = EGPD_MAX) const;

protected:
	// Gets the animation FNode type represented by this ed graph node
	UScriptStruct* GetFNodeType() const;

	// Gets the animation FNode property represented by this ed graph node
	UStructProperty* GetFNodeProperty() const;

	// This will be called when a pose link is found, and can be called with PoseProperty being either of:
	//  - an array property (ArrayIndex >= 0)
	//  - a single pose property (ArrayIndex == INDEX_NONE)
	virtual void CreatePinsForPoseLink(UProperty* PoseProperty, int32 ArrayIndex);


	virtual FPoseLinkMappingRecord GetLinkIDLocation(const UScriptStruct* NodeType, UEdGraphPin* InputLinkPin);

	/** Get the property (and possibly array index) associated with the supplied pin */
	virtual void GetPinAssociatedProperty(const UScriptStruct* NodeType, const UEdGraphPin* InputPin, UProperty*& OutProperty, int32& OutIndex) const;

	// Allocates or reallocates pins
	void InternalPinCreation(TArray<UEdGraphPin*>* OldPins);

	FOnNodePropertyChangedEvent PropertyChangeEvent;

private:
	TArray<FName> OldShownPins;
};
