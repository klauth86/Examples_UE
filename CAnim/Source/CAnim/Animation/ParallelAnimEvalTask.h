#pragma once

#include "Base/CAnimMeshComponentBase.h"

FAutoConsoleTaskPriority CPrio_ParallelAnimEvalTask(
	TEXT("TaskGraph.TaskPriorities.ParallelAnimationEvaluationTask"),
	TEXT("Task and thread priority for FParallelAnimationEvaluationTask"),
	ENamedThreads::HighThreadPriority, // if we have high priority task threads, then use them...
	ENamedThreads::NormalTaskPriority, // .. at normal task priority
	ENamedThreads::HighTaskPriority // if we don't have hi pri threads, then use normal priority threads at high task priority instead
);

class FParallelAnimEvalTask {
	TWeakObjectPtr<UCAnimMeshComponentBase> SkeletalMeshComponent;

public:
	FParallelAnimEvalTask(TWeakObjectPtr<UCAnimMeshComponentBase> InSkeletalMeshComponent)
		: SkeletalMeshComponent(InSkeletalMeshComponent) {}

	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(FParallelAnimEvalTask, STATGROUP_TaskGraphTasks);
	}
	static FORCEINLINE ENamedThreads::Type GetDesiredThread() {
		return CPrio_ParallelAnimEvalTask.Get();
	}
	static FORCEINLINE ESubsequentsMode::Type GetSubsequentsMode() {
		return ESubsequentsMode::TrackSubsequents;
	}

	void DoTask(ENamedThreads::Type CurrentThread, const FGraphEventRef& MyCompletionGraphEvent) {
		if (UCAnimMeshComponentBase* Comp = SkeletalMeshComponent.Get()) {

			if (CurrentThread != ENamedThreads::GameThread) {
				GInitRunaway();
			}

			Comp->ParallelAnimationEvaluation();
		}
	}
};
