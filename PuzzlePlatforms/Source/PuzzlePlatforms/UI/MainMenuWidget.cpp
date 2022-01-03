// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenuWidget.h"
#include "ActionRouter.h"

void UMainMenuWidget::Host() { ActionRouter::OnHost.ExecuteIfBound(); }

void UMainMenuWidget::Join(const FText& addressText) { ActionRouter::OnJoin.ExecuteIfBound(addressText.ToString()); }

void UMainMenuWidget::Quit() { ActionRouter::OnQuit.ExecuteIfBound(); }