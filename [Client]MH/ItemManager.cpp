// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ItemManager.h"
#include "DealDialog.h"
#include "ExchangeDialog.h"
#include "MixDialog.h"
#include <MMSystem.h>  
#include "./Interface/cIcon.h"
#include "./Interface/cImage.h"

#include "GameIn.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "cDivideBox.h"
#include "ObjectManager.h"
#include "DebugDlg.h"

#include "AppearanceManager.h"
#include "GameDef.h"
#include "QuickManager.h"
#include "QuickItem.h"
#include "ExchangeItem.h"
#include "SkillManager_Client.h"

#include "ChatManager.h"
#include "./Audio/MHAudioManager.h"
#include "mhFile.h"


#include "GuildWarehouseDialog.h"
#include "InventoryExDialog.h"
#include "PyogukDialog.h"
#include "QuickDialog.h"
#include "ChatDialog.h"
#include "GameEventManager.h"
#include "UpgradeDlg.h"
#include "ReinforceDlg.h"
#include "MHMap.h"
#include "DissolutionDialog.h"
#include "MugongManager.h"
#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "CharacterDialog.h"
#include "cMsgBox.h"
#include "ObjectStateManager.h"
#include "MoveManager.h"
#include "EventMapInfo.h"

#include "ChaseDialog.h"
#include "NameChangeDialog.h"
#include "ReviveDialog.h"
#include "CharChangeDlg.h"

#include "GMNotifyManager.h"
#include "SealDialog.h"
#include "SiegeWarMgr.h"
#include "ChangeJobDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"

#include "ReinforceManager.h"
#include "../StatsCalcManager.h"

#include "TitanPartsMakeDlg.h"
#include "TitanMixDlg.h"
#include "TitanUpgradeDlg.h"
#include "TitanBreakDlg.h"
#include "TitanRepairDlg.h"
#include "TitanInventoryDlg.h"
#include "TitanRegisterDlg.h"
#include "TitanDissolutionDlg.h"
#include "TitanGuageDlg.h"
#include "PetManager.h"
#include "PetInventoryDlg.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "UniqueItemMixDlg.h"
#include "SkinSelectDialog.h"
#include "CostumeSkinSelectDialog.h"
#include "StreetStallManager.h"
#include "FortWarDialog.h"
#include "FortWarManager.h"
#include "OtherCharacterEquip.h" // character equip include 2014-06-02
#include "ItemLockManager.h"         // the item lock dlg 2014-12-06
#include "ItemUnLockManager.h"       // the item unlock dlg 2014-12-07
#include "./Interface/cListDialog.h" 
#include "shlwapi.h"
#include <tlhelp32.h>
#pragma comment(lib,"shlwapi.lib")
#include <Psapi.h>  
#pragma comment (lib,"Psapi.lib")
#include <algorithm>

#include "TitanManager.h"
#include "ErrorMsg.h"
//#ifdef _TL_LOCAL_
//#include "./Interface/cFont.h"
//#endif
#include "PetManager.h"
#include "BuyItem.h"
#include "cSendItemInfoDlg.h"	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "MainBarDialog.h"
#include <fstream>
#include "GoldShopMainDailog.h"
#include "cShopItemInventPlusDlg.h"//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
//#include "ReDisConnManager.h"
#include "FastConfigPointDlg.h"
#include "FBTipDlg.h"
#include "cEquipItemInherit.h"	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-23][0:57:03]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CItemManager)


CItemManager::CItemManager()
{
	m_ItemHash.Initialize(200);
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);


	m_UnfitItemInfoListForHide.Initialize(100);
	m_ItemToolTipTextList.Initialize(64);		
	m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_UsedItemList.Initialize(32);
	m_IconIndexCreator.Init(IG_MUGONG_STARTINDEX-IG_ITEM_STARTINDEX, IG_ITEM_STARTINDEX);
	m_bAddPrice = FALSE;
	m_RareItemInfoTable.Initialize(500);  
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
	m_SetItemOptionList.Initialize(MAX_SETITEM_KIND_NUM);
	m_GrowItemVal = 0;
#ifdef _GMTOOL_
	m_Color = RGB(230,180,255);
#endif
}

CItemManager::~CItemManager()
{
	//m_GroundHash.R
	Release();
	m_IconIndexCreator.Release();

	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
	{
		delete pInfo;
		pInfo = NULL;
	}
	m_ItemInfoList.RemoveAll();


	m_UnfitItemInfoListForHide.RemoveAll();	

	TOOLTIP_TEXT* pToolTipText = NULL;
	TOOLTIP_TEXT* pTemp = NULL;

	m_ItemToolTipTextList.SetPositionHead();
	while( pToolTipText = m_ItemToolTipTextList.GetData() )
	{
		do
		{
			pTemp = pToolTipText->pNext;
			if( pToolTipText->strToolTip )
				delete[] pToolTipText->strToolTip;
			delete pToolTipText;
			pToolTipText = pTemp;

		} while( pToolTipText );
	}
	m_ItemToolTipTextList.RemoveAll();

	SHOPITEMBASE* pShopItemInfo = NULL;
	m_UsedItemList.SetPositionHead();
	while(pShopItemInfo = m_UsedItemList.GetData())
		delete pShopItemInfo;
	m_UsedItemList.RemoveAll();

	sRareItemInfo* pList = NULL;
	m_RareItemInfoTable.SetPositionHead();
	while( pList = m_RareItemInfoTable.GetData() )
	{
		delete pList;
		pList = NULL;
	}
	m_RareItemInfoTable.RemoveAll();

	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
		delete pSetItemOption;
	m_SetItemOptionList.RemoveAll();
	//[Map清理][BY:十里坡剑神][QQ:112582793][2019-4-7][12:55:47]
	m_ShopItemType.clear();
	m_MixItemInfo.clear();
	m_ShopItemInfo.clear();
	m_ClearPacketInfoList.clear();
	m_GrowItemVal = 0;
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
void CItemManager::LoadValuablesList()
{
	CMHFile file;
	if (file.Init("./Resource/Client/ValuablesList.bin", "rb") == FALSE)
		return;
	char buff[64] = { 0, };
	while (1)
	{
		if (file.IsEOF())
			break;
		WORD t_wTmp = file.GetWord();
		file.GetString(buff);
		m_ValuablesItemList[t_wTmp] = buff;
	}
	file.Release();
}
bool CItemManager::IsValuablesItem(WORD wItemIndex)
{
	std::map<WORD, std::string>::iterator iter;
	iter = m_ValuablesItemList.find(wItemIndex);
	if (iter != m_ValuablesItemList.end())
		return true;
	else
		return false;
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
		ASSERT(0);
		return 0;
	}
}
void CItemManager::LoadShopItemInfo()
{
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
			pInfo.dwCanBuyTime		= file.GetDword();
			pInfo.bBunding		= file.GetBool();
			std::pair<WORD,GOLD_SHOP_ITEM_INFO> p1(pInfo.wItemIndex,pInfo);
			m_ShopItemInfo.insert(p1);
		}
	}
	file.Release();
}
BOOL CItemManager::CheckShopItemDate(DWORD dwTime)
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return Time.value < dwTime;
}
void CItemManager::AddGoldShopItem()
{
	for(Info_multimap iter = m_ShopItemInfo.begin();iter!=m_ShopItemInfo.end(); )
	{
		WORD wCount = m_ShopItemInfo.count(iter->first);
		if(2<=wCount)
		{
			std::pair<Info_multimap,Info_multimap> InfoPair = m_ShopItemInfo.equal_range(iter->first);
			Info_multimap iter2 = InfoPair.first;
			while(iter2!=InfoPair.second)
			{
				if(iter2->second.dwCanBuyTime!=0)
				{
					if(!CheckShopItemDate(iter2->second.dwCanBuyTime))
					{
						++iter2;
						++iter;
						continue;
					}
				}
				GAMEIN->GetGoldShopMainDlg()->AddGoldShopItem(iter2->second);
				++iter2;
				++iter;
			}
		}
		else
		{
			if(iter->second.dwCanBuyTime!=0)
			{
				if(!CheckShopItemDate(iter->second.dwCanBuyTime))
				{
					++iter;
					continue;
				}
			}
			GAMEIN->GetGoldShopMainDlg()->AddGoldShopItem(iter->second);
			++iter;
		}
	}
}
GOLD_SHOP_ITEM_INFO & CItemManager::GetGoldShopItemByTableIdx(WORD wMainIndex, WORD wSubIndex,WORD wGirdIndex)
{
	for(Info_multimap iter = m_ShopItemInfo.begin();iter!=m_ShopItemInfo.end();iter++ )
	{
		if(iter->second.wMainIndex==wMainIndex&&iter->second.wSubIndex==wSubIndex&&iter->second.wGridIndex==wGirdIndex)
			return iter->second;
	}
	GOLD_SHOP_ITEM_INFO Temp;
	memset(&Temp,0,sizeof(GOLD_SHOP_ITEM_INFO));
	return Temp;
}
void CItemManager::ReLoadShopItemInfo()
{
	if(m_ShopItemInfo.size()>0)
		m_ShopItemInfo.clear();
	LoadShopItemInfo();
}
cImage * CItemManager::GetIconImage(WORD ItemIdx, cImage * pImageGet)
{
	SCRIPTMGR->GetImage( GetItemInfo(ItemIdx)->Image2DNum, pImageGet, PFT_ITEMPATH );	
	return pImageGet;
}

CItem* CItemManager::MakeNewItem(ITEMBASE* pBaseInfo, char* strKind)
{
	ITEM_INFO* pItemInfo = GetItemInfo(pBaseInfo->wIconIdx);

	if(pItemInfo == NULL)
	{
		char temp[256];
		sprintf(temp,"NoItemInfo %d,  %d,  %s",HEROID,pBaseInfo->wIconIdx,strKind);
		ASSERTMSG(pItemInfo, temp);
		return NULL;
	}
	
	if(pBaseInfo->dwDBIdx == 0)
		return NULL;

	CItem* pItem = NULL;
	if(!(pItem = GetItem(pBaseInfo->dwDBIdx)))
	{
		DWORD ItemID = m_IconIndexCreator.GenerateIndex();
		char temp[256];
		sprintf(temp,"%d",ItemID);
		ASSERTMSG((ItemID<=4999), temp);
		pItem = new CItem(pBaseInfo);
		m_ItemHash.Add(pItem,pItem->GetDBIdx());
		cImage tmpIconImage;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE,
					GetIconImage(pItem->GetItemIdx(), &tmpIconImage),
					ItemID);
		pItem->SetIconType(eIconType_AllItem);
		pItem->SetData(pItem->GetItemIdx());
		pItem->SetLinkPosition( 0 );

		WINDOWMGR->AddWindow(pItem);
	}
	else
	{
		//char buf[64];
		//sprintf(buf, "吝汗等 DB牢郸胶甫 荤侩! DBIndex: %d", pBaseInfo->dwDBIdx);
		//ASSERTMSG(0, buf);
	}

	return pItem;
}



void CItemManager::SetToolTipIcon(cIcon * pIcon, ITEM_OPTION_INFO * pOptionInfo, ITEM_RARE_OPTION_INFO* pRareOptionInfo, DWORD dwItemDBIdx,BOOL IsGoldShop,BOOL bIsQuey)
{
	char buf[256] = { 0, };
	WORD index = (WORD)pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);
	pIcon->IniTipLineCount();
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	cImage imgToolTip2;
	SCRIPTMGR->GetImage( 218, &imgToolTip2, PFT_HARDPATH );
	pIcon->AddImageTop(&imgToolTip2);
	pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
	ITEM_INFO * pInfo = GetItemInfo(index);
	if (pInfo == NULL) return;
	if (bits == eEQUIP_ITEM_INHERIT)
	{
		SetInheritItemToolTip(pIcon, pInfo, pRareOptionInfo);
	}
	else if ((bits & eEQUIP_ITEM))
	{
		SetEquipItemToolTip( pIcon, pInfo, pOptionInfo, pRareOptionInfo,bIsQuey);
	}
	else if(bits & eMUGONG_ITEM)
	{
		SetMugongItemToolTip( pIcon, pInfo );
	}
	else if(bits & eYOUNGYAK_ITEM)
	{
		SetYoungyakItemToolTip( pIcon, pInfo );
	}
	else if(bits & eEXTRA_ITEM || bits & eEXTRA_ITEM_UPGRADE)
	{
		SetExtraItemToolTip( pIcon, pInfo );
	}
	else if( bits & eQUEST_ITEM )
	{
		char line[128];
		wsprintf(line, "[%s]", pInfo->ItemName);
		pIcon->AddToolTipLine(line);
		pIcon->AddToolTipLine("");

		if (FALSE == IsPetSummonItem(index))
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(260), TTTC_QUESTITEM);

		if (IsPetSummonItem(index))
		{
			SetPetSummonItemToolTip(pIcon, pInfo, dwItemDBIdx);
		}
		AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);
	}
	else if( bits & eCHANGE_ITEM )
	{

		cImage * imgExtraIteminfo;

		imgExtraIteminfo = pIcon->GetBasicImage();

		if (imgExtraIteminfo)
		{
			pIcon->AddToolTipImageEquip(imgExtraIteminfo);
		}
		CExchangeItem* pExcItem = NULL;
		CItem * pcItem = NULL;
		if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
			pExcItem = (CExchangeItem*)pIcon;
		else
			pcItem = (CItem*)pIcon;
		char line[128];
		wsprintf(line, "[%s]", pInfo->ItemName);
		pIcon->AddToolTipLine(line);

		pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2240), NULL);

		if (pcItem)
		{
			if (pcItem->GetStatic() == 1)
			{
				sprintf(line, CHATMGR->GetChatMsg(1608));
				pIcon->AddToolTipLine(line, TTTC_LIMIT);
			}
			else
				pIcon->AddToolTipLine("");
		}
		else
			pIcon->AddToolTipLine("");

		pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
		if (bits == eCHANGE_ITEM_LOCK)
		{
			sprintf(line, CHATMGR->GetChatMsg(238), pInfo->ItemType);
			if (HERO->GetCharacterTotalInfo()->Level < pInfo->ItemType)
				pIcon->AddToolTipLine(line, TTTC_LIMIT);
			else
				pIcon->AddToolTipLine(line, TTTC_FREELIMIT);
		}
		else
			pIcon->AddToolTipLine("");
		AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);
	}
	else if( bits & eSHOP_ITEM )
	{
		cImage * imgGoldShopinfo;
		imgGoldShopinfo=pIcon->GetBasicImage();
		if(imgGoldShopinfo)
		{
			pIcon->AddToolTipImageEquip(imgGoldShopinfo);
		}


		//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
		CExchangeItem* pExcItem = NULL;
		CItem * pcItem = NULL;
		if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
			pExcItem = (CExchangeItem*)pIcon;
		else
			pcItem = (CItem*)pIcon;

		char line[128];
		wsprintf(line, "[%s]", pInfo->ItemName);
		DWORD dwColor = TTTC_DEFAULT;
		if (pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob)
		{
			if (pInfo->LimitGender == 0 && HERO->GetLevel() > 50)
			{
				dwColor = TTTC_LIMIT;
			}
			else if (pInfo->LimitGender == 1 && (HERO->GetLevel() < 51 || HERO->GetLevel() > 70))
			{
				dwColor = TTTC_LIMIT;
			}
			else if (pInfo->LimitGender == 2 && (HERO->GetLevel() < 71 || HERO->GetLevel() > 90))
			{
				dwColor = TTTC_LIMIT;
			}
		}
		else if (HERO->GetLevel() < pInfo->NaeRyukRecoverRate)
		{
			dwColor = TTTC_LIMIT;
		}
		pIcon->AddToolTipLine(line, dwColor);
		if (pInfo->ItemKind == eSHOP_ITEM_PET || pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP)
		{
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2241));
			if (pInfo->ItemKind == eSHOP_ITEM_PET)
			{
				if (pInfo->ItemType == 11)
				{
					if (!(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL))
					{
						sprintf(line, CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line, TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
			}
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION)
		{
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2243));
			if (pcItem)
			{
				if (pcItem->GetStatic() == 1)
				{
					sprintf(line, CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line, TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_CHARM || pInfo->ItemKind == eSHOP_ITEM_HERB ||
			pInfo->ItemKind == eSHOP_ITEM_SUNDRIES || pInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM ||
			pInfo->ItemKind == eSHOP_ITEM_IMAGENAME || pInfo->ItemKind == eSHOP_ITEM_MARRYCHARM ||
			pInfo->ItemKind == eSHOP_ITEM_GOLDITEM || pInfo->ItemKind == eSHOP_ITEM_GROWITEM)
		{
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2244));
			if (pcItem)
			{
				if (pcItem->GetStatic() == 1)
				{
					sprintf(line, CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line, TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");
			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
			pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
		}
		else if (pInfo->ItemKind == eSHOP_ITEM_INCANTATION)
		{
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2245));
			if (pcItem)
			{
				if (pcItem->GetStatic() == 1)
				{
					sprintf(line, CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line, TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");
			pIcon->AddToolTipLine("");
			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
			pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
		}
		else
		{
			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2246));
			if (pcItem)
			{
				if (pcItem->GetStatic() == 1)
				{
					sprintf(line, CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line, TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");
			pIcon->AddToolTipLine("");
			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
			pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
		}



		if (pInfo->NaeRyukRecoverRate != 0)
		{

			sprintf(line, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate);
			if (HERO->GetCharacterTotalInfo()->Level < pInfo->NaeRyukRecoverRate)
				pIcon->AddToolTipLine(line, TTTC_LIMIT);
			else
				pIcon->AddToolTipLine(line, TTTC_FREELIMIT);
		}
		else
			pIcon->AddToolTipLine("");


		if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
		{
			CExchangeItem* pExItem = (CExchangeItem*)pIcon;

			if (pExItem->GetItemParam() & ITEM_PARAM_SEAL)
			{
				if (IsPetSummonItem(index))
				{
					//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
					pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
					SetPetSummonItemToolTip(pIcon, pInfo, dwItemDBIdx);
				}
				AddItemDescriptionToolTip(pIcon, 6000);
				AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);
			}
			else
			{
				const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
				if (IsPetSummonItem(index))
				{
					SetPetSummonItemToolTip(pIcon, pInfo, dwItemDBIdx);

				}

				AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);
			}
			return;
		}

		CItem* pItem = (CItem*)pIcon;
		if (pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION)
		{
			if (!(pItem->GetItemParam() & ITEM_PARAM_SEAL))
			{
				SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(pItem->GetItemIdx());
				if (pShopItemBase)
				{
					AddUsedAvatarItemToolTip(pShopItemBase);
					return;
				}
			}
		}
		if (pInfo->ItemKind == eSHOP_ITEM_EQUIP)
		{
			if (!(pItem->GetItemParam() & ITEM_PARAM_SEAL) && pIcon->GetType() == WT_DEALITEM)
			{
				SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(pItem->GetItemIdx());
				if (pShopItemBase)
				{
					AddUsedShopEquipItemToolTip(pShopItemBase);
					return;
				}
			}
		}
		if (pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP)
		{
			if (!(pItem->GetItemParam() & ITEM_PARAM_SEAL))
			{
				SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(pItem->GetItemIdx());
				if (pShopItemBase)
				{
					AddUsedPetEquipItemToolTip(pShopItemBase);
					return;
				}
				if (IsGoldShop)
				{
					AddShopItemToolTip((cIcon*)pItem, pInfo);
					return;
				}
			}
		}
		if (pIcon->GetType() == WT_ITEM && (pItem->GetItemParam() & ITEM_PARAM_SEAL))
		{
			AddShopItemToolTip((cIcon*)pItem, pInfo);
			AddItemDescriptionToolTip(pIcon, 6000);
			PrintShopItemUseTime(pItem, pInfo);
		}
		else
		{
			AddShopItemToolTip((cIcon*)pItem, pInfo);
			AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);

			if (pInfo->ItemType == 11)
			{
				{
					SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(pItem->GetItemIdx());
					if (pShopItemBase)
					{
						if (pInfo->SellPrice == eShopItemUseParam_Realtime)
						{
							pItem->AddToolTipLine("");
							pItem->AddToolTipLine(CHATMGR->GetChatMsg(766), TTTC_DEFAULT);

							stTIME time;
							char buf[128] = { 0, };
							time.value = pShopItemBase->Remaintime;
							wsprintf(buf, CHATMGR->GetChatMsg(767), time.GetYear()/*+15*/, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
							pItem->AddToolTipLine(buf, TTTC_QUESTITEM);
						}
						else
						{
							pItem->AddToolTipLine("");
							pItem->AddToolTipLine(CHATMGR->GetChatMsg(1442), TTTC_DEFAULT);
							pItem->AddToolTipLine(CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM);

						}

					}

				}
			}
			else{
				PrintShopItemUseTime(pItem, pInfo);
			}

		}
	}
	else if( bits & eTITAN_EQUIPITEM || bits == eTITAN_ITEM_RECIPE || bits == eTITAN_ITEM_PAPER || bits == eTITAN_ITEM_PARTS )
	{
		char line[128];
		wsprintf( line, "[%s]", pInfo->ItemName );
		pIcon->AddToolTipLine( line );
		pIcon->AddToolTipLine( "" );

		// magi82 - Titan(070420)
		if(bits & eTITAN_EQUIPITEM)
		{
			DWORD dwDBIdx = 0;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM ||pIcon->GetType() == WT_BUYITE)
			{
				CExchangeItem* pItem = (CExchangeItem*)pIcon;
				dwDBIdx = pItem->GetDBId();
			}
			else
			{
				CItem* pItem = (CItem*)pIcon;
				dwDBIdx = pItem->GetDBIdx();
			}

			SetTitanEquipItemToolTip(pIcon, pInfo);

			TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
			if(dwItemDBIdx > 0)
			{
				pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(dwItemDBIdx);
			}
			else
			{
				if(dwDBIdx)
				{
					pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(dwDBIdx);
				}
			}

			if(pEnduranceInfo)
			{
				float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
				if(fEnduranceRate > 0.0f && fEnduranceRate < 1.0f)
				{
					fEnduranceRate = 1.0f;
				}

				ZeroMemory(&line, sizeof(line));
				wsprintf( line, CHATMGR->GetChatMsg(1516), (DWORD)fEnduranceRate);
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
			}
		}
		if(bits == eTITAN_ITEM_PAPER )
		{
			DWORD dwDBIdx = 0;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM ||pIcon->GetType() == WT_BUYITE)
			{
				CExchangeItem* pItem = (CExchangeItem*)pIcon;
				dwDBIdx = pItem->GetDBId();
			}
			else
			{
				CItem* pItem = (CItem*)pIcon;
				dwDBIdx = pItem->GetDBIdx();
			}

			ZeroMemory(&line, sizeof(line));
			wsprintf( line, CHATMGR->GetChatMsg(1519), pInfo->Plus_Value );
			pIcon->AddToolTipLine( line );

			if( dwDBIdx != 0 && dwDBIdx == TITANMGR->GetRegistedTitanItemDBIdx() )
			{
				ZeroMemory(&line, sizeof(line));
				wsprintf( line, CHATMGR->GetChatMsg(1520) );
				pIcon->AddToolTipLine( line );
			}
		}

		AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
	}
	else
	{
		ASSERT(0);
	}
}

void CItemManager::SetExtraItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{

	//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	char line[128];
	wsprintf( line, "[%s]", pInfo->ItemName );
	pIcon->AddToolTipLine( line );
	pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2242),NULL);

	if(pcItem)
	{
		if(pcItem->GetStatic()==1)
		{
			sprintf(line,CHATMGR->GetChatMsg(1608));
			pIcon->AddToolTipLine(line,TTTC_LIMIT);
		}
		else
			pIcon->AddToolTipLine("");
	}
	else
		pIcon->AddToolTipLine("");

	pIcon->AddToolTipLine( "" );
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	cImage * imgExtraIteminfo;

	imgExtraIteminfo=pIcon->GetBasicImage();

	if(imgExtraIteminfo)
	{
		pIcon->AddToolTipImageEquip(imgExtraIteminfo);
	}
	switch( pInfo->ItemType )
	{
	case eEXTRA_ITEM_JEWEL:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(251), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_MATERIAL:			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(252), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_METAL:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(253), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_BOOK:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(254), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_HERB:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(255), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_ETC:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(256), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_UPGRADE_ATTACK:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(257), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_UPGRADE_DEFENSE:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(258), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_COMP:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(259), TTTC_EXTRAITEM );	break;
	case eEXTRA_ITEM_QUEST:				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(260), TTTC_EXTRAITEM );	break;
	}

	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}

void CItemManager::SetYoungyakItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{

	//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	char line[128];
	//[物品名][By:十里坡剑神][QQ:112582793][2017/11/28]
	wsprintf( line, "[%s]", pInfo->ItemName );
	pIcon->AddToolTipLine( line );
	//[杂货道具][By:十里坡剑神][QQ:112582793][2017/11/28]
	wsprintf( line, "%s", CHATMGR->GetChatMsg(2239) );
	pIcon->AddToolTipLine( line );

	if(pcItem)
	{//[是否可交易][By:十里坡剑神][QQ:112582793][2018/9/22]
		if(pcItem->GetStatic()==1)
		{
			sprintf(line,CHATMGR->GetChatMsg(1608));
			pIcon->AddToolTipLine(line,TTTC_LIMIT);
		}
		else
			pIcon->AddToolTipLine("");
	}
	else
		pIcon->AddToolTipLine("");

	//[物品图标][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage * imgYoungyakfo;
	imgYoungyakfo=pIcon->GetBasicImage();
	if(imgYoungyakfo)
	{
		pIcon->AddToolTipImageEquip(imgYoungyakfo);
	}
	pIcon->AddToolTipLine( "" );
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	//[属性][By:十里坡剑神][QQ:112582793][2017/11/28]
	if( pInfo->GenGol != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(261), pInfo->GenGol);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->MinChub != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(262), pInfo->MinChub);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->CheRyuk != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(263), pInfo->CheRyuk);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->SimMek != 0)
	{
		wsprintf(line, "%s +%d", CHATMGR->GetChatMsg(264), pInfo->SimMek);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(266), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_WATER)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(267), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_TREE)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(268), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_IRON)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)) !=0 )
	{
		wsprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(269), (int)(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH)));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->LifeRecover !=0 )
	{
		if(pInfo->ItemKind == eYOUNGYAK_ITEM_PET)
		{
			wsprintf(line, "%s %d", CHATMGR->GetChatMsg(1276), pInfo->LifeRecover);
		}
		else
		{
			wsprintf(line, "%s %d", CHATMGR->GetChatMsg(270), pInfo->LifeRecover);
		}
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->LifeRecoverRate*100) !=0 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(270), (int)(pInfo->LifeRecoverRate*100));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( pInfo->NaeRyukRecover !=0 )
	{
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(271), pInfo->NaeRyukRecover);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if( (int)(pInfo->NaeRyukRecoverRate*100) !=0 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(271), (int)(pInfo->NaeRyukRecoverRate*100));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}

	if( pInfo->ItemIdx == 53102 )
	{	//[恢复类][By:十里坡剑神][QQ:112582793][2017/11/28]
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(270), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(271), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d", CHATMGR->GetChatMsg(181), 1000);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53031 || pInfo->ItemIdx == 53094 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53216 || pInfo->ItemIdx == 53222 )
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else if( pInfo->ItemIdx == 53032 || pInfo->ItemIdx == 53095 
		|| pInfo->ItemIdx == 53103 || pInfo->ItemIdx == 53217 || pInfo->ItemIdx == 53223)
	{
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(270), 100);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(271), 100);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		wsprintf(line, "%s %d%%", CHATMGR->GetChatMsg(181), 100);	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}

void CItemManager::SetMugongItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo )
{
	//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;

	char line[128];
	wsprintf( line, "[%s]", pInfo->ItemName );
	pIcon->AddToolTipLine( line );


	//[小图标][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage * imgMugonginfo;
	imgMugonginfo=pIcon->GetBasicImage();
	if(imgMugonginfo)
	{
		pIcon->AddToolTipImageEquip(imgMugonginfo);
	}
	
	switch( pInfo->ItemKind )
	{
	case 1025: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(274), TTTC_MUGONGKIND ); break;
	case 1026: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(275), TTTC_MUGONGKIND ); break;
	case 1027: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(276), TTTC_MUGONGKIND ); break;
	case 1028: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(277), TTTC_MUGONGKIND ); break;
	case 1029: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(278), TTTC_MUGONGKIND ); break;
	case 1030: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(283), TTTC_MUGONGKIND ); break;
	case 1031: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(284), TTTC_MUGONGKIND ); break;
	case 1032: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(285), TTTC_MUGONGKIND ); break;
	case 1033: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(286), TTTC_MUGONGKIND ); break;
	case 1034: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(287), TTTC_MUGONGKIND ); break;
	case 1035: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(288), TTTC_MUGONGKIND ); break;
	case 1036: pIcon->AddToolTipLine( CHATMGR->GetChatMsg(289), TTTC_MUGONGKIND ); break;
	case eMUGONG_ITEM_SIMBUB:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(243), TTTC_MUGONGKIND ); break;
	case eMUGONG_ITEM_JINBUB:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(244), TTTC_MUGONGKIND ); break;
	case eMUGONG_ITEM_KYUNGGONG:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(245), TTTC_MUGONGKIND ); break;
	case 1040:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1362), TTTC_MUGONGKIND ); break;
    case eMUGONG_ITEM_JOB:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1669), TTTC_MUGONGKIND ); break;		//2007. 10. 28. CBH - 傈巩扁贱 辑利
	}

	if(pcItem)
	{
		if(pcItem->GetStatic()==1)
		{
			sprintf(line,CHATMGR->GetChatMsg(1608));
			pIcon->AddToolTipLine(line,TTTC_LIMIT);
		}
		else
			pIcon->AddToolTipLine("");
	}
	else
		pIcon->AddToolTipLine("");

	if( pInfo->LimitLevel != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
//#ifdef _JAPAN_LOCAL_
//	if( pInfo->wItemAttr > 0 && pInfo->wItemAttr < 6 )
//	{
//		wsprintf(line, "%s %s", CHATMGR->GetChatMsg(1194), CHATMGR->GetChatMsg(1186+pInfo->wItemAttr) );
//		if( HERO->GetMainCharAttr() == pInfo->wItemAttr || HERO->GetSubCharAttr() == pInfo->wItemAttr )
//			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
//		else
//			pIcon->AddToolTipLine( line, TTTC_LIMIT );
//	}
//	else if( pInfo->wItemAttr > 10 && pInfo->wItemAttr < 16 )
//	{
//		wsprintf(line, "%s %s", CHATMGR->GetChatMsg(1195), CHATMGR->GetChatMsg(1186+pInfo->wItemAttr-10) );
//		if( HERO->GetMainCharAttr() == pInfo->wItemAttr-10 )
//			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
//		else
//			pIcon->AddToolTipLine( line, TTTC_LIMIT );
//	}
//
//	if( pInfo->wAcquireSkillIdx1 )
//	{
//		CSkillInfo* psinfo = SKILLMGR->GetSkillInfo( pInfo->wAcquireSkillIdx1 );
//		if( psinfo )
//		{
//			char temp[64] = {0,};
//			sprintf( temp, psinfo->GetSkillName() );
//			if( pInfo->wAcquireSkillIdx1 != pInfo->wAcquireSkillIdx2 )
//			{
//				int len = strlen(temp);
//				if( len > 2 ) temp[len-2] = 0;
//			}
//			wsprintf(line, "%s %s", temp, CHATMGR->GetChatMsg(1196));
//			if( CheckItemAquireSkill( pInfo->wAcquireSkillIdx1, pInfo->wAcquireSkillIdx2 ) )
//				pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
//			else
//				pIcon->AddToolTipLine( line, TTTC_LIMIT );
//		}
//	}
//#endif
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	SetItemToolTipForStage( pIcon, pInfo );

	switch( pInfo->LimitGender )
	{
	case 1:
		pIcon->AddToolTipLine(CHATMGR->GetChatMsg(236), TTTC_LIMIT );
		break;
	case 2:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(237), TTTC_LIMIT );
		break;
	}


	if( pInfo->LimitGenGol != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(239), pInfo->LimitGenGol);
		if( HERO->GetHeroTotalInfo()->wGenGol < pInfo->LimitGenGol )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitMinChub != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(240), pInfo->LimitMinChub);
		if( HERO->GetHeroTotalInfo()->wMinChub < pInfo->LimitMinChub )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitCheRyuk != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(241), pInfo->LimitCheRyuk);
		if( HERO->GetHeroTotalInfo()->wCheRyuk < pInfo->LimitCheRyuk )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitSimMek != 0 )
	{
		wsprintf(line, CHATMGR->GetChatMsg(242), pInfo->LimitSimMek);
		if( HERO->GetHeroTotalInfo()->wSimMek < pInfo->LimitSimMek )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2瞒 傈流 - 捞康霖
	// 公傍函券
	if(pInfo->ItemKind == 1040)
	{
		//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
		pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
		SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pInfo->ItemIdx);

		if(pOption)
		{
			if(pOption->Range > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Range < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->ReduceNaeRyuk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->ReduceNaeRyuk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->PhyAtk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyAtk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			
			if(pOption->BaseAtk > 0)
			{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
			pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			
			if(pOption->AttAtk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttAtk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->Life > 0)	
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Life < 0)	
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->NaeRyuk > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->NaeRyuk < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->Shield > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Shield < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->PhyDef > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->PhyDef < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->AttDef > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->AttDef < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(-pOption->AttDef * 100 + 0.5));
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}

			if(pOption->Duration > 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
			if(pOption->Duration < 0)
			{
				char ToolTipText[32];
				wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), pOption->Duration);
				pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );
			}
		}
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
}
BOOL CItemManager::IsRealItem(const char* ItemName)
{
	std::string t_strName = ItemName;
	int t_iFlag = t_strName.find("真|");
	int t_iFlag1 = t_strName.find("魂|");
	int t_iFlag2 = t_strName.find("强|");
	int t_iFlag3 = t_strName.find("魔|");
	if(t_iFlag!=-1||t_iFlag1!=-1||t_iFlag2!=-1||t_iFlag3!=-1)
	{
		return TRUE;
	}
	else
		return FALSE;
}
#ifdef _GMTOOL_
void CItemManager::SetDwordColor(DWORD color)
{
	m_Color = color;
	RefreshAllItem();
}
#endif
void CItemManager::SetInheritItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
{
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219, &imgTopLine, PFT_HARDPATH);
	cImage * imgEquipInfo;
	//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	TOOLTIP_TEXT* pTooltipText;

	if (pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET || pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;
	//[图标获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	imgEquipInfo = pIcon->GetBasicImage();
	pIcon->SetEquipItem();
	if (imgEquipInfo)
	{
		pIcon->AddToolTipImageEquip(imgEquipInfo);
	}
	imgEquipInfo = NULL;

	char line[128];
	
	sprintf(line, "[%s]", pInfo->ItemName);

	int nLen = strlen(pInfo->ItemName);

	if (pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	else
		pIcon->AddToolTipLine(line);

	if (pInfo->wPulsItemEffect == eEQUIP_ITEM_WEAPON)
		sprintf(line, CHATMGR->GetChatMsg(2395));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_DRESS)
		sprintf(line, CHATMGR->GetChatMsg(2396));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_HAT)
		sprintf(line, CHATMGR->GetChatMsg(2397));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_SHOES)
		sprintf(line, CHATMGR->GetChatMsg(2398));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_RING)
		sprintf(line, CHATMGR->GetChatMsg(2399));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_CAPE)
		sprintf(line, CHATMGR->GetChatMsg(2400));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_NECKLACE)
		sprintf(line, CHATMGR->GetChatMsg(2401));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_ARMLET)
		sprintf(line, CHATMGR->GetChatMsg(2402));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_BELT)
		sprintf(line, CHATMGR->GetChatMsg(2403));
	else if (pInfo->wPulsItemEffect == eEQUIP_ITEM_UNIQUE)
		sprintf(line, CHATMGR->GetChatMsg(2404));

	pIcon->AddToolTipLine(line, TTTC_LIMIT);

	pIcon->AddToolTipLine("");
	
	if (pcItem)
	{
		if (pcItem->GetStatic() == 1)
		{
			sprintf(line, CHATMGR->GetChatMsg(1608));
			pIcon->AddToolTipLine(line, TTTC_LIMIT);
		}
		else
			pIcon->AddToolTipLine("");
	}
	else
		pIcon->AddToolTipLine("");

	float attrRareOptValue = 0;

	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());

	if (pInfo->wPulsItemEffect == eEQUIP_ITEM_WEAPON)
	{
		if (pIcon->GetType() != WT_BUYITE)
		{
			char GrowMsg[128];
			DWORD GrowVal;
			if (pcItem != NULL)
				GrowVal = pcItem->GetGrow();
			else if (pExcItem != NULL)
				GrowVal = pExcItem->GetGrow();
			
			if (GrowVal != 0)
			{
				sprintf(GrowMsg, CHATMGR->GetChatMsg(2128), GrowVal);
				pIcon->AddToolTipLine(GrowMsg, TTTC_ITEMGROW6);
				//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
				pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
				pIcon->AddToolTipLine("");   // 添一空行
			}
		}
	}

	DWORD RareState;
	if (pRareOptionInfo && pRareOptionInfo->GenGol)
		RareState = pRareOptionInfo->GenGol;
	else
		RareState = 0;

	if (pRareOptionInfo && pRareOptionInfo->GenGol != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(382), pRareOptionInfo->GenGol);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->MinChub)
		RareState = pRareOptionInfo->MinChub;
	else
		RareState = 0;

	if (pRareOptionInfo && pRareOptionInfo->MinChub != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(383), pRareOptionInfo->MinChub);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->CheRyuk)
		RareState = pRareOptionInfo->CheRyuk;
	else
		RareState = 0;
	if (pRareOptionInfo && pRareOptionInfo->CheRyuk != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(384), pRareOptionInfo->CheRyuk);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->SimMek)
		RareState = pRareOptionInfo->SimMek;
	else
		RareState = 0;
	
	if (pRareOptionInfo && pRareOptionInfo->SimMek != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(385), pRareOptionInfo->SimMek);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->Life)
		RareState = pRareOptionInfo->Life;
	else
		RareState = 0;
	
	if (pRareOptionInfo && pRareOptionInfo->Life != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(386), pRareOptionInfo->Life);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->NaeRyuk)
		RareState = pRareOptionInfo->NaeRyuk;
	else
		RareState = 0;
	if (pRareOptionInfo && pRareOptionInfo->NaeRyuk != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(387), pRareOptionInfo->NaeRyuk);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	if (pRareOptionInfo && pRareOptionInfo->Shield)
		RareState = pRareOptionInfo->Shield;
	else
		RareState = 0;
	if (pRareOptionInfo && pRareOptionInfo->Shield != 0)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(388), pRareOptionInfo->Shield);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}

	for (int i = ATTR_FIRE; i <= ATTR_MAX; ++i)
	{
		if (pRareOptionInfo)
			attrRareOptValue = 100 * pRareOptionInfo->AttrRegist.GetElement_Val(i);
		else attrRareOptValue = 0;

		if (attrRareOptValue)
			sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265 + i - 1), (int)(attrRareOptValue));
		if (attrRareOptValue)
			pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	//#endif
	WORD RareOptPhyAttack;
	if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
		RareOptPhyAttack = pRareOptionInfo->PhyAttack;
	else
		RareOptPhyAttack = 0;


	if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(681), pRareOptionInfo->PhyAttack);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	for (int j = ATTR_FIRE; j <= ATTR_MAX; ++j)
	{
		if (pRareOptionInfo)
			attrRareOptValue = 100 * pRareOptionInfo->AttrAttack.GetElement_Val(j);
		else attrRareOptValue = 0;

		if (attrRareOptValue)
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(392 + j - 1), (int)(attrRareOptValue));
			
		if (attrRareOptValue)
			pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}


	WORD RareOptPhyDef;
	if (pRareOptionInfo && pRareOptionInfo->PhyDefense)
		RareOptPhyDef = pRareOptionInfo->PhyDefense;
	else
		RareOptPhyDef = 0;
	
	if (RareOptPhyDef)
	{
		sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(397), RareOptPhyDef);
		pIcon->AddToolTipLine(line, TTTC_RAREITEM);
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine, pIcon->GetTipLineCount());
	AddItemDescriptionToolTip(pIcon, pInfo->ItemTooltipIdx);
}
void CItemManager::SetEquipItemToolTip( cIcon* pIcon, ITEM_INFO* pInfo, ITEM_OPTION_INFO * pOptionInfo, ITEM_RARE_OPTION_INFO* pRareOptionInfo,BOOL bIsQuey)
{
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);
	cImage * imgEquipInfo;
	//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	CExchangeItem* pExcItem = NULL;
	CItem * pcItem = NULL;
	TOOLTIP_TEXT* pTooltipText;

	if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
		pExcItem = (CExchangeItem*)pIcon;
	else
		pcItem = (CItem*)pIcon;
	//[图标获取][By:十里坡剑神][QQ:112582793][2018/2/1]
	imgEquipInfo=pIcon->GetBasicImage();
	pIcon->SetEquipItem();
	if(imgEquipInfo)
	{
		pIcon->AddToolTipImageEquip(imgEquipInfo);
	}
	imgEquipInfo=NULL;

	char line[128];
	if(pOptionInfo)
		sprintf( line, "[%s*]", pInfo->ItemName );
	else
		sprintf( line, "[%s]", pInfo->ItemName );
	
	int nLen = strlen( pInfo->ItemName );

	if(IsRealItem(pInfo->ItemName ))
#ifdef _GMTOOL_
		pIcon->AddToolTipLine( line, m_Color );
#else
		pIcon->AddToolTipLine( line, RGB(230,180,255) );
#endif
	else if(pRareOptionInfo && pRareOptionInfo->dwItemDBIdx)
		pIcon->AddToolTipLine( line, TTTC_RAREITEM );
	else if(pInfo->wSetItemKind != 0)
		pIcon->AddToolTipLine( line, TTTC_SETITEM_NAME);
	else if( *(pInfo->ItemName + nLen - 2) == '+' || *(pInfo->ItemName + nLen - 3) == '+' )	
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTRITEM );
	else if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)	
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else
		pIcon->AddToolTipLine( line );
	if (pInfo->ItemKind == eEQUIP_ITEM_INHERIT)
		sprintf(line, CHATMGR->GetChatMsg(2245));
	else if(pInfo->WeaponType == 1)
		sprintf(line,CHATMGR->GetChatMsg(2247));
	else if (pInfo->WeaponType == 2)
		sprintf(line,CHATMGR->GetChatMsg(2248));
	else if (pInfo->WeaponType == 3)
		sprintf(line,CHATMGR->GetChatMsg(2249));
	else if (pInfo->WeaponType == 4)
		sprintf(line,CHATMGR->GetChatMsg(2250));
	else if (pInfo->WeaponType == 5)
		sprintf(line,CHATMGR->GetChatMsg(2251));
	else if (pInfo->WeaponType == 6)
		sprintf(line,CHATMGR->GetChatMsg(2252));
	else if (pInfo->WeaponType == 0)
		sprintf(line,CHATMGR->GetChatMsg(2253));
	pIcon->AddToolTipLine(line,TTTC_LIMIT );

	if (pInfo->ItemKind == eEQUIP_ITEM_INHERIT)
			pIcon->AddToolTipLine("");
	if(pcItem)
	{
		if(pcItem->GetStatic()==1)
		{
			sprintf(line,CHATMGR->GetChatMsg(1608));
			pIcon->AddToolTipLine(line,TTTC_LIMIT);
		}
		else
			pIcon->AddToolTipLine("");
	}
	else
		pIcon->AddToolTipLine("");
	
	float attrvalue = 0;
	float attroptvalue = 0;

	float attrRareOptValue = 0;

	SetItemToolTipForStage( pIcon, pInfo );

	switch( pInfo->LimitGender )
	{
	case 1:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(236), TTTC_LIMIT );
		break;
	case 2:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(237), TTTC_LIMIT );
		break;
	}

	if( pInfo->LimitLevel != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitGenGol != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(239), pInfo->LimitGenGol);
		if( HERO->GetHeroTotalInfo()->wGenGol < pInfo->LimitGenGol )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitMinChub != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(240), pInfo->LimitMinChub);
		if( HERO->GetHeroTotalInfo()->wMinChub < pInfo->LimitMinChub )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitCheRyuk != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(241), pInfo->LimitCheRyuk);
		if( HERO->GetHeroTotalInfo()->wCheRyuk < pInfo->LimitCheRyuk )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	if( pInfo->LimitSimMek != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(242), pInfo->LimitSimMek);
		if( HERO->GetHeroTotalInfo()->wSimMek < pInfo->LimitSimMek )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());

	eITEM_KINDBIT bits = GetItemKind(pInfo->ItemIdx);

	if (bits == eEQUIP_ITEM_WEAPON || bits == eEQUIP_ITEM_INHERIT)
	{
		if(pIcon->GetType() !=WT_BUYITE)
		{
			char GrowMsg[128];		
			DWORD GrowVal;
			if(pcItem != NULL)
				GrowVal = pcItem->GetGrow();
			else if(pExcItem !=NULL)
				GrowVal = pExcItem->GetGrow();
			sprintf(GrowMsg,CHATMGR->GetChatMsg(2128),GrowVal);

			pIcon->AddToolTipLine(GrowMsg,TTTC_ITEMGROW6);
			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
			pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			pIcon->AddToolTipLine("");   // 添一空行
		}
	}

	
	switch( pInfo->RangeType )
	{
	case 0:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(272) );
		break;
	case 1:
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(273) );
		break;
	}
	DWORD RareState;
	if( pRareOptionInfo && pRareOptionInfo->GenGol )
		RareState = pRareOptionInfo->GenGol;
	else
		RareState = 0;

	if( pInfo->GenGol != 0)
	{
		if(pOptionInfo && pOptionInfo->GenGol != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(382), pInfo->GenGol, RareState, pOptionInfo->GenGol);
		else if (pRareOptionInfo && pRareOptionInfo->GenGol)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(382), pInfo->GenGol , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(382), pInfo->GenGol);
		
		if(pRareOptionInfo && pRareOptionInfo->GenGol)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->GenGol != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(382), RareState, pOptionInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->GenGol != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(382), pOptionInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->GenGol != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(382), pRareOptionInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->MinChub )
		RareState = pRareOptionInfo->MinChub;
	else
		RareState = 0;
	
	if( pInfo->MinChub != 0)
	{
		if(pOptionInfo && pOptionInfo->MinChub != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(383), pInfo->MinChub , RareState, pOptionInfo->MinChub);
		else if (pRareOptionInfo && pRareOptionInfo->MinChub)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(383), pInfo->MinChub , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(383), pInfo->MinChub);

		if(pRareOptionInfo && pRareOptionInfo->MinChub)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->MinChub != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(383), RareState, pOptionInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->MinChub != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(383), pOptionInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->MinChub != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(383), pRareOptionInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->CheRyuk )
		RareState = pRareOptionInfo->CheRyuk;
	else
		RareState = 0;
	if( pInfo->CheRyuk != 0)
	{
		if(pOptionInfo && pOptionInfo->CheRyuk != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(384), pInfo->CheRyuk , RareState, pOptionInfo->CheRyuk);
		else if (pRareOptionInfo && pRareOptionInfo->CheRyuk)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(384), pInfo->CheRyuk , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(384), pInfo->CheRyuk );

		if(pRareOptionInfo && pRareOptionInfo->CheRyuk)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->CheRyuk != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(384), RareState, pOptionInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->CheRyuk != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(384), pOptionInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->CheRyuk != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(384), pRareOptionInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->SimMek )
		RareState = pRareOptionInfo->SimMek;
	else
		RareState = 0;
	if( pInfo->SimMek != 0)
	{
		if(pOptionInfo && pOptionInfo->SimMek != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(385), pInfo->SimMek , RareState, pOptionInfo->SimMek);
		else if (pRareOptionInfo && pRareOptionInfo->SimMek)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(385), pInfo->SimMek , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(385), pInfo->SimMek );

		if(pRareOptionInfo && pRareOptionInfo->SimMek)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->SimMek != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(385), RareState, pOptionInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->SimMek != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(385), pOptionInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}

		else if(pRareOptionInfo && pRareOptionInfo->SimMek != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(385), pRareOptionInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->Life )
		RareState = pRareOptionInfo->Life;
	else
		RareState = 0;
	if( pInfo->Life != 0)
	{
		if(pOptionInfo && pOptionInfo->Life != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(386), pInfo->Life , RareState, pOptionInfo->Life);
		else if(pRareOptionInfo && pRareOptionInfo->Life)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(386), pInfo->Life , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(386), pInfo->Life );

		if(pRareOptionInfo && pRareOptionInfo->Life)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->Life != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(386), RareState, pOptionInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->Life != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(386), pOptionInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->Life != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(386), pRareOptionInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->NaeRyuk )
		RareState = pRareOptionInfo->NaeRyuk;
	else
		RareState = 0;
	if( pInfo->NaeRyuk != 0)
	{
		if(pOptionInfo && pOptionInfo->NaeRyuk != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(387), pInfo->NaeRyuk , RareState, pOptionInfo->NaeRyuk);
		else if (pRareOptionInfo && pRareOptionInfo->NaeRyuk)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(387), pInfo->NaeRyuk , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(387), pInfo->NaeRyuk );

		if(pRareOptionInfo && pRareOptionInfo->NaeRyuk)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->NaeRyuk != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(387), RareState, pOptionInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->NaeRyuk != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(387), pOptionInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->NaeRyuk != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(387), pRareOptionInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}


	if( pRareOptionInfo && pRareOptionInfo->Shield )
		RareState = pRareOptionInfo->Shield;
	else
		RareState = 0;
	if( pInfo->Shield != 0)
	{
		if(pOptionInfo && pOptionInfo->Shield != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(388), pInfo->Shield , RareState, pOptionInfo->Shield);
		else if (pRareOptionInfo && pRareOptionInfo->Shield)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(388), pInfo->Shield , RareState);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(388), pInfo->Shield);

		if(pRareOptionInfo && pRareOptionInfo->Shield)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if(RareState && pOptionInfo && pOptionInfo->Shield != 0)
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(388), RareState, pOptionInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->Shield != 0)
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(388), pOptionInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(pRareOptionInfo && pRareOptionInfo->Shield != 0)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(388), pRareOptionInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}

	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
	{
		attrvalue = 100 * pInfo->AttrRegist.GetElement_Val(i);
		if(pOptionInfo)
			attroptvalue = 100 * pOptionInfo->AttrRegist.GetElement_Val(i);
		if(pRareOptionInfo)
		{
			attrRareOptValue = 100 * pRareOptionInfo->AttrRegist.GetElement_Val(i);
		}
		else attrRareOptValue = 0;
		
		if( (int)(attrvalue) != 0 )	
		{
			if( (int)(attroptvalue) != 0 )
				sprintf( line, "%s +%d%% [+%d%%](+%d%%)", CHATMGR->GetChatMsg(265+i-1), (int)(attrvalue) , int(attrRareOptValue), (int)(attroptvalue));
			else if (pRareOptionInfo && attrRareOptValue)
				sprintf( line, "%s +%d%% [+%d%%]", CHATMGR->GetChatMsg(265+i-1), (int)(attrvalue) , int(attrRareOptValue) );
			else
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(265 + i - 1), (int)(attrvalue ));

			if (pRareOptionInfo && attrRareOptValue)
				pIcon->AddToolTipLine(line, TTTC_RAREITEM);
			else
				pIcon->AddToolTipLine(line, TTTC_EXTRAATTR);
		}
		else
		{
			if( (int)(attroptvalue) != 0 )
			{
				if( attrRareOptValue )
					sprintf( line, "%s +%d%% (+%d%%)", CHATMGR->GetChatMsg(265+i-1), (int)(attrRareOptValue), (int)(attroptvalue) );
				else
					sprintf( line, "%s (+%d%%)", CHATMGR->GetChatMsg(265+i-1), (int)(attroptvalue) );
			}
			else
			{
				if( attrRareOptValue )
					sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(265+i-1), (int)(attrRareOptValue) );
			}
			if(attrRareOptValue)
				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
			else if(attroptvalue)
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}


//#endif
	WORD RareOptPhyAttack;
	if( pRareOptionInfo && pRareOptionInfo->PhyAttack )
		RareOptPhyAttack = pRareOptionInfo->PhyAttack;
	else
		RareOptPhyAttack = 0;
	
	if( pInfo->MeleeAttackMin || pInfo->MeleeAttackMax )
	{
		if( pInfo->ItemKind != eEQUIP_ITEM_ARMLET )
		{
			if( pInfo->MeleeAttackMin == pInfo->MeleeAttackMax )
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )	
					sprintf( line, "%s %d [+%d](+%d)", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin , RareOptPhyAttack, pOptionInfo->PhyAttack );
				else if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
					sprintf( line, "%s %d [+%d]", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin , RareOptPhyAttack );
				else
					sprintf(line, "%s %d", CHATMGR->GetChatMsg(389), (pInfo->MeleeAttackMin + RareOptPhyAttack));
			}
			else
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d ~ %d [+%d](+%d)", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin , pInfo->MeleeAttackMax , RareOptPhyAttack, pOptionInfo->PhyAttack );
				else if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
					sprintf( line, "%s %d ~ %d [+%d]", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin, pInfo->MeleeAttackMax , RareOptPhyAttack );
				else
					sprintf(line, "%s %d ~ %d", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin , pInfo->MeleeAttackMax );
			}

			if( pRareOptionInfo && pRareOptionInfo->PhyAttack )
				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
			else
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else
		{
			if( pInfo->MeleeAttackMin == pInfo->MeleeAttackMax )
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d%% (+%d)", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin, pOptionInfo->PhyAttack );
				else
					sprintf( line, "%s %d%%", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin );
			}
			else
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d%% ~ %d%% (+%d)", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin, pInfo->MeleeAttackMax, pOptionInfo->PhyAttack );
				else
					sprintf( line, "%s %d%% ~ %d%%", CHATMGR->GetChatMsg(389), pInfo->MeleeAttackMin, pInfo->MeleeAttackMax );
			}
			pIcon->AddToolTipLine( line, TTTC_NUMBER );
		}
	}


	if(pOptionInfo && pOptionInfo->CriticalPercent != 0)
	{
		sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(390), pOptionInfo->CriticalPercent );
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	

	
	if( pInfo->RangeAttackMin || pInfo->RangeAttackMax )
	{
		if( pInfo->ItemKind != eEQUIP_ITEM_ARMLET)	
		{
			if( pInfo->RangeAttackMin == pInfo->RangeAttackMax )
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d [+%d](+%d)", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin , RareOptPhyAttack, pOptionInfo->PhyAttack );
				else if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
					sprintf( line, "%s %d [+%d]", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin , RareOptPhyAttack );
				else
					sprintf(line, "%s %d", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin );
			}
			else
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d ~ %d [+%d](+%d)", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin , pInfo->RangeAttackMax , RareOptPhyAttack, pOptionInfo->PhyAttack );
				else if (pRareOptionInfo && pRareOptionInfo->PhyAttack)
					sprintf( line, "%s %d ~ %d [+%d]", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin , pInfo->RangeAttackMax , RareOptPhyAttack );
				else
					sprintf(line, "%s %d ~ %d", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin, pInfo->RangeAttackMax );
			}

			if( pRareOptionInfo && pRareOptionInfo->PhyAttack )
				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
			else
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else
		{
			if( pInfo->RangeAttackMin == pInfo->RangeAttackMax )
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d%% (+%d)", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMax, pOptionInfo->PhyAttack );
				else
					sprintf( line, "%s %d%%", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMax );
			}
			else
			{
				if( pOptionInfo && pOptionInfo->PhyAttack )
					sprintf( line, "%s %d%% ~ %d%% (+%d)", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin, pInfo->RangeAttackMax, pOptionInfo->PhyAttack );
				else
					sprintf( line, "%s %d%% ~ %d%%", CHATMGR->GetChatMsg(391), pInfo->RangeAttackMin, pInfo->RangeAttackMax );
			}
			pIcon->AddToolTipLine( line, TTTC_NUMBER );
		}
	}

	if( pInfo->MeleeAttackMin == 0  && pInfo->MeleeAttackMax == 0 && pInfo->RangeAttackMin  == 0  && pInfo->RangeAttackMax == 0)
	{
		if( pRareOptionInfo && pRareOptionInfo->PhyAttack && pOptionInfo && pOptionInfo->PhyAttack )
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(681), pRareOptionInfo->PhyAttack, pOptionInfo->PhyAttack );
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if( pOptionInfo && pOptionInfo->PhyAttack != 0 )
		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(681), pOptionInfo->PhyAttack);
			pIcon->AddToolTipLine( line, TTTC_NUMBER );
		}
		else if( pRareOptionInfo && pRareOptionInfo->PhyAttack )
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(681), pRareOptionInfo->PhyAttack );
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}

//#ifdef _JAPAN_LOCAL_
//	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
//	{
//		attrvalue = pInfo->AttrAttack.GetElement_Val(Arr_ReviseAttr[i-1]);
//		if(pOptionInfo)
//			attroptvalue = pOptionInfo->AttrAttack.GetElement_Val(Arr_ReviseAttr[i-1]);
//
//		if(pRareOptionInfo)
//		{
//			attrRareOptValue = pRareOptionInfo->AttrAttack.GetElement_Val(Arr_ReviseAttr[i-1]);
//		}
//		else attrRareOptValue = 0;
//
//		if( (int)(attrvalue) != 0 )
//		{
//			if( (int)(attroptvalue) != 0 )
//			{
//				sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(392+i-1), (int)(attrvalue + attrRareOptValue), (int)(attroptvalue));
//			}
//			else
//			{
//				sprintf(line, "%s +%d", CHATMGR->GetChatMsg(392+i-1), (int)(attrvalue+ attrRareOptValue) );
//			}
//			if(pRareOptionInfo && attrRareOptValue)
//				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
//			else
//				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
//		}
//		else
//		{
//			if( (int)(attroptvalue) != 0 )
//			{
//				if( attrRareOptValue )
//				{
//					sprintf( line, "%s +%d (+%d)", CHATMGR->GetChatMsg(392+i-1), (int)(attrRareOptValue), (int)(attroptvalue) );
//				}
//				else
//				{
//					sprintf( line, "%s (+%d)", CHATMGR->GetChatMsg(392+i-1), (int)(attroptvalue) );
//				}
//			}
//			else
//			{
//				if(attroptvalue)	continue;
//
//				if( attrRareOptValue )
//					sprintf( line, "%s +%d", CHATMGR->GetChatMsg(392+i-1), (int)(attrRareOptValue) );
//			}
//			if(attrRareOptValue)
//				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
//			else if(attroptvalue)
//				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
//
//		}
//	}
//
//#else


	for(int j = ATTR_FIRE; j <= ATTR_MAX; ++j )
	{
		attrvalue = 100 * pInfo->AttrAttack.GetElement_Val(j);
		if(pOptionInfo)
			attroptvalue = 100 * pOptionInfo->AttrAttack.GetElement_Val(j);
		if(pRareOptionInfo)
		{
			attrRareOptValue = 100 * pRareOptionInfo->AttrAttack.GetElement_Val(j);
		}
		else attrRareOptValue = 0;
		
		if( (int)(attrvalue) != 0 )
		{
			if( (int)(attroptvalue) != 0 )
				sprintf( line, "%s +%d%% [+%d%%](+%d%%)", CHATMGR->GetChatMsg(392+j-1), (int)(attrvalue) , int(attrRareOptValue), (int)(attroptvalue));
			else if (pRareOptionInfo && attrRareOptValue)
				sprintf( line, "%s +%d%% [+%d%%]", CHATMGR->GetChatMsg(392+j-1), (int)(attrvalue ), int(attrRareOptValue) );
			else
				sprintf(line, "%s +%d%%", CHATMGR->GetChatMsg(392 + j - 1), (int)(attrvalue));
			
			if(pRareOptionInfo && attrRareOptValue)
				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
			else
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else
		{
			if( (int)(attroptvalue) != 0 )
			{
				if( attrRareOptValue )
					sprintf( line, "%s +%d%% (+%d%%)", CHATMGR->GetChatMsg(392+j-1), (int)(attrRareOptValue), (int)(attroptvalue) );
				else
					sprintf( line, "%s (+%d%%)", CHATMGR->GetChatMsg(392+j-1), (int)(attroptvalue) );
			}
			else
			{
				if( attrRareOptValue )
					sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(392+j-1), (int)(attrRareOptValue) );
			}
			if(attrRareOptValue)
				pIcon->AddToolTipLine( line, TTTC_RAREITEM );
			else if(attroptvalue)
				pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}


//#endif


	WORD RareOptPhyDef;
	if( pRareOptionInfo && pRareOptionInfo->PhyDefense )
		RareOptPhyDef = pRareOptionInfo->PhyDefense;
	else
		RareOptPhyDef = 0;
	if( pInfo->PhyDef != 0 )
	{
		if(pOptionInfo && pOptionInfo->PhyDefense != 0)
			sprintf(line, "%s +%d [+%d](+%d)", CHATMGR->GetChatMsg(397), pInfo->PhyDef , RareOptPhyDef, pOptionInfo->PhyDefense);
		else if (pRareOptionInfo && RareOptPhyDef)
			sprintf(line, "%s +%d [+%d]", CHATMGR->GetChatMsg(397), pInfo->PhyDef , RareOptPhyDef);
		else
			sprintf(line, "%s +%d", CHATMGR->GetChatMsg(397), pInfo->PhyDef + RareOptPhyDef);
		
		if(pRareOptionInfo && RareOptPhyDef)
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		else
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	else
	{
		if( RareOptPhyDef && pOptionInfo && pOptionInfo->PhyDefense )
		{
			sprintf(line, "%s +%d (+%d)", CHATMGR->GetChatMsg(397), RareOptPhyDef, pOptionInfo->PhyDefense);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
		else if(pOptionInfo && pOptionInfo->PhyDefense != 0)

		{
			sprintf(line, "%s (+%d)", CHATMGR->GetChatMsg(397), pOptionInfo->PhyDefense);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		else if(RareOptPhyDef)
		{
			sprintf(line, "%s +%d ", CHATMGR->GetChatMsg(397), RareOptPhyDef);
			pIcon->AddToolTipLine( line, TTTC_RAREITEM );
		}
	}
	
	if( pInfo->Plus_Value != 0 )
	{
		sprintf(line, CHATMGR->GetChatMsg(398), pInfo->ItemName, pInfo->Plus_Value);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}

	if( pInfo->AllPlus_Value != 0 && pInfo->AllPlus_Kind != 0 )
	{
		char buf[32] = {0,};
		switch( pInfo->AllPlus_Kind )
		{
		case 1: strcpy(buf, CHATMGR->GetChatMsg(406) ); break;
		case 2: strcpy(buf, CHATMGR->GetChatMsg(407) ); break;
		case 3: strcpy(buf, CHATMGR->GetChatMsg(408) ); break;
		case 4: strcpy(buf, CHATMGR->GetChatMsg(409) ); break;
		case 5: strcpy(buf, CHATMGR->GetChatMsg(410) ); break;
		case 6: strcpy(buf, CHATMGR->GetChatMsg(411) ); break;
		case 7: strcpy(buf, CHATMGR->GetChatMsg(412) ); break;
		case 8: strcpy(buf, CHATMGR->GetChatMsg(413) ); break;
		case 9: strcpy(buf, CHATMGR->GetChatMsg(414) ); break;
		case 10: strcpy(buf, CHATMGR->GetChatMsg(415) ); break;
		case 11: strcpy(buf, CHATMGR->GetChatMsg(416) ); break;
		}
		sprintf(line, CHATMGR->GetChatMsg(417), buf, pInfo->AllPlus_Value );
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	//CItem *ptItem = (CItem*)pIcon;
	//if(ptItem->GetStatic()==ITEM_STATIC_PD)
	//{
	//	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	//	pIcon->AddToolTipLine("",TTTC_LIMIT);
	//	pIcon->AddToolTipLine(CHATMGR->GetChatMsg(727),TTTC_LIMIT);
	//	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	//}
	if(pInfo->ItemKind & eSHOP_ITEM)
	{

	}
	if(pInfo->wSetItemKind != 0)
	{
		AddSetItemToolTip( (cIcon*)pIcon, pInfo);
	}
	if(pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
	{
		AddUniqueItemToolTip( (cIcon*)pIcon, pInfo);
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
#ifdef _ABRASION_
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	float CurAb,MaxAb;
	if (pcItem!=NULL)
	{		
		CurAb=pcItem->GetAbrasion();
		MaxAb=pcItem->GetMaxItemAbrasion();
	}
	else
	{
		CurAb=pExcItem->GetAbrasion();
		MaxAb=pExcItem->GetMaxItemAbrasion();
	}
	if ((CurAb/MaxAb)*100>=50)
	{
		sprintf(line, CHATMGR->GetChatMsg(1832),(WORD)CurAb,(WORD)MaxAb);
		pIcon->AddToolTipLine("",TTTC_LIMIT);
		pIcon->AddToolTipLine(line,TTTC_RAREITEM);
	}
	else
	{
		sprintf(line, CHATMGR->GetChatMsg(727),(WORD)CurAb,(WORD)MaxAb);
		pIcon->AddToolTipLine("",0);
		pIcon->AddToolTipLine(line,TTTC_LIMIT);
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
#endif
	//if(pIcon->GetType() !=WT_BUYITE)
	//{
	//	if (pcItem!=NULL)
	//	{
	//		if(strcmp(pcItem->GetMaker(),"无")!=0&&strcmp(pcItem->GetMaker1(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1833), pcItem->GetMaker());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//			sprintf(line, CHATMGR->GetChatMsg(1834), pcItem->GetMaker1());
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//		}
	//		else if(strcmp(pcItem->GetMaker(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1833), pcItem->GetMaker());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	//			//pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	//		}
	//		else if (strcmp(pcItem->GetMaker1(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1834), pcItem->GetMaker1());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//		}
	//	}
	//	else if(pExcItem!=NULL)
	//	{
	//		if(strcmp(pExcItem->GetMaker(),"无")!=0&&strcmp(pExcItem->GetMaker1(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1833), pExcItem->GetMaker());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//			sprintf(line, CHATMGR->GetChatMsg(1834), pExcItem->GetMaker1());
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//		}
	//		else if(strcmp(pExcItem->GetMaker(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1833), pExcItem->GetMaker());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//			//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	//			//pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	//		}
	//		else if (strcmp(pExcItem->GetMaker1(),"无")!=0)
	//		{
	//			sprintf(line, CHATMGR->GetChatMsg(1834), pExcItem->GetMaker1());
	//			pIcon->AddToolTipLine("",0);
	//			pIcon->AddToolTipLine(line,TTTC_MAKER);
	//		}
	//	}
	//}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );

	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	//pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	if(!bIsQuey)
	{
		int ItemPos = -1;
		if(pcItem)
			ItemPos = pcItem->GetPosition();
		else if(pExcItem)
			ItemPos = pExcItem->GetPosition();
		if(ItemPos!=-1&&ItemPos>=80&&ItemPos<=89)
		{
			pIcon->SetRelationToolTip(NULL);
			pIcon->SetRelationToolTip1(NULL);
			SetItemRelationTip(pIcon->GetToolTip(),ItemPos);
		}
		else
		{
			CItem * pTemp =NULL;
			CItem * pTemp1=NULL;
			if(pInfo->ItemKind == eEQUIP_ITEM_WEAPON)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(81);
			else if(pInfo->ItemKind == eEQUIP_ITEM_DRESS)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(82);
			else if(pInfo->ItemKind == eEQUIP_ITEM_HAT)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(80);
			else if(pInfo->ItemKind == eEQUIP_ITEM_SHOES)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(83);
			else if(pInfo->ItemKind == eEQUIP_ITEM_CAPE)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(86);
			else if(pInfo->ItemKind == eEQUIP_ITEM_NECKLACE)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(87);
			else if(pInfo->ItemKind == eEQUIP_ITEM_ARMLET)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(88);
			else if(pInfo->ItemKind == eEQUIP_ITEM_BELT)
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(89);
			else if(pInfo->ItemKind == eEQUIP_ITEM_RING)
			{
				pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(84);
				pTemp1 = GAMEIN->GetInventoryDialog()->GetItemForPos(85);
			}
			if(pTemp)
			{
				pIcon->SetRelationToolTip(((cIcon*)pTemp)->GetToolTip());
			}
			else
				pIcon->SetRelationToolTip(NULL);
			if(pTemp1)
			{
				pIcon->SetRelationToolTip1(((cIcon*)pTemp1)->GetToolTip());
			}
			else
				pIcon->SetRelationToolTip1(NULL);
		}
	}
	else
	{
		CItem * pTemp =NULL;
		CItem * pTemp1=NULL;
		if(pInfo->ItemKind == eEQUIP_ITEM_WEAPON)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(81);
		else if(pInfo->ItemKind == eEQUIP_ITEM_DRESS)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(82);
		else if(pInfo->ItemKind == eEQUIP_ITEM_HAT)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(80);
		else if(pInfo->ItemKind == eEQUIP_ITEM_SHOES)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(83);
		else if(pInfo->ItemKind == eEQUIP_ITEM_CAPE)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(86);
		else if(pInfo->ItemKind == eEQUIP_ITEM_NECKLACE)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(87);
		else if(pInfo->ItemKind == eEQUIP_ITEM_ARMLET)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(88);
		else if(pInfo->ItemKind == eEQUIP_ITEM_BELT)
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(89);
		else if(pInfo->ItemKind == eEQUIP_ITEM_RING)
		{
			pTemp = GAMEIN->GetInventoryDialog()->GetItemForPos(84);
			pTemp1 = GAMEIN->GetInventoryDialog()->GetItemForPos(85);
		}
		if(pTemp)
		{
			pIcon->SetRelationToolTip(((cIcon*)pTemp)->GetToolTip());
		}
		else
			pIcon->SetRelationToolTip(NULL);
		if(pTemp1)
		{
			pIcon->SetRelationToolTip1(((cIcon*)pTemp1)->GetToolTip());
		}
		else
			pIcon->SetRelationToolTip1(NULL);
	}
}
void CItemManager::SetItemRelationTip(cMultiLineText* ToolTip,int pos)
{
	if(pos == 80)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_HAT)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 81)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_WEAPON)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 82)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_DRESS)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 83)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_SHOES)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 84)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_RING)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 85)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_RING)
					tmp->SetRelationToolTip1(ToolTip);
			}
		}
	}
	if(pos == 86)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_CAPE)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 87)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_NECKLACE)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 88)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_ARMLET)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
	if(pos == 89)
	{
		CItem * tmp = NULL;
		for(int i=0;i<80;i++)
		{
			tmp = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(tmp)
			{
				if(tmp->GetItemKind()==eEQUIP_ITEM_BELT)
					tmp->SetRelationToolTip(ToolTip);
			}
		}
	}
}
void CItemManager::SetItemToolTipForStage( cIcon * pIcon, ITEM_INFO * pInfo )
{
	BYTE bStage = HERO->GetStage();
	switch( bStage )
	{
	case eStage_Normal:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_LIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Hwa:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Hyun:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_FREELIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_FREELIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_FREELIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_LIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_LIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Geuk:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_LIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_LIMIT );	break;
			}
		}
		break;
	case eStage_Tal:
		{
			switch( pInfo->LimitJob )
			{
			case eItemStage_ChangeStage:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(888), TTTC_FREELIMIT );	break;
			case eItemStage_ChangeStage2:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1354), TTTC_FREELIMIT);	break;
			case eItemStage_OverHwa:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(894), TTTC_LIMIT );	break;
			case eItemStage_Hwa:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(895), TTTC_LIMIT );	break;
			case eItemStage_Hyun:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(896), TTTC_LIMIT );	break;
			case eItemStage_OverGeuk:	pIcon->AddToolTipLine( CHATMGR->GetChatMsg(897), TTTC_FREELIMIT );	break;
			case eItemStage_Geuk:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(898), TTTC_FREELIMIT );	break;
			case eItemStage_Tal:		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(899), TTTC_FREELIMIT );	break;
			}
		}
		break;
	}
}

void CItemManager::SetPetSummonItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx )
{
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	char line[128];

	DWORD	ItemDBIdx = 0;

	const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
	cImage * imgPetIteminfo;

	imgPetIteminfo=pIcon->GetBasicImage();

	if(imgPetIteminfo)
	{
		pIcon->AddToolTipImageEquip(imgPetIteminfo);
	}
	if(dwItemDBIdx)
	{
		ItemDBIdx = dwItemDBIdx;
	}
	else
	{
		ItemDBIdx = pBaseInfo->dwDBIdx;
	}

	
	PET_TOTALINFO* pPetInfo = PETMGR->GetPetInfo(ItemDBIdx);
	BASE_PET_LIST* pList = NULL;
	if(pPetInfo)
		pList = GAMERESRCMNGR->GetPetListInfo(pPetInfo->PetKind);
	PET_BUFF_LIST* pBuffList = NULL;

	if( pPetInfo )
	{
		if( !pPetInfo->bAlive )
		{
			DWORD RedFilter = RGBA_MAKE(255,10,10,255);
			SetIconColorFilter(pIcon,RedFilter);
		}
		wsprintf( line, CHATMGR->GetChatMsg(1244), pPetInfo->PetGrade,pPetInfo->PetFriendly/100000 );
		pIcon->AddToolTipLine( line, ICONTEXT_USABLE );

	}
	else
	{
		DWORD friendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;
		wsprintf( line, CHATMGR->GetChatMsg(1244), PET_DEFAULT_GRADE ,friendly/100000);
		pIcon->AddToolTipLine( line, ICONTEXT_USABLE );
	
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());

	if(pList)
	{
		wsprintf( line, CHATMGR->GetChatMsg(1266), pList->InventoryTapNum[pPetInfo->PetGrade-1]);
		pIcon->AddToolTipLine( line, TTTC_QUESTITEM );
	}
	else
		return;

//#define MAX_PET_BUFF_NUM	3
	if(pList->BuffList[0])
	{
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1267), TTTC_QUESTITEM );

		char BuffName[128] = {0,};
		for( int i = 0; i < MAX_PET_BUFF_NUM; ++i )
		{
			WORD BuffIdx = pList->BuffList[pPetInfo->PetGrade-1][i];

			if(0 == BuffIdx) continue;

			pBuffList = GAMERESRCMNGR->GetPetBuffInfo(BuffIdx);

			if(pBuffList && pBuffList->BuffValueData)
			{
				//char* pBuffName = NULL;
				if( pBuffList->BuffKind == ePB_Demage_Percent )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1270), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Dodge )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1271), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1286), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_Item_DoubleChance )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1305), pBuffList->BuffSuccessProb );
				}
				else if( pBuffList->BuffKind == ePB_NoForeAtkMonster )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1350) );
				}
				else if( pBuffList->BuffKind == ePB_ReduceCriticalDmg )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1388), pBuffList->BuffValueData );
				}
				else if( pBuffList->BuffKind == ePB_MasterAllStatRound )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1389) );
				}
				else if( pBuffList->BuffKind == ePB_Item_RareProbUp )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1476) );
				}
				else if( pBuffList->BuffKind == ePB_MussangTimeIncrease )
				{
					wsprintf( BuffName, CHATMGR->GetChatMsg(1477) );
				}
				else if (pBuffList->BuffKind == ePB_ReduceCriticalRate)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(2410), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
				}
				else if (pBuffList->BuffKind == ePB_ReduceDemageRate)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(2411), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
				}
				else if (pBuffList->BuffKind == ePB_ReduceCriticalDmgPlayer)
				{
					wsprintf(BuffName, CHATMGR->GetChatMsg(2412), pBuffList->BuffSuccessProb, pBuffList->BuffValueData);
				}

			}

			wsprintf( line, CHATMGR->GetChatMsg(1268), BuffName );

			pIcon->AddToolTipLine( line, TTTC_QUESTITEM );
		}
	}


	wsprintf( line, CHATMGR->GetChatMsg(1281), pInfo->LimitLevel );
	pIcon->AddToolTipLine( line, TTTC_QUESTITEM	);
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	//pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());

	
}

void CItemManager::SetTitanEquipItemToolTip(cIcon * pIcon, ITEM_INFO * pInfo)
{
	char line[128];

	CItem* pItem = (CItem*)pIcon;
	cImage * imgTitanEquipinfo;

	imgTitanEquipinfo=pIcon->GetBasicImage();

	if(imgTitanEquipinfo)
	{
		pIcon->AddToolTipImageEquip(imgTitanEquipinfo);
	}
	//[物品等级限制][By:十里坡剑神][QQ:112582793][2018/1/2]
	if( pInfo->LimitLevel != 0 )
	{		
		sprintf(line, CHATMGR->GetChatMsg(238), pInfo->LimitLevel);
		if( HERO->GetCharacterTotalInfo()->Level < pInfo->LimitLevel )
			pIcon->AddToolTipLine( line, TTTC_LIMIT );
		else
			pIcon->AddToolTipLine( line, TTTC_FREELIMIT );				
	}
	//[泰坦状态][By:十里坡剑神][QQ:112582793][2018/1/2]
	if(HERO->InTitan() == TRUE)
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_FREELIMIT );
	else
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(1655), TTTC_LIMIT );
	
	pIcon->AddToolTipLine("");

	if(pInfo->MeleeAttackMin > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1521), pInfo->MeleeAttackMin);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->MeleeAttackMax > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1522), pInfo->MeleeAttackMax);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->RangeAttackMin > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1523), pInfo->RangeAttackMin);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->RangeAttackMax > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1524), pInfo->RangeAttackMax);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->CriticalPercent > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1525), pInfo->CriticalPercent);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->MugongNum > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1526), pInfo->MugongNum);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->PhyDef > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1527), pInfo->PhyDef);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->Life > 0)
	{
		ZeroMemory(&line, sizeof(line));
		wsprintf( line, CHATMGR->GetChatMsg(1528), pInfo->Life);
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1667), pInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1530), pInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1531), pInfo->AttrRegist.GetElement_Val(ATTR_WATER));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1532), pInfo->AttrRegist.GetElement_Val(ATTR_TREE));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1533), pInfo->AttrRegist.GetElement_Val(ATTR_IRON));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0)
	{
		ZeroMemory(&line, sizeof(line));
		sprintf( line, CHATMGR->GetChatMsg(1534), pInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
		pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
	}
	if(pInfo->WeaponType > 0)
	{		
		switch(pInfo->WeaponType)
		{
		case WP_GUM:
			sprintf(line, CHATMGR->GetChatMsg(1661));
			break;
		case WP_GWUN:
			sprintf(line, CHATMGR->GetChatMsg(1663));
			break;
		case WP_DO:		
			sprintf(line, CHATMGR->GetChatMsg(1662));
			break;
		case WP_CHANG:	
			sprintf(line, CHATMGR->GetChatMsg(1664));
			break;
		case WP_GUNG:	
			sprintf(line, CHATMGR->GetChatMsg(1665));

			break;
		case WP_AMGI:	
			sprintf(line, CHATMGR->GetChatMsg(1666));
			break;
		}
		pIcon->AddToolTipLine( line );
	}	
}

void CItemManager::AddGoldShopToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };

	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	cImage * imgGoldShopinfo;
	imgGoldShopinfo=pIcon->GetBasicImage();
	if(imgGoldShopinfo)
	{
		pIcon->AddToolTipImageEquip(imgGoldShopinfo);
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	
	if( pItemInfo->GenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1437), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1409), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1470), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1403), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1438), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1433), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1430), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1441), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1399), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1396), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			sprintf(line, CHATMGR->GetChatMsg(1402));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}

	if( pItemInfo->SimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1431), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1439), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1421), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Life != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1417), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Shield != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1428), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitJob != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1422), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
		{            
			sprintf(line, CHATMGR->GetChatMsg(1432), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGender != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1423), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitLevel != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1424), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1426), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1406), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1427), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1450), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitMinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1410), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1425), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1451), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}

	}
	if( pItemInfo->LimitCheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind ==eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1400), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitSimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1434), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->ItemGrade != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1429), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1447), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeType != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1452), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1405), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1551), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->EquipKind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1393), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1449), pItemInfo->MeleeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1401), pItemInfo->RangeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMax != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)
		{
			sprintf(line, CHATMGR->GetChatMsg(1474), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CriticalPercent != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1390), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1681), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->PhyDef != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1415), pItemInfo->PhyDef);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_MugongIdx != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1407), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_Value != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1408), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->AllPlus_Kind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->AllPlus_Kind);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyukRecover != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM || pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION || pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP||
			pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MARRYCHARM)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1682), pItemInfo->NaeRyukRecover);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{

			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
}



void CItemManager::SetGoldShopToolTipIcon(cIcon * pIcon)
{
	char buf[256] = { 0, };
	WORD index = (WORD)pIcon->GetData();
	eITEM_KINDBIT bits = GetItemKind(index);
	pIcon->IniTipLineCount();
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	cImage imgToolTip2;
	SCRIPTMGR->GetImage( 218, &imgToolTip2, PFT_HARDPATH );
	pIcon->AddImageTop(&imgToolTip2);
	pIcon->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	if(bits & eEQUIP_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetEquipItemToolTip( pIcon, pInfo,NULL, NULL,NULL );
	}
	else if(bits & eMUGONG_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetMugongItemToolTip( pIcon, pInfo );
	}
	else if(bits & eYOUNGYAK_ITEM)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetYoungyakItemToolTip( pIcon, pInfo );
	}
	else if(bits & eEXTRA_ITEM || bits & eEXTRA_ITEM_UPGRADE)
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if(pInfo)
			SetExtraItemToolTip( pIcon, pInfo );
	}
	else if( bits & eQUEST_ITEM )
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if( pInfo )
		{
			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );
			pIcon->AddToolTipLine( line );
			pIcon->AddToolTipLine( "" );

			if( FALSE == IsPetSummonItem(index) )
				pIcon->AddToolTipLine( CHATMGR->GetChatMsg(260), TTTC_QUESTITEM );

			if( IsPetSummonItem(index) )
			{
				SetPetSummonItemToolTip(pIcon,pInfo,NULL);
			}

			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
	}
	else if( bits & eCHANGE_ITEM )
	{
		ITEM_INFO * pInfo = GetItemInfo(index);
		if( pInfo )
		{
			cImage * imgExtraIteminfo;

			imgExtraIteminfo=pIcon->GetBasicImage();

			if(imgExtraIteminfo)
			{
				pIcon->AddToolTipImageEquip(imgExtraIteminfo);
			}
			CExchangeItem* pExcItem = NULL;
			CItem * pcItem = NULL;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
				pExcItem = (CExchangeItem*)pIcon;
			else
				pcItem = (CItem*)pIcon;
			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );			
			pIcon->AddToolTipLine( line );

			pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2240),NULL);

			if(pcItem)
			{
				if(pcItem->GetStatic()==1)
				{
					sprintf(line,CHATMGR->GetChatMsg(1608));
					pIcon->AddToolTipLine(line,TTTC_LIMIT);
				}
				else
					pIcon->AddToolTipLine("");
			}
			else
				pIcon->AddToolTipLine("");

			pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			if( bits == eCHANGE_ITEM_LOCK )
			{
				sprintf(line, CHATMGR->GetChatMsg(238), pInfo->ItemType);
				if( HERO->GetCharacterTotalInfo()->Level < pInfo->ItemType )
					pIcon->AddToolTipLine( line, TTTC_LIMIT );
				else
					pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
			}
			else
				pIcon->AddToolTipLine("");
			AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
		}
	}
	else if( bits & eSHOP_ITEM )
	{
		ITEM_INFO* pInfo = GetItemInfo(index);
		if(pInfo)
		{
			//[物品信息获取][By:十里坡剑神][QQ:112582793][2018/2/1]
			CExchangeItem* pExcItem = NULL;
			CItem * pcItem = NULL;
			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET||pIcon->GetType() == WT_BUYITE )
				pExcItem = (CExchangeItem*)pIcon;
			else
				pcItem = (CItem*)pIcon;

			char line[128];
			wsprintf( line, "[%s]", pInfo->ItemName );
			DWORD dwColor = TTTC_DEFAULT;
			if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
			{				
				if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
				{
					dwColor = TTTC_LIMIT;
				}
				else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
				{
					dwColor = TTTC_LIMIT;
				}
				else if( pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90 ) )
				{
					dwColor = TTTC_LIMIT;
				}
			}
			else if( HERO->GetLevel() < pInfo->NaeRyukRecoverRate )
			{
				dwColor = TTTC_LIMIT;
			}
			pIcon->AddToolTipLine( line, dwColor );
			if (pInfo->ItemKind==eSHOP_ITEM_PET||pInfo->ItemKind==eSHOP_ITEM_PET_EQUIP)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2241));
				if( pInfo->ItemKind == eSHOP_ITEM_PET )
				{
					if( pInfo->ItemType == 11 )
					{
						if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
						{
							sprintf(line,CHATMGR->GetChatMsg(1608));
							pIcon->AddToolTipLine(line,TTTC_LIMIT);
						}
						else
							pIcon->AddToolTipLine("");
					}
				}
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_MAKEUP||pInfo->ItemKind==eSHOP_ITEM_DECORATION)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2243));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_CHARM||pInfo->ItemKind==eSHOP_ITEM_HERB	||
				pInfo->ItemKind==eSHOP_ITEM_SUNDRIES||pInfo->ItemKind==eSHOP_ITEM_GETIN_ITEM ||
				pInfo->ItemKind==eSHOP_ITEM_IMAGENAME ||pInfo->ItemKind==eSHOP_ITEM_MARRYCHARM ||
				pInfo->ItemKind==eSHOP_ITEM_GOLDITEM ||pInfo->ItemKind==eSHOP_ITEM_GROWITEM)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2244));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
				//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
				pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			}
			else if (pInfo->ItemKind==eSHOP_ITEM_INCANTATION)
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2245));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine("");
				//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
				pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			}
			else 
			{
				pIcon->AddToolTipLine(CHATMGR->GetChatMsg(2246));
				if(pcItem)
				{
					if(pcItem->GetStatic()==1)
					{
						sprintf(line,CHATMGR->GetChatMsg(1608));
						pIcon->AddToolTipLine(line,TTTC_LIMIT);
					}
					else
						pIcon->AddToolTipLine("");
				}
				else
					pIcon->AddToolTipLine("");
				pIcon->AddToolTipLine("");
				//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
				pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
			}
			if( pInfo->NaeRyukRecoverRate != 0 )
			{

				sprintf(line, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate);
				if( HERO->GetCharacterTotalInfo()->Level < pInfo->NaeRyukRecoverRate )
					pIcon->AddToolTipLine( line, TTTC_LIMIT );
				else
					pIcon->AddToolTipLine( line, TTTC_FREELIMIT );
			}
			else
				pIcon->AddToolTipLine("");


			if( pIcon->GetType() == WT_EXCHANGEITEM || pIcon->GetType() == WT_STALLITEM || pInfo->ItemKind == eSHOP_ITEM_PET ||pIcon->GetType() == WT_BUYITE)
			{
				CExchangeItem* pExItem = (CExchangeItem*)pIcon;
				cImage * imgGoldShopinfo;
				imgGoldShopinfo=pIcon->GetBasicImage();
				if(imgGoldShopinfo)
				{
					pIcon->AddToolTipImageEquip(imgGoldShopinfo);
				}
				if( pExItem->GetItemParam() & ITEM_PARAM_SEAL )
				{
					if( IsPetSummonItem(index) )
					{
						//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
						pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
						SetPetSummonItemToolTip(pIcon,pInfo,NULL);
					}
					AddItemDescriptionToolTip( pIcon, 6000 );
					AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
				}
				else
				{
					const ITEMBASE* pBaseInfo = ((CItem*)pIcon)->GetItemBaseInfo();
					if( IsPetSummonItem(index) )
					{
						SetPetSummonItemToolTip(pIcon,pInfo,NULL);

					}

					AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );
				}
				return;
			}

			CItem* pItem = (CItem*)pIcon;
			if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION )
			{
				if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
				{
					AddGoldShopToolTip( (cIcon*)pItem, pInfo ); 
					return;
				}				
			}
			if( pIcon->GetType()==WT_ITEM && (pItem->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				AddGoldShopToolTip( (cIcon*)pItem, pInfo );
				AddItemDescriptionToolTip( pIcon, 6000 );
				PrintShopItemUseTime( pItem, pInfo );
			}
			else
			{
				AddGoldShopToolTip( (cIcon*)pItem, pInfo );  
				AddItemDescriptionToolTip( pIcon, pInfo->ItemTooltipIdx );

				if( pInfo->ItemType == 11 )
				{
					{
						SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItem->GetItemIdx() );
						if( pShopItemBase )
						{
							if( pInfo->SellPrice == eShopItemUseParam_Realtime )
							{
								pItem->AddToolTipLine("");
								pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

								stTIME time;
								char buf[128] = { 0, };
								time.value = pShopItemBase->Remaintime;	
								wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()/*+15*/, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
								pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
							}
							else 
							{
								pItem->AddToolTipLine("");
								pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );
								pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );

							}

						}

					}
				}
				else{
					PrintShopItemUseTime( pItem, pInfo );
				}

			}			

		}
	}
}

void CItemManager::AddItemDescriptionToolTip( cWindow* pWindow, WORD wIndex, DWORD dwTxtColor, int nSpaceLine )
{
	TOOLTIP_TEXT* pTooltipText = m_ItemToolTipTextList.GetData( wIndex );

	if( pTooltipText )
	{
		for( int i = 0 ; i < nSpaceLine ; ++i )
			pWindow->AddToolTipLine("");
	}

	while( pTooltipText )
	{
		pWindow->AddToolTipLine( pTooltipText->strToolTip, dwTxtColor );
		pTooltipText = pTooltipText->pNext;
	}
}
void CItemManager::ReleaseGenerateIndex(DWORD val)
{
//#ifdef _GMTOOL_
//	CHATMGR->AddMsg(CTC_SYSMSG,"释放资源ID[%d]",val);
//#endif
	m_IconIndexCreator.ReleaseIndex(val);
}
void CItemManager::ItemDelete(CItem* pItem)
{
	m_ItemHash.Remove(pItem->GetDBIdx());	
//#ifdef _GMTOOL_
//	CHATMGR->AddMsg(CTC_SYSMSG,"释放资源ID[%d]",pItem->GetID());
//#endif
	m_IconIndexCreator.ReleaseIndex(pItem->GetID());
	
	if( pItem->GetLinkItem() )
	{
		GAMEIN->GetExchangeDialog()->DelItem( 0, pItem->GetLinkItem()->GetPosition() );
	}
	WINDOWMGR->AddListDestroyWindow( pItem );
}
void CItemManager::Release()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	while(pItem = m_ItemHash.GetData())
	{
		ItemDelete(pItem);
	}
	m_ItemHash.RemoveAll();

	ITEM_OPTION_INFO * pOInfo = NULL;
	m_ItemOptionList.SetPositionHead();
	while(pOInfo = m_ItemOptionList.GetData())
		delete pOInfo;
	m_ItemOptionList.RemoveAll();

	ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	m_ItemRareOptionList.SetPositionHead();
	while(pRareInfo = m_ItemRareOptionList.GetData())
		delete pRareInfo;
	m_ItemRareOptionList.RemoveAll();

	GAMEIN->GetInventoryDialog()->ReleaseInventory();
}

CItem* CItemManager::GetItem(DWORD dwDBIdx)
{
	CItem* pItem = NULL;
	pItem = m_ItemHash.GetData(dwDBIdx);
	return pItem;
}
void CItemManager::ItemCombineCheck(WORD TabIdx, CItem* FromItem)
{
	switch (TabIdx)
	{
	case eItemTable_Pyoguk:
		{
			CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
			if (pPyogukDlg)
			{
				if (pPyogukDlg->IsActive())
				{//[普通仓库打开，判断选中物品类型是否可存入][BY:十里坡剑神][QQ:112582793][2019-4-16][17:41:58]
					pPyogukDlg->MoveItemToPyoguk(FromItem);
				}
			}
		}
		break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
			if (pPetInven)
			{
				if (pPetInven->IsActive())
				{
					pPetInven->MoveItemToPetInventory(FromItem);
				}
			}
		}
		break;
	default:
		break;
	}
}
void CItemManager::CombineItem( MSG_ITEM_COMBINE_ACK * pMsg )
{	
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);

	CItem * pFromItem = GetItemofTable(FromTableIdx, pMsg->FromPos);
	CItem * pToItem = GetItemofTable(ToTableIdx, pMsg->ToPos);

	if( !pFromItem || !pToItem )
	{
		ASSERTMSG(0, "Item Combine Failed : pToItem=NULL or pFromItem=NULL");
		return;
	}
	if( ( pFromItem->GetItemIdx() != pToItem->GetItemIdx() ) || ( pFromItem->GetItemIdx() != pMsg->wItemIdx) )
	{
		ASSERTMSG(0, "Item Combine Failed : FromItemIdx != ToItemIdx");
		return;
	}
	pFromItem->SetDurability( pMsg->FromDur );
	pToItem->SetDurability( pMsg->ToDur );
	
	if( pMsg->FromDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition());
	}

	if( pMsg->ToDur == 0 )
	{
		DeleteItemofTable(FromTableIdx, pFromItem->GetPosition());
	}
	if (pMsg->FromDur != 0)
	{
		ItemCombineCheck(ToTableIdx, pFromItem);
		WINDOWMGR->BackDragWindow(pFromItem->GetID());
	}
	
	//if(ToTableIdx==eItemTable_Pyoguk)
	//{//[右键仓储-合并完成需要移动一下物品否则图标位置不对][BY:十里坡剑神][QQ:112582793][2019-4-16][21:26:35]
	//	if(TP_PYOGUK_START<pToItem->GetPosition())
	//		GAMEIN->GetPyogukDialog()->FakeGeneralItemMove(pToItem->GetPosition()-1,pToItem,GAMEIN->GetPyogukDialog()->GetItemForPos(pToItem->GetPosition()-1));
	//	else
	//		GAMEIN->GetPyogukDialog()->FakeGeneralItemMove(pToItem->GetPosition()+1,pToItem,GAMEIN->GetPyogukDialog()->GetItemForPos(pToItem->GetPosition()+1));	
	//}
}

void CItemManager::MoveItem( MSG_ITEM_MOVE_ACK * pMsg )
{
 	BYTE tableIdx = GetTableIdxForAbsPos(pMsg->ToPos);
	if(tableIdx == eItemTable_Pyoguk)
		MoveItemToPyoguk(pMsg);
	else if(tableIdx == eItemTable_GuildWarehouse)
		MoveItemToGuild(pMsg);
	else if(tableIdx == eItemTable_Shop)
		MoveItemToShop(pMsg);
	else if(tableIdx == eItemTable_ShopInven)
		MoveItemToShopInven(pMsg);
	else if(tableIdx == eItemTable_PetInven || tableIdx == eItemTable_PetWeared )
	{
		MoveItemToPetInven(pMsg);

	}
	else if(tableIdx == eItemTable_Titan)
		MoveItemToTitanInven(pMsg);
	else if(tableIdx == eItemTable_TitanShopItem)
		MoveItemToTitanShopInven(pMsg);
	else if(tableIdx == eItemTable_Inventory || tableIdx == eItemTable_Weared)
	{
		CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
		CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
		CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
		CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
		CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();		
		CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();
		
		CItem * FromItem = NULL;
		CItem * ToItem = NULL;
		
		pInven->DeleteItem(pMsg->ToPos, &ToItem);

		WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
		if(FromTableIdx == eItemTable_Pyoguk)
		{
			pPyoguk->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_GuildWarehouse)
		{
			pGuild->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_Shop)
		{
			pShop->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_PetInven)
		{
			pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
		}
		else if(FromTableIdx == eItemTable_Titan)
		{
            pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );

			HERO->GetTitanAppearInfo()->WearedItemIdx[pMsg->FromPos-TP_TITANWEAR_START] = 0;
			
			TITANMGR->RemoveTitanUsingEquipItemList(FromItem->GetDBIdx());
		}
		else 
			pInven->DeleteItem( pMsg->FromPos , &FromItem );

		if(ToItem)
		{
			ToItem->SetPosition( pMsg->FromPos );

			ITEM_INFO* pinfo = GetItemInfo( ToItem->GetItemBaseInfo()->wIconIdx );
			if( pinfo )
			{
				POSTYPE pos = 0;
				if( (pinfo->ItemKind & eEQUIP_ITEM) && pInven->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_SHOPINVEN_START <= pMsg->FromPos && pMsg->FromPos < TP_SHOPINVEN_END )
					{
						FromTableIdx = eItemTable_Inventory;                        
						ToItem->SetPosition( pos );						
					}
				}
				else if( (pinfo->ItemKind == eSHOP_ITEM_EQUIP) && pInven->GetShopInven()->GetBlankPositionRestrictRef( pos ) )
				{
					if( TP_INVENTORY_START <= pMsg->FromPos && pMsg->FromPos < TP_INVENTORY_END )
					{
						FromTableIdx = eItemTable_ShopInven;
						ToItem->SetPosition( pos );
					}
				}
			}
			if(FromTableIdx == eItemTable_Pyoguk)
			{
				if(ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
				pPyoguk->AddItem(ToItem);
			}
			else if(FromTableIdx == eItemTable_GuildWarehouse)
			{
				if(ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
				pGuild->AddItem(ToItem);
				QUICKMGR->RefreshQickItem();
			}
			else if(FromTableIdx == eItemTable_PetInven)
			{
				if(POSTYPE Pos = ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(Pos);
				pPetInven->AddItem(ToItem);
			}
			else if(FromTableIdx == eItemTable_Titan)
			{
				if(POSTYPE Pos = ToItem->GetQuickPosition())
					QUICKMGR->RemQuickItem(Pos);
				pTitanInven->AddItem(ToItem);

				if( HERO->IsTitanPreView() )
				{
					HERO->GetTitanPreViewInfo()->WearedItemIdx[pMsg->FromPos-TP_TITANWEAR_START] = ToItem->GetItemIdx();
				}
			}
			else
				pInven->AddItem(ToItem);
		}
		
		if(FromItem == 0)
		{
			ASSERT(0);
			return;
		}
		FromItem->SetPosition(pMsg->ToPos);
		if(pMsg->FromPos>=80&&pMsg->FromPos<=89)
			SetItemRelationTip(NULL,pMsg->FromPos);
		pInven->AddItem(FromItem); 
		QUICKMGR->RefreshQickItem();

		APPEARANCEMGR->InitAppearance( HERO );
		if(HERO->InTitan() == TRUE)
		{			
			GAMEIN->GetTitanGuageDlg()->SetLife(TITANMGR->GetCurRidingTitan()->GetTitanTotalInfo()->Fuel);
		}

		if(((ToItem != NULL) && (ToItem->GetItemInfo()->wSetItemKind != 0)) || (FromItem->GetItemInfo()->wSetItemKind != 0))
		{
			RefreshAllItem();		
		}		
	}
}

void CItemManager::MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg )
{	
	CGuildWarehouseDialog * pGuildDlg	= GAMEIN->GetGuildWarehouseDlg();
	CInventoryExDialog * pInvenDlg = GAMEIN->GetInventoryDialog();
		
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);
	WORD ToTableIdx = GetTableIdxForAbsPos(pMsg->ToPos);

	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pGuildDlg->DeleteItem(pMsg->ToPos , &ToItem);

	if(FromTableIdx == eItemTable_GuildWarehouse)
	{
		pGuildDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInvenDlg->DeleteItem( pMsg->FromPos, &FromItem);
	}
	else 
		ASSERT(0);
	
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory)
			pInvenDlg->AddItem(ToItem);
		else
			pGuildDlg->AddItem(ToItem);
	}

	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pGuildDlg->AddItem(FromItem);
	
	QUICKMGR->RefreshQickItem();

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
}

void CItemManager::MoveItemToPyoguk(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
	
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pPyoguk->DeleteItem(pMsg->ToPos, &ToItem);
	
	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );				
	}
		
	else if(FromTableIdx == eItemTable_Pyoguk)
	{
		pPyoguk->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}
		
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared)
			pInven->AddItem(ToItem);
		else
			pPyoguk->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}

	ReLinkQuickPosition(FromItem);
	
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	ASSERT(FromItem);
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pPyoguk->AddItem(FromItem);
	
	QUICKMGR->RefreshQickItem();
}



void CItemManager::MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();

	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();
	
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pShop->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if( FromTableIdx == eItemTable_Shop )
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );				
	}
	else if( FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
		
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );		

		if( FromTableIdx == eItemTable_Shop )		
			pShop->AddItem(ToItem);	
		else if( FromTableIdx == eItemTable_ShopInven )		
			pInven->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}

	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());

	ASSERT(FromItem);


	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pShop->AddItem(FromItem);

	QUICKMGR->RefreshQickItem();
}



void CItemManager::MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CItemShopDialog* pShop = GAMEIN->GetItemShopDialog();	
	CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
	CTitanInventoryDlg* pTitanShopInven = GAMEIN->GetTitanInventoryDlg();

	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pInven->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if(FromTableIdx == eItemTable_Shop)
	{
		pShop->DeleteItem( pMsg->FromPos, &FromItem );
	}	
	else if(FromTableIdx == eItemTable_ShopInven)
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_Weared )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_PetWeared)
	{
		pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_TitanShopItem)
	{
		pTitanShopInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	
	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );

		if(FromTableIdx == eItemTable_Shop)		
			pShop->AddItem(ToItem);
		else if(FromTableIdx == eItemTable_ShopInven)		
			pInven->AddItem(ToItem);
		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}

	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());

	ASSERT(FromItem);

	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pInven->AddItem(FromItem);	

	QUICKMGR->RefreshQickItem();
}

void CItemManager::MoveItemToPetInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
	CPetInventoryDlg*	pPetInven = GAMEIN->GetPetInventoryDialog();

	CItem* FromItem	= NULL;
	CItem* ToItem = NULL;

	pPetInven->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if(FromTableIdx == eItemTable_PetInven || FromTableIdx == eItemTable_PetWeared)
	{
		pPetInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}

	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven)
		{
			pInven->AddItem(ToItem);
		}
		else if(FromTableIdx == eItemTable_PetInven || FromTableIdx == eItemTable_PetWeared)
		{
			pPetInven->AddItem(ToItem);
		}
		if(ToItem->GetQuickPosition())
		{
			ASSERT(0);
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
			ToItem->SetQuickPosition(0);
		}
	}
	ASSERT(FromItem);
	if(FromItem->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	}
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pPetInven->AddItem(FromItem);

	QUICKMGR->RefreshQickItem();
}

void CItemManager::MoveItemToTitanInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();	
	CTitanInventoryDlg*	pTitanInven = GAMEIN->GetTitanInventoryDlg();

	CItem* FromItem	= NULL;
	CItem* ToItem = NULL;

	pTitanInven->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if( FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if( FromTableIdx == eItemTable_Titan )
	{
		pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else
	{
		ASSERT(0);
		return;
	}

	if(ToItem)
	{
		TITANMGR->RemoveTitanUsingEquipItemList(ToItem->GetDBIdx());
		ToItem->SetPosition( pMsg->FromPos );
		if(FromTableIdx == eItemTable_Inventory || FromTableIdx == eItemTable_Weared || FromTableIdx == eItemTable_ShopInven)
		{
			pInven->AddItem(ToItem);
		}
		if(ToItem->GetQuickPosition())
		{
			ASSERT(0);
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
			ToItem->SetQuickPosition(0);
		}
	}

	ASSERT(FromItem);
	if(FromItem->GetQuickPosition())
	{
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
	}
	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pTitanInven->AddItem(FromItem);

	HERO->GetTitanAppearInfo()->WearedItemIdx[pMsg->ToPos-TP_TITANWEAR_START] = pMsg->wFromItemIdx;
	APPEARANCEMGR->InitAppearance( HERO );	
	GAMEIN->GetTitanGuageDlg()->SetLife(TITANMGR->GetCurRidingTitan()->GetTitanTotalInfo()->Fuel); //2007. 10. 5. CBH - 鸥捞藕 厘厚 馒侩矫 EP DLG 悸泼
	
	TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(FromItem->GetDBIdx());
	if(pInfo)
		TITANMGR->AddTitanUsingEquipItemList(pInfo);
	else
	{
		ASSERT(0);
	}

	QUICKMGR->RefreshQickItem();
}

void CItemManager::MoveItemToTitanShopInven(MSG_ITEM_MOVE_ACK* pMsg)
{
	CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
	CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();

	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	pTitanInven->DeleteItem(pMsg->ToPos, &ToItem);

	WORD FromTableIdx = GetTableIdxForAbsPos(pMsg->FromPos);

	if( FromTableIdx == eItemTable_TitanShopItem )
	{
		pTitanInven->DeleteItem( pMsg->FromPos, &FromItem );
	}
	else if( FromTableIdx == eItemTable_ShopInven )
	{
		pInven->DeleteItem( pMsg->FromPos, &FromItem );
	}

	if(ToItem)
	{
		ToItem->SetPosition( pMsg->FromPos );		

		if( FromTableIdx == eItemTable_Shop )		
			pTitanInven->AddItem(ToItem);	
		else if( FromTableIdx == eItemTable_ShopInven )		
			pInven->AddItem(ToItem);
		else if( FromTableIdx == eItemTable_TitanShopItem )
			pTitanInven->AddItem(ToItem);

		if(ToItem->GetQuickPosition())
			QUICKMGR->RemQuickItem(ToItem->GetQuickPosition());
	}

	ReLinkQuickPosition(FromItem);
	if(FromItem->GetQuickPosition())
		QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());

	ASSERT(FromItem);


	FromItem->SetPosition(pMsg->ToPos);
	FromItem->SetQuickPosition(0);
	pTitanInven->AddItem(FromItem);

	QUICKMGR->RefreshQickItem();
}

void CItemManager::DeleteItem( POSTYPE absPos, CItem** ppItemOut, ITEM_OPTION_INFO * pItemOptionInfoOut, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut, WORD RemainOptionFlag )
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);
	if(pItemOptionInfoOut)
	{
		memset(pItemOptionInfoOut, 0, sizeof(ITEM_OPTION_INFO));
	}

	if(pItemRareOptionInfoOut)
	{
		memset(pItemRareOptionInfoOut, 0, sizeof(ITEM_RARE_OPTION_INFO));
	}
		
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pInven = GAMEIN->GetInventoryDialog();
			pInven->DeleteItem( absPos, ppItemOut );

		}break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog* pPyoguk = GAMEIN->GetPyogukDialog();
			pPyoguk->DeleteItem( absPos, ppItemOut );
			
		}break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog* pGuild = GAMEIN->GetGuildWarehouseDlg();
			pGuild->DeleteItem( absPos, ppItemOut );
		}break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
			pPetInven->DeleteItem( absPos, ppItemOut );
		}break;
	}

	if((*ppItemOut))
	{

		if(IsOptionItem((*ppItemOut)->GetItemIdx(), (*ppItemOut)->GetDurability()) && !(RemainOptionFlag & eRemainReinforceOption))
		{
			RemoveItemOption((*ppItemOut)->GetDurability(), pItemOptionInfoOut);
		}

		if(IsRareOptionItem((*ppItemOut)->GetItemIdx(), (*ppItemOut)->GetRareness()) && !(RemainOptionFlag & eRemainRareOption))
		{
			RemoveItemRareOption((*ppItemOut)->GetRareness(), pItemRareOptionInfoOut);
		}

		if( IsTitanCallItem((*ppItemOut)->GetItemIdx()) )
		{
			TITANMGR->RemoveTitanInfo((*ppItemOut)->GetDBIdx());
		}

		if( IsTitanEquipItem((*ppItemOut)->GetItemIdx()) )
		{
			TITANMGR->RemoveTitanEquipInfo((*ppItemOut)->GetDBIdx());
		}


		if((*ppItemOut)->GetQuickPosition())
		{
			QUICKMGR->RemQuickItem((*ppItemOut)->GetQuickPosition());
		}
		ItemDelete( (*ppItemOut) );


	}
		
}

void CItemManager::DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION_INFO * pItemOptionInfoOut, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut,BOOL bIsDeleteItemInfo,BOOL bIsDeleteQuickInfo )
{
	CItem * pItemOut = NULL;
	if(pItemOptionInfoOut)
	{
		memset(pItemOptionInfoOut, 0, sizeof(ITEM_OPTION_INFO));
	}
	if(pItemRareOptionInfoOut)
	{
		memset(pItemRareOptionInfoOut, 0, sizeof(ITEM_RARE_OPTION_INFO));
	}

	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}

			if(pItemOut)
			{
				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					if( !(pItemOut->GetItemParam() & ITEM_PARAM_SEAL) )
						RemoveUsedItemInfo( pItemOut->GetItemIdx(), pItemOut->GetItemBaseInfo()->dwDBIdx );
					if( pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend || 
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend7 ||
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMove2 ||
						pItemOut->GetItemInfo()->ItemIdx == eIncantation_MemoryMoveExtend30 )
						HERO->CalcShopItemOption( eIncantation_MemoryMoveExtend, FALSE );
				}
			}
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : DeleteItem return FALSE");
				return;
			}

			if(pItemOut)
			{
				if( pItemOut->GetItemInfo()->ItemType == 11 )
				{
					RemoveUsedItemInfo( pItemOut->GetItemInfo()->ItemIdx, pItemOut->GetItemBaseInfo()->dwDBIdx );
				}
			}
		}
		break;
	case eItemTable_PetWeared:
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : Pet Inven DeleteItem return FALSE");
				return;
			}
		}
		break;
	case eItemTable_Titan:
	case eItemTable_TitanShopItem:
		{			
			CTitanInventoryDlg* pDlg = GAMEIN->GetTitanInventoryDlg();
			if( !pDlg->DeleteItem( absPos, &pItemOut ) )
			{
				ASSERTMSG(0, "Item Delete Failed : Titan Inven DeleteItem return FALSE");
				return;
			}

			if( HERO->IsTitanPreView() )
			{
				HERO->GetTitanPreViewInfo()->WearedItemIdx[absPos-TP_TITANWEAR_START] = 0;
				APPEARANCEMGR->InitAppearance( HERO );
			}
		}
		break;
	}
	if(pItemOut)
	{
		if(IsOptionItem(pItemOut->GetItemIdx(), pItemOut->GetDurability())&&! bIsDeleteItemInfo)
		{
			RemoveItemOption(pItemOut->GetDurability(), pItemOptionInfoOut);
		}
		if(IsRareOptionItem(pItemOut->GetItemIdx(), pItemOut->GetRareness())&&! bIsDeleteItemInfo)
		{
			RemoveItemRareOption(pItemOut->GetRareness(), pItemRareOptionInfoOut);
		}
		if(IsPetSummonItem(pItemOut->GetItemIdx()))
		{
			PETMGR->RemovePetInfo(pItemOut->GetDBIdx());
		}
		else if(!bIsDeleteQuickInfo)
		{
			ReLinkQuickPosition(pItemOut);
		}
		if(pItemOut->GetItemKind() == eTITAN_ITEM_PAPER)
		{
			TITANMGR->RemoveTitanInfo(pItemOut->GetDBIdx());

			if(pItemOut->GetDBIdx() != TITANMGR->GetRegistedTitanItemDBIdx())
			{
				TITANMGR->SetRegistedTitanItemDBIdx(0);
			}
		}

		if(pItemOut->GetQuickPosition())
		{
			QUICKMGR->RemQuickItem(pItemOut->GetQuickPosition());
		}
		ItemDelete( pItemOut );

		if(pItemOut->GetItemKind() & eTITAN_EQUIPITEM)
		{
			if( TableIdx == eItemTable_Titan )
			{
				TITANMGR->RemoveTitanUsingEquipItemList(pItemOut->GetDBIdx());
			}

			TITANMGR->RemoveTitanEquipInfo(pItemOut->GetDBIdx());
		}		
	}

	if( (pItemOut != NULL) && (pItemOut->GetItemInfo()->wSetItemKind != 0) )
	{
		RefreshAllItem();		
	}
}

void CItemManager::InitItemRareOption(ITEM_RARE_OPTION_INFO* pInfo, WORD num)
{
	for(int i = 0; i < num; ++ i)
	{
		AddItemRareOption(&pInfo[i]);
	}
}
	
void CItemManager::AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo)
{
	if( m_ItemRareOptionList.GetData(pInfo->dwRareOptionIdx) )
	{
		return;
	}
	ITEM_RARE_OPTION_INFO* pNewInfo = new ITEM_RARE_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_RARE_OPTION_INFO));
	m_ItemRareOptionList.Add(pNewInfo, pNewInfo->dwRareOptionIdx);
}

ITEM_RARE_OPTION_INFO* CItemManager::GetItemRareOption(DWORD dwRareOptionIdx)
{
	ITEM_RARE_OPTION_INFO* pInfo = m_ItemRareOptionList.GetData(dwRareOptionIdx);
	if(pInfo)
		return pInfo;

	return NULL;
}

void CItemManager::RemoveItemRareOption(DWORD dwRareOptionIdx, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut)
{
	ITEM_RARE_OPTION_INFO* pNewInfo = m_ItemRareOptionList.GetData(dwRareOptionIdx);

	if( pNewInfo == NULL)
	{
		ASSERT(0);
		return;
	}
	
	if(pItemRareOptionInfoOut)
		*pItemRareOptionInfoOut = *pNewInfo;
	SAFE_DELETE( pNewInfo );
	m_ItemRareOptionList.Remove(dwRareOptionIdx);
}

void CItemManager::RemoveItemOption(DWORD dwOptionIdx, ITEM_OPTION_INFO * pItemOptionInfoOut)
{
	ITEM_OPTION_INFO * pNewInfo = m_ItemOptionList.GetData(dwOptionIdx);

	if( pNewInfo == NULL )
	{
		ASSERT(0);		
		return;
	}

	if(pItemOptionInfoOut)
		*pItemOptionInfoOut = *pNewInfo;
	SAFE_DELETE( pNewInfo );
	m_ItemOptionList.Remove(dwOptionIdx);
}
bool CItemManager::IsOptionValuablesItem(DWORD OptionIndex)
{
	ITEM_OPTION_INFO* pInfo = GetItemOption(OptionIndex);

	return pInfo->dwItemDBIdx != 0;
}
void CItemManager::AddItemOption(ITEM_OPTION_INFO * pInfo)
{
	if( m_ItemOptionList.GetData(pInfo->dwOptionIdx) )
	{
		return;
	}
	ITEM_OPTION_INFO * pNewInfo = new ITEM_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_OPTION_INFO));
	m_ItemOptionList.Add(pNewInfo, pNewInfo->dwOptionIdx);
}
ITEM_OPTION_INFO * CItemManager::GetItemOption(DWORD dwOptionIdx)
{
	ITEM_OPTION_INFO* pInfo = m_ItemOptionList.GetData(dwOptionIdx);
	if(pInfo)
		return pInfo;
	
	static ITEM_OPTION_INFO NULLINFO;
	memset(&NULLINFO,0,sizeof(ITEM_OPTION_INFO));
	return &NULLINFO;
}
void CItemManager::InitItemOption(ITEM_OPTION_INFO * pInfo, WORD num)
{
	for(int i = 0 ; i < num ; ++i)
	{
		AddItemOption(&pInfo[i]);
	}
}

BOOL CItemManager::IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos,BOOL bIsMix )
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		return (TableIdx == eItemTable_Inventory);
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return (TableIdx == eItemTable_Weared);
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return (TableIdx == eItemTable_GuildWarehouse);
	}
	else if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		return (TableIdx == eItemTable_Pyoguk);
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		if(bIsMix)
			return TRUE;
		else
			return (TableIdx == eItemTable_ShopInven);
	}
	else
	{
		return FALSE;
	}
}
BYTE CItemManager::GetTableIdxForAbsPos(POSTYPE absPos)
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		return eItemTable_Inventory;
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return eItemTable_Weared;
	}
	else if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		return eItemTable_GuildWarehouse; 
	}
	else if(TP_PYOGUK_START <= absPos && absPos < TP_PYOGUK_END)
	{
		return eItemTable_Pyoguk;
	}
	else if(TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END)
	{
		return eItemTable_Shop;
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return eItemTable_ShopInven;
	}
	else if(TP_PETINVEN_START <= absPos && absPos < TP_PETINVEN_END)
	{
		return eItemTable_PetInven;
	}
	else if(TP_PETWEAR_START <= absPos && absPos < TP_PETWEAR_END)
	{
		return eItemTable_PetWeared;
	}
	else if(TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END)
	{
		return eItemTable_Titan;
	}
	else if(TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END)
	{
		return eItemTable_TitanShopItem;
	}
	else
	{
		return 255;
	}
}


void CItemManager::RefreshAllItem()
{
	CItem* pItem;
	m_ItemHash.SetPositionHead();
	
	while(pItem = m_ItemHash.GetData())
	{
		if(pItem->GetItemKind() & eEQUIP_ITEM)
		{
			if(CanEquip(pItem->GetItemIdx()))
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
				pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
				pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
			}
		}
		else if(pItem->GetItemKind() & eMUGONG_ITEM)
		{
			if(CanConvertToMugong(pItem->GetItemIdx()))
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
				pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANCONVERT );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
				pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANNOTCONVERT );
			}
		}
		else if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
		{
			if(HERO->InTitan() == TRUE)
			{
				pItem->SetImageRGB( ICONCLR_USABLE );
			}
			else
			{
				pItem->SetImageRGB( ICONCLR_DISABLE );
			}
		}

		if( pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()) )
		{
			SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0); 
		}
		else
			SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0); 
		
		if( m_bAddPrice )
		{
			DWORD dwColor = TTTC_SELLPRICE;
			char buf[128];
			MONEYTYPE SellPrice = SWPROFIT->CalTexRateForSell(pItem->GetItemInfo()->SellPrice);
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			switch (pItem->GetItemInfo()->wSellType)
			{
			case eMoney:wsprintf(buf, CHATMGR->GetChatMsg(2413), AddComma(SellPrice)); dwColor = TTTC_SELLPRICE;break;
			case eMall:wsprintf(buf, CHATMGR->GetChatMsg(2414), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW5;	break;
			case eGold:wsprintf(buf, CHATMGR->GetChatMsg(2415), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW6;	break;
			default:
				break;
			}
			pItem->AddToolTipLine( "" );
			pItem->AddToolTipLine(buf, dwColor);
		}
	}
}

void CItemManager::RefreshItem( CItem* pItem )
{
	if(pItem->GetItemKind() & eEQUIP_ITEM)
	{
		if(CanEquip(pItem->GetItemIdx()))
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANEQUIP );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
		}
	}
	else if(pItem->GetItemKind() & eMUGONG_ITEM)
	{
		if(CanConvertToMugong(pItem->GetItemIdx()))
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
			pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANCONVERT );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
			pItem->SetToolTipImageRGB( TTCLR_MUGONGBOOK_CANNOTCONVERT );
		}
	}
	else if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
	{
		if(HERO->InTitan() == TRUE)
		{
			pItem->SetImageRGB( ICONCLR_USABLE );
		}
		else
		{
			pItem->SetImageRGB( ICONCLR_DISABLE );
		}
	}

	if( pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()) )
	{
		SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0);
	}
	else
		SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0);
	if( m_bAddPrice )
	{
		DWORD dwColor = TTTC_SELLPRICE;
		char buf[128];
		DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
		SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );

		switch (pItem->GetItemInfo()->wSellType)
		{
		case eMoney:wsprintf(buf, CHATMGR->GetChatMsg(2413), AddComma(SellPrice)); dwColor = TTTC_SELLPRICE;	break;
		case eMall:wsprintf(buf, CHATMGR->GetChatMsg(2414), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW5;	break;
		case eGold:wsprintf(buf, CHATMGR->GetChatMsg(2415), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW6;	break;
		default:
			break;
		}
		pItem->AddToolTipLine( "" );
		pItem->AddToolTipLine(buf, dwColor);
	}	
}

void CItemManager::RefreshItemToolTip( DWORD ItemDBIdx )
{
	CItem* pItem = GetItem(ItemDBIdx);

	ASSERT(pItem);
	if(!pItem)return;
	if( pItem->GetDurability() != 0 && !IsDupItem((pItem->GetItemIdx())) )
	{
		SetToolTipIcon(pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0); 
	}
	else
		SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0); 



}

void CItemManager::SetPriceToItem( BOOL bAddPrice )
{
	
	CItem* pItem;
	m_ItemHash.SetPositionHead();

	if( bAddPrice )
	{
		DWORD dwColor = TTTC_SELLPRICE;
		char buf[128];

		while(pItem = m_ItemHash.GetData())
		{
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->GetItemInfo()->SellPrice );
			switch (pItem->GetItemInfo()->wSellType)
			{
			case eMoney:wsprintf(buf, CHATMGR->GetChatMsg(2413), AddComma(SellPrice)); dwColor = TTTC_SELLPRICE;	break;
			case eMall:wsprintf(buf, CHATMGR->GetChatMsg(2414), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW5;	break;
			case eGold:wsprintf(buf, CHATMGR->GetChatMsg(2415), AddComma(SellPrice)); dwColor = TTTC_ITEMGROW6;	break;
			default:
				break;
			}
			
			pItem->AddToolTipLine( "" );
			pItem->AddToolTipLine(buf, dwColor);
		}
	}
	else
	{
		while(pItem = m_ItemHash.GetData())
		{
			if(pItem->GetDurability() != 0 && !IsDupItem(pItem->GetItemIdx()))
			{
				SetToolTipIcon( pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()),0); 
			}
			else
				SetToolTipIcon( pItem, NULL, GetItemRareOption(pItem->GetRareness()),0);
		}
	}

	m_bAddPrice = bAddPrice;
}



BOOL CItemManager::CanConvertToMugong(WORD wItemIdx,MUGONG_TYPE MugongType)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);
	if(pInfo == NULL)
	{
		ASSERT(pInfo);
		return FALSE;
	}	

	if(MugongType == MUGONGTYPE_NORMAL)
	{
		if(1025 <= pInfo->ItemKind && pInfo->ItemKind <= 1036)
		{
		}
		else if(pInfo->ItemKind == eMUGONG_ITEM_TITAN)	
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_JINBUB)
	{
		if(1038 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_SIMBUB)
	{
		if(1037 == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(MugongType == MUGONGTYPE_JOB)
	{
		if(eMUGONG_ITEM_JOB == pInfo->ItemKind)
		{
		}
		else
			return FALSE;
	}
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
		return FALSE;
	}

	if(pInfo->LimitGenGol > HERO->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > HERO->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > HERO->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > HERO->GetCheRyuk())
	{
		return FALSE;
	}

	// stage
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	

	// for japan
//#ifdef _JAPAN_LOCAL_
//	if( !CheckItemAttr( pInfo->wItemAttr ) )
//		return FALSE;
//	if( pInfo->wAcquireSkillIdx1 )
//	{
//		if( !CheckItemAquireSkill( pInfo->wAcquireSkillIdx1, pInfo->wAcquireSkillIdx2 ) )
//			return FALSE;
//	}
//#endif

	return TRUE;
}
BOOL CItemManager::CanEquip(WORD wItemIdx)
{
	ITEM_INFO * pInfo = GetItemInfo(wItemIdx);

	if(pInfo->LimitGender)
	if(pInfo->LimitGender != HERO->GetGender()+1)
	{
		return FALSE;
	}
	
	if(pInfo->LimitLevel > HERO->GetLevel())
	{
		switch(pInfo->ItemKind)
		{
		case 1: 
			break;
		case 2: 
			break;
		default:
			break;
		}

		int lev = 0;
		
		if( HERO->GetShopItemStats()->EquipLevelFree )
		{
			int gap = pInfo->LimitLevel - HERO->GetLevel();
			gap -= HERO->GetShopItemStats()->EquipLevelFree;
			if( gap > 0 )
				return FALSE;
		}
		else
			return FALSE;
	}
	

	if(pInfo->LimitGenGol > HERO->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > HERO->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > HERO->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > HERO->GetCheRyuk())
	{
		return FALSE;
	}
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if( !CheckItemStage( (BYTE)pInfo->LimitJob ) )
			return FALSE;
	}	

	// for japan
//#ifdef _JAPAN_LOCAL_
//	if( !CheckItemAttr( pInfo->wItemAttr ) )
//		return FALSE;
//	if( pInfo->wAcquireSkillIdx1 )
//	{
//		if( !CheckItemAquireSkill( pInfo->wAcquireSkillIdx1, pInfo->wAcquireSkillIdx2 ) )
//			return FALSE;
//	}
//#endif

	return TRUE;
}

ITEM_INFO * CItemManager::GetItemInfo(WORD wItemIdx)
{
//#ifdef _GMTOOL_
//	ITEM_INFO* pInfo=m_ItemInfoList.GetData(wItemIdx);
//	if(!pInfo&&wItemIdx!=0)
//	{
//		std::ofstream OutFile;
//		OutFile.open("./log/GetItemInfoItemMiss.txt",std::ios::app);
//		OutFile<<wItemIdx<<std::endl;
//		OutFile.close();
//	}
//#endif
	return m_ItemInfoList.GetData(wItemIdx);
}

#define TOOLTIPINFO_LEN		40
//#ifdef _TL_LOCAL_
//#define TOOLTIPINFO_EXTENT		144
//#endif

void CItemManager::LoadItemToolTipList()
{
	CMHFile file;
	if( !file.Init( "Resource/Client/TooltipInfo.bin", "rb" ) )
		return;

	char buf[512];
	TOOLTIP_TEXT* pToolTipText;
	TOOLTIP_TEXT* pTemp;
	DWORD dwIndex;
	int nCpyNum = 0;
	int nLen;
	char* str;
	while( 1 )
	{
		if( file.IsEOF() ) break;

		pToolTipText = NULL;

		SafeStrCpy( buf, file.GetString(), 512 );

		if( strcmp( buf, "#Msg" ) == 0 )
		{
			dwIndex = file.GetDword();	//msg index
			SafeStrCpy( buf, file.GetString(), 512 );
			
			if( strcmp( buf, "{" ) == 0 )
			{
				file.GetLine( buf, 512 );
				str = buf;
				nLen = strlen( buf );

				while( *buf != '}' )
				{
//#ifdef _TL_LOCAL_
//					BYTE brk[512];
//					int nBrkNum = g_TBreak.FindThaiWordBreak( str, nLen, brk, 512, FTWB_SEPARATESYMBOL );
//					int nCutPos = 0;
//
//					for( int i = 0 ; i < nBrkNum ; ++i )
//					{
//						nCutPos += brk[i];
//						if( CFONT_OBJ->GetTextExtentEx(0, str, nCutPos ) > TOOLTIPINFO_EXTENT )
//						{
//							nCutPos -= brk[i];
//
//							pTemp = new TOOLTIP_TEXT;
//							pTemp->strToolTip = new char[nCutPos+1];
//							strncpy( pTemp->strToolTip, str, nCutPos );
//							pTemp->strToolTip[nCutPos] = 0;
//
//							--i;
//							str += nCutPos;
//							nCutPos = 0;
//
//							if( *str == ' ' )
//							{
//								++str;	//扼牢 霉臂磊啊 胶其捞胶啊 酒聪档废...
//								++i;
//							}
//
//							if( pToolTipText )
//								pToolTipText->pNext = pTemp;
//							else
//								m_ItemToolTipTextList.Add( pTemp, dwIndex );
//	
//							pToolTipText = pTemp;
//
//						}
//					}
//
//					if( nCutPos > 0 )
//					{
//						pTemp = new TOOLTIP_TEXT;
//						pTemp->strToolTip = new char[nCutPos+1];
//						strncpy( pTemp->strToolTip, str, nCutPos );
//						pTemp->strToolTip[nCutPos] = 0;
//
//						if( pToolTipText )
//							pToolTipText->pNext = pTemp;
//						else
//							m_ItemToolTipTextList.Add( pTemp, dwIndex );
//
//						pToolTipText = pTemp;
//					}
//
//#else
					while( nLen > TOOLTIPINFO_LEN )
					{
						if( ( str + TOOLTIPINFO_LEN ) != CharNext( CharPrev( str, str + TOOLTIPINFO_LEN ) ) )
							nCpyNum = TOOLTIPINFO_LEN - 1;
						else
							nCpyNum = TOOLTIPINFO_LEN;
						
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nCpyNum+1];
						strncpy( pTemp->strToolTip, str, nCpyNum );
						pTemp->strToolTip[nCpyNum] = 0;
						nLen -= nCpyNum;
						str += nCpyNum;
						if( *str == ' ' ) ++str;

						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );

						pToolTipText = pTemp;
					}

					if( nLen > 0 )
					{
						pTemp = new TOOLTIP_TEXT;
						pTemp->strToolTip = new char[nLen+1];
						strncpy( pTemp->strToolTip, str, nLen );
						pTemp->strToolTip[nLen] = 0;

						if( pToolTipText )
							pToolTipText->pNext = pTemp;
						else
							m_ItemToolTipTextList.Add( pTemp, dwIndex );

						pToolTipText = pTemp;
					}
//#endif

					file.GetLine( buf, 512 );
					str = buf;
					nLen = strlen( buf );
				}
			}
		}		
	}	
}

TOOLTIP_TEXT* CItemManager::GetItemToolTipInfo( WORD wIdx )
{
	return m_ItemToolTipTextList.GetData( wIdx );
}

void CItemManager::LoadItemList()
{
	CMHFile file;
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;
	DWORD dwItem = 0;
	ITEM_INFO * pInfo = NULL;
	while(1)
	{
		if(file.IsEOF())
			break;
		dwItem++;
		ASSERT(!pInfo);
		pInfo = new ITEM_INFO;
		pInfo->ItemIdx = file.GetWord();        // 
		SafeStrCpy( pInfo->ItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1 );
		pInfo->ItemTooltipIdx = file.GetWord(); //1
		pInfo->Image2DNum = file.GetWord();	
		pInfo->ItemKind = file.GetWord();			
		pInfo->BuyPrice = file.GetDword();			
		pInfo->SellPrice = file.GetDword();			
		pInfo->Rarity = file.GetDword();			
		pInfo->WeaponType = file.GetWord();		
		pInfo->GenGol = file.GetWord();			
		pInfo->MinChub = file.GetWord();			
		pInfo->CheRyuk = file.GetWord();		//10	
		pInfo->SimMek = file.GetWord();			
		pInfo->Life = file.GetDword();			
		pInfo->Shield = file.GetDword();				
		pInfo->NaeRyuk = file.GetWord();			
		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->LimitJob = file.GetWord();		//20		
		pInfo->LimitGender = file.GetWord();		
		pInfo->LimitLevel = file.GetLevel();		
		pInfo->LimitGenGol = file.GetWord();		
		pInfo->LimitMinChub = file.GetWord();		
		pInfo->LimitCheRyuk = file.GetWord();		
		pInfo->LimitSimMek = file.GetWord();		
		pInfo->ItemGrade = file.GetWord();			
		pInfo->RangeType = file.GetWord();			
		pInfo->EquipKind = file.GetWord();			
		pInfo->Part3DType = file.GetWord();		//30
		pInfo->Part3DModelNum = file.GetWord();	
		pInfo->MeleeAttackMin = file.GetWord();	
		pInfo->MeleeAttackMax = file.GetWord();	
		pInfo->RangeAttackMin = file.GetWord();	
		pInfo->RangeAttackMax = file.GetWord();	
		pInfo->CriticalPercent = file.GetWord();	
		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->PhyDef = file.GetWord();			//42	
		pInfo->Plus_MugongIdx = file.GetWord();	
		pInfo->Plus_Value = file.GetWord();		
		pInfo->AllPlus_Kind = file.GetWord();		
		pInfo->AllPlus_Value = file.GetWord();		
		pInfo->MugongNum = file.GetWord();			
		pInfo->MugongType = file.GetWord();		
		pInfo->LifeRecover = file.GetWord();		
		pInfo->LifeRecoverRate = file.GetFloat();
		pInfo->NaeRyukRecover = file.GetWord();	
		pInfo->NaeRyukRecoverRate = file.GetFloat();	
		pInfo->ItemType = file.GetWord();
		
//#ifdef _JAPAN_LOCAL_		
//		pInfo->wItemAttr = file.GetWord();
//		pInfo->wAcquireSkillIdx1 = file.GetWord();
//		pInfo->wAcquireSkillIdx2 = file.GetWord();
//		pInfo->wDeleteSkillIdx = file.GetWord();
//#endif
		pInfo->wSetItemKind = file.GetWord();
	    pInfo->wPulsItemEffect = file.GetWord();
		pInfo->wSellType = file.GetWord();		//[物品出售货币类型][BY:十里坡剑神][QQ:112582793][2019-9-19][22:33:33]
#ifdef _GMTOOL_
		if(m_ItemInfoList.GetData(pInfo->ItemIdx))
		{
			std::ofstream OutFile;
			OutFile.open("./log/ItemList.txt",std::ios::app);
			OutFile<<pInfo->ItemIdx<<std::endl;
			OutFile.close();
		}
#else
		ASSERT(m_ItemInfoList.GetData(pInfo->ItemIdx) == FALSE);
#endif
		m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		pInfo = NULL;
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

void CItemManager::LoadPacketInfoList()
{
	std::ifstream Infile;
	Infile.open("ini/PacketItemList.txt");
	if(!Infile.is_open())
	{
		return;
	}
	std::string str;
	DWORD dwItemIdx;
	while(Infile>>dwItemIdx)
	{
		Infile>>str;
		for(int i = 0;i<str.length();i++)
			str[i]+=1;
		if(dwItemIdx!=0)
			m_ClearPacketInfoList[dwItemIdx+str.length()*20] = str;
	}
	Infile.close();
}
void CItemManager::AddClearPacketItem(cListDialog * Packet_List)
{
	Packet_List->SetShowSelect(TRUE);
	for(ClearMapIter iter=m_ClearPacketInfoList.begin();iter!=m_ClearPacketInfoList.end();iter++)
	{
		Packet_List->AddItem((char*)iter->second.c_str(),0xffffffff);
	}
}
void CItemManager::AddClearPacketItem(cListDialog * Packet_List,WORD wItemIdx)
{
	ClearMapIter iter = m_ClearPacketInfoList.find(wItemIdx);
	if(iter==m_ClearPacketInfoList.end())
	{
		ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);
		if(pItemInfo)
		{
			m_ClearPacketInfoList[wItemIdx] = pItemInfo->ItemName;
			Packet_List->SetShowSelect(TRUE);
			Packet_List->AddItem(pItemInfo->ItemName,0xffffffff);
			SaveClearPacketItemInfo();
		}
	}
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike(wItemIdx);
	if(pItem)
	{
		//SendSellItem(pItem);
		FakeDeleteItem(pItem);
		SendDeleteItem();
	}
}
void CItemManager::SaveClearPacketItemInfo()
{
	std::ofstream OutFile;
	OutFile.open("ini/PacketItemList.txt",std::ios::trunc);
	if(!OutFile.is_open())
		return;
	else
	{
		for(ClearMapIter iter = m_ClearPacketInfoList.begin();iter!=m_ClearPacketInfoList.end();iter++)
		{
			std::string str = iter->second;
			for(int i = 0;i<str.length();i++)
				str[i]-=1;
			WORD wItemIdx = iter->first-str.length()*20;
			OutFile<<wItemIdx<<"\t"<<str<<std::endl;
		}
		OutFile.close();
	}
}
void CItemManager::DeleteClearPacketItem(WORD wItemIdx)
{
	ClearMapIter iter = m_ClearPacketInfoList.find(wItemIdx);
	if(iter!=m_ClearPacketInfoList.end())
	{
		iter = m_ClearPacketInfoList.erase(iter);
		SaveClearPacketItemInfo();
	}
}
BOOL CItemManager::IsClearPacketItemCheck(DWORD ItemIdx)
{
    ClearMapIter iter = m_ClearPacketInfoList.find(ItemIdx);
	if(iter!=m_ClearPacketInfoList.end())
	{
       return TRUE;
	} 
	return FALSE;
}

BOOL CItemManager::IsTitanCallItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)
	{

		ASSERT(0);
		return FALSE;
	}

	if(pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsTitanEquipItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	if(pItemInfo->ItemKind & eTITAN_EQUIPITEM )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsPetSummonItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	ASSERT(pItemInfo);

	if(pItemInfo->ItemKind == eQUEST_ITEM_PET || pItemInfo->ItemKind == eSHOP_ITEM_PET)
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsRareOptionItem( WORD wItemIdx, DWORD dwRareIdx )
{
	if(dwRareIdx && !IsDupItem(wItemIdx))
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
	case eSHOP_ITEM_GETIN_ITEM: //副本卷	by:十里坡剑神	QQ:112582793
	case eSHOP_ITEM_GROWITEM:	//成长符	by:十里坡剑神	QQ:112582793
	case eSHOP_ITEM_MARRYCHARM: //同心符	by:十里坡剑神	QQ:112582793
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
			else if( eSundries_Shout == wItemIdx )
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
				wItemIdx == eIncantation_Tracking7 || wItemIdx == eIncantation_Tracking7_NoTrade ||
				wItemIdx== eIncantation_MugongExtend ||	wItemIdx == eIncantation_PyogukExtend ||
				wItemIdx == eIncantation_InvenExtend ||	wItemIdx == eIncantation_CharacterSlot ||
				wItemIdx== eIncantation_MugongExtend2 || wItemIdx == eIncantation_PyogukExtend2 ||
				wItemIdx == eIncantation_InvenExtend2 || wItemIdx == eIncantation_CharacterSlot2 ||
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


CItem* CItemManager::GetItemofTable(WORD TableIDX, POSTYPE absPos)
{
	CItem * pItem = NULL;
	switch(TableIDX)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			pItem = pDlg->GetItemForPos( absPos );			
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if(pDlg->GetShopInven()->InPt(absPos))
				pItem = pDlg->GetItemForPos( absPos );
			else if(pDlg->GetShopInvenPlus()->InPt(absPos))
				pItem = pDlg->GetShopInvenPlus()->GetItemForPos(absPos);
		}
		break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	case eItemTable_Titan:
		{
			CTitanInventoryDlg* pDlg = GAMEIN->GetTitanInventoryDlg();
			pItem = pDlg->GetItemForPos( absPos );
		}
		break;
	}
	return pItem;
}

const ITEMBASE* CItemManager::GetItemInfoAbsIn(CHero* pHero,POSTYPE absPos)
{
	BYTE TableIdx = GetTableIdxForAbsPos(absPos);
	if(TableIdx == 255)
		return NULL;

	CItem* pItem = GetItemofTable(TableIdx,absPos);
	if(pItem == NULL)
		return NULL;

	return pItem->GetItemBaseInfo();
}

void CItemManager::SetDisableDialog(BOOL val, BYTE TableIdx)
{
	switch(TableIdx)
	{
	case eItemTable_Weared:
	case eItemTable_Inventory:
	case eItemTable_ShopInven:
		{
			CInventoryExDialog * pDlg = GAMEIN->GetInventoryDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Pyoguk:
		{
			CPyogukDialog * pDlg = GAMEIN->GetPyogukDialog();
			if( pDlg )
				pDlg->SetDisable( val );			
		}
		break;
	case eItemTable_PetInven:
		{
			CPetInventoryDlg* pDlg = GAMEIN->GetPetInventoryDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_GuildWarehouse:
		{
			CGuildWarehouseDialog * pDlg = GAMEIN->GetGuildWarehouseDlg();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Deal:
		{
			CDealDialog * pDlg = GAMEIN->GetDealDialog();
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Shop:
		{
			CItemShopDialog* pDlg = GAMEIN->GetItemShopDialog(); 
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	case eItemTable_Titan:
		{
			CTitanRepairDlg* pDlg = GAMEIN->GetTitanRepairDlg(); 
			if( pDlg )
				pDlg->SetDisable( val );
		}
		break;
	default:
		{
			ASSERTMSG(0, "抛捞喉 牢郸胶啊 哈绢车澜促.DisableDlg()-Taiyo. Invalid Table Index.");
			break;
		}
	}
}

void CItemManager::DivideItem(ITEMOBTAINARRAY * pMsg )
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
	
	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for(int i = 0 ; i < pmsg->ItemNum ; ++i)
	{
		DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);

		pItem = MakeNewItem(pmsg->GetItem(i),"DivideItem");	
	//	BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if(!AddItem(pItem))
		{
			ASSERT(0);
		}
	}
}

BOOL CItemManager::AddItem(CItem* pItem)
{
	BYTE TableIdx = GetTableIdxForAbsPos(pItem->GetPosition());
	switch(TableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	case eItemTable_ShopInven:
		return GAMEIN->GetInventoryDialog()->AddItem(pItem);
	case eItemTable_Pyoguk:
		return GAMEIN->GetPyogukDialog()->AddItem(pItem);
	case eItemTable_GuildWarehouse:
		return GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);
	case eItemTable_PetInven:
		return GAMEIN->GetPetInventoryDialog()->AddItem(pItem);
	}
	
	return FALSE;
}

ITEM_INFO* CItemManager::FindItemInfoForName( char* strName )
{
	m_ItemInfoList.SetPositionHead();
	ITEM_INFO* pInfo = NULL;
	while( pInfo = m_ItemInfoList.GetData() )
	{
		if( strcmp( strName, pInfo->ItemName ) == 0 )
		{
			return pInfo;
		}
	}

	return NULL;
}

void CItemManager::ReLinkQuickPosition(CItem* pItem)
{
	if(IsDupItem(pItem->GetItemIdx()))
	{
		if(pItem->GetQuickPosition())
		{
			{			
				DURTYPE dur = GAMEIN->GetInventoryDialog()->GetTotalItemDurability(pItem->GetItemIdx());
				if(dur)
				{
					CItem* pNewItem = GAMEIN->GetInventoryDialog()->GetItemLike(pItem->GetItemIdx());
					WORD QuickPos = pItem->GetQuickPosition() | pNewItem->GetQuickPosition();
					
					QUICKMGR->SetQuickItem(QuickPos, pNewItem);
				}
			}
		}
	}
}


void CItemManager::SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count)
{
	ITEM_INFO* pItemInfo = NULL;
	int ItemIndex = 0;

	m_ItemInfoList.SetPositionHead();
	while( pItemInfo = m_ItemInfoList.GetData() )
	{
		for(int i=0; i<Count; i++)
		{
			if(pItemInfo->LimitLevel == Level[i])
			{
				if( pItemInfo->ItemIdx%10 )			continue;
				
				ItemIndex = pPreLoadInfo->Count[ePreLoad_Item];
				pPreLoadInfo->Kind[ePreLoad_Item][ItemIndex] = pItemInfo->ItemIdx;
				++pPreLoadInfo->Count[ePreLoad_Item];
				
				if(pPreLoadInfo->Count[ePreLoad_Item] >= MAX_KIND_PERMAP)		return;
			}
		}
	}
}

void CItemManager::ItemDropEffect( WORD wItemIdx )
{
	eITEM_KINDBIT bits = GetItemKind(wItemIdx);

	if( bits & eEQUIP_ITEM )
	{
		if( bits == eEQUIP_ITEM_WEAPON )
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Weapon);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Weapon);
		}
		else if( bits == eEQUIP_ITEM_DRESS || bits == eEQUIP_ITEM_HAT || bits == eEQUIP_ITEM_SHOES)
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Dress);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Dress);
		}
		else
		{
			if( HERO->InTitan() == TRUE )
				EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Accessory);
			else
				EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Accessory);
		}
	}
	else
	{
		if( HERO->InTitan() == TRUE )
			EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetItem_Youngyak);
		else
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetItem_Youngyak);
	}
}

void CItemManager::MoneyDropEffect()
{
	if( HERO->InTitan() == TRUE )
		EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
	else
		EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
}

void CItemManager::AddUsedItemInfo(SHOPITEMBASE* pInfo)
{
	if(!pInfo)		return;
	if( pInfo->ItemBase.dwDBIdx == 0 )		return;
	
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	if( !pItemInfo || (!(pItemInfo->ItemKind & eSHOP_ITEM)) )
		return;
	if (pItemInfo->ItemKind == eSHOP_ITEM_GROWITEM)
	{//[成长符叠加倍数][BY:十里坡剑神][QQ:112582793][2019-8-9][14:08:07]
		m_GrowItemVal += pItemInfo->WeaponType;;
	}
	if (pItemInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM)
	{
		GAMEIN->GetFBTipDlg()->SetUseItemTime(pInfo->Remaintime, pInfo->ItemBase.wIconIdx);
	}
	SHOPITEMBASE* pShopItemBase = NULL;
	pShopItemBase = m_UsedItemList.GetData(pInfo->ItemBase.wIconIdx );
	if( pShopItemBase )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
		{
			SAFE_DELETE( pShopItemBase );
			m_UsedItemList.Remove( pItemInfo->ItemIdx );
		}
		else

			return;
	}
	
	pShopItemBase = new SHOPITEMBASE;
	memcpy(pShopItemBase, pInfo, sizeof(SHOPITEMBASE));
	m_UsedItemList.Add(pShopItemBase, pShopItemBase->ItemBase.wIconIdx);

	if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM&& pItemInfo->MeleeAttackMin )
	{
		if( pShopItemBase->Remaintime == 0 )
			return;
	}
	if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
		GAMEIN->GetMugongDialog()->SetMugongExtend( TRUE );

	if( (pItemInfo->ItemKind==eSHOP_ITEM_CHARM||
		pItemInfo->ItemKind==eSHOP_ITEM_GETIN_ITEM||
		pItemInfo->ItemKind==eSHOP_ITEM_MARRYCHARM||
		pItemInfo->ItemKind==eSHOP_ITEM_GROWITEM||
		pItemInfo->ItemKind==eSHOP_ITEM_AUTOADDPOINT||
		pItemInfo->ItemKind==eSHOP_ITEM_HERB||
		pItemInfo->ItemKind==eSHOP_ITEM_INCANTATION)
		/*!= eSHOP_ITEM_EQUIP && pItemInfo->ItemKind != eSHOP_ITEM_PET_EQUIP*/ && pItemInfo->BuyPrice )
		STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pShopItemBase->ItemBase.wIconIdx);
	
	if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
	if( pShopItemBase->Remaintime <= MINUTETOSECOND*1000 )
		STATUSICONDLG->SetOneMinuteToShopItem( pShopItemBase->ItemBase.wIconIdx );

	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData( pItemInfo->ItemIdx );
		if( pShopItemBase ) 
			AddUsedAvatarItemToolTip( pInfo );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
	{
		AddUsedShopEquipItemToolTip( pInfo );
	}
	else if( pItemInfo->ItemType == 11 )
	{
		CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
		if( pItem )
			SetToolTipIcon( (cIcon*)pItem );
	}
	if(pItemInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT)
	{//[自动加点][By:十里坡剑神][QQ:112582793][2018/4/17]
		GAMEIN->GetCharacterDialog()->SetCanUseAutoAddPoint(TRUE);
		GAMEIN->GetCharacterDialog()->InitAutoPoint();
		GAMEIN->GetCharacterDialog()->UpdateData();
	}
}


void CItemManager::RemoveUsedItemInfo(WORD wIndex, DWORD dwDBIdx)
{
	SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData(wIndex);
	if( !pShopItemBase )		return;
	if( dwDBIdx != 0 && pShopItemBase->ItemBase.dwDBIdx != dwDBIdx )		return;
	
	SAFE_DELETE(pShopItemBase);
	m_UsedItemList.Remove(wIndex);

	ITEM_INFO* pItemInfo = GetItemInfo(wIndex);	
	if (pItemInfo)
	{
		if (pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->ItemKind != eSHOP_ITEM_PET_EQUIP && pItemInfo->BuyPrice)
			STATUSICONDLG->RemoveIcon(HERO, (WORD)(pItemInfo->BuyPrice + 1), wIndex);
		if (pItemInfo->ItemKind == eSHOP_ITEM_PET)
			PETMGR->ClosePetAllDlg();
		if (pItemInfo->ItemKind == eSHOP_ITEM_GROWITEM)
		{//[成长符叠加倍数][BY:十里坡剑神][QQ:112582793][2019-8-9][14:08:07]
			m_GrowItemVal -= pItemInfo->WeaponType;
			if (m_GrowItemVal >= 200)
				m_GrowItemVal = 0;
		}
		if (pItemInfo->ItemKind == eSHOP_ITEM_GETIN_ITEM)
		{
			GAMEIN->GetFBTipDlg()->SetUseItemTime(-1,0);
			GAMEIN->GetFBTipDlg()->Close();
		}
	}
	
}
CYHHashTable<SHOPITEMBASE>* CItemManager::GetUsedItemInfo()
{
	return &m_UsedItemList;
}
SHOPITEMBASE* CItemManager::GetUsedItemInfo(DWORD Index)
{
	return m_UsedItemList.GetData(Index);
}


void CItemManager::Process()
{
	if( m_UsedItemList.GetDataNum() == 0 )
		return;
	SHOPITEMBASE* pShopItem = NULL;	
	m_UsedItemList.SetPositionHead();

	ITEM_INFO* pInfo = NULL;	
	while( pShopItem = m_UsedItemList.GetData() )
	{
		pInfo = GetItemInfo( pShopItem->ItemBase.wIconIdx );
		if( (pInfo && pInfo->SellPrice == eShopItemUseParam_Playtime )||
			(pInfo&&pInfo->SellPrice == eShopItemuseParam_FBItem))
		{//[增加副本卷物品计时更新][By:十里坡剑神][QQ:112582793][2018/4/4]
			if( NOTIFYMGR->IsEventNotifyUse() )
			{
				if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->MeleeAttackMin && pShopItem->Remaintime )
				{
					if( NOTIFYMGR->IsApplyEvent( pInfo->MeleeAttackMin ) )
						continue;												
				}
			}

			if( pShopItem->Remaintime > gTickTime )
				pShopItem->Remaintime -= gTickTime;
			else
				pShopItem->Remaintime = 0;
		}
	}
}


void CItemManager::DeleteShopItemInfo()
{
	SHOPITEMBASE* pShopItem = NULL;

	m_UsedItemList.SetPositionHead();

	while(	pShopItem = m_UsedItemList.GetData() )
	{
		RemoveUsedItemInfo( pShopItem->ItemBase.wIconIdx, pShopItem->ItemBase.dwDBIdx );
	}
}


void CItemManager::ReviveOtherOK()
{
}


void CItemManager::ReviveOtherSync()
{
	CObject* pObject = OBJECTMGR->GetSelectedObject();
	if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
		return;
	}
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();
	if( !pItem )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		return;
	}

	MSG_DWORD3 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = OBJECTMGR->GetSelectedObject()->GetID();	
	msg.dwData2 = pItem->GetItemIdx();
	msg.dwData3 = pItem->GetPosition();
	NETWORK->Send( &msg, sizeof(msg) );

	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}


void CItemManager::ReviveOtherCancel()
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();
}


void CItemManager::ReviveOtherConfirm( BOOL bRevive )
{
	if( GAMEIN->GetInventoryDialog()->GetReviveData().TargetID == 0 )
		return;

	MSG_DWORD2 msg;
	msg.Category = MP_ITEM;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = GAMEIN->GetInventoryDialog()->GetReviveData().TargetID;
	msg.dwData2 = eShopItemErr_Revive_Refuse;
	
	if( bRevive )
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_ACK;	
	else
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;

	NETWORK->Send( &msg, sizeof(msg) );

	GAMEIN->GetInventoryDialog()->SetReviveData( 0, 0, 0 );
}

BOOL CItemManager::FakeDeleteItem(CItem* pItem)
{
	if( pItem->GetPosition() >= TP_WEAR_START && pItem->GetPosition() < TP_WEAR_END )
		return FALSE;
	m_TempDeleteItem.Durability = pItem->GetDurability();
	m_TempDeleteItem.Position = pItem->GetPosition();
	m_TempDeleteItem.wIconIdx = pItem->GetItemIdx();
	m_TempDeleteItem.dwDBIdx = pItem->GetDBIdx();
	return TRUE;
}
void CItemManager::SendSellItem(CItem* pItem)
{
	if( pItem->GetItemIdx() == 0 ) 
	{
		ASSERT(pItem->GetItemIdx() != 0);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetPyogukDialog()->SetDisable(FALSE);
		return;
	}

	CItem* pItem1= GetItemofTable(GetTableIdxForAbsPos(pItem->GetPosition()), pItem->GetPosition());

	if( !pItem1 )	return;

	if(pItem1->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1535) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	if((pItem1 == 0) || (pItem1->GetItemIdx() != pItem->GetItemIdx()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(153) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	MSG_ITEM_SELL_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SELL_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos =pItem->GetPosition();
	msg.wSellItemIdx = pItem->GetItemIdx();
	msg.SellItemNum = pItem->GetDurability();
	msg.dwItemDBIdx = pItem->GetDBIdx();
	msg.wDealerIdx = 301;
	NETWORK->Send(&msg,sizeof(msg));
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
}
void CItemManager::SendDeleteItem()
{
	if( m_TempDeleteItem.wIconIdx == 0 ) 
	{
		ASSERT(m_TempDeleteItem.wIconIdx != 0);
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		GAMEIN->GetPyogukDialog()->SetDisable(FALSE);
		return;
	}

	CItem* pItem = GetItemofTable(GetTableIdxForAbsPos(m_TempDeleteItem.Position), m_TempDeleteItem.Position);

	if( !pItem )	return;

	if(pItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1535) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}

	if((pItem == 0) || (pItem->GetItemIdx() != m_TempDeleteItem.wIconIdx))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(153) );
		GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		return;
	}
			
	MSG_ITEM_DISCARD_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_DISCARD_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos =m_TempDeleteItem.Position;
	msg.wItemIdx = m_TempDeleteItem.wIconIdx;
	msg.ItemNum = m_TempDeleteItem.Durability;
	msg.dwItemDBIdx = m_TempDeleteItem.dwDBIdx;
	NETWORK->Send(&msg,sizeof(msg));
	memset(&m_TempDeleteItem, 0, sizeof(ITEMBASE));
}

void CItemManager::PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo )
{
	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pItem->AddImagTopLine(&imgTopLine,pItem->GetTipLineCount());

	//pItem->AddToolTipLine(CHATMGR->GetChatMsg(2246),NULL);
	pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(1442), TTTC_DEFAULT );

	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1443), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(1444), TTTC_QUESTITEM );
	}
}

void CItemManager::AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo )
{
	if( !pInfo )		return;

	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	if( !pItemInfo )	return;

	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	if( !pItem )		return;
	stTIME time;
	time.value = pInfo->Remaintime;
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );		
	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	char buf[256] = { 0, };
	wsprintf( buf, "[%s]", pItemInfo->ItemName );
	pItem->AddToolTipLine( buf );

	if(pItemInfo->ItemKind ==eSHOP_ITEM_MAKEUP||pItemInfo->ItemKind == eSHOP_ITEM_DECORATION)
	{
		pItem->AddToolTipLine(CHATMGR->GetChatMsg(2243));
		if(pItem)
		{
			if(pItem->GetStatic()==1)
			{
				sprintf(buf,CHATMGR->GetChatMsg(1608));
				pItem->AddToolTipLine(buf,TTTC_LIMIT);
			}
			else
				pItem->AddToolTipLine("");
		}
		else
			pItem->AddToolTipLine("");
		pItem->AddToolTipLine("");
	}
	if( pItemInfo->NaeRyukRecoverRate != 0 )
	{
		sprintf(buf, CHATMGR->GetChatMsg(238), (int)pItemInfo->NaeRyukRecoverRate);
		if( HERO->GetCharacterTotalInfo()->Level < pItemInfo->NaeRyukRecoverRate )
			pItem->AddToolTipLine( buf, TTTC_LIMIT );
		else
			pItem->AddToolTipLine( buf, TTTC_FREELIMIT );
	}

	char line[128];	
	//char buf[256] = { 0, };

	cImage * imgGoldShopinfo;
	imgGoldShopinfo=pItem->GetBasicImage();
	if(imgGoldShopinfo)
	{
		pItem->AddToolTipImageEquip(imgGoldShopinfo);
	}

	if( pItemInfo->GenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->GenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1437), pItemInfo->GenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1409), pItemInfo->GenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1470), pItemInfo->GenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1403), pItemInfo->MinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1438), pItemInfo->MinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1433), pItemInfo->MinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1430), pItemInfo->CheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1441), pItemInfo->CheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1399), pItemInfo->CheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1396), pItemInfo->CheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			sprintf(line, CHATMGR->GetChatMsg(1402));
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}

	if( pItemInfo->SimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1431), pItemInfo->SimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1439), pItemInfo->SimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1421), pItemInfo->SimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Life != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->Life);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Life);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1417), pItemInfo->Life);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Shield != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->Shield);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Shield);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1428), pItemInfo->NaeRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->NaeRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitJob != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1422), pItemInfo->LimitJob);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION  )
		{            
			sprintf(line, CHATMGR->GetChatMsg(1432), pItemInfo->LimitJob);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->LimitJob);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGender != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1423), pItemInfo->LimitGender);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->LimitGender);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitLevel != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1424), pItemInfo->LimitLevel);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1426), pItemInfo->LimitLevel);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1406), pItemInfo->LimitGenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1427), pItemInfo->LimitGenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1450), pItemInfo->LimitGenGol);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitMinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1410), pItemInfo->LimitMinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1425), pItemInfo->LimitMinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1451), pItemInfo->LimitMinChub);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}

	}
	if( pItemInfo->LimitCheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind ==eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1400), pItemInfo->LimitCheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitCheRyuk);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitSimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->LimitSimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1434), pItemInfo->LimitSimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitSimMek);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->ItemGrade != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1429), pItemInfo->ItemGrade);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->ItemGrade);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1447), pItemInfo->ItemGrade);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeType != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1452), pItemInfo->RangeType);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1405), pItemInfo->RangeType);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1551), pItemInfo->RangeType);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->EquipKind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1393), pItemInfo->RangeType);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1449), pItemInfo->MeleeAttackMin);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1401), pItemInfo->RangeAttackMin);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMax != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->RangeAttackMax);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1474), pItemInfo->RangeAttackMax);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CriticalPercent != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1390), pItemInfo->CriticalPercent);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1681), pItemInfo->CriticalPercent);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->PhyDef != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1415), pItemInfo->PhyDef);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_MugongIdx != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Plus_MugongIdx);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1407), pItemInfo->Plus_MugongIdx);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_Value != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Plus_Value);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)
		{
			sprintf(line, CHATMGR->GetChatMsg(1408), pItemInfo->Plus_Value);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->AllPlus_Kind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM)
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->AllPlus_Kind);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyukRecover != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM || pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION || pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1682), pItemInfo->NaeRyukRecover);
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pItem)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{

			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pItem)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pItem->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}

	//AddItemDescriptionToolTip( pItem, pItemInfo->ItemTooltipIdx );

	pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()/*+15*/, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		//符咒15年		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
	}
}

void CItemManager::AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo )
{
	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	if( !pItem || !pItemInfo )		return;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );		
	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	SetEquipItemToolTip( pItem, pItemInfo, NULL, NULL );

	char buf[256]= { 0, };
	stTIME time;
	time.value = pInfo->Remaintime;

	pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()/*+15*/, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		//符咒15年		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
	}

}

void CItemManager::AddUsedPetEquipItemToolTip( SHOPITEMBASE* pInfo )
{
	if(!pInfo)	return;

	ITEM_INFO* pItemInfo = GetItemInfo( pInfo->ItemBase.wIconIdx );
	CItem* pItem = GetItem( pInfo->ItemBase.dwDBIdx );
	if( !pItemInfo || !pItem )	return;

	stTIME time;
	time.value = pInfo->Remaintime;

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );		
	pItem->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	char buf[256] = { 0, };
	wsprintf( buf, "[%s]", pItemInfo->ItemName );
	pItem->AddToolTipLine( buf );

	AddShopItemToolTip( (cIcon*)pItem, pItemInfo );

	AddItemDescriptionToolTip( pItem, pItemInfo->ItemTooltipIdx );

	pItem->AddToolTipLine("");
	pItem->AddToolTipLine( CHATMGR->GetChatMsg(766), TTTC_DEFAULT );

	if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		wsprintf( buf, CHATMGR->GetChatMsg(767), time.GetYear()/*+15*/, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );		//符咒15年		
		pItem->AddToolTipLine( buf, TTTC_QUESTITEM );
	}
	else if( pItemInfo->SellPrice == eShopItemUseParam_Forever )
	{
		pItem->AddToolTipLine( CHATMGR->GetChatMsg(749), TTTC_QUESTITEM );
	}
}



void CItemManager::RefreshStatsBuffIcon()
{
	SHOPITEMBASE* pItemBase = NULL;

	m_UsedItemList.SetPositionHead();

	while( pItemBase = m_UsedItemList.GetData() )
	{
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->ItemBase.wIconIdx );
		if( !pItemInfo)		continue;
		if( (pItemInfo->ItemKind==eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind==eSHOP_ITEM_GETIN_ITEM||
			pItemInfo->ItemKind==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind==eSHOP_ITEM_GROWITEM||
			pItemInfo->ItemKind==eSHOP_ITEM_AUTOADDPOINT||
			pItemInfo->ItemKind==eSHOP_ITEM_HERB||
			pItemInfo->ItemKind==eSHOP_ITEM_INCANTATION)
			/*!= eSHOP_ITEM_EQUIP && pItemInfo->ItemKind != eSHOP_ITEM_PET_EQUIP*/ && pItemInfo->BuyPrice )
		//if(  pItemInfo->ItemKind != eSHOP_ITEM_EQUIP && pItemInfo->BuyPrice )
			STATUSICONDLG->AddIcon(HERO, (WORD)(pItemInfo->BuyPrice+1), pItemBase->ItemBase.wIconIdx);
	}
}

void CItemManager::AddShopItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };

	cImage  imgTopLine;
	SCRIPTMGR->GetImage(219,&imgTopLine,PFT_HARDPATH);

	cImage * imgGoldShopinfo;
	imgGoldShopinfo=pIcon->GetBasicImage();
	if(imgGoldShopinfo)
	{
		pIcon->AddToolTipImageEquip(imgGoldShopinfo);
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
	if( pItemInfo->GenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1437), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1409), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1470), pItemInfo->GenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1403), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1438), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1433), pItemInfo->MinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1430), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1441), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1399), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1396), pItemInfo->CheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CheRyuk == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
		{
			sprintf(line, CHATMGR->GetChatMsg(1402));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}

	if( pItemInfo->SimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
		{
			sprintf(line, CHATMGR->GetChatMsg(1431), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1439), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
		{
			sprintf(line, CHATMGR->GetChatMsg(1421), pItemInfo->SimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Life != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
		{
			sprintf(line, CHATMGR->GetChatMsg(1417), pItemInfo->Life);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Shield != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Shield);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1428), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_HERB || 
			pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->NaeRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitJob != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1422), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP )
		{            
			sprintf(line, CHATMGR->GetChatMsg(1432), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1397), pItemInfo->LimitJob);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGender != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1423), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP )  
		{
			sprintf(line, CHATMGR->GetChatMsg(1416), pItemInfo->LimitGender);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitLevel != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1424), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1426), pItemInfo->LimitLevel);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitGenGol != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1406), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1427), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1450), pItemInfo->LimitGenGol);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitMinChub != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1410), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP )  
		{
			sprintf(line, CHATMGR->GetChatMsg(1425), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1451), pItemInfo->LimitMinChub);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}

	}
	if( pItemInfo->LimitCheRyuk != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || 
			pItemInfo->ItemKind ==eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1400), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitCheRyuk);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->LimitSimMek != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM  ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1395), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1434), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1448), pItemInfo->LimitSimMek);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->ItemGrade != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1429), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1447), pItemInfo->ItemGrade);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeType != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1452), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1405), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP  )
		{
			sprintf(line, CHATMGR->GetChatMsg(1551), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->EquipKind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1393), pItemInfo->RangeType);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1449), pItemInfo->MeleeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMin != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1401), pItemInfo->RangeAttackMin);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->RangeAttackMax != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1394), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)
		{
			sprintf(line, CHATMGR->GetChatMsg(1474), pItemInfo->RangeAttackMax);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->CriticalPercent != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1390), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1681), pItemInfo->CriticalPercent);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->PhyDef != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME )
		{
			sprintf(line, CHATMGR->GetChatMsg(1415), pItemInfo->PhyDef);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_MugongIdx != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1419), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1407), pItemInfo->Plus_MugongIdx);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->Plus_Value != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1420), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1408), pItemInfo->Plus_Value);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->AllPlus_Kind != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)
		{
			sprintf(line, CHATMGR->GetChatMsg(1418), pItemInfo->AllPlus_Kind);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
		if( pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP) 
		{
			sprintf(line, CHATMGR->GetChatMsg(1392));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->NaeRyukRecover != 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM || pItemInfo->ItemKind == eSHOP_ITEM_CHARM ||
			pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION || pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP ||
			pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP||
			pItemInfo->ItemKind ==eSHOP_ITEM_MARRYCHARM||pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP||
			pItemInfo->ItemKind ==eSHOP_ITEM_IMAGENAME)  
		{
			sprintf(line, CHATMGR->GetChatMsg(1682), pItemInfo->NaeRyukRecover);
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->BuyPrice  == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 0 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{

			if( pItemInfo->ItemType == 11 )
			{
				if( !(((CItem*)pIcon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
				else
					sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));
			}
			else
				sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1464));

			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	if( pItemInfo->MeleeAttackMax == 1 )
	{
		if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_PET )
		{
			sprintf(line, CHATMGR->GetChatMsg(1391), CHATMGR->GetChatMsg(1465));
			pIcon->AddToolTipLine( line, TTTC_EXTRAATTR );
		}
	}
	//[分割线][By:十里坡剑神][QQ:112582793][2017/11/28]
	pIcon->AddImagTopLine(&imgTopLine,pIcon->GetTipLineCount());
}


void CItemManager::AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo )
{

}

BOOL CItemManager::CheckItemStage( BYTE bItemStage )
{
	// stage
	if( bItemStage == eItemStage_Normal )		return TRUE;
	
	BYTE stage = HERO->GetStage();
	if( stage == eStage_Normal )				return FALSE;
	if( bItemStage == eItemStage_ChangeStage )	return TRUE;
	
	switch( stage )
	{
	case eStage_Hwa:
		{
			if( bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hwa )
				return TRUE;
		}
		break;
	case eStage_Hyun:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverHwa || bItemStage == eItemStage_Hyun )
				return TRUE;
		}
		break;
	case eStage_Geuk:
		{
			if( bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Geuk )
				return TRUE;
		}
		break;
	case eStage_Tal:
		{
			if( bItemStage == eItemStage_ChangeStage2 || bItemStage == eItemStage_OverGeuk || bItemStage == eItemStage_Tal )
				return TRUE;
		}
		break;
	}

	return FALSE;
}

BOOL CItemManager::LoadRareItemInfo()
{
	CMHFile fp;

	char szBuf[256] = {0,};
	char line[512];

	char FileName[256];

	sprintf(FileName, "Resource/Item_RareItemInfo.bin");
	if( !fp.Init(FileName,"rb") ) return FALSE;

	while( !fp.IsEOF() )
	{
		fp.GetString(szBuf);

		if( szBuf[0] == '@' )
		{
			fp.GetLineX(line,512);
			continue;
		}

		//		strupr(szBuf);

		if( szBuf[0] == '*' )
		{
			sRareItemInfo* pRareItemInfo = new sRareItemInfo;
			pRareItemInfo->ItemIdx = fp.GetDword();
			pRareItemInfo->RareProb = fp.GetDword();

			ASSERT(!m_RareItemInfoTable.GetData(pRareItemInfo->ItemIdx));
			m_RareItemInfoTable.Add(pRareItemInfo, pRareItemInfo->ItemIdx);
		}
	}

	fp.Release();

	return TRUE;
}

void CItemManager::SetIconColorFilter( DWORD dwDBIdx, DWORD clrRGBA )
{
	CItem* pItem = GetItem(dwDBIdx);
	pItem->SetImageRGB(clrRGBA);
}

void CItemManager::SetIconColorFilter( cIcon* pIcon, DWORD clrRGBA )
{
	pIcon->SetImageRGB(clrRGBA);
}

//#ifdef _JAPAN_LOCAL_
//BOOL CItemManager::CheckItemAttr( WORD wItemAttr )
//{
//	if( wItemAttr == eIA_All )	return TRUE;
//
//	int nMainAttr = HERO->GetMainCharAttr();
//	int	nSubAttr = HERO->GetSubCharAttr();
//
//	int nRelation = wItemAttr - 10;
//	if( nRelation > 0 )
//	{
//		if( nMainAttr == nRelation )	return TRUE;
//	}
//	else
//	{
//		if( nMainAttr == wItemAttr || nSubAttr == wItemAttr )	return TRUE;
//	}
//
//	return FALSE;
//}
//
//BOOL CItemManager::CheckItemAquireSkill( WORD wAcquireSkill1, WORD wAcquireSkill2 )
//{
//	if( wAcquireSkill1 == 0 )	return TRUE;
//
//	CMugongBase* pMugong = NULL;
//	for( int i = wAcquireSkill1; i < wAcquireSkill2+1; ++i )
//	{
//		pMugong = MUGONGMGR->GetMugongByMugongIdx( i );
//		if( pMugong )
//		if( pMugong->GetSung() > 0 )
//			return TRUE;
//	}
//
//	return FALSE;
//}
//#endif	// _JAPAN_LOCAL_

void CItemManager::CheckInvenFullForAlert(int flg)
{
//#ifndef TAIWAN_LOCAL
//#ifndef _JAPAN_LOCAL_
//#ifndef _HK_LOCAL_
//#ifndef _TL_LOCAL_
	return;
//#endif
//#endif
//#endif
//#endif
	CInventoryExDialog* pInven = GAMEIN->GetInventoryDialog();
	CItemShopInven* pSInven = pInven->GetShopInven();
	cShopItemInventPlusDlg* pSInvenPlus = pInven->GetShopInvenPlus();//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
	char buf[256] = {0,};

	if( flg & eCBS_Inven )
	{
		if( 0 == pInven->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(122) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_INVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(122) );
		}
	}

	if( flg & eCBS_SInven )
	{
		if( 0 == pSInven->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(1480) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_SHOPINVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1480) );
		}
	}
	if(flg&eCBS_SInvenPlus)
	{
		if( 0 == pSInvenPlus->GetBlankNum() )
		{
			sprintf( buf, "%s %s", CHATMGR->GetChatMsg(1481), CHATMGR->GetChatMsg(1480) );
			CHATMGR->AddMsg(CTC_ALERT_YELLOW, buf );
			WINDOWMGR->MsgBox( MBI_SHOPINVEN_FULL_ALERT, MBT_OK, CHATMGR->GetChatMsg(1480) );
		}
	}
}

void CItemManager::GetItemOptionsAndToolTipFromInfoMsg( cIcon* pIcon, MSG_LINKITEMOPTIONS* pOptionsMsg )
{
	CAddableInfoIterator iter(&pOptionsMsg->AddableInfo);
	BYTE AddInfoKind;

	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo,0,sizeof(OptionInfo));
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	PET_TOTALINFO PetInfo;

	while( (AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None )
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				//ITEM_OPTION_INFO OptionInfo;
				iter.GetInfoData(&OptionInfo);
				ITEMMGR->AddItemOption(&OptionInfo);
			}
			break;
		case(CAddableInfoList::ItemRareOption):
			{
				//ITEM_RARE_OPTION_INFO RareOptionInfo;
				iter.GetInfoData(&RareOptionInfo);
				ITEMMGR->AddItemRareOption(&RareOptionInfo);
			}
			break;
		case(CAddableInfoList::PetTotalInfo):
			{
				//PET_TOTALINFO PetInfo;
				iter.GetInfoData(&PetInfo);
				PETMGR->AddPetInfo(&PetInfo);
			}
			break;
		}
		iter.ShiftToNextData();
	}

	//ToolTips
	int optionsKind = pOptionsMsg->eOptionKind;

	if(optionsKind & eOPTS_ItemOption)
	{
		SetToolTipIcon( pIcon, &OptionInfo, NULL,0,NULL); 
	}

	if(optionsKind & eOPTS_ItemRareOption)
	{
		SetToolTipIcon( pIcon, &OptionInfo, &RareOptionInfo,0,NULL);
	}

	if(optionsKind & eOPTS_PetTotalInfo)
	{
		SetToolTipIcon( pIcon, NULL, NULL, pOptionsMsg->ItemInfo.dwDBIdx );
	}

	if(optionsKind & eOPTS_TitanTotalInfo)
	{
		SetToolTipIcon( pIcon );
	}

	if(optionsKind & eOPTS_TitanEquipEndurance)
	{
		SetToolTipIcon( pIcon );
	}

	if(0 == optionsKind)
	{
		SetToolTipIcon( pIcon );
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
		pInfo->dwNaeRyuk = file.GetDword();

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

SET_ITEM_OPTION* CItemManager::GetSetItemOption(WORD wSetItemKind)
{
	SET_ITEM_OPTION* pSetItemOption = NULL;

	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		if(pSetItemOption->wSetItemKind == wSetItemKind)
		{
			if(pSetItemOption->wSetValue == pSetItemOption->wApplicationValue)
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

void CItemManager::AddSetItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	DWORD dwSetItemCount = 0;
		
	dwSetItemCount = GetSetItemEquipValue(pItemInfo);

	m_SetItemOptionList.SetPositionHead();	
	SET_ITEM_OPTION* pSetItemOption = NULL;
	while( pSetItemOption = m_SetItemOptionList.GetData() )
	{
		if(pSetItemOption->wSetItemKind == pItemInfo->wSetItemKind)
		{
			WORD wSetValue = pSetItemOption->wSetValue;            

			sprintf( line, CHATMGR->GetChatMsg(1577), wSetValue);
			pIcon->AddToolTipLine(line);

			if(dwSetItemCount >= wSetValue)
			{
				SetSetItemToolTip( pIcon, pItemInfo, TRUE, wSetValue );
			}
			else
			{
				SetSetItemToolTip( pIcon, pItemInfo, FALSE, wSetValue );
			}
		}		
	}
}

DWORD CItemManager::GetSetItemEquipValue(ITEM_INFO* pItemInfo)
{
	CItem* pItem = NULL;
	DWORD dwSetItemCount = 0;

	for(unsigned int ui = TP_WEAR_START ; ui < TP_WEAR_END ; ui++)
	{
		pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( ui );
		if( pItem )
		{
			if(pItem->GetItemInfo()->wSetItemKind == pItemInfo->wSetItemKind)
			{
				dwSetItemCount++;
			}		
		}

		pItem = NULL;
	}

	return dwSetItemCount;
}

void CItemManager::SetSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo, BOOL bSetItemEnable, WORD wSetValue)
{
	char line[128];
	DWORD dwColor;
	float attrvalue = 0;
	
	if(bSetItemEnable == TRUE)
	{
		dwColor = TTTC_SETITEMOPTION_ENABLE;
	}
	else
	{
		dwColor = TTTC_SETITEMOPTION_DISABLE;
	}

	
	SET_ITEM_OPTION* pSetItemOption = NULL;	
	pSetItemOption = GetSetItemOption(pItemInfo->wSetItemKind, wSetValue);
	
	if(pSetItemOption == NULL)
	{
		return;
	}	

	if( pSetItemOption->wGenGol != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(382), pSetItemOption->wGenGol);		
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wMinChub != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(383), pSetItemOption->wMinChub);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wCheRyuk != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(384), pSetItemOption->wCheRyuk);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->wSimMek != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(385), pSetItemOption->wSimMek);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwLife != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(386), pSetItemOption->dwLife);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwShield != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(388), pSetItemOption->dwShield);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if( pSetItemOption->dwNaeRyuk != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(387), pSetItemOption->dwNaeRyuk);
		pIcon->AddToolTipLine( line, dwColor );
	}
	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
	{
		attrvalue = pSetItemOption->AttrRegist.GetElement_Val(i);
		if(attrvalue != 0)
		{
			sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(265+i-1), (int) (attrvalue * 100) );
			pIcon->AddToolTipLine( line, dwColor );
		}
		attrvalue = 0;
	}
	if( pSetItemOption->wMeleeAttackMin != 0 || pSetItemOption->wMeleeAttackMax != 0)
	{
		sprintf( line, "%s %d ~ %d", CHATMGR->GetChatMsg(389), pSetItemOption->wMeleeAttackMin, pSetItemOption->wMeleeAttackMax );
		pIcon->AddToolTipLine( line, dwColor );
	}	
	if( pSetItemOption->wRangeAttackMin != 0 || pSetItemOption->wRangeAttackMax != 0)
	{
		sprintf( line, "%s %d ~ %d", CHATMGR->GetChatMsg(391), pSetItemOption->wRangeAttackMin, pSetItemOption->wRangeAttackMax );
		pIcon->AddToolTipLine( line, dwColor );
	}	
	if( pSetItemOption->wCriticalPercent != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(390), pSetItemOption->wCriticalPercent);
		pIcon->AddToolTipLine( line, dwColor );
	}
	for(int i = ATTR_FIRE; i <= ATTR_MAX; ++i )
	{
		attrvalue = pSetItemOption->AttrAttack.GetElement_Val(i);
		if(attrvalue != 0)
		{
			sprintf( line, "%s +%d%%", CHATMGR->GetChatMsg(392+i-1), (int) (attrvalue * 100) );
			pIcon->AddToolTipLine( line, dwColor );
		}
		attrvalue = 0;
	}		
	if( pSetItemOption->wPhyDef != 0 )
	{
		sprintf(line, "%s +%d", CHATMGR->GetChatMsg(397), pSetItemOption->wPhyDef);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->wLifeRecover != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1564), pSetItemOption->wLifeRecover);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->fLifeRecoverRate != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1566), (WORD)(pSetItemOption->fLifeRecoverRate * 100));
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->wNaeRyukRecover != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1565), pSetItemOption->wNaeRyukRecover);
		pIcon->AddToolTipLine( line, dwColor );
	}
	if(pSetItemOption->fNaeRyukRecoverRate != 0)
	{
		sprintf(line, CHATMGR->GetChatMsg(1567), (WORD)(pSetItemOption->fNaeRyukRecoverRate * 100));
		pIcon->AddToolTipLine( line, dwColor );
	}
}

CYHHashTable<SET_ITEM_OPTION>* CItemManager::GetSetItemOptionList()
{
	return &m_SetItemOptionList;
}

void CItemManager::SetItemEffectProcess(CPlayer* pPlayer)
{
	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];
	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);		

	if( (pPlayer->InTitan() == TRUE) && (pPlayer->GetSetItemEffectID() != 0) )
	{
		pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
		pPlayer->SetSetItemEffectID( 0 );							
		return;
	}

	CHARACTER_TOTALINFO* pCharacterinfo = pPlayer->GetCharacterTotalInfo();

	for(unsigned int i = 0 ; i < eWearedItem_Max ; i++)
	{
		WORD wWearedItemIdx = pCharacterinfo->WearedItemIdx[i];
		ITEM_INFO* pItemInfo = GetItemInfo(wWearedItemIdx);
		if( !pItemInfo )
			continue;

		if(pItemInfo->wSetItemKind != 0)
		{
			for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
			{
				if(setItemInfo[ui].wKind == pItemInfo->wSetItemKind)
				{
					setItemInfo[ui].wCount++;
					break;
				}
				else
				{
					if(setItemInfo[ui].wKind == 0)
					{
						setItemInfo[ui].wKind = pItemInfo->wSetItemKind;
						setItemInfo[ui].wCount++;
						break;
					}					
				}				
			}
		}
	}	
	
	SET_ITEM_OPTION* pSetItemOption = NULL;		
	for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
	{
		if(setItemInfo[ui].wCount != 0)
		{			
			pSetItemOption = GetSetItemOption(setItemInfo[ui].wKind);

			if(pSetItemOption != NULL)
			{
				if( setItemInfo[ui].wCount == pSetItemOption->wSetValue )
				{
					if( pPlayer->GetSetItemEffectID() != SETITEM_STARTEFFECTID+pSetItemOption->wIndex )
					{
						if( pPlayer->GetSetItemEffectID() != 0 )
						{							
							pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
							pPlayer->SetSetItemEffectID( 0 );							
						}

						if(pPlayer->InTitan() == FALSE)
						{
							OBJECTEFFECTDESC desc(FindEffectNum(pSetItemOption->szEffect));
							pPlayer->AddObjectEffect( SETITEM_STARTEFFECTID+pSetItemOption->wIndex, &desc, 1 );
							pPlayer->SetSetItemEffectID( SETITEM_STARTEFFECTID+pSetItemOption->wIndex );
						}						
					}
				}
				else
				{
					if( pPlayer->GetSetItemEffectID() == SETITEM_STARTEFFECTID+pSetItemOption->wIndex )
					{
						pPlayer->RemoveObjectEffect( pPlayer->GetSetItemEffectID() );
						pPlayer->SetSetItemEffectID( 0 );
					}
				}
			}		
		}		
	}	
}

void CItemManager::AddUniqueItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo )
{
	char line[128];	
	char buf[256] = { 0, };
	UNIQUE_ITEM_OPTION_INFO* pUniqueInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pItemInfo->ItemIdx);
	if( !pUniqueInfo )
	{
		return;
	}

	pIcon->AddToolTipLine("");
	if(pUniqueInfo->ExchangeFlag == 1)
	{
		sprintf(line, CHATMGR->GetChatMsg(1607));
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	}
	else
	{
		sprintf(line, CHATMGR->GetChatMsg(1608));
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	}

	if(pUniqueInfo->MixFlag == 1)
	{
		sprintf(line, CHATMGR->GetChatMsg(1609));
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	}
	else
	{
		sprintf(line, CHATMGR->GetChatMsg(1610));
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );
	}

	sprintf(line, CHATMGR->GetChatMsg(1611), pUniqueInfo->nHp);
	if(pUniqueInfo->nHp > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nHp < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1612), pUniqueInfo->nMp);
	if(pUniqueInfo->nMp > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nMp < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1613), pUniqueInfo->nShield);
	if(pUniqueInfo->nShield > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nShield < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1614), pUniqueInfo->nPhyDamage);
	if(pUniqueInfo->nPhyDamage > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nPhyDamage < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1615), pUniqueInfo->nCriDamage);
	if(pUniqueInfo->nCriDamage > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCriDamage < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1616), pUniqueInfo->nCriRate);
	if(pUniqueInfo->nCriRate > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCriRate < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1617), pUniqueInfo->nGengol);
	if(pUniqueInfo->nGengol > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nGengol < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1618), pUniqueInfo->nMinChub);
	if(pUniqueInfo->nMinChub > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nMinChub < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1619), pUniqueInfo->nCheRyuk);
	if(pUniqueInfo->nCheRyuk > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nCheRyuk < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1620), pUniqueInfo->nSimMek);
	if(pUniqueInfo->nSimMek > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nSimMek < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1621), pUniqueInfo->nDefen);
	if(pUniqueInfo->nDefen > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nDefen < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1622), pUniqueInfo->nRange);
	if(pUniqueInfo->nRange > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nRange < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1623), pUniqueInfo->nAttR);
	if(pUniqueInfo->nAttR > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nAttR < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

	sprintf(line, CHATMGR->GetChatMsg(1624), pUniqueInfo->nEnemyDefen);
	if(pUniqueInfo->nEnemyDefen > 0)
		pIcon->AddToolTipLine( line, TTTC_MUGONGKIND );
	else if( pUniqueInfo->nEnemyDefen < 0)
		pIcon->AddToolTipLine( line, TTCLR_ENERMY );

}

void CItemManager::Item_TotalInfo_Local(void * pMsg)
{
	ITEM_TOTALINFO * msg = (ITEM_TOTALINFO *)pMsg;

	for (int i = 0; i < SLOT_INVENTORY_NUM; i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&msg->Inventory[i]);
		if (msg->Inventory[i].QuickPosition != 0)
		{
			if (EVENTMAP->IsEventMap())
			{
				ITEM_INFO* pItemInfo = GetItemInfo(msg->Inventory[i].wIconIdx);
				if (pItemInfo)
					if (pItemInfo->ItemKind != eYOUNGYAK_ITEM)
						QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition, msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
			}
			else
			{
				QUICKMGR->SetQuickItemReal(msg->Inventory[i].QuickPosition, msg->Inventory[i].Position, msg->Inventory[i].wIconIdx);
			}
		}
	}
	for (int i = 0; i < (SLOT_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM); i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&msg->ShopInventory[i]);
		if (msg->ShopInventory[i].QuickPosition != 0)
		{
			QUICKMGR->SetQuickItemReal(msg->ShopInventory[i].QuickPosition, msg->ShopInventory[i].Position, msg->ShopInventory[i].wIconIdx);
		}
	}
	for (int i = 0; i < eWearedItem_Max; i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&msg->WearedItem[i]);
		if (msg->WearedItem[i].QuickPosition != 0)
		{
			QUICKMGR->SetQuickItemReal(msg->WearedItem[i].QuickPosition, msg->WearedItem[i].Position, msg->WearedItem[i].wIconIdx);
		}
	}
	for (int i = 0; i < SLOT_PETWEAR_NUM; i++)
	{
		GAMEIN->GetPetInventoryDialog()->AddItem(&msg->PetWearedItem[i]);
		if (msg->PetWearedItem[i].QuickPosition != 0)
		{
			QUICKMGR->SetQuickItemReal(msg->PetWearedItem[i].QuickPosition, msg->PetWearedItem[i].Position, msg->PetWearedItem[i].wIconIdx);
		}
	}
	for (int i = 0; i < SLOT_TITANWEAR_NUM; i++)
	{
		GAMEIN->GetTitanInventoryDlg()->AddItem(&msg->TitanWearedItem[i]);
		if (msg->TitanWearedItem[i].QuickPosition != 0)
		{
			QUICKMGR->SetQuickItemReal(msg->TitanWearedItem[i].QuickPosition, msg->TitanWearedItem[i].Position, msg->TitanWearedItem[i].wIconIdx);
		}

		DWORD itemDBIdx = msg->TitanWearedItem[i].dwDBIdx;
		if (itemDBIdx)
		{
			TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(itemDBIdx);
			if (pInfo)
			{
				TITANMGR->AddTitanUsingEquipItemList(pInfo);
			}
		}
	}
	for (int i = 0; i < SLOT_TITANSHOPITEM_NUM; i++)
	{
		GAMEIN->GetTitanInventoryDlg()->AddItem(&msg->TitanShopItem[i]);
		if (msg->TitanShopItem[i].QuickPosition != 0)
		{
			QUICKMGR->SetQuickItemReal(msg->TitanShopItem[i].QuickPosition, msg->TitanShopItem[i].Position, msg->TitanShopItem[i].wIconIdx);
		}
	}
	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_Pyoguk_Info_Ack(void * pMsg)
{
	SEND_PYOGUK_ITEM_INFO * pmsg = (SEND_PYOGUK_ITEM_INFO*)pMsg;
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch (AddInfoKind)
		{
		case(CAddableInfoList::ItemOption) :
			{
				ITEM_OPTION_INFO OptionInfo[SLOT_PYOGUK_NUM];
				iter.GetInfoData(&OptionInfo);
				ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
			}
			break;
		case(CAddableInfoList::ItemRareOption) :
			{
				ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PYOGUK_NUM];
				iter.GetInfoData(&RareOptionInfo);
				ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
			}
			break;
		case(CAddableInfoList::PetTotalInfo) :
			{
				PET_TOTALINFO PetInfo[SLOT_PYOGUK_NUM];
				iter.GetInfoData(&PetInfo);
				PETMGR->InitPetInfo(PetInfo, pmsg->wPetInfoCount);
			}
			break;
		}
		iter.ShiftToNextData();
	}

	for (int i = 0; i < SLOT_PYOGUK_NUM; ++i)
	{
		if (pmsg->PyogukItem[i].dwDBIdx != 0)
			GAMEIN->GetPyogukDialog()->AddItem(&pmsg->PyogukItem[i]);
	}
	GAMEIN->GetPyogukDialog()->SetPyogukMoney(pmsg->money);

	GAMEIN->GetPyogukDialog()->SetItemInit(TRUE);
	GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	GAMEIN->GetPyogukDialog()->SetActive(TRUE);
	if (HERO->GetPyogukNum() == 0)
	{

	}
	else
	{
		GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1);
		GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1);
	}
}
void CItemManager::Item_PetInven_Info_Ack(void * pMsg)
{
	SEND_PETINVEN_ITEM_INFO* pmsg = (SEND_PETINVEN_ITEM_INFO*)pMsg;

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch (AddInfoKind)
		{
		case(CAddableInfoList::ItemOption) :
		{
			ITEM_OPTION_INFO OptionInfo[SLOT_PETINVEN_NUM];
			iter.GetInfoData(&OptionInfo);
			ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
		}
										   break;
		case(CAddableInfoList::ItemRareOption) :
		{
			ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PETINVEN_NUM];
			iter.GetInfoData(&RareOptionInfo);
			ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
		}
											   break;
		}

		iter.ShiftToNextData();
	}

	for (int i = 0; i < SLOT_PETINVEN_NUM; ++i)
	{
		if (pmsg->PetInvenItem[i].dwDBIdx != 0)
		{
			GAMEIN->GetPetInventoryDialog()->AddItem(&pmsg->PetInvenItem[i]);
		}
	}

	GAMEIN->GetPetInventoryDialog()->SetItemInfoInit(TRUE);

	PETMGR->SetPetValidInvenTab();

	GAMEIN->GetPetInventoryDialog()->SetActive(TRUE);
}
void CItemManager::Item_Monster_RareObtain_Notify(void * pMsg)
{
	MSG_ITEM_RAREITEM_GET* pmsg = (MSG_ITEM_RAREITEM_GET*)pMsg;

	AddItemRareOption(&pmsg->RareInfo);

	CItem* pItem = NULL;
	if (pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->RareItemBase.Position))
	{
		pItem->SetItemBaseInfo(&pmsg->RareItemBase);
	}
	else
	{
		pItem = MakeNewItem(&pmsg->RareItemBase, "MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY");
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		ASSERT(rt);
	}

	GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem);

	cDialog* pDlg = WINDOWMGR->GetWindowForID(DBOX_DIVIDE_INV);
	if (pDlg)
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc(0);
	}

	if (pItem)
	{
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);

		ItemDropEffect(pItem->GetItemIdx());
	}

	QUICKMGR->RefreshQickItem();


	if (ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
	{
		ITEMMGR->FakeDeleteItem(pItem);

		ITEMMGR->SendDeleteItem();
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2094), pItem->GetItemInfo()->ItemName);
	}
}
void CItemManager::Item_Reinforce_Success_Ack(void* pMsg)
{
	MSG_ITEM_REINFORCE_ACK * pmsg = (MSG_ITEM_REINFORCE_ACK*)pMsg;

	AddItemOption(&pmsg->OptionInfo);

	CItem * ItemOut;
	for (int i = 1; i < MAX_REINFORCEGRID_NUM + 1; ++i)
	{
		CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
		if (pVItem)
			DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
	}

	REINFORCEMGR->SetRareMaterialTotalGravity(0);
	GAMEIN->GetReinforceDialog()->SetGravityText();

	CItem * pTItem = NULL;
	CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(0);
	if (pVItem)
	{
		pTItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pVItem->GetSrcPosition());
		if (pTItem)
		{
			pTItem->SetDurability(pmsg->OptionInfo.dwOptionIdx);
			SetToolTipIcon(pTItem, &pmsg->OptionInfo, ITEMMGR->GetItemRareOption(pTItem->GetRareness()), 0); // 2014-11-15 附
			GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
			pTItem->SetLock(TRUE);
		}
	}
	GAMEIN->GetReinforceDialog()->Release();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(225));
	QUICKMGR->RefreshQickItem();
	GAMEIN->GetReinforceDialog()->SetDisable(FALSE);

	if (GAMEIN->GetReinforceDialog()->IsShopItemUse())
		GAMEIN->GetReinforceDialog()->SetActiveRecursive(FALSE);
	else if (pTItem)
	{
		GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
		pTItem->SetLock(TRUE);
	}
}
void CItemManager::Item_Reinforce_Failed_Ack(void* pMsg)
{
	MSG_ITEM_REINFORCE_SYN * pmsg = (MSG_ITEM_REINFORCE_SYN*)pMsg;

	REINFORCEMGR->SetRareMaterialTotalGravity(0);
	GAMEIN->GetReinforceDialog()->SetGravityText();

	CItem * ItemOut;
	for (int i = 1; i < MAX_REINFORCEGRID_NUM + 1; ++i)
	{
		CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
		if (pVItem)
			DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
	}
	GAMEIN->GetReinforceDialog()->Release(eReinforceViewRelease);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(226));
	QUICKMGR->RefreshQickItem();
	GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
}
void CItemManager::Item_Reinforce_Nack(void* pMsg)
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;
	REINFORCEMGR->SetRareMaterialTotalGravity(0);
	GAMEIN->GetReinforceDialog()->SetGravityText();

	GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
	if (pmsg->ECode == 64)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2257));
	if (pmsg->ECode == 265)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(728));
}
void CItemManager::Item_Reinforce_WithShopItem_Nack(void* pMsg)
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	char buf[256];
	memset(buf, 0, sizeof(char) * 256);

	switch (pmsg->ECode)
	{
	case 3:
		sprintf(buf, CHATMGR->GetChatMsg(1081));
		break;
	case 4:
		sprintf(buf, CHATMGR->GetChatMsg(1082));
		break;
	case 10:
	{
		CItem * ItemOut;
		for (int i = 1; i < MAX_REINFORCEGRID_NUM + 1; ++i)
		{
			CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(i);
			if (pVItem)
				DeleteItem(pVItem->GetSrcPosition(), &ItemOut);
		}

		CVirtualItem * pVItem = GAMEIN->GetReinforceDialog()->GetVirtualItem(0);
		if (pVItem)
		{
			CItem * pTItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pVItem->GetSrcPosition());
			if (pTItem)
			{
				GAMEIN->GetReinforceDialog()->AddVirtualItemWrap(0, pTItem);
				pTItem->SetLock(TRUE);
			}
		}
		GAMEIN->GetReinforceDialog()->Release();
		QUICKMGR->RefreshQickItem();
		GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
		sprintf(buf, CHATMGR->GetChatMsg(1083));
	}
		break;
	case 11:
		sprintf(buf, CHATMGR->GetChatMsg(1084));
		break;
	case 12:
		sprintf(buf, CHATMGR->GetChatMsg(1085));
		break;
	case 13:
		sprintf(buf, CHATMGR->GetChatMsg(1089));
		break;
	case 14:
		sprintf(buf, CHATMGR->GetChatMsg(1090));
		break;
	case 64:
		sprintf(buf, CHATMGR->GetChatMsg(2257));
		break;
	case 256:
		sprintf(buf, CHATMGR->GetChatMsg(728));
		break;
	default:
		break;
	}

	CHATMGR->AddMsg(CTC_SYSMSG, buf);
	GAMEIN->GetReinforceDialog()->SetDisable(FALSE);
	GAMEIN->GetReinforceDialog()->SetActiveRecursive(FALSE);
}
void CItemManager::Item_Move_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	MoveItem((MSG_ITEM_MOVE_ACK *)pMsg);

	WINDOWMGR->DragWindowNull();
}
void CItemManager::Item_Combine_Ack(void* pMsg)
{
	CombineItem((MSG_ITEM_COMBINE_ACK *)pMsg);

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	//WINDOWMGR->BackDragWindow();
	WINDOWMGR->DragWindowNull();
}

void CItemManager::Item_Divide_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_ShopInven);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		pPreItem = NULL;
		DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);

		pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_DIVIDE_ACK");
		if (!AddItem(pItem))
		{
			ASSERT(0);
		}
		if (pItem->GetQuickPosition() != 0)
		{
			WORD QuickPosList[4];
			CQuickManager::AnalyzeQuickPosition(pItem->GetQuickPosition(), QuickPosList);

			for (WORD SheetNum = 0; SheetNum < 4; SheetNum++)
			{
				if (QuickPosList[SheetNum])
					QUICKMGR->AddQuickItemReal(((SheetNum * 10) + (QuickPosList[SheetNum] - 1)), pItem->GetPosition(), pItem->GetItemIdx());
			}
		}
	}

	QUICKMGR->RefreshQickItem();

	WINDOWMGR->DragWindowNull();
}
void CItemManager::Item_Monster_Obtain_Notify(void* pMsg)
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;

	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			pItem->SetItemBaseInfo(pmsg->GetItem(i));
		}
		else
		{
			pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_MONSTER_OBTAIN_NOTIFY");
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
			if (!rt)
			{
				ASSERT(0);
			}
		}

		GAMEEVENTMGR->AddEvent(eGameEvent_ObtainItem);
	}

	cDialog* pDlg = WINDOWMGR->GetWindowForID(DBOX_DIVIDE_INV);
	if (pDlg)
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc(0);
	}

	if (pItem)
	{
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);
		ItemDropEffect(pItem->GetItemIdx());
	}

	QUICKMGR->RefreshQickItem();

	if (ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
	{
		ITEMMGR->FakeDeleteItem(pItem);

		ITEMMGR->SendDeleteItem();
		//ITEMMGR->SendSellItem(pItem);
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2094), pItem->GetItemInfo()->ItemName);
	}
}
void CItemManager::Item_Move_Nack(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	switch (pmsg->wData)
	{
	case 7:
		break;
	default:
		ASSERT(pmsg->wData == 3);
	}
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	WINDOWMGR->BackDragWindow();
}
void CItemManager::Item_Divide_Nack(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_ShopInven);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	WINDOWMGR->BackDragWindow();
}
void CItemManager::Item_Combine_Nack()
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	WINDOWMGR->BackDragWindow();
}
void CItemManager::Item_Discard_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	ASSERT(0);
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	if (pmsg->ECode != 4)
		WINDOWMGR->BackDragWindow();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(799));
}
void CItemManager::Item_Discard_Ack(void* pMsg)
{
	MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;
	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);

	DeleteItemofTable(wTableIdx, pmsg->TargetPos);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_Use_Notify(void* pMsg)
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
	if (pObject == NULL) return;
	if (pObject->GetObjectKind() != eObjectKind_Player) return;

	CPlayer* pPlayer = (CPlayer*)pObject;

	switch (pmsg->bData)
	{
	case 1:
	{
		if (pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_ba_070.beff"));
			pPlayer->AddObjectEffect(LIFE_RECOVER_EFFECTID, &desc, 1);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_ba_070.beff"));
			pPlayer->AddObjectEffect(LIFE_RECOVER_EFFECTID, &desc, 1);
		}
	}
		break;
	case 2:
	{
		if (pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_ba_071.beff"));
			pPlayer->AddObjectEffect(MANA_RECOVER_EFFECTID, &desc, 1);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_ba_071.beff"));
			pPlayer->AddObjectEffect(MANA_RECOVER_EFFECTID, &desc, 1);
		}
	}
		break;
	case 3:
	{
		if (pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_ba_077.beff"));
			pPlayer->AddObjectEffect(SHIELD_RECOVER_EFFECTID, &desc, 1);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_ba_077.beff"));
			pPlayer->AddObjectEffect(SHIELD_RECOVER_EFFECTID, &desc, 1);
		}
	}
		break;
	case 4:
	{
		if (pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_ba_078.beff"));
			pPlayer->AddObjectEffect(ALL_RECOVER_EFFECTID, &desc, 1);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("m_ba_078.beff"));
			pPlayer->AddObjectEffect(ALL_RECOVER_EFFECTID, &desc, 1);
		}
	}
		break;
	}
}
void CItemManager::Item_Use_Ack(void* pMsg)
{
	MSG_ITEM_USE_ACK* pmsg = (MSG_ITEM_USE_ACK*)pMsg;

	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);

	switch (wTableIdx)
	{
	case eItemTable_Inventory:
	case eItemTable_Weared:
	{

		CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
		if (item == NULL) return;
		eITEM_KINDBIT bits = item->GetItemKind();
		if (bits & eYOUNGYAK_ITEM)
		{

			if (item->GetDurability() > 1)
			{
				GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);

				YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
				Tmp = GAMERESRCMNGR->GetYoungYakDelay()->GetData(item->GetItemIdx());
				if (Tmp)
				{//[设置药品回气时间][By:十里坡剑神][QQ:112582793][2017/11/27]
					HERO->GetDelayGroup()->AddYoungYakPotionDelay(Tmp->DelayTime, Tmp->wtype);   
				}
			}
			else
			{
				CItem* pOutItem = NULL;
				DeleteItem(pmsg->TargetPos, &pOutItem);
				if (pOutItem)
					ReLinkQuickPosition(pOutItem);
			}
			if (item)
			{
				if (item->GetItemIdx() == 53032 || item->GetItemIdx() == 53095 ||
					item->GetItemIdx() == 53103 || item->GetItemIdx() == 53102 ||
					item->GetItemIdx() == 53217 || item->GetItemIdx() == 53223 ||
					item->GetItemIdx() == 53232 || item->GetItemIdx() == 53234)
				{
					if (HERO->InTitan() == TRUE)
						EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_FullYoungYak);
					else
						EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_FullYoungYak);
				}
				else
				{
					if (!(item->GetItemKind() & eYOUNGYAK_ITEM))	return;

					if (item->GetItemKind() == eYOUNGYAK_ITEM)
					{
						if (item->GetItemInfo()->LifeRecover || item->GetItemInfo()->LifeRecoverRate)
						{
							if (HERO->InTitan() == TRUE)
								EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_LifeYoungYak);
							else
								EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_LifeYoungYak);
						}
						if (item->GetItemInfo()->NaeRyukRecover || item->GetItemInfo()->NaeRyukRecoverRate)
						{
							if (HERO->InTitan() == TRUE)
								EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_ManaYoungYak);
							else
								EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_ManaYoungYak);
						}
						if (item->GetItemIdx() == 53031 || item->GetItemIdx() == 53094 || item->GetItemIdx() == 53109)
						{
							if (HERO->InTitan() == TRUE)
								EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_HosinYoungYak);
							else
								EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_HosinYoungYak);
						}
						if (item->GetItemIdx() == 53216 || item->GetItemIdx() == 53222)
						{
							if (HERO->InTitan() == TRUE)
								EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_UseItem_HosinYoungYak);
							else
								EFFECTMGR->StartHeroEffectProcess(eEffect_UseItem_HosinYoungYak);
						}
					}
					else if (item->GetItemKind() == eYOUNGYAK_ITEM_PET)
					{
						CPet* pPet = PETMGR->GetCurSummonPet();
						if (pPet)
						{
							TARGETSET set;
							set.pTarget = pPet;
							EFFECTMGR->StartEffectProcess(eEffect_FeedUp, pPet, &set, 0, pPet->GetID());
						}
					}
					else if (item->GetItemKind() == eYOUNGYAK_ITEM_TITAN)
					{
#define TITAN_POTION_DELAY 30000
						DWORD dwTime = TITAN_POTION_DELAY;
						if (TITANMGR->GetTitanShopitemOption()->dwEPReduceRate > 0.0f)
						{
							dwTime = (DWORD)(dwTime * TITANMGR->GetTitanShopitemOption()->dwEPReduceRate);
						}

						HERO->GetDelayGroup()->AddTitanPotionDelay(dwTime);
					}

				}
			}
		}
		else if (bits & eCHANGE_ITEM)
		{
			if (item->GetDurability() > 1)
			{
				GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);
			}
			else
			{
				CItem* pOutItem = NULL;
				DeleteItem(pmsg->TargetPos, &pOutItem);
				if (pOutItem)
					ReLinkQuickPosition(pOutItem);
			}
		}
		else if (bits & eEXTRA_ITEM)
		{
			if (item->GetDurability() > 1)
			{
				GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);
			}
			else
			{
				CItem* pOutItem = NULL;
				DeleteItem(pmsg->TargetPos, &pOutItem);
				if (pOutItem)
					ReLinkQuickPosition(pOutItem);
			}
		}
		else if (bits & eMUGONG_ITEM)
		{
			CItem* pOutItem = NULL;
			DeleteItem(pmsg->TargetPos, &pOutItem);

			AUDIOMGR->Play(56, HERO);
		}
		else if (bits & eQUEST_ITEM)
		{
			if (bits == eQUEST_ITEM_START)
			{
				if (item->GetDurability() > 1)
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem(pmsg->TargetPos, &pOutItem);
					if (pOutItem)
						ReLinkQuickPosition(pOutItem);
				}
			}
		}
		else if (bits & eTITAN_ITEM)
		{

		}
		else
		{
			ASSERT(0);
		}
	}
		break;
	case eItemTable_StreetStall:
		{
			ASSERT(0);
		}
		break;
	case eItemTable_ShopInven:
		{
			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
			eITEM_KINDBIT bits = item->GetItemKind();

			if (bits & eSHOP_ITEM)
			{
				if (item->GetDurability() > 1)
				{
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->TargetPos, -1);
				}
				else
				{
					CItem* pOutItem = NULL;
					DeleteItem(pmsg->TargetPos, &pOutItem);
					if (pOutItem)
						ReLinkQuickPosition(pOutItem);
				}

				AUDIOMGR->Play(68, HERO);
			}
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_Mix_AddItem_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
	if (pItem)
		GAMEIN->GetMixDialog()->AddRealItem(pItem);

	GAMEIN->GetMixDialog()->DieCheck();
}
void CItemManager::Item_Mix_AddItem_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);


	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
	if (pItem)
		pItem->SetLock(FALSE);

	GAMEIN->GetMixDialog()->Release(eMixAllRelease, FALSE);
	GAMEIN->GetMixDialog()->DieCheck();
}
void CItemManager::Item_Mix_Success_Ack(void* pMsg)
{
	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
	GAMEIN->GetMixDialog()->Release(eMixAllRelease, FALSE);

	CItem * BasicItemOut;
	DeleteItem(pmsg->BasicItemPos, &BasicItemOut, NULL, NULL, eRemainAllOption);


	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);

			if (pItem)
				if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
				{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
				}
		}

		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}

	ITEMBASE NewItem;
	NewItem = *(BasicItemOut->GetItemBaseInfo());
	NewItem.Position = pmsg->BasicItemPos;
	NewItem.wIconIdx = pmsg->wResultItemIdx;
	NewItem.RareIdx = pmsg->dwRareIdx;
	NewItem.QuickPosition = 0;

	if (!IsDupItem(NewItem.wIconIdx))
		NewItem.Durability = 0;

	ITEM_INFO* piteminfo = ITEMMGR->GetItemInfo(NewItem.wIconIdx);

	GAMEIN->GetMixDialog()->Release(eMixBasicRelease, FALSE);
	if (piteminfo && !(piteminfo->ItemKind & eSHOP_ITEM))
	{
		GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
		CItem * pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->BasicItemPos);
		if (!(piteminfo->ItemKind&eCHANGE_ITEM))
		{//[自动开箱子与组合冲突修复][By:十里坡剑神][QQ:112582793][2018/1/13]
			//[如果组合出来的物品为箱子，并开启自动开箱子功能，则会报错][By:十里坡剑神][QQ:112582793][2018/1/13]
			GAMEIN->GetMixDialog()->AddVirtualItemWrap(eMixPos_Result, pResultItem);
			pResultItem->SetLock(TRUE);
		}
	}

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(207), piteminfo->ItemName);
	QUICKMGR->RefreshQickItem();
	ITEMMGR->RefreshItemToolTip(NewItem.dwDBIdx);
	GAMEIN->GetMixDialog()->DieCheck();
	if (GAMEIN->GetMixDialog()->IsShopItemUse())
	{
		GAMEIN->GetMixDialog()->Release();
		GAMEIN->GetMixDialog()->SetActiveRecursive(FALSE);
	}
}
void CItemManager::Item_Mix_BigFailed_Ack(void* pMsg)
{
	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
	GAMEIN->GetMixDialog()->Release();

	CItem * BasicItemOut;
	DeleteItem(pmsg->BasicItemPos, &BasicItemOut);

	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
			if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
			{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
			}
		}

		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}
	GAMEIN->GetMixDialog()->Release(eMixBasicRelease);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(208));
	QUICKMGR->RefreshQickItem();

	GAMEIN->GetMixDialog()->DieCheck();
	if (GAMEIN->GetMixDialog()->IsShopItemUse())
		GAMEIN->GetMixDialog()->SetActiveRecursive(FALSE);
}
void CItemManager::Item_Mix_Failed_Ack(void* pMsg)
{
	GAMEIN->GetMixDialog()->ResetCurSelCellPos();

	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;

	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
			if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
			{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
			}
		}
		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(209));
	QUICKMGR->RefreshQickItem();

	GAMEIN->GetMixDialog()->DieCheck();
	if (GAMEIN->GetMixDialog()->IsShopItemUse())
	{
		GAMEIN->GetMixDialog()->Release();
		GAMEIN->GetMixDialog()->SetActiveRecursive(FALSE);
	}
}
void CItemManager::Item_Mix_Msg(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	char buf[256];
	memset(buf, 0, sizeof(char) * 256);

	switch (pmsg->dwData1)
	{
	case 20:
		sprintf(buf, CHATMGR->GetChatMsg(1086));
		break;
	case 21:
	{
		CItem * BasicItemOut;
		DeleteItem((POSTYPE)pmsg->dwData2, &BasicItemOut);
		sprintf(buf, CHATMGR->GetChatMsg(1088));
	}
		break;
	case 22:
		sprintf(buf, CHATMGR->GetChatMsg(1087));
		break;
	case 23:
		sprintf(buf, CHATMGR->GetChatMsg(1082));
		break;
	case 256:
		sprintf(buf, CHATMGR->GetChatMsg(728));
		break;
	}

	CHATMGR->AddMsg(CTC_SYSMSG, buf);
	GAMEIN->GetMixDialog()->Release();
	GAMEIN->GetMixDialog()->DieCheck();
	GAMEIN->GetMixDialog()->SetActiveRecursive(FALSE);
}
void CItemManager::Item_TPM_AddItem_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
	if (pItem)
		GAMEIN->GetTitanPartsMakeDlg()->AddRealItem(pItem);

	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
}
void CItemManager::Item_TPM_AddItem_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);

	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
	if (pItem)
		pItem->SetLock(FALSE);

	GAMEIN->GetTitanPartsMakeDlg()->Release(eTPMAllRelease, FALSE);
	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
}
void CItemManager::Item_TPM_Success_Ack(void* pMsg)
{
	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
	TITAN_ENDURANCE_ITEMINFO EnduranceInfo;
	ITEM_INFO* pBaseInfo = GetItemInfo(pmsg->wBasicItemIdx);
	if (!pBaseInfo)
		return;

	if ((eITEM_KINDBIT)pBaseInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->BasicItemPos);
		if (!pItemBase)
			return;

		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItemBase->dwDBIdx);
		CopyMemory(&EnduranceInfo, pEnduranceInfo, sizeof(TITAN_ENDURANCE_ITEMINFO));
	}

	GAMEIN->GetTitanPartsMakeDlg()->Release(eTPMAllRelease, FALSE);

	CItem * BasicItemOut;

	DeleteItem(pmsg->BasicItemPos, &BasicItemOut, NULL, NULL, eRemainAllOption);

	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);

			if (pItem)
				if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
				{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
				}
		}

		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}

	ITEMBASE NewItem;
	NewItem = *(BasicItemOut->GetItemBaseInfo());
	NewItem.Position = pmsg->BasicItemPos;
	NewItem.wIconIdx = pmsg->wResultItemIdx;
	NewItem.RareIdx = pmsg->dwRareIdx;
	NewItem.QuickPosition = 0;

	if (!IsDupItem(NewItem.wIconIdx))
		NewItem.Durability = 0;

	GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
	CItem * pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->BasicItemPos);
	GAMEIN->GetTitanPartsMakeDlg()->AddVirtualItemWrap(eTPMPos_Result, pResultItem);
	pResultItem->SetLock(TRUE);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(207), pResultItem->GetItemInfo()->ItemName);
	QUICKMGR->RefreshQickItem();

	if ((eITEM_KINDBIT)pBaseInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		TITANMGR->AddTitanEquipItemEnduranceInfo(&EnduranceInfo);
		RefreshItemToolTip(EnduranceInfo.ItemDBIdx);
	}

	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
	if (GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse())
	{
		GAMEIN->GetTitanPartsMakeDlg()->Release();
		GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive(FALSE);
	}
}
void CItemManager::Item_TPM_BigFailed_Ack(void* pMsg)
{
	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;
	GAMEIN->GetTitanPartsMakeDlg()->Release();

	CItem * BasicItemOut;
	DeleteItem(pmsg->BasicItemPos, &BasicItemOut);

	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
			if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
			{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
			}
		}

		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}
	GAMEIN->GetTitanPartsMakeDlg()->Release(eTPMBasicRelease);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(208));
	QUICKMGR->RefreshQickItem();

	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
	if (GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse())
		GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive(FALSE);
}
void CItemManager::Item_TPM_Failed_Ack(void* pMsg)
{
	GAMEIN->GetTitanPartsMakeDlg()->ResetCurSelCellPos();

	MSG_ITEM_MIX_ACK * pmsg = (MSG_ITEM_MIX_ACK *)pMsg;

	CItem * pMatItemOut;
	for (int i = 0; i < pmsg->wMaterialNum; ++i)
	{
		if (IsDupItem(pmsg->Material[i].wItemIdx))
		{
			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->Material[i].ItemPos);
			if (pItem->GetDurability() - pmsg->Material[i].Dur != 0)
			{
				pItem->SetDurability(pItem->GetDurability() - pmsg->Material[i].Dur);
				continue;
			}
		}
		DeleteItem(pmsg->Material[i].ItemPos, &pMatItemOut);
	}
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(209));
	QUICKMGR->RefreshQickItem();

	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
	if (GAMEIN->GetTitanPartsMakeDlg()->IsShopItemUse())
	{
		GAMEIN->GetTitanPartsMakeDlg()->Release();
		GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive(FALSE);
	}
}
void CItemManager::Item_TPM_Msg(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	char buf[256];
	memset(buf, 0, sizeof(char) * 256);

	switch (pmsg->dwData1)

	{
	case 20:
		sprintf(buf, CHATMGR->GetChatMsg(1086));
		break;
	case 21:
	{
		CItem * BasicItemOut;
		DeleteItem((POSTYPE)pmsg->dwData2, &BasicItemOut);
		sprintf(buf, CHATMGR->GetChatMsg(1088));
	}
		break;
	case 22:
		sprintf(buf, CHATMGR->GetChatMsg(1087));
		break;
	case 23:
		sprintf(buf, CHATMGR->GetChatMsg(1082));
		break;
	}

	CHATMGR->AddMsg(CTC_SYSMSG, buf);
	GAMEIN->GetTitanPartsMakeDlg()->Release();
	GAMEIN->GetTitanPartsMakeDlg()->DieCheck();
	GAMEIN->GetTitanPartsMakeDlg()->SetActiveRecursive(FALSE);
}
void CItemManager::Item_Mall_Money(void* pMsg)
{
	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;

	if (pmsg->bFlag == 1)
	{
		ITEM_INFO * pItem = ITEMMGR->GetItemInfo(pmsg->ItemIdx);

		HERO->SetMallMoney(HERO->GetMallMoney() - (pmsg->dwTotalMoney));
		if (pItem)
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2066), pItem->ItemName, pmsg->dwTotalMoney);
	}
	if (pmsg->bFlag == 2)
	{
		HERO->SetMallMoney(HERO->GetMallMoney() + (pmsg->dwTotalMoney));

		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2095), pmsg->dwTotalMoney);
	}
}
void CItemManager::Item_Gold_Money(void* pMsg)
{
	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;
	switch (pmsg->bFlag)
	{
	case eShopUse:
		{
			ITEM_INFO * pItem = ITEMMGR->GetItemInfo(pmsg->ItemIdx);
			HERO->SetGoldMoney(HERO->GetGoldMoney() - (pmsg->dwTotalMoney));
			if (pItem)
				CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2067), pItem->ItemName, pmsg->dwTotalMoney);
		}
		break;
	case eNoneItem:
		{
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2082), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(HERO->GetGoldMoney() - (pmsg->dwTotalMoney));
		}
		break;
	case eFbUse:
		{//[副本使用][BY:十里坡剑神][QQ:112582793][2019-9-19][22:49:14]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2096), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(HERO->GetGoldMoney() - (pmsg->dwTotalMoney));
		}
		break;
	case eGetGoldMoney:
		{//[获得元宝][BY:十里坡剑神][QQ:112582793][2019-9-19][22:49:09]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2101), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(HERO->GetGoldMoney() + (pmsg->dwTotalMoney));
		}
		break;
	case eOnlineRef:
		{//[充值刷新信息][BY:十里坡剑神][QQ:112582793][2019-9-19][22:48:59]
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	case eDeleteBuf:
		{//[buff删除][BY:十里坡剑神][QQ:112582793][2019-9-19][22:48:53]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2179), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	case eFame:
		{//[洗恶名][BY:十里坡剑神][QQ:112582793][2019-9-19][22:48:47]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2202), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	case eSinged:
		{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(1724), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	case eVimuWithGold:
		{//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-27][19:15:42]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2359), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	case eItemInherit:
		{//[成长继承][BY:十里坡剑神][QQ:112582793][2019-8-22][23:15:32]
			CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2392), pmsg->dwTotalMoney);
			HERO->SetGoldMoney(pmsg->dwRealMoney);
		}
		break;
	default:
		break;
	}
}
void CItemManager::Item_SW_Money(void* pMsg)
{
	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;

	if (pmsg->bFlag == 1)
	{
		ITEM_INFO * pItem = ITEMMGR->GetItemInfo(pmsg->ItemIdx);

		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2068), pItem->ItemName, pmsg->dwTotalMoney);
	}

	HERO->SetFame(HERO->GetFame() - (pmsg->dwTotalMoney));
}
void CItemManager::Item_UpGrade_Success_Ack(void* pMsg)
{
	MSG_ITEM_UPGRADE_ACK * pmsg = (MSG_ITEM_UPGRADE_ACK *)pMsg;
	//			CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemPos);
	CItem * itemOut;
	CItem * MaterialItemOut;
	ITEM_OPTION_INFO OptionInfo;
	DeleteItem(pmsg->ItemPos, &itemOut, &OptionInfo);
	DeleteItem(pmsg->MaterialItemPos, &MaterialItemOut);
	//GAMEIN->GetInventoryDialog()->DeleteItem(pmsg->ItemPos, &itemOut);

	int grade = pmsg->wItemIdx - itemOut->GetItemIdx();
	if (grade > 0)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(176), grade);
	else if (grade == 0)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(177));
	else
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(178), grade);

	//GAMEIN->GetInventoryDialog()->DeleteItem(pmsg->MaterialItemPos, &MaterialItemOut)
	//ItemDelete(MaterialItemOut);
	if (OptionInfo.dwOptionIdx != 0)
		AddItemOption(&OptionInfo);
	ITEMBASE NewItem;
	NewItem = *(itemOut->GetItemBaseInfo());
	NewItem.Position = pmsg->ItemPos;
	NewItem.wIconIdx = pmsg->wItemIdx;
	NewItem.QuickPosition = 0;

	//ItemDelete(itemOut);

	GAMEIN->GetInventoryDialog()->AddItem(&NewItem);
	GAMEIN->GetUpgradeDialog()->Release();
	CItem * pResultItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemPos);
	GAMEIN->GetUpgradeDialog()->AddVirtualItemWrap(eRESULTITEM_POS, pResultItem);
	pResultItem->SetLock(TRUE);
}
void CItemManager::Item_Use_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	if (pmsg->ECode == eItemUseErr_PreInsert)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(678));
	else if (pmsg->ECode == eItemUseErr_TitanItemTime)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(909));
	else if (pmsg->ECode == eItemUseErr_NotBoxKey)//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/18]
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2377));
}
void CItemManager::Item_Sell_Ack(void* pMsg)
{
	MSG_ITEM_SELL_ACK * pmsg = (MSG_ITEM_SELL_ACK *)pMsg;
	CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->TargetPos);
	if (IsDupItem(pmsg->wSellItemIdx) && (pItem->GetDurability() - pmsg->SellItemNum) > 0)
	{
		pItem->SetDurability(pItem->GetDurability() - pmsg->SellItemNum);
		DWORD SellPrice = SWPROFIT->CalTexRateForSell(pItem->GetItemInfo()->SellPrice);
		SellPrice = FORTWARMGR->CalTexRateForSell(pItem->GetItemInfo()->SellPrice);
		switch (pItem->GetItemInfo()->wSellType)
		{
		case eMoney:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(218), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
		case eMall:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2416), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
		case eGold:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2417), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
		default:
			break;
		}
		
	}
	else
	{
		DWORD SellPrice = SWPROFIT->CalTexRateForSell(pItem->GetItemInfo()->SellPrice);
		SellPrice = FORTWARMGR->CalTexRateForSell(pItem->GetItemInfo()->SellPrice);
		if (pmsg->SellItemNum == 0 || IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()))
		{
			switch (pItem->GetItemInfo()->wSellType)
			{
			case eMoney:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(217), pItem->GetItemInfo()->ItemName, AddComma(SellPrice)); break;
			case eMall:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2418), pItem->GetItemInfo()->ItemName, AddComma(SellPrice)); break;
			case eGold:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2419), pItem->GetItemInfo()->ItemName, AddComma(SellPrice)); break;
			default:
				break;
			}
		}
		else
		{
			switch (pItem->GetItemInfo()->wSellType)
			{
			case eMoney:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(218), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
			case eMall:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2416), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
			case eGold:CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2417), pItem->GetItemInfo()->ItemName, pmsg->SellItemNum, AddComma(SellPrice*pmsg->SellItemNum)); break;
			default:
				break;
			}
		}

		DeleteItemofTable(GetTableIdxForAbsPos(pmsg->TargetPos), pmsg->TargetPos);
	}
	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	GAMEIN->GetDealDialog()->SetDisable(FALSE);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_Sell_Nack(void* pMsg)
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	if (pmsg->ECode == 9)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(355));
	else if (pmsg->ECode == 11)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1552));


	GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
	GAMEIN->GetDealDialog()->SetDisable(FALSE);
}
void CItemManager::Item_Buy_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Deal);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);

	BOOL SetQuickLink = FALSE; //TEST code
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
			ASSERT(pPreItem);
		}
		pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_BUY_ACK");
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if (!rt)
		{
			ASSERT(0);
		}
		if (pItem->GetQuickPosition() != 0)
		{
			if (SetQuickLink == TRUE)
			{
				char buf[256];
				sprintf(buf, "Set QuickPosition again! CharacterIDX : %d, ItemDBIdx : %d", HEROID, pItem->GetItemBaseInfo()->dwDBIdx);
				ASSERTMSG(0, buf);
			}

			WORD QuickPosList[4];
			CQuickManager::AnalyzeQuickPosition(pItem->GetQuickPosition(), QuickPosList);

			for (WORD SheetNum = 0; SheetNum < 4; SheetNum++)
			{
				if (QuickPosList[SheetNum])
					QUICKMGR->AddQuickItemReal(((SheetNum * 10) + (QuickPosList[SheetNum] - 1)), pItem->GetPosition(), pItem->GetItemIdx());
			}
			SetQuickLink = TRUE;
		}
	}
	MONEYTYPE BuyPrice;
	if (pmsg->BuyType == 1 || pmsg->BuyType == 2)
	{
		return;
	}
	else if (pmsg->BuyType == 3)
	{
		BuyPrice = GAMEIN->GetGoldShopMainDlg()->GetBuyItemPrice();
		if (BuyPrice == 0) return;
	}
	else
	{
		BuyPrice = pItem->GetItemInfo()->BuyPrice;
		if (MAP->IsVillage() == FALSE)
		{
			BuyPrice = (MONEYTYPE)(BuyPrice * 1.2);
		}
		BuyPrice = SWPROFIT->CalTexRateForBuy(BuyPrice);

		BuyPrice = FORTWARMGR->CalTexRateForBuy(BuyPrice);
	}
	if (pmsg->wObtainCount > 1)
	{
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(216), pItem->GetItemInfo()->ItemName, pmsg->wObtainCount,
			AddComma(BuyPrice * pmsg->wObtainCount));	//confirm
	}
	else
	{
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(215), pItem->GetItemInfo()->ItemName, AddComma(BuyPrice));
	}

	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_Buy_Nack(void* pMsg)
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	//most of all, inventory full process : taiyo
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_Deal);
	//GAMEIN->GetDealDialog()->SetDisable( FALSE );

	switch (pmsg->ECode)
	{
	case NOT_MONEY:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(117));
		break;
	case NOT_SPACE:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(354));
		break;
	case NOT_EXIST:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(356));
		break;
	case NOT_PLAYER:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(357));
		break;
	case NO_DEMANDITEM:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1639));
		break;
	case NOT_REMAINITEM:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1632));
		break;
	case NOT_LIMITTIMES:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2319));
		break;
	default:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(358));
		break;
	}
}
void CItemManager::Item_Dealer_Ack(void* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	if (HERO->GetState() != eObjectState_Die)
		GAMEIN->GetDealDialog()->ShowDealer(pmsg->wData);
}
void CItemManager::Item_Dealer_Nack(void* pMsg)
{

}
void CItemManager::Item_Appearance_Change(void* pMsg)
{
	APPEARANCE_INFO* pmsg = (APPEARANCE_INFO*)pMsg;
	CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->PlayerID);
	if (pObject && pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		for (int n = 0; n < eWearedItem_Max; ++n)
			pPlayer->SetWearedItemIdx(n, pmsg->WearedItem[n]);
		APPEARANCEMGR->AddCharacterPartChange(pObject->GetID());
	}
	else
	{
		ASSERT(0);
	}
}
void CItemManager::Item_Money(void* pMsg)
{
	MSG_MONEY* pmsg = (MSG_MONEY*)pMsg;

	MONEYTYPE OriMoney = HERO->GetMoney();
	MONEYTYPE PrintMoney = 0;

	switch (pmsg->bFlag)
	{
	case 64:
		{
			PrintMoney = pmsg->dwTotalMoney - OriMoney;
			if (PrintMoney)
			{
				CHATMGR->AddMsg(CTC_GETMONEY, CHATMGR->GetChatMsg(120), AddComma(PrintMoney));

				if (HERO->InTitan() == TRUE)
					EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
			}
		}
		break;
	case MF_NOMAL:
		{
		}
		break;
	case MF_OBTAIN:
		{
			PrintMoney = pmsg->dwTotalMoney - OriMoney;
			if (PrintMoney)
			{
				CHATMGR->AddMsg(CTC_GETMONEY, CHATMGR->GetChatMsg(120), AddComma(PrintMoney));

				if (HERO->InTitan() == TRUE)
					EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
				else
					EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
			}
		}
		break;
	case MF_LOST:
		{
			PrintMoney = OriMoney - pmsg->dwTotalMoney;
			if (PrintMoney)
			{
				CHATMGR->AddMsg(CTC_GETMONEY, CHATMGR->GetChatMsg(118), AddComma(PrintMoney));
			}
		}
		break;
	default:
		break;
	}

	HERO->SetMoney(pmsg->dwTotalMoney);
}
void CItemManager::Item_Obtain_Money(void* pMsg)
{
	//ASSERT(0);
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	DWORD OriMoney = HERO->GetMoney();
	DWORD PrintMoney = pmsg->dwData - OriMoney;

	if (PrintMoney)
	{
		CHATMGR->AddMsg(CTC_GETMONEY, CHATMGR->GetChatMsg(120), PrintMoney);

		if (HERO->InTitan() == TRUE)
			EFFECTMGR->StartHeroEffectProcess(eEffect_Titan_GetMoney);
		else
			EFFECTMGR->StartHeroEffectProcess(eEffect_GetMoney);
	}

	HERO->SetMoney(pmsg->dwData);
}
void CItemManager::Item_Divide_NewItem_Notify(void* pMsg)
{
	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
	CItem* pItem = MakeNewItem(&pmsg->ItemInfo, "MP_ITEM_DIVIDE_NEWITEM_NOTIFY");
	int tbidx = GetTableIdxForAbsPos(pItem->GetItemBaseInfo()->Position);
	if (tbidx == eItemTable_Inventory || tbidx == eItemTable_ShopInven)
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
	else if (tbidx == eItemTable_Pyoguk)
		BOOL rt = GAMEIN->GetPyogukDialog()->AddItem(pItem);
	else if (tbidx == eItemTable_GuildWarehouse)
		BOOL rt = GAMEIN->GetGuildWarehouseDlg()->AddItem(pItem);
	CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(123));
}
void CItemManager::Item_Guild_Move_Ack(void* pMsg)
{
	MoveItemToGuild((MSG_ITEM_MOVE_ACK*)pMsg);
	WINDOWMGR->DragWindowNull();
}
void CItemManager::Item_Guild_Move_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	WINDOWMGR->BackDragWindow();
}
void CItemManager::Item_Guild_WareHouse_Info_Ack(void* pMsg)
{
	SEND_MUNPA_WAREHOUSE_ITEM_INFO* pmsg = (SEND_MUNPA_WAREHOUSE_ITEM_INFO*)pMsg;

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch (AddInfoKind)
		{
		case(CAddableInfoList::ItemOption) :
		{
			ITEM_OPTION_INFO OptionInfo[TABCELL_GUILDWAREHOUSE_NUM];
			iter.GetInfoData(&OptionInfo);
			ITEMMGR->InitItemOption(OptionInfo, pmsg->wOptionCount);
		}
										   break;
		case(CAddableInfoList::ItemRareOption) :
		{
			ITEM_RARE_OPTION_INFO RareOptionInfo[TABCELL_GUILDWAREHOUSE_NUM];
			iter.GetInfoData(&RareOptionInfo);
			ITEMMGR->InitItemRareOption(RareOptionInfo, pmsg->wRareOptionCount);
		}
											   break;
		}
		iter.ShiftToNextData();
	}

	GAMEIN->GetGuildWarehouseDlg()->SetWarehouseInfo(pmsg);
	RefreshAllItem();
}
void CItemManager::Item_DeleteFrom_GuildWare_Notify(void* pMsg)
{
	MSG_ITEM * pItembase = (MSG_ITEM*)pMsg;
	CItem* pItem = NULL;
	if (GAMEIN->GetGuildWarehouseDlg()->DeleteItem(pItembase->ItemInfo.Position, &pItem) == TRUE)
		ITEMMGR->ItemDelete(pItem);
	else
	{
		ASSERT(0);
	}
}
void CItemManager::Item_AddTo_GuildWare_Notify(void* pMsg)
{
	MSG_ITEM_WITH_OPTION * pItem = (MSG_ITEM_WITH_OPTION*)pMsg;
	if (pItem->IsOptionItem == TRUE)
	{
		if (IsOptionItem(pItem->ItemInfo.wIconIdx, pItem->ItemInfo.Durability) == TRUE)
		{
			AddItemOption(&pItem->OptionInfo);
		}
	}
	if (pItem->IsRareOptionItem == TRUE)
	{
		if (IsRareOptionItem(pItem->ItemInfo.wIconIdx, pItem->ItemInfo.RareIdx) == TRUE)
		{
			AddItemRareOption(&pItem->RareOptionInfo);
		}
	}

	GAMEIN->GetGuildWarehouseDlg()->AddItem(&pItem->ItemInfo);
}
void CItemManager::Item_Pyoguk_Move_Ack(void* pMsg)
{
	MoveItemToPyoguk((MSG_ITEM_MOVE_ACK*)pMsg);
	WINDOWMGR->DragWindowNull();
}
void CItemManager::Item_Dissolution_GetItem(void* pMsg)
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;
	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
			ASSERT(pPreItem);
		}
		pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_DISSOLUTION_GETITEM");
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if (!rt)
		{
			ASSERT(0);
		}
	}
}
void CItemManager::Item_Dissolution_Ack(void* pMsg)
{
	if (GAMEIN->GetDissolutionDialog())
		GAMEIN->GetDissolutionDialog()->EndDissolution(TRUE);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(692));
}
void CItemManager::Item_Dissolution_Nack(void* pMsg)
{
	if (GAMEIN->GetDissolutionDialog())
		GAMEIN->GetDissolutionDialog()->EndDissolution(FALSE);
}
void CItemManager::Item_Error_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);
	WINDOWMGR->BackDragWindow();
	MSG_ITEM_ERROR * pmsg = (MSG_ITEM_ERROR*)pMsg;
	if (pmsg->ECode == eItemUseErr_PreInsert)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(678));
	else if (pmsg->ECode == eItemUseErr_Discard)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(799));
	else if (pmsg->ECode == eItemUseErr_AlreadyUse)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(752));
	else if (pmsg->ECode == eItemUseErr_Mix)
	{
		if (GAMEIN->GetMixDialog())
			GAMEIN->GetMixDialog()->SetNowMixing(FALSE);
	}
	else if (pmsg->ECode == eItemUseErr_MixNotPace)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2259));
	}
	else if (pmsg->ECode == eItemUseErr_TitanPartsMake)
	{
		if (GAMEIN->GetTitanPartsMakeDlg())
			GAMEIN->GetTitanPartsMakeDlg()->SetNowMixing(FALSE);
	}
}
void CItemManager::Item_ChangeItem_Nack()
{
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(354));
}
void CItemManager::Item_Use_ChangeItem_Ack(void* pMsg)
{
	ITEMOBTAINARRAY* pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			pItem->SetItemBaseInfo(pmsg->GetItem(i));
		}
		else
		{
			pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_MONSTER_OBTAIN_NOTIFY");
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
			if (!rt)
			{
				ASSERT(0);
			}
		}
	}

	cDialog* pDlg = WINDOWMGR->GetWindowForID(DBOX_DIVIDE_INV);
	if (pDlg)
	{
		((cDivideBox*)pDlg)->ExcuteDBFunc(0);
	}

	if (pItem)
	{
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(121), pItem->GetItemInfo()->ItemName);
		ItemDropEffect(pItem->GetItemIdx());
	}

	QUICKMGR->RefreshQickItem();

	if (ITEMMGR->IsClearPacketItemCheck(pItem->GetItemIdx()))
	{
		ITEMMGR->FakeDeleteItem(pItem);

		ITEMMGR->SendDeleteItem();
		//ITEMMGR->SendSellItem(pItem);
		CHATMGR->AddMsg(CTC_GETITEM, CHATMGR->GetChatMsg(2094), pItem->GetItemInfo()->ItemName);
	}
}
void CItemManager::Item_ShopItem_Info_Ack(void* pMsg)
{
	SEND_SHOPITEM_INFO* pmsg = (SEND_SHOPITEM_INFO*)pMsg;

	if (pmsg->ItemCount)
	{
		GAMEIN->GetItemShopDialog()->SetItemInfo(pmsg);
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(785));
	}

	GAMEIN->GetItemShopDialog()->SetActive(TRUE);
	GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	GAMEIN->GetInventoryDialog()->OnShopItemBtn();
	GAMEIN->GetInventoryDialog()->SetShopItemInvenBtn(TRUE);
}
void CItemManager::Item_ShopItem_UsedInfo(void* pMsg)
{
	SEND_SHOPITEM_USEDINFO* pmsg = (SEND_SHOPITEM_USEDINFO*)pMsg;

	for (int i = 0; i < pmsg->ItemCount; i++)
	{
		AddUsedItemInfo(&pmsg->Item[i]);

		if (pmsg->Item[i].Param == eShopItemUseParam_EquipAvatar)
		{
			CItem* pItem = GetItem(pmsg->Item[i].ItemBase.dwDBIdx);
			if (pItem)
				pItem->SetUseParam(1);
		}

		if (pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend ||
			pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend7 ||
			pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMove2 ||
			pmsg->Item[i].ItemBase.wIconIdx == eIncantation_MemoryMoveExtend30)
			HERO->CalcShopItemOption(pmsg->Item[i].ItemBase.wIconIdx, TRUE);
	}
}
void CItemManager::Item_ShopItem_MPInfo(void* pMsg)
{
	SEND_MOVEDATA_INFO* pmsg = (SEND_MOVEDATA_INFO*)pMsg;

	if (pmsg->Count == 0 && pmsg->bInited)
	{
		SetDisableDialog(FALSE, eItemTable_Inventory);
		SetDisableDialog(FALSE, eItemTable_Pyoguk);
		SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		SetDisableDialog(FALSE, eItemTable_Shop);

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(794));
	}

	GAMEIN->GetMoveDialog()->SetMoveInfo(pmsg);
	if (pmsg->bInited)
		GAMEIN->GetMoveDialog()->SetActive(TRUE);
}
void CItemManager::Item_ShopItem_UseEnd(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	ITEM_INFO* pInfo = GetItemInfo((WORD)pmsg->dwData);
	if (!pInfo)		return;
	SHOPITEMBASE* pShopItemBase = m_UsedItemList.GetData((WORD)pmsg->dwData);
	if (!pShopItemBase)		return;
	if (pInfo->ItemKind == eSHOP_ITEM_AUTOADDPOINT)
	{//[自动加点][By:十里坡剑神][QQ:112582793][2018/4/17]
		GAMEIN->GetCharacterDialog()->SetCanUseAutoAddPoint(FALSE);
		GAMEIN->GetCharacterDialog()->InitAutoPoint();
		GAMEIN->GetCharacterDialog()->UpdateData();
	}
	if (pInfo->ItemType == 11)
	{
		BOOL bfind = FALSE;
		for (int i = 0; i < (SLOT_SHOPINVEN_NUM/*/2*/ + TABCELL_SHOPINVEN_PLUS_NUM); i++)
		{
			ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, i + TP_SHOPINVEN_START);
			if (pItemBase && pItemBase->dwDBIdx == pShopItemBase->ItemBase.dwDBIdx)
			{
				CItem* pOutItem = NULL;
				DeleteItem(i + TP_SHOPINVEN_START, &pOutItem);

				if (pOutItem)
				{
					char buf[256] = { 0, };
					sprintf(buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName);
					CHATMGR->AddMsg(CTC_SYSMSG, buf);
				}
				bfind = TRUE;
				break;
			}
		}
		if (!bfind)
		{
			for (int i = 0; i < SLOT_WEAR_NUM; i++)
			{
				ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, i + TP_WEAR_START);
				if (pItemBase && pItemBase->dwDBIdx == pShopItemBase->ItemBase.dwDBIdx)
				{
					CItem* pOutItem = NULL;
					DeleteItem(i + TP_WEAR_START, &pOutItem);

					if (pOutItem)
					{
						char buf[256] = { 0, };
						sprintf(buf, CHATMGR->GetChatMsg(750), pOutItem->GetItemInfo()->ItemName);
						CHATMGR->AddMsg(CTC_SYSMSG, buf);
					}

					bfind = TRUE;
					break;
				}
			}
		}
	}

	RemoveUsedItemInfo((WORD)pmsg->dwData, pShopItemBase->ItemBase.dwDBIdx);
	HERO->CalcShopItemOption((WORD)pmsg->dwData, FALSE);
}
void CItemManager::Item_ShopItem_OneMinute(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	STATUSICONDLG->SetOneMinuteToShopItem(pmsg->dwData);
}
void CItemManager::Item_ShopItem_Use_Ack(void* pMsg)
{
	SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;

	CItem * item = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ShopItemPos);
	if (item)
	{
		eITEM_KINDBIT bits = item->GetItemKind();

		if (bits & eSHOP_ITEM)
		{
			ITEM_INFO* pInfo = GetItemInfo(item->GetItemBaseInfo()->wIconIdx);

			if (bits != eSHOP_ITEM_MAKEUP && bits != eSHOP_ITEM_DECORATION && bits != eSHOP_ITEM_PET)
			{
				if (item->GetDurability() > 1)
				{
					GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->ShopItemPos, -1);
				}
				else
				{
					if (pInfo && pInfo->ItemType == 10 && pInfo->ItemIdx != eIncantation_ChangeName &&
						pInfo->ItemIdx != eIncantation_ChangeName_Dntrade)
					{
						CItem* pOutItem = NULL;
						DeleteItem(pmsg->ShopItemPos, &pOutItem);
						if (pOutItem)
							ReLinkQuickPosition(pOutItem);
					}
				}
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(795), item->GetItemInfo()->ItemName);
			}
			if (pInfo->ItemType == 10 && pmsg->ShopItemBase.Param)
				AddUsedItemInfo(&pmsg->ShopItemBase);
			HERO->CalcShopItemOption(pmsg->ShopItemIdx, TRUE);
		}
		QUICKMGR->RefreshQickItem();
	}
}
void CItemManager::Item_ShopItem_Use_Nack(void* pMsg)
{
	MSG_ITEM_ERROR* pmsg = (MSG_ITEM_ERROR*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	switch (pmsg->ECode)
	{
	case eItemUseErr_Lock:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(751));
		break;
	case eItemUseErr_AlreadyUse:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(752));
		break;
	case eItemUseErr_Unabletime:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(757));
		break;
	case eItemUseErr_AlredyChange:
		//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(912) );
		break;
	case eItemuseErr_DontUseToday:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1079));
		break;
	case eItemUseErr_DontDupUse:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1076));
		break;
	case eItemUseErr_UseFull:
		CHATMGR->AddMsg(CTC_ALERT_YELLOW, CHATMGR->GetChatMsg(1435));
		break;
	}
}
void CItemManager::Item_ShopItem_Msg(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
}
void CItemManager::Item_ShopItem_Unseal_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	GAMEIN->GetInventoryDialog()->ItemUnSealing(pmsg->dwData);
	QUICKMGR->RefreshQickItem();
}
void CItemManager::Item_ShopItem_Unseal_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(777));
}
void CItemManager::Item_ShopItem_MoneyProtect(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(753));
	RemoveUsedItemInfo((WORD)pmsg->dwData);
}
void CItemManager::Item_ShopItem_ExpProtect(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(754));
	RemoveUsedItemInfo((WORD)pmsg->dwData);
}
void CItemManager::Item_ShopItem_MixUp(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(755));
	RemoveUsedItemInfo((WORD)pmsg->dwData);
}
void CItemManager::Item_ShopItem_StateMinus(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	GAMEIN->GetCharacterDialog()->RefreshPointInfo();
}
void CItemManager::Item_ShopItem_SavePoint_Add_Ack(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	GAMEIN->GetMoveDialog()->AddMoveInfo(&pmsg->Data);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(792));
}
void CItemManager::Item_ShopItem_SavePoint_Add_Nack(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(793));
}
void CItemManager::Item_ShopItem_SavePoint_Update_Ack(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
	GAMEIN->GetMoveDialog()->UpdateMoveInfo(&pmsg->Data);
}
void CItemManager::Item_ShopItem_SavePoint_Del_Ack(void* pMsg)
{
	SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
	GAMEIN->GetMoveDialog()->DelMoveInfo(&pmsg->Data);
}
void CItemManager::Item_ShopItem_ReviveOther_Syn(void* pMsg)
{
	SEND_REVIVEOTHER* pmsg = (SEND_REVIVEOTHER*)pMsg;

	if (!GAMEIN->GetReviveDialog()->IsActive())
	{
		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
		msg.dwObjectID = HEROID;
		msg.dwData1 = pmsg->TargetID;
		msg.dwData2 = eShopItemErr_Revive_NotReady;	
		NETWORK->Send(&msg, sizeof(msg));
		return;
	}
	GAMEIN->GetInventoryDialog()->SetReviveData(pmsg->TargetID, 0, 0);

	char buf[128] = { 0, };
	wsprintf(buf, CHATMGR->GetChatMsg(762), pmsg->TargetName);
	WINDOWMGR->MsgBox(MBI_REVIVECONFIRM, MBT_YESNO, buf);
}
void CItemManager::Item_ShopItem_ReviveOther_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	//CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData);
	//if (!pPlayer)		return;
}
void CItemManager::Item_ShopItem_ReviveOther_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	switch (pmsg->dwData)
	{
	case eShopItemErr_Revive_Fail:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(765));
		break;
	case eShopItemErr_Revive_NotDead:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(765));
		break;
	case eShopItemErr_Revive_NotUse:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
		break;
	case eShopItemErr_Revive_Refuse:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(787));
		break;
	case eShopItemErr_Revive_TooFar:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(789));
		break;
	case eShopItemErr_Revive_TimeOver:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(790));
		break;
	case eShopItemErr_Revive_NotReady:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(920));
		break;
	}

	cDialog* pDlg = WINDOWMGR->GetWindowForID(MBI_REVIVECONFIRM);
	if (pDlg)
		WINDOWMGR->AddListDestroyWindow(pDlg);
}
void CItemManager::Item_ShopItem_Avatar_PutOn(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
	if (pObject && pObject->GetObjectKind() == eObjectKind_Player)
	{
		APPEARANCEMGR->SetAvatarItem((CPlayer*)pObject, (WORD)pmsg->dwData2);
	}
	else
	{
		ASSERT(0);
	}
}
void CItemManager::Item_ShopItem_Avatar_TakeOff(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CObject* pObject = (CObject*)OBJECTMGR->GetObject(pmsg->dwData1);
	if (pObject && pObject->GetObjectKind() == eObjectKind_Player)
	{
		APPEARANCEMGR->SetAvatarItem((CPlayer*)pObject, (WORD)pmsg->dwData2, FALSE);
	}
	else
	{
		ASSERT(0);
	}
}
void CItemManager::Item_ShopItem_Avatar_Use(void* pMsg)
{
	SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;
	AddUsedItemInfo(&pmsg->ShopItemBase);
}
void CItemManager::Item_ShopItem_Avatar_Info(void* pMsg)
{
	SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
	if (!pPlayer)		return;

	AVATARITEM* pInfo = GAMERESRCMNGR->m_AvatarEquipTable.GetData(pmsg->ItemIdx);
	if (!pInfo)		return;

	if (pmsg->PlayerId == HERO->GetID())
		HERO->CheckShopItemUseInfoToInventory(pmsg->ItemIdx, pmsg->ItemPos);
	//[图片称号物品结束使用设置][By:十里坡剑神][QQ:112582793][2017/11/29]
	ITEM_INFO* Item = GetItemInfo(pmsg->ItemIdx);
	if (pInfo->Position == eAvatar_Effect&&Item)
	{
		if (pmsg->Avatar[eAvatar_Effect] == 0)
			pPlayer->SetImageNameBalloon(0);
		else
			pPlayer->SetImageNameBalloon(Item->wSetItemKind);
		pPlayer->CalcAvatarOption();
		return;
	}
	else if (pInfo->Position == eAvatar_Hair)
	{
		int MotionIdx = -1;
		WORD Weapontype = pPlayer->GetWeaponEquipType();
		if (Weapontype == WP_GUM || Weapontype == WP_GWUN || Weapontype == WP_GUNG || Weapontype == WP_AMGI ||
			Weapontype == WP_EVENT || Weapontype == WP_EVENT_HAMMER)
			MotionIdx = eMotion_Item_ChangeHair_1;
		else if (Weapontype == WP_DO)
			MotionIdx = eMotion_Item_ChangeHair_2;
		else if (Weapontype == WP_CHANG)
			MotionIdx = eMotion_Item_ChangeHair_3;

		if (pPlayer->GetID() == HERO->GetID())
			MOVEMGR->HeroMoveStop();

		if (pPlayer->GetState() == eObjectState_None)
		{
			pPlayer->SetItemUseMotionIdx(MotionIdx);
			pPlayer->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);

		}

		OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_ItemUse);
		OBJECTSTATEMGR->EndObjectState(pPlayer, eObjectState_ItemUse, pPlayer->GetEngineObject()->GetAnimationTime(MotionIdx));
		memcpy(pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max);
	}
	else
	{
		memcpy(pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max);
		APPEARANCEMGR->InitAppearance(pPlayer);
	}
	pPlayer->CalcAvatarOption();
}
void CItemManager::Item_ShopItem_Avatar_Use_Ack(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData2);
	if (!pItem)		return;
	pItem->SetUseParam(1);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(779), pItem->GetItemInfo()->ItemName);
}
void CItemManager::Item_ShopItem_Avatar_Use_AckDB(void* pMsg)
{
	SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ShopItemPos);
	if (!pItem)		return;
	pItem->SetUseParam(1);

	char buf[256] = { 0, };
	wsprintf(buf, CHATMGR->GetChatMsg(779), pItem->GetItemInfo()->ItemName);
	CHATMGR->AddMsg(CTC_SYSMSG, buf);
	AddUsedItemInfo(&pmsg->ShopItemBase);
}
void CItemManager::Item_ShopItem_Avatar_Use_Nack(void* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData2);
	if (!pItem)		return;
	pItem->SetUseParam(0);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(780), pItem->GetItemInfo()->ItemName);
}
void CItemManager::Item_ShopItem_Avatar_Discard(void* pMsg)
{
	SEND_AVATARITEM_INFO* pmsg = (SEND_AVATARITEM_INFO*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->PlayerId);
	if (!pPlayer)		return;

	memcpy(pPlayer->GetShopItemStats()->Avatar, pmsg->Avatar, sizeof(WORD)*eAvatar_Max);
	APPEARANCEMGR->InitAppearance(pPlayer);
}
void CItemManager::Item_ShopItem_Equip_UseInfoDB(void* pMsg)
{
	SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ShopItemPos);
	if (!pItem)
	{
		pItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos(pmsg->ShopItemPos);
	}

	if (!pItem)
		return;

	AddUsedItemInfo(&pmsg->ShopItemBase);
}
void CItemManager::Item_ShopItem_Pet_Use(void* pMsg)
{
	SEND_SHOPITEM_USEONE* pmsg = (SEND_SHOPITEM_USEONE*)pMsg;
	AddUsedItemInfo(&pmsg->ShopItemBase);
}
void CItemManager::Item_ShopItem_Effect(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
	if (!pPlayer)		return;

	EFFECTMGR->StartPlayerEffectProcess(pPlayer, pmsg->dwData2);
}
void CItemManager::Item_ShopItem_SlotoverItem(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if (pmsg->dwData > SLOT_SHOPITEM_IMSI)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(796));
	}
}
void CItemManager::Item_ShopItem_ProtectAll(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(729));
	HERO->GetShopItemStats()->ProtectCount = (char)pmsg->dwData;
}
void CItemManager::Item_ShopItem_Chase_Ack(void* pMsg)
{
	SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;

	//if( MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP )
	if (MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1457));
		return;
	}

	VECTOR3 pos;
	pmsg->Pos.Decompress(&pos);
	if (GAMEIN->GetChaseDlg()->InitMiniMap(pmsg->MapNum, (WORD)pos.x, (WORD)pos.z, pmsg->WandtedName, pmsg->EventMapNum))
	{
		GAMEIN->GetChaseDlg()->SetActive(TRUE);
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(908));
	}
}
void CItemManager::Item_ShopItem_Chase_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	GAMEIN->GetChaseDlg()->SetActive(FALSE);

	if (pmsg->dwData == 1)
		WINDOWMGR->MsgBox(MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(907));
	else if (pmsg->dwData == 2)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(908));
	else if (pmsg->dwData == 3)
		WINDOWMGR->MsgBox(MBI_CHASE_NACK, MBT_OK, CHATMGR->GetChatMsg(921));
}
void CItemManager::Item_ShopItem_Chase_Tracking(void* pMsg)
{

}
void CItemManager::Item_ShopItem_NChange_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	WINDOWMGR->MsgBox(MBI_CHANGENAME_ACK, MBT_OK, CHATMGR->GetChatMsg(917));

	for (int i = 0; i < (SLOT_SHOPINVEN_NUM/*/2*/ + TABCELL_SHOPINVEN_PLUS_NUM); i++)
	{
		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, i + TP_SHOPINVEN_START);
		if (!pItemBase)	continue;

		if (pItemBase->dwDBIdx == GAMEIN->GetNameChangeDlg()->GetItemDBIdx())
		{
			CItem* pItem = NULL;
			DeleteItem(i + TP_SHOPINVEN_START, &pItem);
			break;
		}
	}
}
void CItemManager::Item_ShopItem_NChange_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	switch (pmsg->dwData)
	{
	case 6:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(912));
		break;
	case 5:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(916));
		break;
	case 4:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(913));
		break;
	case 3:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(914));
		break;
	case 2:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(915));
		break;
	}
}
void CItemManager::Item_ShopItem_CharChange_Ack(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	GAMEIN->GetCharChangeDlg()->Reset(TRUE);
	GAMEIN->GetCharChangeDlg()->SetActive(FALSE);
}
void CItemManager::Item_ShopItem_CharChange_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	switch (pmsg->dwData)
	{
	case 1:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
		break;
	case 2:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1185));
		break;
	case 3:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1185));
		break;
	case 4:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1185));
		break;
	case 5:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
		break;
	case 6:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1184));
		break;
	}

	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	GAMEIN->GetCharChangeDlg()->Reset(FALSE);
	GAMEIN->GetCharChangeDlg()->SetActive(FALSE);
}
void CItemManager::Item_ShopItem_CharChange(void* pMsg)
{
	SEND_CHARACTERCHANGE_INFO* pmsg = (SEND_CHARACTERCHANGE_INFO*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->Param);
	if (!pPlayer)		return;

	pPlayer->SetCharChangeInfo(&pmsg->Info);
}
void CItemManager::Item_ShopItem_Seal_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if (GAMEIN->GetSealDlg())
		GAMEIN->GetSealDlg()->ItemSealAck();
}
void CItemManager::Item_ShopItem_Seal_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if (GAMEIN->GetSealDlg())
		GAMEIN->GetSealDlg()->ItemSealNAck();
}
void CItemManager::Item_ShopItem_JobChange_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	WINDOWMGR->MsgBox(MBI_JOBCHANGE_ACK, MBT_OK, CHATMGR->GetChatMsg(1143));
	CChangeJobDialog* pDlg = GAMEIN->GetChangeJobDlg();
	if (!pDlg)		return;

	for (int i = 0; i < (SLOT_SHOPINVEN_NUM/*/2*/ + TABCELL_SHOPINVEN_PLUS_NUM); i++)
	{
		ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, i + TP_SHOPINVEN_START);
		if (!pItemBase)	continue;

		if (pItemBase->dwDBIdx == pDlg->GetItemDBIdx() && pItemBase->Position == pDlg->GetItemPos())
		{
			CItem* pItem = NULL;
			DeleteItem(i + TP_SHOPINVEN_START, &pItem);
			break;
		}
	}
}
void CItemManager::Item_ShopItem_JobChange_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	switch (pmsg->dwData)
	{
	case 1:
		break;
	case 2:
		break;
	}

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
	GAMEIN->GetChangeJobDlg()->CancelChangeJob();
}
void CItemManager::Item_ShopItem_ReinforceReset_Ack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if (GAMEIN->GetReinforceResetDlg())
		GAMEIN->GetReinforceResetDlg()->ItemResetAck();
}
void CItemManager::Item_ShopItem_ReinforceReset_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	if (GAMEIN->GetReinforceResetDlg())
		GAMEIN->GetReinforceResetDlg()->ItemResetNAck();

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1233));
}
void CItemManager::Item_ShopItem_RareCreate_Ack(void* pMsg)
{
	MSG_ITEM_RAREITEM_GET* pmsg = (MSG_ITEM_RAREITEM_GET*)pMsg;

	CItem* pItem = NULL;
	pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->RareItemBase.Position);

	if (!pItem)
	{
		GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
		return;
	}

	ITEMBASE* pItemBase = (ITEMBASE*)pItem->GetItemBaseInfo();
	if (!pItemBase)
	{
		GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
		return;
	}

	if (pItemBase->RareIdx)
		RemoveItemRareOption(pItem->GetItemBaseInfo()->RareIdx);
	pItemBase->RareIdx = pmsg->RareInfo.dwRareOptionIdx;
	AddItemRareOption(&pmsg->RareInfo);

	GAMEIN->GetRareCreateDlg()->ItemRareCreateAck();
	QUICKMGR->RefreshQickItem();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1235));
}
void CItemManager::Item_ShopItem_RareCreate_Nack(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	GAMEIN->GetRareCreateDlg()->ItemRareCreateNAck();
	if (pmsg->dwData == 12)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(728));
	else
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1236));
}
void CItemManager::Item_ShopItem_Shout_SendAck(void* pMsg)
{
	SEND_SHOUTRECEIVE* pmsg = (SEND_SHOUTRECEIVE*)pMsg;
	WINDOWMGR->MsgBox(MBI_SHOUT_ACK, MBT_OK, CHATMGR->GetChatMsg(905), pmsg->Receive.Time / 60 + 1, pmsg->Receive.Count);
}
void CItemManager::Item_EventItem_Use(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);

	if (pmsg->dwData2 == EVENT_ITEM_FIRECRACKER)
	{
		EFFECTMGR->StartPlayerEffectProcess(pPlayer, EVENT_EFFECT_FIRECRACKER);
	}
	if (pmsg->dwData2 == 53151)
	{
		EFFECTMGR->StartPlayerEffectProcess(pPlayer, FindEffectNum("m_ba_082.beff"));
	}
}
void CItemManager::Item_Query_Equi_Ack(void* pMsg)
{
	MSG_QUERY_EQUIPINF* pmsg = (MSG_QUERY_EQUIPINF*)pMsg;

	for (int i = 0; i < eWearedItem_Max; i++)
	{
		if (HERO->m_OtherPlayerEquip[i] != NULL)
		{
			GAMEIN->GetCharacterEquipInfoDlg()->DeleteItem(&HERO->m_OtherPlayerEquip[i]);

			ITEMMGR->ItemDelete(HERO->m_OtherPlayerEquip[i]);

			HERO->m_OtherPlayerEquip[i] = NULL;

		}

		CItem* pItem = MakeNewItem(&pmsg->WearedItem[i], "MP_ITEM_QUERY_EQUI_ACK");

		GAMEIN->GetCharacterEquipInfoDlg()->AddItem(pItem);

		if (!pItem)
		{
			continue;
		}

		ITEMMGR->AddItemRareOption(&pmsg->WearedItemRateInfo[i]);

		ITEMMGR->AddItemOption(&pmsg->WearedItemOptionInfo[i]);

		ITEMMGR->SetToolTipIcon(pItem, GetItemOption(pItem->GetDurability()), GetItemRareOption(pItem->GetRareness()), pItem->GetItemBaseInfo()->dwDBIdx,FALSE, TRUE);// 2014-11-15 附
		HERO->m_OtherPlayerEquip[i] = pItem;
	}
	GAMEIN->GetCharacterEquipInfoDlg()->AddShopItem(pmsg);
	GAMEIN->GetCharacterEquipInfoDlg()->SetAttDef(pmsg);
	GAMEIN->GetCharacterEquipInfoDlg()->SetActive(TRUE);

}
void CItemManager::Item_LockEx_Ack(void* pMsg)
{
	MSG_DWORD2BYTE* pmsg = (MSG_DWORD2BYTE*)pMsg;

	CItem * pItem = GAMEIN->GetItemLockDialog()->GetLockItem();

	if (pItem)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:十里坡剑神][QQ:112582793][2018/4/13]
		NETWORK->Send(&msg, sizeof(msg));
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2118), pInfo->ItemName);
	}

	CItem * pTargetItem = NULL;

	pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		pTargetItem->SetStatic(pmsg->bData);  //[修改为泡点类型][By:十里坡剑神][QQ:112582793][2017/12/16]
		pTargetItem->SetActive(TRUE);
	}

	GAMEIN->GetItemLockDialog()->Release();
	if (pmsg->bData == ITEM_STATIC_LUCK)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2119));
}
void CItemManager::Item_LockEx_Nack(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CItem * pItem = GAMEIN->GetItemLockDialog()->GetLockItem();

	if (pItem)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:十里坡剑神][QQ:112582793][2018/4/13]
		NETWORK->Send(&msg, sizeof(msg));
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2118), pInfo->ItemName);
	}

	CItem * pTargetItem = NULL;

	pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		pTargetItem->SetStatic(0);

		pTargetItem->SetActive(TRUE);
	}

	GAMEIN->GetItemLockDialog()->Release();

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2120));
}
void CItemManager::Item_UnLockEx_Ack(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CItem * pItem = GAMEIN->GetItemUnLockDialog()->GetLockItem();

	if (pItem)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:十里坡剑神][QQ:112582793][2018/4/13]
		NETWORK->Send(&msg, sizeof(msg));
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2118), pInfo->ItemName);
	}

	CItem * pTargetItem = NULL;

	pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem && (pItem->GetItemIdx() == eItemUnLockStoneForPD ||
		pItem->GetItemIdx() == eItemUnLockStone))
	{
		pTargetItem->SetStatic(0);

		pTargetItem->SetActive(TRUE);
	}
	GAMEIN->GetItemUnLockDialog()->Release();
	ITEMMGR->RefreshItemToolTip(pTargetItem->GetDBIdx());
	if (pItem->GetItemIdx() == eItemUnLockStoneForPD)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1238));
	else if (pItem->GetItemIdx() == eItemUnLockStone)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2122));
}
void CItemManager::Item_UnLockEx_Nack(void* pMsg)
{
	MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;

	CItem * pItem = GAMEIN->GetItemUnLockDialog()->GetLockItem();

	if (pItem)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:十里坡剑神][QQ:112582793][2018/4/13]
		NETWORK->Send(&msg, sizeof(msg));
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2118), pInfo->ItemName);
	}

	CItem * pTargetItem = NULL;

	pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		pTargetItem->SetStatic(3);

		pTargetItem->SetActive(TRUE);
	}

	GAMEIN->GetItemUnLockDialog()->Release();

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2123));
}
void CItemManager::Item_Grow_Ack(void* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CItem * pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		pTargetItem->SetGrow(pmsg->dwData3);

		ITEMMGR->RefreshItem(pTargetItem);
		STATSMGR->CalcItemStats(HERO);
		GAMEIN->GetCharacterDialog()->SetAttackRate();
		GAMEIN->GetCharacterDialog()->SetDefenseRate();
		GAMEIN->GetCharacterDialog()->SetGenGol();
		GAMEIN->GetCharacterDialog()->SetMinChub();
		GAMEIN->GetCharacterDialog()->SetCheRyuk();
		GAMEIN->GetCharacterDialog()->SetSimMek();
		GAMEIN->GetCharacterDialog()->SetCritical();
		GAMEIN->GetCharacterDialog()->UpdateData();
		APPEARANCEMGR->InitAppearance(HERO);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2127), pmsg->dwData3);  // 提示值
	}
}
void CItemManager::NetworkMsgParse(WORD Protocol, void* pMsg)
{//[栈空间不足][BY:十里坡剑神][QQ:112582793][2019-8-18][4:53:41]
	switch (Protocol)
	{
	case MP_ITEM_TOTALINFO_LOCAL:				Item_TotalInfo_Local(pMsg);					break;
	case MP_ITEM_PYOGUK_ITEM_INFO_ACK:			Item_Pyoguk_Info_Ack(pMsg);					break;
	case MP_ITEM_PETINVEN_INFO_ACK:				Item_PetInven_Info_Ack(pMsg);				break;
	case MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY:		Item_Monster_RareObtain_Notify(pMsg);		break;
	case MP_ITEM_REINFORCE_SUCCESS_ACK:			Item_Reinforce_Success_Ack(pMsg);			break;
	case MP_ITEM_REINFORCE_FAILED_ACK:			Item_Reinforce_Failed_Ack(pMsg);			break;
	case MP_ITEM_REINFORCE_NACK:				Item_Reinforce_Nack(pMsg);					break;
	case MP_ITEM_REINFORCE_WITHSHOPITEM_NACK:	Item_Reinforce_WithShopItem_Nack(pMsg);		break;
	case MP_ITEM_MOVE_ACK:						Item_Move_Ack(pMsg);						break;
	case MP_ITEM_COMBINE_ACK:					Item_Combine_Ack(pMsg);						break;
	case MP_ITEM_DIVIDE_ACK:					Item_Divide_Ack(pMsg);						break;
	case MP_ITEM_MONSTER_OBTAIN_NOTIFY:			Item_Monster_Obtain_Notify(pMsg);			break;
	case MP_ITEM_MOVE_NACK:						Item_Move_Nack(pMsg);						break;
	case MP_ITEM_DIVIDE_NACK:					Item_Divide_Nack(pMsg);						break;
	case MP_ITEM_COMBINE_NACK:					Item_Combine_Nack();						break;
	case MP_ITEM_DISCARD_NACK:					Item_Discard_Nack(pMsg);					break;
	case MP_ITEM_DISCARD_ACK:					Item_Discard_Ack(pMsg);						break;
	case MP_ITEM_USE_NOTIFY:					Item_Use_Notify(pMsg);						break;
	case MP_ITEM_USE_ACK:						Item_Use_Ack(pMsg);							break;
	case MP_ITEM_MIX_ADDITEM_ACK:				Item_Mix_AddItem_Ack(pMsg);					break;
	case MP_ITEM_MIX_ADDITEM_NACK:				Item_Mix_AddItem_Nack(pMsg);				break;	
	case MP_ITEM_MIX_SUCCESS_ACK:				Item_Mix_Success_Ack(pMsg);					break;
	case MP_ITEM_MIX_BIGFAILED_ACK:				Item_Mix_BigFailed_Ack(pMsg);				break;
	case MP_ITEM_MIX_FAILED_ACK:				Item_Mix_Failed_Ack(pMsg);					break;
	case MP_ITEM_MIX_MSG:						Item_Mix_Msg(pMsg);							break;
	case MP_ITEM_TPM_ADDITEM_ACK:				Item_TPM_AddItem_Ack(pMsg);					break;
	case MP_ITEM_TPM_ADDITEM_NACK:				Item_TPM_AddItem_Nack(pMsg);				break;
	case MP_ITEM_TPM_SUCCESS_ACK:				Item_TPM_Success_Ack(pMsg);					break;
	case MP_ITEM_TPM_BIGFAILED_ACK:				Item_TPM_BigFailed_Ack(pMsg);				break;
	case MP_ITEM_TPM_FAILED_ACK:				Item_TPM_Failed_Ack(pMsg);					break;
	case MP_ITEM_TPM_MSG:						Item_TPM_Msg(pMsg);							break;
	case MP_ITEM_MALL_MONEY:					Item_Mall_Money(pMsg);						break;
	case MP_ITEM_GOLD_MONEY:					Item_Gold_Money(pMsg);						break;
	case MP_ITEM_SW_MONEY:						Item_SW_Money(pMsg);						break;
	case MP_ITEM_UPGRADE_SUCCESS_ACK:			Item_UpGrade_Success_Ack(pMsg);				break;
	case MP_ITEM_UPGRADE_NACK:					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(176));	break;
	case MP_ITEM_USE_NACK:						Item_Use_Nack(pMsg);						break;
	case MP_ITEM_SELL_ACK:						Item_Sell_Ack(pMsg);						break;
	case MP_ITEM_SELL_NACK:						Item_Sell_Nack(pMsg);						break;
	case MP_ITEM_BUY_ACK:						Item_Buy_Ack(pMsg);							break;
	case MP_ITEM_BUY_NACK:						Item_Buy_Nack(pMsg);						break;
	case MP_ITEM_DEALER_ACK:					Item_Dealer_Ack(pMsg);						break;
	case MP_ITEM_DEALER_NACK:					Item_Dealer_Nack(pMsg);						break;
	case MP_ITEM_APPEARANCE_CHANGE:				Item_Appearance_Change(pMsg);				break;
	case MP_ITEM_MONEY:							Item_Money(pMsg);							break;
	case MP_ITEM_MONEY_ERROR:					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(115));	break;
	case MP_ITEM_OBTAIN_MONEY:					Item_Obtain_Money(pMsg);					break;
	case MP_ITEM_DIVIDE_NEWITEM_NOTIFY:			Item_Divide_NewItem_Notify(pMsg);			break;
	case MP_ITEM_GUILD_MOVE_ACK:				Item_Guild_Move_Ack(pMsg);					break;
	case MP_ITEM_GUILD_MOVE_NACK:				Item_Guild_Move_Nack(pMsg);					break;
	case MP_ITEM_GUILD_WAREHOUSE_INFO_ACK:		Item_Guild_WareHouse_Info_Ack(pMsg);		break;
	case MP_ITEM_GUILD_WAREHOUSE_INFO_NACK:		GAMEIN->GetGuildWarehouseDlg()->SetLock(FALSE);	break;
	case MP_ITEM_DELETEFROM_GUILDWARE_NOTIFY:	Item_DeleteFrom_GuildWare_Notify(pMsg);		break;
	case MP_ITEM_ADDTO_GUILDWARE_NOTIFY:		Item_AddTo_GuildWare_Notify(pMsg);			break;
	case MP_ITEM_PYOGUK_MOVE_ACK:				Item_Pyoguk_Move_Ack(pMsg);					break;
	case MP_ITEM_DISSOLUTION_GETITEM:			Item_Dissolution_GetItem(pMsg);				break;
	case MP_ITEM_DISSOLUTION_ACK:				Item_Dissolution_Ack(pMsg);					break;
	case MP_ITEM_DISSOLUTION_NACK:				Item_Dissolution_Nack(pMsg);				break;
	case MP_ITEM_ERROR_NACK:					Item_Error_Nack(pMsg);						break;
	case MP_ITEM_CHANGEITEM_NACK:				Item_ChangeItem_Nack();						break;
	case MP_ITEM_USE_CHANGEITEM_ACK:			Item_Use_ChangeItem_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_INFO_ACK:				Item_ShopItem_Info_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_USEDINFO:				Item_ShopItem_UsedInfo(pMsg);				break;
	case MP_ITEM_SHOPITEM_MPINFO:				Item_ShopItem_MPInfo(pMsg);					break;
	case MP_ITEM_SHOPITEM_USEEND:				Item_ShopItem_UseEnd(pMsg);					break;
	case MP_ITEM_SHOPITEM_ONEMINUTE:			Item_ShopItem_OneMinute(pMsg);				break;
	case MP_ITEM_SHOPITEM_USE_ACK:				Item_ShopItem_Use_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_USE_NACK:				Item_ShopItem_Use_Nack(pMsg);				break;
	case MP_ITEM_SHOPITEM_MSG:					break; Item_ShopItem_Msg(pMsg);				break;
	case MP_ITEM_SHOPITEM_UNSEAL_ACK:			Item_ShopItem_Unseal_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_UNSEAL_NACK:			Item_ShopItem_Unseal_Nack(pMsg);			break;
	case MP_ITEM_SHOPITEM_MONEYPROTECT:			Item_ShopItem_MoneyProtect(pMsg);			break;
	case MP_ITEM_SHOPITEM_EXPPROTECT:			Item_ShopItem_ExpProtect(pMsg);				break;
	case MP_ITEM_SHOPITEM_MIXUP:				Item_ShopItem_MixUp(pMsg);					break;
	case MP_ITEM_SHOPITEM_STATEMINUS:			Item_ShopItem_StateMinus(pMsg);				break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_ACK:	Item_ShopItem_SavePoint_Add_Ack(pMsg);		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_NACK:	Item_ShopItem_SavePoint_Add_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_ACK:	Item_ShopItem_SavePoint_Update_Ack(pMsg);	break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_DEL_ACK:	Item_ShopItem_SavePoint_Del_Ack(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_SYN:		Item_ShopItem_ReviveOther_Syn(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_ACK:		Item_ShopItem_ReviveOther_Ack(pMsg);		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_NACK:		Item_ShopItem_ReviveOther_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_AVATAR_PUTON:			Item_ShopItem_Avatar_PutOn(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_TAKEOFF:		Item_ShopItem_Avatar_TakeOff(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_USE:			Item_ShopItem_Avatar_Use(pMsg);				break;
	case MP_ITEM_SHOPITEM_AVATAR_INFO:			Item_ShopItem_Avatar_Info(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_ACK:		Item_ShopItem_Avatar_Use_Ack(pMsg);			break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_ACKDB:		Item_ShopItem_Avatar_Use_AckDB(pMsg);		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_NACK:		Item_ShopItem_Avatar_Use_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_AVATAR_DISCARD:		Item_ShopItem_Avatar_Discard(pMsg);			break;
	case MP_ITEM_SHOPITEM_EQUIP_USEINFODB:		Item_ShopItem_Equip_UseInfoDB(pMsg);		break;
	case MP_ITEM_SHOPITEM_PET_USE:				Item_ShopItem_Pet_Use(pMsg);				break;
	case MP_ITEM_SHOPITEM_EFFECT:				Item_ShopItem_Effect(pMsg);					break;
	case MP_ITEM_SHOPITEM_SLOTOVERITEM:			Item_ShopItem_SlotoverItem(pMsg);			break;
	case MP_ITEM_SHOPITEM_PROTECTALL:			Item_ShopItem_ProtectAll(pMsg);				break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:			Item_ShopItem_Chase_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:			Item_ShopItem_Chase_Nack(pMsg);				break;
	case MP_ITEM_SHOPITEM_CHASE_TRACKING:		break; Item_ShopItem_Chase_Tracking(pMsg);  break;
	case MP_ITEM_SHOPITEM_NCHANGE_ACK:			Item_ShopItem_NChange_Ack(pMsg);			break;
	case MP_ITEM_SHOPITEM_NCHANGE_NACK:			Item_ShopItem_NChange_Nack(pMsg);			break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_ACK:		Item_ShopItem_CharChange_Ack(pMsg);			break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_NACK:		Item_ShopItem_CharChange_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_CHARCHANGE:			Item_ShopItem_CharChange(pMsg);				break;
	case MP_ITEM_SHOPITEM_SEAL_ACK:				Item_ShopItem_Seal_Ack(pMsg);				break;
	case MP_ITEM_SHOPITEM_SEAL_NACK:			Item_ShopItem_Seal_Nack(pMsg);				break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_ACK:		Item_ShopItem_JobChange_Ack(pMsg);			break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_NACK:		Item_ShopItem_JobChange_Nack(pMsg);			break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_ACK:	Item_ShopItem_ReinforceReset_Ack(pMsg);		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_NACK:	Item_ShopItem_ReinforceReset_Nack(pMsg);	break;
	case MP_ITEM_SHOPITEM_RARECREATE_ACK:		Item_ShopItem_RareCreate_Ack(pMsg);			break;
	case MP_ITEM_SHOPITEM_RARECREATE_NACK:		Item_ShopItem_RareCreate_Nack(pMsg);		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDACK:		Item_ShopItem_Shout_SendAck(pMsg);			break;
	case MP_ITEM_SHOPITEM_SHOUT_NACK:			WINDOWMGR->MsgBox(MBI_SHOUT_NACK, MBT_OK, CHATMGR->GetChatMsg(904));	break;
	case MP_ITEM_EVENTITEM_USE:					Item_EventItem_Use(pMsg);					break;
	case MP_ITEM_FULLINVEN_ALERT:				CHATMGR->AddMsg(CTC_ALERT_YELLOW, CHATMGR->GetChatMsg(1435));			break;
	case MP_ITEM_QUERY_EQUI_ACK:				Item_Query_Equi_Ack(pMsg);					break;
	case MP_ITEM_LOCKEX_ACK:					Item_LockEx_Ack(pMsg);						break;
	case MP_ITEM_LOCKEX_NACK:					Item_LockEx_Nack(pMsg);						break;
	case MP_ITEM_UNLOCKEX_ACK:					Item_UnLockEx_Ack(pMsg);					break;
	case MP_ITEM_UNLOCKEX_NACK:					Item_UnLockEx_Nack(pMsg);					break;
	case MP_ITEM_GROW_ACK:						Item_Grow_Ack(pMsg);						break;
	default:
		break;
	}

}

void CItemManager::ItemExt_ShopItem_Curse_Cancellation_AddItem_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
}
void CItemManager::ItemExt_ShopItem_Curse_Cancellation_AddItem_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
	pItem->SetLock(FALSE);
	GAMEIN->GetUniqueItemCurseCancellationDlg()->Release();
}
void CItemManager::ItemExt_ShopItem_Curse_Cancellation_Ack(void* pMsg)
{
	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
			ASSERT(pPreItem);
		}
		pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_BUY_ACK");
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if (!rt)
		{
			ASSERT(0);
		}
	}

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1627));

	if (GAMEIN->GetUniqueItemCurseCancellationDlg()->IsActive() == TRUE)
	{
		GAMEIN->GetUniqueItemCurseCancellationDlg()->SetActive(FALSE);
	}
}
void CItemManager::ItemExt_ShopItem_Curse_Cancellation_DeleteItem(void* pMsg)
{
	MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;

	GAMEIN->GetUniqueItemCurseCancellationDlg()->Release(FALSE);

	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);

	DeleteItemofTable(wTableIdx, pmsg->TargetPos);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	SetDisableDialog(FALSE, eItemTable_PetInven);
}
void CItemManager::ItemExt_UniqueItem_Mix_AddItem_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->ItemInfo.Position);
}
void CItemManager::ItemExt_UniqueItem_Mix_AddItem_Nack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->wData1);
	pItem->SetLock(FALSE);
	GAMEIN->GetUniqueItemMixDlg()->Release();
}
void CItemManager::ItemExt_UniqueItem_Mix_DeleteItem(void* pMsg)
{
	MSG_ITEM_DISCARD_ACK* pmsg = (MSG_ITEM_DISCARD_ACK*)pMsg;

	GAMEIN->GetUniqueItemMixDlg()->Release(FALSE);

	WORD wTableIdx = GetTableIdxForAbsPos(pmsg->TargetPos);

	DeleteItemofTable(wTableIdx, pmsg->TargetPos);
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);
}
void CItemManager::ItemExt_UniqueItem_Mix_Ack(void* pMsg)
{
	SetDisableDialog(FALSE, eItemTable_Inventory);
	SetDisableDialog(FALSE, eItemTable_Pyoguk);
	SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	SetDisableDialog(FALSE, eItemTable_Shop);

	ITEMOBTAINARRAY * pmsg = (ITEMOBTAINARRAY *)pMsg;

	CItem* pItem = NULL;
	CItem* pPreItem = NULL;
	for (int i = 0; i < pmsg->ItemNum; ++i)
	{
		if (pPreItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->GetItem(i)->Position))
		{
			DeleteItem(pmsg->GetItem(i)->Position, &pPreItem);
			ASSERT(pPreItem);
		}
		pItem = MakeNewItem(pmsg->GetItem(i), "MP_ITEM_BUY_ACK");
		BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
		if (!rt)
		{
			ASSERT(0);
		}
	}
}
void CItemManager::ItemExt_SkinItem_Select_Ack(void* pMsg)
{
	SEND_SKIN_INFO* pmsg = (SEND_SKIN_INFO*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if (!pPlayer)		return;

	memcpy(&pPlayer->GetShopItemStats()->wSkinItem, pmsg->wSkinItem, sizeof(WORD)*eSkinItem_Max);

	APPEARANCEMGR->InitAppearance(pPlayer);

	pPlayer->StartSkinDelayTime();
}
void CItemManager::ItemExt_SkinItem_Select_Nack(void* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if (!pPlayer)		return;

	switch (pmsg->dwData1)
	{
	case eSkinResult_Fail:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
		break;
	case eSkinResult_DelayFail:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1355), pmsg->dwData2 / 1000 + 1);
		break;
	case eSkinResult_LevelFail:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(238), pmsg->dwData3);
		break;
	}
}
void CItemManager::ItemExt_SkinItem_Discard_Ack(void* pMsg)
{
	SEND_SKIN_INFO* pmsg = (SEND_SKIN_INFO*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	if (!pPlayer)		return;

	memcpy(&pPlayer->GetShopItemStats()->wSkinItem, pmsg->wSkinItem, sizeof(WORD)*eSkinItem_Max);

	if (pPlayer->GetObjectKind() == eObjectKind_Player)
	{
		GAMEIN->GetSkinSelectDlg()->SetActive(FALSE);
		GAMEIN->GetCostumeSkinSelectDlg()->SetActive(FALSE);
	}

	APPEARANCEMGR->InitAppearance(pPlayer);
}
void CItemManager::ItemExt_ShopItem_Decoration_On(void* pMsg)
{
	CObject* pObject;
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
	ASSERT(pObject);

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->GetShopItemStats()->dwStreetStallDecoration = pmsg->dwData1;


		if (pmsg->dwData1 == 0)
		{
			OBJECTMGR->SetRemoveDecorationInfo((CPlayer*)pObject);
		}
		else
		{
			if (((CPlayer*)pObject)->GetState() == eObjectState_StreetStall_Owner)
				STREETSTALLMGR->AddStreetStallDecoration((CPlayer*)pObject, pmsg->dwData2);
		}
	}
}
void CItemManager::ItemExt_Abrasion_Ack(void* pMsg)
{//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CItem * pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		pTargetItem->SetAbrasion(pmsg->dwData3);
		pTargetItem->SetMaxItemAbrasion(pmsg->dwData4);
		ITEMMGR->RefreshItem(pTargetItem);
		APPEARANCEMGR->InitAppearance(HERO);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1239), pmsg->dwData3);
	}
}
void CItemManager::ItemExt_Maker_Ack(void* pMsg)
{//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	MSG_DWORD2CHAR* pmsg = (MSG_DWORD2CHAR*)pMsg;
	CItem * pTargetItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);

	if (pTargetItem)
	{
		if (pmsg->bType == 1)
			pTargetItem->SetMaker(pmsg->Maker);
		else
			pTargetItem->SetMaker1(pmsg->Maker);

		ITEMMGR->RefreshItem(pTargetItem);
		APPEARANCEMGR->InitAppearance(HERO);
	}
}
void CItemManager::ItemExt_GoldItemUse_Nack(void* pMsg)
{//元宝物品使用失败，客户端文件被修改  by:十里坡剑神  QQ:418914674
	MSGGOLDITEM* pmsg = (MSGGOLDITEM*)pMsg;

	ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->dwItemPos);
	if (!pItemBase)	return;

	if (pItemBase->dwDBIdx == pmsg->dwItemDBidx)
	{
		if (pItemBase->Durability > 1)
		{
			GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->dwItemPos, -1);
			return;
		}
		CItem* pItem = NULL;
		DeleteItem(pmsg->dwItemPos, &pItem);
		return;
	}
}
void CItemManager::ItemExt_GoldItemUse_Ack(void* pMsg)
{//[元宝物品使用成功协议处理][By:十里坡剑神][QQ:112582793][2018/2/2]
	MSGGOLDITEM* pmsg = (MSGGOLDITEM*)pMsg;

	ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->dwItemPos);
	if (!pItemBase)	return;

	if (pItemBase->dwDBIdx == pmsg->dwItemDBidx)
	{
		if (pItemBase->Durability > 1)
		{
			GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->dwItemPos, -1);
			return;
		}
		CItem* pItem = NULL;
		DeleteItem(pmsg->dwItemPos, &pItem);
	}
}
void CItemManager::ItemExt_NJQuest_Ack(void* pMsg)
{
	SEND_SHOPITEM_BASEINFO* pmsg = (SEND_SHOPITEM_BASEINFO*)pMsg;
	ITEMBASE * pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->ShopItemPos);
	if (!pItemBase) return;
	if (pItemBase->Durability > 1)
	{
		GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->ShopItemPos, -1);
	}
	else
	{
		CItem * pItem = NULL;
		DeleteItem(pmsg->ShopItemPos, &pItem);
	}

	GAMEIN->GetMainInterfaceDialog()->SetAlram(OPT_QUESTDLGICON, TRUE);//[设置图标闪烁][By:十里坡剑神][QQ:112582793][2018/4/21]
}
void CItemManager::ItemExt_SendInfo_Ack(void* pMsg)
{
	//[发送物品信息查询][By:十里坡剑神][QQ:112582793][2018/4/3]
	MSG_QUERY_EQUIPINF_FORSEND* pmsg = (MSG_QUERY_EQUIPINF_FORSEND*)pMsg;
	CItem* pItem = MakeNewItem(&pmsg->ItemBase, "MP_ITEM_SENDINFO_ACK");
	if (!pItem)
		return;
	SetToolTipIcon(pItem, &pmsg->ItemOptionInfo, &pmsg->ItemRateInfo, pItem->GetItemBaseInfo()->dwDBIdx);
	GAMEIN->GetSendItemInfoDlg()->SetItem(pItem->GetDBIdx(), TRUE);
	GAMEIN->GetSendItemInfoDlg()->SetItemTip(pItem->GetToolTip());
	pItem->SetActive(FALSE);
	GAMEIN->GetSendItemInfoDlg()->Show();
}
void CItemManager::ItemExt_PakMove_Ack(void* pMsg)
{
	//[背包整理服务端确认消息][By:十里坡剑神][QQ:112582793][2018/6/11]
	MSG_ITEMINFO_MOVEPAK * pmsg = (MSG_ITEMINFO_MOVEPAK*)pMsg;
	for (int i = 0; i < 80; i++)
	{
		CItem * FromItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
		if (FromItem)
		{
			if (FromItem->GetQuickPosition() != 0)
			{//[放到快捷栏物品需先删除快捷栏图标][By:十里坡剑神][QQ:112582793][2019/3/18][1:35:06]
				//QUICKMGR->RemoveQuickItemReal(i, FromItem->GetItemIdx(), FromItem->GetQuickPosition() - 1);
				QUICKMGR->RemQuickItem(FromItem->GetQuickPosition());
			}
			GAMEIN->GetInventoryDialog()->DeleteItem(i, &FromItem);
			if (FromItem)
			{//[删除客户端物品信息释放资源ID][By:十里坡剑神][QQ:112582793][2019/3/18][1:35:33]
				ITEMMGR->ItemDelete(FromItem);
				FromItem = NULL;
			}
		}
	}
	WINDOWMGR->DestroyWindowProcess();//[手动执行窗口销毁][By:十里坡剑神][QQ:112582793][2019/3/18][1:35:59]
	for (int i = 0; i < SLOT_INVENTORY_NUM; i++)
	{
		GAMEIN->GetInventoryDialog()->AddItem(&pmsg->ItemBase[i]);
		if (pmsg->ItemBase[i].QuickPosition != 0)
		{
			if (EVENTMAP->IsEventMap())
			{
				ITEM_INFO* pItemInfo = GetItemInfo(pmsg->ItemBase[i].wIconIdx);
				if (pItemInfo)
					if (pItemInfo->ItemKind != eYOUNGYAK_ITEM)
						QUICKMGR->SetQuickItemReal(pmsg->ItemBase[i].QuickPosition, pmsg->ItemBase[i].Position, pmsg->ItemBase[i].wIconIdx);
			}
			else
			{
				QUICKMGR->SetQuickItemReal(pmsg->ItemBase[i].QuickPosition, pmsg->ItemBase[i].Position, pmsg->ItemBase[i].wIconIdx);
			}
		}
	}
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	QUICKMGR->RefreshQickItem();
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2158));
}
void CItemManager::ItemExt_PakMove_Nack(void* pMsg)
{
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2291));
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
}
void CItemManager::ItemExt_UpDate_GoldShop_Begin(void* pMsg)
{
	if (GAMEIN->GetGoldShopMainDlg()->IsActive())
		GAMEIN->GetGoldShopMainDlg()->SetActive(FALSE);
	GAMEIN->GetGoldShopMainDlg()->SetDisable(TRUE);
	if (m_ShopItemInfo.size() > 0)
		m_ShopItemInfo.clear();
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2308));
}
void CItemManager::ItemExt_UpDate_GoldShop_Ing(void* pMsg)
{
	SEND_GOLDSHOPINFO * pmsg = (SEND_GOLDSHOPINFO*)pMsg;
	for (int i = 0; i < 50; i++)
	{
		if (pmsg->GoldShopItem[i].wItemIndex != 0)
		{
			std::pair<WORD, GOLD_SHOP_ITEM_INFO> p1(pmsg->GoldShopItem[i].wItemIndex, pmsg->GoldShopItem[i]);
			m_ShopItemInfo.insert(p1);
		}
		else
			break;
	}
}
void CItemManager::ItemExt_UpDate_GoldShop_End(void* pMsg)
{
	GAMEIN->GetGoldShopMainDlg()->SetDisable(FALSE);
	GAMEIN->GetGoldShopMainDlg()->SetActive(TRUE);
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2309));
}
void CItemManager::ItemExt_AddPoint_Nack(void* pMsg)
{
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(786));
	FastConfigPointDlg * pDlg = GAMEIN->GetFastConfigPointDlg();
	if (pDlg)
	{
		if (pDlg->IsActive())
		{
			pDlg->ClearData();
			pDlg->SetActive(FALSE);
		}
	}
}
void CItemManager::ItemExt_AddPoint_Ack(void* pMsg)
{
	MSG_ADDPOINT* pmsg = (MSG_ADDPOINT*)pMsg;

	ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(HERO, pmsg->dwItemPos);
	if (!pItemBase)	return;

	if (pItemBase->dwDBIdx == pmsg->dwItemDBidx)
	{
		if (pItemBase->Durability > 1)
		{
			GAMEIN->GetInventoryDialog()->UpdateItemDurabilityAdd(pmsg->dwItemPos, -1);
			return;
		}
		CItem* pItem = NULL;
		DeleteItem(pmsg->dwItemPos, &pItem);
	}

	FastConfigPointDlg * pDlg = GAMEIN->GetFastConfigPointDlg();
	if (pDlg)
	{
		if (pDlg->IsActive())
		{
			pDlg->ClearData();
			pDlg->SetActive(FALSE);
		}
	}
}
void CItemManager::ItemExt_Inherit_Ack(void* pMsg)
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-23][0:57:20]
	GAMEIN->GetEquipItemInherit()->ItemInheritCreateAck((MSG_ITEM_RAREITEM_GET*)pMsg);
}
void CItemManager::ItemExt_PackMoved_Ack()
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2158));
}
void CItemManager::ItemExt_FBItemTimeAdd_Ack(void* pMsg)
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;
	GAMEIN->GetFBTipDlg()->AddTime(pmsg->nData);
}
void CItemManager::ItemExt_Inherit_Nack(void* pMsg)
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-23][0:57:20]
	MSG_ITEMINHERIT* pmsg = (MSG_ITEMINHERIT*)pMsg;
	GAMEIN->GetEquipItemInherit()->ItemInheritCreateNAck((eInheritErrorKind)pmsg->dwObjectID);
}
void CItemManager::NetworkMsgParseExt(WORD Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_ACK:		ItemExt_ShopItem_Curse_Cancellation_AddItem_Ack(pMsg);		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_NACK:		ItemExt_ShopItem_Curse_Cancellation_AddItem_Nack(pMsg);		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ACK:				ItemExt_ShopItem_Curse_Cancellation_Ack(pMsg);				break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_DELETEITEM:			ItemExt_ShopItem_Curse_Cancellation_DeleteItem(pMsg);		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_ACK:						ItemExt_UniqueItem_Mix_AddItem_Ack(pMsg);					break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_NACK:					ItemExt_UniqueItem_Mix_AddItem_Nack(pMsg);					break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM:						ItemExt_UniqueItem_Mix_DeleteItem(pMsg);					break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_ACK:								ItemExt_UniqueItem_Mix_Ack(pMsg);							break;
	case MP_ITEMEXT_SKINITEM_SELECT_ACK:							ItemExt_SkinItem_Select_Ack(pMsg);							break;
	case MP_ITEMEXT_SKINITEM_SELECT_NACK:							ItemExt_SkinItem_Select_Nack(pMsg);							break;
	case MP_ITEMEXT_SKINITEM_DISCARD_ACK:							ItemExt_SkinItem_Discard_Ack(pMsg);							break;
	case MP_ITEMEXT_SHOPITEM_DECORATION_ON:							ItemExt_ShopItem_Decoration_On(pMsg);						break;
	case MP_ITEM_ABRASION_ACK:										ItemExt_Abrasion_Ack(pMsg);									break;
	case MP_ITEM_MAKER_ACK:											ItemExt_Maker_Ack(pMsg);									break;
	case MP_ITEM_GOLDITEMUSE_NACK:									ItemExt_GoldItemUse_Nack(pMsg);								break;
	case MP_ITEM_GOLDITEMUSE_ACK:									ItemExt_GoldItemUse_Ack(pMsg);								break;
	case MP_ITEMEX_NJQUEST_ACK:										ItemExt_NJQuest_Ack(pMsg);									break;
	case MP_ITEMEX_NJQUEST_NACK:																								break;
	case MP_ITEM_SENDINFO_ACK:										ItemExt_SendInfo_Ack(pMsg);									break;
	case MP_ITEM_SENDINFO_NACK:										CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2263));		break;
	case MP_ITEMEX_PAKMOVE_ACK:										ItemExt_PakMove_Ack(pMsg);									break;
	case MP_ITEMEX_PAKMOVE_NACK:									ItemExt_PakMove_Nack(pMsg);									break;
	case MP_ITEMEX_UPDATE_GOLDSHOP_BEGIN:							ItemExt_UpDate_GoldShop_Begin(pMsg);						break;
	case MP_ITEMEX_UPDATE_GOLDSHOP_ING:								ItemExt_UpDate_GoldShop_Ing(pMsg);							break;
	case MP_ITEMEX_UPDATE_GOLDSHOP_END:								ItemExt_UpDate_GoldShop_End(pMsg);							break;
	case MP_ITEM_ADDPOINT_NACK:										ItemExt_AddPoint_Nack(pMsg);								break;
	case MP_ITEM_ADDPOINT_ACK:										ItemExt_AddPoint_Ack(pMsg);									break;
	case MP_ITEMEX_INHERIT_ACK:										ItemExt_Inherit_Ack(pMsg);									break;
	case MP_ITEMEX_INHERIT_NACK:									ItemExt_Inherit_Nack(pMsg);									break;
	case MP_ITEMEX_PAKMOVED_ACK:									ItemExt_PackMoved_Ack();									break;
	case MP_ITEMEX_FBTIMEADD_ACK:									ItemExt_FBItemTimeAdd_Ack(pMsg);							break;
	default:
		break;
	}
}


