// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTNode_AimAtTarget.generated.h"

class ABasicCharacter;

UCLASS()
class FPSEXAMPLE_API UBTTNode_AimAtTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTNode_AimAtTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	EBTNodeResult::Type AimAtTarget(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

	void Focus(ABasicCharacter* character, AActor* target);

	void Unfocus(ABasicCharacter* character);

protected:

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_SelfActor;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector BK_Target;
};