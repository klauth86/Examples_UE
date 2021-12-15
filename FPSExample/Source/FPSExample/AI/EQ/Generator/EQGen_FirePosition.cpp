// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/EQ/Generator/EQGen_FirePosition.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AI/EQ/Context/EnvQueryContext_Target.h"
#include "Characters/BasicCharacter.h"
#include "Projectiles/BasicProjectile.h"
#include "Components/CapsuleComponent.h"

#define LOCTEXT_NAMESPACE "EnvQueryGenerator"

UEQGen_FirePosition::UEQGen_FirePosition() {
	SpaceBetween.DefaultValue = 50.f;
	Querier = UEnvQueryContext_Querier::StaticClass();
	Target = UEnvQueryContext_Target::StaticClass();
}

void UEQGen_FirePosition::GenerateItems(FEnvQueryInstance& QueryInstance) const
{
	UObject* BindOwner = QueryInstance.Owner.Get();
	SpaceBetween.BindData(BindOwner, QueryInstance.QueryID);

	FEnvQueryContextData querierContextData;
	if (!QueryInstance.PrepareContext(Querier, querierContextData)) return;
	ABasicCharacter* querier = Cast<ABasicCharacter>(UEnvQueryItemType_Actor::GetValue(querierContextData.RawData.GetData()));

	FEnvQueryContextData targetContextData;
	if (!QueryInstance.PrepareContext(Target, targetContextData)) return;
	ABasicCharacter* target = Cast<ABasicCharacter>(UEnvQueryItemType_Actor::GetValue(targetContextData.RawData.GetData()));

	TArray<FNavLocation> GridPoints;

	if (querier && target) {
		
		const float range = querier->GetProjectileClass()->GetDefaultObject<ABasicProjectile>()->GetRange();

		float targetRadius;
		float targetHalfHeight;
		querier->GetCapsuleComponent()->GetScaledCapsuleSize(targetRadius, targetHalfHeight);

		const FVector fireOffset = querier->GetFireOffset();
		const float effectiveRange = range + fireOffset.Size2D() + targetRadius - 1;

		const float effectiveRangeSquared = effectiveRange * effectiveRange;

		const FVector querierLocation = querier->GetActorLocation();
		const FVector targetLocation = target->GetActorLocation();

		const FVector direction = (querierLocation - targetLocation).GetSafeNormal2D();
		const FVector normal = FVector(-direction.Y, direction.X, 0);

		float spaceBetweenValue = SpaceBetween.GetValue();

		const int32 ItemCount = FPlatformMath::TruncToInt((effectiveRange / spaceBetweenValue) + 1);
		const int32 ItemCountHalf = ItemCount / 2;

		for (size_t i = 0; i <= 2 * ItemCountHalf; i++)
		{
			for (size_t j = 0; j <= 2 * ItemCountHalf; j++)
			{
				const FVector testLocation = targetLocation + spaceBetweenValue * (i - ItemCountHalf) * direction
					+ spaceBetweenValue * (j - ItemCountHalf) * normal;

				float distanceSquared = (testLocation - targetLocation).SizeSquared2D();

				if (distanceSquared < effectiveRangeSquared && 4 * distanceSquared > effectiveRangeSquared)
				{
					GridPoints.Add(FNavLocation(testLocation));
				}
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("!!! CNT[%d] NUM[%d]"), ItemCountHalf, GridPoints.Num())
	}

	ProjectAndFilterNavPoints(GridPoints, QueryInstance);
	StoreNavPoints(GridPoints, QueryInstance);
}

FText UEQGen_FirePosition::GetDescriptionTitle() const
{
	return FText::Format(LOCTEXT("SimpleGridDescriptionGenerateAroundContext", "{0}: generate around {1} and {2}"),
		Super::GetDescriptionTitle(), UEnvQueryTypes::DescribeContext(Querier), UEnvQueryTypes::DescribeContext(Target));
};

FText UEQGen_FirePosition::GetDescriptionDetails() const
{
	FText Desc = FText::Format(LOCTEXT("SimpleGridDescription", "space between: {1}"), FText::FromString(SpaceBetween.ToString()));

	FText ProjDesc = ProjectionData.ToText(FEnvTraceData::Brief);
	if (!ProjDesc.IsEmpty())
	{
		FFormatNamedArguments ProjArgs;
		ProjArgs.Add(TEXT("Description"), Desc);
		ProjArgs.Add(TEXT("ProjectionDescription"), ProjDesc);
		Desc = FText::Format(LOCTEXT("SimpleGridDescriptionWithProjection", "{Description}, {ProjectionDescription}"), ProjArgs);
	}

	return Desc;
}

#undef LOCTEXT_NAMESPACE