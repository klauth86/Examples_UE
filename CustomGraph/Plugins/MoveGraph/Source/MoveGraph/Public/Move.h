#pragma once

#include "UObject/Object.h"
#include "Move.generated.h"

class UEdGraph;

UCLASS()
class MOVEGRAPH_API UMoveNode : public UObject
{
	GENERATED_BODY()

public:


};

UCLASS()
class MOVEGRAPH_API UMove : public UObject
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA
	UPROPERTY()
		TArray<UMoveNode*> AllNodes;

	UPROPERTY()
		UEdGraph* MoveGraph;
#endif
};