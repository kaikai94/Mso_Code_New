



// LootingRoom.cpp: implementation of the CLootingRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LootingRoom.h"

#include "Player.h"

#include "InventorySlot.h"
#include "WearSlot.h"
#include "cConstLinkedList.h"
//#include "cLinkedList.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "CharMove.h"
#include "LootingManager.h"
#include "..\[CC]Header\GameResourceManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLootingRoom::CLootingRoom()
{
	m_dwDiePlayer	= 0;
	m_dwAttacker	= 0;

	m_dwLootingStartTime = 0;
}

CLootingRoom::~CLootingRoom()
{

}

void CLootingRoom::SetLootingStart()
{
	m_dwLootingStartTime = gCurTime;
}

void CLootingRoom::MakeLootingRoom( CPlayer* pDiePlayer, CPlayer* pAttackPlayer )
{
	m_dwDiePlayer	= pDiePlayer->GetID();
	m_dwAttacker	= pAttackPlayer->GetID();
	ZeroMemory( m_LootingItemArray, sizeof(m_LootingItemArray) );

	{

		CInventorySlot* pInventory	= (CInventorySlot*)pDiePlayer->GetSlot(eItemTable_Inventory);
		CWearSlot* pWear			= (CWearSlot*)pDiePlayer->GetSlot( eItemTable_Weared );
		MONEYTYPE HasMoney			= pDiePlayer->GetMoney();

		EXPTYPE MaxExp	= GAMERESRCMNGR->GetMaxExpPoint( pDiePlayer->GetLevel() );
		EXPTYPE HasExp	= pDiePlayer->GetPlayerExpPoint();
		int i;

		cConstLinkedList<int> ListNumber;
		for( i = 0 ; i < PKLOOTING_ITEM_NUM ; ++i )
			ListNumber.AddTail( i );

		int nIndex1, nIndex2, nPos;
		const ITEMBASE* pItemInfo;
		if( pInventory->GetItemCount() )
		{
			cConstLinkedList<const ITEMBASE*> ListInvenItem;
			for( i = TP_INVENTORY_START ; i < TP_INVENTORY_END ; ++i )
			{
				if( !pInventory->IsEmpty( i ) )
				{
					if( !pInventory->IsLock( i ) )
						if( FALSE == ITEMMGR->IsPetSummonItem(pInventory->GetItemInfoAbs(i)->wIconIdx) &&
							FALSE == ITEMMGR->IsTitanCallItem(pInventory->GetItemInfoAbs(i)->wIconIdx) &&
							FALSE == ITEMMGR->IsTitanEquipItem(pInventory->GetItemInfoAbs(i)->wIconIdx) )
						{//[锁定、泡点装备不能抢夺][By:十里坡剑神][QQ:112582793][2018/1/1]
							if(pInventory->GetItemInfoAbs(i)->ItemStatic!=ITEM_STATIC_LUCK&&
								pInventory->GetItemInfoAbs(i)->ItemStatic!=ITEM_STATIC_PD)
								ListInvenItem.AddTail( pInventory->GetItemInfoAbs(i) );
						}
				}
						
			}
			int nCount = 0;
			while( ListInvenItem.GetCount() )
			{
				if( ++nCount >= 7 )
					break;

				nIndex1		= rand()%ListInvenItem.GetCount();
				pItemInfo	= ListInvenItem.GetAt( nIndex1 );
				nIndex2		= rand()%ListNumber.GetCount();
				nPos		= ListNumber.GetAt( nIndex2 );
				m_LootingItemArray[nPos].nKind		= eLI_ITEM;
				m_LootingItemArray[nPos].dwData		= pItemInfo->Position;

				ListInvenItem.DeleteAt( nIndex1 );
				ListNumber.DeleteAt( nIndex2 );
			}
			ListInvenItem.DeleteAll();
		}
		//[计算穿戴物品被抢夺的几率][By:十里坡剑神][QQ:112582793][2017/12/7]
		int Ratio = LOOTINGMGR->GetWearItemLootingRatio( pDiePlayer->GetBadFame() );
		int seed = rand()%10000;
		if( seed < Ratio )
		{
			if( pWear->GetItemCount() )
			{
				cConstLinkedList<const ITEMBASE*> ListWearItem;
				for( i = TP_WEAR_START ; i < TP_WEAR_END ; ++i )
				{
					if( !pWear->IsEmpty( i ) )
					{
						if( !pWear->IsLock( i ) )
						{
							if(pWear->GetItemInfoAbs(i))
							{
								if (pWear->GetItemInfoAbs(i)->ItemStatic!=ITEM_STATIC_LUCK&&
									pWear->GetItemInfoAbs(i)->ItemStatic!=ITEM_STATIC_PD)
								{//[锁定、泡点装备不能抢夺][By:十里坡剑神][QQ:112582793][2018/1/1]
									ListWearItem.AddTail( pWear->GetItemInfoAbs(i) );
								}
							}
						}
					}
				}
				if( ListWearItem.GetCount() )
				{
					nIndex1		= rand()%ListWearItem.GetCount();
					pItemInfo	= ListWearItem.GetAt( nIndex1 );
					nIndex2		= rand()%ListNumber.GetCount();
					nPos		= ListNumber.GetAt( nIndex2 );
					m_LootingItemArray[nPos].nKind		= eLI_ITEM;
					m_LootingItemArray[nPos].dwData		= pItemInfo->Position;

					ListWearItem.DeleteAt( nIndex1 );
					ListNumber.DeleteAt( nIndex2 );
				}
				ListWearItem.DeleteAll();
			}

			if( HasMoney > 0 )
			{
				MONEYTYPE LootMoney = HasMoney * float(5.0 / 100.0);
				if( LootMoney )
				{
					nIndex2		= rand()%ListNumber.GetCount();
					nPos		= ListNumber.GetAt( nIndex2 );
					m_LootingItemArray[nPos].nKind		= eLI_MONEY;
					m_LootingItemArray[nPos].dwData		= LootMoney;
					ListNumber.DeleteAt( nIndex2 );
				}
			}

			if( HasExp > 0 )
			{
				DWORD LootExp = DWORD(MaxExp* float(10.0 / 100.0));		//10%

				if( pDiePlayer->GetLevel() == 1 )
				{
					if( LootExp > HasExp )
					LootExp = (DWORD)HasExp;
				}

				if (LootExp)
				{
					nIndex2 = rand() % ListNumber.GetCount();
					nPos = ListNumber.GetAt(nIndex2);
					m_LootingItemArray[nPos].nKind = eLI_EXP;
					m_LootingItemArray[nPos].dwData = LootExp;
					ListNumber.DeleteAt(nIndex2);
				}
			}
			ListNumber.DeleteAll();			
		}
	}
	//[计算可抢夺几次][By:十里坡剑神][QQ:112582793][2017/12/7]
	m_nChance = LOOTINGMGR->GetLootingChance( pDiePlayer->GetBadFame() );
	//[计算可抢夺物品数量][By:十里坡剑神][QQ:112582793][2017/12/7]
	m_nItemLootCount = LOOTINGMGR->GetLootingItemNum( pDiePlayer->GetBadFame() );

	m_dwLootingStartTime = gCurTime;

	m_nLootedItemCount = 0;
}

BOOL CLootingRoom::IsTimeOut()
{
	if( gCurTime - m_dwLootingStartTime > PKLOOTING_LIMIT_TIME + PKLOOTING_DLG_DELAY_TIME + 3000 )
		return TRUE;
	return FALSE;
}

BOOL CLootingRoom::Loot( int nArrayNum, BOOL bForce )
{
	CPlayer* pAttackPlayer	= (CPlayer*)g_pUserTable->FindUser( m_dwAttacker );
	CPlayer* pDiePlayer		= (CPlayer*)g_pUserTable->FindUser( m_dwDiePlayer );
	if( !pAttackPlayer || !pDiePlayer )
		return FALSE;	

	DWORD dwErrorCode = eLOOTINGERROR_OK;
	float fDist = CalcDistanceXZ( CCharMove::GetPosition( pAttackPlayer), CCharMove::GetPosition( pDiePlayer) );

	if( m_nChance <= 0 )
	{
		dwErrorCode = eLOOTINGERROR_NOMORE_CHANCE;
		goto LOOTING_ERROR;
	}
	if( m_nItemLootCount <= 0 )
	{
		dwErrorCode = eLOOTINGERROR_NOMORE_ITEMLOOTCOUNT;
		goto LOOTING_ERROR;
	}
	if( nArrayNum >= PKLOOTING_ITEM_NUM || nArrayNum < 0 )
	{
		dwErrorCode = eLOOTINGERROR_INVALID_POSITION;
		goto LOOTING_ERROR;
	}
	if( m_LootingItemArray[nArrayNum].nKind == eLI_SELECTED )
	{
		dwErrorCode = eLOOTINGERROR_ALREADY_SELECTED;
		goto LOOTING_ERROR;
	}

	if( fDist > PK_LOOTING_DISTANCE + 500 )
	{
		if( !bForce )
		{
			dwErrorCode	= eLOOTINGERROR_OVER_DISTANCE;
			goto LOOTING_ERROR;
		}
	}

	--m_nChance;

	if( m_LootingItemArray[nArrayNum].nKind == eLI_NONE )
	{
		if( !bForce )
		{
			MSG_DWORD2 msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_LOOTING_NOLOOTING;
			msg.dwData1		= m_dwDiePlayer;
			msg.dwData2		= nArrayNum;
			pAttackPlayer->SendMsg( &msg, sizeof(msg) );
		}

		return TRUE;
	}
	
	if( m_LootingItemArray[nArrayNum].nKind == eLI_ITEM )	
	{
		CItemSlot* pAInven	= pAttackPlayer->GetSlot( eItemTable_Inventory );
		WORD wEmptyPos;
		if( pAInven->GetEmptyCell( &wEmptyPos ) == 0 )
		{
			MSG_DWORD msg;
			msg.Category	= MP_PK;
			msg.Protocol	= MP_PK_LOOTING_NOINVENSPACE;
			msg.dwData		= m_dwDiePlayer;
			pAttackPlayer->SendMsg( &msg, sizeof(msg) );
		}
		else
		{
			CItemSlot* pDSlot = pDiePlayer->GetSlot( (POSTYPE)m_LootingItemArray[nArrayNum].dwData );
			ITEMBASE* ItemBase = (ITEMBASE*)pDSlot->GetItemInfoAbs((POSTYPE)m_LootingItemArray[nArrayNum].dwData);
			if(!ItemBase)return TRUE;
			if(ITEMMGR->IsDupItem(ItemBase->wIconIdx)&&ItemBase->Durability>50)
			{//[抢夺叠加物品限制数量修改][By:十里坡剑神][QQ:112582793][2018/5/1 19:40]
				
				if(pDSlot->UpdateItemAbs(pDiePlayer,ItemBase->Position,0,0,0,0,ItemBase->Durability-50,ItemBase->ItemStatic,UB_DURA)!=EI_TRUE)
				{
					return TRUE;
				}
				MSG_EXCHANGE_REMOVEITEM RemoveMsg;
				RemoveMsg.Category		= MP_PK;
				RemoveMsg.Protocol		= MP_PK_LOOTING_ITEMLOOTED_UPDATEF_DIE;
				RemoveMsg.wAbsPosition	= (POSTYPE)m_LootingItemArray[nArrayNum].dwData;
				pDiePlayer->SendMsg( &RemoveMsg, sizeof( RemoveMsg ) );
				//ITEMMGR->LootObtainItem(pAttackPlayer,ItemBase->wIconIdx,50,0,0);
				//ItemBase->Position		= wEmptyPos;
				//ItemBase->QuickPosition	= 0;
				//ItemUpdateToDB(pDiePlayer->GetID(), ItemBase->dwDBIdx, ItemBase->wIconIdx, 
				//	ItemBase->Durability, ItemBase->Position, ItemBase->QuickPosition, ItemBase->RareIdx,ItemBase->ItemStatic );
				pAttackPlayer->SetKilledPlayerID(m_dwDiePlayer);
				pAttackPlayer->SetLootPos(nArrayNum);
				ITEMMGR->LootObtainItem(pAttackPlayer,ItemBase->wIconIdx,50,(POSTYPE)nArrayNum);

				LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
					eLog_ItemObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
					0, ItemBase->wIconIdx,  ItemBase->dwDBIdx, 0, ItemBase->Position, ItemBase->Durability, pDiePlayer->GetPlayerExpPoint());

				++m_nLootedItemCount;
			}
			else
			{
				CItemSlot* pDSlot = pDiePlayer->GetSlot( (POSTYPE)m_LootingItemArray[nArrayNum].dwData );
				ITEMBASE ItemBase1;
				if( EI_TRUE != pDSlot->DeleteItemAbs( pDiePlayer, (POSTYPE)m_LootingItemArray[nArrayNum].dwData, &ItemBase1 ) )
					return TRUE;
				ITEMMGR->CheckAvataChange(pDiePlayer,ItemBase1.wIconIdx,ItemBase1.Position);
				MSG_EXCHANGE_REMOVEITEM RemoveMsg;
				RemoveMsg.Category		= MP_PK;
				RemoveMsg.Protocol		= MP_PK_LOOTING_ITEMLOOTED;
				RemoveMsg.wAbsPosition	= (POSTYPE)m_LootingItemArray[nArrayNum].dwData;
				pDiePlayer->SendMsg( &RemoveMsg, sizeof( RemoveMsg ) );
				
				ItemBase1.Position		= wEmptyPos;
				ItemBase1.QuickPosition	= 0;
				ItemUpdateToDB( m_dwAttacker, ItemBase1.dwDBIdx, ItemBase1.wIconIdx, ItemBase1.Durability,
					ItemBase1.Position, ItemBase1.QuickPosition, ItemBase1.RareIdx,ItemBase1.ItemStatic );

				int rt = pAInven->InsertItemAbs( NULL, ItemBase1.Position, &ItemBase1, SS_LOCKOMIT );
				ASSERT( rt == EI_TRUE );

				LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
					eLog_ItemObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
					0, ItemBase1.wIconIdx,  ItemBase1.dwDBIdx, 0, ItemBase1.Position, ItemBase1.Durability, pDiePlayer->GetPlayerExpPoint());


				MSG_LOOTINGITEM AddMsg;
				AddMsg.Category = MP_PK;
				AddMsg.Protocol = MP_PK_LOOTING_ITEMLOOTING;
				AddMsg.dwMoney	= m_dwDiePlayer;
				AddMsg.ItemInfo	= ItemBase1;
				AddMsg.wAbsPosition = nArrayNum;

				ITEMMGR->SetItemOptionsInfoMsg(pDiePlayer, ItemBase, (MSG_LINKITEMOPTIONS*)&AddMsg);


				pAttackPlayer->SendMsg( &AddMsg, sizeof( AddMsg ) );

				++m_nLootedItemCount;
			}
			
		}
	}
	else if( m_LootingItemArray[nArrayNum].nKind == eLI_MONEY )	
	{
		MONEYTYPE AMoney	= pAttackPlayer->GetMoney();
		MONEYTYPE DMoney	= pDiePlayer->GetMoney();
		MONEYTYPE LootMoney = m_LootingItemArray[nArrayNum].dwData;
		ASSERT( DMoney >= LootMoney );

		MONEYTYPE Real = pAttackPlayer->SetMoney( LootMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetPKLooting, m_dwDiePlayer );
		pDiePlayer->SetMoney( Real, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LosePKLooted, m_dwAttacker );

		MSG_DWORD3 MoneyMsg;
		MoneyMsg.Category	= MP_PK;
		MoneyMsg.dwData1	= m_dwDiePlayer;
		MoneyMsg.dwData2	= nArrayNum;
		MoneyMsg.dwData3	= Real;
		
		MoneyMsg.Protocol	= MP_PK_LOOTING_MOENYLOOTING;
		pAttackPlayer->SendMsg( &MoneyMsg, sizeof( MoneyMsg ) );
		MoneyMsg.Protocol	= MP_PK_LOOTING_MOENYLOOTED;
		pDiePlayer->SendMsg( &MoneyMsg, sizeof( MoneyMsg ) );

		LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
				eLog_MoneyObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
				LootMoney, 0,  0, 0, 0, 0, pDiePlayer->GetPlayerExpPoint());

		++m_nLootedItemCount;
	}
	else if( m_LootingItemArray[nArrayNum].nKind == eLI_EXP )	
	{
		DWORD LootExp = m_LootingItemArray[nArrayNum].dwData;
		if( pDiePlayer->GetLevel() == 1 )
		{
			EXPTYPE DExp	= pDiePlayer->GetPlayerExpPoint();
			if( LootExp > DExp )
				LootExp = (DWORD)DExp;
		}

		pDiePlayer->ReduceExpPoint( LootExp , eExpLog_LosebyLooting );
		pAttackPlayer->AddPlayerExpPoint( LootExp );

		MSG_DWORD3 ExpMsg;
		ExpMsg.Category = MP_PK;
		ExpMsg.dwData1	= m_dwDiePlayer;
		ExpMsg.dwData2	= nArrayNum;
		ExpMsg.dwData3	= LootExp;

		ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTING;
		pAttackPlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );
		ExpMsg.Protocol = MP_PK_LOOTING_EXPLOOTED;
		pDiePlayer->SendMsg( &ExpMsg, sizeof( ExpMsg ) );

		LogItemMoney(m_dwDiePlayer, pDiePlayer->GetObjectName(), m_dwAttacker, pAttackPlayer->GetObjectName(),
				eLog_ExpObtainPK, pDiePlayer->GetMoney(eItemTable_Inventory), pAttackPlayer->GetMoney(eItemTable_Inventory), 
				LootExp, 0, 0, 0, 0, 0, pDiePlayer->GetPlayerExpPoint());

		++m_nLootedItemCount;
	}

	m_LootingItemArray[nArrayNum].nKind		= eLI_SELECTED;
	m_LootingItemArray[nArrayNum].dwData	= 0;
	--m_nItemLootCount;

	return TRUE;

LOOTING_ERROR:

	if( !bForce )
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_LOOTING_ERROR;
		msg.dwData1		= m_dwDiePlayer;
		msg.dwData2		= dwErrorCode;//errorcode
		pAttackPlayer->SendMsg( &msg, sizeof(msg) );
	}

	return TRUE;
}

void CLootingRoom::EndLootingRoomMsg( BOOL bMsgToAttacker )
{
	CPlayer* pAttackPlayer	= (CPlayer*)g_pUserTable->FindUser( m_dwAttacker );
	CPlayer* pDiePlayer		= (CPlayer*)g_pUserTable->FindUser( m_dwDiePlayer );

	MSG_DWORD EndMsg;
	EndMsg.Category		= MP_PK;
	EndMsg.Protocol		= MP_PK_LOOTING_ENDLOOTING;

	EndMsg.dwData		= m_dwDiePlayer;

	if( bMsgToAttacker )
	if( pAttackPlayer )
		pAttackPlayer->SendMsg( &EndMsg, sizeof( EndMsg ) );
	
	if( pDiePlayer )
		pDiePlayer->SendMsg( &EndMsg, sizeof( EndMsg ) );
}


void CLootingRoom::DiePlayerExitPanelty()
{
	m_nChance = PKLOOTING_ITEM_NUM;	//MAX·I
	int nPos = 0;
	while( m_nItemLootCount )
	{
		if( m_LootingItemArray[nPos].nKind == eLI_ITEM ||
			m_LootingItemArray[nPos].nKind == eLI_MONEY ||
			m_LootingItemArray[nPos].nKind == eLI_EXP )
		{
			if( Loot( nPos, TRUE ) == FALSE )
				break;
		}
		++nPos;
		if( nPos >= PKLOOTING_ITEM_NUM ) break;
	}

	LOOTINGMGR->CloseLootingRoom( m_dwDiePlayer, TRUE );
}


