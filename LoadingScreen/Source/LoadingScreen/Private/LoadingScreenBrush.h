#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

struct FLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject {
	FLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize) {
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) {
		if (UObject* CachedResourceObject = GetResourceObject()) {
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};