// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Task/BTTNode_AimAtTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"

UBTTNode_AimAtTarget::UBTTNode_AimAtTarget() {
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTNode_AimAtTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	return AimAtTarget(OwnerComp, NodeMemory, 0);
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
			FRotator aimRot = (target->GetActorLocation() - self->GetActorLocation()).Rotation();
			FRotator controllerRot = self->GetControlRotation();

			float diffYaw = aimRot.Yaw - controllerRot.Yaw;
			if (diffYaw < SMALL_NUMBER) return EBTNodeResult::Type::Succeeded;

			float rotRate = self->GetRotationRate();
			float deltaYaw = FMath::Min(diffYaw, rotRate * DeltaSeconds);

			self->AddControllerYawInput(deltaYaw);

			return EBTNodeResult::Type::InProgress;
		}
	}

	return EBTNodeResult::Type::Succeeded;
}