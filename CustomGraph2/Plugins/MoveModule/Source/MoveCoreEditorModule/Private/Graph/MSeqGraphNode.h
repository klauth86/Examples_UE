// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EdGraph/EdGraphNode.h"
#include "MSeqGraphNode.generated.h"

UCLASS()
class MOVECOREEDITORMODULE_API UMSeqGraphNode : public UEdGraphNode
{
	GENERATED_BODY()	

protected:

	UPROPERTY()
		FString ErrorMessage;

	UPROPERTY()
		uint32 bIsReadOnly : 1;

	virtual bool HasErrors() const { return ErrorMessage.Len() > 0; }

	virtual FText GetDescription() const { return FText::GetEmpty(); }
};