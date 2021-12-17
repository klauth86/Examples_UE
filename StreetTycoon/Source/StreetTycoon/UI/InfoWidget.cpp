// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InfoWidget.h"
#include "Shops/ShopActor.h"
#include "ActionRouter.h"

void UInfoWidget::NativeDestruct() {
	ActionRouter::OnShopPurchase.RemoveAll(this);
	ActionRouter::OnShopVisit.RemoveAll(this);
	ActionRouter::OnShopUpgraded.RemoveAll(this);

	Super::NativeDestruct();
}

void UInfoWidget::SetOwningShopActor(AShopActor* shopActor) {
	OwningShopActor = shopActor;

	Refresh();

	ActionRouter::OnShopUpgraded.AddUObject(this, &UInfoWidget::OnShopUpgraded);
	ActionRouter::OnShopVisit.AddUObject(this, &UInfoWidget::OnShopVisit);
	ActionRouter::OnShopPurchase.AddUObject(this, &UInfoWidget::OnShopPurchase);
}

void UInfoWidget::OnShopUpgraded(AShopActor* shopActor) {
	if (shopActor == OwningShopActor) Refresh();
}

void UInfoWidget::OnShopVisit(AShopActor* shopActor, int visits) {
	if (shopActor == OwningShopActor) Refresh();
}

void UInfoWidget::OnShopPurchase(AShopActor* shopActor, float balance) {
	if (shopActor == OwningShopActor) Refresh();
}