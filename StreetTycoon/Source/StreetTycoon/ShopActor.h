// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "ShopActor.generated.h"

class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class STREETTYCOON_API AShopActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	AShopActor();

	UStaticMeshComponent* GetStaticMeshComponent() const { return StaticMeshComponent; }

	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

	virtual void StartInteract() override;

	virtual void EndInteract() override;

	virtual void SetIsHighlighted(bool) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;
};