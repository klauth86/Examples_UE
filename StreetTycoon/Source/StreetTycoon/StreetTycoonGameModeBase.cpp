// Copyright Epic Games, Inc. All Rights Reserved.

#include "StreetTycoonGameModeBase.h"
#include "StreetTycoonPlayerController.h"
#include "StreetTycoonPawn.h"

AStreetTycoonGameModeBase::AStreetTycoonGameModeBase() {
	PlayerControllerClass = AStreetTycoonPlayerController::StaticClass();
	DefaultPawnClass = AStreetTycoonPawn::StaticClass();
}