// Fill out your copyright notice in the Description page of Project Settings.

#include "StreetTycoonPawn.h"
#include "Camera/CameraComponent.h"

AStreetTycoonPawn::AStreetTycoonPawn() {
	RootComponent = CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
}