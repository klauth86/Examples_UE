// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "ServerButtonWidget.generated.h"

class UButton;

UCLASS()
class PUZZLEPLATFORMS_API UServerButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UButton* GetButton() const { return Button; }

protected:

	UPROPERTY(meta = (BindWidget))
		UButton* Button;
};