// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "JoystickInput.h"
#include "MoveSequence.generated.h"

class UFightAction;
class UEdGraph;

USTRUCT(BlueprintType)
struct MOVECOREMODULE_API FMoveSequenceElement
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
		FJoystickInput ActivationInput;

	UPROPERTY(EditAnywhere)
		UFightAction* FightAction;
};

UCLASS()
class MOVECOREMODULE_API UMoveSequence : public UObject
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	UPROPERTY()
		UEdGraph* EdGraph;
#endif

protected:


};