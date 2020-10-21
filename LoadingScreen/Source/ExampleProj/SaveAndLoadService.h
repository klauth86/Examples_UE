#pragma once

#include "ObjectMacros.h"
#include "LoadingScreen.h"

class SaveAndLoadService {

public:

	static void PlayLoadingScreen(bool bPlayUntilStopped, float PlayTime) {
		ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
		LoadingScreenModule.StartInGameLoadingScreen(bPlayUntilStopped, PlayTime);
	}

	static void StopLoadingScreen() {
		ILoadingScreenModule& LoadingScreenModule = ILoadingScreenModule::Get();
		LoadingScreenModule.StopInGameLoadingScreen();
	}
};