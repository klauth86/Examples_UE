// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Graph/MSeqGraphNode.h"
#include "MSeqGraphNode_Root.generated.h"

UCLASS()
class UMSeqGraphNode_Root : public UMSeqGraphNode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere, Category = Debug)
		TArray<FString> DebugMessages;

	UPROPERTY()
		bool bHasDebugError;

	void LogDebugMessage(const FString& Message);
	void LogDebugError(const FString& Message);

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool HasErrors() const override { return false; }
};