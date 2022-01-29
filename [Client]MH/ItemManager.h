// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)
#define AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "ItemShow.h"

#include "ItemGround.h"
#include "ItemCalc.h"
#include "IndexGenerator.h"
#include "MugongBase.h"
#include "Player.h"

class CDealDialog;
class CExchangeDialog;
class CMixDialog;
class CRecvExchangeDialog;
class ITEM_OPTION_INFO;
class cListDialog;
class cIcon;
class cImage;


#define ITEMMGR USINGTON(CItemManager)

#define PET_DEFAULT_GRADE	1

//---for item tooltip
struct TOOLTIP_TEXT
{
	TOOLTIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	TOOLTIP_TEXT*	pNext;
};
enum eRemainOptionKind{eNoneOption = 0, eRemainReinforceOption = 1, eRemainRareOption = 2, eRemainAllOption = 3, };
typedef std::multimap<WORD,GOLD_SHOP_ITEM_INFO>::iterator Info_multimap;
typedef std::map<WORD,std::string> ClearInfoMap;
typedef std::map<WORD,std::string>::iterator ClearMapIter;
class CItemManager  
{
	std::multimap<WORD,GOLD_SHOP_ITEM_INFO> m_ShopItemInfo;
	std::map<std::string,WORD> m_ShopItemType;
	std::map<WORD,std::string> m_MixItemInfo;
	CItemCalc m_ItemCalc;

	CYHHashTable<CItem> m_ItemHash;

	CYHHashTable<ITEM_INFO> m_ItemInfoList;

	ClearInfoMap m_ClearPacketInfoList;


	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;

	CYHHashTable<TOOLTIP_TEXT>		m_ItemToolTipTextList;

	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionList;
	CYHHashTable<SHOPITEMBASE>		m_UsedItemList;
	CYHHashTable<sRareItemInfo>		m_RareItemInfoTable;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList; 

	CIndexGenerator m_IconIndexCreator;
	BOOL			m_bAddPrice;
	ITEMBASE		m_TempDeleteItem;
	WORD	m_GrowItemVal;//[成长叠加倍数][BY:十里坡剑神][QQ:112582793][2019-8-9][14:06:29]
	//[贵重物品列表][BY:十里坡剑神][QQ:112582793][2019-8-28][12:56:02]
	std::map<WORD, std::string> m_ValuablesItemList;
public:
	CItemManager();
	virtual ~CItemManager();

	void LoadValuablesList();
	bool IsValuablesItem(WORD wItemIndex);
	bool IsOptionValuablesItem(DWORD OptionIndex);
	void InitItemRareOption(ITEM_RARE_OPTION_INFO* pInfo, WORD num);
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareOptionIdx);
	void RemoveItemRareOption(DWORD dwRareOptionIdx, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL);

	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);	
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind);
    void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);



	void LoadPacketInfoList();

  //  CYHHashTable<ITEM_PACKET_CLEAR> * GetClearPacketItem();

	/*ITEM_PACKET_CLEAR * GetClearPacketItem(DWORD ItemIdx);*/
	
	void AddClearPacketItem(cListDialog * Packet_List);
	void AddClearPacketItem(cListDialog * Packet_List,WORD wItemIdx);
	void DeleteClearPacketItem(WORD wItemIdx);
	void SaveClearPacketItemInfo();
	BOOL IsClearPacketItemCheck(DWORD ItemIdx);


	void AddGoldShopToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	void SetGoldShopToolTipIcon(cIcon* pIcon);

	void AddSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	DWORD GetSetItemEquipValue(ITEM_INFO* pItemInfo);
	void SetSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo, BOOL bSetItemEnable, WORD wSetValue);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	void SetItemEffectProcess(CPlayer* pPlayer);
	/////////////////////////////////////////////////////////////////////////////
	void AddUniqueItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo );	

	void InitItemOption(ITEM_OPTION_INFO * pInfo, WORD num);
	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);
	void RemoveItemOption(DWORD dwOptionIdx, ITEM_OPTION_INFO * pItemOptionInfoOut = NULL );

	void Release();

	CItem* MakeNewItem(ITEMBASE* pBaseInfo, char* strKind);
	
	void ItemDelete(CItem* pItem);
	BOOL FakeDeleteItem(CItem* pItem);
	void SendDeleteItem();
	void SendSellItem(CItem* pItem);
	
	cImage* GetIconImage(WORD ItemIdx, cImage * pImageGet);
	CItem* GetItem(DWORD dwDBIdx);
	void SetToolTipIcon(cIcon * pIcon, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo = NULL, DWORD dwItemDBIdx = 0,BOOL IsGoldShop=FALSE,BOOL bIsQuey=FALSE);

	void NetworkMsgParse(WORD Protocol,void* pMsg);
	void NetworkMsgParseExt(WORD Protocol,void* pMsg);


	void MoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void DivideItem(ITEMOBTAINARRAY * pMsg );
	
	void MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToPyoguk( MSG_ITEM_MOVE_ACK* pMsg );

	void MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToPetInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToTitanInven(MSG_ITEM_MOVE_ACK* pMsg);	
	void MoveItemToTitanShopInven(MSG_ITEM_MOVE_ACK* pMsg);	
		
	void CombineItem( MSG_ITEM_COMBINE_ACK * pMsg );
	void DeleteItem( POSTYPE absPos, CItem** ppItemOut, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL , ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL, WORD RemainOptionFlag = eNoneOption );
	BOOL AddItem(CItem* pItem);
	

	void SetYoungyakItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetMugongItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo=NULL,BOOL bIsQuey=FALSE);
	void SetExtraItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetItemToolTipForStage( cIcon * pIcon, ITEM_INFO * pInfo ); 
	void SetPetSummonItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx = 0 );
	void SetTitanEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo ); 
	void SetInheritItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, ITEM_RARE_OPTION_INFO* pRareOptionInfo = NULL);
	BYTE GetTableIdxForAbsPos(POSTYPE absPos);
	CItem* GetItemofTable(WORD TableIDX, POSTYPE absPos);

	const ITEMBASE* GetItemInfoAbsIn(CHero* pHero,POSTYPE absPos);

	void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL,BOOL bIsDeleteItemInfo=FALSE,BOOL bIsDeleteQuickInfo = FALSE);
	BOOL IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos,BOOL bIsMix = FALSE );

	void RefreshAllItem();

	void RefreshItem( CItem* pItem );
	void RefreshItemToolTip(DWORD ItemDBIdx);
	BOOL CanEquip(WORD wItemIdx);
	BOOL CanConvertToMugong(WORD wItemIdx,MUGONG_TYPE MugongType=MUGONGTYPE_MAX);

	void LoadItemList();
	void LoadItemToolTipList();
	ITEM_INFO * GetItemInfo(WORD wItemIdx);
	void SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count);
	DWORD m_dwStateParam;

	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareIdx );

	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem(WORD wItemIdx);
	BOOL IsTitanCallItem(WORD wItemIdx);
	BOOL IsTitanEquipItem(WORD wItemIdx);

	void SetPriceToItem( BOOL bAddPrice );

	void SetDisableDialog(BOOL val, BYTE TableIdx);

	TOOLTIP_TEXT* GetItemToolTipInfo( WORD wIdx );
	void AddItemDescriptionToolTip( cWindow* pWindow, WORD wIndex, DWORD dwTxtColor = TTTC_DESCRIPTION, int nSpaceLine = 1 );
	void AddShopItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );	

	void PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo );
	ITEM_INFO* FindItemInfoForName( char* strName );
	
	void ReLinkQuickPosition(CItem* pItem);	

	void AddUsedItemInfo(SHOPITEMBASE* pInfo);
	void RemoveUsedItemInfo(WORD wIndex, DWORD dwDBIdx=0);
	SHOPITEMBASE* GetUsedItemInfo(DWORD Index);
	CYHHashTable<SHOPITEMBASE>* GetUsedItemInfo();
	void Process();
	void DeleteShopItemInfo();
	void AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedPetEquipItemToolTip( SHOPITEMBASE* pInfo );
	
	void ReviveOtherOK();
	void ReviveOtherSync();
	void ReviveOtherCancel();
	void ReviveOtherConfirm( BOOL bRevive );
	
	void RefreshStatsBuffIcon();
	void AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	BOOL CheckItemStage( BYTE bItemStage );
	
	void ItemDropEffect( WORD wItemIdx );
	void MoneyDropEffect();

	BOOL LoadRareItemInfo();
	BOOL IsRareItemAble( DWORD ItemIdx )
	{
		if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
		return FALSE;
	}

	void SetIconColorFilter(DWORD dwDBIdx, DWORD clrRGBA);
	void SetIconColorFilter(cIcon* pIcon, DWORD clrRGBA);
#ifdef _GMTOOL_
	void SetItemIfoPositionHead()	{ m_ItemInfoList.SetPositionHead(); }
	ITEM_INFO* GetItemInfoData()	{ return m_ItemInfoList.GetData(); }
#endif
	void ClearGrowValue(){ m_GrowItemVal = 0; }
//#ifdef _JAPAN_LOCAL_
//	BOOL	CheckItemAttr( WORD wItemAttr );
//	BOOL	CheckItemAquireSkill( WORD wAcquireSkill1, WORD wAcquireSkill2 );
//#endif
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////
	enum eCheckBlankSlot{eCBS_Inven=1,eCBS_SInven=2,eCBS_All=3,eCBS_SInvenPlus,};
	void CheckInvenFullForAlert(int flg = eCBS_All);

	void GetItemOptionsAndToolTipFromInfoMsg(cIcon* pIcon,MSG_LINKITEMOPTIONS* pOptionsMsg);

	void SetItemRelationTip(cMultiLineText* ToolTip,int pos);
	BOOL IsRealItem(const char* ItemName);
	void LoadShopItemInfo();
	GOLD_SHOP_ITEM_INFO & GetGoldShopItemByTableIdx(WORD wMainIndex, WORD wSubIndex,WORD wGirdIndex);
	void ReLoadShopItemInfo();
	void LoadShopItemTypeInfo();
	WORD GetShopItemType(char * name);
	void AddGoldShopItem();
	BOOL CheckShopItemDate(DWORD dwTime);
	void LoadMixItemInfo();
	BOOL CanMixItem(WORD wItemIndex);
	DWORD GetItemGenerateIndex(){return m_IconIndexCreator.GenerateIndex();}
	void ReleaseGenerateIndex(DWORD val);
	WORD GetGrowVal(){ return m_GrowItemVal; }
#ifdef _GMTOOL_
	DWORD  m_Color;
	void SetDwordColor(DWORD color);
#endif
	void ItemCombineCheck(WORD TabIdx,CItem* FromItem);
public:
	void Item_TotalInfo_Local(void * pMsg);
	void Item_Pyoguk_Info_Ack(void * pMsg);
	void Item_PetInven_Info_Ack(void * pMsg);
	void Item_Monster_RareObtain_Notify(void * pMsg);
	void Item_Reinforce_Success_Ack(void* pMsg);
	void Item_Reinforce_Failed_Ack(void* pMsg);
	void Item_Reinforce_Nack(void* pMsg);
	void Item_Reinforce_WithShopItem_Nack(void* pMsg);
	void Item_Move_Ack(void* pMsg);
	void Item_Move_Nack(void* pMsg);
	void Item_Combine_Ack(void* pMsg);
	void Item_Combine_Nack();
	void Item_Divide_Ack(void* pMsg);
	void Item_Divide_Nack(void* pMsg);
	void Item_Monster_Obtain_Notify(void* pMsg);
	void Item_Discard_Ack(void* pMsg);
	void Item_Discard_Nack(void* pMsg);
	void Item_Use_Notify(void* pMsg);
	void Item_Use_Ack(void* pMsg);
	void Item_Use_Nack(void* pMsg);
	void Item_Mix_AddItem_Ack(void* pMsg);
	void Item_Mix_AddItem_Nack(void* pMsg);
	void Item_Mix_Success_Ack(void* pMsg);
	void Item_Mix_BigFailed_Ack(void* pMsg);
	void Item_Mix_Failed_Ack(void* pMsg);
	void Item_Mix_Msg(void* pMsg);
	void Item_TPM_AddItem_Ack(void* pMsg);
	void Item_TPM_AddItem_Nack(void* pMsg);
	void Item_TPM_Success_Ack(void* pMsg);
	void Item_TPM_BigFailed_Ack(void* pMsg);
	void Item_TPM_Failed_Ack(void* pMsg);
	void Item_TPM_Msg(void* pMsg);
	void Item_Mall_Money(void* pMsg);
	void Item_Gold_Money(void* pMsg);
	void Item_SW_Money(void* pMsg);
	void Item_UpGrade_Success_Ack(void* pMsg);
	void Item_Sell_Ack(void* pMsg);
	void Item_Sell_Nack(void* pMsg);
	void Item_Buy_Ack(void* pMsg);
	void Item_Buy_Nack(void* pMsg);
	void Item_Dealer_Ack(void* pMsg);
	void Item_Dealer_Nack(void* pMsg);
	void Item_Appearance_Change(void* pMsg);
	void Item_Money(void* pMsg);
	void Item_Obtain_Money(void* pMsg);
	void Item_Divide_NewItem_Notify(void* pMsg);
	void Item_Guild_Move_Ack(void* pMsg);
	void Item_Guild_Move_Nack(void* pMsg);
	void Item_Guild_WareHouse_Info_Ack(void* pMsg);
	void Item_DeleteFrom_GuildWare_Notify(void* pMsg);
	void Item_AddTo_GuildWare_Notify(void* pMsg);
	void Item_Pyoguk_Move_Ack(void* pMsg);
	void Item_Dissolution_GetItem(void* pMsg);
	void Item_Dissolution_Ack(void* pMsg);
	void Item_Dissolution_Nack(void* pMsg);
	void Item_Error_Nack(void* pMsg);
	void Item_ChangeItem_Nack();
	void Item_Use_ChangeItem_Ack(void* pMsg);
	void Item_ShopItem_Info_Ack(void* pMsg);
	void Item_ShopItem_UsedInfo(void* pMsg);
	void Item_ShopItem_MPInfo(void* pMsg);
	void Item_ShopItem_UseEnd(void* pMsg);
	void Item_ShopItem_OneMinute(void* pMsg);
	void Item_ShopItem_Use_Ack(void* pMsg);
	void Item_ShopItem_Use_Nack(void* pMsg);
	void Item_ShopItem_Msg(void* pMsg);
	void Item_ShopItem_Unseal_Ack(void* pMsg);
	void Item_ShopItem_Unseal_Nack(void* pMsg);
	void Item_ShopItem_MoneyProtect(void* pMsg);
	void Item_ShopItem_ExpProtect(void* pMsg);
	void Item_ShopItem_MixUp(void* pMsg);
	void Item_ShopItem_StateMinus(void* pMsg);
	void Item_ShopItem_SavePoint_Add_Ack(void* pMsg);
	void Item_ShopItem_SavePoint_Add_Nack(void* pMsg);
	void Item_ShopItem_SavePoint_Update_Ack(void* pMsg);
	void Item_ShopItem_SavePoint_Del_Ack(void* pMsg);
	void Item_ShopItem_ReviveOther_Syn(void* pMsg);
	void Item_ShopItem_ReviveOther_Ack(void* pMsg);
	void Item_ShopItem_ReviveOther_Nack(void* pMsg);
	void Item_ShopItem_Avatar_PutOn(void* pMsg);
	void Item_ShopItem_Avatar_TakeOff(void* pMsg);
	void Item_ShopItem_Avatar_Use(void* pMsg);
	void Item_ShopItem_Avatar_Info(void* pMsg);
	void Item_ShopItem_Avatar_Use_Ack(void* pMsg);
	void Item_ShopItem_Avatar_Use_AckDB(void* pMsg);
	void Item_ShopItem_Avatar_Use_Nack(void* pMsg);
	void Item_ShopItem_Avatar_Discard(void* pMsg);
	void Item_ShopItem_Equip_UseInfoDB(void* pMsg);
	void Item_ShopItem_Pet_Use(void* pMsg);
	void Item_ShopItem_Effect(void* pMsg);
	void Item_ShopItem_SlotoverItem(void* pMsg);
	void Item_ShopItem_ProtectAll(void* pMsg);
	void Item_ShopItem_Chase_Ack(void* pMsg);
	void Item_ShopItem_Chase_Nack(void* pMsg);
	void Item_ShopItem_Chase_Tracking(void* pMsg);
	void Item_ShopItem_NChange_Ack(void* pMsg);
	void Item_ShopItem_NChange_Nack(void* pMsg);
	void Item_ShopItem_CharChange_Ack(void* pMsg);
	void Item_ShopItem_CharChange_Nack(void* pMsg);
	void Item_ShopItem_CharChange(void* pMsg);
	void Item_ShopItem_Seal_Ack(void* pMsg);
	void Item_ShopItem_Seal_Nack(void* pMsg);
	void Item_ShopItem_JobChange_Ack(void* pMsg);
	void Item_ShopItem_JobChange_Nack(void* pMsg);
	void Item_ShopItem_ReinforceReset_Ack(void* pMsg);
	void Item_ShopItem_ReinforceReset_Nack(void* pMsg);
	void Item_ShopItem_RareCreate_Ack(void* pMsg);
	void Item_ShopItem_RareCreate_Nack(void* pMsg);
	void Item_ShopItem_Shout_SendAck(void* pMsg);
	void Item_EventItem_Use(void* pMsg);
	void Item_Query_Equi_Ack(void* pMsg);
	void Item_LockEx_Ack(void* pMsg);
	void Item_LockEx_Nack(void* pMsg);
	void Item_UnLockEx_Ack(void* pMsg);
	void Item_UnLockEx_Nack(void* pMsg);
	void Item_Grow_Ack(void* pMsg);

	void ItemExt_ShopItem_Curse_Cancellation_AddItem_Ack(void* pMsg);
	void ItemExt_ShopItem_Curse_Cancellation_AddItem_Nack(void* pMsg);
	void ItemExt_ShopItem_Curse_Cancellation_Ack(void* pMsg);
	void ItemExt_ShopItem_Curse_Cancellation_DeleteItem(void* pMsg);
	void ItemExt_UniqueItem_Mix_AddItem_Ack(void* pMsg);
	void ItemExt_UniqueItem_Mix_AddItem_Nack(void* pMsg);
	void ItemExt_UniqueItem_Mix_DeleteItem(void* pMsg);
	void ItemExt_UniqueItem_Mix_Ack(void* pMsg);
	void ItemExt_SkinItem_Select_Ack(void* pMsg);
	void ItemExt_SkinItem_Select_Nack(void* pMsg);
	void ItemExt_SkinItem_Discard_Ack(void* pMsg);
	void ItemExt_ShopItem_Decoration_On(void* pMsg);
	void ItemExt_Abrasion_Ack(void* pMsg);
	void ItemExt_Maker_Ack(void* pMsg);
	void ItemExt_GoldItemUse_Nack(void* pMsg);
	void ItemExt_GoldItemUse_Ack(void* pMsg);
	void ItemExt_NJQuest_Ack(void* pMsg);
	void ItemExt_SendInfo_Ack(void* pMsg);
	void ItemExt_PakMove_Ack(void* pMsg);
	void ItemExt_PakMove_Nack(void* pMsg);
	void ItemExt_UpDate_GoldShop_Begin(void* pMsg);
	void ItemExt_UpDate_GoldShop_Ing(void* pMsg);
	void ItemExt_UpDate_GoldShop_End(void* pMsg);
	void ItemExt_AddPoint_Nack(void* pMsg);
	void ItemExt_AddPoint_Ack(void* pMsg);
	void ItemExt_Inherit_Ack(void* pMsg);
	void ItemExt_Inherit_Nack(void* pMsg);
	void ItemExt_PackMoved_Ack();
	void ItemExt_FBItemTimeAdd_Ack(void* pMsg);
};


EXTERNGLOBALTON(CItemManager)

#endif // !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)


