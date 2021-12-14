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

	void Launch(AActor* actor, FVector direction);

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float GetRechargeTime() const { return RechargeTime; }

	float GetRange() const { return Range; }

protected:

	TWeakObjectPtr<AActor> OwnerPtr;

	FVector Direction;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float Velocity;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float RechargeTime;

	UPROPERTY(EditAnywhere, Category = "Projectile")
		float Range;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		USphereComponent* SphereComponent;
};