// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "GameModeBase_Lobby.generated.h"

UCLASS()
class PUZZLEPLATFORMS_API AGameModeBase_Lobby : public AGameModeBase
{
	GENERATED_BODY()

public:

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController* Exiting) override;

private:

	uint32 NumberOfPlayers = 0;
};