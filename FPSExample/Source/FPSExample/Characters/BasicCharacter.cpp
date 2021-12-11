// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ABasicCharacter::ABasicCharacter()
{
	RotationRate = 120;
	CanFire = true;
	SetCanBeDamaged(true);
}

void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABasicCharacter::OnFirePressed);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABasicCharacter::OnFireReleased);

	PlayerInputComponent->BindAxis("MoveRight", this, &ABasicCharacter::OnMoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ABasicCharacter::OnMoveUp);
}

float ABasicCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	Destroy();
	return DamageAmount;
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

		if (ACameraActor* cameraActor = GetWorld()->SpawnActor<ACameraActor>(CameraActorClass, cameraLocation, cameraRotation))
		{
			cameraActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

			if (APlayerController* playerController = GetController<APlayerController>())
			{
				playerController->SetViewTargetWithBlend(cameraActor, 0);
			}
		}
	}
}

void ABasicCharacter::OnFirePressed() {
	if (CanFire && ProjectileClass) {

		FVector projectileOrigin;
		FVector projectileBounds;
		ABasicProjectile* defaultProjectile = ProjectileClass->GetDefaultObject<ABasicProjectile>();
		defaultProjectile->GetActorBounds(false, projectileOrigin , projectileBounds);
		
		const FVector selfLocation = GetActorLocation();
		const FVector spawnLocation = selfLocation + GetActorRotation().RotateVector(FireOffset) + FVector(projectileBounds.X / 2, 0, 0);

		auto world = GetWorld();

		if (ABasicProjectile* projectile = world->SpawnActor<ABasicProjectile>(ProjectileClass, spawnLocation, FRotator::ZeroRotator))
		{
			projectile->Launch(GetActorForwardVector());

			CanFire = false;

			TWeakObjectPtr<ABasicCharacter> characterPtr = this;

			FTimerHandle timerHandle;
			world->GetTimerManager().SetTimer(timerHandle, [characterPtr]() { if (ABasicCharacter* character = characterPtr.Get()) character->CanFire = true; }, defaultProjectile->GetRechargeTime(), false);
		}
	}
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