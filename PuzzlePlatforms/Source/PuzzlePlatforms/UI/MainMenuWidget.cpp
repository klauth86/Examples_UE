// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenuWidget.h"
#include "UI/ServerButtonWidget.h"
#include "ActionRouter.h"

void UMainMenuWidget::AddServerToJoin(const FString& serverName, int32 index)
{
	if (ServerButtonWidgetClass)
	{
		if (UServerButtonWidget* serverButton = CreateWidget<UServerButtonWidget>(this, ServerButtonWidgetClass))
		{
			serverButton->Init(serverName, index);
			AddServerToJoin_BP(serverButton);
		}
	}
}

void UMainMenuWidget::Host() { ActionRouter::OnHost.ExecuteIfBound(); }

void UMainMenuWidget::Join(const FText& addressText) { ActionRouter::OnJoin.ExecuteIfBound(addressText.ToString()); }

void UMainMenuWidget::Quit() { ActionRouter::OnQuit.ExecuteIfBound(); }

void UMainMenuWidget::HostOSS() { ActionRouter::OnHostOSS.ExecuteIfBound(); }

void UMainMenuWidget::RefreshServersToJoin() { ActionRouter::RefreshServersToJoin.ExecuteIfBound(); }