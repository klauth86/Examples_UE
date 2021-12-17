// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "StreetTycoonPlayerController.generated.h"

class UInfoWidget;
class AShopActor;

UCLASS()
class STREETTYCOON_API AStreetTycoonPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	AStreetTycoonPlayerController();

	virtual void BeginPlay();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void PlayerTick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

protected:

	void SetHighlightedActor(const TWeakObjectPtr<AActor>& actorPtr);

	void SetInteractionActor();

	UInfoWidget* GetInfoWidget();

	void OnUpgradeShop(AShopActor* shopActor, int32 index);

protected:

	TWeakObjectPtr<AActor> HighlightedActorPtr;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<UInfoWidget> InfoWidgetClass;

	UPROPERTY()
		UInfoWidget* InfoWidget;
};