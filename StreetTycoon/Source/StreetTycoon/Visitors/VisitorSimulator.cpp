// Fill out your copyright notice in the Description page of Project Settings.

#include "Visitors/VisitorSimulator.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Shops/ShopActor.h"

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
		if (FMath::FRand() < VisitorChance) SimulateVisitor();
		TimeLeftBeforeNextPossibleVisitor = SpawnDelay;
	}
}

void AVisitorSimulator::SimulateVisitor() {
	UWorld* world = GetWorld();
	
	TArray<AShopActor*> shops;
	for (TActorIterator<AShopActor> It(world); It; ++It) shops.Add(*It);

	int32 num = shops.Num();
	if (num > 0) {
		int32 randomIndex = num > 1 ? (FMath::Rand() % num) : 0;
		shops[randomIndex]->OnVisit();
	}
}