// TileManager.cpp: implementation of the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileManager.h"
#include "Object.h"



H2DLine::H2DLine( VECTOR2 p_P1, VECTOR2 p_P2 )
{
	if ( (p_P1.x - p_P2.x) == 0 )
	{
		m_Flag_x_equal_b = TRUE;
		m_a = 0;
		m_b = p_P1.x;
	}
	else
	{
		m_Flag_x_equal_b = FALSE;
		m_a = (p_P1.y - p_P2.y) / (p_P1.x - p_P2.x);
		m_b = p_P1.y - (m_a * p_P1.x);
	}
}

eH2DLineSide H2DLine::CheckSide( VECTOR2 p_Point )
{
	if ( m_Flag_x_equal_b == TRUE )
	{
		if ( p_Point.x < m_b )
		{
			return eH2DLineSide_Up_Left;
		}
		else if (p_Point.x == m_b)
		{
			return eH2DLineSide_Equal;
		}
		else
		{
			return eH2DLineSide_Down_Right;
		}
	}
	else
	{
		float t_Y = (m_a * p_Point.x) + m_b;

		if ( t_Y < p_Point.y )
		{
			return eH2DLineSide_Up_Left;
		}
		else if (t_Y == p_Point.y)
		{
			return eH2DLineSide_Equal;
		}
		else
		{
			return eH2DLineSide_Down_Right;
		}
	}
}

BOOL CollisionCheck_2D_Segment_Line( VECTOR2 p_SegA, VECTOR2 p_SegB, VECTOR2 p_LineA, VECTOR2 p_LineB )
{
	H2DLine l_Line = H2DLine(p_LineA, p_LineB);

	return CollisionCheck_2D_Segment_Line(p_SegA, p_SegB, l_Line);
}

BOOL CollisionCheck_2D_Segment_Line( VECTOR2 p_SegA, VECTOR2 p_SegB, H2DLine& p_Line )
{
	eH2DLineSide l_SideA = p_Line.CheckSide(p_SegA);
	eH2DLineSide l_SideB = p_Line.CheckSide(p_SegB);

	if (l_SideA == eH2DLineSide_Equal || l_SideB == eH2DLineSide_Equal)
	{
		return TRUE;
	}
	else if (l_SideA == l_SideB)
	{
		return FALSE;
	}

	return TRUE;
}

CTileManager::CTileManager() :
m_pAllNode1x(0),
m_pAllNode3x(0)
{
	m_pTile = NULL;
}

CTileManager::~CTileManager()
{
	SAFE_DELETE_ARRAY(m_pAllNode1x);
	SAFE_DELETE_ARRAY(m_pAllNode3x);
	std::vector<VECTOR3>().swap(m_PathResult);
}

CTile* CTileManager::GetTile(DWORD dwX, DWORD dwZ)
{
	if(!(dwX < m_dwTileWidth && dwZ < m_dwTileHeight))
		return NULL;

	return (m_pTile + (m_dwTileHeight * dwZ + dwX));
}

CTile* CTileManager::GetTile(float fx, float fz)
{
	if(!(fx >= 0 && fz >= 0))
		return NULL;	

	DWORD z = DWORD(fz / fTILE_WIDTH);
	DWORD x = DWORD(fx / fTILE_HEIGHT);

	return GetTile(x, z);
}

BOOL CTileManager::LoadTileInfo(char* TileFile)
{
	BOOL bRet = FALSE;
	DWORD dwRead = 0;
	HANDLE hFile = CreateFile(TileFile,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	bRet = ReadFile(hFile, &m_dwTileWidth, sizeof(m_dwTileWidth), &dwRead, NULL);
	if (!bRet) return bRet;
	bRet = ReadFile(hFile, &m_dwTileHeight, sizeof(m_dwTileHeight), &dwRead, NULL);
	if (!bRet) return bRet;
	m_pTile = new CTile[ m_dwTileWidth * m_dwTileHeight ];
	CTile* l_Node = NULL;
	TILE_ATTR l_Attrib;

	for (DWORD j=0; j<m_dwTileHeight; j++)
	{
		for (DWORD i=0; i<m_dwTileWidth; i++)
		{
			bRet = ReadFile(hFile, &l_Attrib, sizeof(TILE_ATTR), &dwRead, NULL);
			if (!bRet) return bRet;
			l_Node = m_pTile + (m_dwTileWidth * j) + i;

			if (l_Node)
			{
				l_Node->SetPosX(i);
				l_Node->SetPosY(j);
				l_Node->SetTileAttrib(l_Attrib);
			}
		}
	}

	CloseHandle(hFile);


	m_dwWidth1x = m_dwTileWidth;
	m_dwHeight1x = m_dwTileHeight;
	m_pAllNode1x = m_pTile;
	
	m_dwWidth3x = m_dwTileWidth/3;
	m_dwHeight3x = m_dwTileHeight/3;

	m_pAllNode3x = new CTile[ m_dwWidth3x * m_dwHeight3x ];

	for (DWORD t_Y=0; t_Y < m_dwHeight3x; t_Y++ )
	{
		for (DWORD t_X=0; t_X < m_dwWidth3x; t_X++ )
		{
			CTile* t_Tile_1 = GetNode(t_X*3, t_Y*3);
			CTile* t_Tile_2 = GetNode(t_X*3+1, t_Y*3);
			CTile* t_Tile_3 = GetNode(t_X*3+2, t_Y*3);
			CTile* t_Tile_4 = GetNode(t_X*3, t_Y*3+1);
			CTile* t_Tile_5 = GetNode(t_X*3+1, t_Y*3+1);
			CTile* t_Tile_6 = GetNode(t_X*3+2, t_Y*3+1);
			CTile* t_Tile_7 = GetNode(t_X*3, t_Y*3+2);
			CTile* t_Tile_8 = GetNode(t_X*3+1, t_Y*3+2);
			CTile* t_Tile_9 = GetNode(t_X*3+2, t_Y*3+2);

			if ( t_Tile_1 && t_Tile_2 && t_Tile_3 && t_Tile_4 &&
				t_Tile_5 && t_Tile_6 && t_Tile_7 && t_Tile_8 && t_Tile_9 )
			{
				if ( !t_Tile_1->IsCollisonTile() &&
					!t_Tile_2->IsCollisonTile() &&
					!t_Tile_3->IsCollisonTile() &&
					!t_Tile_4->IsCollisonTile() &&
					!t_Tile_5->IsCollisonTile() &&
					!t_Tile_6->IsCollisonTile() &&
					!t_Tile_7->IsCollisonTile() &&
					!t_Tile_8->IsCollisonTile() &&
					!t_Tile_9->IsCollisonTile() )
				{
					TILE_ATTR l_Attrib;
					l_Attrib.uAttr = 0;
					l_Attrib.uSkillObjectBlock = 0;
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetTileAttrib(l_Attrib);
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosX(t_X);
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosY(t_Y);

				}
				else
				{
					TILE_ATTR l_Attrib;
					l_Attrib.uAttr = 1;
					l_Attrib.uSkillObjectBlock = 0;
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetTileAttrib(l_Attrib);
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosX(t_X);
					m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosY(t_Y);
				}

			}
		}
	}

	return TRUE;
}


BOOL CTileManager::IsInTile(int cellX, int cellY, MAPTYPE MapNum,CObject* pObject)
{
	if( cellX < 0 || cellY < 0 || cellX >= (int)m_dwTileWidth || cellY >= (int)m_dwTileHeight )
			return FALSE;
		return TRUE;
}
BOOL CTileManager::CollisionTile( int x, int y, MAPTYPE MapNum,CObject* pObject)
{ 
	CTile *pTile = GetTile((DWORD)x, (DWORD)y);
	if(pTile)
		return pTile->IsCollisonTile();
	else
	{
		char temp[256];
		sprintf(temp,"%d %d %s",x,y,pObject->GetObjectName());
		ASSERTMSG(0,temp);
		return TRUE;
	}
}	
BOOL CTileManager::CollisionLine(VECTOR3* pSrc,VECTOR3* pDest, VECTOR3 * pTarget, MAPTYPE MapNum,CObject* pObject)
{
	BOOL bEscape = FALSE;

	int x1 = int(pSrc->x / TILECOLLISON_DETAIL);
	int y1 = int(pSrc->z / TILECOLLISON_DETAIL);
	int x2 = int(pDest->x / TILECOLLISON_DETAIL);
	int y2 = int(pDest->z / TILECOLLISON_DETAIL);

	int dx = x2 - x1;
	int dy = y2 - y1;
	
	int absDX = abs(dx);
	int absDY = abs(dy);
	int MaxDelta = max(absDX, absDY);
	
	int CellX = x1;
	int CellY = y1;
	int x = 0;
	int y = 0;

	int signDX = sign(dx);
	int signDY = sign(dy);
	
	int PrevCellX = 0;
	int PrevCellY = 0;

	for( int i = 0 ; i <= MaxDelta ; ++i )
	{
		PrevCellX = CellX;
		PrevCellY = CellY;

		x += absDX;  
		y += absDY;
		
		if( x > MaxDelta)  
		{  
			x -= MaxDelta;
			CellX += signDX;
		}
		
		if( y > MaxDelta ) 
		{  
			y -= MaxDelta;  
			CellY += signDY;  
		}
		CTile *pTile = GetTile((DWORD)CellX, (DWORD)CellY);
		if(pTile == NULL)
		{
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			
			char temp[256];
			sprintf(temp,"%d %d %s",CellX,CellY,pObject->GetObjectName());
			ASSERTMSG(0,temp);
			
			return TRUE;
		}
		if(pTile->IsCollisonTile())
		{
			if( i == 0 )
			{
				bEscape = TRUE;
				continue;
			}
			if( bEscape && i < 2)
			{
				continue;
			}
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			return TRUE;
		}
	}

	if(pTarget)
	{
		pTarget->x = x2*TILECOLLISON_DETAIL;
		pTarget->y = 0;
		pTarget->z = y2*TILECOLLISON_DETAIL;
	}
	return FALSE;
}

BOOL CTileManager::NonCollisionLine( VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* pRtNonCollisonPos, MAPTYPE MapNum, CObject* pObj )
{
	int x1 = int(pStart->x / TILECOLLISON_DETAIL);
	int y1 = int(pStart->z / TILECOLLISON_DETAIL);
	int x2 = int(pEnd->x / TILECOLLISON_DETAIL);
	int y2 = int(pEnd->z / TILECOLLISON_DETAIL);

	int dx = x1 - x2;
	int dy = y1 - y2;
	
	int absDX = abs(dx);
	int absDY = abs(dy);
	int MaxDelta = max(absDX, absDY);
	
	int CellX = x1;
	int CellY = y1;

	CTile *pCurTile = GetTile((DWORD)CellX, (DWORD)CellY);
	if(pCurTile == NULL)
	{
		if(pRtNonCollisonPos)
		{
			pRtNonCollisonPos->x = CellX * TILECOLLISON_DETAIL;
			pRtNonCollisonPos->y = 0;
			pRtNonCollisonPos->z = CellY * TILECOLLISON_DETAIL;
		}

		char temp[256] = {0,};
		sprintf(temp, "%d %d %s", CellX, CellY, pObj->GetObjectName() );
		ASSERTMSG(0,temp);

		return FALSE;
	}
	if(FALSE == pCurTile->IsCollisonTile())	
	{
		if(pRtNonCollisonPos)
		{
			pRtNonCollisonPos->x = CellX * TILECOLLISON_DETAIL;
			pRtNonCollisonPos->y = 0;
			pRtNonCollisonPos->z = CellY * TILECOLLISON_DETAIL;
		}

		return FALSE;
	}

	int x = 0;
	int y = 0;

	int signDX = -sign(dx);
	int signDY = -sign(dy);
	
	int PrevCellX = 0;
	int PrevCellY = 0;

#define THROUGH_PERMIT_TILE_INDEX 2

	for( int i = 0 ; i <= MaxDelta ; ++i )
	{
		if( i > THROUGH_PERMIT_TILE_INDEX )
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = CellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = CellY*TILECOLLISON_DETAIL;
			}

			return FALSE;
		}

		PrevCellX = CellX;
		PrevCellY = CellY;

		x += absDX;  
		y += absDY;
		
		if( x > MaxDelta)  
		{  
			x -= MaxDelta;
			CellX += signDX;
		}
		
		if( y > MaxDelta ) 
		{  
			y -= MaxDelta;  
			CellY += signDY;  
		}
		CTile *pTile = GetTile((DWORD)CellX, (DWORD)CellY);
		if(pTile == NULL)
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = PrevCellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			
			char temp[256];
			sprintf(temp,"%d %d %s",CellX,CellY,pObj->GetObjectName());
			ASSERTMSG(0,temp);
			
			return FALSE;
		}
		if( FALSE == pTile->IsCollisonTile() )
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = CellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = CellY*TILECOLLISON_DETAIL;
			}
			return TRUE;
		}
	}

	if(pRtNonCollisonPos)
	{
		pRtNonCollisonPos->x = x1*TILECOLLISON_DETAIL;
		pRtNonCollisonPos->y = 0;
		pRtNonCollisonPos->z = y1*TILECOLLISON_DETAIL;
	}
	return FALSE;
}


BOOL CTileManager::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj)
{	
	if( pStart->x >= 51200 || pStart->x < 0 ||
		pStart->z >= 51200 || pStart->z < 0 )
	{
		ASSERTMSG(0,"Â·¾¶²éÕÒÊ§°Ü");
		pRtCollisonPos->x = 25000;
		pRtCollisonPos->z = 25000;
		return TRUE;
	}

	int x0 = int(pStart->x / fTILE_WIDTH);
	int y0 = int(pStart->z / fTILE_HEIGHT);
	int x1 = int(pEnd->x / fTILE_WIDTH);
	int y1 = int(pEnd->z / fTILE_HEIGHT);
	
	int dx = x1-x0,dy = y1-y0;
	int sx,sy;
	if(dx >= 0)
	{
		sx = 1;
	}
	else
	{
		sx = -1;
		dx *= -1;
	}
	if(dy >= 0)
	{
		sy = 1;
	}
	else
	{
		sy = -1;
		dy *= -1;
	}
	
	int ax = 2*dx,ay = 2*dy;

	int x = x0, y = y0;
	int lastx = x, lasty = y;
	
	BOOL bFirst = TRUE;
	if(dx == 0 && dy == 0)
	{
		return FALSE;
	}
	
	if(dx >= dy)
	{
		for(int desc = ay - dx; ;x += sx,desc += ay)
		{
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}

			CTile *pTile = GetTile((DWORD)x, (DWORD)y);
			
#ifdef _DEBUG
			if(pTile == NULL)
			{
				LOGFILE("TILENOTFOUND\t%s",OBJECTLOG(pObj));
				LOGFILE("StartPos\t%4.2f\t%4.2f\t\tEndPos\t%4.2f\t%4.2f",
					pStart->x,pStart->z,pEnd->x,pEnd->z);
				ASSERTMSG(0,"Â·¾¶²éÕÒÊ§°Ü");
				break;
			}			
#endif
			if(pTile == NULL || pTile->IsCollisonTile())
			{
				pRtCollisonPos->x = (float)(lastx*fTILE_WIDTH);
				pRtCollisonPos->y = 0;
				pRtCollisonPos->z = (float)(lasty*fTILE_HEIGHT);
				return TRUE;
			}

			if(x == x1)
				break;

			lastx = x;
			lasty = y;

			if(desc > 0)
			{
				y += sy;
				desc -= ax;
			}
		}
	}
	else
	{
		for(int desc = ax - dy; ;y += sy,desc += ax)
		{
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}

			CTile *pTile = GetTile((DWORD)x, (DWORD)y);
			
#ifdef _DEBUG
			if(pTile == NULL)
			{
				LOGFILE("TILENOTFOUND\t%s",OBJECTLOG(pObj));
				LOGFILE("StartPos\t%4.2f\t%4.2f\t\tEndPos\t%4.2f\t%4.2f",
					pStart->x,pStart->z,pEnd->x,pEnd->z);
				ASSERTMSG(0,"Â·¾¶²éÕÒÊ§°Ü");
				break;
			}			
#endif
			if(pTile == NULL || pTile->IsCollisonTile())
			{
				pRtCollisonPos->x = (float)(lastx*fTILE_WIDTH);
				pRtCollisonPos->y = 0;
				pRtCollisonPos->z = (float)(lasty*fTILE_HEIGHT);
				return TRUE;
			}

			if(y == y1)
				break;
			
			lastx = x;
			lasty = y;

			if(desc > 0)
			{
				x += sx;
				desc -= ay;
			}
		}
	}
	return FALSE;
}

void CTileManager::AddTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->AddTileAttrib(areatile);

	UpdateNodeInfo( pPos );
}

void CTileManager::RemoveTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->RemoveTileAttrib(areatile);

	UpdateNodeInfo( pPos );
}


void CTileManager::SetPathFindMode( ePathFindLayer p_PathFindMode )
{
	switch(p_PathFindMode)
	{
	case ePathFindLayer_1x:
		{
			m_dwTileWidth = m_dwWidth1x;
			m_dwTileHeight = m_dwHeight1x;
			m_pTile = m_pAllNode1x;
			m_NodeRatio = 50;
			break;
		}
	case ePathFindLayer_3x:
		{
			m_dwTileWidth = m_dwWidth3x;
			m_dwTileHeight = m_dwHeight3x;
			m_pTile = m_pAllNode3x;
			m_NodeRatio = 150;
			break;
		}
	default:
		{
			break;
		}
	}

}

ePathFindLayer CTileManager::GetPathFindMode()
{
	if (m_pTile == m_pAllNode1x) return ePathFindLayer_1x;
	else if (m_pTile == m_pAllNode3x) return ePathFindLayer_3x;

	return ePathFindLayer_1x;
}


void CTileManager::AStarDataReset()
{
	for(std::multimap<float, IPathNode*>::iterator it_NowNode = m_map_OpenList.begin();
		it_NowNode != m_map_OpenList.end();
		++it_NowNode)
	{
		IPathNode* t_Node = it_NowNode->second;
		t_Node->Reset();
	}

	for (UINT i = 0; i<m_vec_ClosedList.size(); i++)
	{
		m_vec_ClosedList[i]->Reset();
	}
	for (UINT i = 0; i<m_AStarPath.size(); i++)
	{
		m_AStarPath[i]->Reset();
	}
	for (UINT i = 0; i<m_AStarPath_Optimize.size(); i++)
	{
		m_AStarPath_Optimize[i]->Reset();
	}

	m_map_OpenList.clear();
	m_vec_ClosedList.clear();
	m_AStarPath.clear();
	m_AStarPath_Optimize.clear();

	m_StartNode = NULL;
	m_DestNode = NULL;
	m_NowNode = NULL;

	m_PathResult.clear();
	m_PathResult_Optimize.clear();
	m_PathResult_Optimize_Debug.clear();
}

BOOL CTileManager::InputPoint( IPathNode* p_StartNode, IPathNode* p_DestNode )
{
	if ( p_StartNode == NULL || p_DestNode == NULL )
	{
		return FALSE;
	}

	if ( p_StartNode == p_DestNode )
	{
		return FALSE;
	}

	m_StartNode = p_StartNode;
	m_DestNode = p_DestNode;

	m_NowNode = m_StartNode;
	m_NowNode->Calc_Cost(m_DestNode);
	m_map_OpenList.insert( std::pair<float, IPathNode*>( m_NowNode->GetFCost(), m_NowNode ) );
	
	return TRUE;
}

BOOL CTileManager::InputPoint( const VECTOR3& p_StartPosition, const VECTOR3& p_DestPosition )
{
	return InputPoint( GetMovableNode(p_StartPosition.x, p_StartPosition.z),
					   GetMovableNode(p_DestPosition.x, p_DestPosition.z) );
}

BOOL CTileManager::PathFind_AStar( UINT p_LoopCount )
{
	while(false == m_map_OpenList.empty())
	{
		if(0 == --p_LoopCount)
		{
			break;
		}

		std::multimap<float, IPathNode*>::iterator it_NowNode = m_map_OpenList.begin();
		m_NowNode = it_NowNode->second;
		m_map_OpenList.erase( it_NowNode );

		m_vec_ClosedList.push_back( m_NowNode );
		m_NowNode->SetAStarStatus( eClosed );

		IPathNode* t_ProximityNode = NULL;

		for (int i=0; i<4; i++)
		{
			if ( i == 0 ) t_ProximityNode = GetMovableNode( m_NowNode->GetPosX(), m_NowNode->GetPosY()-1 );	
			if ( i == 1 ) t_ProximityNode = GetMovableNode( m_NowNode->GetPosX(), m_NowNode->GetPosY()+1 );	
			if ( i == 2 ) t_ProximityNode = GetMovableNode( m_NowNode->GetPosX()-1, m_NowNode->GetPosY() );
			if ( i == 3 ) t_ProximityNode = GetMovableNode( m_NowNode->GetPosX()+1, m_NowNode->GetPosY() );	

			if (t_ProximityNode)
			{
				if ( t_ProximityNode->GetAStarStatus() == eUnidentified )
				{
					t_ProximityNode->Astar_MakeOpened( m_NowNode, m_DestNode );
					m_map_OpenList.insert( std::pair<float, IPathNode*>(t_ProximityNode->GetFCost(), t_ProximityNode) );
				}
				else if ( t_ProximityNode->GetAStarStatus() == eOpened )
				{
					t_ProximityNode->Astar_CompareParentCost( m_NowNode, i );
				}

				if (t_ProximityNode == m_DestNode) 
				{
					m_vec_ClosedList.push_back( m_DestNode );
					return TRUE;
				}
			}
		}

		if(m_map_OpenList.empty())
		{
			return FALSE;
		}


	}	// while
	return FALSE;
}

BOOL CTileManager::MakeAStarPath()
{
	IPathNode* t_NowNode = m_DestNode;
	m_AStarPath.push_front( t_NowNode );

	while(TRUE)
	{
		if ( t_NowNode->GetAstarParentNode() )
		{
			t_NowNode->GetAstarParentNode()->SetAstarNextNode( t_NowNode );
			t_NowNode = t_NowNode->GetAstarParentNode();
			m_AStarPath.push_front( t_NowNode );

			if ( t_NowNode == m_StartNode ) break;
		}
		else
		{
			break;
		}
	}
	return TRUE;
}

BOOL CTileManager::Optimize()
{
	std::deque<IPathNode*> t_deq_Original;
	std::deque<IPathNode*> t_deq_Optimize;
	std::deque<IPathNode*> t_deq_Optimize2;

	//HTR_S(eRT_Optimize1);
	Optimize(m_AStarPath, t_deq_Optimize);
	//HTR_E(eRT_Optimize1);

	//HTR_S(eRT_Optimize2);
	Optimize(t_deq_Optimize, t_deq_Optimize2);
	//HTR_E(eRT_Optimize2);

	m_AStarPath_Optimize = t_deq_Optimize2;
	return TRUE;
}

BOOL CTileManager::Optimize(std::deque<IPathNode*>& p_deq_AStarPath, std::deque<IPathNode*>& p_deq_AStarPath_Optimize)
{
	DWORD t_TempStartX, t_TempStartY;				
	DWORD t_NowX, t_NowY;							
	IPathNode* t_StartNode =p_deq_AStarPath.front();
	IPathNode* t_DestNode = p_deq_AStarPath.back();
	IPathNode* t_LastNode = t_StartNode;			
	IPathNode* t_NowNode = NULL;					


	t_TempStartX = t_StartNode->GetPosX();
	t_TempStartY = t_StartNode->GetPosY();
	p_deq_AStarPath_Optimize.push_back(t_StartNode);


	for (UINT i=1; i<p_deq_AStarPath.size(); i++)
	{
		t_NowNode = p_deq_AStarPath[i];
		t_NowX = t_NowNode->GetPosX();
		t_NowY = t_NowNode->GetPosY();

		if ( CollisionCheck_OneLine_New(t_TempStartX, t_TempStartY, t_NowX, t_NowY) == TRUE )
		{
			t_LastNode = t_NowNode;
		}
		else
		{
			if ( t_LastNode->GetPosX() == t_TempStartX && t_LastNode->GetPosY() == t_TempStartY )
			{
				p_deq_AStarPath_Optimize.push_back(t_NowNode);
				t_TempStartX = t_NowX;
				t_TempStartY = t_NowY;
				t_LastNode = t_NowNode;
			}
			else
			{
				p_deq_AStarPath_Optimize.push_back(t_LastNode);

				t_TempStartX = t_LastNode->GetPosX();
				t_TempStartY = t_LastNode->GetPosY();

				i--;
			}

		}

	}

	p_deq_AStarPath_Optimize.push_back(t_DestNode);
	return TRUE;
}

IPathNode* CTileManager::GetMovableNode( DWORD p_PosX, DWORD p_PosY )
{
	if ( (p_PosX >= 0) && (p_PosX < m_dwTileWidth) &&
		(p_PosY >= 0) && (p_PosY < m_dwTileHeight)	)
	{
		CTile* l_Node  = m_pTile + (p_PosY * m_dwTileWidth) + p_PosX;
		if (l_Node)
		{
			if(FALSE == l_Node->IsCollisonTile())
			{
				return l_Node;
			}
		}
	}

	return NULL;
}

IPathNode* CTileManager::GetMovableNode( float p_PosX, float p_PosY )
{
	if (m_NodeRatio == 0)
	{
		return NULL;
	}

	return GetMovableNode((DWORD)(p_PosX/m_NodeRatio), (DWORD)(p_PosY/m_NodeRatio) );
}
CTile* CTileManager::GetNode( DWORD p_PosX, DWORD p_PosY )
{
	if ( (p_PosX >= 0) && (p_PosX < m_dwTileWidth) && 
		 (p_PosY >= 0) && (p_PosY < m_dwTileHeight)	)
	{
		CTile* l_Node = m_pTile + (p_PosY * m_dwTileWidth) + p_PosX;
		if (l_Node)
		{
			return l_Node;
		}
	}

	return NULL;
}

CTile* CTileManager::GetNode( float p_PosX, float p_PosY )
{
	return GetNode((DWORD)(p_PosX/m_NodeRatio), (DWORD)(p_PosY/m_NodeRatio) );
}

CTile* CTileManager::GetNode1x( DWORD p_PosX, DWORD p_PosY )
{
	if ( (p_PosX >= 0) && (p_PosX < m_dwWidth1x) && 
		(p_PosY >= 0) && (p_PosY < m_dwHeight1x)	)
	{
		CTile* l_Node = m_pAllNode1x + (p_PosY * m_dwWidth1x) + p_PosX;
		if (l_Node)
		{
			return l_Node;
		}
	}

	return NULL;
}

CTile* CTileManager::GetNode1x( float p_PosX, float p_PosY )
{
	return GetNode1x((DWORD)(p_PosX/eDefaultTileRatio), (DWORD)(p_PosY/eDefaultTileRatio) );
}

std::vector<VECTOR3> CTileManager::GetPathResult( float p_Ratio )
{
	m_PathResult.clear();
	for (UINT i=0; i<m_AStarPath.size(); i++)
	{
		VECTOR3 t_Vector;
		t_Vector.x = (float)m_AStarPath[i]->GetPosX() * p_Ratio;
		t_Vector.y = 0;
		t_Vector.z = (float)m_AStarPath[i]->GetPosY() * p_Ratio;
		m_PathResult.push_back(t_Vector);
	}

	return m_PathResult;
}

std::deque<VECTOR3> CTileManager::GetPathResult_Optimize( float p_Ratio )
{
	m_PathResult_Optimize.clear();
	for (UINT i=1; i<m_AStarPath_Optimize.size(); i++)
	{
		VECTOR3 t_Vector;
		t_Vector.x = (float)m_AStarPath_Optimize[i]->GetPosX() * p_Ratio + p_Ratio * 0.5f;
		t_Vector.y = 0;
		t_Vector.z = (float)m_AStarPath_Optimize[i]->GetPosY() * p_Ratio + p_Ratio * 0.5f;
		m_PathResult_Optimize.push_back(t_Vector);
	}

	return m_PathResult_Optimize;
}

std::deque<VECTOR3> CTileManager::GetPathResult_Optimize_Debug( float p_Ratio )
{
	m_PathResult_Optimize_Debug.clear();
	for (UINT i=0; i<m_AStarPath_Optimize.size(); i++)
	{
		VECTOR3 t_Vector;
		t_Vector.x = (float)m_AStarPath_Optimize[i]->GetPosX() * p_Ratio + p_Ratio * 0.5f;
		t_Vector.y = 0;
		t_Vector.z = (float)m_AStarPath_Optimize[i]->GetPosY() * p_Ratio + p_Ratio * 0.5f;
		m_PathResult_Optimize_Debug.push_back(t_Vector);
	}

	return m_PathResult_Optimize_Debug;
}

BOOL CTileManager::FindClosestTile( const VECTOR3& p_Position, IPathNode*& p_pClosestNode)
{
	ePathFindLayer l_LayerInfo = GetPathFindMode();

	SetPathFindMode(ePathFindLayer_3x);

	CTile* l_Node = GetNode(p_Position.x, p_Position.z);
	if (l_Node)
	{
		if (l_Node->IsCollisonTile() == FALSE)
		{
			p_pClosestNode = l_Node;
			SetPathFindMode(l_LayerInfo);
			return TRUE;
		}
		else
		{
			DWORD l_NowPosX = l_Node->GetPosX();
			DWORD l_NowPosY = l_Node->GetPosY();

			DWORD l_StartPosX = max(0, l_NowPosX-1);
			DWORD l_StartPosY = max(0, l_NowPosY-1);
			DWORD l_EndPosX = min(m_dwTileWidth, l_NowPosX+1);
			DWORD l_EndPosY = min(m_dwTileHeight, l_NowPosY+1);
			for (DWORD l_TempPosY = l_StartPosY; l_TempPosY <= l_EndPosY; l_TempPosY++)
			{
				for (DWORD l_TempPosX = l_StartPosX; l_TempPosX <= l_EndPosX; l_TempPosX++)
				{
					CTile* l_TempNode = GetNode(l_TempPosX, l_TempPosY);
					if (l_TempNode && (l_TempNode->IsCollisonTile() == FALSE))
					{
						p_pClosestNode = l_TempNode;
						SetPathFindMode(l_LayerInfo);
						return TRUE;
					}
				}
			}
			p_pClosestNode = NULL;
		}
	}
	else
	{
		p_pClosestNode = NULL;
	}


	SetPathFindMode(l_LayerInfo);	
	return FALSE;
}

BOOL CTileManager::CollisionCheck_OneLine_New( DWORD p_StartX, DWORD p_StartY, DWORD p_EndX, DWORD p_EndY )
{
	VECTOR2 l_LineStart, l_LineEnd;
	l_LineStart.x = (float)p_StartX + 0.5f;
	l_LineStart.y = (float)p_StartY + 0.5f;
	l_LineEnd.x = (float)p_EndX + 0.5f;
	l_LineEnd.y = (float)p_EndY + 0.5f;
	H2DLine l_Line = H2DLine(l_LineStart, l_LineEnd);


	DWORD l_NowPosX = p_StartX;
	DWORD l_NowPosY = p_StartY;

	VECTOR2 l_TopSegmentS;
	VECTOR2 l_TopSegmentE;
	VECTOR2 l_LeftSegmentS;
	VECTOR2 l_LeftSegmentE;
	VECTOR2 l_RightSegmentS;
	VECTOR2 l_RightSegmentE;
	VECTOR2 l_BottomSegmentS;
	VECTOR2 l_BottomSegmentE;

	float l_DeltaX = l_LineEnd.x - l_LineStart.x;
	float l_DeltaY = l_LineEnd.y - l_LineStart.y;

	BOOL l_SideA = FALSE; BOOL l_SideB = FALSE;

	int l_Count = 0;
	CTile* l_NowNode = NULL;
	CTile* l_TempNodeA = NULL;
	CTile* l_TempNodeB = NULL;

	while( l_Count < 50000 )
	{
		l_Count++;
		
		l_SideA = FALSE;
		l_SideB = FALSE;

		l_NowNode = GetNode(l_NowPosX, l_NowPosY);
		if (l_NowNode)
		{
			if (l_NowNode->IsCollisonTile())
			{
				return FALSE;
			}
		}
		else
		{
			OutputDebugString(
				"Ñ°Â·Ê§°Ü\n");
			return FALSE;
		}

		l_TopSegmentS.x		=	(float)l_NowPosX;
		l_TopSegmentS.y		=	(float)l_NowPosY;
		l_TopSegmentE.x		=	(float)l_NowPosX + 1;
		l_TopSegmentE.y		=	(float)l_NowPosY;
		l_LeftSegmentS.x	=	(float)l_NowPosX;	
		l_LeftSegmentS.y	=	(float)l_NowPosY;
		l_LeftSegmentE.x	=	(float)l_NowPosX;
		l_LeftSegmentE.y	=	(float)l_NowPosY + 1;
		l_RightSegmentS.x	=	(float)l_NowPosX + 1;
		l_RightSegmentS.y	=	(float)l_NowPosY;
		l_RightSegmentE.x	=	(float)l_NowPosX + 1;
		l_RightSegmentE.y	=	(float)l_NowPosY + 1;
		l_BottomSegmentS.x	=	(float)l_NowPosX;
		l_BottomSegmentS.y	=	(float)l_NowPosY + 1;
		l_BottomSegmentE.x	=	(float)l_NowPosX + 1;
		l_BottomSegmentE.y	=	(float)l_NowPosY + 1;

		if (l_NowPosX == p_EndX && l_NowPosY == p_EndY)
		{
			return TRUE;
		}

		if (l_DeltaX > 0 && l_DeltaY == 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_RightSegmentS, l_RightSegmentE, l_Line) == TRUE)
			{
				l_NowPosX++;
				continue;
			}
		}
		else if (l_DeltaX < 0 && l_DeltaY == 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_LeftSegmentS, l_LeftSegmentE, l_Line) == TRUE)
			{
				l_NowPosX--;
				continue;
			}
		}
		else if (l_DeltaX == 0 && l_DeltaY < 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_TopSegmentS, l_TopSegmentE, l_Line) == TRUE)
			{
				l_NowPosY--;
				continue;
			}
		}
		else if (l_DeltaX == 0 && l_DeltaY > 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_BottomSegmentS, l_BottomSegmentE, l_Line) == TRUE)
			{
				l_NowPosY++;
				continue;
			}
		}
		else if (l_DeltaX < 0 && l_DeltaY < 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_LeftSegmentS, l_LeftSegmentE, l_Line) == TRUE)
			{
				l_SideA = TRUE;
			}
			if (CollisionCheck_2D_Segment_Line(l_TopSegmentS, l_TopSegmentE, l_Line) == TRUE)
			{
				l_SideB = TRUE;
			}
			if (l_SideA == TRUE && l_SideB == FALSE)
			{
				l_NowPosX--;
				continue;
			}
			else if (l_SideA == FALSE && l_SideB == TRUE)
			{
				l_NowPosY--;
				continue;
			}
			else if (l_SideA == TRUE && l_SideB == TRUE)
			{
				l_TempNodeA = GetNode(l_NowPosX-1, l_NowPosY);
				l_TempNodeB = GetNode(l_NowPosX, l_NowPosY-1);
				if (l_TempNodeA && l_TempNodeB &&
					!l_TempNodeA->IsCollisonTile() && !l_TempNodeB->IsCollisonTile() )
				{
					l_NowPosX--;
					l_NowPosY--;
					continue;
				}
				else
				{
					return FALSE;
				}
			}

		}
		else if (l_DeltaX > 0 && l_DeltaY < 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_RightSegmentS, l_RightSegmentE, l_Line) == TRUE)
			{
				l_SideA = TRUE;
			}
			if (CollisionCheck_2D_Segment_Line(l_TopSegmentS, l_TopSegmentE, l_Line) == TRUE)
			{
				l_SideB = TRUE;
			}
			if (l_SideA == TRUE && l_SideB == FALSE)
			{
				l_NowPosX++;
				continue;
			}
			else if (l_SideA == FALSE && l_SideB == TRUE)
			{
				l_NowPosY--;
				continue;
			}
			else if (l_SideA == TRUE && l_SideB == TRUE)
			{
				l_TempNodeA = GetNode(l_NowPosX+1, l_NowPosY);
				l_TempNodeB = GetNode(l_NowPosX, l_NowPosY-1);
				if (l_TempNodeA && l_TempNodeB &&
					!l_TempNodeA->IsCollisonTile() && !l_TempNodeB->IsCollisonTile() )
				{
					l_NowPosX++;
					l_NowPosY--;
					continue;
				}
				else
				{
					return FALSE;
				}
			}

		}
		else if (l_DeltaX < 0 && l_DeltaY > 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_LeftSegmentS, l_LeftSegmentE, l_Line) == TRUE)
			{
				l_SideA = TRUE;
			}
			if (CollisionCheck_2D_Segment_Line(l_BottomSegmentS, l_BottomSegmentE, l_Line) == TRUE)
			{
				l_SideB = TRUE;
			}
			
			if (l_SideA == TRUE && l_SideB == FALSE)
			{
				l_NowPosX--;
				continue;
			}
			else if (l_SideA == FALSE && l_SideB == TRUE)
			{
				l_NowPosY++;
				continue;
			}
			else if (l_SideA == TRUE && l_SideB == TRUE)
			{
				l_TempNodeA = GetNode(l_NowPosX-1, l_NowPosY);
				l_TempNodeB = GetNode(l_NowPosX, l_NowPosY+1);
				if (l_TempNodeA && l_TempNodeB &&
					!l_TempNodeA->IsCollisonTile() && !l_TempNodeB->IsCollisonTile() )
				{
					l_NowPosX--;
					l_NowPosY++;
					continue;
				}
				else
				{
					return FALSE;
				}
			}

		}
		else if (l_DeltaX > 0 && l_DeltaY > 0)
		{
			if (CollisionCheck_2D_Segment_Line(l_RightSegmentS, l_RightSegmentE, l_Line) == TRUE)
			{
				l_SideA = TRUE;
			}
			if (CollisionCheck_2D_Segment_Line(l_BottomSegmentS, l_BottomSegmentE, l_Line) == TRUE)
			{
				l_SideB = TRUE;
			}

			if (l_SideA == TRUE && l_SideB == FALSE)
			{
				l_NowPosX++;
				continue;
			}
			else if (l_SideA == FALSE && l_SideB == TRUE)
			{
				l_NowPosY++;
				continue;
			}
			else if (l_SideA == TRUE && l_SideB == TRUE)
			{
				l_TempNodeA = GetNode(l_NowPosX+1, l_NowPosY);
				l_TempNodeB = GetNode(l_NowPosX, l_NowPosY+1);
				if (l_TempNodeA && l_TempNodeB &&
					!l_TempNodeA->IsCollisonTile() && !l_TempNodeB->IsCollisonTile() )
				{
					l_NowPosX++;
					l_NowPosY++;
					continue;
				}
				else
				{
					return FALSE;
				}
			}


		}

		return FALSE;
	}
	return FALSE;
}

BOOL CTileManager::CollisionCheck_OneLine_New( VECTOR3* p_pStart,VECTOR3* p_pEnd )
{
	if( p_pStart->x >= 51200 || p_pStart->x < 0 ||
		p_pStart->z >= 51200 || p_pStart->z < 0 )
	{
		return FALSE;
	}

	return CollisionCheck_OneLine_New( (DWORD)(p_pStart->x / eDefaultTileRatio),
		(DWORD)(p_pStart->z / eDefaultTileRatio),
		(DWORD)(p_pEnd->x / eDefaultTileRatio),
		(DWORD)(p_pEnd->z / eDefaultTileRatio) );
}

BOOL CTileManager::FindClosest1xTile(VECTOR3& p_SafePosition, DWORD p_NowPosX, DWORD p_NowPosZ )
{
	DWORD l_NowPosXmin = max(0, p_NowPosX - 3);
	DWORD l_NowPosXmax = min(m_dwWidth1x, p_NowPosX + 3);
	
	DWORD l_NowPosZmin = max(0, p_NowPosZ - 3);
	DWORD l_NowPosZmax = min(m_dwHeight1x, p_NowPosZ + 3);

	for (DWORD j = l_NowPosZmin; j<l_NowPosZmax; j++)
	{
		for (DWORD i = l_NowPosXmin; i<l_NowPosXmax; i++)
		{
			if ( GetNode1x(i,j)->IsCollisonTile() == FALSE )
			{
				VECTOR3 l_SafePos;
				l_SafePos.x = (float)(i * eDefaultTileRatio);
				l_SafePos.y = 0;
				l_SafePos.z = (float)(j * eDefaultTileRatio);

				p_SafePosition = l_SafePos;

				return TRUE;
			}
		}
	}

	return FALSE;
}

void CTileManager::UpdateNodeInfo( VECTOR3* pPos )
{
	if( !pPos )
		return;

	DWORD t_X, t_Y;
	t_X = (DWORD)(pPos->x / 150);
	t_Y = (DWORD)(pPos->z / 150);

	CTile* t_Tile_1 = GetNode(t_X*3, t_Y*3);
	CTile* t_Tile_2 = GetNode(t_X*3+1, t_Y*3);
	CTile* t_Tile_3 = GetNode(t_X*3+2, t_Y*3);
	CTile* t_Tile_4 = GetNode(t_X*3, t_Y*3+1);
	CTile* t_Tile_5 = GetNode(t_X*3+1, t_Y*3+1);
	CTile* t_Tile_6 = GetNode(t_X*3+2, t_Y*3+1);
	CTile* t_Tile_7 = GetNode(t_X*3, t_Y*3+2);
	CTile* t_Tile_8 = GetNode(t_X*3+1, t_Y*3+2);
	CTile* t_Tile_9 = GetNode(t_X*3+2, t_Y*3+2);
	if ( t_Tile_1 && t_Tile_2 && t_Tile_3 && t_Tile_4 &&
		t_Tile_5 && t_Tile_6 && t_Tile_7 && t_Tile_8 && t_Tile_9 )
	{
		if ( !t_Tile_1->IsCollisonTile() &&
			!t_Tile_2->IsCollisonTile() &&
			!t_Tile_3->IsCollisonTile() &&
			!t_Tile_4->IsCollisonTile() &&
			!t_Tile_5->IsCollisonTile() &&
			!t_Tile_6->IsCollisonTile() &&
			!t_Tile_7->IsCollisonTile() &&
			!t_Tile_8->IsCollisonTile() &&
			!t_Tile_9->IsCollisonTile() )
		{
			TILE_ATTR l_Attrib;
			l_Attrib.uAttr = 0;
			l_Attrib.uSkillObjectBlock = 0;
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetTileAttrib(l_Attrib);
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosX(t_X);
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosY(t_Y);

		}
		else
		{
			TILE_ATTR l_Attrib;
			l_Attrib.uAttr = 1;
			l_Attrib.uSkillObjectBlock = 0;
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetTileAttrib(l_Attrib);
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosX(t_X);
			m_pAllNode3x[ m_dwWidth3x*t_Y + t_X ].SetPosY(t_Y);
		}
	}	
}