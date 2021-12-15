#pragma once

#include "EdGraph/EdGraph.h"
#include "MoveGraphNode.generated.h"

class MoveNode;

UCLASS(MinimalAPI)
class UMoveGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UMoveNode* MoveNode;
};