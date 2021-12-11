// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

class ABasicProjectile;
class ACameraActor;

UCLASS()
class FPSEXAMPLE_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ABasicCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	void SetupCamera();

	void OnFirePressed();

	void OnFireReleased();

	void OnMoveRight(float value);

	void OnMoveUp(float value);

protected:

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<ACameraActor> CameraActorClass;

	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (MakeEditWidget = true))
		FVector CameraOffset;

	UPROPERTY(EditAnywhere, Category = "Fire")
		TSubclassOf<ABasicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Fire", Meta = (MakeEditWidget = true))
		FVector FireOffset;

	UPROPERTY(EditAnywhere, Category = "Fire")
		uint8 CanFire : 1;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RotationRate;
};