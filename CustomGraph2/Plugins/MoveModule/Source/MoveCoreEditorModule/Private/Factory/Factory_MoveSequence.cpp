// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/Factory_MoveSequence.h"
#include "MoveSequence.h"
#include "AssetTypeCategories.h"

UFactory_MoveSequence::UFactory_MoveSequence(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UMoveSequence::StaticClass();
}

uint32 UFactory_MoveSequence::GetMenuCategories() const { return EAssetTypeCategories::Blueprint; }

UObject* UFactory_MoveSequence::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UMoveSequence* NewObjectAsset = NewObject<UMoveSequence>(InParent, InClass, InName, Flags);
	return NewObjectAsset;
}
