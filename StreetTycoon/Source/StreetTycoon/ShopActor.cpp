// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

AShopActor::AShopActor() {
	RootComponent = StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TransactionChance = 0.2f;
	AverageTransaction = 10;
	Balance = 0;
}

void AShopActor::StartInteract() {

}

void AShopActor::EndInteract() {

}

void AShopActor::SetIsHighlighted(bool isHighlighted) { StaticMeshComponent->SetRenderCustomDepth(isHighlighted); } // Visualize with Post Process