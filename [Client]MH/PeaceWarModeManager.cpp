// PeaceWarModeManager.cpp: implementation of the CPeaceWarModeManager class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PeaceWarModeManager.h"
#include "MoveManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "AppearanceManager.h"
#include "GameIn.h"
#include "MHTimeManager.h"
#include "CharStateDialog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPeaceWarModeManager);
CPeaceWarModeManager::CPeaceWarModeManager()
{
	m_dwBeginCheckTime	= 0;
}

CPeaceWarModeManager::~CPeaceWarModeManager()
{

}

void CPeaceWarModeManager::InitPeace_WarMode(CPlayer* pPlayer)
{
	BOOL bPeace = pPlayer->GetCharacterTotalInfo()->bPeace;
	if(bPeace == TRUE)
		MOVEMGR->SetRunMode(pPlayer);	
	else
		MOVEMGR->SetRunMode(pPlayer);	
}


void CPeaceWarModeManager::ToggleHeroPeace_WarMode()
{
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;
	BOOL bPeace = HERO->GetCharacterTotalInfo()->bPeace;

	if( bPeace == FALSE && HERO->InTitan() == TRUE )
		return;



	bPeace = !bPeace;

	if( bPeace == FALSE )
	{
		m_dwBeginCheckTime = gCurTime;
	}
	else
	{
		for( int i=0 ; i < eSpecialState_Max ; ++i )
		{
			if( HERO->IsInSpecialState( i ) == TRUE )
			{
				m_dwBeginCheckTime = gCurTime;
				return;
			}
		}
	}


	if( MOVEMGR->IsMoving(HERO) )
		MOVEMGR->HeroMoveStop();
	Peace_WarMode(HERO,bPeace);

	MSGBASE msg;
	msg.Category = MP_PEACEWARMODE;
	if(bPeace)
		msg.Protocol = MP_PEACEWARMODE_PEACE;

	else
		msg.Protocol = MP_PEACEWARMODE_WAR;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg,sizeof(msg));
}

void CPeaceWarModeManager::Peace_WarMode(CPlayer* pPlayer, BOOL bPeace)
{
	pPlayer->SetPeaceMode(bPeace?true:false);
	if(bPeace) 
	{
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{
			(pPlayer)->SetBaseMotion();
		}
	}
	else
	{
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{			
			APPEARANCEMGR->ShowWeapon(pPlayer);
			(pPlayer)->SetBaseMotion();
		}
	}
	if(pPlayer->GetState() == eObjectState_None)
		OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);
}

BOOL CPeaceWarModeManager::IsPeaceMode(CPlayer* pPlayer)
{
	return pPlayer->GetCharacterTotalInfo()->bPeace;

}

void CPeaceWarModeManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	CPlayer* pObject;


	MSGBASE* pmsg = (MSGBASE*)pMsg;
	pObject = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	ASSERT(pObject);
	if(pObject==NULL)
		return;
	ASSERT(pObject->GetObjectKind() == eObjectKind_Player);	
	
	switch(Protocol) 
	{
	case MP_PEACEWARMODE_PEACE:
		{

			Peace_WarMode(pObject,TRUE);
		}
		break;
	case MP_PEACEWARMODE_WAR:
		{
			Peace_WarMode(pObject,FALSE);
		}
		break;
	}
}

void CPeaceWarModeManager::DoCheckForPeaceMode(CPlayer* pPlayer)
{

	if( pPlayer->InTitan() == TRUE )
		return;
	if( pPlayer->GetCharacterTotalInfo()->bPeace == FALSE )
	{
		if( pPlayer->GetState() == eObjectState_None )
		{
			if( gCurTime - m_dwBeginCheckTime > MODEVALIDTIME )
			{
				ToggleHeroPeace_WarMode();				
			}
		}
		else
		{
			m_dwBeginCheckTime = gCurTime;
		}
	}
}




