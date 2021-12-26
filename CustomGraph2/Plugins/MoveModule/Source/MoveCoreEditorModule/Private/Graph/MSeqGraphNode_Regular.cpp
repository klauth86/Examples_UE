// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode_Regular.h"
#include "FightAction.h"

FText UMSeqGraphNode_Regular::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FString result = "[";
	result.AppendInt(GetGraph()->Nodes.IndexOfByKey(this));
	result.Append("] ").Append(NodeInstance.FightAction ? NodeInstance.FightAction->GetName() : FString("NONE"));
	return FText::FromString(result);
}

void UMSeqGraphNode_Regular::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Next"));
}

void UMSeqGraphNode_Regular::AddTransition(int32 indexB)
{
	if (!NodeInstance.Transitions.FindByPredicate([indexB](const FActionsGraphTransition& transition) { return transition.TargetIndex == indexB; }))
	{
		int32 addedIndex = NodeInstance.Transitions.Add(FActionsGraphTransition());
		NodeInstance.Transitions[addedIndex].TargetIndex = indexB;
	}
}

void UMSeqGraphNode_Regular::RemoveTransition(int32 indexB, bool decrementOthers)
{
	NodeInstance.Transitions.RemoveAll([indexB](const FActionsGraphTransition& transition) { return transition.TargetIndex == indexB; });

	if (decrementOthers)
	{
		for (FActionsGraphTransition& transition : NodeInstance.Transitions)
		{
			if (transition.TargetIndex > indexB) transition.TargetIndex--;
		}
	}
}