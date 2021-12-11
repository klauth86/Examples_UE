// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicProjectile.h"
#include "Components/SphereComponent.h"

ABasicProjectile::ABasicProjectile()
{
	RootComponent = SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	Velocity = 1000;
	RechargeTime = 1;
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