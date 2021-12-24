// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode_Regular.h"

FText UMSeqGraphNode_Regular::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::GetEmpty();
}

void UMSeqGraphNode_Regular::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Next"));
}