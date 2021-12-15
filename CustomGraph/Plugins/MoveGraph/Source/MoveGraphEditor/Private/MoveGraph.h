#pragma once

#include "EdGraph/EdGraph.h"
#include "MoveGraph.generated.h"

class UMove;

UCLASS(MinimalAPI)
class UMoveGraph : public UEdGraph
{
	GENERATED_BODY()

	MOVEGRAPHEDITOR_API UMove* GetMove() const;
};