// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MSeqGraphNode_Regular.generated.h"

class UMSeqGraph;

UCLASS()
class UMSeqGraphNode_Regular : public UMSeqGraphNode
{
	GENERATED_UCLASS_BODY()
	
	virtual class UMSeqGraph* GetMSeqGraph();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetDescription() const override;
	virtual void AllocateDefaultPins() override;

	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;
};
