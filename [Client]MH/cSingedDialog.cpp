#include "stdafx.h"
#include "cSingedDialog.h"
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
cSingedDialog::cSingedDialog(void)
{
	m_DayInfo[0] = "星期一";
	m_DayInfo[1] = "星期二";
	m_DayInfo[2] = "星期三";
	m_DayInfo[3] = "星期四";
	m_DayInfo[4] = "星期五";
	m_DayInfo[5] = "星期六";
	m_DayInfo[6] = "星期日";
	m_DayInfo[7] = "满签";
	SYSTEMTIME time;
	GetLocalTime(&time);
	m_day = m_Date.GetDayToInt(time.wYear,time.wMonth,time.wDay);
	LoadGiftInfo();
}

cSingedDialog::~cSingedDialog(void)
{
	m_SingedGiftInfo.clear();
	m_DayInfo.clear();
}
void cSingedDialog::Linking()
{
	IconDlg			=(cIconGridDialog *)GetWindowForID(SINGED_ITEM);
	m_btnMonDay		=(cButton*)GetWindowForID(SINGED_MON);
	m_btnTuesDay	=(cButton*)GetWindowForID(SINGED_TUES);
	m_btnWednesDay	=(cButton*)GetWindowForID(SINGED_WED);
	m_btnThursday	=(cButton*)GetWindowForID(SINGED_THUR);
	m_btnFriDay		=(cButton*)GetWindowForID(SINGED_FIR);
	m_btnSaturDay	=(cButton*)GetWindowForID(SINGED_SAT);
	m_btnSunDay		=(cButton*)GetWindowForID(SINGED_SUN);
	m_btnCompelet	=(cButton*)GetWindowForID(SINGED_COMPLETE);
}
void cSingedDialog::LoadGiftInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/SingedGiftInfo.bin","rb"))
		return;
	char Day[16];
	while(1)
	{
		if(file.IsEOF())
			break;
		SINGEDGIFTINFO info;

		file.GetString(Day);
		std::string temp = Day;
		info.wCount = file.GetWord();
		info.wItemIdx = file.GetWord();
		m_SingedGiftInfo[temp]=info;
	}
	Giftiter =m_SingedGiftInfo.find("补签元宝");
	if (Giftiter != m_SingedGiftInfo.end())
		NeedGoldMoney=Giftiter->second.wItemIdx;
	
	file.Release();
}
void cSingedDialog::Show()
{
	CDealItem* pItem = NULL;
	cImage tmpImage;
	int i = 0;
	int idx = 0;
	Giftiter =m_SingedGiftInfo.find(m_DayInfo[i]);
	while(Giftiter!=m_SingedGiftInfo.end())
	{
		pItem = new CDealItem;
		pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(Giftiter->second.wItemIdx, &tmpImage),IG_DEALITEM_START+i++);
		pItem->SetData(Giftiter->second.wItemIdx);
		pItem->SetItemIdx(Giftiter->second.wItemIdx);
		pItem->m_wIdx=i;
		pItem->SetMovable(FALSE);
		ITEMMGR->SetGoldShopToolTipIcon(pItem);
		WINDOWMGR->AddWindow(pItem);
		IconDlg->AddIcon(idx,pItem);
		idx++;
		Giftiter =m_SingedGiftInfo.find(m_DayInfo[i]);
	}
}
void cSingedDialog::ClearItem()
{
	cIcon* pIcon;
	for(int n=0;n<8;++n)
	{
		pIcon = IconDlg->GetIconForIdx(n);
		if(pIcon != NULL)
		{
			IconDlg->DeleteIcon(n,&pIcon);
			pIcon->SetActive( FALSE );
			WINDOWMGR->AddListDestroyWindow( pIcon );
			pIcon = NULL;
		}
	}
}
void cSingedDialog::SetActive(BOOL val)
{
	if (!val)
		ClearItem();
	else
		Show();
	cDialog::SetActive(val);
}
void cSingedDialog::SetSingedInfo(SINGEDINFO &info)
{
	memcpy(&sSingedInfo,&info,sizeof(SINGEDINFO));
	//InitButtonInfo();
}

void cSingedDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		m_SendMsg.Category = MP_SINGED;
		m_SendMsg.Protocol = MP_SINGED_GETGIFT_SYN;
		m_SendMsg.dwObjectID = HERO->GetID();
		if( lId == SINGED_MON )
		{
			m_SendMsg.eType = eMonday;
			if (m_day==eMonday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);

		}
		if( lId == SINGED_TUES )
		{
			m_SendMsg.eType = eTuesday;
			if (m_day==eTuesday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_WED )
		{
			m_SendMsg.eType = eWednesday;
			if (m_day==eWednesday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_THUR )
		{
			m_SendMsg.eType = eThursday;
			if (m_day==eThursday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_FIR )
		{
			m_SendMsg.eType = eFriday;
			if (m_day==eFriday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_SAT )
		{
			m_SendMsg.eType = eSaturday;
			if (m_day==eSaturday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_SUN )
		{
			m_SendMsg.eType = eSunday;
			if (m_day==eSunday)
				NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
			else
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_SINGED,MBT_YESNO,CHATMGR->GetChatMsg(2132),NeedGoldMoney);
		}
		if( lId == SINGED_COMPLETE )
		{
			m_SendMsg.eType = eCompelete;
			NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
		}
	}
}
void cSingedDialog::SendLaterMsg()
{
	if (HERO->GetGoldMoney()<100)
	{
		cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG,MBT_OK,CHATMGR->GetChatMsg(1269));
		ClearLaterMsg();
		return;
	}
	NETWORK->Send(&m_SendMsg,sizeof(m_SendMsg));
}
void cSingedDialog::InitButtonInfo()
{
	m_btnMonDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnThursday->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnFriDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnSaturDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnSunDay->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));
	m_btnCompelet->SetText(CHATMGR->GetChatMsg(2131),RGB_HALF(225,225,225));

	m_btnMonDay->SetDisable(TRUE);
	m_btnTuesDay->SetDisable(TRUE);
	m_btnWednesDay->SetDisable(TRUE);
	m_btnThursday->SetDisable(TRUE);
	m_btnFriDay->SetDisable(TRUE);
	m_btnSaturDay->SetDisable(TRUE);
	m_btnSunDay->SetDisable(TRUE);
	m_btnCompelet->SetDisable(TRUE);
	if (m_day==eMonday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eTuesday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eWednesday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Wed)
		{
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnWednesDay->SetDisable(FALSE);
		}
		else
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eThursday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Wed)
		{
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnWednesDay->SetDisable(FALSE);
		}
		else
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Thur)
		{
			m_btnThursday->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnThursday->SetDisable(FALSE);
		}
		else
			m_btnThursday->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eFriday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Wed)
		{
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnWednesDay->SetDisable(FALSE);
		}
		else
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Thur)
		{
			m_btnThursday->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnThursday->SetDisable(FALSE);
		}
		else
			m_btnThursday->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Fri)
		{
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnFriDay->SetDisable(FALSE);
		}
		else
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eSaturday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Wed)
		{
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnWednesDay->SetDisable(FALSE);
		}
		else
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Thur)
		{
			m_btnThursday->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnThursday->SetDisable(FALSE);
		}
		else
			m_btnThursday->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Fri)
		{
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnFriDay->SetDisable(FALSE);
		}
		else
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Sat)
		{
			m_btnSaturDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnSaturDay->SetDisable(FALSE);
		}
		else
			m_btnSaturDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
	}
	if (m_day==eSunday)
	{
		if (sSingedInfo.Mon)
		{
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnMonDay->SetDisable(FALSE);
		}
		else
			m_btnMonDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Tues)
		{
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnTuesDay->SetDisable(FALSE);
		}
		else
			m_btnTuesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Wed)
		{
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnWednesDay->SetDisable(FALSE);
		}
		else
			m_btnWednesDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Thur)
		{
			m_btnThursday->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnThursday->SetDisable(FALSE);
		}
		else
			m_btnThursday->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Fri)
		{
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnFriDay->SetDisable(FALSE);
		}
		else
			m_btnFriDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Sat)
		{
			m_btnSaturDay->SetText(CHATMGR->GetChatMsg(1836),RGB_HALF(255,225,255));
			m_btnSaturDay->SetDisable(FALSE);
		}
		else
			m_btnSaturDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (sSingedInfo.Sun)
		{
			m_btnSunDay->SetText(CHATMGR->GetChatMsg(1835),RGB_HALF(0,225,0));
			m_btnSunDay->SetDisable(FALSE);
		}
		else
			m_btnSunDay->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));
		if (!sSingedInfo.Mon&&!sSingedInfo.Tues&&!sSingedInfo.Wed&&!sSingedInfo.Thur&&!sSingedInfo.Fri&&!
			sSingedInfo.Sat&&!sSingedInfo.Sun&&sSingedInfo.Complete)
		{
			m_btnCompelet->SetText(CHATMGR->GetChatMsg(2134),RGB_HALF(0,225,0));
			m_btnCompelet->SetDisable(FALSE);
		}
		else if (!sSingedInfo.Mon&&!sSingedInfo.Tues&&!sSingedInfo.Wed&&!sSingedInfo.Thur&&!sSingedInfo.Fri&&!
			sSingedInfo.Sat&&!sSingedInfo.Sun&&!sSingedInfo.Complete)
			m_btnCompelet->SetText(CHATMGR->GetChatMsg(2130),RGB_HALF(255,0,0));			
	}

}
void cSingedDialog::Process()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	m_day = m_Date.GetDayToInt(time.wYear,time.wMonth,time.wDay);
	InitButtonInfo();
	if (m_day==eMonday&&time.wHour==0&&time.wMinute==0&&
		time.wMinute==0&&time.wSecond==0)
		memset(&sSingedInfo,1,sizeof(sSingedInfo));
}