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

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable)
	void SetOwningShopActor(AShopActor* shopActor);

protected:

	void OnShopVisit(AShopActor* shopActor, int visits);

	void OnShopPurchase(AShopActor* shopActor, float balance);

	UFUNCTION(BlueprintCallable)
		void UpgradeShop(int32 index);

protected:

	UPROPERTY(BlueprintReadOnly)
		AShopActor* OwningShopActor;
};