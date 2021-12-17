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

	if (Hit.bBlockingHit && HighlightedActorPtr != Hit.Actor && InteractionActorPtr != Hit.Actor) SetHighlightedActor(Hit.Actor);
}

void AStreetTycoonPlayerController::SetHighlightedActor(const TWeakObjectPtr<AActor>& actorPtr)
{
	if (auto interactable = Cast<IInteractable>(HighlightedActorPtr.Get()))
	{
		interactable->SetIsHighlighted(false);
		HighlightedActorPtr.Reset();
	}

	if (auto interactable = Cast<IInteractable>(actorPtr.Get()))
	{
		HighlightedActorPtr = actorPtr;
		interactable->SetIsHighlighted(true);
	}
}

void AStreetTycoonPlayerController::SetInteractionActor()
{
	if (auto interactable = Cast<IInteractable>(InteractionActorPtr.Get()))
	{
		interactable->EndInteract();
		InteractionActorPtr.Reset();
	}

	if (auto interactable = Cast<IInteractable>(HighlightedActorPtr.Get()))
	{
		InteractionActorPtr = HighlightedActorPtr;
		interactable->StartInteract();
	}
}