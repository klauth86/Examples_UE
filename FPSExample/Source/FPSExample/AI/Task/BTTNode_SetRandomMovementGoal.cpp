// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTNode_SetRandomMovementGoal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "NavigationSystem.h"

UBTTNode_SetRandomMovementGoal::UBTTNode_SetRandomMovementGoal()
{
	Radius = 400;
}

EBTNodeResult::Type UBTTNode_SetRandomMovementGoal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		bb->ClearValue(BK_MovementGoal.SelectedKeyName);

		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{
			UWorld* world = self->GetWorld();
			if (UNavigationSystemV1* navSys = Cast<UNavigationSystemV1>(world->GetNavigationSystem()))
			{
				FNavLocation navLocation;
				if (navSys->GetRandomReachablePointInRadius(self->GetActorLocation(), Radius, navLocation)) {
					bb->SetValueAsVector(BK_MovementGoal.SelectedKeyName, navLocation.Location);
				}
			}
		}
	}

	return EBTNodeResult::Type::Succeeded;
}