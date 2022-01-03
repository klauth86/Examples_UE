// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	AMovingPlatform();

	virtual void BeginPlay() override;

protected:

	virtual void Tick(float DeltaSeconds) override;

	uint8 TargetIndex = 1;

	FVector Locations[2];

	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = "true"))
		FVector TargetLocation;

	UPROPERTY(EditAnywhere)
		float Velocity;
};