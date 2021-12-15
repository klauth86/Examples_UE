#include "Move.h"
#include "EdGraph/EdGraphSchema.h"

#if WITH_EDITOR
UEdGraph* UMove::GetGraph()
{
	return MoveGraph;
}

void UMove::CreateGraph()
{
	if (MoveGraph == nullptr)
	{
		MoveGraph = FBlueprintEditorUtils::CreateNewGraph(this, NAME_None, UMoveGraph::StaticClass(), UMoveGraphSchema::StaticClass())
		MoveGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = MoveGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*SoundCueGraph);
	}
}

void UMove::ClearGraph()
{
	if (SoundCueGraph)
	{
		SoundCueGraph->Nodes.Empty();
		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = SoundCueGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*SoundCueGraph);
	}
}

void UMove::SetupSoundNode(USoundNode* InSoundNode, bool bSelectNewNode/* = true*/)
{
	// Create the graph node
	check(InSoundNode->GraphNode == NULL);

	UMove::GetSoundCueAudioEditor()->SetupSoundNode(SoundCueGraph, InSoundNode, bSelectNewNode);
}

void UMove::LinkGraphNodesFromSoundNodes()
{
	UMove::GetSoundCueAudioEditor()->LinkGraphNodesFromSoundNodes(this);
	CacheAggregateValues();
}

void UMove::CompileSoundNodesFromGraphNodes()
{
	UMove::GetSoundCueAudioEditor()->CompileSoundNodesFromGraphNodes(this);
}

void UMove::SetSoundCueAudioEditor(TSharedPtr<ISoundCueAudioEditor> InSoundCueAudioEditor)
{
	check(!SoundCueAudioEditor.IsValid());
	SoundCueAudioEditor = InSoundCueAudioEditor;
}

void UMove::ResetGraph()
{
	for (const USoundNode* Node : AllNodes)
	{
		SoundCueGraph->RemoveNode(Node->GraphNode);
	}

	AllNodes.Reset();
	FirstNode = nullptr;
}

/** Gets the sound cue graph editor implementation. */
TSharedPtr<ISoundCueAudioEditor> UMove::GetSoundCueAudioEditor()
{
	return SoundCueAudioEditor;
}
#endif // WITH_EDITOR