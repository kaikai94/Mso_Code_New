#include "stdafx.h"
#include "LotSplitItemDlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include ".//interface/cSpin.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include <algorithm>

LotSplitItemDlg::LotSplitItemDlg(void)
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);
	m_ItemIdx = 0;
	m_ItemPos = 0;
	m_ItemDurability = 0;
	m_NeedEmptySolt = 0;
	m_InputNum = 0;
	m_ItemDBIdx= 0;
	m_EmptySolt.reserve(200);
}

LotSplitItemDlg::~LotSplitItemDlg(void)
{
	m_EmptySolt.clear();
	std::vector<BYTE>().swap(m_EmptySolt);
}
void LotSplitItemDlg::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID( LOT_SPLITITEM_ICON );
}

BOOL LotSplitItemDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE; //[判断拖入的图标类型是否是物品][BY:十里坡剑神][QQ:112582793][2019-4-8][8:57:12]

	if( m_bDisable ) return FALSE;  //[窗口锁定不进行任何处理][BY:十里坡剑神][QQ:112582793][2019-4-8][8:57:24]

	CItem * pOrigItem = (CItem *)pOrigIcon;
	
	if( pOrigItem->IsLocked() ) return FALSE; //[物品被锁定不进行处理][BY:十里坡剑神][QQ:112582793][2019-4-8][8:57:46]


	if(!ITEMMGR->IsDupItem(pOrigItem->GetItemIdx()))
	{//[非叠加类物品不可拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][8:56:51]
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2338) );
		SetActive(FALSE);
		return FALSE;
	}

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );

	if( !pBaseItemInfo )//[获取物品类型][BY:十里坡剑神][QQ:112582793][2019-4-8][9:25:20]
		return FALSE;
	if(pBaseItemInfo->ItemKind>=eSHOP_ITEM&&pBaseItemInfo->ItemKind<=eSHOP_ITEM_AUTOADDPOINT)
	{//[百宝类物品不允许使用批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][13:44:32]
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2342) );
		SetActive(FALSE);
		return FALSE;
	}

	//[获取物品栏空闲格子][BY:十里坡剑神][QQ:112582793][2019-4-8][9:30:33]
	GAMEIN->GetInventoryDialog()->GetEmptyNum(0,m_EmptySolt);

	pOrigItem->SetLock(TRUE);	//[锁定物品][BY:十里坡剑神][QQ:112582793][2019-4-8][8:59:19]

	AddVirtualItem( pOrigItem ); //[添加虚拟图标][BY:十里坡剑神][QQ:112582793][2019-4-8][8:59:26]
	//[设置数量输入框最大最小值][BY:十里坡剑神][QQ:112582793][2019-4-8][9:44:42]
	((cSpin*)GetWindowForID(LOT_SPLIT_SPIN_NUM))->SetMinMax(1,pOrigItem->GetDurability()-1);
	//[锁定背包仓库等物品栏][BY:十里坡剑神][QQ:112582793][2019-4-8][8:59:37]
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

	return FALSE;
}
void LotSplitItemDlg::SetActive( BOOL val )
{
	if(!val)
	{
		ReleaseItem();
	}
	cDialog::SetActive(val);
}
void LotSplitItemDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case LOT_SPLIT_BTN_OK:
			{
				CalcNeedSolt();
				ItemSplitSyn();
			}
			break;
		}
	}
}
void LotSplitItemDlg::ReleaseItem()
{//[关闭拆分窗口必须调用此函数，不能直接使用SetActive(FALSE)][BY:十里坡剑神][QQ:112582793][2019-4-8][9:02:43]
	if( m_pIconDlg )
	{
		CVirtualItem* pVItem = NULL;
		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )			
			pVItem->GetLinkItem()->SetLock( FALSE );
		m_VirtualItem.SetLinkItem( NULL );
	}
	ClearData();
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}
void LotSplitItemDlg::ClearData()
{
	m_ItemIdx = 0;
	m_ItemPos = 0;
	m_ItemDurability = 0;
	m_NeedEmptySolt = 0;
	m_InputNum = 0;
	m_ItemDBIdx= 0;
	m_EmptySolt.clear();
}
void LotSplitItemDlg::CalcNeedSolt()
{
	m_InputNum = (WORD)((cSpin*)GetWindowForID(LOT_SPLIT_SPIN_NUM))->GetValue();
	if(m_InputNum>=m_ItemDurability||m_InputNum<1)
	{//[拆分基数不能大于或者等于要拆分物品的数量][BY:十里坡剑神][QQ:112582793][2019-4-8][9:42:36]
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2339));
		ClearData();
		return ;
	}
	if(m_ItemDurability%m_InputNum!=0)
		m_NeedEmptySolt++;
	WORD num = m_ItemDurability/m_InputNum;
	m_NeedEmptySolt+=num;
	if(m_EmptySolt.size()==0||m_NeedEmptySolt>=m_EmptySolt.size())
	{//[背包空间不足][BY:十里坡剑神][QQ:112582793][2019-4-8][10:03:05]
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2340));
		ClearData();
		return ;
	}
}
void LotSplitItemDlg::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );

	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( pShopItemInfo )
		ITEMMGR->AddUsedAvatarItemToolTip( pShopItemInfo );

	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );

	m_ItemIdx = pItem->GetItemIdx();
	m_ItemPos = pItem->GetPosition();
	m_ItemDurability = pItem->GetDurability();
	m_ItemDBIdx = pItem->GetDBIdx();
}

void LotSplitItemDlg::ItemSplitSyn()
{
	for(int i = 0;i<m_NeedEmptySolt;i++)
	{
		m_ItemDurability-=m_InputNum;
		if(m_ItemDurability<m_InputNum)break;
		MSG_ITEM_DIVIDE_SYN	msg;
		msg.Category			= MP_ITEM;
		msg.Protocol			= MP_ITEM_DIVIDE_SYN;
		msg.dwObjectID			= HEROID;

		msg.wItemIdx = m_ItemIdx;
		msg.FromPos = m_ItemPos;
		msg.FromDur = m_ItemDurability;
		msg.ToPos = (POSTYPE)m_EmptySolt.back();
		msg.ToDur = m_InputNum;
		msg.dwItemDBIdx = m_ItemDBIdx;
		m_EmptySolt.pop_back();
		NETWORK->Send(&msg,sizeof(msg));
	}
	ReleaseItem();
}