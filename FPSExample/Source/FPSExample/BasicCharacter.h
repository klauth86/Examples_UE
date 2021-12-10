// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

class ACameraActor;

UCLASS()
class FPSEXAMPLE_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupCamera();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<ACameraActor> CameraActorClass;

	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (MakeEditWidget = true))
		FVector CameraOffset;
};