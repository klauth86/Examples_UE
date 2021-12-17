// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VisitorSimulator.generated.h"

UCLASS()
class STREETTYCOON_API AVisitorSimulator : public AActor
{
	GENERATED_BODY()
	
public:	

	AVisitorSimulator();

	virtual void Tick(float DeltaTime) override;

protected:

	void SimulateVisitor();

protected:

	UPROPERTY(EditAnywhere, Category = "VisitorSimulator", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.8", UIMax = "0.8"))
		float VisitorChance;

	UPROPERTY(EditAnywhere, Category = "VisitorSimulator", meta = (ClampMin = "0.1", UIMin = "0.1", ClampMax = "3600", UIMax = "3600"))
		float SpawnDelay;

	float TimeLeftBeforeNextPossibleVisitor;
};