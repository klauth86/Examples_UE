// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Move.generated.h"

#if WITH_EDITORONLY_DATA
class UEdGraph;
#endif // WITH_EDITORONLY_DATA

UCLASS()
class MOVEGRAPH_API UMove : public UObject
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
		UPROPERTY()
		UEdGraph* EdGraph;
#endif
};