// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/BasicProjectile.h"
#include "Components/SphereComponent.h"

ABasicProjectile::ABasicProjectile()
{
	RootComponent = SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnComponentHit);

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

void ABasicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor) {
		if (OtherActor->CanBeDamaged())
		{
			OtherActor->TakeDamage(100, FDamageEvent(), nullptr, this);
		}
		else
		{
		}

		Destroy();
	}
}