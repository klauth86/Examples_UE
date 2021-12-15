// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorator/BTDecorator_CanFire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"
#include "Components/CapsuleComponent.h"

bool UBTDecorator_CanFire::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName));
		ABasicCharacter* target = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_Target.SelectedKeyName));
		if (self && target)
		{
			const float range = self->GetProjectileClass()->GetDefaultObject<ABasicProjectile>()->GetRange();
			
			float targetRadius;
			float targetHalfHeight;
			target->GetCapsuleComponent()->GetScaledCapsuleSize(targetRadius, targetHalfHeight);

			const FVector fireOffset = self->GetFireOffset();
			const float effectiveRange = range + fireOffset.Size2D() + targetRadius - 1;

			const FVector selfLocation = self->GetActorLocation();
			const FVector targetLocation = target->GetActorLocation();

			return self->CanFire() && (selfLocation - targetLocation).Size2D() < effectiveRange * effectiveRange;
		}
	}

	return false;
}