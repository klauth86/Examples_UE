// Fill out your copyright notice in the Description page of Project Settings.

#include "Browser/SMoveSequenceBrowser.h"
#include "Toolkits/GlobalEditorCommonCommands.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "FightAction.h"
#include "MoveSequence.h"
#include "FrontendFilterBase.h"
#include "FileHelpers.h"
#include "InputCoreTypes.h"

#define LOCTEXT_NAMESPACE "MoveSequenceBrowser"

const FString SMoveSequenceBrowser::SettingsIniSection = TEXT("MoveSequenceBrowser");

const int32 SMoveSequenceBrowser::MaxAssetsHistory = 10;

void SMoveSequenceBrowser::Construct(const FArguments& InArgs)
{
	OnOpenNewAsset = InArgs._OnOpenNewAsset;
	bShowHistory = InArgs._ShowHistory;

	Commands = MakeShareable(new FUICommandList());
	Commands->MapAction(FGlobalEditorCommonCommands::Get().FindInContentBrowser, FUIAction(
		FExecuteAction::CreateSP(this, &SMoveSequenceBrowser::FindInContentBrowser),
		FCanExecuteAction::CreateSP(this, &SMoveSequenceBrowser::CanFindInContentBrowser)
	));

	CurrentAssetHistoryIndex = INDEX_NONE;
	bTriedToCacheOrginalAsset = false;

	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	// Configure filter for asset picker
	Filter.bRecursiveClasses = true;
	Filter.ClassNames.Add(UFightAction::StaticClass()->GetFName());

	FAssetPickerConfig Config;
	Config.Filter = Filter;
	Config.InitialAssetViewType = EAssetViewType::Column;
	Config.bAddFilterUI = true;
	Config.bShowPathInColumnView = true;
	Config.bSortByPathInColumnView = true;

	// Configure response to click and double-click
	Config.OnGetAssetContextMenu = FOnGetAssetContextMenu::CreateSP(this, &SMoveSequenceBrowser::OnGetAssetContextMenu);
	Config.OnAssetTagWantsToBeDisplayed = FOnShouldDisplayAssetTag::CreateSP(this, &SMoveSequenceBrowser::CanShowColumnForAssetRegistryTag);
	Config.SyncToAssetsDelegates.Add(&SyncToAssetsDelegate);
	Config.GetCurrentSelectionDelegates.Add(&GetCurrentSelectionDelegate);
	Config.SetFilterDelegates.Add(&SetFilterDelegate);
	Config.bFocusSearchBoxWhenOpened = false;
	Config.DefaultFilterMenuExpansion = EAssetTypeCategories::Animation;

	Config.SaveSettingsName = SettingsIniSection;

	Config.OnIsAssetValidForCustomToolTip = FOnIsAssetValidForCustomToolTip::CreateLambda([](const FAssetData& AssetData) { return false; });

	// hide all asset registry columns by default (we only really want the name and path)
	TArray<UObject::FAssetRegistryTag> AssetRegistryTags;
	UFightAction::StaticClass()->GetDefaultObject()->GetAssetRegistryTags(AssetRegistryTags);
	for (UObject::FAssetRegistryTag& AssetRegistryTag : AssetRegistryTags)
	{
		Config.HiddenColumnNames.Add(AssetRegistryTag.Name.ToString());
	}

	// Also hide the type column by default (but allow users to enable it, so don't use bShowTypeInColumnView)
	Config.HiddenColumnNames.Add(TEXT("Class"));

	static const FName DefaultForegroundName("DefaultForeground");

	//TSharedRef< SMenuAnchor > BackMenuAnchorPtr = SNew(SMenuAnchor)
	//	.Placement(MenuPlacement_BelowAnchor)
	//	.OnGetMenuContent(this, &SMoveSequenceBrowser::CreateHistoryMenu, true)
	//	[
	//		SNew(SButton)
	//		.OnClicked(this, &SMoveSequenceBrowser::OnGoBackInHistory)
	//	.ForegroundColor(FEditorStyle::GetSlateColor(DefaultForegroundName))
	//	.ButtonStyle(FEditorStyle::Get(), "FlatButton")
	//	.ContentPadding(FMargin(1, 0))
	//	.IsEnabled(this, &SMoveSequenceBrowser::CanStepBackwardInHistory)
	//	.ToolTipText(LOCTEXT("Backward_Tooltip", "Step backward in the asset history. Right click to see full history."))
	//	[
	//		SNew(STextBlock)
	//		.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
	//	.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
	//	.Text(FText::FromString(FString(TEXT("\xf060"))) /*fa-arrow-left*/)
	//	]
	//	];

	//TSharedRef< SMenuAnchor > FwdMenuAnchorPtr = SNew(SMenuAnchor)
	//	.Placement(MenuPlacement_BelowAnchor)
	//	.OnGetMenuContent(this, &SMoveSequenceBrowser::CreateHistoryMenu, false)
	//	[
	//		SNew(SButton)
	//		.OnClicked(this, &SMoveSequenceBrowser::OnGoForwardInHistory)
	//	.ForegroundColor(FEditorStyle::GetSlateColor(DefaultForegroundName))
	//	.ButtonStyle(FEditorStyle::Get(), "FlatButton")
	//	.ContentPadding(FMargin(1, 0))
	//	.IsEnabled(this, &SMoveSequenceBrowser::CanStepForwardInHistory)
	//	.ToolTipText(LOCTEXT("Forward_Tooltip", "Step forward in the asset history. Right click to see full history."))
	//	[
	//		SNew(STextBlock)
	//		.TextStyle(FEditorStyle::Get(), "ContentBrowser.TopBar.Font")
	//	.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
	//	.Text(FText::FromString(FString(TEXT("\xf061"))) /*fa-arrow-right*/)
	//	]
	//	];

	//this->ChildSlot
	//	[
	//		SNew(SVerticalBox)
	//		+ SVerticalBox::Slot()
	//	.AutoHeight()
	//	[
	//		SNew(SBorder)
	//		.Visibility(this, &SMoveSequenceBrowser::GetHistoryVisibility)
	//	.Padding(FMargin(3))
	//	.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
	//	[
	//		SNew(SHorizontalBox)
	//		+ SHorizontalBox::Slot()
	//	.HAlign(HAlign_Left)
	//	[
	//		SNew(SHorizontalBox)
	//		+ SHorizontalBox::Slot()
	//	.AutoWidth()
	//	[
	//		SNew(SBorder)
	//		.OnMouseButtonDown(this, &SMoveSequenceBrowser::OnMouseDownHistory, TWeakPtr<SMenuAnchor>(BackMenuAnchorPtr))
	//	.BorderImage(FEditorStyle::GetBrush("NoBorder"))
	//	[
	//		BackMenuAnchorPtr
	//	]
	//	]

	//+ SHorizontalBox::Slot()
	//	.AutoWidth()
	//	[
	//		SNew(SBorder)
	//		.OnMouseButtonDown(this, &SMoveSequenceBrowser::OnMouseDownHistory, TWeakPtr<SMenuAnchor>(FwdMenuAnchorPtr))
	//	.BorderImage(FEditorStyle::GetBrush("NoBorder"))
	//	[
	//		FwdMenuAnchorPtr
	//	]
	//	]
	//	]
	//	]
	//	]
	//+ SVerticalBox::Slot()
	//	.FillHeight(1.f)
	//	[
	//		SNew(SBorder)
	//		.Padding(FMargin(3))
	//	.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
	//	[
	//		ContentBrowserModule.Get().CreateAssetPicker(Config)
	//	]
	//	]
	//	];

	//// Create the ignore set for asset registry tags
	//// Making Skeleton to be private, and now GET_MEMBER_NAME_CHECKED doesn't work
	//AssetRegistryTagsToIgnore.Add(TEXT("Skeleton"));
	//AssetRegistryTagsToIgnore.Add(GET_MEMBER_NAME_CHECKED(UAnimSequenceBase, SequenceLength));
	//AssetRegistryTagsToIgnore.Add(GET_MEMBER_NAME_CHECKED(UAnimSequenceBase, RateScale));
}

FReply SMoveSequenceBrowser::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (Commands->ProcessCommandBindings(InKeyEvent))
	{
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SMoveSequenceBrowser::SelectAsset(UMoveSequence* InAsset)
{
	FAssetData AssetData(InAsset);

	if (AssetData.IsValid())
	{
		TArray<FAssetData> CurrentSelection = GetCurrentSelectionDelegate.Execute();

		if (!CurrentSelection.Contains(AssetData))
		{
			TArray<FAssetData> AssetsToSelect;
			AssetsToSelect.Add(AssetData);

			SyncToAssetsDelegate.Execute(AssetsToSelect);
		}
	}
}

void SMoveSequenceBrowser::AddToHistory(UMoveSequence* InAsset)
{
	if (InAsset)
	{
		FAssetData AssetData(InAsset);
		AddAssetToHistory(AssetData);
	}
}

TSharedPtr<SWidget> SMoveSequenceBrowser::OnGetAssetContextMenu(const TArray<FAssetData>& SelectedAssets)
{
	FMenuBuilder MenuBuilder(/*bInShouldCloseWindowAfterMenuSelection=*/ true, Commands);

	MenuBuilder.BeginSection("AnimationSequenceOptions", LOCTEXT("OptionsHeading", "Options"));
	{
		MenuBuilder.AddMenuEntry(
			LOCTEXT("SaveSelectedAssets", "Save"),
			LOCTEXT("SaveSelectedAssets_ToolTip", "Save the selected assets"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "Level.SaveIcon16x"),
			FUIAction(
				FExecuteAction::CreateSP(this, &SMoveSequenceBrowser::SaveSelectedAssets, SelectedAssets),
				FCanExecuteAction::CreateSP(this, &SMoveSequenceBrowser::CanSaveSelectedAssets, SelectedAssets)
			)
		);

		MenuBuilder.AddMenuEntry(FGlobalEditorCommonCommands::Get().FindInContentBrowser);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void SMoveSequenceBrowser::FindInContentBrowser()
{
	TArray<FAssetData> CurrentSelection = GetCurrentSelectionDelegate.Execute();
	if (CurrentSelection.Num() > 0)
	{
		FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		ContentBrowserModule.Get().SyncBrowserToAssets(CurrentSelection);
	}
}

bool SMoveSequenceBrowser::CanFindInContentBrowser() const
{
	TArray<FAssetData> CurrentSelection = GetCurrentSelectionDelegate.Execute();
	return CurrentSelection.Num() > 0;
}

void SMoveSequenceBrowser::SaveSelectedAssets(TArray<FAssetData> ObjectsToSave) const
{
	TArray<UPackage*> PackagesToSave;
	GetSelectedPackages(ObjectsToSave, PackagesToSave);

	const bool bCheckDirty = false;
	const bool bPromptToSave = false;
	const FEditorFileUtils::EPromptReturnCode Return = FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, bCheckDirty, bPromptToSave);
}

bool SMoveSequenceBrowser::CanSaveSelectedAssets(TArray<FAssetData> ObjectsToSave) const
{
	TArray<UPackage*> Packages;
	GetSelectedPackages(ObjectsToSave, Packages);
	// Don't offer save option if none of the packages are loaded
	return Packages.Num() > 0;
}

bool SMoveSequenceBrowser::CanShowColumnForAssetRegistryTag(FName AssetType, FName TagName) const
{
	return !AssetRegistryTagsToIgnore.Contains(TagName);
}

void SMoveSequenceBrowser::GetSelectedPackages(const TArray<FAssetData>& Assets, TArray<UPackage*>& OutPackages) const
{
	for (int32 AssetIdx = 0; AssetIdx < Assets.Num(); ++AssetIdx)
	{
		UPackage* Package = FindPackage(NULL, *Assets[AssetIdx].PackageName.ToString());

		if (Package)
		{
			OutPackages.Add(Package);
		}
	}
}

void SMoveSequenceBrowser::AddAssetToHistory(const FAssetData& AssetData)
{
	if (CurrentAssetHistoryIndex == AssetHistory.Num() - 1)
	{
		// History added to the end
		if (AssetHistory.Num() == MaxAssetsHistory)
		{
			// If max history entries has been reached
			// remove the oldest history
			AssetHistory.RemoveAt(0);
		}
	}
	else
	{
		// Clear out any history that is in front of the current location in the history list
		AssetHistory.RemoveAt(CurrentAssetHistoryIndex + 1, AssetHistory.Num() - (CurrentAssetHistoryIndex + 1), true);
	}

	AssetHistory.Add(AssetData);
	CurrentAssetHistoryIndex = AssetHistory.Num() - 1;
}

FReply SMoveSequenceBrowser::OnGoForwardInHistory()
{
	while (CurrentAssetHistoryIndex < AssetHistory.Num() - 1)
	{
		++CurrentAssetHistoryIndex;

		if (AssetHistory[CurrentAssetHistoryIndex].IsValid())
		{
			GoToHistoryIndex(CurrentAssetHistoryIndex);
			break;
		}
	}
	return FReply::Handled();
}

FReply SMoveSequenceBrowser::OnGoBackInHistory()
{
	while (CurrentAssetHistoryIndex > 0)
	{
		--CurrentAssetHistoryIndex;

		if (AssetHistory[CurrentAssetHistoryIndex].IsValid())
		{
			GoToHistoryIndex(CurrentAssetHistoryIndex);
			break;
		}
	}
	return FReply::Handled();
}

void SMoveSequenceBrowser::GoToHistoryIndex(int32 InHistoryIdx)
{
	if (AssetHistory[InHistoryIdx].IsValid())
	{
		CurrentAssetHistoryIndex = InHistoryIdx;
	}
}

bool SMoveSequenceBrowser::CanStepBackwardInHistory() const
{
	int32 HistoryIdx = CurrentAssetHistoryIndex - 1;
	while (HistoryIdx >= 0)
	{
		if (AssetHistory[HistoryIdx].IsValid())
		{
			return true;
		}

		--HistoryIdx;
	}
	return false;
}

bool SMoveSequenceBrowser::CanStepForwardInHistory() const
{
	int32 HistoryIdx = CurrentAssetHistoryIndex + 1;
	while (HistoryIdx < AssetHistory.Num())
	{
		if (AssetHistory[HistoryIdx].IsValid())
		{
			return true;
		}

		++HistoryIdx;
	}
	return false;
}

FReply SMoveSequenceBrowser::OnMouseDownHistory(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TWeakPtr< SMenuAnchor > InMenuAnchor)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		InMenuAnchor.Pin()->SetIsOpen(true);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

TSharedRef<SWidget> SMoveSequenceBrowser::CreateHistoryMenu(bool bInBackHistory) const
{
	FMenuBuilder MenuBuilder(true, NULL);
	if (bInBackHistory)
	{
		int32 HistoryIdx = CurrentAssetHistoryIndex - 1;
		while (HistoryIdx >= 0)
		{
			const FAssetData& AssetData = AssetHistory[HistoryIdx];

			if (AssetData.IsValid())
			{
				const FText DisplayName = FText::FromName(AssetData.AssetName);
				const FText Tooltip = FText::FromString(AssetData.ObjectPath.ToString());

				MenuBuilder.AddMenuEntry(DisplayName, Tooltip, FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateRaw(const_cast<SMoveSequenceBrowser*>(this), &SMoveSequenceBrowser::GoToHistoryIndex, HistoryIdx)
					),
					NAME_None, EUserInterfaceActionType::Button);
			}

			--HistoryIdx;
		}
	}
	else
	{
		int32 HistoryIdx = CurrentAssetHistoryIndex + 1;
		while (HistoryIdx < AssetHistory.Num())
		{
			const FAssetData& AssetData = AssetHistory[HistoryIdx];

			if (AssetData.IsValid())
			{
				const FText DisplayName = FText::FromName(AssetData.AssetName);
				const FText Tooltip = FText::FromString(AssetData.ObjectPath.ToString());

				MenuBuilder.AddMenuEntry(DisplayName, Tooltip, FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateRaw(const_cast<SMoveSequenceBrowser*>(this), &SMoveSequenceBrowser::GoToHistoryIndex, HistoryIdx)
					),
					NAME_None, EUserInterfaceActionType::Button);
			}

			++HistoryIdx;
		}
	}

	return MenuBuilder.MakeWidget();
}

EVisibility SMoveSequenceBrowser::GetHistoryVisibility() const
{
	return bShowHistory ? EVisibility::Visible : EVisibility::Collapsed;
}

#undef LOCTEXT_NAMESPACE