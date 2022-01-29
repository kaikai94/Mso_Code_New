




#pragma once
#include "object.h"
#include "GameResourceStruct.h"

#define DEFAULT_PET_FOLLOW_DIST	300		//4m �Ÿ� ����
#define MAX_LIMIT_PET_DIST	1000		//10m �̻��̸� ����
#define PET_STATE_CHECK_TIME 10000		//�� ���� üũ �ð�, ���׹̳� ����
#define PET_DEFAULT_FRIENDLY	3000000		//�� �⺻ ģ�е�, 3000/10000
#define PET_REVIVAL_FRIENDLY	2000000	//�� ��Ȱ�� ģ�е�

#define PET_REDUCE_FRIENDLY_ZEROSTAMINA	1000

#define PET_MAX_GRADE		3

enum eSTAMINA_DECREASE_AMOUNT{eSDA_REST = 50, eSDA_STAND = 100, eSDA_MOVE = 200,};

//enum ePET_ACTION
//{
//	ePA_REST		=	1,
//	ePA_STAND		=	2,	ePA_SUBSTAND	=	3,
//	ePA_MOVE		=	4,	ePA_KGONG		=	5,
//	ePA_SKILL		=	8,
//	ePA_APPEAL		=	16,	ePA_DESPAIR		=	17,
//	ePA_DIE			=	32,
//	ePA_UNGI		=	64,
//};


enum ePET_MOTION		//chx ���ϳ� ani ����.. ANIMATION INDEX
{

	ePM_STAND = 1,		//�⺻
	ePM_MOVE,			//�̵�
	ePM_SUBSTAND,		//����
	ePM_KGONG,			//����̵�
	ePM_UNGI,			//ĳ�� ����
	ePM_MASTER_SKILL,	//ĳ�� ���ݽ� ����
	ePM_MASTER_DIE,		//ĳ�� ������
	ePM_SKILL,			//�꽺ų ����
	ePM_DIE,			//������
	ePM_STAMINA_ZERO,	//���¹̳� 0�� �� (��縸)
	ePM_DIED = 10,		//���������� Ȧ��(Client������ ���)
	//��ȹ���� �� ���߿� �߰��۾����� ���¹̳� 0�϶� ����߰�. ���� �ִϵ����ʹ� '��������'�� �������.
};

struct sPetState
{
	sPetState() : stateOld(0),stateCur(0) {}
	DWORD	stateOld;
	DWORD	stateCur;
};

class CPet :	public CObject
{
	PET_TOTALINFO	m_PetTotalInfo;
	sPetState		m_PetState;
	char			m_MasterName[MAX_NAME_LENGTH+1];
	DWORD			m_dwMaxStamina;


	DWORD			m_dwStateCheckTime;
	DWORD			m_dwStaminaDecrease;
	BOOL			m_bRest;

	BASE_PET_LIST*	m_pSInfo;
	DWORD			m_dwMasterID;
	CPlayer*		m_pMaster;

	void InitPet(PET_TOTALINFO* pTotalInfo);
public:


	friend class CObjectManager;
	friend class CPetManager;

	CPet(void);

	virtual ~CPet(void);

	void	PetGradeUp();
	DWORD	GetPetIdx()			{	return m_PetTotalInfo.PetKind;	}
	DWORD	GetPetCurGrade()	{	return m_PetTotalInfo.PetGrade;	}
	DWORD	GetPetSommonItemDBIdx()	{	return m_PetTotalInfo.PetSummonItemDBIdx;	}

	void	SetPetRest(BOOL bVal) {	m_bRest=bVal;	}
	BOOL	IsPetRest()		{	return m_bRest;	}
	BOOL	IsPetStaminaFull();


	void Process();

	void	SetMaxStamina(DWORD dwMaxStamina);
	DWORD	GetMaxStamina()	{	return m_dwMaxStamina;	}
	void	CalcStamina();
	void	PlusStamina(DWORD amount);
	void	CalcFriendship();
	void	CalcSkillRecharge();

	void SetMaster(CPlayer* pPlayer);
	void SetMasterID(DWORD dwMasterID)	{	m_dwMasterID = dwMasterID;	}
	CPlayer* GetMaster()	{	return m_pMaster;	}
	void SetPetMasterName(DWORD dwMasterIdx);
	void SetPetMasterName(char* pMasterName);
	BASE_PET_LIST*	GetSInfo() {	return m_pSInfo;	}
	WORD GetPetKind()	{	return m_PetTotalInfo.PetKind;	}
	WORD GetPetGrade()	{	return m_PetTotalInfo.PetGrade;	}
	virtual void SetMotionInState(BYTE State);
	virtual float DoGetMoveSpeed();

	void Die();
};


