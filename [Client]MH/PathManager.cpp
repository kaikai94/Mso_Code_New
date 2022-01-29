








// PathManager.cpp: implementation of the CPathManager class.

//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "PathManager.h"
#include "GameResourceManager.h"
#include "STRPath.h"
#include "MHMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPathManager);


CPathManager::CPathManager()

{
	m_pSTRPath = new CSTRPath;
	m_pMap = NULL;
}


CPathManager::~CPathManager()
{

	delete m_pSTRPath;
}

void CPathManager::SetMap(CMHMap * pMap, MAPTYPE mapNum, int width) 
{ 
	m_pMap = pMap; 
	m_MapType = mapNum; 
	m_pSTRPath->SetWidth(width); 
}

int CPathManager::GetPath(VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointPos, WORD wBufCount, BYTE& wWayPointNum, CObject* pObject, BOOL bReverse, WORD wDepth)
{
	VECTOR3 TargetPos;
	if(m_pMap->CollisionTilePos((int)pDestPos->x, (int)pDestPos->z, m_MapType,pObject))
	{

		if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
		{
			pWayPointPos[0] = TargetPos; wWayPointNum = 1;

			if(m_pMap->CollisionTilePos((int)TargetPos.x, (int)TargetPos.z, m_MapType,pObject))
			{
				return PATH_FAILED;
			}
			return PATH_SUCCESS;
		}
		else
		{	
			return PATH_FAILED;
		}
	}

	if(!m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
	{
		pWayPointPos[0] = TargetPos; wWayPointNum = 1;
		return PATH_SUCCESS;
	}

	if( m_pMap->NonCollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType, pObject) )
	{

		pWayPointPos[0] = TargetPos; wWayPointNum = 1;
		return PATH_SUCCESS;
	}

	m_pSTRPath->IsValidNode = CPathManager::ValidFunc;
	if(!m_pSTRPath->FindPath(pSrcPos, pDestPos, pWayPointPos, wBufCount, wWayPointNum, wDepth))
	{

		if(bReverse)
		{
			return PATH_BLOCKFAILED;
		}
		else
		{		
			if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, m_MapType,pObject))
			{
				pWayPointPos[0] = TargetPos; wWayPointNum = 1;
				return PATH_SUCCESS;
			}
			else
			{
				return PATH_FAILED;
			}
		}
	}

	return PATH_SUCCESS;

}



BOOL CPathManager::ValidFunc(int cellX, int cellY,CObject* pObject)
{
	if(!PATHMGR->m_pMap->IsInTile(cellX, cellY, PATHMGR->m_MapType,pObject))
		return FALSE;

	if(!PATHMGR->m_pMap->CollisionTileCell(cellX, cellY, PATHMGR->m_MapType,pObject))
		return FALSE;
	return TRUE;
}




