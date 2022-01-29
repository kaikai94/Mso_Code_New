



// GuildTournamentMgr.h: interface for the CGuildTournamentMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_)
#define AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\[CC]BattleSystem\GTournament\Battle_GTournament.h"


#define GTMGR CGuildTournamentMgr::GetInstance()
#define GTMAPNUM	28

class CGuild;
class CPlayer;

enum GTFlag{ eGTFlg_Rd/*����*/, eGTFlg_Rgst/*��û*/, eGTFlg_Ing/*����*/};

struct GTInfo
{
	CGuild* pGuild;
	WORD	Ranking;
	WORD	Position;
	WORD	ProcessTournament;		// ������ ��⸦ �����ߴ���
};


class CGuildTournamentMgr
{
	CYHHashTable<CGuild>		m_TournamentCancelList;


	GTInfo						m_GuildInfoList[MAXGUILD_INTOURNAMENT];
	int							m_PositionTable[2][MAXGUILD_INTOURNAMENT/2];
	BATTLE_INFO_GTOURNAMENT		m_BattleInfo[MAXGUILD_INTOURNAMENT/2];
	BATTLE_INFO_GTOURNAMENT		m_EventBattleInfo[MAX_EVENTGT_COUNT];
	DWORD						m_BattleCount;
	stTournamentTime			m_RegistTime[2];
	stTournamentTime			m_TournamentTime[30][eGTT_MAX];
	DWORD						m_RegistedGuildCount;
	DWORD						BattleCount[eGTT_MAX];

	DWORD		m_TournamentCount;			// ���� ��ʸ�Ʈ�ΰ�

	DWORD		m_MaxGroup;
	DWORD		m_CurTournament;			// �����ΰ�
	DWORD		m_TournamentState;			// ��ʸ�Ʈ �����Ȳ

	DWORD		m_EntranceTime;
	DWORD		m_NotifyEntranceTime;
	DWORD		m_FightTime;

	DWORD		m_LeaveTime;
	DWORD		m_RemainTime;
	

	stTIME		m_WaitTime;


	stTIME		m_FirstStartTime;
	
	BOOL		m_Inited;
	BOOL		m_bEvent;

	int			m_EventBattleCount;

	//SW061205 ��� NPC
	WORD		m_wCurDay;
	DWORD		m_dwCurFlag;

	//
	int GetEmptyGuildPosition();

public:
	MAKESINGLETON( CGuildTournamentMgr );

	CGuildTournamentMgr();
	virtual ~CGuildTournamentMgr();

	void Init();
	void Reset();
	void Process();
	

	DWORD AddGuild( CGuild* pGuild, DWORD& Pos );
	DWORD AddedGuild( CGuild* pGuild, DWORD Position );
	void AddPlayer( CPlayer* pPlayer );
	DWORD DeleteAddedGuild(	DWORD GuildIdx );

	DWORD IsRegistedGuid( DWORD GuildIdx );
	int GetCurTournamentGuild( DWORD Group, BOOL bFront );

	void CreateBattle();
	void StartTournament();
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol,void* pMsg );

	DWORD GetRanking( DWORD Ranking, BOOL bWin );
	int GetBattleID( CGuild* pGuild );
	void SetResult( DWORD GuildIdx, BOOL bWin, BOOL bUnearnedWin=FALSE );

	// DB
	void SetTournamentInfo( DWORD Tournament, DWORD CurTournament, DWORD WaitTime );
	void SetGuildInfo( GTDBLoadInfo* pGuildInfo, DWORD Count );
	void AddCancelGuild( DWORD GuildIdx );

	// Event
	DWORD StartTournamentEvent( MSG_GT_EVENTSTART* pEventInfo );
	void AddPlayerEvent( CPlayer* pPlayer );
	int GetBattleIDEvent( CGuild* pGuild );
	void ResetEvent();

	void	CheckDateforFlagNPC();
	DWORD	GetGTState()	{	return m_TournamentState;	}
public:
	void GTouRnament_Registcount(void* pMsg);
	void GTouRnament_Regist_Syn(void* pMsg);
	void GTouRnament_Regist_Ack(void* pMsg);
	void GTouRnament_Regist_Nack(void* pMsg);
	void GTouRnament_RegistCancel_Syn(void* pMsg);
	void GTouRnament_MoveToBattleMap_Syn(void* pMsg);
	void GTouRnament_BattleJoin_Syn(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_ObServerJion_Syn(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_Leave_Syn(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_StandingInfo_Syn(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_BattleInfo_Syn(void* pMsg);
	void GTouRnament_ObServer_Exit(void* pMsg);
	void GTouRnament_RegistAck(void* pMsg);
	void GTouRnament_Regist_Cancel(void* pMsg);
	void GTouRnament_Regist_StateChange(void* pMsg);
	void GTouRnament_CurTournamentChange(void* pMsg);
	void GTouRnament_BattleInfo(void* pMsg);
	void GTouRnament_BatEnd_ToMap(void* pMsg);
	void GTouRnament_Notify_WinLos(void* pMsg);
	void GTouRnament_Win(void* pMsg);
	void GTouRnament_EndBattle(void* pMsg);
	void GTouRnament_Event_Start(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_Event_Satrt_Ack(void* pMsg);
	void GTouRnament_Cheat(DWORD dwConnectionIndex, void* pMsg);
	void GTouRnament_FlagChange(void* pMsg);
};

#endif // !defined(AFX_GUILDTOURNAMENTMGR_H__21A71231_1238_4D1F_99B6_D53A80C23436__INCLUDED_)



