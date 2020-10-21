// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interface_CollisionDataProvider.h"
#include "Base/CAnimMeshComponentBase.h"
#include "CAnimEvaluationContext.h"
#include "CAnimMeshComponent.generated.h"

class Error;
class FPrimitiveDrawInterface;
class UCAnimInstanceBase;
class UPhysicalMaterial;
class USkeletalMeshComponent;
class UClass;
class UAnimationAsset;
class UParticleSystemComponent;
struct FConstraintInstance;
struct FNavigableGeometryExport;
struct FCompactPose;
struct FBlendedHeapCurve;
struct FRootMotionMovementParams;
struct FPoseSnapshot;

// DELEGATES
class FOnAnimInitialized;
class FOnBoneTransformsFinalized;

typedef uint16 FBoneIndexType;

DECLARE_MULTICAST_DELEGATE(FOnCustomSkeletalMeshPropertyChangedMulticaster)

UCLASS()
class UCAnimMeshComponent : public UCAnimMeshComponentBase, public IInterface_CollisionDataProvider {

	GENERATED_UCLASS_BODY()

	friend class FSkinnedMeshComponentRecreateRenderStateContext;
	friend class FParallelAnimEvalCompleteTask;
	friend class USkeletalMesh;
	friend class UDebugSkelMeshComponent;
	friend class FSkeletalMeshComponentDetails;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		class TSubclassOf<UCAnimInstanceBase> AnimClass;

	UPROPERTY(transient, NonTransactional)
		UCAnimInstanceBase* AnimScriptInstance;

	UPROPERTY(transient)
		TArray<UCAnimInstanceBase*> SubInstances;

public:

	TArray<FTransform> GetBoneSpaceTransforms() {
		HandleExistingParallelEvaluationTask(true, true);
		return BoneSpaceTransforms;
	}

private:

	TArray<FTransform> BoneSpaceTransforms;

public:

	UPROPERTY(transient)
		FVector RootBoneTranslation;

	/** If bEnableLineCheckWithBounds is true, scale the bounds by this value before doing line check. */
	UPROPERTY()
		FVector LineCheckBoundsScale;

	FBlendedHeapCurve AnimCurves;

	const TArray<FTransform>& GetCachedComponentSpaceTransforms() const { return CachedComponentSpaceTransforms; }

private:

	UPROPERTY(Transient)
		TArray<FTransform> CachedBoneSpaceTransforms;

	UPROPERTY(Transient)
		TArray<FTransform> CachedComponentSpaceTransforms;

	FBlendedHeapCurve CachedCurve;

public:
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Animation)
		float GlobalAnimRateScale;

private:
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = SkeletalMesh)
		uint8 bAllowAnimCurveEvaluation : 1;

public:
	/**
	 * Optimization
	 */

	 /** Skips Ticking and Bone Refresh. */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = SkeletalMesh)
		uint8 bNoSkeletonUpdate : 1;

	/** pauses this component's animations (doesn't tick them, but still refreshes bones) */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = Animation)
		uint8 bPauseAnims : 1;

	/** On InitAnim should we set to ref pose (if false use first tick of animation data)*/
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = Animation)
		uint8 bUseRefPoseOnInitAnim : 1;

	/**
	* Uses skinned data for collision data.
	*/
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = SkeletalMesh)
		uint8 bEnablePerPolyCollision : 1;

	/**
	 * Misc
	 */

	 /** If true, force the mesh into the reference pose - is an optimization. */
	UPROPERTY()
		uint8 bForceRefpose : 1;

	/** If bForceRefPose was set last tick. */
	UPROPERTY()
		uint8 bOldForceRefPose : 1;

	/** If false, indicates that on the next call to UpdateSkelPose the RequiredBones array should be recalculated. */
	UPROPERTY(transient)
		uint8 bRequiredBonesUpToDate : 1;

	/** If true, AnimTree has been initialised. */
	UPROPERTY(transient)
		uint8 bAnimTreeInitialised : 1;

	/** If true, the Location of this Component will be included into its bounds calculation
	* (this can be useful when using SMU_OnlyTickPoseWhenRendered on a character that moves away from the root and no bones are left near the origin of the component) */
	UPROPERTY(EditAnywhere, AdvancedDisplay, BlueprintReadOnly, Category = SkeletalMesh)
		uint8 bIncludeComponentLocationIntoBounds : 1;

	/** If true, line checks will test against the bounding box of this skeletal mesh component and return a hit if there is a collision. */
	UPROPERTY()
		uint8 bEnableLineCheckWithBounds : 1;

	/** If true, propagates calls to ApplyAnimationCurvesToComponent for slave components, only needed if slave components do not tick themselves */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MasterPoseComponent)
		uint8 bPropagateCurvesToSlaves : 1;

protected:

#if WITH_EDITORONLY_DATA
	/** If true, this will Tick until disabled */
	UPROPERTY(AdvancedDisplay, EditInstanceOnly, transient, Category = SkeletalMesh)
		uint8 bUpdateAnimationInEditor : 1;
#endif

private:

	UPROPERTY(Transient)
		uint8 bNeedsQueuedAnimEventsDispatched : 1;

	uint8 bPostEvaluatingAnimation : 1;

public:

	/** Cache AnimCurveUidVersion from Skeleton and this will be used to identify if it needs to be updated */
	UPROPERTY(transient)
		uint16 CachedAnimCurveUidVersion;

private:

	/** You can choose to disable certain curves if you prefer.
	 * This is transient curves that will be ignored by animation system if you choose this */
	UPROPERTY(transient)
		TArray<FName> DisallowedAnimCurves;

public:
	/** Get the anim instance class via getter callable by sequencer.  */
	UFUNCTION(BlueprintInternalUseOnly)
		UClass*  GetAnimClass() { return AnimClass; }

	/** Set the anim instance class. Clears and re-initializes the anim instance with the new class and sets animation mode to 'AnimationBlueprint' */
	void SetAnimInstanceClass(UClass* NewClass);

	/**
	 * Returns the animation instance that is driving the class (if available). This is typically an instance of
	 * the class set as AnimBlueprintGeneratedClass (generated by an animation blueprint)
	 * Since this instance is transient, it is not safe to be used during construction script
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (Keywords = "AnimBlueprint", UnsafeDuringActorConstruction = "true"))
		UCAnimInstanceBase * GetAnimInstance() const { return AnimScriptInstance; }

	/**
	 * Returns the a tagged sub-instance node. If no sub instances are found or none are tagged with the
	 * supplied name, this will return NULL.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (Keywords = "AnimBlueprint"))
		UCAnimInstanceBase* GetSubInstanceByTag(FName InTag) const;

	/**
	 * Returns all tagged sub-instance nodes that match the tag.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (Keywords = "AnimBlueprint"))
		void GetSubInstancesByTag(FName InTag, TArray<UCAnimInstanceBase*>& OutSubInstances) const;

	/**
	 * Runs through all layer nodes, attempting to find nodes that are implemented by the specified class, then sets up a sub instance of the class for each.
	 * Allocates one sub instance to run each of the groups specified in the class, so state is shared.
	 * If InClass is null, then layers are reset to their defaults.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh|Layers")
		void SetLayerOverlay(TSubclassOf<UCAnimInstanceBase> InClass);

	/** Gets the sub instance corresponding to the specified group */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh|Layers")
		UCAnimInstanceBase* GetLayerSubInstanceByGroup(FName InGroup) const;

	/**
	 * Returns whether there are any valid instances to run, currently this means whether we have
	 * have an animation instance or a post process instance available to process.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (Keywords = "AnimBlueprint"))
		bool HasValidAnimationInstance() const { return AnimScriptInstance != nullptr; }

	/**
	 * Informs any active anim instances (main instance, sub instances, post instance) that a dynamics reset is required
	 * for example if a teleport occurs.
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (Keywords = "Dynamics,Physics", UnsafeDuringActorConstruction = "true"))
		void ResetAnimInstanceDynamics(ETeleportType InTeleportType = ETeleportType::ResetPhysics);

	/**
	 * Set Morph Target with Name and Value(0-1)
	 *
	 * @param bRemoveZeroWeight : Used by editor code when it should stay in the active list with zero weight
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (UnsafeDuringActorConstruction = "true"))
		void SetMorphTarget(FName MorphTargetName, float Value, bool bRemoveZeroWeight = true);

	/**
	 * Clear all Morph Target that are set to this mesh
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		void ClearMorphTargets();

	/**
	 * Get Morph target with given name
	 */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		float GetMorphTarget(FName MorphTargetName) const;

	/**
	 * Takes a snapshot of this skeletal mesh component's pose and saves it to the specified snapshot.
	 * The snapshot is taken at the current LOD, so if for example you took the snapshot at LOD1
	 * and then used it at LOD0 any bones not in LOD1 will use the reference pose
	 */
	UFUNCTION(BlueprintCallable, Category = "Pose")
		void SnapshotPose(UPARAM(ref) FPoseSnapshot& Snapshot);

	/**
	* Sets whether or not to force tick component in order to update animation and refresh transform for this component
	* This is supported only in the editor
	*/
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh", meta = (DevelopmentOnly, UnsafeDuringActorConstruction = "true"))
		void SetUpdateAnimationInEditor(const bool NewUpdateState);

#if WITH_EDITOR
	/**
	 * return true if currently updating in editor is true
	 * this is non BP because this is only used for slave component to detect master component ticking state
	 */
	bool GetUpdateAnimationInEditor() const { return bUpdateAnimationInEditor; }
#endif 

	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		void SetAllowAnimCurveEvaluation(bool bInAllow);

	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		bool GetAllowedAnimCurveEvaluate() const { return bAllowAnimCurveEvaluation; }

	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		void AllowAnimCurveEvaluation(FName NameOfCurve, bool bAllow);

	/** By reset, it will allow all the curves to be evaluated */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		void ResetAllowedAnimCurveEvaluation();

	/** resets, and then only allow the following list to be allowed/disallowed */
	UFUNCTION(BlueprintCallable, Category = "Components|SkeletalMesh")
		void SetAllowedAnimCurvesEvaluation(const TArray<FName>& List, bool bAllow);

	const TArray<FName>& GetDisallowedAnimCurvesEvaluation() const { return DisallowedAnimCurves; }

	/** We detach the Component once we are done playing it.
	 *
	 * @param	ParticleSystemComponent that finished
	 */
	virtual void SkelMeshCompOnParticleSystemFinished(UParticleSystemComponent* PSC);

	bool InitializeAnimScriptInstance(bool bForceReinit = true);

public:
	/** Temporary array of bone indices required this frame. Filled in by UpdateSkelPose. */
	TArray<FBoneIndexType> RequiredBones;

	/** Temporary array of bone indices required to populate component space transforms */
	TArray<FBoneIndexType> FillComponentSpaceTransformsRequiredBones;

private:

	// Can't rely on time value, because those may be affected by dilation and whether or not
	// the game is paused.
	// Also can't just rely on a flag as other components (like CharacterMovementComponent) may tick
	// the pose and we can't guarantee tick order.
	UPROPERTY(Transient)
		uint32 LastPoseTickFrame;

public:

	/** Checked whether we have already ticked the pose this frame */
	bool PoseTickedThisFrame() const { return GFrameCounter == LastPoseTickFrame; }

private:
	/**
	 * Morph Target Curves. This will override AnimInstance MorphTargetCurves
	 * if same curve is found
	 **/
	TMap<FName, float>	MorphTargetCurves;

	/**
	 * Temporary storage for Curve UIDList of evaluating Animation
	 */
	TArray<uint16> CachedCurveUIDList;

public:
	const TMap<FName, float>& GetMorphTargetCurves() const { return MorphTargetCurves; }
	// 
	// Animation
	//
	virtual void InitAnim(bool bForceReinit);

	// Broadcast when the components anim instance is initialized
	UPROPERTY(BlueprintAssignable, Category = Animation)
		FOnAnimInitialized OnAnimInitialized;

	/**
		If VisibilityBasedAnimTickOption == EVisibilityBasedAnimTickOption::OnlyTickMontagesWhenNotRendered
		Should we tick Montages only?
	*/
	bool ShouldOnlyTickMontages(const float DeltaTime) const;

	/** @return whether we should tick animation (we may want to skip it due to URO) */
	bool ShouldTickAnimation() const;

	/** Tick Animation system */
	void TickAnimation(float DeltaTime, bool bNeedsValidRootMotion);

	/**
	 * Recalculates the RequiredBones array in this SkeletalMeshComponent based on current SkeletalMesh, LOD and PhysicsAsset.
	 * Is called when bRequiredBonesUpToDate = false
	 *
	 * @param	LODIndex	Index of LOD [0-(MaxLOD-1)]
	 */
	void RecalcRequiredBones(int32 LODIndex);

	/** Computes the required bones in this SkeletalMeshComponent based on current SkeletalMesh, LOD and PhysicsAsset
	  * @param	LODIndex	Index of LOD [0-(MaxLOD-1)]
	*/
	void ComputeRequiredBones(TArray<FBoneIndexType>& OutRequiredBones, TArray<FBoneIndexType>& OutFillComponentSpaceTransformsRequiredBones, int32 LODIndex, bool bIgnorePhysicsAsset) const;

	/**
	* Recalculates the AnimCurveUids array in RequiredBone of this SkeletalMeshComponent based on current required bone set
	* Is called when Skeleton->IsRequiredCurvesUpToDate() = false
	*/
	void RecalcRequiredCurves();

#pragma region UObject

public:
	virtual void Serialize(FArchive& Ar) override;
	virtual void PostLoad() override;
#if WITH_EDITOR

	FOnCustomSkeletalMeshPropertyChangedMulticaster OnSkeletalMeshPropertyChanged;

	/** Register / Unregister delegates called when the skeletal mesh property is changed */
	FDelegateHandle RegisterOnSkeletalMeshPropertyChanged(const FOnCustomSkeletalMeshPropertyChangedMulticaster::FDelegate& Delegate);
	void UnregisterOnSkeletalMeshPropertyChanged(FDelegateHandle Handle);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	/** Validates the animation asset or blueprint, making sure it is compatible with the current skeleton */
	void ValidateAnimation();

#endif // WITH_EDITOR

#pragma endregion // +++

#pragma region UActorComponent

protected:
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
public:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	virtual void BeginPlay() override;

#pragma endregion // +++

#pragma region USceneComponent

public:
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual void OnUpdateTransform(EUpdateTransformFlags UpdateTransformFlags, ETeleportType Teleport = ETeleportType::None) override;
	virtual bool UpdateOverlapsImpl(const TOverlapArrayView* PendingOverlaps = NULL, bool bDoNotifies = true, const TOverlapArrayView* OverlapsAtEndLocation = NULL) override {
		// Parent class (USkinnedMeshComponent) routes only to children, but we really do want to test our own bodies for overlaps.
		return UPrimitiveComponent::UpdateOverlapsImpl(PendingOverlaps, bDoNotifies, OverlapsAtEndLocation);
	}
	virtual const bool CanHaveStaticMobility() const override { return false; }

#pragma endregion // +++

#pragma region UPrimitiveComponent

	////////protected:
	//////////	/**
	//////////	 *  Test the collision of the supplied component at the supplied location/rotation, and determine the set of components that it overlaps
	//////////	 *  @param  OutOverlaps     Array of overlaps found between this component in specified pose and the world
	//////////	 *  @param  World			World to use for overlap test
	//////////	 *  @param  Pos             Location of the component's geometry for the test against the world
	//////////	 *  @param  Rot             Rotation of the component's geometry for the test against the world
	//////////	 *  @param  TestChannel		The 'channel' that this ray is in, used to determine which components to hit
	//////////	 *	@param	ObjectQueryParams	List of object types it's looking for. When this enters, we do object query with component shape
	//////////	 *  @return TRUE if OutOverlaps contains any blocking results
	//////////	 */
	//////////	virtual bool ComponentOverlapMultiImpl(TArray<struct FOverlapResult>& OutOverlaps, const class UWorld* InWorld, const FVector& Pos, const FQuat& Rot, ECollisionChannel TestChannel, const struct FComponentQueryParams& Params, const struct FCollisionObjectQueryParams& ObjectQueryParams = FCollisionObjectQueryParams::DefaultObjectQueryParam) const override;
	//////////	
	//////////	virtual bool ComponentOverlapComponentImpl(class UPrimitiveComponent* PrimComp, const FVector Pos, const FQuat& Quat, const FCollisionQueryParams& Params) override;
	//////////
	//////////	virtual bool MoveComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = NULL, EMoveComponentFlags MoveFlags = MOVECOMP_NoFlags, ETeleportType Teleport = ETeleportType::None) override;
	//////////
	////////public:
	//////////
	//////////	virtual class UBodySetup* GetBodySetup() override;
	//////////	virtual bool CanEditSimulatePhysics() override;
	//////////	virtual FBodyInstance* GetBodyInstance(FName BoneName = NAME_None, bool bGetWelded = true) const override;
	//////////	virtual void UpdatePhysicsToRBChannels() override;
	//////////	virtual void SetAllPhysicsAngularVelocityInRadians(FVector const& NewVel, bool bAddToCurrent = false) override;
	//////////	virtual void SetAllPhysicsPosition(FVector NewPos) override;
	//////////	virtual void SetAllPhysicsRotation(FRotator NewRot) override;
	//////////	virtual void SetAllPhysicsRotation(const FQuat& NewRot) override;
	//////////	virtual void WakeAllRigidBodies() override;
	//////////	virtual void PutAllRigidBodiesToSleep() override;
	//////////	virtual bool IsAnyRigidBodyAwake() override;
	//////////	virtual void SetEnableGravity(bool bGravityEnabled);
	//////////	virtual bool IsGravityEnabled() const override;
	//////////	virtual void OnComponentCollisionSettingsChanged() override;
	//////////	virtual void SetPhysMaterialOverride(UPhysicalMaterial* NewPhysMaterial) override;
	//////////	virtual bool GetSquaredDistanceToCollision(const FVector& Point, float& OutSquaredDistance, FVector& OutClosestPointOnCollision) const override;
	//////////
	//////////	/**
	//////////	 *	Enables or disables gravity for the given bone.
	//////////	 *	NAME_None indicates the root body will be edited.
	//////////	 *	If the bone name given is otherwise invalid, nothing happens.
	//////////	 *
	//////////	 *	@param bEnableGravity	Whether gravity should be enabled or disabled.
	//////////	 *	@param BoneName			The name of the bone to modify.
	//////////	 */
	//////////	UFUNCTION(BlueprintCallable, Category="Physics|Components")
	//////////	void SetEnableBodyGravity(bool bEnableGravity, FName BoneName);
	//////////
	//////////	/**
	//////////	 *	Checks whether or not gravity is enabled on the given bone.
	//////////	 *	NAME_None indicates the root body should be queried.
	//////////	 *	If the bone name given is otherwise invalid, false is returned.
	//////////	 *
	//////////	 *	@param BoneName	The name of the bone to check.
	//////////	 *	@return True if gravity is enabled on the bone.
	//////////	 */
	//////////	UFUNCTION(BlueprintCallable, Category="Physics|Components")
	//////////	bool IsBodyGravityEnabled(FName BoneName);
	//////////
	//////////	/**
	//////////	*	Enables or disables gravity to all bodies below the given bone.
	//////////	*   NAME_None indicates all bodies will be edited.
	//////////	*	In that case, consider using UPrimitiveComponent::EnableGravity.
	//////////	*
	//////////	*	@param bEnableGravity	Whether gravity should be enabled or disabled.
	//////////	*	@param BoneName			The name of the top most bone.
	//////////	*	@param bIncludeSelf		Whether the bone specified should be edited.
	//////////	*/
	//////////	UFUNCTION(BlueprintCallable, Category = "Physics|Components")
	//////////	void SetEnableGravityOnAllBodiesBelow(bool bEnableGravity, FName BoneName, bool bIncludeSelf=true);
	//////////
	//////////	/** 
	//////////	 *	Given a world position, find the closest point on the physics asset. Note that this is independent of collision and welding. This is based purely on animation position
	//////////	 *  @param	WorldPosition				The point we want the closest point to (i.e. for all bodies in the physics asset, find the one that has a point closest to WorldPosition)
	//////////	 *  @param	ClosestPointOnPhysicsAsset	The data associated with the closest point (position, normal, etc...)
	//////////	 *  @param	bApproximate				The closest body is found using bone transform distance instead of body distance. This approximation means the final point is the closest point on a potentially not closest body. This approximation gets worse as the size of Bodies gets bigger.
	//////////	 *  @return	true if we found a closest point
	//////////	 */
	//////////	bool GetClosestPointOnPhysicsAsset(const FVector& WorldPosition, FClosestPointOnPhysicsAsset& ClosestPointOnPhysicsAsset, bool bApproximate) const;
	//////////
	//////////	/** 
	//////////	 *	Given a world position, find the closest point on the physics asset. Note that this is independent of collision and welding. This is based purely on animation position
	//////////	 *  @param	WorldPosition				The point we want the closest point to (i.e. for all bodies in the physics asset, find the one that has a point closest to WorldPosition)
	//////////	 *  @param	ClosestPointOnPhysicsAsset	The data associated with the closest point (position, normal, etc...)
	//////////	 *  @return	true if we found a closest point
	//////////	 */
	//////////	UFUNCTION(BlueprintCallable, Category="Components|SkeletalMesh", meta=(DisplayName="GetClosestPointOnPhysicsAsset", ScriptName="GetClosestPointOnPhysicsAsset", Keywords="closest point"))
	//////////	bool K2_GetClosestPointOnPhysicsAsset(const FVector& WorldPosition, FVector& ClosestWorldPosition, FVector& Normal, FName& BoneName, float& Distance) const;
	//////////
	//////////	virtual bool LineTraceComponent( FHitResult& OutHit, const FVector Start, const FVector End, const FCollisionQueryParams& Params ) override;
	//////////	
	//////////	/** 
	//////////	 *  Trace a shape against just this component. Will trace against each body, returning as soon as any collision is found. Note that this collision may not be the closest.
	//////////	 *  @param  OutHit          	Information about hit against this component, if true is returned
	//////////	 *  @param  Start           	Start location of the trace
	//////////	 *  @param  End             	End location of the trace
	//////////	 *  @param  ShapeWorldRotation  The rotation applied to the collision shape in world space
	//////////	 *  @param  CollisionShape  	Collision Shape
	//////////	 *	@param	bTraceComplex	Whether or not to trace complex
	//////////	 *  @return true if a hit is found
	//////////	 */
	//////////	 virtual bool SweepComponent( FHitResult& OutHit, const FVector Start, const FVector End, const FQuat& ShapRotation, const FCollisionShape& CollisionShape, bool bTraceComplex=false) override;
	//////////	
	//////////	virtual bool OverlapComponent(const FVector& Pos, const FQuat& Rot, const FCollisionShape& CollisionShape) override;
	//////////	virtual void SetSimulatePhysics(bool bEnabled) override;
	//////////	virtual void AddRadialImpulse(FVector Origin, float Radius, float Strength, ERadialImpulseFalloff Falloff, bool bVelChange=false) override;
	//////////	virtual void AddRadialForce(FVector Origin, float Radius, float Strength, ERadialImpulseFalloff Falloff, bool bAccelChange=false) override;
	//////////	virtual void SetAllPhysicsLinearVelocity(FVector NewVel,bool bAddToCurrent = false) override;
	//////////	virtual void SetAllMassScale(float InMassScale = 1.f) override;
	//////////	virtual float GetMass() const override;
	//////////	virtual void SetAllUseCCD(bool InUseCCD) override;
	//////////
	//////////	/**
	//////////	*	Returns the mass (in kg) of the given bone
	//////////	*
	//////////	*	@param BoneName		Name of the body to return. 'None' indicates root body.
	//////////	*	@param bScaleMass	If true, the mass is scaled by the bone's MassScale.
	//////////	*/
	//////////	UFUNCTION(BlueprintCallable, Category = "Physics")
	//////////	float GetBoneMass(FName BoneName = NAME_None, bool bScaleMass = true) const;
	//////////
	//////////	/**
	//////////	*	Returns the center of mass of the skeletal mesh, instead of the root body's location
	//////////	*/
	//////////	UFUNCTION(BlueprintCallable, Category = "Physics")
	//////////	FVector GetSkeletalCenterOfMass() const;
	//////////
	//////////
	//////////	virtual float CalculateMass(FName BoneName = NAME_None) override;
	//////////	virtual bool DoCustomNavigableGeometryExport(FNavigableGeometryExport& GeomExport) const override;
	//////////
	//////////	/**
	//////////	*	Add a force to all rigid bodies below.
	//////////	*   This is like a 'thruster'. Good for adding a burst over some (non zero) time. Should be called every frame for the duration of the force.
	//////////	*
	//////////	*	@param	Force		 Force vector to apply. Magnitude indicates strength of force.
	//////////	*	@param	BoneName	 If a SkeletalMeshComponent, name of body to apply force to. 'None' indicates root body.
	//////////	*   @param  bAccelChange If true, Force is taken as a change in acceleration instead of a physical force (i.e. mass will have no effect).
	//////////	*   @param  bIncludeSelf If false, Force is only applied to bodies below but not given bone name.
	//////////	*/
	//////////	UFUNCTION(BlueprintCallable, Category = "Physics")
	//////////	virtual void AddForceToAllBodiesBelow(FVector Force, FName BoneName = NAME_None, bool bAccelChange = false, bool bIncludeSelf = true);
	//////////
	//////////	/**
	//////////	*	Add impulse to all single rigid bodies below. Good for one time instant burst.
	//////////	*
	//////////	*	@param	Impulse		Magnitude and direction of impulse to apply.
	//////////	*	@param	BoneName	If a SkeletalMeshComponent, name of body to apply impulse to. 'None' indicates root body.
	//////////	*	@param	bVelChange	If true, the Strength is taken as a change in velocity instead of an impulse (ie. mass will have no effect).
	//////////	*	@param bIncludeSelf If false, Force is only applied to bodies below but not given bone name.
	//////////	*/
	//////////	UFUNCTION(BlueprintCallable, Category = "Physics")
	//////////	virtual void AddImpulseToAllBodiesBelow(FVector Impulse, FName BoneName = NAME_None, bool bVelChange = false, bool bIncludeSelf = true);
	//////////
	//////////#if WITH_EDITOR
	//////////	virtual bool ComponentIsTouchingSelectionBox(const FBox& InSelBBox, const FEngineShowFlags& ShowFlags, const bool bConsiderOnlyBSP, const bool bMustEncompassEntireComponent) const override;
	//////////	virtual bool ComponentIsTouchingSelectionFrustum(const FConvexVolume& InFrustum, const FEngineShowFlags& ShowFlags, const bool bConsiderOnlyBSP, const bool bMustEncompassEntireComponent) const override;
	//////////#endif
	////////protected:
	//////////	virtual FTransform GetComponentTransformFromBodyInstance(FBodyInstance* UseBI) override;

#pragma endregion // SKIPPED (LINKED MOSTLY WITH PHYSICS)

#pragma region USkinnedMeshComponent

public:
	virtual bool UpdateLODStatus() override;
	virtual void UpdateVisualizeLODString(FString& DebugString) override;
	virtual void RefreshBoneTransforms(FActorComponentTickFunction* TickFunction = NULL) override;
	virtual void TickPose(float DeltaTime, bool bNeedsValidRootMotion) override;
	virtual void UpdateSlaveComponent() override;
	virtual bool ShouldUpdateTransform(bool bLODHasChanged) const override;
	virtual bool ShouldTickPose() const override;
	virtual bool AllocateTransformData() override;
	virtual void DeallocateTransformData() override;
	virtual void HideBone(int32 BoneIndex, EPhysBodyOp PhysBodyOption) override;
	virtual void UnHideBone(int32 BoneIndex) override;
	virtual void SetSkeletalMesh(USkeletalMesh* NewMesh, bool bReinitPose = true) override;
	void SetSkeletalMeshWithoutResettingAnimation(USkeletalMesh* NewMesh) { SetSkeletalMesh(NewMesh, false); }

	virtual void FinalizeBoneTransform() override;
	virtual void SetRefPoseOverride(const TArray<FTransform>& NewRefPoseTransforms) override;
	virtual void ClearRefPoseOverride() override;

protected:
	virtual void DispatchParallelTickPose(FActorComponentTickFunction* TickFunction) override;

private:
	virtual void AddSlavePoseComponent(USkinnedMeshComponent* SkinnedMeshComponent) override;
	virtual void RemoveSlavePoseComponent(USkinnedMeshComponent* SkinnedMeshComponent) override;
	virtual void RefreshMorphTargets() override;

#pragma endregion

#pragma region CAnimMeshComponentBase

public:

	virtual void ParallelAnimationEvaluation() override;

	virtual void CompleteParallelAnimationEvaluation(bool bDoPostAnimEvaluation) override;

	virtual bool HasValidEvaluationTask() const override { return IsValidRef(ParallelAnimationEvaluationTask); };

#pragma endregion

public:
	FOnBoneTransformsFinalized OnBoneTransformsFinalized;

	/**
	* Runs the animation evaluation for the current pose into the supplied variables
	* PerformAnimationProcessing runs evaluation based on bInDoEvaluation. PerformAnimationEvaluation
	* always runs evaluation (and exists for backward compatibility)
	*
	* @param	InSkeletalMesh			The skeletal mesh we are animating
	* @param	InAnimInstance			The anim instance we are evaluating
	* @param	bInDoEvaluation			Whether to perform evaluation (we may just want to update)
	* @param	OutSpaceBases			Component space bone transforms
	* @param	OutBoneSpaceTransforms	Local space bone transforms
	* @param	OutRootBoneTranslation	Calculated root bone translation
	* @param	OutCurves				Blended Curve
	*/
#if WITH_EDITOR
	void PerformAnimationEvaluation(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, TArray<FTransform>& OutSpaceBases, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve);
#endif
	void PerformAnimationProcessing(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, bool bInDoEvaluation, TArray<FTransform>& OutSpaceBases, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve);

	void PostAnimEvaluation(FCAnimEvaluationContext& EvaluationContext);

	/**
	 * Change whether to force mesh into ref pose (and use cheaper vertex shader)
	 *
	 * @param	bNewForceRefPose	true if it would like to force ref pose
	 */
	void SetForceRefPose(bool bNewForceRefPose);

public:
	void ClearAnimScriptInstance();

protected:
	bool NeedToSpawnAnimScriptInstance() const;

private:

	/** Evaluate Anim System **/
	void EvaluateAnimation(const USkeletalMesh* InSkeletalMesh, UCAnimInstanceBase* InAnimInstance, FVector& OutRootBoneTranslation, FBlendedHeapCurve& OutCurve, FCompactPose& OutPose) const;

	/** Queues up tasks for parallel update/evaluation, as well as the chained game thread completion task */
	void DispatchParallelEvaluationTasks(FActorComponentTickFunction* TickFunction);

	/** Performs parallel eval/update work, but on the game thread */
	void DoParallelEvaluationTasks_OnGameThread();

	/** Swaps buffers into the evaluation context before and after task dispatch */
	void SwapEvaluationContextBuffers();

	/** Duplicates cached transforms/curves and performs interpolation */
	void ParallelDuplicateAndInterpolate(FCAnimEvaluationContext& InAnimEvaluationContext);

	/**
	* Take the BoneSpaceTransforms array (translation vector, rotation quaternion and scale vector) and update the array of component-space bone transformation matrices (ComponentSpaceTransforms).
	* It will work down hierarchy multiplying the component-space transform of the parent by the relative transform of the child.
	* This code also applies any per-bone rotators etc. as part of the composition process
	*/
	void FillComponentSpaceTransforms(const USkeletalMesh* InSkeletalMesh, const TArray<FTransform>& InBoneSpaceTransforms, TArray<FTransform>& OutComponentSpaceTransforms) const;

	// Reference to our current parallel animation evaluation task (if there is one)
	FGraphEventRef				ParallelAnimationEvaluationTask;

	//Data for parallel evaluation of animation
	FCAnimEvaluationContext AnimEvaluationContext;

public:

	// Returns whether we are currently trying to run a parallel animation evaluation task
	bool IsRunningParallelEvaluation() const { return IsValidRef(ParallelAnimationEvaluationTask); }

	// Management function for if we want to do an evaluation but may already be running one
	// bBlockOnTask - if true and we are currently performing parallel eval we wait for it to finish
	// bPerformPostAnimEvaluation - if true and we are currently performing parallel eval we call PostAnimEvaluation too
	// return true if parallel task was running.
	bool HandleExistingParallelEvaluationTask(bool bBlockOnTask, bool bPerformPostAnimEvaluation);

	/** Apply animation curves to this component */
	void ApplyAnimationCurvesToComponent(const TMap<FName, float>* InMaterialParameterCurves, const TMap<FName, float>* InAnimationMorphCurves);

private:
	// Finalizes pose to OutBoneSpaceTransforms
	void FinalizePoseEvaluationResult(const USkeletalMesh* InMesh, TArray<FTransform>& OutBoneSpaceTransforms, FVector& OutRootBoneTranslation, FCompactPose& InFinalPose) const;

	void FinalizeAnimationUpdate();

	/*
	 * Update MorphTargetCurves from mesh - these are not animation curves, but SetMorphTarget and similar functions that can set to this mesh component
	 */
	void UpdateMorphTargetOverrideCurves();

	/*
	 * Reset MorphTarget Curves - Reset all morphtarget curves
	 */
	void ResetMorphTargetCurves();

public:
	/** Take extracted RootMotion and convert it from local space to world space. */
	FTransform ConvertLocalRootMotionToWorld(const FTransform& InTransform);

	/** Consume and return pending root motion from our internal anim instances (main, sub and post) */
	FRootMotionMovementParams ConsumeRootMotion();

protected:

	/** Consume and return pending root motion from our internal anim instances (main, sub and post) */
	FRootMotionMovementParams ConsumeRootMotion_Internal(float InAlpha);

private:

#if WITH_EDITOR
	/** This is required for recording animations, so save for editor only */
	/** Temporary array of curve arrays that are active on this component - keeps same buffer index as SpaceBases - Please check SkinnedMeshComponent*/
	FBlendedHeapCurve	CurvesArray[2];
public:
	/** Access Curve Array for reading */
	const FBlendedHeapCurve& GetAnimationCurves() const { return CurvesArray[CurrentReadComponentTransforms]; }

	/** Get Access to the current editable Curve Array - uses same buffer as space bases*/
	FBlendedHeapCurve& GetEditableAnimationCurves() { return CurvesArray[CurrentEditableComponentTransforms]; }
	const FBlendedHeapCurve& GetEditableAnimationCurves() const { return CurvesArray[CurrentEditableComponentTransforms]; }
#endif 

private:
	/** Mark current anim UID version to up-to-date. Called when it's recalcualted */
	void MarkRequiredCurveUpToDate();

	/* This will check if the required curves are up-to-date by checking version number with skeleton.
	 * Skeleton's curve list changes whenever newer is added or deleted.
	 * This still has to happen in editor as well as in game as
	 * There is no guarantee of Skeleton having all curves as we've seen over and over again.
	 * Cooking does not guarantee skeleton containing all names
	 */
	bool AreRequiredCurvesUpToDate() const;

public:
	void ConditionallyDispatchQueuedAnimEvents();

	// Are we currently within PostAnimEvaluation
	bool IsPostEvaluatingAnimation() const { return bPostEvaluatingAnimation; }

	float GetInterpolationAlpha() const;
};
