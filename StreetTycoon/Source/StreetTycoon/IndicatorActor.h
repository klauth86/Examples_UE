// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "IndicatorActor.generated.h"

class UBillboardComponent;

UCLASS()
class STREETTYCOON_API AIndicatorActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AIndicatorActor();

public:	

	UBillboardComponent* GetBillboardComponent() const { return BillboardComponent; }

protected:

	UPROPERTY(VisibleAnywhere, Category = "Indicator Actor", meta = (AllowPrivateAccess = "true"))
		UBillboardComponent* BillboardComponent;
};