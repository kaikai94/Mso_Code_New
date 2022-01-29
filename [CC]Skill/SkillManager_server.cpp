// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MAPSERVER_

#include "SkillManager_server.h"

#include "UserTable.h"
#include "CharMove.h"

#include "Player.h"
#include "Monster.h"
#include "Pet.h"
#include "PackedData.h"

#include "BattleSystem_Server.h"
#include "MapDBMsgParser.h"
#include "MugongManager.h"
#include "ItemManager.h"

#include "ObjectStateManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

enum eSkillReleaseKind
{
	eSkillReleaseKind_RELEASE,
	eSkillReleaseKind_Process,
	eSkillReleaseKind_BATTLE,
};
CSkillManager::CSkillManager()
{
	g_PoolSTLIST.Init(1000,500,"g_PoolSTLIST");
	m_ICHandle = NULL;
	m_HwaKyungSkillTable.Initialize(30);
	m_GeukMaSkillTable.Initialize(30);
	m_SkillOptionTable.Initialize(30);
	m_SkillOptionByItemTable.Initialize(30);
	m_SpecialStateInfoTable.Initialize(3);
}

CSkillManager::~CSkillManager()
{
	Release();
	g_PoolSTLIST.Release();
}


void CSkillManager::Init()
{
	m_SkillInfoTable.Initialize(2000);
	m_SkillObjectTable.Initialize(1000);
	LoadSkillInfoList();
	LoadSkillChangeInfoList();

	m_SkillAreaMgr.LoadSkillAreaList();
	
	m_ICHandle = ICCreate();
	ICInitialize(m_ICHandle,SKILLOBJECT_ID_MAX);

	m_JobSkillProbabilityTable.Initialize(MAX_JOBLEVEL_NUM);
	LoadJobSkillProbability();
}
void CSkillManager::Release()
{
	CSkillObject* pObject;
	m_SkillObjectTable.SetPositionHead();
	while(pObject = m_SkillObjectTable.GetData())
	{
		ReleaseSkillObject(pObject,eSkillReleaseKind_RELEASE);
	}
	m_SkillObjectTable.RemoveAll();

	CSkillInfo* pSInfo = NULL;
	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();

	m_SkillAreaMgr.Release();
	
	if(m_ICHandle)
	{
		ICRelease(m_ICHandle);
		m_ICHandle = NULL;
	}
	
	PTRLISTSEARCHSTART(m_SkillChangeList,SKILL_CHANGE_INFO*,pInfo)
		delete pInfo;
	PTRLISTSEARCHEND
	m_SkillChangeList.RemoveAll();

	m_HwaKyungSkillTable.RemoveAll();
	m_GeukMaSkillTable.RemoveAll();
	SKILLOPTION* pSOpt = NULL;
	m_SkillOptionTable.SetPositionHead();
	while(pSOpt = m_SkillOptionTable.GetData())
	{
		delete pSOpt;
	}
	m_SkillOptionTable.RemoveAll();
	m_SkillOptionByItemTable.RemoveAll();

	SPECIAL_STATE_INFO* pStateInfo = NULL;

	m_SpecialStateInfoTable.SetPositionHead();
	while(pStateInfo = m_SpecialStateInfoTable.GetData())
	{
		delete pStateInfo;
	}
	m_SpecialStateInfoTable.RemoveAll();

	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo = NULL;

	m_JobSkillProbabilityTable.SetPositionHead();
	while(pJobSkillInfo = m_JobSkillProbabilityTable.GetData())
	{
		SAFE_DELETE(pJobSkillInfo);		
	}
	m_JobSkillProbabilityTable.RemoveAll();
}


void CSkillManager::LoadSkillInfoList()
{
	CMHFile file;
	file.Init("Resource/SkillList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		CSkillInfo* pInfo = new CSkillInfo;
		pInfo->InitSkillInfo(&file);

		m_SkillInfoTable.Add(pInfo,pInfo->GetSkillIndex());
	}
	file.Release();
	
	
	file.Init("Resource/SAT.bin","rb");
	if(file.IsInited() == FALSE)
	{
		MessageBox(0,"SAT.bin is not found",0,0);
		return;
	}
	char buff[512]={0,};
	int count = file.GetDword();

	for(int n=0;n<count;++n)
	{
		WORD skillIdx = file.GetWord();
		file.GetString(buff);
		DWORD aniTimeMale = file.GetDword();
		DWORD aniTimeFemale = file.GetDword();

		CSkillInfo* pSkillInfo = GetSkillInfo(skillIdx);
		if(pSkillInfo == NULL)
			continue;

		pSkillInfo->SetSkillOperateAnimationTime(aniTimeMale,aniTimeFemale);
	}
	file.Release();

	LoadSkillTreeList();

	LoadJobSkillList();

	LoadSkillOptionList();

	LoadStateList();
}

void CSkillManager::LoadSkillChangeInfoList()
{
	CMHFile file;
	file.Init("Resource/SkillChangeList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		SKILL_CHANGE_INFO * pInfo = new SKILL_CHANGE_INFO;
		pInfo->wMugongIdx = file.GetWord();
		pInfo->wChangeRate = file.GetWord();
		pInfo->wTargetMugongIdx = file.GetWord();

		m_SkillChangeList.AddTail(pInfo);
	}
}

void CSkillManager::LoadSkillTreeList()
{
	CMHFile file;
	file.Init("Resource/SkillTree.bin","rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		WORD Array[5];

		file.GetString();
		file.GetWord(Array,5);
		
		for(int n=0;n<5;++n)
		{
			WORD Before = 0,After = 0;
			WORD Cur = Array[n];
			if(n != 0) Before = Array[n-1];
			if(n != 4) After = Array[n+1];
			
			CSkillInfo* pInfo = GetSkillInfo(Cur);
			if(pInfo == NULL)
				continue;

			pInfo->SetSkillTree(Before,After,Array);
		}
	}
	file.Release();
}


void CSkillManager::LoadJobSkillList()
{
	CMHFile file;
	file.Init("Resource/JobSkillList.bin","rb");

	if(file.IsInited() == FALSE)
	{
		return;
	}


	char buf[32];
	int Count = 0;
	DWORD SkillIdx = 0;

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		file.GetString(buf);

		if( strcmp( buf, "#HWAKUNG" ) == 0 )
		{
			Count = file.GetInt();

			for(int i=0; i<Count; ++i)
			{
				SkillIdx = file.GetDword();
				CSkillInfo* pInfo = m_SkillInfoTable.GetData( SkillIdx );
				if( !pInfo )		continue;

				m_HwaKyungSkillTable.Add( pInfo, SkillIdx );
			}
		}
		if( strcmp( buf, "#GEUKMA" ) == 0 )
		{
			Count = file.GetInt();

			for(int i=0; i<Count; ++i)
			{
				SkillIdx = file.GetDword();
				CSkillInfo* pInfo = m_SkillInfoTable.GetData( SkillIdx );
				if( !pInfo )		continue;

				m_GeukMaSkillTable.Add( pInfo, SkillIdx );
			}
		}
	}

	file.Release();
}

void CSkillManager::LoadSkillOptionList()
{
	CMHFile file;
	file.Init("Resource/SkillOptionList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SKILLOPTION* pSOpt = new SKILLOPTION;

		memset(pSOpt, 0, sizeof(SKILLOPTION));

		pSOpt->Index		= file.GetWord();
		pSOpt->SkillKind	= file.GetWord();
		pSOpt->OptionKind	= file.GetWord();
		pSOpt->OptionGrade	= file.GetWord();	
		pSOpt->ItemIndex	= file.GetWord();

		for(int i = 0; i < MAX_SKILLOPTION_COUNT; i++)
		{
			WORD Kind = file.GetWord();
			
			switch(Kind)
			{
			case eSkillOption_Range:
				pSOpt->Range = file.GetInt();
				break;
			
			case eSkillOption_ReduceNaeRyuk:
				pSOpt->ReduceNaeRyuk = file.GetFloat();
				break;
			
			case eSkillOption_PhyAtk:
				pSOpt->PhyAtk = file.GetFloat();
				break;

			case eSkillOption_BaseAtk:
				pSOpt->BaseAtk = file.GetFloat();
				break;
			
			case eSkillOption_AttAtk:
				pSOpt->AttAtk = file.GetFloat();
				break;

			case eSkillOption_Life:
				pSOpt->Life = file.GetInt();
				break;

			case eSkillOption_NaeRyuk:
				pSOpt->NaeRyuk = file.GetInt();
				break;

			case eSkillOption_Shield:
				pSOpt->Shield = file.GetInt();
				break;

			case eSkillOption_PhyDef:
				pSOpt->PhyDef = file.GetFloat();
				break;

			case eSkillOption_AttDef:
				pSOpt->AttDef = file.GetFloat();
				break;

			case eSkillOption_Duration:
				pSOpt->Duration = file.GetDword();
				break;

			case eSkillOption_None:
			default:
				file.GetWord();
				break;
			}
		}

		m_SkillOptionTable.Add(pSOpt, pSOpt->Index);
		m_SkillOptionByItemTable.Add(pSOpt, pSOpt->ItemIndex);
	}
}

void CSkillManager::LoadStateList()
{
	CMHFile file;
	file.Init("Resource/StateInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;

		SPECIAL_STATE_INFO* pInfo = new SPECIAL_STATE_INFO;

		memset(pInfo, 0, sizeof(SPECIAL_STATE_INFO));

		pInfo->Index = file.GetWord();
		pInfo->IconIdx = file.GetWord();
		pInfo->TickTime = file.GetDword();
		pInfo->NaeRyuk = file.GetWord();
		pInfo->DelayTime = file.GetDword();
		pInfo->DurationTime = file.GetDword();

		m_SpecialStateInfoTable.Add(pInfo, pInfo->Index);
	}
}

DWORD CSkillManager::GetNewSkillObjectID()
{
	static int tempID = SKILLOBJECT_ID_START;
	if(++tempID >= SKILLOBJECT_ID_MAX)
		tempID = SKILLOBJECT_ID_START;
	return tempID;

}
void CSkillManager::ReleaseSkillObjectID(DWORD SkillObjectID)
{
	return;	
}

void CSkillManager::OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg)
{
	CSkillObject* pSObj;
	SKILLOBJECT_INFO info;
	VECTOR3 TargetPos;
	float TargetRadius;
	int SkillLevel = 1;
	CBattle* pBattle;
	CObject * pTargetObject = NULL;
	float skillTreeAmp;
	BYTE Err = 0;

	WORD SkillOptionIndex = 0;
	SKILLOPTION* pSkillOption = NULL;
	
	CObject* pOperator = g_pUserTable->FindUser(pmsg->Operator);

	if(pmsg->Operator > 1000000)
		return;

	if(pOperator == NULL)
		return;

	CSkillInfo* pSkillInfo = GetSkillInfo(pmsg->SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
	{
		Err = eErr_SkillInfoNULL;
		goto SkillStartNack;
	}

	
	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pOperator;
		if (pSkillInfo->IsCheckSkill())
		{
			DWORD dwLSAT = 500;//[普通技能间隔限制][BY:十里坡剑神][QQ:112582793][2019-5-31][16:49:45]

			if (pSkillInfo->GetSkillKind() == 2)
				dwLSAT = 400;//[法师技能间隔限制][BY:十里坡剑神][QQ:112582793][2019-5-31][16:50:00]

			BOOL bHack = FALSE;		
			//g_Console.LOGEX(4, RGB(0, 255, 0), "检查间隔时间[%d]", (gCurTime - pPlayer->GetSkillDelayStartTime()) - dwLSAT);
			if (gCurTime - pPlayer->GetSkillDelayStartTime() < dwLSAT)
			{// [检查技能使用间隔时间] [by:十里坡剑神][QQ:112582793][2018/11/24]
				bHack=pPlayer->SpeedHackCheck((gCurTime - pPlayer->GetSkillDelayStartTime()) - dwLSAT);
				
				bHack = TRUE;
			}
			pPlayer->SetSkillDelayStartTime(gCurTime);

			if (bHack)
			{
				Err = eErr_NotExcutableSkillState;
				goto SkillStartNack;
			}

		}
	
		SkillLevel = pPlayer->GetMugongLevel(pmsg->SkillIdx);
		if(SkillLevel == 0 || SkillLevel == -1)
		{
			Err = eErr_SkillLvl0;
			goto SkillStartNack;
		}

		SkillOptionIndex = pPlayer->GetSkillOptionIndex(pmsg->SkillIdx);

		if(SkillOptionIndex)
			pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);

		if( pPlayer->GetStage() & eStage_Hwa )
		{// [玄境技能检查] [by:十里坡剑神][QQ:112582793][2018/11/24]
			if( m_GeukMaSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
			{
				Err = eErr_NotExcutableSkillState;
				goto SkillStartNack;
			}
		}
		else if( pPlayer->GetStage() & eStage_Geuk )
		{// [脱魔技能检查] [by:十里坡剑神][QQ:112582793][2018/11/24]
			if( m_HwaKyungSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
			{
				Err = eErr_NotExcutableSkillState;
				goto SkillStartNack;
			}
		}
		if( pPlayer->GetWeaponEquipType() == WP_EVENT )
		{// [武器类型检查] [by:十里坡剑神][QQ:112582793][2018/11/24]
			if( pmsg->SkillIdx != COMBO_EVENT_MIN )
				goto SkillStartNack;
		}
		else if( pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			if( pmsg->SkillIdx != COMBO_EVENT_HAMMER )
				goto SkillStartNack;
		}

	}	
	// [设置角色状态] [by:十里坡剑神][QQ:112582793][2018/11/24]
	if( pOperator->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pOperator, eObjectState_Immortal, 0 );
	
	// RaMa - BattleCheck
	if( pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pOperator->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{		
		if( pOperator->GetBattleTeam() == 2 )	
			goto SkillStartNack;
	}

	if(pSkillInfo->IsExcutableSkillState(pOperator,SkillLevel, pSkillOption) == FALSE)
	{// [设置角色技能使用状态] [by:十里坡剑神][QQ:112582793][2018/11/24]
		Err = eErr_NotExcutableSkillState;
		goto SkillStartNack;
	}

	if(CheckTitanWeapon(pOperator, pSkillInfo->GetSkillInfo()) == FALSE)
	{
		Err = eErr_NotExcutableSkillState;
		goto SkillStartNack;
	}

	if(GetMainTargetPos(&pmsg->MainTarget,&TargetPos, &pTargetObject) == FALSE)
	{
		Err = eErr_NoTarget;
		goto SkillStartNack;
	}

	if( pSkillInfo->GetSkillInfo()->TargetKind == 0 && pTargetObject == pOperator )
	{
		Err = eErr_NoTarget;
		goto SkillStartNack;
	}
	
	GetMainTargetRadius(&pmsg->MainTarget,&TargetRadius);

	if(pSkillInfo->ComboTest(pOperator) == FALSE)
	{
		Err = eErr_ComboTest;
		goto SkillStartNack;
	}

	CObject* pTarget = g_pUserTable->FindUser(pmsg->MainTarget.dwMainTargetID);
	if(pTarget != NULL)
	{
		if(!IsJobSkill((CPlayer*)pOperator, pTarget, pSkillInfo))
		{
			Err = eErr_NotExcutableSkillState;
			goto SkillStartNack;
		}	
	}	

	if(pSkillInfo->IsInSkillRange(pOperator,&TargetPos,TargetRadius, pSkillOption) == FALSE)
	{
		if(pTargetObject)
		{
			RESULTINFO DamageInfo;
			DamageInfo.Clear();
			pTargetObject->Damage(pOperator, &DamageInfo);
		}
		Err = eErr_NotInRange;
		goto SkillStartNack;
	}
	
	
	if(pSkillInfo->CheckEvade(&pmsg->MainTarget) == TRUE)
	{
		Err = eErr_CheckEvade;
		goto SkillStartNack;
	}
	
	MUGONGBASE* pCurMugong;	
	pCurMugong = ((CPlayer*)pOperator)->GetMugongBaseByMugongIdx(pmsg->SkillIdx);
	if(pCurMugong)
	if( pCurMugong->Sung == 12 )
	{
		DWORD RandRate;
		PTRLISTSEARCHSTART(m_SkillChangeList,SKILL_CHANGE_INFO*,pChangeInfo)
			if(pChangeInfo->wMugongIdx == pmsg->SkillIdx)
			{
//#ifdef _CHINA_LOCAL_
//				RandRate = random(0, 15000);
//#else
				RandRate = random(0, 10000);
//#endif			
				if(0 <= RandRate && RandRate < pChangeInfo->wChangeRate)
				{
					MUGONGMNGR->ChangeMugong((CPlayer*)pOperator, pmsg->SkillIdx, pCurMugong->Position, pChangeInfo->wTargetMugongIdx, eLog_MugongDestroyByGetJulCho);
				}
				break;
			}
		PTRLISTSEARCHEND
	}

	pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = pmsg->SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = pmsg->SkillDir;
	info.Operator = pmsg->Operator;
	info.SkillLevel = SkillLevel;

	info.MainTarget = pmsg->MainTarget;

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(CCharMove::GetPosition(pOperator),&TargetPos);
	info.StartTime = gCurTime;

	// battle
	info.BattleID = pOperator->GetBattleID();
	pBattle = pOperator->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();

	info.Option = SkillOptionIndex;

	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		if(((CPlayer*)pOperator)->IsSkipSkill())
		{
			((CPlayer*)pOperator)->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
				pSkillInfo->GetSkillInfo()->DelayTime - pSkillInfo->GetSkillOperateAnimationTime( 1 ));
		}
		else
		{
			((CPlayer*)pOperator)->GetDelayGroup()->AddDelay(
				CDelayGroup::eDK_Skill,pSkillInfo->GetSkillIndex(),
				pSkillInfo->GetSkillInfo()->DelayTime);
		}
	}
	skillTreeAmp = GetSkillTreeAmp((CPlayer*)pOperator,pSkillInfo);

	pSObj->Init(&info,&pmsg->TargetList,skillTreeAmp,pTargetObject);
	pSObj->SetBattle(pOperator->GetBattleID(),pOperator->GetBattleTeam());
	pSObj->SetGridID(pOperator->GetGridID());

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);

	//SW051129 Pet
	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pOperator;
		CPet* pPet = (CPet*)pPlayer->GetCurPet();
		if( pPet )
		{
			pPet->GetRandMotionNSpeech(ePM_MASTER_SKILL);
		}
	}


	if(pOperator->GetObjectKind() == eObjectKind_Player)
		((CPlayer*)pOperator)->SetActionTime();

	// autonote
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		if( pTargetObject )
        if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
			((CPlayer*)pOperator)->SetLastAttackTimeForAutoNote( gCurTime );		
	}

	return;

SkillStartNack:
	if(pOperator->GetObjectKind() != eObjectKind_Player)
		return;

	// for delete AmplifiedPower
	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);

	CPlayer* pp = (CPlayer*)pOperator;
	MSG_BYTE msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_START_NACK;
	msg.dwObjectID = pp->GetID();
	msg.bData = Err;
	pp->SendMsg(&msg,sizeof(msg));	

	return;
}

void CSkillManager::OnAddObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
		return;

	CObject* pObject = g_pUserTable->FindUser(ObjectID);
	if(pObject == NULL)
		return;

	pSObj->AddTargetObject(pObject);
}

void CSkillManager::OnRemoveObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
		return;

	pSObj->RemoveTargetObject(ObjectID);
}

CSkillObject* CSkillManager::GetSkillObject(DWORD SkillObjID)
{
	return (CSkillObject*)m_SkillObjectTable.GetData(SkillObjID);
}
CSkillInfo* CSkillManager::GetSkillInfo(WORD SkillInfoIdx)
{
	return (CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

void CSkillManager::ReleaseSkillObject(CSkillObject* pSkillObject,int ReleaseKind)
{
	CSkillInfo* pSInfo = GetSkillInfo(pSkillObject->GetSkillIdx());
	if(!pSInfo)
	{
		CSkillObject* ptemp = m_SkillObjectTable.GetData(pSkillObject->GetID());
        if( ptemp == pSkillObject )
		{
			m_SkillObjectTable.Remove(pSkillObject->GetID());
			g_pServerSystem->RemoveSkillObject(pSkillObject->GetID());
            ReleaseSkillObjectID(pSkillObject->GetID());
		}
		return;
	}

	pSkillObject->SkillObjectFirstUnitResult();

	m_SkillObjectTable.Remove(pSkillObject->GetID());
	g_pServerSystem->RemoveSkillObject(pSkillObject->GetID());
	ReleaseSkillObjectID(pSkillObject->GetID());

	pSInfo->ReleaseSkillObject(pSkillObject,ReleaseKind);	
}

void CSkillManager::OnSkillStartEffect(MSG_DWORD2* pmsg)
{
	CObject* pObject = g_pUserTable->FindUser(pmsg->dwData1);
	if(pObject == NULL)
		return;
	
	if( pObject->GetObjectKind() == eObjectKind_Player )
		((CPlayer*)pObject)->SetActionTime();

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,pmsg,sizeof(MSG_DWORD2));
}

void CSkillManager::OnSkillOperateSyn(MSG_SKILL_OPERATE* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
		return;

	CObject* pReqObj = g_pUserTable->FindUser(pmsg->RequestorID);
	if(pReqObj == NULL)
		return;
		
	BOOL rt = pSObj->Operate(pReqObj,&pmsg->MainTarget,&pmsg->TargetList);
	ASSERT(rt);
}

void CSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	switch(Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = (MSG_SKILL_START_SYN*)pMsg;
			OnSkillStartSyn(pmsg);
		}
		break;
	case MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnAddObjectToSkillObjectArea(pmsg->dwData1,pmsg->dwData2);
		}
		break;
	case MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnRemoveObjectToSkillObjectArea(pmsg->dwData1,pmsg->dwData2);
		}
		break;
	case MP_SKILL_STARTEFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnSkillStartEffect(pmsg);			
		}
		break;
	case MP_SKILL_OPERATE_SYN:
		{
			MSG_SKILL_OPERATE* pmsg = (MSG_SKILL_OPERATE*)pMsg;
			OnSkillOperateSyn(pmsg);
		}
		break;
	}
}



void CSkillManager::Process()
{
	CSkillObject* pSObj;
	DWORD rtCode;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		rtCode = pSObj->Update();
		if(rtCode == SO_DESTROYOBJECT)
		{
			ReleaseSkillObject(pSObj,eSkillReleaseKind_Process);
		}		
	}
}

void CSkillManager::GetMainTargetRadius(MAINTARGET* pMainTarget,float* pRtRadius)
{
	if(pMainTarget->MainTargetKind == MAINTARGET::MAINTARGETKIND_POS)
		*pRtRadius = 0;
	else
	{
		CObject* pTarget = g_pUserTable->FindUser(pMainTarget->dwMainTargetID);
		*pRtRadius = pTarget->GetRadius();
	}	
}

BOOL gTestMonsterAttack = TRUE;
void CSkillManager::MonsterAttack(WORD SkillIdx,CObject* pMonster,CObject* pTarget)
{
	if(gTestMonsterAttack == FALSE)
		return;

	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return;
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pMonPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));


	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pMonster->GetID();
	info.MainTarget.MainTargetKind = MAINTARGET::MAINTARGETKIND_OBJECTID;
	info.MainTarget.dwMainTargetID = pTarget->GetID();

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = 1;

	
	// battle
	info.BattleID = pMonster->GetBattleID();
	pBattle = pMonster->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();



	CTargetList TList;
	TList.Clear();
	CTargetListIterator iter(&TList);
	iter.AddTargetWithNoData(pTarget->GetID(),SKILLRESULTKIND_NEGATIVE);
	iter.Release();

	pSObj->Init(&info,&TList);
	pSObj->SetBattle(pMonster->GetBattleID(),pMonster->GetBattleTeam());
	pSObj->SetGridID(pMonster->GetGridID());

	
	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

void CSkillManager::MonsterAttack(WORD SkillIdx,CObject* pMonster,VECTOR3* pTargetPos, DWORD TargetID)
{
	if(gTestMonsterAttack == FALSE)
		return;

	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return;
	
	VECTOR3* pMonPos = CCharMove::GetPosition(pMonster);
	VECTOR3 dir = *pTargetPos - *pMonPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));


	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pMonster->GetID();
	info.MainTarget.MainTargetKind = MAINTARGET::MAINTARGETKIND_OBJECTID;
	info.MainTarget.dwMainTargetID = TargetID;

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = 1;

	
	// battle
	info.BattleID = pMonster->GetBattleID();
	pBattle = pMonster->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();
	
	
	CTargetList tlist;

	WORD Radius = pSkillInfo->GetSkillInfo()->TargetRange;
	WORD AreaNum = pSkillInfo->GetSkillInfo()->TargetAreaIdx;

	WORD SafeRange = pSkillInfo->GetSafeRange();
	/////////////////////////////////////////////////////
	
	if(AreaNum != 0)
	{
		CSkillArea* pSkillArea = GetSkillArea(pMonPos,pTargetPos,pSkillInfo);	
		g_pUserTable->GetTargetInArea(pSkillArea,&tlist);
	}
	else if(Radius != 0)
	{
		g_pUserTable->GetTargetInRange(pSkillInfo->GetTargetAreaPivotPos(pMonPos,pTargetPos),Radius,&tlist,SafeRange);
	}
	
	pSObj->Init(&info,&tlist);
	pSObj->SetBattle(pMonster->GetBattleID(),pMonster->GetBattleTeam());
	pSObj->SetGridID(pMonster->GetGridID());
	
	
	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

void CSkillManager::PetOperSkill(WORD SkillIdx,CPet* pPet,CObject* pTarget)
{
	CBattle* pBattle;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	ASSERT(pSkillInfo);
	if( NULL == pSkillInfo )
		return;

	VECTOR3* pPetPos = CCharMove::GetPosition(pPet);
	VECTOR3* pTargetPos = CCharMove::GetPosition(pTarget);
	VECTOR3 dir = *pTargetPos - *pPetPos;
	DIRINDEX diridx = RADTODIR(VECTORTORAD(dir));

	SKILLOBJECT_INFO info;
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	info.SkillIdx = SkillIdx;
	info.SkillObjectIdx = GetNewSkillObjectID();
	info.Direction = diridx;
	info.Operator = pPet->GetID();
	info.MainTarget.dwMainTargetID = pTarget->GetID();

	info.Pos = *pSkillInfo->GetTargetAreaPivotPos(pPetPos,pTargetPos);
	info.StartTime = gCurTime;
	info.SkillLevel = (BYTE)pPet->GetPetCurGrade();

	// battle
	info.BattleID = pPet->GetBattleID();
	pBattle = pPet->GetBattle();
	info.BattleKind = pBattle->GetBattleKind();

	CTargetList TList;
	TList.Clear();
	CTargetListIterator iter(&TList);
	iter.AddTargetWithNoData(pTarget->GetID(),SKILLRESULTKIND_NEGATIVE);
	iter.Release();

	pSObj->Init(&info,&TList);
	pSObj->SetBattle(pPet->GetBattleID(),pPet->GetBattleTeam());
	pSObj->SetGridID(pPet->GetGridID());

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	g_pServerSystem->AddSkillObject(pSObj,&info.Pos);
}

CSkillArea* CSkillManager::GetSkillArea(VECTOR3* pObjPos, VECTOR3* pTargetPos, CSkillInfo* pSkillInfo)
{
	VECTOR3 DirVec = *pTargetPos - *pObjPos;
	float fRad = VECTORTORAD(DirVec);
	DIRINDEX dir = RADTODIR(fRad);
	CSkillArea* pArea = GetSkillArea(dir,pSkillInfo->GetSkillAreaIndex());
	
	VECTOR3* pPos;
	pPos = pSkillInfo->GetTargetAreaPivotPos(pObjPos,pTargetPos);
	pArea->SetCenterPos(pPos);

	return pArea;	
}

CSkillArea* CSkillManager::GetSkillArea(DIRINDEX dir,WORD SkillAreaIndex)
{
	CSkillArea* pArea = m_SkillAreaMgr.GetSkillArea(SkillAreaIndex,dir);
	
	return pArea;	
}

void CSkillManager::ReleaseSkillArea(CSkillArea* pSkillArea)
{
	m_SkillAreaMgr.ReleaseSkillArea(pSkillArea);
}

void CSkillManager::ReleaseAllSkillInBattle(DWORD BattleID)
{
	CSkillObject* pSObj;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		if(pSObj->GetBattleID() == BattleID)
			ReleaseSkillObject(pSObj,eSkillReleaseKind_BATTLE);
	}	
}

float CSkillManager::GetSkillTreeAmp(CPlayer* pPlayer,CSkillInfo* pSkillInfo)
{
	if(pSkillInfo->GetBeforeSkill() == 0)
		return 1.f;
	
	if(pPlayer->GetMugongLevel(pSkillInfo->GetBeforeSkill()) == 12)
		return 1.2f;

	return 1.f;
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wMugongIdx == wMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfobyDeadlyMugongIdx(WORD wDeadlyMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wTargetMugongIdx == wDeadlyMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

BOOL CSkillManager::LoadJobSkillProbability()
{
	CMHFile file;
	
	if(!file.Init("Resource/jobskill.bin", "rb"))
		return FALSE;

	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo;
	while(!file.IsEOF())
	{
		pJobSkillInfo = new JOB_SKILL_PROBABILITY_INFO;
		memset( pJobSkillInfo, 0, sizeof(JOB_SKILL_PROBABILITY_INFO) );

		pJobSkillInfo->wSkillLevel = file.GetWord();
		file.GetWord(pJobSkillInfo->ProbabilityArray, MAX_JOBMOB_NUM);

		ASSERT(!m_JobSkillProbabilityTable.GetData(pJobSkillInfo->wSkillLevel));

		m_JobSkillProbabilityTable.Add(pJobSkillInfo, pJobSkillInfo->wSkillLevel);
		pJobSkillInfo = NULL;
	}
	file.Release();
	
	return TRUE;
}

BOOL CSkillManager::IsJobSkill(CPlayer* pPlayer, CObject* pTarget, CSkillInfo* pSkillInfo)
{	
	WORD wSkillKind = pSkillInfo->GetSkillKind();

	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
		if(CheckSkillKind(wSkillKind) == TRUE)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}	
	
	if(pTarget->GetObjectKind() == eObjectKind_MapObject)
	{
		return TRUE;
	}
	
	WORD wObjectKind = pTarget->GetObjectKind();	

	if(CheckSkillKind(wSkillKind) == TRUE)	
	{		
		int nSkillLevel = pPlayer->GetMugongLevel(pSkillInfo->GetSkillIndex());		

		if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_NONE )
		{				
			return FALSE;
		}

		if(pPlayer->InTitan() == TRUE)
		{			
			return FALSE;
		}

		BOOL bJobSkillSuccess = FALSE;
		switch(wSkillKind)
		{
		case SKILLKIND_MINING:
			{			
				if( wObjectKind == eObjectKind_Mining )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_COLLECTION:
			{
				if( wObjectKind == eObjectKind_Collection )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;
		case SKILLKIND_HUNT:
			{
				if( wObjectKind == eObjectKind_Hunt )
				{
					bJobSkillSuccess = TRUE;
				}				
			}
			break;		
		}

		if(bJobSkillSuccess == FALSE)
		{		
			return FALSE;
		}

		CMonster* pMonster = (CMonster*)pTarget;
		if(pMonster == NULL)
		{
			return FALSE;
		}
		if(nSkillLevel < pMonster->GetLevel())
		{		
			return FALSE;
		}
	}
	else if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_JOB )
	{			
		return FALSE;
	}	

	return TRUE;
}

WORD CSkillManager::GetJobSkillProbability(WORD wSkillLevel, WORD wTargetLevel)
{
	WORD wProbability = 0;

	m_JobSkillProbabilityTable.SetPositionHead();
	
	JOB_SKILL_PROBABILITY_INFO* pJobSkillInfo = NULL;	
	pJobSkillInfo = m_JobSkillProbabilityTable.GetData(wSkillLevel);
	
	if(pJobSkillInfo == NULL)
	{	
		return wProbability;
	}

	for(WORD i = 0 ; i < MAX_JOBMOB_NUM ; i++)
	{
		if(i+1 == wTargetLevel)
		{
			wProbability = pJobSkillInfo->ProbabilityArray[i];
			return wProbability;
		}
	}	
			
	return wProbability;
}

BOOL CSkillManager::CheckSkillKind(WORD wSkillKind)
{
	if((wSkillKind == SKILLKIND_MINING) || (wSkillKind == SKILLKIND_COLLECTION) || (wSkillKind == SKILLKIND_HUNT))	
	{
		return TRUE;
	}

	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSkillManager::CheckTitanWeapon(CObject* pObject, SKILLINFO* SkillInfo)
{
	if(((CPlayer*)pObject)->InTitan())
	{
		if(SkillInfo->SkillKind == SKILLKIND_TITAN)
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( ((CPlayer*)pObject)->GetTitanWeaponEquipType() != SkillInfo->WeaponKind )
					return FALSE;

				if( ((CPlayer*)pObject)->GetTitanWeaponEquipType() != ((CPlayer*)pObject)->GetWeaponEquipType() )
					return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if(SkillInfo->SkillKind == SKILLKIND_TITAN)
		{
			return FALSE;
		}
		else
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( ((CPlayer*)pObject)->GetWeaponEquipType() != SkillInfo->WeaponKind )
					return FALSE;
			}
		}
	}

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////

#endif
