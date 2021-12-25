#pragma once

#include "IPropertyTypeCustomization.h"
#include "DetailLayoutBuilder.h"

class FPTCustomization_MSeqGraphNode_Regular : public IPropertyTypeCustomization
{
public:

	~FPTCustomization_MSeqGraphNode_Regular() { grid.Reset(); }

	// Makes a new instance of this detail layout class for a specific detail view requesting it
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	// IPropertyTypeCustomization interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	// End of IPropertyTypeCustomization interface
};