#include "stdafx.h"
#include "cRechargeManager.h"
#include "Item.h"
#include "cImage.h"
#include "WindowIDEnum.h"
#include "MHFile.h"
#include "DealItem.h"
#include "ItemManager.h"
#include "./interface/cWindowManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "cMsgBox.h"
#include "cScriptManager.h"
cRechargeManager::cRechargeManager(void)
{
	LoadInfo();
}

cRechargeManager::~cRechargeManager(void)
{
	m_RechargeGiftInfo.clear();
}
void cRechargeManager::Linking()
{
	DWORD dwColor = RGBA_MAKE(0, 255, 0, 255);
	char buf[64] = { 0, };
	sprintf_s(buf, CHATMGR->GetChatMsg(2134));
	IconDlg[0]		=	(cIconGridDialog *)GetWindowForID(RECHARGE_ITEM);
	IconDlg[1]		=	(cIconGridDialog *)GetWindowForID(RECHARGE_ITEM1);
	m_btnPay	=	(cButton*)GetWindowForID(RE_BTN_RECHARGE);
	for (int i = 0; i < ePayBtnMax; i++)
	{
		m_PayBtn[i] = (cButton*)GetWindowForID(RE_BTN_100+i);
		m_PayBtn[i]->SetText(buf, dwColor);
		m_PayBtn[i]->SetDisable(FALSE);
		m_PayBtn[i]->SetActive(FALSE);
	}

	m_PayMoney	=	(cStatic*)GetWindowForID(RE_STATIC_100);
	m_PayMoneyCount	=	(cStatic*)GetWindowForID(RE_STATIC_300);
	m_Residual		=	(cStatic*)GetWindowForID(RE_STATIC_500);
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_btnPay->SetToolTip(CHATMGR->GetChatMsg(2135),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
}
void cRechargeManager::SetRechargeInfo(sCHARRECHARGEINFO &info)
{
	memcpy(&RechargeInfo,&info,sizeof(sCHARRECHARGEINFO));
	char buf[16];
	sprintf(buf,"%d",RechargeInfo.PayMoney);
	m_PayMoney->SetStaticText(buf);
	sprintf(buf,"%d",RechargeInfo.PayMoneyCount);
	m_PayMoneyCount->SetStaticText(buf);
	sprintf(buf,"%d",RechargeInfo.PayMoney-RechargeInfo.UsedMoney);
	m_Residual->SetStaticText(buf);
	InitBtnStat();
}
void cRechargeManager::SetActive(BOOL val)
{
	if(!val)
		HideWnd();
	else
		ShowWnd();
	cDialog::SetActive(val);
}
void cRechargeManager::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if (lId == RE_BTN_100||lId == RE_BTN_300||
			lId == RE_BTN_500||lId == RE_BTN_700||
			lId == RE_BTN_1000)
		{
			MSG_RECHARGE msg;
			msg.Category = MP_RECHARGE;
			msg.Protocol = MP_RECHARGE_SYN;
			msg.dwObjectID = HEROID;
			switch(lId)
			{
			case RE_BTN_100:
				{
					msg.wType = PayMoney[0];
					RechargeInfo.UsedMoney =PayMoney[0];
				}
				break;
			case RE_BTN_300:
				{
					msg.wType = PayMoney[1];
					RechargeInfo.UsedMoney =PayMoney[1];
				}
				break;
			case RE_BTN_500:
				{
					msg.wType = PayMoney[2];
					RechargeInfo.UsedMoney =PayMoney[2];
				}
				break;
			case RE_BTN_700:
				{
					msg.wType = PayMoney[3];
					RechargeInfo.UsedMoney =PayMoney[3];
				}
				break;
			case RE_BTN_1000:
				{
					msg.wType = PayMoney[4];
					RechargeInfo.UsedMoney =PayMoney[4];
				}
				break;
			}
			NETWORK->Send(&msg,sizeof(msg));
			InitBtnStat();
		}
		
	}
}
void cRechargeManager::LoadInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/RechargeRecordInfo.bin","rb"))
		return;
	while (1)
	{
		if(file.IsEOF())
			break;
		m_RechargeGiftInfo[file.GetDword()] = file.GetWord();
	}
	file.Release();
	std::map<int,int>::iterator iter;
	iter = m_RechargeGiftInfo.begin();
	for(int i=0;i<5;i++,iter++)
	{
		PayMoney[i] = iter->first;
	}
}
void cRechargeManager::HideWnd()
{
	cIcon* pIcon;
	for(int i = 0;i<3;i++)
	{
		pIcon = IconDlg[0]->GetIconForIdx(i);
		if(pIcon != NULL)
		{
			IconDlg[0]->DeleteIcon(i,&pIcon);
			pIcon->SetActive( FALSE );
			WINDOWMGR->AddListDestroyWindow( pIcon );
			pIcon = NULL;
		}
	}
	for(int i = 0;i<2;i++)
	{
		pIcon = IconDlg[1]->GetIconForIdx(i);
		if(pIcon!=NULL)
		{
			IconDlg[1]->DeleteIcon(i,&pIcon);
			pIcon->SetActive(FALSE);
			WINDOWMGR->AddListDestroyWindow(pIcon);
			pIcon = NULL;
		}
	}
}
void cRechargeManager::ShowWnd()
{
	std::map<int,int>::iterator iter;
	CDealItem* pItem = NULL;
	cImage tmpImage;
	int i = 0,j = 0;;
	int idx = 0,idx1=0;
	iter=m_RechargeGiftInfo.begin();
	for (;iter!=m_RechargeGiftInfo.end();iter++)
	{
		pItem = new CDealItem;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(iter->second, &tmpImage),IG_DEALITEM_START+i++);
		pItem->SetData(iter->second);
		pItem->SetItemIdx(iter->second);
		pItem->m_wIdx=i;
		pItem->SetMovable(FALSE);
		ITEMMGR->SetGoldShopToolTipIcon(pItem);
		WINDOWMGR->AddWindow(pItem);
		if(idx<3)
			IconDlg[0]->AddIcon(idx,pItem);
		else
		{
			IconDlg[1]->AddIcon(idx1,pItem);
			j++;
			idx1++;
		}
		idx++;
	}
}

void cRechargeManager::InitBtnStat()
{
	if (RechargeInfo.PayMoney<PayMoney[0])
		return;

	char buf[32] = { 0, };
	DWORD dwColor = RGBA_MAKE(0, 255, 0, 255);
	sprintf_s(buf, CHATMGR->GetChatMsg(2134));
	for (int i = 0; i < ePayBtnMax; i++)
	{
		m_PayBtn[i]->SetText(buf, dwColor);
		m_PayBtn[i]->SetDisable(FALSE);
		m_PayBtn[i]->SetActive(FALSE);
	}
		
	if(RechargeInfo.PayMoney>=PayMoney[4])
	{
		for (int i = 0; i < ePayBtnMax; i++)
			m_PayBtn[i]->SetActive(TRUE);
	}
	else if(RechargeInfo.PayMoney>=PayMoney[3])
	{
		m_PayBtn[ePayBtn1]->SetActive(TRUE);
		m_PayBtn[ePayBtn2]->SetActive(TRUE);
		m_PayBtn[ePayBtn3]->SetActive(TRUE);
		m_PayBtn[ePayBtn4]->SetActive(TRUE);
	}
	else if(RechargeInfo.PayMoney>=PayMoney[2])
	{
		m_PayBtn[ePayBtn1]->SetActive(TRUE);
		m_PayBtn[ePayBtn2]->SetActive(TRUE);
		m_PayBtn[ePayBtn3]->SetActive(TRUE);
	}
	else if(RechargeInfo.PayMoney>=PayMoney[1])
	{
		m_PayBtn[ePayBtn1]->SetActive(TRUE);
		m_PayBtn[ePayBtn2]->SetActive(TRUE);
	}
	else if(RechargeInfo.PayMoney>=PayMoney[0])
	{
		m_PayBtn[ePayBtn1]->SetActive(TRUE);
	}
	sprintf_s(buf, CHATMGR->GetChatMsg(2130));
	dwColor = RGBA_MAKE(255, 0, 0, 255);
	if (RechargeInfo.UsedMoney>=PayMoney[4])
	{
		for (int i = 0; i < ePayBtnMax; i++)
		{
			m_PayBtn[i]->SetText(buf, dwColor);
			m_PayBtn[i]->SetDisable(TRUE);
		}
	}
	else if(RechargeInfo.UsedMoney>=PayMoney[3])
	{
		m_PayBtn[ePayBtn1]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn1]->SetDisable(TRUE);
		m_PayBtn[ePayBtn2]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn2]->SetDisable(TRUE);
		m_PayBtn[ePayBtn3]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn3]->SetDisable(TRUE);
		m_PayBtn[ePayBtn4]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn4]->SetDisable(TRUE);
	}
	else if(RechargeInfo.UsedMoney>=PayMoney[2])
	{
		m_PayBtn[ePayBtn1]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn1]->SetDisable(TRUE);
		m_PayBtn[ePayBtn2]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn2]->SetDisable(TRUE);
		m_PayBtn[ePayBtn3]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn3]->SetDisable(TRUE);
	}
	else if(RechargeInfo.UsedMoney>=PayMoney[1])
	{
		m_PayBtn[ePayBtn1]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn1]->SetDisable(TRUE);
		m_PayBtn[ePayBtn2]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn2]->SetDisable(TRUE);
	}
	else if(RechargeInfo.UsedMoney>=PayMoney[0])
	{
		m_PayBtn[ePayBtn1]->SetText(buf, dwColor);
		m_PayBtn[ePayBtn1]->SetDisable(TRUE);
	}
}