// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EQ/Generator/EQGen_MeleePosition.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AI/EQ/Context/EnvQueryContext_Target.h"
#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEQGen_MeleePosition::UEQGen_MeleePosition() {
	Querier = UEnvQueryContext_Querier::StaticClass();
	Target = UEnvQueryContext_Target::StaticClass();
}

void UEQGen_MeleePosition::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	FEnvQueryContextData querierContextData;
	if (!QueryInstance.PrepareContext(Querier, querierContextData)) return;
	ABasicCharacter* querier = Cast<ABasicCharacter>(UEnvQueryItemType_Actor::GetValue(querierContextData.RawData.GetData()));

	FEnvQueryContextData targetContextData;
	if (!QueryInstance.PrepareContext(Target, targetContextData)) return;
	ABasicCharacter* target = Cast<ABasicCharacter>(UEnvQueryItemType_Actor::GetValue(targetContextData.RawData.GetData()));

	TArray<FNavLocation> GridPoints;

	if (querier && target)
	{

		const float range = querier->GetProjectileClass()->GetDefaultObject<ABasicProjectile>()->GetRange();
		const FVector fireOffset = querier->GetFireOffset();
		const float effectiveRange = range + fireOffset.Size2D();

		const FVector querierLocation = querier->GetActorLocation();
		const FVector targetLocation = target->GetActorLocation();

		const float distance = (querierLocation - targetLocation).Size2D();

		const FVector direction = FVector(querierLocation.X - targetLocation.X, querierLocation.Y - targetLocation.Y, 0) / distance;
		const FVector normal = FVector(-direction.Y, direction.X, 0);

		if (distance < effectiveRange)
		{
			// Change location and attack!
			FVector rotatedDirection = FRotator(0, FMath::Rand() % 31 + 30, 0).RotateVector(direction);
			FVector testLocation = targetLocation + rotatedDirection * (effectiveRange - 1);
			GridPoints.Add(FNavLocation(testLocation));
		}
		else
		{
			const float nextDistance = distance / 2;
			if (nextDistance < effectiveRange)
			{
				// Get close and attack
				FVector rotatedDirection = FRotator(0, FMath::Rand() % 31 + 30, 0).RotateVector(direction);
				FVector testLocation = targetLocation + rotatedDirection * (effectiveRange - 1);
				GridPoints.Add(FNavLocation(testLocation));
			}
			else
			{
				// Zig zag a little bit
				FVector rotatedDirection = FRotator(0, FMath::Rand() % 31 + 30, 0).RotateVector(direction);
				FVector testLocation = targetLocation + rotatedDirection * nextDistance;
				GridPoints.Add(FNavLocation(testLocation));
			}
		}
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UEQGen_MeleePosition::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("SimpleGridDescriptionGenerateAroundContext", "{0}: generate around {1} and {2}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(Querier), UEnvQueryTypes::DescribeContext(Target));
};

FText UEQGen_MeleePosition::GetDescriptionDetails() const
{
	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		return FText::Format(LOCTEXT("SimpleGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE