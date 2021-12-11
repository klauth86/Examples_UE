// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AIController_Base.generated.h"

class UBehaviorTree;

UCLASS()
class FPSEXAMPLE_API AAIController_Base : public AAIController
{
	GENERATED_BODY()
	
public:
	
	AAIController_Base();

	virtual void OnPossess(APawn* pawn) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UBehaviorTree* BehaviorTree;
};