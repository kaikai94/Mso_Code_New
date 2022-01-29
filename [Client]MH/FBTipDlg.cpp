#include "stdafx.h"
#include "WindowIDEnum.h"
#include "FBTipDlg.h"
#include "ItemManager.h"
#include "CommonStruct.h"
#include "MHTimeManager.h"
#include "MHFile.h"
#include "GameResourceManager.h"
#include "ChatManager.h"

#include "ObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DWORD FBTipDlg::m_TipTime;
FBTipDlg::FBTipDlg()
{
	//m_type = WT_FBTIP_DLG;
	Day = 0;
	Minute=0;
	Hour=0;
	Sec=0;
	
	m_StartTip = FALSE;
	Remaintime = -1;
	m_TimeEdit = NULL;
}

FBTipDlg::~FBTipDlg()
{
	Day = 0;
	Minute = 0;
	Hour = 0;
	Sec = 0;
	m_StartTip = FALSE;
	m_TimeEdit = NULL;
	Remaintime = -1;
}

void FBTipDlg::Linking()
{
	TimeTip = (cStatic*)GetWindowForID(FB_TIMETIPSTATIC);
	m_TimeEdit = (cEditBox*)GetWindowForID(FB_TIEM_SET_EDT);
	if (m_TimeEdit)
		m_TimeEdit->SetValidCheck(1);
}
void FBTipDlg::StartTip()
{
	m_TipCurTime = MHTIMEMGR->GetNewCalcCurTime();
	m_TipTime = m_TipCurTime - 1000;
}
void FBTipDlg::UpDataTime()
{
	if (Remaintime <= 0)
	{
		//m_StartTip = FALSE;
		return;
	}
		
	if(gCurTime-m_TipTime>=1000)
	{	
		m_TipTime +=1000;

		Day = 0;
		Hour = 0;
		Minute = 0;
		Sec =Remaintime/1000;

		Minute = Sec/60;
		Sec -=Minute*60;

		if(Minute>=60)
		{
			Hour = Minute/60;
			Minute -= Hour*60;
		}
		if (Hour >= 24)
		{
			Day = Hour/24;
			Hour -= Day * 24;
		}
		char buff[64];
		sprintf(buff,CHATMGR->GetChatMsg( 2190 ),Day,Hour,Minute,Sec);
		TimeTip->SetStaticText(buff);
		Remaintime -=1000;
	}
}
void FBTipDlg::Open()
{
	UpDataTime();
	SetActive(TRUE);
}


void FBTipDlg::Close()
{
	SetActive(FALSE);
	Minute = 0;
	Hour = 0;
	Sec = 0;
	m_StartTip = FALSE;
}
void FBTipDlg::SetUseItemTime(int nTime, WORD wItemIdx)
{ 
	Remaintime = nTime;
	m_wUseItemIdx = wItemIdx;
}
void FBTipDlg::CalcAddTime()
{
	WORD wTime = atoi(m_TimeEdit->GetEditText());
	if (wTime == 0)
		return;
	if (HERO->GetGoldMoney() < wTime * 1000)
		return;
	MSG_INT2 msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEX_FBTIMEADD_SYN;
	msg.dwObjectID = HEROID;
	msg.nData1 = wTime;
	msg.nData2 = m_wUseItemIdx;
	NETWORK->Send(&msg, sizeof(msg));
}