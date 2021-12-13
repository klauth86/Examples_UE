// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTService_Analyze.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "Perception/AIPerceptionComponent.h"

void UBTService_Analyze::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		if (ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName)))
		{
			TArray<AActor*> perceivedActors;
			self->GetAIPerceptionComponent()->GetCurrentlyPerceivedActors(nullptr, perceivedActors);

			AActor** playerActor = perceivedActors.FindByPredicate([](const AActor* actor) { return actor->ActorHasTag(ABasicCharacter::PlayerTag); });
			return playerActor
				? bb->SetValueAsObject(BK_Target.SelectedKeyName, *playerActor)
				: bb->ClearValue(BK_Target.SelectedKeyName);
		}
	}
}