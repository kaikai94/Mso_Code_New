







// TitanUpgradeDlg.cpp: implementation of the CTitanUpgradeDlg class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "TitanUpgradeDlg.h"
#include "cScriptManager.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "item.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "TitanManager.h"
#include "cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cIconDialog.h"
#include "./interface/cIconGridDialog.h"

CTitanUpgradeDlg::CTitanUpgradeDlg()
{
	m_pTitanUpgradeDlg = NULL;
	m_pTitanUpgradeGridDlg = NULL;

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	m_VirtualItem.SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );
	m_VirtualItem.SetMovable(FALSE);

	for( int i = 0; i < MAX_TITAN_UPGRADE_GRID_NUM; i++ )
	{
		m_ViewShowItem[i].SetToolTip( "", RGB_HALF(255,255,255), &imgTooltip );

		m_ViewShowItem[i].SetMovable(FALSE);
	}
}

CTitanUpgradeDlg::~CTitanUpgradeDlg()
{

}

void CTitanUpgradeDlg::Release(BOOL bFlag)
{
	DeleteViewMaterialListAll();
	m_pTitanUpgradeStatic->SetStaticText("");

	m_pTitanUpgradeMoney->SetStaticText("");

	// �ش� Icon������ ����
	CVirtualItem * pVItem;
	if(!m_pTitanUpgradeDlg->IsAddable(0))
	{
		m_pTitanUpgradeDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( bFlag )	// ������ ���� ��ü�ؾ���

		{
			MSG_WORD msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_TITANUPGRADE_RELEASEITEM;
			msg.dwObjectID	= HEROID;
			msg.wData		= pVItem->GetLinkItem()->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}
}

void CTitanUpgradeDlg::Linking()
{
	m_pTitanUpgradeDlg = (cIconDialog *)GetWindowForID(TITANUPGRADE_TITANICONDLG);
	m_pTitanUpgradeGridDlg = (cIconGridDialog *)GetWindowForID(TITANUPGRADE_ICONGRIDDLG);
	m_pTitanUpgradeStatic = (cStatic*)GetWindowForID(TITANUPGRADE_NAME_STATIC);
	m_pTitanUpgradeMoney = (cStatic*)GetWindowForID(TITANUPGRADE_RFEE);
}

void CTitanUpgradeDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		Release();

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

DWORD CTitanUpgradeDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	m_VirtualItem.ActionEvent(mouseInfo);


	return we;
}

BOOL CTitanUpgradeDlg::OnActionEvent( LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case TITANUPGRADE_BTN_OK:	// Ȯ���ϱ�
		{
			CTitanUpgradeDlg* pTitanUpgradeDlg = (CTitanUpgradeDlg*)p;
			CTitanUpgradeDlg::UpgradeOkBtn(pTitanUpgradeDlg);
		}
		break;	
	}

	return TRUE;
}

void CTitanUpgradeDlg::UpgradeOkBtn(CTitanUpgradeDlg * pThis)
{
	// ������ ���ִ��� üũ
	CVirtualItem* pItem = (CVirtualItem*)pThis->m_pTitanUpgradeDlg->GetIconForIdx(0);
	if(!pItem)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1504) );
		return;
	}

	// �κ��� ��ᰡ �ִ��� üũ
	TITAN_UPGRADEINFO * pTitanUpgradeInfo = TITANMGR->GetTitanUpgradeInfoItemInfo(pItem->GetSrcItemIdx());

	MATERIAL_ARRAY ItemOut[TP_INVENTORY_END-TP_INVENTORY_START];

	ZeroMemory(ItemOut, sizeof(ItemOut));
	WORD ItemPosNum = 0;
	DWORD needItemIdx = 0;
	DURTYPE needItemNum = 0;

	if(pTitanUpgradeInfo->dwNextTitanIdx == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1504) );
		return;
	}

	if(OBJECTMGR->GetHero()->GetMoney() < pTitanUpgradeInfo->dwMoney)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		return;
	}

	for( int i = 0; i < pTitanUpgradeInfo->wMaterial_Num; i++ )
	{
		needItemIdx = pTitanUpgradeInfo->pTitanUpgradeMaterial[i].dwIndex;
		needItemNum = pTitanUpgradeInfo->pTitanUpgradeMaterial[i].dwCount;

		for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && needItemNum != 0 ; ++i)
		{

			CItem * pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(pItem && pItem->GetItemIdx() == needItemIdx)
			{
				// ���� ��ᰡ �ʿ��Ѽ������� ���ų� �������
				if(needItemNum <= pItem->GetDurability())
				{
					ItemOut[ItemPosNum].Dur = needItemNum;
					needItemNum = 0;
				}
				else if( !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
				{
					// �ش� ��ᰡ ���� �������� �ƴϸ�..
					ItemOut[ItemPosNum].Dur = 1;
					needItemNum = 0;
				}
				else	// ���� ��ᰡ �ʿ��Ѽ������� �������
				{
					ItemOut[ItemPosNum].Dur = pItem->GetDurability();
					needItemNum -= pItem->GetDurability();
				}
				ItemOut[ItemPosNum].wItemIdx = (WORD)needItemIdx;
				ItemOut[ItemPosNum].ItemPos = i;
				ItemPosNum++;
			}
		}


		if(needItemNum != 0)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(205) );
			return;
		}
	}

	MSG_TITAN_UPGRADE_SYN msg;
	ZeroMemory(&msg, sizeof(MSG_TITAN_UPGRADE_SYN));
	SetProtocol( &msg, MP_ITEM, MP_ITEM_TITANUPGRADE_SYN );
	msg.dwObjectID = HEROID;

	msg.wBasicItemIdx = (WORD)pTitanUpgradeInfo->dwTitanIdx;
	msg.wBasicItemPos = pItem->GetSrcPosition();
	msg.dwMoney = pTitanUpgradeInfo->dwMoney;

	for(int i = 0 ; i < ItemPosNum ; ++i)
	{
		if(ITEMMGR->IsDupItem((WORD)needItemIdx))
			msg.AddMaterial(ItemOut[i].wItemIdx, ItemOut[i].ItemPos, ItemOut[i].Dur);

		else
			msg.AddMaterial(ItemOut[i].wItemIdx, ItemOut[i].ItemPos, 0);
	}


	NETWORK->Send(&msg, msg.GetSize());
}

BOOL CTitanUpgradeDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pOrigItem = (CItem*)pOrigIcon;
	if( pOrigItem->IsLocked() )	return FALSE;

	// �κ��丮 �������� �ƴϸ� FALSE
	if( !ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()) )
	{

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(612) );
		return FALSE;

	}

	// �ɼ� �������̸� FALSE
	if( ITEMMGR->IsOptionItem(pOrigItem->GetItemIdx(), pOrigItem->GetDurability()) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if(bits != eTITAN_ITEM_PAPER)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );
		return FALSE;
	}

	// ��ϵ������� Ÿ��ź�� ��޾� �ȵ�
	if(TITANMGR->GetRegistedTitanItemDBIdx() == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1517) );
		return FALSE;
	}

	if(TITANMGR->GetRegistedTitanItemDBIdx() != pOrigItem->GetDBIdx())
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1517) );
		return FALSE;
	}

	if( pOrigItem->GetItemInfo()->Plus_Value >= 3 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1581));
		return FALSE;
	}


	// ���׷��̵带 ���� ���� ������
	TITAN_UPGRADEINFO* pTitanUpgradeInfo = TITANMGR->GetTitanUpgradeInfoItemInfo(pOrigItem->GetItemIdx());
	if(!pTitanUpgradeInfo)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(202) );	
		return FALSE;
	}

	Release();

	// Ÿ��ź ���� ����
	AddTitanPaper(pOrigItem);

	// ��ܰ�� ���׷��̵� �ϴ��� ������
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	char buf[128] = { 0, };
	sprintf( buf, CHATMGR->GetChatMsg(1550), pItemInfo->Plus_Value + 1 );
	m_pTitanUpgradeStatic->SetStaticText(buf);


	// ���׷��̵忡 �ʿ��� ��Ḧ ������

	ViewMaterialList(pTitanUpgradeInfo);

	// ���׷��̵忡 �ʿ��� ���� ������
	m_pTitanUpgradeMoney->SetStaticValue(pTitanUpgradeInfo->dwMoney);
	
	// Lock Item
	pOrigItem->SetLock(TRUE);

	// Send Message
	MSG_ITEM	msgItem;
	msgItem.Category	= MP_ITEM;
	msgItem.Protocol	= MP_ITEM_TITANUPGRADE_ADDITEM_SYN;
	msgItem.dwObjectID	= HEROID;
	msgItem.ItemInfo	= *pOrigItem->GetItemBaseInfo();
	NETWORK->Send( &msgItem, sizeof(msgItem) );

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_MunpaWarehouse);

	return FALSE;

}

void CTitanUpgradeDlg::AddTitanPaper(CItem* pItem)
{
	m_VirtualItem.SetData(pItem->GetItemIdx());
	m_VirtualItem.SetLinkItem(pItem);


	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, ITEMMGR->GetItemOption(pItem->GetDurability())
			, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0); 
	}	
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness())); 

	m_pTitanUpgradeDlg->AddIcon(0, (cIcon*)&m_VirtualItem);

}

void CTitanUpgradeDlg::ViewMaterialList(TITAN_UPGRADEINFO * pTitanUpgradeInfo)
{
	if(!pTitanUpgradeInfo) return;
	DeleteViewMaterialListAll();

	for( int i = 0; i < pTitanUpgradeInfo->wMaterial_Num; i++ )
	{
		m_ViewShowItem[i].SetData(pTitanUpgradeInfo->pTitanUpgradeMaterial[i].dwIndex);
		m_ViewShowItem[i].Init((WORD)pTitanUpgradeInfo->pTitanUpgradeMaterial[i].dwIndex, pTitanUpgradeInfo->pTitanUpgradeMaterial[i].dwCount);

		ITEMMGR->SetToolTipIcon( (cIcon*)&m_ViewShowItem[i], NULL, ITEMMGR->GetItemRareOption(m_VirtualItem.GetRareness()),0); 


		m_pTitanUpgradeGridDlg->AddIcon(i, (cIcon*)&m_ViewShowItem[i]);
	}
}

void CTitanUpgradeDlg::DeleteViewMaterialListAll()
{
	for(int i = 0 ; i < m_pTitanUpgradeGridDlg->GetCellNum() ; ++i )

	{
		CItemShow * pSItemOut = NULL;
		m_pTitanUpgradeGridDlg->DeleteIcon(i, (cIcon**)&pSItemOut);
	}
	m_pTitanUpgradeGridDlg->SetCurSelCellPos(-1);
}

void CTitanUpgradeDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}
}




