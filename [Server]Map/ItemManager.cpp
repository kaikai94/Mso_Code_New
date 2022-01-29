// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "ItemManager.h"
#include "ItemSlot.h"
#include "Player.h"
#include "Pet.h"
#include "Purse.h"
#include "ItemContainer.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "KyunggongManager.h"
#include "MugongManager.h"
#include "CharacterCalcManager.h"
#include "GuildManager.h"
#include "AbilityManager.h"
#include "CheckRoutine.h"
#include "MHError.h"
#include "PartyManager.h"
#include "LootingManager.h"
#include "QuestManager.h"
#include "BattleSystem_Server.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "ObjectStateManager.h"
#include "MHMap.h"
#include "ReinforceManager.h"
#include "DissolutionManager.h"
#include "RarenessManager.h"
#include "ChangeItemMgr.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "Network.h"
#include "SiegeWarProfitMgr.h"
#include "WantedManager.h"
#include "SurvivalModeManager.h"
#include "TitanItemManager.h"
#include "BobusangManager.h"
#include "Titan.h"
#include "SkillManager_Server.h"
#include "FortWarManager.h"
#include "LimitItemDropManager.h"
//#ifdef _TL_LOCAL_
//#include "PartyWarMgr.h"
//#endif
#include "ShopItemLimitManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemManager::CItemManager()
{
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
    
	m_MarryGetItemInfo.Initialize(10);
	LoadMarryGetItemInfo();


	m_UnfitItemInfoListForHide.Initialize(100);
	//m_MixItemInfoList.Initialize(MAX_ITEM_NUM);
	m_HiLevelItemMixRateInfoList.Initialize(50);
	m_ReinforceItemInfoList.Initialize(20);
	m_ItemArrayPool.Init(500, 100, "ItemArrayPool" );
	m_ArrayIndexCreator.Init(0xFFFFFFFE, 1);
	m_DealerTable.Initialize(32);
	m_AvatarEquipTable.Initialize(50);
	m_Key = 0;
	m_SetItemOptionList.Initialize(MAX_SETITEM_KIND_NUM); 
	//[箱子物品提示初始化][By:十里坡剑神][QQ:112582793][2017/12/6]
	m_TipItemList.Initialize(200);
	LoadTipItem();
	LoadInheritGrowGoldMoney();//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-23][5:34:54]
//#ifdef _HK_LOCAL_
//	ZeroMemory( m_nItemMixBal, sizeof( m_nItemMixBal ) );
//#endif
}

CItemManager::~CItemManager()
{
	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();
	m_UnfitItemInfoListForHide.RemoveAll();



	DealerData* pDealer = NULL;
	m_DealerTable.SetPositionHead();
	while(pDealer = m_DealerTable.GetData())
	{
		POS pos = pDealer->m_DealItemList.GetFirstPos();
		while( pos )
		{
			DealerItem* pDI = pDealer->m_DealItemList.GetNextPos( pos );
			if( pDI ) delete pDI;
		}

		pDealer->m_DealItemList.DeleteAll();
		delete pDealer;
	}
	m_DealerTable.RemoveAll();
	
	/*ITEM_MIX_INFO * pItemInfo = NULL;
	m_MixItemInfoList.SetPositionHead();
	while(pItemInfo = m_MixItemInfoList.GetData())
	{
		if(pItemInfo->psResultItemInfo)
		{
			for(int i = 0 ; i < pItemInfo->wResultItemNum ; ++i)
			{
				if(pItemInfo->psResultItemInfo[i].wResItemIdx != 0)
				{
					if(pItemInfo->psResultItemInfo[i].psMaterialItemInfo)
					{
						delete [] pItemInfo->psResultItemInfo[i].psMaterialItemInfo;
						pItemInfo->psResultItemInfo[i].psMaterialItemInfo = NULL;
					}
				}
			}
			delete [] pItemInfo->psResultItemInfo;
			pItemInfo->psResultItemInfo = NULL;
		}
		delete pItemInfo;
		pItemInfo = NULL;
	}
	m_MixItemInfoList.RemoveAll();*/
	m_MixItemInfoList.clear();

	AVATARITEM* pItem = NULL;
	m_AvatarEquipTable.SetPositionHead();
	while(pItem = m_AvatarEquipTable.GetData())
	{
		delete pItem;
		pItem=NULL;
	}
	m_AvatarEquipTable.RemoveAll();	

	m_ItemArrayPool.Release();

	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		delete pSetItemOption;
		pSetItemOption=NULL;
	}
	m_SetItemOptionList.RemoveAll();

	MARRYGETITEM_INFO* MarryItemInfo = NULL;
	m_MarryGetItemInfo.SetPositionHead();
	while(MarryItemInfo = m_MarryGetItemInfo.GetData())
	{
		delete MarryItemInfo;
		MarryItemInfo=NULL;
	}

	m_MarryGetItemInfo.RemoveAll();

	m_MixItemInfo.clear();
	m_ShopItemType.clear();
	m_ShopItemInfo.clear();
}
ITEMOBTAINARRAYINFO *	CItemManager::Alloc(CPlayer * pPlayer, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx,int BuyType)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = m_ItemArrayPool.Alloc();
	pArrayInfo->ItemArray.Init( c, p, dwObjectID,BuyType );
	pArrayInfo->ItemArray.wObtainCount = ObtainNum;
	pArrayInfo->wObtainArrayID = m_ArrayIndexCreator.GetIndex();//[此ID值不能超过65535][By:十里坡剑神][QQ:112582793][2018/4/15]
	pArrayInfo->CallBack = CallBackFunc;
	pArrayInfo->CallBackEx = CallBackFuncEx;
	pArrayInfo->dwFurnisherIdx = dwFurnisherIdx;
	pArrayInfo->wType = wType;
	pPlayer->AddArray(pArrayInfo);
	return pArrayInfo;
}
void CItemManager::Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info)
{
	info->ItemArray.Clear();
	pPlayer->RemoveArray(info->wObtainArrayID);
	m_ItemArrayPool.Free( info );
}

BOOL CItemManager::IsTitanCallItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsTitanEquipItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind & eTITAN_EQUIPITEM )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsPetSummonItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind == eQUEST_ITEM_PET || pItemInfo->ItemKind == eSHOP_ITEM_PET)
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsRareOptionItem( WORD wItemIdx, DWORD dwRareDBIdx )
{
	if(dwRareDBIdx &&  !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}

BOOL CItemManager::IsOptionItem( WORD wItemIdx, DURTYPE wDurability )
{
	if(wDurability != 0 && !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}
BOOL CItemManager::IsDupItem( WORD wItemIdx )
{
	switch( GetItemKind( wItemIdx ) )
	{
	case eYOUNGYAK_ITEM:
	case eYOUNGYAK_ITEM_PET:
	case eYOUNGYAK_ITEM_UPGRADE_PET:
	case eYOUNGYAK_ITEM_TITAN:
	case eEXTRA_ITEM_JEWEL:
	case eEXTRA_ITEM_MATERIAL:
	case eEXTRA_ITEM_METAL:
	case eEXTRA_ITEM_BOOK:
	case eEXTRA_ITEM_HERB:
	case eEXTRA_ITEM_ETC:
	case eEXTRA_ITEM_USABLE:
	case eSHOP_ITEM_CHARM:
	case eSHOP_ITEM_HERB:
	case eSHOP_ITEM_GETIN_ITEM:
	case eSHOP_ITEM_MARRYCHARM:
	case eSHOP_ITEM_GROWITEM:
	case eSHOP_ITEM_GOLDITEM:
	case eSHOP_ITEM_AUTOADDPOINT:
	case eEXTRA_ITEM_KEY: //[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
	case eCHANGE_ITEM://[箱子类物品叠加][By:十里坡剑神][QQ:112582793][2018/4/18]
		return TRUE;
	case eSHOP_ITEM_SUNDRIES:
		{
			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;
			
			if( pItem->SimMek )	
				return FALSE;
			else if( pItem->CheRyuk )
				return FALSE;
			else if( wItemIdx == eSundries_Shout )
				return FALSE;			
		}
		return TRUE;
	case eSHOP_ITEM_INCANTATION:
		{
			if( wItemIdx == eIncantation_TownMove15 || wItemIdx == eIncantation_MemoryMove15 ||
				wItemIdx == eIncantation_TownMove7 || wItemIdx == eIncantation_MemoryMove7 ||
				wItemIdx == eIncantation_TownMove7_NoTrade || wItemIdx == eIncantation_MemoryMove7_NoTrade ||
				wItemIdx == 55357 || wItemIdx == 55362 || wItemIdx == eIncantation_MemoryMoveExtend || wItemIdx == eIncantation_MemoryMoveExtend7 ||
				wItemIdx == eIncantation_MemoryMove2 || wItemIdx == eIncantation_MemoryMoveExtend30 ||
				wItemIdx == eIncantation_ShowPyoguk || wItemIdx == eIncantation_ChangeName ||
				wItemIdx == eIncantation_ChangeName_Dntrade ||
				wItemIdx == eIncantation_Tracking || wItemIdx == eIncantation_Tracking_Jin ||
				wItemIdx == eIncantation_ChangeJob ||
				wItemIdx == eIncantation_ShowPyoguk7 || wItemIdx == eIncantation_ShowPyoguk7_NoTrade ||
				wItemIdx == eIncantation_Tracking7 ||  wItemIdx == eIncantation_Tracking7_NoTrade ||
				wItemIdx== eIncantation_MugongExtend ||	wItemIdx == eIncantation_PyogukExtend ||
				wItemIdx == eIncantation_InvenExtend ||	wItemIdx == eIncantation_CharacterSlot ||
				wItemIdx== eIncantation_MugongExtend2 ||wItemIdx == eIncantation_PyogukExtend2 ||
				wItemIdx == eIncantation_InvenExtend2 ||wItemIdx == eIncantation_CharacterSlot2 ||
				wItemIdx == eMarryReckon_7 ||wItemIdx == eMarryReckon_15 //[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/11]
				)
				return FALSE;

			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;

			if( pItem->LimitLevel && pItem->SellPrice )
				return FALSE;
		}
		return TRUE;
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
	case eSHOP_ITEM_COSTUME_SKIN:	
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

void CItemManager::SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode )
{
	msg->ECode = ECode;
	pPlayer->SendMsg(msg, msgSize);
}
void CItemManager::SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize)
{
	pPlayer->SendMsg(msg, msgSize);
}

void CItemManager::SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode )
{
	MSG_ITEM_ERROR msg;
	msg.Category = MP_ITEM;
	msg.Protocol = Protocol;
	msg.dwObjectID = pPlayer->GetID();
	msg.ECode = ECode;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

int CItemManager::MoveItem( CPlayer * pPlayer, WORD FromItemIdx, POSTYPE FromPos, WORD ToItemIdx, POSTYPE& ToPos)
{
	CItemSlot * pFromSlot	= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot		= pPlayer->GetSlot(ToPos);

	if( !CheckHackItemMove( pPlayer, pFromSlot, pToSlot ) )	return eItemUseErr_Move;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	if(FromPos == ToPos)
		return 1; 

	if(!CHKRT->ItemOf(pPlayer, FromPos, FromItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ToItemIdx,0,0,CB_ICONIDX))
		return 3;
	
	if( pToSlot == pPlayer->GetSlot(eItemTable_Shop) )
	{
		if( (pFromSlot != pPlayer->GetSlot(eItemTable_Shop)) &&
			(pFromSlot != pPlayer->GetSlot(eItemTable_ShopInven)) )
			return 11;
	}

	const ITEMBASE * pItem = pFromSlot->GetItemInfoAbs(FromPos);
	if( !pItem )		return eItemUseErr_Err;
	ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
	if( !pInfo )		return eItemUseErr_Err;

	//if(TP_SHOPITEM_START<=ToPos&&ToPos<TP_SHOPITEM_END	)
	//{// [解封的物品禁止放入百宝仓库] [by:十里坡剑神][QQ:112582793][2018/11/25]
	//	if(pItem->ItemParam==0)
	//		return 11;
	//}
	ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );

	if(pInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		if((pFromSlot == pPlayer->GetSlot(eItemTable_PetInven) && pToSlot == pPlayer->GetSlot(eItemTable_Titan))
			|| (pFromSlot == pPlayer->GetSlot(eItemTable_Pyoguk) && pToSlot == pPlayer->GetSlot(eItemTable_Titan)))
			return eItemUseErr_Err;

		if( pToSlot == pPlayer->GetSlot(eItemTable_MunpaWarehouse) )
			return eItemUseErr_Err;
		
		if( pToSlot == pPlayer->GetSlot(eItemTable_Titan) )
		{
			if(pInfo->LimitLevel > pPlayer->GetLevel())
				return eItemUseErr_Err;
		}		
	}

	if( pInfo->ItemType == 11 )
	{
		if( pInfo->ItemKind != eSHOP_ITEM_MAKEUP && pInfo->ItemKind != eSHOP_ITEM_DECORATION && pInfo->ItemKind != eSHOP_ITEM_EQUIP && pInfo->ItemKind != eSHOP_ITEM_PET && pInfo->ItemKind != eSHOP_ITEM_PET_EQUIP  && pInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP)	// magi82(26)
		{
			if( !(pItem->ItemParam & ITEM_PARAM_SEAL) )
			if( pFromSlot != pToSlot )
				return eItemUseErr_Err;
		}
	}
	if( pInfo->ItemKind == eSHOP_ITEM_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_Weared) ||
		pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_PetWeared) ||
		pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_TitanShopItem))	
	{
		if( pItem->ItemParam & ITEM_PARAM_SEAL )
			return eItemUseErr_Err;
		else
		{
			SHOPITEMWITHTIME* pusingitem = pPlayer->GetShopItemManager()->GetUsingItemInfo( FromItemIdx );
			if( pusingitem )
			{
				if( pusingitem->ShopItem.ItemBase.dwDBIdx != pItem->dwDBIdx )
					return eItemUseErr_Err;
			}
			else
			{
				IsAbleShopEquipItem( pPlayer->GetID(), pItem->dwDBIdx, FromItemIdx, FromPos, ToItemIdx, ToPos );
				return 99;
			}
		}
	}
	if( pFromSlot != pToSlot )
	{
		if( TRUE == IsPetSummonItem(FromItemIdx) )
		if( TRUE == pPlayer->GetPetManager()->IsCurPetSummonItem(pItem->dwDBIdx) )
			return eItemUseErr_Err;
	}

	const ITEMBASE* pToItem = pToSlot->GetItemInfoAbs(ToPos);
	if( pToItem )
	{
		if( pToInfo && pToInfo->ItemType == 11 )
		if( pToInfo->ItemKind != eSHOP_ITEM_MAKEUP && pToInfo->ItemKind != eSHOP_ITEM_DECORATION && pToInfo->ItemKind != eSHOP_ITEM_EQUIP && pToInfo->ItemKind != eSHOP_ITEM_PET && pToInfo->ItemKind != eSHOP_ITEM_PET_EQUIP && pToInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP )
		{
			if( !(pToItem->ItemParam & ITEM_PARAM_SEAL) )
			if( pFromSlot != pToSlot )
				return eItemUseErr_Err;
		}
		if( pToInfo && (pToInfo->ItemKind & eSHOP_ITEM_EQUIP) )
		{
			if( TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END )
				return eItemUseErr_Err;
            if( (pToItem->ItemParam & ITEM_PARAM_SEAL) &&
				 TP_WEAR_START <= FromPos && FromPos < TP_WEAR_END )
				 return eItemUseErr_Err;
		}
		
	}
	if( pFromSlot != pToSlot )
	{
		if( TRUE == IsPetSummonItem(ToItemIdx) )
		if( TRUE == pPlayer->GetPetManager()->IsCurPetSummonItem(pToItem->dwDBIdx) )
			return eItemUseErr_Err;
	}

	if(!CanbeMoved(pItem->wIconIdx,ToPos,pPlayer))
	{
		ASSERT(0);
		return 9;
	}
	if(!pToSlot->IsEmpty(ToPos))
	{
		pItem = pToSlot->GetItemInfoAbs(ToPos);
		if(pItem)
		{
			ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
			ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );
			if( !pInfo || !pToInfo )			return 10;
			if( pInfo->ItemKind == eSHOP_ITEM_EQUIP && pToInfo->ItemKind & eEQUIP_ITEM )
			{
				CItemSlot* pslot = pPlayer->GetSlot( eItemTable_Inventory );
				WORD pos = 0;
				if( pslot->GetEmptyCell( &pos ) == 0 )
					return 10;
			}
			else if( pInfo->ItemKind & eEQUIP_ITEM && pToInfo->ItemKind == eSHOP_ITEM_EQUIP )
			{
				CItemSlot* pslot = pPlayer->GetSlot( eItemTable_ShopInven );
				WORD pos = 0;
				if( pslot->GetEmptyCell( &pos ) == 0 )
					return 10;

			}
			else if(CanbeMoved(pItem->wIconIdx, FromPos, pPlayer) == FALSE)
			{
				ASSERT(0);
				return 10;
			}
		}
	}
	ITEMBASE DelFromItem;
	if(pFromSlot->DeleteItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
	{
		return 5;
	}

	ClearQuickPos(&DelFromItem, ToPos);
	ITEMBASE DelToItem;
	DelToItem.dwDBIdx = 0;
	if(pToSlot->DeleteItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
	{
		DelFromItem.Position = ToPos;
		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			DelFromItem.Position = FromPos;
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			return 6;
		}
	}
	else
	{
		SWAPVALUE(DelFromItem.Position, DelToItem.Position);
		ClearQuickPos(&DelToItem, FromPos);
		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 7;
		}
		ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );
		if( pToInfo->ItemKind == eSHOP_ITEM_EQUIP )
		{
			if( TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END )
			{
				pFromSlot = pPlayer->GetSlot( eItemTable_ShopInven );
				pFromSlot->GetEmptyCell( &FromPos );
			}
		}
		else if( pToInfo->ItemKind & eEQUIP_ITEM )
		{
			if( TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END )
			{
				pFromSlot = pPlayer->GetSlot( eItemTable_Inventory );
				pFromSlot->GetEmptyCell( &FromPos );
			}
		}
		if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelToItem) != EI_TRUE)
		{
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			ClearQuickPos(&DelToItem, ToPos);
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 8;
		}
	}

	if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
	{
		RemoveGuildItemOption(ToPos, pToSlot->GetItemInfoAbs(ToPos), pPlayer);
	}

	if( TP_SHOPINVEN_START <= DelFromItem.Position && DelFromItem.Position < TP_SHOPINVEN_END )
	{
		if( pInfo->ItemType == 11 && DelFromItem.ItemParam == 0 )
			ShopItemUpdateCharacterIdx( pPlayer->GetID(), DelFromItem.dwDBIdx );
	}
	BOOL bAvatarChange = FALSE;
	WORD AvatarIdx = 0;
	WORD AvatarPos = 0;
	if( ToPos == 81 )
	{
		if( pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
		{
			if( pToInfo && pInfo->WeaponType != pToInfo->WeaponType 
				&& pToInfo->WeaponType != WP_EVENT && pToInfo->WeaponType != WP_EVENT_HAMMER )
			{
				int avatarpos = eAvatar_Weared_Gum+pToInfo->WeaponType-1;
				if( pPlayer->GetShopItemStats()->Avatar[avatarpos] > 1 )
				{
					SHOPITEMWITHTIME* psItem = NULL;
					ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[avatarpos] );
					if( ptInfo )
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ptInfo->ItemIdx, ptInfo->SellPrice );
						if( psItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ptInfo->ItemIdx ) )
							psItem->ShopItem.Param = ptInfo->SellPrice;
					}

					AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pPlayer->GetShopItemStats()->Avatar[eAvatar_Dress] );				
					if( pTemp )
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = pTemp->Item[avatarpos];
					else
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = 1;

					bAvatarChange = TRUE;
					if (psItem)
					{
						AvatarIdx = psItem->ShopItem.ItemBase.wIconIdx;
						AvatarPos = psItem->ShopItem.ItemBase.Position;
					}
				
				}
			}
		}
	}
	else if( FromPos == 81 )
	{
		if( pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
		{
			if( pInfo->WeaponType != WP_EVENT && pInfo->WeaponType != WP_EVENT_HAMMER )
			{
				SHOPITEMWITHTIME* psItem = NULL;
				ITEM_INFO* ptInfo = NULL;
				AVATARITEM* pTemp = NULL;
				int avatarpos = eAvatar_Weared_Gum+pInfo->WeaponType-1;
				if( pPlayer->GetShopItemStats()->Avatar[avatarpos] > 1 )
				{
					ptInfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[avatarpos] );
					if( ptInfo )
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ptInfo->ItemIdx, ptInfo->SellPrice );
						if( psItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ptInfo->ItemIdx ) )
							psItem->ShopItem.Param = ptInfo->SellPrice;
					}

					pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pPlayer->GetShopItemStats()->Avatar[eAvatar_Dress] );				
					if( pTemp )
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = pTemp->Item[avatarpos];
					else
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = 1;

					bAvatarChange = TRUE;
					AvatarIdx = psItem->ShopItem.ItemBase.wIconIdx;
					AvatarPos = psItem->ShopItem.ItemBase.Position;
				}
			}
		}
	}

	if( bAvatarChange )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = pPlayer->GetID();
		msg.ItemIdx = AvatarIdx;
		msg.ItemPos = AvatarPos;
		memcpy( &msg.Avatar, pPlayer->GetShopItemStats()->Avatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );	

		pPlayer->GetShopItemManager()->CalcAvatarOption();
	}

	pPlayer->GetTitanManager()->MoveTitanEquipItemUpdateToDB(&DelFromItem, &DelToItem, FromPos, ToPos);

	if(ItemMoveUpdateToDBbyTable(pPlayer, DelFromItem.dwDBIdx, DelFromItem.wIconIdx, DelFromItem.Position, DelToItem.dwDBIdx, DelToItem.Position) == TRUE)
	{
		ItemMoveLog(FromPos, ToPos, pPlayer, &DelFromItem);
		if(DelToItem.dwDBIdx != 0)
			ItemMoveLog(ToPos, FromPos, pPlayer, &DelToItem);
	}

	return 0;
}

int CItemManager::DivideItem( CPlayer* pPlayer, WORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur,DWORD dwItemDBIdx)
{
	if(FromPos == ToPos)
		return 1;
	if(TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END)
	{
	}
	else if(TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END)
	{
	}
	else
	{
		return 1;
	}
	
	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx, FromDur+ToDur, 0, CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	if(CHKRT->ItemOf(pPlayer, ToPos,0,0,0,CB_EXIST))
		return 3;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);
	const ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );
	if(!FromItemBase) return 4;
	if(dwItemDBIdx!=FromItemBase->dwDBIdx) return 4;
    if( !IsDupItem( FromItemBase->wIconIdx ) )
	{
		return 4;
	}		

		if( FromItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur + ToDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( ToDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur == 0 ) return 1;
		if( ToDur == 0 ) return 1;
	if( pFromSlot->UpdateItemAbs(pPlayer, FromPos, FromItemBase->dwDBIdx, 0, 0, 0, FromDur,FromItemBase->ItemStatic, UB_DURA, SS_CHKDBIDX ) != EI_TRUE )
	{
		return 7;
	}


	ITEMOBTAINARRAYINFO * pArrayInfo = Alloc(pPlayer, MP_ITEM, MP_ITEM_DIVIDE_ACK, pPlayer->GetID(), pPlayer->GetID(), eLog_ItemObtainDivide, 2, (DBResult)DivideItemDBResult);
	/*pArrayInfo->ItemArray.AddItem( FromItemBase->dwDBIdx,
									FromItemBase->wIconIdx,
									FromDur,
									FromItemBase->Position,
									FromItemBase->QuickPosition,
									FromItemBase->ItemParam,
									FromItemBase->RareIdx,
									FromItemBase->ItemStatic);*/
	pArrayInfo->ItemArray.AddItem(FromItemBase);
	ItemUpdatebyTable(pPlayer, FromItemBase->dwDBIdx, FromItemBase->wIconIdx, FromDur, FromItemBase->Position, FromItemBase->QuickPosition,FromItemBase->RareIdx,FromItemBase->ItemStatic);
	
	WORD EmptyCellPos[1];
	EmptyCellPos[0] = ToPos;
	WORD EmptyCellNum = 1;
	return ObtainItemEx(pPlayer, pArrayInfo, FromItemBase->wIconIdx, (WORD)ToDur, EmptyCellPos, EmptyCellNum, EmptyCellNum+1,0,FromItemBase->ItemStatic);
}

void CItemManager::DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID)
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

int CItemManager::CombineItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur)
{
	if(FromPos == ToPos)
		return 1;

	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx,FromDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ItemIdx,ToDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 3;

	if( !IsDupItem(ItemIdx) )
	{
		return 4;
	}

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot = pPlayer->GetSlot(ToPos);

	ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );
	ITEMBASE * ToItemBase = pToSlot->GetItemInfoAbs( ToPos );
	if(FromItemBase->ItemStatic==1||ToItemBase->ItemStatic==1)
	{
		if(FromItemBase->ItemStatic!=ToItemBase->ItemStatic)
			return eItemUseErr_Err;
	}
	if(FromItemBase->QuickPosition!=0)
	{
		ToItemBase->QuickPosition = FromItemBase->QuickPosition;
		FromItemBase->QuickPosition = 0;
	}
	if( (FromItemBase->ItemParam & ITEM_PARAM_SEAL) ||
		(ToItemBase->ItemParam & ITEM_PARAM_SEAL ) )
	{
		return eItemUseErr_Err;
	}
	if( IsDupItem(FromItemBase->wIconIdx) && FromItemBase->Durability >= MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( IsDupItem(ToItemBase->wIconIdx) && ToItemBase->Durability >= MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	
	DURTYPE SumDur = FromItemBase->Durability + ToItemBase->Durability;
	if( SumDur > MAX_YOUNGYAKITEM_DUPNUM )
	{
		if(pFromSlot->UpdateItemAbs(pPlayer, FromPos, 0, 0, 0, FromItemBase->QuickPosition, (SumDur - MAX_YOUNGYAKITEM_DUPNUM),FromItemBase->ItemStatic, UB_DURA|UB_QABSPOS ) != EI_TRUE )
		{
			return 5;
		}
		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, ToItemBase->QuickPosition, MAX_YOUNGYAKITEM_DUPNUM,ToItemBase->ItemStatic, UB_DURA|UB_QABSPOS ) != EI_TRUE)
		{
			return 6;
		}

		ItemCombineUpdateToDB(FromItemBase->dwDBIdx, FromItemBase->Durability, ToItemBase->dwDBIdx, ToItemBase->Durability);

		FromDur		= (SumDur - MAX_YOUNGYAKITEM_DUPNUM);
		ToDur		= MAX_YOUNGYAKITEM_DUPNUM;
	}
	else
	{
		ITEMBASE fromItemOut;
		if(pFromSlot->DeleteItemAbs(pPlayer,  FromPos, &fromItemOut ) != EI_TRUE)
		{
			return 7;
		}
		
		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, ToItemBase->QuickPosition, SumDur,ToItemBase->ItemStatic, UB_DURA|UB_QABSPOS ) != EI_TRUE)
		{
			return 8;
		}

		ItemCombineUpdateToDB(fromItemOut.dwDBIdx, 0, ToItemBase->dwDBIdx, ToItemBase->Durability);
		
		FromDur		= 0;
		ToDur		= SumDur;
	}
	return 0;
}
void CItemManager::CheckAvataChange(CPlayer * pPlayer,WORD ItemIdx,WORD wWhatPos)
{
	//[武器出售后如果带有百宝武器，将百宝武器脱下，防止卡百宝][By:十里坡剑神][QQ:112582793][2017/11/27]
	ITEM_INFO* pInfo = GetItemInfo( ItemIdx );
	if( !pInfo )		return ;
	BOOL bAvatarChange = FALSE;
	WORD AvatarIdx = 0;
	WORD AvatarPos = 0;
	if( wWhatPos == 81 )
	{
		if( pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
		{
			if( pInfo->WeaponType != WP_EVENT && pInfo->WeaponType != WP_EVENT_HAMMER )
			{
				SHOPITEMWITHTIME* psItem = NULL;
				ITEM_INFO* ptInfo = NULL;
				AVATARITEM* pTemp = NULL;
				int avatarpos = eAvatar_Weared_Gum+pInfo->WeaponType-1;
				if( pPlayer->GetShopItemStats()->Avatar[avatarpos] > 1 )
				{
					ptInfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[avatarpos] );
					if( ptInfo )
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ptInfo->ItemIdx, ptInfo->SellPrice );
						if( psItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ptInfo->ItemIdx ) )
							psItem->ShopItem.Param = ptInfo->SellPrice;
					}

					pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pPlayer->GetShopItemStats()->Avatar[eAvatar_Dress] );				
					if( pTemp )
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = pTemp->Item[avatarpos];
					else
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = 1;

					bAvatarChange = TRUE;
					AvatarIdx = psItem->ShopItem.ItemBase.wIconIdx;
					AvatarPos = psItem->ShopItem.ItemBase.Position;
				}
			}
		}
	}

	if( bAvatarChange )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = pPlayer->GetID();
		msg.ItemIdx = AvatarIdx;
		msg.ItemPos = AvatarPos;
		memcpy( &msg.Avatar, pPlayer->GetShopItemStats()->Avatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );	

		pPlayer->GetShopItemManager()->CalcAvatarOption();
	}
}
//[物品删除函数][By:十里坡剑神][QQ:112582793][2018/1/30]
int CItemManager::DiscardItem(CPlayer* pPlayer, POSTYPE whatPos, WORD whatItemIdx, DURTYPE whatItemNum,BOOL bIsCanDiscard)
{
	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	if( !pSlot )	return 1;
	//if(whatPos>=80&&whatPos<=89&&!bIsCanDiscard)
	//{//[装备栏物品不允许直接删除][By:十里坡剑神][QQ:112582793][2018/4/13]
	//	return 1;
	//}
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(whatPos);
	//[获取的物品和需删除的物品不同][By:十里坡剑神][QQ:112582793][2018/1/30]
	if(pItemBase->wIconIdx != whatItemIdx) 
		return 4;
	//[可叠加物品叠加数量超过最大允许叠加数][By:十里坡剑神][QQ:112582793][2018/1/30]
	if( IsDupItem(pItemBase->wIconIdx) && pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;

	if(IsDupItem(pItemBase->wIconIdx) && (pItemBase->Durability > whatItemNum))
	{//[可叠加物品删除处理][By:十里坡剑神][QQ:112582793][2018/1/30]
		if( pSlot->UpdateItemAbs(pPlayer, whatPos, 0, 0, 0, 0, pItemBase->Durability - whatItemNum,pItemBase->ItemStatic, UB_DURA ) != EI_TRUE )
		{
			return 2;
		}

		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx,pItemBase->ItemStatic );
	}
	else
	{//[非叠加物品删除处理][By:十里坡剑神][QQ:112582793][2018/1/30]
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->wIconIdx );
		if( !pItemInfo )	return eItemUseErr_Discard;

		ITEMBASE DelItemOut;
		SHOPITEMWITHTIME* pUsingItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( whatItemIdx );
		if( pUsingItem )
		{//[百宝删除处理移除使用状态][By:十里坡剑神][QQ:112582793][2018/1/30]
			if( pUsingItem->ShopItem.ItemBase.dwDBIdx == pItemBase->dwDBIdx )
			{
				if(pItemInfo->ItemType==10)
				{
                   pPlayer->GetShopItemManager()->DeleteDupParam(pItemInfo);
				}
				else
				{
				   pPlayer->GetShopItemManager()->DeleteUsingShopItem( whatItemIdx );
				}
				

				if( pItemInfo->ItemType == 11 )
				{
					if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
						pPlayer->GetShopItemManager()->DiscardAvatarItem( whatItemIdx, whatPos );					
				}
			}
		}
		//[删除物品][By:十里坡剑神][QQ:112582793][2018/1/30]
		if( pSlot->DeleteItemAbs(pPlayer, whatPos, &DelItemOut, SS_LOCKOMIT ) != EI_TRUE )
			return 3;		
		//[删除物品写入数据库][By:十里坡剑神][QQ:112582793][2018/1/30]
		ItemDeleteToDB(DelItemOut.dwDBIdx);
		if(IsOptionItem(DelItemOut.wIconIdx, DelItemOut.Durability))
		{//[被删除物品强化信息删除][By:十里坡剑神][QQ:112582793][2018/1/30]
			ITEM_OPTION_INFO* pInfo = pPlayer->GetItemOption(DelItemOut.Durability);
			if( pInfo )
			{
				ITEM_OPTION_INFO info = *pInfo;
				info.dwOptionIdx = 0;
				LogItemOption( pPlayer->GetID(), DelItemOut.dwDBIdx, &info );
			}

			if( FALSE == pPlayer->RemoveItemOption(DelItemOut.Durability) )
			{//需要修复
				char szBuf[128];
				sprintf( szBuf, "DeleteOptItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
					,DelItemOut.wIconIdx );
				ASSERTMSG(0, szBuf);
			}
			CharacterItemOptionDelete(DelItemOut.Durability, DelItemOut.dwDBIdx);
		}
		if(IsRareOptionItem(DelItemOut.wIconIdx, DelItemOut.RareIdx))
		{//[被删除物品绿化信息删除][By:十里坡剑神][QQ:112582793][2018/1/30]
			ITEM_RARE_OPTION_INFO* pInfo = pPlayer->GetItemRareOption(DelItemOut.RareIdx);
			if( pInfo )
			{
				ITEM_RARE_OPTION_INFO info = *pInfo;
				info.dwRareOptionIdx = 0;
				LogItemRareOption( pPlayer->GetID(), DelItemOut.dwDBIdx, &info );
			}

			if( FALSE == pPlayer->RemoveItemRareOption(DelItemOut.RareIdx) )
			{
				char szBuf[128];
				sprintf( szBuf, "DeleteRareItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
					,DelItemOut.wIconIdx );
				ASSERTMSG(0, szBuf);
			}
			CharacterItemRareOptionDelete(DelItemOut.RareIdx, DelItemOut.dwDBIdx);
		}
		if( IsPetSummonItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetPetManager()->DeletePet(DelItemOut.dwDBIdx);
		}

		if( IsTitanCallItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetTitanManager()->DeleteTitan(DelItemOut.dwDBIdx);
		}

		if( IsTitanEquipItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetTitanManager()->DeleteTitanEquip(DelItemOut.dwDBIdx);
		}
		if( (pItemInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN) || (pItemInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN) )
			pPlayer->GetShopItemManager()->DiscardSkinItem( whatItemIdx );					
	}
	//[武器出售后如果带有百宝武器，将百宝武器脱下，防止卡百宝][By:十里坡剑神][QQ:112582793][2017/11/27]
	CheckAvataChange(pPlayer,whatItemIdx,whatPos);
	return 0;
}

int CItemManager::SellItem( CPlayer* pPlayer, POSTYPE whatPos, WORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx,DWORD dwItemDBIdx  )
{//[出售物品处理][By:十里坡剑神][QQ:112582793][2018/4/13]
	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal)&&DealerIdx!=301)
		return 1;

	if(!CHKRT->ItemOf(pPlayer, whatPos, wSellItemIdx,sellItemDur,0,CB_EXIST|CB_ICONIDX|CB_ENOUGHDURA))
		return 2;

	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	ITEMBASE Item = *pSlot->GetItemInfoAbs(whatPos);

	ITEM_INFO * pItemInfo = GetItemInfo( wSellItemIdx );
	if( !pItemInfo )	return 1;

	MONEYTYPE money = 0;
	if(Item.dwDBIdx!= dwItemDBIdx) return 1;
	//////////////////////////////////////////////////////////////////////////
	if( IsDupItem(wSellItemIdx) && sellItemDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////
	if( IsDupItem(wSellItemIdx) && !IsOptionItem(wSellItemIdx, sellItemDur) )
		money = pItemInfo->SellPrice * sellItemDur;
	else
		money = pItemInfo->SellPrice;
	if (pItemInfo->wSellType == eMoney)
	{
		money = SWPROFITMGR->AddProfitFromItemSell(money);

		money = FORTWARMGR->AddProfitMoneyFromItemSell(money);

		if (!pPlayer->IsEnoughAdditionMoney(money))
			return EI_MAXMONEY;
	}


	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	if(Item.dwDBIdx == pPlayer->GetTitanManager()->GetRegistTitanCallItemDBIdx())
	{
		return 11;
	}

	if(EI_TRUE != DiscardItem(pPlayer, whatPos, wSellItemIdx, sellItemDur))
	{
		return 3;
	}
	switch (pItemInfo->wSellType)
	{
	case eMoney:
		{
			pPlayer->SetMoney(money, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetStall, DealerIdx);

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), DealerIdx, "",
				eLog_ItemSell, pPlayer->GetMoney(eItemTable_Inventory), 0, money,
				Item.wIconIdx, Item.dwDBIdx, Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());
		}
		break;
	case eMall:
		{
			pPlayer->SetMallMoney(money, 2, Item.wIconIdx);
			ItemShopUseLog(eLog_GetMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(), Item.wIconIdx, pItemInfo->ItemName, Item.Durability,
				pPlayer->GetMallMoney(), money, pPlayer->GetMallMoney() + money, 0, 0, 0, 0, 0, 0);
		}
		break;
	case eGold:
		{
			pPlayer->SetGoldMoney(money, eGetGoldMoney, Item.wIconIdx);
			ItemShopUseLog(eLog_GetGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(), Item.wIconIdx, pItemInfo->ItemName, Item.Durability,
				0, 0, 0, pPlayer->GetGoldMoney(), money, pPlayer->GetGoldMoney() + money, 0, 0, 0);
		}
		break;
	default:
		break;
	}
	
	//[武器出售后如果带有百宝武器，将百宝武器脱下，防止卡百宝][By:十里坡剑神][QQ:112582793][2017/11/27]
	CheckAvataChange(pPlayer,wSellItemIdx,whatPos);
	return 0;
}
int CItemManager::BuyItemForGoldShop(CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx,WORD wMainIndex,WORD wSubIndex,DWORD ItemPos,WORD buyType)
{
	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal) && DealerIdx != MALLSHOPIDX)
		return 1;

	if(!CHKRT->DearlerItemOf(DealerIdx, buyItemIdx) && DealerIdx != MALLSHOPIDX)
		return 2;

	if( buyItemNum == 0 ) return EI_TRUE;	
	if(DealerIdx!=MALLSHOPIDX) return 2;

	if( buyItemNum > MAX_ITEMBUY_NUM )
		buyItemNum = MAX_ITEMBUY_NUM;
	ITEM_INFO * pItemInfo	= GetItemInfo( buyItemIdx );
	if( !pItemInfo )
		return 3;

	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if( pItemInfo->ItemKind & eSHOP_ITEM )//[判断购买的是否是百宝][By:十里坡剑神][QQ:112582793][2018/9/30]
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);

	GOLD_SHOP_ITEM_INFO  PShopGoldInfo;

	WORD obtainItemNum = 0;

	MONEYTYPE ItemPrice = 0;
	if(DealerIdx==MALLSHOPIDX)
	{
		PShopGoldInfo=ITEMMGR->GetGoldShopItem(buyItemIdx,wMainIndex,wSubIndex,ItemPos);
	
		if(PShopGoldInfo.dwCanBuyTime!=0)
		{// [商城物品限制信息] [by:十里坡剑神][QQ:112582793][2018/10/28]
			if(!SHOPITEMLIMITMGR->CanBuyItem(PShopGoldInfo.dwCanBuyTime))
				return NOT_LIMITTIMES;
		}

		if(PShopGoldInfo.wItemIndex==0)return 3;
		ItemPrice = MONEYTYPE(PShopGoldInfo.dwPrice)*MONEYTYPE(buyItemNum);

		if(buyType!=PShopGoldInfo.wCurrency)
			return 3;

		if(PShopGoldInfo.wItemIndex != buyItemIdx)
			return 3;


		if(buyType==1)
		{
			if (PShopGoldInfo.dwPrice <= 0)
				obtainItemNum=buyItemNum;
			else
				obtainItemNum = pPlayer->GetMallMoney() >= ItemPrice ? buyItemNum : (WORD)(pPlayer->GetMallMoney() / PShopGoldInfo.dwPrice);
		}
		else if(buyType==2)
		{
			if (PShopGoldInfo.dwPrice <= 0)
				obtainItemNum=buyItemNum;
			else
				obtainItemNum = pPlayer->GetGoldMoney() >= ItemPrice ? buyItemNum : (WORD)(pPlayer->GetGoldMoney() / PShopGoldInfo.dwPrice);
		}
		else if(buyType==3)
		{
			if (PShopGoldInfo.dwPrice <= 0)
				obtainItemNum=buyItemNum;
			else
				obtainItemNum = pPlayer->GetMoney() >= ItemPrice ? buyItemNum : (WORD)(pPlayer->GetMoney() / PShopGoldInfo.dwPrice);

		}
		if(!(obtainItemNum>=buyItemNum))
			return NOT_MONEY; 
	}
	
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	int rt;
	WORD Seal=0;
	if (pItemInfo->ItemKind & eSHOP_ITEM)
		Seal=1;
	obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum,buyType);
	
	if(obtainItemNum == 0)
		return NOT_SPACE;	
	WORD wTmpType = 2;		
	if(PShopGoldInfo.bBunding)
		wTmpType=1;			
	if(buyType==3&&!PShopGoldInfo.bBunding)
		wTmpType=0;

	rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_BUY_ACK, pPlayer->GetID(), DealerIdx, eLog_ItemObtainBuy, obtainItemNum, (DBResult)(BuyItemDBResult), NULL, buyType), PShopGoldInfo.wItemIndex, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, Seal, wTmpType);

	if( rt == EI_TRUE )
	{
		MONEYTYPE cPrice = buyItemNum * PShopGoldInfo.dwPrice;
		if(DealerIdx==MALLSHOPIDX)
		{
			if(buyType==1)
			{
				if(cPrice>0)
					pPlayer->SetMallMoney(cPrice, 1, PShopGoldInfo.wItemIndex);
				ItemShopUseLog(eLog_UseMallMoney, pPlayer->GetID(), pPlayer->GetObjectName(), PShopGoldInfo.wItemIndex, PShopGoldInfo.strItemName,
					buyItemNum,pPlayer->GetMallMoney()+cPrice,cPrice,pPlayer->GetMallMoney(),0,0,0,0,0,0);
			}
			else if(buyType==2)
			{
				if(cPrice>0)
					pPlayer->SetGoldMoney(cPrice, eShopUse, PShopGoldInfo.wItemIndex, buyItemNum);
				ItemShopUseLog(eLog_UseGoldMoney, pPlayer->GetID(), pPlayer->GetObjectName(), PShopGoldInfo.wItemIndex, PShopGoldInfo.strItemName,
					buyItemNum, 0, 0, 0,pPlayer->GetGoldMoney(),cPrice,pPlayer->GetGoldMoney()-cPrice,0,0,0);
			}
			else if(buyType==3)
			{
				if(cPrice>0)
					pPlayer->SetMoney( cPrice, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStall, DealerIdx );
				ItemShopUseLog(eLog_UseMoney, pPlayer->GetID(), pPlayer->GetObjectName(), PShopGoldInfo.wItemIndex, PShopGoldInfo.strItemName,
					buyItemNum, 0, 0, 0, 0, 0, 0, pPlayer->GetMoney()+cPrice,cPrice,pPlayer->GetMoney());
			}
			
		}
	}
	return rt;
}
int CItemManager::BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx ,DWORD ItemPos,WORD buyType)
{
	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal) && DealerIdx != MALLSHOPIDX)
		return 1;
	
	if(!CHKRT->DearlerItemOf(DealerIdx, buyItemIdx) && DealerIdx != MALLSHOPIDX)
		return 2;

	if( buyItemNum == 0 ) return EI_TRUE;	

	if( DealerIdx == BOBUSANG_wNpcUniqueIdx )
	if( FALSE == BOBUSANGMGR->BuyItem( pPlayer, buyItemIdx, buyItemNum ) )
		return NOT_REMAINITEM;

	if( buyItemNum > MAX_ITEMBUY_NUM )
		buyItemNum = MAX_ITEMBUY_NUM;

	ITEM_INFO * pItemInfo	= GetItemInfo( buyItemIdx );
	if( !pItemInfo )
		return 3;

	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if( pItemInfo->ItemKind & eSHOP_ITEM )
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);


    WORD obtainItemNum = 0;

	MONEYTYPE ItemPrice = pItemInfo->BuyPrice;
	if(g_pServerSystem->GetMap()->IsVillage() == FALSE)
	{
		ItemPrice = (long long)(ItemPrice*1.2);
	}

	ItemPrice = SWPROFITMGR->AddProfitFromItemBuy( ItemPrice );

	ItemPrice = FORTWARMGR->AddProfitMoneyFromItemBuy( ItemPrice );

	obtainItemNum = GetCanBuyNumInMoney(pPlayer, buyItemNum, ItemPrice);

	if(obtainItemNum==0)
        return NOT_MONEY; 

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;


	int rt;
	WORD Seal=0;
	if (pItemInfo->ItemKind & eSHOP_ITEM)
		Seal=1;

	obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum,buyType);
	if(obtainItemNum == 0)
			return NOT_SPACE;	
    rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_BUY_ACK, pPlayer->GetID(), DealerIdx, eLog_ItemObtainBuy, obtainItemNum, (DBResult)(BuyItemDBResult),NULL,buyType), buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,Seal);

	if( rt == EI_TRUE )
	{
	  pPlayer->SetMoney( ItemPrice*obtainItemNum, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStall, DealerIdx );
	}
	return rt;
}
void CItemManager::BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID )
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

void CItemManager::MonsterObtainItem(CPlayer * pPlayer, WORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum)
{
	WORD EmptyCellPos[128];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	ITEM_INFO* pInfo = GetItemInfo( obtainItemIdx );
	if( !pInfo )			return;
	if( pInfo->ItemKind & eSHOP_ITEM )
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		
	BOOL bGetTwice = FALSE;
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	DWORD bSignature;
	BOOL bRare = RAREITEMMGR->GetRare( obtainItemIdx, &RareOptionInfo, pPlayer,bSignature );
	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_DoubleChance, &bGetTwice);


	int rt = 0;
	for( int i = 0; i < 1 + bGetTwice; ++i )
	{
		WORD obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum);
		if(obtainItemNum == 0)
		{
			MSGBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_FULLINVEN_ALERT;
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}

		if( pInfo->ItemKind & eSHOP_ITEM )
			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1);
		else if( pInfo->ItemKind & eEQUIP_ITEM )
		{
			if( FALSE == bRare )
			{				
				if( FORTWARMGR->AddProfitItemFromMonster( pPlayer, obtainItemIdx, 1 ) == FALSE )
                    rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);
			}
			else
			{
				//[怪物掉落率字装备][By:十里坡剑神][QQ:112582793][2017/12/7]
				rt = ObtainRareItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );			
			}
		}
		else
			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult),NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

		if(rt == 0 && pPlayer->GetPartyIdx())
		{
			PARTYMGR->SendObtainItemMsg(pPlayer, obtainItemIdx);
		}
	}
	return;
}
//[充值礼包发放][By:十里坡剑神][QQ:112582793][2018/1/31]
int CItemManager::RechargeObtainItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel,WORD BuyType )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,BuyType );
	if(obtainItemNum == 0)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainRecharge, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );
}
//[攻城战物品发放][By:十里坡剑神][QQ:112582793][2018/2/21]
int CItemManager::SiegeWarObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel,WORD BuyType)
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot=NULL;
	if (bSel==1)
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	else
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,BuyType );
	if(obtainItemNum == 0)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainSiegeWar, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );
}
//[杀人抢夺物品][By:十里坡剑神][QQ:112582793][2018/2/21]
int CItemManager::LootObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,POSTYPE pos,WORD bSel,WORD BuyType )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot=NULL;
	if (bSel==1)
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	else
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,BuyType );
	if(obtainItemNum == 0)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_PK, MP_PK_LOOTING_ITEMLOOTED_UPDATE, pPlayer->GetID(), 0, eLog_ItemObtainPK, obtainItemNum, (DBResult)(LootObtainItemDBResult),NULL,pos), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );

}
int CItemManager::SingedObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel,WORD BuyType )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot=NULL;
	if (bSel==1)
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	else
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,BuyType );
	if(obtainItemNum == 0)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainSinged, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );
}
int CItemManager::MarryObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel,WORD BuyType )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot=NULL;
	if (bSel==1)
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	else
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,BuyType );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemObtainMarry, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );

}
int CItemManager::CheatObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel,WORD BuyType  )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,1 );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,bSel,BuyType );
}

int CItemManager::CheatObtainRareItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum, ITEM_OPTION_INFO* pOptionInfo)
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum,1 );
	if(obtainItemNum == 0)
		return 1;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	RareOptionInfo.GenGol = pOptionInfo->GenGol;
	RareOptionInfo.MinChub = pOptionInfo->MinChub;
	RareOptionInfo.CheRyuk = pOptionInfo->CheRyuk;
	RareOptionInfo.SimMek = pOptionInfo->SimMek;
	RareOptionInfo.Life = pOptionInfo->Life;
	RareOptionInfo.NaeRyuk = pOptionInfo->NaeRyuk;
	RareOptionInfo.Shield = pOptionInfo->Shield;
	RareOptionInfo.PhyAttack = pOptionInfo->PhyAttack;
	RareOptionInfo.PhyDefense = pOptionInfo->PhyDefense;
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER));
	
	return ObtainRareItem( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemRareObtainCheat, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );	
}

int CItemManager::ObtainRareItem(CPlayer* pPlayer, ITEMOBTAINARRAYINFO* pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD ArrayInfoUnitNum, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	if(IsDupItem(whatItemIdx)) ASSERT(0);

	WORD obtainItemNum = whatItemNum;

	ITEMBASE NewItemBase;
	NewItemBase.dwDBIdx			= 9;
	NewItemBase.wIconIdx		= 9;
	NewItemBase.Position		= 9;
	NewItemBase.QuickPosition	= 9;
	NewItemBase.Durability		= 9;
	NewItemBase.RareIdx			= 9;
	NewItemBase.ItemParam		= 9;

	CItemSlot* pSlot = pPlayer->GetSlot(EmptyCellPos[0]);
	BOOL bIsEmpty = pSlot->IsEmpty(EmptyCellPos[0]);
	if( bIsEmpty )
	{
		NewItemBase.Position = EmptyCellPos[0];
		if( pSlot->InsertItemAbs(pPlayer, EmptyCellPos[0], &NewItemBase, SS_PREINSERT) != EI_TRUE )
		{
			ASSERT(0);
			return 1;
		}

		ItemRareInsertToDB( pPlayer->GetID(), whatItemIdx, EmptyCellPos[0], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), pRareOptionInfo );

		obtainItemNum--;
		ASSERT(!obtainItemNum);
	}
	else
	{
			ASSERT(0);
			return 0;
	}

	return 0;
}

int CItemManager::ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD bSeal,WORD BuyType)  // 2014-12-01 重写更新函数  默认0表示普通购买! 2表示购买
{
	WORD obtainItemIdx = whatItemIdx;
	WORD obtainItemNum = whatItemNum;
	
	POSTYPE i = 0;
	
	ITEMBASE NewItemBase; 
	NewItemBase.dwDBIdx			= 9;
	NewItemBase.wIconIdx		= 9;
	NewItemBase.Position		= 9;
	NewItemBase.QuickPosition	= 9;
	NewItemBase.Durability		= 9;
	NewItemBase.ItemParam		= 9;
	NewItemBase.RareIdx			= 9;
	NewItemBase.ItemStatic		= 9;
	if(BuyType==1)
		NewItemBase.ItemStatic = 1;
	BOOL bDBReturn = FALSE;
	if(IsDupItem(whatItemIdx))      
	{	
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(EmptyCellPos[i]);
			BOOL bIsEmpty = pSlot->IsEmpty(EmptyCellPos[i]);
			if( bIsEmpty )
			{
				bDBReturn = TRUE;
				if( obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					NewItemBase.Position		= EmptyCellPos[i];
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 3;
						ASSERT(0);
					}
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal,BuyType,0); // 2014-12-02 数据库
					obtainItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				}
				else
				{
					NewItemBase.Position		= EmptyCellPos[i]; 
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 4;
						ASSERT(0);
					}
					
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, obtainItemNum, EmptyCellPos[i],  MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal,BuyType,0);  // 2014-12-02 数据库
					obtainItemNum = 0;
				}
			}
			else if( !bIsEmpty && pItemBase->wIconIdx == obtainItemIdx && pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )	
			{				
				if( pItemBase->Durability + obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{

					obtainItemNum = obtainItemNum + (WORD)pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, MAX_YOUNGYAKITEM_DUPNUM,pItemBase->ItemStatic, UB_DURA, SS_PREINSERT)  != EI_TRUE )
					{
						return 1;
					}
					
					/*pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx,
						pItemBase->wIconIdx,
						pItemBase->Durability,
						pItemBase->Position,
						pItemBase->QuickPosition,
						pItemBase->ItemParam ,
						pItemBase->ItemStatic);*/
					pArrayInfo->ItemArray.AddItem(pItemBase);
					
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition,pItemBase->RareIdx,pItemBase->ItemStatic );
				}
				else
				{
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, pItemBase->Durability + obtainItemNum,pItemBase->ItemStatic, UB_DURA, SS_PREINSERT )  != EI_TRUE )
					{
						return 2;
					}
					
				/*	pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx, 
						pItemBase->wIconIdx, 
						pItemBase->Durability, 
						pItemBase->Position, 
						pItemBase->QuickPosition,
						pItemBase->ItemParam,
						pItemBase->RareIdx,
						pItemBase->ItemStatic,
						pItemBase->ItemGrow);*/
					pArrayInfo->ItemArray.AddItem(pItemBase);
					
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition,pItemBase->RareIdx,pItemBase->ItemStatic );
					
					obtainItemNum = 0;
				}
			}
			else
				ASSERT(0);
		}
		
		if( bDBReturn == FALSE )
		{
			pArrayInfo->CallBack( pPlayer, (WORD)pArrayInfo->wObtainArrayID );
			return 0;
		}
	}
	else
	{
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			if( pSlot->IsEmpty(EmptyCellPos[i]) )
			{
				NewItemBase.Position		= EmptyCellPos[i]; 
				if( bSeal&&NewItemBase.Position>=390)
				{//[商城购买普通物品显示封印修复][By:十里坡剑神][QQ:112582793][2017/12/6]
					NewItemBase.ItemParam = 1;
				}
				else	
					bSeal=0;

				NewItemBase.ItemStatic=BuyType;

				if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
				{
					return 5;
					ASSERT(0);
				}
				--obtainItemNum;
				ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, 0, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal,BuyType,0);  // 2014-12-02 数据库
			}
		}
	}

	if(obtainItemNum > 0)
	{
		ASSERT(0);
		return 6;
	}

	return 0;
}
void CItemManager::LootObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID)
{//[抢夺叠加物品][By:十里坡剑神][QQ:112582793][2018/5/1 19:39]
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);
	if (pArrayInfo == NULL) return;
	for( int i = 0 ; i < pArrayInfo->ItemArray.ItemNum ; ++i )
	{
		ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem( i );
		CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
		if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
		{
			continue;
		}
	}
	pArrayInfo->ItemArray.dwLootID= pPlayer->GetKilledPlayerID();
	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );
	//MSG_LOOTINGITEM AddMsg;
	//AddMsg.Category = pArrayInfo->ItemArray.Category;
	//AddMsg.Protocol = pArrayInfo->ItemArray.Protocol;
	//AddMsg.ItemInfo = pArrayInfo->ItemArray.ItemInfo[0];
	//AddMsg.wAbsPosition = pPlayer->GetLootPos();
	//AddMsg.dwMoney = pPlayer->GetKilledPlayerID();
	//pPlayer->SendMsg(&AddMsg,sizeof(AddMsg));
	ITEMMGR->Free(pPlayer, pArrayInfo);
}
void CItemManager::ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);
	if (pArrayInfo == NULL) return;
	MONEYTYPE ChangeMoney = 0;
	for( int i = 0 ; i < pArrayInfo->ItemArray.ItemNum ; ++i )
	{
		ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem( i );
		CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
		if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
		{
			continue;
		}

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ItemInfo->wIconIdx );
		if(pArrayInfo->wType == eLog_ItemObtainBuy)
		{
			ChangeMoney = pInfo->BuyPrice;
		}

		if( pInfo->ItemKind == eQUEST_ITEM_PET || pInfo->ItemKind == eSHOP_ITEM_PET )
		{
			PET_TOTALINFO* pPetInfo = pPlayer->GetPetManager()->GetPetTotalInfo(ItemInfo->dwDBIdx, eWithNULL );
			if( NULL == pPetInfo )
			{
				g_pServerSystem->CreateNewPet(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_MugongIdx);
			}
		}

		if( pInfo->ItemKind == eTITAN_ITEM_PAPER )
		{
			TITAN_TOTALINFO* pTitanInfo = pPlayer->GetTitanManager()->GetTitanTotalInfo(ItemInfo->dwDBIdx);
			if( !pTitanInfo )
			{
				pPlayer->GetTitanManager()->CreateNewTitan(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_MugongIdx, pInfo->Plus_Value);
			}
		}

		if( pInfo->ItemKind & eTITAN_EQUIPITEM )
		{
			pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, ItemInfo);
		}

		LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
				pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), ChangeMoney,
				ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->Durability, pPlayer->GetPlayerExpPoint());

		if( pArrayInfo->wType == eLog_ItemObtainCheat )
		{
			LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemObtainCheat, 
				ItemInfo->wIconIdx, ItemInfo->dwDBIdx );
		}
	}

	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

unsigned int CItemManager::GetTotalEmptySlotNum(CItemSlot* pSlot)
{
	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;

	unsigned int totalemptyNum = 0;
	for( i = startPos ; i < EndPos ; ++i )
	{
		if(pSlot->IsEmpty(i)) totalemptyNum++;
	}

	return totalemptyNum;
}
//防止泡点购买和元宝购买叠加重写此函数  by:十里坡剑神	 QQ:112582793
WORD CItemManager::GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum,WORD buyType)
{
	DURTYPE EstItemNum = whatItemNum;
	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;
	EmptyCellNum = 0;

	if(!IsDupItem(whatItemIdx))
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				--EstItemNum;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}
	else
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(i);
			if(buyType == 1)
			{
				if( !pSlot->IsEmpty(i) && !pSlot->IsLock(i) &&
				pItemBase->wIconIdx == whatItemIdx &&
				pItemBase->ItemStatic == 1&&
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )
				{
					if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
						EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
					else
						EstItemNum = 0;
					EmptyCellPos[EmptyCellNum++] = i;
				}
			}
			else
			{
				if( !pSlot->IsEmpty(i) && !pSlot->IsLock(i) &&
				pItemBase->wIconIdx == whatItemIdx &&
				pItemBase->ItemStatic != 1&&
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )
				{
					if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
						EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
					else
						EstItemNum = 0;
					EmptyCellPos[EmptyCellNum++] = i;
				}
			}
		}
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				if( EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}
	return (WORD)(whatItemNum - EstItemNum);
}

WORD CItemManager::GetCanBuyNumInMoney(CPlayer * pPlayer, WORD buyNum, MONEYTYPE Price)
{
	long long money = pPlayer->GetMoney();

	if (money >= long long(buyNum * Price))
	{
		return buyNum;
	}
	else
	{
		return (WORD)(money / Price);
	}
}
int CItemManager::UseItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx,DWORD dwItemDBIdx ,BOOL bIsDeal)
{
	if(pPlayer->GetState() == eObjectState_Die)
		return eItemUseErr_Err;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return eItemUseErr_Err;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	ITEMBASE Item = *pItemBase;
	ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);
	if(!pItemInfo||!pItemBase) return eItemUseErr_Err;

	if((pItemBase->dwDBIdx!= dwItemDBIdx)&&!bIsDeal) return eItemUseErr_Err;
	//[物品使用增加数据库编号检查][By:十里坡剑神][QQ:112582793][2018/4/13]
	if(pPlayer->IsHide())
	if(IsUnfitItemInfoListForHide(wItemIdx))
	{
		return eItemUseErr_Err;
	}

		//[吃药回气检查][By:十里坡剑神][QQ:112582793][2017/11/27]
	if(pItemInfo->ItemKind==eYOUNGYAK_ITEM)
	{
		YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
		Tmp=GAMERESRCMNGR->GetYoungYakDelay()->GetData(pItemInfo->ItemIdx);
		if(Tmp)
		{
			DWORD dwRemainTime = 0;

			float delayRate = pPlayer->GetDelayGroup()->CheckYoungYakPotionDelay(Tmp->wtype,&dwRemainTime );
			if( delayRate > 0.0f )
			{
				return eItemUseErr_Err;
			}
			else
			{
				pPlayer->GetDelayGroup()->AddYoungYakPotionDelay(Tmp->DelayTime,Tmp->wtype);
			}
		}
	}
	if(pItemInfo->ItemKind!=eYOUNGYAK_ITEM&&pItemInfo->ItemKind!=eSHOP_ITEM_CHARM
		&&pItemInfo->ItemKind!=eSHOP_ITEM_HERB)
	{
		if(pPlayer->IsSafeLock())
		{//[安全锁-物品使用][By:十里坡剑神][QQ:112582793][2018/5/6 23:40]
			MSG_DWORD2 msg;
			msg.Category = MP_SAFELOCK;
			msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
			msg.dwData1 = 4;
			pPlayer->SendMsg(&msg,sizeof(msg));
			return eItemUseErr_Err;
		}
	}
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
	{
		if( !(pItemInfo->ItemKind == eYOUNGYAK_ITEM)
			&& !(pItemInfo->ItemKind & eEQUIP_ITEM)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_MAKEUP)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_DECORATION)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_EQUIP)
			)
			return eItemUseErr_Err;

		if( pItemInfo->ItemKind == eYOUNGYAK_ITEM )
		{
			if( SVVMODEMGR->GetCurModeState() == eSVVMode_Fight )
			if( FALSE == SVVMODEMGR->AddItemUsingCount(pPlayer) )
				return eItemUseErr_Err;
		}
	}

	if(pItemInfo->ItemKind & eYOUNGYAK_ITEM)
	{
//#ifdef _TL_LOCAL_
//		if( g_pServerSystem->GetMap()->IsVillage() )
//		if( pPlayer->GetBattle()->GetBattleKind() ==eBATTLE_KIND_VIMUSTREET || PARTYWARMGR->IsMemberInPartyWar( pPlayer ) > 0 )
//		{
//			return eItemUseErr_Err;
//		}
//#endif

		if( g_pServerSystem->GetMapNum() == PKEVENTMAP )
			return eItemUseErr_Err;

		if(pItemBase->Durability == 0)
			return eItemUseErr_Err;

		if(pItemInfo->ItemIdx == 10700)
		{
			if(pPlayer->GetSingleSpecialState(eSingleSpecialState_Detect))
				return eItemUseErr_Err;

            SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(eSingleSpecialState_DetectItem);

			if(pInfo)
			{
				DWORD time = pInfo->DurationTime;

				pPlayer->StartSpecialState(eSpecialState_DetectItem, time, 0, 0, 0, 0, 0);
			}
			else
				return eItemUseErr_Err;
		}

		if(pItemInfo->ItemKind == eYOUNGYAK_ITEM_TITAN)
		{
			if( FALSE == pPlayer->InTitan() )
				return eItemUseErr_Err;

			int rt = pPlayer->GetTitanManager()->ApplyYoungYakItemToCurTitan(pItemInfo);
			if( rt == eItemUseSuccess )
			{
                if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
				{
                    return eItemUseErr_Err;
				}
			}
			return rt;
		}

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		if(pItemInfo->ItemKind == eYOUNGYAK_ITEM_PET)
		{
			pPlayer->GetPetManager()->FeedUpPet(pItemInfo->LifeRecover);
			return eItemUseSuccess;
		}		

		BYTE bEffectKind = 0;

		if(pItemInfo->LifeRecover>0)
		{
			WORD plusLife = pItemInfo->LifeRecover;

			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fLifeRecoverRate) )
			{
				WORD wSetItemLifeRate = (WORD)(pPlayer->GetSetItemStats()->fLifeRecoverRate * 100);
				plusLife += (WORD)(plusLife*((pPlayer->GetAvatarOption()->RecoverRate + wSetItemLifeRate)*0.01));
			}

			if(pPlayer->GetSetItemStats()->wLifeRecover > 0)
			{
				plusLife += pPlayer->GetSetItemStats()->wLifeRecover;
			}

			CCharacterCalcManager::StartUpdateLife( pPlayer, plusLife );

			bEffectKind = 1;
		}
 		if(pItemInfo->LifeRecoverRate>0)
		{
			WORD plusLife = (WORD)(pPlayer->GetMaxLife()*pItemInfo->LifeRecoverRate);
			
			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fLifeRecoverRate) )
			{
				WORD wSetItemLifeRate = (WORD)(pPlayer->GetSetItemStats()->fLifeRecoverRate * 100);
				plusLife += (WORD)(plusLife*((pPlayer->GetAvatarOption()->RecoverRate + wSetItemLifeRate)*0.01));
			}

			if(pPlayer->GetSetItemStats()->wLifeRecover > 0)
			{
				plusLife += pPlayer->GetSetItemStats()->wLifeRecover;
			}

			CCharacterCalcManager::StartUpdateLife( pPlayer, plusLife );

			bEffectKind = 1;
		}

		if(pItemInfo->NaeRyukRecover>0)	
		{
			WORD plusNaeryuk = pItemInfo->NaeRyukRecover;

			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fNaeRyukRecoverRate) )
			{
				WORD wSetItemNaeRyuk = (WORD)(pPlayer->GetSetItemStats()->fNaeRyukRecoverRate * 100);
				plusNaeryuk += (WORD)(plusNaeryuk*((pPlayer->GetAvatarOption()->RecoverRate+wSetItemNaeRyuk)*0.01));
			}

			if(pPlayer->GetSetItemStats()->wNaeRyukRecover > 0)
			{
				plusNaeryuk += pPlayer->GetSetItemStats()->wNaeRyukRecover;
			}

			if( pPlayer->GetAvatarOption()->ShieldRecoverRate )
			{
				WORD addshield = (WORD)(pItemInfo->NaeRyukRecover*(pPlayer->GetAvatarOption()->ShieldRecoverRate*0.01));
				CCharacterCalcManager::StartUpdateShield( pPlayer, addshield, 5, 1500 );
			}			

			CCharacterCalcManager::StartUpdateNaeRyuk( pPlayer, plusNaeryuk );

			bEffectKind = 2;
		}
		if(pItemInfo->NaeRyukRecoverRate>0)	
		{
			WORD plusNaeryuk = (WORD)(pPlayer->GetMaxNaeRyuk()*pItemInfo->NaeRyukRecoverRate);
			
			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fNaeRyukRecoverRate) )
			{
				WORD wSetItemNaeRyuk = (WORD)(pPlayer->GetSetItemStats()->fNaeRyukRecoverRate * 100);
				plusNaeryuk += (WORD)(plusNaeryuk*((pPlayer->GetAvatarOption()->RecoverRate+wSetItemNaeRyuk)*0.01));
			}

			if(pPlayer->GetSetItemStats()->wNaeRyukRecover > 0)
			{
				plusNaeryuk += pPlayer->GetSetItemStats()->wNaeRyukRecover;
			}

			if( pPlayer->GetAvatarOption()->ShieldRecoverRate )
			{
				WORD addshield = (WORD)((pPlayer->DoGetMaxNaeRyuk()*pItemInfo->NaeRyukRecoverRate)*(pPlayer->GetAvatarOption()->ShieldRecoverRate*0.01));
				CCharacterCalcManager::StartUpdateShield( pPlayer, addshield, 5, 1500 );
			}

			CCharacterCalcManager::StartUpdateNaeRyuk( pPlayer, plusNaeryuk );

			bEffectKind = 2;

		}

		if( pItemInfo->ItemIdx == 53216 || pItemInfo->ItemIdx == 53222 )
		{
//#ifdef _HK_LOCAL_
//			CCharacterCalcManager::StartUpdateShield( pPlayer, 1000, 5, 1500 );
//#else
			WORD addshied = (WORD)(pPlayer->GetMaxShield() - pPlayer->GetShield());
			CCharacterCalcManager::StartUpdateShield( pPlayer, addshied, 5, 1500 );
//#endif
			bEffectKind = 3;
		}

		if(pItemInfo->NaeRyuk>0)
			pPlayer->SetMaxNaeRyuk(pPlayer->GetMaxNaeRyuk()+pItemInfo->NaeRyuk);
		
		if(pItemInfo->GenGol>0)		
			pPlayer->AddGenGol(pItemInfo->GenGol);
		
		if(pItemInfo->MinChub>0)	
			pPlayer->AddMinChub(pItemInfo->MinChub);
		
		if(pItemInfo->CheRyuk>0)		
			pPlayer->AddCheRyuk(pItemInfo->CheRyuk);
		
		if(pItemInfo->SimMek>0)	
			pPlayer->AddSimMek(pItemInfo->SimMek);
		if(pItemInfo->Life>0)
		{
			pPlayer->SetMaxLife(pPlayer->GetMaxLife()+pItemInfo->Life);
		}
		if( pItemInfo->ItemIdx == 53102 || pItemInfo->ItemIdx == 53232 || pItemInfo->ItemIdx == 53234 )
		{
			WORD wPlus = 1000;
//#ifdef _HK_LOCAL_
//			if( pItemInfo->ItemIdx == 53232 )
//				wPlus = 400;
//			else if( pItemInfo->ItemIdx == 53234 )
//				wPlus = 400;
//#else
			if( pItemInfo->ItemIdx == 53232 )
				wPlus = 2000;
			else if( pItemInfo->ItemIdx == 53234 )
				wPlus = 400;
//#endif

			if( pPlayer->GetAvatarOption()->RecoverRate )
				wPlus += (WORD)(wPlus*(pPlayer->GetAvatarOption()->RecoverRate*0.1f));

			pPlayer->SetLife(pPlayer->GetLife()+wPlus);
			pPlayer->SetShield(pPlayer->GetShield()+wPlus);
			pPlayer->SetNaeRyuk(pPlayer->GetNaeRyuk()+wPlus);

			bEffectKind = 4;
		}
		else if(pItemInfo->ItemIdx == EVENTITEM_SHIELD_RECOVER || pItemInfo->ItemIdx == 53094 || pItemInfo->ItemIdx == 53109 )
		{
			pPlayer->SetShield(pPlayer->GetMaxShield());
			bEffectKind = 3;
		}
		else if(pItemInfo->ItemIdx == EVENTITEM_ALL_RECOVER || pItemInfo->ItemIdx == 53095
			|| pItemInfo->ItemIdx == 53103 || pItemInfo->ItemIdx == 53217 || pItemInfo->ItemIdx == 53223 )
		{
			pPlayer->SetLife(pPlayer->GetMaxLife());
			pPlayer->SetShield(pPlayer->GetMaxShield());
			pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk());
			bEffectKind = 4;
		}
		else if(pItemInfo->ItemIdx == 53074 )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}
		else if(pItemInfo->ItemIdx == EVENT_ITEM_FIRECRACKER )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}
		else if(pItemInfo->ItemIdx == 53151 )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}

		if( bEffectKind != 0 )
		if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET )	
		{
			MSG_DWORDBYTE msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_USE_NOTIFY;
			msg.dwData		= pPlayer->GetID();
			msg.bData		= bEffectKind;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
		}
	}
	else if(pItemInfo->ItemKind & eMUGONG_ITEM)
	{
		if(!CHKRT->LevelOf(pPlayer, pItemInfo->LimitLevel))
			return eItemUseErr_Err;
		if(!CHKRT->ItemStageOf(pPlayer, (BYTE)pItemInfo->LimitJob))
			return eItemUseErr_Err;
//#ifdef _JAPAN_LOCAL_
//		if( !CHKRT->ItemAttrOf(pPlayer, pItemInfo->wItemAttr) )
//			return eItemUseErr_Err;
//		if( pItemInfo->wAcquireSkillIdx1 )
//		{
//			if( !CHKRT->ItemAquireSkillOf( pPlayer, pItemInfo->wAcquireSkillIdx1, pItemInfo->wAcquireSkillIdx2 ) )
//				return eItemUseErr_Err;
//		}
//#endif
		if(CHKRT->MugongOf(pPlayer, pItemInfo->ItemIdx))
			return eItemUseErr_PreInsert;
		
		if(pItemInfo->ItemKind == eMUGONG_ITEM_KYUNGGONG)
		{
			if(KYUNGGONGMGR->IsSetableKyungGong(pPlayer,pItemInfo) == FALSE)
				return eItemUseErr_Err;
			ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYUPITEM_USE,pItemInfo->ItemIdx,pPlayer->GetAbilityGroup());
		}

		if( pItemInfo->ItemKind == eMUGONG_ITEM_TITAN )
		{
			if( pPlayer->InTitan() == FALSE )
                return eItemUseErr_Err;
		}

//#ifdef _JAPAN_LOCAL_
//		if( pItemInfo->wDeleteSkillIdx )
//		{
//			if( !MUGONGMNGR->DeleteSkill( pPlayer, pItemInfo->wDeleteSkillIdx ) )
//				return eItemUseErr_Err;
//		}
//#endif
		if(!MUGONGMNGR->AddMugong(pPlayer, pItemInfo->MugongNum,pItemInfo->ItemKind))
			return eItemUseErr_Err;

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());	
	}
	else if(pItemInfo->ItemKind & eEXTRA_ITEM)
	{
		if( pItemInfo->ItemIdx == EVENT_ITEM_SNOW || pItemInfo->ItemIdx == EVENT_ITEM_HARDSNOW )
		{
			pPlayer->SetEventSnowIdx(pItemInfo->ItemIdx);
		}
		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	else if( pItemInfo->ItemKind & eQUEST_ITEM )
	{
		if( pItemInfo->ItemKind == eQUEST_ITEM_PET )
		{
			if(pItemInfo->LimitLevel > pPlayer->GetMaxLevel())
			{
				return eItemUseErr_Err;
			}
			pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
			return eItemUseSuccess;
		}

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		if( pItemInfo->ItemKind == eQUEST_ITEM_START )
		{
			CQuestEvent qe;
			qe.SetValue( eQuestEvent_UseItem, pItemInfo->ItemIdx, pItemInfo->ItemType );
			QUESTMGR->AddQuestEvent( pPlayer, &qe );
		}

		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	else if( pItemInfo->ItemKind & eTITAN_ITEM )
	{
		if( pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		{
			if(pPlayer->GetState() != eObjectState_None)
				return eItemUseErr_Err;

			//2007. 10. 19. CBH - 公街 葛靛老锭 鸥捞藕 啪铰 角菩
			CHARACTER_TOTALINFO CTInfo;
			pPlayer->GetCharacterTotalInfo( &CTInfo );
			if(CTInfo.bMussangMode == TRUE)
			{
				return eItemUseErr_Err;
			}

			CTitanManager* pTitanManager = pPlayer->GetTitanManager();
			//pPlayer->GetTitanManager()->RideInTitan(pItemBase->dwDBIdx);
			if(pTitanManager->TitanRecallStartSyn(pItemBase->dwDBIdx) == TRUE)
			{
				pTitanManager->InitTitanRecall();

				DWORD dwRegistTitanItemDBIdx = pPlayer->GetTitanManager()->GetRegistTitanCallItemDBIdx();				
				if( dwRegistTitanItemDBIdx <= 0 )
					return eItemUseErr_Err;
				
				TITAN_TOTALINFO* pTitanInfo = pPlayer->GetTitanManager()->GetTitanTotalInfo(dwRegistTitanItemDBIdx);
				if( pTitanInfo == NULL )
					return eItemUseErr_Err;

				TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pTitanInfo->TitanKind, pTitanInfo->TitanGrade);
				if( pGrade == NULL )
					return eItemUseErr_Err;

				DWORD dwRecallProcessTime = (pGrade->dwRecallSpellTime - pTitanManager->GetTitanShopitemOption()->dwSummonReduceTime) + gCurTime;	// magi82(26)
				pTitanManager->SetRecallProcessTime(dwRecallProcessTime);
				pTitanManager->StartTitanRecall();
			}
			else
			{
				return eItemUseErr_Err;
			}

			return eItemUseSuccess;
		}
	}
	else
	{
		ASSERT(0);
		return eItemUseErr_Err;
	}

	return eItemUseSuccess;
}




BOOL CItemManager::EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum)
{
	DURTYPE deNeedItemNum = needItemNum;
	for(int i = 0 ; i < wMaterialNum ; ++i)
	{
		if(deNeedItemNum == 0) break;

		if(pMaterialArray[i].wItemIdx == needItemIdx)
		{
			if(pMaterialArray[i].Dur < deNeedItemNum)
			{
				if(!IsDupItem(pMaterialArray[i].wItemIdx))
					deNeedItemNum = deNeedItemNum -1;
				else 
					deNeedItemNum -= pMaterialArray[i].Dur;
			}
			else
			{
				deNeedItemNum = 0;
			}
		}
	}

	if(deNeedItemNum == 0)
		return TRUE;
	return FALSE;
}

BOOL CItemManager::EnoughDissolveSpace(CPlayer * pPlayer, WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * pJewelArray, WORD & wJewelNum)
{
	ASSERT(wJewelNum<=MAX_DISSOLVEGRID_NUM);
	if(!EnoughDissolveInvSpace(pPlayer, wJewelNum))
		return FALSE;
	else
		return TRUE;
}

BOOL CItemManager::EnoughDissolveInvSpace(CPlayer * pPlayer, WORD wJewelNum)
{
	for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && wJewelNum != 0 ; ++i)
	{
		CItemSlot * pSlot = pPlayer->GetSlot(i);
		BOOL bIsEmpty = pSlot->IsEmpty(i);
		if(bIsEmpty)
		{
			--wJewelNum;
		}
	}

	if(wJewelNum == 0)
		return TRUE;
	else
		return FALSE;
}

int CItemManager::MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan)	// magi82 - Titan(070118)
{
	if(!bTitan)
	{
		if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Mix))
			return 1;
	}

	if(!CHKRT->ItemOf(pPlayer, BasicItemPos, wBasicItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

	eITEM_KINDBIT eBit = GetItemKind(wBasicItemIdx);
	if(eBit == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(wBasicItemIdx)->MixFlag == 0)
	{
		return 2;
	}

	ITEM_INFO* pShopItem = NULL;
	if( ShopItemIdx )
	{
		if( !CHKRT->ItemOf(pPlayer, ShopItemPos, ShopItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
			return 20;
		pShopItem = GetItemInfo( ShopItemIdx );
		if( !pShopItem )		return 2;
	}

	int i = 0 ;
	for(i = 0 ; i < wMaterialNum ; ++i)
	{
		if( BasicItemPos == pMaterialArray[i].ItemPos )
			return 3;
		if(!CHKRT->ItemOf(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur,0, CB_EXIST|CB_ICONIDX|CB_ENOUGHDURA))
			return 3;
	}

	ITEM_MIX_INFO * pMixInfo = NULL;
	if(bTitan)
        pMixInfo = TITANITEMMGR->GetTitanMixItemInfo(wBasicItemIdx);
	else
		pMixInfo = GetMixItemInfo(wBasicItemIdx);
	//////////////////////////////////////////////////////////////////////////
	if(!pMixInfo)
		return 4;

	if( pMixInfo->wResultItemNum <= ResultIdx )	return 4;
	
	
	ITEMBASE * pBasicItemBase = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, BasicItemPos);
	if(!pBasicItemBase) return 7;

	BOOL bIsCanMix = FALSE;
	bIsCanMix = CanMixItem(pBasicItemBase->wIconIdx);

	if( GetItemKind(pBasicItemBase->wIconIdx) != eYOUNGYAK_ITEM &&
		GetItemKind(pBasicItemBase->wIconIdx) != eEXTRA_ITEM_JEWEL &&
		!bIsCanMix&&
		(pBasicItemBase->Durability != 1&&!IsEquipItem(GetItemKind(pBasicItemBase->wIconIdx))) )
	{
		return 7;		
	}
	WORD ItemStatic = pBasicItemBase->ItemStatic;//[泡点组合加锁][By:十里坡剑神][QQ:112582793][2017/12/16]
	MONEYTYPE money;
	if(IsDupItem(wBasicItemIdx))
		money = pMixInfo->psResultItemInfo[ResultIdx].Money * pBasicItemBase->Durability;
	else
		money = pMixInfo->psResultItemInfo[ResultIdx].Money;

	if( pPlayer->GetMoney() < money )
		return 6;

	WORD needItemIdx=0;
	WORD needItemNum=0;

	for(i = 0 ; i < pMixInfo->psResultItemInfo[ResultIdx].wMaterialItemNum ; ++i)
	{
		needItemIdx = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemIdx;
		needItemNum = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemNum;
		if(IsDupItem(wBasicItemIdx))
			needItemNum *= (WORD)pBasicItemBase->Durability;
		if(!EnoughMixMaterial(needItemIdx, needItemNum, pMaterialArray, wMaterialNum))
			return 5;
	}

	ITEM_INFO* pNewItemInfo = GetItemInfo( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx );
	if (pNewItemInfo->ItemKind&eSHOP_ITEM)
	{
		CItemSlot *pSlot1= pPlayer->GetSlot(eItemTable_ShopInven);
		if(pSlot1==NULL) 
		{
			g_Console.LOG(4,"组合获取百宝栏信息失败");
			return 68;
		}
		unsigned int nNum= ITEMMGR->GetTotalEmptySlotNum(pSlot1);
		if(nNum==0)
			return 68;
	}
	if( ShopItemIdx )
	{
		if( !pNewItemInfo )			return 2;
		if( pNewItemInfo->LimitLevel < pShopItem->GenGol || pNewItemInfo->LimitLevel > pShopItem->MinChub )
			return 23;
	}

	DWORD ratio = 0;
	WORD mixGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMMIX, pPlayer->GetAbilityGroup());
	WORD per = MAX_MIX_PERCENT/100;

	ITEM_INFO* pResItemInfo = GetItemInfo(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx);
	if( pResItemInfo->ItemGrade >= 10 )
	{
		HILEVEL_ITEM_MIX_RATE_INFO* pRateInfo = m_HiLevelItemMixRateInfoList.GetData( pResItemInfo->ItemGrade );
		if(pRateInfo)
			ratio = pRateInfo->dwBaseRate + ( mixGrade * 10000 / pRateInfo->dwModulate );
		else
			ratio = (pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio + mixGrade*per);
	}
	else
		ratio = (pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio + mixGrade*per);

//#ifdef _HK_LOCAL_
//	if( pNewItemInfo )
//	if( pNewItemInfo->ItemGrade > 0 && pNewItemInfo->ItemGrade <= 9 )
//	if( pNewItemInfo->LimitLevel > 0 && pNewItemInfo->LimitLevel <= 99 )
//	{
//		if( m_nItemMixBal[pNewItemInfo->LimitLevel-1][pNewItemInfo->ItemGrade-1] )
//		{
//			ratio = pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio
//			+ mixGrade * per * m_nItemMixBal[pNewItemInfo->LimitLevel-1][pNewItemInfo->ItemGrade-1] / 3000;
//		}
//	}
//#endif
//
//#ifdef _JAPAN_LOCAL_
//	ITEM_INFO* pItem = GetItemInfo( wBasicItemIdx );
//	if( pItem )
//	{
//		if( pItem->LimitLevel > pPlayer->GetMaxLevel() )
//		{
//			ratio = ratio / 100 * ( 100 - ( pItem->LimitLevel - pPlayer->GetMaxLevel() ) );
//		}
//		if( pItem->LimitLevel > 60 )
//		{
//			ratio = ratio / 100 * ( 100 - ( pItem->LimitLevel - 60 ) * 2 );
//		}
//	}
//	else
//		ratio = 0;
//#endif	// _JAPAN_LOCAL_


	DWORD Key = MakeNewKey();
	
	ITEMBASE ItemBase;
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0); 

	BOOL bUseMixUp = FALSE;
	BOOL bSuccess = FALSE;
	BOOL bProtection = FALSE;
	DWORD Seed;
	if( pResItemInfo->ItemGrade >= 10 )
		Seed = LargeRandom( 0, MAX_MIX_LARGE_PERCENT );
	else
		Seed = random(0, MAX_MIX_PERCENT);
	
	if( pPlayer->GetShopItemStats()->ItemMixSuccess )
	{
		SHOPITEMWITHTIME* ShopInfo = pPlayer->GetShopItemManager()->GetUsingItemInfo(55322);
		//if (ShopInfo)
		//{//[泡点组合加锁][By:十里坡剑神][QQ:112582793][2017/12/16]
		//	if(ShopInfo->ShopItem.ItemBase.ItemStatic == 1)
		//	{
		//		ItemStatic=ShopInfo->ShopItem.ItemBase.ItemStatic;
		//	}
		//}
			
		ratio += (WORD)(MAX_MIX_PERCENT*0.1f);
		pPlayer->GetShopItemManager()->CalcShopItemOption( eIncantation_MixUp, FALSE );
		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer( MP_ITEM_SHOPITEM_MIXUP, eIncantation_MixUp );
		pPlayer->GetShopItemManager()->DeleteUsingShopItem( eIncantation_MixUp );

		bUseMixUp = TRUE;
	}

	if(Seed < ratio)		
		bSuccess = TRUE;

	//如果组合的等级>=10的时候,强制成功100%
	if ( pResItemInfo->ItemGrade >= 10)
	{
		bSuccess = TRUE;
	}

	if( !bSuccess && ShopItemIdx )
	{
		bProtection = TRUE;		
	}
	else
	{
		for(i = 0 ; i < wMaterialNum ; ++i)
		{
			CItemSlot * pSlotT = NULL;
			if (pMaterialArray[i].ItemPos >= TP_INVENTORY_START&&pMaterialArray[i].ItemPos < TP_INVENTORY_END)
				pSlotT = pPlayer->GetSlot(eItemTable_Inventory);
			else if (pMaterialArray[i].ItemPos >= TP_SHOPINVEN_START&&pMaterialArray[i].ItemPos < TP_SHOPINVEN_END)
				pSlotT = pPlayer->GetSlot(eItemTable_ShopInven);

			Item = *pSlotT->GetItemInfoAbs(pMaterialArray[i].ItemPos);
			if(EI_TRUE != DiscardItem(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur))
				return 10;

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyMix, 
				Item.Durability, Item.Durability - pMaterialArray[i].Dur, pMaterialArray[i].Dur, Item.wIconIdx, Item.dwDBIdx, 
				Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());
		}
		pPlayer->SetMoney(money, MONEY_SUBTRACTION);
	}

	if( ShopItemIdx )
	{
		const ITEMBASE* pShop = GetItemInfoAbsIn( pPlayer, ShopItemPos );
		//if(pShop)
		//{//[泡点组合加锁][By:十里坡剑神][QQ:112582793][2017/12/16]
		//	if (pShop->ItemStatic==1)
		//	{
		//		ItemStatic = 1;
		//	}
		//}
		if( pShopItem &&pShop)
		{
			if( EI_TRUE != DiscardItem( pPlayer, ShopItemPos, ShopItemIdx, 1 ) )
				ASSERT(0);
			
			SEND_SHOPITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;		
			msg.ShopItemPos = ShopItemPos;
			msg.ShopItemIdx = ShopItemIdx;
			SendAckMsg(pPlayer, &msg, sizeof(msg));
			
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse, 
				pPlayer->GetMoney(), pPlayer->GetMoney(), 0, ShopItemIdx, pShop->dwDBIdx, ShopItemPos, 0, 
				Item.Position, pPlayer->GetPlayerExpPoint());
		}
	}
		
	if( bSuccess )
	{
		ITEM_INFO* pinfo = GetItemInfo( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx );
		if( !pinfo )
			return 2;

		CItemSlot * pSlot = pPlayer->GetSlot(BasicItemPos);
		if( pinfo->ItemKind & eSHOP_ITEM )
		{
			if( EI_TRUE != DiscardItem( pPlayer, BasicItemPos, wBasicItemIdx, 1 ) )	
				return 2;
			if (ItemStatic==1)
				ObtainItemFromChangeItem( pPlayer, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx,1, ItemStatic );
			else
				ObtainItemFromChangeItem( pPlayer, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx,1, 2 );

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pinfo->ItemIdx, 0, 0, 0, 1,
				pPlayer->GetPlayerExpPoint());

		}
		else
		{
			WORD flag = UB_ICONIDX;
			if( !IsDupItem( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx ) )
				flag |= UB_DURA;

			if(EI_TRUE != pSlot->UpdateItemAbs(pPlayer,BasicItemPos, 0, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx,0,0,0,ItemStatic,flag, SS_LOCKOMIT))
				return 6;

			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(BasicItemPos);

			if(ItemStatic==1)
				ItemLockUpdate(pPlayer->GetID(),pItemBase->dwDBIdx,1);

			if(pItemBase->RareIdx < 0)
			{
				char temp[128];
				sprintf(temp, "RareIdxError!(<0) ItemDBIdx : %d, PlayerID : %d", pItemBase->dwDBIdx, pPlayer->GetID() );
				ASSERTMSG(0, temp);
			}
			if( pItemBase->RareIdx < 0 )
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, 0,ItemStatic );

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
				pPlayer->GetPlayerExpPoint());			
			}
			else
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx,ItemStatic);

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
				pPlayer->GetPlayerExpPoint());
			}
		}

		return 0;
	}
	else
	{
		Seed = random(0, MAX_MIX_PERCENT);
		ItemBase = *pSlot->GetItemInfoAbs(BasicItemPos);
	
//#ifdef _CHINA_LOCAL_
//		eITEM_KINDBIT ItemKind = GetItemKind( ItemBase.wIconIdx );
//		if( ItemKind & eEQUIP_ITEM )
//		{
//			if( ItemKind == eEQUIP_ITEM_WEAPON || ItemKind == eEQUIP_ITEM_DRESS ||
//				ItemKind == eEQUIP_ITEM_HAT || ItemKind == eEQUIP_ITEM_SHOES )
//			{
//				if( ItemBase.wIconIdx % 10 < 3 )
//				{
//					LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixFail, 
//						pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 
//						ItemBase.Position, ItemBase.Durability, pPlayer->GetPlayerExpPoint());
//					return 1001;
//				}
//			}
//		}
//#endif
		float fBicFail_Prob;

		int eBigFail_Kind;
		int eFail_Kind;
		DWORD dwOptionIdx;
		
		if(!ItemBase.RareIdx)
		{
			fBicFail_Prob = 0.2f;
			eBigFail_Kind = eLog_ItemMixBigFail;
			eFail_Kind = eLog_ItemMixFail;
			dwOptionIdx = ItemBase.Durability;		
		}
		else
		{
			fBicFail_Prob = 0.3f;
			eBigFail_Kind = eLog_ItemRareMixBigFail;
			eFail_Kind	= eLog_ItemRareMixFail;
			dwOptionIdx = ItemBase.RareIdx;
		}

		if( !bUseMixUp && Seed <= MAX_MIX_PERCENT*fBicFail_Prob)
		{
			if(EI_TRUE != DiscardItem(pPlayer, BasicItemPos, wBasicItemIdx, 9999))
				return 8;
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eBigFail_Kind, 
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 0, 
				dwOptionIdx, pPlayer->GetPlayerExpPoint());

			if( bProtection )
				return 21;
			else
				return 1000;
		}
		else
		{
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eFail_Kind, 
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 
				ItemBase.Position, dwOptionIdx, pPlayer->GetPlayerExpPoint());

			if( bProtection )
				return 22;
			else
				return 1001;
		}
	}
	
	return 11;
}

const ITEMBASE * CItemManager::GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos)
{
	CItemSlot * pSlot = pPlayer->GetSlot(Pos);
	if(!pSlot) return NULL;
	return pSlot->GetItemInfoAbs(Pos);
}
BOOL CItemManager::IsSameUseItem(ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum)
{
	for(int i = 0;i<wJewelNum;i++)
	{
		if(pJewelInfo[0].wItemIdx!=pJewelInfo[i].wItemIdx)
			return FALSE;
	}
	return  TRUE;
}
int CItemManager::ReinforceItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum)
{
	if(wJewelNum>1)
	{
		if (!IsSameUseItem(pJewelInfo,wJewelNum))
			return 64;
	}
		
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Reinforce))
		return 1;

	if(!CHKRT->ItemOf(pPlayer, TargetPos, wTargetItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

	int i = 0 ;
	for(i = 0 ; i < wJewelNum ; ++i)
	{
		if(!CHKRT->ItemOf(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, pJewelInfo[i].Dur,0, CB_EXIST|CB_ICONIDX))
			return 3;
	}

	const ITEMBASE * pTargetItemBase = GetItemInfoAbsIn(pPlayer, TargetPos);
#ifdef _ABRASION_
	if (pTargetItemBase->ItemAbrasion<=0)
		return 265;//[持久为0不能强化][By:十里坡剑神][QQ:112582793][2018/1/6]
#endif
	ITEM_OPTION_INFO* pOptionInfo = pPlayer->GetItemOption( pTargetItemBase->Durability );
	int eOptKind = eIOK_Normal;	

	if(IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))	
	{
		eOptKind = eIOK_Rare;
	}
	
	int jnum = 0;
	BOOL bFound = FALSE;
	float ReinforceBackup=-1;
	//[是否签名判断][By:十里坡剑神][QQ:112582793][2018/1/6]
	BOOL bSignature = FALSE;
	ITEM_JEWEL_POS_EX jewelExInfo[MAX_ITEM_OPTION_NUM];
	for( i = 0 ; i < wJewelNum ; ++i)
	{
		const ITEMBASE * pJewelItemBase = GetItemInfoAbsIn(pPlayer, pJewelInfo[i].pos);
		
		if( REINFORCEMGR->CheckValidMaterial( pTargetItemBase->wIconIdx, pJewelItemBase->wIconIdx, eOptKind ) == FALSE )	//@措惑 酒捞袍狼 碍拳 可记俊 嘎绰 犁丰牢瘤 犬牢
			return 8;

		if( REINFORCEMGR->CheckMaterialTotalGravity( pJewelItemBase->wIconIdx, (WORD)pJewelItemBase->Durability, eOptKind) == FALSE )	//犁丰狼 醚厚吝阑 眉农
			return 20;
		
		float res = REINFORCEMGR->GetReinforceWithMetariel( pOptionInfo, pJewelItemBase->wIconIdx, eOptKind);
		if( res != 0 )
		{
			if( ReinforceBackup != res )		
			{
				if( ReinforceBackup == -1 )			ReinforceBackup = res;
				else								return 5;
			}
		}

		int j = 0 ;
		for(j = 0 ; j < jnum ; ++j)
		{
			if(jewelExInfo[j].wItemIdx == pJewelInfo[i].wItemIdx)
			{
				bFound ^= TRUE;
				break;
			}
		}

		if(bFound)
		{
			jewelExInfo[j].Dur += pJewelItemBase->Durability;

			if( pJewelItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM || jewelExInfo[j].Dur > 100 )
				return 6;

			bFound ^= TRUE;
		}
		else
		{
			jewelExInfo[jnum].wItemIdx = pJewelInfo[i].wItemIdx;
			jewelExInfo[jnum].pos = pJewelInfo[i].pos;
			jewelExInfo[jnum].Dur = pJewelItemBase->Durability;
			++jnum;

		}
	}
	REINFORCEMGR->InitGravity();	


	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo, 0, sizeof(OptionInfo));
	WORD reinforceGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMREINFORCE, pPlayer->GetAbilityGroup());

	if( reinforceGrade == 0 || reinforceGrade > 30 )
		return 7;

	BOOL bConvert = FALSE;

	ITEM_INFO* pBaseItemInfo = GetItemInfo( pTargetItemBase->wIconIdx );
	if( !pBaseItemInfo )
		return 4;
	
	for(i = 0 ; i < jnum ; ++i)
	{
		sITEM_REINFORCE_INFO* pReInforceInfo = NULL;
		if(eOptKind == eIOK_Normal)
		{
			pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[i].wItemIdx );
		}
		else if(eOptKind == eIOK_Rare)
		{
			pReInforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( jewelExInfo[i].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
		}
		ASSERTMSG(pReInforceInfo, "甘 辑滚惑俊 碍拳 沥焊啊 绝促");
		//WORD ItemLimitLevel = REINFORCEMGR->GetItemLevelLimit(pBaseItemInfo->LimitLevel);
		int successSeed = random(0, 100) - (pBaseItemInfo->LimitLevel/2) + reinforceGrade;
		if(successSeed < 0)	successSeed = 0;
		float plusValue = pReInforceInfo->fw*jewelExInfo[i].Dur*successSeed/100;
		
//#ifdef _JAPAN_LOCAL_
//		if( pBaseItemInfo->LimitLevel > pPlayer->GetMaxLevel() )
//		{
//			int nLevel = pBaseItemInfo->LimitLevel - pPlayer->GetMaxLevel();
//			plusValue = plusValue / 100 * (100 - nLevel * nLevel / 5 );
//			if( plusValue < 0.0f )	plusValue = 0.0f;
//		}
//		if( plusValue < 1.0f )	plusValue = 0.0f;
//#endif
		if (plusValue>ReinforceBackup)
			bSignature = TRUE;//[是否签名判断][By:十里坡剑神][QQ:112582793][2018/1/6]
		switch(pReInforceInfo->ReinforceType)
		{
		case eRJP_GENGOL:
			{
				OptionInfo.GenGol = (WORD)plusValue;
				if(OptionInfo.GenGol > 0) bConvert = TRUE;
			}
			break;
		case eRJP_MINCHUB:
			{
				OptionInfo.MinChub = (WORD)plusValue;
				if(OptionInfo.MinChub > 0) bConvert = TRUE;
			}
			break;
		case eRJP_CHERYUK:
			{
				OptionInfo.CheRyuk = (WORD)plusValue;
				if(OptionInfo.CheRyuk > 0) bConvert = TRUE;
			}
			break;
		case eRJP_SIMMEK:
			{
				OptionInfo.SimMek = (WORD)plusValue;
				if(OptionInfo.SimMek > 0) bConvert = TRUE;
			}
			break;
		case eRJP_LIFE:
			{
				OptionInfo.Life = (DWORD)plusValue;
				if(OptionInfo.Life > 0) bConvert = TRUE;
			}
			break;
		case eRJP_NAERYUK:
			{
				OptionInfo.NaeRyuk = (WORD)plusValue;
				if(OptionInfo.NaeRyuk > 0) bConvert = TRUE;
			}
			break;
		case eRJP_SHIELD:
			{
				OptionInfo.Shield = (DWORD)plusValue;
				if(OptionInfo.Shield > 0) bConvert = TRUE;
			}
			break;
		case eRJP_FIREREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_WATERREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, plusValue);
					bConvert = TRUE;
				}
				
			}
			break;
		case eRJP_TREEREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_GOLDREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_EARTHREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_PHYATTACK:
			{
				OptionInfo.PhyAttack = (WORD)plusValue;
				if(OptionInfo.PhyAttack > 0) bConvert = TRUE;
			}
			break;
		case eRJP_CRITICAL:
			{
				OptionInfo.CriticalPercent = (WORD)plusValue;
				if(OptionInfo.CriticalPercent > 0) bConvert = TRUE;
			}
			break;		
		case eRJP_FIREATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, plusValue);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_WATERATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, plusValue);
				}
			}
			break;	
		case eRJP_TREEATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, plusValue);
				}
			}
			break;	
		case eRJP_GOLDATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, plusValue);
				}
			}
			break;	
		case eRJP_EARTHATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, plusValue);
				}
			}
			break;	
		case eRJP_PHYDEFENSE:
			{
				OptionInfo.PhyDefense = (WORD)plusValue;
				if(OptionInfo.PhyDefense > 0) bConvert = TRUE;
			}
			break;	
		default:
			{
				return 3;
			}
		}
	}

	DWORD Key = MakeNewKey();
	
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0);

	for(i = 0 ; i < wJewelNum ; ++i)
	{//[删除强化材料][By:十里坡剑神][QQ:112582793][2018/1/6]
		Item = *pSlot->GetItemInfoAbs(pJewelInfo[i].pos);
		if(EI_TRUE != DiscardItem(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, 9999))
			return 5;

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyReinforce, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, Item.wIconIdx, Item.dwDBIdx, Item.Position, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	
	if(bConvert)
	{
		if(pTargetItemBase->Durability != 0)
			CharacterItemOptionDelete(pTargetItemBase->Durability, pTargetItemBase->dwDBIdx);
		
		CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
		if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, 0,pTargetItemBase->ItemStatic, UB_DURA, SS_PREINSERT|SS_LOCKOMIT ) != EI_TRUE )
		{
			return 6; 
		}
		OptionInfo.dwItemDBIdx = pTargetItemBase->dwDBIdx;
		ItemOptionInsertToDB(pPlayer->GetID(), wTargetItemIdx, TargetPos, &OptionInfo);
		if (bSignature)
		{//[制造者签名][By:十里坡剑神][QQ:112582793][2018/1/6]
			if (strcmp(pPlayer->GetObjectName(),pTargetItemBase->ItemMaker1)!=0)
				ItemMaker1Update(pPlayer->GetID(),pTargetItemBase->dwDBIdx,pPlayer->GetObjectName());
		}
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceSuccess, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
	}
	else            
	{
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceFail, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
		return 99;
	}

	return 0;
}


int CItemManager::ReinforceItemWithShopItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, WORD wShopItemIdx, POSTYPE ShopItemPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum)
{
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Reinforce))
		return 1;
	ITEMBASE* pTargetItemBase = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, TargetPos );
	if( !pTargetItemBase )			return 2;
#ifdef _ABRASION_
	if (pTargetItemBase->ItemAbrasion<=0)
		return 256;//[持久为0不能强化][By:十里坡剑神][QQ:112582793][2018/1/6]
#endif
	ITEM_OPTION_INFO* pOptionInfo = pPlayer->GetItemOption( pTargetItemBase->Durability );
	if( pOptionInfo->GetOptionType() == 0 )
		return 3;
	//[是否签名判断][By:十里坡剑神][QQ:112582793][2018/1/6]
	BOOL bSignature = FALSE;
	const ITEMBASE* pShop = GetItemInfoAbsIn( pPlayer, ShopItemPos );
	//if (pShop->ItemStatic==1)
	//{//[强化物品加锁][By:十里坡剑神][QQ:112582793][2017/12/16]
	//	ItemLockUpdate(pPlayer->GetID(),pTargetItemBase->dwDBIdx,1);
	//}

	if( !CHKRT->ItemOf( pPlayer, ShopItemPos, wShopItemIdx, 0, 0, CB_EXIST|CB_ICONIDX ) )
		return 12;
	
	int i = 0 ;
	for(i = 0 ; i < wJewelNum ; ++i)
	{
		if(!CHKRT->ItemOf(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, pJewelInfo[i].Dur,0, CB_EXIST|CB_ICONIDX))
			return 13;
	}
	ITEM_INFO* pShopItem = GetItemInfo( wShopItemIdx );
	if( !pShopItem )		return 12;
	ITEM_INFO* pBaseItemInfo = GetItemInfo( wTargetItemIdx );
	if( !pBaseItemInfo )			return 2;
	if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
		return 4;

	if( pBaseItemInfo->WeaponType > WP_KEY )
		return 12;

	int eOptKind = eIOK_Normal;	

	if(IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))
	{
		eOptKind = eIOK_Rare;
	}

	sITEM_REINFORCE_INFO* pReinforceInfo = NULL;	
	if(eOptKind == eIOK_Normal)
	{
		pReinforceInfo = REINFORCEMGR->GetReinforceInfo( pJewelInfo[0].wItemIdx );
	}
	else if(eOptKind == eIOK_Rare)
	{
		pReinforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( pJewelInfo[0].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
	}
	ASSERTMSG(pReinforceInfo, "甘 辑滚惑俊 碍拳 沥焊啊 绝促");
	if( !pReinforceInfo )	return 2;

	int jnum = 0;
	BOOL bFound = FALSE;
	DWORD jewelIdx = 0;
	float ReinforceBackup = -1;
	ITEM_JEWEL_POS_EX jewelExInfo[MAX_ITEM_OPTION_NUM];
	for( i = 0 ; i < wJewelNum ; ++i)
	{
		const ITEMBASE * pJewelItemBase = GetItemInfoAbsIn(pPlayer, pJewelInfo[i].pos);
		if( REINFORCEMGR->CheckValidMaterial( pTargetItemBase->wIconIdx, pJewelItemBase->wIconIdx, eOptKind ) == FALSE )
			return 11;

		if( REINFORCEMGR->CheckMaterialTotalGravity( pJewelItemBase->wIconIdx, (WORD)pJewelItemBase->Durability, eOptKind) == FALSE )
			return 20;

		float res = REINFORCEMGR->GetReinforceWithMetariel( pOptionInfo, pJewelItemBase->wIconIdx, eOptKind);
		if( res == 0 )						return 11;
		if( ReinforceBackup != res )		
		{
			if( ReinforceBackup == -1 )			ReinforceBackup = res;
			else								return 5;
		}

		if( jewelIdx && jewelIdx != pJewelItemBase->wIconIdx )
			return 6;
		else if( !jewelIdx )
			jewelIdx = pJewelItemBase->wIconIdx;

		int j=0;
		for(j=0; j<jnum; ++j)
		{
			if(jewelExInfo[j].wItemIdx == pJewelInfo[i].wItemIdx)
			{
				bFound ^= TRUE;
				break;
			}
		}

		if(bFound)
		{
			jewelExInfo[j].Dur += pJewelItemBase->Durability;
			if( pJewelItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM || jewelExInfo[j].Dur > 100 )
				return 6;

			bFound ^= TRUE;
		}
		else
		{
			jewelExInfo[jnum].wItemIdx = pJewelInfo[i].wItemIdx;
			jewelExInfo[jnum].pos = pJewelInfo[i].pos;
			jewelExInfo[jnum].Dur = pJewelItemBase->Durability;
			++jnum;
		}
	}
	REINFORCEMGR->InitGravity();

	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo, 0, sizeof(OptionInfo));
	WORD reinforceGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMREINFORCE, pPlayer->GetAbilityGroup());

	if( reinforceGrade == 0 || reinforceGrade > 30 )
		return 7;

	BOOL bConvert = FALSE;
	float plusValue = 0;
	sITEM_REINFORCE_INFO * pReInforceInfo = NULL;
	if(eOptKind == eIOK_Normal)
	{
		pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[0].wItemIdx );
	}
	else if(eOptKind == eIOK_Rare)
	{
		pReInforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( jewelExInfo[0].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
	}
	else
		ASSERT(0);
	
	int successSeed = random(0, 100) - (pBaseItemInfo->LimitLevel/2) + reinforceGrade;
	if(successSeed < 0)	successSeed = 0;
	plusValue = pReInforceInfo->fw*jewelExInfo[0].Dur*successSeed/100;

	switch( pReinforceInfo->ReinforceType )
	{
		case eRJP_FIREREGIST:
		case eRJP_WATERREGIST:
		case eRJP_TREEREGIST:
		case eRJP_GOLDREGIST:
		case eRJP_EARTHREGIST:
		case eRJP_FIREATTACK:
		case eRJP_WATERATTACK:
		case eRJP_TREEATTACK:
		case eRJP_GOLDATTACK:
		case eRJP_EARTHATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
			}
			break;
	}

	BOOL bProtection = FALSE;
	if( plusValue <= ReinforceBackup )
	{
		plusValue = ReinforceBackup;
		bProtection = TRUE;
	}
	else
		bSignature=TRUE;

	switch( pReInforceInfo->ReinforceType )
	{
	case eRJP_GENGOL:
		{
			OptionInfo.GenGol = (WORD)plusValue;
			if(OptionInfo.GenGol > 0) bConvert = TRUE;
		}
		break;
	case eRJP_MINCHUB:
		{
			OptionInfo.MinChub = (WORD)plusValue;
			if(OptionInfo.MinChub > 0) bConvert = TRUE;
		}
		break;
	case eRJP_CHERYUK:
		{
			OptionInfo.CheRyuk = (WORD)plusValue;
			if(OptionInfo.CheRyuk > 0) bConvert = TRUE;
		}
		break;
	case eRJP_SIMMEK:
		{
			OptionInfo.SimMek = (WORD)plusValue;
			if(OptionInfo.SimMek > 0) bConvert = TRUE;
		}
		break;
	case eRJP_LIFE:
		{
			OptionInfo.Life = (DWORD)plusValue;
			if(OptionInfo.Life > 0) bConvert = TRUE;
		}
		break;
	case eRJP_NAERYUK:
		{
			OptionInfo.NaeRyuk = (WORD)plusValue;
			if(OptionInfo.NaeRyuk > 0) bConvert = TRUE;
		}
		break;
	case eRJP_SHIELD:
		{
			OptionInfo.Shield = (DWORD)plusValue;
			if(OptionInfo.Shield > 0) bConvert = TRUE;
		}
		break;
	case eRJP_FIREREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_WATERREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, plusValue);
				bConvert = TRUE;
			}
			
		}
		break;
	case eRJP_TREEREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_GOLDREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_EARTHREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_PHYATTACK:
		{
			OptionInfo.PhyAttack = (WORD)plusValue;
			if(OptionInfo.PhyAttack > 0) bConvert = TRUE;
		}
		break;
	case eRJP_CRITICAL:
		{
			OptionInfo.CriticalPercent = (WORD)plusValue;
			if(OptionInfo.CriticalPercent > 0) bConvert = TRUE;
		}
		break;		
	case eRJP_FIREATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, plusValue);
				bConvert = TRUE;
			}
		}
		break;	
	case eRJP_WATERATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, plusValue);
			}
		}
		break;	
	case eRJP_TREEATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, plusValue);
			}
		}
		break;	
	case eRJP_GOLDATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, plusValue);
			}
		}
		break;	
	case eRJP_EARTHATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, plusValue);
			}
		}
		break;	
	case eRJP_PHYDEFENSE:
		{
			OptionInfo.PhyDefense = (WORD)plusValue;
			if(OptionInfo.PhyDefense > 0) bConvert = TRUE;
		}
		break;	
	default:
		{
			return 3;
		}
	}

	DWORD Key = MakeNewKey();
	
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0); 

	for(i = 0 ; i < wJewelNum ; ++i)
	{
		Item = *pSlot->GetItemInfoAbs(pJewelInfo[i].pos);
		if(EI_TRUE != DiscardItem(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, 9999))
			return 5;
		
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyReinforceWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, Item.wIconIdx, Item.dwDBIdx, Item.Position, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}


	if( pShopItem )
	{
		if( EI_TRUE != DiscardItem( pPlayer, ShopItemPos, wShopItemIdx, 1 ) )
			ASSERT(0);

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;		
		msg.ShopItemPos = ShopItemPos;
		msg.ShopItemIdx = wShopItemIdx;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, wShopItemIdx, pShop->dwDBIdx, ShopItemPos, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}

	if( bProtection )
	{
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceProtectWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());

		return 10;
	}	
	else if( bConvert )
	{
		if(pTargetItemBase->Durability != 0)
			CharacterItemOptionDelete(pTargetItemBase->Durability, pTargetItemBase->dwDBIdx);
		
		CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
		if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0,0,pShop->ItemStatic, UB_DURA, SS_PREINSERT|SS_LOCKOMIT  ) != EI_TRUE )
		{
			return 6; 
		}
		OptionInfo.dwItemDBIdx = pTargetItemBase->dwDBIdx;
		ItemOptionInsertToDB(pPlayer->GetID(), wTargetItemIdx, TargetPos, &OptionInfo);
		if (bSignature)
		{//[制造者签名][By:十里坡剑神][QQ:112582793][2018/1/6]
			if (strcmp(pPlayer->GetObjectName(),pTargetItemBase->ItemMaker1)!=0)
				ItemMaker1Update(pPlayer->GetID(),pTargetItemBase->dwDBIdx,pPlayer->GetObjectName());
		}

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceSuccessWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
	}
	else
	{
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceFailWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
		return 99;
	}

	return 0;
}

void CItemManager::ReinforceItemDBResult(CPlayer * pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION_INFO * pOptionInfo)
{
	CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
	if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, pOptionInfo->dwOptionIdx,0, UB_DURA) != EI_TRUE )
	{
		ASSERTMSG(0, "ㄏoI∷?i; ¨〓O⒋? ¨〓∷〓¨u¨∷?a!-¨u¨AIAU āㄏEㄏ PREINSERT⊥iE ¨u¨AIAU∷?∷㈢ dura update ∷?∷㈢⒋?");
		return ;
	}
	const ITEMBASE * pItemBase = pTargetSlot->GetItemInfoAbs(TargetPos);
	LogItemOption(pPlayer->GetID(), pItemBase->dwDBIdx, pOptionInfo);

	MSG_ITEM_REINFORCE_ACK msg;
	msg.Category							= MP_ITEM;
	msg.Protocol							= MP_ITEM_REINFORCE_SUCCESS_ACK;
	msg.dwObjectID							= pPlayer->GetID();
	msg.wTargetItemIdx						= wItemIdx;
	msg.TargetPos							= TargetPos;
	msg.OptionInfo							= *pOptionInfo;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CItemManager::RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);
	if (pArrayInfo == NULL) return;
	ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem(0);
	CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
	if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
	{
		ASSERT(0);
		return;
	}
	LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
		pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), 0,
		ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->RareIdx, pPlayer->GetPlayerExpPoint());

	if( pArrayInfo->wType == eLog_ItemRareObtainCheat )
	{
		LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemRareObtainCheat, 
			ItemInfo->wIconIdx, ItemInfo->dwDBIdx );
	}

	LogItemRareOption(pPlayer->GetID(), ItemInfo->dwDBIdx, pRareOptionInfo);

	MSG_ITEM_RAREITEM_GET msg;
	msg.Category = pArrayInfo->ItemArray.Category;	
	msg.Protocol = pArrayInfo->ItemArray.Protocol;	
	msg.dwObjectID = pArrayInfo->ItemArray.dwObjectID;	
	msg.RareItemBase = *pArrayInfo->ItemArray.GetItem(0);
	msg.RareInfo = *pRareOptionInfo;

	pPlayer->SendMsg(&msg, sizeof(msg));

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

void CItemManager::TitanUpgradeItemDBResult(CPlayer* pPlayer, WORD ArrayID)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);
	if (pArrayInfo == NULL) return;
	ASSERT(pArrayInfo->ItemArray.ItemNum == 0);
	ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem(0);
	CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
	if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
	{
		ASSERT(0);
		return;
	}

	LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
		pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), 0,
		ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->Durability, pPlayer->GetPlayerExpPoint());

	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );

	ItemInfo->ItemParam |= ITEM_PARAM_TITAN_REGIST;
	ShopItemParamUpdateToDB(pPlayer->GetID(), ItemInfo->dwDBIdx, ItemInfo->ItemParam);

	pPlayer->GetTitanManager()->CreateUpgradeTitan(pPlayer, ItemInfo->wIconIdx, ItemInfo->dwDBIdx);

	pPlayer->GetTitanManager()->SetRegistTitanCallItemDBIdx(ItemInfo->dwDBIdx);

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

int CItemManager::UpgradeItem(CPlayer* pPlayer, WORD & TargetItemIdx, POSTYPE TargetPos, WORD MatItemIdx, POSTYPE MatItemPos)
{
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Upgrade))
		return 1;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, TargetItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, MatItemPos, MatItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 3;

	ITEM_INFO * pTargetItem = GetItemInfo(TargetItemIdx);
	WORD upgradeGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMUPGRADE, pPlayer->GetAbilityGroup());

	int tmpValue = (int)((20 - ( (pTargetItem->LimitLevel / 2) + (pTargetItem->ItemGrade*18) ) + random(0, 300) + upgradeGrade*0.2 ));
	if(tmpValue<0)
		tmpValue -= 100;
	int gradeUp = tmpValue/100;


	if(gradeUp <-2)
	{
		gradeUp = -2;
	}
	else if(gradeUp > 3)
	{
		gradeUp = 3;
	}

	int tmpGrade = pTargetItem->ItemGrade + gradeUp;
	
	int var=0;
	if(tmpGrade>9)
		var = 9-pTargetItem->ItemGrade;
	else if(tmpGrade<0)
		var = -pTargetItem->ItemGrade;
	else
		var = gradeUp;

	TargetItemIdx += var;
	
	if(EI_TRUE != DiscardItem(pPlayer, MatItemPos, MatItemIdx, 9999))
		return 5;
	
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroyUpgrade, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		TargetItemIdx , 0, MatItemPos, 0, 0, pPlayer->GetPlayerExpPoint());

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);
	pSlot->UpdateItemAbs(pPlayer,TargetPos, 0, TargetItemIdx,0,0,0,0, UB_ICONIDX);
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);

	ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
					pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx,pItemBase->ItemStatic );

	return 0;
}



ITEM_INFO * CItemManager::GetItemInfo(WORD wItemIdx)
{
	if (wItemIdx == 0 || wItemIdx < 8000)
		return NULL;
	else
		return m_ItemInfoList.GetData(wItemIdx);
}
ITEM_MIX_INFO * CItemManager::GetMixItemInfo(WORD wItemIdx)
{
	std::map<WORD,ITEM_MIX_INFO>::iterator Iter;
	Iter = m_MixItemInfoList.find(wItemIdx);
	if(Iter!=m_MixItemInfoList.end())
		return &Iter->second;
	else
		return NULL;
}
ITEM_REINFORCE_INFO * CItemManager::GetReinforceItemInfo(WORD wItemIdx)
{
	return m_ReinforceItemInfoList.GetData(wItemIdx);
}
void CItemManager::LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1 );
	pResInfo->wMixKind = fp->GetWord();
	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	if(pResInfo->wMaterialItemNum!=0)
	{
		pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
		for( int i = 0 ; i < pResInfo->wMaterialItemNum ; ++i)
		{
			pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
			SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
			pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
		}
	}
}
void CItemManager::LoadMixList()
{
	CMHFile file;
	if(!file.Init("Resource/ItemMixList.bin", "rb"))
		return;


	WORD wTmpItemIdx = 0;

	WORD wTmpRestIdx = 0;
	WORD wTmpResultNum = 0;
	std::map<WORD,ITEM_MIX_INFO>::iterator Iter;
	while(1)
	{
		if(file.IsEOF())
			break;

		wTmpItemIdx = file.GetWord();
		Iter = m_MixItemInfoList.find(wTmpItemIdx);
		if(Iter!=m_MixItemInfoList.end())
		{
			SafeStrCpy(Iter->second.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			Iter->second.wResultItemNum =wTmpResultNum;
			for(wTmpRestIdx = 0 ;wTmpRestIdx < Iter->second.wResultItemNum; ++wTmpRestIdx)
			{
				if(Iter->second.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &Iter->second.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
		}
		else
		{
			ITEM_MIX_INFO  pInfo ;
			memset(&pInfo,0,sizeof(ITEM_MIX_INFO));
			pInfo.wItemIdx = wTmpItemIdx;
			SafeStrCpy(pInfo.szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
			wTmpResultNum = file.GetWord();
			pInfo.wResultItemNum = wTmpResultNum;		


			if(!pInfo.psResultItemInfo)
			{
				pInfo.psResultItemInfo = new ITEM_MIX_RES[pInfo.wResultItemNum];
				memset(pInfo.psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo.wResultItemNum);
			}
			for(wTmpRestIdx = 0 ; wTmpRestIdx < pInfo.wResultItemNum ; ++wTmpRestIdx)
			{	
				if(pInfo.psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
				{
					LoadMixResultItemInfo(&file, &pInfo.psResultItemInfo[wTmpRestIdx]);
					break;
				}
			}
			m_MixItemInfoList[pInfo.wItemIdx] = pInfo;
		}		
	}
	file.Release();
	if(!file.Init("Resource/Server/HiLevelItemMixRate.bin", "rb"))
		return;

	while(1)
	{
		if(file.IsEOF())
			break;

		HILEVEL_ITEM_MIX_RATE_INFO*	pRateInfo = NULL;
		
		pRateInfo = new HILEVEL_ITEM_MIX_RATE_INFO;
		ZeroMemory( pRateInfo, sizeof(HILEVEL_ITEM_MIX_RATE_INFO) );

		pRateInfo->wItemLevel	= file.GetWord();
		pRateInfo->dwBaseRate	= file.GetDword();
		pRateInfo->dwModulate	= file.GetWord();

		m_HiLevelItemMixRateInfoList.Add( pRateInfo, pRateInfo->wItemLevel );
	}
}

void CItemManager::LoadItemList()
{
	ITEM_INFO * pInfo = NULL;
	CMHFile file;
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;
	char buf[64];
	while(1)
	{
		if(file.IsEOF())
			break;

		WORD ItemIdx = file.GetWord();                   
		if(m_ItemInfoList.GetData(ItemIdx) == NULL)
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
		else
		{
			char temp[512];
			file.GetLine(temp,512);
			sprintf(buf,"LoadItemlist=%d,ItemName=%s",ItemIdx,m_ItemInfoList.GetData(ItemIdx)->ItemName);
			ASSERTMSG(0, buf);
		}
	}
	file.Release();

	if(!file.Init("Resource/HideItemLock.bin", "rb"))
		return;
	char buff[512]={0,};
	while(1)
	{
		if(file.IsEOF())
			break;

		pInfo = NULL;
		WORD Index = file.GetWord();
		file.GetString(buff);
		pInfo = m_ItemInfoList.GetData(Index);

		if(pInfo)
			m_UnfitItemInfoListForHide.Add(pInfo, pInfo->ItemIdx);
	}

	file.Release();
}

void CItemManager::ReloadItemList()
{
	ITEM_INFO * pInfo = NULL;
	CMHFile file;
	
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;

	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();

	while(1)
	{
		if(file.IsEOF())
			break;

		WORD ItemIdx = file.GetWord();
		pInfo = m_ItemInfoList.GetData(ItemIdx);
		if(pInfo)
		{
			SetItemInfo(ItemIdx,pInfo,&file);
		}
		else
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
	}
	file.Release();
}
void CItemManager::SendGoldShopItemInfo(CPlayer* pPlayer)
{
	MSGBASE msgBegin;
	msgBegin.Category = MP_ITEMEXT;
	msgBegin.Protocol = MP_ITEMEX_UPDATE_GOLDSHOP_BEGIN;
	pPlayer->SendMsg(&msgBegin,sizeof(msgBegin));

	SEND_GOLDSHOPINFO msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEX_UPDATE_GOLDSHOP_ING;
	Info_multimap iter = m_ShopItemInfo.begin();
	WORD wCount = 0;
	BOOL bNeedSend = FALSE;
	WORD wLocalCount = 0;
	while(1)
	{
		if(iter==m_ShopItemInfo.end())
			break;
		else
		{
			int Count = m_ShopItemInfo.count(iter->first);
			for(int i = 0;i!=Count;i++)
			{
				if(iter==m_ShopItemInfo.end())
					goto lreturn;
				else if(wCount==50)
				{
					wCount=0;
					pPlayer->SendMsg(&msg,sizeof(msg));
					wLocalCount+=50;
					bNeedSend = TRUE;
					memset(&msg.GoldShopItem,0,sizeof(msg.GoldShopItem));
					msg.GoldShopItem[wCount] = iter->second;
					wCount++;
				}
				else
				{
					bNeedSend = FALSE;
					msg.GoldShopItem[wCount] = iter->second;
					wCount++;
				}
				iter++;
			}
		}
	}
lreturn:
	if(!bNeedSend&&wLocalCount!=m_ShopItemInfo.size())
		pPlayer->SendMsg(&msg,sizeof(msg));
	MSGBASE msgEnd;
	msgEnd.Category = MP_ITEMEXT;
	msgEnd.Protocol = MP_ITEMEX_UPDATE_GOLDSHOP_END;
	pPlayer->SendMsg(&msgEnd,sizeof(msgEnd));
}
void CItemManager::ReloadGoldShopItem()
{
	if(m_ShopItemType.size()>0)
		m_ShopItemType.clear();
	if(m_ShopItemInfo.size()>0)
		m_ShopItemInfo.clear();
	LoadGoldShopItem();
	g_Console.LOG(4,"商品数量[%d]",m_ShopItemInfo.size());
}
void CItemManager::LoadShopItemTypeInfo()
{
	CMHFile file;
	if(file.Init("./Resource/GoldShopType.bin","rb")==FALSE) return;
	char buf[256] = {0,};
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buf);
		WORD t_wTmp = file.GetWord();
		m_ShopItemType[buf] = t_wTmp;
	}
	file.Release();
}
WORD CItemManager::GetShopItemType(char * name)
{
	std::map<std::string,WORD>::iterator iter;
	iter = m_ShopItemType.find(name);
	if(iter!=m_ShopItemType.end())
		return iter->second;
	else
	{
		ASSERTMSG(0,name);
		return 0;
	}
}
void CItemManager::LoadMixItemInfo()
{
	CMHFile file;
	if(file.Init("./Resource/MixItemInfo.bin","rb")==FALSE)
		return;
	char buff[64] = {0,};
	while(1)
	{
		if(file.IsEOF())
			break;
		WORD t_wTmp = file.GetWord();
		file.GetString(buff);
		m_MixItemInfo[t_wTmp] = buff;
	}
	file.Release();
}
BOOL CItemManager::CanMixItem(WORD wItemIndex)
{
	std::map<WORD,std::string>::iterator iter;
	iter = m_MixItemInfo.find(wItemIndex);
	if(iter!=m_MixItemInfo.end())
		return TRUE;
	else
		return FALSE;
}
//商城物品加载	 by:十里坡剑神	QQ:112582793
void CItemManager::LoadGoldShopItem()
{
	LoadShopItemTypeInfo();
	char buff[256]={0,};
	CMHFile file;
	if(file.Init("./Resource/GoldShopItemList.bin","rb")==FALSE)return;
	WORD t_MainIndex = 0;
	while(1)
	{
		file.GetString(buff);
		if(file.IsEOF())
			break;
		if(buff[0] == '@')
		{
			file.GetLineX(buff, 256);
			continue;
		}
		else if(buff[0]=='#')
		{
			GOLD_SHOP_ITEM_INFO pInfo;
			pInfo.wMainIndex	= GetShopItemType(buff);
			pInfo.wSubIndex	= file.GetWord();
			pInfo.wGridIndex	= file.GetWord();
			pInfo.wItemIndex	= file.GetWord();
			file.GetString(pInfo.strItemName);
			file.GetString(pInfo.strItemTip);
			pInfo.dwPrice		= file.GetDword();
			pInfo.wCurrency	= file.GetWord();
			pInfo.dwCanBuyTime	= file.GetDword();
			pInfo.bBunding	   = file.GetBool();
			std::pair<WORD,GOLD_SHOP_ITEM_INFO> p1(pInfo.wItemIndex,pInfo);
			m_ShopItemInfo.insert(p1);
		}
	}
	file.Release();
}

GOLD_SHOP_ITEM_INFO & CItemManager::GetGoldShopItem(WORD wItemIndex, WORD wMainIndex,WORD wSubIndex,WORD wGridIndex)
{
	for(Info_multimap iter = m_ShopItemInfo.begin();iter!=m_ShopItemInfo.end();iter++ )
	{
		if(iter->second.wMainIndex==wMainIndex&&iter->second.wSubIndex==wSubIndex&&iter->second.wGridIndex==wGridIndex)
			return iter->second;
	}
	GOLD_SHOP_ITEM_INFO Temp;
	memset(&Temp,0,sizeof(GOLD_SHOP_ITEM_INFO));
	return Temp;
}


void CItemManager::LoadReinforceItemInfo()
{
}
void CItemManager::SetItemInfo(WORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile)
{
	pInfo->ItemIdx = ItemIdx;
	SafeStrCpy( pInfo->ItemName, pFile->GetString(), MAX_ITEMNAME_LENGTH+1 );
	pInfo->ItemTooltipIdx = pFile->GetWord();
	pInfo->Image2DNum = pFile->GetWord();	
	pInfo->ItemKind = pFile->GetWord();			
	pInfo->BuyPrice = pFile->GetDword();			
	pInfo->SellPrice = pFile->GetDword();			
	pInfo->Rarity = pFile->GetDword();			
	pInfo->WeaponType = pFile->GetWord();		
	pInfo->GenGol = pFile->GetWord();			
	pInfo->MinChub = pFile->GetWord();			
	pInfo->CheRyuk = pFile->GetWord();			
	pInfo->SimMek = pFile->GetWord();			
	pInfo->Life = pFile->GetDword();			
	pInfo->Shield = pFile->GetDword();				
	pInfo->NaeRyuk = pFile->GetWord();			
	pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_WATER,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_TREE,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_IRON,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,pFile->GetFloat());
	pInfo->LimitJob = pFile->GetWord();			
	pInfo->LimitGender = pFile->GetWord();		
	pInfo->LimitLevel = pFile->GetLevel();		
	pInfo->LimitGenGol = pFile->GetWord();		
	pInfo->LimitMinChub = pFile->GetWord();		
	pInfo->LimitCheRyuk = pFile->GetWord();		
	pInfo->LimitSimMek = pFile->GetWord();		
	pInfo->ItemGrade = pFile->GetWord();			
	pInfo->RangeType = pFile->GetWord();			
	pInfo->EquipKind = pFile->GetWord();			
	pInfo->Part3DType = pFile->GetWord();		
	pInfo->Part3DModelNum = pFile->GetWord();	
	pInfo->MeleeAttackMin = pFile->GetWord();	
	pInfo->MeleeAttackMax = pFile->GetWord();	
	pInfo->RangeAttackMin = pFile->GetWord();	
	pInfo->RangeAttackMax = pFile->GetWord();	
	pInfo->CriticalPercent = pFile->GetWord();	
	pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_WATER,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_TREE,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_IRON,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,pFile->GetFloat());
	pInfo->PhyDef = pFile->GetWord();			
	pInfo->Plus_MugongIdx = pFile->GetWord();	
	pInfo->Plus_Value = pFile->GetWord();		
	pInfo->AllPlus_Kind = pFile->GetWord();		
	pInfo->AllPlus_Value = pFile->GetWord();		
	pInfo->MugongNum = pFile->GetWord();			
	pInfo->MugongType = pFile->GetWord();		
	pInfo->LifeRecover = pFile->GetWord();		
	pInfo->LifeRecoverRate = pFile->GetFloat();
	pInfo->NaeRyukRecover = pFile->GetWord();	
	pInfo->NaeRyukRecoverRate = pFile->GetFloat();		
	pInfo->ItemType = pFile->GetWord();	
	
//#ifdef _JAPAN_LOCAL_
//	pInfo->wItemAttr = pFile->GetWord();
//	pInfo->wAcquireSkillIdx1 = pFile->GetWord();
//	pInfo->wAcquireSkillIdx2 = pFile->GetWord();
//	pInfo->wDeleteSkillIdx = pFile->GetWord();
//#endif
	pInfo->wSetItemKind = pFile->GetWord();	
	pInfo->wPulsItemEffect = pFile->GetWord();
	pInfo->wSellType = pFile->GetWord();		//[物品出售货币类型][BY:十里坡剑神][QQ:112582793][2019-9-19][22:34:54]
} 
void CItemManager::Item_Move_Syn(void* pMsg)
{
	MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	int rt = 0;
	if (EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
	{
		MSG_ITEM_MOVE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));

		msg.Protocol = MP_ITEM_MOVE_ACK;

		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else if (rt != 99)
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ERROR_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Move;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Combine_Syn(void* pMsg)
{
	MSG_ITEM_COMBINE_SYN * pmsg = (MSG_ITEM_COMBINE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	int rt = 0;
	if (EI_TRUE == (rt = CombineItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur)))
	{
		MSG_ITEM_COMBINE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_COMBINE_SYN));
		msg.Protocol = MP_ITEM_COMBINE_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_COMBINE_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Combine;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Divide_Syn(void* pMsg)
{
	MSG_ITEM_DIVIDE_SYN * pmsg = (MSG_ITEM_DIVIDE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	int rt = 0;
	const ITEMBASE * pItem = GetItemInfoAbsIn(pPlayer, pmsg->FromPos);
	if (!pItem) return;
	if (pItem->dwDBIdx != pmsg->dwItemDBIdx) return;
	if (EI_TRUE == (rt = DivideItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur, pmsg->dwItemDBIdx)))
	{

	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ERROR_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Divide;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Discard_Syn(void* pMsg)
{
	MSG_ITEM_DISCARD_SYN * pmsg = (MSG_ITEM_DISCARD_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	if (pPlayer->IsSafeLock())
	{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	CItemSlot * pSlot = pPlayer->GetSlot(pmsg->TargetPos);
	if (!pSlot) return;

	if (!pSlot->GetItemInfoAbs(pmsg->TargetPos))
		return;//[判断是否获取到物品][By:十里坡剑神][QQ:112582793][2018/4/13]

	ITEMBASE Item = *pSlot->GetItemInfoAbs(pmsg->TargetPos);

	if (Item.dwDBIdx != pmsg->dwItemDBIdx)
		return;//[判断获取到的物品和要删除的物品是否是同一个物品][By:十里坡剑神][QQ:112582793][2018/4/13]

	int rt = 0;
	if (LOOTINGMGR->IsLootedPlayer(pPlayer->GetID()))
	{
		rt = 10;
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ERROR_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Discard;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
		return;
	}

	if (EI_TRUE == (rt = DiscardItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx, pmsg->ItemNum)))
	{
		MSG_ITEM_DISCARD_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_DISCARD_SYN));
		msg.Protocol = MP_ITEM_DISCARD_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
		LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
			eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_DISCARD_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = rt;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Buy_Syn(void* pMsg)
{
	MSG_ITEM_BUY_SYN * pmsg = (MSG_ITEM_BUY_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (CheckHackNpc(pPlayer, pmsg->wDealerIdx) == FALSE && pmsg->wDealerIdx != MALLSHOPIDX)
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_BUY_NACK;
		msg.dwObjectID = pPlayer->GetID();
		SendErrorMsg(pPlayer, &msg, sizeof(msg), NOT_EXIST);
		return;
	}

	if (FALSE == CheckDemandItem(pPlayer, pmsg) && pmsg->wDealerIdx != MALLSHOPIDX)
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_BUY_NACK;
		msg.dwObjectID = pPlayer->GetID();
		SendErrorMsg(pPlayer, &msg, sizeof(msg), NO_DEMANDITEM);
		return;
	}


	int rt = 0;
	if (pmsg->wDealerIdx == 10000)
		rt = BuyItemForGoldShop(pPlayer, pmsg->wBuyItemIdx, pmsg->BuyItemNum, pmsg->wDealerIdx, pmsg->wMainIndex, pmsg->wSunIndex, pmsg->wIdxPos, pmsg->wBuyType);
	else
		rt = BuyItem(pPlayer, pmsg->wBuyItemIdx, pmsg->BuyItemNum, pmsg->wDealerIdx, pmsg->wIdxPos, pmsg->wBuyType);
	if (EI_TRUE == rt)
	{
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_BUY_NACK;
		msg.dwObjectID = pPlayer->GetID();
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Sell_Syn(void* pMsg)
{
	//[物品出售未判断合法性][By:十里坡剑神][QQ:112582793][2018/4/15]
	MSG_ITEM_SELL_SYN * pmsg = (MSG_ITEM_SELL_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 5;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (CheckHackNpc(pPlayer, pmsg->wDealerIdx) == FALSE)
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SELL_NACK;
		msg.dwObjectID = pPlayer->GetID();
		SendErrorMsg(pPlayer, &msg, sizeof(msg), NOT_EXIST);
		return;
	}

	int rt = 0;
	if (EI_TRUE == (rt = SellItem(pPlayer, pmsg->TargetPos, pmsg->wSellItemIdx, pmsg->SellItemNum, pmsg->wDealerIdx, pmsg->dwItemDBIdx)))
	{
		MSG_ITEM_SELL_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_SELL_SYN));
		msg.Protocol = MP_ITEM_SELL_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SELL_NACK;
		msg.dwObjectID = pPlayer->GetID();
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Use_Syn(void* pMsg)
{
	MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx, pmsg->dwItemDBIdx);
	if (rt == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
		msg.Protocol = MP_ITEM_USE_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = rt;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Mix_AddItem_Syn(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 6;
		msg.dwData2 = pmsg->ItemInfo.Position;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	WORD wErrorCode = 0;
	ITEM_MIX_INFO* pMixItemInfo = NULL;
	CItemSlot * pSlot = NULL;
	MSG_ITEM msgAck;
	BOOL bIsCanMix = FALSE;
	bIsCanMix = CanMixItem(pmsg->ItemInfo.wIconIdx);
	if (GetTableIdxPosition(pmsg->ItemInfo.Position) != eItemTable_Inventory&&
		!bIsCanMix)
	{
		wErrorCode = 1;
		goto go_MIX_ADDITEM_NACK;
	}
	if (!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0, 0, CB_EXIST | CB_ICONIDX))
	{
		wErrorCode = 2;
		goto go_MIX_ADDITEM_NACK;
	}
	if (GetTableIdxPosition(pmsg->ItemInfo.Position) == eItemTable_ShopInven)
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	else
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (pSlot)
		if (pSlot->IsLock(pmsg->ItemInfo.Position))
		{
		wErrorCode = 3;
		goto go_MIX_ADDITEM_NACK;
		}
	if (ITEMMGR->IsOptionItem(pmsg->ItemInfo.wIconIdx, pmsg->ItemInfo.Durability))
	{
		wErrorCode = 4;
		goto go_MIX_ADDITEM_NACK;
	}

	pMixItemInfo = ITEMMGR->GetMixItemInfo(pmsg->ItemInfo.wIconIdx);
	if (!pMixItemInfo)
	{
		wErrorCode = 5;
		goto go_MIX_ADDITEM_NACK;
	}

	if (GetItemKind(pmsg->ItemInfo.wIconIdx) != eYOUNGYAK_ITEM &&
		GetItemKind(pmsg->ItemInfo.wIconIdx) != eEXTRA_ITEM_JEWEL &&
		pmsg->ItemInfo.Durability > 1 &&
		!bIsCanMix)
	{
		wErrorCode = 6;
		goto go_MIX_ADDITEM_NACK;
	}

	pSlot->SetLock(pmsg->ItemInfo.Position, TRUE);

	msgAck.Category = MP_ITEM;
	msgAck.Protocol = MP_ITEM_MIX_ADDITEM_ACK;
	msgAck.ItemInfo = pmsg->ItemInfo;
	pPlayer->SendMsg(&msgAck, sizeof(msgAck));
	return;

go_MIX_ADDITEM_NACK:
	MSG_WORD2 msgNack;
	msgNack.Category = MP_ITEM;
	msgNack.Protocol = MP_ITEM_MIX_ADDITEM_NACK;
	msgNack.wData1 = pmsg->ItemInfo.Position;
	msgNack.wData2 = wErrorCode;
	pPlayer->SendMsg(&msgNack, sizeof(msgNack));
}
void CItemManager::Item_Mix_ReleaseItem(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
	if (pSlot)
		pSlot->SetLock(pmsg->wData, FALSE);
}
void CItemManager::Item_Mix_Syn(void* pMsg)
{
	MSG_ITEM_MIX_SYN * pmsg = (MSG_ITEM_MIX_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	int rt = 0;
	if (EI_TRUE == (rt = MixItem(pPlayer, pmsg->wBasicItemIdx, pmsg->BasicItemPos, pmsg->ResultIndex, pmsg->Material, pmsg->wMaterialNum, pmsg->ShopItemIdx, pmsg->ShopItemPos)))
	{
		MSG_ITEM_MIX_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
		msg.Protocol = MP_ITEM_MIX_SUCCESS_ACK;
		SendAckMsg(pPlayer, &msg, msg.GetSize());
	}
	else
	{
		switch (rt)
		{
		case 1000:
		{
			MSG_ITEM_MIX_ACK msg;
			memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
			msg.Protocol = MP_ITEM_MIX_BIGFAILED_ACK;
			SendAckMsg(pPlayer, &msg, msg.GetSize());
		}
			break;
		case 1001:
		{
			MSG_ITEM_MIX_ACK msg;
			memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
			msg.Protocol = MP_ITEM_MIX_FAILED_ACK;
			SendAckMsg(pPlayer, &msg, msg.GetSize());
		}
			break;
		case 20:
		case 21:
		case 22:
		case 23:
		case 256:
		{
			MSG_DWORD2 msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_MIX_MSG;
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData1 = rt;
			msg.dwData2 = pmsg->BasicItemPos;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
			break;
		default:
		{
			if (rt != 2)
			{
				char temp[256];
				sprintf(temp, "组合失败 错误代码:%d", rt);
				ASSERTMSG(0, temp);
			}
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_ERROR_NACK;
			msg.dwObjectID = pPlayer->GetID();
			msg.ECode = eItemUseErr_Mix;
			SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
		}
			break;
		}
	}
}
void CItemManager::Item_TPM_AddItem_Syn(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	TITANITEMMGR->TitanPartsMakeAddItem(pPlayer, pmsg);
}
void CItemManager::Item_TPM_ReleaseItem(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
	if (pSlot)
		pSlot->SetLock(pmsg->wData, FALSE);
}
void CItemManager::Item_TPM_Syn(void* pMsg)
{
	MSG_ITEM_MIX_SYN * pmsg = (MSG_ITEM_MIX_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	TITANITEMMGR->TitanPartsMake(pPlayer, pmsg);
}
void CItemManager::Item_UpGrade_Syn(void* pMsg)
{
	MSG_ITEM_UPGRADE_SYN * pmsg = (MSG_ITEM_UPGRADE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	int rt = 0;
	if (EI_TRUE == (rt = UpgradeItem(pPlayer, pmsg->wItemIdx, pmsg->ItemPos, pmsg->wMaterialItemIdx, pmsg->MaterialItemPos)))
	{
		MSG_ITEM_UPGRADE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_UPGRADE_SYN));
		msg.Protocol = MP_ITEM_UPGRADE_SUCCESS_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_ERROR_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Upgrade;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_Reinforce_Syn(void* pMsg)
{
	MSG_ITEM_REINFORCE_SYN* pmsg = (MSG_ITEM_REINFORCE_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)
		return;

	int rt = 0;
	if (EI_TRUE == (rt = ReinforceItem(pPlayer, pmsg->wTargetItemIdx, pmsg->TargetPos, pmsg->JewelWhich, pmsg->wJewelUnit)))
	{

	}
	else
	{
		switch (rt)
		{
		case 99:
			{
				pmsg->Protocol = MP_ITEM_REINFORCE_FAILED_ACK;
				SendAckMsg(pPlayer, pmsg, pmsg->GetSize());
			}
			break;
		default:
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_REINFORCE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
			break;
		}
	}
}
void CItemManager::Item_Reinforce_WithShopItem_Syn(void* pMsg)
{
	MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN* pmsg = (MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)
		return;

	int rt = 0;
	if (EI_TRUE == (rt = ReinforceItemWithShopItem(pPlayer, pmsg->wTargetItemIdx, pmsg->TargetPos, pmsg->wShopItemIdx, pmsg->ShopItemPos, pmsg->JewelWhich, pmsg->wJewelUnit)))
	{

	}
	else
	{
		switch (rt)
		{
		case 99:
		{
			pmsg->Protocol = MP_ITEM_REINFORCE_FAILED_ACK;
			SendAckMsg(pPlayer, pmsg, pmsg->GetSize());
		}
			break;
		default:
		{
			MSG_ITEM_ERROR msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_REINFORCE_WITHSHOPITEM_NACK;
			msg.dwObjectID = pPlayer->GetID();
			SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
		}
			break;
		}
	}
}
void CItemManager::Item_Dealer_Syn(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)
		return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (CheckHackNpc(pPlayer, pmsg->wData) == FALSE)
		return;

	MSG_WORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_DEALER_ACK;
	msg.dwObjectID = pmsg->dwObjectID;
	msg.wData = pmsg->wData;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_Guild_WareHouse_Info_Syn(void* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;
	GUILDMGR->GuildWarehouseInfo(pPlayer, pmsg->bData);
}
void CItemManager::Item_Guild_Move_Syn(void* pMsg)
{
	MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	if (!CanMovetoGuildWare(pmsg->FromPos, pmsg->ToPos, pPlayer))
	{
		ASSERT(0);
		SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, 4);
		return;
	}
	int rt = 0;
	if (EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
	{
		MSG_ITEM_MOVE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));
		msg.Protocol = MP_ITEM_GUILD_MOVE_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, rt);
	}
}
void CItemManager::Item_Guild_WareHouse_Leave(void* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	GUILDMGR->LeaveWareHouse(pPlayer, pmsg->bData);
}
void CItemManager::Item_Pyoguk_Item_Info_Syn(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)	return;

	if (CheckHackNpc(pPlayer, pmsg->wData1, pmsg->wData2) == FALSE)
		return;

	if (pPlayer->IsGotWarehouseItems() == TRUE)
		return;

	pPlayer->SetGotWarehouseItems(TRUE);
	PyogukItemOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), 0);
}
void CItemManager::Item_PetInven_Info_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)	return;

	if (!pPlayer->GetPetManager()->GetCurSummonPet())	return;

	PetInvenItemOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
}
void CItemManager::Item_Use_For_QuestStart_Syn(void* pMsg)
{
	MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx, pmsg->dwItemDBIdx);
	if (rt == eItemUseSuccess)
	{
		MSG_ITEM_USE_ACK msg;
		memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
		msg.Protocol = MP_ITEM_USE_ACK;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = eItemUseErr_Quest;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), eItemUseErr_Quest);
	}
}
void CItemManager::Item_Dissolution_Syn(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)	return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 7;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	MSGBASE msg;
	msg.Category = MP_ITEM;

	if (DISSOLUTIONMGR->ItemDissollution(pPlayer, &pmsg->ItemInfo) == 0)
	{
		msg.Protocol = MP_ITEM_DISSOLUTION_ACK;
	}
	else
	{
		msg.Protocol = MP_ITEM_DISSOLUTION_NACK;
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_Use_ChangeItem_Syn(void* pMsg)
{
	MSG_ITEM_USE_SYN* pmsg = (MSG_ITEM_USE_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	int rt = CHANGEITEMMGR->UseChangeItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx);
	if (rt != 1)
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_NACK;
		msg.dwObjectID = pPlayer->GetID();
		msg.ECode = rt;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_ShopItem_Info_Syn(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)
		return;
	pPlayer->SetShopItemInit(FALSE);
	CharacterShopItemInfo(pPlayer->GetID(), 0);
}
void CItemManager::Item_ShopItem_Use_Syn(void* pMsg)
{
	MSG_SHOPITEM_USE_SYN * pmsg = (MSG_SHOPITEM_USE_SYN *)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	SHOPITEMBASE BaseInfo;
	BYTE rt = UseShopItem(pPlayer, pmsg->UseBaseInfo, &BaseInfo);
	if (rt == eItemUseSuccess)
	{
		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemBase = BaseInfo;
		msg.ShopItemPos = pmsg->UseBaseInfo.ShopItemPos;
		msg.ShopItemIdx = pmsg->UseBaseInfo.ShopItemIdx;
		SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
		msg.ECode = rt;
		SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
	}
}
void CItemManager::Item_ShopItem_Unseal_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.dwData = pmsg->dwData;

	if (ITEMMGR->ItemUnsealing(pPlayer, (POSTYPE)pmsg->dwData))
	{
		msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_ACK;
	}
	else
	{
		msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_NACK;
	}

	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_ChangeMap_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
}
void CItemManager::Item_ShopItem_MapInfo(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	SavedMovePointInfo(pPlayer->GetID());
}
void CItemManager::Item_ShopItem_SavePoint_Add_Syn(void* pMsg)
{
	SEND_MOVEDATA_WITHITEM* pmsg = (SEND_MOVEDATA_WITHITEM*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
	if (pPlayer->GetShopItemManager()->GetUsingItemInfo(eIncantation_MemoryMoveExtend) ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo(eIncantation_MemoryMoveExtend7) ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo(eIncantation_MemoryMove2) ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo(eIncantation_MemoryMoveExtend30))
		Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

	BYTE rt;
	SHOPITEMBASE ReturnItem;

	if (pPlayer->GetShopItemManager()->GetSavedMPNum() >= Validsavenum)
		goto SAVEPOINT_ADD_FAILED;

	rt = UseShopItem(pPlayer, pmsg->Item, &ReturnItem);
	if (rt == eItemUseSuccess)
	{
		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemBase = ReturnItem;
		msg.ShopItemPos = pmsg->Item.ShopItemPos;
		msg.ShopItemIdx = pmsg->Item.ShopItemIdx;
		pPlayer->SendMsg(&msg, sizeof(msg));

		SavedMovePointInsert(pPlayer->GetID(), pmsg->Data.Name, pmsg->Data.MapNum, pmsg->Data.Point.value);
	}
	else
	{
SAVEPOINT_ADD_FAILED:
		MSG_ITEM_ERROR msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
		msg.ECode = rt;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CItemManager::Item_ShopItem_SavePoint_Update_Syn(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	if (pPlayer->GetShopItemManager()->ReNameMovePoint(pmsg->Data.DBIdx, pmsg->Data.Name))
	{
		pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_ACK;

		SavedMovePointUpdate(pPlayer->GetID(), pmsg->Data.DBIdx, pmsg->Data.Name);
	}
	else
	{
		pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_NACK;
	}

	pPlayer->SendMsg(pmsg, sizeof(*pmsg));
}
void CItemManager::Item_ShopItem_SavePoint_Del_Syn(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	if (pPlayer->GetShopItemManager()->DeleteMovePoint(pmsg->Data.DBIdx))
	{
		pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_DEL_ACK;

		SavedMovePointDelete(pPlayer->GetID(), pmsg->Data.DBIdx);
	}
	else
	{
		pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_DEL_NACK;
	}

	pPlayer->SendMsg(pmsg, sizeof(*pmsg));
}
void CItemManager::Item_ShopItem_ReviveOther_Syn(void* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
	if (pTargetPlayer == NULL ||
		pTargetPlayer->GetState() != eObjectState_Die)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.dwData = eShopItemErr_Revive_NotDead;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	if (pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR)
	{
		if (pPlayer->GetBattleTeam() == 2)
		{
			ITEM_INFO * pItemInfo = GetItemInfo((WORD)pmsg->dwData2);
			if (pItemInfo && (pItemInfo->ItemKind & eSHOP_ITEM))
			{
				if (pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel)
				{
					MSG_DWORD msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
					msg.dwData = eShopItemErr_Revive_NotReady;
					pPlayer->SendMsg(&msg, sizeof(msg));
					return;
				}
			}
		}
	}

	if (IsUseAbleShopItem(pPlayer, (WORD)pmsg->dwData2, (POSTYPE)pmsg->dwData3))
	{
		SEND_REVIVEOTHER msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_SYN;
		SafeStrCpy(msg.TargetName, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		msg.TargetID = pPlayer->GetID();
		pTargetPlayer->SendMsg(&msg, sizeof(msg));

		pPlayer->SetReviveData(pmsg->dwData1, (WORD)pmsg->dwData2, (POSTYPE)pmsg->dwData3);
		pPlayer->SetReviveTime(60000);
	}
	else
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.dwData = eShopItemErr_Revive_NotUse;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CItemManager::Item_ShopItem_ReviveOther_Ack(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
	if (pTargetPlayer == NULL)		return;

	if (pPlayer->GetState() != eObjectState_Die)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.wData = eShopItemErr_Revive_NotDead;
		pPlayer->SendMsg(&msg, sizeof(msg));
		pTargetPlayer->SendMsg(&msg, sizeof(msg));

		pTargetPlayer->SetReviveData(0, 0, 0);
		pTargetPlayer->SetReviveTime(0);
		return;
	}

	REVIVEDATA* pData = pTargetPlayer->GetReviveData();

	if (IsUseAbleShopItem(pTargetPlayer, pData->ItemIdx, pData->ItemPos) == FALSE)
	{
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.wData = eShopItemErr_Revive_NotUse;
		pTargetPlayer->SendMsg(&msg, sizeof(msg));

		msg.wData = eShopItemErr_Revive_Fail;
		pPlayer->SendMsg(&msg, sizeof(msg));

		pTargetPlayer->SetReviveData(0, 0, 0);
		pTargetPlayer->SetReviveTime(0);
		return;
	}


	SHOPITEMUSEBASE ItemUseBase;
	ItemUseBase.ShopItemIdx = pData->ItemIdx;
	ItemUseBase.ShopItemPos = pData->ItemPos;
	SHOPITEMBASE ItemBase;

	ITEM_INFO* pItemInfo = GetItemInfo(pData->ItemIdx);
	if (!pItemInfo)
		goto Revive_Failed;
	if (pItemInfo->ItemKind != eSHOP_ITEM_INCANTATION || !pItemInfo->LimitLevel)
		goto Revive_Failed;

	if (pPlayer->IsAbleReviveOther())
	{
		SHOPITEMWITHTIME* pShopItem = pTargetPlayer->GetShopItemManager()->GetUsingItemInfo(pData->ItemIdx);
		if (!pShopItem)
		{
			if (pItemInfo->SellPrice)
				goto Revive_Failed;
			if (EI_TRUE != DiscardItem(pTargetPlayer, ItemUseBase.ShopItemPos, ItemUseBase.ShopItemIdx, 1))
				goto Revive_Failed;
		}

		pPlayer->ReviveShopItem(pData->ItemIdx);

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemBase = ItemBase;
		msg.ShopItemIdx = pData->ItemIdx;
		msg.ShopItemPos = pData->ItemPos;
		pTargetPlayer->SendMsg(&msg, sizeof(msg));

		MSG_DWORD msgb;
		msgb.Category = MP_ITEM;
		msgb.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_ACK;
		msgb.dwData = pPlayer->GetID();
		pTargetPlayer->SendMsg(&msgb, sizeof(msgb));
		pPlayer->SendMsg(&msgb, sizeof(msgb));
	}
	else
	{
Revive_Failed:
		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.wData = eShopItemErr_Revive_Fail;
		pTargetPlayer->SendMsg(&msg, sizeof(msg));
		pPlayer->SendMsg(&msg, sizeof(msg));
	}

	pTargetPlayer->SetReviveData(0, 0, 0);
	pTargetPlayer->SetReviveTime(0);
}
void CItemManager::Item_ShopItem_ReviveOther_Nack(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
	if (pTargetPlayer == NULL)		return;

	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
	msg.dwData = pmsg->dwData2;
	pTargetPlayer->SendMsg(&msg, sizeof(msg));

	pTargetPlayer->SetReviveData(0, 0, 0);
	pTargetPlayer->SetReviveTime(0);
}
void CItemManager::Item_ShopItem_Avatar_TakeOff(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	if (IsUseAbleShopItem(pPlayer, pmsg->wData1, pmsg->wData2))
	{
		if (pPlayer->GetShopItemManager()->TakeOffAvatarItem(pmsg->wData1, pmsg->wData2) == FALSE)
		{
			pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;
			pPlayer->SendMsg(pmsg, sizeof(*pmsg));
		}
	}
	else
	{
		pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;
		pPlayer->SendMsg(pmsg, sizeof(*pmsg));
	}
}
void CItemManager::Item_ShopItem_Avatar_Change(void* pMsg)
{
	MSG_WORD2*pmsg = (MSG_WORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	pPlayer->GetShopItemManager()->CalcShopItemOption(pmsg->wData2, TRUE);

	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = pmsg->wData2;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_Avatar_Use_Syn(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	if (pPlayer->GetState() != eObjectState_None && pPlayer->GetState() != eObjectState_Immortal)
		goto _Avataruse_failed;

	if (!IsUseAbleShopItem(pPlayer, pmsg->wData1, pmsg->wData2))
		goto _Avataruse_failed;


	const ITEMBASE* pItembase = GetItemInfoAbsIn(pPlayer, pmsg->wData2);
	if (!pItembase)
		goto _Avataruse_failed;

	if (CheckWeaponToShopItem(pPlayer, pmsg->wData1) == FALSE)
	{
		goto _Avataruse_failed;
	}


	SHOPITEMWITHTIME* pItem = pPlayer->GetShopItemManager()->GetUsingItemInfo(pmsg->wData1);
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pmsg->wData1);
	if (pItem)
	{
		if (pItem->ShopItem.ItemBase.dwDBIdx == pItembase->dwDBIdx)
		{
			if (pPlayer->GetShopItemManager()->PutOnAvatarItem(pmsg->wData1, pmsg->wData2))
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_ACK;
				pPlayer->SendMsg(pmsg, sizeof(*pmsg));
				if (pInfo->ItemKind == eSHOP_ITEM_IMAGENAME)    //[图片称号服务端设置][By:十里坡剑神][QQ:112582793][2017/11/29]
					pPlayer->SetImageName(pInfo->wSetItemKind);
				return;
			}
			else
				goto _Avataruse_failed;
		}
		else
		{
			goto _Avataruse_failed;
		}
	}
	else
	{
		IsUseAbleShopAvatarItem(pPlayer->GetID(), pItembase->dwDBIdx, pItembase->wIconIdx, pItembase->Position);
		return;
	}


_Avataruse_failed:
	pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;
	pPlayer->SendMsg(pmsg, sizeof(*pmsg));
}
void CItemManager::Item_ShopItem_ChaseUse_Syn(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)				return;

	MSGBASE msg;
	msg.Category = MP_ITEM;

	if (pPlayer->GetShopItemManager()->GetUsingItemInfo(pmsg->wData1))
		msg.Protocol = MP_ITEM_SHOPITEM_CHASEUSE_ACK;
	else
		msg.Protocol = MP_ITEM_SHOPITEM_CHASEUSE_NACK;

	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_Chase_Syn(DWORD dwConnectionIndex, void* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData2);
	if (!pPlayer)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
		msg.dwData = pmsg->dwData1;

		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	if (pmsg->dwData3 == eIncantation_Tracking ||
		pmsg->dwData3 == eIncantation_Tracking7 ||
		pmsg->dwData3 == eIncantation_Tracking7_NoTrade)
	{
		SEND_CHASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_ACK;
		msg.dwObjectID = pPlayer->GetID();

		SafeStrCpy(msg.WandtedName, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
		VECTOR3 pos;
		pPlayer->GetPosition(&pos);
		msg.Pos.Compress(&pos);
		msg.MapNum = GAMERESRCMNGR->GetLoadMapNum();
		if (pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN)
			msg.EventMapNum = 44;
		else
			msg.EventMapNum = (MAPTYPE)pPlayer->GetChannelID();
		msg.CharacterIdx = pmsg->dwData1;
		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));

		MSGBASE msg1;
		msg1.Category = MP_ITEM;
		msg1.Protocol = MP_ITEM_SHOPITEM_CHASE_TRACKING;
		pPlayer->SendMsg(&msg1, sizeof(msg1));
	}
}
void CItemManager::Item_ShopItem_Nchange_Syn(void* pMsg)
{
	SEND_CHANGENAMEBASE* pmsg = (SEND_CHANGENAMEBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	const ITEMBASE* pItemBase = NULL;
	int i = 0;
	for (i = 0; i < (SLOT_SHOPINVEN_NUM/*/2*/ + TABCELL_SHOPINVEN_PLUS_NUM); i++)
	{
		pItemBase = GetItemInfoAbsIn(pPlayer, i + TP_SHOPINVEN_START);
		if (!pItemBase)	continue;

		if (pItemBase->dwDBIdx == pmsg->DBIdx)
			break;
	}

	if (i >= (SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM)/*/2*/)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;
		msg.dwData = 6;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	CharacterChangeName(pmsg->dwObjectID, pmsg->Name, pmsg->DBIdx);
}
void CItemManager::Item_ShopItem_CharChange_Syn(void* pMsg)
{
	SEND_CHARACTERCHANGE_INFO* pmsg = (SEND_CHARACTERCHANGE_INFO*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	MSG_DWORD msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE_NACK);
	CHARACTER_TOTALINFO CTInfo;
	pPlayer->GetCharacterTotalInfo(&CTInfo);

	for (int i = 0; i<eAvatar_Effect; ++i)
	{
		if (pPlayer->GetShopItemStats()->Avatar[i])
		{
			msg.dwData = 6;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}
	}

	ITEMBASE* pItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->Param);
	if (!pItem || (pItem->wIconIdx != eIncantation_CharChange && pItem->wIconIdx != eIncantation_ShapeChange))
	{
		msg.dwData = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pmsg->Info.Height > 1.1f || pmsg->Info.Height < 0.9f ||
		pmsg->Info.Width > 1.1f || pmsg->Info.Width < 0.9f)
	{
		msg.dwData = 2;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pmsg->Info.Gender > 2)
	{
		msg.dwData = 3;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pmsg->Info.HairType > 4 || pmsg->Info.FaceType > 4)
	{
		msg.dwData = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	if (EI_TRUE != DiscardItem(pPlayer, (POSTYPE)pmsg->Param, pItem->wIconIdx, 1))
	{
		msg.dwData = 5;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	else
	{
		pPlayer->SetCharChangeInfo(&pmsg->Info);

		SEND_SHOPITEM_BASEINFO msg;
		SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK);
		msg.ShopItemPos = (POSTYPE)pmsg->Param;
		msg.ShopItemIdx = pItem->wIconIdx;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

		//
		SEND_CHARACTERCHANGE_INFO msg1;
		SetProtocol(&msg1, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE);
		msg1.Param = pPlayer->GetID();
		memcpy(&msg1.Info, &pmsg->Info, sizeof(CHARACTERCHANGE_INFO));
		if (pItem->wIconIdx == eIncantation_ShapeChange)
		{
			msg1.Info.Gender = CTInfo.Gender;
			msg1.Info.Height = CTInfo.Height;
			msg1.Info.Width = CTInfo.Width;
		}
		PACKEDDATA_OBJ->QuickSend(pPlayer, &msg1, sizeof(msg1));
	}

	// To DB
	if (pItem->wIconIdx == eIncantation_ShapeChange)
		CharacterChangeInfoToDB(pPlayer->GetID(), CTInfo.Gender,
		pmsg->Info.HairType, pmsg->Info.FaceType, CTInfo.Height, CTInfo.Width);
	else
		CharacterChangeInfoToDB(pPlayer->GetID(), pmsg->Info.Gender, pmsg->Info.HairType, pmsg->Info.FaceType,
		pmsg->Info.Height, pmsg->Info.Width);

	MSGBASE msg1;
	SetProtocol(&msg1, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE_ACK);
	pPlayer->SendMsg(&msg1, sizeof(msg1));

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
}
void CItemManager::Item_ShopItem_Seal_Syn(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	MSG_DWORD msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_SEAL_NACK);

	if (!IsUseAbleShopItem(pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2))
	{
		msg.dwData = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (!IsUseAbleShopItem(pPlayer, (WORD)pmsg->dwData3, (POSTYPE)pmsg->dwData4))
	{

		msg.dwData = 2;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	ITEMBASE* pSealItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData4);
	if (!pSealItem || !pTargetItem)
	{
		msg.dwData = 3;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	ITEM_INFO* pItemInfo = GetItemInfo((WORD)pmsg->dwData3);
	if (!pItemInfo)
	{
		msg.dwData = 3;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	if (pSealItem->wIconIdx != eIncantation_ItemSeal)
	{
		msg.dwData = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pItemInfo->ItemKind != eSHOP_ITEM_MAKEUP && pItemInfo->ItemKind != eSHOP_ITEM_DECORATION && pItemInfo->ItemKind != eSHOP_ITEM_PET)
	{
		msg.dwData = 5;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pItemInfo->SellPrice != eShopItemUseParam_Forever)
	{
		msg.dwData = 6;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pTargetItem->ItemParam & ITEM_PARAM_SEAL)
	{
		msg.dwData = 7;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	if (EI_TRUE != DiscardItem(pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1))
	{
		msg.dwData = 9;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pSealItem->wIconIdx, pSealItem->dwDBIdx, pSealItem->Position, 0, pSealItem->Durability, pPlayer->GetPlayerExpPoint());

	DWORD Param = pTargetItem->ItemParam;
	Param |= ITEM_PARAM_SEAL;
	pTargetItem->ItemParam = Param;

	pPlayer->GetShopItemManager()->DeleteUsingShopItemInfo(pTargetItem->wIconIdx);
	ShopItemParamUpdateToDB(pPlayer->GetID(), pTargetItem->dwDBIdx, pTargetItem->ItemParam);
	ShopItemDeleteToDB(pPlayer->GetID(), pTargetItem->dwDBIdx);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemSeal, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pTargetItem->wIconIdx, pTargetItem->dwDBIdx, pTargetItem->Position, 0, pTargetItem->Durability, pPlayer->GetPlayerExpPoint());

	SEND_SHOPITEM_BASEINFO usemsg;
	SetProtocol(&usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK);
	usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
	usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
	SendAckMsg(pPlayer, &usemsg, sizeof(usemsg));

	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_SEAL_ACK);
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_JobChange_Syn(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	const ITEMBASE* pItem = pSlot->GetItemInfoAbs((POSTYPE)pmsg->dwData1);;
	DWORD stage = pPlayer->GetStage();
	DWORD changestage = 0;
	MSG_DWORD msg;

	if (stage != eStage_Hwa && stage != eStage_Geuk)
	{
		msg.dwData = 1;
		goto JobChangeFailed;
	}
	if (!pSlot || !pItem)
	{
		msg.dwData = 2;
		goto JobChangeFailed;
	}
	if (pItem->wIconIdx != eIncantation_ChangeJob || pItem->dwDBIdx != pmsg->dwData2)
	{
		msg.dwData = 2;
		goto JobChangeFailed;
	}
	if (EI_TRUE != DiscardItem(pPlayer, pItem->Position, pItem->wIconIdx, 1))
	{
		msg.dwData = 3;
		goto JobChangeFailed;
	}

	ChangeCharacterStageAbility(pPlayer->GetID(), (BYTE)stage, pPlayer->GetAbilityGroup());

	if (stage == eStage_Hwa)					changestage = eStage_Geuk;
	else if (stage == eStage_Geuk)				changestage = eStage_Hwa;
	pPlayer->SetStage((BYTE)changestage);


	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_JOBCHANGE_ACK);
	pPlayer->SendMsg(&msg, sizeof(msg));

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse,
		stage, changestage, 0, pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0,
		pItem->Position, pPlayer->GetPlayerExpPoint());

	return;

JobChangeFailed:
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_JOBCHANGE_NACK);
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_ReinforceReset_Syn(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	MSG_DWORD msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_REINFORCERESET_NACK);

	if (!IsUseAbleShopItem(pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2))
	{
		msg.dwData = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	ITEMBASE* pShopItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData4);
	if (!pShopItem || !pTargetItem)
	{
		msg.dwData = 2;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	ITEM_INFO* pItemInfo = GetItemInfo((WORD)pmsg->dwData3);
	if (!pItemInfo)
	{
		msg.dwData = 3;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pShopItem->wIconIdx != eIncantation_ReinforceReset)
	{
		msg.dwData = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (!(pItemInfo->ItemKind & eEQUIP_ITEM) || pItemInfo->ItemKind > eEQUIP_ITEM_BELT)
	{
		msg.dwData = 5;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pPlayer->GetItemOption(pTargetItem->Durability) == NULL&&pPlayer->GetItemRareOption(pTargetItem->RareIdx) == NULL)
	{
		msg.dwData = 6;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	if (EI_TRUE != DiscardItem(pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1))
	{
		msg.dwData = 9;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pShopItem->wIconIdx, pShopItem->dwDBIdx, pShopItem->Position, 0, pShopItem->Durability, pPlayer->GetPlayerExpPoint());

	pPlayer->RemoveItemOption(pTargetItem->Durability);
	//pPlayer->RemoveItemRareOption(pTargetItem->RareIdx);

	CharacterItemOptionDelete(pTargetItem->Durability, pTargetItem->dwDBIdx);
	//CharacterItemRareOptionDelete(pTargetItem->RareIdx,pTargetItem->dwDBIdx);
	pTargetItem->Durability = 0;
	//pTargetItem->RareIdx = 0;
	ItemUpdateToDB(pPlayer->GetID(), pTargetItem->dwDBIdx, pTargetItem->wIconIdx, 0, pTargetItem->Position, pTargetItem->QuickPosition, pTargetItem->RareIdx, pTargetItem->ItemStatic);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItem_ReinforceReset,
		pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItem->wIconIdx, pTargetItem->dwDBIdx,
		pTargetItem->Position, pTargetItem->Position, pTargetItem->Durability, pPlayer->GetPlayerExpPoint());

	pTargetItem->Durability = 0;
	//
	SEND_SHOPITEM_BASEINFO usemsg;
	SetProtocol(&usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK);
	usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
	usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
	SendAckMsg(pPlayer, &usemsg, sizeof(usemsg));

	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_REINFORCERESET_ACK);
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_ShopItem_RareCreate_Syn(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 8;
		msg.dwData2 = (POSTYPE)pmsg->dwData4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	MSG_DWORD msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_NACK);

	if (!IsUseAbleShopItem(pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2))
	{
		msg.dwData = 1;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	ITEMBASE* pShopItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData4);

	if (!pShopItem || !pTargetItem)
	{
		msg.dwData = 2;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}


	//if (pShopItem->ItemStatic==1)
	//{	//[泡点绿化加锁][By:十里坡剑神][QQ:112582793][2017/12/16]
	//	ItemLockUpdate(pPlayer->GetID(),pTargetItem->dwDBIdx,1);
	//}
	ITEM_INFO* pShopItemInfo = GetItemInfo((WORD)pmsg->dwData1);
	ITEM_INFO* pItemInfo = GetItemInfo((WORD)pmsg->dwData3);
	if (!pShopItemInfo || !pItemInfo)
	{
		msg.dwData = 3;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pShopItem->wIconIdx != eSundries_RareItemCreate50 &&
		pShopItem->wIconIdx != eSundries_RareItemCreate70 &&
		pShopItem->wIconIdx != eSundries_RareItemCreate90)
	{
		msg.dwData = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (!(pItemInfo->ItemKind & eEQUIP_ITEM))
	{
		msg.dwData = 5;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	ITEM_OPTION_INFO* pOptionInfo = pPlayer->GetItemOption(pTargetItem->Durability);
	if (pTargetItem->Durability || pOptionInfo->dwOptionIdx)
	{
		msg.dwData = 6;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pItemInfo->ItemKind <= eEQUIP_ITEM && pTargetItem->wIconIdx % 10)
	{
		msg.dwData = 7;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pItemInfo->LimitLevel < pShopItemInfo->GenGol || pItemInfo->LimitLevel > pShopItemInfo->MinChub)
	{
		msg.dwData = 8;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (!RAREITEMMGR->IsRareItemAble(pTargetItem->wIconIdx))
	{
		msg.dwData = 9;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}


	ITEM_RARE_OPTION_INFO rareoption;
	ITEM_RARE_OPTION_INFO* OldRareOption = NULL;
	OldRareOption = pPlayer->GetItemRareOption(pTargetItem->RareIdx);
	DWORD bSignature;
	if (RAREITEMMGR->GetRare(pTargetItem->wIconIdx, &rareoption, pPlayer, bSignature, TRUE, OldRareOption) == FALSE)
	{
		msg.dwData = 10;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (bSignature == 1 && pShopItem->ItemStatic != 1)
	{//[打造者添加][By:十里坡剑神][QQ:112582793][2018/1/2]
		if (strcmp(pPlayer->GetObjectName(), pTargetItem->ItemMaker) != 0)
			ItemMakerUpdate(pPlayer->GetID(), pTargetItem->dwDBIdx, pPlayer->GetObjectName());
	}
	if (EI_TRUE != DiscardItem(pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1))
	{
		msg.dwData = 11;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	ShopItemRareInsertToDB(pPlayer->GetID(), pTargetItem->wIconIdx, pTargetItem->Position, pTargetItem->dwDBIdx, &rareoption);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pTargetItem->dwDBIdx, "",
		eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pShopItem->wIconIdx, pShopItem->dwDBIdx, pShopItem->Position, 0, pShopItem->Durability, pPlayer->GetPlayerExpPoint());

	SEND_SHOPITEM_BASEINFO usemsg;
	SetProtocol(&usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK);
	usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
	usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
	SendAckMsg(pPlayer, &usemsg, sizeof(usemsg));
}
void CItemManager::Item_ShopItem_Shout_Syn(void* pMsg)
{
	SEND_SHOUTBASE_ITEMINFO* pmsg = (SEND_SHOUTBASE_ITEMINFO*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	MSGBASE nmsg;
	SetProtocol(&nmsg, MP_ITEM, MP_ITEM_SHOPITEM_SHOUT_NACK);

	if (IsUseAbleShopItem(pPlayer, pmsg->ItemIdx, pmsg->ItemPos) == FALSE)
	{
		pPlayer->SendMsg(&nmsg, sizeof(nmsg));
		return;
	}

	if (pmsg->ItemIdx == eSundries_Shout_Once || pmsg->ItemIdx == eSundries_Shout_Once_NoTrade)
	{
		if (EI_TRUE != DiscardItem(pPlayer, pmsg->ItemPos, pmsg->ItemIdx, 1))
		{
			pPlayer->SendMsg(&nmsg, sizeof(nmsg));
			return;
		}

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemIdx = pmsg->ItemIdx;
		msg.ShopItemPos = pmsg->ItemPos;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	if (pmsg->ItemIdx == eSundries_SuperShout_Once ||
		pmsg->ItemIdx == eSundries_SuperShout_Once_NoTrade)
	{//[超级狮子吼][By:十里坡剑神][QQ:112582793][2018/2/19]
		if (EI_TRUE != DiscardItem(pPlayer, pmsg->ItemPos, pmsg->ItemIdx, 1))
		{
			pPlayer->SendMsg(&nmsg, sizeof(nmsg));
			return;
		}

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemIdx = pmsg->ItemIdx;
		msg.ShopItemPos = pmsg->ItemPos;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
	}
	SEND_SHOUTBASE msg;
	SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_SHOUT_ACK);
	SafeStrCpy(msg.ShoutMsg[0].Msg, pmsg->Msg, MAX_SHOUT_LENGTH + 1);
	msg.ShoutMsg[0].CharacterIdx = pPlayer->GetID();
	msg.Count = 1;
	g_Network.Send2AgentServer((char*)&msg, msg.GetSize());
}
void CItemManager::Item_ShopItem_Shout_Ack(void* pMsg)
{
	SEND_SHOUTRECEIVE* pmsg = (SEND_SHOUTRECEIVE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->Receive.CharacterIdx);
	if (!pPlayer)			return;

	pmsg->Protocol = MP_ITEM_SHOPITEM_SHOUT_SENDACK;
	pPlayer->SendMsg(pmsg, sizeof(*pmsg));
}
void CItemManager::Item_ShopItem_Shout_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData);
	if (!pPlayer)			return;

	pPlayer->SendMsg(pmsg, sizeof(*pmsg));
}
void CItemManager::Item_Query_Equi_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData);

	if (pPlayer == NULL || pTarget == NULL) return;

	ITEM_TOTALINFO totalinfo;

	ITEM_RARE_OPTION_INFO  pRateInfo[eWearedItem_Max];

	ITEM_OPTION_INFO  pOptionInfo[eWearedItem_Max];

	pTarget->GetItemtotalInfo(&totalinfo, GETITEM_FLAG_INVENTORY | GETITEM_FLAG_WEAR | GETITEM_FLAG_PETWEAR | GETITEM_FLAG_TITANWEAR | GETITEM_FLAG_TITANSHOP);

	for (int i = 0; i < eWearedItem_Max; i++)
	{
		memcpy(&pRateInfo[i], pTarget->GetItemRareOption(totalinfo.WearedItem[i].RareIdx), sizeof(ITEM_RARE_OPTION_INFO));

		memcpy(&pOptionInfo[i], pTarget->GetItemOption(totalinfo.WearedItem[i].Durability), sizeof(ITEM_OPTION_INFO));
	}

	MSG_QUERY_EQUIPINF msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_QUERY_EQUI_ACK;
	memcpy(msg.WearedItemRateInfo, pRateInfo, eWearedItem_Max * sizeof(ITEM_RARE_OPTION_INFO));
	memcpy(msg.WearedItemOptionInfo, pOptionInfo, eWearedItem_Max * sizeof(ITEM_OPTION_INFO));
	memcpy(msg.WearedItem, totalinfo.WearedItem, eWearedItem_Max * sizeof(ITEMBASE));
	CYHHashTable<SHOPITEMWITHTIME> * pUsingShopItem = pTarget->GetShopItemManager()->GetUsingItemInfoAll();
	SHOPITEMWITHTIME* pItem = NULL;
	pUsingShopItem->SetPositionHead();
	while (pItem = pUsingShopItem->GetData())
	{
		if (msg.ItemCount >= 105) break;
		memcpy(&msg.Item[msg.ItemCount], &pItem->ShopItem, sizeof(SHOPITEMBASE));
		msg.ItemCount++;
	}
	msg.Att_Fire = pTarget->GetAttDefense(ATTR_FIRE);
	msg.Att_Earth = pTarget->GetAttDefense(ATTR_EARTH);
	msg.Att_Iron = pTarget->GetAttDefense(ATTR_IRON);
	msg.Att_Water = pTarget->GetAttDefense(ATTR_WATER);
	msg.Att_Tree = pTarget->GetAttDefense(ATTR_TREE);
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::Item_LockEx_Syn(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)
	{
		return;
	}
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (SetItemLock(pPlayer, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4) != 1) // 异常 返回结果！
	{
		MSG_DWORD4 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_LOCKEX_NACK;
		msg.dwData1 = pmsg->dwData1;
		msg.dwData2 = pmsg->dwData2;
		msg.dwData3 = pmsg->dwData3;
		msg.dwData4 = pmsg->dwData4;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CItemManager::Item_UnLockEx_Syn(void* pMsg)
{
	MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)
	{
		return;
	}
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (SetItemUnLock(pPlayer, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4, (eUNLOCKTYPE)pmsg->dwData5) != 1)
	{
		MSG_DWORD5 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_UNLOCKEX_NACK;
		msg.dwData1 = pmsg->dwData1;
		msg.dwData2 = pmsg->dwData2;
		msg.dwData3 = pmsg->dwData3;
		msg.dwData4 = pmsg->dwData4;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CItemManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{	
	MSGBASE * pmsg = (MSGBASE *)pMsg;
    CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

	if(pPlayer)
	if( BATTLESYSTEM->GetBattle(pPlayer)->GetBattleKind() == eBATTLE_KIND_SURYUN )
	{
		char buf[128];
		sprintf(buf, "荐访厘俊辑 酒捞袍 皋矫瘤甫 焊陈嚼聪促. character_Idx: %d, Protocol : %d", pmsg->dwObjectID, pmsg->Protocol);
		ASSERTMSG(0, buf);
		return;
	}

	switch(Protocol)
	{
	case MP_ITEM_MOVE_SYN:								Item_Move_Syn(pMsg);						break;
	case MP_ITEM_COMBINE_SYN:							Item_Combine_Syn(pMsg);						break;
	case MP_ITEM_DIVIDE_SYN:							Item_Divide_Syn(pMsg);						break;
	case MP_ITEM_DISCARD_SYN:							Item_Discard_Syn(pMsg);						break;
	case MP_ITEM_BUY_SYN:								Item_Buy_Syn(pMsg);							break;
	case MP_ITEM_SELL_SYN:								Item_Sell_Syn(pMsg);						break;
	case MP_ITEM_USE_SYN:								Item_Use_Syn(pMsg);							break;
	case MP_ITEM_MIX_ADDITEM_SYN:						Item_Mix_AddItem_Syn(pMsg);					break;
	case MP_ITEM_MIX_RELEASEITEM:						Item_Mix_ReleaseItem(pMsg);					break;
	case MP_ITEM_MIX_SYN:								Item_Mix_Syn(pMsg);							break;
	case MP_ITEM_TPM_ADDITEM_SYN:						Item_TPM_AddItem_Syn(pMsg);					break;
	case MP_ITEM_TPM_RELEASEITEM:						Item_TPM_ReleaseItem(pMsg);					break;
	case MP_ITEM_TPM_SYN:								Item_TPM_Syn(pMsg);							break;
	case MP_ITEM_UPGRADE_SYN:							Item_UpGrade_Syn(pMsg);						break;
	case MP_ITEM_REINFORCE_SYN:							Item_Reinforce_Syn(pMsg);					break;
	case MP_ITEM_REINFORCE_WITHSHOPITEM_SYN:			Item_Reinforce_WithShopItem_Syn(pMsg);		break;
	case MP_ITEM_DEALER_SYN:							Item_Dealer_Syn(pMsg);						break;
	case MP_ITEM_GUILD_WAREHOUSE_INFO_SYN:				Item_Guild_WareHouse_Info_Syn(pMsg);		break;
	case MP_ITEM_GUILD_MOVE_SYN:						Item_Guild_Move_Syn(pMsg);					break;
	case MP_ITEM_GUILD_WAREHOUSE_LEAVE:					Item_Guild_WareHouse_Leave(pMsg);			break;
	case MP_ITEM_PYOGUK_ITEM_INFO_SYN:					Item_Pyoguk_Item_Info_Syn(pMsg);			break;
	case MP_ITEM_PETINVEN_INFO_SYN:						Item_PetInven_Info_Syn(pMsg);				break;
	case MP_ITEM_USE_FOR_QUESTSTART_SYN:				Item_Use_For_QuestStart_Syn(pMsg);			break;
	case MP_ITEM_DISSOLUTION_SYN:						Item_Dissolution_Syn(pMsg);					break;
	case MP_ITEM_USE_CHANGEITEM_SYN:					Item_Use_ChangeItem_Syn(pMsg);				break;
	case MP_ITEM_SHOPITEM_INFO_SYN:						Item_ShopItem_Info_Syn(pMsg);				break;
	case MP_ITEM_SHOPITEM_USE_SYN:						Item_ShopItem_Use_Syn(pMsg);				break;
	case MP_ITEM_SHOPITEM_UNSEAL_SYN:					Item_ShopItem_Unseal_Syn(pMsg);				break;
	//case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:				Item_ShopItem_ChangeMap_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_MPINFO:						Item_ShopItem_MapInfo(pMsg);				break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_SYN:			Item_ShopItem_SavePoint_Add_Syn(pMsg);		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_SYN:			Item_ShopItem_SavePoint_Update_Syn(pMsg);	break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_DEL_SYN:			Item_ShopItem_SavePoint_Del_Syn(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_SYN:				Item_ShopItem_ReviveOther_Syn(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_ACK:				Item_ShopItem_ReviveOther_Ack(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_NACK:				Item_ShopItem_ReviveOther_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_AVATAR_TAKEOFF:				Item_ShopItem_Avatar_TakeOff(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_CHANGE:				Item_ShopItem_Avatar_Change(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_SYN:				Item_ShopItem_Avatar_Use_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_CHASEUSE_SYN:					Item_ShopItem_ChaseUse_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_CHASE_SYN:					Item_ShopItem_Chase_Syn(dwConnectionIndex, pMsg);				break;
	case MP_ITEM_SHOPITEM_NCHANGE_SYN:					Item_ShopItem_Nchange_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_SYN:				Item_ShopItem_CharChange_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_SEAL_SYN:						Item_ShopItem_Seal_Syn(pMsg);				break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_SYN:				Item_ShopItem_JobChange_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_SYN:			Item_ShopItem_ReinforceReset_Syn(pMsg);		break;
	case MP_ITEM_SHOPITEM_RARECREATE_SYN:				Item_ShopItem_RareCreate_Syn(pMsg);			break;
	case MP_ITEM_SHOPITEM_SHOUT_SYN:					Item_ShopItem_Shout_Syn(pMsg);				break;
	case MP_ITEM_SHOPITEM_SHOUT_ACK:					Item_ShopItem_Shout_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_SHOUT_NACK:					Item_ShopItem_Shout_Nack(pMsg);				break;
	case MP_ITEM_QUERY_EQUI_SYN:						Item_Query_Equi_Syn(pMsg);					break;
	case MP_ITEM_LOCKEX_SYN:							Item_LockEx_Syn(pMsg);						break;
	case MP_ITEM_UNLOCKEX_SYN:							Item_UnLockEx_Syn(pMsg);					break;
	default:
		break;
	}
}
void CItemManager::ItemExt_ShopItemCurse_Cancellation_AddItem_Syn(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	WORD wErrorCode = 0;
	CItemSlot * pSlot = NULL;

	if (GetTableIdxPosition(pmsg->ItemInfo.Position) != eItemTable_Inventory)
		wErrorCode = 1;

	if (!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0, 0, CB_EXIST | CB_ICONIDX))
		wErrorCode = 2;

	pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (pSlot)
	{
		if (pSlot->IsLock(pmsg->ItemInfo.Position))
			wErrorCode = 3;

		if (wErrorCode == 0)
		{
			pSlot->SetLock(pmsg->ItemInfo.Position, TRUE);
			MSG_ITEM msgAck;

			msgAck.Category = MP_ITEMEXT;
			msgAck.Protocol = MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_ACK;
			msgAck.ItemInfo = pmsg->ItemInfo;
			pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		}
		else
		{
			MSG_WORD2 msgNack;
			msgNack.Category = MP_ITEMEXT;
			msgNack.Protocol = MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_NACK;
			msgNack.wData1 = pmsg->ItemInfo.Position;
			msgNack.wData2 = wErrorCode;
			pPlayer->SendMsg(&msgNack, sizeof(msgNack));
		}
	}
}
void CItemManager::ItemExt_ShopItem_Curse_Cancellation_Release(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);

	if (pSlot)
	{
		pSlot->SetLock(pmsg->wData, FALSE);
	}
}
void CItemManager::ItemExt_ShopItem_Curse_Cancellation_Syn(void* pMsg)
{
	//[祝福武器功能][By:十里坡剑神][QQ:112582793][2018/4/15]
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	MSG_DWORD msg;
	SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_NACK);

	if (!CHKRT->ItemOf(pPlayer, pmsg->dwData2, (WORD)pmsg->dwData1, 0, 0, CB_EXIST | CB_ICONIDX))
	{
		msg.dwData = 1;
		goto UniqueItemCurseCancellation;
	}

	UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pmsg->dwData1);
	if (!pInfo)
	{
		msg.dwData = 2;
		goto UniqueItemCurseCancellation;
	}

	if (pInfo->dwCurseCancellation == 0)
	{
		msg.dwData = 2;
		goto UniqueItemCurseCancellation;
	}

	ITEMBASE* pItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData3);
	if (!pItem)return;
	if (pItem->dwDBIdx != pmsg->dwData4) return;
	if (EI_TRUE != DiscardItem(pPlayer, (POSTYPE)pmsg->dwData3, pItem->wIconIdx, 1))
	{
		msg.dwData = 3;
		goto UniqueItemCurseCancellation;
	}
	else
	{
		SEND_SHOPITEM_BASEINFO msg;
		SetProtocol(&msg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK);
		msg.ShopItemPos = (POSTYPE)pmsg->dwData3;
		msg.ShopItemIdx = pItem->wIconIdx;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
	}
	pItem = NULL;
	pItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->dwData2);
	if (!pItem) return;
	if (pItem->dwDBIdx != pmsg->dwData4)return;
	if (EI_TRUE == DiscardItem(pPlayer, pmsg->dwData2, (WORD)pmsg->dwData1, 1))
	{
		WORD wItemIdx = 0;
		DURTYPE ItemNum = 0;

		MSG_ITEM_DISCARD_ACK msg;
		msg.TargetPos = pmsg->dwData2;
		msg.wItemIdx = (WORD)pmsg->dwData1;
		msg.ItemNum = 1;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_DELETEITEM;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

		LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
			eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0,
			pItem->Durability, pPlayer->GetPlayerExpPoint());
	}

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, (WORD)pInfo->dwCurseCancellation, 1, EmptyCellPos, EmptyCellNum);
	if (obtainItemNum == 0)
		return;

	ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(ObtainItemDBResult)), (WORD)pInfo->dwCurseCancellation, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum);

UniqueItemCurseCancellation:
	pPlayer->SendMsg(&msg, sizeof(msg));
	return;
}
void CItemManager::ItemExt_Uniqueitem_Mix_AddItem_Syn(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	WORD wErrorCode = 0;
	CItemSlot * pSlot = NULL;

	if (GetTableIdxPosition(pmsg->ItemInfo.Position) != eItemTable_Inventory)
		wErrorCode = 1;

	if (!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0, 0, CB_EXIST | CB_ICONIDX))
		wErrorCode = 2;

	pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if (pSlot)
	{
		if (pSlot->IsLock(pmsg->ItemInfo.Position))
			wErrorCode = 3;

		if (wErrorCode == 0)
		{
			pSlot->SetLock(pmsg->ItemInfo.Position, TRUE);
			MSG_ITEM msgAck;

			msgAck.Category = MP_ITEMEXT;
			msgAck.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_ACK;
			msgAck.ItemInfo = pmsg->ItemInfo;
			pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		}
		else
		{
			MSG_WORD2 msgNack;
			msgNack.Category = MP_ITEMEXT;
			msgNack.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_NACK;
			msgNack.wData1 = pmsg->ItemInfo.Position;
			msgNack.wData2 = wErrorCode;
			pPlayer->SendMsg(&msgNack, sizeof(msgNack));
		}
	}
}
void CItemManager::ItemExt_Uniqueitem_Mix_Release(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);

	if (pSlot)
	{
		pSlot->SetLock(pmsg->wData, FALSE);
	}
}
void CItemManager::ItemExt_Uniqueitem_Mix_Syn(void* pMsg)
{
	MSG_UNIQUEITEM_MIX_SYN* pmsg = (MSG_UNIQUEITEM_MIX_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	WORD	wBasicItemIdx = pmsg->wBasicItemIdx;
	POSTYPE wBasicItemPos = pmsg->wBasicItemPos;
	WORD	wMaterialNum = pmsg->wMaterialNum;
	MATERIAL_ARRAY sMaterial[MAX_MIX_MATERIAL] = { 0, };
	CopyMemory(sMaterial, pmsg->Material, sizeof(MATERIAL_ARRAY) * MAX_MIX_MATERIAL);


	if (!CHKRT->ItemOf(pPlayer, wBasicItemPos, wBasicItemIdx, 0, 0, CB_EXIST | CB_ICONIDX))
		return;

	for (int i = 0; i < wMaterialNum; i++)
	{
		if (pmsg->wBasicItemPos == sMaterial[i].ItemPos)
			return;

		if (!CHKRT->ItemOf(pPlayer, sMaterial[i].ItemPos, sMaterial[i].wItemIdx, sMaterial[i].Dur, 0, CB_EXIST | CB_ICONIDX))
			return;
	}

	UNIQUE_ITEM_MIX_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemMixList(wBasicItemIdx);
	if (!pInfo)
		return;

	DWORD dwNeedItemIdx = 0;
	WORD wNeedItemDur = 0;
	for (int i = 0; i < pInfo->wMaterialKindNum; i++)
	{
		dwNeedItemIdx = pInfo->sUniqueItemMixMaterial[i].dwMaterialIdx;
		wNeedItemDur = pInfo->sUniqueItemMixMaterial[i].wMaterialNum;

		if (!EnoughMixMaterial((WORD)dwNeedItemIdx, wNeedItemDur, sMaterial, wMaterialNum))
			return;
	}

	ITEMBASE Item;
	memset(&Item, 0, sizeof(ITEMBASE));
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	for (int i = 0; i < wMaterialNum; i++)
	{
		Item = *pSlot->GetItemInfoAbs(sMaterial[i].ItemPos);
		if (Item.dwDBIdx == sMaterial[i].dwItemDBIdx)
		{
			if (EI_TRUE == ITEMMGR->DiscardItem(pPlayer, sMaterial[i].ItemPos, sMaterial[i].wItemIdx, sMaterial[i].Dur))
			{
				POSTYPE TargetPos = 0;
				WORD wItemIdx = 0;
				DURTYPE ItemNum = 0;

				MSG_ITEM_DISCARD_ACK msg;
				msg.TargetPos = sMaterial[i].ItemPos;
				msg.wItemIdx = sMaterial[i].wItemIdx;
				msg.ItemNum = sMaterial[i].Dur;
				msg.Category = MP_ITEMEXT;
				msg.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM;
				ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
					eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
					Item.Durability, pPlayer->GetPlayerExpPoint());
			}
		}
	}

	Item = *pSlot->GetItemInfoAbs(wBasicItemPos);
	if (Item.dwDBIdx != pmsg->dwBasicItemDBIdx)return;
	if (EI_TRUE == ITEMMGR->DiscardItem(pPlayer, wBasicItemPos, wBasicItemIdx, 1))
	{
		POSTYPE TargetPos = 0;
		WORD wItemIdx = 0;
		DURTYPE ItemNum = 0;

		MSG_ITEM_DISCARD_ACK msg;
		msg.TargetPos = wBasicItemPos;
		msg.wItemIdx = wBasicItemIdx;
		msg.ItemNum = 1;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

		LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
			eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}

	WORD wSeed = random(1, 100);
	WORD wResultItemIdx = 0;
	WORD wMinRate = 0, wMaxRate = 0;

	for (int i = 0; i < 5; i++)
	{
		wMinRate = wMaxRate;
		wMaxRate += pInfo->sUniqueItemMixResult[i].wResultRate;

		if (wSeed > wMinRate && wSeed <= wMaxRate)
		{
			wResultItemIdx = (WORD)pInfo->sUniqueItemMixResult[i].dwResultIdx;
			break;
		}
	}

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;

	WORD obtainItemNum = ITEMMGR->GetCanBuyNumInSpace(pPlayer, pSlot, wResultItemIdx, 1, EmptyCellPos, EmptyCellNum);
	if (obtainItemNum == 0)
		return;

	ITEMMGR->ObtainItemEx(pPlayer, ITEMMGR->Alloc(pPlayer, MP_ITEMEXT, MP_ITEMEXT_UNIQUEITEM_MIX_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(ObtainItemDBResult)), wResultItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum);
}
void CItemManager::ItemExt_SkinItem_Select_Syn(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)
		return;

	if (pmsg->dwData1 > 0)
	{
		DWORD dwResult = pPlayer->GetShopItemManager()->PutSkinSelectItem(pmsg);

		if (dwResult == eSkinResult_Success)
		{
			pPlayer->InitSkinDelay();
			pPlayer->StartSkinDelay();

			SEND_SKIN_INFO msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEXT_SKINITEM_SELECT_ACK;
			msg.dwObjectID = pPlayer->GetID();
			memcpy(&msg.wSkinItem, pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max);
			PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
		}
		else
		{
			SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(pmsg->dwData1);
			if (pSkinInfo == NULL)
				return;

			MSG_DWORD3 msg;
			SetProtocol(&msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_NACK);
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData1 = dwResult;
			msg.dwData2 = pPlayer->GetSkinDelayTime();
			msg.dwData3 = pSkinInfo->dwLimitLevel;

			pPlayer->SendMsg(&msg, sizeof(MSG_DWORD3));
		}
	}
	else if (pmsg->dwData1 == 0)
	{
		pPlayer->InitSkinDelay();
		pPlayer->StartSkinDelay();

		pPlayer->GetShopItemManager()->RemoveEquipSkin(pmsg->dwData2);

		SEND_SKIN_INFO msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEXT_SKINITEM_SELECT_ACK;
		msg.dwObjectID = pPlayer->GetID();
		memcpy(&msg.wSkinItem, pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max);
		PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
	}
}
void CItemManager::ItemExt_ShopItem_Decoration_On(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL)
		return;

	MSG_DWORD2 msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_SHOPITEM_DECORATION_ON;
	msg.dwObjectID = pPlayer->GetID();
	msg.dwData1 = pmsg->dwData1;
	msg.dwData2 = pmsg->dwData2;

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
}
void CItemManager::ItemExt_GoldMoney_Syn(void* pMsg)
{
	//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/22]
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)
	{
		return;
	}
	pPlayer->GetDBGoldMoney();
}
void CItemManager::ItemExt_Buff_Del_Syn(void* pMsg)
{
	//[右键删除buff服务端处理][By:十里坡剑神][QQ:112582793][2017/11/22]
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

	if (!pPlayer)
	{
		return;
	}
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pmsg->wData1);
	if (!pItemInfo)
	{
		return;
	}
	else
	{
		SHOPITEMWITHTIME* pInfo = pPlayer->GetShopItemManager()->GetUsingItemInfo(pmsg->wData1);
		if (!pInfo)
			return;
		if (pItemInfo->SellPrice == 1)
			pInfo->ShopItem.Remaintime = 0;
		else
			pInfo->ShopItem.Remaintime -= pInfo->ShopItem.Remaintime;
		pPlayer->GetShopItemManager()->CheckEndTime(TRUE);
		return;
	}
}
void CItemManager::ItemExt_AddPoint_Syn(void* pMsg)
{
	//[快速加点][By:十里坡剑神][QQ:112582793][2017/12/5]
	MSG_ADDPOINT* pmsg = (MSG_ADDPOINT*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	const ITEMBASE* pItemBase = NULL;

	MSG_ADDPOINT msg;
	msg.Category = MP_ITEMEXT;
	msg.dwItemDBidx = pmsg->dwItemDBidx;
	msg.dwItemPos = pmsg->dwItemPos;
	{
		pItemBase = GetItemInfoAbsIn(pPlayer, pmsg->dwItemPos);
		if (!pItemBase) return;

		if (pItemBase->dwDBIdx == pmsg->dwItemDBidx)
		{
			if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->dwItemPos, pItemBase->wIconIdx, 1))
			{
				char buf[64];
				sprintf(buf, "快速加点卷物品删除失败:角色ID %d", pPlayer->GetID());
				ASSERTMSG(0, buf);
				msg.Protocol = MP_ITEM_GOLDITEMUSE_NACK;
				pPlayer->SendMsg(&msg, sizeof(msg));
				return;
			}
		}
		else
		{
			msg.Protocol = MP_ITEM_GOLDITEMUSE_NACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
			return;
		}

	}
	msg.Protocol = MP_ITEM_ADDPOINT_ACK;
	{
		HERO_TOTALINFO* HeroInfo = pPlayer->GetHeroTotalInfo();

		if ((pmsg->GenGol + pmsg->MinChub + pmsg->CheRyuk + pmsg->SimMek) > HeroInfo->LevelUpPoint)
		{
			g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", 
				RGB(255, 0, 0), 
				"快速加点卷使用异常ID[%d]，角色名[%s]", 
				pPlayer->GetID(), pPlayer->GetObjectName());

			MSG_PLUGIN msg;
			msg.Category = MP_CLIENT;
			msg.Protocol = MP_CLINET_MSG_CLEARDISK;
			pPlayer->SendMsg(&msg, sizeof(msg));

			return;
		}
			

		HeroInfo->wGenGol += pmsg->GenGol;
		HeroInfo->wMinChub += pmsg->MinChub;
		HeroInfo->wCheRyuk += pmsg->CheRyuk;
		HeroInfo->wSimMek += pmsg->SimMek;

		pPlayer->SetGenGol(HeroInfo->wGenGol);
		pPlayer->SetMinChub(HeroInfo->wMinChub);
		pPlayer->SetCheRyuk(HeroInfo->wCheRyuk);
		pPlayer->SetSimMek(HeroInfo->wSimMek);
		HeroInfo->LevelUpPoint -= (pmsg->GenGol + pmsg->MinChub + pmsg->CheRyuk + pmsg->SimMek);

		pPlayer->SetPlayerLevelUpPoint(HeroInfo->LevelUpPoint);
	}
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::ItemExt_GoldItemUse_Syn(void* pMsg)
{
	//[元宝物品使用请求协议处理][By:十里坡剑神][QQ:112582793][2018/2/2]
	MSGGOLDITEM * pmsg = (MSGGOLDITEM *)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	const ITEMBASE* pItemBase = NULL;

	MSGGOLDITEM msg;
	msg.Category = MP_ITEMEXT;
	msg.dwItemDBidx = pmsg->dwItemDBidx;
	msg.dwGoldMoney = pmsg->dwGoldMoney;
	msg.dwItemPos = pmsg->dwItemPos;
	ITEM_INFO * pInfo = NULL;
	pInfo = m_ItemInfoList.GetData(pmsg->dwItemIdx);
	if (!pInfo) return;
	if (pInfo->SellPrice != pmsg->dwGoldMoney)
	{//[判断客户端文件和服务端文件是否一致防止客户端修改元宝数量][By:十里坡剑神][QQ:112582793][2018/2/2]
		msg.Protocol = MP_ITEM_GOLDITEMUSE_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));

		g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt",
			RGB(255, 0, 0), "元宝物品使用异常ID[%d]，角色名[%s]",
			pPlayer->GetID(), pPlayer->GetObjectName());

		MSG_PLUGIN msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLINET_MSG_CLEARDISK;
		pPlayer->SendMsg(&msg, sizeof(msg));

		return;
	}

	pItemBase = GetItemInfoAbsIn(pPlayer, pmsg->dwItemPos);
	if (!pItemBase)	return;

	if (pItemBase->dwDBIdx != pmsg->dwItemDBidx)
	{
		return;
	}
	if (pItemBase->wIconIdx != pmsg->dwItemIdx)
	{
		return;
	}


	//if (pItemBase->dwDBIdx == pmsg->dwItemDBidx)
	//{
		if (EI_TRUE != ITEMMGR->DiscardItem(pPlayer, pmsg->dwItemPos, pItemBase->wIconIdx, 1))
		{
			char buf[64];
			sprintf(buf, "元宝物品删除失败:角色ID %d", pPlayer->GetID());
			ASSERTMSG(0, buf);
			msg.Protocol = MP_ITEM_GOLDITEMUSE_NACK;
		}
		pPlayer->GetShopItemManager()->DeleteUsingShopItem(pItemBase->wIconIdx);
		pPlayer->SetGoldMoney(pInfo->SellPrice, 4);
		msg.Protocol = MP_ITEM_GOLDITEMUSE_ACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
	//}
}
void CItemManager::ItemExt_ShopItem_SuperShout_Syn(void* pMsg)
{
	//[超级狮子吼][By:十里坡剑神][QQ:112582793][2018/2/19]
	SEND_SHOUTBASE_ITEMINFO* pmsg = (SEND_SHOUTBASE_ITEMINFO*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	MSGBASE nmsg;
	SetProtocol(&nmsg, MP_ITEMEXT, MP_ITEM_SHOPITEM_SHOUT_NACK);

	if (IsUseAbleShopItem(pPlayer, pmsg->ItemIdx, pmsg->ItemPos) == FALSE)
	{
		pPlayer->SendMsg(&nmsg, sizeof(nmsg));
		return;
	}

	if (pmsg->ItemIdx == eSundries_Shout_Once ||
		pmsg->ItemIdx == eSundries_Shout_Once_NoTrade ||
		pmsg->ItemIdx == eSundries_SuperShout_Once ||
		pmsg->ItemIdx == eSundries_SuperShout_Once_NoTrade)
	{
		if (EI_TRUE != DiscardItem(pPlayer, pmsg->ItemPos, pmsg->ItemIdx, 1))
		{
			pPlayer->SendMsg(&nmsg, sizeof(nmsg));
			return;
		}

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
		msg.ShopItemIdx = pmsg->ItemIdx;
		msg.ShopItemPos = pmsg->ItemPos;
		ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));
	}

	SEND_SHOUTBASE msg;
	SetProtocol(&msg, MP_ITEMEXT, MP_ITEM_SHOPITEM_SUPERSHOUT_ACK);
	SafeStrCpy(msg.ShoutMsg[0].Msg, pmsg->Msg, MAX_SHOUT_LENGTH + 1);
	msg.ShoutMsg[0].CharacterIdx = pPlayer->GetID();
	msg.Count = 1;
	msg.ShoutMsg[0].IsSuperMsg = TRUE;
	g_Network.Send2AgentServer((char*)&msg, msg.GetSize());
}
void CItemManager::ItemExt_SendInfo_Syn(DWORD dwConnectionIndex, void* pMsg)
{
	//[发送物品信息查询][By:十里坡剑神][QQ:112582793][2018/4/3]
	MSG_GETSENDITEMINFO* pmsg = (MSG_GETSENDITEMINFO*)pMsg;
	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObject);
	if (pTarget == NULL) return;

	MSG_QUERY_EQUIPINF_FORSEND msg;
	memset(&msg, 0, sizeof(MSG_QUERY_EQUIPINF_FORSEND));

	memcpy(&msg.ItemBase, pTarget->GetItemInfoFromDBIdx(pmsg->dwItemDBIdx), sizeof(ITEMBASE));
	memcpy(&msg.ItemRateInfo, pTarget->GetItemRareOption(msg.ItemBase.RareIdx), sizeof(ITEM_RARE_OPTION_INFO));
	memcpy(&msg.ItemOptionInfo, pTarget->GetItemOption(msg.ItemBase.Durability), sizeof(ITEM_OPTION_INFO));
	msg.dwObjectID = pmsg->dwObjectID;
	msg.Category = MP_ITEMEXT;
	if (msg.ItemBase.dwDBIdx == 0)
		msg.Protocol = MP_ITEM_SENDINFO_NACK;
	else
		msg.Protocol = MP_ITEM_SENDINFO_ACK;

	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void CItemManager::ItemExt_NJQuest_Syn(void* pMsg)
{
	//[牛巨任务卷服务端处理逻辑][By:十里坡剑神][QQ:112582793][2018/6/11]
	MSGNJQUESTITEMUSE * pmsg = (MSGNJQUESTITEMUSE*)pMsg;
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer) return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 4;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	const ITEMBASE * pItem = GetItemInfoAbsIn(pPlayer, pmsg->wPos);
	if (!pItem) return;
	if (pItem->dwDBIdx != pmsg->dwItemDBidx)return;

	SEND_SHOPITEM_BASEINFO msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEX_NJQUEST_NACK;

	if (EI_TRUE != DiscardItem(pPlayer, pmsg->wPos, pmsg->dwItemIdx, 1))
	{
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	if (pmsg->dwItemIdx == eLazyItem_N)
	{
		LONG QuestIndex = 286;
		if (pmsg->bRepeat)
			QuestIndex = 153;
		CQuestEvent qe;
		//[第一步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:01]
		qe.SetValue(eQuestEvent_NpcTalk, 45, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第二步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:07]
		qe.SetValue(eQuestEvent_NpcTalk, 39, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第三步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:18]
		qe.SetValue(eQuestEvent_NpcTalk, 239, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第四步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:25]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第五步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:31]
		qe.SetValue(eQuestEvent_NpcTalk, 6, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第六步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:37]
		qe.SetValue(eQuestEvent_NpcTalk, 568, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
		//[第七步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:46]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[最后一步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:31:17]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
	}
	if (pmsg->dwItemIdx == eLazyItem_J)
	{
		LONG QuestIndex = 283;
		if (pmsg->bRepeat)
			QuestIndex = 23;
		CQuestEvent qe;
		//[第一步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:01]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[第二步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:07]
		qe.SetValue(eQuestEvent_NpcTalk, 249, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[第三步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:18]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[第四步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:25]
		qe.SetValue(eQuestEvent_NpcTalk, 58, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[第五步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:31]
		qe.SetValue(eQuestEvent_NpcTalk, 249, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);

		//[第六步][BY:十里坡剑神][QQ:112582793][2019-4-14][20:30:37]
		qe.SetValue(eQuestEvent_NpcTalk, 69, QuestIndex);
		QUESTMGR->AddQuestEvent(pPlayer, &qe);
	}
	msg.Protocol = MP_ITEMEX_NJQUEST_ACK;
	msg.ShopItemIdx = pItem->dwDBIdx;
	msg.ShopItemPos = pmsg->wPos;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::ItemExt_PakMove_Syn(void* pMsg)
{
	//[背包整理服务端处理逻辑][By:十里坡剑神][QQ:112582793][2018/6/11]
	MSGBASE * pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;

	if (0 == pPlayer->IsCanMovePack())
	{
		bool bRet = false;
		pPlayer->MoveInventPack(bRet);
		if (bRet)
			LoadCharacterItemInfoForMovePak(pPlayer->GetID());
		else
		{
			MSGBASE msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEX_PAKMOVED_ACK;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEX_PAKMOVE_NACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}
}
void CItemManager::ItemExt_Lock_Syn(void* pMsg)
{
	//[物品锁定][By:十里坡剑神][QQ:112582793][2018/6/17]
	MSG_ITEM_LOCK * pmsg = (MSG_ITEM_LOCK*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == NULL) return;
	const ITEMBASE * pItem = GetItemInfoAbsIn(pPlayer, pmsg->FromPos);
	if (!pItem) return;
	if (pItem->dwDBIdx != pmsg->dwItemDBIdx)return;
	CItemSlot * pToSlot = pPlayer->GetSlot(pmsg->FromPos);
	pToSlot->SetLock(pmsg->FromPos, pmsg->bIsLock);
}
void CItemManager::ItemExt_UpDate_GoldShop_Syn()
{
	//[商城信息重加载][By:十里坡剑神][QQ:112582793][2018/10/3]
	ReloadGoldShopItem();
	g_pUserTable->SetPositionUserHead();
	CPlayer* pPlayer = NULL;
	while (pPlayer = (CPlayer*)g_pUserTable->GetData())
	{
		if (pPlayer->GetObjectKind() == eObjectKind_Player)
			SendGoldShopItemInfo(pPlayer);
	}
}
void CItemManager::ItemExt_Inherit_Syn(void* pMsg)
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][22:19:01]
	MSG_ITEMINHERIT* pmsg = (MSG_ITEMINHERIT*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;
	if (pPlayer->IsSafeLock())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
		msg.dwData1 = 8;
		msg.dwData2 = (POSTYPE)pmsg->wOrigItemPos;
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}
	MSG_ITEMINHERIT msgAck;
	msgAck.Category = MP_ITEMEXT;
	msgAck.Protocol = MP_ITEMEX_INHERIT_NACK;

	msgAck.wInheritItemIdx = pmsg->wInheritItemIdx;
	msgAck.wInheritItemPos = pmsg->wInheritItemPos;
	msgAck.wOrigItemIdx = pmsg->wOrigItemIdx;
	msgAck.wOrigItemPos = pmsg->wOrigItemPos;

	ITEM_INFO * pOrigInfo = NULL;
	ITEM_INFO * pInheritInfo = NULL;
	pOrigInfo = GetItemInfo(pmsg->wOrigItemIdx);
	pInheritInfo = GetItemInfo(pmsg->wInheritItemIdx);

	if (pOrigInfo == NULL || pInheritInfo == NULL)
	{
		msgAck.dwObjectID = 2;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	if (pInheritInfo->wPulsItemEffect != pOrigInfo->ItemKind)
	{
		msgAck.dwObjectID = 1;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	if (pmsg->bIsGetGrow)
	{
		if (pPlayer->GetGoldMoney() < m_InheritGoldMoney)
		{//[传承成长扣减元宝检查][BY:十里坡剑神][QQ:112582793][2019-9-16][20:47:18]
			msgAck.dwObjectID = 0;
			pPlayer->SendMsg(&msgAck, sizeof(msgAck));
			return;
		}
	}
	if (!(pOrigInfo->ItemKind >= eEQUIP_ITEM_WEAPON&&pOrigInfo->ItemKind <= eEQUIP_ITEM_UNIQUE))
	{
		msgAck.dwObjectID = 3;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	if (pOrigInfo->LimitLevel < pInheritInfo->GenGol || pOrigInfo->LimitLevel > pInheritInfo->MinChub)
	{//[装备等级和传承石等级不匹配][BY:十里坡剑神][QQ:112582793][2019-8-22][14:21:27]
		msgAck.dwObjectID = 6;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return ;
	}

	if (pOrigInfo->ItemKind != pInheritInfo->wPulsItemEffect)
	{
		msgAck.dwObjectID = 7;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return ;
	}
	ITEMBASE* pInheritItem = NULL;
	pInheritItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->wInheritItemPos);
	ITEMBASE* pTargetItem =  NULL; 
	pTargetItem = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, (POSTYPE)pmsg->wOrigItemPos);
	if (pInheritItem->dwDBIdx!=pmsg->dwInheritItemDBIdx || pTargetItem->dwDBIdx !=pmsg->dwOrigItemDBIdx)
	{
		msgAck.dwObjectID = 2;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	if (pInheritItem->RareIdx == 0 && pTargetItem->RareIdx==0)
	{
		msgAck.dwObjectID = 4;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	CItemSlot* pSlot = NULL;
	pSlot = pPlayer->GetSlot(pmsg->wInheritItemPos);
	if (pSlot == NULL)
	{
		msgAck.dwObjectID = 2;
		pPlayer->SendMsg(&msgAck, sizeof(msgAck));
		return;
	}
	if (pInheritItem->RareIdx != 0)
	{//[属性附加操作][BY:十里坡剑神][QQ:112582793][2019-8-22][22:55:38]
		DWORD dwTempRareIdx = pInheritItem->RareIdx;
		DWORD dwTempGrowValue = pInheritItem->ItemGrow;
		ITEM_RARE_OPTION_INFO RareInfo;
		ITEM_RARE_OPTION_INFO* pRareInfo = pPlayer->GetItemRareOption(dwTempRareIdx);
		memcpy(&RareInfo, pRareInfo, sizeof(ITEM_RARE_OPTION_INFO));
		if (DiscardItem(pPlayer, pInheritItem->Position, pInheritItem->wIconIdx, 1, TRUE) != EI_TRUE)
		{
			msgAck.dwObjectID = 5;
			pPlayer->SendMsg(&msgAck, sizeof(msgAck));
			return;
		}
		ShopItemRareInsertToDB(pPlayer->GetID(), pTargetItem->wIconIdx, pTargetItem->Position, pTargetItem->dwDBIdx, &RareInfo, 1);

		if (dwTempGrowValue != 0)
			ItemGrowUpdate(pPlayer->GetID(), pTargetItem->dwDBIdx, dwTempGrowValue, TRUE);
		return;
	}
	else
	{
		if (!pmsg->bIsGetGrow)
		{//[不涉及成长属性继承操作][BY:十里坡剑神][QQ:112582793][2019-8-22][22:55:07]
			DWORD dwTempRareIdx = pTargetItem->RareIdx;
			pSlot->UpdateItemAbs(pPlayer, pTargetItem->Position, pTargetItem->dwDBIdx, pTargetItem->wIconIdx, 0, 0, 0, 0, UB_RARE, SS_CHKDBIDX, 0);

			//ItemUpdateToDB(pPlayer->GetID(), pTargetItem->Durability, pTargetItem->wIconIdx, pTargetItem->Durability, pTargetItem->Position,
			//	pTargetItem->QuickPosition, pTargetItem->RareIdx, pTargetItem->ItemStatic);

			ITEM_RARE_OPTION_INFO* pRareInfo = pPlayer->GetItemRareOption(dwTempRareIdx);
			ShopItemRareInsertToDB(pPlayer->GetID(), pInheritItem->wIconIdx, pInheritItem->Position, pInheritItem->dwDBIdx, pRareInfo, 1);
			pPlayer->RemoveItemRareOption(dwTempRareIdx);

			return;
		}
		else
		{//[附带成长继承操作][BY:十里坡剑神][QQ:112582793][2019-8-22][23:01:35]
		 //[附带成长只能进行继承，附加默认检查继承石成长值，不为0则附加到目标物品][BY:十里坡剑神][QQ:112582793][2019-8-22][23:05:22]
			
			
			DWORD dwTempRareIdx = pTargetItem->RareIdx;
			DWORD dwTempGrow = pTargetItem->ItemGrow;
			pSlot->UpdateItemAbs(pPlayer, pTargetItem->Position, pTargetItem->dwDBIdx, pTargetItem->wIconIdx, 0, 0, 0, 0, UB_RARE | UB_GROW, SS_CHKDBIDX, 0,0);
			ItemGrowUpdate(pPlayer->GetID(), pTargetItem->dwDBIdx, 0, TRUE);
			pSlot->UpdateItemAbs(pPlayer, pInheritItem->Position, pInheritItem->dwDBIdx, pInheritItem->wIconIdx, 0, 0, 0, 0, UB_RARE | UB_GROW, SS_CHKDBIDX, 0, dwTempGrow);

			//ItemUpdateToDB(pPlayer->GetID(), pTargetItem->Durability, pTargetItem->wIconIdx, pTargetItem->Durability, pTargetItem->Position,
			//	pTargetItem->QuickPosition, pTargetItem->RareIdx, pTargetItem->ItemStatic);

			ITEM_RARE_OPTION_INFO* pRareInfo = pPlayer->GetItemRareOption(dwTempRareIdx);
			ShopItemRareInsertToDB(pPlayer->GetID(), pInheritItem->wIconIdx, pInheritItem->Position, pInheritItem->dwDBIdx, pRareInfo, 1);
			pPlayer->RemoveItemRareOption(dwTempRareIdx);
			ItemGrowUpdate(pPlayer->GetID(), pInheritItem->dwDBIdx, dwTempGrow,TRUE);
			pPlayer->SetGoldMoney(m_InheritGoldMoney, eItemInherit);
			//LogGoldMoney(eLog_EquipItemGrowInherit, pPlayer->GetID(), (pPlayer->GetGoldMoney() + m_InheritGoldMoney), 65533, pPlayer->GetGoldMoney(), m_InheritGoldMoney, 0,0,0);
			ItemShopUseLog(eLog_EquipItemGrowInherit, pPlayer->GetID(), pPlayer->GetObjectName(), pTargetItem->wIconIdx, GetItemInfo(pTargetItem->wIconIdx)->ItemName,1,0,0,0,pPlayer->GetGoldMoney(),
				m_InheritGoldMoney, pPlayer->GetGoldMoney() - m_InheritGoldMoney, 0, 0, 0);
			return;
		}
	}
	

	ASSERT(0);
}
void CItemManager::ItemExt_FBItemTimeAdd_Syn(void* pMsg)
{
	MSG_INT2* pmsg = (MSG_INT2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)			return;

	if (pmsg->nData1 <= 0 || pPlayer->GetGoldMoney() < pmsg->nData1 * 1000)
	{
		g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", RGB(0, 0, 255), "副本时间增加数值异常 角色ID[%d] 角色名[%s] 角色元宝数[%d] 增加时间[%d] 所需元宝数[%d]",
			pPlayer->GetID(),pPlayer->GetObjectName(),pPlayer->GetGoldMoney(),pmsg->nData1,pmsg->nData1*1000);
		return;
	}
	pPlayer->SetGoldMoney(pmsg->nData1 * 1000, eNoneItem);
	ItemShopUseLog(2, pPlayer->GetID(), pPlayer->GetObjectName(), 110, "副本时间增加", pmsg->nData1, 0, 0, 0, pPlayer->GetGoldMoney() + pmsg->nData1 * 1000,
		pmsg->nData1 * 1000, pPlayer->GetGoldMoney(), 0, 0, 0);
	if (pPlayer->GetShopItemManager()->AddFBItemTime(pmsg->nData2, pmsg->nData1))
	{
		MSG_INT msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEX_FBTIMEADD_ACK;
		msg.nData = pmsg->nData1 * 3600000;
		pPlayer->SendMsg(&msg, sizeof(msg));
	}

}
void CItemManager::NetworkMsgParseExt( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE * pmsg = (MSGBASE *)pMsg;	

	switch(Protocol)
	{
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_SYN:	ItemExt_ShopItemCurse_Cancellation_AddItem_Syn(pMsg);	break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_RELEASE:		ItemExt_ShopItem_Curse_Cancellation_Release(pMsg);		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_SYN:			ItemExt_ShopItem_Curse_Cancellation_Syn(pMsg);			break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_SYN:					ItemExt_Uniqueitem_Mix_AddItem_Syn(pMsg);				break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_RELEASE:						ItemExt_Uniqueitem_Mix_Release(pMsg);					break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_SYN:							ItemExt_Uniqueitem_Mix_Syn(pMsg);						break;
	case MP_ITEMEXT_SKINITEM_SELECT_SYN:						ItemExt_SkinItem_Select_Syn(pMsg);						break;
	case MP_ITEMEXT_SHOPITEM_DECORATION_ON:						ItemExt_ShopItem_Decoration_On(pMsg);					break;
	case MP_ITEM_GOLD_MONEY_SYN:								ItemExt_GoldMoney_Syn(pMsg);							break;
	case MP_ITEM_BUF_DEL_SYN:									ItemExt_Buff_Del_Syn(pMsg);								break;
	case MP_ITEM_ADDPOINT_SYN:									ItemExt_AddPoint_Syn(pMsg);								break;
	case MP_ITEM_GOLDITEMUSE_SYN:								ItemExt_GoldItemUse_Syn(pMsg);							break;
	case MP_ITEM_SHOPITEM_SUPERSHOUT_SYN:						ItemExt_ShopItem_SuperShout_Syn(pMsg);					break;
	case MP_ITEM_SENDINFO_SYN:									ItemExt_SendInfo_Syn(dwConnectionIndex,pMsg);			break;
	case MP_ITEMEX_NJQUEST_SYN:									ItemExt_NJQuest_Syn(pMsg);								break;
	case MP_ITEMEX_PAKMOVE_SYN:									ItemExt_PakMove_Syn(pMsg);								break;
	case MP_ITEMEX_LOCK_SYN:									ItemExt_Lock_Syn(pMsg);									break;
	case MP_ITEMEX_UPDATE_GOLDSHOP_SYN:							ItemExt_UpDate_GoldShop_Syn();							break;
	case MP_ITEMEX_INHERIT_SYN:									ItemExt_Inherit_Syn(pMsg);								break;
	case MP_ITEMEX_FBTIMEADD_SYN:								ItemExt_FBItemTimeAdd_Syn(pMsg);						break;
	default:
		break;
	}
}
BOOL CItemManager::ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, WORD dwfromIconIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	if(TP_INVENTORY_START <= frompos && frompos < TP_WEAR_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_GUILDWAREHOUSE_START <= frompos && frompos < TP_GUILDWAREHOUSE_END)
	{
		ItemMoveGuildUpdateToDB(pPlayer->GetID(), pPlayer->GetGuildIdx(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_PYOGUK_START <= frompos && frompos < TP_PYOGUK_END)
	{
		ItemMovePyogukUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END)
	{
		ItemMoveShopUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}	
	else if(TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_PETINVEN_START <= frompos && frompos < TP_PETWEAR_END)
	{
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_TITANWEAR_START <= frompos && frompos < TP_TITANWEAR_END)
	{
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_TITANSHOPITEM_START <= frompos && frompos < TP_TITANSHOPITEM_END)
	{
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

void CItemManager::ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, WORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition,DWORD iRareIdx,WORD ItemStatic)
{
	if(TP_INVENTORY_START <= Position && Position < TP_WEAR_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition,iRareIdx,ItemStatic );
	}
	else if(TP_SHOPINVEN_START <= Position && Position < TP_SHOPINVEN_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition,iRareIdx,ItemStatic );
	}
	else if(TP_PETINVEN_START <= Position && Position < TP_PETINVEN_END )
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition,iRareIdx,ItemStatic );
	}
	else if(TP_GUILDWAREHOUSE_START <= Position && Position < TP_GUILDWAREHOUSE_END )
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition,iRareIdx,ItemStatic );
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CItemManager::CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer)
{
	CItemSlot * pFromSlot		= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot			= pPlayer->GetSlot(ToPos);

	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
	{
		if(!pToSlot->IsEmpty(ToPos)) 
			return FALSE;
		return TRUE;
	}
	
	return FALSE;
}

void CItemManager::ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos)
{
	if((TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END) 
		|| (TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END))
	{
		pItem->QuickPosition = 0;
		return;
	}
}
DealerData * CItemManager::GetDealer(WORD wDealerIdx)
{
	return m_DealerTable.GetData(wDealerIdx);
}
void CItemManager::LoadDealerItem()
{
	char buff[1024];
	char mapname[256];
	char npcname[256];
	BOOL IsItem = FALSE;
	WORD ItemIdx;

	int	ItemCount = 0;
	
	DealerItem* pItem;
	DealerData* pCurDealer = NULL;
	char seps[]   = "\t\n";
	char* token;
	int count = 0;

	CMHFile fp;
	fp.Init("./Resource/DealItem.bin","rb");

	while(1)
	{
		if(fp.IsEOF())
		{
			break;
		}
		count++;
		WORD map_num = fp.GetWord();
		fp.GetString(mapname);
		WORD npc_kind = fp.GetWord();
		fp.GetString(npcname);
		WORD npc_index = fp.GetWord();
		WORD point_x = fp.GetWord();
		WORD point_z = fp.GetWord();
		WORD angle = fp.GetWord();
		BYTE tabnum = fp.GetByte();
		BYTE Pos = 0;
		
		fp.GetLine(buff,1024);

		token = strtok( buff, seps );
		if(token == NULL)
			continue;
		token = strtok( NULL, seps );
		ItemIdx = atoi(token);

		token = strtok( NULL, seps );
		ItemCount = atoi(token);

		pCurDealer = m_DealerTable.GetData(npc_index);
		if(pCurDealer == NULL)
		{
			pCurDealer = new DealerData;
			m_DealerTable.Add(pCurDealer,npc_index);
		}
		
		if( ItemIdx != 0 )
		{
			pItem = new DealerItem;
			ASSERT(tabnum)
			pItem->Tab = tabnum-1;
			pItem->Pos = Pos++;
			pItem->ItemIdx = ItemIdx;
			pItem->ItemCount = ItemCount;
			pCurDealer->m_DealItemList.AddTail(pItem);
		}
		else
		{
			Pos++;
		}

		while( 1 )
		{
			token = strtok( NULL, seps );
			if(token == NULL)
				break;
			token = strtok( NULL, seps );
			ItemIdx = atoi(token);
			
			token = strtok( NULL, seps );
			ItemCount = atoi(token);

			if( ItemIdx != 0 )
			{
				pItem = new DealerItem;
				pItem->Tab = tabnum-1;
				pItem->Pos = Pos++;
				pItem->ItemIdx = ItemIdx;
				pItem->ItemCount = ItemCount;
				pCurDealer->m_DealItemList.AddTail(pItem);
			}
			else
			{
				Pos++;
			}
		}
	}
	fp.Release();
}

int CItemManager::ObtainItemFromQuest( CPlayer* pPlayer, WORD wItemKind, DWORD dwItemNum )
{//[任务获得物品，在此判断物品类型][By:十里坡剑神][QQ:112582793][2017/12/9]
	if(LIMITITEMDROPMGR->CheckCtrlDropItem(pPlayer->GetID(),wItemKind))
		dwItemNum=1;
	ITEM_INFO * pItemInfo	= GetItemInfo( wItemKind );
	if (!pItemInfo)
		return 0;

	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	WORD obtainItemNum=0;
	CItemSlot* pSlot=NULL;
	WORD wbSeal = 0;
	WORD wBuyType = 0;
	if (pItemInfo->ItemKind>=256&&pItemInfo->ItemKind<=271)
	{
		wbSeal=1;
		wBuyType=2;
		pSlot= pPlayer->GetSlot(eItemTable_ShopInven);
		obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, dwItemNum, EmptyCellPos, EmptyCellNum );
	}
	else
	{
		pSlot = pPlayer->GetSlot(eItemTable_Inventory);
		obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, dwItemNum, EmptyCellPos, EmptyCellNum );
	}
	sTIPITEMINFO * GetItem = NULL;
	GetItem = ITEMMGR->GetTipItem(wItemKind);
	if(GetItem)
	{
		MSG_TIP msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.Flag = eQuestGetItemTip;
		SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
		SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
		pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
	}
	if( obtainItemNum == 0 || pSlot==NULL)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_QUEST, MP_QUEST_TAKEITEM_ACK,
		pPlayer->GetID(), 0, eLog_ItemObtainQuest, obtainItemNum, (DBResult)(ObtainItemDBResult)),
		wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum,wbSeal,wBuyType );
	return 0;
}

int CItemManager::ObtainItemFromChangeItem( CPlayer* pPlayer, WORD wItemKind, WORD wItemNum,WORD ItemStatic )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;

	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD bSeal = 0;
	ITEM_INFO* pItemInfo = GetItemInfo( wItemKind );
	if( !pItemInfo )		return 2;
	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		bSeal = 1;
	}

	
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, wItemNum, EmptyCellPos, EmptyCellNum,ItemStatic );
	if( obtainItemNum == 0 )
		return 1;

	if( obtainItemNum != wItemNum )
		return 2;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_USE_CHANGEITEM_ACK,
						 pPlayer->GetID(), 0, eLog_ItemObtainFromChangeItem, obtainItemNum, (DBResult)(ObtainItemDBResult)),
						 wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, bSeal,ItemStatic );
}

BOOL CItemManager::CanbeMoved(WORD wIconIdx,POSTYPE pos, CPlayer* pPlayer)
{
	if(TP_INVENTORY_START<= pos && pos < TP_INVENTORY_END)
	{
//#ifdef _JAPAN_LOCAL_
//
//		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _JAPAN_LOCAL_
//#ifdef _HK_LOCAL_
//		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _HK_LOCAL_
//
//#ifdef _TL_LOCAL_
//
//		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _TL_LOCAL_

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eSHOP_ITEM )
			return FALSE;

		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !( pInfo->ItemKind & eEQUIP_ITEM ) )
		{
			if( ( pInfo->ItemKind & eQUEST_ITEM ) )
			{
			}
			else if( ( pInfo->ItemKind == eSHOP_ITEM_EQUIP ) )
			{
			}
			else
				return FALSE;
		}

		if(pInfo->EquipKind == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		
		return CanEquip(pInfo, pPlayer);
	}
	else if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
//#ifdef _JAPAN_LOCAL_
//
//		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _JAPAN_LOCAL_
//
//#ifdef _HK_LOCAL_
//
//		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _HK_LOCAL_
//
//#ifdef _TL_LOCAL_
//
//		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
//		if( pos >= GivenInvenCount )
//		{
//			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
//			if( ExtraSlot == 0 ||
//				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
//				return FALSE;
//		}
//
//#endif	// _TL_LOCAL_

		return TRUE;
	}
	else if(TP_GUILDWAREHOUSE_START <= pos && pos < TP_GUILDWAREHOUSE_END)
	{
		return TRUE;
	}
	else if(TP_SHOPITEM_START <= pos && pos < TP_SHOPITEM_END)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{		
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;		
		return TRUE;
	}
	//!!!脐 家券 咯何绰 绢叼辑 眉农秦拎具且鳖
	else if(TP_PETINVEN_START <= pos && pos < TP_PETINVEN_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eSHOP_ITEM )
			return FALSE;
		return TRUE;
	}
	else if(TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
			return TRUE;
		return FALSE;
	}
	else if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eTITAN_EQUIPITEM )
		{
			if(pos - TP_TITANWEAR_START == pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET)
				return TRUE;
		}
		return FALSE;
	}
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
			return TRUE;

		return FALSE;
	}
	else
		return FALSE;
}

BOOL CItemManager::CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer)
{
	if(pInfo->LimitLevel > pPlayer->GetLevel())
	{
		if( pPlayer->GetShopItemStats()->EquipLevelFree )
		{
			int gap = pInfo->LimitLevel - pPlayer->GetLevel();
			gap -= pPlayer->GetShopItemStats()->EquipLevelFree;
			if( gap > 0 )
				return FALSE;
		}
		else
			return FALSE;
	}	
	if(pInfo->LimitGenGol > pPlayer->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > pPlayer->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > pPlayer->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > pPlayer->GetCheRyuk())
	{
		return FALSE;
	}
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if(!CHKRT->ItemStageOf(pPlayer, (BYTE)pInfo->LimitJob))
			return FALSE;
	}		

	return TRUE;	
}

void CItemManager::ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem)
{
	DWORD FromID = 0;
	DWORD ToID = 0;
	WORD Logtype = 0;
	MONEYTYPE FromTotalMoney =0;
	MONEYTYPE ToTotalMoney = 0;
	char FromName[MAX_NAME_LENGTH+1];
	char ToName[MAX_NAME_LENGTH+1];


	if(TP_INVENTORY_START<= FromPos && FromPos < TP_WEAR_END)
	{
		FromID = pPlayer->GetID();
		FromTotalMoney = pPlayer->GetMoney();
		if(TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END)
		{			
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney(eItemTable_Pyoguk);
			Logtype = eLog_ItemMoveInvenToPyoguk;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Pyoguk", MAX_NAME_LENGTH+1);
		}
		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		{
			ToID = pPlayer->GetGuildIdx();
			Logtype = eLog_ItemMoveInvenToGuild;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Munpa", MAX_NAME_LENGTH+1);
		}
		else if(TP_PETINVEN_START <= ToPos && ToPos < TP_PETINVEN_END)
		{
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMoveInvenToPetInven;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#PetInven", MAX_NAME_LENGTH+1);
		}
		else
			return;
	}
	else if(TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetUserID();
			FromTotalMoney = pPlayer->GetMoney(eItemTable_Pyoguk);
			ToTotalMoney = pPlayer->GetMoney();
			ToID = pPlayer->GetID();
			Logtype = eLog_ItemMovePyogukToInven;
			
			SafeStrCpy(FromName, "#Pyoguk", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else if(TP_PYOGUK_START<= ToPos && ToPos < TP_PYOGUK_END)
		{
			return;
		}
		else
		{
			ASSERT(0); return;			
		}

	}
	else if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetGuildIdx();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMoveGuildToInven;
			SafeStrCpy(FromName, "#Munpa", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		{
			return;
		}
		else
		{
			ASSERT(0);
			return;
		}		
	}
	else if(TP_SHOPITEM_START <= FromPos && FromPos < TP_SHOPITEM_END)
	{
		if(TP_SHOPINVEN_START<= ToPos && ToPos < TP_SHOPINVEN_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ShopItemMoveToInven;
			SafeStrCpy(FromName, "#ShopHouse", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}		
	}
	else if(TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END)
	{
		if(TP_SHOPITEM_START<= ToPos && ToPos < TP_SHOPITEM_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ShopItemMoveToInven;
			SafeStrCpy(FromName, "#ShopInven", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}		
	}
	else if(TP_PETINVEN_START <= FromPos && FromPos < TP_PETINVEN_END)
	{
		if(TP_INVENTORY_START <= FromPos && FromPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMovePetInvenToInven;
			SafeStrCpy(FromName, "#PetInven", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}
	}
	else 
		return; 

	LogItemMoney(FromID, FromName, ToID, ToName,
		Logtype, FromTotalMoney, ToTotalMoney, 0,
		pItem->wIconIdx, pItem->dwDBIdx,  FromPos, ToPos, pItem->Durability, pPlayer->GetPlayerExpPoint());
}

WORD CItemManager::GetWeaponKind( WORD wWeapon )
{
	WORD rt = 0;
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wWeapon );
	if( pItemInfo )
		rt = pItemInfo->WeaponType;

	return rt;	
}

void CItemManager::GetItemKindType( WORD wItemIdx, WORD* wKind, WORD* wType )
{
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wItemIdx );
	if( pItemInfo )
	{
		*wKind = pItemInfo->ItemKind;
		*wType = pItemInfo->ItemType;
	}
	else
	{
		*wKind = 0;
		*wType = 0;
	}
}

DWORD CItemManager::MakeNewKey()
{
	if(m_Key == 0)
		++m_Key;
	return m_Key++; 
}

void CItemManager::AddGuildItemOption(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer)
{
	CItemSlot * pSlot;	
	const ITEMBASE * pItem;

	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
	{
		if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
			return;
		pSlot = pPlayer->GetSlot(ToPos);
		pItem = pSlot->GetItemInfoAbs(ToPos);
		if(IsOptionItem(pItem->wIconIdx, pItem->Durability))
			GUILDMGR->AddGuildItemOption(pPlayer->GetItemOption(pItem->Durability));

		if(IsRareOptionItem(pItem->wIconIdx, pItem->RareIdx))
			GUILDMGR->AddGuildItemRareOption(pPlayer->GetItemRareOption(pItem->RareIdx));
	}
}

void CItemManager::RemoveGuildItemOption(POSTYPE ToPos, const ITEMBASE* pItem, CPlayer* pPlayer)
{
	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		return;
	if(IsOptionItem(pItem->wIconIdx, pItem->Durability))
	{
		ITEM_OPTION_INFO * pInfo = GUILDMGR->GetItemOption(pItem->Durability);
		pPlayer->AddItemOption(pInfo);
		GUILDMGR->RemoveGuildItemOption(pItem->Durability);
	}
	if(IsRareOptionItem(pItem->wIconIdx, pItem->RareIdx))
	{
		ITEM_RARE_OPTION_INFO* pRareInfo = GUILDMGR->GetItemRareOption(pItem->RareIdx);
		pPlayer->AddItemRareOption(pRareInfo);
		GUILDMGR->RemoveGuildItemRareOption(pItem->RareIdx);
	}
}

int CItemManager::UseShopItem( CPlayer* pPlayer, SHOPITEMUSEBASE Info, SHOPITEMBASE* pShopItem )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return eItemUseErr_Err;
	if(!CHKRT->ItemOf(pPlayer, Info.ShopItemPos, Info.ShopItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return eItemUseErr_Err;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(Info.ShopItemPos);
	if( pSlot && pSlot->IsLock(Info.ShopItemPos) )
		return eItemUseErr_Lock;
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(Info.ShopItemPos);
	ITEM_INFO * pItemInfo = GetItemInfo(Info.ShopItemIdx);

	if(pPlayer->IsHide())
	if(IsUnfitItemInfoListForHide(Info.ShopItemIdx))
	{
		return eItemUseErr_Err;
	}

	if( pItemInfo && pItemInfo->ItemKind & eSHOP_ITEM )
	{
		if(pItemInfo->ItemKind!=eSHOP_ITEM_CHARM&&
			pItemInfo->ItemKind!=eSHOP_ITEM_HERB)
		{//[百宝使用安全锁][By:十里坡剑神][QQ:112582793][2018/5/7 0:23]
			if(pPlayer->IsSafeLock())
			{
				MSG_DWORD2 msg;
				msg.Category = MP_SAFELOCK;
				msg.Protocol = MP_SAFELOCK_LOCKEDTOCLIENT_ACK;
				msg.dwData1 = 1;
				pPlayer->SendMsg(&msg,sizeof(msg));
				return eItemUseErr_Err;
			}
		}
		return pPlayer->GetShopItemManager()->UseShopItem( (ITEMBASE*)pItemBase, Info, pShopItem );
	}
	else
	{
		ASSERT(0);
		return eItemUseErr_Err;
	}
	return eItemUseSuccess;
}

BOOL CItemManager::IsUseAbleShopItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE ItemPos )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return FALSE;
	if(!CHKRT->ItemOf(pPlayer, ItemPos, ItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return FALSE;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	if( pSlot && pSlot->IsLock(ItemPos) )
		return FALSE;

	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( ItemPos );
	if(!pItemBase)		return FALSE;
	if(pItemBase->wIconIdx != ItemIdx)
		return FALSE;
	
	ITEM_INFO * pItemInfo = GetItemInfo(ItemIdx);
	if( pItemInfo && ( pItemInfo->ItemKind & eSHOP_ITEM ) )
	{
		return TRUE;
	}
	return FALSE;
}

void CItemManager::ShopItemUseUpgrade( ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, WORD wItemIdx)
{
	if(!CHKRT->ItemOf(pPlayer, ItemPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
			return;
	const ITEMBASE* pSrcItem = ITEMMGR->GetItemInfoAbsIn( pPlayer, ItemPos );
	if(!pSrcItem)		return;

	DWORD tdw = pSrcItem->wIconIdx%10;
	if(tdw >= 9)
	{
		return;
	}

	DWORD plusoption = 0;

	if( (plusoption+tdw) > 9 )
		plusoption = 1;

	wItemIdx = (WORD)(pSrcItem->wIconIdx + plusoption);


	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	WORD flag = UB_ICONIDX;		
	if( !IsDupItem( (WORD)(pSrcItem->wIconIdx+plusoption) ) )
		flag |= UB_DURA;

	if(EI_TRUE != pSlot->UpdateItemAbs( pPlayer, ItemPos, 0, (WORD)(pSrcItem->wIconIdx+plusoption),0,0,0,0, flag, SS_LOCKOMIT))
		return;

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(ItemPos);
	ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition,pItemBase->RareIdx,pItemBase->ItemStatic);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse_MixSuccess, 
		pPlayer->GetMoney(), pSrcItem->wIconIdx+plusoption, pPlayer->GetMoney(), pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
		pPlayer->GetPlayerExpPoint());		
}

BOOL CItemManager::ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos)
{
	ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, absPos);
	if(!pItemBase)		return FALSE;

	CItemSlot* pItemSlot = pPlayer->GetSlot( absPos );	
	if( pItemSlot )
	if( pItemSlot->IsLock( absPos ) )
		return FALSE;
	
	if( pItemBase->ItemParam & ITEM_PARAM_SEAL )
	{
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->wIconIdx );
		if( pItemInfo )
		{
			if( pItemInfo->ItemType == 11 )
			{
				SHOPITEMWITHTIME* pItem = NULL;

				//if( pItemInfo->ItemKind != eSHOP_ITEM_PET )
				if( pItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( pItemInfo->ItemIdx ) )
					return FALSE;

				if( pPlayer->GetShopItemManager()->IsDupAble( pItemInfo ) == FALSE )
					return FALSE;
				
				SHOPITEMBASE	ShopItem;
				memcpy( &ShopItem.ItemBase, pItemBase, sizeof(ITEMBASE) );

				SYSTEMTIME systime;
				stTIME startime, usetime, endtime;
				GetLocalTime(&systime);
				startime.SetTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
				
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

				ShopItem.BeginTime = startime;
				ShopItem.Remaintime = endtime.value;
				ShopItem.ItemBase.ItemParam ^= ITEM_PARAM_SEAL;

				UsingShopItemUpdateToDB( 0, pPlayer->GetID(), ShopItem.ItemBase.wIconIdx, ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, ShopItem.BeginTime.value, ShopItem.Remaintime );

				ShopItem.Param = pItemInfo->SellPrice;
				pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItem );
				pPlayer->GetShopItemManager()->AddDupParam( pItemInfo );

				if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
					pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{					
					SEND_SHOPITEM_USEONE msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE;
					msg.ShopItemBase = ShopItem;
					pPlayer->SendMsg( &msg, sizeof(msg) );
				}				
				else
				{
					SEND_SHOPITEM_USEDINFO msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;
					msg.ItemCount = 1;
					msg.Item[0] = ShopItem;					
					pPlayer->SendMsg( &msg, msg.GetSize() );
				}
					LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					ShopItem.ItemBase.wIconIdx, ShopItem.ItemBase.dwDBIdx, ShopItem.ItemBase.Position, 0, ShopItem.ItemBase.Durability, pPlayer->GetPlayerExpPoint());
			}

			pItemBase->ItemParam ^= ITEM_PARAM_SEAL;			
			ShopItemParamUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->ItemParam);			
			return TRUE;
		}
		return FALSE;
	}
	else
		return FALSE;
}

BOOL CItemManager::CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam )
{
	if( wParam == eNpcParam_ShowpPyoguk )
		return TRUE;

	if( wNpcIdx == BOBUSANG_wNpcUniqueIdx )
	{
		return BOBUSANGMGR->CheckHackBobusangDist(pPlayer);
	}
	else if(wNpcIdx==AutoClearItmeSell)
	{
		return TRUE;
	}
	else
	{
		STATIC_NPCINFO* pInfo = GAMERESRCMNGR->GetStaticNpcInfo( wNpcIdx );
		if( pInfo == NULL )	return FALSE;

		if( pInfo->MapNum != g_pServerSystem->GetMapNum() )
			return FALSE;

		VECTOR3 vPos;
		pPlayer->GetPosition( &vPos );
		float dist = CalcDistanceXZ( &vPos, &pInfo->vPos );
		if( dist > 3000.0f )	return FALSE;

		return TRUE;
	}
}

int CItemManager::CheatObtainShopItem( CPlayer* pPlayer, WORD obtainItemIdx, DWORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;
	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1 );
}

BOOL CItemManager::CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot )
{
	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk ) != NULL ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7 ) != NULL ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7_NoTrade ) != NULL )
		return TRUE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );

	CItemSlot* pGuildSlot = pPlayer->GetSlot( eItemTable_MunpaWarehouse );
	CItemSlot* pWareSlot = pPlayer->GetSlot( eItemTable_Pyoguk );

	if( pFromSlot == pGuildSlot || pToSlot == pGuildSlot || pFromSlot == pWareSlot || pToSlot == pWareSlot )
	{
		if( !g_pServerSystem->GetMap()->IsVillage() )	return FALSE;
		if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;
	}
	return TRUE;
}

void CItemManager::SetItemOptionsInfoMsg( CPlayer* pItemOwner, ITEMBASE* pItemBase, MSG_LINKITEMOPTIONS* rtMsg )
{
	ITEM_OPTION_INFO*			pOptionInfo;
	ITEM_RARE_OPTION_INFO*		pRareOptionInfo;
	PET_TOTALINFO*				pPetTotalInfo;

	static char msgTemp[256] = {0,};

	if( IsOptionItem(pItemBase->wIconIdx, pItemBase->Durability) )
	{
		pOptionInfo = pItemOwner->GetItemOption(pItemBase->Durability);

		if(!pOptionInfo)
		{
			sprintf(msgTemp, "Data Absence [OptionInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_ItemOption;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO), pOptionInfo );
		}
	}

	if( IsRareOptionItem(pItemBase->wIconIdx, pItemBase->RareIdx) )
	{
		pRareOptionInfo = pItemOwner->GetItemRareOption(pItemBase->RareIdx);

		if(!pRareOptionInfo)
		{
			sprintf(msgTemp, "Data Absence [RareOptionInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_ItemRareOption;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO), pRareOptionInfo );
		}
	}

	if( IsPetSummonItem(pItemBase->wIconIdx) )
	{
		pPetTotalInfo = pItemOwner->GetPetManager()->GetPetTotalInfo(pItemBase->dwDBIdx);

		if(!pPetTotalInfo)
		{
			sprintf(msgTemp, "Data Absence [PetTotalInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_PetTotalInfo;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO), pPetTotalInfo );
		}

	}
}

BOOL CItemManager::LoadSetItemOption()
{
	CMHFile file;
	if(!file.Init("Resource/setitem_opt.bin", "rb"))
		return FALSE;

	SET_ITEM_OPTION* pInfo = NULL;
	while(!file.IsEOF())
	{
		ASSERT(!pInfo);
		pInfo = new SET_ITEM_OPTION;

		pInfo->wIndex = file.GetWord();
		pInfo->wSetItemKind = file.GetWord();
		SafeStrCpy( pInfo->szSetItemName, file.GetString(), MAX_NAME_LENGTH+1 );

		pInfo->wGenGol = file.GetWord();
		pInfo->wMinChub = file.GetWord();
		pInfo->wCheRyuk = file.GetWord();
		pInfo->wSimMek = file.GetWord();
		pInfo->dwLife = file.GetDword();
		pInfo->dwShield = file.GetDword();
		pInfo->dwNaeRyuk = file.GetWord();

		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->wLimitLevel = file.GetWord();
		SafeStrCpy( pInfo->szEffect, file.GetString(), 100 );
		pInfo->wLimitGenGol = file.GetWord();
		pInfo->wLimitMinChub = file.GetWord();
		pInfo->wLimitCheRyuk = file.GetWord();
		pInfo->wLimitSimMek = file.GetWord();

		pInfo->wMeleeAttackMin = file.GetWord();
		pInfo->wMeleeAttackMax = file.GetWord();
		pInfo->wRangeAttackMin = file.GetWord();
		pInfo->wRangeAttackMax = file.GetWord();
		pInfo->wCriticalPercent = file.GetWord();        		
		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());

		pInfo->wPhyDef = file.GetWord();
		pInfo->wLifeRecover = file.GetWord();
		pInfo->fLifeRecoverRate = file.GetFloat();
		pInfo->wNaeRyukRecover = file.GetWord();
		pInfo->fNaeRyukRecoverRate = file.GetFloat();
		pInfo->wSetValue = file.GetWord();
		pInfo->wApplicationValue = file.GetWord();

		ASSERT(!m_SetItemOptionList.GetData(pInfo->wIndex));

		m_SetItemOptionList.Add(pInfo, pInfo->wIndex);
		pInfo = NULL;
	}
	file.Release();
	return TRUE;
}

SET_ITEM_OPTION* CItemManager::GetSetItemOption(WORD wSetItemKind, WORD wSetValue)
{
	SET_ITEM_OPTION* pSetItemOption = NULL;

	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		if(pSetItemOption->wSetItemKind == wSetItemKind)
		{
			if(wSetValue == pSetItemOption->wSetValue)
			{				
				return pSetItemOption;
			}
		}
	}
	return NULL;
}

void CItemManager::RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut)
{
	SET_ITEM_OPTION* pInfo = m_SetItemOptionList.GetData(wIndex);

	if( pInfo == NULL )
	{
		ASSERT(0);		
		return;
	}
	if(pSetItemOptionOut)
		*pSetItemOptionOut = *pInfo;
	SAFE_DELETE( pInfo );
	m_SetItemOptionList.Remove(wIndex);
}

CYHHashTable<SET_ITEM_OPTION>* CItemManager::GetSetItemOptionList()
{
	return &m_SetItemOptionList;
}

BOOL CItemManager::CheckDemandItem( CPlayer* pPlayer, MSG_ITEM_BUY_SYN* pmsg )
{
	BOOL bNotEnough = FALSE;
	if( BOBUSANG_wNpcUniqueIdx == pmsg->wDealerIdx )
	{
		for( WORD i = 0; i < pmsg->slotNum; ++i )
		{
			if(!CHKRT->ItemOf(pPlayer, 
				pmsg->demandItem[i].demandItemPos,
				pmsg->demandItem[i].demandItemIdx,
				pmsg->demandItem[i].demandItemDur,
				0,
				CB_EXIST|CB_ICONIDX)
				)
				bNotEnough |= TRUE;
		}
	}
	if(bNotEnough)
		return FALSE;
	else	
	{
		BYTE rt = FALSE;
		for( WORD i = 0; i < pmsg->slotNum; ++i )
		{
			rt = UseItem(pPlayer, pmsg->demandItem[i].demandItemPos, pmsg->demandItem[i].demandItemIdx,0,TRUE);
			if( eItemUseSuccess == rt )
			{
				MSG_ITEM_USE_ACK msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_ACK;		
				msg.TargetPos = pmsg->demandItem[i].demandItemPos;
				msg.wItemIdx = pmsg->demandItem[i].demandItemIdx;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				ASSERT(0);
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CItemManager::CheckWeaponToShopItem(CPlayer* pPlayer, WORD wItemIndex)
{	
	ITEM_INFO * pItemInfo = GetItemInfo(wItemIndex);

	if(pItemInfo->WeaponType != 0)
	{
		const ITEMBASE * pItem = GetItemInfoAbsIn(pPlayer, TP_WEAR_START+1);
		
		if(pItem->wIconIdx == 0)
		{
			return FALSE;
		}
	}	
	return TRUE;
}

WORD CItemManager::ObtainItemFromGameEvent( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum )
{
	ITEM_INFO* pItemInfo = GetItemInfo( dwItemIdx );
	if( NULL == pItemInfo )
		return 0;			
	
	CItemSlot* pSlot = NULL;
	WORD wSeal = 0;
	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{		
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		wSeal = 1;
	}
	else
	{
        pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	}

	WORD wEmptyCellPos[255] = {0};
	WORD wEmptyCellNum;

	WORD wCanObtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, dwItemIdx, wItemNum, wEmptyCellPos, wEmptyCellNum );
	if( 0 == wCanObtainItemNum )
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_FULLINVEN_ALERT;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return 0;		
	}

	ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY,
        pPlayer->GetID(), 0, eLog_ItemObtainFromGameEvent, wCanObtainItemNum, (DBResult)(ObtainItemDBResult)),
        dwItemIdx, wCanObtainItemNum, wEmptyCellPos, wEmptyCellNum, wEmptyCellNum, wSeal );

	return wCanObtainItemNum;
}



int CItemManager::SetItemLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos)
{
	const ITEMBASE * pTargetItemBase = GetItemInfoAbsIn(pPlayer, ItemPos);

	const ITEMBASE * pStoneItemBase=GetItemInfoAbsIn(pPlayer,StonePos);

	if(!pTargetItemBase || !pStoneItemBase)
	{
		return 2;
	}

	if(StoneIdx!=eItemLockStone)
	{
		return 3;
	}

	if(TP_WEAR_START <= ItemPos && ItemPos < TP_WEAR_END) 
	{
        return 4;
	}
	
	ITEM_INFO * pTargetInfo = GetItemInfo(ItemIdx);

	if(!pTargetInfo)
	{
		return 6;
	}

	if(IsDupItem(ItemIdx) || pTargetItemBase->ItemStatic==ITEM_STATIC_PD) 
	{
		return 7;
	}
	if(pTargetItemBase->ItemStatic==ITEM_STATIC_LUCK)
	{
		return 8; 
	}

	ITEM_INFO * pStoneItem=ITEMMGR->GetItemInfo(StoneIdx);

	if(!pStoneItem)
	{
		return 9;
	}
	else
	{
	    ItemLockUpdate(pPlayer->GetID(),pTargetItemBase->dwDBIdx,3);  
		return 1;
	}
}

void CItemManager::ItemLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos,BYTE LockFlg)
{

	MSG_DWORD2BYTE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_LOCKEX_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  =wItemDBIdx;
	msg.dwData2  =TargetPos;
	msg.bData = LockFlg;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

int CItemManager::SetItemUnLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos,eUNLOCKTYPE type)
{
	const ITEMBASE * pTargetItemBase = GetItemInfoAbsIn(pPlayer, ItemPos);

	const ITEMBASE * pStoneItemBase=GetItemInfoAbsIn(pPlayer,StonePos);

	if(!pTargetItemBase || !pStoneItemBase)
	{
		return 2;
	}

	if(StoneIdx!=eItemUnLockStone&&StoneIdx!=eItemUnLockStoneForPD&&
		StoneIdx!=eItemMixEquiLow&&StoneIdx!=eItemMixEquiMid&&
		StoneIdx!=eItemMixEquiMax)
	{
		return 3;
	}
	
	if(TP_WEAR_START <= ItemPos && ItemPos < TP_WEAR_END)
	{
        return 4;
	}
	
	ITEM_INFO * pTargetInfo = GetItemInfo(ItemIdx);

	if(!pTargetInfo)
	{
		return 6;
	}
	if(pTargetInfo->ItemKind<eEQUIP_ITEM||pTargetInfo->ItemKind>eEQUIP_ITEM_UNIQUE)
	{
		return 7;
	}

	if(IsDupItem(ItemIdx))
	{
		return 7;
	}
	if((type==eUnLockForLock&&pTargetItemBase->ItemStatic!=ITEM_STATIC_LUCK)||
		(type==eUnLockForPD&&pTargetItemBase->ItemStatic!=ITEM_STATIC_PD)||
		(type==eUnMixEqui&&pTargetItemBase->ItemAbrasion==pTargetItemBase->MaxItemAbrasion))
	{
		return 8;
	}

	ITEM_INFO * pStoneItem=ITEMMGR->GetItemInfo(StoneIdx);

	if(!pStoneItem)
	{
		return 9;
	}
	if(type==eUnLockForLock||type==eUnLockForPD)
	{
		CItemSlot* pDSlot = pPlayer->GetSlot( (POSTYPE)ItemPos );
		if(!pDSlot) return 9;
		ITEMBASE* ItemBase = (ITEMBASE*)pDSlot->GetItemInfoAbs((POSTYPE)ItemPos);
		if(!ItemBase) return 9;
		pDSlot->UpdateItemAbs(pPlayer,ItemPos,ItemBase->dwDBIdx,ItemBase->wIconIdx,ItemBase->Position,ItemBase->QuickPosition,ItemBase->Durability,0,UB_UNLOCK,SS_NONE);
		
	    ItemUnLockUpdate(pPlayer->GetID(),pTargetItemBase->dwDBIdx,0);
		return 1;
	}
#ifdef _ABRASION_
	if (type==eUnMixEqui)
	{//[装备修理][By:十里坡剑神][QQ:112582793][2018/1/4]
		WORD Abrasion = pTargetItemBase->ItemAbrasion;
		Abrasion +=pStoneItem->Rarity;
		if (Abrasion>pTargetItemBase->MaxItemAbrasion)
			Abrasion =pTargetItemBase->MaxItemAbrasion;
		ItemAbrasionUpdate(pPlayer->GetID(),pTargetItemBase->dwDBIdx,Abrasion,pTargetItemBase->MaxItemAbrasion);
		
		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_UNLOCKEX_ACK;
		msg.dwObjectID= pPlayer->GetID();
		msg.dwData1  =pTargetItemBase->dwDBIdx;
		msg.dwData2  =ItemPos;
		pPlayer->SendMsg(&msg, sizeof(msg));
		
		return 1;
	}
#endif
}

void CItemManager::ItemUnLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos)
{
	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_UNLOCKEX_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  =wItemDBIdx;
	msg.dwData2  =TargetPos;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CItemManager::GrowItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, DWORD dwItemGrow)
{
   	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GROW_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  =wTargetItemDBIdx;
	msg.dwData2  =wTargetItemPos;
	msg.dwData3  =dwItemGrow;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CItemManager::AbrasionItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, WORD wItemAbrasion,WORD wMaxItemAbrasion)
{//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	MSG_DWORD4 msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEM_ABRASION_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  =wTargetItemDBIdx;
	msg.dwData2  =wTargetItemPos;
	msg.dwData3  =wItemAbrasion;
	msg.dwData4  =wMaxItemAbrasion;
	pPlayer->SendMsg(&msg, sizeof(msg));
}
void CItemManager::MakerItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, char * Maker,BYTE type)
{//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	MSG_DWORD2CHAR msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEM_MAKER_ACK;
	msg.dwObjectID= pPlayer->GetID();
	msg.dwData1  =wTargetItemDBIdx;
	msg.dwData2  =wTargetItemPos;
	msg.bType = type;
	strcpy(msg.Maker,Maker);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CItemManager::SetItemAbrasion(CPlayer * pPlayer,POSTYPE pos,WORD Value,BYTE Type)
{//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
	CItemSlot * pSlot = pPlayer->GetSlot(pos);
	ITEMBASE  *pTargetItem = (ITEMBASE  *)pSlot->GetItemInfoAbs(pos);
	switch(Type)
	{
	case 1: //[加]
		{
			pTargetItem->ItemAbrasion+=Value;
			if (pTargetItem->ItemAbrasion>pTargetItem->MaxItemAbrasion)
				pTargetItem->ItemAbrasion==pTargetItem->MaxItemAbrasion;
		}
		break;
	case 2: //[减]
		{
			pTargetItem->ItemAbrasion-=Value;
			pTargetItem->MaxItemAbrasion+=1;
			if (pTargetItem->MaxItemAbrasion>MAX_ITEM_ABRASION)
				pTargetItem->MaxItemAbrasion=MAX_ITEM_ABRASION;
			if (pTargetItem->ItemAbrasion>pTargetItem->MaxItemAbrasion)
				pTargetItem->ItemAbrasion=0;
		}
		break;
	}
	pSlot->UpdateItemAbs(pPlayer,pos,
						pTargetItem->dwDBIdx,
						pTargetItem->wIconIdx,
						pos,
						0,
						0,
						pTargetItem->ItemStatic,
						UB_ABRASION|UB_MAXABRASION,
						SS_NONE,
						0,
						0,
						pTargetItem->ItemAbrasion,
						pTargetItem->MaxItemAbrasion
						);
	ItemAbrasionUpdate(pPlayer->GetID(),pTargetItem->dwDBIdx,pTargetItem->ItemAbrasion,pTargetItem->MaxItemAbrasion);
}

void CItemManager::LoadMarryGetItemInfo()
{
	CMHFile file;
	if(!file.Init("Resource/Server/MarryGetItem.bin", "rb"))
		return ;

	MARRYGETITEM_INFO * TempInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;
		TempInfo = new MARRYGETITEM_INFO;
		TempInfo->ItemIdx = file.GetDword();	//[物品编号][BY:十里坡剑神][QQ:112582793][2019-8-14][21:15:34]
		TempInfo->Gender = file.GetWord();		//[性别 男=0，女=1 ][BY:十里坡剑神][QQ:112582793][2019-8-14][21:15:40]
		TempInfo->ItemType = file.GetWord();	//[物品类型，百宝=1，普通物品=0][BY:十里坡剑神][QQ:112582793][2019-8-14][21:15:47]
		TempInfo->ItemNum = file.GetWord();		//[数量][BY:十里坡剑神][QQ:112582793][2019-8-14][21:16:15]
		m_MarryGetItemInfo.Add(TempInfo,TempInfo->ItemIdx);
	}
	file.Release();
}

void CItemManager::LoadTipItem()
{//[箱子物品提示加载][By:十里坡剑神][QQ:112582793][2017/12/6]
	CMHFile file;
	if( !file.Init( "./Resource/Server/TipBoxItemList.bin", "rb" ) )
		return ;

	while(1)
	{
		if( file.IsEOF() )
			break;
		sTIPITEMINFO * pInfo = new sTIPITEMINFO;
		pInfo->ItemIndex = file.GetDword();
		file.GetString(pInfo->ItemName);
		if(!m_TipItemList.GetData(pInfo->ItemIndex))
			m_TipItemList.Add(pInfo,pInfo->ItemIndex);
	}
	file.Release();
}
sTIPITEMINFO * CItemManager::GetTipItem(WORD ItemIdx)
{
	sTIPITEMINFO * GetItem = NULL;
	GetItem=m_TipItemList.GetData(ItemIdx);
	if (NULL!=GetItem)
		return GetItem;
	else
		return NULL;
}

void CItemManager::LoadInheritGrowGoldMoney()
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][22:19:01]
	CMHFile file;
	if (!file.Init("./Resource/Server/InheritGold.bin", "rb"))
	{
		m_InheritGoldMoney = 5000;
		g_Console.LOGEX(4, RGB(255, 0, 0), "继承成长消耗元宝加载失败，当前消耗元宝数量[%d]", m_InheritGoldMoney);
		return;
	}
	m_InheritGoldMoney = file.GetDword();
	file.Release();
}