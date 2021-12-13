// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "UObject/ObjectMacros.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTTNode_RunEQ.generated.h"

class UBehaviorTree;
class UEnvQuery;

struct FBTEnvQueryTaskMemory
{
	/** Query request ID */
	int32 RequestID;
};

UCLASS()
class FPSEXAMPLE_API UBTTNode_RunEQ : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(Category = EQS, EditAnywhere)
		FEQSParametrizedQueryExecutionRequest EQSRequest;

	FQueryFinishedSignature QueryFinishedDelegate;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;
	virtual uint16 GetInstanceMemorySize() const override;

	/** finish task */
	void OnQueryFinished(TSharedPtr<FEnvQueryResult> Result);

#if WITH_EDITOR
	/** prepare query params */
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual FName GetNodeIconName() const override;
#endif
};