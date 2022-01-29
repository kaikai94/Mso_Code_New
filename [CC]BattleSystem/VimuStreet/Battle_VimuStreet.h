



#ifndef _BATTLE_VIMUSTREET_H
#define _BATTLE_VIMUSTREET_H

#include "battle.h"
#include "BattleTeam_VimuStreet.h"

#ifndef _MAPSERVER_

#include "../ImageNumber.h"
#endif

#define BATTLE_VIMUSTREET_READYTIME		6000	
#define BATTLE_VIMUSTREET_FIGHTTIME		60000
#define BATTLE_VIMUSTREET_RESULTTIME	10000

enum eVIMUSTREET_TEAM

{
	VIMUTEAM_BLUE,
	VIMUTEAM_RED,
};


struct BATTLE_INFO_VIMUSTREET : public BATTLE_INFO_BASE
{
	BATTLE_INFO_VIMUSTREET()
	{
		memset(this,0,sizeof(BATTLE_INFO_VIMUSTREET));
	}

	DWORD		Character[eBattleTeam_Max];
	VECTOR3		vStgPos;
	DWORD		dwGoldMoney;
	BOOL		bIsGoldBattle;
};

class CBattle_VimuStreet : public CBattle
{	
	BATTLE_INFO_VIMUSTREET m_VimuStreetInfo;


	CBattleTeam_VimuStreet m_Team[2];
	
#ifndef _MAPSERVER_
	CImageNumber	m_ImageNumber;

//	cImage			m_ImageReady;
	cImage			m_ImageStart;
	cImage			m_ImageWin;
	cImage			m_ImageLose;
	cImage			m_ImageDraw;
	cImage*			m_pCurShowImage;

	VECTOR2			m_vTitlePos;
	VECTOR2			m_vTitleScale;

	DWORD	m_dwFadeOutStartTime;
	BOOL	m_bFadeOut;


#else
	BOOL			m_bDieByOp[2];
#endif
	

public:
	CBattle_VimuStreet();
	virtual ~CBattle_VimuStreet();

	void Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
#ifdef _MAPSERVER_
	virtual void GetBattleInfo(char* pInfo,WORD* size);
#endif

	virtual BOOL IsEnemy(CObject* pOperator,CObject* pTarget);
	virtual BOOL IsFriend(CObject* pOperator,CObject* pTarget);


	virtual void OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2);
	virtual void OnFightStart();
	virtual void OnDestroy();
	virtual void OnTeamMemberAdd( int Team, DWORD MemberID, char* Name );
	virtual BOOL OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID);
	virtual BOOL OnTeamMemberDelete(int Team,DWORD MemberID,char* Name);

	virtual void OnTeamObjectCreate(int Team,CBattleObject* pBattleObject);
	virtual void OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject);


	virtual void OnTick();

	virtual BOOL Judge();
	BOOL JudgeOneTeamWinsOtherTeam(int TheTeam,int OtherTeam);
	virtual void Victory(int WinnerTeamNum,int LoserTeamNum);
	virtual void Draw();
	//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-27][17:55:32]
	void SetGoldVimuStreet(DWORD dwGold){m_VimuStreetInfo.dwGoldMoney = dwGold;m_VimuStreetInfo.bIsGoldBattle = TRUE;}
	

	

#ifdef _MHCLIENT_
	virtual void Render();
#endif	
};


#endif


