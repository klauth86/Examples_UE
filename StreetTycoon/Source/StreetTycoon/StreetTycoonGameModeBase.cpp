// Copyright Epic Games, Inc. All Rights Reserved.

#include "StreetTycoonGameModeBase.h"
#include "StreetTycoonPlayerController.h"

AStreetTycoonGameModeBase::AStreetTycoonGameModeBase() {
	PlayerControllerClass = AStreetTycoonPlayerController::StaticClass();
}