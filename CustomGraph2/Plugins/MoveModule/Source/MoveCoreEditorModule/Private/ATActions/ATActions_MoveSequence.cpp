// Fill out your copyright notice in the Description page of Project Settings.

#include "ATActions/ATActions_MoveSequence.h"
#include "MoveSequence.h"
#include "MSeqEditor.h"

UClass* FATActions_MoveSequence::GetSupportedClass() const { return UMoveSequence::StaticClass(); }

void FATActions_MoveSequence::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (TArray<UObject*>::TConstIterator ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UMoveSequence* mSeq = Cast<UMoveSequence>(*ObjIt))
		{
			TSharedRef<FMSeqEditor> NewEditor(new FMSeqEditor());
			NewEditor->InitEditor(Mode, EditWithinLevelEditor, mSeq);
		}
	}
}