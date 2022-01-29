




#pragma once

#include "./Interface/cImage.h"


#define PETMGR	USINGTON(CPetManager)

#define PET_DIST_CHECKTIME 500
#define PET_MAX_FRIENDSHIP		10000000
#define PET_SKILLCHARGE_CHECKTIME	1000
#define PET_MAX_SKILL_CHARGE	10000
#define PET_RESUMMON_VALID_TIME	30000

#define MAX_PET_BUFF_NUM	3

#define CRISTMAS_EVENTPET 8


enum ePetFeedResult{ePFR_Sucess=0, ePFR_Unsummoned, ePFR_StaminaFull};

enum ePetUpgradeDlgGrade{ePUDG_Default, ePUDG_Grade2=1, ePUDG_Grade3=2};

enum ePetRevivalDlgGrade{ePRDG_Default, ePRDG_Grade1, ePRDG_Grade2, ePRDG_Grade3, ePRDG_Grade_All};	//ePRDG_Grade_All => ShopItem 侩

//滚橇

enum ePetBuffKind
{
	ePB_None,
	ePB_Demage_Percent = 1,			//[增加攻击力][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:41]		
	ePB_Dodge = 2,					//[闪避][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:35]				
	ePB_MasterAllStatUp = 3,		//[所有属性增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:29]	
	ePB_Item_DoubleChance = 4,		//[获得物品机率提升][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:23]	
	ePB_NoForeAtkMonster = 5,		//[安抚主动怪物][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:17]
	ePB_ReduceCriticalDmg = 6,		//[减少受到奋力一击伤害][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:09]	
	ePB_MasterAllStatRound = 7,		//[属性调整][BY:十里坡剑神][QQ:112582793][2019-9-11][18:04:02]	
	ePB_Item_RareProbUp = 8,		//[获得祝福装备机率增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:03:55]
	ePB_MussangTimeIncrease = 9,	//[无双模式维持时间增加][BY:十里坡剑神][QQ:112582793][2019-9-11][18:03:48]
	ePB_ReduceCriticalRate = 10,	//[减少对方一击概率][BY:十里坡剑神][QQ:112582793][2019-9-11][16:14:55]
	ePB_ReduceDemageRate = 11,		//[减少受到伤害的百分比][BY:十里坡剑神][QQ:112582793][2019-9-11][17:02:47]
	ePB_ReduceCriticalDmgPlayer = 12,		//[减少受到玩家一击伤害百分比][BY:十里坡剑神][QQ:112582793][2019-9-11][18:02:35]
	ePB_Kind_Max
};

enum {ePetRestBtn,ePetUseBtn};

//脐辆幅
enum ePetKind{ePK_None, ePK_CommonPet=1, ePK_ShopItemPet=2, ePK_EventPet=4,};

class CPet;

class cDialog;
class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;

class CPetManager
{
	CYHHashTable<PET_TOTALINFO>		m_PetInfoList;
	CYHHashTable<cImage>			m_PetImageList;

	DWORD				m_dwStateCheckTime;				
	DWORD				m_dwStaminaDecrease;		
	DWORD				m_dwOldFriendShipForToolTipChange;

	DWORD				m_dwDistCheckTime;					//橇肺技胶 鸥烙(林牢-脐芭府眉农)
	BOOL				m_bValidDistance;				//框流咯具 瞪 芭府牢啊
	BOOL				m_bReadytoMove;					//框流咯档 瞪 炼扒牢啊(movemgr俊辑 技泼)

	DWORD				m_dwSkillRechargeCheckTime;		//胶懦霸捞瘤 眉农 鸥烙(1檬)
	DWORD				m_dwSkillRechargeAmount;
	BOOL				m_bSkillGuageFull;
	BOOL				m_bReadyToSendSkillMsg;

	DWORD				m_dwResummonDelayTime;			//脐 犁家券 饭捞//豪牢饶 30檬 墨款飘

	CPet*				m_pCurSummonPet;
	BOOL				m_bIsPetStateDlgToggle;			//配臂 惑怕 历厘
	cDialog*			m_pCurPetStateDlg;				//泅犁 脐 惑怕芒
	CPetStateDlg*		m_pStateDlg;					//脐 惑怕奴芒

	CPetStateMiniDlg*	m_pStateMiniDlg;				//脐 惑怕累篮芒
	CPetInventoryDlg*	m_pInvenDlg;
	BOOL				m_bReadyToSendRestMsg;			//脐 绒侥 惑怕 汲沥沥焊 辑滚 府畔 第 TRUE

	WORD				m_wPetKind;						//脐辆幅. 老馆/酒捞袍隔/捞亥飘 脐

public:
	CPetManager(void);
	~CPetManager(void);

	void	PetMgrInit();
	void	PetMgrRelease();
	void	PetMgrProcess();
	void	AddHeroPet(CPet* pPet);
	void	InitPetInfo(PET_TOTALINFO* pPetInfo, int num);
	void	AddPetInfo(PET_TOTALINFO* pPetInfo);
	void	RemovePetInfo(DWORD dwSummonItemDBIdx);
	void	ReleasePetInfoList();
	PET_TOTALINFO*	GetPetInfo(DWORD dwItemDBIdx);
	void	UpdateCurPetInfo(CPet* pPet);
	void	OnPetRemove(CPet* pPet);

	void	SetResummonCheckTime();
	BOOL	CheckResummonAvailable();
	DWORD	GetPetResummonRestTime();

	BOOL	CheckDefaultFriendship(DWORD dwItemDBIdx);
	DWORD	GetPetFriendship(DWORD dwItemDBIdx);
	BOOL	IsCurPetStaminaFull();
	BOOL	IsCurPetSummonItem(DWORD dwItemDBIdx);
	void	RemovePetFromTable(DWORD dwPetID);
	void	SetCurSummonPet(CPet* pPet)	{	m_pCurSummonPet=pPet;	}
	CPet*	GetCurSummonPet() {	return m_pCurSummonPet;	}
	CPet*	GetHeroPet(DWORD dwPetID);
	DWORD	GetHeroPetID();
	BOOL	CheckPetSummoned();
	BOOL	CheckPetAlive(DWORD dwSummonItemDBIdx);
	void	ReleasePetTable();

	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	void	CalcSkillRecharge();
	void	SetSkillRechargeAmount(DWORD amount);
	BOOL	IsSkillGuageFull()	{	return m_bSkillGuageFull;	}
	void	SetSkillGuageFull(BOOL bVal)	{m_bSkillGuageFull = bVal;}

	//蝶扼啊扁
	void	CheckDistWithMaster();
	BOOL	IsValidDist() {	return m_bValidDistance;	}
	void	SetMoveReady(BOOL bVal)	{	m_bReadytoMove = bVal;	}

	//UI
	void	OpenPetStateDlg();		//泅犁 脐 惑怕芒 凯扁
	void	OpenPetInvenDlg();
	void	SetCurPetStateDlg(cDialog* pDlg) {m_pCurPetStateDlg=pDlg;}	//泅犁 脐 惑怕芒 技泼

	void	SetPetStateDlg(CPetStateDlg* pDlg) {m_pStateDlg=pDlg;}
	void	SetPetStateMiniDlg(CPetStateMiniDlg* pDlg) {m_pStateMiniDlg=pDlg;}
	void	SetPetInventoryDlg(CPetInventoryDlg* pDlg) {m_pInvenDlg=pDlg;}
	//060310 UI 历厘
	void	SetPetDlgToggle(BOOL bVal)	{	m_bIsPetStateDlgToggle = bVal;	}	//FALSE 啊 奴芒(扁夯)
	BOOL	GetPetDlgToggle()	{	return m_bIsPetStateDlgToggle;	}
	cDialog* GetCurPetStateDlg()	{	return m_pCurPetStateDlg;	}

	void	TogglePetStateDlg();
	void	SetPetStateDlgInfo(CPet* pPet);
	void	SetPetStateMiniDlgInfo(CPet* pPet);
	void	SetPetStateDlgUseRestInfo(CPet* pPet);
	void	ClosePetAllDlg();

	void	SetPetGuageText(DWORD dwStamina, DWORD dwFriendShip);

	DWORD	GetPetValidInvenMaxTabNum();
	void	SetPetValidInvenTab();

//	BOOL	GetPetDlgToggle() {return m_bIsPetStateDlgToggle;}			//配臂惑怕啊廉坷扁. 奴芒/累篮芒
	
	//rest
	void	SetCurPetRest(BOOL bRest);
	void	SendPetRestMsg(BOOL bRest);//脐 荤侩/绒侥 沥焊 焊郴扁
	void	SetReadyToSendRestMsg(BOOL bReady) {	m_bReadyToSendRestMsg = bReady;	}
	BOOL	IsReadyToSendRestMsg()	{	return m_bReadyToSendRestMsg;	}

	//seal

	void	SendPetSealMsg();

	//ability icon. pet skill
	void	CheckRestNSkillUse();	//公傍芒 胶懦滚瓢 扁瓷. 绒侥惑怕搁 秦力矫虐绊 霸捞瘤钱捞搁 胶懦 荤侩. 扁裙磊俊霸 巩狼饶 舅霸凳.

	//skill
	void	InitPetSkillGuage();
	void	UseCurPetSkill();
	void	SetReadyToSendSkillMsg(BOOL bReady) {	m_bReadyToSendSkillMsg = bReady;	}
	BOOL	IsReadyToSendSkillMsg()	{	return m_bReadyToSendSkillMsg;	}
	//SW060324 脐 滚橇 眠啊
	void	AddMasterStatFromPetBuff(player_calc_stats* pMasterAdditionalStat);
	void	RefleshPetMaintainBuff();

	//pet equip
	WORD	GetPetEquipItemMax(DWORD dwItemDBIdx);

	void	LoadPetImage();
	cImage* GetPetImage(DWORD dwPetIdx);
	void	RemovePetImage();

	void	Release();

	void	SetCurSummonPetKind(CPet* pPet);
	BOOL	CheckCurSummonPetKindIs(int kind);
	WORD	GetPetSummon();
};

EXTERNGLOBALTON(CPetManager)

//UI 沥焊包府
//HERO's Pet 包府, PetObject绰 窍唱父 乐促. 葛电 家蜡 脐甸狼 扁夯沥焊父 包府.
//HERO狼 脐 家券 酒捞袍俊 啊瘤绊 乐绰 脐 酒捞袍狼 脐 沥焊甫 啊瘤绊 乐促. 酒捞袍 砒屏 殿俊 焊咯霖促.


