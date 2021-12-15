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

#if WITH_EDITOR
	/** Create the basic sound graph */
	void CreateGraph();

	/** Clears all nodes from the graph (for old editor's buffer soundcue) */
	void ClearGraph();

	/** Set up EdGraph parts of a SoundNode */
	void SetupSoundNode(UMoveNode* InSoundNode, bool bSelectNewNode = true);

	/** Use the SoundCue's children to link EdGraph Nodes together */
	void LinkGraphNodesFromSoundNodes();

	/** Use the EdGraph representation to compile the SoundCue */
	void CompileSoundNodesFromGraphNodes();

	/** Get the EdGraph of SoundNodes */
	UEdGraph* GetGraph();

	/** Resets all graph data and nodes */
	void ResetGraph();

private:

	/** Recursively sets the branch culling exclusion on random nodes in this sound cue. */
	void RecursivelySetExcludeBranchCulling(UMoveNode* CurrentNode);

#endif // WITH_EDITOR
};