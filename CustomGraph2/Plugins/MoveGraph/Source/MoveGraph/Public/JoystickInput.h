// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "JoystickInput.generated.h"

USTRUCT(BlueprintType)
struct MOVEGRAPH_API FJoystickInput
{
	GENERATED_USTRUCT_BODY()

public:

	FJoystickInput()
	{
		PackedBits = 0;
	}

	bool HasLeftStick() const { return LeftStick_0 | LeftStick_45 | LeftStick_90 | LeftStick_135 | LeftStick_180 | LeftStick_225 | LeftStick_270 | LeftStick_315; }

	bool HasRightStick() const { return RightStick_0 | RightStick_45 | RightStick_90 | RightStick_135 | RightStick_180 | RightStick_225 | RightStick_270 | RightStick_315; }

	union
	{
		struct
		{
			// LEFT STICK

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_0 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_45 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_90 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_135 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_180 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_225 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_270 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_315 : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftStick_HOLD_IN_PARENT : 1;

			// LEFT TRIGGER

			UPROPERTY(EditAnywhere)
				uint64 LeftUpperTrigger : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftUpperTrigger_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftUpperTrigger_HOLD_IN_PARENT : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftTrigger : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftTrigger_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 LeftTrigger_HOLD_IN_PARENT : 1;

			// RIGHT STICK

			UPROPERTY(EditAnywhere)
				uint64 RightStick_0 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_45 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_90 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_135 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_180 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_225 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_270 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_315 : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightStick_HOLD_IN_PARENT : 1;

			// RIGHT TRIGGER

			UPROPERTY(EditAnywhere)
				uint64 RightUpperTrigger : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightUpperTrigger_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightUpperTrigger_HOLD_IN_PARENT : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightTrigger : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightTrigger_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 RightTrigger_HOLD_IN_PARENT : 1;

			// XYAB

			UPROPERTY(EditAnywhere)
				uint64 X : 1;

			UPROPERTY(EditAnywhere)
				uint64 X_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 X_HOLD_IN_PARENT : 1;

			UPROPERTY(EditAnywhere)
				uint64 Y : 1;

			UPROPERTY(EditAnywhere)
				uint64 Y_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 Y_HOLD_IN_PARENT : 1;

			UPROPERTY(EditAnywhere)
				uint64 A : 1;

			UPROPERTY(EditAnywhere)
				uint64 A_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 A_HOLD_IN_PARENT : 1;

			UPROPERTY(EditAnywhere)
				uint64 B : 1;

			UPROPERTY(EditAnywhere)
				uint64 B_HOLD : 1;

			UPROPERTY(EditAnywhere)
				uint64 B_HOLD_IN_PARENT : 1;
		};

		uint64 PackedBits;
	};
};