// SkillManager.h: interface for the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
#define AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillInfo.h"
#include "SkillObject_Client.h"
#include "SkillAreaManager.h"
#include <map>
class CHero;
class CActionTarget;

//#define SKILLMGR USINGTON(CSkillManager)
#define SKILLMGR	CSkillManager::GetInstance()

class CSkillManager  
{
	//std::map<WORD,DWORD> m_SkillUseTime;

	CYHHashTable<CSkillInfo>	m_SkillInfoTable;
	CYHHashTable<CSkillObject>	m_SkillObjectTable;
	cPtrList m_SkillChangeList;
	
	CSkillInfo* m_pSkillInfo;	

	CSkillAreaManager m_SkillAreaMgr;

	CYHHashTable<CSkillInfo>	m_HwaKyungSkillTable;
	CYHHashTable<CSkillInfo>	m_GeukMaSkillTable;

	CYHHashTable<SKILLOPTION>	m_SkillOptionTable;
	CYHHashTable<SKILLOPTION>	m_SkillOptionByItemTable;

	CYHHashTable<SPECIAL_STATE_INFO> m_SpecialStateInfoTable;

	CYHHashTable<JOB_SKILL_PROBABILITY_INFO>	m_JobSkillProbabilityTable;

	void SkillStartEffect(CObject* pObject,CSkillInfo* pSkillInfo);
	BOOL HeroSkillStartEffect(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);
	

	CSkillObject* CreateSkillObject(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo);
	CSkillObject* CreateTempSkillObject(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget);
	void ChangeTempObjToRealObj(MSG_SKILLOBJECT_ADD* pSkillObjectAddInfo);

	void DoCreateSkillObject(CSkillObject* pSObj, SKILLOBJECT_INFO* pSOInfo,CTargetList* pTList);

	void ReleaseSkillObject(CSkillObject* pObject);
	
public:
	CSkillManager();
	 ~CSkillManager();

	GETINSTANCE(CSkillManager);
	void ReLoadSkillInfo();    //[断线重连技能清理函数][By:十里坡剑神][QQ:112582793][2017/12/5]
	void Init();
	void Release();

	void LoadSkillInfoList();
	void LoadSkillTreeList();
	void LoadSkillChangeInfoList();
	void LoadJobSkillList();
	void LoadSkillOptionList();
	void LoadStateList();

	CSkillObject* GetSkillObject(DWORD SkillObjectID);
	CSkillInfo*	GetSkillInfo(WORD SkillInfoIdx);
	CSkillInfo*	GetDummySkillInfo(WORD SkillInfoIdx);
	SKILL_CHANGE_INFO * CSkillManager::GetSkillChangeInfo(WORD wMugongIdx);
	
	WORD GetSkillTooltipInfo(WORD SkillInfoIdx);
	WORD GetComboSkillIdx(CHero* pHero);
	void SetSkill(CSkillInfo* pSkillInfo) { m_pSkillInfo = pSkillInfo; }

	CSkillArea* GetSkillArea(CObject* pObject, CActionTarget* pTarget, CSkillInfo* pSkillInfo);
	CSkillArea* GetSkillArea(DIRINDEX dir,WORD SkillAreaIndex);
	void ReleaseSkillArea(CSkillArea* pSkillArea);
	
	void GetMultiTargetList(CSkillInfo* pSkillInfo,CHero* pHero,CActionTarget* pTarget);

	BOOL IsDeadlyMugong(WORD wMugongIdx);
	
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);
	BOOL ExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);
	BOOL ExcuteTacticSkillSYN(CHero* pHero,TACTIC_TOTALINFO* pTInfo,BYTE OperNum);

	BOOL RealExcuteSkillSYN(CHero* pHero,CActionTarget* pTarget,CSkillInfo* pSkillInfo);

	void SetNextComboSkill(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);

	BOOL OnSkillCommand(CHero* pHero,CActionTarget* pTarget,BOOL bMugong);
	void OnComboTurningPoint(CHero* pHero);
	void OnExcuteSkillNACKed(SKILLOBJECT_INFO* pInfo);
	
	void UpdateSkillObjectTargetList(CObject* pObject);
	
	void RemoveTarget(CObject* pObject,BYTE bTargetKind);
	
	CSkillInfo* IsHwaKungSkill( DWORD SkillIdx )		{	return m_HwaKyungSkillTable.GetData( SkillIdx );	}
	CSkillInfo* IsGeukMaSkill( DWORD SkillIdx )			{	return m_GeukMaSkillTable.GetData( SkillIdx );	}


	void OnReceiveSkillObjectAdd(MSG_SKILLOBJECT_ADD* pmsg);
	void OnReceiveSkillObjectRemove(MSG_DWORD* pmsg);
	void OnReceiveSkillStartNack();
	void OnReceiveSkillSingleResult(MSG_SKILL_SINGLE_RESULT* pmsg);
	void OnReceiveSkillObjectTargetListAdd(DWORD SkillObjectID,DWORD AddedObjectID,BYTE bTargetKind);
	void OnReceiveSkillObjectTargetListRemove(DWORD SkillObjectID,DWORD RemovedObjectID);
	void OnReceiveSkillStartEffect(DWORD SkillOperator,DWORD SkillIdx);
	void OnReceiveSkillObjectOperate(MSG_SKILL_OPERATE* pmsg);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void MakeSAT();

	void DeleteTempSkill();

	SKILLOPTION* GetSkillOption(WORD index) { return (SKILLOPTION*)(m_SkillOptionTable.GetData(index)); }
	SKILLOPTION* GetSkillOptionByItemIndex(WORD index) { return (SKILLOPTION*)(m_SkillOptionByItemTable.GetData(index)); }
	SPECIAL_STATE_INFO* GetSpecialStateInfo(WORD index) { return (SPECIAL_STATE_INFO*)(m_SpecialStateInfoTable.GetData(index)); }

	BOOL LoadJobSkillProbability();
	BOOL IsJobSkill(CHero* pHero,CActionTarget* pTarget, CSkillInfo* pSkillInfo);
	BOOL CheckSkillKind(WORD wSkillKind);

	BOOL CheckTitanWeapon(CHero* pHero, SKILLINFO* SkillInfo);	
	/*BOOL CheckSkillUseTime(CHero* pHero, CSkillInfo* SkillInfo);*/
};
#endif // !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
