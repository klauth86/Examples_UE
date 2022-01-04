// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenuWidget.h"
#include "UI/ServerButtonWidget.h"
#include "ActionRouter.h"

#include "Components/Button.h"

void UMainMenuWidget::AddServerToJoin(int32 index)
{
	if (ServerButtonWidgetClass)
	{
		if (UServerButtonWidget* serverButton = CreateWidget<UServerButtonWidget>(this, ServerButtonWidgetClass))
		{
			if (UButton* button = serverButton->GetButton())
			{
				button->OnClicked.AddDynamic(this, &UMainMenuWidget::JoinOSS, index);
			}
		}
	}
}

void UMainMenuWidget::Host() { ActionRouter::OnHost.ExecuteIfBound(); }

void UMainMenuWidget::Join(const FText& addressText) { ActionRouter::OnJoin.ExecuteIfBound(addressText.ToString()); }

void UMainMenuWidget::Quit() { ActionRouter::OnQuit.ExecuteIfBound(); }

void UMainMenuWidget::HostOSS() { ActionRouter::OnHostOSS.ExecuteIfBound(); }

void UMainMenuWidget::JoinOSS(int32 index) { ActionRouter::OnJoinOSS.ExecuteIfBound(index); }