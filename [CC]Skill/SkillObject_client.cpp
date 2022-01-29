// SkillObject.cpp: implementation of the CSkillObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MHCLIENT_

#include "SkillObject_Client.h"
#include "SkillInfo.h"
#include "ActionTarget.h"
#include "SkillObjectTerminator.h"
#include "SkillObjectTargetList.h"
#include "SkillObjectFirstUnit.h"
#include "SkillObjectStateUnit.h"

#include "ObjectManager.h"
#include "ObjectStateManager.h"

#include "GameIn.h"
#include "BattleSystem_Client.h"
#include "PartyManager.h"

#include "OptionManager.h"
#include "SkillManager_Client.h"
#include "CharacterDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObject::CSkillObject( CSkillInfo* pSkillInfo,
							CSkillObjectTerminator* pTerminator,
							CSkillObjectTargetList* pTargetList,
							CSkillObjectFirstUnit* pFirstUnit)
{
	m_pSkillInfo = pSkillInfo;
	m_pTerminator = pTerminator;
	m_pTargetList = pTargetList;
	m_pTargetList->SetSkillObject(this);
	m_pFirstUnit = pFirstUnit;
	m_PositiveTargetType = pSkillInfo->GetPositiveResultTargetType();
	m_NegativeTargetType = pSkillInfo->GetNegativeResultTargetType();
	m_SkillObjectMoveType = pSkillInfo->GetSkillInfo()->MoveTargetArea;

	m_SkillObjectInfo.SkillObjectIdx = 0;
	
	m_ObjectKind = eObjectKind_SkillObject;

	m_hObjectEff = NULL;

	m_OptionIndex = 0;
}

CSkillObject::~CSkillObject()
{
	Release();

	m_SkillObjectInfo.SkillObjectIdx = 0;
	
	SAFE_DELETE(m_pTerminator);
	SAFE_DELETE(m_pTargetList);

	CSkillObjectSingleUnit* pSingleUnit;
	while(pSingleUnit = (CSkillObjectSingleUnit*)m_SingleUnitList.RemoveTail())
	{
		delete pSingleUnit;
	}

	CSkillObjectStateUnit* pStateUnit;
	while(pStateUnit = (CSkillObjectStateUnit*)m_StateUnitList.RemoveTail())
	{
		delete pStateUnit;
	}
	m_OptionIndex = 0;
}
WORD CSkillObject::GetComboNum()
{
	return m_pSkillInfo->GetSkillInfo()->ComboNum;
}

void CSkillObject::SetOperatorState()
{
	CObject* pObject;
	BOOL bBinding = m_pSkillInfo->GetSkillInfo()->BindOperator != 0;
	DWORD StateEndTime = 0;

	pObject = OBJECTMGR->GetObject(m_SkillObjectInfo.Operator);
	if(pObject == NULL)
		return;
	
	if(pObject->GetState() == eObjectState_Die)
		return;

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;
		if(!bBinding)
		{
			StateEndTime = m_pSkillInfo->GetSkillOperateAnimationTime( 0 );
			if(StateEndTime == 0)
				StateEndTime = 500;
		}
	}
	else
	{
		StateEndTime = EFFECTMGR->GetOperatorAnimatioEndTime(
			GetSkillInfo()->GetSkillInfo()->EffectUse ,eEffectForMan,pObject->GetEngineObject());
		if(StateEndTime == 0)
			StateEndTime = 500;
	}

	if(m_bWasTempObject)
	{
		if(pObject->GetState() == eObjectState_SkillUsing)
		{
			OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_SkillUsing);
		}

		return;
	}
		
	if(bBinding)
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_SkillBinding);
	else if(pObject->GetState() != eObjectState_SkillSyn)
	{
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_SkillSyn);
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_SkillSyn,StateEndTime);
	}
	
}

void CSkillObject::EndOperatorState()
{
	CObject* pObject;
	DWORD StateEndTime = 0;
	BYTE State;
	BOOL bBinding = m_pSkillInfo->GetSkillInfo()->BindOperator != FALSE;

	pObject = OBJECTMGR->GetObject(m_SkillObjectInfo.Operator);
	if(pObject == NULL)
		return;

	State = pObject->GetState();
	if( State == eObjectState_SkillBinding ||
		State == eObjectState_SkillUsing )
		OBJECTSTATEMGR->EndObjectState(pObject,State);

}


void CSkillObject::Init(SKILLOBJECT_INFO* pInfo,CTargetList* pTList)
{
	m_bInited = TRUE;
	m_bDieFlag = FALSE;

	m_bWasTempObject = m_SkillObjectInfo.SkillObjectIdx == TEMP_SKILLOBJECT_ID;

	if(m_bWasTempObject || pInfo->SkillObjectIdx == TEMP_SKILLOBJECT_ID)
		m_IsHeroSkill = TRUE;
	else
		m_IsHeroSkill = FALSE;		

	m_BaseObjectInfo.dwObjectID = pInfo->SkillObjectIdx;
	SafeStrCpy(m_BaseObjectInfo.ObjectName,m_pSkillInfo->GetSkillName(), MAX_NAME_LENGTH+1);
	m_BaseObjectInfo.ObjectState = eObjectState_None;

	memcpy(&m_SkillObjectInfo,pInfo,sizeof(SKILLOBJECT_INFO));

	m_SkillObjectInfo.StartTime = gCurTime - m_SkillObjectInfo.StartTime;
	
	m_pTerminator->Init(m_pSkillInfo,pInfo->SkillLevel);
	m_pTargetList->InitTargetList(this,pTList,NULL,&m_SkillObjectInfo.MainTarget);

	m_pOperator = OBJECTMGR->GetObject(pInfo->Operator);

	if(pInfo->SkillObjectIdx != TEMP_SKILLOBJECT_ID)
	{		
		PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
			pStateUnit->Init(m_pSkillInfo,pInfo->SkillLevel);
		PTRLISTSEARCHEND;
		
		PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
			pStateUnit->StartState(m_pTargetList);
		PTRLISTSEARCHEND;
	}
	
	if(m_bWasTempObject)
	{
		m_pFirstUnit->SetTargetArray(m_pOperator,pTList,&m_SkillObjectInfo.MainTarget);
	}
	else
	{
		if(m_pOperator == NULL)
			return;

		if(m_SkillObjectInfo.SkillObjectIdx == TEMP_SKILLOBJECT_ID)
			m_pFirstUnit->StartFirstEffect(m_pOperator,NULL,&m_SkillObjectInfo.MainTarget);
		else
			m_pFirstUnit->StartFirstEffect(m_pOperator,pTList,&m_SkillObjectInfo.MainTarget);
	}
	
	StartObjectEffect(pTList,&m_SkillObjectInfo.MainTarget);
	SetOperatorState();

	if(m_pOperator->GetObjectKind() == eObjectKind_Player)
	{
		m_OptionIndex = pInfo->Option;
		SKILLOPTION* pSkillOption = SKILLMGR->GetSkillOption(m_OptionIndex);
		SKILLSTATSOPTION* pSkillStatsOption = ((CPlayer*)m_pOperator)->GetSkillStatsOption();
 
		if(pSkillOption)
		{
			m_pTargetList->SetPositionHead();
			CObject* pTarget = NULL;

			while(1)
			{
				pTarget = m_pTargetList->GetNextTarget();

				if(!pTarget)
					break;

				if(pTarget->GetObjectKind() != eObjectKind_Player)
					continue;

				((CPlayer*)pTarget)->AddSkillStatsOption(pSkillOption);

				if(pTarget == HERO)
					GAMEIN->GetCharacterDialog()->UpdateData();
			}
		}
	}
}

void CSkillObject::StartObjectEffect(CTargetList* pTList,MAINTARGET* pMainTarget)
{
	m_hObjectEff = NULL;
	
	if(GetID() != TEMP_SKILLOBJECT_ID)
	{
		WORD EffNum = m_pSkillInfo->GetSkillInfo()->EffectMapObjectCreate;
		if(EffNum != 0)
		{
			DWORD EffectFlag = 0;
			if(m_pOperator->GetID() == HEROID)
				EffectFlag |= EFFECT_FLAG_HEROATTACK;
			if(m_pSkillInfo->GetSkillInfo()->FatalDamage)
				EffectFlag |= EFFECT_FLAG_FATALDAMAGE;
			if(m_pSkillInfo->GetSkillInfo()->Life)
				EffectFlag |= EFFECT_FLAG_LINKTOOBJECT;
			
			m_hObjectEff = EFFECTMGR->StartEffectProcessWithTargetList(EffNum,this,pTList,pMainTarget,EffectFlag);
		}
	}
}
void CSkillObject::EndObjectEffect()
{
	if(m_hObjectEff)
		EFFECTMGR->ForcedEndEffect(m_hObjectEff);
	m_hObjectEff = NULL;	
}

void CSkillObject::Release()
{

}

void CSkillObject::ReleaseSkillObject()
{
	if(!m_pOperator) return;
	if(m_pOperator->GetObjectKind() == eObjectKind_Player)
	{
		SKILLOPTION* pSkillOption = SKILLMGR->GetSkillOption(m_OptionIndex);
		SKILLSTATSOPTION* pSkillStatsOption = ((CPlayer*)m_pOperator)->GetSkillStatsOption();

		if(pSkillOption)
		{
			m_pTargetList->SetPositionHead();
			CObject* pTarget = NULL;

			while(1)
			{
				pTarget = m_pTargetList->GetNextTarget();

				if(!pTarget)
					break;

				if(pTarget->GetObjectKind() != eObjectKind_Player)
					continue;

				((CPlayer*)pTarget)->RemoveSkillStatsOption(pSkillOption);

				if(pTarget == HERO)
					GAMEIN->GetCharacterDialog()->UpdateData();
			}

			m_OptionIndex = 0;
		}
	}

	EndObjectEffect();

	if(m_SkillObjectInfo.SkillObjectIdx != TEMP_SKILLOBJECT_ID)
	{
		PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
			pStateUnit->EndState(m_pTargetList);
		PTRLISTSEARCHEND;
	}

	EndOperatorState();

	m_pFirstUnit->Release();
	m_pTargetList->Release(this);
	
	PTRLISTPOS pos = m_SingleUnitList.GetHeadPosition();

	while( pos )
	{
		CSkillObjectSingleUnit* pSingleUnit = (CSkillObjectSingleUnit*)m_SingleUnitList.GetNext(pos);
		if( pSingleUnit )
			pSingleUnit->Release();
	}

	m_SkillObjectInfo.SkillObjectIdx = 0;
    m_pOperator = NULL;
	m_bInited = FALSE;
}

void CSkillObject::AddSingleUnit(CSkillObjectSingleUnit* pSingleUnit)
{
	m_SingleUnitList.AddTail(pSingleUnit);
	pSingleUnit->SetSkillObject(this);
}

void CSkillObject::AddStateUnit(CSkillObjectStateUnit* pStateUnit)
{
	m_StateUnitList.AddTail(pStateUnit);
	pStateUnit->SetSkillObject(this);
}

DWORD CSkillObject::Update()
{	
	if(m_SkillObjectMoveType != 0)
	{
		if(m_SkillObjectMoveType == 1)
		{
			
		}
		else if(m_SkillObjectMoveType == 2)
		{
			VECTOR3 pos;
			m_pOperator->GetPosition(&pos);
			SetPosition(&pos);
		}
		else
		{
			
		}		
	}

	if(m_IsHeroSkill)	
	{
		OBJECTMGR->UpdateTargetList_Monster(this);
	}
	
	if(m_pTerminator->CheckTerminate(&m_SkillObjectInfo) == TRUE)
	{
		return SO_DESTROYOBJECT;
	}

	return SO_OK;
}

void CSkillObject::UpdateTargetList(CObject* pObject)
{
	if(GetID() == TEMP_SKILLOBJECT_ID)
		return;
	DWORD val = m_pTargetList->UpdateTargetList(pObject);
	if(val == SOTL_NOTCHANGED)
		return;

	MSG_DWORD2 msg;
	msg.Category = MP_SKILL;
	if(val == SOTL_ADDED)
	{
		msg.Protocol = MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_SYN;
	}
	else if(val == SOTL_REMOVED)
	{
		msg.Protocol = MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_SYN;
	}
	else
	{
		return;
	}
	msg.dwObjectID = HEROID;
	msg.dwData1 = GetID();		
	msg.dwData2 = pObject->GetID();
	NETWORK->Send(&msg,sizeof(msg));
}

void CSkillObject::RemoveTarget(CObject* pObject,BYTE bTargetKind)
{
	if(GetID() == TEMP_SKILLOBJECT_ID)
		return;

	if((m_pTargetList->GetTargetKind(pObject->GetID()) & bTargetKind) == FALSE)
		return;
	
	MSG_DWORD2 msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_SYN;
	msg.dwObjectID = HEROID;	
	msg.dwData1 = GetID();	
	msg.dwData2 = pObject->GetID();
	NETWORK->Send(&msg,sizeof(msg));
}

void CSkillObject::AddTargetObject(CObject* pObject,BYTE bTargetKind)
{
	if(m_pTargetList->IsInTargetList(pObject) == TRUE)
		return;

	m_pTargetList->AddTargetObject(pObject,bTargetKind);

	PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
		pStateUnit->StartState(pObject,bTargetKind);
	PTRLISTSEARCHEND;
}
void CSkillObject::RemoveTargetObject(DWORD ObjectID)
{
	if(m_pTargetList->IsInTargetList(ObjectID) == FALSE)
		return;

	BYTE bTargetKind = m_pTargetList->RemoveTargetObject(ObjectID);
	
	CObject* pObject = OBJECTMGR->GetObject(ObjectID);
	if(pObject)
	{
		PTRLISTSEARCHSTART(m_StateUnitList,CSkillObjectStateUnit*,pStateUnit)
			pStateUnit->EndState(pObject,bTargetKind);
		PTRLISTSEARCHEND;
	}
}

void CSkillObject::OnReceiveSkillSingleResult(MSG_SKILL_SINGLE_RESULT* pmsg)
{	
	CObject* pOperator = m_pOperator;
	if(pOperator == NULL)
		return;
	if(pOperator->IsInited() == FALSE)
		return;
	
	DWORD counter = 0;
	CSkillObjectSingleUnit* pSUnit = 0;
	PTRLISTSEARCHSTART(m_SingleUnitList,CSkillObjectSingleUnit*,pSingleUnit)
		if(counter == pmsg->SingleUnitNum)
		{
			pSUnit = pSingleUnit;
			break;
		}
		++counter;
	PTRLISTSEARCHEND;
	
	ASSERT(pSUnit);
	if(pSUnit == NULL)
		return;

	pSUnit->StartSingleUnitEffect(pOperator,&pmsg->TargetList);
}


void CSkillObject::SetPosition(VECTOR3* pPos)
{
	m_SkillObjectInfo.Pos = *pPos;
	m_pTargetList->SetSkillObjectPosition(pPos);
}
void CSkillObject::GetPosition(VECTOR3* pPos)
{
	*pPos = m_SkillObjectInfo.Pos;
}
void CSkillObject::SetAngle(float AngleRad)
{
	//ASSERT(0);
}
float CSkillObject::GetAngle()
{
	return DIRTORAD(m_SkillObjectInfo.Direction);
}

DIRINDEX CSkillObject::GetDirectionIndex()
{
	return m_SkillObjectInfo.Direction;
}


BOOL CSkillObject::IsNegativeTarget(CObject* pObject)
{
	return TargetTypeCheck(m_NegativeTargetType,pObject);
}

BOOL CSkillObject::IsPositiveTarget(CObject* pObject)
{
	return TargetTypeCheck(m_PositiveTargetType,pObject);
}

BOOL CSkillObject::TargetTypeCheck(WORD TargetType,CObject* pObject)
{
	switch(TargetType)
	{
	case SKILLTARGETTYPE_ENEMY:
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(m_pOperator->GetBattleID());	
			return pBattle->IsEnemy(m_pOperator,pObject);
		}
		break;
	case SKILLTARGETTYPE_TEAMMATE:
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(m_pOperator->GetBattleID());
			return pBattle->IsFriend(m_pOperator,pObject);
		}
		break;
	case SKILLTARGETTYPE_SELF:
		{
			if(pObject != m_pOperator)
				return FALSE;
		}
		break;
	case SKILLTARGETTYPE_ENEMYANDTEAM:
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(m_pOperator->GetBattleID());	
			if(pBattle->IsEnemy(m_pOperator,pObject))
				return TRUE;
			if(pBattle->IsFriend(m_pOperator,pObject))
				return TRUE;

			return FALSE;
		}
		break;
	case SKILLTARGETTYPE_NOTENEMYTARGET:
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(m_pOperator->GetBattleID());	
			if(pBattle->IsEnemy(m_pOperator,pObject))
				return FALSE;

			return TRUE;
		}
		break;
	}
	return TRUE;
}

void CSkillObject::SetMissed()
{
	CTargetList tlist;
	CTargetListIterator iter(&tlist);
	
	m_pTargetList->SetPositionHead();
	CObject* pObject;
	while(pObject = m_pTargetList->GetNextTarget())
	{
		iter.AddTargetWithNoData(pObject->GetID(),SKILLRESULTKIND_NONE);
	}
	iter.Release();
	
	m_pFirstUnit->SetTargetArray(m_pOperator,&tlist,&m_SkillObjectInfo.MainTarget);	
}

void CSkillObject::SetMotionInState(BYTE State)
{
	m_ObjectState.State_Start_Motion = -1;
	m_ObjectState.State_Ing_Motion = -1;
	m_ObjectState.State_End_Motion = -1;
}



void CSkillObject::Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage)
{
	if( pAttacker )
	{
		if( OPTIONMGR->IsShowMemberDamage() )
		{
			if( pAttacker == HERO || PARTYMGR->IsPartyMember( pAttacker->GetID() ) )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}
		else
		{
			if( pAttacker == HERO )
				EFFECTMGR->AddDamageNumber(Damage,pAttacker,this,DamageKind,bCritical,bDecisive);
		}

	}
		
}

#endif
