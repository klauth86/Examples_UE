#include "MSeqEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "Editor/EditorEngine.h"
#include "EngineGlobals.h"
#include "MoveSequence.h"
#include "Graph/MSeqGraph.h"
#include "Graph/MSeqGraphNode.h"
#include "ScopedTransaction.h"
#include "EdGraphUtilities.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "HAL/PlatformApplicationMisc.h"
#include "EdGraph/EdGraphSchema.h"

#define LOCTEXT_NAMESPACE "MSeqGraph"

const FName FMSeqEditor::EQSUpdateGraphTabId(TEXT("EnvironmentQueryEditor_UpdateGraph"));
const FName FMSeqEditor::EQSPropertiesTabId(TEXT("EnvironmentQueryEditor_Properties"));
const FName FMSeqEditor::EQSProfilerTabId(TEXT("EnvironmentQueryEditor_Profiler"));

void FMSeqEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_EnvironmentQueryEditor", "Environment Query Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(EQSUpdateGraphTabId, FOnSpawnTab::CreateSP(this, &FMSeqEditor::SpawnTab_UpdateGraph))
		.SetDisplayName(NSLOCTEXT("EnvironmentQueryEditor", "Graph", "Graph"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(EQSPropertiesTabId, FOnSpawnTab::CreateSP(this, &FMSeqEditor::SpawnTab_Properties))
		.SetDisplayName(NSLOCTEXT("EnvironmentQueryEditor", "PropertiesTab", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(EQSProfilerTabId, FOnSpawnTab::CreateSP(this, &FMSeqEditor::SpawnTab_Profiler))
		.SetDisplayName(NSLOCTEXT("EnvironmentQueryEditor", "ProfilerTab", "Profiler"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "Profiler.EventGraph.ExpandHotPath"));
}

void FMSeqEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(EQSPropertiesTabId);
	InTabManager->UnregisterTabSpawner(EQSUpdateGraphTabId);
	InTabManager->UnregisterTabSpawner(EQSProfilerTabId);
}

const FName EnvironmentQueryEditorAppIdentifier(TEXT("EnvironmentQueryEditorApp"));

void FMSeqEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UMoveSequence* mSeq)
{
	MoveSequence = mSeq;
	check(MoveSequence != NULL);

	TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_EnvironmentQuery_Layout");
	//////->AddArea
	//////(
	//////		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
	//////		->Split
	//////		(
	//////			FTabManager::NewStack()
	//////			->SetSizeCoefficient(0.1f)
	//////			->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
	//////			->SetHideTabWell(true)
	//////		)
	//////		->Split
	//////		(
	//////			FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
	//////			->Split
	//////			(
	//////				FTabManager::NewStack()
	//////				->SetSizeCoefficient(0.7f)
	//////				->AddTab(EQSUpdateGraphTabId, ETabState::OpenedTab)
	//////			)
	//////			->Split
	//////			(
	//////				FTabManager::NewStack()
	//////				->SetSizeCoefficient(0.3f)
	//////				->AddTab(EQSPropertiesTabId, ETabState::OpenedTab)
	//////				->AddTab(EQSProfilerTabId, ETabState::OpenedTab)
	//////				->SetForegroundTab(EQSPropertiesTabId)
	//////			)
	//////		)
	//////)

	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, EnvironmentQueryEditorAppIdentifier, StandaloneDefaultLayout, true, true, MoveSequence);

	//////FEnvironmentQueryEditorModule& EnvironmentQueryEditorModule = FModuleManager::LoadModuleChecked<FEnvironmentQueryEditorModule>("EnvironmentQueryEditor");
	//////AddMenuExtender(EnvironmentQueryEditorModule.GetMenuExtensibilityManager()->GetAllExtenders(GetToolkitCommands(), GetEditingObjects()));

	//////BindCommands();
	//////ExtendToolbar();
	//////RegenerateMenusAndToolbars();

	//////// Update BT asset data based on saved graph to have correct data in editor
	//////TSharedPtr<SGraphEditor> UpdateGraphEditor = UpdateGraphEdPtr.Pin();
	//////if (UpdateGraphEditor.IsValid() && UpdateGraphEditor->GetCurrentGraph() != NULL)
	//////{
	//////	//let's find root node
	//////	UEnvironmentQueryGraph* EQSGraph = Cast<UEnvironmentQueryGraph>(UpdateGraphEditor->GetCurrentGraph());
	//////	EQSGraph->UpdateAsset();
	//////}
}

FName FMSeqEditor::GetToolkitFName() const
{
	return FName("Move Sequence");
}

FText FMSeqEditor::GetBaseToolkitName() const
{
	return NSLOCTEXT("MoveSequenceEditor", "AppLabel", "MoveSequence");
}

FString FMSeqEditor::GetWorldCentricTabPrefix() const
{
	return NSLOCTEXT("MoveSequenceEditor", "WorldCentricTabPrefix", "MoveSequence ").ToString();
}

FLinearColor FMSeqEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.0f, 0.0f, 0.2f, 0.5f);
}

void FMSeqEditor::SaveAsset_Execute()
{
	// modify BT asset
	TSharedPtr<SGraphEditor> UpdateGraphEditor = UpdateGraphEdPtr.Pin();
	if (UpdateGraphEditor.IsValid() && UpdateGraphEditor->GetCurrentGraph() != NULL)
	{
		//let's find root node
		UMSeqGraph* EdGraph = Cast<UMSeqGraph>(UpdateGraphEditor->GetCurrentGraph());
		//////EdGraph->UpdateAsset(); //////TODO
	}
	// save it
	////// TODO IEnvironmentQueryEditor::SaveAsset_Execute();
}

TSharedRef<SGraphEditor> FMSeqEditor::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	check(InGraph != NULL);

	// Create the appearance info
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = NSLOCTEXT("EnvironmentQueryEditor", "AppearanceCornerText", "ENVIRONMENT QUERY");

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FMSeqEditor::OnSelectedNodesChanged);

	CreateCommandList();

	// Make title bar
	TSharedRef<SWidget> TitleBarWidget =
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.FillWidth(1.f)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("EnvironmentQueryEditor", "TheQueryGraphLabel", "Query Graph"))
		.TextStyle(FEditorStyle::Get(), TEXT("GraphBreadcrumbButtonText"))
		]
		];

	// Make full graph editor
	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.GraphEvents(InEvents);
}

void FMSeqEditor::CreateInternalWidgets()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	const FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::ObjectsUseNameArea, false);
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(NULL);
}

TSharedRef<SDockTab> FMSeqEditor::SpawnTab_UpdateGraph(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId().TabType == EQSUpdateGraphTabId);

	//////UMSeqGraph* MyGraph = Cast<UMSeqGraph>(Query->EdGraph);
	//////if (Query->EdGraph == NULL)
	//////{
	//////	MyGraph = NewObject<UEnvironmentQueryGraph>(Query, NAME_None, RF_Transactional);
	//////	Query->EdGraph = MyGraph;

	//////	// let's read data from BT script and generate nodes
	//////	const UEdGraphSchema* Schema = Query->EdGraph->GetSchema();
	//////	Schema->CreateDefaultNodesForGraph(*Query->EdGraph);

	//////	MyGraph->OnCreated();
	//////}
	//////else
	//////{
	//////	MyGraph->OnLoaded();
	//////}

	//////MyGraph->Initialize();

	//////TSharedRef<SGraphEditor> UpdateGraphEditor = CreateGraphEditorWidget(Query->EdGraph);
	//////UpdateGraphEdPtr = UpdateGraphEditor; // Keep pointer to editor

	return SNew(SDockTab)
		.Label(NSLOCTEXT("EnvironmentQueryEditor", "UpdateGraph", "Update Graph"))
		.TabColorScale(GetTabColorScale())
		//////[
		//////	UpdateGraphEditor
		//////]
		;
}

TSharedRef<SDockTab> FMSeqEditor::SpawnTab_Properties(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == EQSPropertiesTabId);

	CreateInternalWidgets();

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("SoundClassEditor.Tabs.Properties"))
		.Label(NSLOCTEXT("EnvironmentQueryEditor", "PropertiesTab", "Details"))
		[
			DetailsView.ToSharedRef()
		];

	return SpawnedTab;
}

TSharedRef<SDockTab> FMSeqEditor::SpawnTab_Profiler(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == EQSProfilerTabId);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("SoundClassEditor.Tabs.Properties"))
		.Label(NSLOCTEXT("EnvironmentQueryEditor", "ProfilerTab", "Profiler"))
		//[
		//	ProfilerView.ToSharedRef()
		//]
	;

	return SpawnedTab;
}

FMSeqEditor::FMSeqEditor()
{
	UEditorEngine* Editor = (UEditorEngine*)GEngine;
	if (Editor != NULL)
	{
		Editor->RegisterForUndo(this);
	}
}

FMSeqEditor::~FMSeqEditor()
{
	UEditorEngine* Editor = (UEditorEngine*)GEngine;
	if (Editor)
	{
		Editor->UnregisterForUndo(this);
	}
}

void FMSeqEditor::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanDuplicateNodes)
	);

	GraphEditorCommands->MapAction(
		FGraphEditorCommands::Get().CreateComment,
		FExecuteAction::CreateRaw(this, &FMSeqEditor::OnCreateComment),
		FCanExecuteAction::CreateRaw(this, &FMSeqEditor::CanCreateComment)
	);
}

FGraphPanelSelectionSet FMSeqEditor::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	if (TSharedPtr<SGraphEditor> FocusedGraphEd = UpdateGraphEdPtr.Pin())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FMSeqEditor::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	if (NewSelection.Num())
	{
		for (TSet<class UObject*>::TConstIterator SetIt(NewSelection); SetIt; ++SetIt)
		{
			UMSeqGraphNode* GraphNode = Cast<UMSeqGraphNode>(*SetIt);
			if (GraphNode)
			{
				////// TODO Selection.Add(GraphNode->NodeInstance);
			}
		}
	}

	return Selection.Num() == 1 ? DetailsView->SetObjects(Selection) : DetailsView->SetObject(NULL);
}

void FMSeqEditor::PostUndo(bool bSuccess)
{
	if (bSuccess)
	{
		// Clear selection, to avoid holding refs to nodes that go away
		if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
		{
			CurrentGraphEditor->ClearSelectionSet();
			CurrentGraphEditor->NotifyGraphChanged();
		}
		FSlateApplication::Get().DismissAllMenus();
	}
}

void FMSeqEditor::PostRedo(bool bSuccess)
{
	if (bSuccess)
	{
		// Clear selection, to avoid holding refs to nodes that go away
		if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
		{
			CurrentGraphEditor->ClearSelectionSet();
			CurrentGraphEditor->NotifyGraphChanged();
		}
		FSlateApplication::Get().DismissAllMenus();
	}
}

void FMSeqEditor::SelectAllNodes()
{
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FMSeqEditor::CanSelectAllNodes() const
{
	return true;
}

void FMSeqEditor::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}

bool FMSeqEditor::CanDeleteNodes() const
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FMSeqEditor::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FMSeqEditor::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FMSeqEditor::CanCutNodes() const
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FMSeqEditor::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	TArray<UMSeqGraphNode*> SubNodes;

	FString ExportedText;

	int32 CopySubNodeIndex = 0;
	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		UMSeqGraphNode* mSeqNode = Cast<UMSeqGraphNode>(Node);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		Node->PrepareForCopying();
	}

	for (int32 Idx = 0; Idx < SubNodes.Num(); Idx++)
	{
		SelectedNodes.Add(SubNodes[Idx]);
		SubNodes[Idx]->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FMSeqEditor::CanCopyNodes() const
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FMSeqEditor::PasteNodes()
{
	if (TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FMSeqEditor::PasteNodesHere(const FVector2D& Location)
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	// Undo/Redo support
	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();
	UMSeqGraph* mSeqGraph = Cast<UMSeqGraph>(EdGraph);

	EdGraph->Modify();
	if (mSeqGraph)
	{
		mSeqGraph->LockUpdates();
	}

	// Clear the selection set (newly pasted stuff will be selected)
	CurrentGraphEditor->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, /*out*/ PastedNodes);

	//Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);

	// Number of nodes used to calculate AvgNodePosition
	int32 AvgCount = 0;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* EdNode = *It;
		UMSeqGraphNode* AINode = Cast<UMSeqGraphNode>(EdNode);
		if (EdNode)
		{
			AvgNodePosition.X += EdNode->NodePosX;
			AvgNodePosition.Y += EdNode->NodePosY;
			++AvgCount;
		}
	}

	if (AvgCount > 0)
	{
		float InvNumNodes = 1.0f / float(AvgCount);
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	bool bPastedParentNode = false;

	TMap<FGuid/*New*/, FGuid/*Old*/> NewToOldNodeMapping;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UEdGraphNode* PasteNode = *It;
		UMSeqGraphNode* PasteAINode = Cast<UMSeqGraphNode>(PasteNode);

		if (PasteNode)
		{
			bPastedParentNode = true;

			// Select the newly pasted stuff
			CurrentGraphEditor->SetNodeSelection(PasteNode, true);

			PasteNode->NodePosX = (PasteNode->NodePosX - AvgNodePosition.X) + Location.X;
			PasteNode->NodePosY = (PasteNode->NodePosY - AvgNodePosition.Y) + Location.Y;

			PasteNode->SnapToGrid(16);

			const FGuid OldGuid = PasteNode->NodeGuid;

			// Give new node a different Guid from the old one
			PasteNode->CreateNewGuid();

			const FGuid NewGuid = PasteNode->NodeGuid;

			NewToOldNodeMapping.Add(NewGuid, OldGuid);
		}
	}

	FixupPastedNodes(PastedNodes, NewToOldNodeMapping);

	if (mSeqGraph)
	{
		//mSeqGraph->UpdateClassData(); ////// TODO
		//mSeqGraph->OnNodesPasted(TextToImport);
		mSeqGraph->UnlockUpdates();
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

void FMSeqEditor::FixupPastedNodes(const TSet<UEdGraphNode*>& PastedGraphNodes, const TMap<FGuid/*New*/, FGuid/*Old*/>& NewToOldNodeMapping)
{

}

bool FMSeqEditor::CanPasteNodes() const
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FMSeqEditor::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FMSeqEditor::CanDuplicateNodes() const
{
	return CanCopyNodes();
}

bool FMSeqEditor::CanCreateComment() const
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	return CurrentGraphEditor.IsValid() ? (CurrentGraphEditor->GetNumberOfSelectedNodes() != 0) : false;
}

void FMSeqEditor::OnCreateComment()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = UpdateGraphEdPtr.Pin();
	if (UEdGraph* EdGraph = CurrentGraphEditor.IsValid() ? CurrentGraphEditor->GetCurrentGraph() : nullptr)
	{
		TSharedPtr<FEdGraphSchemaAction> Action = EdGraph->GetSchema()->GetCreateCommentAction();
		if (Action.IsValid())
		{
			Action->PerformAction(EdGraph, nullptr, FVector2D());
		}
	}
}

#undef LOCTEXT_NAMESPACE
