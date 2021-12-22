// Fill out your copyright notice in the Description page of Project Settings.

#include "ATActions/ATActions_MoveSequence.h"
#include "FightAction.h"

UClass* FATActions_MoveSequence::GetSupportedClass() const { return UFightAction::StaticClass(); }