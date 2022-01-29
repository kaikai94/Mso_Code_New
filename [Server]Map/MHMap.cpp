// MHMap.cpp: implementation of the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MHMap.h"
#include "TileManager.h"
#include "MHFile.h"
#include "..\[CC]Header\GameResourceManager.h"

extern int	g_nServerSetNum;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMHMap::CMHMap()
{
	m_pTileManager = NULL;

}

CMHMap::~CMHMap()
{

}

void CMHMap::InitMap( MAPTYPE MapNum )
{
	m_pTileManager = new CTileManager;
	
	memset(m_szTileName, 0, sizeof(256));
	sprintf(m_szTileName, "Resource/Map/%d.ttb",MapNum);

	m_pTileManager->LoadFixedTileInfo(MapNum, m_szTileName);

//KES 040712 MapServerDesc
	char filename[256];
	sprintf(filename,"serverset/%d/MapServerDesc.txt",g_nServerSetNum);
	LoadMapServerDesc( filename, MapNum );


/*
	m_pTileManager = new CTileManager;
	TileNameSetting();	// Å¸ÀÏ ÀÌ¸§ ¼ÂÆÃ

	char MapFile[64];
	strcpy(MapFile, m_szTileName[MapNum]);
	m_pTileManager->LoadFixedTileInfo(MapNum, MapFile);

	//KES 040712 MapServerDesc
	char filename[256];
	sprintf(filename,"serverset/%d/MapServerDesc.txt",g_nServerSetNum);
	LoadMapServerDesc( filename, MapNum );
	*/
}


void CMHMap::LoadMapServerDesc( char* strFileName, MAPTYPE MapNum )
{
	m_msDesc.MapNum = MapNum;

	char buf[256];
	CMHFile file;

	if( file.Init( strFileName,"rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		while(1)
		{
			if( file.IsEOF() )
				break;

			file.GetString( buf );
			if( strcmp( buf, "$MAPNUM" ) == 0 )
			{
				if( file.GetWord() != MapNum )
					continue;
				
				file.GetLine( buf, 256 );
				file.GetString( buf );
				if( buf[0] == '{' )
				{
					file.GetLine( buf, 256 );
					while(1)

					{
						if( file.IsEOF() )
							break;
						file.GetString( buf );
						if( strcmp( buf, "*VILLAGE" ) == 0 )
						{
							m_msDesc.bVillage = file.GetBool();
						}
						else if( strcmp( buf, "*PKALLOW" ) == 0 )
						{
							m_msDesc.bPKAllow = file.GetBool();
						}
						else if( strcmp( buf, "*AUTONOTEALLOW" ) == 0 )
						{
							m_msDesc.bAutoNoteAllow = file.GetBool();
						}
						else if( buf[0] == '}' )
							break;
					}

				}
				break;
			}
		}
	
		file.Release();	
	}
}

void CMHMap::TileNameSetting()
{
	/*
	for(int i = 0; i < MAX_MAP_NUM; i++)
		{
			memset(m_szTileName[i], 0, sizeof(MAX_FILE_NAME));
	//		sprintf(m_szTileName[i], "Resource/%d.ttb",i);
			sprintf(m_szTileName[i], "Resource/Map/%d.ttb",i);
		}	*/
	
}

void CMHMap::Release()
{
	SAFE_DELETE(m_pTileManager);
}
int CMHMap::GetTileWidth(MAPTYPE MapNum)
{ 
	if(m_pTileManager == NULL)
		return 0;

	return m_pTileManager->GetTileWidth(MapNum);
}

BOOL CMHMap::CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3* Target, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	return m_pTileManager->CollisionLine(pStart, pEnd, Target, pObject);
}

BOOL CMHMap::IsInTile(int cellX, int cellY, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->IsInTile(cellX, cellY, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisionTileCell(int x, int y, CObject* pObject)

{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTileWithTileIndex(x, y, pObject))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CMHMap::CollisionTilePos(float x, float y, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTileWithPosition(x, y, pObject))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CMHMap::CollisionTilePos(float x, float y, MAPTYPE MapNum)
{
	if(m_pTileManager == NULL)
		return FALSE;

	if(m_pTileManager->CollisionTileWithPosition(x, y, MapNum))
	{
		return TRUE;
	}
	return FALSE;

}
BOOL CMHMap::CollisionCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos, CObject* pObject)
{
	if(m_pTileManager == NULL)
		return FALSE;
	if(m_pTileManager->CollisionCheck(pStart,pEnd,pRtCollisonPos, pObject) == TRUE)
		return TRUE;
	
	


	return FALSE;
}

// magi82(37) �?속성 데이�?

char temp[64];	// 예외처리�?전역변�?

char* CMHMap::GetMapName(DWORD dwMapNum)
{
	if(dwMapNum == 0)
		dwMapNum = g_pServerSystem->GetMapNum();	// g_pServerSystem->GetMapNum() 에는 현재 자신�?있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);
		return temp;
	}


	return pInfo->strMapName;
}

BOOL CMHMap::IsMapKind(DWORD dwMapKind, DWORD dwMapNum)
{
	if(dwMapNum == 0)
		dwMapNum = g_pServerSystem->GetMapNum();	// g_pServerSystem->GetMapNum() 에는 현재 자신�?있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);
		return FALSE;
	}

	return ( pInfo->dwMapStateBItFlag & dwMapKind );
}

BOOL CMHMap::IsMapSame(DWORD dwMapKind, DWORD dwMapNum)
{
	if(dwMapNum == 0)

		dwMapNum = g_pServerSystem->GetMapNum();	// g_pServerSystem->GetMapNum() 에는 현재 자신�?있는 맵의 번호가 있다.

	stMAPKINDINFO* pInfo = GAMERESRCMNGR->GetMapKindInfo(dwMapNum);
	if( !pInfo )
	{
		ASSERT(0);
		ZeroMemory(temp, sizeof(temp));
		wsprintf(temp, "NoMapInfo : %d", dwMapNum);
		return FALSE;
	}

	DWORD MapKind = pInfo->dwMapStateBItFlag;		// 스크립트 상의 비트플래�?

	DWORD CurMap = dwMapKind;						// 체크해야�?비트플래�?

	DWORD nHighBit = MapKind & CurMap;				// 상위 비트
	DWORD nSubBit = MapKind - CurMap;				// 상위 비트�?뺀 나머지(하위비트 포함)
	DWORD CheckFlag = 0x000003F;					// 하위비트 체크�?위한 플래�?

	DWORD nLowBit = ( nSubBit & CheckFlag );

	return ( (nHighBit == CurMap) & !nLowBit);
}


