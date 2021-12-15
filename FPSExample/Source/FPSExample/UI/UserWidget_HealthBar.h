// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "UserWidget_HealthBar.generated.h"

class ABasicCharacter;

UCLASS()
class FPSEXAMPLE_API UUserWidget_HealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void OnSetCharacter();

	void SetCharacter(ABasicCharacter* character) { Character = character; OnSetCharacter(); }

protected:

	UPROPERTY()
		ABasicCharacter* Character;
};