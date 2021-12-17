// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"

AShopActor::AShopActor() {
	RootComponent = StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AShopActor::StartInteract() {

}

void AShopActor::EndInteract() {

}

void AShopActor::AddHighlight(EHighlightMode highlightMode) {
	if (HighlightMode > highlightMode) return;

	HighlightMode = highlightMode;

	RefreshHighlights();
}

void AShopActor::RemoveHighlight(EHighlightMode highlightMode) {
	if (HighlightMode > highlightMode) return;

	HighlightMode = EHighlightMode::UNSET;

	RefreshHighlights();
}

void AShopActor::RefreshHighlights() {
	StaticMeshComponent->SetRenderCustomDepth(HighlightMode == EHighlightMode::HOVER);

	// TODO SELECTED MODE
}