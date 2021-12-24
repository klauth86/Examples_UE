// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "MSeqGraphSchema.generated.h"

UCLASS()
class MOVECOREEDITORMODULE_API UMSeqGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;

	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* PinA, const UEdGraphPin* PinB) const;

	virtual void DroppedAssetsOnGraph(const TArray<struct FAssetData>& Assets, const FVector2D& GraphPosition, UEdGraph* Graph) const override;
};