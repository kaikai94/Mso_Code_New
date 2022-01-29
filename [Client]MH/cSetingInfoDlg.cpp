#include "stdafx.h"
#include "cSetingInfoDlg.h"
#include "WindowIDEnum.h"
#include "cScriptManager.h"
#include "ChatManager.h"
#include "cButton.h"
#include "ObjectManager.h"
cSetingInfoDlg::cSetingInfoDlg(void)
{
	m_RestTimes = NULL;
	m_RestLevel = NULL;
	m_RestOpenTime = NULL;
	m_SprofitTime = NULL;
	m_SiegeWarState = NULL;
	m_dwGetInfo = 0;
}

cSetingInfoDlg::~cSetingInfoDlg(void)
{
}

void cSetingInfoDlg::Linking()
{
	m_RestTimes		= (cStatic *)GetWindowForID(SYS_INFO_STATIC_ZS);
	m_RestLevel		= (cStatic *)GetWindowForID(SYS_INFO_STATIC_DJ);
	m_RestOpenTime	= (cStatic *)GetWindowForID(SYS_INFO_STATIC_TIME);
	m_SprofitTime	= (cStatic *)GetWindowForID(SYS_INFO_STATIC_PRTIME);
	m_SiegeWarState = (cStatic *)GetWindowForID(SYS_INFO_STATIC_SIEGEWAR);

	m_RestTimes->SetAlign(TXT_CENTER);
	m_RestLevel->SetAlign(TXT_CENTER);
	m_RestOpenTime->SetAlign(TXT_CENTER);
	m_SprofitTime->SetAlign(TXT_CENTER);
	m_SiegeWarState->SetAlign(TXT_CENTER);

	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	((cButton*)this->GetWindowForID(SYS_INFO_BTN_CLOSE))->SetToolTip(CHATMGR->GetChatMsg(2136),RGBA_MAKE(255, 255, 255, 255),&imgToolTip,TTCLR_ITEM_CANEQUIP);
	this->SetAlwaysTop(TRUE);
}
void cSetingInfoDlg::SetSiegeWarInfo(WORD val)
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	switch(val)
	{
	case 0:
		sprintf(buf,CHATMGR->GetChatMsg(2165));
		break;
	case 1:
		sprintf(buf,CHATMGR->GetChatMsg(2166));
		break;
	case 2:
		sprintf(buf,CHATMGR->GetChatMsg(2167));
		break;
	case 3:
		sprintf(buf,CHATMGR->GetChatMsg(2168));
		break;
	case 4:
		sprintf(buf,CHATMGR->GetChatMsg(2169));
		break;
	case 5:
		sprintf(buf,CHATMGR->GetChatMsg(2170));
		break;
	}
	m_SiegeWarState->SetStaticText(buf);
}
void cSetingInfoDlg::Show(BOOL val)
{
	if(!val)
		SetActive(FALSE);
	else
	{
		if(gCurTime-m_dwGetInfo>=60000||m_dwGetInfo==0)//[10分钟主动刷新间隔][By:十里坡剑神][QQ:112582793][2018/3/10]
		{
			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_CHANGELIFE_INFO_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			m_dwGetInfo = gCurTime;
		}
		else
			SetActive(TRUE);
	}
}
void cSetingInfoDlg::NetWorkParser(MSGRESTINFO * msg)
{
	SetAllInfo(msg);
	SetActive(TRUE);
}
void cSetingInfoDlg::SetRestSproFitTime(DWORD val)
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	stTIME stOpenTime;
	SYSTEMTIME syTime;

	stOpenTime.SetTime(val);
	stOpenTime.GetTimeToSystemTime(syTime);

	sprintf(buf,CHATMGR->GetChatMsg(2173),syTime.wYear,
		syTime.wMonth,syTime.wDay,syTime.wHour,syTime.wMinute);
	m_SprofitTime->SetStaticText(buf);
}
void cSetingInfoDlg::SetRestOpenTime(DWORD val)
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	stTIME stOpenTime;
	SYSTEMTIME syTime;

	stOpenTime.SetTime(val);
	stOpenTime.GetTimeToSystemTime(syTime);

	sprintf(buf,CHATMGR->GetChatMsg(2173),syTime.wYear,
		syTime.wMonth,syTime.wDay,syTime.wHour,syTime.wMinute);
	m_RestOpenTime->SetStaticText(buf);
}
void cSetingInfoDlg::SetRestLevel()
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	if (HERO)
	{
		if (HERO->GetReset() == m_wRestTimes)
		{
			sprintf(buf, CHATMGR->GetChatMsg(2137));
		}
		else
		{
			sprintf(buf, "%d", HERO->GetReset() + m_wMinRestLevel);
		}
		m_RestLevel->SetStaticText(buf);
	}
}
void cSetingInfoDlg::SetRestTimesAdd()
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	m_wRestTimes +=1;
	sprintf(buf,"%d",m_wRestTimes);
	m_RestTimes->SetStaticText(buf);
	SetRestLevel();
}
void cSetingInfoDlg::SetAllInfo(MSGRESTINFO * msg)
{
	char buf[64];
	memset(&buf,0,sizeof(buf));
	m_wMinRestLevel=msg->wMinRestLevel;
	m_wRestTimes=msg->wMaxRestSet;

	sprintf(buf,"%d",m_wRestTimes);
	m_RestTimes->SetStaticText(buf);
	if (HERO)
	{
		if (HERO->GetReset() == m_wRestTimes)
		{
			sprintf(buf, CHATMGR->GetChatMsg(2137));
		}
		else
		{
			sprintf(buf, "%d", HERO->GetReset() + m_wMinRestLevel);
		}
		m_RestLevel->SetStaticText(buf);
	}
	

	stTIME OpenTime;
	SYSTEMTIME syTime;
	OpenTime.SetTime(msg->dwRestOpenTime);
	OpenTime.GetTimeToSystemTime(syTime);
	sprintf(buf,CHATMGR->GetChatMsg(2173),syTime.wYear,
		syTime.wMonth,syTime.wDay,syTime.wHour,syTime.wMinute);
	m_RestOpenTime->SetStaticText(buf);

	OpenTime.SetTime(msg->dwRestProfitTime);
	OpenTime.GetTimeToSystemTime(syTime);
	sprintf(buf,CHATMGR->GetChatMsg(2173),syTime.wYear,
		syTime.wMonth,syTime.wDay,syTime.wHour,syTime.wMinute);
	m_SprofitTime->SetStaticText(buf);

	SetSiegeWarInfo(msg->wSiegeWarState);
}