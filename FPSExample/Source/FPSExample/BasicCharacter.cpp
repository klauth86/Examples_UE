// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCharacter.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCamera();
}

void ABasicCharacter::SetupCamera() {
	if (CameraActorClass)
	{
		const FVector selfLocation = GetActorLocation();
		const FVector cameraLocation = selfLocation + CameraOffset;

		FRotator cameraRotation = (selfLocation - cameraLocation).Rotation();

		auto cameraActor = GetWorld()->SpawnActor<ACameraActor>(CameraActorClass);
		cameraActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
		if (auto playerController = GetController<APlayerController>()) {
			playerController->SetViewTargetWithBlend(cameraActor, 0);
		}
	}
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}