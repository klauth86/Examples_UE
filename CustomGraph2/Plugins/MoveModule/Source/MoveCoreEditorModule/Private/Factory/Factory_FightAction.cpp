// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/Factory_FightAction.h"
#include "FightAction.h"
#include "AssetTypeCategories.h"

UFactory_FightAction::UFactory_FightAction(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UFightAction::StaticClass();
}

uint32 UFactory_FightAction::GetMenuCategories() const { return EAssetTypeCategories::Blueprint; }

UObject* UFactory_FightAction::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UFightAction* NewObjectAsset = NewObject<UFightAction>(InParent, InClass, InName, Flags);
	return NewObjectAsset;
}
