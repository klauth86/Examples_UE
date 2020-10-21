// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OwnerMulticasterComponentBase.h"
#include "TestComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OWNERMULTICASTER_API UTestComponent : public UActorComponent, public IOwnerMulticasterComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type endPlayReason) override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual UActorComponent* GetSelfAsActorComponent() override { return this; };

	UFUNCTION(BlueprintCallable)
		void TestAsPawn();

	UFUNCTION(BlueprintCallable)
		void TestAsActor();
};
