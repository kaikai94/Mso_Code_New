



// TacticManager.h: interface for the CTacticManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICMANAGER_H__1DDB15D4_3E95_443D_9D35_FF717CAB00AB__INCLUDED_)
#define AFX_TACTICMANAGER_H__1DDB15D4_3E95_443D_9D35_FF717CAB00AB__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define  TACTICMGR CTacticManager::GetInstance()

class CTaticAbilityInfo;

class CTacticManager  
{

	CYHHashTable<CTacticStartInfo> m_TacticInfoTable;
	CYHHashTable<CTacticObject> m_GarbageTacticTable;
	CTaticAbilityInfo* m_TacticAbilityInfo;

	void RegistTacticObject(CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction);



public:
	GETINSTANCE(CTacticManager);
	CTacticManager();
	virtual ~CTacticManager();

	void AddTacticStartInfo(TACTICSTART_INFO* pList);
	void AddTaticAbilityInfo(TATIC_ABILITY_INFO* pList);
	
	CTacticStartInfo* GetTacticStartInfo(WORD TacticID);
	CTaticAbilityInfo* GetTaticAbilityInfo(int num);

	void OnTacticStartReceived(CPlayer* pPlayer, SEND_TACTIC_START* pmsg);

	void OnTacticJoinReceived(CPlayer* pPlayer, SEND_TACTIC_JOIN* pmsg);
	

	BOOL TacticProcess(CTacticObject* pTacObj);
	
	void AddGarbageTactic(CTacticObject* pTacObj);
	void AddGarbageTactic(CPlayer* pOperator);
	void ProcessGarbageTactic();

	CTacticObject* GetTacticObject(DWORD TacticObjectID);



	DWORD GetNewTacticObjectIdx();

	DWORD	GetAttackValue( CPlayer* pOperator, DWORD AttackValue );
//	DWORD	GetAttackMaxValue( CPlayer* pOperator, WORD SkillIdx );

//	DWORD	GetAttackMinValue( CPlayer* pOperator, WORD SkillIdx );
	void	GetRecoverValue( CPlayer* pOperator, WORD SkillIdx, WORD &Life, WORD &Naeryuk );
	float	GetBuffRateForPhyAttack( CPlayer* pOperator, WORD SkillIdx );		// �������� ������
	float	GetBuffRateForPhyDefense( CPlayer* pOperator, WORD SkillIdx );		// ������� ������
	float	GetBuffRateForAttrDefense( CPlayer* pOperator, WORD SkillIdx );		// �Ӽ���� ������
	DWORD	GetBuffValueForNaeryuk( CPlayer* pOperator, WORD SkillIdx );		// ���»�¾�
	DWORD	GetBuffValueForLife( CPlayer* pOperator, WORD SkillIdx );			// ����»�¾�
	DWORD	GetBuffValueForShield( CPlayer* pOperator, WORD SkillIdx );			// ȣ�Ű����¾�
};

#endif // !defined(AFX_TACTICMANAGER_H__1DDB15D4_3E95_443D_9D35_FF717CAB00AB__INCLUDED_)


