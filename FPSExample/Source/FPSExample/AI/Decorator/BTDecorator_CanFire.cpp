// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorator/BTDecorator_CanFire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"

bool UBTDecorator_CanFire::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{
			return self->CanFire();
		}
	}

	return false;
}