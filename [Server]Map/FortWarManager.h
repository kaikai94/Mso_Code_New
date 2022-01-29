#pragma once

#include "FortWarWareSlot.h"


#pragma pack(push,1)

enum eFortWarState

{
	eFortWarState_None = 0,
	eFortWarState_Before10Min,
	eFortWarState_Ing,
};

enum eFortWarDay
{
	eFortWarDay_Sun = 0,
	eFortWarDay_Mon,
	eFortWarDay_Tue,
	eFortWarDay_Wed,

	eFortWarDay_Thur,
	eFortWarDay_Fri,
	eFortWarDay_Sat,
	eFortWarDay_Max,
};

struct sFortWarTimeInfo
{
	BOOL		bOn;
	SYSTEMTIME	FortWarStartTime;
	DWORD		dwWarTime;
};

struct sFortWarInfo
{
	int		nRelationFortWarIDX;
	DWORD	dwCharacterIdx;
	DWORD	dwGuildIdx;
	char	CharacterName[MAX_NAME_LENGTH+1];
	DWORD	dwProfitMoney;
};

#pragma pack(pop)

#define FORTWARMGR	USINGTON(CFortWarManager)

class CPlayer;
class CMonster;

class CFortWarManager
{

protected:
	BOOL			m_bLoadDataFromDB;					
	BOOL			m_bFortWarMap;						
	DWORD			m_dwChannelNum;					
	DWORD			m_dwWarState;					
	DWORD			m_dwWarTime;						
    sFortWarTimeInfo	m_FortWarTimeInfo[eFortWarDay_Max];
	STATIC_NPCINFO*	m_pNpcInfo;							

	CPlayer*		m_pEngravePlayer;					
	DWORD			m_dwEngraveTime;					


	CYHHashTable<sFortWarInfo>	m_FortWarInfoTable;		
	WORD						m_wRelationFortWarMapNum;	
	int							m_nRelationFortWarIDX;		

	CFortWarWareSlot	m_FortWarWareSlot01;			
	CFortWarWareSlot	m_FortWarWareSlot02;				
	CFortWarWareSlot	m_FortWarWareSlot03;				
	CFortWarWareSlot	m_SiegeWarWareSlot;					
	int					m_nRelationSiegeWarIDX;				
	BOOL				m_bSiegeWarWareUsableMap;			

	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable01;		
	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable02;		
	CYHHashTable<CPlayer>	m_FortWarWarePlayerTable03;		
	CYHHashTable<CPlayer>	m_SiegeWarWarePlayerTable;		

	DWORD		m_dwFortWarProfitMoney;				
	int			m_nFortWarTexRate;					
	int			m_nFortWarWareRate;					
	DWORD		m_dwUpdateTime;

protected:
    int GetSecondInterval( SYSTEMTIME curTime, SYSTEMTIME basicTime );			

public:
	CFortWarManager(void);

	~CFortWarManager(void);

	void Init();
	void Release();
	//[要塞战不开红即可攻击服务端判断函数][By:十里坡剑神][QQ:112582793][2017/12/7]
	BOOL IsFortWaring(CPlayer* pPlayer);

	void ProcessFortWar();
	BOOL LoadFortWarInfo();
	void AddPlayer( CPlayer* pPlayer );
	void RemovePlayer( CPlayer* pPlayer );	
	void DiePlayer( CPlayer* pPlayer );
	void SetTotalFortWarInfoFromDB( sFortWarInfo info );
	void UpdateFortWarInfo( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx );
	void CompleteLoadDataFromDB()	{ m_bLoadDataFromDB = TRUE; }

	DWORD GetMasterID();
	char* GetMasterName();

	DWORD GetMasterGuildID();

	void ProcessDBUpdateProfitMoney();
	DWORD AddProfitMoneyFromItemSell( DWORD dwMoney );
	DWORD AddProfitMoneyFromItemBuy( DWORD dwMoney );
	DWORD AddProfitMoneyFromMonster( DWORD dwMoney );
	void SetFortWarProfitMoneyFromDB( int nWarIDX, DWORD dwMoney );

	void RemoveWarePlayerTable( CPlayer* pPlayer );
	void TotalFortWarItemLoad();
	void SetTotalFortWarItemInfoFromDB( int nWarIDX, ITEMBASE* pItem );
	BOOL AddProfitItemFromMonster( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum );
	void AddProfitItemFromDBResult( int nWarIDX, ITEMBASE* pItem );
	CFortWarWareSlot* GetFortWarWareSlot( int nWarIDX );
	void SendItemInfoToFortWarePlayer( int nWarIDX, MSGBASE* pMsg, int nSize );
	void SendItemInfoToFortWarePlayerExceptOne( int nWarIDX, DWORD dwPlayerID, MSGBASE* pMsg, int nSize );
	void SendItemInfoToSeigeWarePlayer( MSGBASE* pMsg, int nSize );
	void SendItemInfoToSeigeWarePlayerExceptOne( DWORD dwPlayerID, MSGBASE* pMsg, int nSize );
	void SetSiegeWarWareUsable( BOOL bUse )		{ m_bSiegeWarWareUsableMap = bUse; }

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );

protected:
	void Msg_MP_FORTWAR_ENGRAVE_START_SYN( void* pMsg );
	void Msg_MP_FORTWAR_ENGRAVE_CANCEL_SYN( void* pMsg );


    void Msg_MP_FORTWAR_WAREHOUSE_INFO_SYN( void* pMsg );

	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_LEAVE( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN( void* pMsg );
	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN( void* pMsg );

	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP( void* pMsg );

	void Msg_MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg );
	void Msg_MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP( void* pMsg );
	void Msg_MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg );
};

EXTERNGLOBALTON(CFortWarManager)


