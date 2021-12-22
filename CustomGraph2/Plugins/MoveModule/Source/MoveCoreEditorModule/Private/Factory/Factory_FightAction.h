// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Factories/Factory.h"
#include "Factory_FightAction.generated.h"

UCLASS()
class UFactory_FightAction : public UFactory
{
	GENERATED_UCLASS_BODY()

public:

	virtual FText GetDisplayName() const override { return NSLOCTEXT("Factory_FightAction", "UFactory_FightAction", "Fight Action"); }
	virtual uint32 GetMenuCategories() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
	}
};