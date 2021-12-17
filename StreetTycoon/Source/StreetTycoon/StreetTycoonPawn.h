// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "StreetTycoonPawn.generated.h"

class UCameraComponent;

UCLASS()
class STREETTYCOON_API AStreetTycoonPawn : public APawn
{
	GENERATED_BODY()

public:

	AStreetTycoonPawn();

	UCameraComponent* GetCameraComponent() const { return CameraComponent; }

protected:

	UPROPERTY(VisibleAnywhere, Category = "ShopActor", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
};