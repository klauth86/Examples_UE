// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "StreetTycoonPlayerController.generated.h"

class UDetailsWidget;

UCLASS()
class STREETTYCOON_API AStreetTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AStreetTycoonPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void SetHighlightedActor(const TWeakObjectPtr<AActor>& actorPtr);

	void SetInteractionActor();

protected:

	TWeakObjectPtr<AActor> HighlightedActorPtr;

	TWeakObjectPtr<AActor> InteractionActorPtr;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<UDetailsWidget> DetailsWidgetClass;
};