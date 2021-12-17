// Fill out your copyright notice in the Description page of Project Settings.

#include "StreetTycoonPlayerController.h"
#include "Interface/Interactable.h"

AStreetTycoonPlayerController::AStreetTycoonPlayerController()
{
	bShowMouseCursor = true;
}

void AStreetTycoonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		if (Hit.Actor != HighlightedActorPtr)
		{
			if (auto interactable = Cast<IInteractable>(Hit.Actor.Get())) {
				interactable = 
			}
		}
	}
}