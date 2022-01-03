// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	Locations[0] = GetActorLocation();
	Locations[1] = Locations[0] + TargetLocation;

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority() && !TargetLocation.IsNearlyZero())
	{
		FVector location = GetActorLocation();
		FVector direction = Locations[TargetIndex] - Locations[1 - TargetIndex];
		FVector actualDirection = Locations[TargetIndex] - location;

		SetActorLocation(location + actualDirection.GetSafeNormal() * Velocity * DeltaSeconds);

		if ((direction | actualDirection) < 0) TargetIndex = 1 - TargetIndex;
	}
}