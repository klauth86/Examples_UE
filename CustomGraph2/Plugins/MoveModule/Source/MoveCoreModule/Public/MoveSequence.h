// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "JoystickInput.h"
#include "MoveSequence.generated.h"

class UFightAction;
class UEdGraph;

USTRUCT()
struct MOVECOREMODULE_API FActionsGraphTransition
{
	GENERATED_USTRUCT_BODY()

public:

	int32 TargetIndex;

	FJoystickInput TransitionInput;
};

USTRUCT()
struct MOVECOREMODULE_API FActionsGraphNode
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere)
		UFightAction* FightAction;

	UPROPERTY(EditAnywhere)
		TArray<FActionsGraphTransition> Transitions;
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

	TMap<int32, FActionsGraphNode>& GetActionsGraph() { return ActionsGraph; }

protected:

	TMap<int32, FActionsGraphNode> ActionsGraph;
};