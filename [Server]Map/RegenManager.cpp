




// RegenManager.cpp: implementation of the CRegenManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegenManager.h"
#include "RegenPrototype.h"
#include "AIGroupManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Object.h"
#include "AIGroupManager.h"
#include "TileManager.h"
#include "Monster.h"
#include "BossMonster.h"
#include "GridSystem.h"
//test
#include "MHError.h"
//test

#include "UserTable.h"

#include "Player.h"

// �ʵ庸�� - 05.12 �̿���
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegenManager::CRegenManager()
{
	m_RegenPrototypeList.Initialize(500);
}

CRegenManager::~CRegenManager()
{
	Release();
}

void CRegenManager::Release()
{
	CRegenPrototype * pPrototype= NULL;
	m_RegenPrototypeList.SetPositionHead();
	while((pPrototype = m_RegenPrototypeList.GetData()))
	{
		delete pPrototype;
		pPrototype = NULL;

	}
	m_RegenPrototypeList.RemoveAll();
}

void CRegenManager::AddPrototype(CRegenPrototype * pType)
{
	m_RegenPrototypeList.Add(pType, pType->dwObjectID);

}
CRegenPrototype * CRegenManager::GetPrototype(DWORD dwID)
{
	return m_RegenPrototypeList.GetData(dwID);
}
void CRegenManager::RegenGroup(DWORD groupID, DWORD dwGridID)
{
	GROUPMGR->GetGroup(groupID, dwGridID)->ForceRegen();

}

BOOL CRegenManager::RegenObject(DWORD groupID, DWORD dwGridID, DWORD dwObjectID,BOOL bRandomPos)
{
	CAIGroup * pGroup = GROUPMGR->GetGroup(groupID, dwGridID);
	CRegenObject * pRegenInfo = pGroup->GetRegenObject(dwObjectID);
	BYTE kind = pRegenInfo->GetObjectKind();

	if( kind == eObjectKind_BossMonster || kind == eObjectKind_FieldBossMonster )
		bRandomPos = FALSE;

    if( GetObjectKindGroup(kind) == eOBJECTKINDGROUP_JOB )
		bRandomPos = FALSE;
	return RegenObject(dwObjectID, pRegenInfo->GetSubID(), dwGridID, 
		pRegenInfo->GetObjectKind(), 
		pRegenInfo->GetMonsterKind(), 
		pRegenInfo->GetPos(), 
		groupID, 0, 100, bRandomPos)
		? TRUE : FALSE;
}
void CRegenManager::RangePosAtOrig(VECTOR3 * OrigPos, int Range, VECTOR3 * OutPos)
{
	int ran=0;
	float dx=0,dz=0;
	ran = rand();
	dx = float(ran%Range) * (ran%2?1:-1);
	ran = rand();
	dz = float(ran%Range) * (ran%2?1:-1);
	OutPos->x = OrigPos->x + dx;
	OutPos->z = OrigPos->z + dz;
	
	if( OutPos->x < 0 )				OutPos->x = 0;
	else if( OutPos->x > 51199 )	OutPos->x = 51199;

	if( OutPos->z < 0 )				OutPos->z = 0;
	else if( OutPos->z > 51199 )	OutPos->z = 51199;
}
CMonster* CRegenManager::RegenObjectForGMTool(DWORD dwID, DWORD dwSubID, DWORD dwGridID, WORD wObjectKind, WORD wMonsterKind, VECTOR3 * vPos, DWORD dwGroupID, WORD DropItemID, DWORD dwDropRatio,BOOL bRandomPos, BOOL bEventRegen)
{
	if( g_pUserTable->FindUser( dwID ) )
	{
		ASSERT(0);
		return NULL;
	}

	int nRegenPosIndex = 0;

	VECTOR3 vOutPos;
	if(bRandomPos)
		RangePosAtOrig(vPos, MONSTER_REGEN_RANDOM_RANGE, &vOutPos);
	else
		vOutPos = *vPos;

	MAPTYPE MapNum = g_pServerSystem->GetGridSystem()->GetGridMapNum(dwGridID);
	vOutPos.x = ((DWORD)(vOutPos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	vOutPos.z = ((DWORD)(vOutPos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);

	switch( wObjectKind )
	{
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
	case eObjectKind_Mining:
	case eObjectKind_Collection:
	case eObjectKind_Hunt:
	case eObjectKind_TitanMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(pMonsterListInfo == NULL)
				return NULL;

			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			Baseinfo.BattleID = dwGridID;

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CMonster* pMonster = g_pServerSystem->AddMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos,wObjectKind);
			if(!pMonster)
				return NULL;

			pMonster->SetEventMob(bEventRegen);
			pMonster->SetDropItemID(DropItemID, dwDropRatio);
			return pMonster;

		}
		break;
	case eObjectKind_FieldBossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);

			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldBossMonster * pMob = g_pServerSystem->AddFieldBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetRegenPosIndex(nRegenPosIndex);		

			FIELDBOSSMONMGR->AddFieldBossMonster(pMob);
			pMob->SetEventMob(bEventRegen);
			g_Console.LOG(4, "BossMonster Regen : ����:%s, ����:x=%.1f,y=%.1f", Baseinfo.ObjectName,vOutPos.x,vOutPos.z);
			return pMob;
		}
		break;
	case eObjectKind_FieldSubMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;

			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);


			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldSubMonster * pMob = g_pServerSystem->AddFieldSubMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);

			return pMob;
		}
		break;

	case eObjectKind_BossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CBossMonster * pMob = g_pServerSystem->AddBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);

			return pMob;
		}
		break;
	case eObjectKind_Npc:

		{			
			BASEOBJECT_INFO Baseinfo;
			NPC_TOTALINFO NpcTotalInfo;
			STATIC_NPCINFO* pNpcInfo = GAMERESRCMNGR->GetStaticNpcInfo(wMonsterKind);
			if(pNpcInfo == 0)
			{
				return NULL;
			}
			NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcList(pNpcInfo->wNpcJob);
			if(pNpcList == 0)
			{
				return NULL;
			}
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}
			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;

			SafeStrCpy(Baseinfo.ObjectName, pNpcInfo->NpcName, MAX_NAME_LENGTH+1);
			NpcTotalInfo.Group = (WORD)dwGroupID;
			NpcTotalInfo.MapNum = MapNum;
			NpcTotalInfo.NpcJob = pNpcInfo->wNpcJob;
			NpcTotalInfo.NpcKind = pNpcList->NpcKind;
			NpcTotalInfo.NpcUniqueIdx = pNpcInfo->wNpcUniqueIdx;

			g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
			return NULL;
		}
		break;		
	default:
		{
			ASSERT(0);
			return NULL;
		}
	}
	return NULL;
}

CMonster* CRegenManager::RegenObject( DWORD dwID, DWORD dwSubID, DWORD dwGridID, WORD wObjectKind, WORD wMonsterKind, VECTOR3 * vPos, DWORD dwGroupID, WORD DropItemID /* = 0 */, DWORD dwDropRatio /* = 100  */,BOOL bRandomPos /* = TRUE */, BOOL bEventRegen /* = FALSE  */)
{
	if( g_pUserTable->FindUser( dwID ) )
	{
		ASSERT(0);
		return NULL;
	}

	int nRegenPosIndex = 0;
	if( wObjectKind == eObjectKind_FieldBossMonster )
	{
		WORD wRegenPosCount = FIELDBOSSMONMGR->GetRegenPositionCount();
		if( wRegenPosCount > 0 )
		{
			nRegenPosIndex = rand() % wRegenPosCount;
			VECTOR3 pos = FIELDBOSSMONMGR->GetRegenPosition(nRegenPosIndex);
			vPos->x = pos.x;
			vPos->z = pos.z;
		}
	}

	VECTOR3 vOutPos;
	if(bRandomPos)
		RangePosAtOrig(vPos, MONSTER_REGEN_RANDOM_RANGE, &vOutPos);
	else
		vOutPos = *vPos;

	MAPTYPE MapNum = g_pServerSystem->GetGridSystem()->GetGridMapNum(dwGridID);
	vOutPos.x = ((DWORD)(vOutPos.x/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);
	vOutPos.z = ((DWORD)(vOutPos.z/TILECOLLISON_DETAIL)*TILECOLLISON_DETAIL);

	switch( wObjectKind )
	{
	case eObjectKind_ToghterPlayMonster:
	case eObjectKind_SpecialMonster:
	case eObjectKind_Monster:
	case eObjectKind_Mining:
	case eObjectKind_Collection:
	case eObjectKind_Hunt:
	case eObjectKind_TitanMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;
			
			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(pMonsterListInfo == NULL)
				return NULL;

			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
		//		g_Console.LOG(4, "Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			Baseinfo.BattleID = dwGridID;
	//		g_Console.LOG(4, "Regen Succeeded : CHANNEL:%d, GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f)", dwGridID, dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z);

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;
			
			CMonster* pMonster = g_pServerSystem->AddMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos,wObjectKind);
			if(!pMonster)
				return NULL;

			pMonster->SetEventMob(bEventRegen);
			pMonster->SetDropItemID(DropItemID, dwDropRatio);
			return pMonster;

		}
		break;
	case eObjectKind_FieldBossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);

			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
	//			g_Console.LOG(4, "BossMonster Regen Failed : ID: %d, NAME: %s", dwID, pMonsterListInfo->Name);
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldBossMonster * pMob = g_pServerSystem->AddFieldBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetRegenPosIndex(nRegenPosIndex);		
			
			FIELDBOSSMONMGR->AddFieldBossMonster(pMob);
			pMob->SetEventMob(bEventRegen);
			g_Console.LOG(4, "����ˢ�� : ����:[%s], ����:x=%.1f,y=%.1f", Baseinfo.ObjectName,vOutPos.x/100,vOutPos.z/100);
			return pMob;
		}
		break;
	case eObjectKind_FieldSubMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;

			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);


			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CFieldSubMonster * pMob = g_pServerSystem->AddFieldSubMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);

			return pMob;
		}
		break;

	case eObjectKind_BossMonster:
		{
			MONSTER_TOTALINFO MonsterInfo;
			BASEOBJECT_INFO Baseinfo;
			BASE_MONSTER_LIST * pMonsterListInfo = NULL;

			pMonsterListInfo = GAMERESRCMNGR->GetMonsterListInfo(wMonsterKind);
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
				return NULL;
			}

			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			SafeStrCpy(Baseinfo.ObjectName, pMonsterListInfo->Name, MAX_NAME_LENGTH+1);
			//g_Console.LOG(4, "BossMonster Regen Succeeded : GROUPID: %d, ID: %d, NAME: %s, (x,z):(%f,%f)", dwGroupID, dwID, pMonsterListInfo->Name, vOutPos.x, vOutPos.z);
			//MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Boss Regen = GridID: %d, gCurTime: %d", dwGridID, gCurTime));

			MonsterInfo.Life = pMonsterListInfo->Life;
			MonsterInfo.Shield = pMonsterListInfo->Shield;
			MonsterInfo.MonsterKind = wMonsterKind;
			MonsterInfo.Group = (WORD)dwGroupID;

			CBossMonster * pMob = g_pServerSystem->AddBossMonster(dwSubID, &Baseinfo, &MonsterInfo, &vOutPos);
			pMob->SetEventMob(bEventRegen);
		
			return pMob;
		}
		break;
	case eObjectKind_Npc:

		{			
			BASEOBJECT_INFO Baseinfo;
			NPC_TOTALINFO NpcTotalInfo;
			STATIC_NPCINFO* pNpcInfo = GAMERESRCMNGR->GetStaticNpcInfo(wMonsterKind);
			if(pNpcInfo == 0)
			{
//				char buf[256];

//				sprintf(buf, "NO NPC Info - NpcKind: %d", wMonsterKind);
//				ASSERTMSG(0, buf);
				return NULL;
			}
			NPC_LIST* pNpcList = GAMERESRCMNGR->GetNpcList(pNpcInfo->wNpcJob);
			if(pNpcList == 0)
			{
//				char buf[256];
//				sprintf(buf, "NO NPC Info - NpcKind: %d", wMonsterKind);
//				ASSERTMSG(0, buf);
				return NULL;
			}
			if(g_pServerSystem->GetMap()->CollisionTilePos(vOutPos.x,vOutPos.z,MapNum) == TRUE)
			{
		//		g_Console.LOG(4, "Regen Failed : ID: %d, NpcUniqueIdx: %d", dwID, pNpcInfo->wNpcUniqueIdx);
				return NULL;
			}
			Baseinfo.dwObjectID = dwID;
			Baseinfo.BattleID = dwGridID;
			
			SafeStrCpy(Baseinfo.ObjectName, pNpcInfo->NpcName, MAX_NAME_LENGTH+1);
			NpcTotalInfo.Group = (WORD)dwGroupID;
			NpcTotalInfo.MapNum = MapNum;
			NpcTotalInfo.NpcJob = pNpcInfo->wNpcJob;
			NpcTotalInfo.NpcKind = pNpcList->NpcKind;
			NpcTotalInfo.NpcUniqueIdx = pNpcInfo->wNpcUniqueIdx;
		
			g_pServerSystem->AddNpc(&Baseinfo, &NpcTotalInfo, &vOutPos);
	//		g_Console.LOG(4, "NPC Regen Succeeded : ID: %d, UniqueIdx: %d, NAME: %s, (x,z):(%f,%f)", dwID, pNpcInfo->wNpcUniqueIdx, pNpcInfo->NpcName, vOutPos.x, vOutPos.z);

			return NULL;
		}
		break;		
	default:
		{
			ASSERT(0);
			return NULL;
		}
	}
	return NULL;
}


