// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/BaseWidget.h"
#include "MainMenuWidget.generated.h"

class UServerButtonWidget;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void AddServerToJoin(UWidget* widget);

	virtual void AddServerToJoin(int32 index) override;

protected:

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Host();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Join(const FText& addressText);

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void Quit();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void HostOSS();

	UFUNCTION(BlueprintCallable, Category = "MainMenuWidget")
		void JoinOSS(int32 index);

	UPROPERTY(EditDefaultsOnly, Category = "MyGameInstance")
		TSubclassOf<UServerButtonWidget> ServerButtonWidgetClass;
};