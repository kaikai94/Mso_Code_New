// ItemManager.h: interface for the CItemManager class.
// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)

#define AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "IndexGenerator.h"
#include "cLinkedList.h"
#include "HashTable.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "IndexManager.h"
#include <map>
class CMHFile;
struct ITEM_INFO;
class ITEM_MIX_INFO;
struct MATERIAL_ARRAY;
class ITEM_MIX_RES;
class ITEM_REINFORCE_INFO;

#define MAX_MIX_PERCENT	10000
#define MAX_MIX_LARGE_PERCENT	1000000
#define ITEMMGR CItemManager::GetInstance()

#define EVENTITEM_SHIELD_RECOVER 53031
#define EVENTITEM_ALL_RECOVER 53032

class CItemSlot;

struct DealerItem
{
	BYTE Tab;
	BYTE Pos;
	WORD ItemIdx;
	int	ItemCount;
};
struct DealerData
{

public:
	BOOL FindItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)
				return TRUE;
		}

		return FALSE;
	}

	DealerItem*	GetItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)
				return pItem;
		}

		return NULL;
	}

	void MakeSellingList(cPtrList* pList, DWORD tabNum)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->Tab == tabNum)
			{
				DealerItem* pTmp = new DealerItem;
				*pTmp = *pItem;
				pList->AddTail(pTmp);
			}
		}
	}

	cLinkedList<DealerItem> m_DealItemList;
};

class CItemManager  
{
	CYHHashTable<DealerData>	m_DealerTable;
	DWORD m_Key;
	DWORD m_InheritGoldMoney;
	DWORD MakeNewKey();

	


//#ifdef _HK_LOCAL_
//	int	m_nItemMixBal[99][9];
//#endif

public:
	CItemManager();
	virtual ~CItemManager();
	GETINSTANCE(CItemManager);
	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][22:19:01]
	void LoadInheritGrowGoldMoney();
	void LoadDealerItem();
	DealerData * GetDealer(WORD wDealerIdx);
	int DivideItem( CPlayer* pPlayer, WORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur,DWORD dwItemDBIdx = 0);
	int CombineItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur);
	int MoveItem( CPlayer * pPlayer, WORD FromItemIdx, POSTYPE FromPos, WORD ToItemIdx, POSTYPE& ToPos);
	int DiscardItem(CPlayer * pPlayer, POSTYPE whatPos, WORD whatItemIdx, DURTYPE whatItemNum,BOOL bIsCanDiscard=FALSE);
	void CheckAvataChange(CPlayer * pPlayer,WORD ItemIdx,WORD wWhatPos);
	void SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode);
	void SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize);
	void SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode );

	int ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD bSeal=0,WORD BuyType=0);  // 2014-12-02  默认0表示普通NPC购买! 1表示泡点购买 2表示购买
	int ObtainRareItem(CPlayer* pPlayer, ITEMOBTAINARRAYINFO* pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD ArrayInfoUnitNum, ITEM_RARE_OPTION_INFO* pRareOptionInfo );

	int SellItem( CPlayer* pPlayer, POSTYPE whatPos, WORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx,DWORD dwItemDBIdx );
	int BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx,DWORD ItemPos=0,WORD buyType=0); // 2014-12-02 默认0表示普通NPC购买!
	int BuyItemForGoldShop(CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx,WORD wMainIndex,WORD wSubIndex,DWORD ItemPos=0,WORD buyType=0);
	void MonsterObtainItem(CPlayer * pPlayer, WORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum = 1);

	int CheatObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	int CheatObtainRareItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum, ITEM_OPTION_INFO* pOptionInfo);
	//[签到物品发放][By:十里坡剑神][QQ:112582793][2018/1/8]
	int SingedObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	//[结婚物品发放][By:十里坡剑神][QQ:112582793][2018/1/8]
	int MarryObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	//[充值礼包发放][By:十里坡剑神][QQ:112582793][2018/1/31]
	int RechargeObtainItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	//[攻城战物品发放][By:十里坡剑神][QQ:112582793][2018/2/21]
	int SiegeWarObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,WORD bSel=0,WORD BuyType=0 );
	//[杀人抢夺物品][By:十里坡剑神][QQ:112582793][2018/2/21]
	int LootObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum,POSTYPE pos,WORD bSel=0,WORD BuyType=0 );
	static void LootObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID);

	static void ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID);
	static void BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID );	

	static void DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID);
	static void RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
	static void TitanUpgradeItemDBResult(CPlayer* pPlayer, WORD ArrayID);

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void NetworkMsgParseExt( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void LoadMixList();
	void LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	void LoadReinforceItemInfo();

	void LoadItemList();
	void ReloadItemList();

	WORD GetShopItemType(char * name);
	void LoadShopItemTypeInfo();
	void LoadGoldShopItem();
	void ReloadGoldShopItem();
	void SendGoldShopItemInfo(CPlayer* pPlayer);
    GOLD_SHOP_ITEM_INFO & GetGoldShopItem(WORD wItemIndex, WORD wMainIndex,WORD wSubIndex,WORD wGridIndex);




	int SetItemLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos);

	void ItemLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos,BYTE LockFlg);


	int SetItemUnLock(CPlayer * pPlayer,DWORD ItemIdx,WORD ItemPos, DWORD StoneIdx,WORD StonePos,eUNLOCKTYPE type);

	void ItemUnLockDBResult(CPlayer * pPlayer,DWORD wItemDBIdx,POSTYPE TargetPos);

	void SetItemInfo(WORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile);
	ITEM_INFO * GetItemInfo( WORD wItemIdx );
	ITEM_MIX_INFO * GetMixItemInfo(WORD wItemIdx);
	ITEM_REINFORCE_INFO * GetReinforceItemInfo(WORD wItemIdx);



	void GrowItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, DWORD dwItemGrow);
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	void AbrasionItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, WORD wItemAbrasion,WORD wMaxItemAbrasion);
	void MakerItemDBResult(CPlayer* pPlayer, DWORD wTargetItemDBIdx,POSTYPE wTargetItemPos, char * Maker,BYTE type);

	int UseItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx,DWORD dwItemDBIdx = 0,BOOL bIsDeal = FALSE );
	int UpgradeItem(CPlayer* pPlayer, WORD & TargetItemIdx, POSTYPE TargetPos, WORD MatItemIdx, POSTYPE MatItemPos);

	int MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan = FALSE);	// magi82 - Titan(070118)
	int ReinforceItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);
	BOOL IsSameUseItem(ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);
	void ReinforceItemDBResult(CPlayer * pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION_INFO * pOptionInfo);

	BOOL EnoughDissolveSpace(CPlayer * pPlayer, WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * pJewelArray, WORD & wJewelNum);
	BOOL EnoughDissolveInvSpace(CPlayer * pPlayer, WORD wJewelNum);
	
	BOOL EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum);

	ITEMOBTAINARRAYINFO *	Alloc(CPlayer * pPlayer, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc = NULL, DBResultEx CallBackFuncEx = NULL,int BuyType = 0);
	void Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info);
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareDBIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem( WORD wItemIdx );
	BOOL IsTitanCallItem( WORD wItemIdx );
	BOOL IsTitanEquipItem( WORD wItemIdx );
	
	BOOL ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, WORD dwfromIconIdx,  POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
	void ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, WORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition,DWORD iRareIdx,WORD ItemStatic);
	const ITEMBASE * GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos);

	void ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem);
	void AddGuildItemOption(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	void RemoveGuildItemOption(POSTYPE ToPos, const ITEMBASE* pItem, CPlayer* pPlayer);

	unsigned int GetTotalEmptySlotNum(CItemSlot* pSlot); //060612 Add by wonju
	WORD GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, DWORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum,WORD buyType=0);

private:

	WORD GetCanBuyNumInMoney(CPlayer * pPlayer, WORD butNum, MONEYTYPE Price);
	BOOL CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	BOOL CanbeMoved(WORD wIconIdx,POSTYPE pos, CPlayer* pPlayer);
	BOOL CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer);
	void ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos);

	CYHHashTable<ITEM_INFO> m_ItemInfoList;


	std::multimap<WORD,GOLD_SHOP_ITEM_INFO> m_ShopItemInfo;
	std::map<std::string,WORD> m_ShopItemType;
	typedef std::multimap<WORD,GOLD_SHOP_ITEM_INFO>::iterator Info_multimap;

	std::map<WORD,std::string> m_MixItemInfo;



	std::map<WORD,ITEM_MIX_INFO> m_MixItemInfoList;
	CYHHashTable<HILEVEL_ITEM_MIX_RATE_INFO> m_HiLevelItemMixRateInfoList;
	CYHHashTable<ITEM_REINFORCE_INFO> m_ReinforceItemInfoList;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList;
	CMemoryPoolTempl<ITEMOBTAINARRAYINFO> m_ItemArrayPool;
	//CIndexGenerator		m_ArrayIndexCreator;
	IndexManager m_ArrayIndexCreator;
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;

	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;

	CYHHashTable<MARRYGETITEM_INFO> m_MarryGetItemInfo;
	//[箱子物品提示链表][By:十里坡剑神][QQ:112582793][2017/12/6]
	CYHHashTable<sTIPITEMINFO>		m_TipItemList;
public:
	int		ObtainItemFromQuest( CPlayer* pPlayer, WORD wItemKind, DWORD dwItemNum );
	WORD	GetWeaponKind( WORD wWeapon );
	void	GetItemKindType( WORD wItemIdx, WORD* wKind, WORD* wType );

	int		ObtainItemFromChangeItem( CPlayer* pPlayer, WORD wItemKind, WORD wItemNum,WORD ItemStatic);
	BOOL	CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam=0 );	
	int UseShopItem(CPlayer* pPlayer, SHOPITEMUSEBASE pInfo, SHOPITEMBASE* pShopItem);
	BOOL IsUseAbleShopItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE ItemPos );
	void ShopItemUseUpgrade(ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, WORD wItemIdx);
	int ReinforceItemWithShopItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, WORD wShopItemIdx, POSTYPE ShopItemPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);

	BOOL LoadAvatarEquipList();
	BOOL ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos);
	
	int CheatObtainShopItem( CPlayer* pPlayer, WORD obtainItemIdx, DWORD ItemNum );
	
	BOOL	CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot );

	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////
	void	SetItemOptionsInfoMsg(CPlayer* pItemOwner, ITEMBASE* pItemBase, MSG_LINKITEMOPTIONS* rtMsg);
	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);
	void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	/////////////////////////////////////////////////////////////////////////////
	BOOL	CheckDemandItem(CPlayer* pPlayer, MSG_ITEM_BUY_SYN* pmsg);
	BOOL	PayForDeal(CPlayer* pPlayer, DWORD DealerIdx, DWORD dwPayItemIdx, POSTYPE PayItemPos = 99, DWORD dwPayItemNum = 1);

	BOOL CheckWeaponToShopItem(CPlayer* pPlayer, WORD wItemIndex); 

	WORD ObtainItemFromGameEvent( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum );
	//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
	void SetItemAbrasion(CPlayer * pPlayer,POSTYPE pos,WORD Value,BYTE Type);
	void LoadMarryGetItemInfo();
	CYHHashTable<MARRYGETITEM_INFO>* GetMarryGetItemInfo(){return &m_MarryGetItemInfo;}
	//[加载箱子物品提示信息][By:十里坡剑神][QQ:112582793][2017/12/6]
	void LoadTipItem();
	sTIPITEMINFO * GetTipItem(WORD ItemIdx);
	void LoadMixItemInfo();
	BOOL CanMixItem(WORD wItemIndex);

public:
	void Item_Move_Syn(void* pMsg);
	void Item_Combine_Syn(void* pMsg);
	void Item_Divide_Syn(void* pMsg);
	void Item_Discard_Syn(void* pMsg);
	void Item_Buy_Syn(void* pMsg);
	void Item_Sell_Syn(void* pMsg);
	void Item_Use_Syn(void* pMsg);
	void Item_Mix_AddItem_Syn(void* pMsg);
	void Item_Mix_ReleaseItem(void* pMsg);
	void Item_Mix_Syn(void* pMsg);
	void Item_TPM_AddItem_Syn(void* pMsg);
	void Item_TPM_ReleaseItem(void* pMsg);
	void Item_TPM_Syn(void* pMsg);
	void Item_UpGrade_Syn(void* pMsg);
	void Item_Reinforce_Syn(void* pMsg);
	void Item_Reinforce_WithShopItem_Syn(void* pMsg);
	void Item_Dealer_Syn(void* pMsg);
	void Item_Guild_WareHouse_Info_Syn(void* pMsg);
	void Item_Guild_Move_Syn(void* pMsg);
	void Item_Guild_WareHouse_Leave(void* pMsg);
	void Item_Pyoguk_Item_Info_Syn(void* pMsg);
	void Item_PetInven_Info_Syn(void* pMsg);
	void Item_Use_For_QuestStart_Syn(void* pMsg);
	void Item_Dissolution_Syn(void* pMsg);
	void Item_Use_ChangeItem_Syn(void* pMsg);
	void Item_ShopItem_Info_Syn(void* pMsg);
	void Item_ShopItem_Use_Syn(void* pMsg);
	void Item_ShopItem_Unseal_Syn(void* pMsg);
	void Item_ShopItem_ChangeMap_Syn(void* pMsg);
	void Item_ShopItem_MapInfo(void* pMsg);
	void Item_ShopItem_SavePoint_Add_Syn(void* pMsg);
	void Item_ShopItem_SavePoint_Update_Syn(void* pMsg);
	void Item_ShopItem_SavePoint_Del_Syn(void* pMsg);
	void Item_ShopItem_ReviveOther_Syn(void* pMsg);
	void Item_ShopItem_ReviveOther_Ack(void* pMsg);
	void Item_ShopItem_ReviveOther_Nack(void* pMsg);
	void Item_ShopItem_Avatar_TakeOff(void* pMsg);
	void Item_ShopItem_Avatar_Change(void* pMsg);
	void Item_ShopItem_Avatar_Use_Syn(void* pMsg);
	void Item_ShopItem_ChaseUse_Syn(void* pMsg); 
	void Item_ShopItem_Chase_Syn(DWORD dwConnectionIndex, void* pMsg);
	void Item_ShopItem_Nchange_Syn(void* pMsg);
	void Item_ShopItem_CharChange_Syn(void* pMsg);
	void Item_ShopItem_Seal_Syn(void* pMsg);
	void Item_ShopItem_JobChange_Syn(void* pMsg);
	void Item_ShopItem_ReinforceReset_Syn(void* pMsg);
	void Item_ShopItem_RareCreate_Syn(void* pMsg);
	void Item_ShopItem_Shout_Syn(void* pMsg);
	void Item_ShopItem_Shout_Ack(void* pMsg);
	void Item_ShopItem_Shout_Nack(void* pMsg);
	void Item_Query_Equi_Syn(void* pMsg);
	void Item_LockEx_Syn(void* pMsg);
	void Item_UnLockEx_Syn(void* pMsg);
public:
	
	void ItemExt_ShopItemCurse_Cancellation_AddItem_Syn(void* pMsg);
	void ItemExt_ShopItem_Curse_Cancellation_Release(void* pMsg);
	void ItemExt_ShopItem_Curse_Cancellation_Syn(void* pMsg);
	void ItemExt_Uniqueitem_Mix_AddItem_Syn(void* pMsg);
	void ItemExt_Uniqueitem_Mix_Release(void* pMsg);
	void ItemExt_Uniqueitem_Mix_Syn(void* pMsg);
	void ItemExt_SkinItem_Select_Syn(void* pMsg);
	void ItemExt_ShopItem_Decoration_On(void* pMsg);
	void ItemExt_GoldMoney_Syn(void* pMsg);
	void ItemExt_Buff_Del_Syn(void* pMsg);
	void ItemExt_AddPoint_Syn(void* pMsg);
	void ItemExt_GoldItemUse_Syn(void* pMsg);
	void ItemExt_ShopItem_SuperShout_Syn(void* pMsg);
	void ItemExt_SendInfo_Syn(DWORD dwConnectionIndex, void* pMsg);
	void ItemExt_NJQuest_Syn(void* pMsg);
	void ItemExt_PakMove_Syn(void* pMsg);
	void ItemExt_Lock_Syn(void* pMsg);
	void ItemExt_UpDate_GoldShop_Syn();
	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][22:19:01]
	void ItemExt_Inherit_Syn(void* pMsg);
	//[副本时间增加][BY:十里坡剑神][QQ:112582793][2019-9-21][15:15:57]
	void ItemExt_FBItemTimeAdd_Syn(void* pMsg);
};


#endif // !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)


