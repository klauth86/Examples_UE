// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ServerButtonWidget.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UServerButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	UFUNCTION(BlueprintCallable, Category = "ServerButtonWidget")
		void Join();

	UFUNCTION(BlueprintImplementableEvent)
		void Init_BP(const FString& serverName);

public:

	void Init(const FString& serverName, int32 index) { Index = index; Init_BP(serverName); }

protected:

	int32 Index;
};