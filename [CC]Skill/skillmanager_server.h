// SkillManager.h: interface for the CSkillManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
#define AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillInfo.h"
#include "SkillObject_Server.h"
#include "IndexGenerator.h"
#include "SkillAreaManager.h"
#include "../4DyuchiGXGFunc/global.h"


#define SKILLMGR	CSkillManager::GetInstance()

enum errnum
{
	eErr_SkillInfoNULL = 1, 
	eErr_SkillLvl0,
	eErr_NotExcutableSkillState,
	eErr_NoTarget,
	eErr_ComboTest,
	eErr_NotInRange,
	eErr_CheckEvade,
};

class CSkillManager  
{
public:
	CYHHashTable<CSkillInfo>	m_SkillInfoTable;
	CYHHashTable<CSkillObject>	m_SkillObjectTable;
	cPtrList m_SkillChangeList;

	INDEXCR_HANDLE		m_ICHandle;

	CSkillAreaManager m_SkillAreaMgr;
	
	// ȭ��, �ظ�
	CYHHashTable<CSkillInfo>	m_HwaKyungSkillTable;
	CYHHashTable<CSkillInfo>	m_GeukMaSkillTable;

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	CYHHashTable<SKILLOPTION>	m_SkillOptionTable;
	CYHHashTable<SKILLOPTION>	m_SkillOptionByItemTable;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ����/����	
	CYHHashTable<SPECIAL_STATE_INFO> m_SpecialStateInfoTable;
	//////////////////////////////////////////////////////////////////////////

	////////// 2007. 6. 28. CBH - ������� Ȯ�� ����Ʈ //////////////////////
	CYHHashTable<JOB_SKILL_PROBABILITY_INFO>	m_JobSkillProbabilityTable;	
	/////////////////////////////////////////////////////////////////////////

	float GetSkillTreeAmp(CPlayer* pPlayer,CSkillInfo* pSkillInfo);
public:
	CSkillManager();
	virtual ~CSkillManager();
	
	GETINSTANCE(CSkillManager);


	//////////////////////////////////////////////////////////////////////////
	
	void Init();
	void Release();

	void LoadSkillInfoList();
	void LoadSkillTreeList();
	void LoadSkillChangeInfoList();
	void LoadJobSkillList();
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	void LoadSkillOptionList();
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ����/����
	void LoadStateList();
	//////////////////////////////////////////////////////////////////////////

	void ReleaseSkillObject(CSkillObject* pSkillObject,int ReleaseKind);

	CSkillInfo* GetSkillInfo(WORD SkillInfoIdx);
	CSkillObject* GetSkillObject(DWORD SkillObjID);
	SKILL_CHANGE_INFO * GetSkillChangeInfo(WORD wMugongIdx);
	SKILL_CHANGE_INFO * GetSkillChangeInfobyDeadlyMugongIdx(WORD wDeadlyMugongIdx);
	
	CSkillArea* GetSkillArea(VECTOR3* pObjPos, VECTOR3* pTargetPos, CSkillInfo* pSkillInfo);
	CSkillArea* GetSkillArea(DIRINDEX dir,WORD SkillAreaIndex);
	void ReleaseSkillArea(CSkillArea* pSkillArea);

	DWORD GetNewSkillObjectID();
	void ReleaseSkillObjectID(DWORD SkillObjectID);
	
	void OnSkillStartSyn(MSG_SKILL_START_SYN* pmsg);
	void OnAddObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID);
	void OnRemoveObjectToSkillObjectArea(DWORD SkillObjectID,DWORD ObjectID);
	void OnSkillStartEffect(MSG_DWORD2* pmsg);
	void OnSkillOperateSyn(MSG_SKILL_OPERATE* pmsg);
	
	void MonsterAttack(WORD SkillIdx,CObject* pMonster,CObject* pTarget);
	void MonsterAttack(WORD SkillIdx,CObject* pMonster,VECTOR3* pTargetPos,DWORD TargetID);
	
	void PetOperSkill(WORD SkillIdx,CPet* pPet,CObject* pTarget);	//���� ���� ��ų ���� ����

	void Process();
	
	void NetworkMsgParse(BYTE Protocol,void* pMsg);

	void GetMainTargetRadius(MAINTARGET* pMainTarget,float* pRtRadius);
	
	void ReleaseAllSkillInBattle(DWORD BattleID);

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ���� ��ȯ �߰�
	SKILLOPTION* GetSkillOption(WORD index) { return (SKILLOPTION*)(m_SkillOptionTable.GetData(index)); }
	SKILLOPTION* GetSkillOptionByItemIndex(WORD index) { return (SKILLOPTION*)(m_SkillOptionByItemTable.GetData(index)); }
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2�� ���� - �̿���
	// ����/����
	SPECIAL_STATE_INFO* GetSpecialStateInfo(WORD index) { return (SPECIAL_STATE_INFO*)(m_SpecialStateInfoTable.GetData(index)); }
	//////////////////////////////////////////////////////////////////////////

	////////// 2007. 6. 28. CBH - ������� ���� �Լ� �߰� ////////////////////
	BOOL LoadJobSkillProbability();		//������� Ȯ�� ��ũ��Ʈ �ε�
	BOOL IsJobSkill(CPlayer* pPlayer, CObject* pTarget, CSkillInfo* pSkillInfo);	//��ų�� ������� ������Ʈ ���� ó��
	WORD GetJobSkillProbability(WORD wSkillLevel, WORD wTargetLevel);	//�ش� ������� Ȯ�� ��ȯ	
	BOOL CheckSkillKind(WORD wSkillKind);
	///////////////////////////////////////////////////////////////////////////////

	///// 2007. 10. 15. CBH - Ÿ��ź ������ ���� üũ(SkillInfo���� �̵�) /////////////
	BOOL CheckTitanWeapon(CObject* pObject, SKILLINFO* SkillInfo);
	//////////////////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_SKILLMANAGER_H__77A8371D_54A8_40CF_AEBC_7D8C21051312__INCLUDED_)
