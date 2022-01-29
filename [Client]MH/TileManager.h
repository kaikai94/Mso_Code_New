// TileManager.h: interface for the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
#define AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tile.h"
#include "IPathFinder.h"
#include <vector>
#include <map>
#include <deque>
#define fTILE_WIDTH 50
#define fTILE_HEIGHT 50
#define TILECOLLISON_DETAIL		50.f


enum ePathFindLayer
{
	ePathFindLayer_1x,
	ePathFindLayer_3x,

	eDefaultTileRatio = 50,
};

enum eH2DLineSide
{
	eH2DLineSide_Up_Left,		
	eH2DLineSide_Down_Right,	
	eH2DLineSide_Equal,			
};

class H2DLine
{
public:
	H2DLine( VECTOR2 p_P1, VECTOR2 p_P2 );

	eH2DLineSide CheckSide( VECTOR2 p_Point );

	float GetA() const { return m_a; }
	BOOL GetFlag_x_equal_b() const { return m_Flag_x_equal_b; }

private:
	float	m_a;			
	float	m_b;			
	BOOL	m_Flag_x_equal_b;	

};


BOOL CollisionCheck_2D_Segment_Line(VECTOR2 p_SegA, VECTOR2 p_SegB, VECTOR2 p_LineA, VECTOR2 p_LineB);
BOOL CollisionCheck_2D_Segment_Line(VECTOR2 p_SegA, VECTOR2 p_SegB, H2DLine& p_Line);

class CTileManager : public IPathFinder
{
protected:
	DWORD m_dwTileWidth;
	DWORD m_dwTileHeight;
	
	CTile* m_pTile;
	
	CTile* GetTile(DWORD dwX, DWORD dwZ);
	CTile* GetTile(float fx, float fz);

public:
	CTileManager();
	~CTileManager();

	DWORD GetTileWidth() { return m_dwTileWidth; }
	BOOL IsInTile(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionTile(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3 * Target, MAPTYPE MapNum,CObject* pObject);
	BOOL NonCollisionLine(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* pRtNonCollisonPos, MAPTYPE MapNum, CObject* pObj);	

	BOOL LoadTileInfo(char* TileFile);
	BOOL CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj);		

	void AddTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile);
	void RemoveTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile);

	void			SetPathFindMode( ePathFindLayer p_PathFindMode );
	ePathFindLayer	GetPathFindMode();

	BOOL CollisionCheck_OneLine_New(DWORD p_StartX, DWORD p_StartY, DWORD p_EndX, DWORD p_EndY);
	BOOL CollisionCheck_OneLine_New(VECTOR3* p_pStart,VECTOR3* p_pEnd);

	

private:
	DWORD	m_dwWidth1x;	
	DWORD	m_dwHeight1x;

	DWORD	m_dwWidth3x;	
	DWORD	m_dwHeight3x;
	
	DWORD	m_dwWidth4x;	
	DWORD	m_dwHeight4x;

	float	m_NodeRatio;


public:

	virtual void AStarDataReset();

	virtual BOOL InputPoint( IPathNode* p_StartNode, IPathNode* p_DestNode );
			BOOL InputPoint( const VECTOR3& p_StartPosition, const VECTOR3& p_DestPosition );

	virtual BOOL PathFind_AStar( UINT p_LoopCount );

	virtual BOOL MakeAStarPath();

	virtual BOOL Optimize();
		private:
			BOOL Optimize(std::deque<IPathNode*>& p_deq_AStarPath, std::deque<IPathNode*>& p_deq_AStarPath_Optimize);


public:
	DWORD GetWidth1x() const { return m_dwWidth1x; }
	DWORD GetHeight1x() const { return m_dwHeight1x; }

	IPathNode*	GetMovableNode(DWORD p_PosX, DWORD p_PosY);
	IPathNode*	GetMovableNode(float p_PosX, float p_PosY);
	CTile*	GetNode(DWORD p_PosX, DWORD p_PosY);
	CTile*	GetNode(float p_PosX, float p_PosY);
	
	CTile*	GetNode1x(DWORD p_PosX, DWORD p_PosY);
	CTile*	GetNode1x(float p_PosX, float p_PosY);

	std::vector<VECTOR3> GetPathResult(float p_Ratio);
	std::deque<VECTOR3> GetPathResult_Optimize(float p_Ratio);
	std::deque<VECTOR3> GetPathResult_Optimize_Debug(float p_Ratio);


	BOOL FindClosestTile(const VECTOR3& p_Position, IPathNode*& p_ClosestNode);

	BOOL FindClosest1xTile(VECTOR3& p_SafePosition, DWORD p_NowPosX, DWORD p_NowPosY);

	float GetNodeRatio() const { return m_NodeRatio; }

	void UpdateNodeInfo( VECTOR3* pPos );


private:
	CTile*								m_pAllNode1x;		
	CTile*								m_pAllNode3x;	

	std::multimap<float, IPathNode*>	m_map_OpenList;
	std::vector<IPathNode*>				m_vec_ClosedList;
	IPathNode*							m_StartNode;
	IPathNode*							m_DestNode;
	IPathNode*							m_NowNode;

	std::deque<IPathNode*>				m_AStarPath;
	std::deque<IPathNode*>				m_AStarPath_Optimize;

	std::vector<VECTOR3>				m_PathResult;
	std::deque<VECTOR3>					m_PathResult_Optimize;
	std::deque<VECTOR3>					m_PathResult_Optimize_Debug;
};

#endif // !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
