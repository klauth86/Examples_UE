// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTNode_SetRandomMovementGoal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTNode_SetRandomMovementGoal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		bb->SetValueAsVector(BK_MovementGoal.SelectedKeyName, FVector::ZeroVector);

		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{
			UWorld* world = self->GetWorld();
			if (UNavigationSystemV1* navSys = Cast<UNavigationSystemV1>(world->GetNavigationSystem()))
			{
				FVector result = navSys->GetRandomReachablePointInRadius(world, self->GetActorLocation(), 200.f);
				bb->SetValueAsVector(BK_MovementGoal.SelectedKeyName, result);
			}
		}
	}

	return EBTNodeResult::Type::Succeeded;
}