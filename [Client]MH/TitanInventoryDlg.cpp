







// TitanInventoryDlg.cpp: implementation of the CTitanInventoryDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanInventoryDlg.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "INTERFACE\cIconDialog.h"
#include "cScriptManager.h"
#include "ItemManager.h"
#include "cWindowManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "MHAudioManager.h"
#include "cWindowManager.h"
#include "TitanManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "MouseCursor.h"
#include "cMsgBox.h"

CTitanInventoryDlg::CTitanInventoryDlg()
{
	m_pTitanInvenWearedDlg = NULL;	
	m_pTitanShopItemDlg = NULL;

	ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );

	cImage imgTooltip;
	SCRIPTMGR->GetImage( 63, &imgTooltip, PFT_HARDPATH );

	SCRIPTMGR->GetImage( 65, &m_Image );
}

CTitanInventoryDlg::~CTitanInventoryDlg()
{

}

void CTitanInventoryDlg::Linking()
{

	m_pTitanInvenWearedDlg = (CWearedExDialog *)GetWindowForID(TITAN_INVENTORY_WEAREDDLG);	
	m_pTitanShopItemDlg = (cIconDialog*)GetWindowForID(TITAN_INVENTORY_INVEN_SET_DLG);

    m_TitanInvenCtrl.titanType = (cStatic*)GetWindowForID(TITAN_INVENTORY_TYPE);
	m_TitanInvenCtrl.titanAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARATTACK);
	m_TitanInvenCtrl.titanLongAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_LONGATTACK);
	m_TitanInvenCtrl.titanMagicAtt = (cStatic*)GetWindowForID(TITAN_INVENTORY_MAGICATTACK);
	m_TitanInvenCtrl.titanCritical = (cStatic*)GetWindowForID(TITAN_INVENTORY_CRITICAL);
	m_TitanInvenCtrl.titanDistance = (cStatic*)GetWindowForID(TITAN_INVENTORY_DISTANCE);
	m_TitanInvenCtrl.titanDef = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARDEFENSE);
	m_TitanInvenCtrl.titanLife = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARLIFE);

	//m_TitanInvenCtrl.titanMana = (cStatic*)GetWindowForID(TITAN_INVENTORY_CHARNAERYUK);

	for( int i = 0; i < ATTR_MAX; i++ )
	{
		m_TitanInvenCtrl.TitanAttrDef.pStatic.SetElement_Val(ATTR_FIRE+i,(cStatic *)GetWindowForID(TITAN_INVENTORY_CHARHWA+i));
		m_TitanInvenCtrl.TitanAttrDef.pGuage.SetElement_Val(ATTR_FIRE+i,(cGuagen *)GetWindowForID(TITAN_INVENTORY_DEFENCE_HWA+i));
	}
}

void CTitanInventoryDlg::SetActive(BOOL val)
{
	cDialog::SetActive( val );
	if( val == TRUE )
	{
		SetTitanEquipInfo();
	}	
}

void CTitanInventoryDlg::UpdateData()
{
	char buf[64] = { 0, };
	titan_calc_stats* TitanStatsInfo = TITANMGR->GetTitanStats();	// Ÿ��ź ������������ �������� ����� ����Ʈ

	// Ÿ��ź Ÿ��
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, CHATMGR->GetChatMsg(1500), TITANMGR->GetTitanGrade(TITANMGR->GetRegistedTitanItemDBIdx()));
	m_TitanInvenCtrl.titanType->SetStaticText(buf);

	// Ÿ��ź �ּ�/�ִ� ���ݷ�
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d ~ %d", TitanStatsInfo->MeleeAttackPowerMin, TitanStatsInfo->MeleeAttackPowerMax);
	m_TitanInvenCtrl.titanAtt->SetStaticText(buf);

	// Ÿ��ź ���Ÿ� ���ݷ�
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d ~ %d", TitanStatsInfo->RangeAttackPowerMin, TitanStatsInfo->RangeAttackPowerMax);
	m_TitanInvenCtrl.titanLongAtt->SetStaticText(buf);

	// Ÿ��ź ��/���� �ϰ�
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->Critical);
	m_TitanInvenCtrl.titanCritical->SetStaticText(buf);

	// Ÿ��ź �����Ÿ�
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->AttackRange);
	m_TitanInvenCtrl.titanDistance->SetStaticText(buf);

	// Ÿ��ź ����
	ZeroMemory(buf, sizeof(buf));

	sprintf(buf, "%d", TitanStatsInfo->PhysicalDefense);
	m_TitanInvenCtrl.titanDef->SetStaticText(buf);

	// Ÿ��ź ����
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->MaxFuel);
	m_TitanInvenCtrl.titanLife->SetStaticText(buf);
/*
	// Ÿ��ź ����
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%d", TitanStatsInfo->MaxSpell);
	m_TitanInvenCtrl.titanMana->SetStaticText(buf);
*/
	// Ÿ��ź ��������(ȭ �� �� �� �� ���� ������ ��ġ�� �־����Ƿ� � ���� �־�������)
	ZeroMemory(buf, sizeof(buf));
	sprintf(buf, "%.2f", TitanStatsInfo->AttributeAttack.GetElement_Val(ATTR_FIRE));
	m_TitanInvenCtrl.titanMagicAtt->SetStaticText(buf);

	// Ÿ��ź ���׷�
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_FIRE, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_FIRE)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_WATER, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_WATER)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_TREE, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_TREE)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_IRON, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_IRON)), RGB_HALF(255,255,255));
	m_TitanInvenCtrl.TitanAttrDef.SetValue(ATTR_EARTH, (WORD)(TitanStatsInfo->AttributeResist.GetElement_Val(ATTR_EARTH)), RGB_HALF(255,255,255));
}


BOOL CTitanInventoryDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{

	switch(we)
	{	
	case WE_LBTNDBLCLICK:
		{
			// magi82(26)
			CItem * pItem = NULL;
			if( lId == TITAN_INVENTORY_WEAREDDLG )
			{
				pItem = (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx((WORD)m_pTitanInvenWearedDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;
			}
			else if( lId == TITAN_INVENTORY_INVEN_SET_DLG )
			{
				pItem = (CItem*)m_pTitanShopItemDlg->GetIconForIdx((WORD)m_pTitanShopItemDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;
			}

			GAMEIN->GetInventoryDialog()->UseItem(pItem);			

		}
		break;
	case WE_LBTNCLICK:
		{
			// magi82(7) - Titan(071024) Ÿ��ź ���������� ��������
			if( CURSOR->GetCursor() == eCURSOR_TITANREPAIR )
			{
				CItem * pItem = NULL;
				pItem = (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx((WORD)m_pTitanInvenWearedDlg->GetCurSelCellPos());			
				if( !pItem ) return FALSE;

				if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
				{
					TITANMGR->SetTitanRepairItem(pItem);

					// magi82(8) - Titan(071024) Ÿ��ź ���������� ���������Ҷ� ���� ������ ���
					TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
					if( pInfo )
					{
						float fEnduranceRate = (float)pInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
						if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
						{
							fEnduranceRate = 1.0f;
						}
						DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

						WINDOWMGR->MsgBox( MBI_TITAN_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1538), dwRepairPrice );
						ITEMMGR->SetDisableDialog(TRUE, eItemTable_Titan);
						ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
					}
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1539) );
				}
			}
		}
		break;
	case WE_CLOSEWINDOW:
		{			
			return TRUE;
		}
		break;
	}

	return TRUE;
}

BOOL CTitanInventoryDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	// ���������� üũ
	if(icon->GetType() != WT_ITEM)	return FALSE;
	if( m_bDisable ) return FALSE;

	CItem* pFromItem = (CItem*)icon;
	if( pFromItem->IsLocked() )	return FALSE;

	eITEM_KINDBIT bits = pFromItem->GetItemKind();

	//// ���콺 �̺�Ʈ �߻��Ѱ��� ����� üũ
	WORD ToPos=0;
	if( !GetPositionForXYRef( x, y, ToPos, bits) )	// ������ġ �Ѿ� ��
		return FALSE;

	// ������ ���������� �������� �̵��������� üũ
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

    // ���콺 �̺�Ʈ �߻��� ���Կ� �������� �̹� �ִ��� üũ
	CItem * pToItem = NULL;

	if(bits == eSHOP_ITEM_TITAN_EQUIP)
	{		
		pToItem = (CItem *)m_pTitanShopItemDlg->GetIconForIdx(ToPos-TP_TITANSHOPITEM_START);
	}
	else
	{		
		pToItem = (CItem *)m_pTitanInvenWearedDlg->GetIconForIdx(ToPos-TP_TITANWEAR_START);
	}	


	// ���� ������ �����ǿ� �������������� �ش�������� �ű���� �������� �����ǿ� �̵��������� üũ
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{

		return FALSE;
	}


	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	// â�� -> Ÿ��ź ���� �Ǹ� �ȵ�
	if(FromIdx == eItemTable_Pyoguk)	return FALSE;
	// �꽽�� -> Ÿ��ź ���� �Ǹ� �ȵ�
	if(FromIdx == eItemTable_PetInven)	return FALSE;

	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
		return FALSE;
	}

	//sound
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)

		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

 	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;

	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}

BOOL CTitanInventoryDlg::GetPositionForXYRef(LONG x, LONG y, WORD& pos, eITEM_KINDBIT bits)
{
	WORD position=0;										// ��� ��ġ

	//Ÿ��ź �޾����� �̸�..
	if(bits == eSHOP_ITEM_TITAN_EQUIP)
	{
		if(m_pTitanShopItemDlg->GetPositionForXYRef(x, y, position))
		{
			pos = position+TP_TITANSHOPITEM_START;					// -> ���� ��ġ�� �ٲ�
			return TRUE;
		}	
	}
	else	//Ÿ��ź ���...
	{
		if(m_pTitanInvenWearedDlg->GetPositionForXYRef(x, y, position))

		{
			pos = position+TP_TITANWEAR_START;					// -> ���� ��ġ�� �ٲ�
			return TRUE;
		}	
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	//2007. 11. 15. CBH - Ÿ��ź ž�� ���¿����� ��� �� �� �ְ� ���´�.
	if(HERO->InTitan() == FALSE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
		return FALSE;
	}

	if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		// Ÿ��ź ������������ �ƴϸ� FALSE
		if( ( pItem->GetItemKind() & eTITAN_EQUIPITEM ) )
		{
			// ���� ���ڸ��� �ƴϸ� FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			if(pos-TP_TITANWEAR_START != pInfo->ItemKind-eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1505) );
				return FALSE;
			}

			//2007. 10. 25. CBH - Ÿ��ź ��� ����� �������� üũ					
			if(pInfo->LimitLevel > HERO->GetLevel())
			{
				char temp[256] = {0, };
				sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->LimitLevel );
				CHATMGR->AddMsg( CTC_SYSMSG, temp );
				return FALSE;
			}


			return TRUE;
		}
	}
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		// Ÿ��ź ������������ �ƴϸ� FALSE
		if( ( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP ) )
		{
			// ���� ���ڸ��� �ƴϸ� FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			// magi82(26)
			if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
				return FALSE;


			return TRUE;
		}
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{
		// Ÿ��ź ������������ �ƴϸ� FALSE
		if( ( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP ) )
		{
			// ���� ���ڸ��� �ƴϸ� FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		if(!m_pTitanInvenWearedDlg->IsAddable(Pos - TP_TITANWEAR_START))
		{
			return m_pTitanInvenWearedDlg->DeleteItem(Pos - TP_TITANWEAR_START, (cIcon**)ppItem);

		}
		else
			return FALSE;
	}	
	else if( TP_TITANSHOPITEM_START <= Pos && Pos < TP_TITANSHOPITEM_END )
	{
		if(!m_pTitanShopItemDlg->IsAddable(Pos - TP_TITANSHOPITEM_START))
		{
			// magi82(26)
			CItem* pItem = GetItemForPos(Pos);
            if( pItem )
			{
				TITANMGR->SetTitanShopitemOption(pItem->GetItemIdx(), FALSE);
			}

			return m_pTitanShopItemDlg->DeleteIcon(Pos - TP_TITANSHOPITEM_START, (cIcon**)ppItem);
		}
		else
			return FALSE;
	}

	return FALSE;
}

BOOL CTitanInventoryDlg::AddItem( ITEMBASE* pItemBase )
{
	if( pItemBase->dwDBIdx == 0 )
	{
		//		ASSERTMSG( 0, "PetInven NewItem DBIdx is Null");
		return FALSE;
	}
	CItem* pNewItem = ITEMMGR->MakeNewItem( pItemBase, "CTitanInventoryDlg::AddItem" );
	if(pNewItem == NULL)
		return FALSE;

	return AddItem(pNewItem);
}

BOOL CTitanInventoryDlg::AddItem(CItem* pItem)
{
	if( NULL == pItem )
	{
		ASSERT(pItem);
		return FALSE;
	}

	ITEMMGR->RefreshItem(pItem);

	int Pos = pItem->GetPosition();

	if( TP_TITANWEAR_START <= Pos && Pos < TP_TITANWEAR_END )
	{
		return m_pTitanInvenWearedDlg->AddItem(Pos - TP_TITANWEAR_START, pItem);
	}
	else if( TP_TITANSHOPITEM_START <= Pos && Pos < TP_TITANSHOPITEM_END )	
	{
		// magi82(26)
		TITANMGR->SetTitanShopitemOption(pItem->GetItemIdx(), TRUE);

		return m_pTitanShopItemDlg->AddIcon(Pos - TP_TITANSHOPITEM_START, pItem);
	}
	
	return FALSE;	
}

void CTitanInventoryDlg::Render()
{
	if( !m_bActive )		return;

	cDialog::Render();
}

void CTitanInventoryDlg::SetTitanInvenInfo()
{
	if(TITANMGR->GetRegistedTitanItemDBIdx())
	{
		UpdateData();
	}
}

void CTitanInventoryDlg::SetTitanEquipInfo()
{	
	ZeroMemory( m_dwWearInfoArray, sizeof(m_dwWearInfoArray) );


	for( int i = 0; i < eTitanWearedItem_Max; i++ )
	{
		CItem * pItem = (CItem *)m_pTitanInvenWearedDlg->GetIconForIdx(i);
		if( pItem != NULL )
		{
			ITEM_INFO* pinfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			m_dwWearInfoArray[i] = pItem->GetItemIdx();
		}		
	}
}

CItem* CTitanInventoryDlg::GetItemForPos(POSTYPE absPos)
{
	if(TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END)
	{
		return (CItem*)m_pTitanInvenWearedDlg->GetIconForIdx(absPos-TP_TITANWEAR_START);
	}
	// magi82(26)
	else if(TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END)
	{
		return (CItem*)m_pTitanShopItemDlg->GetIconForIdx(absPos-TP_TITANSHOPITEM_START);

	}
	else
		return NULL;
}

DWORD CTitanInventoryDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	return we;
}

// magi82(26)
BOOL CTitanInventoryDlg::GetBlankSlotPosition( POSTYPE& pos )
{
	for( int i = 0; i < SLOT_TITANSHOPITEM_NUM; i++ )
	{
		if( m_pTitanShopItemDlg->IsAddable(i) == TRUE )
		{
            pos = TP_TITANSHOPITEM_START + i;
			return TRUE;
		}

	}
	return FALSE;
}




