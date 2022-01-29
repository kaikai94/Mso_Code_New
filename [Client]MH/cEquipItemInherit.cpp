#include "stdafx.h"
#include "cEquipItemInherit.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "cCheckBox.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "ItemManager.h"
#include "QuickManager.h"
cEquipItemInherit::cEquipItemInherit()
{
	cImage imgToolTip;


	SCRIPTMGR->GetImage(63, &imgToolTip, PFT_HARDPATH);
	m_VirtualItem.SetToolTip("", RGB_HALF(255, 255, 255), &imgToolTip);
	m_VirtualItem.SetMovable(FALSE);

	m_pIconDlg = NULL;

	m_TitleStatic = NULL;
	m_TipStatic = NULL;
	m_GrowCheck = NULL;
	m_GrowStatic = NULL;
	m_pItem = NULL;
}


cEquipItemInherit::~cEquipItemInherit()
{

}
void cEquipItemInherit::Linking()
{
	m_pIconDlg		= (cIconDialog*)GetWindowForID(INHERIT_ICON);
	m_TitleStatic   = (cStatic*)GetWindowForID(INHERIT_TITLESTATIC);
	m_TipStatic     = (cStatic*)GetWindowForID(INHERIT_TIPSTATIC);
	m_GrowStatic	= (cStatic*)GetWindowForID(INHERIT_GROWSTATIC);
	m_GrowCheck		= (cCheckBox*)GetWindowForID(INHERIT_GROWCHECK);
	if (m_GrowCheck)
		m_GrowCheck->SetActive(FALSE);
	if (m_GrowStatic)
		m_GrowStatic->SetActive(FALSE);
}
BOOL cEquipItemInherit::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if (icon->GetType() != WT_ITEM) return FALSE;

	if (m_bDisable) return FALSE;

	if (m_pItem == NULL) return FALSE;

	CItem * pOrigItem = (CItem *)icon;

	if (pOrigItem->IsLocked())
	{//[添加物品已锁定][BY:十里坡剑神][QQ:112582793][2019-8-22][14:18:41]
		return FALSE;
	}

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pBaseItemInfo)
	{//[添加物品信息获取失败][BY:十里坡剑神][QQ:112582793][2019-8-22][14:18:23]
		return FALSE;
	}
		

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();

	if (!(bits >= eEQUIP_ITEM_WEAPON&&bits <= eEQUIP_ITEM_UNIQUE))
	{//[添加的物品是否是装备类型][BY:十里坡剑神][QQ:112582793][2019-8-22][14:17:57]
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2379));
		return FALSE;
	}

	if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;//[装备是否存在背吧][BY:十里坡剑神][QQ:112582793][2019-8-22][14:17:46]

	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo((WORD)m_pItem->GetItemIdx());
	if (!pShopItem)
	{//[传承石物品信息获取失败][BY:十里坡剑神][QQ:112582793][2019-8-22][14:21:17]
		return FALSE;
	}
	
	if (pOrigItem->GetRareness() == 0 && m_pItem->GetRareness() == 0)
	{//[传承石是继承则物品必须是绿过的][BY:十里坡剑神][QQ:112582793][2019-8-22][14:24:16]
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2381));
		return FALSE;
	}

	if (pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub)
	{//[装备等级和传承石等级不匹配][BY:十里坡剑神][QQ:112582793][2019-8-22][14:21:27]
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
		return FALSE;
	}

	if (pBaseItemInfo->ItemKind!= pShopItem->wPulsItemEffect)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2380));
		return FALSE;
	}

	pOrigItem->SetLock(TRUE);

	AddVirtualItem(pOrigItem);

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}
void cEquipItemInherit::Show(BOOL bIsWeapon)
{
	if (IsActive())
		SetActive(FALSE);
	if (m_pItem->GetRareness() != 0)
	{//[附加][BY:十里坡剑神][QQ:112582793][2019-8-22][14:40:57]
		m_TitleStatic->SetStaticText(CHATMGR->GetChatMsg(2383));
		m_TipStatic->SetStaticText(CHATMGR->GetChatMsg(2385));
	}
	else
	{
		m_TitleStatic->SetStaticText(CHATMGR->GetChatMsg(2382));
		m_TipStatic->SetStaticText(CHATMGR->GetChatMsg(2384));
	}
	if (bIsWeapon&&m_pItem->GetRareness() == 0)
	{
		if (m_GrowCheck)
			m_GrowCheck->SetActive(TRUE);
		if (m_GrowStatic)
			m_GrowStatic->SetActive(TRUE);
	}
	SetActive(TRUE);
}
void cEquipItemInherit::ReleaseItem()
{
	m_pItem = NULL;
		
	if (m_pIconDlg)
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
		if (pVItem)
			pVItem->GetLinkItem()->SetLock(FALSE);

		m_VirtualItem.SetLinkItem(NULL);
	}

	if (m_pItem)
		m_pItem->SetLock(FALSE);

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	if (m_GrowCheck)
		m_GrowCheck->SetActive(FALSE);
	if (m_GrowStatic)
		m_GrowStatic->SetActive(FALSE);
	m_GrowCheck->SetChecked(FALSE);
	SetActive(FALSE);
}
void cEquipItemInherit::AddVirtualItem(CItem* pItem)
{
	m_VirtualItem.SetData(pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);

	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo(pItem->GetItemBaseInfo()->wIconIdx);
	if (pShopItemInfo)
		ITEMMGR->AddUsedAvatarItemToolTip(pShopItemInfo);
	m_pIconDlg->AddIcon(0, (cIcon*)&m_VirtualItem);
}
void cEquipItemInherit::SetShopItemInfo(CItem* pItem)
{ 
	m_pItem = pItem;
}

void cEquipItemInherit::ItemInheritCreateSyn()
{
	if (!m_VirtualItem.GetLinkItem())		return;
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (!pItem)
	{
		if (m_pIconDlg)
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
			if (pVItem)
				pVItem->GetLinkItem()->SetLock(FALSE);
		}
		m_pItem = NULL;

		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}
	MSG_ITEMINHERIT msg;
	msg.dwObjectID = HEROID;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEX_INHERIT_SYN;
	msg.wInheritItemIdx = m_pItem->GetItemIdx();
	msg.wInheritItemPos = m_pItem->GetPosition();
	msg.dwInheritItemDBIdx = m_pItem->GetDBIdx();
	msg.wOrigItemIdx = pItem->GetItemIdx();
	msg.wOrigItemPos = pItem->GetPosition();
	msg.dwOrigItemDBIdx = pItem->GetDBIdx();
	msg.bIsGetGrow = m_GrowCheck->IsChecked();

	NETWORK->Send(&msg, sizeof(msg));
	SetActive(FALSE);
}
void cEquipItemInherit::ItemInheritCreateAck(MSG_ITEM_RAREITEM_GET* pmsg)
{
	ITEMMGR->AddItemRareOption(&pmsg->RareInfo);
	CVirtualItem* pVItem = NULL;
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		pItem->SetLock(FALSE);

		if (m_pIconDlg)

			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
		if (m_pItem->GetRareness() != 0)
		{
			ITEMMGR->RemoveItemRareOption(m_pItem->GetRareness());
			m_pItem->SetRareness(0);
			pItem->SetRareness(pmsg->RareInfo.dwRareOptionIdx);
			ITEMMGR->SetToolTipIcon((cIcon*)pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()), 0);
			ITEMMGR->SetToolTipIcon((cIcon*)m_pItem, NULL, NULL, 0);

			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem(m_pItem->GetPosition(), &pOutItem);
			if (pOutItem)
				ITEMMGR->ReLinkQuickPosition(pOutItem);
		}
		else
		{		
			ITEMMGR->RemoveItemRareOption(pItem->GetRareness());
			pItem->SetRareness(0);
			m_pItem->SetRareness(pmsg->RareInfo.dwRareOptionIdx);
			ITEMMGR->SetToolTipIcon((cIcon*)pItem, NULL, NULL, 0);
			ITEMMGR->SetToolTipIcon((cIcon*)m_pItem, NULL, ITEMMGR->GetItemRareOption(m_pItem->GetRareness()), 0);
		}
		m_VirtualItem.SetLinkItem(NULL);
	}
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	QUICKMGR->RefreshQickItem();
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2393));
	ReleaseItem();
}
void cEquipItemInherit::ItemInheritCreateNAck(eInheritErrorKind kind)
{
	switch (kind)
	{
	case eErrorNoGoldMoney:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2387));
		break;
	case eErrorEquipNotUse:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2388));
		break;
	case eErrorItemInfo:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2389));
		break;
	case eErrorNotEquipItem:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2390));
		break;
	case eErrorEquipNotRare:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2381));
		break;
	case eErrorInheritDelFaild:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2391));
		break;
	case eErrorNotUseLevel:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
		break;
	case eErrorNotUseType:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2380));
		break;
	default:
		break;
	}
	CVirtualItem* pVItem = NULL;

	m_pItem = NULL;

	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		pItem->SetLock(FALSE);

		if (m_pIconDlg)
			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);

		m_VirtualItem.SetLinkItem(NULL);
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	ReleaseItem();
}