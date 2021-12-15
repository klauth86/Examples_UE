#include "MoveGraph.h"
#include "Move.h"

UMove* UMoveGraph::GetMove() const
{
	return CastChecked<UMove>(GetOuter());
}