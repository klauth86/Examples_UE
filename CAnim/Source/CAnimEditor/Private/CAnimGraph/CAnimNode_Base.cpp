#include "CAnimNode_Base.h"
#include "Animation/CAnimContext.h"
#include "Animation/Base/CAnimInstanceProxyBase.h"

#include "Animation/AnimNodeBase.h"

const FExposedValueHandler& FCAnimNode_Base::GetEvaluateGraphExposedInputs() {
	// Inverting control (entering via the immutable data rather than the mutable data) would allow
	// us to remove this static local. Would also allow us to remove the vtable from FAnimNode_Base.
	static const FExposedValueHandler Default;
	if (ExposedValueHandler) {
		return *ExposedValueHandler;
	}
	else {
		return Default;
	}
}

bool FCAnimNode_Base::IsLODEnabled(FCAnimInstanceProxyBase* AnimInstanceProxy) {
	const int32 NodeLODThreshold = GetLODThreshold();
	return ((NodeLODThreshold == INDEX_NONE) || (AnimInstanceProxy->GetLODLevel() <= NodeLODThreshold));
}