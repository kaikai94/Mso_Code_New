#include "stdafx.h"
#include "KillTopManager.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "ItemManager.h"
#include "Network.h"
#include "UserTable.h"
#include "Console.h"

KillTopManager::KillTopManager(void)
{
}

KillTopManager::~KillTopManager(void)
{
	m_KillTopList.clear();
	m_TopListGiftInfo.clear();
}

//[添加排行信息到map][By:十里坡剑神][QQ:112582793][2018/4/22]
void KillTopManager::AddKillTopListInfo(const DWORD& UserId,const SINGEDINFO& info)
{
	SINGEDINFO pInfo;
	memcpy(&pInfo,&info,sizeof(SINGEDINFO));
	m_KillTopList[UserId]=pInfo;
}

//[移除排行信息][By:十里坡剑神][QQ:112582793][2018/4/22]
void KillTopManager::RemoveKillTopListInfo(const DWORD& UserId)
{
	std::map<DWORD,SINGEDINFO>::iterator iter;
	iter = m_KillTopList.find(UserId);
	if(iter!=m_KillTopList.end())
		m_KillTopList.erase(iter);
}
//[判断玩家是否可领取礼包][By:十里坡剑神][QQ:112582793][2018/4/22]
BOOL KillTopManager::CanGetGift(const DWORD& UserID,eSINGED Day)
{
	std::map<DWORD,SINGEDINFO>::iterator iter;
	iter = m_KillTopList.find(UserID);
	if (iter!=m_KillTopList.end())
	{
		switch(Day)
		{
		case eMonday:
			return (iter->second.Mon==eGift_Get_OK);
		case eTuesday:
			return (iter->second.Tues==eGift_Get_OK);
		case eWednesday:
			return (iter->second.Wed==eGift_Get_OK);
		case eThursday:
			return (iter->second.Thur==eGift_Get_OK);
		case eFriday:
			return (iter->second.Fri==eGift_Get_OK);
		case eSaturday:
			return (iter->second.Sat==eGift_Get_OK);
		case eSunday:
			{
				if(iter->second.Mon==eGift_Get_Geted&&
				   iter->second.Tues==eGift_Get_Geted&&
				   iter->second.Wed==eGift_Get_Geted&&
				   iter->second.Thur==eGift_Get_Geted&&
				   iter->second.Fri==eGift_Get_Geted&&
				   iter->second.Sat==eGift_Get_Geted&&
				   iter->second.Sun==eGift_Get_OK)
					return TRUE;
				else
					return FALSE;
			}
		}
	}
	return FALSE;
}

//[领取后更新数据库][By:十里坡剑神][QQ:112582793][2018/4/22]
void KillTopManager::SetTopListInfo(const DWORD& UserID,eSINGED Day)
{
	std::map<DWORD,SINGEDINFO>::iterator iter;
	iter = m_KillTopList.find(UserID);
	if(iter!=m_KillTopList.end())
	{
		switch(Day)
		{
		case eMonday:
			iter->second.Mon=eGift_Get_Geted;
			break;
		case eTuesday:
			iter->second.Tues=eGift_Get_Geted;
			break;
		case eWednesday:
			iter->second.Wed=eGift_Get_Geted;
			break;
		case eThursday:
			iter->second.Thur=eGift_Get_Geted;
			break;
		case eFriday:
			iter->second.Fri=eGift_Get_Geted;
			break;
		case eSaturday:
			iter->second.Sat=eGift_Get_Geted;
			break;
		case eSunday:
			iter->second.Sun=eGift_Get_Geted;
			break;
		}
	}
	UpDateKillTopInfo(UserID,m_NewTime.GetDay(Day).c_str());
}

//[加载奖励物品信息][By:十里坡剑神][QQ:112582793][2018/4/23]
void KillTopManager::LoadGiftInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/KillTopListGiftInfo.bin","rb"))
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
		m_TopListGiftInfo[temp]=info;
	}
	file.Release();
}

//[重置击杀排行信息][By:十里坡剑神][QQ:112582793][2018/4/23]
void KillTopManager::RestKillTopListInfo()
{
	std::map<DWORD,SINGEDINFO>::iterator iter;
	iter = m_KillTopList.begin();
	for(;iter!=m_KillTopList.end();iter++)
	{
		iter->second.Complete = eGift_Get_No;
		iter->second.Fri  = eGift_Get_No;
		iter->second.Mon  = eGift_Get_No;
		iter->second.Sat  = eGift_Get_No;
		iter->second.Sun  = eGift_Get_No;
		iter->second.Thur = eGift_Get_No;
		iter->second.Tues = eGift_Get_No;
		iter->second.Wed  = eGift_Get_No;
	}
}
//[奖励发放][By:十里坡剑神][QQ:112582793][2018/4/23]
eSINGED KillTopManager::GiftGiving(CPlayer* pPlayer,eSINGED Day)
{
	if (!CanGetGift(pPlayer->GetID(),Day))
		return eGived;
	std::map<std::string,SINGEDGIFTINFO>::iterator iter;
	iter = m_TopListGiftInfo.find(m_NewTime.GetDay(Day));
	if (iter!=m_TopListGiftInfo.end())
	{
		if(ITEMMGR->SingedObtainItem(pPlayer,iter->second.wItemIdx,iter->second.wCount,0,1)==1)
			return eNoPice;
		SetTopListInfo(pPlayer->GetID(),Day);
		return eOK;
	}
	return eErr;
}
//[网络消息处理][By:十里坡剑神][QQ:112582793][2018/4/23]
void KillTopManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{//[屏蔽击杀排行礼包][By:十里坡剑神][QQ:112582793][2018/5/3 12:38]
	//MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	//switch(pTempMsg->Protocol)
	//{
	//case MP_KILLTOPLIST_SYN://[礼包领取消息][By:十里坡剑神][QQ:112582793][2018/4/23]
	//	{
	//		MSG_SINGED *pmsg = (MSG_SINGED*)pMsg;
	//		CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
	//		if(NULL==pPlayer) return;
	//		MSG_SINGEDINFO msg;
	//		msg.Category = MP_KILLTOPLIST;
	//		msg.Protocol = MP_KILLTOPLIST_ACK;

	//		eSINGED rt;
	//		rt=GiftGiving(pPlayer,pmsg->eType);
	//		if (rt==eOK)
	//		{
	//			std::map<DWORD,SINGEDINFO>::iterator iter;
	//			iter = m_KillTopList.find(pPlayer->GetID());
	//			if(iter!=m_KillTopList.end())
	//				memcpy(&msg.sInfo,&iter->second,sizeof(msg.sInfo));
	//		}
	//		else
	//		{
	//			msg.Protocol = MP_KILLTOPLIST_NACK;
	//			msg.eType = rt;
	//		}
	//		pPlayer->SendMsg(&msg,sizeof(msg));
	//	}
	//	break;
	//}
}
