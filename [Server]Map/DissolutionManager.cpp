// DissolutionManager.cpp: implementation of the CDissolutionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionManager.h"
#include "MHFile.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "ItemManager.h"
#include "Player.h"
#include "MapDBMsgParser.h"
#include "CheckRoutine.h"

CDissolutionManager::CDissolutionManager()
{
	m_DissolutionInfoList.Initialize( 1000 );
	m_nTotalUnitOutNum = 0;
	ZeroMemory( m_DissolutionItemArray, sizeof(m_DissolutionItemArray) );
}

CDissolutionManager::~CDissolutionManager()
{
	Release();
}

BOOL CDissolutionManager::Init()
{
	if( !LoadDissolutionInfo() ) 
	{

		ASSERTMSG( 0, "LoadDissolutionInfo()" );
		return FALSE;
	}
	
	return TRUE;
}

void CDissolutionManager::Release()
{
	sITEM_DISSOLUTION_INFO* pInfo = NULL;
	m_DissolutionInfoList.SetPositionHead();
	while( pInfo = m_DissolutionInfoList.GetData() )
	{
		delete[] pInfo->pUnitItem;
		delete pInfo;
	}

	m_DissolutionInfoList.RemoveAll();	
}
void CDissolutionManager::ReloadDissolutionInfo()
{
	Release();
	LoadDissolutionInfo();
}

BOOL CDissolutionManager::LoadDissolutionInfo()
{
	CMHFile file;
	char buff[512] = {0,};
	if( !file.Init( "./Resource/itembreak.bin", "rb" ) )
		return FALSE;

	if( m_DissolutionInfoList.GetDataNum() > 0 )
		Release();
	WORD LasIndex = 0;
	while(1)
	{
		if( file.IsEOF() )
			break;

		WORD wTempDissolutionItemIdx	= file.GetWord();
		file.GetString(buff);
		WORD wTempUnitKindNum			= file.GetWord();
		WORD wTempUnitTotalNum			= file.GetWord();

		if( wTempDissolutionItemIdx != 0 && wTempUnitKindNum != 0 && wTempUnitTotalNum != 0
			&& m_DissolutionInfoList.GetData( wTempDissolutionItemIdx ) == NULL )
		{
			sITEM_DISSOLUTION_INFO* pInfo = new sITEM_DISSOLUTION_INFO;
			pInfo->wDissolutionItemIdx	= wTempDissolutionItemIdx;
			pInfo->wUnitKindNum			= wTempUnitKindNum;
			pInfo->wUnitTotalNum		= wTempUnitTotalNum;
			pInfo->pUnitItem			= new sITEM_DISSOLUTIONITEM[wTempUnitKindNum];

			WORD wTempTotalNum = 0;
			for( int i = 0 ; i < wTempUnitKindNum ; ++i )
			{
				WORD wTempUnitItemIdx = file.GetWord();
				file.GetString(buff);
				WORD wTempUnitItemNum = file.GetWord();



				if( wTempUnitItemIdx != 0 && wTempUnitItemNum != 0 )
				{

					pInfo->pUnitItem[i].wUnitItemIdx	= wTempUnitItemIdx;
					pInfo->pUnitItem[i].wUnitItemNum	= wTempUnitItemNum;
					wTempTotalNum += wTempUnitItemNum;
				}
				else
				{
					g_Console.LOG(4, "--->[116][分解列表加载失败]");
					file.Release();
					return FALSE;
				}
			}

			if( wTempTotalNum != pInfo->wUnitTotalNum || wTempTotalNum > 300 )
			{
				g_Console.LOG(4, "--->[124][分解列表加载失败]");
				file.Release();

				return FALSE;
			}

			m_DissolutionInfoList.Add( pInfo, pInfo->wDissolutionItemIdx );
			LasIndex = pInfo->wDissolutionItemIdx;
		}
		else
		{
			if( file.IsEOF())
				break;
			file.Release();
			g_Console.LOG(4, "--->[137][分解列表加载失败]");
			return FALSE;
		}
	}

	file.Release();
	g_Console.LOG(4, "--->[分解列表加载成功]");
	return TRUE;	
}

BOOL CDissolutionManager::CanDissolutionItem( WORD wItemIdx )
{
	sITEM_DISSOLUTION_INFO* pInfo = m_DissolutionInfoList.GetData( wItemIdx );
	if( pInfo )
	if( ITEMMGR->GetItemInfo( wItemIdx )->LimitLevel != 1 )
		return TRUE;

	return FALSE;
}

sITEM_DISSOLUTIONITEM* CDissolutionManager::GetDissolutionUnitItemInfo( WORD wDissolutionItemIdx, WORD* pwSize )
{
	sITEM_DISSOLUTION_INFO* pInfo = m_DissolutionInfoList.GetData( wDissolutionItemIdx );
	if( pInfo == NULL )
	{
		if( pwSize ) *pwSize = 0;
		return NULL;
	}

	if( pwSize ) *pwSize = pInfo->wUnitKindNum;

	return m_DissolutionInfoList.GetData(wDissolutionItemIdx)->pUnitItem;
}


int CDissolutionManager::ItemDissollution( CPlayer* pPlayer, ITEMBASE* pDissolutionItemInfo )
{
	if( !pPlayer )				return -100;
	if( !pDissolutionItemInfo ) return -101;

	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Dissolve))
		return -102;


	const ITEMBASE* pInventoryItemInfo = ITEMMGR->GetItemInfoAbsIn( pPlayer, pDissolutionItemInfo->Position );
	if( !pInventoryItemInfo ) return -103;
	
	if( pDissolutionItemInfo->dwDBIdx != pInventoryItemInfo->dwDBIdx ||
		pDissolutionItemInfo->wIconIdx != pInventoryItemInfo->wIconIdx ||
		pDissolutionItemInfo->Position != pInventoryItemInfo->Position ||
		pDissolutionItemInfo->Durability != pInventoryItemInfo->Durability ||
		pDissolutionItemInfo->RareIdx != pInventoryItemInfo->RareIdx ||
		pDissolutionItemInfo->QuickPosition != pInventoryItemInfo->QuickPosition )
		return -104;

	if( ITEMMGR->IsOptionItem( pDissolutionItemInfo->wIconIdx, pDissolutionItemInfo->Durability ) )
	{
		return -105;
	}

	if( ITEMMGR->IsRareOptionItem( pDissolutionItemInfo->wIconIdx, pDissolutionItemInfo->RareIdx ))
	{
		return -107;
	}

	if( !CanDissolutionItem( pDissolutionItemInfo->wIconIdx ) )
	{
		return -106;
	}
		
	m_nTotalUnitOutNum = 0;
	ZeroMemory( m_DissolutionItemArray, sizeof(m_DissolutionItemArray) );
	int rt = FillDissolutionUnitToArray( pDissolutionItemInfo->wIconIdx );
	if( rt != 0 )	return rt;

	return AddDissolutionUnitToPlayer( pPlayer, pDissolutionItemInfo );
}

int CDissolutionManager::AddDissolutionUnitToPlayer( CPlayer* pPlayer, ITEMBASE* pDissolutionItemInfo )
{
	CInventorySlot* pInventory = (CInventorySlot*)pPlayer->GetSlot( eItemTable_Inventory );
	if( !pInventory ) return -110;

	const ITEMBASE* pitem = pInventory->GetItemInfoAbs(pDissolutionItemInfo->Position);
	ITEMBASE logitem  = *pitem;
	
	if( m_nTotalUnitOutNum == 0 || m_nTotalUnitOutNum > MAX_DISSOLUTION_UNITNUM )
		return -111;

	WORD EmptySellPos[MAX_DISSOLUTION_UNITNUM] = {0,};

	WORD wOutNum = pInventory->GetEmptyCell( EmptySellPos, m_nTotalUnitOutNum - 1 ); 
	
	if( wOutNum != m_nTotalUnitOutNum - 1 )
		return -10;	
	
	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, pDissolutionItemInfo->Position, pDissolutionItemInfo->wIconIdx,
					pDissolutionItemInfo->Durability ) )
	{
		return -112;
	}	

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroyDissolution, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		logitem.wIconIdx , logitem.dwDBIdx, logitem.Position, 0, logitem.Durability, pPlayer->GetPlayerExpPoint());

	wOutNum = pInventory->GetEmptyCell( EmptySellPos, m_nTotalUnitOutNum );

	for( int i = 0 ; i < m_nTotalUnitOutNum ; ++i )
	{
		m_DissolutionItemArray[i].Position = EmptySellPos[i];
		if( pInventory->InsertItemAbs( pPlayer, EmptySellPos[i], &m_DissolutionItemArray[i], SS_PREINSERT ) == EI_TRUE )
		{
			ITEMOBTAINARRAYINFO* pArrayInfo = ITEMMGR->Alloc( pPlayer, MP_ITEM, MP_ITEM_DISSOLUTION_GETITEM, 

				pPlayer->GetID(), pPlayer->GetID(), eLog_ItemObtainDissolution, 1, 
				(DBResult)(ITEMMGR->ObtainItemDBResult));
			ItemInsertToDB( pPlayer->GetID(), m_DissolutionItemArray[i].wIconIdx, m_DissolutionItemArray[i].Durability, EmptySellPos[i], MAKEDWORD( 1, pArrayInfo->wObtainArrayID ) );
		}
	}

	return 0;
}

int CDissolutionManager::FillDissolutionUnitToArray( WORD wDissolutionItemIdx )
{
	WORD wUnitKindNum = 0;
	sITEM_DISSOLUTIONITEM* pUnitInfo = GetDissolutionUnitItemInfo( wDissolutionItemIdx, &wUnitKindNum );

	if( pUnitInfo == NULL )
		return -120;
	
	if( wUnitKindNum > MAX_DISSOLUTION_UNITNUM ) return -121;	
	
	for( int i = 0 ; i < wUnitKindNum ; ++i )
	{
		WORD tempUnitItemIdx = pUnitInfo[i].wUnitItemIdx;
		WORD tempUnitItemNum = pUnitInfo[i].wUnitItemNum;

		if( tempUnitItemIdx == 0 || tempUnitItemNum == 0 ) return -122;

		eITEM_KINDBIT eItemKind = GetItemKind( tempUnitItemIdx );
		if( !(eItemKind & eEXTRA_ITEM) )
		{
			if( FillDissolutionUnitToArray( tempUnitItemIdx ) == 0 )	
				continue;			
		}
		
		while( tempUnitItemNum > 0 )
		{
			if( m_nTotalUnitOutNum >= MAX_DISSOLUTION_UNITNUM ) return -123;

			m_DissolutionItemArray[m_nTotalUnitOutNum].wIconIdx		= tempUnitItemIdx;

			if( tempUnitItemNum > MAX_YOUNGYAKITEM_DUPNUM )
			{
				m_DissolutionItemArray[m_nTotalUnitOutNum].Durability	= MAX_YOUNGYAKITEM_DUPNUM;
				tempUnitItemNum -= MAX_YOUNGYAKITEM_DUPNUM; 
			}
			else
			{
				m_DissolutionItemArray[m_nTotalUnitOutNum].Durability	= tempUnitItemNum;
				tempUnitItemNum = 0;
			}
		
			++m_nTotalUnitOutNum;
		}
	}

	if( m_nTotalUnitOutNum > MAX_DISSOLUTION_UNITNUM ) return -124;

	return 0;
}



