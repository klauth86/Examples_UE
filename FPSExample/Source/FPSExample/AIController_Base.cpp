// Fill out your copyright notice in the Description page of Project Settings.

#include "AIController_Base.h"

AAIController_Base::AAIController_Base() {
	bSetControlRotationFromPawnOrientation = false;
}

void AAIController_Base::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	if (BehaviorTree) RunBehaviorTree(BehaviorTree);
}