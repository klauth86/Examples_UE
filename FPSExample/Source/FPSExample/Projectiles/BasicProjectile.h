// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BasicProjectile.generated.h"

class USphereComponent;

UCLASS()
class FPSEXAMPLE_API ABasicProjectile : public AActor
{
	GENERATED_BODY()

public:

	ABasicProjectile();

public:

	void Launch(FVector direction);

	virtual void Tick(float DeltaTime) override;

	float GetRechargeTime() const { return RechargeTime; }

protected:

	FVector Direction;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float Velocity;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float RechargeTime;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		USphereComponent* SphereComponent;
};