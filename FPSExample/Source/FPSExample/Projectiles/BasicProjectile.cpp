// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectiles/BasicProjectile.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"

ABasicProjectile::ABasicProjectile()
{
	RootComponent = SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	SphereComponent->SetUseCCD(true);
	SphereComponent->AreaClass = nullptr;

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);

	Velocity = 1000;
	RechargeTime = 1;
	Range = 500;
}

void ABasicProjectile::Launch(AActor* actor, FVector direction)
{
	OwnerPtr = actor;

	Direction = direction;

	SphereComponent->OnComponentHit.AddDynamic(this, &ABasicProjectile::OnComponentHit);

	SetActorTickEnabled(true);
}

void ABasicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float deltaPath = DeltaTime * Velocity;

	Range -= deltaPath;

	if (Range < 0)
	{
		Destroy();
		return;
	}

	FVector deltaLocation = Direction * deltaPath;

	SphereComponent->MoveComponent(deltaLocation, GetActorRotation(), true);
}

void ABasicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor) {

		if (OwnerPtr == OtherActor) return;

		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName())

		if (OtherActor->CanBeDamaged())
		{
			//OtherActor->TakeDamage(100, FDamageEvent(), nullptr, this);
		}
		else
		{

		}

		Destroy();
	}
}