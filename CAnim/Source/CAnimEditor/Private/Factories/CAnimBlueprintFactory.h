// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
//#include "Templates/SubclassOf.h"
//#include "Engine/Blueprint.h"
//#include "Factories/Factory.h"
//#include "CAnimBlueprintFactory.generated.h"
//
///**
// * 
// */
//UCLASS()
//class CANIMEDITOR_API UCAnimBlueprintFactory : public UFactory
//{
//	GENERATED_UCLASS_BODY()
//
//	// The type of blueprint that will be created
//	UPROPERTY(EditAnywhere, Category = CAnimBlueprintFactory)
//	TEnumAsByte<EBlueprintType> BlueprintType;
//
//	// The parent class of the created blueprint
//	UPROPERTY(EditAnywhere, Category = CAnimBlueprintFactory, meta = (AllowAbstract = ""))
//	TSubclassOf<class UCAnimInstance> ParentClass;
//
//	// The kind of skeleton that animation graphs compiled from the blueprint will animate
//	UPROPERTY(EditAnywhere, Category = CAnimBlueprintFactory)
//	class USkeleton* TargetSkeleton;
//
//	// The preview mesh to use with this animation blueprint
//	UPROPERTY(EditAnywhere, Category = CAnimBlueprintFactory)
//	class USkeletalMesh* PreviewSkeletalMesh;
//
//	//~ Begin UFactory Interface
//	virtual FText GetDisplayName() const override;
//	virtual bool ConfigureProperties() override;
//	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
//	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
//	//~ Begin UFactory Interface
//};
