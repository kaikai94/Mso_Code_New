




// PKManager.h: interface for the CPKManager class.
//

//////////////////////////////////////////////////////////////////////


#if !defined(AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_)
#define AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer;

#define PKMGR CPKManager::GetInstance()
struct PANELTYINFO
{
	char wFame[64];
	WORD wDestroyRatio;
	WORD wMinDestroy;
	WORD wMaxDestroy;
};
enum eBADFAMELEVE
{
	eBadFameLeve0_50,
	eBadFameLeve50_100,
	eBadFameLeve100_500,
	eBadFameLeve500_1000,
	eBadFameLeve1000,
	eBadFameLeveNull,
	eBadFameLeveMax,
};
class CPKManager  
{
	PANELTYINFO m_PaneltyInfo[eBadFameLeveMax];
	eBADFAMELEVE m_BadFameLevel;
protected:
	BOOL	m_bPKAllow[MAX_CHANNEL_NUM];
public:
	CPKManager();

	virtual ~CPKManager();

	MAKESINGLETON( CPKManager );
	void LoadPaneltyInfo();
	void Init( BOOL bPKAllow );
	void InitForChannel();

	BOOL IsPKAllow( DWORD dwChannel )
	{
		if( dwChannel < 1 )
			dwChannel = 1;
		return m_bPKAllow[dwChannel-1];
	}
	void SetPKAllowWithMsg( BOOL bAllow );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void PKModeExitPanelty( CPlayer* pPlayer, BOOL bPKModePanelty = FALSE );
	void SetNormalExitPanelty(CPlayer* pPlayer);
	void DiePanelty( CPlayer* pDiePlayer, CObject* pAttacker );

	void PKPlayerDiePanelty( CPlayer* pPlayer );
	void PKPlayerKillPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker );
	void PKPlayerKiiledByWantedPanelty( CPlayer* pDiePlayer, CPlayer* pAttacker );
	// [新增PK惩罚控制] [by:十里坡剑神][QQ:112582793][2018/11/25]
	BOOL GetPKPaneltyInfo(DWORD dwBadFame);
};

#endif // !defined(AFX_PKMANAGER_H__7471A7CD_E158_427A_B1A1_6E6943F84662__INCLUDED_)


