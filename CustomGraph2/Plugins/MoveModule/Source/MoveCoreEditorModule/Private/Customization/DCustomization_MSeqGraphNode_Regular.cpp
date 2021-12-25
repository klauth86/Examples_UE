#include "DCustomization_MSeqGraphNode_Regular.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Graph/MSeqGraphNode_Regular.h"
#include "MoveSequence.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "PropertyTypeCustomization_MSeqGraphNode_Regular.h"

TSharedRef<IDetailCustomization> FDCustomization_MSeqGraphNode_Regular::MakeInstance()
{
	return MakeShareable(new FDCustomization_MSeqGraphNode_Regular);
}

void FDCustomization_MSeqGraphNode_Regular::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	if (Objects.Num() != 1) return;

	if (UMSeqGraphNode_Regular* node = Cast<UMSeqGraphNode_Regular>(Objects[0]))
	{
		int32 nodeIndex = node->GetGraph()->Nodes.IndexOfByKey(node);

		if (UMoveSequence* moveSequence = node->GetGraph()->GetTypedOuter<UMoveSequence>())
		{
			if (moveSequence->GetActionsGraph().Contains(nodeIndex))
			{
				FActionsGraphNode& transitionCollection = moveSequence->GetActionsGraph()[nodeIndex];

				auto EditStructData = MakeShared<FStructOnScope>(FActionsGraphNode::StaticStruct(), (uint8*)&transitionCollection);

				//DetailBuilder. (EditStructData);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE