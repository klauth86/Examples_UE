#pragma once

#include "CoreMinimal.h"
#include "Engine/Blueprint.h"
#include "Interfaces/Interface_PreviewMeshProvider.h"
#include "CAnimBlueprintBase.generated.h"

/**
 *
 */
 /**
  * An CAnim Blueprint is custom implementation of Anim Blueprint.
  */
UCLASS(BlueprintType)
class CANIM_API UCAnimBlueprintBase : public UBlueprint, public IInterface_PreviewMeshProvider {
	GENERATED_BODY()

public:

	virtual void SetPreviewMesh(USkeletalMesh* PreviewMesh, bool bMarkAsDirty = true) {};

	virtual USkeletalMesh* GetPreviewMesh() const { return NULL; };
};
