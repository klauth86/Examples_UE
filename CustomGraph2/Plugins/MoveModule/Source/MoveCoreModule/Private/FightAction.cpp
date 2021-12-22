// Fill out your copyright notice in the Description page of Project Settings.

#include "FightAction.h"
#include "FightActionType.h"

UFightAction::UFightAction(const FObjectInitializer& objectInitializer) :Super(objectInitializer) {
	Damage = 10;
	Stamina = 10;

	Priority = EFightActionPriority::NORMAL;
}

UAnimMontage* UFightAction::GetAnim(EFightActionType actionType) const {
	switch (actionType) {
	case EFightActionType::ACTION: return ActionAnim;
	case EFightActionType::HIT: return HitAnim;
	case EFightActionType::KO: return KOAnim;
	case EFightActionType::GETUP: return GetUpAnim;
	case EFightActionType::SIDE_LEFT_BLOCK: return SideLeftBlockAnim;
	case EFightActionType::SIDE_RIGHT_BLOCK: return SideRightBlockAnim;
	case EFightActionType::FORWARD_BLOCK: return ForwardBlockAnim;
	case EFightActionType::BACKWARD_BLOCK: return BackwardBlockAnim;
	}

	return nullptr;
}