#pragma once

#include "UObject/Object.h"
#include "BoneContainer.h"
#include "Animation/AnimationAsset.h"
#include "CAnimInstanceBase.generated.h"

class USkeleton;
class UCAnimMeshComponentBase;
class USkeletalMesh;
struct FCurveEvaluationOption;
struct FCAnimEvaluationContext;
struct FBlendedHeapCurve;
struct FCompactPose;

UCLASS()
class CANIM_API UCAnimInstanceBase : public UObject {

	GENERATED_BODY()

public:

	enum class EUpdateCAnimationFlag : uint8 {
		/** Enforce an immediate update, regardless of state*/
		ForceImmediateUpdate,
		/** Enforces a parallel update, regardless of state */
		ForceParallelUpdate,
		/** Use state to determine whether or not to immediately or update in parallel */
		Default
	};

public:

	UFUNCTION(BlueprintPure, Category = "Sub-Instances")
		UCAnimInstanceBase* GetSubInstanceByTag(FName InTag) const { return nullptr; }

	UFUNCTION(BlueprintPure, Category = "Sub-Instances")
		void GetSubInstancesByTag(FName InTag, TArray<UCAnimInstanceBase*>& OutSubInstances) const {};

	UFUNCTION(BlueprintCallable, Category = "Layers")
		void SetLayerOverlay(TSubclassOf<UCAnimInstanceBase> InClass) {}

	UFUNCTION(BlueprintCallable, Category = "Layers")
		UCAnimInstanceBase* GetLayerSubInstanceByGroup(FName InGroup) const { return NULL; }

	UFUNCTION(BlueprintCallable, Category = "Animation", meta = (NotBlueprintThreadSafe))
		void ResetDynamics(ETeleportType InTeleportType) {}

	bool UpdateAnimation(float DeltaSeconds, bool bNeedsValidRootMotion, EUpdateCAnimationFlag UpdateFlag = EUpdateCAnimationFlag::Default) { return false; }

	void PostUpdateAnimation() {}

	void DispatchQueuedAnimEvents() {}

	void RecalcRequiredBones() {}

	void RecalcRequiredCurves(const FCurveEvaluationOption& CurveEvalOption) {}

	void EndNotifyStates() {}

	void InitializeAnimation() {}

	USkeleton* GetCurrentSkeleton() { return NULL; }

	FRootMotionMovementParams ConsumeExtractedRootMotion(float Alpha) { return FRootMotionMovementParams(); }

	virtual void OnUROPreInterpolation_AnyThread(FCAnimEvaluationContext& InOutContext) {}

	void UpdateCurvesPostEvaluation() {}

	void PostEvaluateAnimation() {}

	void UninitializeAnimation() {}

	virtual void NativeBeginPlay() {}

	UFUNCTION(BlueprintImplementableEvent)
		void BlueprintBeginPlay();

	FBoneContainer bb;
	FBoneContainer& GetRequiredBones() { return bb; }

	virtual bool CanRunParallelWork() const { return true; }

	bool NeedsUpdate() const { return false; }

	FGraphTraversalCounter bbb;
	const FGraphTraversalCounter& GetUpdateCounter() const { return bbb; }

	void PreEvaluateAnimation() {}

	void ParallelUpdateAnimation() {}

	void ParallelEvaluateAnimation(bool bForceRefPose, const USkeletalMesh* InSkeletalMesh, FBlendedHeapCurve& OutCurve, FCompactPose& OutPose) {}

	virtual void OnUROSkipTickAnimation() {}

	bool HasMorphTargetCurves() const { return false; }

	void RefreshCurves(UCAnimMeshComponentBase* Component) {}

	bool ParallelCanEvaluate(const USkeletalMesh* InSkeletalMesh) const { return false; }

	void CopyCurveValues(const UCAnimInstanceBase& InSourceInstance) {}

	void UpdateCurvesToEvaluationContext(const FCAnimEvaluationContext& InOutContext) {}
};