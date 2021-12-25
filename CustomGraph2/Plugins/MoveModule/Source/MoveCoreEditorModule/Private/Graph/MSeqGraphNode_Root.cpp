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