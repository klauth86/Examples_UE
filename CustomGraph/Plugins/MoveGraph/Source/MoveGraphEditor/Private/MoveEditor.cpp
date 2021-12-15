//// Copyright Epic Games, Inc. All Rights Reserved.
//
//#include "MoveEditor.h"
//#include "Framework/MultiBox/MultiBoxBuilder.h"
//#include "EdGraph/EdGraphNode.h"
//#include "Modules/ModuleManager.h"
//#include "EditorStyleSet.h"
//#include "SoundCueGraph/SoundCueGraph.h"
//#include "SoundCueGraph/SoundCueGraphNode.h"
//#include "SoundCueGraph/SoundCueGraphNode_Root.h"
//#include "SoundCueGraph/SoundCueGraphSchema.h"
//#include "Sound/SoundWave.h"
//#include "Sound/DialogueWave.h"
//#include "Components/AudioComponent.h"
//#include "AudioEditorModule.h"
//#include "Sound/SoundNodeWavePlayer.h"
//#include "ScopedTransaction.h"
//#include "GraphEditor.h"
//#include "GraphEditorActions.h"
//#include "Kismet2/BlueprintEditorUtils.h"
//#include "EdGraphUtilities.h"
//#include "SNodePanel.h"
//#include "Editor.h"
//#include "SoundCueGraphEditorCommands.h"
//#include "PropertyEditorModule.h"
//#include "IDetailsView.h"
//#include "Widgets/Docking/SDockTab.h"
//#include "Framework/Commands/GenericCommands.h"
//#include "Sound/SoundNodeDialoguePlayer.h"
//#include "HAL/PlatformApplicationMisc.h"
//#include "AudioDeviceManager.h"
//#include "Audio/AudioDebug.h"
//#include "Move.h"
//
//#define LOCTEXT_NAMESPACE "MoveEditor"
//
//const FName FMoveEditor::GraphCanvasTabId(TEXT("MoveEditor_GraphCanvas"));
//const FName FMoveEditor::PropertiesTabId(TEXT("MoveEditor_Properties"));
//
//FMoveEditor::FMoveEditor()
//	: Move(nullptr)
//{
//}
//
//void FMoveEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
//{
//	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_MoveEditor", "Sound Cue Editor"));
//	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
//
//	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
//
//	InTabManager->RegisterTabSpawner(GraphCanvasTabId, FOnSpawnTab::CreateSP(this, &FMoveEditor::SpawnTab_GraphCanvas))
//		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
//		.SetGroup(WorkspaceMenuCategoryRef)
//		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));
//
//	InTabManager->RegisterTabSpawner(PropertiesTabId, FOnSpawnTab::CreateSP(this, &FMoveEditor::SpawnTab_Properties))
//		.SetDisplayName(LOCTEXT("DetailsTab", "Details"))
//		.SetGroup(WorkspaceMenuCategoryRef)
//		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
//}
//
//void FMoveEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
//{
//	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
//
//	InTabManager->UnregisterTabSpawner(GraphCanvasTabId);
//	InTabManager->UnregisterTabSpawner(PropertiesTabId);
//}
//
//FMoveEditor::~FMoveEditor()
//{
//	GEditor->UnregisterForUndo(this);
//}
//
//void FMoveEditor::InitMoveEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UObject* ObjectToEdit)
//{
//	Move = CastChecked<UMove>(ObjectToEdit);
//
//	// Support undo/redo
//	Move->SetFlags(RF_Transactional);
//
//	GEditor->RegisterForUndo(this);
//
//	FGraphEditorCommands::Register();
//	FSoundCueGraphEditorCommands::Register();
//
//	BindGraphCommands();
//
//	CreateInternalWidgets();
//
//	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_MoveEditor_Layout_v4")
//		->AddArea
//		(
//			FTabManager::NewPrimaryArea()
//			->SetOrientation(Orient_Vertical)
//			->Split
//			(
//				FTabManager::NewStack()
//				->SetSizeCoefficient(0.1f)
//				->SetHideTabWell(true)
//				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
//			)
//			->Split(FTabManager::NewSplitter()
//				->SetOrientation(Orient_Horizontal)
//				->SetSizeCoefficient(0.9f)
//				->Split
//				(
//					FTabManager::NewStack()
//					->SetSizeCoefficient(0.225f)
//					->SetHideTabWell(true)
//					->AddTab(PropertiesTabId, ETabState::OpenedTab)
//				)
//				->Split
//				(
//					FTabManager::NewStack()
//					->SetSizeCoefficient(0.65f)
//					->SetHideTabWell(true)
//					->AddTab(GraphCanvasTabId, ETabState::OpenedTab)
//				)
//			)
//		);
//
//	const bool bCreateDefaultStandaloneMenu = true;
//	const bool bCreateDefaultToolbar = true;
//	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TEXT("MoveEditorApp"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, ObjectToEdit, false);
//
//	IAudioEditorModule* AudioEditorModule = &FModuleManager::LoadModuleChecked<IAudioEditorModule>("AudioEditor");
//	AddMenuExtender(AudioEditorModule->GetSoundCueMenuExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));
//
//	ExtendToolbar();
//	RegenerateMenusAndToolbars();
//
//	// @todo toolkit world centric editing
//	/*if(IsWorldCentricAssetEditor())
//	{
//		SpawnToolkitTab(GetToolbarTabId(), FString(), EToolkitTabSpot::ToolBar);
//		SpawnToolkitTab(GraphCanvasTabId, FString(), EToolkitTabSpot::Viewport);
//		SpawnToolkitTab(PropertiesTabId, FString(), EToolkitTabSpot::Details);
//	}*/
//}
//
//void FMoveEditor::SetSelection(TArray<UObject*> SelectedObjects)
//{
//	if (SoundCueProperties.IsValid())
//	{
//		SoundCueProperties->SetObjects(SelectedObjects);
//	}
//}
//
//bool FMoveEditor::GetBoundsForSelectedNodes(class FSlateRect& Rect, float Padding)
//{
//	return SoundCueGraphEditor->GetBoundsForSelectedNodes(Rect, Padding);
//}
//
//int32 FMoveEditor::GetNumberOfSelectedNodes() const
//{
//	return SoundCueGraphEditor->GetSelectedNodes().Num();
//}
//
//FName FMoveEditor::GetToolkitFName() const
//{
//	return FName("MoveEditor");
//}
//
//FText FMoveEditor::GetBaseToolkitName() const
//{
//	return LOCTEXT("AppLabel", "Move Editor");
//}
//
//FString FMoveEditor::GetWorldCentricTabPrefix() const
//{
//	return LOCTEXT("WorldCentricTabPrefix", "Move ").ToString();
//}
//
//FLinearColor FMoveEditor::GetWorldCentricTabColorScale() const
//{
//	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
//}
//
//TSharedRef<SDockTab> FMoveEditor::SpawnTab_GraphCanvas(const FSpawnTabArgs& Args)
//{
//	check(Args.GetTabId() == GraphCanvasTabId);
//
//	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
//		.Label(LOCTEXT("SoundCueGraphCanvasTitle", "Viewport"));
//
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SpawnedTab->SetContent(SoundCueGraphEditor.ToSharedRef());
//	}
//
//	return SpawnedTab;
//}
//
//TSharedRef<SDockTab> FMoveEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
//{
//	check(Args.GetTabId() == PropertiesTabId);
//
//	return SNew(SDockTab)
//		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
//		.Label(LOCTEXT("SoundCueDetailsTitle", "Details"))
//		[
//			SoundCueProperties.ToSharedRef()
//		];
//}
//
//void FMoveEditor::AddReferencedObjects(FReferenceCollector& Collector)
//{
//	Collector.AddReferencedObject(Move);
//}
//
//void FMoveEditor::PostUndo(bool bSuccess)
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->ClearSelectionSet();
//		SoundCueGraphEditor->NotifyGraphChanged();
//		FSlateApplication::Get().DismissAllMenus();
//	}
//
//}
//
//void FMoveEditor::NotifyPostChange(const FPropertyChangedEvent& PropertyChangedEvent, class FProperty* PropertyThatChanged)
//{
//	if (SoundCueGraphEditor.IsValid() && PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
//	{
//		SoundCueGraphEditor->NotifyGraphChanged();
//	}
//}
//
//void FMoveEditor::CreateInternalWidgets()
//{
//	SoundCueGraphEditor = CreateGraphEditorWidget();
//
//	FDetailsViewArgs Args;
//	Args.bHideSelectionTip = true;
//	Args.NotifyHook = this;
//
//	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
//	SoundCueProperties = PropertyModule.CreateDetailView(Args);
//	SoundCueProperties->SetObject(Move);
//}
//
//void FMoveEditor::ExtendToolbar()
//{
//	struct Local
//	{
//		static void FillToolbar(FToolBarBuilder& ToolbarBuilder)
//		{
//			ToolbarBuilder.BeginSection("Debug");
//			{
//				ToolbarBuilder.AddToolBarButton(FSoundCueGraphEditorCommands::Get().ToggleSolo);
//
//				ToolbarBuilder.AddToolBarButton(FSoundCueGraphEditorCommands::Get().ToggleMute);
//			}
//			ToolbarBuilder.EndSection();
//
//			ToolbarBuilder.BeginSection("Toolbar");
//			{
//				ToolbarBuilder.AddToolBarButton(FSoundCueGraphEditorCommands::Get().PlayCue);
//
//				ToolbarBuilder.AddToolBarButton(FSoundCueGraphEditorCommands::Get().PlayNode);
//
//				ToolbarBuilder.AddToolBarButton(FSoundCueGraphEditorCommands::Get().StopCueNode);
//			}
//			ToolbarBuilder.EndSection();
//		}
//	};
//
//	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
//
//	ToolbarExtender->AddToolBarExtension(
//		"Asset",
//		EExtensionHook::After,
//		GetToolkitCommands(),
//		FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar)
//	);
//
//	AddToolbarExtender(ToolbarExtender);
//
//	IAudioEditorModule* AudioEditorModule = &FModuleManager::LoadModuleChecked<IAudioEditorModule>("AudioEditor");
//	AddToolbarExtender(AudioEditorModule->GetSoundCueToolBarExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));
//}
//
//void FMoveEditor::BindGraphCommands()
//{
//	const FSoundCueGraphEditorCommands& Commands = FSoundCueGraphEditorCommands::Get();
//
//	ToolkitCommands->MapAction(
//		FGenericCommands::Get().Undo,
//		FExecuteAction::CreateSP(this, &FMoveEditor::UndoGraphAction));
//
//	ToolkitCommands->MapAction(
//		FGenericCommands::Get().Redo,
//		FExecuteAction::CreateSP(this, &FMoveEditor::RedoGraphAction));
//}
//
//void FMoveEditor::SyncInBrowser()
//{
//	TArray<UObject*> ObjectsToSync;
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
//	{
//		USoundCueGraphNode* SelectedNode = Cast<USoundCueGraphNode>(*NodeIt);
//
//		if (SelectedNode)
//		{
//			USoundNodeWavePlayer* SelectedWave = Cast<USoundNodeWavePlayer>(SelectedNode->SoundNode);
//			if (SelectedWave && SelectedWave->GetSoundWave())
//			{
//				ObjectsToSync.AddUnique(SelectedWave->GetSoundWave());
//			}
//
//			USoundNodeDialoguePlayer* SelectedDialogue = Cast<USoundNodeDialoguePlayer>(SelectedNode->SoundNode);
//			if (SelectedDialogue && SelectedDialogue->GetDialogueWave())
//			{
//				ObjectsToSync.AddUnique(SelectedDialogue->GetDialogueWave());
//			}
//		}
//	}
//
//	if (ObjectsToSync.Num() > 0)
//	{
//		GEditor->SyncBrowserToObjects(ObjectsToSync);
//	}
//}
//
//bool FMoveEditor::CanSyncInBrowser() const
//{
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
//	{
//		USoundCueGraphNode* SelectedNode = Cast<USoundCueGraphNode>(*NodeIt);
//
//		if (SelectedNode)
//		{
//			USoundNodeWavePlayer* WavePlayer = Cast<USoundNodeWavePlayer>(SelectedNode->SoundNode);
//
//			if (WavePlayer && WavePlayer->GetSoundWave())
//			{
//				return true;
//			}
//
//			USoundNodeDialoguePlayer* SelectedDialogue = Cast<USoundNodeDialoguePlayer>(SelectedNode->SoundNode);
//			if (SelectedDialogue && SelectedDialogue->GetDialogueWave())
//			{
//				return true;
//			}
//		}
//	}
//	return false;
//}
//
//void FMoveEditor::AddInput()
//{
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	// Iterator used but should only contain one node
//	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
//	{
//		USoundCueGraphNode* SelectedNode = Cast<USoundCueGraphNode>(*NodeIt);
//
//		if (SelectedNode)
//		{
//			SelectedNode->AddInputPin();
//			break;
//		}
//	}
//}
//
//bool FMoveEditor::CanAddInput() const
//{
//	return GetSelectedNodes().Num() == 1;
//}
//
//void FMoveEditor::DeleteInput()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		UEdGraphPin* SelectedPin = SoundCueGraphEditor->GetGraphPinForMenu();
//		if (ensure(SelectedPin))
//		{
//			USoundCueGraphNode* SelectedNode = Cast<USoundCueGraphNode>(SelectedPin->GetOwningNode());
//
//			if (SelectedNode && SelectedNode == SelectedPin->GetOwningNode())
//			{
//				SelectedNode->RemoveInputPin(SelectedPin);
//			}
//		}
//	}
//}
//
//bool FMoveEditor::CanDeleteInput() const
//{
//	return true;
//}
//
//void FMoveEditor::OnCreateComment()
//{
//	FSoundCueGraphSchemaAction_NewComment CommentAction;
//	CommentAction.PerformAction(Move->MoveGraph, NULL, SoundCueGraphEditor->GetPasteLocation());
//}
//
//TSharedRef<SGraphEditor> FMoveEditor::CreateGraphEditorWidget()
//{
//	if (!GraphEditorCommands.IsValid())
//	{
//		GraphEditorCommands = MakeShareable(new FUICommandList);
//
//		GraphEditorCommands->MapAction(FSoundCueGraphEditorCommands::Get().BrowserSync,
//			FExecuteAction::CreateSP(this, &FMoveEditor::SyncInBrowser),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanSyncInBrowser));
//
//		GraphEditorCommands->MapAction(FSoundCueGraphEditorCommands::Get().AddInput,
//			FExecuteAction::CreateSP(this, &FMoveEditor::AddInput),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanAddInput));
//
//		GraphEditorCommands->MapAction(FSoundCueGraphEditorCommands::Get().DeleteInput,
//			FExecuteAction::CreateSP(this, &FMoveEditor::DeleteInput),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanDeleteInput));
//
//		// Graph Editor Commands
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().CreateComment,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnCreateComment)
//		);
//
//		// Editing commands
//		GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
//			FExecuteAction::CreateSP(this, &FMoveEditor::SelectAllNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanSelectAllNodes)
//		);
//
//		GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
//			FExecuteAction::CreateSP(this, &FMoveEditor::DeleteSelectedNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanDeleteNodes)
//		);
//
//		GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
//			FExecuteAction::CreateSP(this, &FMoveEditor::CopySelectedNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanCopyNodes)
//		);
//
//		GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
//			FExecuteAction::CreateSP(this, &FMoveEditor::CutSelectedNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanCutNodes)
//		);
//
//		GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
//			FExecuteAction::CreateSP(this, &FMoveEditor::PasteNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanPasteNodes)
//		);
//
//		GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
//			FExecuteAction::CreateSP(this, &FMoveEditor::DuplicateNodes),
//			FCanExecuteAction::CreateSP(this, &FMoveEditor::CanDuplicateNodes)
//		);
//
//		// Alignment Commands
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesTop,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignTop)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesMiddle,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignMiddle)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesBottom,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignBottom)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesLeft,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignLeft)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesCenter,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignCenter)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().AlignNodesRight,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnAlignRight)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().StraightenConnections,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnStraightenConnections)
//		);
//
//		// Distribution Commands
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().DistributeNodesHorizontally,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnDistributeNodesH)
//		);
//
//		GraphEditorCommands->MapAction(FGraphEditorCommands::Get().DistributeNodesVertically,
//			FExecuteAction::CreateSP(this, &FMoveEditor::OnDistributeNodesV)
//		);
//	}
//
//	FGraphAppearanceInfo AppearanceInfo;
//	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_SoundCue", "SOUND CUE");
//
//	SGraphEditor::FGraphEditorEvents InEvents;
//	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FMoveEditor::OnSelectedNodesChanged);
//	InEvents.OnTextCommitted = FOnNodeTextCommitted::CreateSP(this, &FMoveEditor::OnNodeTitleCommitted);
//
//	return SNew(SGraphEditor)
//		.AdditionalCommands(GraphEditorCommands)
//		.IsEditable(true)
//		.Appearance(AppearanceInfo)
//		.GraphToEdit(Move->MoveGraph)
//		.GraphEvents(InEvents)
//		.AutoExpandActionMenu(true)
//		.ShowGraphStateOverlay(false);
//}
//
//FGraphPanelSelectionSet FMoveEditor::GetSelectedNodes() const
//{
//	FGraphPanelSelectionSet CurrentSelection;
//	if (SoundCueGraphEditor.IsValid())
//	{
//		CurrentSelection = SoundCueGraphEditor->GetSelectedNodes();
//	}
//	return CurrentSelection;
//}
//
//void FMoveEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
//{
//	TArray<UObject*> Selection;
//
//	if (NewSelection.Num())
//	{
//		for (TSet<class UObject*>::TConstIterator SetIt(NewSelection); SetIt; ++SetIt)
//		{
//			if (Cast<USoundCueGraphNode_Root>(*SetIt))
//			{
//				Selection.Add(GetMove());
//			}
//			else if (USoundCueGraphNode* GraphNode = Cast<USoundCueGraphNode>(*SetIt))
//			{
//				Selection.Add(GraphNode->SoundNode);
//			}
//			else
//			{
//				Selection.Add(*SetIt);
//			}
//		}
//		//Selection = NewSelection.Array();
//	}
//	else
//	{
//		Selection.Add(GetMove());
//	}
//
//	SetSelection(Selection);
//}
//
//void FMoveEditor::OnNodeTitleCommitted(const FText& NewText, ETextCommit::Type CommitInfo, UEdGraphNode* NodeBeingChanged)
//{
//	if (NodeBeingChanged)
//	{
//		const FScopedTransaction Transaction(LOCTEXT("RenameNode", "Rename Node"));
//		NodeBeingChanged->Modify();
//		NodeBeingChanged->OnRenameNode(NewText.ToString());
//	}
//}
//
//void FMoveEditor::SelectAllNodes()
//{
//	SoundCueGraphEditor->SelectAllNodes();
//}
//
//bool FMoveEditor::CanSelectAllNodes() const
//{
//	return true;
//}
//
//void FMoveEditor::DeleteSelectedNodes()
//{
//	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "MoveEditorDeleteSelectedNode", "Delete Selected Sound Cue Node"));
//
//	SoundCueGraphEditor->GetCurrentGraph()->Modify();
//
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	SoundCueGraphEditor->ClearSelectionSet();
//
//	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
//	{
//		UEdGraphNode* Node = CastChecked<UEdGraphNode>(*NodeIt);
//
//		if (Node->CanUserDeleteNode())
//		{
//			if (UMoveGraphNode* SoundGraphNode = Cast<UMoveGraphNode>(Node))
//			{
//				UMoveNode* DelNode = SoundGraphNode->MoveNode;
//
//				FBlueprintEditorUtils::RemoveNode(NULL, SoundGraphNode, true);
//
//				// Remove this node from the SoundCue's list of all SoundNodes
//				Move->AllNodes.Remove(DelNode);
//				Move->MarkPackageDirty();
//			}
//			else
//			{
//				FBlueprintEditorUtils::RemoveNode(NULL, Node, true);
//			}
//		}
//	}
//}
//
//bool FMoveEditor::CanDeleteNodes() const
//{
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	if (SelectedNodes.Num() == 1)
//	{
//		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
//		{
//			if (Cast<USoundCueGraphNode_Root>(*NodeIt))
//			{
//				// Return false if only root node is selected, as it can't be deleted
//				return false;
//			}
//		}
//	}
//
//	return SelectedNodes.Num() > 0;
//}
//
//void FMoveEditor::DeleteSelectedDuplicatableNodes()
//{
//	// Cache off the old selection
//	const FGraphPanelSelectionSet OldSelectedNodes = GetSelectedNodes();
//
//	// Clear the selection and only select the nodes that can be duplicated
//	FGraphPanelSelectionSet RemainingNodes;
//	SoundCueGraphEditor->ClearSelectionSet();
//
//	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
//	{
//		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
//		if ((Node != NULL) && Node->CanDuplicateNode())
//		{
//			SoundCueGraphEditor->SetNodeSelection(Node, true);
//		}
//		else
//		{
//			RemainingNodes.Add(Node);
//		}
//	}
//
//	// Delete the duplicatable nodes
//	DeleteSelectedNodes();
//
//	// Reselect whatever's left from the original selection after the deletion
//	SoundCueGraphEditor->ClearSelectionSet();
//
//	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(RemainingNodes); SelectedIter; ++SelectedIter)
//	{
//		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
//		{
//			SoundCueGraphEditor->SetNodeSelection(Node, true);
//		}
//	}
//}
//
//void FMoveEditor::CutSelectedNodes()
//{
//	CopySelectedNodes();
//	// Cut should only delete nodes that can be duplicated
//	DeleteSelectedDuplicatableNodes();
//}
//
//bool FMoveEditor::CanCutNodes() const
//{
//	return CanCopyNodes() && CanDeleteNodes();
//}
//
//void FMoveEditor::CopySelectedNodes()
//{
//	// Export the selected nodes and place the text on the clipboard
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//
//	FString ExportedText;
//
//	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
//	{
//		if (USoundCueGraphNode* Node = Cast<USoundCueGraphNode>(*SelectedIter))
//		{
//			Node->PrepareForCopying();
//		}
//	}
//
//	FEdGraphUtilities::ExportNodesToText(SelectedNodes, /*out*/ ExportedText);
//	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
//
//	// Make sure SoundCue remains the owner of the copied nodes
//	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
//	{
//		if (USoundCueGraphNode* Node = Cast<USoundCueGraphNode>(*SelectedIter))
//		{
//			Node->PostCopyNode();
//		}
//	}
//}
//
//bool FMoveEditor::CanCopyNodes() const
//{
//	// If any of the nodes can be duplicated then we should allow copying
//	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
//	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
//	{
//		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
//		if ((Node != NULL) && Node->CanDuplicateNode())
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//void FMoveEditor::PasteNodes()
//{
//	PasteNodesHere(SoundCueGraphEditor->GetPasteLocation());
//}
//
//void FMoveEditor::PasteNodesHere(const FVector2D& Location)
//{
//	// Undo/Redo support
//	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "MoveEditorPaste", "Paste Sound Cue Node"));
//	Move->MoveGraph->Modify();
//	Move->Modify();
//
//	// Clear the selection set (newly pasted stuff will be selected)
//	SoundCueGraphEditor->ClearSelectionSet();
//
//	// Grab the text to paste from the clipboard.
//	FString TextToImport;
//	FPlatformApplicationMisc::ClipboardPaste(TextToImport);
//
//	// Import the nodes
//	TSet<UEdGraphNode*> PastedNodes;
//	FEdGraphUtilities::ImportNodesFromText(Move->MoveGraph, TextToImport, /*out*/ PastedNodes);
//
//	//Average position of nodes so we can move them while still maintaining relative distances to each other
//	FVector2D AvgNodePosition(0.0f, 0.0f);
//
//	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
//	{
//		UEdGraphNode* Node = *It;
//		AvgNodePosition.X += Node->NodePosX;
//		AvgNodePosition.Y += Node->NodePosY;
//	}
//
//	if (PastedNodes.Num() > 0)
//	{
//		float InvNumNodes = 1.0f / float(PastedNodes.Num());
//		AvgNodePosition.X *= InvNumNodes;
//		AvgNodePosition.Y *= InvNumNodes;
//	}
//
//	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
//	{
//		UEdGraphNode* Node = *It;
//
//		if (UMoveGraphNode* SoundGraphNode = Cast<UMoveGraphNode>(Node))
//		{
//			Move->AllNodes.Add(SoundGraphNode->MoveNode);
//		}
//
//		// Select the newly pasted stuff
//		SoundCueGraphEditor->SetNodeSelection(Node, true);
//
//		Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
//		Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;
//
//		Node->SnapToGrid(SNodePanel::GetSnapGridSize());
//
//		// Give new node a different Guid from the old one
//		Node->CreateNewGuid();
//	}
//
//	// Update UI
//	SoundCueGraphEditor->NotifyGraphChanged();
//
//	Move->PostEditChange();
//	Move->MarkPackageDirty();
//}
//
//bool FMoveEditor::CanPasteNodes() const
//{
//	FString ClipboardContent;
//	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);
//
//	return FEdGraphUtilities::CanImportNodesFromText(Move->MoveGraph, ClipboardContent);
//}
//
//void FMoveEditor::DuplicateNodes()
//{
//	// Copy and paste current selection
//	CopySelectedNodes();
//	PasteNodes();
//}
//
//bool FMoveEditor::CanDuplicateNodes() const
//{
//	return CanCopyNodes();
//}
//
//void FMoveEditor::UndoGraphAction()
//{
//	GEditor->UndoTransaction();
//}
//
//void FMoveEditor::RedoGraphAction()
//{
//	// Clear selection, to avoid holding refs to nodes that go away
//	SoundCueGraphEditor->ClearSelectionSet();
//
//	GEditor->RedoTransaction();
//}
//
//void FMoveEditor::OnAlignTop()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignTop();
//	}
//}
//
//void FMoveEditor::OnAlignMiddle()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignMiddle();
//	}
//}
//
//void FMoveEditor::OnAlignBottom()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignBottom();
//	}
//}
//
//void FMoveEditor::OnAlignLeft()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignLeft();
//	}
//}
//
//void FMoveEditor::OnAlignCenter()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignCenter();
//	}
//}
//
//void FMoveEditor::OnAlignRight()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnAlignRight();
//	}
//}
//
//void FMoveEditor::OnStraightenConnections()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnStraightenConnections();
//	}
//}
//
//void FMoveEditor::OnDistributeNodesH()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnDistributeNodesH();
//	}
//}
//
//void FMoveEditor::OnDistributeNodesV()
//{
//	if (SoundCueGraphEditor.IsValid())
//	{
//		SoundCueGraphEditor->OnDistributeNodesV();
//	}
//}
//
//#undef LOCTEXT_NAMESPACE
