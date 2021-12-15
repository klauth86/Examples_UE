// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"
#include "BasicCharacter.generated.h"

class ABasicProjectile;
class ACameraActor;

class UAIPerceptionComponent;
class UAIPerceptionStimuliSourceComponent;
class UWidgetComponent;

UCLASS()
class FPSEXAMPLE_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	static FName PlayerTag;

	ABasicCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool CanFire() const { return bCanFire; }

	const TSubclassOf<ABasicProjectile>& GetProjectileClass() const { return ProjectileClass; }

	UAIPerceptionComponent* GetAIPerceptionComponent() const { return AIPerceptionComponent; }

	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

	float GetRotationRate() const { return RotationRate; }

	FVector GetFireOffset() const { return FireOffset; }

	void Fire();

protected:

	virtual void BeginPlay() override;

	void SetupCamera();

	void OnFirePressed() { Fire(); }

	void OnFireReleased() {}

	void OnMoveRight(float value);

	void OnMoveUp(float value);

	UFUNCTION()
		void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
		void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
		void OnTargetPerceptionInfoUpdated(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION(BlueprintCallable)
		void Test();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "BasicCharacter")
		int32 HitPoints;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

	UPROPERTY(VisibleAnywhere, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Camera")
		TSubclassOf<ACameraActor> CameraActorClass;

	UPROPERTY(EditAnywhere, Category = "Camera", Meta = (MakeEditWidget = true))
		FVector CameraOffset;

	UPROPERTY(EditAnywhere, Category = "Fire")
		TSubclassOf<ABasicProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Fire", Meta = (MakeEditWidget = true))
		FVector FireOffset;

	UPROPERTY(EditAnywhere, Category = "Fire")
		uint8 bCanFire : 1;

	UPROPERTY(EditAnywhere, Category = "Movement")
		float RotationRate;
};