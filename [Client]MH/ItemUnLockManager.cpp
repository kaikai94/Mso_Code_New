#include "stdafx.h"
#include "ItemUnLockManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"

CItemUnLockDialog::CItemUnLockDialog()
{
   pDlg=NULL;

   m_pIconDlg = NULL;

   m_LockItem=NULL;
   m_UnLockType=eUnLockForLock;
}

CItemUnLockDialog::~CItemUnLockDialog()
{
   pDlg=NULL;

   m_pIconDlg = NULL;

   m_LockItem=NULL;
}

void CItemUnLockDialog::Linking()
{
    pDlg = WINDOWMGR->GetWindowForID( ITEM_UNLOCKDLGEX );

	m_pIconDlg = (cIconDialog*)GetWindowForID(IT_UNLOCKICONEX);
	m_OkBtn=(cButton*)GetWindowForID(IT_UNLOCKOKBTNEX);
	m_CapText=(cStatic*)GetWindowForID(IT_UNLOCKCAPTEXT);
	m_Tip1=(cStatic* )GetWindowForID(IT_UNLOCKTIP1);
	m_Tip2=(cStatic* )GetWindowForID(IT_UNLOCKTIP2);
}

BOOL CItemUnLockDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if( icon->GetType() != WT_ITEM )	
		
	return FALSE;

	if( m_bDisable )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;

	if( pOrigItem->IsLocked() )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx());

	if(!pItemInfo)	return FALSE;
	if(pItemInfo->ItemKind<eEQUIP_ITEM||pItemInfo->ItemKind>eEQUIP_ITEM_UNIQUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2097));
		return FALSE;
	}

	if(ITEMMGR->IsDupItem(pOrigItem->GetItemIdx()))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2125));

		return FALSE;
	}

	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) && !ITEMMGR->IsEqualTableIdxForPos(eItemTable_ShopInven, pOrigItem->GetPosition()))
	{
		return FALSE;
	}

	if (m_UnLockType==eUnLockForLock&&pOrigItem->GetStatic()!=ITEM_STATIC_LUCK)
	{//[加锁物品解锁][By:十里坡剑神][QQ:112582793][2018/1/1]
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2121));
		return FALSE;
	}
	if(m_UnLockType==eUnLockForPD&&pOrigItem->GetStatic()!=ITEM_STATIC_PD)  // 2014-12-05  3表示被   泡点购买1保持一致！锁的禁止解锁!
	{//[易碎物品强化][By:十里坡剑神][QQ:112582793][2018/1/1]
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2098));
		return FALSE;
	}
	if (m_UnLockType==eUnMixEqui&&pOrigItem->GetAbrasion()==pOrigItem->GetMaxItemAbrasion())
	{//[装备修理][By:十里坡剑神][QQ:112582793][2018/1/4]
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(1797));
		return FALSE;
	}
	pOrigItem->SetLock(TRUE);

	AddVirtualItem( pOrigItem );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	return FALSE;
}

void CItemUnLockDialog::AddVirtualItem(CItem * pItem)
{
	cIcon* pIcon=(cIcon*)pItem;
	m_VirtualItem.SetData( pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem);

	if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
	{
       ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability()),ITEMMGR->GetItemRareOption( pItem->GetRareness()),0); // 将到被 2014-11-14
	}
	else
	{
		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);  // 如果是强化过的,则添!
	}
}

BOOL CItemUnLockDialog::VirtualItemIsNull()
{
	if(m_VirtualItem.GetLinkItem())
	{
		return FALSE;
	}
	else 
	{
		return TRUE;
	}
}

void CItemUnLockDialog::SetLockItem(CItem * pItem)
{
      m_LockItem=pItem;
}

CItem * CItemUnLockDialog::GetLockItem()
{
      return m_LockItem;
}
void CItemUnLockDialog::SendSynMsg()
{
	MSG_DWORD5 msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_UNLOCKEX_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1	   = m_VirtualItem.GetLinkItem()->GetItemIdx();
	msg.dwData2	   = m_VirtualItem.GetLinkItem()->GetPosition();
	msg.dwData3    = m_LockItem->GetItemIdx();
	msg.dwData4	   = m_LockItem->GetPosition();
	msg.dwData5	   = m_UnLockType;
	NETWORK->Send(&msg, sizeof(msg));
	pDlg->SetActive(TRUE);
}

void CItemUnLockDialog::Release()
{
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	SetActive( FALSE );

    if(m_LockItem)
	{
		m_LockItem->SetLock(FALSE);
	}
	pDlg->SetActive(FALSE);
}
void CItemUnLockDialog::SetLockType(eUNLOCKTYPE type)
{
	m_UnLockType = type;
	if (m_UnLockType==eUnLockForLock)
	{//[解锁][By:十里坡剑神][QQ:112582793][2018/1/4]
		m_CapText->SetStaticText(CHATMGR->GetChatMsg(1648));
		m_Tip1->SetStaticText(CHATMGR->GetChatMsg(1743));
		m_Tip2->SetStaticText(CHATMGR->GetChatMsg(1744));
		m_OkBtn->SetText(CHATMGR->GetChatMsg(1649),RGB_HALF(255,255,255));
	}
	else if(m_UnLockType==eUnLockForPD)
	{//[净化][By:十里坡剑神][QQ:112582793][2018/1/4]
		m_CapText->SetStaticText(CHATMGR->GetChatMsg(1650));
		m_Tip1->SetStaticText(CHATMGR->GetChatMsg(1777));
		m_Tip2->SetStaticText(CHATMGR->GetChatMsg(1778));
		m_OkBtn->SetText(CHATMGR->GetChatMsg(1651),RGB_HALF(255,255,255));
	}
	else 
	{//[修理][By:十里坡剑神][QQ:112582793][2018/1/4]
		m_CapText->SetStaticText(CHATMGR->GetChatMsg(1798));
		m_Tip1->SetStaticText("");
		m_Tip2->SetStaticText("");
		m_OkBtn->SetText(CHATMGR->GetChatMsg(2133),RGB_HALF(255,255,255));
	}
}