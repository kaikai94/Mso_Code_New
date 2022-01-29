




// TitanManager.h: interface for the CTitanManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(TitanManager_H)
#define TitanManager_H


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "HashTable.h"
#include "GameResourceStruct.h"

enum eTitanNotifyMsg {
	TNM_MASTER_LIFE,

	TNM_EQUIP_ENDURANCE,
	TNM_EQUIP_NUM,
	TNM_CANNOT_DO,

};
enum eTitanRepairTableType { INVEN, TITAN };

#define TITANMGR USINGTON(CTitanManager)


#define MAX_TITANGRADE	3
#define TITAN_EQUIPITEM_ENDURANCE_MAX	10000000	// õ��

//#define TITAN_EFFECT_MAINTAIN	3620
#define TITAN_EFFECT_RIDEIN	3624
#define TITAN_EFFECT_GETOFF	3625
#define TITAN_EFFECT_FORCED_GETOFF 3626

enum	GetOffKindofReason{eNormal, eFromUser, eMasterLifeRate, eExhaustFuel, eExhaustSpell,};

class CMHFile;
class ITEM_MIX_INFO;
class ITEM_MIX_RES;
class ITEM_MIX_MATERIAL;
class CVirtualItem;
class CTitan;
class CItem;


class CTitanManager  
{

	CTitan*									m_pCurRidingTitan;
	QSTATETYPE									m_dwRecallCheckTime;	//����15��

	BOOL									m_bAvaliableEndurance;	// ������ ��ȿ�� �˻縦 ���� ���º���.

	DWORD									m_TitanBreakRate;	// Ÿ��ź ���� Ȯ��
	DWORD									m_RegistedTitanItemDBIdx;	// ��ϵ� Ÿ��ź�������� DB�ε���
	CItem*									m_TitanRefairItem;			// ������ Ÿ��ź ������

	titan_calc_stats						m_TitanStats;

	CYHHashTable<ITEM_MIX_INFO>				m_TPMItemTable;

	CMemoryPoolTempl<TITAN_PARTS_KIND>*		m_TitanPartsKindPool;
	CYHHashTable<TITAN_PARTS_KIND>			m_TitanPartsKindTable;

	CYHHashTable<TITAN_UPGRADEINFO>			m_TitanUpgradeInfoTable;

	CYHHashTable<TITAN_BREAKINFO>			m_TitanBreakInfoTable;

//// Ÿ��ź ����
	CYHHashTable<TITAN_TOTALINFO>				m_TitanInfoList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemEnduranceList;
	CYHHashTable<TITAN_ENDURANCE_ITEMINFO>		m_ItemUsingEnduranceList;

	stTIME									m_serverT;	// ���� �α��� �ð�. GAMEIN �� �ʱ�ȭ.

	TITAN_SHOPITEM_OPTION m_TitanShopitemOption;	// magi82(26)

public:

	CTitanManager();
	virtual ~CTitanManager();
	void	TitanMgrInit();
	void	TitanMgrRelease();
	void	Release();

	// ���� ���� ���̺� ���� �Լ�
	void	LoadTPMList();
	void	LoadTPMResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	ITEM_MIX_INFO * GetTPMItemInfo(WORD wItemIdx);

	// ���� �� ���̺� ���� �Լ�
	BOOL LoadTitanPartsKindList();
	TITAN_PARTS_KIND* GetTitanPartsKind(DWORD partsIdx);

	// Ÿ��ź ���׷��̵� ���̺� ���� �Լ�
	void	LoadTitanUpgradeInfoList();
	void	LoadTitanUpgradeInfoResultItemInfo(CMHFile * fp, TITAN_UPGRADE_MATERIAL * pMaterialInfo);
	TITAN_UPGRADEINFO * GetTitanUpgradeInfoItemInfo(DWORD titanIdx);

	// Ÿ��ź ���� ���̺� ���� �Լ�
	void	LoadTitanBreakInfoMaterialInfo(CMHFile * fp, TITAN_BREAK_MATERIAL * pMaterialInfo);
	void	LoadTitanBreakInfoList();
	TITAN_BREAKINFO * GetTitanBreakInfoItemInfo(DWORD dwIdx);

	void	SetCurRidingTitan(CTitan* pTitan)	{	m_pCurRidingTitan = pTitan;	}
	CTitan*	GetCurRidingTitan()		{	return m_pCurRidingTitan;	}
	DWORD	GetCallItemDBIdx();

	void	ChangeTitanGuageFuel(int changeAmount);
	void	ChangeTitanGuageSpell(int changeAmount);
	// Ÿ��ź ��ȯ/���� ����
	BOOL	CheckRecallAvailable();
	void	SetRecallRemainTime(DWORD RecallState, DWORD stTimeValue = 0);	// magi82(18)
	BOOL	CheckTimeRecallAvailable();

	// magi82 - Titan(070604)
	stTIME	GetTitanResummonTime();

	// Ÿ��ź ��� ����
	BOOL	IsRegistedTitan(DWORD dwCallItemDBIdx);

	// Ÿ��ź ���׷��̵� ����
	DWORD	GetTitanGrade(DWORD dwCallItemDBIdx);

	//// Ÿ��ź ����
	void	InitTitanInfo(TITAN_TOTALINFO* pTitanInfo, int num);
	void	AddTitanInfo(TITAN_TOTALINFO* pTitanInfo);
	void	RemoveTitanInfo(DWORD dwCallItemDBIdx);
	void	ReleaseTitanInfoList();
	TITAN_TOTALINFO* GetTitanInfo(DWORD dwCallItemDBIdx);
	void	UpdateCurTitanInfo(CTitan* pTitan);
	void	OnTitanRemove(CTitan* pTitan);

	// Ÿ��ź ��� ������ ����
	void	InitTitanEquipItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo, int num);
	void	AddTitanEquipItemEnduranceInfo(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	void	RemoveTitanEquipInfo(DWORD dwEquipItemDBIdx);
	void	ReleaseTitanEquipInfo();
	TITAN_ENDURANCE_ITEMINFO* GetTitanEnduranceInfo(DWORD dwEquipItemDBIdx);

	// Ÿ��ź ����� ���� ������ ���� (for DBUpdate)
	void	AddTitanUsingEquipItemList(TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo);
	void	RemoveTitanUsingEquipItemList(DWORD equipItemDBIdx);

	BOOL	CheckUsingEquipItemNum();
	BOOL	CheckUsingEquipItemEndurance();
	BOOL	IsAvaliableEndurance()	{	return m_bAvaliableEndurance;	}

	int		GetTitanEquipTotalInfo(DWORD* temp);	// magi82 - Titan(070423)
	void	SetTitanEnduranceView( DWORD dwEquipItemDBIdx );// magi82 - Titan(070424)
	void	SetTitanTotalEnduranceView();			// magi82 - Titan(070424)


	BOOL	CheckMasterLifeAmountForRiding();


	// magi82 - Titan(070418)
	DWORD	GetRegistedTitanItemDBIdx()	{	return m_RegistedTitanItemDBIdx;	}
	void	SetRegistedTitanItemDBIdx(DWORD dwIdx)	{	m_RegistedTitanItemDBIdx = dwIdx;	}

	void	SetTitanStats(titan_calc_stats* pStats)	{	m_TitanStats = *pStats;	}
	titan_calc_stats*	GetTitanStats()	{	return &m_TitanStats;	}

	void	CheckNotifyMsg(int checkKind);	//eTitanNotifyMsg
	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	// magi82 - Titan(070418)
	CItem*	GetTitanRepairItem()	{	return m_TitanRefairItem;	}
	void	SetTitanRepairItem( CItem* pItem )	{	m_TitanRefairItem = pItem;	}

	// magi82 - Titan(070515)
	void SetTitanRepairTotalEquipItemLock( BOOL bFlag );

	// magi82 - Titan(070619) - ������ ���� �Լ�(TRUE:������, FALSE:������ ������ ��󳻱�)
	DWORD GetTitanEnduranceTotalInfo(MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* msg, BOOL bFlag = FALSE);

	void SetTitanPaperDelayTime();

	void SetServerLogInTime();

	// magi82(26)
	void	SetTitanShopitemOption(DWORD dwItemIdx, BOOL bAdd);
	TITAN_SHOPITEM_OPTION* GetTitanShopitemOption()	{	return &m_TitanShopitemOption;	};
};
EXTERNGLOBALTON(CTitanManager)
#endif // !defined(TitanManager_H)



