// Fill out your copyright notice in the Description page of Project Settings.

#include "ATActions/ATActions_MoveSequence.h"
#include "MoveSequence.h"

UClass* FATActions_MoveSequence::GetSupportedClass() const { return UMoveSequence::StaticClass(); }