// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/BaseWidget.h"
#include "MainMenuWidget.generated.h"

class UServerButtonWidget;

UCLASS()
class PUZZLEPLATFORMS_API UMainMenuWidget : public UBaseWidget
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintImplementableEvent)
	void AddServerToJoin_BP(UWidget* widget);

	UFUNCTION(BlueprintImplementableEvent)
	void ClearServersToJoin_BP();

public:

	virtual void AddServerToJoin(const FString& serverName, int32 index) override;

	virtual void ClearServersToJoin() override { ClearServersToJoin_BP(); }

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
		void RefreshServersToJoin();

	UPROPERTY(EditDefaultsOnly, Category = "MainMenuWidget")
		TSubclassOf<UServerButtonWidget> ServerButtonWidgetClass;
};