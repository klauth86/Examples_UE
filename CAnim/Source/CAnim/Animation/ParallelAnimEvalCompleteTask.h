#pragma once

#include "Base/CAnimMeshComponentBase.h"

class FParallelAnimEvalCompleteTask {
	TWeakObjectPtr<UCAnimMeshComponentBase> SkeletalMeshComponent;

public:
	FParallelAnimEvalCompleteTask(TWeakObjectPtr<UCAnimMeshComponentBase> InSkeletalMeshComponent)
		: SkeletalMeshComponent(InSkeletalMeshComponent) {}

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FParallelAnimationCompletionTask, STATGROUP_TaskGraphTasks);
	}
	static ENamedThreads::Type GetDesiredThread() {
		return ENamedThreads::GameThread;
	}
	static ESubsequentsMode::Type GetSubsequentsMode() {
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) {
		if (UCAnimMeshComponentBase* Comp = SkeletalMeshComponent.Get()) {
			if (Comp->HasValidEvaluationTask()) {
				Comp->CompleteParallelAnimationEvaluation(true);
			}
		}
	}
};