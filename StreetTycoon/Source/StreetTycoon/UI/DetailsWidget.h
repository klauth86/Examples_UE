// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/InfoWidget.h"
#include "DetailsWidget.generated.h"

UCLASS()
class STREETTYCOON_API UDetailsWidget : public UInfoWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
		void UpgradeShop(int32 index) {}
};