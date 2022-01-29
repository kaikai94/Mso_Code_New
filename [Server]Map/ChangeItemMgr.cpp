// ChangeItemMgr.cpp: implementation of the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeItemMgr.h"
#include "MHFile.h"
#include "../[CC]Header/GameResourceStruct.h"
#include "ItemManager.h"
#include "Player.h"
#include "ObjectStateManager.h"
#include "MapDBMsgParser.h"
#include "CheckRoutine.h"
#include <vector>
#include "Guild.h"
#include "GuildManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeItemMgr::CChangeItemMgr()
{

}

CChangeItemMgr::~CChangeItemMgr()
{
	Release();
}

void CChangeItemMgr::Init()
{
	m_ChangeItemList.Initialize( 300 );
	m_MultiChangeItemList.Initialize( 100 );
	LoadItemChange();
}

void CChangeItemMgr::Release()

{
	sCHANGEITEM* pData = NULL;
	m_ChangeItemList.SetPositionHead();
	while( pData = m_ChangeItemList.GetData() )
	{
		delete pData;
	}
	m_ChangeItemList.RemoveAll();	

	sMULTICHANGEITEM* pData1 = NULL;
	m_MultiChangeItemList.SetPositionHead();
	while( pData1 = m_MultiChangeItemList.GetData() )
	{
		delete[] pData1->pChangeItem;
		delete pData1;
	}
	m_MultiChangeItemList.RemoveAll();	
}

BOOL CChangeItemMgr::LoadItemChange()
{
	CMHFile file;
	if( !file.Init( "./Resource/Server/ItemChangeList.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sCHANGEITEM* pData = new sCHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wBoxKeyType = file.GetWord();//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
		pData->wToItemType = file.GetWord();
		pData->wMaxToItem = file.GetWord();
		pData->pItemUnit = new sCHANGEITEMUNIT[pData->wMaxToItem];
		for( WORD i = 0; i < pData->wMaxToItem; ++i )
		{
			pData->pItemUnit[i].wToItemIdx = file.GetWord();
			pData->pItemUnit[i].dwToItemDur = file.GetDword();
			pData->pItemUnit[i].dwPercent = file.GetDword();
		}

		m_ChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	if( !file.Init( "./Resource/Server/ItemChangeListMulti.bin", "rb" ) )
		return FALSE;
	while( 1 )
	{
		if( file.IsEOF() )
			break;
		sMULTICHANGEITEM* pData = new sMULTICHANGEITEM;
		pData->wItemIdx = file.GetWord();
		pData->wBoxKeyType = file.GetWord();//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
		pData->wToItemType = file.GetWord();
		pData->wLimitLevel = file.GetWord();
		pData->wMaxSet = file.GetWord();
		pData->pChangeItem = new sCHANGEITEM[pData->wMaxSet];
		for( WORD i = 0; i < pData->wMaxSet; ++i )
		{
			pData->pChangeItem[i].wItemIdx = pData->wItemIdx;
			pData->pChangeItem[i].wMaxToItem = file.GetWord();
			pData->pChangeItem[i].pItemUnit = new sCHANGEITEMUNIT[pData->pChangeItem[i].wMaxToItem];

			for( WORD j = 0; j < pData->pChangeItem[i].wMaxToItem; ++j )
			{
				pData->pChangeItem[i].pItemUnit[j].wToItemIdx = file.GetWord();
				pData->pChangeItem[i].pItemUnit[j].dwToItemDur = file.GetDword();
				pData->pChangeItem[i].pItemUnit[j].dwPercent = file.GetDword();
			}
		}

		pData->nMaxItemSpace = ChangedTotalItemNum(pData,pData->wMaxSet);

		m_MultiChangeItemList.Add( pData, pData->wItemIdx );
	}
	file.Release();

	return TRUE;	
}

int CChangeItemMgr::UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( !CHKRT->ItemOf( pPlayer, TargetPos, wItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot* pSlot = pPlayer->GetSlot( TargetPos );
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( TargetPos );
	ITEMBASE Item = *pItemBase;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	
	if( pItemInfo )
	if( pItemInfo->ItemKind & eCHANGE_ITEM )
	{
		if( m_MultiChangeItemList.GetData( wItemIdx ) )
		{
			return UseMultiChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
		else
		{
			return UseNormalChangeItem( pPlayer, TargetPos, wItemIdx, &Item );
		}
	}
	else
		return 0;
	return 1;
}

int CChangeItemMgr::UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	WORD wKeyItemPos = 0;
	DWORD dwKeyItemIdx=0;
	if(CheckBoxKey(BoxKeyType(wItemIdx),pPlayer,wKeyItemPos,dwKeyItemIdx)) 
	{
		if(dwKeyItemIdx!=0)
		{
			if(ITEMMGR->DiscardItem(pPlayer,wKeyItemPos,dwKeyItemIdx,1)!=EI_TRUE)
				return 0;	
			else
			{
				MSG_ITEM_USE_ACK msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_ACK;
				msg.TargetPos = wKeyItemPos;
				msg.wItemIdx = dwKeyItemIdx;
				ITEMMGR->SendAckMsg(pPlayer,&msg,sizeof(msg));
			}
		}
	}
	else
		return 20;//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]

	WORD ItemStatic = GetToItemTypeForNormal( wItemIdx );
	if( !pItemUnit )	return 0;
	
	if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == eAbilityExp||
		pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == eChangeCharExp||
		pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
	{

	}
	else
	{
		ITEM_INFO* pNewItemInfo = ITEMMGR->GetItemInfo( pItemUnit->wToItemIdx );
		CItemSlot* pSlot =NULL;
		unsigned int nNum =0;
		if(pNewItemInfo)
		{
			if (pNewItemInfo->ItemKind & eSHOP_ITEM)
			{
				pSlot= pPlayer->GetSlot(eItemTable_ShopInven);
				nNum= ITEMMGR->GetTotalEmptySlotNum(pSlot);
			}
			else
			{
				pSlot= pPlayer->GetSlot(eItemTable_Inventory);
				nNum= ITEMMGR->GetTotalEmptySlotNum(pSlot);
				nNum++;
			}
		}

		if(NULL == pSlot) ASSERTMSG(0,"角色背包数据获取失败");
		unsigned int haveSpaceItemtotalcount = 0;

		if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			unsigned int devidecount = pItemUnit->dwToItemDur/MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;

			haveSpaceItemtotalcount += devidecount;
			if( pluscount )
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
		if( nNum < haveSpaceItemtotalcount)
		{
			MSG_WORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
			msg.wData = NOT_SPACE;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return 0;
		}
	}


	

	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )
		return 0;
	//[箱子物品提示发送][By:十里坡剑神][QQ:112582793][2017/12/6]
	sTIPITEMINFO * GetItem = NULL;
	GetItem = ITEMMGR->GetTipItem(pItemUnit->wToItemIdx);
	if(GetItem)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
		MSG_TIP msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.Flag = eItemTip;
		SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.ItemName,pItemInfo->ItemName,MAX_ITEMNAME_LENGTH+1);
		SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
		pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
	}
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	

	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	if( pItemUnit->wToItemIdx == eChangeMoney )	
	{
		pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, 64 );	
	}
	else if( pItemUnit->wToItemIdx == eAbilityExp )
	{
		pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
	}
	else if (pItemUnit->wToItemIdx == eChangeGoldMoney)
	{
		pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
		ItemShopUseLog(eLog_ChangeItemGetGoldMoney,pPlayer->GetID(),pPlayer->GetObjectName(),
			pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
			0, 0, 0, 
			pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
			0,0,0);
		//LogGoldMoney(eLog_ChangeItemGetGoldMoney, 65533, pPlayer->GetGoldMoney() - pItemUnit->dwToItemDur, pPlayer->GetID(), pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, 0,0,0);
	}
	else if (pItemUnit->wToItemIdx == eChangeCharExp)
	{
		pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeMallMoney)
	{
		ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
			pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
			pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
			0, 0, 0,
			0, 0, 0);
		pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

	}
	else if (pItemUnit->wToItemIdx == eChangeGuildExp)
	{
		DWORD GuildIdx = pPlayer->GetGuildIdx();
		if (0 == GuildIdx) return 0;

		CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

		if (NULL == pGuild) return 0;

		LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

		GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
	}
	else if( pItemUnit->wToItemIdx == eEventItem_Use )
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else if( pItemUnit->wToItemIdx == eEventItem_Use2 )
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur,ItemStatic);

	return 1;
}

unsigned int CChangeItemMgr::ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem,WORD wMaxSet)
{
	unsigned int haveSpaceItemtotalcount = 0;

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		DWORD nMin = 0;

		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
		{
			haveSpaceItemtotalcount++;
		}
		else
			pItemUnit = GetMaxSpaceItemRef( &pMultiChangeItem->pChangeItem[i] ,nMin);

		if( pItemUnit )
		{
			if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == eAbilityExp ||
				pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == eChangeCharExp||
				pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
			{
				
			}
			else
			{
				if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx)) 
				{
					if(nMin > (pItemUnit->dwToItemDur/MAX_YOUNGYAKITEM_DUPNUM))
					{
						haveSpaceItemtotalcount += nMin;
					}
					else
					{
						haveSpaceItemtotalcount += pItemUnit->dwToItemDur/MAX_YOUNGYAKITEM_DUPNUM;
                        DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;
						if( pluscount )
							haveSpaceItemtotalcount += 1;
					}
				}
				else
				{
					haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
				}			
			}
		}
	}

	return haveSpaceItemtotalcount;
}

int CChangeItemMgr::UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem )
{
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	if(NULL == pSlot) ASSERTMSG(0,"棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );
	WORD wKeyItemPos = 0;
	DWORD dwKeyItemIdx=0;
	if(CheckBoxKey(BoxKeyType(wItemIdx),pPlayer,wKeyItemPos,dwKeyItemIdx)) 
	{
		if(wKeyItemPos!=0&&dwKeyItemIdx!=0)
		{
			if(ITEMMGR->DiscardItem(pPlayer,wKeyItemPos,dwKeyItemIdx,1)!=EI_TRUE)
				return 0;	
			else
			{
				MSG_ITEM_USE_ACK msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_ACK;
				msg.TargetPos = wKeyItemPos;
				msg.wItemIdx = dwKeyItemIdx;
				ITEMMGR->SendAckMsg(pPlayer,&msg,sizeof(msg));
			}
		}
	}
	else
		return 20;//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;
	
	if( (nNum+1)  < haveSpaceItemtotalcount)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
		msg.wData = NOT_SPACE;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return 0;
	}

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;		
	if( EI_TRUE != ITEMMGR->DiscardItem( pPlayer, TargetPos, wItemIdx, 1 ) )	return 0;

	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				  eLog_ItemDestroybyChangeItem, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				  pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint() );	


	MSG_ITEM_USE_ACK msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_ACK;
	msg.dwObjectID = pPlayer->GetID();
	msg.TargetPos = TargetPos;
	msg.wItemIdx = wItemIdx;
	ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent( &pMultiChangeItem->pChangeItem[i] );

		if( pItemUnit )
		{
			if (pItemUnit->wToItemIdx == eChangeMoney)
			{
				pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, 64 );
			}
			else if (pItemUnit->wToItemIdx == eAbilityExp)
			{
				pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
			}
			else if (pItemUnit->wToItemIdx == eChangeGoldMoney)
			{
				pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
				ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
					pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
					0, 0, 0,
					pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
					0, 0, 0);
				//LogGoldMoney(eLog_ChangeItemGetGoldMoney, 65533, pPlayer->GetGoldMoney() - pItemUnit->dwToItemDur, pPlayer->GetID(), pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, 0,0, 0);
			}
			else if (pItemUnit->wToItemIdx == eChangeCharExp)
			{
				pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
			}
			else if (pItemUnit->wToItemIdx == eChangeMallMoney)
			{
				ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
					pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
					pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
					0, 0, 0,
					0, 0, 0);
				pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

			}
			else if (pItemUnit->wToItemIdx == eChangeGuildExp)
			{
				DWORD GuildIdx = pPlayer->GetGuildIdx();
				if (0 == GuildIdx) return 0;

				CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

				if (NULL == pGuild) return 0;

				LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

				GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
			}
			else
			{
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur,pMultiChangeItem->wToItemType);
			}
		}
	}

	return 1;
}
//[箱子物品是否加锁][By:十里坡剑神][QQ:112582793][2018/1/5]
WORD CChangeItemMgr::GetToItemTypeForNormal(WORD wItemIdx)
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( wItemIdx );
	if( pData == NULL ) 
		return 2;
	else
		return pData->wToItemType;
}

sCHANGEITEMUNIT* CChangeItemMgr::GetItemUnitFromCalPercent( WORD wItemIdx )
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( wItemIdx );
	if( pData == NULL ) return NULL;

//#ifdef _HK_LOCAL_
//	DWORD RandRateHigh	= rand()%1000;
//	DWORD RandRateLow	= rand()%1000;
//	DWORD RandRate		= RandRateHigh*1000 + RandRateLow;
//#else
	DWORD RandRate = rand()%MAX_CHANGE_RATE;
//#endif
	DWORD FromPercent = 0;
	DWORD ToPercent = 0;

	for( WORD i = 0; i < pData->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pData->pItemUnit[i].dwPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pData->pItemUnit[i];
		}
	}
	return NULL;
}


sCHANGEITEMUNIT* CChangeItemMgr::GetMaxSpaceItemRef(sCHANGEITEM* pSet , DWORD& nMin )
{
	DWORD MaxSpaceCount = 0;
	WORD MaxSpaceItemIdx = 0;
	for( WORD i = 0; i < pSet->wMaxToItem; ++i )
	{
		if(MaxSpaceCount < pSet->pItemUnit[i].dwToItemDur)
		{
			MaxSpaceCount = pSet->pItemUnit[i].dwToItemDur;
			MaxSpaceItemIdx = i;
		}

		if(FALSE == ITEMMGR->IsDupItem(pSet->pItemUnit[i].wToItemIdx))
		{
			if(nMin < pSet->pItemUnit[i].dwToItemDur)
				nMin = pSet->pItemUnit[i].dwToItemDur;
		}
	}

	return &pSet->pItemUnit[MaxSpaceItemIdx];
}

sCHANGEITEMUNIT* CChangeItemMgr::GetMultiItemUnitFromCalPercent( sCHANGEITEM* pSet )
{
//#ifdef _HK_LOCAL_
//	DWORD RandRateHigh	= rand()%1000;
//	DWORD RandRateLow	= rand()%1000;
//	DWORD RandRate		= RandRateHigh*1000 + RandRateLow;	//(0 ~ 999,999)归父盒狼老
//#else
	DWORD RandRate = rand()%MAX_CHANGE_RATE;	
//#endif


	DWORD FromPercent = 0;
	DWORD ToPercent = 0;

	for( WORD i = 0; i < pSet->wMaxToItem; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += pSet->pItemUnit[i].dwPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &pSet->pItemUnit[i];
		}
	}

	return NULL;
}

int	CChangeItemMgr::UseChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx )
{
	if( pPlayer->GetState() == eObjectState_Die )
		return 0;
	if( pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( wItemIdx );
	if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		if( m_MultiChangeItemList.GetData( wItemIdx ) )
		{
			return UseMultiChangeItemFromShopItem( pPlayer, wItemIdx );
		}
		else
		{
			return UseNormalChangeItemFromShopItem( pPlayer, wItemIdx );
		}
	}
	return 1;
}

int CChangeItemMgr::UseNormalChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx )
{//[单物品箱子使用][BY:十里坡剑神][QQ:112582793][2019-5-6][22:33:04]
	sCHANGEITEMUNIT* pItemUnit = GetItemUnitFromCalPercent( wItemIdx );
	if( !pItemUnit )	return 1;

	ITEM_INFO* pResultItemInfo = NULL;
	CItemSlot* pSlot = NULL;	
	unsigned int nNum = 0;

	if (pItemUnit->wToItemIdx != eEventItem_Use2 && pItemUnit->wToItemIdx != eAbilityExp &&
		pItemUnit->wToItemIdx != eEventItem_Use && pItemUnit->wToItemIdx != eChangeMoney&&
		pItemUnit->wToItemIdx != eChangeCharExp && pItemUnit->wToItemIdx != eChangeGoldMoney&&
		pItemUnit->wToItemIdx != eChangeMallMoney&&pItemUnit->wToItemIdx != eChangeGuildExp)
	{
		pResultItemInfo = ITEMMGR->GetItemInfo( pItemUnit->wToItemIdx );
		if( !pResultItemInfo )
			return 1;

		if( pResultItemInfo->ItemKind & eSHOP_ITEM )
			pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		else
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);

		if(NULL == pSlot) ASSERTMSG(0,"棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

		nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);
	}

	unsigned int haveSpaceItemtotalcount = 0;

	if (pItemUnit->wToItemIdx == eChangeMoney || pItemUnit->wToItemIdx == eAbilityExp||
		pItemUnit->wToItemIdx == eChangeGoldMoney || pItemUnit->wToItemIdx == eChangeCharExp||
		pItemUnit->wToItemIdx == eChangeMallMoney || pItemUnit->wToItemIdx == eChangeGuildExp)
	{

	}
	else
	{
		if(ITEMMGR->IsDupItem(pItemUnit->wToItemIdx))
		{
			unsigned int devidecount = pItemUnit->dwToItemDur/MAX_YOUNGYAKITEM_DUPNUM;
			DWORD pluscount = pItemUnit->dwToItemDur%MAX_YOUNGYAKITEM_DUPNUM;

			haveSpaceItemtotalcount += devidecount;
			if( pluscount )
				haveSpaceItemtotalcount += 1;
		}
		else
		{
			haveSpaceItemtotalcount += pItemUnit->dwToItemDur;
		}
	}

	if( (nNum+1)  < haveSpaceItemtotalcount)
		return 2;

	if (pItemUnit->wToItemIdx == eChangeMoney)
	{
		pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN );
	}
	else if (pItemUnit->wToItemIdx == eAbilityExp)
	{
		pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
	}
	else if (pItemUnit->wToItemIdx == eChangeGoldMoney)
	{
		pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
		ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
			pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
			0, 0, 0,
			pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
			0, 0, 0);
		//LogGoldMoney(eLog_ChangeItemGetGoldMoney, 65533, pPlayer->GetGoldMoney() - pItemUnit->dwToItemDur, pPlayer->GetID(), pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, 0,0, 0);
	}
	else if (pItemUnit->wToItemIdx == eChangeCharExp)
	{
		pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
	}
	else if (pItemUnit->wToItemIdx == eChangeMallMoney)
	{
		ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
			pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
			pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
			0, 0, 0,
			0, 0, 0);
		pPlayer->SetMallMoney(pItemUnit->dwToItemDur, 2);

	}
	else if (pItemUnit->wToItemIdx == eChangeGuildExp)
	{
		DWORD GuildIdx = pPlayer->GetGuildIdx();
		if (0 == GuildIdx) return 0;

		CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

		if (NULL == pGuild) return 0;

		LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

		GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
	}
	else if (pItemUnit->wToItemIdx == eEventItem_Use)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else if (pItemUnit->wToItemIdx == eEventItem_Use2)
	{
		MSG_NAME msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_EVENTITEM_USE2;
		SafeStrCpy( msg.Name, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
		pPlayer->SendMsg( &msg, sizeof(msg) );
	}
	else
		return ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur,2 );

	return 0;
}

int CChangeItemMgr::UseMultiChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx )
{//[多物品箱子使用][BY:十里坡剑神][QQ:112582793][2019-5-6][22:33:40]
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if(NULL == pSlot) ASSERTMSG(0,"棵官弗 酒捞袍 浇吩阑 掘阑 荐 绝嚼聪促.");

	unsigned int nNum = ITEMMGR->GetTotalEmptySlotNum(pSlot);

	sMULTICHANGEITEM* pMultiChangeItem = m_MultiChangeItemList.GetData( wItemIdx );

	unsigned int haveSpaceItemtotalcount = pMultiChangeItem->nMaxItemSpace;

	if( (nNum+1)  < haveSpaceItemtotalcount)
		return 2;

	if( pPlayer->GetMaxLevel() < pMultiChangeItem->wLimitLevel )	return 0;		

	sCHANGEITEMUNIT* pItemUnit = NULL;
	for( WORD i = 0; i < pMultiChangeItem->wMaxSet; ++i )
	{
		if( pMultiChangeItem->pChangeItem[i].wMaxToItem == 1 )
			pItemUnit = &pMultiChangeItem->pChangeItem[i].pItemUnit[0];
		else
			pItemUnit = GetMultiItemUnitFromCalPercent( &pMultiChangeItem->pChangeItem[i] );

		if( pItemUnit )
		{
			if (pItemUnit->wToItemIdx == eChangeMoney)
			{
				pPlayer->SetMoney( pItemUnit->dwToItemDur, MONEY_ADDITION, MF_OBTAIN );
			}
			else if (pItemUnit->wToItemIdx == eAbilityExp)
			{
				pPlayer->AddAbilityExp( pItemUnit->dwToItemDur );
			}
			else if (pItemUnit->wToItemIdx == eChangeGoldMoney)
			{
				pPlayer->SetGoldMoney(pItemUnit->dwToItemDur, eGetGoldMoney);
				ItemShopUseLog(eLog_ChangeItemGetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
					pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
					0, 0, 0,
					pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, pPlayer->GetGoldMoney() + pItemUnit->dwToItemDur,
					0, 0, 0);
				//LogGoldMoney(eLog_ChangeItemGetGoldMoney, 65533, pPlayer->GetGoldMoney() - pItemUnit->dwToItemDur, pPlayer->GetID(), pPlayer->GetGoldMoney(), pItemUnit->dwToItemDur, 0,0, 0);
			}
			else if (pItemUnit->wToItemIdx == eChangeCharExp)
			{
				pPlayer->AddPlayerExpPoint(pItemUnit->dwToItemDur);
			}
			else if (pItemUnit->wToItemIdx == eChangeMallMoney)
			{
				ItemShopUseLog(eLog_ChangeItemGetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(),
					pItemUnit->wToItemIdx, ITEMMGR->GetItemInfo(wItemIdx)->ItemName, 1,
					pPlayer->GetMallMoney(), pItemUnit->dwToItemDur, pPlayer->GetMallMoney() + pItemUnit->dwToItemDur,
					0, 0, 0,
					0, 0, 0);
				pPlayer->SetMallMoney(pItemUnit->dwToItemDur,2);

			}
			else if (pItemUnit->wToItemIdx == eChangeGuildExp)
			{
				DWORD GuildIdx = pPlayer->GetGuildIdx();
				if (0 == GuildIdx) return 0;

				CGuild* pGuild = GUILDMGR->GetGuild(GuildIdx);

				if (NULL == pGuild) return 0;

				LogGuildPoint(GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildUseChangeItem, pItemUnit->dwToItemDur, pPlayer->GetMaxLevel(), pPlayer->GetID());

				GuildAddGuildPoint(GuildIdx, pItemUnit->dwToItemDur, eGuildUseChangeItem, pPlayer->GetID());
			}
			else
			{
				ITEMMGR->ObtainItemFromChangeItem( pPlayer, pItemUnit->wToItemIdx, (WORD)pItemUnit->dwToItemDur,2 );
			}
		}
	}

	return 0;
}

//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/17]
BOOL CChangeItemMgr::CheckBoxKey(WORD wCurNeedBoxKey, CPlayer* pPlayer,WORD &wKeyItemPos,DWORD & dwKeyItemIdx)
{
	//[所需钥匙类型设置为0则不需要使用钥匙][By:十里坡剑神][QQ:112582793][2018/8/19]
	if(wCurNeedBoxKey == 0)
		return TRUE;
	//[获取角色背包内所有物品][By:十里坡剑神][QQ:112582793][2018/8/18]
	ITEMBASE Inventory[SLOT_INVENTORY_NUM];
	memset(Inventory,0,sizeof(Inventory));

	//[该函数为开发背包整理功能时写的新函数][By:十里坡剑神][QQ:112582793][2018/8/18]
	pPlayer->GetItemtotalInfo(Inventory);

	//[将背包内物品放入vec用于遍历查找所需物品][By:十里坡剑神][QQ:112582793][2018/8/18]
	std::vector<ITEMBASE> vcInventoryInfo;
	for(int i = 0;i<80;i++)
	{
		if(Inventory[i].dwDBIdx!=0)
			vcInventoryInfo.push_back(Inventory[i]);
	}
	//[开始遍历查找钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
	std::vector<ITEMBASE>::iterator iter;
	iter = vcInventoryInfo.begin();
	ITEM_INFO * pItemInfo  = NULL;
	for(;iter!=vcInventoryInfo.end();iter++)
	{
		pItemInfo = ITEMMGR->GetItemInfo(iter->wIconIdx);
		if(pItemInfo!=NULL)
		{
			if(pItemInfo->ItemKind==eEXTRA_ITEM_KEY)
			{
				if(pItemInfo->BuyPrice==wCurNeedBoxKey)
				{//[获取到所需钥匙类型在背包的位置，用于删除][By:十里坡剑神][QQ:112582793][2018/8/19]
					wKeyItemPos = iter->Position;
					dwKeyItemIdx = iter->wIconIdx;
					return TRUE;
				}
			}
		}
	}
	//[遍历完成未查找到钥匙返回][By:十里坡剑神][QQ:112582793][2018/8/19]
	return FALSE;
}

WORD CChangeItemMgr::BoxKeyType(DWORD dwItemIdx)
{
	sCHANGEITEM* pData = m_ChangeItemList.GetData( dwItemIdx );
	if( pData == NULL ) 
		return 0;
	else
		return pData->wBoxKeyType;
}