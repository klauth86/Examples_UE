// Fill out your copyright notice in the Description page of Project Settings.


#include "TestComponent.h"
#include "GameFramework/Pawn.h"

// Sets default values for this component's properties
UTestComponent::UTestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UTestComponent::EndPlay(EEndPlayReason::Type endPlayReason) {
	Super::EndPlay(endPlayReason);

	OnComponentEndPlay.Broadcast();
}

// Called every frame
void UTestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto actor = GetOwnerAs<AActor>();
}

void UTestComponent::TestAsPawn() {
	auto pawn = GetOwnerAs<APawn>();
}

void UTestComponent::TestAsActor() {
	auto actor = GetOwnerAs<AActor>();
}
