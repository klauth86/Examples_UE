// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"
#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"

FName ABasicCharacter::PlayerTag = FName("Player");

ABasicCharacter::ABasicCharacter()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("AIPerceptionComponent");
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABasicCharacter::OnPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABasicCharacter::OnTargetPerceptionUpdated);
	AIPerceptionComponent->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ABasicCharacter::OnTargetPerceptionInfoUpdated);

	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>("AIPerceptionStimuliSourceComponent");

	RotationRate = 120;
	bCanFire = true;
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

void ABasicCharacter::Fire()
{
	if (bCanFire && ProjectileClass)
	{

		FVector projectileOrigin;
		FVector projectileBounds;
		ABasicProjectile* defaultProjectile = ProjectileClass->GetDefaultObject<ABasicProjectile>();
		defaultProjectile->GetActorBounds(false, projectileOrigin, projectileBounds);

		const FVector selfLocation = GetActorLocation();
		const FVector spawnLocation = selfLocation + GetActorRotation().RotateVector(FireOffset) + FVector(projectileBounds.X / 2, 0, 0);

		UWorld* world = GetWorld();

		if (ABasicProjectile* projectile = world->SpawnActor<ABasicProjectile>(ProjectileClass, spawnLocation, FRotator::ZeroRotator))
		{
			projectile->Launch(this, GetActorForwardVector());

			bCanFire = false;

			TWeakObjectPtr<ABasicCharacter> characterPtr = this;

			FTimerHandle timerHandle;
			world->GetTimerManager().SetTimer(timerHandle, [characterPtr]() { if (ABasicCharacter* character = characterPtr.Get()) character->bCanFire = true; }, defaultProjectile->GetRechargeTime(), false);
		}
	}
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

void ABasicCharacter::OnMoveRight(float value)
{
	AddControllerYawInput(value * RotationRate * GetWorld()->GetDeltaSeconds());
}

void ABasicCharacter::OnMoveUp(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ABasicCharacter::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	UE_LOG(LogTemp, Warning, TEXT("$$$ OnPerceptionUpdated %d"), UpdatedActors.Num())
}

void ABasicCharacter::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("$$$ OnTargetPerceptionUpdated %s"), *Actor->GetName())
}

void ABasicCharacter::OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("$$$ OnTargetPerceptionInfoUpdated %s"), UpdateInfo.Target.Get() ? *UpdateInfo.Target.Get()->GetName() : *FString("NULL"))
}