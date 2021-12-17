// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_OneParam(FInteractionEvent, class IInteractable*);

class STREETTYCOON_API IInteractable
{
	GENERATED_BODY()

public:

	static FInteractionEvent OnStartInteraction;

	static FInteractionEvent OnEndInteraction;

	virtual void StartInteract() { OnStartInteraction.Broadcast(this); }

	virtual void EndInteract() { OnEndInteraction.Broadcast(this); }

	virtual void SetIsHighlighted(bool) {}
};