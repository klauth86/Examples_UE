// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/DetailsWidget.h"
#include "ActionRouter.h"

void UDetailsWidget::UpgradeShop(int32 index) {
	ActionRouter::OnUpgradeShop.Broadcast(OwningShopActor, index);
}