// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Interactable.generated.h"

enum class EHighlightMode :uint8
{
	UNSET = 0,
	HOVER,
	SELECTED
};

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class STREETTYCOON_API IInteractable
{
	GENERATED_BODY()

public:

	virtual void StartInteract() {}

	virtual void EndInteract() {}

	virtual void AddHighlight(EHighlightMode highlightMode) {}

	virtual void RemoveHighlight(EHighlightMode highlightMode) {}
};