// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

public:

	virtual void AddServerToJoin(int32 index) {}
};