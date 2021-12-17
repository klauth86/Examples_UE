// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ObjectMacros.h"

class AShopActor;

DECLARE_MULTICAST_DELEGATE_OneParam(FShopVisitedEvent, AShopActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FShopUpgradedEvent, AShopActor*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FShopBalanceChangedEvent, AShopActor*, float);

namespace ActionRouter
{
	extern FShopVisitedEvent OnShopVisited;
	extern FShopUpgradedEvent OnShopUpgraded;
	extern FShopBalanceChanged OnShopBalanceChanged;
};