// PKManager.cpp: implementation of the CPKManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PKManager.h"
#include "UserTable.h"
#include "PackedData.h"

#include "Player.h"
#include "..\[CC]Header\CommonGameFunc.h"
#include "LootingManager.h"
#include "MapDBMsgParser.h"
#include "cConstLinkedList.h"
#include "ItemManager.h"
#include "MugongManager.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "FameManager.h"
#include "GuildFieldWarMgr.h"
#include "EventMapMgr.h"
#include "PartyWarMgr.h"
#include "Battle.h"

#include "QuestMapMgr.h"
#include "MHFile.h"
#include "ChannelSystem.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern int g_nServerSetNum;

CPKManager::CPKManager()
{
	
}

CPKManager::~CPKManager()
{

}

void CPKManager::Init( BOOL bPKAllow )
{
	for( int i = 0; i < MAX_CHANNEL_NUM; ++i )
		m_bPKAllow[i] = bPKAllow;
}
void CPKManager::LoadPaneltyInfo()
{
	CMHFile file;
	char filename[256];
	sprintf(filename,"Resource/Server/PaneltyInfo.bin");
	file.Init(filename, "rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}
	for(int i = 0;i<eBadFameLeveMax-1;i++)
	{
		file.GetString(m_PaneltyInfo[i].wFame);
		m_PaneltyInfo[i].wDestroyRatio = file.GetWord();
		m_PaneltyInfo[i].wMinDestroy = file.GetWord();
		m_PaneltyInfo[i].wMaxDestroy = file.GetWord();
	}
	file.Release();
}
void CPKManager::InitForChannel()
{
	CMHFile file;
	char value[256] = {0,};

	char filename[256];
	sprintf(filename,"serverset/%d/ChannelInfo.bin",g_nServerSetNum);

	file.Init(filename, "rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF())
			break;

		strcpy( value, strupr(file.GetString()) );

		if( strcmp( value,"*PKCHANNEL" ) == 0 )
		{
			int channel = file.GetInt();
			if( channel < 1 )
				channel = 1;
			m_bPKAllow[channel-1] = TRUE;
		}
	}
	file.Release();
}

void CPKManager::SetPKAllowWithMsg( BOOL bAllow )
{
	if( m_bPKAllow[0] == bAllow ) return;
	for( int i = 0; i < MAX_CHANNEL_NUM; ++i )
        m_bPKAllow[i] = bAllow;
	
	MSG_BYTE msg;
	msg.Category	= MP_PK;
	msg.Protocol	= MP_PK_PKALLOW_CHANGED;
	
	CObject* pObject = NULL;
	g_pUserTable->SetPositionHead();
	while( pObject = g_pUserTable->GetData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;

		if( bAllow == FALSE )
		{
			if( ((CPlayer*)pObject)->IsPKMode() )
				((CPlayer*)pObject)->PKModeOffForce();
		}

		msg.bData		= bAllow;
						
		((CPlayer*)pObject)->SendMsg( &msg, sizeof( msg ) );
	}
}

void CPKManager::DiePanelty( CPlayer* pDiePlayer, CObject* pAttacker )
{
	if( QUESTMAPMGR->IsQuestMap() )
		return;
	if( EVENTMAPMGR->IsEventMap() )
		return;
	if( pDiePlayer->IsPKMode() )
	{
		pDiePlayer->SetDieForGFW( FALSE );
	}
	if( PARTYWARMGR->IsEnemy( pDiePlayer, (CPlayer*)pAttacker ) )
		return;
	if( pDiePlayer->m_bNeedRevive == FALSE )
		return;
	if( pDiePlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
		return;
	if( pDiePlayer->IsActionPanelty() == FALSE )
		return;
	PKPlayerDiePanelty( pDiePlayer );

	if( pAttacker->GetObjectKind() != eObjectKind_Player )
		return;
	PKPlayerKillPanelty( pDiePlayer, (CPlayer*)pAttacker );
	PKPlayerKiiledByWantedPanelty( pDiePlayer, (CPlayer*)pAttacker );
}


void CPKManager::PKPlayerKillPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker )
{
	if( pAttacker->IsPKMode() )
	{
		if( !pDiePlayer->IsPKMode() )
		{
			DWORD dwBadFame = pAttacker->GetBadFame();
			DWORD dwAddBadFame = 0;
			if( dwBadFame < 50 )
				dwAddBadFame = 2;
			else if( dwBadFame < 4000 )
				dwAddBadFame = 5;
			else if( dwBadFame < 20000 )
				dwAddBadFame = 10;
			else if( dwBadFame < 80000 )
				dwAddBadFame = 30;
			else if( dwBadFame < 400000 )
				dwAddBadFame = 50;
			else if( dwBadFame < 1600000 )
				dwAddBadFame = 80;
			else if( dwBadFame < 8000000 )
				dwAddBadFame = 100;
			else if( dwBadFame < 32000000 )
				dwAddBadFame = 200;
			else if( dwBadFame < 100000000 )
				dwAddBadFame = 300;
			else if( dwBadFame < 500000000 )

				dwAddBadFame = 500;
			else
				dwAddBadFame = 600;
			
			if( dwBadFame + dwAddBadFame < 1000000000 )
				pAttacker->SetBadFame( dwBadFame + dwAddBadFame );
			else
				pAttacker->SetBadFame( 1000000000 );
			
			BadFameCharacterUpdate( pAttacker->GetID(), pAttacker->GetBadFame() );
			FAMEMGR->SendBadFameMsg( pAttacker, pAttacker->GetBadFame() );
		}

		pAttacker->AddPKContinueTime( 60000 );
	}
}

void CPKManager::PKPlayerKiiledByWantedPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker )
{
	if( pAttacker->IsWantedOwner( pDiePlayer->GetWantedIdx() ) )
	{
		DWORD dwBadFame = pDiePlayer->GetBadFame();

		if( dwBadFame != 0 )

		{
			DWORD dwMinusBadFame = 0;
			
			if( dwBadFame < 50 )
				dwMinusBadFame = 2;
			else if( dwBadFame < 4000 )
				dwMinusBadFame = 5;
			else if( dwBadFame < 20000 )
				dwMinusBadFame = 10;
			else if( dwBadFame < 80000 )
				dwMinusBadFame = 30;
			else if( dwBadFame < 400000 )
				dwMinusBadFame = 50;
			else if( dwBadFame < 1600000 )
				dwMinusBadFame = 80;
			else if( dwBadFame < 8000000 )
				dwMinusBadFame = 100;
			else if( dwBadFame < 32000000 )
				dwMinusBadFame = 200;
			else if( dwBadFame < 100000000 )
				dwMinusBadFame = 300;
			else if( dwBadFame < 500000000 )
				dwMinusBadFame = 500;
			else
				dwMinusBadFame = 600;
			
			if( dwBadFame > dwMinusBadFame )
				pDiePlayer->SetBadFame( dwBadFame - dwMinusBadFame );
			else
				pDiePlayer->SetBadFame( 0 );	
			BadFameCharacterUpdate( pDiePlayer->GetID(), pDiePlayer->GetBadFame() );
			FAMEMGR->SendBadFameMsg( pDiePlayer, pDiePlayer->GetBadFame() );
		}
	}
}
// [新增PK惩罚控制] [by:十里坡剑神][QQ:112582793][2018/11/25]
BOOL CPKManager::GetPKPaneltyInfo(DWORD dwBadFame)
{
	m_BadFameLevel = eBadFameLeveNull;
	if(dwBadFame >0&&dwBadFame<=50)
		m_BadFameLevel = eBadFameLeve0_50;
	else if(dwBadFame > 50 && dwBadFame<=100)
		m_BadFameLevel = eBadFameLeve50_100;
	else if(dwBadFame >100&& dwBadFame <=500)
		m_BadFameLevel = eBadFameLeve100_500;
	else if(dwBadFame>500&&dwBadFame<=1000)
		m_BadFameLevel = eBadFameLeve500_1000;
	else if(dwBadFame>1000)
		m_BadFameLevel = eBadFameLeve1000;
	if(m_BadFameLevel!=eBadFameLeveNull)
		return TRUE;
	else
		return FALSE;
}

void CPKManager::PKPlayerDiePanelty( CPlayer* pPlayer )
{
	//PKModeExitPanelty( pPlayer );
	//SetNormalExitPanelty( pPlayer );
	DWORD dwBadFame = pPlayer->GetBadFame();
	GetPKPaneltyInfo(dwBadFame);
	//int nMinDestroyItemNum	= 0;
	//int nMaxDestroyItemNum	= 0;

	//if( dwBadFame < 50 ) //[恶名小于30不暴装备][By:十里坡剑神][QQ:112582793][2018/1/1]
	//{
	//	/*nMinDestroyItemNum = 0;
	//	nMaxDestroyItemNum = 0;*/
	//	return;
	//}
	//else if( dwBadFame < 100)
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 1;
	//}
	//else if( dwBadFame < 4000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 2;
	//}
	//else if( dwBadFame < 20000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 2;
	//}
	//else if( dwBadFame < 80000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 2;
	//}
	//else if( dwBadFame < 400000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 3;
	//}
	//else if( dwBadFame < 1600000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 3;
	//}
	//else if( dwBadFame < 8000000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 4;
	//}
	//else if( dwBadFame < 32000000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 4;
	//}
	//else if( dwBadFame < 100000000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 4;
	//}
	//else if( dwBadFame < 500000000 )
	//{
	//	nMinDestroyItemNum = 1;
	//	nMaxDestroyItemNum = 4;
	//}
	//else
	//{
	//	nMinDestroyItemNum = 2;
	//	nMaxDestroyItemNum = 4;
	//}

	//int nDestroyRatio	= dwBadFame ? (int)(0.3070f * sqrt((float)dwBadFame) + 2.6930f) : 0;
	int nDestroyRatio = m_PaneltyInfo[m_BadFameLevel].wDestroyRatio;

	BOOL bWantedPlayer = FALSE;
	if( pPlayer->GetWantedIdx() != 0 )	
	{
		nDestroyRatio = nDestroyRatio*1.3;
		bWantedPlayer = TRUE;
	}

	if( nDestroyRatio == 0 ) return;	

	if( nDestroyRatio > 100 ) nDestroyRatio = 100;	

	int nDestoryItemNum = random( m_PaneltyInfo[m_BadFameLevel].wMinDestroy, m_PaneltyInfo[m_BadFameLevel].wMaxDestroy );

	int nSeed = rand()%100;

	if( nDestoryItemNum != 0 )
	{
		if( nSeed < nDestroyRatio )
		{
			CWearSlot* pWear			= (CWearSlot*)pPlayer->GetSlot( eItemTable_Weared );
			if( pWear->GetItemCount() )
			{
				cConstLinkedList<const ITEMBASE*> ListWearItem;

				for( int i = TP_WEAR_START ; i < TP_WEAR_END ; ++i )
				{
					if( !pWear->IsEmpty( i ) )
						if( !pWear->IsLock( i ) )
						{
							ListWearItem.AddTail( pWear->GetItemInfoAbs(i) );
						}
				}
				
				while( ListWearItem.GetCount() )
				{
					int nIndex		= rand()%ListWearItem.GetCount();
					ITEMBASE ItemInfo = *ListWearItem.GetAt( nIndex );
					
					ListWearItem.DeleteAt( nIndex );
					
					
					MSG_ITEM_DESTROY msg;
					msg.Category		= MP_PK;
					msg.Protocol		= MP_PK_DESTROY_ITEM;
					msg.wAbsPosition	= ItemInfo.Position;

						
					WORD wType = bWantedPlayer && nSeed < 3 ? eLog_ItemDestroyByWanted : eLog_ItemDestroyByBadFame;
					if( wType == eLog_ItemDestroyByWanted )
						msg.cbReason		= 1;		
					else
						msg.cbReason		= 0;		
					if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, ItemInfo.Position, ItemInfo.wIconIdx,
						ItemInfo.Durability,TRUE ) )
					{
						pPlayer->SendMsg( &msg, sizeof(msg) );
						LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
							wType, pPlayer->GetMoney(), 0, 0, 
							ItemInfo.wIconIdx, ItemInfo.dwDBIdx, ItemInfo.Position, 0, 
							ItemInfo.Durability, pPlayer->GetPlayerExpPoint());
					}	
					if( --nDestoryItemNum <= 0 ) break;
				}
				ListWearItem.DeleteAll();
			}
		}
	}

	nSeed = rand()%100;
	if( nSeed < nDestroyRatio &&dwBadFame>=50)
	{
		MUGONG_TOTALINFO TotalInfo;
		pPlayer->GetMugongTotalInfo( &TotalInfo );

		cConstLinkedList<const MUGONGBASE*> ListMugong;
		for( int i = 0 ; i < SLOT_MUGONGTOTAL_NUM ; ++i )
		{
			if( TotalInfo.mugong[i].dwDBIdx != 0 )
			{
				ListMugong.AddTail( &TotalInfo.mugong[i] );
			}
		}
				
		if( ListMugong.GetCount() != 0 ) 
		{
			int nIndex = rand()%ListMugong.GetCount();
			const MUGONGBASE* pMugong = ListMugong.GetAt(nIndex);

			WORD logType = bWantedPlayer && nSeed < 3 ? eLog_MugongLevelDownByWanted : eLog_MugongLevelDownByBadFame;
			
			if( pMugong )
				pPlayer->MugongLevelDown(pMugong->wIconIdx, logType);
					
			ListMugong.DeleteAll();
		}
	}
}
void CPKManager::SetNormalExitPanelty(CPlayer* pPlayer)
{//[开红强退判断是否使用护魂卷][By:十里坡剑神][QQ:112582793][2018/3/31]
	DWORD dwCheckBit = eSiegeWarMap | eTournament | eEventMap | eQuestRoom | eSurvivalMap;
	if( !(CHANNELSYSTEM->IsBattleChannel(pPlayer->GetChannelID()) && pPlayer->IsDieForBattleChannel()) )
	{
		if( FALSE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
		{
			if(pPlayer->GetLevel() >= 5 )
			{		
				BOOL bExchanged = FALSE;
				DWORD ItemIdx = 0;
				SHOPITEMWITHTIME* pShopItem = NULL;
				if( pPlayer->GetShopItemStats()->ProtectCount )		
				{
					DWORD ItemIdx = pPlayer->GetShopItemManager()->GetProtectItemIdx();
					if( ItemIdx == 0 )
						goto ProtectAll_UseFail;
					pShopItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ItemIdx );
					if( !pShopItem )
						goto ProtectAll_UseFail;

					--pPlayer->GetShopItemStats()->ProtectCount;
					if( pPlayer->GetShopItemStats()->ProtectCount <= 0 )
					{
						pPlayer->GetShopItemManager()->SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
						pPlayer->GetShopItemManager()->DeleteUsingShopItem( ItemIdx );
						pPlayer->GetShopItemManager()->SetProtectItemIdx( 0 );
					}
					else
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ItemIdx, pPlayer->GetShopItemStats()->ProtectCount );
					}
				}
				else
				{
ProtectAll_UseFail:
					if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ExpProtect ) )
					{
						pPlayer->GetShopItemManager()->DeleteUsingShopItem( eIncantation_ExpProtect );
					}
					else
					{
						PKModeExitPanelty(pPlayer,TRUE);
					}
				}

			}
		}
	}
}
void CPKManager::PKModeExitPanelty( CPlayer* pPlayer, BOOL bPKModePanelty )
{
	DWORD dwBadFame = pPlayer->GetBadFame();

	if( dwBadFame < 50 && !bPKModePanelty )
	{
		return;
	}

	EXPTYPE dwExp		= 0;
	MONEYTYPE dwMoney	= 0;
	EXPTYPE pointForLevel	= GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );	

//#ifdef _HK_LOCAL_
//#ifndef _TW_LOCAL_
//
//	if( dwBadFame < 50 )
//	{
//		dwExp	= pointForLevel / 100;
//		dwMoney = pPlayer->GetMoney() * 2 / 100;
//	}
//	else if( dwBadFame < 100 )
//	{
//		dwExp	= pointForLevel * 2 / 100;
//		dwMoney = pPlayer->GetMoney() * 4 / 100;
//	}
//	else if( dwBadFame < 500 )
//	{
//		dwExp	= pointForLevel * 3 / 100;
//		dwMoney = pPlayer->GetMoney() * 6 / 100;
//	}
//	else if( dwBadFame < 1000 )
//	{
//		dwExp	= pointForLevel * 10 / 100;
//		dwMoney = pPlayer->GetMoney() * 13 / 100;
//	}
//	else if( dwBadFame < 2000 )
//
//	{
//		dwExp	= pointForLevel * 17 / 100;
//		dwMoney = pPlayer->GetMoney() * 21 / 100;
//	}
//	else if( dwBadFame < 5000 )
//	{
//		dwExp	= pointForLevel * 24 / 100;
//		dwMoney = pPlayer->GetMoney() * 29 / 100;
//	}
//	else if( dwBadFame < 10000 )
//	{
//		dwExp	= pointForLevel * 31 / 100;
//		dwMoney = pPlayer->GetMoney() * 36 / 100;
//	}
//
//	else if( dwBadFame < 20000 )
//	{
//		dwExp	= pointForLevel * 38 / 100;
//		dwMoney = pPlayer->GetMoney() * 44 / 100;
//	}
//	else if( dwBadFame < 50000 )
//	{
//		dwExp	= pointForLevel * 45 / 100;
//		dwMoney = pPlayer->GetMoney() * 52 / 100;
//	}
//	else if( dwBadFame < 100000 )
//	{
//		dwExp	= pointForLevel * 52 / 100;
//		dwMoney = pPlayer->GetMoney() * 59 / 100;
//	}
//	else if( dwBadFame < 200000 )
//	{
//		dwExp	= pointForLevel * 59 / 100;
//		dwMoney = pPlayer->GetMoney() * 67 / 100;
//	}
//	else if( dwBadFame < 500000 )
//	{
//		dwExp	= pointForLevel * 66 / 100;
//		dwMoney = pPlayer->GetMoney() * 74 / 100;
//	}
//	else if( dwBadFame < 1000000 )
//	{
//		dwExp	= pointForLevel * 73 / 100;
//		dwMoney = pPlayer->GetMoney() * 82 / 100;
//	}
//	else	//10撅鳖瘤
//	{
//		dwExp	= pointForLevel * 80 / 100;
//		dwMoney = pPlayer->GetMoney() * 90 / 100;
//	}
//
//#else
//
//	if( dwBadFame < 2000 )
//	{
//		dwExp	= pointForLevel / 100;
//		dwMoney = pPlayer->GetMoney() * 2 / 100;
//	}
//	else if( dwBadFame < 5000 )
//	{
//		dwExp	= pointForLevel * 2 / 100;
//		dwMoney = pPlayer->GetMoney() * 4 / 100;
//	}
//	else if( dwBadFame < 20000 )
//	{
//		dwExp	= pointForLevel * 3 / 100;
//		dwMoney = pPlayer->GetMoney() * 6 / 100;
//	}
//	else if( dwBadFame < 50000 )
//	{
//		dwExp	= pointForLevel * 10 / 100;
//		dwMoney = pPlayer->GetMoney() * 13 / 100;
//	}
//	else if( dwBadFame < 200000 )
//	{
//		dwExp	= pointForLevel * 17 / 100;
//		dwMoney = pPlayer->GetMoney() * 21 / 100;
//	}
//	else if( dwBadFame < 500000 )
//	{
//		dwExp	= pointForLevel * 24 / 100;
//		dwMoney = pPlayer->GetMoney() * 29 / 100;
//	}
//	else if( dwBadFame < 2000000 )
//	{
//		dwExp	= pointForLevel * 31 / 100;
//		dwMoney = pPlayer->GetMoney() * 36 / 100;
//	}
//	else if( dwBadFame < 5000000 )
//	{
//		dwExp	= pointForLevel * 38 / 100;
//		dwMoney = pPlayer->GetMoney() * 44 / 100;
//	}
//	else if( dwBadFame < 20000000 )
//	{
//		dwExp	= pointForLevel * 45 / 100;
//		dwMoney = pPlayer->GetMoney() * 52 / 100;
//	}
//	else if( dwBadFame < 100000000 )
//	{
//		dwExp	= pointForLevel * 52 / 100;
//		dwMoney = pPlayer->GetMoney() * 59 / 100;
//	}
//	else if( dwBadFame < 200000000 )
//	{
//		dwExp	= pointForLevel * 59 / 100;
//		dwMoney = pPlayer->GetMoney() * 67 / 100;
//	}
//	else if( dwBadFame < 400000000 )
//	{
//		dwExp	= pointForLevel * 66 / 100;
//		dwMoney = pPlayer->GetMoney() * 74 / 100;
//	}
//	else if( dwBadFame < 800000000 )
//	{
//		dwExp	= pointForLevel * 73 / 100;
//		dwMoney = pPlayer->GetMoney() * 82 / 100;
//	}
//	else	//10撅鳖瘤
//	{
//		dwExp	= pointForLevel * 80 / 100;
//		dwMoney = pPlayer->GetMoney() * 90 / 100;
//	}
//
//#endif // _TW_LOCAL_
//#else
	
	if( dwBadFame < 2000 )
	{
		dwExp	= pointForLevel / 100;
		dwMoney = pPlayer->GetMoney() * 2 / 100;
	}
	else if( dwBadFame < 5000 )
	{
		dwExp	= pointForLevel * 2 / 100;
		dwMoney = pPlayer->GetMoney() * 4 / 100;
	}
	else if( dwBadFame < 20000 )
	{
		dwExp	= pointForLevel * 3 / 100;
		dwMoney = pPlayer->GetMoney() * 6 / 100;
	}
	else if( dwBadFame < 50000 )
	{
		dwExp	= pointForLevel * 10 / 100;
		dwMoney = pPlayer->GetMoney() * 13 / 100;
	}
	else if( dwBadFame < 200000 )
	{
		dwExp	= pointForLevel * 17 / 100;
		dwMoney = pPlayer->GetMoney() * 21 / 100;
	}
	else if( dwBadFame < 500000 )
	{
		dwExp	= pointForLevel * 24 / 100;
		dwMoney = pPlayer->GetMoney() * 29 / 100;
	}
	else if( dwBadFame < 2000000 )
	{
		dwExp	= pointForLevel * 31 / 100;
		dwMoney = pPlayer->GetMoney() * 36 / 100;
	}
	else if( dwBadFame < 5000000 )
	{
		dwExp	= pointForLevel * 38 / 100;
		dwMoney = pPlayer->GetMoney() * 44 / 100;
	}
	else if( dwBadFame < 20000000 )
	{
		dwExp	= pointForLevel * 45 / 100;
		dwMoney = pPlayer->GetMoney() * 52 / 100;
	}
	else if( dwBadFame < 100000000 )
	{
		dwExp	= pointForLevel * 52 / 100;
		dwMoney = pPlayer->GetMoney() * 59 / 100;
	}
	else if( dwBadFame < 200000000 )
	{
		dwExp	= pointForLevel * 59 / 100;
		dwMoney = pPlayer->GetMoney() * 67 / 100;
	}
	else if( dwBadFame < 400000000 )
	{
		dwExp	= pointForLevel * 66 / 100;
		dwMoney = pPlayer->GetMoney() * 74 / 100;
	}
	else if( dwBadFame < 800000000 )
	{
		dwExp	= pointForLevel * 73 / 100;
		dwMoney = pPlayer->GetMoney() * 82 / 100;
	}
	else
	{
		dwExp	= pointForLevel * 80 / 100;
		dwMoney = pPlayer->GetMoney() * 90 / 100;
	}

//#endif 

	if( dwMoney )
	{
		pPlayer->SetMoney( dwMoney, MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LosePKModeExitPanelty, 0 );
	}

	if( dwExp && pPlayer->GetLevel() >= 5 )	
	{
		pPlayer->ReduceExpPoint( dwExp , eExpLog_LosebyBadFame );
	}
}

void CPKManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_PK_PKON_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer	= (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) break;

			DWORD dwChannel = pPlayer->GetChannelID();
			
			if( /*!IsPKAllow(dwChannel)||*/CHANNELSYSTEM->IsPeaceChannel(dwChannel) )	
			{//[增加安全线路配置][By:十里坡剑神][QQ:112582793][2018/3/13]
				MSG_BYTE msg;			
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKON_NACK;
				msg.bData		= ePKCODE_NOTALLAW;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				break;
			}

			int rt;
			if( ( rt = pPlayer->PKModeOn() ) == ePKCODE_OK )
			{
				MSG_DWORD msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKON_ACK;
				msg.dwData		= pmsg->dwObjectID;
				msg.dwObjectID	= pPlayer->GetID();

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
			}
			else
			{
				MSG_BYTE msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKON_NACK;
				msg.bData		= rt;
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;

	case MP_PK_PKOFF_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer	= (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) break;

			if( pPlayer->PKModeOff() )
			{
				MSG_DWORD msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKOFF_ACK;
				msg.dwData		= pmsg->dwObjectID;
				msg.dwObjectID	= pPlayer->GetID();

				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
			}
			else
			{
				MSG_DWORD msg;
				//MSGBASE msg;
				msg.Category	= MP_PK;
				msg.Protocol	= MP_PK_PKOFF_NACK;
				msg.dwData  = pPlayer->GetPKOffTime();
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;
	case MP_PK_LOOTING_SELECT_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CLootingRoom* pRoom = LOOTINGMGR->GetLootingRoom( pmsg->dwData1 );
			if( pRoom )
			{
				LOOTINGMGR->Loot( pmsg->dwData1, pmsg->dwData2 );
				
			}
			else
			{
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
				if( pPlayer )
				{
					MSG_DWORD2 msg;
					msg.Category	= MP_PK;
					msg.Protocol	= MP_PK_LOOTING_ERROR;
					msg.dwData1		= pmsg->dwData1;
					msg.dwData2		= eLOOTINGERROR_NO_LOOTINGROOM;
					pPlayer->SendMsg( &msg, sizeof(msg) );
				}
			}
		}
		break;
	
	case MP_PK_LOOTING_ENDLOOTING:
		{
			MSG_DWORD*	pmsg = (MSG_DWORD*)pMsg;
			LOOTINGMGR->CloseLootingRoom( pmsg->dwData, FALSE );
		}
	}	
}


