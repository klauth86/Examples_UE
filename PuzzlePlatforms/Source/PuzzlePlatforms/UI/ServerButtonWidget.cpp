// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/ServerButtonWidget.h"
#include "ActionRouter.h"

void UServerButtonWidget::Join()
{
	ActionRouter::OnJoinOSS.ExecuteIfBound(Index);
}