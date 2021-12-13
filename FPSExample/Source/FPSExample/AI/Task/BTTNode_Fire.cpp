// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTNode_Fire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"

EBTNodeResult::Type UBTTNode_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{
			self->Fire();
		}
	}

	return EBTNodeResult::Type::Succeeded;
}