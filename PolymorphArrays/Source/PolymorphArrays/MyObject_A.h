// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyObject_A.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, EditInlineNew)
class POLYMORPHARRAYS_API UMyObject_A : public UObject {

	GENERATED_BODY()

public:

	UMyObject_A() {}

	friend FArchive& operator<<(FArchive& Ar, UMyObject_A& context);

	virtual void Reset() { StringProp = ""; }

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DATA")
	FString StringProp;
};
