#include "stdafx.h"
#include ".\bobusangmanager.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "Player.h"
#include "Npc.h"
#include "ChannelSystem.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "RegenNPC.h"
#include "Network.h"
#include "QuestMapMgr.h"
#include "ChannelSystem.h"

GLOBALTON(BobusangManager)

BobusangManager::BobusangManager(void)
{
}

BobusangManager::~BobusangManager(void)
{
}

void BobusangManager::BobusangMgr_Init()
{
	m_pBobusang = NULL;
	DWORD channelCnt = CHANNELSYSTEM->GetChannelCount();
	m_pBobusang = new BOBUSANGTOTALINFO[channelCnt];
}

void BobusangManager::BobusangMgr_Process()
{

}

void BobusangManager::BobusangMgr_Release()

{
	DWORD channelCnt = CHANNELSYSTEM->GetChannelCount();

	for( DWORD i = 0; i < channelCnt; ++i )
	{
		RemoveBobusangNpc(i);
	}
	SAFE_DELETE_ARRAY(m_pBobusang);
}

BOOL BobusangManager::MakeNewBobusangNpc( BOBUSANGINFO* pBobusangInfo )
{
	if( !g_bReady )
		return FALSE;

	if( !pBobusangInfo )
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD channelNum = pBobusangInfo->AppearanceChannel;

	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return FALSE;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];

	if(pInfo->pBobusang)
	{
		RemoveBobusangNpc(channelNum);
	}

	pInfo->AppearanceInfo = *pBobusangInfo;
	NPC_LIST* Npclist;
	BASEOBJECT_INFO Baseinfo;
	NPC_TOTALINFO Npcinfo;

	Npclist = GAMERESRCMNGR->GetNpcInfo(BOBUSANG_NPCIDX);
	REGENNPC_OBJ->AddNpcObjectID();
	Baseinfo.dwObjectID = REGENNPC_OBJ->GetNpcObjectID();
	Baseinfo.BattleID = pInfo->AppearanceInfo.AppearanceChannel + 1;

	SafeStrCpy(Baseinfo.ObjectName, Npclist->Name, MAX_NAME_LENGTH+1);
	Npcinfo.Group = 999;

	Npcinfo.MapNum = g_pServerSystem->GetMapNum();
	Npcinfo.NpcKind = Npclist->NpcKind;
	Npcinfo.NpcUniqueIdx = BOBUSANG_wNpcUniqueIdx;
	Npcinfo.NpcJob = Npclist->JobKind;

	BobusangPosPerMap* pPos = GAMERESRCMNGR->GetBobusangPos(Npcinfo.MapNum, pBobusangInfo->AppearancePosIdx);

	pInfo->pBobusang = g_pServerSystem->AddNpc( &Baseinfo, &Npcinfo, &(pPos->ApprPos), pPos->ApprDir );

	pInfo->pDealItemInfo = ITEMMGR->GetDealer(BOBUSANG_wNpcUniqueIdx);

	if( NULL == pInfo->pDealItemInfo )
	{
		ASSERT(0);
		return FALSE;
	}

	pInfo->pDealItemInfo->MakeSellingList(&pInfo->SellingItemList, pInfo->AppearanceInfo.SellingListIndex);

	return TRUE;
}

BOOL BobusangManager::RemoveBobusangNpc( DWORD channelNum )
{
	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];

	if(!pInfo)
		return FALSE;

	if(!pInfo->pBobusang)
		return FALSE;

	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return FALSE;

	g_pServerSystem->RemoveNpc(pInfo->pBobusang->GetID());
	pInfo->pBobusang = NULL;

	pInfo->pDealItemInfo = NULL;

	PTRLISTPOS pos = pInfo->SellingItemList.GetHeadPosition();
	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pInfo->SellingItemList.GetAt(pos);
		if(pItem)
		{
			SAFE_DELETE(pItem);
		}
		pInfo->SellingItemList.GetNext(pos);
	}
	pInfo->SellingItemList.RemoveAll();

	pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pCustomer = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pCustomer->GetID());
		if(!pPlayer)
			pInfo->pCustomerList.RemoveAt(pos);
		else
			SendLeaveGuestMsg(pPlayer);

		pInfo->pCustomerList.GetNext(pos);
	}
	pInfo->pCustomerList.RemoveAll();

	return TRUE;
}

void BobusangManager::SetBobusanInfo( BOBUSANGINFO* pBobusangInfo )
{
	if( !pBobusangInfo )
	{
		ASSERT(0);
		return;
	}

	DWORD channelNum = pBobusangInfo->AppearanceChannel;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];
	pInfo->AppearanceInfo = *pBobusangInfo;
}

BOBUSANGTOTALINFO* BobusangManager::GetBobusang( DWORD mapChannelNum )
{
	if( 0 == mapChannelNum )
	{
		ASSERT(0);
		return NULL;
	}

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[mapChannelNum - 1];

	return pInfo;
}

int BobusangManager::GetBobusangSellingRt( DWORD channelNum, DealerItem* pItemList )
{
	BOBUSANGTOTALINFO* pInfo = GetBobusang(channelNum);

	PTRLISTPOS pos = pInfo->SellingItemList.GetHeadPosition();
	int invenNum = 0;

	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pInfo->SellingItemList.GetAt(pos);

		if( pItem )
		{
			pItemList[invenNum] = *pItem;
			++invenNum;
		}

		pInfo->SellingItemList.GetNext(pos);
	}

	return invenNum;
}

void BobusangManager::AddGuest( CPlayer* pPlayer )
{
	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());

	if( !pInfo->pBobusang )	return;	

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		if( pGuest == pPlayer )
		{
			break;
		}

		pInfo->pCustomerList.GetNext(pos);
	}

	pInfo->pCustomerList.AddTail((void*)pPlayer);


	DealerItem	SellingItem[SLOT_NPCINVEN_NUM];

	int dataNum = BOBUSANGMGR->GetBobusangSellingRt(pPlayer->GetChannelID(), SellingItem);

	MSG_ADDABLE_ONEKIND msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_ALL_DEALITEMINFO_TO_GUEST;
	msg.AddableInfo.AddInfo( CAddableInfoList::BobusangItemInfo, sizeof(DealerItem)*dataNum, SellingItem);
	msg.DataNum = dataNum;

	pPlayer->SendMsg(&msg, msg.GetSize());
}

void BobusangManager::LeaveGuest( CPlayer* pPlayer )
{
	if( !pPlayer )
		return;
	
	if( QUESTMAPMGR->IsQuestMap() )
		return;

	if( CHANNELSYSTEM->GetChannelCount() < pPlayer->GetChannelID() )
	{
		ASSERT(0);
		return;
	}

	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());

	if( !pInfo->pBobusang )	return; 

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		if( pGuest == pPlayer )
		{
			pInfo->pCustomerList.RemoveAt(pos);
			SendLeaveGuestMsg( pPlayer );
			break;
		}

		pInfo->pCustomerList.GetNext(pos);
	}
}

void BobusangManager::SendLeaveGuestMsg( CPlayer* pPlayer )
{
	MSG_DWORD	msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_LEAVE_GUEST_ACK;

	pPlayer->SendMsg(&msg, sizeof(msg));
}


BOOL BobusangManager::BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD BuyItemNum )
{
	BOBUSANGTOTALINFO* pInfo = GetBobusang(pPlayer->GetChannelID());
	if(!pInfo)
	{
		return FALSE;
	}

	BOOL bGuest = FALSE;
	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();
	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);

		if(pGuest == pPlayer)
		{
			bGuest = TRUE;
			break;
		}
		pInfo->pCustomerList.GetNext(pos);
	}

	if( FALSE == bGuest )
	{
		return FALSE;
	}

	DealerItem* pBuyItem = NULL;
	pBuyItem = GetSellingItem(&pInfo->SellingItemList, buyItemIdx);
	if( !pBuyItem )
	{
		return FALSE;
	}
	if( pBuyItem->ItemCount < BuyItemNum )
	{
		return FALSE;

	}
	pBuyItem->ItemCount -= BuyItemNum;
	
	MSG_DWORD3 msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_DEALITEMINFO_TO_GUEST;
	msg.dwData1	= pBuyItem->ItemIdx;
	msg.dwData2 = pBuyItem->Pos;
	msg.dwData3 = pBuyItem->ItemCount;

	SendDealerItemMsgToGuestAll(pInfo->AppearanceInfo.AppearanceChannel, &msg, sizeof(msg));

	return TRUE;
}

DealerItem*	BobusangManager::GetSellingItem( cPtrList* pSellingItemList, WORD buyItemIdx )
{
	if(!pSellingItemList)
		return NULL;

	PTRLISTPOS pos = pSellingItemList->GetHeadPosition();

	while(pos)
	{
		DealerItem* pItem = (DealerItem*)pSellingItemList->GetAt(pos);

		if( pItem->ItemIdx == buyItemIdx )
			return pItem;

		pSellingItemList->GetNext(pos);
	}

	return NULL;
}


void BobusangManager::AppearBobusang( MSG_BOBUSANG_INFO* pmsg )
{
	if( TRUE == MakeNewBobusangNpc(&pmsg->bobusangInfo) )
	{
		MSG_DWORD msg;
		msg.Category = MP_BOBUSANG;
		msg.Protocol = MP_BOBUSANG_APPEAR_MAP_TO_AGENT;
		msg.dwData	= pmsg->bobusangInfo.AppearanceChannel;
		g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
	}
}

void BobusangManager::DisappearBobusang( MSG_DWORD2* pmsg )
{
	if( TRUE == RemoveBobusangNpc(pmsg->dwData1) )
	{
		MSG_DWORD msg;
		msg.Category = MP_BOBUSANG;
		msg.Protocol = MP_BOBUSANG_DISAPPEAR_MAP_TO_AGENT;
		msg.dwData	= pmsg->dwData1;
		g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
	}
}

BOOL BobusangManager::CheckHackBobusangDist( CPlayer* pGuest )
{
	DWORD	guestChannel = pGuest->GetChannelID();

	BOBUSANGTOTALINFO* pInfo = GetBobusang(guestChannel);
	if(!pInfo)
		return FALSE;

	if(!pInfo->pBobusang)
		return FALSE;

	VECTOR3 vGPos, vBPos;
	pGuest->GetPosition(&vGPos);
	pInfo->pBobusang->GetPosition(&vBPos);
	float dist = CalcDistanceXZ( &vGPos, &vBPos );
	if( dist > 3000.0f )	return FALSE;

	return TRUE;
}

void BobusangManager::SendDealerItemMsgToGuestAll( DWORD channelNum, MSGBASE* pmsg, int size )
{
	if(channelNum > CHANNELSYSTEM->GetChannelCount()-1)
		return;

	BOBUSANGTOTALINFO* pInfo = &m_pBobusang[channelNum];
	if(!pInfo)
	{
		return;

	}

	PTRLISTPOS pos = pInfo->pCustomerList.GetHeadPosition();

	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)pInfo->pCustomerList.GetAt(pos);
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pGuest->GetID());
		if(!pPlayer)
			pInfo->pCustomerList.RemoveAt(pos);
		else
			pPlayer->SendMsg(pmsg, size);

		pInfo->pCustomerList.GetNext(pos);
	}
}

void BobusangManager::NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_BOBUSANG_ADD_GUEST_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			CPlayer* pGuest = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pGuest)	return;

			DWORD channel = pGuest->GetChannelID();

			BOBUSANGTOTALINFO* pInfo = GetBobusang(channel);

			if( !pInfo->pBobusang ||
				FALSE == ITEMMGR->CheckHackNpc( pGuest, pmsg->wData ) )
			{
				pmsg->Protocol = MP_BOBUSANG_ADD_GUEST_NACK;
				pGuest->SendMsg(pmsg, sizeof(*pmsg));
				return;
			}

			AddGuest(pGuest);
		}
		break;
	case MP_BOBUSANG_LEAVE_GUEST_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pGuest = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pGuest)	return;

			DWORD channel = pGuest->GetChannelID();

			BOBUSANGTOTALINFO* pInfo = GetBobusang(channel);


			if( !pInfo->pBobusang )	return;	

			LeaveGuest(pGuest);
		}
		break;
	case MP_BOBUSANG_INFO_AGENT_TO_MAP:
		{
			MSG_BOBUSANG_INFO* pmsg = (MSG_BOBUSANG_INFO*)pMsg;
			AppearBobusang(pmsg);	
		}
		break;
	case MP_BOBUSANG_DISAPPEAR_AGENT_TO_MAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			DisappearBobusang(pmsg);
		}	
		break;
	case MP_BOBUSANG_NOTIFY_FOR_DISAPPEARANCE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			SendDealerItemMsgToGuestAll(pmsg->dwData1, pmsg, sizeof(MSG_DWORD2));
		}
		break;
	default:
		{
			ASSERT(0);
			break;
		}
	}
}


