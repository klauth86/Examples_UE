// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/BaseWidget.h"
#include "ActionRouter.h"

void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ActionRouter::OnWidgetConstruct.ExecuteIfBound();
}

void UBaseWidget::NativeDestruct()
{
	ActionRouter::OnWidgetDestruct.ExecuteIfBound();

	Super::NativeDestruct();
}