// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode_Regular.h"
#include "Graph/MSeqGraphSchema.h"
#include "Graph/MSeqGraph.h"

UMSeqGraphNode_Regular::UMSeqGraphNode_Regular(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FText UMSeqGraphNode_Regular::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::GetEmpty();
}

FText UMSeqGraphNode_Regular::GetDescription() const
{
	return FText::GetEmpty();
}

void UMSeqGraphNode_Regular::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Next"));
}

UMSeqGraph* UMSeqGraphNode_Regular::GetMSeqGraph()
{
	return CastChecked<UMSeqGraph>(GetGraph());
}

bool UMSeqGraphNode_Regular::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UMSeqGraphSchema::StaticClass());
}

