// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InfoWidget.h"
#include "Shops/ShopActor.h"
#include "ActionRouter.h"

void UInfoWidget::NativeConstruct() {
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UInfoWidget::NativeDestruct() {
	ActionRouter::OnShopPurchase.RemoveAll(this);
	ActionRouter::OnShopVisit.RemoveAll(this);

	Super::NativeDestruct();
}

void UInfoWidget::SetOwningShopActor(AShopActor* shopActor) {
	OwningShopActor = shopActor;

	if (shopActor) {
		Refresh();

		ActionRouter::OnShopVisit.AddUObject(this, &UInfoWidget::OnShopVisit);
		ActionRouter::OnShopPurchase.AddUObject(this, &UInfoWidget::OnShopPurchase);
	}
	else {
		ActionRouter::OnShopPurchase.RemoveAll(this);
		ActionRouter::OnShopVisit.RemoveAll(this);
	}
}

void UInfoWidget::OnShopVisit(AShopActor* shopActor, int visits) {
	if (shopActor == OwningShopActor) Refresh();
}

void UInfoWidget::OnShopPurchase(AShopActor* shopActor, float balance) {
	if (shopActor == OwningShopActor) Refresh();
}

void UInfoWidget::UpgradeShop(int32 index)
{
	ActionRouter::OnUpgradeShop.Broadcast(OwningShopActor, index);
}