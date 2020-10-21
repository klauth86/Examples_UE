#include "CAnimMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "HAL/UnrealMemory.h"
#include "Misc/RuntimeErrors.h"
#include "ContentStreaming.h"

#include "Rendering/SkeletalMeshRenderData.h"
#include "Engine/SkeletalMeshSocket.h"

#include "AnimationRuntime.h"
#include "Animation/AnimClassInterface.h"
#include "Animation/AnimationSettings.h"
#include "Animation/AnimInstance.h"

#include "AI/NavigationSystemHelpers.h"

#include "ParallelAnimEvalTask.h"
#include "ParallelAnimEvalCompleteTask.h"

#include "Base/CAnimInstanceBase.h"

#include "Components/SkeletalMeshComponent.h"

static void MergeInBoneIndexArrays(TArray<FBoneIndexType>& BaseArray, const TArray<FBoneIndexType>& InsertArray) {
	// Then we merge them into the array of required bones.
	int32 BaseBonePos = 0;
	int32 InsertBonePos = 0;

	// Iterate over each of the bones we need.
	while (InsertBonePos < InsertArray.Num()) {
		// Find index of physics bone
		FBoneIndexType InsertBoneIndex = InsertArray[InsertBonePos];

		// If at end of BaseArray array - just append.
		if (BaseBonePos == BaseArray.Num()) {
			BaseArray.Add(InsertBoneIndex);
			BaseBonePos++;
			InsertBonePos++;
		}
		// If in the middle of BaseArray, merge together.
		else {
			// Check that the BaseArray array is strictly increasing, otherwise merge code does not work.
			check(BaseBonePos == 0 || BaseArray[BaseBonePos - 1] < BaseArray[BaseBonePos]);

			// Get next required bone index.
			FBoneIndexType BaseBoneIndex = BaseArray[BaseBonePos];

			// We have a bone in BaseArray not required by Insert. Thats ok - skip.
			if (BaseBoneIndex < InsertBoneIndex) {
				BaseBonePos++;
			}
			// Bone required by Insert is in 
			else if (BaseBoneIndex == InsertBoneIndex) {
				BaseBonePos++;
				InsertBonePos++;
			}
			// Bone required by Insert is missing - insert it now.
			else // BaseBoneIndex > InsertBoneIndex
			{
				BaseArray.InsertUninitialized(BaseBonePos);
				BaseArray[BaseBonePos] = InsertBoneIndex;

				BaseBonePos++;
				InsertBonePos++;
			}
		}
	}
}



TAutoConsoleVariable<int32> CVarUseParallelAnimationEvaluation(TEXT("a.ParallelAnimEvaluation"), 1, TEXT("If 1, animation evaluation will be run across the task graph system. If 0, evaluation will run purely on the game thread"));
TAutoConsoleVariable<int32> CVarUseParallelAnimationInterpolation(TEXT("a.ParallelAnimInterpolation"), 1, TEXT("If 1, animation interpolation will be run across the task graph system. If 0, interpolation will run purely on the game thread"));

static TAutoConsoleVariable<int32> CVarAnimationDelaysEndGroup(
	TEXT("tick.AnimationDelaysEndGroup"),
	1,
	TEXT("If > 0, then skeletal meshes that do not rely on physics simulation will set their animation end tick group to TG_PostPhysics."));

static TAutoConsoleVariable<int32> CVarHiPriSkinnedMeshesTicks(
	TEXT("tick.HiPriSkinnedMeshes"),
	1,
	TEXT("If > 0, then schedule the skinned component ticks in a tick group before other ticks."));

static FThreadSafeCounter Ticked;
static FThreadSafeCounter NotTicked;
static TAutoConsoleVariable<int32> CVarSpewAnimRateOptimization(
	TEXT("SpewAnimRateOptimization"),
	0,
	TEXT("True to spew overall anim rate optimization tick rates."));

UCAnimMeshComponent::UCAnimMeshComponent(const FObjectInitializer& ObjectInitializer) {
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	bWantsInitializeComponent = true;
	GlobalAnimRateScale = 1.0f;
	bNoSkeletonUpdate = false;
	VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	SetGenerateOverlapEvents(false);
	LineCheckBoundsScale = FVector(1.0f, 1.0f, 1.0f);

	bEnableUpdateRateOptimizations = false;

	LastPoseTickFrame = 0u;

	bHasCustomNavigableGeometry = EHasCustomNavigableGeometry::Yes;

	bTickInEditor = true;

	CachedAnimCurveUidVersion = 0;

	bPostEvaluatingAnimation = false;
	bAllowAnimCurveEvaluation = true;

	bPropagateCurvesToSlaves = false;
}

UCAnimInstanceBase* UCAnimMeshComponent::GetSubInstanceByTag(FName InName) const {
	if (AnimScriptInstance) {
		return AnimScriptInstance->GetSubInstanceByTag(InName);
	}
	return nullptr;
}

void UCAnimMeshComponent::GetSubInstancesByTag(FName InTag, TArray<UCAnimInstanceBase*>& OutSubInstances) const {
	if (AnimScriptInstance) {
		AnimScriptInstance->GetSubInstancesByTag(InTag, OutSubInstances);
	}
}

void UCAnimMeshComponent::SetLayerOverlay(TSubclassOf<UCAnimInstanceBase> InClass) {
	if (AnimScriptInstance) {
		AnimScriptInstance->SetLayerOverlay(InClass);
	}
}

UCAnimInstanceBase* UCAnimMeshComponent::GetLayerSubInstanceByGroup(FName InGroup) const {
	if (AnimScriptInstance) {
		return AnimScriptInstance->GetLayerSubInstanceByGroup(InGroup);
	}
	return nullptr;
}

void UCAnimMeshComponent::ResetAnimInstanceDynamics(ETeleportType InTeleportType) {
	if (AnimScriptInstance) {
		AnimScriptInstance->ResetDynamics(InTeleportType);
	}

	for (UCAnimInstanceBase* SubInstanceIter : SubInstances) {
		SubInstanceIter->ResetDynamics(InTeleportType);
	}
}

void UCAnimMeshComponent::SetMorphTarget(FName MorphTargetName, float Value, bool bRemoveZeroWeight) {
	float *CurveValPtr = MorphTargetCurves.Find(MorphTargetName);
	bool bShouldAddToList = !bRemoveZeroWeight || FPlatformMath::Abs(Value) > ZERO_ANIMWEIGHT_THRESH;
	if (bShouldAddToList) {
		if (CurveValPtr) {
			// sum up, in the future we might normalize, but for now this just sums up
			// this won't work well if all of them have full weight - i.e. additive 
			*CurveValPtr = Value;
		}
		else {
			MorphTargetCurves.Add(MorphTargetName, Value);
		}
	}
	// if less than ZERO_ANIMWEIGHT_THRESH
	// no reason to keep them on the list
	else {
		// remove if found
		MorphTargetCurves.Remove(MorphTargetName);
	}
}

void UCAnimMeshComponent::ClearMorphTargets() {
	MorphTargetCurves.Empty();
}

float UCAnimMeshComponent::GetMorphTarget(FName MorphTargetName) const {
	const float *CurveValPtr = MorphTargetCurves.Find(MorphTargetName);

	if (CurveValPtr) {
		return *CurveValPtr;
	}
	else {
		return 0.0f;
	}
}

void UCAnimMeshComponent::SnapshotPose(FPoseSnapshot& Snapshot) {
	if (ensureAsRuntimeWarning(SkeletalMesh != nullptr)) {
		const TArray<FTransform>& ComponentSpaceTMs = GetComponentSpaceTransforms();
		const FReferenceSkeleton& RefSkeleton = SkeletalMesh->RefSkeleton;
		const TArray<FTransform>& RefPoseSpaceBaseTMs = RefSkeleton.GetRefBonePose();

		Snapshot.SkeletalMeshName = SkeletalMesh->GetFName();

		const int32 NumSpaceBases = ComponentSpaceTMs.Num();
		Snapshot.LocalTransforms.Reset(NumSpaceBases);
		Snapshot.LocalTransforms.AddUninitialized(NumSpaceBases);
		Snapshot.BoneNames.Reset(NumSpaceBases);
		Snapshot.BoneNames.AddUninitialized(NumSpaceBases);

		//Set root bone which is always evaluated.
		Snapshot.LocalTransforms[0] = ComponentSpaceTMs[0];
		Snapshot.BoneNames[0] = RefSkeleton.GetBoneName(0);

		int32 CurrentRequiredBone = 1;
		for (int32 ComponentSpaceIdx = 1; ComponentSpaceIdx < NumSpaceBases; ++ComponentSpaceIdx) {
			Snapshot.BoneNames[ComponentSpaceIdx] = RefSkeleton.GetBoneName(ComponentSpaceIdx);

			const bool bBoneHasEvaluated = FillComponentSpaceTransformsRequiredBones.IsValidIndex(CurrentRequiredBone) && ComponentSpaceIdx == FillComponentSpaceTransformsRequiredBones[CurrentRequiredBone];
			const int32 ParentIndex = RefSkeleton.GetParentIndex(ComponentSpaceIdx);
			ensureMsgf(ParentIndex != INDEX_NONE, TEXT("Getting an invalid parent bone for bone %d, but this should not be possible since this is not the root bone!"), ComponentSpaceIdx);

			const FTransform& ParentTransform = ComponentSpaceTMs[ParentIndex];
			const FTransform& ChildTransform = ComponentSpaceTMs[ComponentSpaceIdx];
			Snapshot.LocalTransforms[ComponentSpaceIdx] = bBoneHasEvaluated ? ChildTransform.GetRelativeTransform(ParentTransform) : RefPoseSpaceBaseTMs[ComponentSpaceIdx];

			if (bBoneHasEvaluated) {
				CurrentRequiredBone++;
			}
		}

		Snapshot.bIsValid = true;
	}
	else {
		Snapshot.bIsValid = false;
	}
}

void UCAnimMeshComponent::SetUpdateAnimationInEditor(const bool NewUpdateState) {
#if WITH_EDITOR
	if (IsRegistered()) {
		bUpdateAnimationInEditor = NewUpdateState;
	}
#endif
}

void UCAnimMeshComponent::SetAllowAnimCurveEvaluation(bool bInAllow) {
	if (bAllowAnimCurveEvaluation != bInAllow) {
		bAllowAnimCurveEvaluation = bInAllow;
		// clear cache uid version, so it will update required curves
		CachedAnimCurveUidVersion = 0;
	}
}

void UCAnimMeshComponent::AllowAnimCurveEvaluation(FName NameOfCurve, bool bAllow) {
	// if allow is same as disallowed curve, which means it mismatches
	if (bAllow == DisallowedAnimCurves.Contains(NameOfCurve)) {
		if (bAllow) {
			DisallowedAnimCurves.Remove(NameOfCurve);
			CachedAnimCurveUidVersion = 0;
		}
		else {
			DisallowedAnimCurves.Add(NameOfCurve);
			CachedAnimCurveUidVersion = 0;

		}
	}
}

void UCAnimMeshComponent::ResetAllowedAnimCurveEvaluation() {
	DisallowedAnimCurves.Reset();
	CachedAnimCurveUidVersion = 0;
}

void UCAnimMeshComponent::SetAllowedAnimCurvesEvaluation(const TArray<FName>& List, bool bAllow) {
	// Reset already clears the version - CachedAnimCurveUidVersion = 0;
	ResetAllowedAnimCurveEvaluation();
	if (bAllow) {
		struct FFilterDisallowedList {
			FFilterDisallowedList(const TArray<FName>& InAllowedList) : AllowedList(InAllowedList) {}

			FORCEINLINE bool operator()(const FName& Name) const {
				return AllowedList.Contains(Name);
			}
			const TArray<FName>& AllowedList;
		};

		if (SkeletalMesh) {
			USkeleton* Skeleton = SkeletalMesh->Skeleton;
			if (Skeleton) {
				const FSmartNameMapping* Mapping = Skeleton->GetSmartNameContainer(USkeleton::AnimCurveMappingName);
				if (Mapping != nullptr) {
					TArray<FName> CurveNames;
					Mapping->FillNameArray(CurveNames);

					DisallowedAnimCurves = CurveNames;
					DisallowedAnimCurves.RemoveAllSwap(FFilterDisallowedList(List));
				}
			}

		}
	}
	else {
		DisallowedAnimCurves = List;
	}
}

void UCAnimMeshComponent::SkelMeshCompOnParticleSystemFinished(UParticleSystemComponent* PSC) {
	PSC->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	PSC->UnregisterComponent();
}

bool UCAnimMeshComponent::ShouldOnlyTickMontages(const float DeltaTime) const {
	// Ignore DeltaSeconds == 0.f, as that is used when we want to force an update followed by RefreshBoneTransforms.
	// RefreshBoneTransforms will need an updated graph.
	return (VisibilityBasedAnimTickOption == EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered)
		&& !bRecentlyRendered
		&& (DeltaTime > 0.f);
}

bool UCAnimMeshComponent::ShouldTickAnimation() const {
	if (bExternalTickRateControlled) {
		return bExternalUpdate;
	}
	else {
		return (AnimUpdateRateParams != nullptr) && (!ShouldUseUpdateRateOptimizations() || !AnimUpdateRateParams->ShouldSkipUpdate());
	}
}

void UCAnimMeshComponent::TickAnimation(float DeltaTime, bool bNeedsValidRootMotion) {
	// if curves have to be refreshed before updating animation
	if (!AreRequiredCurvesUpToDate()) {
		RecalcRequiredCurves();
	}

	if (SkeletalMesh != nullptr) {
		// We're about to UpdateAnimation, this will potentially queue events that we'll need to dispatch.
		bNeedsQueuedAnimEventsDispatched = true;

		// We update sub instances first incase we're using either root motion or non-threaded update.
		// This ensures that we go through the pre update process and initialize the proxies correctly.

		// Accumulate whether or not any of the sub-instances wanted to do an immediate Animation Update
		bool bWantsImmediateUpdate = false;
		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			bWantsImmediateUpdate |= SubInstance->UpdateAnimation(DeltaTime * GlobalAnimRateScale, false, UCAnimInstanceBase::EUpdateCAnimationFlag::ForceParallelUpdate);
		}

		if (AnimScriptInstance != nullptr) {
			// In case any of the sub-instances required an immediate animation update, make sure we immediately do so for the main anim instance
			const UCAnimInstanceBase::EUpdateCAnimationFlag UpdateFlag = bWantsImmediateUpdate ? UCAnimInstanceBase::EUpdateCAnimationFlag::ForceImmediateUpdate : UCAnimInstanceBase::EUpdateCAnimationFlag::Default;

			// Tick the animation
			bWantsImmediateUpdate |= AnimScriptInstance->UpdateAnimation(DeltaTime * GlobalAnimRateScale, bNeedsValidRootMotion, UpdateFlag);
		}

		// Make sure PostUpdateAnimation is called on the subinstances if they, or the main instance did an immediate Animation Update
		if (bWantsImmediateUpdate) {
			for (UCAnimInstanceBase* SubInstance : SubInstances) {
				SubInstance->PostUpdateAnimation();
			}
		}

		/**
			If we're called directly for autonomous proxies, TickComponent is not guaranteed to get called.
			So dispatch all queued events here if we're doing MontageOnly ticking.
		*/
		if (ShouldOnlyTickMontages(DeltaTime)) {
			ConditionallyDispatchQueuedAnimEvents();
		}
	}
}

bool UCAnimMeshComponent::AreRequiredCurvesUpToDate() const {
	return (!SkeletalMesh || !SkeletalMesh->Skeleton || CachedAnimCurveUidVersion == SkeletalMesh->Skeleton->GetAnimCurveUidVersion());
}

void UCAnimMeshComponent::ConditionallyDispatchQueuedAnimEvents() {
	if (bNeedsQueuedAnimEventsDispatched) {
		bNeedsQueuedAnimEventsDispatched = false;

		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			SubInstance->DispatchQueuedAnimEvents();
		}

		if (AnimScriptInstance) {
			AnimScriptInstance->DispatchQueuedAnimEvents();
		}
	}
}

void UCAnimMeshComponent::RecalcRequiredCurves() {
	if (!SkeletalMesh) {
		return;
	}

	if (SkeletalMesh->Skeleton) {
		CachedCurveUIDList = SkeletalMesh->Skeleton->GetDefaultCurveUIDList();
	}

	const FCurveEvaluationOption CurveEvalOption(bAllowAnimCurveEvaluation, &DisallowedAnimCurves, PredictedLODLevel);

	// make sure animation requiredcurve to mark as dirty
	if (AnimScriptInstance) {
		AnimScriptInstance->RecalcRequiredCurves(CurveEvalOption);
	}

	for (UCAnimInstanceBase* SubInstance : SubInstances) {
		SubInstance->RecalcRequiredCurves(CurveEvalOption);
	}

	MarkRequiredCurveUpToDate();
}

void UCAnimMeshComponent::MarkRequiredCurveUpToDate() {
	if (SkeletalMesh && SkeletalMesh->Skeleton) {
		CachedAnimCurveUidVersion = SkeletalMesh->Skeleton->GetAnimCurveUidVersion();
	}
}

void UCAnimMeshComponent::RecalcRequiredBones(int32 LODIndex) {
	if (!SkeletalMesh) {
		return;
	}

	ComputeRequiredBones(RequiredBones, FillComponentSpaceTransformsRequiredBones, LODIndex, /*bIgnorePhysicsAsset=*/ false);
	BoneSpaceTransforms = SkeletalMesh->RefSkeleton.GetRefBonePose();
	// make sure animation requiredBone to mark as dirty
	if (AnimScriptInstance) {
		AnimScriptInstance->RecalcRequiredBones();
	}

	for (UCAnimInstanceBase* SubInstance : SubInstances) {
		SubInstance->RecalcRequiredBones();
	}

	// when recalc requiredbones happend
	// this should always happen
	MarkRequiredCurveUpToDate();
	bRequiredBonesUpToDate = true;

	// Invalidate cached bones.
	CachedBoneSpaceTransforms.Empty();
	CachedComponentSpaceTransforms.Empty();
	CachedCurve.Empty();
}

void UCAnimMeshComponent::ComputeRequiredBones(TArray<FBoneIndexType>& OutRequiredBones, TArray<FBoneIndexType>& OutFillComponentSpaceTransformsRequiredBones, int32 LODIndex, bool bIgnorePhysicsAsset) const {
	OutRequiredBones.Reset();
	OutFillComponentSpaceTransformsRequiredBones.Reset();

	if (!SkeletalMesh) {
		return;
	}

	FSkeletalMeshRenderData* SkelMeshRenderData = GetSkeletalMeshRenderData();
	if (!SkelMeshRenderData) {
		//No Render Data?
		// Jira UE-64409
		UE_LOG(LogAnimation, Warning, TEXT("Skeletal Mesh asset '%s' has no render data"), *SkeletalMesh->GetName());
		return;
	}

	// Make sure we access a valid LOD
	// @fixme jira UE-30028 Avoid crash when called with partially loaded asset
	if (SkelMeshRenderData->LODRenderData.Num() == 0) {
		//No LODS?
		UE_LOG(LogAnimation, Warning, TEXT("Skeletal Mesh asset '%s' has no LODs"), *SkeletalMesh->GetName());
		return;
	}

	LODIndex = FMath::Clamp(LODIndex, 0, SkelMeshRenderData->LODRenderData.Num() - 1);

	// The list of bones we want is taken from the predicted LOD level.
	FSkeletalMeshLODRenderData& LODData = SkelMeshRenderData->LODRenderData[LODIndex];
	OutRequiredBones = LODData.RequiredBones;

	// Add virtual bones
	MergeInBoneIndexArrays(OutRequiredBones, SkeletalMesh->RefSkeleton.GetRequiredVirtualBones());

	// Make sure that bones with per-poly collision are also always updated.
	// TODO UE4

	// Purge invisible bones and their children
	// this has to be done before mirror table check/physics body checks
	// mirror table/phys body ones has to be calculated
	if (ShouldUpdateBoneVisibility()) {
		const TArray<uint8>& EditableBoneVisibilityStates = GetEditableBoneVisibilityStates();
		check(EditableBoneVisibilityStates.Num() == GetNumComponentSpaceTransforms());

		if (ensureMsgf(EditableBoneVisibilityStates.Num() >= OutRequiredBones.Num(),
			TEXT("Skeletal Mesh asset '%s' has incorrect BoneVisibilityStates. # of BoneVisibilityStatese (%d), # of OutRequiredBones (%d)"),
			*SkeletalMesh->GetName(), EditableBoneVisibilityStates.Num(), OutRequiredBones.Num())) {
			int32 VisibleBoneWriteIndex = 0;
			for (int32 i = 0; i < OutRequiredBones.Num(); ++i) {
				FBoneIndexType CurBoneIndex = OutRequiredBones[i];

				// Current bone visible?
				if (EditableBoneVisibilityStates[CurBoneIndex] == BVS_Visible) {
					OutRequiredBones[VisibleBoneWriteIndex++] = CurBoneIndex;
				}
			}

			// Remove any trailing junk in the OutRequiredBones array
			const int32 NumBonesHidden = OutRequiredBones.Num() - VisibleBoneWriteIndex;
			if (NumBonesHidden > 0) {
				OutRequiredBones.RemoveAt(VisibleBoneWriteIndex, NumBonesHidden);
			}
		}
	}

	// Add in any bones that may be required when mirroring.
	// JTODO: This is only required if there are mirroring nodes in the tree, but hard to know...
	if (SkeletalMesh->SkelMirrorTable.Num() > 0 &&
		SkeletalMesh->SkelMirrorTable.Num() == BoneSpaceTransforms.Num()) {
		TArray<FBoneIndexType> MirroredDesiredBones;
		MirroredDesiredBones.AddUninitialized(RequiredBones.Num());

		// Look up each bone in the mirroring table.
		for (int32 i = 0; i < OutRequiredBones.Num(); i++) {
			MirroredDesiredBones[i] = SkeletalMesh->SkelMirrorTable[OutRequiredBones[i]].SourceIndex;
		}

		// Sort to ensure strictly increasing order.
		MirroredDesiredBones.Sort();

		// Make sure all of these are in OutRequiredBones, and 
		MergeInBoneIndexArrays(OutRequiredBones, MirroredDesiredBones);
	}

	TArray<FBoneIndexType> NeededBonesForFillComponentSpaceTransforms;
	{
		TArray<FBoneIndexType> ForceAnimatedSocketBones;

		for (const USkeletalMeshSocket* Socket : SkeletalMesh->GetActiveSocketList()) {
			int32 BoneIndex = SkeletalMesh->RefSkeleton.FindBoneIndex(Socket->BoneName);
			if (BoneIndex != INDEX_NONE) {
				if (Socket->bForceAlwaysAnimated) {
					ForceAnimatedSocketBones.AddUnique(BoneIndex);
				}
				else {
					NeededBonesForFillComponentSpaceTransforms.AddUnique(BoneIndex);
				}
			}
		}

		// Then sort array of required bones in hierarchy order
		ForceAnimatedSocketBones.Sort();

		// Make sure all of these are in OutRequiredBones.
		MergeInBoneIndexArrays(OutRequiredBones, ForceAnimatedSocketBones);
	}

	// Gather any bones referenced by shadow shapes
	if (FSkeletalMeshSceneProxy* SkeletalMeshProxy = (FSkeletalMeshSceneProxy*)SceneProxy) {
		const TArray<FBoneIndexType>& ShadowShapeBones = SkeletalMeshProxy->GetSortedShadowBoneIndices();

		if (ShadowShapeBones.Num()) {
			// Sort in hierarchy order then merge to required bones array
			MergeInBoneIndexArrays(OutRequiredBones, ShadowShapeBones);
		}
	}

	// Ensure that we have a complete hierarchy down to those bones.
	FAnimationRuntime::EnsureParentsPresent(OutRequiredBones, SkeletalMesh->RefSkeleton);

	OutFillComponentSpaceTransformsRequiredBones.Reset(OutRequiredBones.Num() + NeededBonesForFillComponentSpaceTransforms.Num());
	OutFillComponentSpaceTransformsRequiredBones = OutRequiredBones;

	NeededBonesForFillComponentSpaceTransforms.Sort();
	MergeInBoneIndexArrays(OutFillComponentSpaceTransformsRequiredBones, NeededBonesForFillComponentSpaceTransforms);
	FAnimationRuntime::EnsureParentsPresent(OutFillComponentSpaceTransformsRequiredBones, SkeletalMesh->RefSkeleton);
}

void UCAnimMeshComponent::ClearAnimScriptInstance() {
	if (AnimScriptInstance) {
		// We may be doing parallel evaluation on the current anim instance
		// Calling this here with true will block this init till that thread completes
		// and it is safe to continue
		const bool bBlockOnTask = true; // wait on evaluation task so it is safe to swap the buffers
		const bool bPerformPostAnimEvaluation = true; // Do PostEvaluation so we make sure to swap the buffers back. 
		HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);

		AnimScriptInstance->EndNotifyStates();
	}
	AnimScriptInstance = nullptr;
	SubInstances.Empty();
}

bool UCAnimMeshComponent::NeedToSpawnAnimScriptInstance() const {
	IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(AnimClass);
	const USkeleton* AnimSkeleton = (AnimClassInterface) ? AnimClassInterface->GetTargetSkeleton() : nullptr;
	const bool bAnimSkelValid = !AnimClassInterface || (AnimSkeleton && SkeletalMesh && SkeletalMesh->Skeleton->IsCompatible(AnimSkeleton) && AnimSkeleton->IsCompatibleMesh(SkeletalMesh));

	if (AnimClass && bAnimSkelValid) {
		// Check for an 'invalid' AnimScriptInstance:
		// - Could be NULL (in the case of 'standard' first-time initialization)
		// - Could have a different class (in the case where the active anim BP has changed)
		// - Could have a different outer (in the case where an actor has been spawned using an existing actor as a template, as the component is shallow copied directly from the template)
		if ((AnimScriptInstance == nullptr) || (AnimScriptInstance->GetClass() != AnimClass) || AnimScriptInstance->GetOuter() != this) {
			return true;
		}
	}

	return false;
}

void UCAnimMeshComponent::EvaluateAnimation(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve, FCompactPose& OutPose) const {
	if (!InSkeletalMesh) {
		return;
	}

	// We can only evaluate animation if RequiredBones is properly setup for the right mesh!
	if (InSkeletalMesh->Skeleton &&
		InAnimInstance &&
		InAnimInstance->ParallelCanEvaluate(InSkeletalMesh)) {
		InAnimInstance->ParallelEvaluateAnimation(bForceRefpose, InSkeletalMesh, OutCurve, OutPose);
	}
	else {
		OutCurve.InitFrom(&CachedCurveUIDList);
	}
}

void UCAnimMeshComponent::SwapEvaluationContextBuffers() {
	Exchange(AnimEvaluationContext.ComponentSpaceTransforms, GetEditableComponentSpaceTransforms());
	Exchange(AnimEvaluationContext.CachedComponentSpaceTransforms, CachedComponentSpaceTransforms);
	Exchange(AnimEvaluationContext.BoneSpaceTransforms, BoneSpaceTransforms);
	Exchange(AnimEvaluationContext.CachedBoneSpaceTransforms, CachedBoneSpaceTransforms);
	Exchange(AnimEvaluationContext.Curve, AnimCurves);
	Exchange(AnimEvaluationContext.CachedCurve, CachedCurve);
	Exchange(AnimEvaluationContext.RootBoneTranslation, RootBoneTranslation);
}

void UCAnimMeshComponent::FillComponentSpaceTransforms(const USkeletalMesh* InSkeletalMesh, const TArray<FTransform>& InBoneSpaceTransforms, TArray<FTransform>& OutComponentSpaceTransforms) const {
	if (!InSkeletalMesh) {
		return;
	}

	// right now all this does is populate DestSpaceBases
	check(InSkeletalMesh->RefSkeleton.GetNum() == InBoneSpaceTransforms.Num());
	check(InSkeletalMesh->RefSkeleton.GetNum() == OutComponentSpaceTransforms.Num());

	const int32 NumBones = InBoneSpaceTransforms.Num();

#if DO_GUARD_SLOW
	/** Keep track of which bones have been processed for fast look up */
	TArray<uint8, TInlineAllocator<256>> BoneProcessed;
	BoneProcessed.AddZeroed(NumBones);
#endif

	const FTransform* LocalTransformsData = InBoneSpaceTransforms.GetData();
	FTransform* ComponentSpaceData = OutComponentSpaceTransforms.GetData();

	// First bone is always root bone, and it doesn't have a parent.
	{
		check(FillComponentSpaceTransformsRequiredBones[0] == 0);
		OutComponentSpaceTransforms[0] = InBoneSpaceTransforms[0];

#if DO_GUARD_SLOW
		// Mark bone as processed
		BoneProcessed[0] = 1;
#endif
	}

	for (int32 i = 1; i < FillComponentSpaceTransformsRequiredBones.Num(); i++) {
		const int32 BoneIndex = FillComponentSpaceTransformsRequiredBones[i];
		FTransform* SpaceBase = ComponentSpaceData + BoneIndex;

		FPlatformMisc::Prefetch(SpaceBase);

#if DO_GUARD_SLOW
		// Mark bone as processed
		BoneProcessed[BoneIndex] = 1;
#endif
		// For all bones below the root, final component-space transform is relative transform * component-space transform of parent.
		const int32 ParentIndex = InSkeletalMesh->RefSkeleton.GetParentIndex(BoneIndex);
		FTransform* ParentSpaceBase = ComponentSpaceData + ParentIndex;
		FPlatformMisc::Prefetch(ParentSpaceBase);

#if DO_GUARD_SLOW
		// Check the precondition that Parents occur before Children in the RequiredBones array.
		checkSlow(BoneProcessed[ParentIndex] == 1);
#endif
		FTransform::Multiply(SpaceBase, LocalTransformsData + BoneIndex, ParentSpaceBase);

		SpaceBase->NormalizeRotation();

		checkSlow(SpaceBase->IsRotationNormalized());
		checkSlow(!SpaceBase->ContainsNaN());
	}
}

void UCAnimMeshComponent::SetForceRefPose(bool bNewForceRefPose) {
	bForceRefpose = bNewForceRefPose;
	MarkRenderStateDirty();
}

#if WITH_EDITOR

void UCAnimMeshComponent::PerformAnimationEvaluation(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, TArray<FTransform>& OutSpaceBases, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve) {
	PerformAnimationProcessing(InSkeletalMesh, InAnimInstance, true, OutSpaceBases, OutBoneSpaceTransforms, OutRootBoneTranslation, OutCurve);
}

#endif

void UCAnimMeshComponent::PerformAnimationProcessing(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, bool bInDoEvaluation, TArray<FTransform>& OutSpaceBases, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve) {
	// Can't do anything without a SkeletalMesh
	if (!InSkeletalMesh) {
		return;
	}

	// update anim instance
	if (InAnimInstance && InAnimInstance->NeedsUpdate()) {
		InAnimInstance->ParallelUpdateAnimation();
	}

	// Do nothing more if no bones in skeleton.
	if (bInDoEvaluation && OutSpaceBases.Num() > 0) {
		FMemMark Mark(FMemStack::Get());
		FCompactPose EvaluatedPose;

		// evaluate pure animations, and fill up BoneSpaceTransforms
		EvaluateAnimation(InSkeletalMesh, InAnimInstance, OutRootBoneTranslation, OutCurve, EvaluatedPose);
		FinalizePoseEvaluationResult(InSkeletalMesh, OutBoneSpaceTransforms, OutRootBoneTranslation, EvaluatedPose);
		FillComponentSpaceTransforms(InSkeletalMesh, OutBoneSpaceTransforms, OutSpaceBases);
	}
}

void UCAnimMeshComponent::DispatchParallelEvaluationTasks(FActorComponentTickFunction* TickFunction) {
	SwapEvaluationContextBuffers();

	// start parallel work
	check(!IsValidRef(ParallelAnimationEvaluationTask));
	ParallelAnimationEvaluationTask = TGraphTask<FParallelAnimEvalTask>::CreateTask().ConstructAndDispatchWhenReady(this);

	// set up a task to run on the game thread to accept the results
	FGraphEventArray Prerequistes;
	Prerequistes.Add(ParallelAnimationEvaluationTask);
	FGraphEventRef TickCompletionEvent = TGraphTask<FParallelAnimEvalCompleteTask>::CreateTask(&Prerequistes).ConstructAndDispatchWhenReady(this);

	if (TickFunction) {
		TickFunction->GetCompletionHandle()->SetGatherThreadForDontCompleteUntil(ENamedThreads::GameThread);
		TickFunction->GetCompletionHandle()->DontCompleteUntil(TickCompletionEvent);
	}
}

void UCAnimMeshComponent::DoParallelEvaluationTasks_OnGameThread() {
	SwapEvaluationContextBuffers();

	ParallelAnimationEvaluation();

	SwapEvaluationContextBuffers();
}

void UCAnimMeshComponent::ParallelAnimationEvaluation() {
	if (AnimEvaluationContext.bDoInterpolation) {
		PerformAnimationProcessing(AnimEvaluationContext.SkeletalMesh, AnimEvaluationContext.AnimInstance, AnimEvaluationContext.bDoEvaluation, AnimEvaluationContext.CachedComponentSpaceTransforms, AnimEvaluationContext.CachedBoneSpaceTransforms, AnimEvaluationContext.RootBoneTranslation, AnimEvaluationContext.CachedCurve);
	}
	else {
		PerformAnimationProcessing(AnimEvaluationContext.SkeletalMesh, AnimEvaluationContext.AnimInstance, AnimEvaluationContext.bDoEvaluation, AnimEvaluationContext.ComponentSpaceTransforms, AnimEvaluationContext.BoneSpaceTransforms, AnimEvaluationContext.RootBoneTranslation, AnimEvaluationContext.Curve);
	}

	ParallelDuplicateAndInterpolate(AnimEvaluationContext);

	if (AnimEvaluationContext.bDoEvaluation || AnimEvaluationContext.bDoInterpolation) {
		if (AnimEvaluationContext.AnimInstance) {
			AnimEvaluationContext.AnimInstance->UpdateCurvesToEvaluationContext(AnimEvaluationContext);
		}
	}
}

bool UCAnimMeshComponent::HandleExistingParallelEvaluationTask(bool bBlockOnTask, bool bPerformPostAnimEvaluation) {
	if (IsRunningParallelEvaluation()) // We are already processing eval on another thread
	{
		if (bBlockOnTask) {
			check(IsInGameThread()); // Only attempt this from game thread!
			FTaskGraphInterface::Get().WaitUntilTaskCompletes(ParallelAnimationEvaluationTask, ENamedThreads::GameThread);
			CompleteParallelAnimationEvaluation(bPerformPostAnimEvaluation); //Perform completion now
		}
		return true;
	}
	return false;
}

void UCAnimMeshComponent::ApplyAnimationCurvesToComponent(const TMap<FName, float>* InMaterialParameterCurves, const TMap<FName, float>* InAnimationMorphCurves) {
	const bool bContainsMaterialCurves = InMaterialParameterCurves && InMaterialParameterCurves->Num() > 0;
	if (bContainsMaterialCurves) {
		for (auto Iter = InMaterialParameterCurves->CreateConstIterator(); Iter; ++Iter) {
			FName ParameterName = Iter.Key();
			float ParameterValue = Iter.Value();
			SetScalarParameterValueOnMaterials(ParameterName, ParameterValue);
		}
	}

	const bool bContainsMorphCurves = InAnimationMorphCurves && InAnimationMorphCurves->Num() > 0;
	if (SkeletalMesh && bContainsMorphCurves) {
		// we want to append to existing curves - i.e. BP driven curves 
		FAnimationRuntime::AppendActiveMorphTargets(SkeletalMesh, *InAnimationMorphCurves, ActiveMorphTargets, MorphTargetWeights);
	}

	////// TODO AT THE MOMENT IGNORE ALL SLAVES !!!
	///////** Push through curves to slave components */
	//////if (bPropagateCurvesToSlaves && bContainsMorphCurves && bContainsMaterialCurves && SlavePoseComponents.Num() > 0) {
	//////	for (TWeakObjectPtr<USkinnedMeshComponent> MeshComponent : SlavePoseComponents) {
	//////		if (USkeletalMeshComponent* SKComponent = Cast<USkeletalMeshComponent>(MeshComponent.Get())) {
	//////			SKComponent->ApplyAnimationCurvesToComponent(InMaterialParameterCurves, InAnimationMorphCurves);
	//////		}
	//////	}
	//////}
}

void UCAnimMeshComponent::CompleteParallelAnimationEvaluation(bool bDoPostAnimEvaluation) {
	ParallelAnimationEvaluationTask.SafeRelease(); //We are done with this task now, clean up!

	if (bDoPostAnimEvaluation && (AnimEvaluationContext.AnimInstance == AnimScriptInstance) &&
		(AnimEvaluationContext.SkeletalMesh == SkeletalMesh) &&
		(AnimEvaluationContext.ComponentSpaceTransforms.Num() == GetNumComponentSpaceTransforms())) {
		SwapEvaluationContextBuffers();

		PostAnimEvaluation(AnimEvaluationContext);
	}
	AnimEvaluationContext.Clear();
}

void UCAnimMeshComponent::ResetMorphTargetCurves() {
	ActiveMorphTargets.Reset();

	if (SkeletalMesh) {
		MorphTargetWeights.SetNum(SkeletalMesh->MorphTargets.Num());

		// we need this code to ensure the buffer gets cleared whether or not you have morphtarget curve set
		// the case, where you had morphtargets weight on, and when you clear the weight, you want to make sure 
		// the buffer gets cleared and resized
		if (MorphTargetWeights.Num() > 0) {
			FMemory::Memzero(MorphTargetWeights.GetData(), MorphTargetWeights.GetAllocatedSize());
		}
	}
	else {
		MorphTargetWeights.Reset();
	}
}

void UCAnimMeshComponent::UpdateMorphTargetOverrideCurves() {
	if (SkeletalMesh) {
		if (MorphTargetCurves.Num() > 0) {
			FAnimationRuntime::AppendActiveMorphTargets(SkeletalMesh, MorphTargetCurves, ActiveMorphTargets, MorphTargetWeights);
		}
	}
}

void UCAnimMeshComponent::FinalizeAnimationUpdate() {
	// Flip bone buffer and send 'post anim' notification
	FinalizeBoneTransform();

	UpdateChildTransforms(EUpdateTransformFlags::OnlyUpdateIfUsingSocket);

	////// TODO AT THE MOMENT OVERLAP WILL USE PRIMITIVE CAPSULE AND NO ROOT MOTION EXPECTED !!!
	//////// animation often change overlap. 
	//////UpdateOverlaps();

	// update bounds
	InvalidateCachedBounds();
	UpdateBounds();

	// Need to send new bounds to 
	MarkRenderTransformDirty();

	// New bone positions need to be sent to render thread
	MarkRenderDynamicDataDirty();

	// If we have any Slave Components, they need to be refreshed as well.
	RefreshSlaveComponents();
}

void UCAnimMeshComponent::FinalizePoseEvaluationResult(const USkeletalMesh* InMesh, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FCompactPose& InFinalPose) const {
	OutBoneSpaceTransforms = InMesh->RefSkeleton.GetRefBonePose();

	if (InFinalPose.IsValid() && InFinalPose.GetNumBones() > 0) {
		InFinalPose.NormalizeRotations();

		for (const FCompactPoseBoneIndex BoneIndex : InFinalPose.ForEachBoneIndex()) {
			FMeshPoseBoneIndex MeshPoseIndex = InFinalPose.GetBoneContainer().MakeMeshPoseIndex(BoneIndex);
			OutBoneSpaceTransforms[MeshPoseIndex.GetInt()] = InFinalPose[BoneIndex];
		}
	}
	else {
		OutBoneSpaceTransforms = InMesh->RefSkeleton.GetRefBonePose();
	}

	OutRootBoneTranslation = OutBoneSpaceTransforms[0].GetTranslation() - InMesh->RefSkeleton.GetRefBonePose()[0].GetTranslation();
}

void UCAnimMeshComponent::SetAnimInstanceClass(class UClass* NewClass) {
	if (NewClass != nullptr) {
		// set the animation mode
		if (NewClass != AnimClass) {
			// Only need to initialize if it hasn't already been set or we weren't previously using a blueprint instance
			AnimClass = NewClass;
			ClearAnimScriptInstance();
			InitAnim(true);
		}
	}
	else {
		// Need to clear the instance as well as the blueprint.
		// @todo is this it?
		AnimClass = nullptr;
		ClearAnimScriptInstance();
	}
}

bool UCAnimMeshComponent::InitializeAnimScriptInstance(bool bForceReinit) {
	bool bInitializedMainInstance = false;

	if (IsRegistered()) {
		check(SkeletalMesh);

		if (NeedToSpawnAnimScriptInstance()) {
			AnimScriptInstance = NewObject<UCAnimInstanceBase>(this, AnimClass);

			if (AnimScriptInstance) {
				// If we have any sub-instances left we need to clear them out now, we're about to have a new master instance
				SubInstances.Empty();
				AnimScriptInstance->InitializeAnimation();
				bInitializedMainInstance = true;
			}
		}

		if (AnimScriptInstance && !bInitializedMainInstance && bForceReinit) {
			AnimScriptInstance->InitializeAnimation();
			bInitializedMainInstance = true;
		}

		// refresh morph targets - this can happen when re-registration happens
		RefreshMorphTargets();
	}
	return bInitializedMainInstance;
}

void UCAnimMeshComponent::InitAnim(bool bForceReinit) {
	// a lot of places just call InitAnim without checking Mesh, so 
	// I'm moving the check here
	if (SkeletalMesh != nullptr && IsRegistered()) {
		//clear cache UID since we don't know if skeleton changed
		CachedAnimCurveUidVersion = 0;

		// we still need this in case users doesn't call tick, but sent to renderer
		MorphTargetWeights.SetNumZeroed(SkeletalMesh->MorphTargets.Num());

		// We may be doing parallel evaluation on the current anim instance
		// Calling this here with true will block this init till that thread completes
		// and it is safe to continue
		const bool bBlockOnTask = true; // wait on evaluation task so it is safe to continue with Init
		const bool bPerformPostAnimEvaluation = false; // Skip post evaluation, it would be wasted work
		HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);

		bool bBlueprintMismatch = (AnimClass != nullptr) &&
			(AnimScriptInstance != nullptr) && (AnimScriptInstance->GetClass() != AnimClass);

		const USkeleton* AnimSkeleton = (AnimScriptInstance) ? AnimScriptInstance->GetCurrentSkeleton() : nullptr;

		const bool bClearAnimInstance = AnimScriptInstance && !AnimSkeleton;
		const bool bSkeletonMismatch = AnimSkeleton && (AnimScriptInstance->GetCurrentSkeleton() != SkeletalMesh->Skeleton);
		const bool bSkeletonNotCompatible = AnimSkeleton && !bSkeletonMismatch && (AnimSkeleton->IsCompatibleMesh(SkeletalMesh) == false);

		LastPoseTickFrame = 0;

		if (bBlueprintMismatch || bSkeletonMismatch || bSkeletonNotCompatible || bClearAnimInstance) {
			ClearAnimScriptInstance();
		}

		// this has to be called before Initialize Animation because it will required RequiredBones list when InitializeAnimScript
		RecalcRequiredBones(PredictedLODLevel);

		const bool bInitializedAnimInstance = InitializeAnimScriptInstance(bForceReinit);

		// Make sure we have a valid pose.
		// We don't allocate transform data when using MasterPoseComponent, so we have nothing to render.
		if (!MasterPoseComponent.IsValid()) {
			if (bInitializedAnimInstance || (AnimScriptInstance == nullptr)) {
				// In Editor, animations won't get ticked. So Update once to get accurate representation instead of T-Pose.
				// Also allow this to be an option to support pre-4.19 games that might need it..
				const bool bTickAnimationNow =
					((GetWorld()->WorldType == EWorldType::Editor) && !bUseRefPoseOnInitAnim)
					|| UAnimationSettings::Get()->bTickAnimationOnSkeletalMeshInit;

				if (bTickAnimationNow) {
					TickAnimation(0.f, false);
					RefreshBoneTransforms();
				}
				else {
					BoneSpaceTransforms = SkeletalMesh->RefSkeleton.GetRefBonePose();
					//Mini RefreshBoneTransforms (the bit we actually care about)
					FillComponentSpaceTransforms(SkeletalMesh, BoneSpaceTransforms, GetEditableComponentSpaceTransforms());
					bNeedToFlipSpaceBaseBuffers = true; // Have updated space bases so need to flip
					FlipEditableSpaceBases();
				}

				if (bInitializedAnimInstance) {
					OnAnimInitialized.Broadcast();
				}
			}
		}

		UpdateComponentToWorld();
	}
}

FTransform UCAnimMeshComponent::ConvertLocalRootMotionToWorld(const FTransform& InTransform) {
	// Make sure component to world is up to date
	ConditionalUpdateComponentToWorld();

	//Calculate new actor transform after applying root motion to this component
	const FTransform ActorToWorld = GetOwner()->GetTransform();

	const FTransform ComponentToActor = ActorToWorld.GetRelativeTransform(GetComponentTransform());
	const FTransform NewComponentToWorld = InTransform * GetComponentTransform();
	const FTransform NewActorTransform = ComponentToActor * NewComponentToWorld;

	const FVector DeltaWorldTranslation = NewActorTransform.GetTranslation() - ActorToWorld.GetTranslation();

	const FQuat NewWorldRotation = GetComponentTransform().GetRotation() * InTransform.GetRotation();
	const FQuat DeltaWorldRotation = NewWorldRotation * GetComponentTransform().GetRotation().Inverse();

	const FTransform DeltaWorldTransform(DeltaWorldRotation, DeltaWorldTranslation);

	UE_LOG(LogRootMotion, Log, TEXT("ConvertLocalRootMotionToWorld LocalT: %s, LocalR: %s, WorldT: %s, WorldR: %s."),
		*InTransform.GetTranslation().ToCompactString(), *InTransform.GetRotation().Rotator().ToCompactString(),
		*DeltaWorldTransform.GetTranslation().ToCompactString(), *DeltaWorldTransform.GetRotation().Rotator().ToCompactString());

	return DeltaWorldTransform;
}

FRootMotionMovementParams UCAnimMeshComponent::ConsumeRootMotion() {
	float InterpAlpha = 1.0;
	if (ShouldUseUpdateRateOptimizations() && AnimUpdateRateParams->OptimizeMode == FAnimUpdateRateParameters::EOptimizeMode::LookAheadMode) {
		InterpAlpha = FMath::Clamp(AnimUpdateRateParams->ThisTickDelta / (AnimUpdateRateParams->TickedPoseOffestTime + AnimUpdateRateParams->ThisTickDelta), 0.f, 1.f);
	}
	return ConsumeRootMotion_Internal(InterpAlpha);
}

FRootMotionMovementParams UCAnimMeshComponent::ConsumeRootMotion_Internal(float InAlpha) {
	FRootMotionMovementParams RootMotion;

	if (AnimScriptInstance) {
		RootMotion.Accumulate(AnimScriptInstance->ConsumeExtractedRootMotion(InAlpha));

		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			RootMotion.Accumulate(SubInstance->ConsumeExtractedRootMotion(InAlpha));
		}
	}

	return RootMotion;
}

void UCAnimMeshComponent::PostAnimEvaluation(FCAnimEvaluationContext& EvaluationContext) {

	if (EvaluationContext.AnimInstance && EvaluationContext.AnimInstance->NeedsUpdate()) {
		EvaluationContext.AnimInstance->PostUpdateAnimation();

		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			SubInstance->PostUpdateAnimation();
		}
	}

	if (!IsRegistered()) // Notify/Event has caused us to go away so cannot carry on from here
	{
		return;
	}

	if (CVarUseParallelAnimationInterpolation.GetValueOnGameThread() == 0) {
		if (EvaluationContext.bDuplicateToCacheCurve) {
			ensureAlwaysMsgf(AnimCurves.IsValid(), TEXT("Animation Curve is invalid (%s). TotalCount(%d) "),
				*GetNameSafe(SkeletalMesh), AnimCurves.NumValidCurveCount);
			CachedCurve.CopyFrom(AnimCurves);
		}

		if (EvaluationContext.bDuplicateToCacheBones) {
			CachedComponentSpaceTransforms.Reset();
			CachedComponentSpaceTransforms.Append(GetEditableComponentSpaceTransforms());
			CachedBoneSpaceTransforms.Reset();
			CachedBoneSpaceTransforms.Append(BoneSpaceTransforms);
		}

		if (EvaluationContext.bDoInterpolation) {

			float Alpha;
			if (bEnableUpdateRateOptimizations && AnimUpdateRateParams) {
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
					if (AnimScriptInstance) {
						AnimScriptInstance->OnUROPreInterpolation_AnyThread(AnimEvaluationContext);
					}

				for (UCAnimInstanceBase* SubInstance : SubInstances) {
					AnimScriptInstance->OnUROPreInterpolation_AnyThread(AnimEvaluationContext);
				}

				PRAGMA_ENABLE_DEPRECATION_WARNINGS

					Alpha = GetInterpolationAlpha();
			}
			else {
				Alpha = ExternalInterpolationAlpha;
			}

			FAnimationRuntime::LerpBoneTransforms(BoneSpaceTransforms, CachedBoneSpaceTransforms, Alpha, RequiredBones);
			FillComponentSpaceTransforms(SkeletalMesh, BoneSpaceTransforms, GetEditableComponentSpaceTransforms());
			// interpolate curve
			AnimCurves.LerpTo(CachedCurve, Alpha);
		}
	}

	// Work below only matters if bone transforms have been updated.
	// i.e. if we're using URO and skipping a frame with no interpolation, 
	// we don't need to do that work.
	if (EvaluationContext.bDoEvaluation || EvaluationContext.bDoInterpolation) {
		// clear morphtarget curve sets since we're going to apply new changes
		ResetMorphTargetCurves();

		if (AnimScriptInstance) {
#if WITH_EDITOR
			GetEditableAnimationCurves() = AnimCurves;
#endif 
			// curve update happens first
			AnimScriptInstance->UpdateCurvesPostEvaluation();

			// this is same curves, and we don't have to process same for everything. 
			// we just copy curves from main for the case where GetCurveValue works in that instance
			for (UCAnimInstanceBase* SubInstance : SubInstances) {
				SubInstance->CopyCurveValues(*AnimScriptInstance);
			}
		}

		// now update morphtarget curves that are added via SetMorphTarget
		UpdateMorphTargetOverrideCurves();

		// If we have actually evaluated animations, we need to call PostEvaluateAnimation now.
		if (EvaluationContext.bDoEvaluation) {
			if (AnimScriptInstance) {
				AnimScriptInstance->PostEvaluateAnimation();

				for (UCAnimInstanceBase* SubInstance : SubInstances) {
					SubInstance->PostEvaluateAnimation();
				}
			}
		}

		bNeedToFlipSpaceBaseBuffers = true;
		FinalizeAnimationUpdate();
	}
	else {
		// Since we're not calling FinalizeBoneTransforms via PostBlendPhysics,
		// make sure we call ConditionallyDispatchQueuedAnimEvents() in case we ticked, but didn't evalutate.

		/////////////////////////////////////////////////////////////////////////////
		// Notify / Event Handling!
		// This can do anything to our component (including destroy it) 
		// Any code added after this point needs to take that into account
		/////////////////////////////////////////////////////////////////////////////

		ConditionallyDispatchQueuedAnimEvents();
	}

	AnimEvaluationContext.Clear();
}

void UCAnimMeshComponent::ParallelDuplicateAndInterpolate(FCAnimEvaluationContext& InAnimEvaluationContext) {
	if (CVarUseParallelAnimationInterpolation.GetValueOnAnyThread() != 0) {
		if (InAnimEvaluationContext.bDuplicateToCacheCurve) {
			ensureAlwaysMsgf(InAnimEvaluationContext.Curve.IsValid(), TEXT("Animation Curve is invalid (%s). TotalCount(%d) "),
				*GetNameSafe(SkeletalMesh), InAnimEvaluationContext.Curve.NumValidCurveCount);
			InAnimEvaluationContext.CachedCurve.CopyFrom(InAnimEvaluationContext.Curve);
		}

		if (InAnimEvaluationContext.bDuplicateToCacheBones) {
			InAnimEvaluationContext.CachedComponentSpaceTransforms.Reset();
			InAnimEvaluationContext.CachedComponentSpaceTransforms.Append(InAnimEvaluationContext.ComponentSpaceTransforms);
			InAnimEvaluationContext.CachedBoneSpaceTransforms.Reset();
			InAnimEvaluationContext.CachedBoneSpaceTransforms.Append(InAnimEvaluationContext.BoneSpaceTransforms);
		}

		if (InAnimEvaluationContext.bDoInterpolation) {
			float Alpha;
			if (bEnableUpdateRateOptimizations && AnimUpdateRateParams) {
				PRAGMA_DISABLE_DEPRECATION_WARNINGS
					if (AnimScriptInstance) {
						AnimScriptInstance->OnUROPreInterpolation_AnyThread(InAnimEvaluationContext);
					}

				for (UCAnimInstanceBase* SubInstance : SubInstances) {
					SubInstance->OnUROPreInterpolation_AnyThread(InAnimEvaluationContext);
				}
				PRAGMA_ENABLE_DEPRECATION_WARNINGS

					Alpha = GetInterpolationAlpha();
			}
			else {
				Alpha = ExternalInterpolationAlpha;
			}

			FAnimationRuntime::LerpBoneTransforms(InAnimEvaluationContext.BoneSpaceTransforms, InAnimEvaluationContext.CachedBoneSpaceTransforms, Alpha, RequiredBones);
			FillComponentSpaceTransforms(InAnimEvaluationContext.SkeletalMesh, InAnimEvaluationContext.BoneSpaceTransforms, InAnimEvaluationContext.ComponentSpaceTransforms);

			// interpolate curve
			InAnimEvaluationContext.Curve.LerpTo(InAnimEvaluationContext.CachedCurve, Alpha);
		}
	}
}

float UCAnimMeshComponent::GetInterpolationAlpha() const {
	if (AnimUpdateRateParams->OptimizeMode == FAnimUpdateRateParameters::EOptimizeMode::TrailMode) {
		return 0.25f + (1.f / float(FMath::Max(AnimUpdateRateParams->EvaluationRate, 2) * 2));
	}
	else if (AnimUpdateRateParams->OptimizeMode == FAnimUpdateRateParameters::EOptimizeMode::LookAheadMode) {
		return FMath::Clamp(AnimUpdateRateParams->ThisTickDelta / (AnimUpdateRateParams->TickedPoseOffestTime + AnimUpdateRateParams->ThisTickDelta), 0.f, 1.f);
	}
	check(false); // Unknown mode
	return 0.f;
}

#pragma region UObject

void UCAnimMeshComponent::Serialize(FArchive& Ar) {

	Super::Serialize(Ar);

	// to count memory : TODO: REMOVE?
	if (Ar.IsCountingMemory()) {
		BoneSpaceTransforms.CountBytes(Ar);
		RequiredBones.CountBytes(Ar);
	}

	if (Ar.IsLoading() && (Ar.UE4Ver() < VER_UE4_AUTO_WELDING)) {
		BodyInstance.bAutoWeld = false;
	}

	Ar.UsingCustomVersion(FAnimPhysObjectVersion::GUID);
}

void UCAnimMeshComponent::PostLoad() {
	Super::PostLoad();

	// We know for sure that an override was set if this is non-zero.
	if (MinLodModel > 0) {
		bOverrideMinLod = true;
	}
}

FDelegateHandle UCAnimMeshComponent::RegisterOnSkeletalMeshPropertyChanged(const FOnCustomSkeletalMeshPropertyChangedMulticaster::FDelegate& Delegate) {
	return OnSkeletalMeshPropertyChanged.Add(Delegate);
}

void UCAnimMeshComponent::UnregisterOnSkeletalMeshPropertyChanged(FDelegateHandle Handle) {
	OnSkeletalMeshPropertyChanged.Remove(Handle);
}

#if WITH_EDITOR
void UCAnimMeshComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);

	UProperty* PropertyThatChanged = PropertyChangedEvent.Property;

	if (PropertyThatChanged != nullptr) {
		// if the blueprint has changed, recreate the AnimInstance

		if (PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(UCAnimMeshComponent, AnimClass)) {
			InitAnim(false);
		}

		if (PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(UCAnimMeshComponent, SkeletalMesh)) {
			ValidateAnimation();

			if (OnSkeletalMeshPropertyChanged.IsBound()) {
				OnSkeletalMeshPropertyChanged.Broadcast();
			}

			// Skeletal mesh was switched so we should clean up the override materials and dirty the render state to recreate material proxies
			if (OverrideMaterials.Num()) {
				CleanUpOverrideMaterials();
				MarkRenderStateDirty();
			}
		}

		////// TODO AT THE MOMENT DONT SIMULATE PHYSICS, ONLY ANIMATION
		//////// when user changes simulate physics, just make sure to update blendphysics together
		//////// bBlendPhysics isn't the editor exposed property, it should work with simulate physics
		//////if (PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(FBodyInstance, bSimulatePhysics)) {
		//////	bBlendPhysics = BodyInstance.bSimulatePhysics;
		//////}

		////// TODO WE ARE USING ONLY BP
		//////if (PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(FSingleAnimationPlayData, AnimToPlay)) {
		//////	// make sure the animation skeleton matches the current skeletalmesh
		//////	if (AnimationData.AnimToPlay != nullptr && SkeletalMesh && AnimationData.AnimToPlay->GetSkeleton() != SkeletalMesh->Skeleton) {
		//////		UE_LOG(LogAnimation, Warning, TEXT("Invalid animation"));
		//////		AnimationData.AnimToPlay = nullptr;
		//////	}
		//////	else {
		//////		PlayAnimation(AnimationData.AnimToPlay, false);
		//////	}
		//////}

		//////if (PropertyThatChanged->GetFName() == GET_MEMBER_NAME_CHECKED(FSingleAnimationPlayData, SavedPosition)) {
		//////	AnimationData.ValidatePosition();
		//////	SetPosition(AnimationData.SavedPosition, false);
		//////}
	}
}

#endif // WITH_EDITOR

void UCAnimMeshComponent::ValidateAnimation() {
	if (SkeletalMesh && SkeletalMesh->Skeleton == nullptr) {
		UE_LOG(LogAnimation, Warning, TEXT("SkeletalMesh %s has no skeleton. This needs to fixed before an animation can be set"), *SkeletalMesh->GetName());
		AnimClass = nullptr;
		return;
	}
	IAnimClassInterface* AnimClassInterface = IAnimClassInterface::GetFromClass(AnimClass);
	if (AnimClassInterface && SkeletalMesh && AnimClassInterface->GetTargetSkeleton() != SkeletalMesh->Skeleton) {
		if (SkeletalMesh->Skeleton) {
			UE_LOG(LogAnimation, Warning, TEXT("AnimBP %s is incompatible with skeleton %s, removing AnimBP from actor."), *AnimClass->GetName(), *SkeletalMesh->Skeleton->GetName());
		}
		else {
			UE_LOG(LogAnimation, Warning, TEXT("AnimBP %s is incompatible because mesh %s has no skeleton, removing AnimBP from actor."), *AnimClass->GetName(), *SkeletalMesh->GetName());
		}

		AnimClass = nullptr;
	}
}

#pragma endregion

#pragma region UActorComponent

void UCAnimMeshComponent::OnRegister() {
	Super::OnRegister();

	// Ensure we have an empty list of subinstances on registration. Ready for the initialization below 
	// to correctly populate that list.
	SubInstances.Reset();

	// We force an initialization here because we're in one of two cases.
	// 1) First register, no spawned instance, need to initialize
	// 2) We're being re-registered, in which case we've went through
	// OnUnregister and unconditionally uninitialized our anim instances
	// so we need to force initialize them before we begin to tick.
	InitAnim(true);

	if (bRenderStatic || (VisibilityBasedAnimTickOption == EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered && !FApp::CanEverRender())) {
		SetComponentTickEnabled(false);
	}
}

void UCAnimMeshComponent::OnUnregister() {
	const bool bBlockOnTask = true; // wait on evaluation task so we complete any work before this component goes away
	const bool bPerformPostAnimEvaluation = false; // Skip post evaluation, it would be wasted work

	// Wait for any in flight animation evaluation to complete
	HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);

	if (AnimScriptInstance) {
		AnimScriptInstance->UninitializeAnimation();
	}

	for (UCAnimInstanceBase* SubInstance : SubInstances) {
		SubInstance->UninitializeAnimation();
	}
	SubInstances.Reset();

	Super::OnUnregister();
}

void UCAnimMeshComponent::InitializeComponent() {
	Super::InitializeComponent();

	InitAnim(false);
}

void UCAnimMeshComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Update bOldForceRefPose
	bOldForceRefPose = bForceRefpose;

	/** Update the end group and tick priority */
	const bool bDoLateEnd = CVarAnimationDelaysEndGroup.GetValueOnGameThread() > 0;
	const ETickingGroup EndTickGroup = bDoLateEnd ? TG_PostPhysics : TG_PrePhysics;
	if (ThisTickFunction) {
		ThisTickFunction->EndTickGroup = EndTickGroup;

		// Note that if animation is so long that we are blocked in EndPhysics we may want to reduce the priority. However, there is a risk that this function will not go wide early enough.
		// This requires profiling and is very game dependent so cvar for now makes sense
		bool bDoHiPri = CVarHiPriSkinnedMeshesTicks.GetValueOnGameThread() > 0;
		if (ThisTickFunction->bHighPriority != bDoHiPri) {
			ThisTickFunction->SetPriorityIncludingPrerequisites(bDoHiPri);
		}
	}

	// If we are waiting for ParallelEval to complete or if we require Physics, 
	// then FinalizeBoneTransform will be called and Anim events will be dispatched there. 
	// We prefer doing it there so these events are triggered once we have a new updated pose.
	// Note that it's possible that FinalizeBoneTransform has already been called here if not using ParallelUpdate.
	// or it's possible that it hasn't been called at all if we're skipping Evaluate due to not being visible.
	// ConditionallyDispatchQueuedAnimEvents will catch that and only Dispatch events if not already done.
	if (!IsRunningParallelEvaluation()) {
		/////////////////////////////////////////////////////////////////////////////
		// Notify / Event Handling!
		// This can do anything to our component (including destroy it) 
		// Any code added after this point needs to take that into account
		/////////////////////////////////////////////////////////////////////////////

		ConditionallyDispatchQueuedAnimEvents();
	}
}

void UCAnimMeshComponent::BeginPlay() {
	Super::BeginPlay();
	if (AnimScriptInstance) {
		AnimScriptInstance->NativeBeginPlay();
		AnimScriptInstance->BlueprintBeginPlay();
	}
}

#pragma endregion

#pragma region USceneComponent

FBoxSphereBounds UCAnimMeshComponent::CalcBounds(const FTransform& LocalToWorld) const {
	// fixme laurent - extend concept of LocalBounds to all SceneComponent
	// as rendered calls CalcBounds*() directly in FScene::UpdatePrimitiveTransform, which is pretty expensive for SkelMeshes.
	// No need to calculated that again, just use cached local bounds.
	if (bCachedLocalBoundsUpToDate) {
		if (bIncludeComponentLocationIntoBounds) {
			const FVector ComponentLocation = GetComponentLocation();
			return CachedLocalBounds.TransformBy(LocalToWorld) + FBoxSphereBounds(ComponentLocation, FVector(1.0f), 1.0f);
		}
		else {
			return CachedLocalBounds.TransformBy(LocalToWorld);
		}
	}
	// Calculate new bounds
	else {
		FVector RootBoneOffset = RootBoneTranslation;

		////// TODO AT THE MOMENT IGNORE MASTERPOSE
		//////// if to use MasterPoseComponent's fixed skel bounds, 
		//////// send MasterPoseComponent's Root Bone Translation
		//////if (MasterPoseComponent.IsValid()) {
		//////	const USkinnedMeshComponent* const MasterPoseComponentInst = MasterPoseComponent.Get();
		//////	check(MasterPoseComponentInst);
		//////	if (MasterPoseComponentInst->SkeletalMesh &&
		//////		MasterPoseComponentInst->bComponentUseFixedSkelBounds &&
		//////		MasterPoseComponentInst->IsA((USkeletalMeshComponent::StaticClass()))) {
		//////		const USkeletalMeshComponent* BaseComponent = CastChecked<USkeletalMeshComponent>(MasterPoseComponentInst);
		//////		RootBoneOffset = BaseComponent->RootBoneTranslation; // Adjust bounds by root bone translation
		//////	}
		//////}

		FBoxSphereBounds NewBounds = CalcMeshBound(RootBoneOffset, true, LocalToWorld);

		if (bIncludeComponentLocationIntoBounds) {
			const FVector ComponentLocation = GetComponentLocation();
			NewBounds = NewBounds + FBoxSphereBounds(ComponentLocation, FVector(1.0f), 1.0f);
		}

		bCachedLocalBoundsUpToDate = true;
		CachedLocalBounds = NewBounds.TransformBy(LocalToWorld.ToInverseMatrixWithScale());

		return NewBounds;
	}
}

void UCAnimMeshComponent::OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport) {
	// We are handling the physics move below, so don't handle it at higher levels
	Super::OnUpdateTransform(UpdateTransformFlags | EUpdateTransformFlags::SkipPhysicsUpdate, Teleport);

	// Pass teleports on to anything in the animation tree that might be interested (e.g. AnimDynamics, RigidBody Node, etc.)
	if (Teleport != ETeleportType::None) {
		ResetAnimInstanceDynamics(Teleport);
	}
}

#pragma endregion

#pragma region USkinnedMeshComponent

bool UCAnimMeshComponent::UpdateLODStatus() {
	if (Super::UpdateLODStatus()) {
		bRequiredBonesUpToDate = false;
		return true;
	}
	return false;
}

void UCAnimMeshComponent::UpdateVisualizeLODString(FString& DebugString) {
	Super::UpdateVisualizeLODString(DebugString);

	uint32 NumVertices = 0;
	if (SkeletalMesh) {
		if (FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering()) {
			if (RenderData->LODRenderData.IsValidIndex(PredictedLODLevel)) {
				NumVertices = RenderData->LODRenderData[PredictedLODLevel].GetNumVertices();
			}
		}
	}

	DebugString = DebugString + FString::Printf(TEXT("\nRequiredBones(%d) NumVerts(%d)"),
		RequiredBones.Num(), NumVertices);
}

void UCAnimMeshComponent::RefreshBoneTransforms(FActorComponentTickFunction* TickFunction) {

	check(IsInGameThread()); //Only want to call this from the game thread as we set up tasks etc

	if (!SkeletalMesh || GetNumComponentSpaceTransforms() == 0) {
		return;
	}

	// Recalculate the RequiredBones array, if necessary
	if (!bRequiredBonesUpToDate) {
		RecalcRequiredBones(PredictedLODLevel);
	}
	// if curves have to be refreshed
	else if (!AreRequiredCurvesUpToDate()) {
		RecalcRequiredCurves();
	}

	const bool bCachedShouldUseUpdateRateOptimizations = ShouldUseUpdateRateOptimizations() && AnimUpdateRateParams != nullptr;
	const bool bDoEvaluationRateOptimization = (bExternalTickRateControlled && bExternalEvaluationRateLimited) || (bCachedShouldUseUpdateRateOptimizations && AnimUpdateRateParams->DoEvaluationRateOptimizations());

	//Handle update rate optimization setup
	//Dont mark cache as invalid if we aren't performing optimization anyway
	const bool bInvalidCachedBones = bDoEvaluationRateOptimization &&
		((BoneSpaceTransforms.Num() != SkeletalMesh->RefSkeleton.GetNum())
			|| (BoneSpaceTransforms.Num() != CachedBoneSpaceTransforms.Num())
			|| (GetNumComponentSpaceTransforms() != CachedComponentSpaceTransforms.Num()));

	TArray<uint16> const* CurrentAnimCurveUIDFinder = (AnimScriptInstance) ? &AnimScriptInstance->GetRequiredBones().GetUIDToArrayLookupTable() : nullptr;
	const bool bAnimInstanceHasCurveUIDList = CurrentAnimCurveUIDFinder != nullptr;

	const int32 CurrentCurveCount = (CurrentAnimCurveUIDFinder) ? FBlendedCurve::GetValidElementCount(CurrentAnimCurveUIDFinder) : 0;

	const bool bInvalidCachedCurve = bDoEvaluationRateOptimization &&
		bAnimInstanceHasCurveUIDList &&
		(CachedCurve.UIDToArrayIndexLUT != CurrentAnimCurveUIDFinder || CachedCurve.Num() != CurrentCurveCount);

	const bool bShouldDoEvaluation = !bDoEvaluationRateOptimization || bInvalidCachedBones || bInvalidCachedCurve || (bExternalTickRateControlled && bExternalUpdate) || (bCachedShouldUseUpdateRateOptimizations && !AnimUpdateRateParams->ShouldSkipEvaluation());

	const bool bShouldInterpolateSkippedFrames = (bExternalTickRateControlled && bExternalInterpolate) || (bCachedShouldUseUpdateRateOptimizations && AnimUpdateRateParams->ShouldInterpolateSkippedFrames());

	const bool bShouldDoInterpolation = TickFunction != nullptr && bDoEvaluationRateOptimization && !bInvalidCachedBones && bShouldInterpolateSkippedFrames && bAnimInstanceHasCurveUIDList;

	const bool bShouldDoParallelInterpolation = bShouldDoInterpolation && CVarUseParallelAnimationInterpolation.GetValueOnGameThread() == 1;

	const bool bDoPAE = !!CVarUseParallelAnimationEvaluation.GetValueOnGameThread() && FApp::ShouldUseThreadingForPerformance();

	const bool bMainInstanceValidForParallelWork = AnimScriptInstance == nullptr || AnimScriptInstance->CanRunParallelWork();
	const bool bHasValidInstanceForParallelWork = HasValidAnimationInstance() && bMainInstanceValidForParallelWork;
	const bool bDoParallelEvaluation = bHasValidInstanceForParallelWork && bDoPAE && (bShouldDoEvaluation || bShouldDoParallelInterpolation) && TickFunction && (TickFunction->GetActualTickGroup() == TickFunction->TickGroup) && TickFunction->IsCompletionHandleValid();
	const bool bBlockOnTask = !bDoParallelEvaluation;  // If we aren't trying to do parallel evaluation then we
															// will need to wait on an existing task.

	const bool bPerformPostAnimEvaluation = true;
	if (HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation)) {
		return;
	}

	AnimEvaluationContext.SkeletalMesh = SkeletalMesh;
	AnimEvaluationContext.AnimInstance = AnimScriptInstance;

	if (CurrentAnimCurveUIDFinder) {
		if (AnimCurves.UIDToArrayIndexLUT != CurrentAnimCurveUIDFinder || AnimCurves.Num() != CurrentCurveCount) {
			AnimCurves.InitFrom(CurrentAnimCurveUIDFinder);
		}
	}
	else {
		AnimCurves.Empty();
	}

	AnimEvaluationContext.bDoEvaluation = bShouldDoEvaluation;
	AnimEvaluationContext.bDoInterpolation = bShouldDoInterpolation;
	AnimEvaluationContext.bDuplicateToCacheBones = bInvalidCachedBones || (bDoEvaluationRateOptimization && AnimEvaluationContext.bDoEvaluation && !AnimEvaluationContext.bDoInterpolation);
	AnimEvaluationContext.bDuplicateToCacheCurve = bInvalidCachedCurve || (bDoEvaluationRateOptimization && AnimEvaluationContext.bDoEvaluation && !AnimEvaluationContext.bDoInterpolation && CurrentAnimCurveUIDFinder != nullptr);
	if (!bDoEvaluationRateOptimization) {
		//If we aren't optimizing clear the cached local atoms
		CachedBoneSpaceTransforms.Reset();
		CachedComponentSpaceTransforms.Reset();
		CachedCurve.Empty();
	}

	if (bShouldDoEvaluation) {
		// If we need to eval the graph, and we're not going to update it.
		// make sure it's been ticked at least once!
		{
			bool bShouldTickAnimation = false;
			if (AnimScriptInstance && !AnimScriptInstance->NeedsUpdate()) {
				bShouldTickAnimation = bShouldTickAnimation || !AnimScriptInstance->GetUpdateCounter().HasEverBeenUpdated();
				for (const UCAnimInstanceBase* SubInstance : SubInstances) {
					bShouldTickAnimation = bShouldTickAnimation || (SubInstance && !SubInstance->GetUpdateCounter().HasEverBeenUpdated());
				}
			}

			bShouldTickAnimation = bShouldTickAnimation;

			if (bShouldTickAnimation) {
				// We bypass TickPose() and call TickAnimation directly, so URO doesn't intercept us.
				TickAnimation(0.f, false);
			}
		}

		// If we're going to evaluate animation, call PreEvaluateAnimation()
		{
			if (AnimScriptInstance) {
				AnimScriptInstance->PreEvaluateAnimation();

				for (UCAnimInstanceBase* SubInstance : SubInstances) {
					SubInstance->PreEvaluateAnimation();
				}
			}
		}
	}

	if (bDoParallelEvaluation) {
		DispatchParallelEvaluationTasks(TickFunction);
	}
	else {
		if (AnimEvaluationContext.bDoEvaluation || AnimEvaluationContext.bDoInterpolation) {
			DoParallelEvaluationTasks_OnGameThread();
		}
		else {
			if (!AnimEvaluationContext.bDoInterpolation) {

				if (CachedBoneSpaceTransforms.Num()) {
					BoneSpaceTransforms.Reset();
					BoneSpaceTransforms.Append(CachedBoneSpaceTransforms);
				}
				if (CachedComponentSpaceTransforms.Num()) {
					TArray<FTransform>& LocalEditableSpaceBases = GetEditableComponentSpaceTransforms();
					LocalEditableSpaceBases.Reset();
					LocalEditableSpaceBases.Append(CachedComponentSpaceTransforms);
				}
				if (CachedCurve.IsValid()) {
					AnimCurves.CopyFrom(CachedCurve);
				}
			}
			if (AnimScriptInstance && AnimScriptInstance->NeedsUpdate()) {
				AnimScriptInstance->ParallelUpdateAnimation();
			}
		}

		PostAnimEvaluation(AnimEvaluationContext);
		AnimEvaluationContext.Clear();
	}
}

void UCAnimMeshComponent::TickPose(float DeltaTime, bool bNeedsValidRootMotion) {
	Super::TickPose(DeltaTime, bNeedsValidRootMotion);

	if (ShouldTickAnimation()) {
		// Don't care about roll over, just care about uniqueness (and 32-bits should give plenty).
		LastPoseTickFrame = static_cast<uint32>(GFrameCounter);

		float DeltaTimeForTick;
		if (bExternalTickRateControlled) {
			DeltaTimeForTick = ExternalDeltaTime;
		}
		else if (ShouldUseUpdateRateOptimizations()) {
			DeltaTimeForTick = DeltaTime + AnimUpdateRateParams->GetTimeAdjustment();
		}
		else {
			DeltaTimeForTick = DeltaTime;
		}

		TickAnimation(DeltaTimeForTick, bNeedsValidRootMotion);
		if (CVarSpewAnimRateOptimization.GetValueOnGameThread() > 0 && Ticked.Increment() == 500) {
			UE_LOG(LogTemp, Display, TEXT("%d Ticked %d NotTicked"), Ticked.GetValue(), NotTicked.GetValue());
			Ticked.Reset();
			NotTicked.Reset();
		}
	}
	else if (!bExternalTickRateControlled) {
		if (AnimScriptInstance) {
			AnimScriptInstance->OnUROSkipTickAnimation();
		}

		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			SubInstance->OnUROSkipTickAnimation();
		}

		if (CVarSpewAnimRateOptimization.GetValueOnGameThread()) {
			NotTicked.Increment();
		}
	}
}

void UCAnimMeshComponent::UpdateSlaveComponent() {
	check(MasterPoseComponent.IsValid());

	ResetMorphTargetCurves();

	////// TODO AT THE MOMENT IGNORE MASTERPOSE
	//////if (USkeletalMeshComponent* MasterSMC = Cast<USkeletalMeshComponent>(MasterPoseComponent.Get())) {
	//////	// first set any animation-driven curves from the master SMC
	//////	if (MasterSMC->AnimScriptInstance) {
	//////		MasterSMC->AnimScriptInstance->RefreshCurves(this);
	//////	}

	//////	// we changed order of morphtarget to be overriden by SetMorphTarget from BP
	//////	// so this has to go first
	//////	// now propagate BP-driven curves from the master SMC...
	//////	if (SkeletalMesh) {
	//////		check(MorphTargetWeights.Num() == SkeletalMesh->MorphTargets.Num());
	//////		if (MasterSMC->MorphTargetCurves.Num() > 0) {
	//////			FAnimationRuntime::AppendActiveMorphTargets(SkeletalMesh, MasterSMC->MorphTargetCurves, ActiveMorphTargets, MorphTargetWeights);
	//////		}

	//////		// if slave also has it, add it here. 
	//////		if (MorphTargetCurves.Num() > 0) {
	//////			FAnimationRuntime::AppendActiveMorphTargets(SkeletalMesh, MorphTargetCurves, ActiveMorphTargets, MorphTargetWeights);
	//////		}
	//////	}

	//////}

	Super::UpdateSlaveComponent();
}

bool UCAnimMeshComponent::ShouldUpdateTransform(bool bLODHasChanged) const {
#if WITH_EDITOR	
	// If we're in an editor world (Non running, WorldType will be PIE when simulating or in PIE) then we only want transform updates on LOD changes as the
	// animation isn't running so it would just waste CPU time
	if (GetWorld()->WorldType == EWorldType::Editor) {
		if (bUpdateAnimationInEditor) {
			return true;
		}

		////// TODO AT THE MOMENT IGNORE MASTERPOSE
		//////// if master pose is ticking, slave also has to update it
		//////if (MasterPoseComponent.IsValid()) {
		//////	const USkeletalMeshComponent* Master = CastChecked<USkeletalMeshComponent>(MasterPoseComponent.Get());
		//////	if (Master->GetUpdateAnimationInEditor()) {
		//////		return true;
		//////	}
		//////}

		return bLODHasChanged;
	}
#endif

	// If forcing RefPose we can skip updating the skeleton for perf, except if it's using MorphTargets.
	const bool bSkipBecauseOfRefPose = bForceRefpose && bOldForceRefPose && (MorphTargetCurves.Num() == 0) && ((AnimScriptInstance) ? !AnimScriptInstance->HasMorphTargetCurves() : true);

	return (Super::ShouldUpdateTransform(bLODHasChanged) && !bNoSkeletonUpdate && !bSkipBecauseOfRefPose);
}

bool UCAnimMeshComponent::ShouldTickPose() const {
	// When we stop root motion we go back to ticking after CharacterMovement. Unfortunately that means that we could tick twice that frame.
	// So only enforce a single tick per frame.
	const bool bAlreadyTickedThisFrame = PoseTickedThisFrame();

#if WITH_EDITOR
	if (GetWorld()->WorldType == EWorldType::Editor) {
		if (bUpdateAnimationInEditor) {
			return true;
		}
	}
#endif 

	// When playing networked Root Motion Montages, we want these to play on dedicated servers and remote clients for networking and position correction purposes.
	// So we force pose updates in that case to keep root motion and position in sync.
	const bool bShouldTickBasedOnVisibility = ((VisibilityBasedAnimTickOption < EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered) || bRecentlyRendered);

	return (bShouldTickBasedOnVisibility && IsRegistered() && AnimScriptInstance && !bPauseAnims && GetWorld()->AreActorsInitialized() && !bNoSkeletonUpdate);
}

bool UCAnimMeshComponent::AllocateTransformData() {
	// Allocate transforms if not present.
	if (Super::AllocateTransformData()) {
		if (BoneSpaceTransforms.Num() != SkeletalMesh->RefSkeleton.GetNum()) {
			BoneSpaceTransforms = SkeletalMesh->RefSkeleton.GetRefBonePose();
		}
		return true;
	}

	BoneSpaceTransforms.Empty();
	return false;
}

void UCAnimMeshComponent::DeallocateTransformData() {
	Super::DeallocateTransformData();
	BoneSpaceTransforms.Empty();
}

void UCAnimMeshComponent::HideBone(int32 BoneIndex, EPhysBodyOp PhysBodyOption) {
	Super::HideBone(BoneIndex, PhysBodyOption);

	if (!SkeletalMesh) {
		return;
	}

	if (MasterPoseComponent.IsValid()) {
		return;
	}

	// if valid bone index
	if (BoneIndex >= 0 && GetNumBones() > BoneIndex) {
		bRequiredBonesUpToDate = false;
	}
	else {
		UE_LOG(LogSkeletalMesh, Warning, TEXT("HideBone[%s]: Invalid Body Index (%d) has entered. This component doesn't contain buffer for the given body."), *GetNameSafe(SkeletalMesh), BoneIndex);
	}
}

void UCAnimMeshComponent::UnHideBone(int32 BoneIndex) {
	Super::UnHideBone(BoneIndex);

	if (!SkeletalMesh) {
		return;
	}

	if (MasterPoseComponent.IsValid()) {
		return;
	}

	if (BoneIndex >= 0 && GetNumBones() > BoneIndex) {
		bRequiredBonesUpToDate = false;

		//FName HideBoneName = SkeletalMesh->RefSkeleton.GetBoneName(BoneIndex);
		// It's okay to turn this on for terminated bodies
		// It won't do any if BodyData isn't found
		// @JTODO
		//SetCollisionBelow(true, HideBoneName);
	}
	else {
		UE_LOG(LogSkeletalMesh, Warning, TEXT("UnHideBone[%s]: Invalid Body Index (%d) has entered. This component doesn't contain buffer for the given body."), *GetNameSafe(SkeletalMesh), BoneIndex);
	}
}

void UCAnimMeshComponent::SetSkeletalMesh(USkeletalMesh* InSkelMesh, bool bReinitPose) {
	if (InSkelMesh == SkeletalMesh) {
		// do nothing if the input mesh is the same mesh we're already using.
		return;
	}

	// We may be doing parallel evaluation on the current anim instance
	// Calling this here with true will block this init till that thread completes
	// and it is safe to continue
	const bool bBlockOnTask = true; // wait on evaluation task so it is safe to continue with Init
	const bool bPerformPostAnimEvaluation = true;
	HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation);

	{
		FRenderStateRecreator RenderStateRecreator(this);
		Super::SetSkeletalMesh(InSkelMesh, bReinitPose);

#if WITH_EDITOR
		ValidateAnimation();
#endif

		ClearMorphTargets();

		InitAnim(bReinitPose);
	}

	// Mark cached material parameter names dirty
	MarkCachedMaterialParameterNameIndicesDirty();

	// Update this component streaming data.
	IStreamingManager::Get().NotifyPrimitiveUpdated(this);
}

void UCAnimMeshComponent::FinalizeBoneTransform() {
	Super::FinalizeBoneTransform();

	// After pose has been finalized, dispatch AnimNotifyEvents in case they want to use up to date pose.
	// (For example attaching particle systems to up to date sockets).

	/////////////////////////////////////////////////////////////////////////////
	// Notify / Event Handling!
	// This can do anything to our component (including destroy it) 
	// Any code added after this point needs to take that into account
	/////////////////////////////////////////////////////////////////////////////

	ConditionallyDispatchQueuedAnimEvents();

	OnBoneTransformsFinalized.Broadcast();
}

void UCAnimMeshComponent::SetRefPoseOverride(const TArray<FTransform>& NewRefPoseTransforms) {
	Super::SetRefPoseOverride(NewRefPoseTransforms);
	bRequiredBonesUpToDate = false;
}

void UCAnimMeshComponent::ClearRefPoseOverride() {
	Super::ClearRefPoseOverride();
	bRequiredBonesUpToDate = false;
}

void UCAnimMeshComponent::DispatchParallelTickPose(FActorComponentTickFunction* TickFunction) {
	check(TickFunction);

	if (SkeletalMesh != nullptr) {
		if (AnimScriptInstance && AnimScriptInstance->NeedsUpdate()) {
			if (ShouldTickAnimation()) {

				// This duplicates *some* of the logic from RefreshBoneTransforms()
				const bool bDoPAE = !!CVarUseParallelAnimationEvaluation.GetValueOnGameThread() && FApp::ShouldUseThreadingForPerformance();

				const bool bDoParallelUpdate = bDoPAE && (TickFunction->GetActualTickGroup() == TickFunction->TickGroup) && TickFunction->IsCompletionHandleValid();

				const bool bBlockOnTask = !bDoParallelUpdate;   // If we aren't trying to do parallel update then we
																// will need to wait on an existing task.

				const bool bPerformPostAnimEvaluation = true;
				if (HandleExistingParallelEvaluationTask(bBlockOnTask, bPerformPostAnimEvaluation)) {
					return;
				}

				// Do a mini-setup of the eval context
				AnimEvaluationContext.SkeletalMesh = SkeletalMesh;
				AnimEvaluationContext.AnimInstance = AnimScriptInstance;

				// We dont set up the Curve here, as we dont use it in Update()
				AnimCurves.Empty();

				// Set us up to NOT perform evaluation
				AnimEvaluationContext.bDoEvaluation = false;
				AnimEvaluationContext.bDoInterpolation = false;
				AnimEvaluationContext.bDuplicateToCacheBones = false;
				AnimEvaluationContext.bDuplicateToCacheCurve = false;

				if (bDoParallelUpdate) {
					DispatchParallelEvaluationTasks(TickFunction);
				}
				else {
					// we cant update on a worker thread, so perform the work here
					DoParallelEvaluationTasks_OnGameThread();
					PostAnimEvaluation(AnimEvaluationContext);
				}
			}
		}
	}
}

void UCAnimMeshComponent::AddSlavePoseComponent(USkinnedMeshComponent* SkinnedMeshComponent) {
	Super::AddSlavePoseComponent(SkinnedMeshComponent);

	if (UCAnimMeshComponent* SkeletalMeshComponent = Cast<UCAnimMeshComponent>(SkinnedMeshComponent)) {
		SkeletalMeshComponent->bRequiredBonesUpToDate = false;
	}

	bRequiredBonesUpToDate = false;
}

void UCAnimMeshComponent::RemoveSlavePoseComponent(USkinnedMeshComponent* SkinnedMeshComponent) {
	Super::RemoveSlavePoseComponent(SkinnedMeshComponent);

	if (UCAnimMeshComponent* SkeletalMeshComponent = Cast<UCAnimMeshComponent>(SkinnedMeshComponent)) {
		SkeletalMeshComponent->bRequiredBonesUpToDate = false;
	}

	bRequiredBonesUpToDate = false;
}

void UCAnimMeshComponent::RefreshMorphTargets() {
	ResetMorphTargetCurves();

	if (SkeletalMesh && AnimScriptInstance) {
		// as this can be called from any worker thread (i.e. from CreateRenderState_Concurrent) we cant currently be doing parallel evaluation
		check(!IsRunningParallelEvaluation());
		AnimScriptInstance->RefreshCurves(this);

		for (UCAnimInstanceBase* SubInstance : SubInstances) {
			SubInstance->RefreshCurves(this);
		}
	}
	//// TODO AT THE MOMENT IGNORE MASTERPOSE
	////else if (USkeletalMeshComponent* MasterSMC = Cast<USkeletalMeshComponent>(MasterPoseComponent.Get())) {
	////	if (MasterSMC->AnimScriptInstance) {
	////		MasterSMC->AnimScriptInstance->RefreshCurves(this);
	////	}
	////}
	UpdateMorphTargetOverrideCurves();
}

#pragma endregion
