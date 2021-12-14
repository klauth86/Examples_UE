// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTNode_AimAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

UBTTNode_AimAtTarget::UBTTNode_AimAtTarget() {
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTNode_AimAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	return AimAtTarget(OwnerComp, NodeMemory, 0);
}

EBTNodeResult::Type UBTTNode_AimAtTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName));
		
		if (self) Unfocus(self);
	}

	return EBTNodeResult::Type::Aborted;
}

void UBTTNode_AimAtTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	UBTTaskNode::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EBTNodeResult::Type result = AimAtTarget(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (result != EBTNodeResult::Type::InProgress) FinishLatentTask(OwnerComp, result);
}

EBTNodeResult::Type UBTTNode_AimAtTarget::AimAtTarget(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName));
		ABasicCharacter* target = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_Target.SelectedKeyName));

		if (self && target)
		{
			Focus(self, target);

			const FRotator controlRot = self->GetControlRotation();
			const FRotator aimRot = (target->GetActorLocation() - self->GetActorLocation()).Rotation();

			float diffYaw = controlRot.Yaw - aimRot.Yaw;
			const bool aimedSuccessfully = FMath::Abs(diffYaw) < 1;

			if (aimedSuccessfully) {
				Unfocus(self);
				return EBTNodeResult::Type::Succeeded;
			}

			return EBTNodeResult::Type::InProgress;
		}
	}

	return EBTNodeResult::Type::Succeeded;
}

void UBTTNode_AimAtTarget::Focus(ABasicCharacter* self, AActor* target) {
	self->GetCharacterMovement()->bOrientRotationToMovement = false;
	self->GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AAIController* controller = self->GetController<AAIController>();
	controller->SetFocus(target, EAIFocusPriority::Gameplay);
}

void UBTTNode_AimAtTarget::Unfocus(ABasicCharacter* self) {
	self->GetCharacterMovement()->bOrientRotationToMovement = true;
	self->GetCharacterMovement()->bUseControllerDesiredRotation =false;

	AAIController* controller = self->GetController<AAIController>();
	controller->ClearFocus(EAIFocusPriority::Gameplay);
}