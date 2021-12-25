// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MoveSequence.h"
#include "MSeqGraphNode_Regular.generated.h"

UCLASS()
class UMSeqGraphNode_Regular : public UMSeqGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void AllocateDefaultPins() override;

	virtual void AddTransition(int32 indexB) override;

	virtual void RemoveTransition(int32 indexB) override;

	void SetFightAction(UFightAction* fightAction) { NodeInstance.FightAction = fightAction; }

protected:

	UPROPERTY(EditAnywhere, meta = (ShowOnlyInnerProperties))
		FActionsGraphNode NodeInstance;
};