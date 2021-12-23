// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

class UMoveSequence;

class IMoveSequenceBrowser : public SCompoundWidget
{
public:

	virtual void SelectAsset(UMoveSequence* InAsset) = 0;

	virtual void AddToHistory(UMoveSequence* InAsset) = 0;
};
