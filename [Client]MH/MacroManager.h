



// MacroManager.h: interface for the CMacroManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)
#define AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MACROMGR USINGTON(CMacroManager)

class CKeyboard;

enum eMacroEvent {	
	
	ME_TOGGLE_EXITDLG,
	ME_USE_QUICKITEM01,
	ME_USE_QUICKITEM02,
	ME_USE_QUICKITEM03,
	ME_USE_QUICKITEM04,
	ME_USE_QUICKITEM05,
	ME_USE_QUICKITEM06,
	ME_USE_QUICKITEM07,
	ME_USE_QUICKITEM08,
	ME_USE_QUICKITEM09,
	ME_USE_QUICKITEM10,
	ME_TOGGLE_MUGONGDLG,
	ME_TOGGLE_INVENTORYDLG,
	ME_TOGGLE_CHARACTERDLG,
	ME_TOGGLE_MUNPADLG,
	ME_TOGGLE_MINIMAP,
	ME_TOGGLE_QUESTDLG,
	ME_TOGGLE_OPTIONDLG,
	ME_TOGGLE_FRIENDLIST,
	ME_SEND_MEMO,
	ME_TOGGLE_MOVEMODE,
	ME_SELECT_QUICKSLOT1,
	ME_SELECT_QUICKSLOT2,
	ME_SELECT_QUICKSLOT3,
	ME_SELECT_QUICKSLOT4,

	ME_TOGGLE_PEACEWARMODE,

	ME_TOGGLE_AUTOATTACK,

	ME_TOGGLE_AUTOATTACK2,

	ME_TOGGLE_HELP,
	
	ME_TOGGLE_CAMERAVIEWMODE,

	ME_SCREENCAPTURE,

	ME_SHOWALLNAME_ON,
	ME_SHOWALLNAME_OFF,

	ME_CHANGE_WHISPERLIST_UP,
	ME_CHANGE_WHISPERLIST_DOWN,
	
	ME_TOGGLE_BIGMAP,
	ME_MUSSANG_ON,

	ME_TOGGLE_KEY_SETTING_TIP_SHOW,
	ME_TOGGLE_KEY_SETTING_TIP_HIDE,

	ME_TOGGLE_TITANINVENTORYDLG,	
	ME_TOGGLE_GOLDSHOP,
	ME_COUNT,				
};

enum eMacroPressEvent
{
	MPE_CAMERAMOVE_ZOOMIN,
	MPE_CAMERAMOVE_ZOOMOUT,
	
	MPE_CAMERAMOVE_UP,

	MPE_CAMERAMOVE_DOWN,
	MPE_CAMERAMOVE_RIGHT,
	MPE_CAMERAMOVE_LEFT,
	
	MPE_COUNT,
};


enum eSysKey {

	MSK_NONE	= 1,
	MSK_CTRL	= 2,
	MSK_ALT		= 4,
	MSK_SHIFT	= 8,
	MSK_ALL		= MSK_NONE | MSK_CTRL | MSK_ALT | MSK_SHIFT,
};

//////////////
//매크로구조체
struct sMACRO
{
	int		nSysKey;	//조합키
	WORD	wKey;		//실행키
	BOOL	bAllMode;	//채팅모드에 관계없이 되어야하는 것체크
	BOOL	bUp;		//up에 이벤트가 발생해야 할것
};


enum eMacroMode {


	MM_CHAT,					//채팅 우선모드
	MM_MACRO,					//단축키 우선모드

//	MM_DEFAULT_CHAT = 0,		//디폴트 채팅우선모드
//	MM_DEFAULT_GENERAL,			//디폴트 일반모드

//	MM_USERSET_CHAT,			//유저셋 채팅우선모드
//	MM_USERSET_GENERAL,			//유저셋 일반모드

	MM_COUNT,
};




class CMacroManager
{
#ifdef _GMTOOL_
	WORD m_idx;
#endif
protected:

	sMACRO		m_DefaultKey[MM_COUNT][ME_COUNT];	
	sMACRO		m_MacroKey[MM_COUNT][ME_COUNT];		


	sMACRO		m_DefaultPressKey[MM_COUNT][MPE_COUNT];		
	sMACRO		m_MacroPressKey[MM_COUNT][MPE_COUNT];	


	int			m_nMacroMode;					
	BOOL		m_bPause;
	BOOL		m_bChatMode;
	BOOL		m_bInVisibleCursor;

protected:
	BOOL LoadMacro( LPCTSTR strPath );
	BOOL SaveMacro( LPCTSTR strPath );
	void SetDefaultMacro( int nMacroMode );


public:
	CMacroManager();
	virtual ~CMacroManager();

	BOOL IsVisibleCursor() { return !m_bInVisibleCursor; }

	void Init();
	void KeyboardInput( CKeyboard* keyInfo );
	void KeyboardPressInput( CKeyboard* keyInfo );
	void LoadUserMacro();
	void SaveUserMacro();

	void GetMacro( int nMacroMode, sMACRO* pMacro );		
	void SetMacro( int nMacroMode, sMACRO* pMacro );		

	sMACRO* GetCurMacroKey( int nMacroEvent )	
	{ 
		if( nMacroEvent >= ME_COUNT )
			return NULL;
		
		return &m_DefaultKey[m_nMacroMode][nMacroEvent];	
	}
	
	void GetDefaultMacro( int nMacroMode, sMACRO* pMacro );

	int GetMacroMode() { return m_nMacroMode; }
	void SetMacroMode( int nMacroMode );


	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	int IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode );
	BOOL IsChatMode()	{ return m_bChatMode; }

	void PlayMacro( int nMacroEvent );
	void PlayMacroPress( int nMacroEvent );
};

EXTERNGLOBALTON(CMacroManager)

#endif // !defined(AFX_MACROMANAGER_H__A85EAD4A_B009_45AE_AC28_B00E31856F59__INCLUDED_)


