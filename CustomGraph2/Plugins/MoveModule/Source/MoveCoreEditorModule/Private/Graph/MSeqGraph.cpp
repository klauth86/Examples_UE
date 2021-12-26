// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraph.h"
#include "Graph/MSeqGraphSchema.h"
#include "Graph/MSeqGraphNode.h"
#include "MoveSequence.h"

UMSeqGraph::UMSeqGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UMSeqGraphSchema::StaticClass();
}

void UMSeqGraph::Initialize() {
	LockUpdates();
	////// TODO Spawn missed nodes if should
	UnlockUpdates();
}

void UMSeqGraph::UpdateAsset(int32 UpdateFlags = 0)
{
	if (IsLocked()) return;

	if (UMoveSequence* moveSequence = GetTypedOuter<UMoveSequence>())
	{
		TMap<int32, FActionsGraphNode>& actionsGraph = moveSequence->GetActionsGraph();
		actionsGraph.Empty();

		for (size_t nodeIndex = 0; nodeIndex < Nodes.Num(); nodeIndex++)
		{
			if (UMSeqGraphNode* graphNode = Cast<UMSeqGraphNode>(Nodes[nodeIndex]))
			{
				FActionsGraphNode& actionsGraphNode = actionsGraph.Emplace(nodeIndex);
				actionsGraphNode.FightAction = graphNode->GetFightAction();
				actionsGraphNode.Transitions = graphNode->GetTransitions();
			}
		}
	}
}

void UMSeqGraph::UpdateClassData() {

}