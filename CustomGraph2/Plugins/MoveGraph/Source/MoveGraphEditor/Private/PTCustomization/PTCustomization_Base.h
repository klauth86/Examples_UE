#pragma once

#include "IPropertyTypeCustomization.h"
#include "DetailLayoutBuilder.h"

class FPTCustomization_Base : public IPropertyTypeCustomization
{
protected:

	template<typename T>
	T* GetPropertyAs() const
	{
		if (StructPropertyHandle.IsValid())
		{
			TArray<void*> RawData;
			StructPropertyHandle->AccessRawData(RawData);
			return reinterpret_cast<T*>(RawData[0]);
		}

		return nullptr;
	}

	TSharedPtr<IPropertyHandle> StructPropertyHandle;
};