



#include "stdafx.h"
#include "VimuManager.h"
#include "UserTable.h"
#include "Player.h"
#include "BattleSystem_server.h"
#include "VimuStreet/Battle_VimuStreet.h"
#include "CharMove.h"
#include "TileManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "objectstatemanager.h"
#include "PartyWarMgr.h"
#include "MapDBMsgParser.h"
CVimuManager::CVimuManager()
{
}

CVimuManager::~CVimuManager()
{

}

BOOL CVimuManager::CanApplyVimu( CPlayer* pApplyer, CPlayer* pAccepter )
{
//	if( g_pServerSystem->GetMapNum() == PKEVENTMAP )			return FALSE;
	//if( g_pServerSystem->GetMapNum() == RUNNINGMAP )			return FALSE;
	if( g_pServerSystem->GetMap()->IsMapSame(eRunningMap) )		return FALSE;
	//if( g_pServerSystem->CheckMapKindIs(eSurvival) )		return FALSE;
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )	return FALSE;
	if( BATTLESYSTEM->GetBattle(pApplyer)->GetBattleKind() != eBATTLE_KIND_NONE )		return FALSE;
	if( pApplyer->IsVimuing() )				return FALSE;
	if( pApplyer->IsPKMode() )				return FALSE;
	if( pApplyer->GetState() != eObjectState_None && pApplyer->GetState() != eObjectState_Move )
		return FALSE;

	if( pAccepter == NULL )											return FALSE;
	if( BATTLESYSTEM->GetBattle(pAccepter)->GetBattleKind() != eBATTLE_KIND_NONE )		return FALSE;
	if( pAccepter->IsVimuing() )									return FALSE;
	if( pAccepter->IsPKMode() )										return FALSE;
	if( pAccepter->GetState() == eObjectState_BattleReady )			return FALSE;
	if( pAccepter->GetState() == eObjectState_Deal )				return FALSE;
	if( pAccepter->GetState() == eObjectState_Exchange )			return FALSE;
	if( pAccepter->GetState() == eObjectState_StreetStall_Owner )	return FALSE;
	if( pAccepter->GetState() == eObjectState_StreetStall_Guest )	return FALSE;
	if( PARTYWARMGR->IsMemberInPartyWar( pApplyer ) || PARTYWARMGR->IsMemberInPartyWar( pAccepter ) )	return FALSE;

	return TRUE;
}

BOOL CVimuManager::CanAcceptVimu( CPlayer* pApplyer, CPlayer* pAccepter )
{
	if( pApplyer == NULL )									return FALSE;
	if( pApplyer->GetState() != eObjectState_BattleReady )	return FALSE;
	if( BATTLESYSTEM->GetBattle(pApplyer)->GetBattleKind() != eBATTLE_KIND_NONE )		return FALSE;
	if( !pApplyer->IsVimuing() )							return FALSE;
	if( pApplyer->IsPKMode() )								return FALSE;

	if( BATTLESYSTEM->GetBattle(pAccepter)->GetBattleKind() != eBATTLE_KIND_NONE )		return FALSE;
	if( !pAccepter->IsVimuing() )							return FALSE;
	if( pAccepter->IsPKMode() )								return FALSE;
	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move )
		return FALSE;

	return TRUE;
}

void CVimuManager::UserLogOut( CPlayer* pPlayer )
{
	if( pPlayer->IsVimuing() )		//厚公 脚没例瞒 贸府吝

	if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_NONE )
	{
		CPlayer* pOpPlayer = (CPlayer*)g_pUserTable->FindUser( pPlayer->GetVimuOpPlayerID() );
		if( !pOpPlayer ) return;

		if( pPlayer->GetState() == eObjectState_BattleReady )	//脚没磊
		{
			MSGBASE CancelMsg;
			CancelMsg.Category = MP_BATTLE;
			CancelMsg.Protocol = MP_BATTLE_VIMU_WAITING_CANCEL;
			pOpPlayer->SendMsg( &CancelMsg, sizeof( MSGBASE ) );

			pOpPlayer->SetVimuing( FALSE );
			if( pOpPlayer->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR_OBJ->EndObjectState( pOpPlayer, eObjectState_BattleReady );
		}
		else
		{
			MSGBASE RejectMsg;
			RejectMsg.Category	= MP_BATTLE;
			RejectMsg.Protocol	= MP_BATTLE_VIMU_REJECT;
			
			pOpPlayer->SendMsg( &RejectMsg, sizeof( RejectMsg ) );
			pOpPlayer->SetVimuing( FALSE );
			if( pOpPlayer->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR_OBJ->EndObjectState( pOpPlayer, eObjectState_BattleReady );
		}
	}
}

void CVimuManager::Battle_VimuWithGold_Apply_Syn(void * pMsg)
{
	//[押注比武邀请][BY:十里坡剑神][QQ:112582793][2019-4-27][18:27:15]
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer1) return;
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData1);
	if (pPlayer1->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Immortal);
	if (pPlayer2)
	{
		if (pPlayer2->GetGameOption()->bNoVimu)
		{
			MSG_BYTE msgError;
			msgError.Category = MP_BATTLE;
			msgError.Protocol = MP_BATTLE_VIMU_ERROR;
			msgError.bData = 3;
			pPlayer1->SendMsg(&msgError, sizeof(MSG_BYTE));

			return;
		}
		MSG_DWORD ApplySynMsg;
		ApplySynMsg.Category = MP_BATTLE;
		ApplySynMsg.dwData = pmsg->dwData1;
		if (!CanApplyVimu(pPlayer1, pPlayer2))
		{
			ApplySynMsg.Protocol = MP_BATTLE_VIMU_APPLY_NACK;
			pPlayer1->SendMsg(&ApplySynMsg, sizeof(MSG_DWORD));
			return;
		}
		if (pPlayer1->GetGoldMoney() < pmsg->dwData2)
		{//[押注比武元宝数量检查][BY:十里坡剑神][QQ:112582793][2019-4-27][18:10:47]
			g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", RGB(0, 0, 255), "元宝比武数据错误 角色ID1[%d] 角色名1[%s] 角色1元宝[%d],角色ID2[%d] 角色名2[%s] 角色2元宝[%d] 所需元宝数[%d]",
				pPlayer1->GetID(), pPlayer1->GetObjectName(), pPlayer1->GetGoldMoney(), pPlayer2->GetID(), pPlayer2->GetObjectName(), pPlayer2->GetGoldMoney(), pmsg->dwData2);
			ApplySynMsg.Protocol = MP_BATTLE_VIMUWITHGOLD_APPLY_NACK;
			pPlayer1->SendMsg(&ApplySynMsg, sizeof(MSG_DWORD));
			return;
		}
		ApplySynMsg.Protocol = MP_BATTLE_VIMU_APPLY_ACK;
		pPlayer1->SendMsg(&ApplySynMsg, sizeof(MSG_DWORD));

		pPlayer1->SetVimuing(TRUE);
		pPlayer1->SetVimuOpPlayerID(pPlayer2->GetID());
		OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer1, eObjectState_BattleReady, 0);
		pPlayer2->SetVimuing(TRUE);
		pPlayer2->SetVimuOpPlayerID(pPlayer1->GetID());

		MSG_DWORD2 ApplyMsg;
		ApplyMsg.Category = MP_BATTLE;
		ApplyMsg.Protocol = MP_BATTLE_VIMUWITHGOLD_APPLY;
		ApplyMsg.dwData1 = pmsg->dwObjectID;
		ApplyMsg.dwData2 = pmsg->dwData2;
		pPlayer2->SendMsg(&ApplyMsg, sizeof(MSG_DWORD2));
	}
}

void CVimuManager::Battle_VimuWithGold_Accept_Syn(void * pMsg)
{
	//[押注比武接受][BY:十里坡剑神][QQ:112582793][2019-4-27][18:27:31]
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer1) return;
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pPlayer1->GetVimuOpPlayerID());

	if (pPlayer1->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Immortal);
	if (pPlayer1->GetGoldMoney() < pmsg->dwData||pPlayer2->GetGoldMoney()<pmsg->dwData )
	{
		g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", RGB(0, 0, 255), "元宝比武数据错误 角色ID1[%d] 角色名1[%s] 角色1元宝[%d],角色ID2[%d] 角色名2[%s] 角色2元宝[%d] 所需元宝数[%d]",
			pPlayer1->GetID(), pPlayer1->GetObjectName(), pPlayer1->GetGoldMoney(), pPlayer2->GetID(), pPlayer2->GetObjectName(), pPlayer2->GetGoldMoney(), pmsg->dwData);
		return;
	}

	if (!CanAcceptVimu(pPlayer2, pPlayer1))
	{
		MSGBASE AcceptNackMsg;
		AcceptNackMsg.Category = MP_BATTLE;
		AcceptNackMsg.Protocol = MP_BATTLE_VIMU_ACCEPT_NACK;
		pPlayer1->SendMsg(&AcceptNackMsg, sizeof(MSGBASE));
		pPlayer1->SetVimuing(FALSE);

		if (pPlayer2)
		{
			MSG_BYTE ErrorMsg;
			ErrorMsg.Category = MP_BATTLE;
			ErrorMsg.Protocol = MP_BATTLE_VIMU_ERROR;
			ErrorMsg.bData = 1;
			pPlayer2->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));

			pPlayer2->SetVimuing(FALSE);
			if (pPlayer2->GetState() == eObjectState_BattleReady)
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);
		}

		return;
	}


	VECTOR3 vPosApplyer, vPosAccepter;
	pPlayer2->GetPosition(&vPosApplyer);
	pPlayer1->GetPosition(&vPosAccepter);
	float fDist = CalcDistanceXZ(&vPosApplyer, &vPosAccepter);

	if (fDist > 1500.0f)
	{
		MSG_BYTE ErrorMsg;
		ErrorMsg.Category = MP_BATTLE;
		ErrorMsg.Protocol = MP_BATTLE_VIMU_ERROR;
		ErrorMsg.bData = 2;
		pPlayer1->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));
		pPlayer2->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));

		pPlayer1->SetVimuing(FALSE);
		pPlayer2->SetVimuing(FALSE);
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);

		return;
	}

	BATTLE_INFO_VIMUSTREET				BattleInfo;
	BattleInfo.BattleKind = eBATTLE_KIND_VIMUSTREET;
	BattleInfo.BattleState = eBATTLE_STATE_READY;
	BattleInfo.BattleTime = BATTLE_VIMUSTREET_READYTIME;
	BattleInfo.Character[VIMUTEAM_BLUE] = pPlayer1->GetID();
	BattleInfo.Character[VIMUTEAM_RED] = pPlayer2->GetID();


	BattleInfo.vStgPos.x = (vPosApplyer.x + vPosAccepter.x) / 2.0f;
	BattleInfo.vStgPos.z = (vPosApplyer.z + vPosAccepter.z) / 2.0f;

	BATTLESYSTEM->CreateBattle(&BattleInfo, GAMERESRCMNGR->GetLoadMapNum());

	CBattle* pBattle = BATTLESYSTEM->GetBattle(BattleInfo.BattleID);

	pBattle->AddObjectToBattle(pPlayer2);
	pBattle->AddObjectToBattle(pPlayer1);

	pPlayer2->SetGoldMoney(pmsg->dwData, eVimuWithGold);
	pPlayer1->SetGoldMoney(pmsg->dwData, eVimuWithGold);
	ItemShopUseLog(eLog_VimuBattleLost, pPlayer2->GetID(), pPlayer2->GetObjectName(),0,"比武押金",0,
		0,0,0,
		pPlayer2->GetGoldMoney(), pmsg->dwData, pPlayer2->GetGoldMoney() - pmsg->dwData,
		0,0,0);
	ItemShopUseLog(eLog_VimuBattleLost, pPlayer1->GetID(), pPlayer1->GetObjectName(), 0, "比武押金", 0,
		0, 0, 0,
		pPlayer1->GetGoldMoney(), pmsg->dwData, pPlayer1->GetGoldMoney() - pmsg->dwData ,
		0, 0, 0);

	((CBattle_VimuStreet*)pBattle)->SetGoldVimuStreet((pmsg->dwData * 2)*0.975);

	MSG_DWORD AcceptMsg;
	AcceptMsg.Category = MP_BATTLE;
	AcceptMsg.Protocol = MP_BATTLE_VIMU_ACCEPT;
	AcceptMsg.dwData = pPlayer1->GetID();
	pPlayer2->SendMsg(&AcceptMsg, sizeof(MSG_DWORD));

	MSGBASE	AcceptAck;
	AcceptAck.Category = MP_BATTLE;
	AcceptMsg.Protocol = MP_BATTLE_VIMU_ACCEPT_ACK;
	pPlayer1->SendMsg(&AcceptMsg, sizeof(MSGBASE));
}
void CVimuManager::Battle_Vimu_Apply_Syn(void* pMsg)
{
	MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
	if (!pPlayer1) return;
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(msg->dwData1);

	if (pPlayer1->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Immortal);

	if (pPlayer2)
	{
		if (pPlayer2->GetGameOption()->bNoVimu)
		{
			MSG_BYTE msgError;
			msgError.Category = MP_BATTLE;
			msgError.Protocol = MP_BATTLE_VIMU_ERROR;
			msgError.bData = 3;
			pPlayer1->SendMsg(&msgError, sizeof(MSG_BYTE));

			return;
		}
	}


	MSG_DWORD ApplySynMsg;

	ApplySynMsg.Category = MP_BATTLE;
	ApplySynMsg.dwData = msg->dwData1;

	if (!CanApplyVimu(pPlayer1, pPlayer2))
	{
		ApplySynMsg.Protocol = MP_BATTLE_VIMU_APPLY_NACK;
		pPlayer1->SendMsg(&ApplySynMsg, sizeof(MSG_DWORD));
		return;
	}

	ApplySynMsg.Protocol = MP_BATTLE_VIMU_APPLY_ACK;
	pPlayer1->SendMsg(&ApplySynMsg, sizeof(MSG_DWORD));

	pPlayer1->SetVimuing(TRUE);
	pPlayer1->SetVimuOpPlayerID(pPlayer2->GetID());
	OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer1, eObjectState_BattleReady, 0);
	pPlayer2->SetVimuing(TRUE);
	pPlayer2->SetVimuOpPlayerID(pPlayer1->GetID());

	MSG_DWORD ApplyMsg;
	ApplyMsg.Category = MP_BATTLE;
	ApplyMsg.Protocol = MP_BATTLE_VIMU_APPLY;
	ApplyMsg.dwData = msg->dwObjectID;

	pPlayer2->SendMsg(&ApplyMsg, sizeof(MSG_DWORD));
}
void CVimuManager::Battle_Vimu_Accept_Syn(void* pMsg)
{
	MSGBASE* msg = (MSGBASE*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
	if (!pPlayer1) return;
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pPlayer1->GetVimuOpPlayerID());

	if (pPlayer1->GetState() == eObjectState_Immortal)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Immortal);


	if (!CanAcceptVimu(pPlayer2, pPlayer1))
	{
		MSGBASE AcceptNackMsg;
		AcceptNackMsg.Category = MP_BATTLE;
		AcceptNackMsg.Protocol = MP_BATTLE_VIMU_ACCEPT_NACK;
		pPlayer1->SendMsg(&AcceptNackMsg, sizeof(MSGBASE));
		pPlayer1->SetVimuing(FALSE);

		if (pPlayer2)
		{
			MSG_BYTE ErrorMsg;
			ErrorMsg.Category = MP_BATTLE;
			ErrorMsg.Protocol = MP_BATTLE_VIMU_ERROR;
			ErrorMsg.bData = 1;
			pPlayer2->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));

			pPlayer2->SetVimuing(FALSE);
			if (pPlayer2->GetState() == eObjectState_BattleReady)
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);
		}

		return;
	}


	VECTOR3 vPosApplyer, vPosAccepter;
	pPlayer2->GetPosition(&vPosApplyer);
	pPlayer1->GetPosition(&vPosAccepter);
	float fDist = CalcDistanceXZ(&vPosApplyer, &vPosAccepter);

	if (fDist > 1500.0f)
	{
		MSG_BYTE ErrorMsg;
		ErrorMsg.Category = MP_BATTLE;
		ErrorMsg.Protocol = MP_BATTLE_VIMU_ERROR;
		ErrorMsg.bData = 2;
		pPlayer1->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));
		pPlayer2->SendMsg(&ErrorMsg, sizeof(MSG_BYTE));

		pPlayer1->SetVimuing(FALSE);
		pPlayer2->SetVimuing(FALSE);
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);

		return;
	}

	BATTLE_INFO_VIMUSTREET				BattleInfo;
	BattleInfo.BattleKind = eBATTLE_KIND_VIMUSTREET;
	BattleInfo.BattleState = eBATTLE_STATE_READY;
	BattleInfo.BattleTime = BATTLE_VIMUSTREET_READYTIME;
	BattleInfo.Character[VIMUTEAM_BLUE] = pPlayer1->GetID();
	BattleInfo.Character[VIMUTEAM_RED] = pPlayer2->GetID();


	BattleInfo.vStgPos.x = (vPosApplyer.x + vPosAccepter.x) / 2.0f;
	BattleInfo.vStgPos.z = (vPosApplyer.z + vPosAccepter.z) / 2.0f;

	//			CTileManager* pTileManager = g_pServerSystem->GetMap()->GetTileManager();


	//			static VECTOR2 vOffset[4] = { { 200.0f, 0.0f }, { -200.0f, 0.0f },
	//										{ 0.0f, 200.0f }, { 0.0f, -200.0f } };

	//			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
	//			for( int i = 0 ; i < 4 ; ++i )
	//			{
	//				if( !pTileManager->CollisionTileWithPosition( vPos.x+vOffset[i].x, vPos.z+vOffset[i].y, MapNum ) )
	//				{
	//					vPos2.x		= vPos.x + vOffset[i].x;
	//					vPos2.z		= vPos.z + vOffset[i].y;
	//					BattleInfo.vStgPos.x = vPos.x + vOffset[i].x / 2.0f;
	//					BattleInfo.vStgPos.z = vPos.z + vOffset[i].y / 2.0f;
	//					break;
	//				}
	//			}
	//			if( i == 4 ) return;

	////Player1 WARP!
	//			VECTOR3 vPlayer1Pos;
	//			pPlayer1->GetPosition( &vPlayer1Pos );
	//			if( !( vPlayer1Pos.x > BattleInfo.vStgPos.x - 800.0f && vPlayer1Pos.x < BattleInfo.vStgPos.x + 800.0f &&
	//				vPlayer1Pos.z > BattleInfo.vStgPos.z - 800.0f && vPlayer1Pos.z < BattleInfo.vStgPos.z + 800.0f ) )
	//			{
	//				CCharMove::Warp( pPlayer1, &vPos2 );
	//			}

	//MAKE VIMU STAGE!
	//void CTileManager::AddTileAttrByAreaData(VECTOR3* pPos,AREATILE areatile);

	BATTLESYSTEM->CreateBattle(&BattleInfo, GAMERESRCMNGR->GetLoadMapNum());

	CBattle* pBattle = BATTLESYSTEM->GetBattle(BattleInfo.BattleID);

	pBattle->AddObjectToBattle(pPlayer2);
	pBattle->AddObjectToBattle(pPlayer1);

	MSG_DWORD AcceptMsg;
	AcceptMsg.Category = MP_BATTLE;
	AcceptMsg.Protocol = MP_BATTLE_VIMU_ACCEPT;
	AcceptMsg.dwData = pPlayer1->GetID();
	pPlayer2->SendMsg(&AcceptMsg, sizeof(MSG_DWORD));

	MSGBASE	AcceptAck;
	AcceptAck.Category = MP_BATTLE;
	AcceptMsg.Protocol = MP_BATTLE_VIMU_ACCEPT_ACK;
	pPlayer1->SendMsg(&AcceptMsg, sizeof(MSGBASE));
}
void CVimuManager::Battle_Vimu_Reject_Syn(void* pMsg)
{
	MSGBASE* msg = (MSGBASE*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
	if (pPlayer1 == NULL) return;

	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pPlayer1->GetVimuOpPlayerID());

	MSGBASE RejectAck;
	RejectAck.Category = MP_BATTLE;
	RejectAck.Protocol = MP_BATTLE_VIMU_REJECT_ACK;
	pPlayer1->SendMsg(&RejectAck, sizeof(MSGBASE));
	pPlayer1->SetVimuing(FALSE);

	if (pPlayer2)
	{
		MSGBASE RejectMsg;
		RejectMsg.Category = MP_BATTLE;
		RejectMsg.Protocol = MP_BATTLE_VIMU_REJECT;

		pPlayer2->SendMsg(&RejectMsg, sizeof(RejectMsg));

		pPlayer2->SetVimuing(FALSE);
		if (pPlayer2->GetState() == eObjectState_BattleReady)
			OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);
	}
}
void CVimuManager::Battle_Vimu_Waiting_Cancel_Syn(void* pMsg)
{
	MSGBASE* msg = (MSGBASE*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
	if (pPlayer1 == NULL) return;

	if (pPlayer1->GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET)
	{
		MSGBASE CancelMsg = *msg;

		CancelMsg.Protocol = MP_BATTLE_VIMU_WAITING_CANCEL_NACK;
		pPlayer1->SendMsg(&CancelMsg, sizeof(MSGBASE));
		return;
	}

	MSGBASE CancelMsg = *msg;
	CancelMsg.Protocol = MP_BATTLE_VIMU_WAITING_CANCEL_ACK;
	pPlayer1->SendMsg(&CancelMsg, sizeof(MSGBASE));

	pPlayer1->SetVimuing(FALSE);
	OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_BattleReady);

	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pPlayer1->GetVimuOpPlayerID());
	if (pPlayer2 == NULL) return;

	CancelMsg.Protocol = MP_BATTLE_VIMU_WAITING_CANCEL;
	pPlayer2->SendMsg(&CancelMsg, sizeof(MSGBASE));

	pPlayer2->SetVimuing(FALSE);
	if (pPlayer2->GetState() == eObjectState_BattleReady)
		OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);
}
void CVimuManager::Battle_Vimu_Error(void* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	CPlayer* pPlayer2 = NULL;

	if (pPlayer1)
		pPlayer2 = (CPlayer*)g_pUserTable->FindUser(pPlayer1->GetVimuOpPlayerID());

	if (pmsg->bData == 0)
	{

	}
	else if (pmsg->bData == 1)		//accept error from client
	{
		if (pPlayer1)	pPlayer1->SetVimuing(FALSE);
		if (pPlayer2)
		{

			OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_BattleReady);

			pPlayer2->SetVimuing(FALSE);
			pPlayer2->SendMsg(pmsg, sizeof(MSG_BYTE));
		}
	}
	else if (pmsg->bData == 2)
	{

	}
}
void CVimuManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_BATTLE_VIMUWITHGOLD_APPLY_SYN:				Battle_VimuWithGold_Apply_Syn(pMsg);				break;
	case MP_BATTLE_VIMUWITHGOLD_ACCEPT_SYN:				Battle_VimuWithGold_Accept_Syn(pMsg);				break;
	case MP_BATTLE_VIMU_APPLY_SYN:						Battle_Vimu_Apply_Syn(pMsg);						break;
	case MP_BATTLE_VIMU_ACCEPT_SYN:						Battle_Vimu_Accept_Syn(pMsg);						break;
	case MP_BATTLE_VIMU_REJECT_SYN:						Battle_Vimu_Reject_Syn(pMsg);						break;
	case MP_BATTLE_VIMU_WAITING_CANCEL_SYN:				Battle_Vimu_Waiting_Cancel_Syn(pMsg);				break;
	case MP_BATTLE_VIMU_ERROR:							Battle_Vimu_Error(pMsg);							break;
	}
}


