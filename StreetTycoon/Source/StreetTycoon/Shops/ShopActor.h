// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "ShopActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;
class UInfoWidget;
class AFloatingTextActor;
class AIndicatorActor;

USTRUCT(BlueprintType)
struct FShopStat
{
	GENERATED_USTRUCT_BODY()

public:

	FShopStat() {
		Visits = 0;
		Purchases = 0;
		Balance = 0;
	}

	void Reset(const FShopStat& shopStat) {
		Visits = shopStat.Visits;
		Purchases = shopStat.Purchases;
		Balance = shopStat.Balance;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopStat", meta = (ClampMin = "0.0", UIMin = "0.0"))
		int Visits;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopStat", meta = (ClampMin = "0.0", UIMin = "0.0"))
		int Purchases;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopStat", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float Balance;
};

UCLASS(ABSTRACT)
class STREETTYCOON_API AShopActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AShopActor();

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

	virtual void BeginPlay() override;

	virtual void SetIsHighlighted(bool) override;

	void OnVisit();

	const TMap<TSubclassOf<AShopActor>, float>& GetUpgrades() const { return Upgrades; }

	const FShopStat& GetShopStat() const { return ShopStat; }

	void SetShopStat(const FShopStat& shopStat) { ShopStat.Reset(shopStat); }

	void MarkAsLeader(UWorld* world);

protected:

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<AFloatingTextActor> FloatingTextActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<AIndicatorActor> IndicatorActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "ShopActor")
		TSubclassOf<UInfoWidget> InfoWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopActor")
		FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopActor")
		TMap<TSubclassOf<AShopActor>, float> Upgrades;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "0.8", UIMax = "0.8"))
		float TransactionChance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ShopActor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float AverageTransaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShopActor")
		FShopStat ShopStat;
};