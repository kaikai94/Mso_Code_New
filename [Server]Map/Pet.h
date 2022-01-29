#pragma once
#include "object.h"
#include "..\[CC]Header\GameResourceStruct.h"

class CPlayer;
struct BASE_PET_LIST;

#define PET_DEFAULT_FRIENDLY 3000000	
#define PET_REVIVAL_FRIENDLY 2000000	
#define PET_MAX_FRIENDLY	10000000	

#define PET_STATE_CHECK_TIME 10000	

#define PET_MAX_GRADE		3

enum eFRIENDSHIP_INCREASE_AMOUNT	
{
	eFIA_MASTER_DIE		= -1000000,
	eFIA_STAMINA_ZERO	= -2000,	
	eFIA_TRADE			= -1000000,
	eFIA_UPGRADE_FAIL	= -1000000,
};
enum ePET_MOTION
{
	ePM_STAND = 1,		
	ePM_MOVE,			
	ePM_SUBSTAND,		
	ePM_KGONG,			
	ePM_UNGI,			
	ePM_MASTER_SKILL,
	ePM_MASTER_DIE,		
	ePM_SKILL,			
	ePM_DIE,			
	ePM_STAMINA_ZERO,	
	ePM_DIED = 10,		
};

enum ePET_BASIC_STATE
{
	ePBS_NONE,
	ePBS_MOVING,
	ePBS_UNGI,
};

enum ePET_MOTION_FREQUENCY
{
	ePMF_RANDOM,
	ePMF_ALWAYS,
};

class CPet : public CObject
{
	PET_TOTALINFO	m_PetTotalInfo;
	DWORD			m_dwMaxStamina;

	DWORD			m_dwStateCheckTime;	
	DWORD			m_dwRndGapTime;			
	BYTE			m_CurBasicState;		

	DWORD			m_dwInfoCheckTime;	
	DWORD			m_dwStaminaDecrease;
	WORD			m_wFrameCounter;		

	CPlayer*		m_pMaster;
	BASE_PET_LIST*	m_pBaseInfo;

public:
	CPet(void);
	virtual ~CPet(void);

	virtual void DoDie(CObject* pAttacker);
	virtual void Release();

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);

	virtual float DoGetMoveSpeed();

	void	InitPet(PET_TOTALINFO* pTotalinfo);
	void	SetMaster(CPlayer* pPlayer) {m_pMaster=pPlayer;}

	CPlayer*	GetMaster() {	return m_pMaster;	}

	void	GetPetTotalInfoRt(PET_TOTALINFO* pRtInfo);
	void	GetPetMasterNameRt(char* pRtMasterName);
	const PET_TOTALINFO* GetPetTotalInfo()	{	return &m_PetTotalInfo;	}
	DWORD	GetPetSummonItemDBIdx()	{	return m_PetTotalInfo.PetSummonItemDBIdx;	}
	WORD	GetPetKind()	{	return m_PetTotalInfo.PetKind;	}
	void	SetPetStamina(DWORD dwStamina);
	void	SetPetMaxStamina(DWORD val, BOOL bSendMsg = FALSE);
	void	SetPetFriendShip(DWORD dwFriendShip);
	void	SetPetAlive(BOOL bVal)	{	m_PetTotalInfo.bAlive = bVal;	}

	DWORD	GetPetFriendShip()	{ return m_PetTotalInfo.PetFriendly; }

	void	PetGradeUp(WORD wPetLevel);
	DWORD	GetPetCurGrade()	{	return m_PetTotalInfo.PetGrade;	}
	void	CheckPetMoving();

	BOOL	IsPetMoving()	{	return m_MoveInfo.bMoving;	}
	void	SetPetRest(BOOL bRest) {	m_PetTotalInfo.bRest = bRest;	}
	BOOL	IsPetRest()		{	return m_PetTotalInfo.bRest;	}
	void	SetPetSummonning(BOOL bVal)	{	m_PetTotalInfo.bSummonning = bVal;	}
	void	CheckCurBasicState();
	void	SetPetBasicState(BYTE basicstate) {	m_CurBasicState = basicstate;	}
	BYTE	GetPetBasicState()	{	return m_CurBasicState;	}
	BYTE	GetMotionNumFromBasicState();
	void	GetPetMotionFromBasicState();		
	void	GetRandMotionNSpeech(DWORD state=0, BYTE frequency=ePMF_RANDOM);

	void	Process();
	const BASE_PET_LIST* GetPetBaseInfo() {	return m_pBaseInfo;	}

	void	CalcPetMaxStamina();
	void	CalcStamina();				
	void	AddStamina(int stamina, BOOL bSendMsg = FALSE);

	void	AddFriendship(int friendship, BOOL bSendMsg = FALSE);
	void	CalcFriendship();
	void	CalcSkillRecharge();
	BOOL	IsPetMaxFriendship();
	BOOL	IsPetStaminaZero()		{	return (0 == m_PetTotalInfo.PetStamina);	}
	BOOL	IsPetStaminaFull();

	BOOL	UsePetSkill();

	void	SendPetInfoMsg();
	void	SendPetDieMsg();
};


