#pragma once

class IPathNode;

class IPathFinder
{
public:
	virtual void AStarDataReset() PURE;

	virtual BOOL InputPoint( IPathNode* p_StartNode, IPathNode* p_DestNode ) PURE;

	virtual BOOL PathFind_AStar( UINT p_LoopCount ) PURE;

	virtual BOOL MakeAStarPath() PURE;

	virtual BOOL Optimize() PURE;

};