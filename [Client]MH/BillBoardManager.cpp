#include "stdafx.h"
#include "BillBoardManager.h"
#include "CommonStruct.h"
#include "ObjectManager.h"
#include "OnlineFame.h"
#include "cTopRankDlg.h"
#include "GameIn.h"
#include "BillboardDialog.h"
#include "GuildTopList.h"
#include "cKillTopList.h"
#include "ChatManager.h"
BillBoardManager::BillBoardManager(void)
{
	memset(&m_TopListInfo, 0, sizeof(TOP_LIST_INFO));
	m_wTopListCount=0;
	m_wTopListGuildCount=0;
	m_wTopListKillerCount=0;
}

BillBoardManager::~BillBoardManager(void)
{

}
bool BillBoardManager::IsCanUpdate()
{
	bool bRet = false;
	SYSTEMTIME t;
	GetLocalTime(&t);
	stTIME Local, UpDate,Esp;
	Local.SetTime(t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond);
	UpDate.SetTime(UpdataTime.wYear, UpdataTime.wMonth, UpdataTime.wDay, UpdataTime.wHour, UpdataTime.wMinute, UpdataTime.wSecond);
	Esp.SetTime(0, 0, 0, 0, 5, 0);
	UpDate += Esp;
	if (Local.value >= UpDate.value)
	{
		bRet = true;
	}
	if (!bRet)
	{
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2409), UpDate.GetHour(), UpDate.GetMinute());
	}
		
	return bRet;
}
//[排行标志设置][By:十里坡剑神][QQ:112582793][2018/2/25]
void BillBoardManager::SetHeroTopListFlg(CPlayer* pPlayer)
{
	if(!pPlayer) return;
	pPlayer->InitTopListFlg();

	if (m_TopListInfo.HeroList[0].dwCharacterIdx!= 0)
	{
		if (m_TopListInfo.HeroList[0].dwCharacterIdx == pPlayer->GetID())
		{
			pPlayer->SetTopListFlg(0);
			return;
		}
	}

	if (m_TopListInfo.HeroList[1].dwCharacterIdx)
	{
		if (m_TopListInfo.HeroList[1].dwCharacterIdx == pPlayer->GetID())
		{
			pPlayer->SetTopListFlg(1);
			return;
		}
	}

	if (m_TopListInfo.HeroList[2].dwCharacterIdx)
	{
		if (m_TopListInfo.HeroList[2].dwCharacterIdx == pPlayer->GetID())
		{
			pPlayer->SetTopListFlg(2);
			return;
		}
	}
}
void BillBoardManager::ResetTopList()
{
	memset(&m_TopListInfo, 0, sizeof(TOP_LIST_INFO));
}
void BillBoardManager::SetTopList(const MSG_TOP_LIST_HERO* msg)
{
	ResetTopList();
	memcpy_s(&m_TopListInfo, sizeof(m_TopListInfo), &msg->TopListInfo, sizeof(TOP_LIST_INFO));

	if (m_TopListInfo.HeroList[0].dwCharacterIdx != 0)
	{
		if(GAMEIN->GetTopRankDlg())
			GAMEIN->GetTopRankDlg()->SetCharTopText(m_TopListInfo.HeroList[0].Name);
	}

	if (m_TopListInfo.GuildList[0].GuildLevel != 0)
	{
		if (GAMEIN->GetTopRankDlg())
		{
			GAMEIN->GetTopRankDlg()->SetGuildTopText(m_TopListInfo.GuildList[0].GuildName);
			GAMEIN->GetTopRankDlg()->SetUniTopText(m_TopListInfo.GuildList[0].MasterName);
		}
	}

	m_wTopListGuildCount = msg->GuildCount;
	m_wTopListCount = msg->HeroCount;
	m_wTopListKillerCount = msg->KillerCount;

	UpdataTime = msg->UpdataTime;

	if (GAMEIN->GetBillboardDialog())
	{
		if (GAMEIN->GetBillboardDialog()->IsActive())
		{
			GAMEIN->GetBillboardDialog()->EndDialog();
			GAMEIN->GetBillboardDialog()->ShowHeroTopList();
		}
			//GAMEIN->GetBillboardDialog()->SetActive(FALSE);
	}

	if (GAMEIN->GetKillTopListDlg())
	{
		if (GAMEIN->GetKillTopListDlg()->IsActive())
		{
			GAMEIN->GetKillTopListDlg()->EndDialog();
			GAMEIN->GetKillTopListDlg()->ShowKillTopList();
		}
			//GAMEIN->GetKillTopListDlg()->SetActive(FALSE);
	}

	if (GAMEIN->GetGuildTopListDlg())
	{
		if (GAMEIN->GetGuildTopListDlg()->IsActive())
		{
			GAMEIN->GetGuildTopListDlg()->ClearList();
			GAMEIN->GetGuildTopListDlg()->ShowList();
		}
			//GAMEIN->GetGuildTopListDlg()->SetActive(FALSE);
	}
}
