// MoveManager.cpp: implementation of the CMoveManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoveManager.h"
#include "Object.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameResourceManager.h"
#include "KyungGongManager.h"
#include "GameIn.h"

#include ".\Engine\EngineObject.h"
#include "MHMap.h"

#include "MovePoint.h"

#include "CAction.h"
#include "ActionTarget.h"


#include "CharStateDialog.h"
#include "PathManager.h"
#include "TileManager.h"

#include "PartyIconManager.h"
#include "PetBase.h"
#include "Pet.h"
#include "PetManager.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
#include "ChatManager.h"
#include "MarryManager.h"
#include "./input/UserInput.h"
#include "AIManager.h"
#include "ReConnect.h"
extern int gChannelNum ;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMoveManager)
CMoveManager::CMoveManager()
{
	m_bSetMove = FALSE;
	m_bMoveCount = 0;
}

CMoveManager::~CMoveManager()
{

}

void CMoveManager::InitMove(CObject* pObject,BASEMOVE_INFO* pMoveInfo,float AngleDeg)
{
	pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
	pObject->m_RotateInfo.bRotating = FALSE;

	if(pMoveInfo)
	{
		memcpy(&pObject->m_MoveInfo,pMoveInfo,sizeof(BASEMOVE_INFO));
		if(pMoveInfo->bMoving)
		{
			StartMoveEx(pObject,NULL, gCurTime);
		}
	}
	else
	{
		memset(&pObject->m_MoveInfo,0,sizeof(BASEMOVE_INFO));
	}
	pObject->m_MoveInfo.m_bLastMoving = FALSE;

	pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{

		((CPlayer*)pObject)->SetBaseMotion();
	}
}

void CMoveManager::SetPosition(CObject* pObject,VECTOR3* pPos)
{
	pObject->m_MoveInfo.CurPosition = *pPos;
	pObject->SetPosition(pPos);
}

void CMoveManager::EndYMove(CObject* pObject)
{
	pObject->m_bIsYMoving = FALSE;
	pObject->GetEngineObject()->ApplyHeightField(TRUE);
}

void CMoveManager::EndMove(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Move)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move);

	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	pObject->m_MoveInfo.bMoving = FALSE;
	pObject->m_MoveInfo.InitTargetPosition();

	if(pObject->m_MoveInfo.KyungGongIdx)
	{
		if(pObject == HERO)
			ToggleHeroKyungGongMode();
	}
}

int CMoveManager::TargetUpdate(CObject* pObject, MOVE_INFO * pMoveInfo, VECTOR3 * TargetPos)
{
	float dx = TargetPos->x - pMoveInfo->CurPosition.x;
	float dz = TargetPos->z - pMoveInfo->CurPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return 1;
	}

	float Speed = pObject->GetMoveSpeed();
	float ttt = Speed / Distance;

	pMoveInfo->Move_StartTime			= gCurTime;
	pMoveInfo->Move_StartPosition		= pMoveInfo->CurPosition;
	pMoveInfo->Move_Direction.x			= dx*ttt;
	pMoveInfo->Move_Direction.y			= 0;
	pMoveInfo->Move_Direction.z			= dz*ttt;
	pMoveInfo->Move_EstimateMoveTime	= Speed==0 ? 999999999 : Distance / Speed;
	if (DWORD(Distance / pMoveInfo->Move_EstimateMoveTime) > 1800)
	{
		pMoveInfo->Move_EstimateMoveTime = 999999999;
		MSG_HACKUSER msg;
		memset(&msg, 0, sizeof(msg));
		msg.Category = MP_HACKCHECK;
		msg.Protocol = MP_CLIENT_HACKUSER_MSG_SYN;
		sprintf(msg.ProcessName, CHATMGR->GetChatMsg(2363));
		sprintf(msg.PrcessPath, CHATMGR->GetChatMsg(2375), DWORD (Distance / pMoveInfo->Move_EstimateMoveTime));
		sprintf(msg.UserName, REDISCONNECT->GetUserAccount());
		msg.UserID = REDISCONNECT->GetCharID();
		NETWORK->Send(&msg, sizeof(msg));
		NETWORK->Disconnect();
	}
	return 0;
}
void CMoveManager::StartMoveEx(CObject* pObject, VECTOR3* pStartPos ,DWORD CurTime)
{
	if(pObject->GetMoveSpeed() == 0 && pStartPos)
	{
		if(IsMoving(pObject))
			EndMove(pObject);
		SetPosition(pObject,pStartPos);
		return;
	}

	ASSERT( pObject->m_MoveInfo.GetCurTargetPosIdx() <= pObject->m_MoveInfo.GetMaxTargetPosIdx() );	//0000
	StartMoveEx(pObject, CurTime, pObject->m_MoveInfo.GetTargetPosition(pObject->m_MoveInfo.GetCurTargetPosIdx()));
}


void CMoveManager::StartMoveEx(CObject* pObject, DWORD CurTime, VECTOR3 * pTargetPos)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	if(pObject->GetState() == eObjectState_Die)
	{
		pMoveInfo->bMoving = FALSE;
		return;
	}

	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	pMoveInfo->bMoving = TRUE;
	pObject->m_MoveInfo.m_bEffectMoving = FALSE;
	
	if(TargetUpdate(pObject, pMoveInfo, pTargetPos) != 0)
	{
		return;
	}
	
	float TargetAngle = RADTODEG(VECTORTORAD(pMoveInfo->Move_Direction));
	pObject->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pObject->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pObject->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pObject->m_RotateInfo.Rotate_StartTime = CurTime;
	pObject->m_RotateInfo.bRotating = TRUE;
	
	if(pObject->GetState() != eObjectState_Move)
	{
		if(OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_Move) == TRUE)
			OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_Move,(DWORD)(pMoveInfo->Move_EstimateMoveTime*1000));		
	}
	else
	{
		pObject->SetMotionInState(eObjectState_Move);
		OBJECTSTATEMGR->EndObjectState(pObject, eObjectState_Move, (DWORD)(pMoveInfo->Move_EstimateMoveTime * 1000));
	}
	//if (pObject->GetObjectKind() == eObjectKind_Player)
	//{
		//char buf[512];
		//sprintf(buf, "移动速度 = %.3f,所需时间 = %.3f", pObject->GetMoveSpeed(), pMoveInfo->Move_EstimateMoveTime);
		//CHATMGR->AddMsg(CTC_TRACKING, buf);
	//}//debug输出信息
}

#define MOVESKIPCOUNT2	1
BOOL CMoveManager::CalcPositionEx(CObject* pObject,DWORD CurTime)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	
	if(pMoveInfo->bMoving)
	{
		if(IsGameSlow() == TRUE && pObject != HERO)
		{
			if(pObject->m_bMoveSkipCount < MOVESKIPCOUNT2)
			{
				++pObject->m_bMoveSkipCount;
				return FALSE;
			}
			else
				pObject->m_bMoveSkipCount = 0;
		}

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);
		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			if(pMoveInfo->GetCurTargetPosIdx()< pMoveInfo->GetMaxTargetPosIdx()- 1)
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				pMoveInfo->SetCurTargetPosIdx(pMoveInfo->GetCurTargetPosIdx()+1);
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				StartMoveEx(pObject, CurTime, pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx()));
			}
			else
			{
				ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
				pMoveInfo->CurPosition.x = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x;
				pMoveInfo->CurPosition.z = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z;
				bVal = TRUE;
			}
		}
		else
		{

			pMoveInfo->CurPosition.x = pMoveInfo->Move_StartPosition.x + pMoveInfo->Move_Direction.x * fMoveTime;
			pMoveInfo->CurPosition.z = pMoveInfo->Move_StartPosition.z + pMoveInfo->Move_Direction.z * fMoveTime;
		}
		if(pObject->m_MoveInfo.CurPosition.x!=0&&pObject->m_MoveInfo.CurPosition.z!=0)
			pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
		//else
		//	return FALSE;

		return bVal;
	}


	return FALSE;
}

BOOL CMoveManager::CalcYPosition(CObject* pObject,DWORD CurTime)
{
	if(pObject->m_bIsYMoving)
	{
		MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

		BOOL bVal = FALSE;
		float fMoveTime = ((CurTime - pMoveInfo->Move_StartTime)*0.001f);
		if(pMoveInfo->Move_EstimateMoveTime < fMoveTime)
		{
			pMoveInfo->CurPosition.y = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y;
			bVal = TRUE;
		}
		else
		{
			pMoveInfo->CurPosition.y = pMoveInfo->Move_StartPosition.y + pMoveInfo->Move_Direction.y * fMoveTime;
		}
		
		pObject->SetPosition(&pMoveInfo->CurPosition);

		return bVal;
	}

	return FALSE;
}

void CMoveManager::CalcAngle(CObject* pObject,DWORD CurTime)
{	
	if(pObject->m_RotateInfo.bRotating)
	{
		float fRotateTime = ((CurTime - pObject->m_RotateInfo.Rotate_StartTime)*0.001f);
		if(fRotateTime >= pObject->m_RotateInfo.EstimatedRotateTime)
		{
			pObject->m_RotateInfo.Angle.Process(1);
			pObject->m_RotateInfo.bRotating = FALSE;
		}
		else
		{
			pObject->m_RotateInfo.Angle.Process(fRotateTime / pObject->m_RotateInfo.EstimatedRotateTime);
		}
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
}

void CMoveManager::MoveProcess(CObject* pObject)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		if(CalcYPosition(pObject,gCurTime) == TRUE)
			EndYMove(pObject);
	}
	else
	{
		if(CalcPositionEx(pObject,gCurTime) == TRUE)
			EndMove(pObject);
	}
	CalcAngle(pObject,gCurTime);
	pMoveInfo->m_bLastMoving = pMoveInfo->bMoving;

}
void CMoveManager::SetHeroTarget(VECTOR3* pPos,BOOL bMousePointDisplay)
{
	//CHATMGR->AddMsg(CTC_SYSMSG,"正常移动");
	m_bSetMove = FALSE;
	CHero* pHero = OBJECTMGR->GetHero();

	if(pHero->IsInSpecialState(eSpecialState_Stun))
		return;

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanMove &&
		pHero->GetState() != eObjectState_Immortal  )
	{
		if( pHero->GetState() == eObjectState_Society )
		{
			if( pHero->EndSocietyAct() == FALSE )
			{
				CAction act;
				act.InitMoveAction(pPos);

				pHero->SetNextAction(&act);
				pHero->DisableAutoAttack();
				return;
			}
		}
		else
		{
			CAction act;
			act.InitMoveAction(pPos);
			pHero->SetNextAction(&act);
			pHero->DisableAutoAttack();
			return;
		}
	}
	if( pHero->GetState() == eObjectState_Move &&
		pHero->GetEngineObject()->GetCurMotion() == pHero->m_ObjectState.State_End_Motion)
		return;


	if(IsMoving(pHero))
		CalcPositionEx(pHero,gCurTime);

	pPos->x = (int(pPos->x / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	pPos->y = 0;
	pPos->z = (int(pPos->z / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;

	pHero->m_MoveInfo.InitTargetPosition();
	switch(PATHMGR->GetPath(&pHero->m_MoveInfo.CurPosition,pPos,pHero->m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, pHero->m_MoveInfo.GetMaxTargetPosIdxRef(), pHero, FALSE, 100))
	{
	case PATH_BLOCKFAILED:
	case PATH_FAILED:
		return;
	}
	WORD count = pHero->m_MoveInfo.GetMaxTargetPosIdx();



	if(bMousePointDisplay)
		MOVEPOINT->SetPoint(pPos);

	if( count == 0 )
		return;
	else if( count == 1 )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos( &pHero->m_MoveInfo.CurPosition );
		msg.SetTargetPos( pHero->m_MoveInfo.GetTargetPosition(0) );

		NETWORK->Send(&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_TARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos(&pHero->m_MoveInfo.CurPosition);

		ASSERT( count <= MAX_CHARTARGETPOSBUF_SIZE );
		for(int i=0;i<count;++i)
		{
			msg.AddTargetPos(pHero->m_MoveInfo.GetTargetPosition(i));
		}

		NETWORK->Send(&msg,msg.GetSize());
	}

	StartMoveEx(pHero,NULL,gCurTime);

	SetPetTarget(&pHero->m_MoveInfo, count);
}

BOOL CMoveManager::SetHeroTarget(VECTOR3* pPos,float biubiubiu)
{
	//CHATMGR->AddMsg(CTC_SYSMSG,"自动寻路");
	
	CHero* pHero = OBJECTMGR->GetHero();

	if(pHero->IsInSpecialState(eSpecialState_Stun))
		return FALSE;

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanMove &&
		pHero->GetState() != eObjectState_Immortal  )
	{
		if( pHero->GetState() == eObjectState_Society )
		{
			if( pHero->EndSocietyAct() == FALSE )
			{
				CAction act;
				act.InitMoveAction(pPos);

				pHero->SetNextAction(&act);
				pHero->DisableAutoAttack();
				return FALSE;
			}
		}
		else
		{
			CAction act;
			act.InitMoveAction(pPos);
			pHero->SetNextAction(&act);
			pHero->DisableAutoAttack();
			return FALSE;
		}
	}
	if( pHero->GetState() == eObjectState_Move &&
		pHero->GetEngineObject()->GetCurMotion() == pHero->m_ObjectState.State_End_Motion)
		return FALSE;

	
	if(IsMoving(pHero))
		CalcPositionEx(pHero,gCurTime);
	
	pPos->x = (int(pPos->x / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;
	pPos->y = 0;
	pPos->z = (int(pPos->z / TILECOLLISON_DETAIL) + 0.5f) * TILECOLLISON_DETAIL;

	pHero->m_MoveInfo.InitTargetPosition();

	VECTOR3 t_StartPos;
	HERO->GetPosition( &t_StartPos );
	if (MAP->CollisionCheck_OneTile( pPos ) == FALSE)
	{
		if( MAP->CollisionLine( &t_StartPos, pPos, pPos, MAP->GetMapNum(), pHero ) == FALSE )
			return FALSE;
	}
	if( MOVEMGR->IsMoving( pHero ) )
		MOVEMGR->CalcPositionEx( pHero, gCurTime );
	
	BOOL t_CollisionCheck_Oneline = MAP->CollisionCheck_OneLine_New( &t_StartPos, pPos );

	if ( t_CollisionCheck_Oneline == TRUE)
	{
		pHero->m_MoveInfo.InitTargetPosition();
		pHero->m_MoveInfo.SetTargetPosition( 0, *pPos );
		pHero->m_MoveInfo.SetMaxTargetPosIdx( 1 );

		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = HEROID;
		msg.SetStartPos(&pHero->m_MoveInfo.CurPosition);
		msg.SetTargetPos(pHero->m_MoveInfo.GetTargetPosition(0));
		NETWORK->Send(&msg,sizeof(msg));
		StartMoveEx(pHero,NULL,gCurTime);
		SetPetTarget(&HERO->m_MoveInfo,1);
		m_bMoveCount = 0;
		return TRUE;
	}
	else
	{
		//CHATMGR->AddMsg(CTC_SYSMSG, "自动寻路");
		m_bSetMove = TRUE;
		if( MOVEMGR->IsMoving( pHero ) )
			MOVEMGR->HeroMoveStop();
		return HERO->Move_UsePath(pPos,TRUE,FALSE,TRUE);
	}
}

void CMoveManager::SetPetTarget( MOVE_INFO* pHeroMoveInfo, WORD wPosCount )
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;

	if(IsMoving(pPet))
	{
		CalcPositionEx(pPet,gCurTime);
	}

	VECTOR3 PetLastPos;
	PetLastPos.x = PetLastPos.y = PetLastPos.z = 0;

	VECTOR3* pHeroLastPos = pHeroMoveInfo->GetTargetPosition(wPosCount-1);

	float Indist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, &pHeroMoveInfo->CurPosition);

	if( IsMoving(pPet) )
	{
		float HeroToTargetDist = CalcDistanceXZ(&pHeroMoveInfo->CurPosition, pHeroLastPos);
		float PetToTargetDist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, pHeroLastPos);
		if( (HeroToTargetDist - PetToTargetDist) >= 0 )
		{
			PetMoveStop();
			return;
		}
	}

	float dist = 0.f;
	if( 0 == wPosCount )	return;

	if( 1 == wPosCount )
	{
		dist = CalcDistanceXZ(&pHeroMoveInfo->CurPosition, pHeroLastPos);
		if( DEFAULT_PET_FOLLOW_DIST < dist )
		{
			if(!GetPetLastPos(&PetLastPos,pHeroLastPos,&pHeroMoveInfo->CurPosition))
				return;
		}
		else
		{
			PetLastPos = pHeroMoveInfo->CurPosition;
		}
	}
	else
	{
		VECTOR3* pHeroLastBeforePos = pHeroMoveInfo->GetTargetPosition(wPosCount-2);
		dist = CalcDistanceXZ(pHeroLastPos, pHeroLastBeforePos);

		if( DEFAULT_PET_FOLLOW_DIST < dist )

		{
			if(!GetPetLastPos(&PetLastPos,pHeroLastPos,pHeroLastBeforePos))
				return;
		}
		else
		{
			PetLastPos = *pHeroLastBeforePos;
		}
	}

	pPet->m_MoveInfo.InitTargetPosition();
	switch(PATHMGR->GetPath(&pPet->m_MoveInfo.CurPosition,&PetLastPos,pPet->m_MoveInfo.GetTargetPositionArray(), MAX_CHARTARGETPOSBUF_SIZE, pPet->m_MoveInfo.GetMaxTargetPosIdxRef(), pPet, FALSE, 100))
	{
	case PATH_BLOCKFAILED:
	case PATH_FAILED:
		return;
	}

	
	WORD count = pPet->m_MoveInfo.GetMaxTargetPosIdx();
	if(TargetUpdate(pPet,&pPet->m_MoveInfo,pPet->m_MoveInfo.GetTargetPosition(count-1)) != 0)
		return;

	OBJECTSTATEMGR->EndObjectState(pPet,eObjectState_Move,(DWORD)(pPet->m_MoveInfo.Move_EstimateMoveTime*1000));

	float TargetAngle = RADTODEG(VECTORTORAD(pPet->m_MoveInfo.Move_Direction));
	pPet->m_RotateInfo.Angle.SetTargetDeg(TargetAngle);
	float fChangedAngle = pPet->m_RotateInfo.Angle.GetChangeAmountDeg();
	if(fChangedAngle < 0)
		fChangedAngle*=-1.f;
	pPet->m_RotateInfo.EstimatedRotateTime = 0.1f + fChangedAngle * 0.2f / 180.f;
	pPet->m_RotateInfo.Rotate_StartTime = gCurTime;
	pPet->m_RotateInfo.bRotating = TRUE;
	
	Indist = CalcDistanceXZ(pPet->m_MoveInfo.GetTargetPosition(count-1), pHeroLastPos);
	dist = CalcDistanceXZ(&pPet->m_MoveInfo.CurPosition, pHeroLastPos);

 	if( DEFAULT_PET_FOLLOW_DIST < Indist || DEFAULT_PET_FOLLOW_DIST < dist)
 	{
 		PETMGR->SetMoveReady(TRUE);
 	}
	
}

void CMoveManager::SendPetMoveMsg()
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;

	WORD count = pPet->m_MoveInfo.GetMaxTargetPosIdx();

	if( 0 == count ) return;
	else if( 1 == count )
	{
		MOVE_ONETARGETPOS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_ONETARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pPet->GetID();
		msg.SetStartPos( &pPet->m_MoveInfo.CurPosition );
		msg.SetTargetPos( pPet->m_MoveInfo.GetTargetPosition(0) );

		NETWORK->Send(&msg,msg.GetSize());
	}
	else
	{
		MOVE_TARGETPOS msg;
		msg.Init();
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_PET_TARGET;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pPet->GetID();
		msg.SetStartPos( &pPet->m_MoveInfo.CurPosition );
		ASSERT( count <= MAX_CHARTARGETPOSBUF_SIZE );
		for( int i = 0; i < count; ++i )
		{
			msg.AddTargetPos(pPet->m_MoveInfo.GetTargetPosition(i));
		}
		NETWORK->Send(&msg,msg.GetSize());
	}

	StartMoveEx(pPet,NULL,gCurTime);

	PETMGR->SetMoveReady(FALSE);
}

BOOL CMoveManager::GetPetLastPos( VECTOR3* pSrcPos, VECTOR3* pFromPos, VECTOR3* pToPos )
{
	VECTOR3 Pos = *pToPos - *pFromPos;
	float dist = sqrtf(Pos.x*Pos.x + Pos.z*Pos.z);

	if(dist < DEFAULT_PET_FOLLOW_DIST)
	{
		ASSERT(0);
		printf("ASSERT %f\n",dist);
		return FALSE;
	}

	pSrcPos->x = pFromPos->x + (Pos.x / dist * DEFAULT_PET_FOLLOW_DIST);
	pSrcPos->z = pFromPos->z + (Pos.z / dist * DEFAULT_PET_FOLLOW_DIST);


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////
void CMoveManager::SetLookatPos(CObject* pObject, VECTOR3* pPos,float TurnDuration, DWORD CurTime, int AddDeg)
{
	pObject->m_MoveInfo.CurPosition.y = 0;
	VECTOR3 PositionGap = *pPos - pObject->m_MoveInfo.CurPosition;
	float Distance = VECTOR3Length(&PositionGap);
	if(Distance == 0)
		return;
	VECTOR3 TDirection = PositionGap / Distance;
	float angle = RADTODEG(VECTORTORAD(TDirection)) + AddDeg;
	SetAngle(pObject,angle,TurnDuration);
}
/////////////////////////////////////////////////////////////////////////////////////

void CMoveManager::SetAngle(CObject* pObject,float AngleDeg,float TurnDuration)
{
	if (pObject == NULL) return;
	if(TurnDuration == 0)
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.bRotating = FALSE;
		pObject->SetAngle(pObject->m_RotateInfo.Angle.ToRad());
	}
	else
	{
		pObject->m_RotateInfo.Angle.SetAngleDeg(AngleDeg);
		pObject->m_RotateInfo.EstimatedRotateTime = TurnDuration;
		pObject->m_RotateInfo.Rotate_StartTime = gCurTime;
		pObject->m_RotateInfo.bRotating = TRUE;
	}
}

void CMoveManager::SetHeroActionMove(CActionTarget* pTarget,CAction* pAction)
{
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return;
	if (!m_bSetMove || m_bMoveCount < 2)
	{
		SetHeroTarget(pTargetPos, 1.f);
		m_bMoveCount++;
	}
		
	//else
	//	SetHeroTarget(pTargetPos,FALSE);

	CHero* pHero = OBJECTMGR->GetHero();
	pHero->SetMovingAction(pAction);
	
	pHero->SetCurComboNum(0);
}

void CMoveManager::MoveStop(CObject* pObject,VECTOR3* pStopPos)
{
	if(pObject->m_MoveInfo.bMoving)
		EndMove(pObject);
	if(pStopPos)
	{
		pObject->m_MoveInfo.CurPosition = *pStopPos;
		pObject->SetPosition(pStopPos);
	}
	else
	{
		pObject->SetPosition(&pObject->m_MoveInfo.CurPosition);
	}
}

void CMoveManager::HeroMoveStop()
{
	CHero* pHero = OBJECTMGR->GetHero();
	if(pHero->m_MoveInfo.bMoving == FALSE)
		return;

	CalcPositionEx(pHero,gCurTime);
	
	MoveStop(pHero,&pHero->m_MoveInfo.CurPosition);

	if(pHero->GetState() == eObjectState_Die)
		return;

	if( !OBJECTSTATEMGR->StartObjectState(pHero, eObjectState_None) )
		return;	

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = HEROID;
	msg.cpos.Compress(&pHero->m_MoveInfo.CurPosition);

	NETWORK->Send(&msg,sizeof(MOVE_POS));

	if(PETMGR->GetCurSummonPet())
	{
		PetMoveStop();
	}
}

void CMoveManager::PetMoveStop()
{
	CPet* pPet = HERO->GetPet();
	if(!pPet) return;
	if( FALSE == pPet->m_MoveInfo.bMoving )
		return;

	CalcPositionEx(pPet, gCurTime);

	MoveStop(pPet, &pPet->m_MoveInfo.CurPosition);

	MOVE_POS msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_PET_STOP;
	msg.dwObjectID = HEROID;
	msg.dwMoverID = pPet->GetID();
	msg.cpos.Compress(&pPet->m_MoveInfo.CurPosition);

	NETWORK->Send(&msg,sizeof(msg));
}

float CMoveManager::CalcDistance(CObject* pObject1,CObject* pObject2)
{
	float tx = pObject1->m_MoveInfo.CurPosition.x - pObject2->m_MoveInfo.CurPosition.x;
	float tz = pObject1->m_MoveInfo.CurPosition.z - pObject2->m_MoveInfo.CurPosition.z;

	return sqrtf(tx*tx+tz*tz);
}


void CMoveManager::SetWalkMode(CObject* pObject)
{
	pObject->m_MoveInfo.MoveMode = eMoveMode_Walk;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

void CMoveManager::SetRunMode(CObject* pObject)
{

	pObject->m_MoveInfo.MoveMode = eMoveMode_Run;
	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}
}

BOOL CMoveManager::SetKyungGong(CObject* pObject,WORD KGIdx)
{
	if(IsValidKyungGong(pObject,KGIdx) == FALSE)
		return FALSE;

	pObject->m_MoveInfo.KyungGongIdx = KGIdx;

	
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetBaseMotion();
	}


	return TRUE;
}
BOOL CMoveManager::SetMonsterMoveType(CObject* pObject,WORD KGIdx)
{
	if(!pObject) return FALSE;
	if((pObject->GetObjectKind() & eObjectKind_Monster) == FALSE)
	{
		ASSERT(0);
		return FALSE;
	}
	
	pObject->m_MoveInfo.KyungGongIdx = KGIdx;
	return TRUE;
}

BOOL CMoveManager::IsValidKyungGong(CObject* pObject,WORD KGIdx)
{
	return TRUE;
}

float CMoveManager::GetKyungGongSpeed(WORD KyungGongIdx)
{

	ASSERT(KyungGongIdx);
	CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(KyungGongIdx);
	ASSERT(pKGInfo);
	if(pKGInfo == NULL)
		return 0;

	return pKGInfo->GetSpeed();
}

void CMoveManager::EffectYMove(CObject* pObject,float StartY,float EndY,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;

	pMoveInfo->bMoving = TRUE;
	pObject->m_bIsYMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;

	VECTOR3 CurPos;
	pObject->GetPosition(&CurPos);

	pMoveInfo->Move_StartPosition.x = CurPos.x;
	pMoveInfo->Move_StartPosition.z = CurPos.z;
	pMoveInfo->Move_StartPosition.y = CurPos.y + StartY;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );	//0000
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = CurPos.x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = CurPos.z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = CurPos.y + EndY;

	float Distance = EndY - StartY;
	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;

	Speed = Distance / fTime;
	pMoveInfo->Move_Direction.x = 0;
	pMoveInfo->Move_Direction.y = Speed;
	pMoveInfo->Move_Direction.z = 0;
	pMoveInfo->Move_EstimateMoveTime = fTime;
	
	pObject->GetEngineObject()->ApplyHeightField(FALSE);
}

void CMoveManager::EffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration)
{
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	if(pObject->m_bIsYMoving)
	{
		EndYMove(pObject);
	}

	VECTOR3 repos;
	if( MAP->CollisonCheck(&pMoveInfo->CurPosition,TargetPos,&repos,pObject) == TRUE )
		*TargetPos = repos;



	pMoveInfo->bMoving = TRUE;
	pMoveInfo->m_bEffectMoving = TRUE;

	float Speed;	
	
	pMoveInfo->Move_StartTime = gCurTime;


	pMoveInfo->SetCurTargetPosIdx(0);
	pMoveInfo->SetMaxTargetPosIdx(1);

	pMoveInfo->Move_StartPosition.x = pMoveInfo->CurPosition.x;
	pMoveInfo->Move_StartPosition.z = pMoveInfo->CurPosition.z;
	pMoveInfo->Move_StartPosition.y = 0;

	ASSERT( pMoveInfo->GetCurTargetPosIdx() <= MAX_CHARTARGETPOSBUF_SIZE );
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x = TargetPos->x;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z = TargetPos->z;
	pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->y = 0;

	float dx = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->x - pMoveInfo->Move_StartPosition.x;
	float dz = pMoveInfo->GetTargetPosition(pMoveInfo->GetCurTargetPosIdx())->z - pMoveInfo->Move_StartPosition.z;
	float Distance = sqrtf(dx*dx+dz*dz);
	if(Distance <= 0.01)
	{
		EndMove(pObject);
		return;
	}

	if(Duration == 0)
		Duration = 1;
	float fTime = Duration * 0.001f;
	Speed = Distance / fTime;
	float ttt = Speed / Distance;
	pMoveInfo->Move_Direction.x = dx*ttt;
	pMoveInfo->Move_Direction.y = 0;
	pMoveInfo->Move_Direction.z = dz*ttt;
	pMoveInfo->Move_EstimateMoveTime = fTime;
}

void CMoveManager::HeroEffectMove(CObject* pObject,VECTOR3* TargetPos,DWORD Duration,CObject* pOperator)
{
	if( pObject->GetObjectKind() == eObjectKind_SkillObject ||
		pObject->GetObjectKind() == eObjectKind_Tactic ||
		pObject->GetObjectKind() == eObjectKind_Item )
		return;

	if(pObject != HERO && pOperator != HERO)
	{
		EffectMove(pObject,TargetPos,Duration);
		return;
	}

	if(pObject == HERO)
	{
		HeroMoveStop();
	}
	
	pObject->m_MoveInfo.SetCurTargetPosIdx(0);
	pObject->m_MoveInfo.SetMaxTargetPosIdx(1);
	EffectMove(pObject,TargetPos,Duration);

	if(pObject->IsDied() == FALSE)
	{
		MOVE_POS msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_EFFECTMOVE;
		msg.dwObjectID = HEROID;
		msg.dwMoverID = pObject->GetID();
		msg.cpos.Compress(TargetPos);
		NETWORK->Send(&msg,sizeof(msg));	
	}
}

void CMoveManager::EffectMoveInterpolation(CObject* pObject,VECTOR3* TargetPos)
{
	if(pObject->IsDied())
		return;

	
	MOVE_INFO* pMoveInfo = &pObject->m_MoveInfo;
	DWORD dwTime = (DWORD)(pMoveInfo->Move_EstimateMoveTime * 1000);
	DWORD ElapsedTime = gCurTime - pMoveInfo->Move_StartTime;
	if(ElapsedTime >= dwTime)
	{
		if(pObject->m_MoveInfo.bMoving == FALSE)
			SetPosition(pObject,TargetPos);
	}
	else
	{
		DWORD RemainTime = dwTime - ElapsedTime;
		EffectMove(pObject,TargetPos,RemainTime);
	}
	
}

void CMoveManager::ToggleHeroMoveMode()
{
	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	}
	HeroMoveStop();
	
	MSGBASE msg;
	msg.Category = MP_MOVE;
	msg.dwObjectID = HEROID;
	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
	{
		msg.Protocol = MP_MOVE_RUNMODE;
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( FALSE );
	}
	else
	{
		msg.Protocol = MP_MOVE_WALKMODE;
		if(GAMEIN->GetCharStateDialog())
			GAMEIN->GetCharStateDialog()->SetMoveMode( TRUE );
	}
	NETWORK->Send(&msg,sizeof(msg));

	if(HERO->m_MoveInfo.MoveMode == eMoveMode_Walk)
		SetRunMode(HERO);
	else
		SetWalkMode(HERO);

	if(bTarget)
	{
		if(m_bSetMove)
			SetHeroTarget(&TargetPos,1.f);
		else
			SetHeroTarget(&TargetPos,FALSE);
	}
}

void CMoveManager::ToggleHeroKyungGongMode()
{
	WORD kgidx = HERO->m_MoveInfo.KyungGongIdx;
	MSG_DWORD2 msg;
	msg.Category = MP_MOVE;
	msg.Protocol = MP_MOVE_KYUNGGONG_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = HEROID;


	VECTOR3 TargetPos;
	BOOL bTarget = FALSE;
	if(IsMoving(HERO))
	{
		bTarget = TRUE;
		TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	}
	
	HeroMoveStop();

	if(kgidx == 0)
	{
		msg.dwData2 = HERO->GetKyungGongGrade();
		SetKyungGong(HERO,HERO->GetKyungGongGrade());
	}
	else
	{
		msg.dwData2 = 0;
		SetKyungGong(HERO,0);
	}
	NETWORK->Send(&msg,sizeof(msg));


	if(bTarget)
	{
		if(m_bSetMove)
			SetHeroTarget(&TargetPos,1.f);
		else
			SetHeroTarget(&TargetPos,FALSE);
	}
}


BYTE CMoveManager::GetMoveMode(CObject* pObject)
{
	return pObject->m_MoveInfo.MoveMode;
}

BOOL CMoveManager::IsMoving(CObject* pObject)
{
	return pObject->m_MoveInfo.bMoving;
}



void CMoveManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_MOVE_TARGET:
		{
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject)
			{
				CalcPositionEx(pObject,gCurTime);
				pmsg->GetTargetInfo(&pObject->m_MoveInfo);
				VECTOR3 spos;
				pmsg->spos.Decompress(&spos);
				StartMoveEx(pObject, &spos, gCurTime);
			}
			else
			{
#ifdef _GMTOOL_
				++GMTOOLMGR->m_nNullMove;
#endif				
			}
		}
		break;
	case MP_MOVE_ONETARGET:
		{
			MOVE_ONETARGETPOS_FROMSERVER* pmsg = (MOVE_ONETARGETPOS_FROMSERVER*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(pObject)
			{
				CalcPositionEx(pObject,gCurTime);
				pmsg->GetTargetInfo(&pObject->m_MoveInfo);
				VECTOR3 spos;
				pmsg->GetStartPos(&spos);
				StartMoveEx(pObject, &spos, gCurTime);
			}
			else
			{
#ifdef _GMTOOL_
			++GMTOOLMGR->m_nNullMove;
#endif
			}
		}
		break;
	case MP_MOVE_PET_STOP:
	case MP_MOVE_STOP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject)
			{
				VECTOR3 pos;
				pmsg->cpos.Decompress(&pos);
				CMoveManager::MoveStop(pObject,&pos);
			}
			else
			{
			}
		}
		break;


	case MP_MOVE_WALKMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if( pPlayer )
				SetWalkMode(pPlayer);

			PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Walk);
		}
		break;
	
	case MP_MOVE_RUNMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			if( pPlayer )
				SetRunMode(pPlayer);

			PICONMGR->SetMoveState(pmsg->dwObjectID, eMoveMode_Run);
		}
		break;
	case MP_MOVE_MONSTERMOVE_NOTIFY:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CMonster* pMob = (CMonster*)OBJECTMGR->GetObject(pmsg->dwObjectID);
			BOOL rt = SetMonsterMoveType(pMob, pmsg->wData);			
		}
		break;
	case MP_MOVE_KYUNGGONG_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			PICONMGR->SetKyungGong(pmsg->dwData1, pmsg->dwData2);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
			if(pPlayer == NULL)
				return;

			BOOL rt = SetKyungGong(pPlayer,(WORD)pmsg->dwData2);
			ASSERT(rt);			
		}
		break;

	case MP_MOVE_KYUNGGONG_NACK:
		{
 			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
 			
 			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
 			if(pPlayer == NULL)
 				return;
 
 			if( pPlayer == HERO )
 			{
 				SetKyungGong(pPlayer,0);
 				HeroMoveStop();
 			}
 
		}
		break;
	case MP_MOVE_EFFECTMOVE:
		{

		}
		break;
	case MP_MOVE_PET_CORRECTION:
	case MP_MOVE_CORRECTION:
		{	
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
			{
				return;
			}

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);

			if(IsMoving(pObject))
				MoveStop(pObject,&pos);
			else
				SetPosition(pObject,&pos);
		}
		break;
	case MP_MOVE_FORCESTOPKYUNGGONG:
		{
			HERO->SetKyungGongMode(FALSE);
		}
		break;
		
	case MP_MOVE_WARP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pObject == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			MoveStop( pObject, &pos );
		}
		break;
	case MP_CHAR_FB_ACK:  //副本地图移出指令解析  by:十里坡剑神 QQ:112582793
		{
			stMOVEPOINT stMovePoint;
			stMovePoint.SetMovePoint(8625, 9825);
			MARRYMGR->SetTargerPos(stMovePoint.value);
			MARRYMGR->SetChannelIdx(0);
			g_UserInput.GetHeroMove()->MapChange( 17, 0, 2019, eMapChange_SOS );
			//MSG_NAME_DWORD2 msg;
			//msg.Category=MP_CHEAT;
			//msg.Protocol = MP_USERFBCONN_MOVEMAP_SYN;
			//msg.dwObjectID = HEROID;
			//msg.dwData1 = 17;
			//msg.dwData2 = 1;
			gChannelNum = 0;
			//msg.Name[0] = 0;
			//NETWORK->Send(&msg,sizeof(msg));
		}
		break;
	case MP_MOVE_PET_WARP_ACK:
		{

			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwMoverID);
			if(pPet == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);


			CalcPositionEx(pPet,gCurTime);

			MoveStop(pPet,&pos);

			TARGETSET set;
			set.pTarget = pPet;
			EFFECTMGR->StartEffectProcess(eEffect_PetWarp,pPet,&set,0,pPet->GetID());

			if( PETMGR->GetCurSummonPet() != pPet )	return;

			if( IsMoving(HERO) )
			{
				WORD count = HERO->m_MoveInfo.GetMaxTargetPosIdx();

				if(count)
					SetPetTarget(&HERO->m_MoveInfo,count);
			}
		}
		break;
	}
}

void CMoveManager::RefreshHeroMove()
{
	if(IsMoving(HERO) == FALSE)
		return;

	VECTOR3 TargetPos = *HERO->m_MoveInfo.GetTargetPosition(HERO->m_MoveInfo.GetCurTargetPosIdx());
	SetHeroTarget(&TargetPos,FALSE);
}




