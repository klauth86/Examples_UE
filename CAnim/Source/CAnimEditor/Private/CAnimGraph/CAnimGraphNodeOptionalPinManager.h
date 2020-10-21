// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"

class UEdGraphPin;
class UCAnimGraphNode_Base;

struct FCAnimGraphNodeOptionalPinManager : public FOptionalPinManager {

protected:
	UCAnimGraphNode_Base* BaseNode;

	TArray<UEdGraphPin*>* OldPins;
	TMap<FName, UEdGraphPin*> OldPinMap;

public:
	FCAnimGraphNodeOptionalPinManager(UCAnimGraphNode_Base* Node, TArray<UEdGraphPin*>* InOldPins);

	virtual void GetRecordDefaults(UProperty* TestProperty, FOptionalPinFromProperty& Record) const override;
	virtual void CustomizePinData(UEdGraphPin* Pin, FName SourcePropertyName, int32 ArrayIndex, UProperty* Property) const override;
	virtual void PostInitNewPin(UEdGraphPin* Pin, FOptionalPinFromProperty& Record, int32 ArrayIndex, UProperty* Property, uint8* PropertyAddress, uint8* DefaultPropertyAddress) const override;
	virtual void PostRemovedOldPin(FOptionalPinFromProperty& Record, int32 ArrayIndex, UProperty* Property, uint8* PropertyAddress, uint8* DefaultPropertyAddress) const override;

	void AllocateDefaultPins(UStruct* SourceStruct, uint8* StructBasePtr, uint8* DefaultsPtr);
};
