#pragma once

#include "HAL/Platform.h"

struct FAIState
{
	FAIState() { PackedBits = 0; }

	union
	{

		struct
		{
			uint8 ShouldAttack : 1;

		} Flags;

		int32 PackedBits;
	};
};