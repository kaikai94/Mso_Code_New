#include "stdafx.h"
#include "SingedManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "ItemManager.h"
#include "Network.h"
#include "UserTable.h"
#include "Console.h"
#include "KillTopManager.h"
SingedManager::SingedManager(void)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	UpdateTime(time);
}

SingedManager::~SingedManager(void)
{
	m_SingedInfo.clear();
	m_SingedGiftInfo.clear();
}
void SingedManager::AddSinged(const DWORD& UserId,const SINGEDINFO& info)
{
	SINGEDINFO pInfo;
	memcpy(&pInfo,&info,sizeof(SINGEDINFO));
	m_SingedInfo[UserId]=pInfo;
}
void SingedManager::RemoveSinged(const DWORD& UserId)
{
	iter = m_SingedInfo.find(UserId);
	if (iter!=m_SingedInfo.end())
		m_SingedInfo.erase(iter);
	iter = m_SingedInfo.end();
}
BOOL SingedManager::CanGetCompeletGift(const DWORD& UserID)
{
	iter = m_SingedInfo.find(UserID);
	if (iter!=m_SingedInfo.end())
	{
		return iter->second.Complete;
	}
}
BOOL SingedManager::CanGetGift(const DWORD& UserID)
{
	iter = m_SingedInfo.find(UserID);
	if (iter!=m_SingedInfo.end())
	{
		switch(m_day)
		{
		case eMonday:
			return iter->second.Mon;
		case eTuesday:
			return iter->second.Tues;
		case eWednesday:
			return iter->second.Wed;
		case eThursday:
			return iter->second.Thur;
		case eFriday:
			return iter->second.Fri;
		case eSaturday:
			return iter->second.Sat;
		case eSunday:
			return iter->second.Sun;
		case eCompelete:
			return iter->second.Complete;
		}
	}
	return FALSE;
}
void SingedManager::SetSinged(const DWORD& UserID)
{
	iter = m_SingedInfo.find(UserID);
	if (iter!=m_SingedInfo.end())
	{
		switch(m_day)
		{
		case eMonday:
			m_SingedInfo[UserID].Mon=FALSE;
			break;
		case eTuesday:
			m_SingedInfo[UserID].Tues=FALSE;
			break;
		case eWednesday:
			m_SingedInfo[UserID].Wed=FALSE;
			break;
		case eThursday:
			m_SingedInfo[UserID].Thur=FALSE;
			break;
		case eFriday:
			m_SingedInfo[UserID].Fri=FALSE;
			break;
		case eSaturday:
			m_SingedInfo[UserID].Sat=FALSE;
			break;
		case eSunday:
			m_SingedInfo[UserID].Sun=FALSE;
			break;
		case eCompelete:
			m_SingedInfo[UserID].Complete=FALSE;
			break;
		}
	}
	UpdateSingedInfo(UserID,m_dayChar.c_str());
}
void SingedManager::SetSinged(const DWORD& UserID,eSINGED Day)
{
	iter = m_SingedInfo.find(UserID);
	if (iter!=m_SingedInfo.end())
	{
		switch(Day)
		{
		case eMonday:
			m_SingedInfo[UserID].Mon=FALSE;
			break;
		case eTuesday:
			m_SingedInfo[UserID].Tues=FALSE;
			break;
		case eWednesday:
			m_SingedInfo[UserID].Wed=FALSE;
			break;
		case eThursday:
			m_SingedInfo[UserID].Thur=FALSE;
			break;
		case eFriday:
			m_SingedInfo[UserID].Fri=FALSE;
			break;
		case eSaturday:
			m_SingedInfo[UserID].Sat=FALSE;
			break;
		case eSunday:
			m_SingedInfo[UserID].Sun=FALSE;
			break;
		case eCompelete:
			m_SingedInfo[UserID].Complete=FALSE;
			break;
		}
	}
	UpdateSingedInfo(UserID,m_NewTime.GetDay(Day).c_str());
}
BOOL SingedManager::CanGetGift(const DWORD& UserID,eSINGED Day)
{
	iter = m_SingedInfo.find(UserID);
	if (iter!=m_SingedInfo.end())
	{
		switch(Day)
		{
		case eMonday:
			return iter->second.Mon;
		case eTuesday:
			return iter->second.Tues;
		case eWednesday:
			return iter->second.Wed;
		case eThursday:
			return iter->second.Thur;
		case eFriday:
			return iter->second.Fri;
		case eSaturday:
			return iter->second.Sat;
		case eSunday:
			return iter->second.Sun;
		case eCompelete:
			return iter->second.Complete;
		}
	}
	return FALSE;
}
eSINGED SingedManager::GiftGiving(CPlayer* pPlayer,eSINGED Day)
{
	if (!CanGetGift(pPlayer->GetUserID(),Day))
		return eGived;
	Giftiter = m_SingedGiftInfo.find(m_NewTime.GetDay(Day));
	if (Giftiter!=m_SingedGiftInfo.end())
	{
		if(ITEMMGR->SingedObtainItem(pPlayer,Giftiter->second.wItemIdx,Giftiter->second.wCount,0,1)==1)
			return eNoPice;
		SetSinged(pPlayer->GetUserID(),Day);
		return eOK;
	}
	return eErr;
}
void SingedManager::RestSingedInfo()
{
	for(iter = m_SingedInfo.begin();iter!=m_SingedInfo.end();iter++)
	{
		iter->second.Complete = FALSE;
		iter->second.Fri  = TRUE;
		iter->second.Mon  = TRUE;
		iter->second.Sat  = TRUE;
		iter->second.Sun  = TRUE;
		iter->second.Thur = TRUE;
		iter->second.Tues = TRUE;
		iter->second.Wed  = TRUE;
	}
}
void SingedManager::UpdateTime(SYSTEMTIME& time)
{
	m_NewTime;
	m_day=m_NewTime.GetDayToInt(time.wYear,time.wMonth,time.wDay);
	m_dayChar=m_NewTime.GetDayToChar(time.wYear,time.wMonth,time.wDay);
	if (m_day==eMonday&&time.wHour==0&&time.wMinute==0&&
		time.wMinute==0&&time.wSecond==0)
	{
		RestSingedInfo();//[更新签到信息][By:十里坡剑神][QQ:112582793][2018/4/23]
		KILLTOPMGR->RestKillTopListInfo();//[更新击杀排行信息][By:十里坡剑神][QQ:112582793][2018/4/23]
		MSGBASE msg;
		msg.Category = MP_CLIENT;
		msg.Protocol = MP_SERVER_SINGED_REST_SYN;
		g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
	}
}

void SingedManager::LoadGiftInfo()
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
	Giftiter = m_SingedGiftInfo.find("补签元宝");
	if (Giftiter!=m_SingedGiftInfo.end())
		NeedGoldMoney=Giftiter->second.wItemIdx;
	else
		g_Console.LOGEX(4,RGB(255,0,0),"补签元宝读取错误");
	
	file.Release();
}
eSINGED SingedManager::GiftGivingCompelet(CPlayer* pPlayer)
{
	if (!CanGetCompeletGift(pPlayer->GetUserID()))
		return eGived;
	Giftiter = m_SingedGiftInfo.find("满签");
	if (Giftiter!=m_SingedGiftInfo.end())
	{
		if(ITEMMGR->SingedObtainItem(pPlayer,Giftiter->second.wItemIdx,Giftiter->second.wCount,0,1)==1)
			return eNoPice;//[背包没有位置][By:十里坡剑神][QQ:112582793][2018/1/4]
		SetSinged(pPlayer->GetUserID(),eCompelete);
		return eOK;
	}
	return eErr;
}
eSINGED SingedManager::GiftGiving(CPlayer* pPlayer)
{
	if (!CanGetGift(pPlayer->GetUserID()))
		return eGived;
	Giftiter = m_SingedGiftInfo.find(m_dayChar);
	if (Giftiter!=m_SingedGiftInfo.end())
	{
		if(ITEMMGR->SingedObtainItem(pPlayer,Giftiter->second.wItemIdx,Giftiter->second.wCount,0,1)==1)
			return eNoPice;//[背包没有位置][By:十里坡剑神][QQ:112582793][2018/1/4]
		SetSinged(pPlayer->GetUserID());
		return eOK;
	}
	return eErr;
}
void SingedManager::SetSingedMsgInfo(CPlayer* pPlayer,MSG_SINGEDINFO &info)
{
	iter = m_SingedInfo.find(pPlayer->GetUserID());
	if(iter!=m_SingedInfo.end())
	{
		memcpy(&info.sInfo,&iter->second,sizeof(SINGEDINFO));
	}
}
void SingedManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
		case MP_SINGED_GETGIFT_SYN:
			{
				MSG_SINGED *pmsg = (MSG_SINGED*)pMsg;
				CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
				if(NULL==pPlayer) return;

				MSG_SINGEDINFO msg;
				msg.Category = MP_SINGED;
				msg.Protocol = MP_SINGED_GETGIFT_ACK;
				eSINGED rt;
				if (pmsg->eType<m_day)
				{//[日期小于当前补签][By:十里坡剑神][QQ:112582793][2018/1/4]
					if (pPlayer->GetGoldMoney()<NeedGoldMoney) //[签到元宝检查][BY:十里坡剑神][QQ:112582793][2019-9-16][20:44:46]
						msg.eType=eNotMoney;
					else
					{
						rt=GiftGiving(pPlayer,pmsg->eType);
						if (rt==eOK)
						{
							SetSingedMsgInfo(pPlayer,msg);
							pPlayer->SetGoldMoney(NeedGoldMoney,eSinged);//[补签成功扣除元宝][By:十里坡剑神][QQ:112582793][2018/1/4]
							ItemShopUseLog(eLog_Singed,pPlayer->GetID(),pPlayer->GetObjectName(),0,"补签消耗",0,
								0,0,0,
								pPlayer->GetGoldMoney(), NeedGoldMoney,pPlayer->GetGoldMoney()-NeedGoldMoney,
								0,0,0);
							//LogGoldMoney(eLog_Singed, 65533, pPlayer->GetGoldMoney() + NeedGoldMoney, pPlayer->GetID(), pPlayer->GetGoldMoney(), NeedGoldMoney, 0,0, 0);
						}
					}
				}
				else if (pmsg->eType==m_day)
					rt = GiftGiving(pPlayer);//[正常签到][By:十里坡剑神][QQ:112582793][2018/1/4]
				else if(pmsg->eType==eCompelete)
					rt = GiftGivingCompelet(pPlayer);//[满签领取][By:十里坡剑神][QQ:112582793][2018/1/8]
				else
					rt = eCanGetDay; //[签到日期大于当前不允许签到][By:十里坡剑神][QQ:112582793][2018/1/4]
				if (rt=eOK)
					SetSingedMsgInfo(pPlayer,msg);
				msg.eType = rt;
				pPlayer->SendMsg(&msg,sizeof(msg));
			}
			break;
	}
}