// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SaveAndLoadService.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.generated.h"

UCLASS()
class EXAMPLEPROJ_API UMyGameInstance : public UGameInstance {
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
		void OpenLevel(FName LevelName) {
		BeginLoadingScreen();
		UGameplayStatics::OpenLevel(this, LevelName);
	}

	UFUNCTION(BlueprintCallable)
		void BeginLoadingScreen() {
		SaveAndLoadService::PlayLoadingScreen(false, 3.f);
	}

	UFUNCTION(BlueprintCallable)
		void StopLoadingScreen() {
		//SaveAndLoadService::StopLoadingScreen();
	}
};
