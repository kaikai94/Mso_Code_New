// SkillManager.cpp: implementation of the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _MHCLIENT_

#include "SkillManager_client.h"
#include "ActionTarget.h"

#include "Hero.h"
#include "MoveManager.h"
#include "QuickManager.h"
#include "Gamein.h"
#include "QuickItem.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "TacticManager.h"

#include "ChatManager.h"
#include "ExchangeManager.h"

#include "GameResourceManager.h"
#include "QuickDialog.h"

#include "PKManager.h"
#include "ObjectActionManager.h"
#include "BattleSystem_Client.h"
#include "../[CC]BattleSystem/GTournament/Battle_GTournament.h"

#include "PeaceWarModeManager.h"

#include "MAINGAME.h"

#include "SkillDelayManager.h"

#include "PeaceWarModeManager.h"
#include "InventoryExDialog.h"
#include "Item.h"
#include "PartyWar.h"
#include "ItemManager.h"
#include "AIManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//GLOBALTON(CSkillManager);
CSkillManager::CSkillManager()
{
	g_PoolSTLIST.Init(100,50,"g_PoolSTLIST");
	m_pSkillInfo = NULL;

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
	//m_SkillUseTime.clear();
}
//[断线重连技能清理函数][By:十里坡剑神][QQ:112582793][2017/12/5]
void CSkillManager::ReLoadSkillInfo()
{
	CSkillInfo* pSInfo = NULL;
	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();
	
	CSkillObject* pSObj = NULL;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		OBJECTMGR->AddGarbageObject((CObject*)pSObj);
		delete pSObj;
	}
	m_SkillObjectTable.RemoveAll();
	m_SkillAreaMgr.Release();

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

	g_PoolSTLIST.Release();
	g_PoolSTLIST.Init(100,50,"g_PoolSTLIST");

	LoadSkillInfoList();
	LoadSkillChangeInfoList();
	LoadJobSkillProbability();
	m_SkillAreaMgr.LoadSkillAreaList();
	m_JobSkillProbabilityTable.Initialize(MAX_JOBLEVEL_NUM);
}

void CSkillManager::Init()
{
	m_SkillInfoTable.Initialize(2000);
	m_SkillObjectTable.Initialize(512);
	LoadSkillInfoList();
	LoadSkillChangeInfoList();

	m_SkillAreaMgr.LoadSkillAreaList();

	m_JobSkillProbabilityTable.Initialize(MAX_JOBLEVEL_NUM);	
	LoadJobSkillProbability();	
}
void CSkillManager::Release()
{
	CSkillInfo* pSInfo = NULL;

	m_SkillInfoTable.SetPositionHead();
	while(pSInfo = m_SkillInfoTable.GetData())
	{
		delete pSInfo;
	}
	m_SkillInfoTable.RemoveAll();
	
	CSkillObject* pSObj = NULL;

	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		OBJECTMGR->AddGarbageObject((CObject*)pSObj);
		delete pSObj;
	}
	m_SkillObjectTable.RemoveAll();

	m_SkillAreaMgr.Release();

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
	//////////////////////////////////////////////////////////////////////////
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

		WORD SkillIndex = pInfo->GetSkillIndex();
	
		ASSERT(m_SkillInfoTable.GetData(pInfo->GetSkillIndex()) == NULL);
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
	//////////////////////////////////////////////////////////////////////////
	LoadSkillOptionList();
	//////////////////////////////////////////////////////////////////////////
	LoadStateList();
	//////////////////////////////////////////////////////////////////////////
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


void CSkillManager::LoadSkillChangeInfoList()
{
	CMHFile file;
	file.Init("Resource/SkillChangeList.bin","rb");
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0,"SkillChangeList初始化失败");
		return;
	}

	while(1)
	{
		if(file.IsEOF() != FALSE)
			break;
		SKILL_CHANGE_INFO * pInfo = new SKILL_CHANGE_INFO;
		pInfo->wMugongIdx = file.GetWord();
		pInfo->wChangeRate = file.GetByte();
		pInfo->wTargetMugongIdx = file.GetWord();

		m_SkillChangeList.AddTail(pInfo);
	}
}

//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////
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
//////////////////////////////////////////////////////////////////////////

CSkillInfo*	CSkillManager::GetSkillInfo(WORD SkillInfoIdx)
{
	return (CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx);
}

WORD CSkillManager::GetSkillTooltipInfo(WORD SkillInfoIdx)
{
	SKILLINFO* skInfo = ((CSkillInfo*)m_SkillInfoTable.GetData(SkillInfoIdx))->GetSkillInfo();
	if(!skInfo)
		return 0;
	return skInfo->SkillTooltipIdx;
}
CSkillObject* CSkillManager::GetSkillObject(DWORD SkillObjectID)
{
	return (CSkillObject*)m_SkillObjectTable.GetData(SkillObjectID);
}
WORD CSkillManager::GetComboSkillIdx(CHero* pHero)
{
	WORD SkillNum = 0;
	int WeaponEquipType = pHero->GetWeaponEquipType();
	int CurComboNum = pHero->GetCurComboNum();
	if( CurComboNum == SKILL_COMBO_NUM || 
		CurComboNum == MAX_COMBO_NUM ||
		CurComboNum >= pHero->GetLevel() * 2)
	{
		CurComboNum = 0;
		pHero->SetCurComboNum(0);
	}
	switch(WeaponEquipType)
	{
	case WP_GUM:
		SkillNum = COMBO_GUM_MIN + CurComboNum;
		break;
	case WP_GWUN:
		SkillNum = COMBO_GWUN_MIN + CurComboNum;
		break;
	case WP_DO:
		SkillNum = COMBO_DO_MIN + CurComboNum;
		break;
	case WP_CHANG:
		SkillNum = COMBO_CHANG_MIN + CurComboNum;
		break;
	case WP_GUNG:
		SkillNum = COMBO_GUNG_MIN + CurComboNum;
		break;
	case WP_AMGI:
		SkillNum = COMBO_AMGI_MIN + CurComboNum;
		break;
	case WP_EVENT:
		SkillNum = COMBO_EVENT_MIN;
		break;
	case WP_EVENT_HAMMER:
		SkillNum = COMBO_EVENT_HAMMER;
		break;
	}

	if(pHero->InTitan())
		SkillNum += 10000;

	return SkillNum;
}

BOOL CSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
	WORD SkillNum;
	CSkillInfo* pSkillInfo;

	if(bMugong == FALSE)
	{
		SkillNum = GetComboSkillIdx(pHero);
		pSkillInfo = GetSkillInfo(SkillNum);
	}
	else
		pSkillInfo = m_pSkillInfo;

	if(pSkillInfo == NULL) 
		return FALSE;

	return ExcuteSkillSYN(pHero,pTarget,pSkillInfo);
}
//BOOL CSkillManager::CheckSkillUseTime(CHero* pHero, CSkillInfo* SkillInfo)
//{
//	std::map<WORD,DWORD>::iterator iter;
//	iter = m_SkillUseTime.find(SkillInfo->GetSkillIndex());
//	if(iter!=m_SkillUseTime.end())
//	{
//		if(gCurTime-iter->second>=0)
//		{
//			if(pHero->IsSkipSkill())
//				iter->second = gCurTime+SkillInfo->GetSkillInfo()->DelayTime-SkillInfo->GetSkillOperateAnimationTime(pHero->GetGender());
//			else
//				iter->second = gCurTime+SkillInfo->GetSkillOperateAnimationTime(pHero->GetGender())+SkillInfo->GetSkillInfo()->DelayTime;
//			return TRUE;
//		}
//		else
//			return FALSE;
//	}
//	else
//	{
//		m_SkillUseTime[SkillInfo->GetSkillIndex()] = gCurTime+SkillInfo->GetSkillOperateAnimationTime(pHero->GetGender());
//		return TRUE;
//	}
//}
BOOL CSkillManager::ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
	CActionTarget target;
	target.CopyFrom(pTarget);
	SKILLOPTION* pSkillOption = NULL;

	if(pSkillInfo == NULL)
		return FALSE;
	//if(!CheckSkillUseTime(pHero,pSkillInfo))
	//{// [技能释放检查] [by:十里坡剑神][QQ:112582793][2018/11/6]
	//	return FALSE;
	//}
	int SkillLevel = 0;
		SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());

	if( 0 == SkillLevel )
		return FALSE;

	WORD wSkillKind = pSkillInfo->GetSkillKind();

	WORD SkillOptionIndex = pHero->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	
	if(SkillOptionIndex)
		pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);

	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pSkillOption) == FALSE)
	{
		WORD wSkillKind = pSkillInfo->GetSkillKind();
		if( CheckSkillKind(wSkillKind) == FALSE )
		{
			pHero->SetCurComboNum(0);
		}	
		return FALSE;
	}
	
	if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
	{
		TACTICMGR->HeroTacticStart(pSkillInfo->GetSkillIndex());
		return FALSE;
	}

	if(pSkillInfo->ComboTest(pHero) == FALSE)
	{
		return FALSE;
	}
	
	if(pSkillInfo->ConvertTargetToSkill(pHero,&target) == FALSE)
	{
		return FALSE;
	}

	if(!IsJobSkill(pHero, pTarget, pSkillInfo))
	{
		return FALSE;
	}

	if(pSkillInfo->IsValidTarget(pHero, &target) == FALSE)
		return FALSE;

	if(CheckTitanWeapon(pHero, pSkillInfo->GetSkillInfo()) == FALSE)
	{
		return FALSE;
	}

	if(PEACEWARMGR->IsPeaceMode(pHero) == TRUE)	
		PEACEWARMGR->ToggleHeroPeace_WarMode();
	
	if( pHero->GetStage() & eStage_Hwa )
	{
		if( m_GeukMaSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1144), CHATMGR->GetChatMsg(892) );
			return FALSE;
		}
	}
	else if( pHero->GetStage() & eStage_Geuk )
	{
		if( m_HwaKyungSkillTable.GetData( pSkillInfo->GetSkillIndex() ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1144), CHATMGR->GetChatMsg(890) );
			return FALSE;
		}
	}
	VECTOR3* pTargetPos = target.GetTargetPosition();
	if(pTargetPos == NULL)
		return FALSE;
	
	if(pSkillInfo->IsInSkillRange(pHero,&target,pSkillOption) == TRUE)
	{	
		pHero->ClearWayPoint();
		MOVEMGR->HeroMoveStop();
		if( pSkillInfo->GetSkillInfo()->TargetKind != 1 )
			MOVEMGR->SetLookatPos(pHero,pTargetPos,0,gCurTime);
				
		pHero->SetMovingAction(NULL);		

		if( pHero->IsSkipSkill() == eSkipEffect_Start && pSkillInfo->CanSkipEffect() )
			return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
		if( pSkillInfo->GetSkillInfo()->EffectStart != 0 &&
			pSkillInfo->GetSkillInfo()->EffectStartTime != 0)	
		{
			return HeroSkillStartEffect(pHero,&target,pSkillInfo);
		}
		else
		{
			return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
		}
	}
	else
	{	
		pHero->SetCurComboNum(0);
		CAction MoveAction;
		if(pSkillInfo->IsMugong() == FALSE)
		{
			pSkillInfo = GetSkillInfo(GetComboSkillIdx(pHero));
		}
		MoveAction.InitSkillAction(pSkillInfo,&target);
		MOVEMGR->SetHeroActionMove(&target,&MoveAction);
		sCPlayerAI->SetTooFar(TRUE);//[选怪卡点修复][By:十里坡剑神][QQ:112582793][2018/1/13]
		return FALSE;
	}
	return TRUE;
}

BOOL CSkillManager::ExcuteTacticSkillSYN(CHero* pHero,TACTIC_TOTALINFO* pTInfo,BYTE OperNum)
{
	CActionTarget target;
	target.InitActionTarget(&pTInfo->Pos,NULL);

	CSkillInfo* pSkillInfo = GetSkillInfo(pTInfo->TacticId);
	//ASSERT(pSkillInfo);
	if(pSkillInfo == NULL)
		return FALSE;

	return RealExcuteSkillSYN(pHero,&target,pSkillInfo);
}

void CSkillManager::GetMultiTargetList(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget)
{
	WORD Radius = pSkillInfo->GetSkillInfo()->TargetRange;
	WORD AreaNum = pSkillInfo->GetSkillInfo()->TargetAreaIdx;
	if(AreaNum != 0)
	{
		CSkillArea* pSkillArea = GetSkillArea(pHero,pTarget,pSkillInfo);	
		pTarget->SetTargetObjectsInArea(pHero,pSkillArea);
	}
	else if(Radius != 0)
	{
		if( pSkillInfo->GetSkillInfo()->TargetAreaPivot == 1 && pSkillInfo->GetSkillInfo()->TargetRange != 0 )
		{
			Radius += (WORD)HERO->GetAddAttackRange();

			WORD SkillOptionIndex = HERO->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
			SKILLOPTION* pSkillOption = NULL;

			if(SkillOptionIndex)
			{
				pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);
			}

			if(pSkillOption)
			{
				Radius += pSkillOption->Range;
			}	
		}

		pTarget->SetTargetObjectsInRange(pSkillInfo->GetTargetAreaPivotPos(&pHero->GetCurPosition(),pTarget->GetTargetPosition()),Radius);
	}
}

BOOL CSkillManager::RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
	SKILLOPTION* pSkillOption = NULL;

	if(pSkillInfo == NULL)
		return FALSE;
		
	int SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());

	WORD SkillOptionIndex = pHero->GetSkillOptionIndex(pSkillInfo->GetSkillIndex());
	
	if(SkillOptionIndex)
		pSkillOption = m_SkillOptionTable.GetData(SkillOptionIndex);

	if(pSkillInfo->IsExcutableSkillState(pHero,SkillLevel,pSkillOption) == FALSE)
	{
		pHero->SetCurComboNum(0);
		return FALSE;
	}
	//if(!CheckSkillUseTime(pHero,pSkillInfo))
	//	return FALSE;// [技能释放检查] [by:十里坡剑神][QQ:112582793][2018/11/6]
	if( pHero->GetWeaponEquipType() == WP_EVENT && pSkillInfo->GetSkillIndex() == COMBO_EVENT_MIN )
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();
		
		if( !pItem )
		{
			return	FALSE;
		}

		MSG_ITEM_USE_SYN msg;

		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();
		msg.dwItemDBIdx = pItem->GetDBIdx();
		//[增加物品使用数据库编号判断][By:十里坡剑神][QQ:112582793][2018/4/13]
		NETWORK->Send(&msg,sizeof(msg));
	}

	if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB && pHero->GetNaeRyuk() < pSkillInfo->GetNeedNaeRyuk(1))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );		
		return FALSE;
	}

	if( HERO->IsPKMode() )
	{
		CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if( pObject )
		if( pObject->GetObjectKind() == eObjectKind_Player )
		{
			int SkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());
			if( pSkillInfo->GetFirstPhyAttack( SkillLevel ) ||
				pSkillInfo->GetFirstAttAttack( SkillLevel ) )
			{
				PKMGR->SetPKStartTimeReset();
			}
		}
	}

	HERO->SetNextAction(NULL);

	if(pSkillInfo->IsMultipleTargetSkill() == TRUE)
	{
		pTarget->ConvertMainTargetToPosition(pHero,pSkillInfo->GetSkillRange());
		GetMultiTargetList(pSkillInfo,pHero,pTarget);
	}

	CSkillObject* pSObj = CreateTempSkillObject(pSkillInfo,pHero,pTarget);
	if(pSObj == NULL)
		return FALSE;
	
	pSkillInfo->SendMsgToServer(pHero,pTarget);
	// [技能间隔] [by:十里坡剑神][QQ:112582793][2018/12/6]
	if (pSkillInfo->IsCheckSkill())
		pHero->SetSkillCoolTime(700);
	else
		pHero->SetSkillCoolTime(50);

	if (pHero->IsSkipSkill())
	{
		pHero->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill, pSkillInfo->GetSkillIndex(),
			pSkillInfo->GetSkillInfo()->DelayTime - pSkillInfo->GetSkillOperateAnimationTime(1));
	}
	else
	{
		pHero->GetDelayGroup()->AddDelay(
			CDelayGroup::eDK_Skill, pSkillInfo->GetSkillIndex(),
			pSkillInfo->GetSkillInfo()->DelayTime);
	}

	SKILLDELAYMGR->AddSkillDelay(pSkillInfo->GetSkillIndex());


	pHero->SetCurComboNum(pSkillInfo->GetSkillInfo()->ComboNum);

	m_pSkillInfo = NULL;

	PEACEWARMGR->SetCheckTime(gCurTime);
	sCPlayerAI->SetTooFar(FALSE);//[选怪卡点修复][By:十里坡剑神][QQ:112582793][2018/1/13]
	sCPlayerAI->SetUseSkill();
	MOVEMGR->SetFindPath();
	return TRUE;
}

void CSkillManager::SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
	CSkillInfo* pNextSkill;
	WORD NextSkillIdx;

	if(bMugong)
		pNextSkill = m_pSkillInfo;
	else
	{
		NextSkillIdx = GetComboSkillIdx(pHero);
		pNextSkill = GetSkillInfo(NextSkillIdx);
	}
	
	if(pNextSkill == NULL)
		return;
	CAction act;
	act.InitSkillAction(pNextSkill,pTarget);
	pHero->SetNextAction(&act);
}

BOOL CSkillManager::OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bMugong)
{
	if(pTarget->GetTargetID() != 0)
	{
		CObject* pTObj = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if(pTObj == NULL)
			return FALSE;
		if(pTObj->IsDied() == TRUE)
			return FALSE;

		if( pHero->GetWeaponEquipType() != WP_EVENT )
		{
			if( pTObj->GetObjectKind() & eObjectKind_Monster )
			{
				CMonster* pMonster = (CMonster*)pTObj;
				if(	pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD
					|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG
					)
					return FALSE;
			}
		}
	}

	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pHero->GetBattleTeam() == 2 )
			return FALSE;	
	}
	if( pHero->GetWeaponEquipType() == WP_EVENT )
	{
		if( bMugong )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(586) );	
			return FALSE;
		}
		else
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetPriorityItemForCristmasEvent();

			if( !pItem )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(583) );
				return	FALSE;
			}
		}
	}
	else if( pHero->GetWeaponEquipType() == WP_EVENT_HAMMER )
	{
		if( bMugong )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );	
			return FALSE;
		}
	}

	if( pHero->GetState() == eObjectState_SkillSyn ||
		pHero->GetState() == eObjectState_SkillUsing)
	{
		CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());
		if(pObject == NULL)
		{
			return FALSE;
		}

		if( GetObjectKindGroup(pObject->GetObjectKind()) == eOBJECTKINDGROUP_JOB )
		{
			pHero->SetStage(eObjectState_None);
			return FALSE;
		}
		else
		{
			SetNextComboSkill(pHero,pTarget,bMugong);
		}		
	}
	else
		ExcuteSkillSYN(pHero,pTarget,bMugong);		
	return TRUE;
}
DWORD GetComboDelayTime(WORD WeaponKind)
{
	DWORD time = 0;
	ySWITCH(WeaponKind)
		yCASE(WP_GUM)	time = 120;
		yCASE(WP_GWUN)	time = 150;
		yCASE(WP_DO)	time = 150;
		yCASE(WP_CHANG)	time = 150;
		yCASE(WP_GUNG)	time = 50;
		yCASE(WP_AMGI)	time = 80;
	yENDSWITCH
	return time;
}
void CSkillManager::OnComboTurningPoint(CHero* pHero)
{
	if(pHero->GetNextAction()->HasAction())
	{
		if(pHero->GetNextAction()->GetActionKind() != eActionKind_Skill)
			pHero->SetCurComboNum(0);

		pHero->GetNextAction()->ExcuteAction(pHero);
		pHero->GetNextAction()->Clear();
	}
	else
	{		
		if(pHero->IsAutoAttacking())
		{
			if(pHero->GetCurComboNum() < 2)	
			{
				if(SKILLMGR->OnSkillCommand(pHero,pHero->GetAutoAttackTarget(),FALSE) == FALSE)
					pHero->DisableAutoAttack();
			}
			else
			{
				OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillDelay);
				OBJECTSTATEMGR->EndObjectState(pHero,eObjectState_SkillDelay,GetComboDelayTime(pHero->GetWeaponEquipType()));
				pHero->SetCurComboNum(0);
			}
		}
	}
}
void CSkillManager::OnExcuteSkillNACKed(SKILLOBJECT_INFO* pInfo)
{

}


void CSkillManager::DoCreateSkillObject(CSkillObject* pSObj,SKILLOBJECT_INFO* pSOInfo,CTargetList* pTList)
{
	pSObj->Init(pSOInfo,pTList);
	
	CSkillObject* pPreObj = m_SkillObjectTable.GetData(pSObj->GetID());
	if(pPreObj != NULL)
	{
		ReleaseSkillObject(pPreObj);
	}

	m_SkillObjectTable.Add(pSObj,pSObj->GetID());
	OBJECTMGR->AddSkillObject(pSObj);
}

CSkillObject* CSkillManager::CreateSkillObject(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo)
{
	CObject* pOperator = OBJECTMGR->GetObject(pSkillObjectAddInfo->SkillObjectInfo.Operator);

	WORD SkillIdx = pSkillObjectAddInfo->SkillObjectInfo.SkillIdx;
	CSkillInfo* pSkillInfo = GetSkillInfo(SkillIdx);
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();

	if(pOperator == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(pOperator,&pSkillObjectAddInfo->TargetList,eDamageKind_Front);
		return NULL;
	}
	if(pOperator->GetObjectKind() & eObjectKind_Monster)
	{
		VECTOR3 MainTargetPos;
		GetMainTargetPos(&pSkillObjectAddInfo->SkillObjectInfo.MainTarget,&MainTargetPos,NULL);
		MOVEMGR->SetLookatPos(pOperator,&MainTargetPos,0,gCurTime, pSkillInfo->GetAddDegree());
	}		
	else
		MOVEMGR->SetAngle(pOperator,DIRTODEG(pSkillObjectAddInfo->SkillObjectInfo.Direction),0);


	DoCreateSkillObject(pSObj,&pSkillObjectAddInfo->SkillObjectInfo,
						&pSkillObjectAddInfo->TargetList);

	return pSObj;
}


CSkillObject* CSkillManager::CreateTempSkillObject(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget)
{

	CSkillObject* pTempObj = GetSkillObject(TEMP_SKILLOBJECT_ID);
	if(pTempObj)
	{
		return NULL;
	}
	CSkillObject* pSObj = pSkillInfo->GetSkillObject();
	
	SKILLOBJECT_INFO sinfo;
	sinfo.SkillIdx = pSkillInfo->GetSkillIndex();
	sinfo.SkillObjectIdx = TEMP_SKILLOBJECT_ID;
	sinfo.Direction = DEGTODIR(pHero->GetAngleDeg());
	pTarget->GetMainTarget(&sinfo.MainTarget);
	sinfo.Operator = pHero->GetID();
	VECTOR3* pHeroPos = &pHero->GetCurPosition();
	VECTOR3* pTargetPos = pTarget->GetTargetPosition();
	if(pTargetPos == NULL)
		return NULL;
	sinfo.Pos = *(pSkillInfo->GetTargetAreaPivotPos(pHeroPos,pTargetPos));
	sinfo.StartTime = gCurTime;

	CTargetList tlist;
	pTarget->GetTargetArray(&tlist);

	DoCreateSkillObject(pSObj,&sinfo,&tlist);

	return pSObj;
}

void CSkillManager::ChangeTempObjToRealObj(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo)
{
	CSkillObject* pTempObj = GetSkillObject(TEMP_SKILLOBJECT_ID);

	if(pTempObj == NULL)
	{
		return;
	}

	m_SkillObjectTable.Remove(pTempObj->GetID());
	OBJECTMGR->AddGarbageObject(pTempObj);
	
	DoCreateSkillObject(pTempObj,&pSkillObjectAddInfo->SkillObjectInfo,
							&pSkillObjectAddInfo->TargetList);
}

void CSkillManager::ReleaseSkillObject(CSkillObject* pSkillObject)
{
	CSkillInfo* pSInfo = GetSkillInfo(pSkillObject->GetSkillIdx());
	if(pSInfo == NULL)
	{
		char temp[256];
		sprintf(temp,"skillkind:%d  SkillName:%s  oper:%d",
			pSkillObject->GetSkillIdx(),
			pSkillObject->GetSkillInfo()->GetSkillName(),
			pSkillObject->GetOperator() ? pSkillObject->GetOperator()->GetID() : 0);
		ASSERTMSG(0,temp);
		return;
	}

	m_SkillObjectTable.Remove(pSkillObject->GetID());
	OBJECTMGR->AddGarbageObject(pSkillObject);
	
	pSkillObject->ReleaseSkillObject();
	pSInfo->ReleaseSkillObject(pSkillObject,0);
	
}
void CSkillManager::OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD* pmsg)
{
	if(pmsg->SkillObjectInfo.Operator == HEROID && pmsg->bCreate)
		ChangeTempObjToRealObj(pmsg);
	else
		CreateSkillObject(pmsg);
}
void CSkillManager::OnReceiveSkillObjectRemove(MSG_DWORD* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->dwData);
	if(pSObj == NULL)
	{
		return;
	}
	ReleaseSkillObject(pSObj);
}

void CSkillManager::OnReceiveSkillStartNack()
{
	HERO->SetCurComboNum(0);
	CSkillObject* pSObj = GetSkillObject(TEMP_SKILLOBJECT_ID);
	if(pSObj == NULL)
		return;

	pSObj->SetMissed();
	ReleaseSkillObject(pSObj);
}

void CSkillManager::OnReceiveSkillSingleResult(MSG_SKILL_SINGLE_RESULT* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(NULL,&pmsg->TargetList,eDamageKind_ContinueDamage);
		return;
	}

	pSObj->OnReceiveSkillSingleResult(pmsg);
}

void CSkillManager::OnReceiveSkillObjectTargetListAdd(DWORD SkillObjectID,DWORD AddedObjectID,BYTE bTargetKind)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
	{
		return;
	}
	CObject* pObject = OBJECTMGR->GetObject(AddedObjectID);
	if(pObject == NULL)
	{
		return;
	}

	pSObj->AddTargetObject(pObject,bTargetKind);
}
void CSkillManager::OnReceiveSkillObjectTargetListRemove(DWORD SkillObjectID,DWORD RemovedObjectID)
{
	CSkillObject* pSObj = GetSkillObject(SkillObjectID);
	if(pSObj == NULL)
	{		
		return;
	}

	pSObj->RemoveTargetObject(RemovedObjectID);
}

void CSkillManager::OnReceiveSkillStartEffect(DWORD SkillOperator,DWORD SkillIdx)
{
	CObject* pOperator = OBJECTMGR->GetObject(SkillOperator);
	if(pOperator == NULL)
	{
		return;
	}
	
	CSkillInfo* pSkillInfo = GetSkillInfo((WORD)SkillIdx);
	if(pSkillInfo == NULL)
	{		
		return;
	}

	SkillStartEffect(pOperator,pSkillInfo);
}

void CSkillManager::OnReceiveSkillObjectOperate(MSG_SKILL_OPERATE* pmsg)
{
	CSkillObject* pSObj = GetSkillObject(pmsg->SkillObjectID);
	if(pSObj == NULL)
	{		
		return;
	}

	CObject* pReqObj = OBJECTMGR->GetObject(pmsg->RequestorID);
	pSObj->Operate(pReqObj,&pmsg->MainTarget,&pmsg->TargetList);

}

void CSkillManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
//	printf("[%d] %d // %d\n", ((MSGBASE*)pMsg)->CheckSum, Protocol, ((MSGBASE*)pMsg)->dwObjectID);

	switch(Protocol)
	{
	case MP_SKILL_SKILLOBJECT_ADD:
		{
			MSG_SKILLOBJECT_ADD* pmsg = (MSG_SKILLOBJECT_ADD*)pMsg;
//			printf("%d\n", pmsg->SkillObjectInfo.SkillIdx);
			OnReceiveSkillObjectAdd(pmsg);
		}
		break;
	case MP_SKILL_SKILLOBJECT_REMOVE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CSkillObject* pSObj = GetSkillObject(pmsg->dwData);
			OnReceiveSkillObjectRemove(pmsg);
		}
		break;
	case MP_SKILL_START_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			OnReceiveSkillStartNack();
		}
		break;
	case MP_SKILL_SKILL_SINGLE_RESULT:
		{
			MSG_SKILL_SINGLE_RESULT* pmsg = (MSG_SKILL_SINGLE_RESULT*)pMsg;
			OnReceiveSkillSingleResult(pmsg);
		}
		break;

	case MP_SKILL_ADDOBJECT_TO_SKILLOBJECTAREA_ACK:
		{
			MSG_DWORD2BYTE* pmsg = (MSG_DWORD2BYTE*)pMsg;
			OnReceiveSkillObjectTargetListAdd(pmsg->dwData1,pmsg->dwData2,pmsg->bData);
		}
		break;
	case MP_SKILL_REMOVEOBJECT_FROM_SKILLOBJECTAREA_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillObjectTargetListRemove(pmsg->dwData1,pmsg->dwData2);
		}
		break;

	case MP_SKILL_STARTEFFECT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			OnReceiveSkillStartEffect(pmsg->dwData1,pmsg->dwData2);
		}
		break;

	case MP_SKILL_OPERATE_ACK:
		{
			MSG_SKILL_OPERATE* pmsg = (MSG_SKILL_OPERATE*)pMsg;
			OnReceiveSkillObjectOperate(pmsg);
		}
		break;
		
	case MP_SKILL_DELAY_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			SKILLDELAYMGR->ContinueSkillDelay( pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_SKILL_JOB_NACK:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "收获继续努力");
		}
		break;
	}
}

CSkillArea* CSkillManager::GetSkillArea(CObject* pObject, CActionTarget* pTarget, CSkillInfo* pSkillInfo)
{
	CSkillArea* pArea = GetSkillArea(pObject->GetDirectionIndex(),pSkillInfo->GetSkillAreaIndex());
	
	VECTOR3* pPos;
	pPos = pSkillInfo->GetTargetAreaPivotPos(&pObject->GetCurPosition(),pTarget->GetTargetPosition());
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

void CSkillManager::UpdateSkillObjectTargetList(CObject* pObject)
{
	CSkillObject* pSObj;
	DWORD rtCode;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		rtCode = pSObj->Update();		
		pSObj->UpdateTargetList(pObject);
	}
}

void CSkillManager::RemoveTarget(CObject* pObject,BYTE bTargetKind)
{
	CSkillObject* pSObj;
	m_SkillObjectTable.SetPositionHead();
	while(pSObj = m_SkillObjectTable.GetData())
	{
		pSObj->RemoveTarget(pObject,bTargetKind);
	}
}

void CSkillManager::SkillStartEffect(CObject* pObject,CSkillInfo* pSkillInfo)
{
	TARGETSET set;
	set.pTarget = pObject;
	DWORD flag = 0;
	if(pObject->GetID() == HEROID)
		flag |= EFFECT_FLAG_HEROATTACK;
	EFFECTMGR->StartEffectProcess(pSkillInfo->GetSkillInfo()->EffectStart,pObject,
								&set,1,pObject->GetID(),flag);
}

BOOL CSkillManager::HeroSkillStartEffect(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo)
{
	//if(!CheckSkillUseTime(pHero,pSkillInfo))
	//	return FALSE;// [技能释放检查] [by:十里坡剑神][QQ:112582793][2018/11/6]
	SkillStartEffect(pHero,pSkillInfo);
	OBJECTSTATEMGR->StartObjectState(pHero,eObjectState_SkillStart);
	OBJECTSTATEMGR->EndObjectState(pHero,eObjectState_SkillStart,pSkillInfo->GetSkillInfo()->EffectStartTime);
	CAction act;
	act.InitSkillActionRealExecute(pSkillInfo,pTarget);
	pHero->SetSkillStartAction(&act);

	MSG_DWORD2 msg;
	msg.Category = MP_SKILL;
	msg.Protocol = MP_SKILL_STARTEFFECT;
	msg.dwData1 = pHero->GetID();
	msg.dwData2 = pSkillInfo->GetSkillIndex();
	NETWORK->Send(&msg,sizeof(msg));

	return TRUE;
}

void CSkillManager::MakeSAT()
{
	FILE* fp = fopen("SAT.txt","w");
	fprintf(fp,"%d\n",m_SkillInfoTable.GetDataNum());
	
	CEngineObject man,woman;
	man.Init("man.chx",NULL,eEngineObjectType_Character);
	woman.Init("woman.chx",NULL,eEngineObjectType_Character);
	CSkillInfo* pSkillInfo;
	m_SkillInfoTable.SetPositionHead();
	while(pSkillInfo = m_SkillInfoTable.GetData())
	{
		DWORD StateEndTimeMan = 0;
		DWORD StateEndTimeWoman = 0;
		BOOL bBinding = pSkillInfo->GetSkillInfo()->BindOperator != 0;
		if(!bBinding)
		{
			WORD effectuse = pSkillInfo->GetSkillInfo()->EffectUse;
			StateEndTimeMan = EFFECTMGR->GetOperatorAnimatioEndTime(effectuse,eEffectForMan,&man);
			StateEndTimeWoman = EFFECTMGR->GetOperatorAnimatioEndTime(effectuse,eEffectForWoman,&woman);
			if(StateEndTimeMan == 0)	StateEndTimeMan = 500;
			if(StateEndTimeWoman == 0)	StateEndTimeWoman = 500;			
		}

		fprintf(fp,"%d\t%d\t%d\n",pSkillInfo->GetSkillIndex(),StateEndTimeMan,StateEndTimeWoman);
	}

	man.Release();
	woman.Release();

	fclose(fp);
}

SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wMugongIdx == wMugongIdx)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

BOOL CSkillManager::IsDeadlyMugong(WORD wMugongIdx)
{
	PTRLISTSEARCHSTART(m_SkillChangeList, SKILL_CHANGE_INFO *, pInfo)
		if(pInfo->wTargetMugongIdx == wMugongIdx)
			return TRUE;
	PTRLISTSEARCHEND
	return FALSE;
}
void CSkillManager::DeleteTempSkill()
{
	CSkillObject* pSObj = GetSkillObject(TEMP_SKILLOBJECT_ID);

	ReleaseSkillObject(pSObj);
}
///////////////////////////////////////////////////////////////////
BOOL CSkillManager::LoadJobSkillProbability()
{
	CMHFile file;
	if(!file.Init("Resource/jobskill.bin","rb"))
	{
		return FALSE;
	}

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

BOOL CSkillManager::IsJobSkill(CHero* pHero,CActionTarget* pTarget, CSkillInfo* pSkillInfo)
{	
	CObject* pObject = OBJECTMGR->GetObject(pTarget->GetTargetID());

	if(pObject == NULL)
	{
		return FALSE;
	}
	
	WORD wSkillKind = pSkillInfo->GetSkillKind();
	if( pObject->GetObjectKind() == eObjectKind_Player )		
	{
		if(CheckSkillKind(wSkillKind) == TRUE)
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "生活技，");
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	if(pObject->GetObjectKind() == eObjectKind_MapObject)
	{
		return TRUE;
	}

	WORD wObjectKind = pObject->GetObjectKind();	

	if(CheckSkillKind(wSkillKind) == TRUE)	
	{
		pHero->DisableAutoAttack(); 

		int nSkillLevel = pHero->GetMugongLevel(pSkillInfo->GetSkillIndex());

		if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_NONE )
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "生活技，");						
			return FALSE;
		}

		if(pHero->InTitan() == TRUE)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1657) );
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
			//CHATMGR->AddMsg(CTC_SYSMSG, "是对应的技");								
			return FALSE;
		}

		CMonster* pMonster = (CMonster*)pObject;
		if(pMonster == NULL)
		{				
			return FALSE;
		}

		if(nSkillLevel < pMonster->GetMonsterLevel())
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, "技等级过低");
			return FALSE;
		}		
	}
	else if( GetObjectKindGroup(wObjectKind) == eOBJECTKINDGROUP_JOB )
	{
	//	CHATMGR->AddMsg(CTC_SYSMSG, "请把相应技拖至技栏点击使用");
		pHero->DisableAutoAttack();	
		return FALSE;
	}	
	
	return TRUE;
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
BOOL CSkillManager::CheckTitanWeapon(CHero* pHero, SKILLINFO* SkillInfo)
{
	if( pHero->InTitan() == TRUE )
	{
		if( SkillInfo->SkillKind == SKILLKIND_TITAN )
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( pHero->GetTitanWeaponEquipType() != SkillInfo->WeaponKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
					pHero->DisableAutoAttack();
					return FALSE;
				}

				if( pHero->GetWeaponEquipType() != pHero->GetTitanWeaponEquipType() )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1644));
					pHero->DisableAutoAttack();
					return FALSE;
				}
			}
		}
		else
		{
			if(SKILLMGR->CheckSkillKind(SkillInfo->SkillKind) == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1657) );
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1653) );
			}			
			return FALSE;
		}
	}
	else	
	{
		if( SkillInfo->SkillKind == SKILLKIND_TITAN )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
			return FALSE;
		}
		else
		{
			if( SkillInfo->WeaponKind != 0 )
			{
				if( pHero->GetWeaponEquipType() != SkillInfo->WeaponKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(151) );
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////

#endif
