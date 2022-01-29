



#pragma once

#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_MAX_LEVEL		3
#define PET_GRADEUP_PROB_1TO2	80
#define PET_GRADEUP_PROB_2TO3	80

#define PET_RESUMMON_VALID_TIME	30000

#define CRISTMAS_EVENTPET 8
#define CRISTMAS_EVENTPET_SUMMONNING_TIME 60000*30

enum eFncOptPetTotalInfo{ eWithNULLOBJ, eWithNULL };
enum ePetUpgradeResult{eUpgradeSucess=0, eUpgradeFailforProb=1, eUpgradeFailforEtc=2, eUpgradeFailfor3rdUp=3, eUpgradeFailforSamePetSummoned=4,};
enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

enum ePetBuffKind
{
	ePB_None,
	ePB_Demage_Percent=1,				//[增加攻击力][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:41]		
	ePB_Dodge=2,						//[闪避][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:35]				
	ePB_MasterAllStatUp=3,				//[所有属性增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:29]	
	ePB_Item_DoubleChance=4,			//[获得物品机率提升][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:23]	
	ePB_NoForeAtkMonster=5,				//[安抚主动怪物][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:17]
	ePB_ReduceCriticalDmg=6,			//[减少受到奋力一击伤害][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:09]	
	ePB_MasterAllStatRound=7,			//[属性调整][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:02]	
	ePB_Item_RareProbUp=8,				//[获得祝福装备机率增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:03:55]
	ePB_MussangTimeIncrease=9,			//[无双模式维持时间增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:03:48]
	ePB_ReduceCriticalRate = 10,		//[减少对方一击概率][BY:十里坡剑神][QQ:112582793][2019-9-11][16:14:55]
	ePB_ReduceDemageRate = 11,			//[减少受到伤害的百分比][BY:十里坡剑神][QQ:112582793][2019-9-11][17:02:47]
	ePB_ReduceCriticalDmgPlayer = 12,	//[减少受到玩家一击伤害百分比][BY:十里坡剑神][QQ:112582793][2019-9-11][18:02:35]
	ePB_Kind_Max
};
enum PetSummonning{ePSS_ReleaseSummon,ePSS_SaveSummon};

enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};


struct BuffData
{
	BuffData():Prob(0),BuffValueData(0),BuffAdditionalData(0) {};
	DWORD	Prob;

	DWORD	BuffValueData;
	DWORD	BuffAdditionalData;
};

enum ePetEquipItemIdx
{
	ePEII_FriendshipIncrease50perc = 55800,
	ePEII_StaminaReductionDecrease50perc = 55801,
};


struct PETEQUIP_ITEMOPTION
{
	PETEQUIP_ITEMOPTION():iPetStaminaReductionDecrease(0),
		iPetStaminaMaxIncreaseAmount(0),
		iPetStaminaRecoverateIncrease(0),
		iPetStaminaRecoverateAmount(0),
		iPetFriendshipIncrease(0),
		fPetFriendshipProtectionRate(0){};
	int	iPetStaminaReductionDecrease;
	int	iPetStaminaMaxIncreaseAmount;	
	int	iPetStaminaRecoverateIncrease;	
	int	iPetStaminaRecoverateAmount;	
	int	iPetFriendshipIncrease;
	float fPetFriendshipProtectionRate;	
};

class CPlayer;

class CPetManager
{
	static DWORD					m_dwPetObjectID;

	CPlayer*						m_pPlayer;
	CPet*							m_pCurSummonPet;

	PETEQUIP_ITEMOPTION				m_PetEquipOption;

	DWORD							m_dwSkillRechargeCheckTime;

	DWORD							m_dwSkillRechargeAmount;
	BOOL							m_bSkillGuageFull;

	BOOL							m_bPetStaminaZero;

	DWORD							m_dwReleaseDelayTime;

	DWORD							m_dwResummonDelayTime;

	int								m_iFriendshipReduceAmount;

	DWORD							m_dwEventPetSummonRemainTime;	
	DWORD							m_dwEventPetCheckTime;			

	WORD							m_wPetKind;

	DWORD		m_dwPetValidDistPosCheckTime;

	int								m_BuffFlag;
	BuffData						m_BuffData[ePB_Kind_Max];

	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;

public:

	CPetManager(void);
	virtual ~CPetManager(void);

	void		Init(CPlayer* pPlayer);
	void		Release();

	void		AddPet(CPet* pPet,DWORD	ItemDBIdx);
	void		AddPetTotalInfo(PET_TOTALINFO* pPetInfo,int flagSendMsgTo = eServerOnly);
	void		RemovePetTotalInfo(DWORD dwSummonItemDBIdx);
	void		DeletePet(DWORD ItemDBIdx);	
	void		ReleaseCurPetMove();
	void		UpGradeSummonPet();
	BOOL		UpGradePet(DWORD dwSummonItemDBIdx, BOOL bCheckProb = TRUE);
	BOOL		RevivalPet(DWORD dwSummonItemDBIdx, int iGrade);
	void		RemovePet();

	void		AddPetInfoList(PET_TOTALINFO* pPetInfo);
	void		RemovePetInfoList(DWORD dwSummonItemDBIdx);
	WORD		GetPetInfoList(PET_TOTALINFO* RtInfo);

	CPet*		GetPet(DWORD ItemDBIdx);
	PET_TOTALINFO* GetPetTotalInfo(DWORD dwItemDBIdx, int flg = eWithNULLOBJ);
	BOOL		CheckPetDefaultFriendship(DWORD dwItemDBIdx);
	CPet*		GetPetFromSummonItemDBIdx(DWORD dwItemDBIdx);
	CPet*		GetCurSummonPet() {	return m_pCurSummonPet;	}
	void		SetCurSummonPetNull()	{	m_pCurSummonPet = NULL;	}

	void		SummonPet(DWORD dwItemDBIdx, BOOL bSummonFromUser = TRUE);
	BOOL		CheckBeforeSummonPet(DWORD dwItemDBIdx);
	void		SummonEventPet();
	void		SetCurSummonPetKind(WORD kind)	{ m_wPetKind = kind;	}
	BOOL		CheckCurSummonPetKindIs(int kind);
	void		CheckEventPetSummonRemainTime();

	void		SealPet();

	void		ExchangePet(DWORD dwItemDBIdx, CPlayer* pNewMaster, BOOL bReduceFriendship = TRUE );
	BOOL		IsCurPetSummonItem(DWORD dwItemDBIdx);
	void		FeedUpPet(DWORD dwFeedAmount);
	
	void		PetProcess();
	void		CheckPosValidDistWithMaster();

	void		CheckStaminaZero();

	void		CalcPetSkillRecharge();
	BOOL		IsSkillGuageFull()	{	return m_bSkillGuageFull;	}

	void		ReleaseMoveWithDelay(DWORD delayTime);
	void		CheckDelayRelease();

	void		NetworkMsgParse(BYTE Protocol, void* pMsg);

	void		UpdateCurPetInfo();
	void		UpdateLogoutToDB();

	void		SetSommonPetStamina(BYTE bFlag);
	int			SetSommonPetFriendship(DWORD dwFriendship);
	void		SetSommonPetSkillReady();

	void		SetPetBuffInfo();
	void		GetPetBuffResultRt(int BuffKind, void* Data);
	void		RefleshPetMaintainBuff();


	void		SetPetEquipOption(DWORD ItemIdx, BOOL bAddOpt);

	PETEQUIP_ITEMOPTION*	GetPetEquipOption()	{	return &m_PetEquipOption;	};

	void		SetPetSummonning(int flag);
	void		CheckSummonningPet();

	void		SetResummonCheckTime();
	BOOL		CheckResummonAvailable();

	void		SendPetInfo(PET_TOTALINFO* pPetInfo);

	void		SetReduceAmountPetFriendship(CObject* pAttacker);
	void		ReducePetFriendshipWithMastersRevive();
};


