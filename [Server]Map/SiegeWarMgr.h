



// SiegeWarMgr.h: interface for the CSiegeWarMgr class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_)
#define AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\[CC]BattleSystem\SiegeWar\Battle_SiegeWar.h"

#define SIEGEWARMGR CSiegeWarMgr::GetInstance()

// 惑隆拱苞狼 芭府眉农.. 怖秦林磊.
enum
{
	NAKYANG_SYMBOL,
	MAX_SYMBOL
};


class CGuild;
class CPlayer;



enum eSW_Error
{
	eSWError_NoError=0,
	eSWError_Error,
	eSWError_NoGuildInfo,					// Don't find guildinfo
	eSWError_NoGuildMaster,					// Not a Guild Master
	eSWError_NoCastleGuild,					// Not a Castle Guild
	eSWError_FailedRegistSiegeWarTime,		// Failed Regist siegewar-time
	eSWError_AlreadyDefenceProposal,		// Already Defence-proposal
	eSWError_AlreadyAttackRegist,			// Registed Attack guild

	eSWError_DontDefenceProposal,			// Don't proposal to defence
	eSWError_NoProposalGuild,				// Not a proposal guild
	eSWError_MyUnionIsCastleUnion,			// Proposal Failed
	eSWError_MyUnionIsOtherTeam,			//	
	eSWError_OverGuildCount,				// Over the guild count at siegewar
	eSWError_RegistTime,					// Regist error : siegewar start-time
	eSWError_NoProposalTime,	
	eSWError_CastleGuild,					// Engrave
	eSWError_NoPerfectGuild,				// Not a Perfect Guild Level(5)
	eSWError_OtherPlayerEngrave,			// Already engrave by the other player
	eSWError_NoBaseVillage,					// Not a Guild-basetown
	eSWError_NoSiegeWarTime,				// Not a siegewar-time

	eSWError_Observer,						//
	eSWError_NoAcceptTime,
};

enum eSW_State
{
	eSWState_Before=0,		//[开始前][By:十里坡剑神][QQ:112582793][2018/3/9]
	eSWState_Proclamation,	//[公布期间][By:十里坡剑神][QQ:112582793][2018/3/9]
	eSWState_Acceptance,	//[申请期间][By:十里坡剑神][QQ:112582793][2018/3/9]
	eSWState_BeforeSiegeWar,
	eSWState_SiegeWar,		//[攻城中][By:十里坡剑神][QQ:112582793][2018/3/9]
	eSWState_EndSiegeWar,	//[攻城结束][By:十里坡剑神][QQ:112582793][2018/3/9]
};

enum eSW_GuildType
{
	eSWGuildState_None=0,
	eSWGuildState_CastleGuild,			
	eSWGuildState_DefenceGuild,				
	eSWGuildState_DefenceProposalGuild,		
	eSWGuildState_AttackGuild,				
	eSWGuildState_CastleUnionGuild,			
};

enum eSW_GateKind
{
	eSWGateKind_OuterGate1 = 1,
	eSWGateKind_OuterGate2 = 2,
	eSWGateKind_InnerGate1 = 3,
	eSWGateKind_InnerGate2 = 4,
};



class CSiegeWarMgr  
{
	CYHHashTable<CGuild>		m_DefenceProposalList;		
	CYHHashTable<CGuild>		m_DefenceAcceptList;		
	CYHHashTable<CGuild>		m_AttackGuildList;		
	CYHHashTable<CGuild>		m_AttackUnionList;			
	
	CBattle_SiegeWar*			m_pBattle;	

	DWORD			m_SiegeWarIdx;
	DWORD			m_SiegeWarState;			
	DWORD			m_CastleUnionIdx;			
	DWORD			m_CastleGuildIdx;			
	stTIME			m_SiegeWarTime[2];			

	DWORD			m_SiegeWarMapNum[SIEGEWAR_MAX_SIEGEMAP][SIEGEWAR_MAX_AFFECTED_MAP];	
	DWORD			m_SiegeMapCount;		

	DWORD			m_TaxRate;					
	DWORD			m_EngraveIdx;				
	DWORD			m_EngraveTimer;				
	DWORD			m_EngraveGuildIdx;			

	DWORD			m_SiegeWarSuccessTimer;		
	DWORD			m_SiegeWarEndTimer;			

	DWORD			m_FightTime;				

	stCASTLEGATELEVEL		m_GateInfo;
	cPtrList				m_CastleGateList;	
	CASTLEGATE_BASEINFO		m_CastleGateInfoList[MAX_CASTLEGATE_NUM];
	int						m_CastleGateCount;


	DWORD			m_SymbolIndex[MAX_SYMBOL];

	WORD			m_wCurDay;
	DWORD			m_dwCurFlag;

public:
	MAKESINGLETON( CSiegeWarMgr );
	CSiegeWarMgr();
	virtual ~CSiegeWarMgr();

	void Init();
	void LoadCastleGateInfo();
	void CreateSiegeBattle();
	void SetSiegeWarInfo( DWORD SiegeWarIdx, DWORD TaxRate, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level );
	void SetSiegeGuildInfo( SIEGEWAR_GUILDDBINFO* pGuildList, DWORD Count );
	void CreateCastleGate();
	void AddCastleGate();
	void DeleteCastleGate( DWORD GateID );
	void SetFirstSiegeWarTime();

	
	DWORD AddProposalGuildList( DWORD GuildIdx );
	DWORD AddAttackGuildList( DWORD GuildIdx );
	BOOL IsRegistTime( DWORD dwTime );
	void BreakUpGuild( DWORD GuildIdx );
	BOOL IsPossibleUnion( DWORD GuildIdx1, DWORD GuildIdx2 );
	void CreateUnionCheck( DWORD UnionIdx );
	void AddAcceptGuild( DWORD GuildIdx );
 	void DeleteAcceptGuild( DWORD GuildIdx );
 	void DestoryCastleUnion();
 	void SetCastleUnionIdx( DWORD UnionIdx )				{	m_CastleUnionIdx = UnionIdx;	}




	BOOL IsAcceptGuild( DWORD GuildIdx );
	BOOL IsAttackGuild( DWORD GuildIdx );
	BOOL IsAttackUnion( DWORD UnionIdx );
	void AddPlayer( CPlayer* pPlayer );

	void SetGateLevel( DWORD GateID, DWORD Level )			{	m_GateInfo.SetLevel( GateID, Level );	}
	DWORD GetGateLevel( DWORD GateID )						{	m_GateInfo.GetLevel( GateID );	}

	void Process();
	void StartSiegeWar();
	void EndSiegeWar();	
	void SetBattleInfo();

	void SuccessSiegeWar( DWORD GuildIdx );	
	void SendBattleInfoToVillageMap();
	int AddEngraveSyn( CPlayer* pPlayer, DWORD GuildIdx );

	DWORD GetSiegeWarIdx()					{	return m_SiegeWarIdx;	}
	DWORD GetGuildTeamIdx( DWORD GuildIdx );
	DWORD IsAbleOrganizeUnion( DWORD GuildIdx1, DWORD GuildIdx2 );
	void SendMsgToSeigeGuild( MSGBASE* pMsg, DWORD dwLength );
	DWORD GetEngraveIdx()					{	return m_EngraveIdx;	}
	void CancelEngraveIdx();

	DWORD GetVillageMapNum();
	DWORD GetSiegeMapNum();
	BOOL IsNeedLoadSiegeInfo();
	BOOL IsProcessSiegewar()
	{
		if( m_SiegeWarState == eSWState_SiegeWar )	return TRUE;
		else										return FALSE;
	}
	BOOL IsInSiegeWar( DWORD GuildIdx );
	void	GetSiegeWarTime( DWORD* pTime0, DWORD* pTime1 );
	DWORD GetCastleUnionIdx()				{	return m_CastleUnionIdx;	}
 	DWORD GetCastleGuildIdx()				{	return m_CastleGuildIdx;	}

	void UserLogOut( CPlayer* pPlayer );
	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol,void* pMsg );	

	void	CheckDateforFlagNPC();
	DWORD	GetSWState()	{	return m_SiegeWarState;	}
};

#endif // !defined(AFX_SIEGEWARMGR_H__0620B52A_B018_4B66_9EF9_6A9D6703763D__INCLUDED_)


