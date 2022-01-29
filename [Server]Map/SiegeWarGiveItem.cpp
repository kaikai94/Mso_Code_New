#include "stdafx.h"
#include "SiegeWarGiveItem.h"
#include "MHFile.h"
#include "Player.h"
#include "ItemManager.h"
#include "MapDBMsgParser.h"
SiegeWarGiveItem::SiegeWarGiveItem(void)
{
}

SiegeWarGiveItem::~SiegeWarGiveItem(void)
{
	m_GiveItemInfo.clear();
}
void SiegeWarGiveItem::LoadGiveItemInfo()
{
	char buf[512];
	CMHFile file;
	if (!file.Init("./Resource/Server/SiegeWarGiveItemInfo.bin","rb"))
		return;
	file.GetLine(buf,512);
	while(1)
	{
		if(file.IsEOF())
			break;
		SIEGEWARGIVEITEMINFO info;
		
		info.wPlayerType = file.GetWord();
		info.wItemType   = file.GetWord();
		info.wItemCount  = file.GetWord();
		info.wItemIdx    = file.GetWord();
		std::pair<WORD, SIEGEWARGIVEITEMINFO> p1(info.wPlayerType, info);
		m_GiveItemInfo.insert(p1);
	}
	file.Release();
}
void SiegeWarGiveItem::SendItem(CPlayer* pPlayer, const SIEGEWARGIVEITEMINFO & info)
{
	switch (info.wItemType)
	{
	case eGiveMoney:
		{
			pPlayer->SetMoney(info.wItemIdx, MONEY_ADDITION, 64);
		}
		break;
	case eGiveFame:
		{
			pPlayer->SetFame(info.wItemIdx);
		}
		break;
	case eGiveNomorItem:
		{
			ITEMMGR->SiegeWarObtainItem(pPlayer,
				info.wItemIdx, info.wItemCount, 0, 1);
		}
		break;
	case eGiveEtcItem:
		{
			ITEMMGR->SiegeWarObtainItem(pPlayer,
				info.wItemIdx, info.wItemCount, 1, 1);
		}
		break;
	case eGiveGoldMoney:
		{
			pPlayer->SetGoldMoney(info.wItemIdx, eGetGoldMoney);
			ItemShopUseLog(eLog_SiegeWarGet,pPlayer->GetID(),pPlayer->GetObjectName(),0,"¹¥³Ç½±Àø",0,
				0,0,0,
				pPlayer->GetGoldMoney(), info.wItemIdx, pPlayer->GetGoldMoney() + info.wItemIdx,
				0,0,0);
			//LogGoldMoney(eLog_SiegeWarGet, 65533, 0, pPlayer->GetID(), pPlayer->GetGoldMoney() - info.wItemIdx,
			//	info.wItemIdx, 0,0, 0);
		}
		break;
	case eGiveMallMoney:
		{
			pPlayer->SetMallMoney(info.wItemIdx, 2);
		}
		break;
	default:
		break;
	}
}
void SiegeWarGiveItem::SendItem(CPlayer* pPlayer,eSIEGEWARPLAYER type)
{
	WORD wCount = m_GiveItemInfo.count(type);
	if (2 <= wCount)
	{
		std::pair<Gift_multimap, Gift_multimap> InfoPair = m_GiveItemInfo.equal_range(type);
		Gift_multimap iter = InfoPair.first;
		while (iter != InfoPair.second)
		{
			SendItem(pPlayer,iter->second);
			iter++;
		}
	}
	else
	{
		Gift_multimap iter = m_GiveItemInfo.find(type);
		if (iter != m_GiveItemInfo.end())
			SendItem(pPlayer,iter->second);
	}
}