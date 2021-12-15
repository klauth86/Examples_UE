#pragma once

#include "Toolkits/AssetEditorToolkit.h"

class UMove;

class IMoveEditor : public FAssetEditorToolkit
{
public:
	virtual UMove* GetMove() const = 0;

	virtual void SetSelection(TArray<UObject*> SelectedObjects) = 0;

	virtual bool CanPasteNodes() const = 0;

	virtual void PasteNodesHere(const FVector2D& Location) = 0;

	virtual bool GetBoundsForSelectedNodes(class FSlateRect& Rect, float Padding) = 0;

	virtual int32 GetNumberOfSelectedNodes() const = 0;

	virtual TSet<UObject*> GetSelectedNodes() const = 0;
};