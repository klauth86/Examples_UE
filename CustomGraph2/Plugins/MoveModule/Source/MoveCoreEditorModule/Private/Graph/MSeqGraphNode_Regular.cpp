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