#include "stdafx.h"
#include "RechargeRecordManager.h"
#include "MHFile.h"
#include "Player.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
#include "Network.h"
#include "UserTable.h"
RechargeRecordManager::RechargeRecordManager(void)
{

}

RechargeRecordManager::~RechargeRecordManager(void)
{
	m_CharacterRechargeInfo.clear();
	m_RechargeGiftInfo.clear();
}
void RechargeRecordManager::UpdateRecharge()
{
	if (m_CharacterRechargeInfo.size() == 0 ) return;
	std::map<WORD, sCHARRECHARGEINFO>::iterator iter;
	iter = m_CharacterRechargeInfo.begin();
	for (;;)
	{
		if (iter == m_CharacterRechargeInfo.end())
			break;
		else
		{
			CPlayer * pPlayer = NULL;
			pPlayer = (CPlayer *)g_pUserTable->FindUser(iter->first);
			if (pPlayer==NULL)
				continue;
			else if (iter->second.UsedMoney!=0)
			{
				iter->second.PayMoney = iter->second.PayMoney - iter->second.UsedMoney;
				iter->second.UsedMoney = 0;
				UpdateRechargeInfo(pPlayer->GetID(), iter->second.PayMoney, 0);
				SendInfoToClient(pPlayer);
				if (iter->second.PayMoney == 0)
				{
					iter = m_CharacterRechargeInfo.erase(iter);
					if (iter == m_CharacterRechargeInfo.end())
						break;
				}
					
			}
		}
		iter++;
	}
}
void RechargeRecordManager::LoadRechargeInfo()
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
	std::map<WORD, WORD>::iterator iter;
	iter = m_RechargeGiftInfo.begin();
	for (; iter != m_RechargeGiftInfo.end(); ++iter)
	{
		m_GiftMoneyInfo.push_back(iter->first);
		//m_GiftMoneyInfo[i] = iter->first;
	}
	file.Release();
}

void RechargeRecordManager::SetCharcterRechargeInfo(DWORD Character_idx,DWORD PayMoney,DWORD UsedMoney,DWORD PayMoneyCount)
{
	CPlayer * pPlayer = NULL;
	pPlayer = (CPlayer *)g_pUserTable->FindUser(Character_idx);
	if (pPlayer == NULL) return;

	sCHARRECHARGEINFO info;
	info.PayMoney = PayMoney;
	info.UsedMoney = UsedMoney;
	info.PayMoneyCount = PayMoneyCount;

	if (PayMoney <m_GiftMoneyInfo[0])
	{
		MSG_CHARRECHARGEINFO msg;
		msg.Category = MP_RECHARGE;
		msg.Protocol = MP_RECHARGE_ACK;
		memcpy(&msg.sInfo, &info, sizeof(sCHARRECHARGEINFO));
		pPlayer->SendMsg(&msg, sizeof(msg));
		return;
	}

	m_CharacterRechargeInfo[Character_idx] = info;
	SendInfoToClient(pPlayer);
}
void RechargeRecordManager::RemoveRechargeInfo(DWORD Character_idx)
{
	std::map<WORD,sCHARRECHARGEINFO>::iterator citer;
	citer = m_CharacterRechargeInfo.find(Character_idx);
	if(citer!=m_CharacterRechargeInfo.end())
		m_CharacterRechargeInfo.erase(citer);
}
WORD RechargeRecordManager::GetGiftCount(WORD Type)
{
	std::vector<WORD>::iterator iter;
	iter = m_GiftMoneyInfo.begin();
	WORD wCount = 0;
	for (; iter != m_GiftMoneyInfo.end(); ++iter)
	{
		if (Type >= *iter)
			wCount++;
		else
			break;
		
	}
	return wCount;
}
void RechargeRecordManager::GetRechargeGift(CPlayer* pPlayer,WORD type)
{
	std::map<WORD,sCHARRECHARGEINFO>::iterator citer;
	std::map<WORD, WORD>::iterator giter;
	citer = m_CharacterRechargeInfo.find(pPlayer->GetID());
	//[充值信息获取失败][By:十里坡剑神][QQ:112582793][2018/1/31]
	if(citer==m_CharacterRechargeInfo.end()) return;
	//[已经领取][By:十里坡剑神][QQ:112582793][2018/1/31]
	
	if(citer->second.UsedMoney>=type||citer->second.PayMoney<type) return;
	int count = GetGiftCount(type);
	giter = m_RechargeGiftInfo.begin();
	
	if(citer->second.UsedMoney!=0)
	{//[领取过计算领取的礼包][By:十里坡剑神][QQ:112582793][2018/4/26]
		int count2 = GetGiftCount(citer->second.UsedMoney);
		for(int i = 0;i<count2;i++)
		{
			if (giter == m_RechargeGiftInfo.end())	break;
			giter++;
		}
		count -=count2;
	}
	for(int i = 0;i<count;i++)
	{
		if (giter == m_RechargeGiftInfo.end())	break;
		if(ITEMMGR->RechargeObtainItem(pPlayer,giter->second,1,0,2)==1)
		{
			type = m_GiftMoneyInfo[i];
			break;
		}	
		giter++;
	}
	m_CharacterRechargeInfo[citer->first].UsedMoney = type;
	UpdateRechargeInfo(pPlayer->GetID(), m_CharacterRechargeInfo[citer->first].PayMoney,m_CharacterRechargeInfo[citer->first].UsedMoney);
	SendInfoToClient(pPlayer);
}
void RechargeRecordManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
		case MP_RECHARGE_SYN:
			{
				MSG_RECHARGE * pmsg = (MSG_RECHARGE *)pMsg;
				CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
				if(NULL==pPlayer) return;
				GetRechargeGift(pPlayer,pmsg->wType);
			}
			break;
	}
}

void RechargeRecordManager::SendInfoToClient(CPlayer* pPlayer)
{
	MSG_CHARRECHARGEINFO msg;
	msg.Category = MP_RECHARGE;
	msg.Protocol = MP_RECHARGE_ACK;
	std::map<WORD, sCHARRECHARGEINFO>::iterator citer;
	citer = m_CharacterRechargeInfo.find(pPlayer->GetID());
	if(citer!=m_CharacterRechargeInfo.end())
		memcpy(&msg.sInfo,&citer->second,sizeof(sCHARRECHARGEINFO));
	else
		memset(&msg.sInfo,0,sizeof(msg.sInfo));
	pPlayer->SendMsg(&msg,sizeof(msg));
}