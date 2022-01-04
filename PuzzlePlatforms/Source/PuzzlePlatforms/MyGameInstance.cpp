// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "UI/BaseWidget.h"
#include "ActionRouter.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

IOnlineSessionPtr SessionInterface;
TSharedPtr<FOnlineSessionSearch> SessionSearch;

void UMyGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	if (!NewWorld) return;

	if (NewWorld->RemovePIEPrefix(NewWorld->GetPathName()) == MainMenuMap.GetAssetPathString())
	{
		NewWorld->OnWorldBeginPlay.AddUObject(this, &UMyGameInstance::OnWorldBeginPlay);
	}
}

void UMyGameInstance::OnWorldBeginPlay()
{
	CurrentWidget = CreateWidget<UBaseWidget>(this, MainMenuWidgetClass);
	CurrentWidget->AddToViewport();

	UWorld* world = GetWorld();
	world->OnWorldBeginPlay.RemoveAll(this);
}

void UMyGameInstance::Init()
{
	Super::Init();

	ActionRouter::OnHost.BindUObject(this, &UMyGameInstance::Host);
	ActionRouter::OnJoin.BindUObject(this, &UMyGameInstance::Join);
	ActionRouter::OnLeave.BindUObject(this, &UMyGameInstance::Leave);
	ActionRouter::OnQuit.BindUObject(this, &UMyGameInstance::Quit);

	ActionRouter::OnWidgetConstruct.BindUObject(this, &UMyGameInstance::OnWidgetConstruct);
	ActionRouter::OnWidgetDestruct.BindUObject(this, &UMyGameInstance::OnWidgetDestruct);

	if (IOnlineSubsystem* oss = IOnlineSubsystem::Get())
	{
		SessionInterface = oss->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyGameInstance::OnJoinSessionComplete);
		}
	}
}

void UMyGameInstance::BeginDestroy()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnFindSessionsCompleteDelegates.RemoveAll(this);
		SessionInterface->OnJoinSessionCompleteDelegates.RemoveAll(this);
	}

	ActionRouter::OnHost.Unbind();
	ActionRouter::OnJoin.Unbind();
	ActionRouter::OnLeave.Unbind();
	ActionRouter::OnQuit.Unbind();

	ActionRouter::OnWidgetConstruct.Unbind();
	ActionRouter::OnWidgetDestruct.Unbind();

	OnWorldChanged(GetWorld(), nullptr);

	Super::BeginDestroy();
}

void UMyGameInstance::Host()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UMyGameInstance::Join(const FString& address)
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	GetFirstLocalPlayerController()->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UMyGameInstance::Leave()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	GetFirstLocalPlayerController()->ClientTravel("/Game/ThirdPersonCPP/Maps/LobbyMap", ETravelType::TRAVEL_Absolute);
}

void UMyGameInstance::Quit()
{
	GetFirstLocalPlayerController()->ConsoleCommand("quit");
}

void UMyGameInstance::OnWidgetConstruct()
{
	if (APlayerController* pc = GetFirstLocalPlayerController())
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(CurrentWidget->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		pc->SetInputMode(InputModeData);
		pc->bShowMouseCursor = true;
	}
}

void UMyGameInstance::OnWidgetDestruct()
{
	if (APlayerController* pc = GetFirstLocalPlayerController())
	{
		FInputModeGameOnly InputModeData;

		pc->SetInputMode(InputModeData);
		pc->bShowMouseCursor = false;
	}
}

void UMyGameInstance::ToggleGameMenu()
{
	UWorld* world = GetWorld();

	if (world->RemovePIEPrefix(world->GetPathName()) == MainMenuMap.GetAssetPathString()) return;

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}
	else
	{
		CurrentWidget = CreateWidget<UBaseWidget>(this, GameMenuWidgetClass);
		CurrentWidget->AddToViewport();
	}
}

void UMyGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{

}

void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{

}

void UMyGameInstance::OnFindSessionsComplete(bool Success)
{

}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{

}