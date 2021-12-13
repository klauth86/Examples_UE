// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTNode_SetRandomMovementGoal.generated.h"

UCLASS()
class FPSEXAMPLE_API UBTTNode_SetRandomMovementGoal : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_SelfActor;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_MovementGoal;
};