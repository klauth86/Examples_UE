// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EQ/EnvQueryContext_Target.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEnvQueryContext_Target::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	APawn* PawnOwner = Cast<APawn>(QueryInstance.Owner.Get());

	ContextData.ValueType = UEnvQueryItemType_Actor::StaticClass();
	ContextData.NumValues = 1;
	ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr));

	AAIController* controller = PawnOwner->GetController<AAIController>();
	AActor* target = Cast<AActor>(controller->GetBlackboardComponent()->GetValueAsObject(FName("Target")))

	UEnvQueryItemType_Actor::SetValue(ContextData.RawData.GetData(), target);
}