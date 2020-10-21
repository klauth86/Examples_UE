#pragma once

#include "Animation/AnimCurveTypes.h"
#include "Base/CAnimInstanceBase.h"

struct FCAnimEvaluationContext {
	// The anim instance we are evaluating
	UCAnimInstanceBase* AnimInstance;

	// The SkeletalMesh we are evaluating for
	USkeletalMesh* SkeletalMesh;

	// Evaluation data, swapped in from the component when we are running parallel eval
	TArray<FTransform> ComponentSpaceTransforms;
	TArray<FTransform> BoneSpaceTransforms;
	TArray<FTransform> CachedComponentSpaceTransforms;
	TArray<FTransform> CachedBoneSpaceTransforms;
	FVector RootBoneTranslation;

	// Are we performing interpolation this tick
	bool bDoInterpolation;

	// Are we evaluating this tick
	bool bDoEvaluation;

	// Are we storing data in cache bones this tick
	bool bDuplicateToCacheBones;

	// duplicate the cache curves
	bool bDuplicateToCacheCurve;

	// Curve data, swapped in from the component when we are running parallel eval
	FBlendedHeapCurve	Curve;
	FBlendedHeapCurve	CachedCurve;

	FCAnimEvaluationContext() {
		Clear();
	}

	void Copy(const FCAnimEvaluationContext& Other) {
		AnimInstance = Other.AnimInstance;
		SkeletalMesh = Other.SkeletalMesh;
		ComponentSpaceTransforms.Reset();
		ComponentSpaceTransforms.Append(Other.ComponentSpaceTransforms);
		BoneSpaceTransforms.Reset();
		BoneSpaceTransforms.Append(Other.BoneSpaceTransforms);
		CachedComponentSpaceTransforms.Reset();
		CachedComponentSpaceTransforms.Append(Other.CachedComponentSpaceTransforms);
		CachedBoneSpaceTransforms.Reset();
		CachedBoneSpaceTransforms.Append(Other.CachedBoneSpaceTransforms);
		RootBoneTranslation = Other.RootBoneTranslation;
		Curve.CopyFrom(Other.Curve);
		CachedCurve.CopyFrom(Other.CachedCurve);
		bDoInterpolation = Other.bDoInterpolation;
		bDoEvaluation = Other.bDoEvaluation;
		bDuplicateToCacheBones = Other.bDuplicateToCacheBones;
		bDuplicateToCacheCurve = Other.bDuplicateToCacheCurve;
	}

	void Clear() {
		AnimInstance = nullptr;
		SkeletalMesh = nullptr;
	}
};