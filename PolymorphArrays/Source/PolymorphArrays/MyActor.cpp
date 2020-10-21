// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Serialization/MemoryReader.h"
#include "MyObject_A.h"
#include "MyObject_B.h"

AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyActor::Save() {

	FBufferArchive buffer;

	auto& item1 = *objs[0];
	buffer << item1;

	auto& item2 = *objs[1];
	buffer << item2;

	Buffer = buffer;
}

void AMyActor::Load() {

	FMemoryReader reader = FMemoryReader(Buffer, true);
	reader.Seek(0);

	auto& item1 = *objs[0];
	item1.Reset();
	reader << item1;

	auto& item2 = *objs[1];
	item2.Reset();
	reader << item2;

	reader.FlushCache();
	reader.Close();
}