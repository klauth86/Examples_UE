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
			FRotator selfRot = self->GetActorRotation();

			float diffYaw = aimRot.Yaw - selfRot.Yaw;
			float deltaYaw = self->GetRotationRate() * DeltaSeconds;

			const bool bHasAimed = diffYaw < deltaYaw;

			FRotator deltaRot = FRotator(0, bHasAimed ? diffYaw : deltaYaw, 0);

			UE_LOG(LogTemp, Warning, TEXT("@@@ DIFF[%f] DELTA[%f] AIM[%s] R[%s] DR[%s]"), diffYaw, deltaYaw, *FString(bHasAimed ? "T" : "F"), *selfRot.ToString(), *deltaRot.ToString())

			self->AddActorWorldRotation(deltaRot);
			return bHasAimed ? EBTNodeResult::Type::Succeeded : EBTNodeResult::Type::InProgress;
		}
	}

	return EBTNodeResult::Type::Succeeded;
}