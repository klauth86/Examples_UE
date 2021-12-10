// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicCharacter.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ABasicCharacter::ABasicCharacter()
{
	RotationRate = 120;
}

void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicCharacter::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasicCharacter::OnFireReleased);

	PlayerInputComponent->BindAxis("MoveRight", this, &ABasicCharacter::OnMoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ABasicCharacter::OnMoveUp);
}

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

		auto cameraActor = GetWorld()->SpawnActor<ACameraActor>(CameraActorClass, cameraLocation, cameraRotation);
		cameraActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		
		if (auto playerController = GetController<APlayerController>()) {
			playerController->SetViewTargetWithBlend(cameraActor, 0);
		}
	}
}

void ABasicCharacter::OnFirePressed() {

}

void ABasicCharacter::OnFireReleased() {

}

void ABasicCharacter::OnMoveRight(float value)
{
	AddControllerYawInput(value * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABasicCharacter::OnMoveUp(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}