// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphSchema.h"
#include "MSeqGraphNode_Root.h"

void UMSeqGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UMSeqGraphNode_Root> NodeCreator(Graph);
	UMSeqGraphNode_Root* MyNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(MyNode, FNodeMetadata::DefaultGraphNode);
}
