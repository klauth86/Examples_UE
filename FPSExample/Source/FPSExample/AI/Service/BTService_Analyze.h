// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_Analyze.generated.h"

UCLASS()
class FPSEXAMPLE_API UBTService_Analyze : public UBTService
{
	GENERATED_BODY()
	
protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_SelfActor;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_Target;
};