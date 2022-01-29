#include "stdafx.h"
#include "sRestManager.h"
#include "Player.h"
#include "Network.h"
#include "MHFile.h"
#include "Console.h"
#include "UserTable.h"
#include "MapDBMsgParser.h"
#include "SiegeWarMgr.h"
#include <fstream>
sRestManager::sRestManager(void)
{
	m_wInterval = 14;
}

sRestManager::~sRestManager(void)
{
	m_RsetSetStaut.clear();
}
void sRestManager::InitRestConfig(WORD mapnum,BOOL bReSetPower,WORD wMaxReSetTime,WORD wMinReSetLevel,DWORD dwReSetMoney)
{
	m_MapNum = mapnum;
	m_sRestCfg.ReSetPower = bReSetPower;
	m_sRestCfg.MaxReSetTime = wMaxReSetTime;
	m_sRestCfg.MinReSetLevel = wMinReSetLevel;
	m_sRestCfg.ReSetMoney = dwReSetMoney;
}
void sRestManager::LoadSprofitInfo()
{
	CMHFile file;
	if(!file.Init("./Resource/Server/RestSprofitInfo.bin","rb"))
	{
		g_Console.LOGEX(4,RGB(255,0,0),"转生奖励加载失败");
		return;
	}
	while(1)
	{
		if(file.IsEOF())
			break;
		m_sSprofit.m_Type = file.GetDword();
		m_TopSend.wHour = file.GetWord();
		m_TopSend.wMinute = file.GetWord();
		m_sSprofit.First = file.GetWord();
		m_sSprofit.second = file.GetWord();
		m_sSprofit.third = file.GetWord();
		m_sSprofit.fourth = file.GetWord();
		m_sSprofit.fifth = file.GetWord();
		m_sSprofit.sixth = file.GetWord();
		m_sSprofit.seventh = file.GetWord();
		m_sSprofit.eighth = file.GetWord();
		m_sSprofit.ninth = file.GetWord();
		m_sSprofit.tenth = file.GetWord();
		m_sSprofit.eleventh = file.GetWord();
		m_sSprofit.twelfth = file.GetWord();
		m_sSprofit.thirteenth = file.GetWord();
		m_sSprofit.fourteenth = file.GetWord();
		m_sSprofit.fifteenth = file.GetWord();
		m_sSprofit.sixteenth = file.GetWord();
		m_sSprofit.seventeenth = file.GetWord();
		m_sSprofit.eighteenth = file.GetWord();
		m_sSprofit.nineteenth = file.GetWord();
		m_sSprofit.twentieth = file.GetWord();
		break;
	}
	file.Release();
}

void sRestManager::AddRestTimes()
{
	stTIME tp,cp;
	cp = m_RestTime;
	tp.SetTime(0,0,m_wInterval,0,0,0);
	cp += tp;
	m_RestTime.SetTime(cp.value);
	m_sRestCfg.MaxReSetTime += 1;
	
	if(m_MapNum==17)
	{
		MSG_TIP msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_CLIENT_MSG_AGENT;
		msg.Flag = eRestOpenTip;
		msg.dwObjectID = m_RestTime.value;
		msg.KillTime = m_sRestCfg.MaxReSetTime;
		g_Network.SendAll2AgentServer((char*)&msg,sizeof(msg));
		g_pServerSystem->SetRestConfig(m_sRestCfg.MaxReSetTime,TRUE);
	}
	else
		g_pServerSystem->SetRestConfig(m_sRestCfg.MaxReSetTime);
}
void sRestManager::ShowConfigInformation()
{
	SYSTEMTIME RestTime,RestProfitTime;
	m_RestTime.GetTimeToSystemTime(RestTime);
	m_GiveProfitTime.GetTimeToSystemTime(RestProfitTime);
	g_Console.LOGEX(4,RGB(255,0,0),"第%d转开转时间:%d年%d月%d日%d时",m_sRestCfg.MaxReSetTime+1,
		RestTime.wYear,RestTime.wMonth,RestTime.wDay,RestTime.wHour);
	g_Console.LOGEX(4,RGB(255,0,0),"第%d转奖励发放时间:%d年%d月%d日%d时",m_sRestCfg.MaxReSetTime+1,
		RestProfitTime.wYear,RestProfitTime.wMonth,RestProfitTime.wDay,RestProfitTime.wHour);
}
void sRestManager::ReLoadRestTime()
{
	std::ifstream Infile;
	Infile.open("./ServerSet/RestTime.txt");
	stTIME RestTime;
	SYSTEMTIME time;
	Infile>>time.wYear;
	Infile>>time.wMonth;
	Infile>>time.wDay;
	Infile>>time.wHour;
	Infile>>time.wMinute;
	Infile>>time.wSecond;
	m_RestTime.SetTime(time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	RestTime.SetTime(0, 0, 0, m_TopSend.wHour, m_TopSend.wMinute,0);
	RestTime += m_RestTime;
	m_GiveProfitTime.SetTime(RestTime.value);
	Infile.close();
	m_RestTime.GetTimeToSystemTime(time);
	g_Console.LOGEX(4,RGB(0,255,0),"转生时间:%d年-%d月-%d日-%d时-%d分-%d秒",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	UpdateRestSetTime(m_RestTime.value,m_GiveProfitTime.value);
}
void sRestManager::BeingSendSprofit()
{
	stTIME tp,cp;
	cp = m_RestTime;
	tp.SetTime(0, 0, 0, m_TopSend.wHour, m_TopSend.wMinute, 0);
	cp +=tp;
	m_GiveProfitTime.SetTime(cp.value);
	if(m_MapNum==17)
	{
		SendSprofit(m_sSprofit);
		UpdateRestSetTime(m_RestTime.value,m_GiveProfitTime.value);	
	}

	MSG_TIP msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	msg.Flag = eRestProfitTip;
	msg.dwObjectID = m_GiveProfitTime.value;
	msg.KillTime = m_sRestCfg.MaxReSetTime;
	g_Network.SendAll2AgentServer((char*)&msg,sizeof(msg));
	g_Console.LOGEX(4,RGB(255,0,0),"转生时间已经更新");

	ShowConfigInformation();
}
void sRestManager::Process()
{
	if(!m_sRestCfg.ReSetPower) return;

	SYSTEMTIME time;
	GetLocalTime(&time);
	stTIME stTime;

	stTime.SetTime(time.wYear,
		time.wMonth,time.wDay,
		time.wHour,time.wMinute,
		time.wSecond);

	if(stTime>m_RestTime)
		AddRestTimes();

	if(stTime>m_GiveProfitTime)
		BeingSendSprofit();
}

void sRestManager::LoadFirstRestTime()
{
	CMHFile file;
	if(!file.Init("./Resource/Server/FirstRestTime.bin","rb"))
	{
		g_Console.LOGEX(4,RGB(255,0,0),"转生时间加载失败");
		return;
	}
	SYSTEMTIME time;
	while(1)
	{
		if(file.IsEOF())
			break;
		m_wInterval = file.GetWord();
		time.wYear = file.GetWord();
		time.wMonth = file.GetWord();
		time.wDay  = file.GetWord();
		time.wHour = file.GetWord();
		time.wMinute = file.GetWord();
		time.wSecond = file.GetWord();
		break;
	}
	file.Release();
	m_RestTime.SetTime(time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	stTIME time1,time2;
	time2 = m_RestTime;
	time1.SetTime(0, 0, 0, m_TopSend.wHour, m_TopSend.wMinute, 0);
	time2 += time1;
	m_GiveProfitTime.SetTime(time2.value);
}

void sRestManager::SetRestTime(DWORD val)
{
	m_RestTime.SetTime(val);
	stTIME tp,cp;
	cp = m_RestTime;
	tp.SetTime(0, 0, 0, m_TopSend.wHour, m_TopSend.wMinute, 0);
	cp +=tp;
	m_GiveProfitTime.SetTime(cp.value);
}
void sRestManager::LoadRestSetStaut()
{
	CMHFile file;
	if(!file.Init("./Resource/RestSetLevelPoint.bin","rb"))
	{
		g_Console.LOGEX(4,RGB(255,0,0),"转生属性配置加载失败");
		return;
	}
	WORD level = 0;
	while(1)
	{
		if(file.IsEOF())
			break;
		level = file.GetWord();
		if(level!=0)
		{
			RSETPOINT info;
			info.wBase = file.GetWord();
			info.wLevel = file.GetWord();
			m_RsetSetStaut[level] = info;
			level = 0;
		}
	}
	file.Release();
}
WORD sRestManager::GetRestSetStaut(WORD pLayerLevel)
{
	std::map<WORD,RSETPOINT>::iterator iter;
	iter = m_RsetSetStaut.find(pLayerLevel);
	if(iter == m_RsetSetStaut.end())
	{
		g_Console.LOGEX(4,RGB(255,0,0),"转生属性配置不存在等级%d",pLayerLevel);
		return 0;
	}
	else
	{
		return iter->second.wBase;
	}
}
WORD sRestManager::GetRestSetLevel(WORD pLayerLevel)
{
	std::map<WORD,RSETPOINT>::iterator iter;
	iter = m_RsetSetStaut.find(pLayerLevel);
	if(iter == m_RsetSetStaut.end())
	{
		g_Console.LOGEX(4,RGB(255,0,0),"转生属性配置不存在等级%d",pLayerLevel);
		return 0;
	}
	else
	{
		return iter->second.wLevel;
	}
}

void sRestManager::NetWorkMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_CHAR_CHANGELIFE_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if(!pPlayer) return;
			
			if(m_sRestCfg.ReSetPower==0)
			{//[转生功能关闭][By:十里坡剑神][QQ:112582793][2018/3/9]
				pPlayer->SetReSetN(0,2); 
				return;
			}
			if(pPlayer->GetReSet()>=m_sRestCfg.MaxReSetTime)
			{//[已经达到最大转数][By:十里坡剑神][QQ:112582793][2018/3/9]
				pPlayer->SetReSetN(m_sRestCfg.MaxReSetTime,3); 
				return ;
			}
			if(pPlayer->GetLevel()<(m_sRestCfg.MinReSetLevel+pPlayer->GetReSet()))
			{//[等级未达到转生等级][By:十里坡剑神][QQ:112582793][2018/3/9]
				pPlayer->SetReSetN(m_sRestCfg.MinReSetLevel+pPlayer->GetReSet(),4);
				return;
			}
			if(pPlayer->GetMoney()<(pPlayer->GetReSet()*m_sRestCfg.ReSetMoney))
			{//[游戏币不足][By:十里坡剑神][QQ:112582793][2018/3/9]
				pPlayer->SetReSetN((pPlayer->GetReSet()*m_sRestCfg.ReSetMoney),5); 
				return;
			}
			WORD PlayerLevel = pPlayer->GetLevel();
			WORD LastBasicPoint = pPlayer->GetBasicPoint();

			WORD RestBasicPoint = GetRestSetStaut(PlayerLevel);
			WORD LevelPoint = GetRestSetLevel(PlayerLevel);

			LastBasicPoint +=RestBasicPoint;

			pPlayer->SetBasicPoint(LastBasicPoint);

			pPlayer->SetPlayerLevelUpPoint(LevelPoint*(pPlayer->GetReSet()+1));

			pPlayer->SetCheRyuk(LastBasicPoint);
			pPlayer->SetSimMek(12);
			pPlayer->SetGenGol(12);
			pPlayer->SetMinChub(12);
			pPlayer->SetPlayerExpPoint(0);				
			pPlayer->SetMoney((pPlayer->GetReSet()*m_sRestCfg.ReSetMoney),MONEY_SUBTRACTION);			
			pPlayer->SetLevel(1);
			pPlayer->SetMaxLevel(1);
			pPlayer->SetReSet(pPlayer->GetReSet()+1,1);
			CharacterTotalInfoUpdate(pPlayer);
		}
		break;
	case MP_CHAR_CHANGELIFE_INFO_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if(!pPlayer) return;

			MSGRESTINFO msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_CHANGELIFE_INFO_ACK;
			msg.wMaxRestSet = m_sRestCfg.MaxReSetTime;
			msg.wMinRestLevel = m_sRestCfg.MinReSetLevel;
			msg.dwRestOpenTime = m_RestTime.value;
			msg.dwRestProfitTime = m_GiveProfitTime.value;
			msg.wSiegeWarState = SIEGEWARMGR->GetSWState();
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		break;
	}
}