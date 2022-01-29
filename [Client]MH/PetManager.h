




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

enum ePetRevivalDlgGrade{ePRDG_Default, ePRDG_Grade1, ePRDG_Grade2, ePRDG_Grade3, ePRDG_Grade_All};	//ePRDG_Grade_All => ShopItem ��

//����

enum ePetBuffKind
{
	ePB_None,
	ePB_Demage_Percent = 1,			//[���ӹ�����][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:41]		
	ePB_Dodge = 2,					//[����][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:35]				
	ePB_MasterAllStatUp = 3,		//[������������][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:29]	
	ePB_Item_DoubleChance = 4,		//[�����Ʒ��������][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:23]	
	ePB_NoForeAtkMonster = 5,		//[������������][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:17]
	ePB_ReduceCriticalDmg = 6,		//[�����ܵ�����һ���˺�][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:09]	
	ePB_MasterAllStatRound = 7,		//[���Ե���][BY:ʮ���½���][QQ:112582793][2019-9-11][18:04:02]	
	ePB_Item_RareProbUp = 8,		//[���ף��װ����������][BY:ʮ���½���][QQ:112582793][2019-9-11][18:03:55]
	ePB_MussangTimeIncrease = 9,	//[��˫ģʽά��ʱ������][BY:ʮ���½���][QQ:112582793][2019-9-11][18:03:48]
	ePB_ReduceCriticalRate = 10,	//[���ٶԷ�һ������][BY:ʮ���½���][QQ:112582793][2019-9-11][16:14:55]
	ePB_ReduceDemageRate = 11,		//[�����ܵ��˺��İٷֱ�][BY:ʮ���½���][QQ:112582793][2019-9-11][17:02:47]
	ePB_ReduceCriticalDmgPlayer = 12,		//[�����ܵ����һ���˺��ٷֱ�][BY:ʮ���½���][QQ:112582793][2019-9-11][18:02:35]
	ePB_Kind_Max
};

enum {ePetRestBtn,ePetUseBtn};

//������
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

	DWORD				m_dwDistCheckTime;					//���μ��� Ÿ��(����-��Ÿ�üũ)
	BOOL				m_bValidDistance;				//�������� �� �Ÿ��ΰ�
	BOOL				m_bReadytoMove;					//�������� �� �����ΰ�(movemgr���� ����)

	DWORD				m_dwSkillRechargeCheckTime;		//��ų������ üũ Ÿ��(1��)
	DWORD				m_dwSkillRechargeAmount;
	BOOL				m_bSkillGuageFull;
	BOOL				m_bReadyToSendSkillMsg;

	DWORD				m_dwResummonDelayTime;			//�� ���ȯ ����//������ 30�� ī��Ʈ

	CPet*				m_pCurSummonPet;
	BOOL				m_bIsPetStateDlgToggle;			//��� ���� ����
	cDialog*			m_pCurPetStateDlg;				//���� �� ����â
	CPetStateDlg*		m_pStateDlg;					//�� ����ūâ

	CPetStateMiniDlg*	m_pStateMiniDlg;				//�� ��������â
	CPetInventoryDlg*	m_pInvenDlg;
	BOOL				m_bReadyToSendRestMsg;			//�� �޽� ���� �������� ���� ���� �� TRUE

	WORD				m_wPetKind;						//������. �Ϲ�/�����۸�/�̺�Ʈ ��

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

	//���󰡱�
	void	CheckDistWithMaster();
	BOOL	IsValidDist() {	return m_bValidDistance;	}
	void	SetMoveReady(BOOL bVal)	{	m_bReadytoMove = bVal;	}

	//UI
	void	OpenPetStateDlg();		//���� �� ����â ����
	void	OpenPetInvenDlg();
	void	SetCurPetStateDlg(cDialog* pDlg) {m_pCurPetStateDlg=pDlg;}	//���� �� ����â ����

	void	SetPetStateDlg(CPetStateDlg* pDlg) {m_pStateDlg=pDlg;}
	void	SetPetStateMiniDlg(CPetStateMiniDlg* pDlg) {m_pStateMiniDlg=pDlg;}
	void	SetPetInventoryDlg(CPetInventoryDlg* pDlg) {m_pInvenDlg=pDlg;}
	//060310 UI ����
	void	SetPetDlgToggle(BOOL bVal)	{	m_bIsPetStateDlgToggle = bVal;	}	//FALSE �� ūâ(�⺻)
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

//	BOOL	GetPetDlgToggle() {return m_bIsPetStateDlgToggle;}			//��ۻ��°�������. ūâ/����â
	
	//rest
	void	SetCurPetRest(BOOL bRest);
	void	SendPetRestMsg(BOOL bRest);//�� ���/�޽� ���� ������
	void	SetReadyToSendRestMsg(BOOL bReady) {	m_bReadyToSendRestMsg = bReady;	}
	BOOL	IsReadyToSendRestMsg()	{	return m_bReadyToSendRestMsg;	}

	//seal

	void	SendPetSealMsg();

	//ability icon. pet skill
	void	CheckRestNSkillUse();	//����â ��ų��ư ���. �޽Ļ��¸� ������Ű�� ������Ǯ�̸� ��ų ���. ��ȹ�ڿ��� ������ �˰Ե�.

	//skill
	void	InitPetSkillGuage();
	void	UseCurPetSkill();
	void	SetReadyToSendSkillMsg(BOOL bReady) {	m_bReadyToSendSkillMsg = bReady;	}
	BOOL	IsReadyToSendSkillMsg()	{	return m_bReadyToSendSkillMsg;	}
	//SW060324 �� ���� �߰�
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

//UI ��������
//HERO's Pet ����, PetObject�� �ϳ��� �ִ�. ��� ���� ����� �⺻������ ����.
//HERO�� �� ��ȯ �����ۿ� ������ �ִ� �� �������� �� ������ ������ �ִ�. ������ ���� � �����ش�.


