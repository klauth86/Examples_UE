// Fill out your copyright notice in the Description page of Project Settings.

#include "ATActions/ATActions_Move.h"
#include "Move.h"

UClass* FATActions_Move::GetSupportedClass() const { return UMove::StaticClass(); }