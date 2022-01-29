







// GuildWarehouseDialog.cpp: implementation of the CGuildWarehouseDialog class.
//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarehouseDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cStatic.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "GuildManager.h"
#include "./Input/UserInput.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "./interface/cWindowManager.h"
#include "cDivideBox.h"
#include "InventoryExDialog.h"
#include "./MHMap.h"

#include "PetManager.h"
#include "TitanManager.h"
#include "./input/UserInput.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CGuildWarehouseDialog::CGuildWarehouseDialog()
{
	m_type = WT_GUILDWAREHOUSEDLG;
	m_Lock = FALSE;
	m_InitInfo = FALSE;
}


CGuildWarehouseDialog::~CGuildWarehouseDialog()
{


}

void CGuildWarehouseDialog::Linking()
{
	m_pGuildMoney = (cStatic *)GetWindowForID(GDW_MONEY);
}

void CGuildWarehouseDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if(GetLock() == TRUE)
			return;
		SetLock(TRUE);
		GUILDMGR->GuildWarehouseInfoSyn(GetCurTabNum());
		if(GAMEIN->GetInventoryDialog()->IsActive() == FALSE)
			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			
	}
	else
	{
		if(HERO == 0)
			return;
		if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
		{
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		GUILDMGR->GuildWarehouseLeave(GetCurTabNum());
		cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY );
		if( pDivideBox )
		{

			pDivideBox->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow( pDivideBox );

		}

	}
	cTabDialog::SetActive(val);
}

DWORD CGuildWarehouseDialog::ActionEvent(CMouse * mouseInfo)
{
	// �������� �޴� ���̸� ��ٸ�. 
	// ������ �� ������ �� Ǯ�� 
	// �ٸ� ��ư�� ���õǸ� ���� �� MSG_BYTE�� MP_ITEM_GUILD_WAREHOUSE_LEAVE ���� 
	// ��ư �������� ������ �����޶�� ��û. �ޱ� ���� �ٸ� �� �� ������ �� 
	//GUILDMGR->GuildWarehouseInfoSyn();
	if(GetLock())
		return NULL;
	return cTabDialog::ActionEvent( mouseInfo );
}

void CGuildWarehouseDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(!m_bActive) return;
	if(GetLock())
		return;
	if(we & WE_PUSHDOWN)
	{	
		GUILDMGR->GuildWarehouseLeave(!(lId - GDW_WAREHOUSE1BTN));
		GUILDMGR->GuildWarehouseInfoSyn((BYTE)(lId - GDW_WAREHOUSE1BTN));
		SetLock(TRUE);
	}
	if (we == WE_RBTNCLICK)
	{
		//[�Ҽ��ִ�][BY:ʮ���½���][QQ:112582793][2019-8-24][19:09:06]
		POSTYPE pos = 0;
		if (GetPositionForXYRef(0, g_UserInput.GetMouse()->GetMouseEventX(),
			g_UserInput.GetMouse()->GetMouseEventY(), pos))
		{
			CItem * pItem = NULL;
			pItem = GetItemForPos(pos);
			if (!pItem)
				return;
			GAMEIN->GetInventoryDialog()->GetItemFromPyoguk(pItem);
		}	
	}
}

void CGuildWarehouseDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}

void CGuildWarehouseDialog::Add(cWindow * window)

{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}

BOOL CGuildWarehouseDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )
		return FALSE;

	if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}

	return FALSE;
}


BOOL CGuildWarehouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	if( pFromItem->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;
	}

	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	
		return FALSE;
	if( pFromItem->IsLocked() ) 
		return FALSE;
	CItem * pToItem = GetItemForPos( ToPos );
	if( pToItem )
	{
		return FALSE;
	}

	const ITEMBASE* pInfo = pFromItem->GetItemBaseInfo();
	if( PETMGR->IsCurPetSummonItem(pInfo->dwDBIdx) )

		return FALSE;

	if( ITEMMGR->IsPetSummonItem(pInfo->wIconIdx) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1258) );
		return FALSE;
	}

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pInfo->wIconIdx);
	if( !pItemInfo )
		return FALSE;

	if(pItemInfo->ItemKind == eTITAN_ITEM_PAPER)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "Ÿ��ź ������ ����â�� ������ �����ϴ�.");

		return FALSE;
	}


	// magi82(20) - Titan(071105) ���� Ÿ��ź ���� �������� â�� ���� �ʴ´�.
	if(TITANMGR->IsRegistedTitan(pFromItem->GetDBIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
		return FALSE;
	}

	if( ITEMMGR->IsTitanEquipItem(pInfo->wIconIdx) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, "�̵� �� �� ���� ������ �Դϴ�.");
		return FALSE;
	}


	if(TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_INVENTORY_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_INVENTORY_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_WEAR_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_WEAR_END)
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );

	return FALSE;
}

BOOL CGuildWarehouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	BYTE FromTable = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromTable >= eItemTable_Max) return FALSE;
	if( pToItem )
	{
		return FALSE;
	}
	if(pFromItem->GetStatic() ==1 )
	{//[�ݵ���Ʒ��ֹ�������ɲֿ�][By:ʮ���½���][QQ:112582793][2017/12/16]
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2258));
		return FALSE;
	}
	if(FromTable == eItemTable_Pyoguk)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(47) );
		return FALSE;
	}

	BYTE ToTable = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToTable >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_GUILD_MOVE_SYN;
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

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}

BOOL CGuildWarehouseDialog::CanBeMoved(CItem* pItem,POSTYPE toPos)

{
	if(pItem == 0)
		return FALSE;

	
	POSTYPE fromPos = pItem->GetPosition();
	
	if(TP_GUILDWAREHOUSE_START<= toPos && toPos < TP_GUILDWAREHOUSE_END)
	{
		return TRUE;
	}
	return FALSE;

}

BOOL CGuildWarehouseDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;										

	cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
	if(gridDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position+TP_GUILDWAREHOUSE_START + TABCELL_GUILDWAREHOUSE_NUM * GetCurTabNum();
		return TRUE;
	}
	return FALSE;
}

CItem * CGuildWarehouseDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END)
	{
		int TabIndex = GetCurTabNum();
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(absPos < (TP_GUILDWAREHOUSE_START+TABCELL_GUILDWAREHOUSE_NUM*TabIndex))
			return NULL;
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex);
	}
	return NULL;
}


BOOL CGuildWarehouseDialog::AddItem(ITEMBASE* pItemInfo)
{
	if(pItemInfo->dwDBIdx == 0)
	{
		ASSERTMSG(0,"���â��-������ ����ε����� 0. Invalid GuildWarehouse Index.");
		return FALSE;
	}

	CItem* pItem = ITEMMGR->GetItem(pItemInfo->dwDBIdx);
	if(pItem == 0)
		pItem = ITEMMGR->MakeNewItem(pItemInfo,"CGuildWarehouseDialog::AddItem");
	ASSERT(pItem);

	return AddItem(pItem);
}

BOOL CGuildWarehouseDialog::AddItem(CItem* pItem)
{
	ITEMMGR->RefreshItem(pItem);
	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());
	int tabIndex = GetTabIndex( pItem->GetPosition() );
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
	return dlg->AddIcon(relPos, pItem);
}


BOOL CGuildWarehouseDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	int TabIndex = GetTabIndex( Pos );
	cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
	if(!Dlg->IsAddable(Pos - TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex))
		return Dlg->DeleteIcon(Pos-TP_GUILDWAREHOUSE_START-TABCELL_GUILDWAREHOUSE_NUM*TabIndex, (cIcon **)ppItem);
	else
		return FALSE;

}

POSTYPE CGuildWarehouseDialog::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - TP_GUILDWAREHOUSE_START ) % TABCELL_GUILDWAREHOUSE_NUM;
}

WORD CGuildWarehouseDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_GUILDWAREHOUSE_START ) / TABCELL_GUILDWAREHOUSE_NUM;
}

void CGuildWarehouseDialog::SetLock(BOOL val)
{
	m_Lock = val;
}

BOOL CGuildWarehouseDialog::GetLock()
{
	return m_Lock;
}

void CGuildWarehouseDialog::SetWarehouseInfo(SEND_MUNPA_WAREHOUSE_ITEM_INFO* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->bTabNum);

	CItem * pItem = NULL;

	for(int i=0; i<TABCELL_GUILDWAREHOUSE_NUM; ++i)
	{	
		if(GAMEIN->GetGuildWarehouseDlg()->DeleteItem(TP_GUILDWAREHOUSE_START+i+TABCELL_GUILDWAREHOUSE_NUM*GetCurTabNum(), &pItem) == TRUE)
			ITEMMGR->ItemDelete(pItem);
		if(pInfo->info.WarehouseItem[i].dwDBIdx != 0)
			AddItem(&pInfo->info.WarehouseItem[i]);
	}
	SetMoney(pInfo->info.money);

	SetLock(FALSE);
}

void CGuildWarehouseDialog::PutInOutMoney(int vals)
{	
	CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();

	cStatic* pGuildMoney = (cStatic*)WINDOWMGR->GetWindowForIDEx(GDW_MONEY);

	if(vals)
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pGuildMoney->GetAbsX()-50) , (LONG)(pGuildMoney->GetAbsY()-100), OnPutInMoneyGuildOk, NULL, pInvenDlg, this, CHATMGR->GetChatMsg(230) );
	}
	else
	{
		cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(pGuildMoney->GetAbsX()-50) , (LONG)(pGuildMoney->GetAbsY()-100), OnPutOutMoneyGuildOk, NULL, pInvenDlg, this, CHATMGR->GetChatMsg(231) );
	}
}



void CGuildWarehouseDialog::OnPutInMoneyGuildOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	if(param1 == 0)	return;

	if(HERO->GetMoney() < param1)
		param1 = HERO->GetMoney();

	if( HERO->GetState() != eObjectState_Deal )
		return;
	if( GUILDMGR->GetLocation() != MAP->GetMapNum() )
		return;
	if( !GAMEIN->GetGuildWarehouseDlg()->IsActive() )
		return;

	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PUTIN_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;
	
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildWarehouseDialog::PutInOutMoneyResult(MSG_DWORD3 * msg)
{
	if(HEROID == msg->dwData1)
	{
		HERO->SetMoney(msg->dwData2);
	//	GAMEIN->GetCharacterDialog()->UpdateData();
		
	}
	SetMoney(msg->dwData3);
}

void CGuildWarehouseDialog::OnPutOutMoneyGuildOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	if(param1 == 0)	return;

	if( HERO->GetState() != eObjectState_Deal )
		return;
	if( GUILDMGR->GetLocation() != MAP->GetMapNum() )
		return;
	if( !GAMEIN->GetGuildWarehouseDlg()->IsActive() )
		return;

	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PUTOUT_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;
	
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildWarehouseDialog::SetMoney(MONEYTYPE money)
{
	m_pGuildMoney->SetStaticText(AddComma(money));
}

//[�Ҽ��ִ�][BY:ʮ���½���][QQ:112582793][2019-8-24][19:09:06]
void CGuildWarehouseDialog::MoveItemToGuildPyoguk(CItem* pFromItem)
{
	CItem * pToItem = NULL;
	for (int i = TP_GUILDWAREHOUSE_START + m_bSelTabNum * TABCELL_GUILDWAREHOUSE_NUM; i < i+TABCELL_GUILDWAREHOUSE_NUM; i++)
	{
		pToItem = GetItemForPos(i);
		if (pToItem == NULL)
		{
			FakeGeneralItemMove(i, pFromItem, NULL);
			return;
		}
			
	}	
}

