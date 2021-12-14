// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnvironmentQuery/Generators/EnvQueryGenerator_ProjectedPoints.h"
#include "EQGen_MeleePosition.generated.h"

UCLASS()
class FPSEXAMPLE_API UEQGen_MeleePosition : public UEnvQueryGenerator_ProjectedPoints
{
	GENERATED_BODY()
	
public:

	UEQGen_MeleePosition();

	UPROPERTY(EditDefaultsOnly, Category = Generator)
		TSubclassOf<UEnvQueryContext> Querier;

	UPROPERTY(EditDefaultsOnly, Category = Generator)
		TSubclassOf<UEnvQueryContext> Target;

	virtual void GenerateItems(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;
};