#include "stdafx.h"
#include "cInventoryRBTDlg.h"
#include "InventoryExDialog.h"
#include "GameIn.h"
#include "WindowIDEnum.h"
#include "Item.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "LotSplitItemDlg.h"
#include "OptionDialog.h"
#include "ShoutDialog.h"

cInventoryRBTDlg::cInventoryRBTDlg(void)
{
	m_pSelecItem = NULL;
}

cInventoryRBTDlg::~cInventoryRBTDlg(void)
{
}
void cInventoryRBTDlg::Linking()
{

}

void cInventoryRBTDlg::OnActionEvent(LONG lId, void *p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if(lId==INVENTORY_RBT_USE)
		{
			if(!m_pSelecItem)return;
			GAMEIN->GetOptionDialog()->AddPortectItem((cIcon*)m_pSelecItem);
			//GAMEIN->GetInventoryDialog()->UseItem(m_pSelecItem);
			m_pSelecItem = NULL;
		}
		if(lId == INVENTORY_RBT_DEL)
		{
			if (m_pSelecItem)
			{
				if (ITEMMGR->GetItemRareOption(m_pSelecItem->GetRareness()) || ITEMMGR->IsOptionValuablesItem(m_pSelecItem->GetDurability()) ||
					ITEMMGR->IsValuablesItem(m_pSelecItem->GetItemIdx()))
				{//[物品删除消息提示][BY:十里坡剑神][QQ:112582793][2019-8-28][12:52:26]
					WINDOWMGR->MsgBoxTip(MSBOX_INVENTORY_RBT_DEL, MBT_YESNO, CHATMGR->GetChatMsg(2408), m_pSelecItem->GetItemInfo()->ItemName);
				}
				else
					cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_INVENTORY_RBT_DEL, MBT_YESNO, CHATMGR->GetChatMsg(363), m_pSelecItem->GetItemInfo()->ItemName);
			}
				
		}
		if(lId==INVENTORY_RBT_SEND)
		{
			char buf[1024]={0,};
			const ITEM_INFO * pInfo = m_pSelecItem->GetItemInfo();
			if(pInfo)
			{
				sprintf(buf,"[%s]&%d&%d",pInfo->ItemName,m_pSelecItem->GetDBIdx(),HEROID);
				//GAMEIN->CopyToClipboard(buf,strlen(buf));
				if (GAMEIN->GetShoutDlg())
				{
					if (GAMEIN->GetShoutDlg()->IsActive())
					{
						GAMEIN->GetShoutDlg()->SendItemInfoSyn(buf);
						return;
					}
				}
				
				CHATMGR->SendItemInfoMsg((char*)pInfo->ItemName, m_pSelecItem->GetDBIdx());
				//cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX,MBT_OK,CHATMGR->GetChatMsg(2264));
			}
		}
		if(lId==LOT_SPLIT_BIT)
		{
			LotSplitItemDlg* pDlg = GAMEIN->GetLotSplitItemDlg();
			if (pDlg)
			{
				if(!pDlg->IsActive())
					pDlg->SetActive(TRUE);
				pDlg->FakeMoveIcon(0,0,(cIcon *)m_pSelecItem);
				
			}
		}
	}

	SetActive(FALSE);
}
void cInventoryRBTDlg::DeleteItem()
{
	if(!m_pSelecItem)return;
	MSG_ITEM_DISCARD_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_DISCARD_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetPos =m_pSelecItem->GetPosition();
	msg.wItemIdx = m_pSelecItem->GetItemIdx();
	msg.ItemNum = m_pSelecItem->GetDurability();
	msg.dwItemDBIdx = m_pSelecItem->GetDBIdx();
	NETWORK->Send(&msg,sizeof(msg));
	m_pSelecItem = NULL;
}
