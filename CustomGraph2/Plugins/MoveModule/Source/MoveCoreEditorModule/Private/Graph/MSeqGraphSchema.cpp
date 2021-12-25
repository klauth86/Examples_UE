// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphSchema.h"
#include "Graph/MSeqGraphNode_Root.h"
#include "Graph/MSeqGraphNode_Regular.h"
#include "Graph/MSeqGraph.h"
#include "FightAction.h"
#include "MoveSequence.h"

#define LOCTEXT_NAMESPACE "MSeqGraphSchema"

void UMSeqGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UMSeqGraphNode_Root> NodeCreator(Graph);
	UMSeqGraphNode_Root* MyNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);

	if (UMoveSequence* moveSequence = Graph.GetTypedOuter<UMoveSequence>())
	{
		moveSequence->GetActionsGraph().Emplace(Graph.Nodes.IndexOfByKey(MyNode));
	}
}

const FPinConnectionResponse UMSeqGraphSchema::CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const
{
	// Make sure the pins are not on the same node
	if (PinA->GetOwningNode() == PinB->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Both are on the same node"));
	}

	if ((PinA->Direction == EGPD_Input && PinA->LinkedTo.Num() > 0) ||
		(PinB->Direction == EGPD_Input && PinB->LinkedTo.Num() > 0))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	// Compare the directions
	bool bDirectionsOK = false;

	if ((PinA->Direction == EGPD_Input) && (PinB->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}
	else if ((PinB->Direction == EGPD_Input) && (PinA->Direction == EGPD_Output))
	{
		bDirectionsOK = true;
	}

	if (bDirectionsOK)
	{
		if ((PinA->Direction == EGPD_Input && PinA->LinkedTo.Num() > 0) || (PinB->Direction == EGPD_Input && PinB->LinkedTo.Num() > 0))
		{
			return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Already connected with other"));
		}
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));
	}

	if (UMSeqGraphNode* graphNode = Cast<UMSeqGraphNode>(PinA->GetOwningNode())) {
		graphNode->AddTransition(graphNode->GetGraph()->Nodes.IndexOfByKey(PinB->GetOwningNode()));
	}

	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}

void UMSeqGraphSchema::DroppedAssetsOnGraph(const TArray<struct FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const {
	UMSeqGraph* MSeqGraph = CastChecked<UMSeqGraph>(Graph);

	TArray<UFightAction*> fightActions;
	for (int32 AssetIdx = 0; AssetIdx < Assets.Num(); ++AssetIdx)
	{
		if (UFightAction* fightAction = Cast<UFightAction>(Assets[AssetIdx].GetAsset())) fightActions.Add(fightAction);
	}

	if (fightActions.Num() > 0)
	{
		const FScopedTransaction Transaction(LOCTEXT("MSeqEditorDropClasses", "Move Sequence Editor: Drag and Drop Fight Actions"));

		MSeqGraph->Modify();

		float NodePosX = GraphPosition.X;
		float NodePosY = GraphPosition.Y;

		for (int32 ClassIndex = 0; ClassIndex < fightActions.Num(); ClassIndex++)
		{
			FGraphNodeCreator<UMSeqGraphNode_Regular> NodeCreator(*MSeqGraph);
			UMSeqGraphNode_Regular* MyNode = NodeCreator.CreateNode(true);
			MyNode->NodePosX = NodePosX;
			MyNode->NodePosY = NodePosY;
			NodeCreator.Finalize();

			MyNode->SetFightAction(fightActions[ClassIndex]);

			if (UMoveSequence* moveSequence = Graph->GetTypedOuter<UMoveSequence>())
			{
				moveSequence->GetActionsGraph().Emplace(Graph->Nodes.IndexOfByKey(MyNode));
			}

			NodePosY += 400;
		}

		MSeqGraph->NotifyGraphChanged();
	}
}

#undef LOCTEXT_NAMESPACE