// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "JoystickInput.generated.h"

USTRUCT(BlueprintType)
struct MOVEGRAPH_API FJoystickInput
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_0 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_45 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_90 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_135 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_180 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_225 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_270 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_315 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftStick_315 : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftTrigger : 1;

	UPROPERTY(EditAnywhere)
		uint32 LeftUpperTrigger : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_0 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_45 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_90 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_135 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_180 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_225 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_270 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_315 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightStick_315 : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightTrigger : 1;

	UPROPERTY(EditAnywhere)
		uint32 RightUpperTrigger : 1;
};