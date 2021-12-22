// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "FightAction.generated.h"

class UAnimMontage;
class USoundBase;
enum class EFightActionType :uint8;

UENUM()
enum class EFightActionPriority :uint8 {
	MOMENTARY = 0,

	VERY_LOW = 16,
	LOW = 64,
	NORMAL = 128,
	HIGH = 192,
	VERY_HIGH = 240,

	UNINTERRUPTIBLE = 255
};

UCLASS(BlueprintType)
class MOVECOREMODULE_API UFightAction : public UObject {

	GENERATED_UCLASS_BODY()

public:

	UAnimMontage* GetAnim(EFightActionType actionType) const;

public:

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* ActionAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* HitAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* CrouchHitAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* KOAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* GetUpAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* SideLeftBlockAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* SideRightBlockAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* ForwardBlockAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* BackwardBlockAnim = nullptr;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* HitSfx = nullptr;

	UPROPERTY(EditDefaultsOnly)
		USoundBase* WhooshSfx = nullptr;

	UPROPERTY(EditDefaultsOnly)
		FName BoneName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
		float Damage;

	UPROPERTY(EditDefaultsOnly)
		float Stamina;

	UPROPERTY(EditDefaultsOnly)
		float AttackRadiusSquared;

	UPROPERTY(EditDefaultsOnly)
		TEnumAsByte<EFightActionPriority> Priority;
};