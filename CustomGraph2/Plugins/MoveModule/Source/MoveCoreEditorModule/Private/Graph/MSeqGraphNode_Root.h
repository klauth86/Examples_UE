// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MSeqGraphNode_Root.generated.h"

UCLASS()
class UMSeqGraphNode_Root : public UMSeqGraphNode
{
	GENERATED_BODY()

public:

	virtual bool CanDuplicateNode() const override { return false; }

	virtual bool CanUserDeleteNode() const override { return false; }

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void AllocateDefaultPins() override;

	virtual void AddTransition(int32 indexB, const FJoystickInput& transitionInput = FJoystickInput::GetEmpty()) override;

	virtual void RemoveTransition(int32 indexB, bool decrementOthers) override;

	virtual const TArray<FActionsGraphTransition>& GetTransitions() const override { return Transitions; }

protected:

	UPROPERTY(VisibleAnywhere)
		TArray<FActionsGraphTransition> Transitions;
};