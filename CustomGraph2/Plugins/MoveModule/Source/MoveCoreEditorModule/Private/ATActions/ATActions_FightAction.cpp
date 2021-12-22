// Fill out your copyright notice in the Description page of Project Settings.

#include "ATActions/ATActions_FightAction.h"
#include "FightAction.h"

UClass* FATActions_FightAction::GetSupportedClass() const { return UFightAction::StaticClass(); }