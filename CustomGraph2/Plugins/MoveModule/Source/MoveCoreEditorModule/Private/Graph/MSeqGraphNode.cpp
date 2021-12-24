// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraphNode.h"
#include "Graph/MSeqGraphSchema.h"

bool UMSeqGraphNode::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const
{
	return DesiredSchema->GetClass()->IsChildOf(UMSeqGraphSchema::StaticClass());
}