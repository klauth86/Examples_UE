// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/BaseWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UMainMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Host();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Join(const FText& addressText);

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Quit();
};