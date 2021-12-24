// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MSeqGraphNode_Root.generated.h"

UCLASS()
class UMSeqGraphNode_Root : public UMSeqGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void AllocateDefaultPins() override;
};