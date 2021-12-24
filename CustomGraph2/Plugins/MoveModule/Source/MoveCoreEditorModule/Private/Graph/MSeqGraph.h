// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EdGraph/EdGraph.h"
#include "MSeqGraph.generated.h"

UCLASS()
class MOVECOREEDITORMODULE_API UMSeqGraph : public UEdGraph
{
	GENERATED_UCLASS_BODY()

public:

	bool IsLocked() const { return bLockUpdates; }
	void LockUpdates() { bLockUpdates = 1; }
	void UnlockUpdates() { bLockUpdates = 0; }

	virtual void OnCreated() {}
	virtual void OnLoaded() {}
	virtual void Initialize() {}

	virtual void UpdateAsset(int32 UpdateFlags = 0) {}
	void UpdateClassData() {}
	virtual void OnNodesPasted(const FString& ImportStr) {}

protected:

	uint32 bLockUpdates : 1;
};