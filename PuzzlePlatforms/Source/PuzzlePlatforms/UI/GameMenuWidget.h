// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/BaseWidget.h"
#include "GameMenuWidget.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UGameMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Cancel();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Leave();
};