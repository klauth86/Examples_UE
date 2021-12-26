// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode_Root.h"

FText UMSeqGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("MSeqEditor", "Root", "[0] ROOT");
}

void UMSeqGraphNode_Root::AllocateDefaultPins()
{
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Next"));
}

void UMSeqGraphNode_Root::AddTransition(int32 indexB)
{
	if (!Transitions.FindByPredicate([indexB](const FActionsGraphTransition& transition) { return transition.TargetIndex == indexB; }))
	{
		int32 addedIndex = Transitions.Add(FActionsGraphTransition());
		Transitions[addedIndex].TargetIndex = indexB;
	}
}

void UMSeqGraphNode_Root::RemoveTransition(int32 indexB, bool decrementOthers)
{
	Transitions.RemoveAll([indexB](const FActionsGraphTransition& transition) { return transition.TargetIndex == indexB; });

	if (decrementOthers)
	{
		for (FActionsGraphTransition& transition : Transitions)
		{
			if (transition.TargetIndex > indexB) transition.TargetIndex--;
		}
	}
}