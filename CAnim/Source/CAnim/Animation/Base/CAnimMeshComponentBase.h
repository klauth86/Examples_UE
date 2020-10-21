// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkinnedMeshComponent.h"
#include "CAnimMeshComponentBase.generated.h"

class UCAnimInstanceBase;

/**
 * 
 */
UCLASS()
class CANIM_API UCAnimMeshComponentBase : public USkinnedMeshComponent {

	GENERATED_BODY()

#pragma region Parallel Anim

public:

	virtual void ParallelAnimationEvaluation() {}

	virtual void CompleteParallelAnimationEvaluation(bool bDoPostAnimEvaluation) {}

	virtual bool HasValidEvaluationTask() const { return false; }

#pragma endregion

public:
	virtual UCAnimInstanceBase* GetAnimInstance() const { return NULL; }

};
