// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_Lobby.h"
#include "ActionRouter.h"

#include "TimerManager.h"

void AGameModeBase_Lobby::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= 2) {
		bUseSeamlessTravel = true;
		ActionRouter::OnStartGame.ExecuteIfBound();
	}
}

void AGameModeBase_Lobby::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}