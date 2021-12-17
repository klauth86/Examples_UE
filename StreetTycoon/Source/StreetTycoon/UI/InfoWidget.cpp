// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InfoWidget.h"
#include "Shops/ShopActor.h"
#include "ActionRouter.h"
void UInfoWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (AShopActor* shopActor = GetTypedOuter<AShopActor>()) {
		Refresh(shopActor);
	}

	ActionRouter::OnShopUpgraded.AddUObject(this, &UInfoWidget::OnShopUpgraded);
	ActionRouter::OnShopUpgraded.AddUObject(this, &UInfoWidget::OnShopVisit);
	ActionRouter::OnShopUpgraded.AddUObject(this, &UInfoWidget::OnShopPurchase);
}

void UInfoWidget::NativeDestruct() {
	ActionRouter::OnShopUpgraded.RemoveAll(this);
	ActionRouter::OnShopUpgraded.RemoveAll(this);
	ActionRouter::OnShopUpgraded.RemoveAll(this);

	Super::NativeDestruct();
}

void UInfoWidget::OnShopUpgraded(AShopActor* shopActor) {
	if (shopActor == GetTypedOuter<AShopActor>()) Refresh(shopActor);
}

void UInfoWidget::OnShopVisit(AShopActor* shopActor, int visits) {
	if (shopActor == GetTypedOuter<AShopActor>()) Refresh(shopActor);
}

void UInfoWidget::OnShopPurchase(AShopActor* shopActor, float balance) {
	if (shopActor == GetTypedOuter<AShopActor>()) Refresh(shopActor);
}