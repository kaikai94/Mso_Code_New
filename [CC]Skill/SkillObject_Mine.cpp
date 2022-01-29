// SkillObject_Mine.cpp: implementation of the CSkillObject_Mine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObject_Mine.h"

#ifdef _MHCLIENT_
#include "ObjectManager.h"
#include "EFFECT/EffectManager.h"
#include "SkillObjectFirstUnit.h"
#include "ObjectActionManager.h"
#else
#include "UserTable.h"
#include "SkillManager_server.h"
#include "AttackManager.h"
#include "PackedData.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObject_Mine::CSkillObject_Mine(CSkillInfo* pSkillInfo,
				CSkillObjectTerminator* pTerminator,
				CSkillObjectTargetList* pTargetList,
				CSkillObjectFirstUnit* pFirstUnit)
:	CSkillObject(pSkillInfo,pTerminator,pTargetList,pFirstUnit)
{

}

CSkillObject_Mine::~CSkillObject_Mine()
{

}

void CSkillObject_Mine::Init(SKILLOBJECT_INFO* pInfo,CTargetList* pTList)
{
	CSkillObject::Init(pInfo,pTList);
	m_bOperate = FALSE;
}

void CSkillObject_Mine::UpdateTargetList(CObject* pObject)
{
	CSkillObject::UpdateTargetList(pObject);

#ifdef _MHCLIENT_
	CheckOperate(pObject);
#endif
}

DWORD CSkillObject_Mine::Update()
{
#ifdef _MAPSERVER_
	if(m_pOperator->GetObjectKind() & eObjectKind_Monster)
	{
		m_pTargetList->SetPositionHead();
		CObject* pObject;
		BYTE bTargetKind;
		while(m_pTargetList->GetNextNegativeTarget(&pObject,&bTargetKind))
		{
			if(CheckOperate(pObject) == TRUE)
				break;
		}
	}
#endif

	// ����� �׽�Ʈ ������ ���� �ڼ��� ���� �����Ұ���. LBS
	DWORD rt = CSkillObject::Update();

	if( rt == SO_DESTROYOBJECT )
	{
		VECTOR3 pos;
		GetPosition(&pos);
		m_bOperate = TRUE;

#ifdef _MAPSERVER_
		MAINTARGET mt(&pos);
		CTargetList tlist;
		CTargetListIterator iter(&tlist);

		m_pTargetList->SetPositionHead();
		STLIST* pList;
		while(pList = m_pTargetList->GetNextTargetList())
		{
			iter.AddTargetWithNoData(pList->pObject->GetID(),pList->bTargetKind);
		}
		iter.Release();
		Operate(NULL,&mt,&tlist);
#else
		ObjectArray<LPOBJECT> objarray;
		OBJECTMGR->GetTargetInRange(&pos,GetSkillInfo()->GetSkillRange(),&objarray,FALSE);

		LPOBJECT pObj;
		MAINTARGET mt(&pos);
		MSG_SKILL_OPERATE msg;
		msg.InitMsg(MP_SKILL_OPERATE_SYN,&mt);
		msg.RequestorID = HEROID;
		msg.SkillObjectID = GetID();
		CTargetListIterator iter(&msg.TargetList);

		objarray.SetPositionHead();
		while(pObj = objarray.GetNextObject())
		{
			iter.AddTargetWithNoData(pObj->GetID(),0);
		}
		iter.Release();

		NETWORK->Send(&msg,msg.GetMsgLength());	
#endif
	}

	return rt;
}


BOOL CSkillObject_Mine::Operate(CObject* pRequestor,MAINTARGET* pMainTarget,CTargetList* pTList)
{	
#ifdef _MAPSERVER_	
	MSG_SKILL_OPERATE msg;
	msg.InitMsg(MP_SKILL_OPERATE_ACK,pMainTarget);
	msg.SkillObjectID = GetID();
	msg.RequestorID = pRequestor ? pRequestor->GetID() : 0;

	CTargetListIterator riter(pTList);			
	CTargetListIterator siter(&msg.TargetList);	

	CObject* pTarget;

	int SkillLevel = m_SkillObjectInfo.SkillLevel;
	float phy_rate = m_pSkillInfo->GetFirstPhyAttack(SkillLevel);
	WORD att_min = m_pSkillInfo->GetFirstAttAttackMin(SkillLevel);
	WORD att_max = m_pSkillInfo->GetFirstAttAttackMax(SkillLevel);
	WORD Attrib = m_pSkillInfo->GetAttrib();
	float att_rate = m_pSkillInfo->GetFirstAttAttack(SkillLevel);

	if(m_SkillObjectInfo.Option)
	{
		SKILLOPTION* pSkillOption = SKILLMGR->GetSkillOption(m_SkillObjectInfo.Option);

		if(pSkillOption)
		{
			if(pSkillOption->PhyAtk)
				phy_rate = phy_rate * (1.0f + pSkillOption->PhyAtk);
			if(pSkillOption->AttAtk)
				att_rate = att_rate * (1.0f + pSkillOption->AttAtk);
		}
	}
	//////////////////////////////////////////////////////////////////////////

	RESULTINFO dinfo;
	while(riter.GetNextTarget())
	{
		pTarget = g_pUserTable->FindUser(riter.GetTargetID());
		if(pTarget == NULL)
			continue;

		if(IsNegativeTarget(pTarget) == FALSE)
			continue;

		ATTACKMGR->Attack(TRUE, m_pOperator,pTarget,0,phy_rate,
						Attrib,att_min,att_max,att_rate,
						0,&dinfo,FALSE,1.0f,0);

		siter.AddTargetWithResultInfo(riter.GetTargetID(),SKILLRESULTKIND_NEGATIVE,&dinfo);
	}
	siter.Release();
	riter.Release();
	
	PACKEDDATA_OBJ->QuickSend(this,&msg,msg.GetMsgLength());

	DoDie(NULL);

	return TRUE;
#else
	MSG_SKILL_OPERATE msg;
	msg.InitMsg(MP_SKILL_OPERATE_ACK,pMainTarget);

	
	DWORD flag = 0;
	if(m_pOperator->GetID() == HEROID)
		flag |= EFFECT_FLAG_HEROATTACK;
	if(m_pSkillInfo->GetSkillInfo()->EffectMineOperate)
		EFFECTMGR->StartEffectProcessWithTargetList(m_pSkillInfo->GetSkillInfo()->EffectMineOperate,this,pTList,pMainTarget,flag);	
	else
	{
		CObject* pTarget;
		RESULTINFO rinfo;
		CTargetListIterator iter(pTList);
		while(iter.GetNextTarget())
		{
			pTarget = OBJECTMGR->GetObject(iter.GetTargetID());
			iter.GetTargetData(&rinfo);
			if(pTarget == NULL)
				continue;
			OBJECTACTIONMGR->ApplyResult(pTarget,GetOperator(),&rinfo,eDamageKind_Front);
		}
		iter.Release();		
	}


	return TRUE;
#endif
}

BOOL CSkillObject_Mine::CheckOperate(CObject* pObject)
{
	if(m_bOperate == TRUE)	
		return TRUE;
	if(gCurTime - m_SkillObjectInfo.StartTime < m_pSkillInfo->GetSkillInfo()->MineCheckStartTime)
		return TRUE;

	VECTOR3 pos1,pos2;
	pObject->GetPosition(&pos1);
	GetPosition(&pos2);
	pos1.y = pos2.y = 0;
	
	float dist = CalcDistanceXZ(&pos1,&pos2);
	if(dist <= m_pSkillInfo->GetSkillInfo()->MineCheckRange)
	{
		m_bOperate = TRUE;

#ifdef _MAPSERVER_
		MAINTARGET mt(&pos2);
		CTargetList tlist;
		CTargetListIterator iter(&tlist);

		m_pTargetList->SetPositionHead();
		STLIST* pList;
		while(pList = m_pTargetList->GetNextTargetList())
		{
			iter.AddTargetWithNoData(pList->pObject->GetID(),pList->bTargetKind);
		}
		iter.Release();
		Operate(NULL,&mt,&tlist);
#else
		ObjectArray<LPOBJECT> objarray;
		OBJECTMGR->GetTargetInRange(&pos2,GetSkillInfo()->GetSkillRange(),&objarray,FALSE);

		LPOBJECT pObj;
		MAINTARGET mt(&pos2);
		MSG_SKILL_OPERATE msg;
		msg.InitMsg(MP_SKILL_OPERATE_SYN,&mt);
		msg.RequestorID = HEROID;
		msg.SkillObjectID = GetID();
		CTargetListIterator iter(&msg.TargetList);

		objarray.SetPositionHead();
		while(pObj = objarray.GetNextObject())
		{
			iter.AddTargetWithNoData(pObj->GetID(),0);
		}
		iter.Release();

		NETWORK->Send(&msg,msg.GetMsgLength());	
#endif
		return TRUE;
	}
	return FALSE;
}
