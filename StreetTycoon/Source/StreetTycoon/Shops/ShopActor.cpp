// Fill out your copyright notice in the Description page of Project Settings.

#include "Shops/ShopActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ActionRouter.h"
#include "FloatingTextActor.h"
#include "IndicatorActor.h"
#include "UI/InfoWidget.h"
#include "EngineUtils.h"

float MaxBalance = 0;

AShopActor::AShopActor() {
	RootComponent = StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TransactionChance = 0.2f;
	AverageTransaction = 10;
}

void AShopActor::BeginPlay() {
	Super::BeginPlay();

	WidgetComponent->SetWidgetClass(InfoWidgetClass);	
	if (UInfoWidget* infoWidget = Cast<UInfoWidget>(WidgetComponent->GetWidget())) infoWidget->SetOwningShopActor(this);
}

void AShopActor::SetIsHighlighted(bool isHighlighted) { 	
	StaticMeshComponent->SetRenderCustomDepth(isHighlighted);	// Visualize with Post Process

	UUserWidget* infoWidget = WidgetComponent->GetWidget();
	if (infoWidget) infoWidget->SetVisibility(isHighlighted ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void AShopActor::OnVisit() {
	ShopStat.Visits++;	
	ActionRouter::OnShopVisit.Broadcast(this, ShopStat.Visits);

	UWorld* world = GetWorld();

	if (FMath::FRand() < TransactionChance) {
		ShopStat.Purchases++;
		ShopStat.Balance += AverageTransaction;

		if (ShopStat.Balance > MaxBalance) {
			MaxBalance = ShopStat.Balance;

			MarkAsLeader(world);
		}

		if (FloatingTextActorClass) {
			world->SpawnActor<AFloatingTextActor>(FloatingTextActorClass, GetActorTransform());
		}

		ActionRouter::OnShopPurchase.Broadcast(this, AverageTransaction);
	}
}

void AShopActor::MarkAsLeader(UWorld* world) {

	TArray<AIndicatorActor*> indicators;
	for (TActorIterator<AIndicatorActor> It(world); It; ++It) indicators.Add(*It);
	
	AIndicatorActor* indicator = nullptr;

	if (indicators.Num() > 0)
	{
		indicator = indicators[0];
	}

	if (indicator && (!IndicatorActorClass || IndicatorActorClass != indicator->GetClass()))
	{
		indicator->Destroy();
		indicator = nullptr;
	}

	if (indicator) {
		indicator->SetActorTransform(GetActorTransform());
	}
	else if (IndicatorActorClass)
	{
		world->SpawnActor<AIndicatorActor>(IndicatorActorClass, GetActorTransform());
	}
}