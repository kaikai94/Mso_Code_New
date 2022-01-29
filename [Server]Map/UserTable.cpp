



// UserTable.cpp: implementation of the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"


#ifdef __MAPSERVER__
#ifndef _MURIMNET_
#include "SkillArea.h"
#include "CharMove.h"
#include "Player.h"
#endif
#endif

#include "Network.h"
#ifdef _AGENTSERVER
#include "TrafficLog.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserTable * g_pUserTable = NULL;
CUserTable * g_pUserTableForUserID = NULL;
#ifdef __AGENTSERVER__
#include "AgentDBMsgParser.h"
CUserTable * g_pUserTableForObjectID = NULL;
#endif



CUserTable::CUserTable()
{
}

CUserTable::~CUserTable()
{
}

#ifndef __MAPSERVER__
void CUserTable::Init(DWORD dwBucket)
{
	CYHHashTable<USERINFO>::Initialize(dwBucket);
	m_MaxUserCount = 0;
	m_dwUserCount = 0;
	
	m_addCount = 0;
	m_removeCount = 0;
}
USERINFO* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<USERINFO>::GetData(dwKey);
}

BOOL CUserTable::AddUser(USERINFO* pObject,DWORD dwKey)
{
	ASSERT(!FindUser(dwKey));
	++m_dwUserCount;

	++ m_addCount;

	return CYHHashTable<USERINFO>::Add(pObject,dwKey);	
}

USERINFO * CUserTable::RemoveUser(DWORD dwKey)
{
	USERINFO * info = NULL;
	info = (USERINFO *)GetData(dwKey);
	Remove(dwKey);

	if( info == NULL ) return NULL;

	--m_dwUserCount;

	++m_removeCount;

	return info;
}

void CUserTable::RemoveAllUser()
{
/*
//	SetPositionHead();
//	USERINFO * info = NULL;
//	while(info = (USERINFO *)GetData())
//	{
//		Remove(info->dwConnectionIndex);
//		info = NULL;
//	}
*///林籍贸府.
	m_dwUserCount = 0;
	CYHHashTable<USERINFO>::RemoveAll();
}

void CUserTable::SetCalcMaxCount(DWORD CurCount)
{
	if(m_MaxUserCount < CurCount)
	{
		m_MaxUserCount = CurCount;
	}
}

#else // __MAPSERVER__ 老锭 利侩
#ifndef _MURIMNET_
#include "ObjectFactory.h"
#endif
void CUserTable::Init(DWORD dwBucket)
{
	CYHHashTable<CObject>::Initialize(dwBucket);
	m_dwUserCount = 0;
	m_dwMonsterCount = 0;
	m_dwNpcCount = 0;
	m_dwExtraCount = 0;
}
CObject* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<CObject>::GetData(dwKey);
}

CObject* CUserTable::FindUserForName( char* strCharacterName )
{
	SetPositionHead();
	CObject* pObject = NULL;

	char buf1[MAX_NAME_LENGTH+1];
	char buf2[MAX_NAME_LENGTH+1];

	SafeStrCpy( buf1, strCharacterName, MAX_NAME_LENGTH+1 );
	strupr( buf1 );

	while( pObject = (CObject*)GetData() )
	{
		SafeStrCpy( buf2, pObject->GetObjectName(), MAX_NAME_LENGTH+1 );
		strupr( buf2 );

		if( strncmp( buf2, buf1, MAX_NAME_LENGTH ) == 0 && pObject->GetObjectKind()==eObjectKind_Player)
			return pObject;
	}

	return NULL;
}

BOOL CUserTable::AddUser(CObject* pObject,DWORD dwKey)
{
	switch(pObject->GetObjectKind())
	{
	case eObjectKind_Player:
		{
			++m_dwUserCount;
		}
		break;
	case eObjectKind_Monster:
		{
			++m_dwMonsterCount;
		}
		break;

	case eObjectKind_SpecialMonster:
		{
			++m_dwMonsterCount;
		}
		break;

	case eObjectKind_ToghterPlayMonster:
		{
			++m_dwMonsterCount;
		}
		break;

	case eObjectKind_Npc:
		{
			++m_dwNpcCount;
		}
		break;
	case eObjectKind_TitanMonster:
		{
			++m_dwMonsterCount;
		}
		break;

	default:
		{
			++m_dwExtraCount;
		}
		break;
	}
	return CYHHashTable<CObject>::Add(pObject,dwKey);	
}

CObject * CUserTable::RemoveUser(DWORD dwKey)
{
	CObject * info = NULL;
	info = (CObject *)GetData(dwKey);
	Remove(dwKey);
	
	if( info == NULL ) return NULL;

	switch(info->GetObjectKind())
	{
	case eObjectKind_Player:
		{
			--m_dwUserCount;
		}
		break;
	case eObjectKind_Monster:
		{
			--m_dwMonsterCount;
		}
		break;
	case eObjectKind_Npc:
		{
			--m_dwNpcCount;
		}
		break;
	case eObjectKind_TitanMonster:
		{
			--m_dwMonsterCount;
		}
		break;
	default:
		{

			--m_dwExtraCount;
		}
		break;
	}
	return info;
}

void CUserTable::RemoveAllUser()
{
	SetPositionHead();
	CObject * info = NULL;
	while(info = (CObject *)GetData())
	{
#ifndef _MURIMNET_
		g_pObjectFactory->ReleaseObject(info);
#endif
	}


	m_dwUserCount = 0;
	m_dwMonsterCount = 0;
	m_dwNpcCount = 0;
	m_dwExtraCount = 0;
}

void CUserTable::GetTargetInRange(VECTOR3* pPivotPos,float Radius,CTargetList* pTList,float SafeRange)
{
#ifndef _MURIMNET_
	pTList->Clear();
	CTargetListIterator iter(pTList);

	CObject* pObject;
	VECTOR3* pObjectPos;
	float dist;
	float dx,dz;

	SetPositionHead();
	while(pObject = GetData())
	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal)
				continue;
			
			pObjectPos = CCharMove::GetPosition(pObject); 

			dx = (pPivotPos->x - pObjectPos->x);
			dz = (pPivotPos->z - pObjectPos->z);
			dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
			if(dist <= Radius && dist >= SafeRange)
			{
				iter.AddTargetWithNoData(pObject->GetID(),SKILLRESULTKIND_NONE);
			}
		}
	}
	iter.Release();
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////

void CUserTable::GetTargetInArea(CSkillArea* pSkillArea,CTargetList* pTList)
{
#ifndef _MURIMNET_
	pTList->Clear();
	CTargetListIterator iter(pTList);

	CObject* pObject;
	VECTOR3* pObjectPos;
	
	SetPositionHead();
	while(pObject = GetData())
	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal)
				continue;
			
			pObjectPos = CCharMove::GetPosition(pObject); 
			if(pSkillArea->IsInDamageTile(pObjectPos) == TRUE)
			{
				iter.AddTargetWithNoData(pObject->GetID(),SKILLRESULTKIND_NONE);
			}
		}
	}
	iter.Release();
#endif
}

void CUserTable::GetTargetInDonutRangeInChannel(DWORD MonGridID, VECTOR3* pPivotPos,float MaxRadius, float MinRadius, CObject* rtpObject)
{
#ifndef _MURIMNET_
	
	CObject* pObject;
	VECTOR3* pObjectPos;
	float dist;
	float dx,dz;

	SetPositionHead();

	while(pObject = GetData())

	{
		if(	pObject->GetObjectKind() == eObjectKind_Player )
		{
			if(pObject->GetState() == eObjectState_Die || pObject->GetState() == eObjectState_Immortal)
				continue;
			if(pObject->GetGridID() != MonGridID)
				continue;

			if( !((CPlayer*)pObject)->IsVisible() )

				continue;

			pObjectPos = CCharMove::GetPosition(pObject); 
			dx = (pPivotPos->x - pObjectPos->x);
			dz = (pPivotPos->z - pObjectPos->z);
			dist = sqrtf(dx*dx + dz*dz) - pObject->GetRadius();
			if((dist <= MaxRadius) && (dist >= MinRadius))
			{
				rtpObject = pObject;

				break;
			}
		}

	}
#endif
}

#endif
