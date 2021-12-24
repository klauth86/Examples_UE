// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Browser/IMoveSequenceBrowser.h"
#include "ContentBrowserDelegates.h"
#include "AssetRegistry/ARFilter.h"

DECLARE_DELEGATE_OneParam(FMyOnOpenNewAsset, UObject* /* InAsset */);

class SMoveSequenceBrowser : public IMoveSequenceBrowser
{
public:
	SLATE_BEGIN_ARGS(SMoveSequenceBrowser)
		: _ShowHistory(false)
	{
	}

	SLATE_ARGUMENT(bool, ShowHistory)

	SLATE_ARGUMENT(FMyOnOpenNewAsset, OnOpenNewAsset)

	SLATE_END_ARGS()

public:

	static const FString SettingsIniSection;

	static const int32 MaxAssetsHistory;

public:

	void Construct(const FArguments& InArgs);

	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	/** IMoveSequenceBrowser interface */
	virtual void SelectAsset(UMoveSequence* InAsset) override;
	
	virtual void AddToHistory(UMoveSequence* InAsset) override;

	/** Delegate that handles creation of context menu */
	TSharedPtr<SWidget> OnGetAssetContextMenu(const TArray<FAssetData>& SelectedAssets);

	/** Delegate to handle "Find in Content Browser" context menu option */
	void FindInContentBrowser();

	/** Delegate to handle enabling the "Find in Content Browser" context menu option */
	bool CanFindInContentBrowser() const;

	/** Delegate to handle "Save" context menu option */
	void SaveSelectedAssets(TArray<FAssetData> ObjectsToSave) const;

	/** Delegate to handle enabling the "Save" context menu option */
	bool CanSaveSelectedAssets(TArray<FAssetData> ObjectsToSave) const;

protected:

	bool CanShowColumnForAssetRegistryTag(FName AssetType, FName TagName) const;

	/** Populate supplied OutPackages with the packages for the supplied Assets array */
	void GetSelectedPackages(const TArray<FAssetData>& Assets, TArray<UPackage*>& OutPackages) const;

	/** Adds the supplied asset to the asset history */
	void AddAssetToHistory(const FAssetData& AssetData);

	/** Single step forward in history */
	FReply OnGoForwardInHistory();

	/** Single step back in history */
	FReply OnGoBackInHistory();

	/** Jumps immediately to an index in the history if valid */
	void GoToHistoryIndex(int32 InHistoryIdx);

	/** Returns TRUE if stepping backward in history is allowed */
	bool CanStepBackwardInHistory() const;

	/** Returns TRUE if stepping forward in history is allowed */
	bool CanStepForwardInHistory() const;

	/**
	 * Mouse down callback to display a history menu
	 *
	 * @param InMenuAnchor		This is the anchor the menu will use for positioning
	 */
	FReply OnMouseDownHistory(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TWeakPtr< SMenuAnchor > InMenuAnchor);

	/**
	 * Callback to create the history menu.
	 *
	 * @param bInBackHistory		TRUE if the back history is requested, FALSE if the forward history is
	 *
	 * @return						The menu widget displaying all available history
	 */
	TSharedRef<SWidget> CreateHistoryMenu(bool bInBackHistory) const;

	/** Returns visible when not in a Blueprint mode (anim mode, etc...) */
	EVisibility GetHistoryVisibility() const;

protected:

	TSharedPtr<FUICommandList> Commands;

	// Set of tags to prevent creating details view columns for (infrequently used)
	TSet<FName> AssetRegistryTagsToIgnore;

	// List of recently opened assets
	TArray<FAssetData> AssetHistory;

	// Current position in the asset history
	int32 CurrentAssetHistoryIndex;

	// Track if we have tried to cache the first asset we were playing
	uint8 bTriedToCacheOrginalAsset : 1;

	/** Whether to show the history widgets */
	uint8 bShowHistory: 1;

	/** Delegate called to open a new asset for editing */
	FMyOnOpenNewAsset OnOpenNewAsset;

	// delegate to sync the asset picker to selected assets
	FSyncToAssetsDelegate SyncToAssetsDelegate;
	
	FGetCurrentSelectionDelegate GetCurrentSelectionDelegate;

	/** Delegate used to set the AR filter after the fact */
	FSetARFilterDelegate SetFilterDelegate;

	/** Keep the AR filter around so we can modify it */
	FARFilter Filter;
};