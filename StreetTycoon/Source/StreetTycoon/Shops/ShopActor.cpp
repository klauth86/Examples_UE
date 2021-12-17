// Fill out your copyright notice in the Description page of Project Settings.

#include "Shops/ShopActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ActionRouter.h"
#include "FloatingTextActor.h"
#include "UI/InfoWidget.h"

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

	if (FMath::FRand() < TransactionChance) {
		ShopStat.Purchases++;
		ShopStat.Balance += AverageTransaction;

		if (FloatingTextActorClass) {
			GetWorld()->SpawnActor<AFloatingTextActor>(FloatingTextActorClass, GetActorTransform());
		}

		ActionRouter::OnShopPurchase.Broadcast(this, AverageTransaction);
	}
}