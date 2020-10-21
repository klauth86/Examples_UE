// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject_A.h"
#include "MyObject_B.generated.h"

/**
 * 
 */
UCLASS()
class POLYMORPHARRAYS_API UMyObject_B : public UMyObject_A {

	GENERATED_BODY()
	
public:

	UMyObject_B(): Super() {}

	friend FArchive& operator<<(FArchive& Ar, UMyObject_B& context);

	virtual void Reset() override { 
		Super::Reset();
		FloatProp = 0;
	}

protected:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "DATA")
	float FloatProp;
};
