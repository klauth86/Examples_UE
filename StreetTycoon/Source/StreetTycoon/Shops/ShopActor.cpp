// Fill out your copyright notice in the Description page of Project Settings.

#include "Shops/ShopActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "ActionRouter.h"
#include "UI/InfoWidget.h"

AShopActor::AShopActor() {
	RootComponent = StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TransactionChance = 0.2f;
	AverageTransaction = 10;
	Visits = 0;
	Balance = 0;
}

void AShopActor::SetIsHighlighted(bool isHighlighted) { 
	StaticMeshComponent->SetRenderCustomDepth(isHighlighted);	// Visualize with Post Process
	WidgetComponent->SetWidgetClass(isHighlighted ? InfoWidgetClass : nullptr);
}

void AShopActor::OnVisit() {
	Visits++;	
	ActionRouter::OnShopVisitChanged.Broadcast(this, Visits);

	if (FMath::FRand() < TransactionChance) {
		Balance += AverageTransaction;
		ActionRouter::OnShopBalanceChanged.Broadcast(this, AverageTransaction);
	}
}