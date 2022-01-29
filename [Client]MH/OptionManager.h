



// OptionManager.h: interface for the COptionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_)
#define AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OPTIONMGR USINGTON(COptionManager)

class COptionDialog;
class cGuageBar;

struct sGAMEOPTION
{
	BOOL bNoDeal;	
	BOOL bNoParty;	
	BOOL bNoFriend;	
	BOOL bNoVimu;	

	BOOL bNameMunpa;
	BOOL bNameParty;
	BOOL bNameOthers;
	BOOL bNoMemberDamage;
	BOOL bNoGameTip;
	BOOL bMunpaIntro;	

	int nMacroMode;

	BOOL bNoWhisper;	
	BOOL bNoChatting;
	BOOL bNoBalloon;
	BOOL bAutoHide;
	BOOL bNoShoutChat;
	BOOL bNoGuildChat;
	BOOL bNoAllianceChat;
	BOOL bNoSystemMsg;
	BOOL bNoExpMsg;
	BOOL bNoItemMsg;
	BOOL bNoTipMsg;
	int nGamma;
	int nSightDistance;

	BOOL bGraphicCursor;	

	BOOL bShadowHero;
    BOOL bShadowMonster;
	BOOL bShadowOthers;

	BOOL bAutoCtrl;
	int nLODMode;
	int nEffectMode;
	int nEffectSnow; 

	BOOL bSoundBGM;				
	BOOL bSoundEnvironment;		
	int	nVolumnBGM;				
	int nVolumnEnvironment;		
	BOOL bAmbientMax;
	int nLoginCombo;	
	BOOL bIntroFlag;	
	BOOL bNoAvatarView;
	BOOL bShowPet;     
	BOOL bShowNpc;     
	BOOL bShowMonster; 
};


class COptionManager  
{
protected:


	sGAMEOPTION		m_GameOption;

	BOOL			m_bBGMSoundChanged;
	WORD			m_wSendOption;

	BOOL			m_bShowMenberDamage;

	COptionDialog*	m_pOptionDlg;
	cGuageBar*		m_pCammaGB;		
	cGuageBar*		m_pSightGB;
	LONG			m_SigntGBMinValue;
	LONG			m_SigntGBMaxValue;
	DWORD			m_dwShdowOption;
	enum {LOWEST=1,LOW,MID,HIGH,STANDARD_FRAME=15,NICE_FRAME=20};
	DWORD			m_dwLastCheckTime;
	DWORD			m_dwCheck;
	DWORD			m_dwFrameAverage;
	DWORD			m_dwCountFrame;
	int				m_nCurOptionState;

	
protected:

	BOOL LoadGameOption();
	WORD MakeOptionFlag();

public:


	//MAKESINGLETON(COptionManager);
	
	COptionManager();
	virtual ~COptionManager();

	void Init();

	void SaveGameOption();
	void SetDefaultOption();

	void ApplySettings();
	void CancelSettings();

	void SendOptionMsg();

	sGAMEOPTION* GetGameOption() { return &m_GameOption; }

	void SetGameOption( sGAMEOPTION* pOption );


//parsing
	void NetworkMsgParse( BYTE Protocol, void* pMsg );



	void SetMemberDemageActive(BOOL bDo) {m_bShowMenberDamage = bDo;}
	BOOL IsShowMemberDamage() {return m_bShowMenberDamage;}	

	void GraphicAutoCtrlProcess();	
	void ApplyGraphicSetting();
	void InitForGameIn();
};


EXTERNGLOBALTON(COptionManager);

#endif // !defined(AFX_OPTIONMANAGER_H__16984CD0_B39F_40C4_AA38_3BCF9894A020__INCLUDED_)



