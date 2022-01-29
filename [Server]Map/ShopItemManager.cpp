



// ShopItemManager.cpp: implementation of the CShopItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "player.h"
#include "ShopItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MapDBMsgParser.h"
#include "AbilityManager.h"
#include "PackedData.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "StatsCalcManager.h"
#include "EventMapMgr.h"
#include "WantedManager.h"
#include "Battle.h"
#include "SiegeWarMgr.h"
#include "WeatherManager.h"
#include "ChangeItemMgr.h"
#include "QuestMapMgr.h"
#include "sRestManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern BOOL g_bPlusTime;



CShopItemManager::CShopItemManager()
{
	m_UsingItemPool = new CMemoryPoolTempl<SHOPITEMWITHTIME>;
	m_UsingItemPool->Init( 300, 10, "ShopItemPool" );

	m_MovePointPool = new CMemoryPoolTempl<MOVEDATA>;
	m_MovePointPool->Init( 50, 10, "MovePointPool" );

	m_UsingItemTable.Initialize( 300 );
	m_MovePointTable.Initialize( 30 );
}

CShopItemManager::~CShopItemManager()
{
	SAFE_DELETE( m_UsingItemPool );
	SAFE_DELETE( m_MovePointPool );
}


void CShopItemManager::Init(CPlayer* pPlayer)
{	
	m_pPlayer = pPlayer;
	m_Updatetime = 0;
	m_Checktime = 0;
	m_DupIncantation = 0;
	m_DupCharm = 0;
	m_DupHerb = 0;
	m_DupSundries = 0;
	m_DupPetEquip = 0;
	m_ProtectItemIdx = 0;
}


void CShopItemManager::Release()
{
	m_UsingItemTable.SetPositionHead();
	SHOPITEMWITHTIME* pShopItem = NULL;
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		m_UsingItemPool->Free( pShopItem );
	}
	m_UsingItemTable.RemoveAll();

	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}

void CShopItemManager::DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );

	if( bDelete )
	{
		ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 );
		CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
		SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
	}

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx );
	if( pItemInfo )
		DeleteDupParam( pItemInfo );

	ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

	stTIME curtime;
	curtime.value = GetCurTime();
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
		pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
}

void CShopItemManager::DeleteUsingShopItemInfo( DWORD ItemIdx )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );
}

void CShopItemManager::CheckEndTime(BOOL IsCheck)
{
	if( m_UsingItemTable.GetDataNum() == 0 )
		return;

	BOOL bUpdate = FALSE;
	if(IsCheck)
		m_Updatetime = 600002;
	else
		m_Updatetime += gTickTime;
	if( m_Updatetime > 600000 )
	{
		m_Updatetime = 0;
		bUpdate = TRUE;
	}

	m_Checktime += gTickTime;
	if( !bUpdate && m_Checktime < 30000 )	
		return;


	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	cPtrList listRemove;
	cPtrList listDel;

	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;
		if( pItemInfo->SellPrice == eShopItemUseParam_Forever ) 
			continue;//[永久类直接跳过][by:十里坡剑神][QQ:112582793][2019-3-24][15:51:24]
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_ShopInven );				

				if( pItemInfo->ItemType == 11 )
				{
					BOOL bfind = FALSE;
					//[查找物品是否存在背包][By:十里坡剑神][QQ:112582793][2017/11/21]
					for(int i=0; i<(SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/; i++)
					{
						ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
						if( !pItemBase )	continue;

						if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
						{
							pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
							bfind = TRUE;
							break;
						}
					}
					//[未查找到相关物品则查找身上穿戴物品][By:十里坡剑神][QQ:112582793][2017/11/21]
					if( !bfind )
					{
						CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_Weared );
						for(int i=0; i<SLOT_WEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					//[还没找到就看看宠物仓库][By:十里坡剑神][QQ:112582793][2017/11/21]
					if( !bfind )
					{
						CPetWearSlot* pSlot = (CPetWearSlot*)m_pPlayer->GetSlot( eItemTable_PetWeared );
						for(int i=0; i<SLOT_PETWEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					//[都没有？再看看泰坦仓库][By：十里坡剑神][QQ:112582793][2017/11/21]
					if( !bfind )
					{
						CTitanShopItemSlot* pSlot = (CTitanShopItemSlot*)m_pPlayer->GetSlot( eItemTable_TitanShopItem );
						for(int i=0; i<SLOT_TITANSHOPITEM_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}//[这尼玛神了，打印个Debug信息吧。继续下一个物品的使用时间检查  ][By：十里坡剑神][QQ:112582793][2017/11/21]
					if( !bfind )
					{
						char buf[256];
						sprintf(buf, "物品未找到 - 物品编号=[%d]，背包位置=[%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}
					//[找到了就丢弃物品，丢弃失败输出个日志][By:十里坡剑神][QQ:112582793][2017/11/21]
					if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
					{
						char buf[256];
						sprintf(buf, "物品未找到 - 物品编号=[%d]，背包位置=[%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}

					if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
					{
						MSG_ITEM_DISCARD_ACK msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_DISCARD_ACK;
						msg.wItemIdx = pShopItem->ShopItem.ItemBase.wIconIdx;
						msg.TargetPos = pShopItem->ShopItem.ItemBase.Position;
						msg.ItemNum = 1;
						m_pPlayer->SendMsg( &msg, sizeof(msg) );

						TakeOffAvatarItem( pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
					}

					if( (pItemInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN) || (pItemInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN) )
						DiscardSkinItem( pItemInfo->ItemIdx );					
				}
				else
				{
					listDel.AddTail(pShopItem);
				}

				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
				//if(IsCheck)//[右键删除buf扣除元宝][By:十里坡剑神][QQ:112582793][2017/11/21]
				//{//此处没有进行元宝和泡点数量检查
				//	switch(g_pServerSystem->GetGameConfig().DelBuffMode)
				//	{
				//	case  eUseGold:
				//		m_pPlayer->SetGoldMoney(g_pServerSystem->GetGameConfig().DelBuff,eDeleteBuf);
				//		ItemShopUseLog(eLog_GoldMoneyDeleteBuf,m_pPlayer->GetID(),m_pPlayer->GetObjectName(),0,"buff删除",0,
				//			0,0,0,
				//			m_pPlayer->GetGoldMoney(), g_pServerSystem->GetGameConfig().DelBuff, m_pPlayer->GetGoldMoney() - g_pServerSystem->GetGameConfig().DelBuff,
				//			0,0,0);
				//		//LogGoldMoney(eLog_GoldMoneyDeleteBuf,m_pPlayer->GetID(),m_pPlayer->GetGoldMoney(),65533,m_pPlayer->GetGoldMoney()-g_pServerSystem->GetGameConfig().DelBuff,g_pServerSystem->GetGameConfig().DelBuff,0,0);
				//		break;
				//	case eUseMall:
				//		m_pPlayer->SetMallMoney(g_pServerSystem->GetGameConfig().DelBuff,1);
				//		break;
				//	}				
				//}
			}
			else
			{
				stTIME checksum;
				checksum = EndTime;
				checksum -= curtime;

				if( !checksum.GetYear() && !checksum.GetMonth() && !checksum.GetDay() && !checksum.GetHour() && !checksum.GetMinute() )
					SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);				

			}
		}

		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime||
			(pItemInfo->SellPrice== eShopItemuseParam_FBItem&&GAMERESRCMNGR->IsFBMap(m_pPlayer->GetCurMapNum()))||IsCheck)
		{ //[计算游戏时间物品时间检查][By:十里坡剑神][QQ:112582793][2017/11/21]
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}
			int backuptime = pShopItem->ShopItem.Remaintime;
			int checksum = pShopItem->ShopItem.Remaintime - (gCurTime - pShopItem->LastCheckTime);
			if( checksum <= 0 )
			{				
				pShopItem->ShopItem.Remaintime = 0;

				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);

				ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );
				listRemove.AddTail(pShopItem);

				DeleteDupParam( pItemInfo );
				if(GAMERESRCMNGR->IsFBMap(m_pPlayer->GetCurMapNum())&&pItemInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM)
				{//[副本地图计时结束发送传送指令将角色送出副本][By:十里坡剑神][QQ:112582793][2017/11/21] 
					MSGBASE msg;
					msg.Category = MP_MOVE;
					msg.Protocol = MP_CHAR_FB_ACK;
					msg.dwObjectID = m_pPlayer->GetID();
					m_pPlayer->SendMsg(&msg,sizeof(msg));
				}
				if(pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT)
				{//[自动加点卷使用结束][By:十里坡剑神][QQ:112582793][2018/4/17]
					m_pPlayer->SetAddPoint(FALSE);
					m_pPlayer->SetAddPointFlag(0);
				}

				//if(IsCheck)//[右键删除buf扣除元宝][By:十里坡剑神][QQ:112582793][2017/11/21]
				//{//此处没有进行元宝和泡点数量检查
				//	switch(g_pServerSystem->GetGameConfig().DelBuffMode)
				//	{
				//	case  eUseGold:
				//		m_pPlayer->SetGoldMoney(g_pServerSystem->GetGameConfig().DelBuff,eDeleteBuf);
				//		ItemShopUseLog(eLog_GoldMoneyDeleteBuf,m_pPlayer->GetID(),m_pPlayer->GetObjectName(),0,"buff删除",0,
				//			0,0,0,
				//			m_pPlayer->GetGoldMoney(), g_pServerSystem->GetGameConfig().DelBuff, m_pPlayer->GetGoldMoney() - g_pServerSystem->GetGameConfig().DelBuff,
				//			0,0,0);
				//		LogGoldMoney(eLog_GoldMoneyDeleteBuf,m_pPlayer->GetID(),m_pPlayer->GetGoldMoney(),65533,m_pPlayer->GetGoldMoney()-g_pServerSystem->GetGameConfig().DelBuff,g_pServerSystem->GetGameConfig().DelBuff,0,0);
				//		break;
				//	case eUseMall:
				//		m_pPlayer->SetMallMoney(g_pServerSystem->GetGameConfig().DelBuff,1);
				//		break;
				//	}
				//}
				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
			else if( pShopItem->ShopItem.Remaintime )
			{//[计算游戏时间物品时间未到更新时间][By:十里坡剑神][QQ:112582793][2017/11/21]
				if(!GAMERESRCMNGR->IsFBMap(m_pPlayer->GetCurMapNum())&&pItemInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM)
					continue;//[副本地图物品不处于副本地图不计时][By:十里坡剑神][QQ:112582793][2017/11/21]
				pShopItem->ShopItem.Remaintime -= (gCurTime - pShopItem->LastCheckTime);
				pShopItem->LastCheckTime = gCurTime;
			}

			if( bUpdate && checksum > 0 )
				ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
			if( backuptime > (MINUTETOSECOND*1000) && pShopItem->ShopItem.Remaintime <= (MINUTETOSECOND*1000) )
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}

	PTRLISTPOS pos = listDel.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listDel.GetNext(pos);
		DeleteUsingShopItem( p->ShopItem.ItemBase.wIconIdx );
	}
	listDel.RemoveAll();

	pos = listRemove.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listRemove.GetNext(pos);
		m_UsingItemTable.Remove( p->ShopItem.ItemBase.wIconIdx );
		m_UsingItemPool->Free(p);
	}
	listRemove.RemoveAll();

	m_Checktime = 0;
}

void CShopItemManager::CheckAvatarEndtime()
{
	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;

		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
				{
					ASSERTMSG(0, "ShoItem - CheckEndTime");
					continue;
				}
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
				ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
		}
	}
}


void CShopItemManager::SendMsgDwordToPlayer( DWORD Msg, DWORD Param )
{
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = (BYTE)Msg;
	msg.dwData = Param;
	
	m_pPlayer->SendMsg(&msg, sizeof(msg));

}


void CShopItemManager::UpdateLogoutToDB()
{
	stTIME curtime;
	SHOPITEMWITHTIME* pShopItem = NULL;
	ITEM_INFO* pItemInfo = NULL;


	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo )		continue;

		if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			if( (pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
				pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM
				||pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT
				)&& pItemInfo->MeleeAttackMin )
			{
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}

			DWORD checktime = gCurTime - pShopItem->LastCheckTime;
			if( checktime > 30000 )			checktime = 30000;

			if( checktime >= pShopItem->ShopItem.Remaintime )
				pShopItem->ShopItem.Remaintime = 0;
			else
				pShopItem->ShopItem.Remaintime -= checktime;

			ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
		}
		if(pItemInfo->SellPrice == eShopItemuseParam_FBItem)//副本券单独检查，是否是从副本地图移动到其他普通地图  by:十里坡剑神	QQ:112582793
		{
			if(GAMERESRCMNGR->IsFBMap(m_pPlayer->GetCurMapNum()))
			{
				DWORD checktime = gCurTime - pShopItem->LastCheckTime;
				if( checktime > 30000 )			checktime = 30000;
				if( checktime >= pShopItem->ShopItem.Remaintime )
					pShopItem->ShopItem.Remaintime = 0;
				else
					pShopItem->ShopItem.Remaintime -= checktime;
				ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
			}
		}
	}
}

BOOL CShopItemManager::CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param )
{
	if( wIdx == 0 )			return FALSE;
	
	int calc = -1;
	if(bAdd)	calc=1;
	SHOPITEMOPTION* pShopItemOption = m_pPlayer->GetShopItemStats();
	if( !pShopItemOption )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)wIdx );
	if(!pItemInfo)
	{
		if( wIdx != eIncantation_SkPointRedist &&
			wIdx != eIncantation_StatePoint )
		{
			char buf[256]= { 0, };
			sprintf(buf, "S ItemInfo啊 绝促. PlayerIdx[%d] Idx[%d]", m_pPlayer->GetID(), wIdx);
			ASSERTMSG(0, buf);
			return FALSE;
		}
		else
			return FALSE;
	}

	if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		if( pItemInfo->ItemIdx == eIncantation_MixUp )
		{
			pShopItemOption->ItemMixSuccess += (10*calc);
			if(pShopItemOption->ItemMixSuccess < 0)
				pShopItemOption->ItemMixSuccess=0;
		}
		if( pItemInfo->GenGol )
		{
			if( Param > 0 )
				pShopItemOption->StatePoint += (WORD)Param;
		}
		else if( pItemInfo->Life )
		{
			if( Param > 0 )
				pShopItemOption->SkillPoint += pItemInfo->Life;
		}
		else if( pItemInfo->CheRyuk )
		{
			if( bAdd )
			{
				if( Param == 0 )
					pShopItemOption->ProtectCount = (char)pItemInfo->CheRyuk;
				else if( Param )
					pShopItemOption->ProtectCount = (char)Param;

				m_ProtectItemIdx = pItemInfo->ItemIdx;
			}
			else
				m_ProtectItemIdx = 0;
		}
		else if( pItemInfo->LimitJob )
		{
			if( bAdd )
				pShopItemOption->EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
				pShopItemOption->EquipLevelFree -= (BYTE)pItemInfo->LimitJob;				
			}
		}
//#ifdef _JAPAN_LOCAL_
//		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
//		{	
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
//		{
//			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
//		}
//#elif defined _HK_LOCAL_
//		else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) )
//		{	
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
//		}
//		else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) )
//		{
//			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
//		}
//#elif defined _TL_LOCAL_
//		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
//		{	
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
//		{
//			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
//				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
//		}
//		else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
//		{
//			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
//		}
//#endif
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
		pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM )
	{
		if( pItemInfo->GenGol>0 )
		{
			pShopItemOption->Gengol += (pItemInfo->GenGol*calc);
			if(pShopItemOption->Gengol < 0)
				pShopItemOption->Gengol=0;
			
			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->MinChub>0 )
		{
			pShopItemOption->Minchub += (pItemInfo->MinChub*calc);
			if(pShopItemOption->Minchub < 0)
				pShopItemOption->Minchub=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}

		if( pItemInfo->CheRyuk>0 )
		{		
			pShopItemOption->Cheryuk += (pItemInfo->CheRyuk*calc);
			if(pShopItemOption->Cheryuk < 0)
				pShopItemOption->Cheryuk=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->SimMek>0 )
		{
			pShopItemOption->Simmek += (pItemInfo->SimMek*calc);
			if(pShopItemOption->Simmek < 0)
				pShopItemOption->Simmek=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->Life>0 )
		{
			pShopItemOption->NeagongDamage += (char)(pItemInfo->Life*calc);
			if(pShopItemOption->NeagongDamage < 0)
				pShopItemOption->NeagongDamage=0;
		}
		if( pItemInfo->Shield>0 )
		{
			pShopItemOption->WoigongDamage += (char)(pItemInfo->Shield*calc);
			if(pShopItemOption->WoigongDamage < 0)
				pShopItemOption->WoigongDamage=0;
		}
		if( pItemInfo->NaeRyuk>0 )
		{
			pShopItemOption->AddSung += (pItemInfo->NaeRyuk*calc);
			if(pShopItemOption->AddSung < 0)
				pShopItemOption->AddSung=0;

			if( m_pPlayer->GetInited() )
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,m_pPlayer->GetAbilityGroup());
		}
		if( pItemInfo->LimitJob>0 )
		{
			pShopItemOption->ComboDamage += (pItemInfo->LimitJob*calc);
			if(pShopItemOption->ComboDamage < 0)
				pShopItemOption->ComboDamage=0;
		}
		if( pItemInfo->LimitGender>0 )
		{
			pShopItemOption->Critical += (pItemInfo->LimitGender*calc);
			if(pShopItemOption->Critical < 0)
				pShopItemOption->Critical=0;
		}
		if( pItemInfo->LimitLevel>0 )
		{
			pShopItemOption->StunByCri += (pItemInfo->LimitLevel*calc);
			if(pShopItemOption->StunByCri < 0)
				pShopItemOption->StunByCri=0;
		}
		if( pItemInfo->LimitGenGol>0 )
		{
			pShopItemOption->RegistPhys += (pItemInfo->LimitGenGol*calc);
			if(pShopItemOption->RegistPhys < 0)
				pShopItemOption->RegistPhys = 0;
		}
		if( pItemInfo->LimitMinChub>0 )
		{
			pShopItemOption->RegistAttr += (pItemInfo->LimitMinChub*calc);
			if(pShopItemOption->RegistAttr < 0)
				pShopItemOption->RegistAttr = 0;
		}
		if( pItemInfo->LimitCheRyuk>0 )
		{
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeNaeruyk = (char)pItemInfo->LimitCheRyuk;
				else
					pShopItemOption->PlustimeNaeruyk = 0;
			}
			else
			{

				pShopItemOption->NeaRyukSpend += (pItemInfo->LimitCheRyuk*calc);
				if(pShopItemOption->NeaRyukSpend < 0 )
					pShopItemOption->NeaRyukSpend = 0;
			}
		}
		if( pItemInfo->LimitSimMek>0 )
		{
			if( pItemInfo->MeleeAttackMin )
			{				
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeExp = (char)pItemInfo->LimitSimMek;
				else
					pShopItemOption->PlustimeExp = 0;
			}
			else
			{
				pShopItemOption->AddExp += (pItemInfo->LimitSimMek*calc);
				if(pShopItemOption->AddExp < 0)
					pShopItemOption->AddExp=0;
			}
		}
		if( pItemInfo->ItemGrade>0 )
		{
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeAbil = (char)pItemInfo->ItemGrade;
				else
					pShopItemOption->PlustimeAbil = 0;
			}
			else
			{
				pShopItemOption->AddAbility += (pItemInfo->ItemGrade*calc);
				if(pShopItemOption->AddAbility < 0)
					pShopItemOption->AddAbility=0;
			}
		}
		if( pItemInfo->RangeType>0 )
		{
			pShopItemOption->AddMugongExp += (pItemInfo->RangeType*calc);
			if(pShopItemOption->AddMugongExp < 0)
				pShopItemOption->AddMugongExp=0;
		}
		if( pItemInfo->Plus_MugongIdx )
		{
			pShopItemOption->Life += (pItemInfo->Plus_MugongIdx*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				STATSMGR->CalcCharLife(m_pPlayer);
		}
		if( pItemInfo->Plus_Value )
		{
			pShopItemOption->Shield += (pItemInfo->Plus_Value*calc);
			if( pShopItemOption->Shield <= 0 )
				pShopItemOption->Shield = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharShield(m_pPlayer);
		}
		if( pItemInfo->AllPlus_Kind )
		{
			pShopItemOption->Naeryuk += (pItemInfo->AllPlus_Kind*calc);
			if( pShopItemOption->Naeryuk <= 0 )
				pShopItemOption->Naeryuk = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharNaeruyk(m_pPlayer);
		}
		if( pItemInfo->RangeAttackMin )
		{
			pShopItemOption->bKyungGong += (pItemInfo->RangeAttackMin*calc);
		}
		if( pItemInfo->RangeAttackMax )
		{
			pShopItemOption->KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
		}
		if( pItemInfo->CriticalPercent )
		{
			pShopItemOption->ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			if( m_pPlayer->GetInited() )
				STATSMGR->CalcItemStats( m_pPlayer );
		}
		if( pItemInfo->PhyDef )
		{
			pShopItemOption->AddItemDrop += (pItemInfo->PhyDef*calc);
		}
		if( pItemInfo->NaeRyukRecover > 0 )
		{
			pShopItemOption->Decisive += (pItemInfo->NaeRyukRecover*calc);
			if(pShopItemOption->Decisive < 0)
				pShopItemOption->Decisive = 0;
		}
		if( (DWORD)(pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) > 0 )
		{
			pShopItemOption->dwStreetStallDecoration = pItemInfo->ItemIdx;

			if(bAdd == FALSE)
			{
				pShopItemOption->dwStreetStallDecoration = 0;
			}
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		if( pItemInfo->Life > 0 )
		{
			pShopItemOption->Life += (WORD)(pItemInfo->Life*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				STATSMGR->CalcCharLife(m_pPlayer);
		}
		if( pItemInfo->Shield > 0 )
		{
			pShopItemOption->Shield += (WORD)(pItemInfo->Shield*calc);
			if( pShopItemOption->Shield <= 0 )
				pShopItemOption->Shield = 0;


			if( bAdd==FALSE )
				STATSMGR->CalcCharShield(m_pPlayer);
		}
		if( pItemInfo->NaeRyuk > 0 )
		{
			pShopItemOption->Naeryuk += (pItemInfo->NaeRyuk*calc);
			if( pShopItemOption->Naeryuk <= 0 )
				pShopItemOption->Naeryuk = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharNaeruyk(m_pPlayer);
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		// CheckAvatarEquip( pItemInfo->ItemIdx, bAdd );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
//#ifdef _HK_LOCAL_
//		if( pItemInfo->CheRyuk )
//			pShopItemOption->bStreetStall += (pItemInfo->CheRyuk*calc);
//#endif
	}
	return TRUE;
}


void CShopItemManager::SendShopItemUseInfo()
{
	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;
	msg.ItemCount = 0;

	SHOPITEMWITHTIME* pItem = NULL; 
	m_UsingItemTable.SetPositionHead();
	while( pItem = m_UsingItemTable.GetData() )
	{
		memcpy( &msg.Item[msg.ItemCount], &pItem->ShopItem, sizeof(SHOPITEMBASE) );
		++msg.ItemCount;
	}

	m_pPlayer->SendMsg(&msg, msg.GetSize());
}


BOOL CShopItemManager::AddMovePoint( MOVEDATA* pData )
{
	DWORD ValidCount = MAX_MOVEDATA_PERPAGE;
	if( m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend7 ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMove2 ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend30 ) )
		ValidCount = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

	if( m_MovePointTable.GetDataNum() >= ValidCount )
		return FALSE;

	MOVEDATA* pNewData = m_MovePointPool->Alloc();
	memcpy(pNewData, pData, sizeof(MOVEDATA));

	m_MovePointTable.Add( pNewData, pNewData->DBIdx );

	return TRUE;
}


BOOL CShopItemManager::DeleteMovePoint( DWORD DBIdx )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	m_MovePointTable.Remove( DBIdx );
	m_MovePointPool->Free( pData );

	return TRUE;
}


BOOL CShopItemManager::ReNameMovePoint( DWORD DBIdx, char* newName )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	memset(pData->Name, 0, MAX_SAVEDMOVE_NAME);
	strncpy(pData->Name, newName, MAX_SAVEDMOVE_NAME-1);

	return TRUE;
}


void CShopItemManager::ReleseMovePoint()
{
	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}


SHOPITEMWITHTIME* CShopItemManager::GetUsingItemInfo( DWORD ItemIdx )
{
	return m_UsingItemTable.GetData( ItemIdx );
}


void CShopItemManager::AddShopItemUse( SHOPITEMBASE* pShopItem )
{
	SHOPITEMWITHTIME* pAddShopItem = m_UsingItemPool->Alloc();

	memcpy( &pAddShopItem->ShopItem, pShopItem, sizeof(SHOPITEMBASE) );
	pAddShopItem->LastCheckTime = gCurTime;

	m_UsingItemTable.Add( pAddShopItem, pShopItem->ItemBase.wIconIdx );
}



BOOL CShopItemManager::PutOnAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats )
{

	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData(ItemIdx);
	const ITEMBASE* pItembase = ITEMMGR->GetItemInfoAbsIn( m_pPlayer, ItemPos );
	if( !pItembase )
		return FALSE;

	if( !pShopItem || pShopItem->ShopItem.ItemBase.dwDBIdx != pItembase->dwDBIdx )
		return FALSE;


	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return FALSE;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return FALSE;

	if( pAvatarEquip->Position == eAvatar_Hat && pAvatar[eAvatar_Dress] )
	{
		AVATARITEM* pDressEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[eAvatar_Dress] );
		if( pDressEquip )
		{
			if( pDressEquip->Item[eAvatar_Hat] == 0 )
				return FALSE;
		}
	}

	SHOPITEMWITHTIME* pItem = NULL;

	if( pAvatarEquip->Position >= eAvatar_Weared_Gum )
	{
		if( m_pPlayer->GetInited() )
		{
			if( pAvatarEquip->Position != (eAvatar_Weared_Gum+m_pPlayer->GetWeaponEquipType()-1) )
				return FALSE;
		}

		if( pAvatar[pAvatarEquip->Position] > 1 )
		{
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[pAvatarEquip->Position] );
			if(!ptInfo)		return FALSE;

			ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[pAvatarEquip->Position], ptInfo->SellPrice );
			if( pItem = m_UsingItemTable.GetData( pAvatar[pAvatarEquip->Position] ) )
				pItem->ShopItem.Param = ptInfo->SellPrice;
		}

		pAvatar[pAvatarEquip->Position] = ItemIdx;
		ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, eShopItemUseParam_EquipAvatar );
		if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
			pItem->ShopItem.Param = eShopItemUseParam_EquipAvatar;
	}
	
	for(int i=0; i<eAvatar_Weared_Gum; i++)
	{		
		if( i >= eAvatar_Weared_Hair )
		{
			if( !pAvatarEquip->Item[i] )
				pAvatar[i] = 0;
		}
		else
		{
			if( i == pAvatarEquip->Position )
			{
				if( pAvatar[i] )
				{
					AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
					if( !pTemp )		continue;
					ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!ptInfo)		continue;

					for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
					{
						if( !pTemp->Item[n] )
							pAvatar[n] = 1;
					}

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( pAvatar[i] ) )
						pItem->ShopItem.Param = ptInfo->SellPrice;
				}
				
				pAvatar[i] = ItemIdx;
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, eShopItemUseParam_EquipAvatar );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = eShopItemUseParam_EquipAvatar;
			}
			
			if( !pAvatarEquip->Item[i] && pAvatar[i] &&pAvatarEquip->Position!=eAvatar_Effect)
			{//修复戴称号时过地图翅膀消失  by:十里坡剑神
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
				if( !pTemp )		continue;
				ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
				if(!ptInfo)		continue;
				
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pTemp->Item[n] )
						pAvatar[n] = 1;
				}				
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
				pAvatar[i] = 0;
			}
		}
	}

	if( ItemPos )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = m_pPlayer->GetID();
		msg.ItemIdx = ItemIdx;
		msg.ItemPos = ItemPos;
		memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
	}

	CalcAvatarOption( bCalcStats );

	return TRUE;
}


BOOL CShopItemManager::TakeOffAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats  )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return FALSE;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return FALSE;
	

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return FALSE;


	SHOPITEMWITHTIME* pItem = NULL;
	if( pAvatarEquip->Position >= eAvatar_Weared_Gum )
	{
		if( pAvatar[pAvatarEquip->Position] > 1 )
		{
			BOOL bCheck = FALSE;
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[pAvatarEquip->Position] );
			if(!ptInfo)		return FALSE;

			if( pAvatar[eAvatar_Dress] )
			{
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[eAvatar_Dress] );				
				if( !pTemp )		return FALSE;

				pAvatar[pAvatarEquip->Position] = pTemp->Item[pAvatarEquip->Position];
			}				
			else
			{				
				if(pAvatar[pAvatarEquip->Position] <= 1)
					pAvatar[pAvatarEquip->Position] = 0;
				else
					bCheck = TRUE;
			}

			if(bCheck == FALSE)
			{
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[pAvatarEquip->Position], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( pAvatar[pAvatarEquip->Position] ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
			}
			
		}
	}


	for(int i=0; i<eAvatar_Max; i++)
	{
		if( pAvatar[i] )
		{
			if( i==pAvatarEquip->Position )
			{
				pAvatar[i] = 0;
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pAvatarEquip->Item[n] )
						pAvatar[n] = 1;
				}

				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = pItemInfo->SellPrice;
			}
			if( pAvatarEquip->Item[i] == 0 )
			{
				if( i>=eAvatar_Hat && i<eAvatar_Weared_Hair )
				{
					ITEM_INFO* poffinfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!poffinfo)		return FALSE;

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], poffinfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( pAvatar[i] ) )
						pItem->ShopItem.Param = poffinfo->SellPrice;
					pAvatar[i] = 0;
				}
			}
		}
	}
	
	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	CalcAvatarOption( bCalcStats );
	return TRUE;
}


void CShopItemManager::CalcAvatarOption( BOOL bCalcStats )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;

	AVATARITEMOPTION* pAvatarOption = m_pPlayer->GetAvatarOption();
	memset( pAvatarOption, 0, sizeof(AVATARITEMOPTION) );

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( pAvatar[i] < 2 )					continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
		if( !pItemInfo )		continue;

		if( pItemInfo->GenGol > 0 )
			pAvatarOption->Gengol += pItemInfo->GenGol;
		if( pItemInfo->MinChub > 0 )
			pAvatarOption->Minchub += pItemInfo->MinChub;
		if( pItemInfo->CheRyuk > 0 )
			pAvatarOption->Cheryuk += pItemInfo->CheRyuk;
		if( pItemInfo->SimMek > 0 )
			pAvatarOption->Simmek += pItemInfo->SimMek;
		if( pItemInfo->Life > 0 )
			pAvatarOption->Life += (WORD)pItemInfo->Life;
		if( pItemInfo->Shield > 0 )
			pAvatarOption->Shield += (WORD)pItemInfo->Shield;
		if( pItemInfo->NaeRyuk > 0 )
			pAvatarOption->Naeruyk += pItemInfo->NaeRyuk;
		if( pItemInfo->MeleeAttackMin > 0 )
			pAvatarOption->Attack += pItemInfo->MeleeAttackMin;
		if( pItemInfo->CriticalPercent > 0 )
			pAvatarOption->Critical += pItemInfo->CriticalPercent;
		if( pItemInfo->Plus_MugongIdx > 0 )
			pAvatarOption->CounterPercent += pItemInfo->Plus_MugongIdx;
		if( pItemInfo->Plus_Value > 0 )
			pAvatarOption->CounterDamage += pItemInfo->Plus_Value;
		if( pItemInfo->AllPlus_Kind == 1 )
			pAvatarOption->bKyungGong = 1;
		if( pItemInfo->LimitCheRyuk > 0 )
			pAvatarOption->NeaRyukSpend += pItemInfo->LimitCheRyuk;
		if( pItemInfo->LimitJob > 0 )
			pAvatarOption->NeagongDamage += pItemInfo->LimitJob;
		if( pItemInfo->LimitGender > 0 )
			pAvatarOption->WoigongDamage += pItemInfo->LimitGender;
		if( pItemInfo->LimitLevel > 0 )
			pAvatarOption->TargetPhyDefDown += pItemInfo->LimitLevel;
		if( pItemInfo->LimitGenGol > 0 )
			pAvatarOption->TargetAttrDefDown += pItemInfo->LimitGenGol;
		if( pItemInfo->LimitMinChub > 0 )
			pAvatarOption->TargetAtkDown += pItemInfo->LimitMinChub;
		if( pItemInfo->LimitSimMek )
			pAvatarOption->RecoverRate += pItemInfo->LimitSimMek;
		if( pItemInfo->ItemGrade )
			pAvatarOption->KyunggongSpeed += pItemInfo->ItemGrade;
        if( pItemInfo->RangeType )
			pAvatarOption->MussangCharge += pItemInfo->RangeType;
		if( pItemInfo->EquipKind == 1 )
			pAvatarOption->NaeruykspendbyKG = 1;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if( pItemInfo->NaeRyukRecover > 0 )
			pAvatarOption->Decisive += pItemInfo->NaeRyukRecover;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		if( pItemInfo->RangeAttackMin > 0 )
			pAvatarOption->ShieldRecoverRate += pItemInfo->RangeAttackMin;
		if( pItemInfo->RangeAttackMax > 0 )
			pAvatarOption->MussangDamage += pItemInfo->RangeAttackMax;
	}

#define SHOPITEM_COS_CAT_HAT		56019
#define SHOPITEM_COS_CAT_DRESS		56020
#define SHOPITEM_COS_WEDDING_MAN	56021
#define SHOPITEM_COS_WEDDING_WOMAN	56022

	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
	{
		if( pAvatar[eAvatar_Hat] == SHOPITEM_COS_CAT_HAT )
		{
			pAvatarOption->Life += 50;
			pAvatarOption->Shield += 50;
			pAvatarOption->Naeruyk += 50;
			pAvatarOption->RecoverRate += 10;
			pAvatarOption->NaeruykspendbyKG = 1;
		}

		if( pAvatar[eAvatar_Dress] == SHOPITEM_COS_CAT_DRESS )
		{
			pAvatarOption->Life += 50;
			pAvatarOption->Shield += 50;
			pAvatarOption->Naeruyk += 50;
			pAvatarOption->KyunggongSpeed += 50;
			pAvatarOption->TargetPhyDefDown += 5;
			pAvatarOption->TargetAttrDefDown += 5;
		}

		if( pAvatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_MAN ||
			pAvatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_MAN )
		{
			pAvatarOption->Life += 100;
			pAvatarOption->Shield += 100;
			pAvatarOption->Naeruyk += 100;
			pAvatarOption->RecoverRate += 10;
			pAvatarOption->NaeruykspendbyKG = 1;
			pAvatarOption->KyunggongSpeed += 50;
			pAvatarOption->TargetPhyDefDown += 5;
			pAvatarOption->TargetAttrDefDown += 5;
		}
	}

	if( bCalcStats )
		m_pPlayer->CalcState();
}


void CShopItemManager::DiscardAvatarItem( WORD ItemIdx, WORD ItemPos )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return ;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return;

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( i==pAvatarEquip->Position && pAvatar[i] )
		{
			pAvatar[i] = 0;
			for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
			{
				if( !pAvatarEquip->Item[n] )
					pAvatar[n] = 1;
			}
						
			ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
		}
	}

	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_DISCARD;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	CalcAvatarOption();
}


void CShopItemManager::CalcPlusTime( DWORD dwEventIdx, DWORD dwType )
{	
	ITEM_INFO* pItem = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;

	m_UsingItemTable.SetPositionHead();

	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItem = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItem )		continue;

		if( pItem->ItemKind == eSHOP_ITEM_CHARM ||
			pItem->ItemKind == eSHOP_ITEM_MARRYCHARM )
		{
			switch( dwType )
			{
			case 0:
				{
					if( pShopItem->ShopItem.Remaintime )
					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
						CalcShopItemOption( pItem->ItemIdx, FALSE );
				}
				break;
			case MP_CHEAT_PLUSTIME_ON:
				{
					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
					{
						CalcShopItemOption( pItem->ItemIdx, FALSE );
						return;
					}
				}
				break;
			case MP_CHEAT_PLUSTIME_OFF:
				{
					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
					{
						pShopItem->LastCheckTime = gCurTime;
						CalcShopItemOption( pItem->ItemIdx, TRUE );
						return;
					}
				}
				break;
			case MP_CHEAT_PLUSTIME_ALLOFF:
				{
					if( pShopItem->ShopItem.Remaintime )
					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
					{
						pShopItem->LastCheckTime = gCurTime;
						CalcShopItemOption( pItem->ItemIdx, TRUE );
					}
				}
				break;
			}
		}
	}	
}


void CShopItemManager::AddDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;

	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_WoigongDamage )			//[外功伤害][By:十里坡剑神][QQ:112582793][2018/2/11]
			m_DupCharm |= eDontDupUse_WoigongDamage;
		if( pDupOption->Param & eDontDupUse_NaegongDamage )			//[内功伤害][By:十里坡剑神][QQ:112582793][2018/2/11]
			m_DupCharm |= eDontDupUse_NaegongDamage;	
		if( pDupOption->Param & eDontDupUse_Exppoint )				//[经验加成][By:十里坡剑神][QQ:112582793][2018/2/11]
			m_DupCharm |= eDontDupUse_Exppoint;	
		if( pDupOption->Param & eDontDupUse_Reinforce )				//[强化几率][By:十里坡剑神][QQ:112582793][2018/2/11]
			m_DupCharm |= eDontDupUse_Reinforce;	
		if( pDupOption->Param & eDontDupUse_Kyunggong )				//[轻工速度][By:十里坡剑神][QQ:112582793][2018/2/11]
			m_DupCharm |= eDontDupUse_Kyunggong;

		if( pDupOption->Param & eDontDupUse_Ghost )					
			m_DupCharm |= eDontDupUse_Ghost;			
		if( pDupOption->Param & eDontDupUse_Woigong )			
			m_DupCharm |= eDontDupUse_Woigong;			
		if( pDupOption->Param & eDontDupUse_Naegong )				
			m_DupCharm |= eDontDupUse_Naegong;			
		if( pDupOption->Param & eDontDupUse_Hunter )			
			m_DupCharm |= eDontDupUse_Hunter;
		if( pDupOption->Param & eDontDupUse_ExpDay )			
			m_DupCharm |= eDontDupUse_ExpDay;
	}
	
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_Life )			
			m_DupHerb |= eDontDupUse_Life;
		if( pDupOption->Param & eDontDupUse_Shield )		
			m_DupHerb |= eDontDupUse_Shield;
		if( pDupOption->Param & eDontDupUse_Naeruyk )			
			m_DupHerb |= eDontDupUse_Naeruyk;
		if( pDupOption->Param & eDontDupUse_GreateLife )		
			m_DupHerb |= eDontDupUse_GreateLife;
		if( pDupOption->Param & eDontDupUse_GreateShield )		
			m_DupHerb |= eDontDupUse_GreateShield;
		if( pDupOption->Param & eDontDupUse_GreateNaeruyk )		
			m_DupHerb |= eDontDupUse_GreateNaeruyk;
		if( pDupOption->Param & eDontDupUse_EventSatang )		
			m_DupHerb |= eDontDupUse_EventSatang;
		if( pDupOption->Param & eDontDupUse_Doll )		
			m_DupHerb |= eDontDupUse_Doll;

		
	}

	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_MemoryMove )		
			m_DupIncantation |= eDontDupUse_MemoryMove;			
		if( pDupOption->Param & eDontDupUse_ProtectAll )	
			m_DupIncantation |= eDontDupUse_ProtectAll;
		if( pDupOption->Param & eDontDupUse_LevelCancel50 )		
			m_DupIncantation |= eDontDupUse_LevelCancel50;
		if( pDupOption->Param & eDontDupUse_LevelCancel70 )	
			m_DupIncantation |= eDontDupUse_LevelCancel70;
		if( pDupOption->Param & eDontDupUse_LevelCancel90 )		
			m_DupIncantation |= eDontDupUse_LevelCancel90;
		if( pDupOption->Param & eDontDupUse_ShowPyoguk )	
			m_DupIncantation |= eDontDupUse_ShowPyoguk;
		if( pDupOption->Param & eDontDupUse_Chase )				
			m_DupIncantation |= eDontDupUse_Chase;
		if( pDupOption->Param & eDontDupUse_TownMove )			
			m_DupIncantation |= eDontDupUse_TownMove;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_StreeStall )		
		{
			m_DupSundries |= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 1;
		}
	}
	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDupPetEquip_PomanRing )		
		{
			m_DupPetEquip |= eDupPetEquip_PomanRing;
		}
	}
}


void CShopItemManager::DeleteDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;

	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&

			(m_DupCharm & eDontDupUse_WoigongDamage) )			
			m_DupCharm ^= eDontDupUse_WoigongDamage;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&
			(m_DupCharm & eDontDupUse_NaegongDamage) )			
			m_DupCharm ^= eDontDupUse_NaegongDamage;	
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&
			(m_DupCharm & eDontDupUse_Exppoint) )					
			m_DupCharm ^= eDontDupUse_Exppoint;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&
			(m_DupCharm & eDontDupUse_Reinforce) )				
			m_DupCharm ^= eDontDupUse_Reinforce;
		if( (pDupOption->Param & eDontDupUse_Kyunggong) &&
			(m_DupCharm & eDontDupUse_Kyunggong) )					
			m_DupCharm ^= eDontDupUse_Kyunggong;
		if( pDupOption->Param & eDontDupUse_Ghost )				
			m_DupCharm ^= eDontDupUse_Ghost;			
		if( pDupOption->Param & eDontDupUse_Woigong )			
			m_DupCharm ^= eDontDupUse_Woigong;			
		if( pDupOption->Param & eDontDupUse_Naegong )			
			m_DupCharm ^= eDontDupUse_Naegong;			
		if( pDupOption->Param & eDontDupUse_Hunter )			
			m_DupCharm ^= eDontDupUse_Hunter;
		if( pDupOption->Param & eDontDupUse_ExpDay )			
			m_DupCharm ^= eDontDupUse_ExpDay;
	}
	
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_Life) &&
			(m_DupHerb & eDontDupUse_Life) )			
			m_DupHerb ^= eDontDupUse_Life;
		if( (pDupOption->Param & eDontDupUse_Shield) &&
			(m_DupHerb & eDontDupUse_Shield) )			
			m_DupHerb ^= eDontDupUse_Shield;
		if( (pDupOption->Param & eDontDupUse_Naeruyk) &&
			(m_DupHerb & eDontDupUse_Naeruyk) )			
			m_DupHerb ^= eDontDupUse_Naeruyk;
		if( (pDupOption->Param & eDontDupUse_GreateLife) &&
			(m_DupHerb & eDontDupUse_GreateLife) )				
			m_DupHerb ^= eDontDupUse_GreateLife;
		if( (pDupOption->Param & eDontDupUse_GreateShield) &&
			(m_DupHerb & eDontDupUse_GreateShield) )				
			m_DupHerb ^= eDontDupUse_GreateShield;
		if( (pDupOption->Param & eDontDupUse_GreateNaeruyk) &&
			(m_DupHerb & eDontDupUse_GreateNaeruyk) )				
			m_DupHerb ^= eDontDupUse_GreateNaeruyk;
		if( (pDupOption->Param & eDontDupUse_EventSatang) &&
			(m_DupHerb & eDontDupUse_EventSatang) )				
			m_DupHerb ^= eDontDupUse_EventSatang;
		if( (pDupOption->Param & eDontDupUse_Doll) &&
			(m_DupHerb & eDontDupUse_Doll) )				
			m_DupHerb ^= eDontDupUse_Doll;
	}

	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) && 
			(m_DupIncantation & eDontDupUse_MemoryMove) )					
			m_DupIncantation ^= eDontDupUse_MemoryMove;			
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&
			(m_DupIncantation & eDontDupUse_ProtectAll) )					
			m_DupIncantation ^= eDontDupUse_ProtectAll;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&
			(m_DupSundries & eDontDupUse_StreeStall) )					
		{
			m_DupSundries ^= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 0;
		}
	}
	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDupPetEquip_PomanRing) &&
			(m_DupPetEquip & eDupPetEquip_PomanRing) )					
		{
			m_DupPetEquip ^= eDupPetEquip_PomanRing;			
		}
	}
}


BOOL CShopItemManager::IsDupAble( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return FALSE;

	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&			
			(m_DupCharm & eDontDupUse_WoigongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&			
			(m_DupCharm & eDontDupUse_NaegongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&				
			(m_DupCharm & eDontDupUse_Exppoint) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&				
			(m_DupCharm & eDontDupUse_Reinforce) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Kyunggong) &&			
			(m_DupCharm & eDontDupUse_Kyunggong) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Ghost) &&			
			(m_DupCharm & eDontDupUse_Ghost) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Woigong) &&			
			(m_DupCharm & eDontDupUse_Woigong) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Naegong) &&			
			(m_DupCharm & eDontDupUse_Naegong) )
			return FALSE;		
		if( (pDupOption->Param & eDontDupUse_Hunter) && 				
			(m_DupCharm & eDontDupUse_Hunter) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ExpDay) &&		
			(m_DupCharm & eDontDupUse_ExpDay) )
			return FALSE;
	}
	
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_Life) &&		
			(m_DupHerb & eDontDupUse_Life) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Shield) &&		
			(m_DupHerb & eDontDupUse_Shield) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Naeruyk) &&		
			(m_DupHerb & eDontDupUse_Naeruyk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateLife) &&		
			(m_DupHerb & eDontDupUse_GreateLife) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateShield) &&			
			(m_DupHerb & eDontDupUse_GreateShield) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateNaeruyk) &&			
			(m_DupHerb & eDontDupUse_GreateNaeruyk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_EventSatang) &&		
			(m_DupHerb & eDontDupUse_EventSatang) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Doll) &&		
			(m_DupHerb & eDontDupUse_Doll) )
			return FALSE;
	}

	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) &&		
			(m_DupIncantation & eDontDupUse_MemoryMove) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&		
			(m_DupIncantation & eDontDupUse_ProtectAll) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel50) &&		
			(m_DupIncantation & eDontDupUse_LevelCancel50) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel70) &&	
			(m_DupIncantation & eDontDupUse_LevelCancel70) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel90) &&		
			(m_DupIncantation & eDontDupUse_LevelCancel90) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ShowPyoguk) &&			
			(m_DupIncantation & eDontDupUse_ShowPyoguk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Chase) &&		
			(m_DupIncantation & eDontDupUse_Chase) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_TownMove) &&	
			(m_DupIncantation & eDontDupUse_TownMove) )
			return FALSE;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&		
			(m_DupSundries & eDontDupUse_StreeStall) )
			return FALSE;
	}

	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDupPetEquip_PomanRing) &&		
			(m_DupPetEquip & eDupPetEquip_PomanRing) )
			return FALSE;
	}

	return TRUE;
}

DWORD CShopItemManager::PutSkinSelectItem(MSG_DWORD2* pMsg)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
		 
	DWORD dwSkinIndex = pMsg->dwData1;
	DWORD dwSkinKind = pMsg->dwData2;

	if(dwSkinIndex < 1)
		return eSkinResult_Fail;

	switch(dwSkinKind)
	{
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
		{
			pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(dwSkinIndex);
		}
		break;
	case eSHOP_ITEM_COSTUME_SKIN:
		{
			pSkinInfo = GAMERESRCMNGR->GetCostumeSkinList(dwSkinIndex);
		}
		break;
	}
	
	if(pSkinInfo == NULL)
		return eSkinResult_Fail;

	if(dwSkinKind == eSHOP_ITEM_NOMALCLOTHES_SKIN)
	{
		if(m_pPlayer->GetLevel() < pSkinInfo->dwLimitLevel)
			return eSkinResult_LevelFail;
	}	

	if(m_pPlayer->GetSkinDelayResult() == TRUE)
		return eSkinResult_DelayFail;

	for(int i = 0 ; i < SKINITEM_LIST_MAX ; i++)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pSkinInfo->wEquipItem[i] );
		if(pItemInfo == NULL)
			continue;

		DWORD dwSkinType = pItemInfo->Part3DType;
		DWORD dwEquipIndex = 0;

        if( (dwSkinType == eSkinItem_Hat) || (dwSkinType == 6) ) 
			dwEquipIndex = eSkinItem_Hat;

		else if(dwSkinType == eSkinItem_Mask)
			dwEquipIndex = eSkinItem_Mask;
		else if(dwSkinType == eSkinItem_Dress)
			dwEquipIndex = eSkinItem_Dress;
		else if(dwSkinType == eSkinItem_Shoes)
			dwEquipIndex = eSkinItem_Shoes;

		m_pPlayer->GetShopItemStats()->wSkinItem[dwEquipIndex] = pItemInfo->ItemIdx;		

		if( (dwSkinType == eSkinItem_Dress) && (dwSkinKind == eSHOP_ITEM_COSTUME_SKIN) )
			m_pPlayer->GetShopItemStats()->wSkinItem[eSkinItem_Shoes] = 0;
	}

	return eSkinResult_Success;
}

void CShopItemManager::DiscardSkinItem(DWORD dwItemIndex)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( dwItemIndex );
	if(!pItemInfo)		return;

	RemoveEquipSkin(pItemInfo->ItemKind);
	CharacterSkinInfoUpdate(m_pPlayer);

	SEND_SKIN_INFO msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_SKINITEM_DISCARD_ACK;
	msg.dwObjectID = m_pPlayer->GetID();
	memcpy( &msg.wSkinItem, m_pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
}

void CShopItemManager::RemoveEquipSkin(DWORD dwSkinKind)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* pSkinListTable = NULL;
	WORD* wSkinItem = m_pPlayer->GetShopItemStats()->wSkinItem;
		
	switch(dwSkinKind)
	{
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
		{
			pSkinListTable = GAMERESRCMNGR->GetNomalClothesSkinTable();			
		}
		break;
	case eSHOP_ITEM_COSTUME_SKIN:
		{	
			pSkinListTable = GAMERESRCMNGR->GetCostumeSkinTable();
		}
		break;
	}

	if(pSkinListTable == NULL)
		return;

	DWORD dwSkinMaxNum = pSkinListTable->GetDataNum();
	pSkinListTable->SetPositionHead();	
	for(int nCount = 0 ; nCount < dwSkinMaxNum ; nCount++)
	{
		pSkinInfo = NULL;
		pSkinInfo = pSkinListTable->GetData();
		if(pSkinInfo == NULL)
			continue;

		for(int i = 0 ; i < eSkinItem_Max ; i++)
		{			
			if(wSkinItem[i] == 0)
				continue;

			for(int j = 0 ; j < SKINITEM_LIST_MAX ; j++)
			{
				if(wSkinItem[i] == pSkinInfo->wEquipItem[j])
				{
					wSkinItem[i] = 0;
				}
			}		
		}		
	}
}

void CShopItemManager::AddUsingShopItem(SHOPITEMWITHTIME* pShopItem, WORD dwItemIndex)
{
	SHOPITEMWITHTIME* pAddShopItem = m_UsingItemPool->Alloc();
	memcpy(pAddShopItem, pShopItem, sizeof(SHOPITEMWITHTIME));

	m_UsingItemTable.Add(pAddShopItem, dwItemIndex);
}

int CShopItemManager::UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem )
{
	if( !pItemBase )
		return eItemUseErr_Err;

	if( EVENTMAPMGR->IsEventMap() )
		return eItemUseErr_Err;

	DWORD dwCheckBit = eBossMap | eSiegeWarMap | eQuestRoom | eEventMap | eSurvivalMap;
	if( pItemBase->wIconIdx == eIncantation_MemoryStone &&
		g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
		return eItemUseErr_Err;


	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UseBaseInfo.ShopItemIdx);
	if(!pItemInfo)		return eItemUseErr_Err;

	if(pItemInfo->NaeRyukRecoverRate > m_pPlayer->GetLevel())
	{
		return eItemUseErr_Err;
	}


	if( g_pServerSystem->GetMap()->IsMapKind(eTournament) )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone ||
			(pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel) )
			return eItemUseErr_Err;
	}

	if( m_pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone )
			return eItemUseErr_Err;

		if( m_pPlayer->GetBattleTeam() == 2 ) 
		{
			if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel )
				return eItemUseErr_Err;
		}
	}

	SHOPITEMWITHTIME* pUsingShopItem = NULL;
	pUsingShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);

	if( pUsingShopItem )
	{
		if( pItemInfo->SellPrice && pItemInfo->ItemType == 10 )
		{

			if( (pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
				pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT||
				pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM )
				&& pItemInfo->MeleeAttackMin)
			{
				if( pUsingShopItem->ShopItem.Remaintime )
					return eItemUseErr_AlreadyUse;
				else
				{
					ShopItemDeleteToDB( m_pPlayer->GetID(), pUsingShopItem->ShopItem.ItemBase.dwDBIdx );
					m_UsingItemTable.Remove( pUsingShopItem->ShopItem.ItemBase.wIconIdx );
					m_UsingItemPool->Free( pUsingShopItem );
					pUsingShopItem = NULL;
				}				
			}
//#ifdef _JAPAN_LOCAL_
//			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
//				return eItemUseErr_AlreadyUse;
//			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= EXTRA_PYOGUK_SLOT )
//				return eItemUseErr_AlreadyUse;
//			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
//				return eItemUseErr_AlreadyUse;
//			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
//				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
//				pItemInfo->ItemIdx != eIncantation_MugongExtend)
//				return eItemUseErr_AlreadyUse;
//		}
//#elif defined _HK_LOCAL_
//			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
//				m_pPlayer->GetExtraInvenSlot() >= 2 )
//				return eItemUseErr_AlreadyUse;
//			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
//				m_pPlayer->GetExtraPyogukSlot() >= 3 )
//				return eItemUseErr_AlreadyUse;
//			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
//				m_pPlayer->GetExtraMugongSlot() >= 2)
//				return eItemUseErr_AlreadyUse;
//			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
//				m_pPlayer->GetExtraCharacterSlot() >= 3 )
//				return eItemUseErr_UseFull;
//			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend && pItemInfo->ItemIdx != eIncantation_InvenExtend2 &&
//				pItemInfo->ItemIdx != eIncantation_PyogukExtend && pItemInfo->ItemIdx != eIncantation_PyogukExtend2 &&
//				pItemInfo->ItemIdx != eIncantation_MugongExtend && pItemInfo->ItemIdx != eIncantation_MugongExtend2 )
//				return eItemUseErr_AlreadyUse;
//		}
//#elif defined _TL_LOCAL_
//			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
//				return eItemUseErr_UseFull;
//			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= 3 )
//				return eItemUseErr_UseFull;
//			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
//				return eItemUseErr_UseFull;
//			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot && m_pPlayer->GetExtraCharacterSlot() >= 3 )
//				return eItemUseErr_UseFull;
//			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
//				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
//				pItemInfo->ItemIdx != eIncantation_MugongExtend &&
//				pItemInfo->ItemIdx != eIncantation_CharacterSlot )
//				return eItemUseErr_AlreadyUse;
//		}
//
//#else

			else
				return eItemUseErr_AlreadyUse;
		}
//#endif		
	}
//#ifdef _HK_LOCAL_
//	else
//	{
//		if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
//			m_pPlayer->GetExtraInvenSlot() >= 2 )
//			return eItemUseErr_AlreadyUse;
//		else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
//			m_pPlayer->GetExtraPyogukSlot() >= 3 )
//			return eItemUseErr_AlreadyUse;
//		else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
//			m_pPlayer->GetExtraMugongSlot() >= 2)
//			return eItemUseErr_AlreadyUse;
//		else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
//			m_pPlayer->GetExtraCharacterSlot() >= 3 )
//			return eItemUseErr_UseFull;
//	}
//#endif
	if( pItemBase->wIconIdx != 55353 && pItemBase->wIconIdx != 57504 &&
		pItemBase->wIconIdx != 57505 )
	{
		if( IsDupAble( pItemInfo ) == FALSE )
			return eItemUseErr_DontDupUse;	
	}

	SHOPITEMWITHTIME AddShopItem;
	AddShopItem.ShopItem.ItemBase = *pItemBase;
	AddShopItem.ShopItem.Param = pItemInfo->SellPrice;

	if( AddShopItem.ShopItem.Param && pItemInfo->ItemType == 10 )
	{
		SYSTEMTIME systime;
		stTIME startime, usetime, endtime;
		GetLocalTime(&systime);
		startime.SetTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
		AddShopItem.ShopItem.BeginTime = startime;

		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			endtime = startime;
			
			if( pItemInfo->Rarity >= 525600 )
			{
				DWORD day = pItemInfo->Rarity/(24*60) - 365;
				usetime.SetTime(1, 0, day, 0, 0, 0);
			}
			else
			{
				DWORD day = pItemInfo->Rarity/(24*60);
				DWORD hour = (pItemInfo->Rarity%(24*60))/60;
				DWORD minute = (pItemInfo->Rarity%(24*60))%60;
                usetime.SetTime(0, 0, day, hour, minute, 0);
			}

			endtime += usetime;	

			AddShopItem.ShopItem.Remaintime = endtime.value;
			AddShopItem.LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime||
			pItemInfo->SellPrice == eShopItemuseParam_FBItem)//增加副本入场券使用计时	by:十里坡剑神	QQ:112582793 
		{
			QSTATETYPE remaintime = 0;
			remaintime = pItemInfo->Rarity*60000;
			AddShopItem.ShopItem.Remaintime = remaintime;
			AddShopItem.LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
		{
			AddShopItem.ShopItem.Remaintime = 0;
			AddShopItem.LastCheckTime = 0;
		}
	}

	if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->GenGol > 0 )
			msg.dwData2 = eEffect_ShopItem_Life;
		else if( pItemInfo->MinChub > 0 )
			msg.dwData2 = eEffect_ShopItem_Shield;
		else if( pItemInfo->CheRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_NaeRuyk;
		else if( pItemInfo->Life > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxLife;
		else if( pItemInfo->Shield > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxShield;
		else if( pItemInfo->NaeRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxNaeRuyk;
		else
			msg.dwData2 = 0;

		if( msg.dwData2 )
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	


		if( pItemInfo->SellPrice == 0 )
		{
			if( pItemInfo->GenGol > 0 )
				m_pPlayer->AddLife( pItemInfo->GenGol, NULL );
			if( pItemInfo->MinChub > 0 )
				m_pPlayer->AddShield( pItemInfo->MinChub );
			if( pItemInfo->CheRyuk > 0 )
				m_pPlayer->AddNaeRyuk( pItemInfo->CheRyuk, NULL );
		}
		else
		{
			CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );			
			AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		if( pItemInfo->ItemType == 10 )
		{
			if( pItemInfo->LimitJob )
			{
				if( pItemInfo->LimitGender == 0 && m_pPlayer->GetLevel() > 50 )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 1 && (m_pPlayer->GetLevel() < 51 || m_pPlayer->GetLevel() > 70 ) )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 2 && (m_pPlayer->GetLevel() < 71 || m_pPlayer->GetLevel() > 90 ) )
					return eItemUseErr_Err;
			}

			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
				return eItemUseErr_Err;			

			if( pItemInfo->CheRyuk || pItemInfo->LimitJob )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->CheRyuk, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
//#ifdef _JAPAN_LOCAL_
//			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
//			{
//				if( m_pPlayer->GetExtraInvenSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
//			{
//				if( m_pPlayer->GetExtraPyogukSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
//			{
//				if( m_pPlayer->GetExtraMugongSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
//			{
//				if( m_pPlayer->GetExtraCharacterSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//					m_pPlayer->SetExtraCharacterSlot( 1 );
//				}
//			}
//#elif defined _HK_LOCAL_
//			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2))
//			{
//				if( m_pPlayer->GetExtraInvenSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) )
//			{
//				if( m_pPlayer->GetExtraPyogukSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) )
//			{
//				if( m_pPlayer->GetExtraMugongSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) )
//			{
//				if( m_pPlayer->GetExtraCharacterSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//					m_pPlayer->SetExtraCharacterSlot( 1 );
//				}
//			}
//#elif defined _TL_LOCAL_
//			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
//			{
//				if( m_pPlayer->GetExtraInvenSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
//			{
//				if( m_pPlayer->GetExtraPyogukSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
//			{
//				if( m_pPlayer->GetExtraMugongSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
//					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//				}
//				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
//			}
//			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
//			{
//				if( m_pPlayer->GetExtraCharacterSlot() )
//				{
//					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot()+1;
//					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
//					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
//				}
//				else
//				{
//					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
//					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
//					m_pPlayer->SetExtraCharacterSlot( 1 );
//				}
//			}
//#endif
			else if( pItemInfo->GenGol )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_StatePoint );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, m_pPlayer->GetShopItemStats()->StatePoint );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->GenGol, AddShopItem.ShopItem.BeginTime.value, 0 );
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					AddUsingShopItem(&AddShopItem, eIncantation_StatePoint);								
				}

			}
			else if( pItemInfo->Life )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_SkPointRedist );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, m_pPlayer->GetShopItemStats()->SkillPoint );
				}
				else
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, AddShopItem.ShopItem.ItemBase.dwDBIdx,
						m_pPlayer->GetShopItemStats()->SkillPoint, AddShopItem.ShopItem.BeginTime.value, m_pPlayer->GetShopItemStats()->UseSkillPoint );					
					AddUsingShopItem(&AddShopItem, eIncantation_SkPointRedist);			
				}
			}
			else if( pItemInfo->ItemIdx == eIncantation_MixUp )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
#define LEVEL_70		70
			else if( pItemInfo->ItemIdx == eIncantation_StatusPointReset_71 ||
					 pItemInfo->ItemIdx == eIncantation_StatusPointReset_1_50 ||
					 pItemInfo->ItemIdx == eIncantation_StatusPointReset_1_99 )
			{
				if( m_pPlayer->GetShopItemStats()->UseStatePoint > 0 )
					return eItemUseErr_Err;

				int nMaxLevel = m_pPlayer->GetMaxLevel();

				if( nMaxLevel < pItemInfo->RangeType || nMaxLevel > pItemInfo->EquipKind )
					return eItemUseErr_Err;
			
				WORD BasicStatus = m_pPlayer->GetBasicPoint();
				WORD wLevel = RESTDMGR->GetRestSetLevel(m_pPlayer->GetLevel());
				
				//if(m_pPlayer->GetReSet()!=0)
					wLevel = wLevel*m_pPlayer->GetReSet();

				m_pPlayer->SetGenGol(12);
				m_pPlayer->SetMinChub(12);
				m_pPlayer->SetCheRyuk(BasicStatus);
				m_pPlayer->SetSimMek(12);

				DWORD dwMaxLevel = m_pPlayer->GetMaxLevel();
				int nLevelStatus = 0;
				BYTE bExpFlag = m_pPlayer->GetExpFlag();

				if( LEVEL_70 < dwMaxLevel )
				{
					nLevelStatus = ( (LEVEL_70 - 1) * 6 ) + ( (dwMaxLevel - LEVEL_70) * 8 ); //[属性还原卷轴附加转生属性点!][By:十里坡剑神][QQ:112582793][2017/12/8]
				}
				else
				{
					nLevelStatus = (dwMaxLevel - 1) * 6; //[属性还原卷轴附加转生属性点!][By:十里坡剑神][QQ:112582793][2017/12/8]
				}
				if(m_pPlayer->GetLevel()>1)
				{
					for( int i = 0; i < 4; i++ )
					{
						if( CheckBit(bExpFlag, i) )
							++nLevelStatus;
					}
				}
				m_pPlayer->SetAddPoint(FALSE);
				m_pPlayer->SetAddPointFlag(0);
				m_pPlayer->SetPlayerLevelUpPoint(nLevelStatus+wLevel);
				CharacterHeroInfoUpdate(m_pPlayer);
				//CharacterUpdateResetStatusPoint(m_pPlayer->GetID(), nLevelStatus+wLevel);
				m_pPlayer->SetAddPoint(TRUE);
			}
			else
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||	pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM ||
		pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES|| pItemInfo->ItemKind== eSHOP_ITEM_GETIN_ITEM||//副本卷、成长符、同心符 使用增加	by:十里坡剑神	QQ:112582793
		pItemInfo->ItemKind == eSHOP_ITEM_GROWITEM || pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM 
		||pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT
		)
	{
		if( ( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||	pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM ||
			pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES|| pItemInfo->ItemKind== eSHOP_ITEM_GETIN_ITEM||//副本卷、成长符、同心符 使用增加	by:十里坡剑神	QQ:112582793
			pItemInfo->ItemKind == eSHOP_ITEM_GROWITEM || pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM 
			||pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT
			) && pItemInfo->EquipKind )
		{
			if( m_pPlayer->GetMaxLevel() < pItemInfo->EquipKind )
				return eItemUseErr_Err;
		}

		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pItemInfo->SimMek )
		{
			int err = CHANGEITEMMGR->UseChangeItemFromShopItem( m_pPlayer, pItemInfo->SimMek );
			if(  err == 2 )
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
				msg.wData = NOT_SPACE;
				m_pPlayer->SendMsg(&msg, sizeof(msg));				
				return eItemUseErr_Err;
			}
			else if( err != EI_TRUE )
				return eItemUseErr_Err;
		}

		if( pItemInfo->ItemType == 10 )
			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
			{
				return eItemUseErr_Err;
			}

			MSG_DWORD2	msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
			msg.dwData1 = m_pPlayer->GetID();

			if( pItemInfo->ItemIdx == eSundries_Boom_Butterfly )
			{
				msg.dwData2 = eEffect_ShopItem_Boom1;
				PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
			}
			else if( pItemInfo->ItemIdx == eSundries_Boom_Star )
			{
				msg.dwData2 = eEffect_ShopItem_Boom2;
				PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
			}

			CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();

			if( pItemInfo->SellPrice && pItemInfo->ItemType==10 )
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION  ||pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME)
	{//[图片称号物品穿戴类型增加][By:十里坡剑神][QQ:112582793][2017/11/29]
		CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
		AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			

		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
		msg.dwData1 = m_pPlayer->GetID();
		msg.dwData2 = pItemInfo->ItemIdx;

		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pPlayer, &msg, sizeof(msg));
		goto LogNotUse;
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET )
	{
		if(pItemInfo->LimitLevel > m_pPlayer->GetMaxLevel())
		{
			return eItemUseErr_Err;
		}
		m_pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
		goto LogNotUse;
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
	{
		if(!m_pPlayer->GetPetManager()->GetCurSummonPet())
		{
			return eItemUseErr_Err;
		}

		m_pPlayer->GetPetManager()->SetPetEquipOption(pItemInfo->ItemIdx, TRUE);
	}
	if(pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT)
	{//[自动加点卷使用][By:十里坡剑神][QQ:112582793][2018/4/17]
		m_pPlayer->SetAddPoint(TRUE);
	}
	AddDupParam( pItemInfo );

	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, AddShopItem.ShopItem.ItemBase.Position, 0, AddShopItem.ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

LogNotUse:

	memcpy(pShopItem, &AddShopItem.ShopItem, sizeof(SHOPITEMBASE));

	return eItemUseSuccess;
}

BOOL CShopItemManager::UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime )
{
	if( !pItemBase )
		return FALSE;

	if( m_UsingItemTable.GetData(pItemBase->wIconIdx) )
		return FALSE;

	SHOPITEMWITHTIME ShopItem;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if( !pItemInfo )
	{
		if( pItemBase->wIconIdx == eIncantation_SkPointRedist )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->SkillPoint = Param;
			pOption->UseSkillPoint = RemainTime;
		}
		else if( pItemBase->wIconIdx == eIncantation_StatePoint )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->StatePoint = (WORD)Param;
			pOption->UseStatePoint = (WORD)RemainTime;
		}
		else
			return FALSE;
	}
	else if( pItemBase->wIconIdx == eIncantation_StatePoint_30 )
	{
		SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
		pOption->StatePoint = (WORD)RemainTime;
		pOption->UseStatePoint = 30 - (WORD)RemainTime;
		pItemBase->wIconIdx = eIncantation_StatePoint;

		ShopItemAllUseInfoUpdateToDB( pItemBase->dwDBIdx, m_pPlayer->GetID(), eIncantation_StatePoint, pOption->StatePoint, pOption->UseStatePoint );
	}

	ShopItem.ShopItem.ItemBase = *pItemBase;
	ShopItem.ShopItem.Param = Param;
	ShopItem.ShopItem.BeginTime = BeginTime;
	ShopItem.ShopItem.Remaintime = RemainTime;
	ShopItem.LastCheckTime = gCurTime;

	if( pItemInfo && pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		stTIME ctime, rtime;
		ctime.value = GetCurTime();
		rtime.SetTime( ShopItem.ShopItem.Remaintime );
		if( ctime > rtime )
		{
			if( pItemInfo->ItemType == 11 )
			{				
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, ShopItem.ShopItem.ItemBase.Position, ShopItem.ShopItem.ItemBase.wIconIdx, 1 ))
				{
					return FALSE;
				}
				if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{
				}

			}

			ShopItemDeleteToDB( m_pPlayer->GetID(), ShopItem.ShopItem.ItemBase.dwDBIdx );

			LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), ShopItem.ShopItem.BeginTime.value, ShopItem.ShopItem.Remaintime,
				ShopItem.ShopItem.ItemBase.wIconIdx, ShopItem.ShopItem.ItemBase.dwDBIdx, ShopItem.ShopItem.ItemBase.Position, 0, ShopItem.ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

			return FALSE;
		}
	}


	AddUsingShopItem(&ShopItem, ShopItem.ShopItem.ItemBase.wIconIdx);

//#ifdef _JAPAN_LOCAL_
//	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
//		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
//		pItemBase->wIconIdx == eIncantation_InvenExtend ||
//		pItemBase->wIconIdx == eIncantation_CharacterSlot )
//		return	TRUE;
//#elif defined _HK_LOCAL_
//	if( pItemBase->wIconIdx == eIncantation_MugongExtend || pItemBase->wIconIdx == eIncantation_MugongExtend2 ||
//		pItemBase->wIconIdx == eIncantation_PyogukExtend || pItemBase->wIconIdx == eIncantation_PyogukExtend2 ||
//		pItemBase->wIconIdx == eIncantation_InvenExtend || pItemBase->wIconIdx == eIncantation_InvenExtend2 ||
//		pItemBase->wIconIdx == eIncantation_CharacterSlot || pItemBase->wIconIdx == eIncantation_CharacterSlot2 )
//		return	TRUE;
//#elif defined _TL_LOCAL_
//	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
//		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
//		pItemBase->wIconIdx == eIncantation_InvenExtend ||
//		pItemBase->wIconIdx == eIncantation_CharacterSlot )
//		return	TRUE;
//#endif // #ifdef _JAPAN_LOCAL_
	if(pItemInfo!=NULL)
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->CheRyuk )
		{
			CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem.ShopItem.Param);
		}
		else
		{
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin && RemainTime==0 )
				return TRUE;
			else
				CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem.ShopItem.Remaintime);
		}
		if(pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME&&Param==10)
		{//[图片称号设置][By:十里坡剑神][QQ:112582793][2017/11/29]
			m_pPlayer->SetImageName(pItemInfo->wSetItemKind);
		}
		if(pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT)
		{//[自动加点卷使用][By:十里坡剑神][QQ:112582793][2018/4/17]
			m_pPlayer->SetAddPoint(TRUE);
		}
		AddDupParam( pItemInfo );
	}
	return TRUE;
}


CYHHashTable<SHOPITEMWITHTIME> * CShopItemManager::GetUsingItemInfoAll() //[获取玩家增益信息][By:十里坡剑神][QQ:112582793][2017/12/8]
{
	return &m_UsingItemTable;
}
BOOL CShopItemManager::IsUseGrowItem(WORD & wVal)
{
	SHOPITEMWITHTIME * UsingItemInfo = NULL;
	m_UsingItemTable.SetPositionHead();
	while(UsingItemInfo = m_UsingItemTable.GetData() )
	{
		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UsingItemInfo->ShopItem.ItemBase.wIconIdx);
		if(pItemInfo)
		{
			if(pItemInfo->ItemKind == eSHOP_ITEM_GROWITEM )
			{
				wVal += pItemInfo->WeaponType;
				
			}
		}
	}
	if (wVal!=0)
		return TRUE;
	else
		return FALSE;
}
BOOL CShopItemManager::IsCanInPutFBMap()  //副本进入物品是否使用检查	by:十里坡剑神	QQ:112582793
{
	SHOPITEMWITHTIME * UsingItemInfo = NULL;
	m_UsingItemTable.SetPositionHead();
	while(UsingItemInfo =m_UsingItemTable.GetData() )
	{
		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UsingItemInfo->ShopItem.ItemBase.wIconIdx);
		if(pItemInfo)
		{
			if(pItemInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM )
				return TRUE;
		}
	}
	return FALSE;
}
bool CShopItemManager::AddFBItemTime(WORD wItemIdx, DWORD dwTime)
{
	SHOPITEMWITHTIME * UsingItemInfo = NULL;
	UsingItemInfo = m_UsingItemTable.GetData(wItemIdx);
	if (UsingItemInfo == NULL)
		return false;
	dwTime *= 60;
	dwTime *= 60;
	dwTime *= 1000;
	UsingItemInfo->ShopItem.Remaintime += dwTime;
	ShopItemUpdatetimeToDB(m_pPlayer->GetID(), UsingItemInfo->ShopItem.ItemBase.wIconIdx, UsingItemInfo->ShopItem.Remaintime);
}