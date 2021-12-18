// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "JoystickInput.generated.h"

union MOVEGRAPH_API FJoystickInputElement
{
	FJoystickInputElement() { PackedBits = 0; }

	void SetFromParent(const FJoystickInputElement& parentElement) {
	
	}

struct
{
	bool HasLeftStick() const { return LeftStick_0 | LeftStick_45 | LeftStick_90 | LeftStick_135 | LeftStick_180 | LeftStick_225 | LeftStick_270 | LeftStick_315; }

	void ResetLeftStick() { LeftStick_0 = false; LeftStick_45 = false; LeftStick_90 = false; LeftStick_135 = false; LeftStick_180 = false; LeftStick_225 = false; LeftStick_270 = false; LeftStick_315 = false; LeftStick_HOLD = false; }

	bool HasRightStick() const { return RightStick_0 | RightStick_45 | RightStick_90 | RightStick_135 | RightStick_180 | RightStick_225 | RightStick_270 | RightStick_315; }

	void ResetRightStick() { RightStick_0 = false; RightStick_45 = false; RightStick_90 = false; RightStick_135 = false; RightStick_180 = false; RightStick_225 = false; RightStick_270 = false; RightStick_315 = false; RightStick_HOLD = false; }

	// LEFT TRIGGER

	uint64 LeftUpperTrigger : 1;

	uint64 LeftUpperTrigger_HOLD : 1;

	uint64 LeftTrigger : 1;

	uint64 LeftTrigger_HOLD : 1;

	// LEFT STICK

	uint64 LeftStick_0 : 1;

	uint64 LeftStick_45 : 1;

	uint64 LeftStick_90 : 1;

	uint64 LeftStick_135 : 1;

	uint64 LeftStick_180 : 1;

	uint64 LeftStick_225 : 1;

	uint64 LeftStick_270 : 1;

	uint64 LeftStick_315 : 1;

	uint64 LeftStick_HOLD : 1;

	// RIGHT STICK

	uint64 RightStick_0 : 1;

	uint64 RightStick_45 : 1;

	uint64 RightStick_90 : 1;

	uint64 RightStick_135 : 1;

	uint64 RightStick_180 : 1;

	uint64 RightStick_225 : 1;

	uint64 RightStick_270 : 1;

	uint64 RightStick_315 : 1;

	uint64 RightStick_HOLD : 1;

	// RIGHT TRIGGER

	uint64 RightUpperTrigger : 1;

	uint64 RightUpperTrigger_HOLD : 1;

	uint64 RightTrigger : 1;

	uint64 RightTrigger_HOLD : 1;

	// XYAB

	uint64 X : 1;

	uint64 X_HOLD : 1;

	uint64 Y : 1;

	uint64 Y_HOLD : 1;

	uint64 A : 1;

	uint64 A_HOLD : 1;

	uint64 B : 1;

	uint64 B_HOLD : 1;
} Flags;

	uint64 PackedBits;
};

USTRUCT(BlueprintType)
struct MOVEGRAPH_API FJoystickInput
{
	GENERATED_USTRUCT_BODY()

public:

	TArray<FJoystickInputElement> InputElements;
};