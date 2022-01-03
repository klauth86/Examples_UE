// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/GameMenuWidget.h"
#include "ActionRouter.h"

void UGameMenuWidget::Cancel() { RemoveFromViewport(); }

void UGameMenuWidget::Leave() { ActionRouter::OnLeave.ExecuteIfBound(); }