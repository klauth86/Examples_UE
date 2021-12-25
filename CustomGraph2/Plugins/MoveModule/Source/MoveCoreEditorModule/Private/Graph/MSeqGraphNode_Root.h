// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MoveSequence.h"
#include "MSeqGraphNode_Root.generated.h"

UCLASS()
class UMSeqGraphNode_Root : public UMSeqGraphNode
{
	GENERATED_BODY()

public:

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void AllocateDefaultPins() override;

	virtual void AddTransition(int32 indexB) override
	{
		if (!Transitions.FindByPredicate([indexB](const FActionsGraphTransition& transition) { return transition.TargetIndex == indexB; }))
		{
			int32 addedIndex = Transitions.Add(FActionsGraphTransition());
			Transitions[addedIndex].TargetIndex = indexB;
		}
	}

protected:

	UPROPERTY(VisibleAnywhere)
		TArray<FActionsGraphTransition> Transitions;
};