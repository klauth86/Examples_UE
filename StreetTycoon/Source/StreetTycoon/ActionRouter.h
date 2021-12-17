// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

class AShopActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FShopUpgradedEvent, AShopActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FShopVisitEvent, AShopActor*, int);
DECLARE_MULTICAST_DELEGATE_TwoParams(FShopPurchaseEvent, AShopActor*, float);

namespace ActionRouter
{
	extern FShopUpgradedEvent OnShopUpgraded;
	extern FShopVisitEvent OnShopVisit;
	extern FShopPurchaseEvent OnShopPurchase;
};