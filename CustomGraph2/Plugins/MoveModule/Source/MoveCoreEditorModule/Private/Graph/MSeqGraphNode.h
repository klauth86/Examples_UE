// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "MSeqGraphNode.generated.h"

UCLASS()
class MOVECOREEDITORMODULE_API UMSeqGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* DesiredSchema) const override;

	virtual void DestroyNode() override;

	virtual void AddTransition(int32 indexB) {}

	virtual void RemoveTransition(int32 indexB, bool decrementOthers) {}
};