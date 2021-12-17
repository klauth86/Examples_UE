// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "ShopActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UInfoWidget;

UCLASS(ABSTRACT)
class STREETTYCOON_API AShopActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AShopActor();

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

	virtual void SetIsHighlighted(bool) override;

	void OnVisit();

protected:

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<UInfoWidget> InfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TMap<TSubclassOf<AShopActor>, float> Upgrades;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.8", UIMax = "0.8"))
		float TransactionChance;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float AverageTransaction;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		int Visits;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		int Purchases;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float Balance;
};