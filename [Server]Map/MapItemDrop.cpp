#include "StdAfx.h"
#include ".\mapitemdrop.h"
#include "MHFile.h"
#include "time.h"
#include "Player.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "../[CC]Header/GameResourceManager.h"

CMapItemDrop::CMapItemDrop(void)
{
	m_bLoadSucceed = FALSE;
	m_bInited = FALSE;
	ZeroMemory( &m_PrevInitedTime, sizeof(SYSTEMTIME) );
	m_MapItemDropTable.Initialize(500);
}

CMapItemDrop::~CMapItemDrop(void)
{
	ClearMapDropItemList();
}

BOOL CMapItemDrop::LoadMapDropItemList()
{
	char	strFileName[64];
	CMHFile	mhListFile;
	CMHFile mhDateFile;

	sprintf( strFileName,"./Resource/Server/MapDropListInitDate.bin" );
	if( !mhDateFile.Init( strFileName, "rb" ) )
	{
		g_Console.LOG( 4, "Failed to open script file(%s). Set default value.", strFileName );

		m_wInitDay	= 5;
		m_wInitHour	= 0;
	}
	else
	{
		while(1)
		{
			if( mhDateFile.IsEOF() )
				break;

			WORD	wInitDay	= mhDateFile.GetWord();
			WORD	wInitHour	= mhDateFile.GetWord();


			if( !( 0 <= wInitDay && wInitDay <= 6 ) ||
				!( 0 <= wInitHour && wInitHour <= 23 ) )
			{
				g_Console.LOG( 4, "Wrong value in file(%s)", strFileName );

				wInitDay = 5;	
				wInitHour = 0;	
			}

			m_wInitDay	= wInitDay;
			m_wInitHour	= wInitHour;
		}
	}

	sprintf( strFileName,"./Resource/Server/MapDropItemList.bin" );
	if( !mhListFile.Init( strFileName, "rb" ) )
	{
		g_Console.LOG( 4, "Failed to open script file(%s).", strFileName );
		
		return FALSE;
	}

	while(1)
	{
		if( mhListFile.IsEOF() )
			break;

		WORD	wItemCount = 0;

		WORD	wMapNumber = mhListFile.GetWord();	

		if( wMapNumber == g_pServerSystem->GetMapNum() )
		{
			MAP_DROP_ITEM*	pDropItem = new MAP_DROP_ITEM;
			ZeroMemory( pDropItem, sizeof(MAP_DROP_ITEM) );

			pDropItem->wChannel = mhListFile.GetWord();		
			pDropItem->wMaxDropNum = mhListFile.GetWord();	

			for( int i = 0 ; i < MAX_DROPITEM_NUM ; i++ )
			{
#ifdef _DEBUG
				SafeStrCpy( pDropItem->arrMapItems[i].Name, mhListFile.GetString(), MAX_ITEMNAME_LENGTH + 1 );
#else
				char	szTempName[64];
				mhListFile.GetString( szTempName );
#endif

				pDropItem->arrMapItems[i].wItemIdx = mhListFile.GetWord();
				pDropItem->arrMapItems[i].dwDropPercent = mhListFile.GetDword();
				pDropItem->arrMapItems[i].wMaxDropCount = mhListFile.GetWord();

				pDropItem->dwTotalSumDropPercent += pDropItem->arrMapItems[i].dwDropPercent;

				if( pDropItem->arrMapItems[i].dwDropPercent )
					++wItemCount;
			}

			pDropItem->wNumDropItem = wItemCount;

			m_MapItemDropTable.Add( pDropItem, pDropItem->wChannel );
		}
		else
		{
			mhListFile.GetWord();	
			mhListFile.GetWord();

			for( int i = 0 ; i < MAX_DROPITEM_NUM ; i++ )

			{
				mhListFile.GetString();
				mhListFile.GetWord();
				mhListFile.GetDword();
				mhListFile.GetWord();
			}
		}
	}

	mhListFile.Release();

	return TRUE;
}

void CMapItemDrop::ClearMapDropItemList()
{
	m_MapItemDropTable.SetPositionHead();

	MAP_DROP_ITEM* pDropItem;
	while( pDropItem = m_MapItemDropTable.GetData() )
	{
		delete pDropItem;
	}

	m_MapItemDropTable.RemoveAll();
}

BOOL CMapItemDrop::LoadInitedTimeFile()
{
	FILE* fp = NULL;
	fp = fopen( "InitedDate", "rt" );

	if( fp == NULL )
	{
		fp = fopen( "InitedDate", "w" );
		fclose( fp );

		return FALSE;
	}
	else
	{
		fclose( fp );
		CMHFile mhInitedDateFile;
		mhInitedDateFile.Init( "InitedDate", "rt" );

		m_PrevInitedTime.wYear			= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wMonth			= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wDay			= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wDayOfWeek		= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wHour			= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wMinute		= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wSecond		= mhInitedDateFile.GetWord();
		m_PrevInitedTime.wMilliseconds	= mhInitedDateFile.GetWord();

		if( mhInitedDateFile.IsInited() )
			mhInitedDateFile.Release();
	}

	return TRUE;
}

MAP_DROP_ITEM* CMapItemDrop::GetMapDropItem( DWORD wChannel )
{

	return m_MapItemDropTable.GetData( wChannel );
}

MAP_ITEM* CMapItemDrop::GetMapItem( WORD wChannel, WORD wItemIdx )
{
	MAP_DROP_ITEM* pDropItem = GetMapDropItem( wChannel );


	if( !pDropItem )
		return NULL;

	for( int i = 0 ; i < pDropItem->wNumDropItem ; i++ )
	{
		if( pDropItem->arrMapItems[i].wItemIdx == wItemIdx )
		{
			return &(pDropItem->arrMapItems[i]);
		}
	}

	return NULL;
}

void CMapItemDrop::DropItem( CPlayer* pPlayer, MAP_ITEM* pMapItem, WORD MonsterKind )
{
	if( pMapItem->wItemIdx == 0 )
		return;
	else
	{
		if( pMapItem->wDropCount < pMapItem->wMaxDropCount )
		{
			ITEMMGR->MonsterObtainItem( pPlayer, pMapItem->wItemIdx, MonsterKind );
			//[物品提示发送][By:十里坡剑神][QQ:112582793][2017/12/6]
			sTIPITEMINFO * GetItem = NULL;
			GetItem = ITEMMGR->GetTipItem(pMapItem->wItemIdx);
			BASE_MONSTER_LIST* pMonster = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
			if(GetItem&&pMonster)
			{
				MSG_TIP msg;
				msg.Category = MP_CLIENT;
				msg.Protocol = MP_CLIENT_MSG_AGENT;
				msg.Flag = eMonsterDropItemTip;
				SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
				SafeStrCpy(msg.ItemName,pMonster->Name,MAX_ITEMNAME_LENGTH+1);
				SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
				pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
			}
			++pMapItem->wDropCount;

			UpdateItemDropInfoToDB( pPlayer->GetID(),
									g_pServerSystem->GetMapNum(),
									pPlayer->GetChannelID(),
									pMapItem->wItemIdx,

									pMapItem->wDropCount,
									pMapItem->wMaxDropCount
									);


			LogMapDropItem( pPlayer->GetUserID(),
							pPlayer->GetID(),
							pMapItem->wItemIdx,
							g_pServerSystem->GetMapNum(),
							pPlayer->GetChannelID(),
							pMapItem->wDropCount,
							pMapItem->wMaxDropCount
							);
		}
	}
}

void CMapItemDrop::Init()
{
	ClearMapDropItemList();
	if( m_bLoadSucceed = LoadMapDropItemList() )
	{
		SYSTEMTIME	sTime;
		GetLocalTime( &sTime );

		if( !LoadInitedTimeFile() )
			SetPrevInitedTime( &sTime );

		if(	sTime.wDayOfWeek == m_wInitDay )
		{
			if( sTime.wHour		== m_wInitHour &&
				sTime.wMinute	<= 5 )	
			{
				InitItemDropInfo();
				SetPrevInitedTime( &sTime );
				m_bInited = TRUE;
			}
			else
			{
				if( m_PrevInitedTime.wHour < m_wInitHour &&
					m_wInitHour < sTime.wHour + ( sTime.wHour < m_PrevInitedTime.wHour ) ? 24 : 0 )
				{
					InitItemDropInfo();
					SetPrevInitedTime( &sTime );
					m_bInited = TRUE;
				}
			}
		}
		else
		{
			if( m_PrevInitedTime.wDayOfWeek < m_wInitDay &&
				m_wInitDay < sTime.wDayOfWeek + ( sTime.wDayOfWeek < m_PrevInitedTime.wDayOfWeek ) ? 7 : 0 )
			{
				InitItemDropInfo();
				SetPrevInitedTime( &sTime );
				m_bInited = TRUE;
			}
		}

		GetDropItemInfoFromDB();
		srand((unsigned)time(NULL));
	}
}

void CMapItemDrop::Process()
{
	SYSTEMTIME	sTime;
	GetLocalTime( &sTime );

	if( m_bLoadSucceed )
	{
		if(		sTime.wDayOfWeek	== m_wInitDay
			&&	sTime.wHour			== m_wInitHour )
		{
			if( sTime.wMinute <= 5 )
			{
				InitItemDropInfo();
				SetPrevInitedTime( &sTime );

				m_bInited = TRUE;
			}
			else if( 5 < sTime.wMinute && sTime.wMinute <= 10 )	
			{
				if( m_bInited == FALSE )
				{
					InitItemDropInfo();
					SetPrevInitedTime( &sTime );
				}

				m_bInited = FALSE;
			}
			else
				m_bInited = FALSE;
		}
	}
}

void CMapItemDrop::InitItemDropInfo()
{
	MAP_DROP_ITEM*	pDropItem;
	m_MapItemDropTable.SetPositionHead();
	while( pDropItem = m_MapItemDropTable.GetData() )
	{
		for( int i = 0 ; i < pDropItem->wNumDropItem ; i++ )
			pDropItem->arrMapItems[i].wDropCount = 0;
	}

	InitItemDropInfoToDB();
}

void CMapItemDrop::CalculateDropRate( CPlayer* pPlayer, WORD MonsterKind )
{
	if( !m_bLoadSucceed )
	{
		g_Console.LOG( 4, "Script file is not loaded : MapItemDrop.cpp(381)" );
		
		return;
	}

	MAP_DROP_ITEM* pDropItem = GetMapDropItem( pPlayer->GetChannelID() );
	if( pDropItem == NULL )
		return;

	if( !pDropItem )
		return;

	for( int i = 0 ; i < pDropItem->wMaxDropNum ; i++ )
	{
		DWORD	dwRandRate		= LargeRandom( 1, pDropItem->dwTotalSumDropPercent );
		DWORD	dwFromPercent	= 0;
		DWORD	dwToPercent		= 0;

        for( int j = 0 ; j < pDropItem->wNumDropItem ; j++ )
		{
			dwFromPercent = dwToPercent;
			dwToPercent += pDropItem->arrMapItems[j].dwDropPercent;

			if( dwFromPercent < dwRandRate && dwRandRate <= dwToPercent )
				DropItem( pPlayer, &(pDropItem->arrMapItems[j]), MonsterKind );
		}
	}
}

void CMapItemDrop::SetPrevInitedTime( SYSTEMTIME* sTime )
{
	memcpy( &m_PrevInitedTime, sTime, sizeof(SYSTEMTIME) );

	CMHFile	mhInitedDateFile;

	if( !mhInitedDateFile.Init( "InitedDate.txt", "wt" ) )
		return;
	else
	{
		char	strPrevInitedDate[128];
		sprintf( strPrevInitedDate, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
									sTime->wYear,
									sTime->wMonth,
									sTime->wDay,
									sTime->wDayOfWeek,
									sTime->wHour,
									sTime->wMinute,
									sTime->wSecond,
									sTime->wMilliseconds );
		mhInitedDateFile.SetString( strPrevInitedDate );
	}

	if( mhInitedDateFile.IsInited() )
		mhInitedDateFile.Release();
}

SYSTEMTIME* CMapItemDrop::GetPrevInitedTime()
{
	return &m_PrevInitedTime;
}

void CMapItemDrop::InitItemDropInfoToDB()
{
	MapItemDropListInit( g_pServerSystem->GetMapNum() );
}

void CMapItemDrop::UpdateItemDropInfoToDB( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount )
{
	MapItemDropListUpdate( dwCharacterIdx, wMapNum, wChannel, wItemIDX, wDropCount, wMaxDropCount );
}

void CMapItemDrop::SetItemDropInfoByDB(WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount)
{
	if( !m_bLoadSucceed )
	{
		g_Console.LOG( 4, "Script file is not loaded : MapItemDrop.cpp(423)" );

		return;
	}

	if( wMapNum != g_pServerSystem->GetMapNum() )
		return;

	MAP_DROP_ITEM*	pMapDropItem = GetMapDropItem( wChannel );

	if( !pMapDropItem )
	{
		g_Console.LOG( 4, "Do not exist MapDropItem Info : MapItemDrop.cpp(435)" );
		
		return;
	}
	
	for( int i = 0 ; i < pMapDropItem->wNumDropItem ; i++ )
	{
		if( pMapDropItem->arrMapItems[i].wItemIdx == wItemIDX )
		{
			pMapDropItem->arrMapItems[i].wDropCount = wDropCount;

			break;
		}
	}
}

void CMapItemDrop::LogMapDropItem( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount )
{
	MapItemDropLog( wUserIDX, wCharacterIDX, wItemIDX, wMap, wChannel, wDropCount, wMaxDropCount );
}

void CMapItemDrop::GetDropItemInfoFromDB()
{
	MapItemDropListSelect( g_pServerSystem->GetMapNum() );
}


