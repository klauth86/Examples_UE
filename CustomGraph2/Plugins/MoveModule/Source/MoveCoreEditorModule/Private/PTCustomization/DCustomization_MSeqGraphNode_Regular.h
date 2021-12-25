#pragma once

#include "IDetailCustomization.h"

class FDCustomization_MSeqGraphNode_Regular : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};