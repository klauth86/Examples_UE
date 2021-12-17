// Fill out your copyright notice in the Description page of Project Settings.

#include "VisitorSimulator.h"

AVisitorSimulator::AVisitorSimulator()
{
	VisitorChance = 0.2f;
	SpawnDelay = 5;

	TimeLeftBeforeNextPossibleVisitor = SpawnDelay;

	PrimaryActorTick.bCanEverTick = true;
}

void AVisitorSimulator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLeftBeforeNextPossibleVisitor -= DeltaTime;
	if (TimeLeftBeforeNextPossibleVisitor < 0) {
		TimeLeftBeforeNextPossibleVisitor = SpawnDelay;

		if (FMath::FRand() < VisitorChance) SimulateVisitor();
	}
}

void AVisitorSimulator::SimulateVisitor() {

}