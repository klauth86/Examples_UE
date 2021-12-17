// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class STREETTYCOON_API IInteractable
{
	GENERATED_BODY()

public:

	virtual void Interact() {}
};
