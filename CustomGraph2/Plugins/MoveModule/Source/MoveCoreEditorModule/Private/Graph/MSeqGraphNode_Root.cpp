// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode_Root.h"

UMSeqGraphNode_Root::UMSeqGraphNode_Root(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsReadOnly = true;
}

void UMSeqGraphNode_Root::AllocateDefaultPins()
{
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Next"));
}

FText UMSeqGraphNode_Root::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("MSeqEditor", "Root", "ROOT");
}

void UMSeqGraphNode_Root::LogDebugMessage(const FString& Message)
{
	if (DebugMessages.Num() == 0)
	{
		bHasDebugError = false;
	}

	// store only 1 error message, discard everything after it
	if (!bHasDebugError)
	{
		DebugMessages.Add(Message);
	}
}

void UMSeqGraphNode_Root::LogDebugError(const FString& Message)
{
	if (DebugMessages.Num() == 0)
	{
		bHasDebugError = false;
	}

	// store only 1 error message, discard everything after it
	if (!bHasDebugError)
	{
		DebugMessages.Add(Message);
		bHasDebugError = true;
	}
}
