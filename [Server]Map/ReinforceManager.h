




// ReinforceManager.h: interface for the CReinforceManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REINFORCEMANAGER_H__52980508_F9A4_4852_82C0_2E5F39ED8B33__INCLUDED_)
#define AFX_REINFORCEMANAGER_H__52980508_F9A4_4852_82C0_2E5F39ED8B33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
//#include <hash_map>

#endif // _MSC_VER > 1000



#define MAX_REINFORCE		50		
#define MAX_RTL_ABILITY		30
#define MAX_RTL_MATERIAL	100
#define MAX_RTL_ITEMLEVEL	100


#define MATERIAL_TOTALGRAVITY_MAX	100

#define REINFORCEMGR CReinforceManager::GetInstance()


struct sITEM_REINFORCE_INFO;
struct sITEM_RAREREINFORCE_INFO;

class CPlayer;

enum eItemOptionKind{ eIOK_Normal, eIOK_Rare, };


class CReinforceManager  
{
protected:

	float	m_fTableAbility[MAX_RTL_ABILITY][MAX_REINFORCE];

	float	m_fTableMaterial[MAX_RTL_MATERIAL][MAX_REINFORCE];
	float	m_fTableItemLevel[MAX_RTL_ITEMLEVEL][MAX_REINFORCE];
	float	m_fTableAdjust[MAX_REINFORCE];

	CYHHashTable<sITEM_REINFORCE_INFO> m_ReinforceItemInfoList;
	CYHHashTable<sITEM_RAREREINFORCE_INFO> m_RareReinforceItemInfoList;

	DWORD m_dwRareMaterialTotalGravity;
public:

	MAKESINGLETON( CReinforceManager );

	CReinforceManager();
	virtual ~CReinforceManager();

	BOOL Init();
	void Release();

	BOOL LoadReinforceInfo();
	BOOL LoadReinforceTable();
	//BOOL LoadItemLevelLimitInfo();
	//BOOL RLoadItemLevelLimitInfo();
	//WORD GetItemLevelLimit(WORD level);
	sITEM_REINFORCE_INFO* GetReinforceInfo( WORD wItemIdx );

	BOOL CheckValidMaterial( WORD wReinforceItemIdx, WORD wMaterialItemIdx, WORD eItemOption = eIOK_Normal );
	float GetReinforceWithMetariel( ITEM_OPTION_INFO* pOption, DWORD MaterialItemIdx, WORD eItemOption = eIOK_Normal );

	BOOL LoadRareReinforceInfo();	
	sITEM_RAREREINFORCE_INFO* GetRareReinforceInfo( WORD wItemIdx );
	BOOL CheckMaterialTotalGravity(WORD wMaterialItemIdx, WORD wMaterialCount, WORD eItemOption);
	void InitGravity() {m_dwRareMaterialTotalGravity=0;}

	WORD GetValueForAbility( WORD wAbilGrade );	
	WORD GetValueForMaterialNum( WORD wMaterialNum );	
	WORD GetValueForItemLevel( WORD wItemLevel );	
	WORD GetAdjustValue();	
	void ReLoadRareReinforceInfo();
};

#endif // !defined(AFX_REINFORCEMANAGER_H__52980508_F9A4_4852_82C0_2E5F39ED8B33__INCLUDED_)


