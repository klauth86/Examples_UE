// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MyGameInstance.generated.h"

class UBaseWidget;

UCLASS()
class PUZZLEPLATFORMS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;

	void OnWorldBeginPlay();

	virtual void Init() override;

	virtual void BeginDestroy() override;

protected:

	// SIMPLE

	UFUNCTION(Exec)
		void Host();

	UFUNCTION(Exec)
		void Join(const FString& address);

	UFUNCTION(Exec)
		void Leave();

	// OSS

	void HostOSS();

	void JoinOSS(int32 index);

	void CreateSessionOSS();

	void FindSessionsOSS();

	void Quit();

	void OnWidgetConstruct();

	void OnWidgetDestruct();

	void ToggleGameMenu();

	void OnCreateSessionComplete(FName SessionName, bool Success);
	
	void OnDestroySessionComplete(FName SessionName, bool Success);
	
	void OnFindSessionsComplete(bool Success);
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "MyGameInstance")
		TSubclassOf<UBaseWidget> MainMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "MyGameInstance")
		TSubclassOf<UBaseWidget> GameMenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "MyGameInstance", Meta = (AllowedClasses = "World"))
		FSoftObjectPath MainMenuMap;

	UPROPERTY()
		UBaseWidget* CurrentWidget;
};