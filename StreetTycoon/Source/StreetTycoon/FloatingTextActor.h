// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "FloatingTextActor.generated.h"

class UWidgetComponent;

UCLASS()
class STREETTYCOON_API AFloatingTextActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AFloatingTextActor();

	UWidgetComponent* GetWidgetComponent() const { return WidgetComponent; }

public:	

	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Floating Text Actor", meta = (AllowPrivateAccess = "true"))
		UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = "Floating Text Actor", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float BlowRate;
};