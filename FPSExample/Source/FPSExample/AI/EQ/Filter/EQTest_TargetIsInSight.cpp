// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EQ/Filter/EQTest_TargetIsInSight.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "Characters/BasicCharacter.h"

void UEQTest_TargetIsInSight::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FEnvQueryContext_TargetData ContextData;
	if (!QueryInstance.PrepareContext(ContextClass, ContextData))
	{
		return;
	}

	ABasicCharacter* characterOwner = Cast<ABasicCharacter>(UEnvQueryItemType_Actor::GetValue(ContextData.RawData.GetData()));

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());


	}
}