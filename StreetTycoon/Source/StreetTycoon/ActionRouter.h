// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

class AShopActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FShopVisitedEvent, AShopActor*);
typedef FShopVisitedEvent FShopUpgradedEvent;
DECLARE_MULTICAST_DELEGATE_TwoParams(FShopVisitChangedEvent, AShopActor*, int);
typedef FShopVisitChangedEvent FShopPurchasesChangedEvent;
DECLARE_MULTICAST_DELEGATE_TwoParams(FShopBalanceChangedEvent, AShopActor*, float);

namespace ActionRouter
{
	extern FShopVisitedEvent OnShopVisited;
	extern FShopUpgradedEvent OnShopUpgraded;
	extern FShopVisitChangedEvent OnShopVisitChanged;
	extern FShopPurchasesChangedEvent OnShopPurchasesChanged;
	extern FShopBalanceChangedEvent OnShopBalanceChanged;
};