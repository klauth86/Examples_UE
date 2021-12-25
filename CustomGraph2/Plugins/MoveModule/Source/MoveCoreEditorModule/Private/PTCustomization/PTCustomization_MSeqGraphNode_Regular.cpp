#include "PTCustomization_MSeqGraphNode_Regular.h"
#include "IDetailChildrenBuilder.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Graph/MSeqGraphNode_Regular.h"
#include "MoveSequence.h"

#define LOCTEXT_NAMESPACE "PropertyTypeCustomization_MSeqGraphNode_Regular.h"

TSharedRef<IPropertyTypeCustomization> FPTCustomization_MSeqGraphNode_Regular::MakeInstance()
{
	return MakeShareable(new FPTCustomization_MSeqGraphNode_Regular);
}

void FPTCustomization_MSeqGraphNode_Regular::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	//FPTCustomization_Base::CustomizeHeader(PropertyHandle, HeaderRow, CustomizationUtils);
}

void FPTCustomization_MSeqGraphNode_Regular::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	InternalPropertyHandle = PropertyHandle;

	ChildBuilder.AddCustomRow(LOCTEXT("SearchString", "Transitions"))[
		SAssignNew(grid, SGridPanel).FillColumn(0, 1)
	];

	if (UMSeqGraphNode_Regular* node = GetPropertyAs<UMSeqGraphNode_Regular>()) {
		int32 nodeIndex = node->GetGraph()->Nodes.IndexOfByKey(node);

		if (UMoveSequence* moveSequence = node->GetGraph()->GetTypedOuter<UMoveSequence>())
		{

		}
	}
}

#undef LOCTEXT_NAMESPACE