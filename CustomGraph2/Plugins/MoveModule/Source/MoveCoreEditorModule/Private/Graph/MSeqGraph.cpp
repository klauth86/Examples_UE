// Fill out your copyright notice in the Description page of Project Settings.

#include "Graph/MSeqGraph.h"
#include "MSeqGraphSchema.h"

UMSeqGraph::UMSeqGraph(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Schema = UMSeqGraphSchema::StaticClass();
}