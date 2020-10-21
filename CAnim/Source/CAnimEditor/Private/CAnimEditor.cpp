
#include "CAnimEditor.h"
#include "Editor.h"
#include "EditorReimportHandler.h"
#include "Preferences/AnimationBlueprintEditorOptions.h"

// Kismet
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/DebuggerCommands.h"
#include "SKismetInspector.h"
#include "SBlueprintEditorToolbar.h"

#include "EditorFontGlyphs.h"
#include "EditorStyleSet.h"

#include "CAnimGraph/CAnimGraphNode_Base.h"
#include "CAnimGraph/CAnimNode_Base.h"

#include "SSingleObjectDetailsPanel.h"
#include "IDetailsView.h"

// For PerconaToolkit
#include "IPersonaViewport.h"
#include "PersonaModule.h"
#include "IPersonaPreviewScene.h"
#include "IPersonaEditorModeManager.h"
#include "PersonaCommonCommands.h"
#include "IPersonaToolkit.h"
#include "Animation/AnimSequenceBase.h"
#include "Animation/DebugSkelMeshComponent.h"
#include "AnimPreviewInstance.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimationGraph.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "AnimGraphCommands.h"
#include "Framework/Commands/GenericCommands.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"

#include "Algo/Transform.h"
#include "ISkeletonTree.h"
#include "ISkeletonTreeItem.h"

#include "ICAnimEditorModule.h"
#include "ICAnimEditor.h"
#include "CAnimEditorModule.h"

#include "Animation/Base/CAnimBlueprintBase.h"

//#include "EdGraph/EdGraph.h"
//#include "AssetData.h"
//#include "Animation/AnimBlueprint.h"
//
//#include "BlueprintEditorTabs.h"
//
//
//#include "EdGraphUtilities.h"
//
//#include "AnimationBlueprintEditorMode.h"
//
//#include "AnimGraphNode_Base.h"
//#include "AnimGraphNode_BlendListByInt.h"
//#include "AnimGraphNode_BlendSpaceEvaluator.h"
//#include "AnimGraphNode_BlendSpacePlayer.h"
//#include "AnimGraphNode_LayeredBoneBlend.h"
//#include "AnimGraphNode_SequencePlayer.h"
//#include "AnimGraphNode_SequenceEvaluator.h"
//#include "AnimGraphNode_PoseByName.h"
//#include "AnimGraphNode_PoseBlendNode.h"
//#include "AnimGraphNode_MultiWayBlend.h"
//
//
//
//
//#include "AnimationEditorUtils.h"

//#include "ISkeletonEditorModule.h"
//#include "IAssetFamily.h"
//
//#include "AnimGraphNode_AimOffsetLookAt.h"
//#include "AnimGraphNode_RotationOffsetBlendSpace.h"
//#include "AnimationBlueprintInterfaceEditorMode.h"

// Hide related nodes feature
#include "EdGraphNode_Comment.h"
#include "AnimStateNodeBase.h"
#include "AnimStateEntryNode.h"

#define LOCTEXT_NAMESPACE "CAnimEditor"

#pragma region SSingleObjectDetailsPanel

class SCAnimBlueprintPreviewPropertyEditor : public SSingleObjectDetailsPanel {
public:
	SLATE_BEGIN_ARGS(SCAnimBlueprintPreviewPropertyEditor) {}
	SLATE_END_ARGS()

private:
	// Pointer back to owning Persona editor instance (the keeper of state)
	TWeakPtr<FCAnimEditor> AnimationBlueprintEditorPtr;
public:
	void Construct(const FArguments& InArgs, TSharedPtr<FCAnimEditor> InAnimationBlueprintEditor) {
		AnimationBlueprintEditorPtr = InAnimationBlueprintEditor;

		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InAnimationBlueprintEditor->GetToolkitCommands()).HostTabManager(InAnimationBlueprintEditor->GetTabManager()), /*bAutomaticallyObserveViaGetObjectToObserve*/ true, /*bAllowSearch*/ true);

		PropertyView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateStatic([] { return !GIntraFrameDebuggingGameThread; }));
	}

	// SSingleObjectDetailsPanel interface
	virtual UObject* GetObjectToObserve() const override {
		if (UDebugSkelMeshComponent* PreviewMeshComponent = AnimationBlueprintEditorPtr.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent()) {
			return PreviewMeshComponent->GetAnimInstance();
		}

		return nullptr;
	}

	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override {
		return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.f, 8.f, 0.f, 0.f)
			[
				SNew(SBorder)
				.BorderImage(FEditorStyle::GetBrush("Persona.PreviewPropertiesWarning"))
			[
				SNew(STextBlock)
				.Text(LOCTEXT("AnimBlueprintEditPreviewText", "Changes to preview options are not saved in the asset."))
			.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
			.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
			.ShadowOffset(FVector2D::UnitVector)
			]
			]
		+ SVerticalBox::Slot()
			.FillHeight(1)
			[
				PropertyEditorWidget
			];
	}
};

#pragma endregion













//const FName AnimationBlueprintEditorAppName(TEXT("AnimationBlueprintEditorApp"));
//
//const FName FAnimationBlueprintEditorModes::AnimationBlueprintEditorMode("GraphName");	// For backwards compatibility we keep the old mode name here
//const FName FAnimationBlueprintEditorModes::AnimationBlueprintInterfaceEditorMode("Interface");
//
//namespace AnimationBlueprintEditorTabs
//{
//	const FName DetailsTab(TEXT("DetailsTab"));
//	const FName SkeletonTreeTab(TEXT("SkeletonTreeView"));
//	const FName ViewportTab(TEXT("Viewport"));
//	const FName AdvancedPreviewTab(TEXT("AdvancedPreviewTab"));
//	const FName AssetBrowserTab(TEXT("SequenceBrowser"));
//	const FName AnimBlueprintPreviewEditorTab(TEXT("AnimBlueprintPreviewEditor"));
//	const FName AssetOverridesTab(TEXT("AnimBlueprintParentPlayerEditor"));
//	const FName SlotNamesTab(TEXT("SkeletonSlotNames"));
//	const FName CurveNamesTab(TEXT("AnimCurveViewerTab"));
//};
//
///////////////////////////////////////////////////////
//// SAnimBlueprintPreviewPropertyEditor
//
//class SAnimBlueprintPreviewPropertyEditor : public SSingleObjectDetailsPanel
//{
//public:
//	SLATE_BEGIN_ARGS(SAnimBlueprintPreviewPropertyEditor) {}
//	SLATE_END_ARGS()
//
//private:
//	// Pointer back to owning Persona editor instance (the keeper of state)
//	TWeakPtr<FAnimationBlueprintEditor> AnimationBlueprintEditorPtr;
//public:
//	void Construct(const FArguments& InArgs, TSharedPtr<FAnimationBlueprintEditor> InAnimationBlueprintEditor)
//	{
//		AnimationBlueprintEditorPtr = InAnimationBlueprintEditor;
//
//		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InAnimationBlueprintEditor->GetToolkitCommands()).HostTabManager(InAnimationBlueprintEditor->GetTabManager()), /*bAutomaticallyObserveViaGetObjectToObserve*/ true, /*bAllowSearch*/ true);
//
//		PropertyView->SetIsPropertyEditingEnabledDelegate(FIsPropertyEditingEnabled::CreateStatic([] { return !GIntraFrameDebuggingGameThread; }));
//	}
//
//	// SSingleObjectDetailsPanel interface
//	virtual UObject* GetObjectToObserve() const override
//	{
//		if (UDebugSkelMeshComponent* PreviewMeshComponent = AnimationBlueprintEditorPtr.Pin()->GetPersonaToolkit()->GetPreviewMeshComponent())
//		{
//			return PreviewMeshComponent->GetAnimInstance();
//		}
//
//		return nullptr;
//	}
//
//	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override
//	{
//		return SNew(SVerticalBox)
//			+SVerticalBox::Slot()
//			.AutoHeight()
//			.Padding(0.f, 8.f, 0.f, 0.f)
//			[
//				SNew(SBorder)
//				.BorderImage(FEditorStyle::GetBrush("Persona.PreviewPropertiesWarning"))
//				[
//					SNew(STextBlock)
//					.Text(LOCTEXT("AnimBlueprintEditPreviewText", "Changes to preview options are not saved in the asset."))
//					.Font(FEditorStyle::GetFontStyle("PropertyWindow.NormalFont"))
//					.ShadowColorAndOpacity(FLinearColor::Black.CopyWithNewOpacity(0.3f))
//					.ShadowOffset(FVector2D::UnitVector)
//				]
//			]
//			+SVerticalBox::Slot()
//			.FillHeight(1)
//			[
//				PropertyEditorWidget
//			];
//	}
//	// End of SSingleObjectDetailsPanel interface
//};
//
/////////////////////////////////////////////////////
// FAnimationBlueprintEditor

FCAnimEditor::FCAnimEditor()
	: PersonaMeshDetailLayout(NULL)
{
	GEditor->OnBlueprintPreCompile().AddRaw(this, &FCAnimEditor::OnBlueprintPreCompile);
	LastGraphPinType.ResetToDefaults();
	LastGraphPinType.PinCategory = UEdGraphSchema_K2::PC_Boolean;
}

FCAnimEditor::~FCAnimEditor()
{
	GEditor->OnBlueprintPreCompile().RemoveAll(this);

	GEditor->GetEditorSubsystem<UImportSubsystem>()->OnAssetPostImport.RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	// NOTE: Any tabs that we still have hanging out when destroyed will be cleaned up by FBaseToolkit's destructor

	SaveEditorSettings();
}

UCAnimBlueprintBase* FCAnimEditor::GetCAnimBlueprint() const
{
	return Cast<UCAnimBlueprintBase>(GetBlueprintObj());
}

void FCAnimEditor::ExtendMenu()
{
	if(MenuExtender.IsValid())
	{
		RemoveMenuExtender(MenuExtender);
		MenuExtender.Reset();
	}

	MenuExtender = MakeShareable(new FExtender);

	UCAnimBlueprintBase* AnimBlueprint = GetCAnimBlueprint();
	if(AnimBlueprint)
	{
		TSharedPtr<FExtender> AnimBPMenuExtender = MakeShareable(new FExtender);
		FKismet2Menu::SetupBlueprintEditorMenu(AnimBPMenuExtender, *this);
		AddMenuExtender(AnimBPMenuExtender);
	}

	AddMenuExtender(MenuExtender);

	// add extensible menu if exists
	FCAnimEditorModule& AnimationBlueprintEditorModule = FModuleManager::LoadModuleChecked<FCAnimEditorModule>("AnimationBlueprintEditor");
	AddMenuExtender(AnimationBlueprintEditorModule.GetMenuExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));
}

//void FCAnimEditor::InitAnimationBlueprintEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UAnimBlueprint* InAnimBlueprint)
//{
//	// Record if we have been newly created
//	bool bNewlyCreated = InAnimBlueprint->bIsNewlyCreated;
//	InAnimBlueprint->bIsNewlyCreated = false;
//
//	if (!Toolbar.IsValid())
//	{
//		Toolbar = MakeShareable(new FBlueprintEditorToolbar(SharedThis(this)));
//	}
//
//	LoadEditorSettings();
//
//	GetToolkitCommands()->Append(FPlayWorldCommands::GlobalPlayWorldActions.ToSharedRef());
//
//	FPersonaModule& PersonaModule = FModuleManager::GetModuleChecked<FPersonaModule>("Persona");
//	PersonaToolkit = PersonaModule.CreatePersonaToolkit(InAnimBlueprint);
//
//	PersonaToolkit->GetPreviewScene()->SetDefaultAnimationMode(EPreviewSceneDefaultAnimationMode::AnimationBlueprint);
//	PersonaToolkit->GetPreviewScene()->RegisterOnPreviewMeshChanged(FOnPreviewMeshChanged::CreateSP(this, &FCAnimEditor::HandlePreviewMeshChanged));
//
//	TSharedRef<IAssetFamily> AssetFamily = PersonaModule.CreatePersonaAssetFamily(InAnimBlueprint);
//	AssetFamily->RecordAssetOpened(FAssetData(InAnimBlueprint));
//
//	if(InAnimBlueprint->BlueprintType != BPTYPE_Interface)
//	{
//		// create the skeleton tree
//		FSkeletonTreeArgs SkeletonTreeArgs;
//		SkeletonTreeArgs.OnSelectionChanged = FOnSkeletonTreeSelectionChanged::CreateSP(this, &FCAnimEditor::HandleSelectionChanged);
//		SkeletonTreeArgs.PreviewScene = GetPreviewScene();
//		SkeletonTreeArgs.ContextName = GetToolkitFName();
//
//		ISkeletonEditorModule& SkeletonEditorModule = FModuleManager::LoadModuleChecked<ISkeletonEditorModule>("SkeletonEditor");
//		SkeletonTree = SkeletonEditorModule.CreateSkeletonTree(PersonaToolkit->GetSkeleton(), SkeletonTreeArgs);
//	}
//
//	// Build up a list of objects being edited in this asset editor
//	TArray<UObject*> ObjectsBeingEdited;
//	ObjectsBeingEdited.Add(InAnimBlueprint);
//
//	// Initialize the asset editor and spawn tabs
//	const TSharedRef<FTabManager::FLayout> DummyLayout = FTabManager::NewLayout("NullLayout")->AddArea(FTabManager::NewPrimaryArea());
//	const bool bCreateDefaultStandaloneMenu = true;
//	const bool bCreateDefaultToolbar = true;
//	InitAssetEditor(Mode, InitToolkitHost, AnimationBlueprintEditorAppName, DummyLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectsBeingEdited);
//
//	TArray<UBlueprint*> AnimBlueprints;
//	AnimBlueprints.Add(InAnimBlueprint);
//
//	CommonInitialization(AnimBlueprints);
//
//	BindCommands();
//
//	if(InAnimBlueprint->BlueprintType == BPTYPE_Interface)
//	{
//		AddApplicationMode(
//			FAnimationBlueprintEditorModes::AnimationBlueprintInterfaceEditorMode,
//			MakeShareable(new FAnimationBlueprintInterfaceEditorMode(SharedThis(this))));
//
//		ExtendMenu();
//		ExtendToolbar();
//		RegenerateMenusAndToolbars();
//
//		// Activate the initial mode (which will populate with a real layout)
//		SetCurrentMode(FAnimationBlueprintEditorModes::AnimationBlueprintInterfaceEditorMode);
//	}
//	else
//	{
//		AddApplicationMode(
//			FAnimationBlueprintEditorModes::AnimationBlueprintEditorMode,
//			MakeShareable(new FAnimationBlueprintEditorMode(SharedThis(this))));
//
//		UDebugSkelMeshComponent* PreviewMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
//		UAnimBlueprint* AnimBlueprint = PersonaToolkit->GetAnimBlueprint();
//		PreviewMeshComponent->SetAnimInstanceClass(AnimBlueprint ? AnimBlueprint->GeneratedClass : NULL);
//
//		// Make sure the object being debugged is the preview instance
//		AnimBlueprint->SetObjectBeingDebugged(PreviewMeshComponent->GetAnimInstance());
//
//		ExtendMenu();
//		ExtendToolbar();
//		RegenerateMenusAndToolbars();
//
//		// Activate the initial mode (which will populate with a real layout)
//		SetCurrentMode(FAnimationBlueprintEditorModes::AnimationBlueprintEditorMode);
//	}
//
//	// Post-layout initialization
//	PostLayoutBlueprintEditorInitialization();
//
//	// register customization of Slot node for this Animation Blueprint Editor
//	// this is so that you can open the manage window per Animation Blueprint Editor
//	PersonaModule.CustomizeSlotNodeDetails(Inspector->GetPropertyView().ToSharedRef(), FOnInvokeTab::CreateSP(this, &FAssetEditorToolkit::InvokeTab));
//
//	if(bNewlyCreated && InAnimBlueprint->BlueprintType == BPTYPE_Interface)
//	{
//		NewDocument_OnClick(CGT_NewAnimationLayer);
//	}
//}

void FCAnimEditor::BindCommands()
{
	GetToolkitCommands()->MapAction(FPersonaCommonCommands::Get().TogglePlay,
		FExecuteAction::CreateRaw(&GetPersonaToolkit()->GetPreviewScene().Get(), &IPersonaPreviewScene::TogglePlayback));
}

void FCAnimEditor::ExtendToolbar()
{
	// If the ToolbarExtender is valid, remove it before rebuilding it
	if(ToolbarExtender.IsValid())
	{
		RemoveToolbarExtender(ToolbarExtender);
		ToolbarExtender.Reset();
	}

	ToolbarExtender = MakeShareable(new FExtender);

	AddToolbarExtender(ToolbarExtender);

	FCAnimEditorModule& AnimationBlueprintEditorModule = FModuleManager::LoadModuleChecked<FCAnimEditorModule>("AnimationBlueprintEditor");
	AddToolbarExtender(AnimationBlueprintEditorModule.GetToolBarExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));

	TArray<ICAnimEditorModule::FCAnimEditorToolbarExtender> ToolbarExtenderDelegates = AnimationBlueprintEditorModule.GetAllCAnimEditorToolbarExtenders();

	for (auto& ToolbarExtenderDelegate : ToolbarExtenderDelegates)
	{
		if(ToolbarExtenderDelegate.IsBound())
		{
			AddToolbarExtender(ToolbarExtenderDelegate.Execute(GetToolkitCommands(), SharedThis(this)));
		}
	}

	// TODO COUPLED
	//UAnimBlueprint* AnimBlueprint = PersonaToolkit->GetAnimBlueprint();
	//if(AnimBlueprint && AnimBlueprint->BlueprintType != BPTYPE_Interface)
	//{
	//	ToolbarExtender->AddToolBarExtension(
	//		"Asset",
	//		EExtensionHook::After,
	//		GetToolkitCommands(),
	//		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& ParentToolbarBuilder)
	//		{
	//			FPersonaModule& PersonaModule = FModuleManager::LoadModuleChecked<FPersonaModule>("Persona");
	//			FPersonaModule::FCommonToolbarExtensionArgs Args;
	//			Args.bPreviewAnimation = false;
	//			PersonaModule.AddCommonToolbarExtensions(ParentToolbarBuilder, PersonaToolkit.ToSharedRef(), Args);

	//			TSharedRef<class IAssetFamily> AssetFamily = PersonaModule.CreatePersonaAssetFamily(GetBlueprintObj());
	//			AddToolbarWidget(PersonaModule.CreateAssetFamilyShortcutWidget(SharedThis(this), AssetFamily));
	//		}
	//	));
	//}

	struct Local
	{
		static void FillToolbar(FToolBarBuilder& ToolbarBuilder, const TSharedRef< FUICommandList > ToolkitCommands, FCAnimEditor* BlueprintEditor)
		{
			ToolbarBuilder.BeginSection("Graph");
			{
				ToolbarBuilder.AddToolBarButton(
					FAnimGraphCommands::Get().ToggleHideUnrelatedNodes,
					NAME_None,
					TAttribute<FText>(),
					TAttribute<FText>(),
					FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.ToggleHideUnrelatedNodes")
				);
				ToolbarBuilder.AddComboButton(
					FUIAction(),
					FOnGetContent::CreateSP(BlueprintEditor, &FBlueprintEditor::MakeHideUnrelatedNodesOptionsMenu),
					LOCTEXT("HideUnrelatedNodesOptions", "Hide Unrelated Nodes Options"),
					LOCTEXT("HideUnrelatedNodesOptionsMenu", "Hide Unrelated Nodes options menu"),
					TAttribute<FSlateIcon>(),
					true
				);
			}
			ToolbarBuilder.EndSection();
		}
	};

    ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateStatic( &Local::FillToolbar, GetToolkitCommands(), this )
	);
}

UBlueprint* FCAnimEditor::GetBlueprintObj() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();
	for (int32 i = 0; i < EditingObjs.Num(); ++i)
	{
		if (EditingObjs[i]->IsA<UCAnimBlueprintBase>()) {return (UBlueprint*)EditingObjs[i];}
	}
	return nullptr;
}

void FCAnimEditor::SetDetailObjects(const TArray<UObject*>& InObjects)
{
	Inspector->ShowDetailsForObjects(InObjects);
}

void FCAnimEditor::SetDetailObject(UObject* Obj)
{
	TArray<UObject*> Objects;
	if (Obj)
	{
		Objects.Add(Obj);
	}
	SetDetailObjects(Objects);
}

void FCAnimEditor::OnTogglePoseWatch()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	UCAnimBlueprintBase* AnimBP = GetCAnimBlueprint();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UCAnimGraphNode_Base* SelectedNode = Cast<UCAnimGraphNode_Base>(*NodeIt))
		{
			// TODO COUPLED
			//UPoseWatch* PoseWatch = AnimationEditorUtils::FindPoseWatchForNode(SelectedNode, AnimBP);
			//if (PoseWatch)
			//{
			//	AnimationEditorUtils::RemovePoseWatch(PoseWatch, AnimBP);
			//}
			//else
			//{
			//	AnimationEditorUtils::MakePoseWatchForNode(AnimBP, SelectedNode, FColor::Red);
			//}
		}
	}
}

void FCAnimEditor::RecompileAnimBlueprintIfDirty()
{
	if (UBlueprint* Blueprint = GetBlueprintObj())
	{
		if (!Blueprint->IsUpToDate())
		{
			Compile();
		}
	}
}

void FCAnimEditor::OnActiveTabChanged( TSharedPtr<SDockTab> PreviouslyActive, TSharedPtr<SDockTab> NewlyActivated )
{
	if (!NewlyActivated.IsValid())
	{
		TArray<UObject*> ObjArray;
		Inspector->ShowDetailsForObjects(ObjArray);
	}
	else 
	{
		FBlueprintEditor::OnActiveTabChanged(PreviouslyActive, NewlyActivated);
	}
}

void FCAnimEditor::SetPreviewMesh(USkeletalMesh* NewPreviewMesh)
{
	GetSkeletonTree()->SetSkeletalMesh(NewPreviewMesh);
}

void FCAnimEditor::RefreshPreviewInstanceTrackCurves()
{
	// need to refresh the preview mesh
	UDebugSkelMeshComponent* PreviewMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
	if(PreviewMeshComponent->PreviewInstance)
	{
		PreviewMeshComponent->PreviewInstance->RefreshCurveBoneControllers();
	}
}

void FCAnimEditor::ClearupPreviewMeshAnimNotifyStates()
{
	UDebugSkelMeshComponent* PreviewMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
	if ( PreviewMeshComponent )
	{
		UAnimInstance* AnimInstanace = PreviewMeshComponent->GetAnimInstance();

		if (AnimInstanace)
		{
			// empty this because otherwise, it can have corrupted data
			// this will cause state to be interrupted, but that is better
			// than crashing
			AnimInstanace->ActiveAnimNotifyState.Empty();
		}
	}
}

void FCAnimEditor::ClearSelectedActor()
{
	GetPreviewScene()->ClearSelectedActor();
}

void FCAnimEditor::ClearSelectedAnimGraphNode()
{
	SelectedAnimGraphNode.Reset();
}

void FCAnimEditor::DeselectAll()
{
	GetSkeletonTree()->DeselectAll();
	ClearSelectedActor();
	ClearSelectedAnimGraphNode();
}

void FCAnimEditor::UndoAction()
{
	GEditor->UndoTransaction();
}

void FCAnimEditor::RedoAction()
{
	GEditor->RedoTransaction();
}

void FCAnimEditor::OnBlueprintPreCompile(UBlueprint* BlueprintToCompile)
{
	if (PersonaToolkit.IsValid())
	{
		UDebugSkelMeshComponent* PreviewMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
		if(PreviewMeshComponent && PreviewMeshComponent->PreviewInstance)
		{
			// If we are compiling an anim notify state the class will soon be sanitized and 
			// if an anim instance is running a state when that happens it will likely
			// crash, so we end any states that are about to compile.
			UAnimPreviewInstance* Instance = PreviewMeshComponent->PreviewInstance;
			USkeletalMeshComponent* SkelMeshComp = Instance->GetSkelMeshComponent();

			for(int32 Idx = Instance->ActiveAnimNotifyState.Num() - 1 ; Idx >= 0 ; --Idx)
			{
				FAnimNotifyEvent& Event = Instance->ActiveAnimNotifyState[Idx];
				if(Event.NotifyStateClass->GetClass() == BlueprintToCompile->GeneratedClass)
				{
					Event.NotifyStateClass->NotifyEnd(SkelMeshComp, Cast<UAnimSequenceBase>(Event.NotifyStateClass->GetOuter()));
					Instance->ActiveAnimNotifyState.RemoveAt(Idx);
				}
			}
		}
	}
}

TSharedRef<IPersonaPreviewScene> FCAnimEditor::GetPreviewScene() const
{ 
	return PersonaToolkit->GetPreviewScene(); 
}

void FCAnimEditor::HandleObjectsSelected(const TArray<UObject*>& InObjects)
{
	SetDetailObjects(InObjects);
}

void FCAnimEditor::HandleObjectSelected(UObject* InObject)
{
	SetDetailObject(InObject);
}

void FCAnimEditor::HandleSelectionChanged(const TArrayView<TSharedPtr<ISkeletonTreeItem>>& InSelectedItems, ESelectInfo::Type InSelectInfo)
{
	TArray<UObject*> Objects;
	Algo::TransformIf(InSelectedItems, Objects, [](const TSharedPtr<ISkeletonTreeItem>& InItem) { return InItem->GetObject() != nullptr; }, [](const TSharedPtr<ISkeletonTreeItem>& InItem) { return InItem->GetObject(); });
	SetDetailObjects(Objects);
}

UObject* FCAnimEditor::HandleGetObject()
{
	return GetEditingObject();
}

void FCAnimEditor::HandleOpenNewAsset(UObject* InNewAsset)
{
	FAssetEditorManager::Get().OpenEditorForAsset(InNewAsset);
}

FCAnimNode_Base* FCAnimEditor::FindAnimNode(UCAnimGraphNode_Base* AnimGraphNode) const
{
	FCAnimNode_Base* AnimNode = nullptr;
	if (AnimGraphNode)
	{
		// TODO COUPLED
		//UDebugSkelMeshComponent* PreviewMeshComponent = GetPreviewScene()->GetPreviewMeshComponent();
		//if (PreviewMeshComponent != nullptr && PreviewMeshComponent->GetAnimInstance() != nullptr)
		//{
		//	AnimNode = AnimGraphNode->FindDebugAnimNode(PreviewMeshComponent);
		//}
	}

	return AnimNode;
}

void FCAnimEditor::OnSelectedNodesChangedImpl(const TSet<class UObject*>& NewSelection)
{
	FBlueprintEditor::OnSelectedNodesChangedImpl(NewSelection);

	IPersonaEditorModeManager* PersonaEditorModeManager = static_cast<IPersonaEditorModeManager*>(GetAssetEditorModeManager());

	if (UCAnimGraphNode_Base* SelectedAnimGraphNodePtr = SelectedAnimGraphNode.Get())
	{
		FCAnimNode_Base* PreviewNode = FindAnimNode(SelectedAnimGraphNodePtr);
		if (PersonaEditorModeManager)
		{
			SelectedAnimGraphNodePtr->OnNodeSelected(false, *PersonaEditorModeManager, PreviewNode);
		}

		SelectedAnimGraphNode.Reset();
	}

	// if we only have one node selected, let it know
	UCAnimGraphNode_Base* NewSelectedAnimGraphNode = nullptr;
	if (NewSelection.Num() == 1)
	{
		NewSelectedAnimGraphNode = Cast<UCAnimGraphNode_Base>(*NewSelection.CreateConstIterator());
		if (NewSelectedAnimGraphNode != nullptr)
		{
			SelectedAnimGraphNode = NewSelectedAnimGraphNode;

			FCAnimNode_Base* PreviewNode = FindAnimNode(NewSelectedAnimGraphNode);
			if (PreviewNode && PersonaEditorModeManager)
			{
				NewSelectedAnimGraphNode->OnNodeSelected(true, *PersonaEditorModeManager, PreviewNode);
			}
		}
	}

    bSelectRegularNode = false;
	for (FGraphPanelSelectionSet::TConstIterator It(NewSelection); It; ++It)
	{
		UEdGraphNode_Comment* SeqNode = Cast<UEdGraphNode_Comment>(*It);
		UAnimStateNodeBase* AnimGraphNodeBase = Cast<UAnimStateNodeBase>(*It);
		UAnimStateEntryNode* AnimStateEntryNode = Cast<UAnimStateEntryNode>(*It);
		if (!SeqNode && !AnimGraphNodeBase && !AnimStateEntryNode)
		{
			bSelectRegularNode = true;
			break;
		}
	}

    if (bHideUnrelatedNodes && !bLockNodeFadeState)
	{
		ResetAllNodesUnrelatedStates();

		if ( bSelectRegularNode )
		{
			HideUnrelatedNodes();
		}
	}
}

void FCAnimEditor::OnPostCompile()
{
	// act as if we have re-selected, so internal pointers are updated
	if (CurrentUISelection == FBlueprintEditor::SelectionState_Graph)
	{
		FGraphPanelSelectionSet SelectionSet = GetSelectedNodes();
		OnSelectedNodesChangedImpl(SelectionSet);
		FocusInspectorOnGraphSelection(SelectionSet, /*bForceRefresh=*/ true);
	}

	// if the user manipulated Pin values directly from the node, then should copy updated values to the internal node to retain data consistency
	UEdGraph* FocusedGraph = GetFocusedGraph();
	if (FocusedGraph)
	{
		// find UCAnimGraphNode_Base
		for (UEdGraphNode* Node : FocusedGraph->Nodes)
		{
			UCAnimGraphNode_Base* AnimGraphNode = Cast<UCAnimGraphNode_Base>(Node);
			if (AnimGraphNode)
			{
				FCAnimNode_Base* AnimNode = FindAnimNode(AnimGraphNode);
				if (AnimNode)
				{
					AnimGraphNode->CopyNodeDataToPreviewNode(AnimNode);
				}
			}
		}
	}
}

void FCAnimEditor::HandlePinDefaultValueChanged(UEdGraphPin* InPinThatChanged)
{
	UCAnimGraphNode_Base* AnimGraphNode = Cast<UCAnimGraphNode_Base>(InPinThatChanged->GetOwningNode());
	if (AnimGraphNode)
	{
		FCAnimNode_Base* AnimNode = FindAnimNode(AnimGraphNode);
		if (AnimNode)
		{
			AnimGraphNode->CopyNodeDataToPreviewNode(AnimNode);
		}
	}
}

void FCAnimEditor::HandleSetObjectBeingDebugged(UObject* InObject)
{
	FBlueprintEditor::HandleSetObjectBeingDebugged(InObject);
	
	// TODO COUPLED
	//if (UAnimInstance* AnimInstance = Cast<UAnimInstance>(InObject))
	//{
	//	USkeletalMeshComponent* SkeletalMeshComponent = AnimInstance->GetSkelMeshComponent();
	//	if (SkeletalMeshComponent)
	//	{
	//		// If we are selecting the preview instance, reset us back to 'normal'
	//		if (InObject->GetWorld()->IsPreviewWorld())
	//		{
	//			GetPreviewScene()->ShowDefaultMode();
	//			GetPreviewScene()->GetPreviewMeshComponent()->PreviewInstance->SetDebugSkeletalMeshComponent(nullptr);
	//		}
	//		else
	//		{
	//			// Otherwise eet us to display the debugged instance via copy-pose
	//			GetPreviewScene()->GetPreviewMeshComponent()->EnablePreview(true, nullptr);
	//			GetPreviewScene()->GetPreviewMeshComponent()->PreviewInstance->SetDebugSkeletalMeshComponent(SkeletalMeshComponent);
	//		}
	//	}
	//}
	//else
	//{
	//	// Clear the copy-pose component and set us back to 'normal'
	//	GetPreviewScene()->ShowDefaultMode();
	//	GetPreviewScene()->GetPreviewMeshComponent()->PreviewInstance->SetDebugSkeletalMeshComponent(nullptr);
	//}
}

void FCAnimEditor::HandlePreviewMeshChanged(USkeletalMesh* OldPreviewMesh, USkeletalMesh* NewPreviewMesh)
{
	UObject* Object = GetBlueprintObj()->GetObjectBeingDebugged();
	if(Object)
	{
		HandleSetObjectBeingDebugged(Object);
	}
}

void FCAnimEditor::HandleViewportCreated(const TSharedRef<IPersonaViewport>& InPersonaViewport)
{
	auto GetCompilationStateText = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			switch (Blueprint->Status)
			{
			case BS_UpToDate:
			case BS_UpToDateWithWarnings:
				// Fall thru and return empty string
				break;
			case BS_Dirty:
				return LOCTEXT("AnimBP_Dirty", "Preview out of date");
			case BS_Error:
				return LOCTEXT("AnimBP_CompileError", "Compile Error");
			default:
				return LOCTEXT("AnimBP_UnknownStatus", "Unknown Status");
			}
		}

		return FText::GetEmpty();
	};

	auto GetCompilationStateVisibility = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			const bool bUpToDate = (Blueprint->Status == BS_UpToDate) || (Blueprint->Status == BS_UpToDateWithWarnings);
			return bUpToDate ? EVisibility::Collapsed : EVisibility::Visible;
		}

		return EVisibility::Collapsed;
	};

	auto GetCompileButtonVisibility = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			return (Blueprint->Status == BS_Dirty) ? EVisibility::Visible : EVisibility::Collapsed;
		}

		return EVisibility::Collapsed;
	};

	auto CompileBlueprint = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			if (!Blueprint->IsUpToDate())
			{
				Compile();
			}
		}

		return FReply::Handled();
	};

	auto GetErrorSeverity = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			return (Blueprint->Status == BS_Error) ? EMessageSeverity::Error : EMessageSeverity::Warning;
		}

		return EMessageSeverity::Warning;
	};

	auto GetIcon = [this]()
	{
		if (UBlueprint* Blueprint = GetBlueprintObj())
		{
			return (Blueprint->Status == BS_Error) ? FEditorFontGlyphs::Exclamation_Triangle : FEditorFontGlyphs::Eye;
		}

		return FEditorFontGlyphs::Eye;
	};

	InPersonaViewport->AddNotification(MakeAttributeLambda(GetErrorSeverity),
		false,
		SNew(SHorizontalBox)
		.Visibility_Lambda(GetCompilationStateVisibility)
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(4.0f, 4.0f)
		[
			SNew(SHorizontalBox)
			.ToolTipText_Lambda(GetCompilationStateText)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 4.0f, 0.0f)
			[
				SNew(STextBlock)
				.TextStyle(FEditorStyle::Get(), "AnimViewport.MessageText")
				.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
				.Text_Lambda(GetIcon)
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.FillWidth(1.0f)
			[
				SNew(STextBlock)
				.Text_Lambda(GetCompilationStateText)
				.TextStyle(FEditorStyle::Get(), "AnimViewport.MessageText")
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(2.0f, 0.0f)
		[
			SNew(SButton)
			.ForegroundColor(FSlateColor::UseForeground())
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
			.Visibility_Lambda(GetCompileButtonVisibility)
			.ToolTipText(LOCTEXT("AnimBPViewportCompileButtonToolTip", "Compile this Animation Blueprint to update the preview to reflect any recent changes."))
			.OnClicked_Lambda(CompileBlueprint)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 4.0f, 0.0f)
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "AnimViewport.MessageText")
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
					.Text(FEditorFontGlyphs::Cog)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "AnimViewport.MessageText")
					.Text(LOCTEXT("AnimBPViewportCompileButtonLabel", "Compile"))
				]
			]
		]
	);
}

void FCAnimEditor::LoadEditorSettings()
{
	EditorOptions = NewObject<UAnimationBlueprintEditorOptions>();

	if (EditorOptions->bHideUnrelatedNodes)
	{
		ToggleHideUnrelatedNodes();
	}
}

void FCAnimEditor::SaveEditorSettings()
{
	if ( EditorOptions )
	{
		EditorOptions->bHideUnrelatedNodes = bHideUnrelatedNodes;
		EditorOptions->SaveConfig();
	}
}

#pragma region FGCObject Interface

void FCAnimEditor::AddReferencedObjects(FReferenceCollector& Collector) {
	Collector.AddReferencedObject(EditorOptions);
}

#pragma endregion // End FGCObject Interface

#pragma region IToolkit Interface

FText FCAnimEditor::GetBaseToolkitName() const { return LOCTEXT("AppLabel", "CAnim Blueprint Editor"); }

FText FCAnimEditor::GetToolkitToolTipText() const { return FAssetEditorToolkit::GetToolTipTextForObject(GetBlueprintObj()); }

FString FCAnimEditor::GetWorldCentricTabPrefix() const { return LOCTEXT("WorldCentricTabPrefix", "Animation Blueprint Editor ").ToString(); }

FLinearColor FCAnimEditor::GetWorldCentricTabColorScale() const { return FLinearColor(0.5f, 0.25f, 0.35f, 0.5f); }

#pragma endregion // End IToolkit Interface

#pragma region FTickableEditorObject Interface

void FCAnimEditor::Tick(float DeltaTime) {
	FBlueprintEditor::Tick(DeltaTime);

	GetPreviewScene()->InvalidateViews();
}

TStatId FCAnimEditor::GetStatId() const {
	RETURN_QUICK_DECLARE_CYCLE_STAT(FCAnimEditor, STATGROUP_Tickables);
}

#pragma endregion // End FTickableEditorObject Interface

#pragma region FBlueprintEditor Interface

/** Create Default Tabs **/
void FCAnimEditor::CreateDefaultCommands() {
	if (GetBlueprintObj()) {
		FBlueprintEditor::CreateDefaultCommands();

		ToolkitCommands->MapAction(
			FAnimGraphCommands::Get().ToggleHideUnrelatedNodes,
			FExecuteAction::CreateSP(this, &FBlueprintEditor::ToggleHideUnrelatedNodes),
			FCanExecuteAction(),
			FIsActionChecked::CreateSP(this, &FBlueprintEditor::IsToggleHideUnrelatedNodesChecked));
	}
	else {
		ToolkitCommands->MapAction(FGenericCommands::Get().Undo,
			FExecuteAction::CreateSP(this, &FCAnimEditor::UndoAction));
		ToolkitCommands->MapAction(FGenericCommands::Get().Redo,
			FExecuteAction::CreateSP(this, &FCAnimEditor::RedoAction));
	}
}

void FCAnimEditor::OnCreateGraphEditorCommands(TSharedPtr<FUICommandList> GraphEditorCommandsList) {
	GraphEditorCommandsList->MapAction(FAnimGraphCommands::Get().TogglePoseWatch,
		FExecuteAction::CreateSP(this, &FCAnimEditor::OnTogglePoseWatch));
}

void FCAnimEditor::OnAddPosePin()
{
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	if (SelectedNodes.Num() == 1)
	{
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UObject* Node = *NodeIt;

			// TODO COUPLED Old Node Types
			//if (UAnimGraphNode_BlendListByInt* BlendNode = Cast<UAnimGraphNode_BlendListByInt>(Node))
			//{
			//	BlendNode->AddPinToBlendList();
			//	break;
			//}
			//else if (UAnimGraphNode_LayeredBoneBlend* FilterNode = Cast<UAnimGraphNode_LayeredBoneBlend>(Node))
			//{
			//	FilterNode->AddPinToBlendByFilter();
			//	break;
			//}
			//else if (UAnimGraphNode_MultiWayBlend* MultiBlendNode = Cast<UAnimGraphNode_MultiWayBlend>(Node))
			//{
			//	MultiBlendNode->AddPinToBlendNode();
			//	break;
			//}
		}
	}
}

void FCAnimEditor::OnRemovePosePin() {
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	//UAnimGraphNode_BlendListByInt* BlendListIntNode = nullptr;
	//UAnimGraphNode_LayeredBoneBlend* BlendByFilterNode = nullptr;
	//UAnimGraphNode_MultiWayBlend* BlendByMultiway = nullptr;

	if (SelectedNodes.Num() == 1) {
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt) {
			// TODO COUPLED Old Node Types
			//if (UAnimGraphNode_BlendListByInt* BlendNode = Cast<UAnimGraphNode_BlendListByInt>(*NodeIt))
			//{
			//	BlendListIntNode = BlendNode;
			//	break;
			//}
			//else if (UAnimGraphNode_LayeredBoneBlend* LayeredBlendNode = Cast<UAnimGraphNode_LayeredBoneBlend>(*NodeIt))
			//{
			//	BlendByFilterNode = LayeredBlendNode;
			//	break;
			//}		
			//else if (UAnimGraphNode_MultiWayBlend* MultiwayBlendNode = Cast<UAnimGraphNode_MultiWayBlend>(*NodeIt))
			//{
			//	BlendByMultiway = MultiwayBlendNode;
			//	break;
			//}
		}
	}
	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();
	if (FocusedGraphEd.IsValid()) {
		// @fixme: I think we can make blendlistbase to have common functionality
		// and each can implement the common function, but for now, we separate them
		// each implement their menu, so we still can use listbase as the root
		//if (BlendListIntNode) {
		//	// make sure we at least have BlendListNode selected
		//	UEdGraphPin* SelectedPin = FocusedGraphEd->GetGraphPinForMenu();

		//	BlendListIntNode->RemovePinFromBlendList(SelectedPin);

		//	// Update the graph so that the node will be refreshed
		//	FocusedGraphEd->NotifyGraphChanged();
		//}

		//if (BlendByFilterNode) {
		//	// make sure we at least have BlendListNode selected
		//	UEdGraphPin* SelectedPin = FocusedGraphEd->GetGraphPinForMenu();

		//	BlendByFilterNode->RemovePinFromBlendByFilter(SelectedPin);

		//	// Update the graph so that the node will be refreshed
		//	FocusedGraphEd->NotifyGraphChanged();
		//}

		//if (BlendByMultiway) {
		//	// make sure we at least have BlendListNode selected
		//	UEdGraphPin* SelectedPin = FocusedGraphEd->GetGraphPinForMenu();

		//	BlendByMultiway->RemovePinFromBlendNode(SelectedPin);

		//	// Update the graph so that the node will be refreshed
		//	FocusedGraphEd->NotifyGraphChanged();
		//}
	}
}

void FCAnimEditor::Compile()
{
	// TODO COUPLED 
	//// Grab the currently debugged object, so we can re-set it below
	//USkeletalMeshComponent* DebuggedMeshComponent = nullptr;
	//
	//if (UBlueprint* Blueprint = GetBlueprintObj())
	//{
	//	UAnimInstance* CurrentDebugObject = Cast<UAnimInstance>(Blueprint->GetObjectBeingDebugged());
	//	if(CurrentDebugObject)
	//	{
	//		// Force close any asset editors that are using the AnimScriptInstance (such as the Property Matrix), the class will be garbage collected
	//		FAssetEditorManager::Get().CloseOtherEditors(CurrentDebugObject, nullptr);
	//		DebuggedMeshComponent = CurrentDebugObject->GetSkelMeshComponent();
	//	}
	//}

	//// Compile the blueprint
	//FBlueprintEditor::Compile();

	//if (DebuggedMeshComponent != nullptr)
	//{
	//	if (DebuggedMeshComponent->GetAnimInstance() == nullptr)
	//	{
	//		// try reinitialize animation if it doesn't exist
	//		DebuggedMeshComponent->InitAnim(true);
	//	}

	//	UAnimInstance* NewInstance = DebuggedMeshComponent->GetAnimInstance();
	//	UBlueprint* Blueprint = GetBlueprintObj();

	//	if(NewInstance->IsA(Blueprint->GeneratedClass))
	//	{
	//		Blueprint->SetObjectBeingDebugged(NewInstance);
	//	}
	//}

	//// reset the selected skeletal control node
	//SelectedAnimGraphNode.Reset();

	//// if the user manipulated Pin values directly from the node, then should copy updated values to the internal node to retain data consistency
	//OnPostCompile();
}

void FCAnimEditor::OnGraphEditorFocused(const TSharedRef<class SGraphEditor>& InGraphEditor)
{
	// in the future, depending on which graph editor is this will act different
	FBlueprintEditor::OnGraphEditorFocused(InGraphEditor);

	// install callback to allow us to propagate pin default changes live to the preview
	UAnimationGraph* AnimationGraph = Cast<UAnimationGraph>(InGraphEditor->GetCurrentGraph());
	if (AnimationGraph)
	{
		OnPinDefaultValueChangedHandle = AnimationGraph->OnPinDefaultValueChanged.Add(
			FOnPinDefaultValueChanged::FDelegate::CreateSP(this, &FCAnimEditor::HandlePinDefaultValueChanged));
	}

	if (bHideUnrelatedNodes && GetSelectedNodes().Num() <= 0)
	{
		ResetAllNodesUnrelatedStates();
	}
}

void FCAnimEditor::OnGraphEditorBackgrounded(const TSharedRef<SGraphEditor>& InGraphEditor)
{
	FBlueprintEditor::OnGraphEditorBackgrounded(InGraphEditor);

	UAnimationGraph* AnimationGraph = Cast<UAnimationGraph>(InGraphEditor->GetCurrentGraph());
	if (AnimationGraph)
	{
		AnimationGraph->OnPinDefaultValueChanged.Remove(OnPinDefaultValueChangedHandle);
	}
}

void FCAnimEditor::OnConvertToSequenceEvaluator()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_SequencePlayer* OldNode = Cast<UAnimGraphNode_SequencePlayer>(*NodeIter);

	//		// see if sequence player
	//		if ( OldNode && OldNode->Node.Sequence )
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );

	//			// convert to sequence evaluator
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new evaluator
	//			FGraphNodeCreator<UAnimGraphNode_SequenceEvaluator> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_SequenceEvaluator* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.Sequence = OldNode->Node.Sequence;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToSequencePlayer()
{
	// TODO COUPLED
	//FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_SequenceEvaluator* OldNode = Cast<UAnimGraphNode_SequenceEvaluator>(*NodeIter);

	//		// see if sequence player
	//		if ( OldNode && OldNode->Node.Sequence )
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );
	//			// convert to sequence player
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new player
	//			FGraphNodeCreator<UAnimGraphNode_SequencePlayer> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_SequencePlayer* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.Sequence = OldNode->Node.Sequence;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToBlendSpaceEvaluator() 
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_BlendSpacePlayer* OldNode = Cast<UAnimGraphNode_BlendSpacePlayer>(*NodeIter);

	//		// see if sequence player
	//		if ( OldNode && OldNode->Node.BlendSpace )
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );

	//			// convert to sequence evaluator
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new evaluator
	//			FGraphNodeCreator<UAnimGraphNode_BlendSpaceEvaluator> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_BlendSpaceEvaluator* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.BlendSpace = OldNode->Node.BlendSpace;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("X"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("X"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			OldPosePin = OldNode->FindPin(TEXT("Y"));
	//			NewPosePin = NewNode->FindPin(TEXT("Y"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}


	//			OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToBlendSpacePlayer() 
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	//TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_BlendSpaceEvaluator* OldNode = Cast<UAnimGraphNode_BlendSpaceEvaluator>(*NodeIter);

	//		// see if sequence player
	//		if ( OldNode && OldNode->Node.BlendSpace )
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );
	//			// convert to sequence player
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new player
	//			FGraphNodeCreator<UAnimGraphNode_BlendSpacePlayer> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_BlendSpacePlayer* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.BlendSpace = OldNode->Node.BlendSpace;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("X"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("X"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			OldPosePin = OldNode->FindPin(TEXT("Y"));
	//			NewPosePin = NewNode->FindPin(TEXT("Y"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}


	//			OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();
	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToPoseBlender()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_PoseByName* OldNode = Cast<UAnimGraphNode_PoseByName>(*NodeIter);

	//		// see if sequence player
	//		if (OldNode && OldNode->Node.PoseAsset)
	//		{
	//			// convert to sequence player
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new player
	//			FGraphNodeCreator<UAnimGraphNode_PoseBlendNode> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_PoseBlendNode* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.PoseAsset = OldNode->Node.PoseAsset;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToPoseByName()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_PoseBlendNode* OldNode = Cast<UAnimGraphNode_PoseBlendNode>(*NodeIter);

	//		// see if sequence player
	//		if (OldNode && OldNode->Node.PoseAsset)
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );
	//			// convert to sequence player
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new player
	//			FGraphNodeCreator<UAnimGraphNode_PoseByName> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_PoseByName* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.PoseAsset = OldNode->Node.PoseAsset;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToAimOffsetLookAt()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_RotationOffsetBlendSpace* OldNode = Cast<UAnimGraphNode_RotationOffsetBlendSpace>(*NodeIter);

	//		// see if sequence player
	//		if (OldNode && OldNode->Node.BlendSpace)
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );

	//			// convert to sequence evaluator
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new evaluator
	//			FGraphNodeCreator<UAnimGraphNode_AimOffsetLookAt> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_AimOffsetLookAt* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.BlendSpace = OldNode->Node.BlendSpace;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			OldPosePin = OldNode->FindPin(TEXT("BasePose"));
	//			NewPosePin = NewNode->FindPin(TEXT("BasePose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();

	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnConvertToAimOffsetSimple()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	// TODO COUPLED
	//if (SelectedNodes.Num() > 0)
	//{
	//	for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
	//	{
	//		UAnimGraphNode_AimOffsetLookAt* OldNode = Cast<UAnimGraphNode_AimOffsetLookAt>(*NodeIter);

	//		// see if sequence player
	//		if (OldNode && OldNode->Node.BlendSpace)
	//		{
	//			//const FScopedTransaction Transaction( LOCTEXT("ConvertToSequenceEvaluator", "Convert to Single Frame Animation") );
	//			// convert to sequence player
	//			UEdGraph* TargetGraph = OldNode->GetGraph();
	//			// create new player
	//			FGraphNodeCreator<UAnimGraphNode_RotationOffsetBlendSpace> NodeCreator(*TargetGraph);
	//			UAnimGraphNode_RotationOffsetBlendSpace* NewNode = NodeCreator.CreateNode();
	//			NewNode->Node.BlendSpace = OldNode->Node.BlendSpace;
	//			NodeCreator.Finalize();

	//			// get default data from old node to new node
	//			FEdGraphUtilities::CopyCommonState(OldNode, NewNode);

	//			UEdGraphPin* OldPosePin = OldNode->FindPin(TEXT("Pose"));
	//			UEdGraphPin* NewPosePin = NewNode->FindPin(TEXT("Pose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			OldPosePin = OldNode->FindPin(TEXT("BasePose"));
	//			NewPosePin = NewNode->FindPin(TEXT("BasePose"));

	//			if (ensure(OldPosePin && NewPosePin))
	//			{
	//				NewPosePin->MovePersistentDataFromOldPin(*OldPosePin);
	//			}

	//			// remove from selection and from graph
	//			NodeIter.RemoveCurrent();
	//			TargetGraph->RemoveNode(OldNode);

	//			NewNode->Modify();
	//		}
	//	}

	//	// @todo fixme: below code doesn't work
	//	// because of SetAndCenterObject kicks in after new node is added
	//	// will need to disable that first
	//	TSharedPtr<SGraphEditor> FocusedGraphEd = FocusedGraphEdPtr.Pin();
	//	// Update the graph so that the node will be refreshed
	//	FocusedGraphEd->NotifyGraphChanged();
	//	// It's possible to leave invalid objects in the selection set if they get GC'd, so clear it out
	//	FocusedGraphEd->ClearSelectionSet();

	//	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetAnimBlueprint());
	//}
}

void FCAnimEditor::OnOpenRelatedAsset()
{
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	
	EToolkitMode::Type Mode = EToolkitMode::Standalone;
	if (SelectedNodes.Num() > 0)
	{
		for (auto NodeIter = SelectedNodes.CreateIterator(); NodeIter; ++NodeIter)
		{
			if(UCAnimGraphNode_Base* Node = Cast<UCAnimGraphNode_Base>(*NodeIter))
			{
				UAnimationAsset* AnimAsset = Node->GetAnimationAsset();
				if(AnimAsset)
				{
					FAssetEditorManager::Get().OpenEditorForAsset(AnimAsset, Mode);
				}
			}
		}
	}
}

void FCAnimEditor::GetCustomDebugObjects(TArray<FCustomDebugObject>& DebugList) const
{
	UDebugSkelMeshComponent* PreviewMeshComponent = PersonaToolkit->GetPreviewMeshComponent();
	if (PreviewMeshComponent->IsAnimBlueprintInstanced())
	{
		new (DebugList) FCustomDebugObject(PreviewMeshComponent->GetAnimInstance(), LOCTEXT("PreviewObjectLabel", "Preview Instance").ToString());
	}
}

void FCAnimEditor::CreateDefaultTabContents(const TArray<UBlueprint*>& InBlueprints)
{
	FBlueprintEditor::CreateDefaultTabContents(InBlueprints);

	PreviewEditor = SNew(SCAnimBlueprintPreviewPropertyEditor, SharedThis(this));
}

FGraphAppearanceInfo FCAnimEditor::GetGraphAppearance(UEdGraph* InGraph) const
{
	FGraphAppearanceInfo AppearanceInfo = FBlueprintEditor::GetGraphAppearance(InGraph);

	if ( GetBlueprintObj()->IsA(UAnimBlueprint::StaticClass()) )
	{
		AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_Animation", "ANIMATION");
	}

	return AppearanceInfo;
}

bool FCAnimEditor::IsEditable(UEdGraph* InGraph) const
{
	bool bEditable = FBlueprintEditor::IsEditable(InGraph);
	bEditable &= IsGraphInCurrentBlueprint(InGraph);
	return bEditable;
}

FText FCAnimEditor::GetGraphDecorationString(UEdGraph* InGraph) const
{
	if (!IsGraphInCurrentBlueprint(InGraph))
	{
		return LOCTEXT("PersonaExternalGraphDecoration", " Parent Graph Preview");
	}
	return FText::GetEmpty();
}

void FCAnimEditor::OnBlueprintChangedImpl(UBlueprint* InBlueprint, bool bIsJustBeingCompiled /*= false*/)
{
	FBlueprintEditor::OnBlueprintChangedImpl(InBlueprint, bIsJustBeingCompiled);

	// calls PostCompile to copy proper values between anim nodes
	OnPostCompile();
}

#pragma endregion // End FBlueprintEditor Interface

#pragma region FEditorUndoClient Interface

void FCAnimEditor::PostUndo(bool bSuccess) {
	DocumentManager->CleanInvalidTabs();
	DocumentManager->RefreshAllTabs();

	FBlueprintEditor::PostUndo(bSuccess);

	// If we undid a node creation that caused us to clean up a tab/graph we need to refresh the UI state
	RefreshEditors();

	// PostUndo broadcast
	OnPostUndo.Broadcast();

	RefreshPreviewInstanceTrackCurves();

	// clear up preview anim notify states
	// animnotify states are saved in AnimInstance
	// if those are undoed or redoed, they have to be 
	// cleared up, otherwise, they might have invalid data
	ClearupPreviewMeshAnimNotifyStates();

	OnPostCompile();
}

void FCAnimEditor::PostRedo(bool bSuccess) {
	DocumentManager->RefreshAllTabs();

	FBlueprintEditor::PostRedo(bSuccess);

	// PostUndo broadcast, OnPostRedo
	OnPostUndo.Broadcast();

	// clear up preview anim notify states
	// animnotify states are saved in AnimInstance
	// if those are undoed or redoed, they have to be 
	// cleared up, otherwise, they might have invalid data
	ClearupPreviewMeshAnimNotifyStates();

	// calls PostCompile to copy proper values between anim nodes
	OnPostCompile();
}

#pragma endregion // End FEditorUndoClient Interface

#pragma region FNotifyHook Interface

void FCAnimEditor::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, UProperty* PropertyThatChanged) {
	
	FBlueprintEditor::NotifyPostChange(PropertyChangedEvent, PropertyThatChanged);

	// When you change properties on a node, call CopyNodeDataToPreviewNode to allow pushing those to preview instance, for live editing
	UCAnimGraphNode_Base* SelectedNode = SelectedAnimGraphNode.Get();
	if (SelectedNode)
	{
		FCAnimNode_Base* PreviewNode = FindAnimNode(SelectedNode);
		if (PreviewNode)
		{
			SelectedNode->CopyNodeDataToPreviewNode(PreviewNode);
		}
	}
}

#pragma endregion // End FNotifyHook Interface

#undef LOCTEXT_NAMESPACE

