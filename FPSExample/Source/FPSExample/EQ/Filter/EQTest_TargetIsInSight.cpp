// Fill out your copyright notice in the Description page of Project Settings.

#include "EQ/Filter/EQTest_TargetIsInSight.h"

void UEQTest_TargetIsInSight::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FEnvQueryContextData ContextData;
	if (!QueryInstance.PrepareContext(ContextClass, ContextData))
	{
		return;
	}

	ContextData.RawData.GetData();

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());


	}
}