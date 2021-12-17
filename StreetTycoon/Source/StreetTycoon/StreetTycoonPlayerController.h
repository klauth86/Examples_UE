// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "StreetTycoonPlayerController.generated.h"

UCLASS()
class STREETTYCOON_API AStreetTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AStreetTycoonPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:

	TWeakObjectPtr<AActor> HighlightedActorPtr;
	TWeakObjectPtr<AActor> InteractionActorPtr;
};