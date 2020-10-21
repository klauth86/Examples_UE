#pragma once

#include "LoadingScreenBrush.h"
#include "MoviePlayer.h"
#include "SThrobber.h"

class SLoadingScreen : public SCompoundWidget {
public:
	SLATE_BEGIN_ARGS(SLoadingScreen) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs) {

		static const FName LoadingScreenName(TEXT("/Game/UI/T_ActionRPG_TransparentLogo.T_ActionRPG_TransparentLogo"));

		LoadingScreenBrush = MakeShareable(new FLoadingScreenBrush(LoadingScreenName, FVector2D(1024, 256)));

		FSlateBrush *BGBrush = new FSlateBrush();
		BGBrush->TintColor = FLinearColor(0.034f, 0.034f, 0.034f, 1.0f);

		ChildSlot
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage(BGBrush)
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
			]
		+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
			.VAlign(VAlign_Bottom)
			.HAlign(HAlign_Right)
			.Padding(FMargin(10.0f))
			[
				SNew(SThrobber)
				.Visibility(this, &SLoadingScreen::GetLoadIndicatorVisibility)
			]
			]
			];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const {

		return EVisibility::Visible;

		bool Vis = GetMoviePlayer()->IsLoadingFinished();
		return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
	}

	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};
