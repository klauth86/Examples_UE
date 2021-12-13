// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTNode_Fire.generated.h"

UCLASS()
class FPSEXAMPLE_API UBTTNode_Fire : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_SelfActor;
};