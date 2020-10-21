
#include "ObjectMacros.h"
#include "CAnimNode_Base.generated.h"

enum class ETeleportType : uint8;

class USkeleton;
class UAnimationAsset;
struct FExposedValueHandler;

// CAnim
class UCAnimInstanceBase;
struct FCAnimInstanceProxyBase;
struct FCAnimContext;

USTRUCT()
struct FCAnimNode_Base {

	GENERATED_USTRUCT_BODY()

	virtual void Initialize_AnyThread(const FCAnimContext& Context) {}

	virtual void CacheBones_AnyThread(const FCAnimContext& Context) {}

	virtual void Update_AnyThread(const FCAnimContext& Context) {}

	virtual void Evaluate_AnyThread(FCAnimContext& Output) {}

	virtual void EvaluateComponentSpace_AnyThread(FCAnimContext& Output) {}

	virtual void OverrideAsset(UAnimationAsset* NewAsset) {}

	virtual bool CanUpdateInWorkerThread() const { return true; }

	virtual bool HasPreUpdate() const { return false; }

	virtual void PreUpdate(const UCAnimInstanceBase* InAnimInstance) {}

	virtual bool NeedsDynamicReset() const { return false; }

	virtual void ResetDynamics(ETeleportType InTeleportType) {}

	virtual void PostCompile(const USkeleton* InSkeleton) {}

	virtual ~FCAnimNode_Base() {}

protected:
	bool IsLODEnabled(FCAnimInstanceProxyBase* AnimInstanceProxy);
	virtual int32 GetLODThreshold() const { return INDEX_NONE; }

	virtual void OnInitializeAnimInstance(const FCAnimInstanceProxyBase* InProxy, const UCAnimInstanceBase* InAnimInstance) {}

private:
	const FExposedValueHandler* ExposedValueHandler = nullptr;
public:
	const FExposedValueHandler& GetEvaluateGraphExposedInputs();
	void SetExposedValueHandler(const FExposedValueHandler* Handler) { ExposedValueHandler = Handler; }
};