#pragma once

#include "HAL/Platform.h"

UENUM(BlueprintType)
enum class EFightActionType :uint8 {
	UNSET = 0,

	ACTION,
	HIT,
	KO,
	GETUP,
	SIDE_LEFT_BLOCK,
	SIDE_RIGHT_BLOCK,
	FORWARD_BLOCK,
	BACKWARD_BLOCK,
};