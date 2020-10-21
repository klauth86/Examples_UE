#pragma once

#include "LoadingScreen.h"
#include "SLoadingScreen.h"

class FLoadingScreenModule : public ILoadingScreenModule {
public:

	virtual void StartupModule() override {
		LoadObject<UObject>(nullptr, TEXT("/Game/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

		if (IsMoviePlayerEnabled()) {
			CreateScreen();
		}
	}

	virtual bool IsGameModule() const override { return true; }

	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) override {
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.bWaitForManualStop = bPlayUntilStopped;
		LoadingScreen.bAllowEngineTick = bPlayUntilStopped;
		LoadingScreen.MinimumLoadingScreenDisplayTime = PlayTime;
		LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	};

	virtual void StopInGameLoadingScreen() override { GetMoviePlayer()->StopMovie(); };

	virtual void CreateScreen() {
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 5.f;
		LoadingScreen.WidgetLoadingScreen = SNew(SLoadingScreen);
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
};

IMPLEMENT_MODULE(FLoadingScreenModule, LoadingScreen);
