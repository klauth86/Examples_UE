// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/Factory_MoveSequence.h"
#include "FightAction.h"
#include "AssetTypeCategories.h"

UFactory_MoveSequence::UFactory_MoveSequence(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UFightAction::StaticClass();
}

uint32 UFactory_MoveSequence::GetMenuCategories() const { return EAssetTypeCategories::Blueprint; }

UObject* UFactory_MoveSequence::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	auto NewObjectAsset = NewObject<UFightAction>(InParent, InClass, InName, Flags);
	return NewObjectAsset;
}
