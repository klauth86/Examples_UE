// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_Analyze.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"

void UBTService_Analyze::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{

		}
	}
}