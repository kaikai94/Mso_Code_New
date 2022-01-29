



// Monster.h: interface for the CMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)
#define AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"
#include "GameResourceStruct.h"

class CMonster : public CObject  
{
	MONSTER_TOTALINFO m_DummyMonsterInfo;
	MONSTER_TOTALINFO m_MonsterInfo;

	BASE_MONSTER_LIST * m_pSInfo;


	void InitMonster(MONSTER_TOTALINFO* pTotalInfo);

public:
	void CheckTmpFunc();
	CMonster();

	virtual ~CMonster();
	
	BASE_MONSTER_LIST * GetSInfo() {  return m_pSInfo; }
	virtual void SetLife(DWORD val, BYTE type = 1);
	virtual DWORD GetLife(){  return m_MonsterInfo.Life;	}
	virtual DWORD DoGetMaxLife(){return m_DummyMonsterInfo.Life;}
	virtual void SetShield(DWORD val, BYTE type = 1);
	virtual DWORD GetShield(){  return m_MonsterInfo.Shield;	}

	virtual float DoGetMoveSpeed();

	virtual void Process();

	friend class CObjectManager;

	WORD GetMonsterKind()		{ return m_MonsterInfo.MonsterKind;	}
	
	virtual void SetMotionInState(BYTE State);
	

	void Attack(int AttackNum,CObject* pTarget);
	
	virtual void Die(CObject* pAttacker,BOOL bFatalDamage,BOOL bCritical, BOOL bDecisive);
	virtual void Damage(CObject* pAttacker,BYTE DamageKind,DWORD Damage,DWORD ShieldDamage,BOOL bCritical, BOOL bDecisive, DWORD titanObserbDamage);
	virtual void Heal(CObject* pHealer,BYTE HealKind,DWORD HealVal);

	float GetWeight();

	float GetRadius();

	virtual void AddStatus(CStatus* pStatus);

	WORD GetObjectKind();
	WORD GetMonsterLevel();
	///////////////////////////////////////////////
};


#endif // !defined(AFX_MONSTER_H__00F01EA1_24D7_4E54_8F96_54AD6FF735BF__INCLUDED_)


