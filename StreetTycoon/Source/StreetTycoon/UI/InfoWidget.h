// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "InfoWidget.generated.h"

class AShopActor;

UCLASS()
class STREETTYCOON_API UInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent)
		void Refresh();

	virtual void NativeDestruct() override;

	void SetOwningShopActor(AShopActor* shopActor);

protected:

	void OnShopUpgraded(AShopActor* shopActor);

	void OnShopVisit(AShopActor* shopActor, int visits);

	void OnShopPurchase(AShopActor* shopActor, float balance);

protected:

	UPROPERTY(BlueprintReadOnly)
		AShopActor* OwningShopActor;
};