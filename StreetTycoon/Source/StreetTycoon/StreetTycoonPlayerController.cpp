// Fill out your copyright notice in the Description page of Project Settings.

#include "StreetTycoonPlayerController.h"
#include "Interface/Interactable.h"
#include "UI/DetailsWidget.h"
#include "Shops/ShopActor.h"

AStreetTycoonPlayerController::AStreetTycoonPlayerController()
{
	bShowMouseCursor = true;
}

void AStreetTycoonPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit && HighlightedActorPtr != Hit.Actor) SetHighlightedActor(Hit.Actor);
}

void AStreetTycoonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SetInteraction", IE_Pressed, this, &AStreetTycoonPlayerController::SetInteractionActor);
}

void AStreetTycoonPlayerController::SetHighlightedActor(const TWeakObjectPtr<AActor>& actorPtr)
{
	if (IInteractable* interactable = Cast<IInteractable>(HighlightedActorPtr.Get()))
	{
		interactable->SetIsHighlighted(false);
		HighlightedActorPtr.Reset();
	}

	if (IInteractable* interactable = Cast<IInteractable>(actorPtr.Get()))
	{
		HighlightedActorPtr = actorPtr;
		interactable->SetIsHighlighted(true);
	}
}

void AStreetTycoonPlayerController::SetInteractionActor()
{
	if (UDetailsWidget* detailsWidget = GetDetailsWidget()) {
		detailsWidget->SetOwningShopActor(Cast<AShopActor>(HighlightedActorPtr.Get()));
	}
}

UDetailsWidget* AStreetTycoonPlayerController::GetDetailsWidget() {
	if (!DetailsWidget && DetailsWidgetClass) {
		DetailsWidget = CreateWidget<UDetailsWidget>(this, DetailsWidgetClass);
		if (DetailsWidget) DetailsWidget->AddToViewport();
	}

	return DetailsWidget;
}