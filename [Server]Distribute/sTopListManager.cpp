#include "stdafx.h"
#include "sTopListManager.h"
#include "DistributeDBMsgParser.h"
#include "Network.h"
sTopListManager* sTopListManager::g_Instance;
sTopListManager::sTopListManager(void)
{
	m_BoardUpdate=5;
	GetLocalTime(&m_tBoardTime);
	g_Instance = NULL;
	m_wTopListCount=0;
	WORD m_wTopListGuildCount=0;
	WORD m_wTopListKillerCount=0;
	m_bCanSend = FALSE;
}

sTopListManager::~sTopListManager(void)
{
	if(g_Instance)
	{
		delete g_Instance;
		g_Instance = NULL;
	}
}
/**************************************************************/
void sTopListManager::SetTopList(TOPLISTINFOHERO& TopList)
{
	if (m_wTopListCount >= MAX_TOP_LIST_COUNT) return;
	memcpy(&m_TopListInfo.HeroList[m_wTopListCount], &TopList, sizeof(TOPLISTINFOHERO));
	m_wTopListCount++;
	GetLocalTime(&m_tBoardTime);
}
void sTopListManager::SetTopList(TOPLISTINFOGUILD& TopList)
{
	if (m_wTopListGuildCount >= MAX_TOP_LIST_COUNT) return;
	memcpy(&m_TopListInfo.GuildList[m_wTopListGuildCount], &TopList, sizeof(TOPLISTINFOGUILD));
	m_wTopListGuildCount++;
}
void sTopListManager::SetTopList(KILLPLAYERTOPLIST& TopList)
{
	if (m_wTopListKillerCount >= MAX_TOP_LIST_COUNT) return;
	memcpy(&m_TopListInfo.KillerList[m_wTopListKillerCount], &TopList, sizeof(KILLPLAYERTOPLIST));
	m_wTopListKillerCount++;
}
void sTopListManager::ReloadTopList()
{
	ReleseTopList();
	TopListInfoForHero(1);
}

void sTopListManager::ReleseTopList()
{
	memset(&m_TopListInfo, 0, sizeof(TOP_LIST_INFO));

	m_wTopListCount=0;
	m_wTopListGuildCount=0;
	m_wTopListKillerCount=0;
}

sTopListManager * sTopListManager::GetInstance()
{
	if(!g_Instance)
		g_Instance = new sTopListManager;
	return g_Instance;
}
void sTopListManager::TopListPorcess()
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
	if ((sTime.wMinute%m_BoardUpdate == 0) && !m_bCanSend)
	{//[排行榜更新时间设置][By:十里坡剑神][QQ:112582793][2017/11/25]
		if (sTime.wHour == 0 && sTime.wMinute == 0 && sTime.wSecond == 0)
		{
			m_dayChar = m_NewTime.GetDayToChar(sTime.wYear, sTime.wMonth, sTime.wDay - 1);
			UpdateTopListForKiller(m_dayChar.c_str());
		}
		ReloadTopList();
		m_bCanSend = TRUE;
		GetLocalTime(&m_tBoardTime);
	}
	if (m_bCanSend&&sTime.wMinute - m_tBoardTime.wMinute >= 1)
	{
		m_bCanSend = FALSE;
	}
}
void sTopListManager::SendTopListToAG(DWORD dwConnectionIndex, DWORD dwClientConnIndex)
{
	MSG_TOP_LIST_HERO msg;
	msg.Category = MP_TOPLIST;
	msg.Protocol = MP_TOPLIST_TO_CLIENT;
	msg.HeroCount = m_wTopListCount;
	msg.GuildCount = m_wTopListGuildCount;
	msg.KillerCount = m_wTopListKillerCount;
	msg.UpdataTime = m_tBoardTime;
	msg.dwObjectID = dwClientConnIndex;
	memcpy(&msg.TopListInfo, &m_TopListInfo, sizeof(TOP_LIST_INFO));
	g_Network.Send2Server(dwConnectionIndex,(char*)&msg, sizeof(msg));

}
void sTopListManager::SendTopList(DWORD dwConnectionIndex)
{
	MSG_TOP_LIST_HERO msg;
	msg.Category = MP_TOPLIST;
	msg.Protocol = MP_TOPLIST_TO_CLIENT;
	msg.HeroCount = m_wTopListCount;
	msg.GuildCount = m_wTopListGuildCount;
	msg.KillerCount = m_wTopListKillerCount;
	msg.UpdataTime = m_tBoardTime;
	memcpy(&msg.TopListInfo, &m_TopListInfo, sizeof(TOP_LIST_INFO));
	g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
}
/**************************************************************/