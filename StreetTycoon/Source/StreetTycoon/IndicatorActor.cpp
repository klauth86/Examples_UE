// Fill out your copyright notice in the Description page of Project Settings.

#include "IndicatorActor.h"
#include "Components/BillboardComponent.h"

AIndicatorActor::AIndicatorActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
		
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	BillboardComponent->bHiddenInGame = false;
	BillboardComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}