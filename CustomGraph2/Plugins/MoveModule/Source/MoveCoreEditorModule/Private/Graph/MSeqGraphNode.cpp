// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode.h"
#include "Graph/MSeqGraphSchema.h"

bool UMSeqGraphNode::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UMSeqGraphSchema::StaticClass());
}

void UMSeqGraphNode::DestroyNode() {

	int32 nodeIndex = GetGraph()->Nodes.IndexOfByKey(this);

	for (UEdGraphNode* node : GetGraph()->Nodes)
	{
		if (UMSeqGraphNode* graphNode = Cast<UMSeqGraphNode>(node)) {
			graphNode->RemoveTransition(nodeIndex, true);
		}
	}

	Super::DestroyNode();
}