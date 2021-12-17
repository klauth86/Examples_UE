// Fill out your copyright notice in the Description page of Project Settings.

#include "FloatingTextActor.h"
#include "Components/WidgetComponent.h"

AFloatingTextActor::AFloatingTextActor()
{
	RootComponent = WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");

	PrimaryActorTick.bCanEverTick = true;
}

void AFloatingTextActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RootComponent->MoveComponent(FVector(0, 0, DeltaTime * BlowRate), GetActorRotation(), false);
}