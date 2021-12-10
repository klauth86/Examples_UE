// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicProjectile.h"

ABasicProjectile::ABasicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void ABasicProjectile::Launch(FVector direction)
{
	Direction = direction;
	SetActorTickEnabled(true);
}

void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + DeltaTime * Direction * Velocity);
}