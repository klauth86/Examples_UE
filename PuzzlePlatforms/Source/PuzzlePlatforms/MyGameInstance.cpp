// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "UI/BaseWidget.h"
#include "ActionRouter.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

IOnlineSessionPtr SessionInterface;
TSharedPtr<FOnlineSessionSearch> SessionSearch;

const FName SESSION_NAME = "GAME";
const FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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

	ActionRouter::OnHostOSS.BindUObject(this, &UMyGameInstance::HostOSS);
	ActionRouter::OnJoinOSS.BindUObject(this, &UMyGameInstance::JoinOSS);

	ActionRouter::RefreshServersToJoin.BindUObject(this, &UMyGameInstance::FindSessionsOSS);

	ActionRouter::OnStartGame.BindUObject(this, &UMyGameInstance::OnStartGame);

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

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UMyGameInstance::OnNetworkFailure);
	}
}

void UMyGameInstance::BeginDestroy()
{
	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().RemoveAll(this);
	}

	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnFindSessionsCompleteDelegates.RemoveAll(this);
		SessionInterface->OnJoinSessionCompleteDelegates.RemoveAll(this);
	}

	SessionInterface.Reset();

	ActionRouter::OnHost.Unbind();
	ActionRouter::OnJoin.Unbind();
	ActionRouter::OnLeave.Unbind();
	ActionRouter::OnQuit.Unbind();

	ActionRouter::OnHostOSS.Unbind();
	ActionRouter::OnJoinOSS.Unbind();

	ActionRouter::RefreshServersToJoin.Unbind();

	ActionRouter::OnStartGame.Unbind();
	
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

void UMyGameInstance::HostOSS()
{
	if (SessionInterface.IsValid())
	{
		if (FNamedOnlineSession* existingSession = SessionInterface->GetNamedSession(SESSION_NAME))
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSessionOSS();
		}
	}
}

void UMyGameInstance::JoinOSS(int32 index)
{
	if (!SessionInterface.IsValid()) return;
	
	if (!SessionSearch.IsValid()) return;

	if (!SessionSearch->SearchResults.IsValidIndex(index)) return;

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[index]);
}

void UMyGameInstance::CreateSessionOSS()
{
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	//SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, "", EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UMyGameInstance::FindSessionsOSS()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	
	if (SessionSearch.IsValid())
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UMyGameInstance::OnStartGame() {
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
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
	if (SESSION_NAME != SessionName) return;

	if (!Success) return;

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	GetWorld()->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
}

void UMyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (SESSION_NAME != SessionName) return;

	if (!Success) return;

	CreateSessionOSS();
}

void UMyGameInstance::OnFindSessionsComplete(bool Success)
{
	UWorld* world = GetWorld();

	if (world->RemovePIEPrefix(world->GetPathName()) != MainMenuMap.GetAssetPathString()) return;

	if (Success && SessionSearch.IsValid())
	{
		if (CurrentWidget) {

			CurrentWidget->ClearServersToJoin();

			for (size_t index = 0; index < SessionSearch->SearchResults.Num(); index++)
			{
				FString serverName = SessionSearch->SearchResults[index].GetSessionIdStr();
				CurrentWidget->AddServerToJoin(serverName, index);
			}
		}
	}
}

void UMyGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SESSION_NAME != SessionName) return;

	if (Result != EOnJoinSessionCompleteResult::Success) return;

	FString address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, address)) return;

	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	GetFirstLocalPlayerController()->ClientTravel(address, ETravelType::TRAVEL_Absolute);
}

void UMyGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}

void UMyGameInstance::LoadMainMenu()
{
	GetFirstLocalPlayerController()->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}