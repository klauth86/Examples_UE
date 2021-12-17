// Fill out your copyright notice in the Description page of Project Settings.

#include "StreetTycoonPlayerController.h"
#include "Interface/Interactable.h"
#include "UI/InfoWidget.h"
#include "Shops/ShopActor.h"
#include "ActionRouter.h"

AStreetTycoonPlayerController::AStreetTycoonPlayerController()
{
	bShowMouseCursor = true;
}

void AStreetTycoonPlayerController::BeginPlay() {
	Super::BeginPlay();

	ActionRouter::OnUpgradeShop.AddUObject(this, &AStreetTycoonPlayerController::OnUpgradeShop);
}

void AStreetTycoonPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	ActionRouter::OnUpgradeShop.RemoveAll(this);

	Super::EndPlay(EndPlayReason);
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
	if (UInfoWidget* infoWidget = GetInfoWidget()) {		
		AShopActor* shopActor = Cast<AShopActor>(HighlightedActorPtr.Get());
		infoWidget->SetOwningShopActor(shopActor);
		infoWidget->SetVisibility(shopActor ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

UInfoWidget* AStreetTycoonPlayerController::GetInfoWidget()
{
	if (!InfoWidget && InfoWidgetClass)
	{
		InfoWidget = CreateWidget<UInfoWidget>(this, InfoWidgetClass);
		if (InfoWidget) InfoWidget->AddToViewport();
	}

	return InfoWidget;
}

void AStreetTycoonPlayerController::OnUpgradeShop(AShopActor* shopActor, int32 index)
{
	const TMap<TSubclassOf<AShopActor>, float>& upgrades = shopActor->GetUpgrades();
	TArray<TSubclassOf<AShopActor>> keys;
	upgrades.GetKeys(keys);

	if (keys.IsValidIndex(index))
	{
		FTransform spawnTransform = shopActor->GetActorTransform();

		TSubclassOf<AShopActor> upgradeClass = keys[index];

		FShopStat shopStat;
		shopStat.Reset(shopActor->GetShopStat());

		float upgradeCost = upgrades[upgradeClass];
		shopStat.Balance -= upgradeCost;

		if (UInfoWidget* infoWidget = GetInfoWidget())
		{
			infoWidget->SetOwningShopActor(nullptr);
			infoWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		shopActor->Destroy();

		if (AShopActor* upgradedShopActor = GetWorld()->SpawnActor<AShopActor>(upgradeClass, spawnTransform))
		{
			upgradedShopActor->SetShopStat(shopStat);

			if (UInfoWidget* infoWidget = GetInfoWidget())
			{
				infoWidget->SetOwningShopActor(upgradedShopActor);
				infoWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}