// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Decorator/BTDecorator_CanFire.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"

bool UBTDecorator_CanFire::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent())
	{
		ABasicCharacter* self = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_SelfActor.SelectedKeyName));
		ABasicCharacter * target = Cast<ABasicCharacter>(bb->GetValueAsObject(BK_Target.SelectedKeyName));
		if (self && target)
		{
			ABasicProjectile* defaultProjectile = self->GetProjectileClass()->GetDefaultObject<ABasicProjectile>();
			float range = defaultProjectile->GetRange();
			float effectiveRange = range + self->GetFireOffset().Size2D();

			const FVector selfLocation = self->GetActorLocation();
			const FVector targetLocation = target->GetActorLocation();

			return self->CanFire() && (selfLocation - targetLocation).Size2D() < effectiveRange * effectiveRange;
		}
	}

	return false;
}