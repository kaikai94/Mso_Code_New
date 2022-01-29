



#ifndef _CHATMANAGER_H

#define _CHATMANAGER_H


#define CHATMGR USINGTON(CChatManager)

#define MAX_CHATMSGNUM	2500   // Change The MsgNum  2000->2500

class CKeyboard;

#include "ChatDialog.h"
#include <hash_map>
enum eTextClass
{
	CTC_OPERATOR,		
	CTC_GENERALCHAT,	
	CTC_PARTYCHAT,		
	CTC_GUILDCHAT,	
	CTC_ALLIANCE,	
	CTC_SHOUT,	
	CTC_TIPS,			
	CTC_WHISPER,				
	CTC_MONSTERSPEECH,
	CTC_MONSTERSHOUT,
	CTC_ATTACK,						
	CTC_ATTACKED,		
	CTC_DEFENCE,		
	CTC_DEFENCED,		
	CTC_KILLED,			
	CTC_MPWARNING,		
	CTC_HPWARNING,		
	CTC_GETITEM,		
	CTC_GETMONEY,		
	CTC_GETEXP,
	CTC_GETABILITYEXP,
	CTC_ABILITYUPGRADE_SUCCESS,
	CTC_ABILITYUPGRADE_FAILED,
	CTC_SYSMSG,			
	CTC_TOGM,			
	CTC_TOWHOLE,
	CTC_TOPARTY,
	CTC_TOGUILD,
	CTC_CHEAT_1,
	CTC_CHEAT_2,
	CTC_CHEAT_3,
	CTC_CHEAT_4,
	CTC_CHEAT_5,
	CTC_QUEST,
	CTC_GMCHAT,
	CTC_BILLING,	
	CTC_TRACKING,
	CTC_GTOURNAMENT,
	CTC_JACKPOT,
	CTC_GUILD_NOTICE,
	CTC_SURVIVAL,
	CTC_ALERT_YELLOW,
	CTC_ALERT_RED,
	CTC_AI_TIP,
	CTC_TIP_MONSTER,
	CTC_TIP_ITEM,
	CTC_TIP_KILL,
	CTC_TIP_GG,
	CTC_COUNT,	
};


const DWORD dwChatTextColor[CTC_COUNT] = {

	RGBA_MAKE(0, 255, 60, 255),			//CTC_OPERATOR
	RGBA_MAKE(255, 255, 255, 255),		//CTC_GENERAL
	RGBA_MAKE(185, 241, 69, 255),		//CTC_PARTYCHAT
	RGBA_MAKE(1, 209, 247, 255),		//CTC_GUILDCHAT
	RGBA_MAKE(240, 255, 0, 255),		//CTC_ALLIANCE
	RGBA_MAKE(247, 182, 24, 255),		//CTC_SHOUT
	RGBA_MAKE(2, 255, 163, 255),		//CTC_TIPS
	RGBA_MAKE(235, 195, 206, 255),		//CTC_WHISPER
	RGBA_MAKE(255, 255, 255, 255),		//CTC_MONSTERSPEECH
	RGBA_MAKE(247, 182, 24, 255),		//CTC_MONSTERSHOUT
	RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACK
	RGBA_MAKE(255, 97, 19, 255),		//CTC_ATTACKED
	RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCE
	RGBA_MAKE(255, 97, 19, 255),		//CTC_DEFENCED
	RGBA_MAKE(255, 97, 19, 255),		//CTC_KILLED
	RGBA_MAKE(255, 52, 19, 255),		//CTC_MPWARNING
	RGBA_MAKE(255, 52, 19, 255),		//CTC_HPWARNING
#ifdef _NEWCHATMODE
	RGBA_MAKE(2, 255, 163, 255),		//CTC_GETITEM
	RGBA_MAKE(255, 255, 255, 255),		//CTC_GETMONEY
	RGBA_MAKE(253, 55, 106, 255),		//CTC_GETEXP
	RGBA_MAKE(252,255,110, 255),		//CTC_GETABILITYEXP
#else
	RGBA_MAKE(2, 255, 163, 255),		//CTC_GETITEM
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETMONEY
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETEXP
	RGBA_MAKE(200, 200, 200, 255),		//CTC_GETABILITYEXP
#endif
	RGBA_MAKE(82, 255, 204, 255),		//CTC_ABILITYUPGRADE_SUCCESS,
	RGBA_MAKE(82, 255, 204, 255),		//CTC_ABILITYUPGRADE_FAILED,
	RGBA_MAKE(82, 255, 204, 255),		//CTC_SYSMSG
	RGBA_MAKE(82, 255, 204, 255),		//CTC_TOGM
	RGBA_MAKE(171, 195, 255, 255),		//CTC_TOWHOLE
	RGBA_MAKE(255, 158, 6, 255),		//CTC_TOPARTY
	RGBA_MAKE(240, 255, 0, 255),		//CTC_TOGUILD
	RGBA_MAKE(255, 0, 0, 255),			//CTC_CHEAT_1
	RGBA_MAKE(255, 255, 0, 255),		//CTC_CHEAT_2
	RGBA_MAKE(255, 0, 255, 255),		//CTC_CHEAT_3
	RGBA_MAKE(0, 255, 255, 255),		//CTC_CHEAT_4
	RGBA_MAKE(100, 50, 100, 255),		//CTC_CHEAT_5
	RGBA_MAKE( 0, 255, 255, 255 ),		//CTC_QUEST
	RGBA_MAKE( 28, 233, 151, 255 ),		//CTC_GMCHAT
	RGBA_MAKE(0, 255, 60, 255),			//CTC_BILLING
	RGBA_MAKE(255, 20, 20, 255),		//CTC_TRACKING
	RGBA_MAKE(20, 255, 60, 255),		//CTC_GTOURNAMENT
	RGBA_MAKE(144, 252, 222, 255),		//CTC_JACKPOT
	RGBA_MAKE(255, 214, 0, 255),		//CTC_GUILD_NOTICE
	RGBA_MAKE(144, 252, 222, 255),		//CTC_SURVIVAL
	RGBA_MAKE(255, 252, 0, 255),		//CTC_ALERT_YELLOW
	RGBA_MAKE(252,255,110, 255),		//CTC_AI_TIP
	RGBA_MAKE(252,255,110, 255),		//CTC_AI_TIP
	RGBA_MAKE(133,253,252,255),			//CTC_TIP_MONSTER
	RGBA_MAKE(0,255,0,255),				//CTC_TIP_ITEM,
	RGBA_MAKE(255,0,0,255),				//CTC_TIP_KILL,
	RGBA_MAKE(255, 214, 0, 255),		//CTC_TIP_GG
};



enum eChatOption
{
	CTO_NOSYSMSG,
	CTO_NOITEMMSG,
	CTO_NOEXPMSG,
	CTO_NOCHATMSG,
	CTO_AUTOHIDECHAT,
	CTO_NOSHOUTMSG,
	CTO_NOGUILDMSG,
	CTO_NOALLIANCEMSG,	
	CTO_COUNT,
};

struct sChatOption
{
	BOOL bOption[CTO_COUNT];
};


struct sSOCIETY_CHAT
{
	int nActNum;
	int nLen;
	char buf[MAX_NAME_LENGTH+1];

};

struct sABILITY_CHAT
{
	int nAbilityIndex;
	char buf[MAX_NAME_LENGTH+1];
};

#define MAX_WHISPERLIST_PER_SIDE 2
enum eCHAT_DELAY
{
	eCHATDELAY_PARTY,
	eCHATDELAY_GUILD,
	eCHATDELAY_UNION,
	eCHATDELAY_SHOUT,
	eCHATDELAY_WHISPER,
	eCHATDELAY_MAX,
};

class CUserCommendParser;

class CChatManager
{

protected:

	BOOL			m_bPause;
	sChatOption		m_ChatOption;
	CChatDialog*	m_pChatDialog;
	CUserCommendParser* m_pUserCommendParser;
	
	typedef DWORD TextIndex;
	typedef stdext::hash_map< TextIndex, std::string > TextContainer;
	TextContainer mTextContainer;
	char			m_NULL;	

	char			m_strLastName[MAX_NAME_LENGTH+2];
	
	cPtrList		m_ListChat[MAX_CHAT_COUNT];
	
	DWORD			m_dwLastChatTime;	
	DWORD			m_dwLastChatDelayTime[eCHATDELAY_MAX];

	cPtrList		m_ListAct;
	cPtrList		m_ListAbility;	

	char			m_WhisperList[MAX_WHISPERLIST_PER_SIDE*2+1][MAX_NAME_LENGTH+1];
	int				m_nWhisperListNum;
	//[角色右键菜单私聊][By:十里坡剑神][QQ:112582793][2017/11/23]
	char			m_WhisperName[MAX_NAME_LENGTH+1];
protected:

	void LoadSocietyActionFilter();
	int CaptureSocietyAction( char* pStr );

	void LoadAbilityFilter();

	void LoadChatMsg();


	void PrintMsg( int nClass, char* msg );

	void AddWhisperSenderList( char* pName );
	void AddWhisperReceiverList( char* pName );
	void RemoveWhisperSenderList( char* pName );
	void RemoveWhisperReceiverList( char* pName );

public:
	//[角色右键菜单添加私聊][By:十里坡剑神][QQ:112582793][2017/11/23]
	void SetWhisperName(char* name);
	void AutoWhisperForRbt();	
	void ChangeWhisperUser( BOOL bUp );

	CChatManager();
	virtual ~CChatManager();

	void Init();

	void Release();
	void MsgProc( UINT msg, WPARAM wParam );

	void SetOption( sChatOption* pChatOption );
	sChatOption* GetOption()					{ return &m_ChatOption; }
	void AddMsg( int nClass, char* str, ... );

	char* GetChatMsg( int nMsgNum );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void SetPause( BOOL bPause ) { m_bPause = bPause; }
	void SetChatDialog( CChatDialog* pChatDlg );
	
	void OnInputChatMsg( char* str,bool bIsItemInfo = false );

	void SendItemInfoMsg(char* ItemName,DWORD dwDBIndex);

	void SetAutoWhisper();

	void AddChatMsg( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddChatMsgAll( DWORD MsgColor, char* str );

	BOOL CanChatTime( char cToken );

	void SaveChatList();
	void RestoreChatList();
	void ReleaseChatList();

	void AddMsgToList( BYTE ChatLimit, DWORD MsgColor, char* str );
	void AddMsgAllToList( DWORD MsgColor, char* str );

};
EXTERNGLOBALTON(CChatManager)

BOOL TargetFunc(char *str);
#endif


