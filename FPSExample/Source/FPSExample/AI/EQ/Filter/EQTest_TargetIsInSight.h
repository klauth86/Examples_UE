// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentQuery/EnvQueryTest.h"
#include "EQTest_TargetIsInSight.generated.h"

UCLASS()
class FPSEXAMPLE_API UEQTest_TargetIsInSight : public UEnvQueryTest
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Target Is In Sight")
		TSubclassOf<UEnvQueryContext_Target> ContextClass;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};