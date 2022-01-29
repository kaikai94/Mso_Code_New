



// ItemDrop.cpp: implementation of the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemDrop.h"
#include "Player.h"
#include "time.h"
#include "ItemManager.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "PartyManager.h"
#include "Distribute_Random.h"
#include "Party.h"
#include "ChannelSystem.h"
#include "LimitItemDropManager.h"
//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CItemDrop::CItemDrop()
{
	m_MonsterItemDropTable.Initialize(500);
}

CItemDrop::~CItemDrop()
{
	ClearMonsterDropItemList();
}

void CItemDrop::Init()
{
	srand((unsigned)time(NULL));
	LoadMonsterDropItemList();
}
//[怪物掉落编号选择计算][By:十里坡剑神][QQ:112582793][2018/1/10]
int CItemDrop::DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer)
{
	DWORD CurTotalRate = 0;
	DWORD DropRate[eDROPITEMKIND_MAX];

	int i=0;
	for(i=0; i<eDROPITEMKIND_MAX; i++)
	{
		DropRate[i] = pMonsterExList->CurDropRate[i];
		
		if(i == eMONEY)
		{
			//[获取掉落游戏币的概率][By:十里坡剑神][QQ:112582793][2017/12/7]
			DropRate[eMONEY] = (DWORD)(DropRate[eMONEY]*gEventRate[eEvent_MoneyRate]);
		}
		else if(i >= eITEM1 && i <= eITEM3)
		{
			//[掉落物品概率，对应三个物品规则][By:十里坡剑神][QQ:112582793][2017/12/7]
			DropRate[i] = (DWORD)(DropRate[i]*gEventRate[eEvent_ItemRate]);

			if( pPlayer->GetPartyIdx() )
			{
				CParty* pParty = PARTYMGR->GetParty( pPlayer->GetPartyIdx() );
				if( pParty )
				{//[组队物品掉落概率增加附加][By:十里坡剑神][QQ:112582793][2017/12/7]
					int count = pParty->GetMemberCountofMap( pPlayer->GetID() );
					if( count && gPartyEvent[ePartyEvent_ItemRate].Rate[count-1] )
						DropRate[i] = (DWORD)(DropRate[i]*gPartyEvent[ePartyEvent_ItemRate].Rate[count-1]);
				}
			}
		}
		//[此次累加几个掉落规则概率][By:十里坡剑神][QQ:112582793][2017/12/7]
		CurTotalRate += DropRate[i];	
	}

	//[如果使用增加掉落的物品，此处进行附加][By:十里坡剑神][QQ:112582793][2017/12/7]
	if( pPlayer->GetShopItemStats()->AddItemDrop )
	{
		for(i=eITEM1; i<=eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] *= (DWORD)(1+(pPlayer->GetShopItemStats()->AddItemDrop*0.01f) + 0.001f);
			CurTotalRate += DropRate[i];
		}
	}
	//[频道掉落加成附加][By:十里坡剑神][QQ:112582793][2017/12/7]
	float fDropRate = CHANNELSYSTEM->GetDropRate( pPlayer->GetChannelID());
	if( fDropRate != 1.0f )

	{
		for(i=eITEM1; i<= eITEM3; ++i)
		{
			CurTotalRate -= DropRate[i];
			DropRate[i] = DWORD(fDropRate * DropRate[i]);
			CurTotalRate += DropRate[i];
		}
	}

	if( CurTotalRate > MAX_DROP_ITEM_PERCENT )
	{
		int dwTemp = DropRate[eMONEY]+DropRate[eITEM1]+DropRate[eITEM2]+DropRate[eITEM3];
		//[掉落的规则概率累加，如果小于掉落比例(10000)，则计算不掉落情况][By:十里坡剑神][QQ:112582793][2017/12/7]
		if( dwTemp < MAX_DROP_ITEM_PERCENT )
		{
			DropRate[eNOITEM] = MAX_DROP_ITEM_PERCENT - dwTemp;
			CurTotalRate = MAX_DROP_ITEM_PERCENT;
		}
		else
		{//[否则，掉落物品][By:十里坡剑神][QQ:112582793][2017/12/7]
			DropRate[eNOITEM] = 0;
			CurTotalRate = dwTemp;
		}
	}
	//[计算后的掉落概率为0则输出错误日志][By:十里坡剑神][QQ:112582793][2017/12/7]
	if( CurTotalRate == 0 )
	{
		ASSERT(0);
		return -1;
	}
	//[掉落的比例 = 1-CurTotalRate之间的一个随机数][By:十里坡剑神][QQ:112582793][2017/12/7]
	DWORD RandRate = random(1, CurTotalRate);
	DWORD CurMinRate=0,CurMaxRate=0;

	for( i=0; i<eDROPITEMKIND_MAX; i++)
	{//[将随机生成的概率与相连两个掉落比例对比，如果随即掉率大于前一个掉率，且小于][By:十里坡剑神][QQ:112582793][2017/12/7]
	 //[等于后一个掉落比例，则放回该编号，作为掉落规则][By:十里坡剑神][QQ:112582793][2017/12/7]
	 //[如游戏币掉落率大于三个物品规则，则掉落游戏币的几率大。][By:十里坡剑神][QQ:112582793][2017/12/7]
	 //[三个物品规则中哪个掉落比例小，则掉落几率大][By:十里坡剑神][QQ:112582793][2017/12/7]
		CurMinRate = CurMaxRate;
		CurMaxRate += DropRate[i];

		if(RandRate > CurMinRate && RandRate <= CurMaxRate)
		{
			return i;
		}
	}
	ASSERT(0);

	char buf[128];
	wsprintf( buf, "MonsterIdx:%d, CurTotalRate:%d, RandRate:%d, CurMinRate:%d, CurMaxRate:%d,",
		pMonsterExList->MonsterKind, CurTotalRate, RandRate, CurMinRate, CurMaxRate );
	ASSERTMSG(0, buf);

	return -1;

}
//[怪物物品掉落计算][By:十里坡剑神][QQ:112582793][2018/1/10]
MONEYTYPE CItemDrop::MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind)
{
	MONEYTYPE TotalMoney = 0; 

	for( int i = 0 ; i < pMonInfo->MaxDropItemNum ; ++i )
	{
		int eDropItemKind = DropItemKind(pMonInfo, pPlayer);

		switch(eDropItemKind)
		{
		case eNOITEM:
			{

			}
			break;
		case eMONEY:
			{
				if( g_pServerSystem->GetMap()->IsMapKind(eTitanMap) )
				{
					MONSTERDROP_TIMAP* pMonsterDropInfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( pMonInfo->Level );
					if( pMonsterDropInfo )
						TotalMoney = random( pMonsterDropInfo->stDropInfo[0].wMoneyMin, pMonsterDropInfo->stDropInfo[0].wMoneyMax );
				}
				else
					TotalMoney += MonsterDropMoney(pMonInfo);
			}
			break;
		case eITEM1:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum1, pPlayer);
			}
			break;
		case eITEM2:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum2, pPlayer);
			}
			break;
		case eITEM3:
			{
				MonsterDropItem(MonsterKind, pMonInfo->ItemListNum3, pPlayer);
			}
			break;
		default:
			{
				ASSERT(0);
			}
		}
	}

	return TotalMoney;
}

MONEYTYPE CItemDrop::MonsterDropMoney(MONSTEREX_LIST * pMonsterExList)
{
	MONEYTYPE MinMoney = pMonsterExList->MinDropMoney;
	MONEYTYPE MaxMoney = pMonsterExList->MaxDropMoney;

	MONEYTYPE ResultMoney = 0;

	if( MaxMoney > MinMoney )
	{
		ResultMoney = random( MinMoney, MaxMoney );
	}
	else
	{
		ResultMoney = MinMoney;
	}
	ResultMoney = (MONEYTYPE)(ResultMoney*gEventRate[eEvent_GetMoney]);
	return ResultMoney;
}
//[怪物物品掉落计算][By:十里坡剑神][QQ:112582793][2018/1/10]
void CItemDrop::MonsterDropItem(WORD MonsterKind, WORD wDropItemIdx, CPlayer* pPlayer)
{
	MONSTER_DROP_ITEM * pDropItem = GetMonsterItemDrop(wDropItemIdx);
	ASSERT(pDropItem);
	if(pDropItem == NULL)
		return;
	
	if( pDropItem->dwCurTotalRate == 0 )
	{
		if(ReloadMonsterDropItem(pDropItem) == FALSE)
			return;
	}

	DWORD RandRate = random( 1, pDropItem->dwCurTotalRate );
	
	DWORD FromPercent	= 0;
	DWORD ToPercent		= 0;

	for(WORD i = 0 ; i < pDropItem->wTotalNum ; ++i)
	{
		FromPercent = ToPercent;
		ToPercent += pDropItem->dwCurMonsterDropItemPercent[i];
		if(FromPercent < RandRate && RandRate <= ToPercent)
		{
			--pDropItem->dwCurMonsterDropItemPercent[i];
			--pDropItem->dwCurTotalRate;


			if(pDropItem->MonsterDropItemPercent[i].wItemIdx)
			{
				WORD DropItemId = pDropItem->MonsterDropItemPercent[i].wItemIdx;
				
				DISTRIBUTERAND->ItemChangeAtLv(DropItemId);
				LIMITITEMDROPMGR->DropLimitItem(MonsterKind,DropItemId,pPlayer->GetID());//[掉落控制][By:十里坡剑神][QQ:112582793][2018/5/13]
				//[物品提示发送][By:十里坡剑神][QQ:112582793][2017/12/6]
				sTIPITEMINFO * GetItem = NULL;
				GetItem = ITEMMGR->GetTipItem(DropItemId);
				BASE_MONSTER_LIST* pMonster = GAMERESRCMNGR->GetMonsterListInfo(MonsterKind);
				if(GetItem&&pMonster)
				{
					MSG_TIP msg;
					msg.Category = MP_CLIENT;
					msg.Protocol = MP_CLIENT_MSG_AGENT;
					msg.Flag = eMonsterDropItemTip;
					SafeStrCpy(msg.Name1,pPlayer->GetObjectName(),MAX_NAME_LENGTH+1);
					SafeStrCpy(msg.ItemName,pMonster->Name,MAX_ITEMNAME_LENGTH+1);
					SafeStrCpy(msg.ItemName1,GetItem->ItemName,MAX_ITEMNAME_LENGTH+1);
					pPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
				}
				ITEMMGR->MonsterObtainItem(pPlayer, DropItemId, MonsterKind);
			}
			break;
		}
	}

}


BOOL CItemDrop::ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem)
{
	WORD HaveItem = pDropItem->wTotalNum;

	pDropItem->dwCurTotalRate = 0;
	for(WORD i = 0; i < HaveItem; ++i)
	{
		pDropItem->dwCurMonsterDropItemPercent[i] = pDropItem->MonsterDropItemPercent[i].dwDropPercent;
		pDropItem->dwCurTotalRate += pDropItem->dwCurMonsterDropItemPercent[i];
	}
	if(pDropItem->dwCurTotalRate == 0)
		return FALSE;

	return TRUE;
}


void CItemDrop::ClearMonsterDropItemList()
{
	m_MonsterItemDropTable.SetPositionHead();
	MONSTER_DROP_ITEM* pDropItem;
	while(pDropItem = m_MonsterItemDropTable.GetData())
	{
		delete pDropItem;
	}
	m_MonsterItemDropTable.RemoveAll();
}

BOOL CItemDrop::LoadMonsterDropItemList()
{
	ClearMonsterDropItemList();

	char filename[64];
	char MonsterName[ MAX_MONSTER_NAME_LENGTH+1];
	CMHFile file;
	sprintf(filename,"./Resource/Server/MonsterDropItemList.bin");
	if(!file.Init(filename, "rb"))
		return FALSE;

	while(1)
	{
		if(file.IsEOF())
			break;
		
		MONSTER_DROP_ITEM* pDropItem = new MONSTER_DROP_ITEM;
		memset(pDropItem, 0, sizeof(MONSTER_DROP_ITEM));
		pDropItem->wItemDropIndex = file.GetWord();
		SafeStrCpy(MonsterName,file.GetString(), MAX_MONSTER_NAME_LENGTH+1);
		WORD totalnum = 0;
		for(int idx=0; idx<MAX_DROPITEM_NUM; ++idx)
		{
#ifdef _DEBUG
			SafeStrCpy( pDropItem->MonsterDropItemPercent[idx].Name, file.GetString(), MAX_ITEMNAME_LENGTH+1 );

			
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
				++totalnum;
#else
			char szTempName[256];
			file.GetString(szTempName);
			pDropItem->MonsterDropItemPercent[idx].wItemIdx = file.GetWord();		
			pDropItem->MonsterDropItemPercent[idx].dwDropPercent = file.GetDword();
			if(pDropItem->MonsterDropItemPercent[idx].dwDropPercent)
				++totalnum;
#endif
		}
		
		pDropItem->wTotalNum = totalnum;
		pDropItem->dwCurTotalRate = 0;

		ASSERT(!m_MonsterItemDropTable.GetData(pDropItem->wItemDropIndex));
		m_MonsterItemDropTable.Add(pDropItem,pDropItem->wItemDropIndex);

		ReloadMonsterDropItem(pDropItem);
	}	
	
	file.Release();
	
	return TRUE;
}
MONSTER_DROP_ITEM* CItemDrop::GetMonsterItemDrop(WORD wItemDropIdx)
{
	return m_MonsterItemDropTable.GetData(wItemDropIdx);
}
