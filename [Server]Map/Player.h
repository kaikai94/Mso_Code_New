#if !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)
#define AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Object.h"
#include "ExchangeManager.h"
#include "OptionManager.h"
#include "ItemContainer.h"
#include "InventorySlot.h"
#include "PyogukSlot.h"
#include "WearSlot.h"
#include "ShopItemSlot.h"
#include "ShopInvenSlot.h"
#include "PetInvenSlot.h"
#include "PetWearSlot.h"
#include "TitanWearSlot.h"
#include "TitanShopItemSlot.h"
#include "Purse.h"
#include "AbilityGroup.h"
#include "../[CC]Skill/DelayGroup.h"
#include "ShopItemManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "..\[CC]Quest\QuestDefines.h"
#include "QuestGroup.h"

class cStreetStall;
class CMonster;
class CItemContainer;
class CItemSlot;
class CQuestBase;
class CQuestScript;
class CQuestGroup;
class CPet;
class CTitan;

enum
{
	PLAYERINITSTATE_ONLY_ADDED = 0,
	PLAYERINITSTATE_HERO_INFO = 1,
	PLAYERINITSTATE_MUGONG_INFO = 2,
	PLAYERINITSTATE_ITEM_INFO = 4,
	PLAYERINITSTATE_ITEM_OPTION_INFO = 8,
	PLAYERINITSTATE_ABILITY_INFO = 16,
	PLAYERINITSTATE_PYOGUK_INFO = 32,
	PLAYERINITSTATE_SHOPITEM_USEDINFO = 64,	
	PLAYERINITSTATE_ACKMSG_SEND = 128,
	PLAYERINITSTATE_INITED = 256,
	PLAYERINITSTATE_GMCONTROL_INFO = 512,
	PLAYERINITSTATE_ITEM_RARE_OPTION_INFO = 1024,
	PLAYERINITSTATE_PET_INFO = 2048,
	PLAYERINITSTATE_TITAN_INFO = 4096,
};

enum
{
	MONEY_ADDITION = 0,
	MONEY_SUBTRACTION,
};

#define MUSSANG_VALID_TIME		1000*60*10
enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};

#define GETITEM_FLAG_INVENTORY	0x00000001
#define GETITEM_FLAG_WEAR		0x00000002
#define GETITEM_FLAG_PETWEAR	0x00000004
#define GETITEM_FLAG_TITANWEAR	0x00000008		
#define GETITEM_FLAG_TITANSHOP	0x00000010		
#define WANTED_DESTROYED_TIME 48*3600*1000
class CPlayer : public CObject  
{
private:
	DWORD m_dwMoveMapTime;
    enum SKIN_DELAYTIME
	{
		eSkinDelayTime = 10000,
	};

	//std::map<DWORD,DWORD> m_KillTopCheckList;
	//[自动加点标志][By:十里坡剑神][QQ:112582793][2018/4/17]

	BYTE	m_AutoAddPointFlag;	
	BOOL	m_IsAutoAddPoint;
	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	BOOL	m_bIsSafeLock;
	DWORD   m_KilledPlayerID;
	DWORD   m_LootingPos;

	//[假人添加测试][By:十里坡剑神][QQ:112582793][2018/4/22]
	BOOL	m_bIsRobot;
	CDelayGroup m_DelayGroup;
	
	DWORD m_ChannelID;

	BOOL m_bPetInvenInit;

	int m_LastPaoDianTime;
	int m_LastMapTime;
	DWORD m_ItemGrowCount;
	DWORD m_dwConnection;
	BOOL m_bGotWarehouseItems;
	BOOL m_bPyogukInit;
	BOOL m_bShopInit;
	DWORD m_UniqueIDinAgent;
	int m_initState;
	DWORD m_dwHpMpBitFlag;	
	CHARACTER_TOTALINFO m_HeroCharacterInfo;
	HERO_TOTALINFO		m_HeroInfo;
	MUGONG_TOTALINFO	m_HeroMugongInfo;
	
	CAbilityGroup		m_AbilityGroup;

	CItemContainer		m_ItemContainer;
	CInventorySlot		m_InventorySlot;
	CWearSlot			m_WearSlot;
	CPyogukSlot			m_PyogukSlot;
	CShopItemSlot		m_ShopItemSlot;			
	CShopInvenSlot		m_ShopInvenSlot;
	CPetInvenSlot		m_PetInvenSlot;			
	CPetWearSlot		m_PetWearSlot;			
	CTitanWearSlot		m_TitanWearSlot;		
	CTitanShopItemSlot	m_TitanShopItemSlot;	
	CInventoryPurse		m_InventoryPurse;
	CPyogukPurse		m_PyogukPurse;

	char m_StreetStallTitle[MAX_STREETSTALL_TITLELEN+1];

	player_calc_stats m_charStats;
	player_calc_stats m_itemStats;
	SET_ITEM_OPTION m_setItemStats;		
	UNIQUE_ITEM_OPTION_INFO m_UniqueItemStats;	
	ABILITY_STATS m_AbilityStats;
		 
	RECOVER_TIME m_NaeRyukRecoverTime;
	YYRECOVER_TIME m_YYNaeRyukRecoverTime;

	WORD m_ChangeBeforeMapNum;
	WORD m_ChangeWantMapNum;

	DWORD m_CurComboNum;

	WORD m_ChangeNpcIndex;

	BYTE m_curGravity;

	int	m_nHackCount;			
	DWORD m_dwHackStartTime;
	
	DWORD	m_dwSkillDelayStartTime;		
	//DWORD	m_dwLastSkillAnimationTime;

	DWORD m_dwLastSocietyAction;

	sEXCHANGECONTAINER	m_ExchangeContainer;
	BOOL				m_bVimuing;
	DWORD				m_dwOpPlayerID;
	sGAMEOPTION			m_GameOption;
	DWORD				m_dwPKModeStartTime;	
	DWORD				m_dwPKContinueTime;
	DWORD				m_dwLastReduceHour;		
	void				AddBadFameReduceTime();
	BOOL				m_bExit;	
	DWORD				m_dwExitStartTime;
	BOOL				m_bNormalExit;
	BOOL				m_bWaitExitPlayer;
	DWORD				m_dwUserLevel;
	cStreetStall*		m_pGuetStall;
	DWORD				m_dwProgressTime;
	WANTEDTYPE			m_WantedIDX;
	cPtrList			m_WantedList;
	WORD				m_MurdererKind;
	DWORD				m_MurdererIDX;
	BOOL				m_bReadyToRevive; 
	WORD				m_MurderObjKind;
	BOOL				m_bPenaltyByDie;
	DWORD				m_LoginTime;
	DWORD				m_ContinuePlayTime;	
	
	SHOPITEMOPTION		m_ShopItemOption;
	AVATARITEMOPTION	m_AvatarOption;
	CShopItemManager	m_ShopItemManager;

	SKILLSTATSOPTION	m_SkillStatsOption;

	CYHHashTable<CSkillObject> m_SkillList;

	CPetManager			m_PetManager;

	CTitanManager		m_TitanManager;

	DWORD				m_InitPosition;
	REVIVEDATA			m_ReviveData;
	int					m_ReviveTime;
	BOOL				m_bSavePointInited;
	static ITEM_RARE_OPTION_INFO m_NULLRAREINFO;
	static ITEM_OPTION_INFO NULLOPTIONINFO;
	DWORD				m_dwMussangPoint;
	DWORD				m_dwMussangStartTime;
	BOOL				m_bIsMussangReady;
	BYTE				m_bMussangFlag;

	DWORD	m_dwWantMarryID;
	CHAR	m_szWantMarryName[MAX_NAME_LENGTH + 1];
	WORD	m_SkillFailCount;

	int		m_FullMoonEventHCt;
	float	m_fFullMoonEventTime;

	DWORD	m_LastActionTime;
	BOOL	m_bActionPanelty;

	DWORD	m_CristmasEventSnowIdx;
	DWORD	m_dwSkinDelayTime;	
	BOOL	m_bSkinDelayResult;	
	
	void SkinDelayTimeProcess();
    
	void DoSetInited();
	virtual void SetInitedGrid();

	friend class CCharacterCalcManager;
	friend class CInventoryPurse;
	friend class CAbilityGroup;
	
	void SetMurderIDX(DWORD CharacterIDX)			{ m_MurdererIDX = CharacterIDX; }
	void SetMurderKind(WORD Kind)					{ m_MurdererKind = Kind; }

	int					m_SOSState;		

//#ifndef _JAPAN_LOCAL_
	DWORD	m_dwMussangTime;
	WORD	m_wMussangStat;
//#endif

//#ifdef _JAPAN_LOCAL_
//	DWORD	m_dwCharacterSlotNum;
//#endif
//#ifdef _HK_LOCAL_
//	DWORD	m_dwCharacterSlotNum;
//#endif
//#ifdef _TL_LOCAL_
//	DWORD	m_dwCharacterSlotNum;
//#endif

public:

	void	SetMurderObjKind(WORD ObjKind)		{	m_MurderObjKind = ObjKind;	}
	WORD	GetMurderObjKind()					{	return m_MurderObjKind;	}

	void	SetPenaltyByDie(BOOL bVal)		{	m_bPenaltyByDie = bVal;	}	
	BOOL	IsPenaltyByDie()				{	return m_bPenaltyByDie;	}

	BOOL IsGotWarehouseItems() { return m_bGotWarehouseItems; }
	void SetGotWarehouseItems( BOOL bGot ) { m_bGotWarehouseItems = bGot; }

	void SetWaitExitPlayer( BOOL bWait ) { m_bWaitExitPlayer = bWait; }
	BOOL IsWaitExitPlayer() { return m_bWaitExitPlayer; }

	void SetChannelID(DWORD id)		{	m_ChannelID = id;	}
	DWORD GetChannelID()			{	return m_ChannelID;	}
	BOOL				m_bNeedRevive;
	DWORD GetSkillDelayStartTime()				{ return m_dwSkillDelayStartTime; }
	void SetSkillDelayStartTime( DWORD dwTime )	{m_dwSkillDelayStartTime = dwTime; }

	//DWORD GetLastSkillAnimationTime()			{ return m_dwLastSkillAnimationTime; }
	//void SetLastSkillAnimationTime(DWORD dwTime);
	
	BOOL SpeedHackCheck(WORD wTime= 0);
	BOOL CanSocietyAction( DWORD curTime );
	
	int GetGender() { return m_HeroCharacterInfo.Gender; }
	HERO_TOTALINFO* GetHeroTotalInfo();

	ABILITY_STATS*	GetAbilityStats()	{	return &m_AbilityStats;	}
	CAbilityGroup*	GetAbilityGroup()	{	return &m_AbilityGroup;	}
	CDelayGroup*	GetDelayGroup()		{	return &m_DelayGroup;	}

	CItemSlot * GetSlot(POSTYPE absPos);
	CItemSlot * GetSlot(eITEMTABLE tableIdx);

	void SetUniqueIDinAgent(DWORD uniqueid)	{	m_UniqueIDinAgent = uniqueid;	}
	DWORD GetUniqueIDinAgent()	{	return m_UniqueIDinAgent;	}
	
	void InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo);

	BOOL IsVisible()
	{
		return (m_HeroCharacterInfo.bVisible ? TRUE : FALSE);
	}
	void SetVisible(BOOL val)
	{
		m_HeroCharacterInfo.bVisible = val ? true : false;
	}
	
	virtual DWORD GetGravity();
	virtual void SetGravity(DWORD gravity)
	{
	}

	CPlayer();
	virtual ~CPlayer();
	void InitClearData();

	virtual BOOL Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo);
	virtual void Release();

	void InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo);
	void InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo);
	void InitMugongTotalInfo(MUGONG_TOTALINFO* pMugongInfo);
	void InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo);
	void InitPyogukItemInfo(PYOGUK_ITEM* pItemInfo);

	void InitPetInvenItemInfo(PETINVEN_ITEM* pItemInfo);
	void SendPetInvenItemInfo();
	void InitPetWearItemInfo(ITEMBASE* pWearItemInfo);

	int GetInitState(){ return m_initState; }
	void SetInitState(int initstate,DWORD protocol);

	void SendPlayerLifeToParty(DWORD life, DWORD PartyIdx);
	void SendPlayerShieldToParty(DWORD Shield, DWORD PartyIdx);
	void SendPlayerNaeRyukToParty(DWORD naeryuk, DWORD PartyIdx);
	void SendMoveInfotoParty();

	LEVELTYPE GetPlayerLevelUpPoint() { return m_HeroInfo.LevelUpPoint;	}
	void SetPlayerLevelUpPoint(LEVELTYPE val);
	EXPTYPE GetPlayerExpPoint() { return m_HeroInfo.ExpPoint; }
	DOUBLE GetPlayerTotalExpPoint();
	void SetPlayerExpPoint(EXPTYPE point,BYTE bDie=0);
	void SetLevelUpStatPoint();			
	void SetNormalStatPoint();			
	BYTE GetExpFlag()	{	return m_HeroInfo.ExpFlag;	}	
	void AddPlayerExpPoint(EXPTYPE Exp);
	void ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType);
	DWORD GetPlayerAbilityExpPoint() { return m_AbilityGroup.GetAbilityExp(); }
	void SetPlayerAbilityExpPoint(DWORD point,BOOL bUse=0);
	void AddAbilityExp(DWORD AbilityExp);

	DWORD AddExpPointToMugong(WORD MugongIdx,DWORD exp,BOOL bSuryunComplete = FALSE);
	void SetPlayerMugongSung(POSTYPE abspos, BYTE sung, WORD Reason);
	void MugongLevelDown(WORD MugongIdx, WORD Reason);
	
	FAMETYPE GetFame()			{ return m_HeroInfo.Fame;	}
	void SetFame(FAMETYPE val)	{ m_HeroInfo.Fame = val; 	}

	FAMETYPE GetBadFame()			{ return m_HeroCharacterInfo.BadFame;	}
	void SetBadFame(FAMETYPE val)	{ m_HeroCharacterInfo.BadFame = val; 	}

	WORD GetCheRyuk(){ return (int)(m_HeroInfo.wCheRyuk + GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk
		+ GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk + GetUniqueItemStats()->nCheRyuk) >= 0 ? (WORD)(m_HeroInfo.wCheRyuk
		+ GetAbilityStats()->StatChe + GetItemStats()->CheRyuk + GetShopItemStats()->Cheryuk + GetAvatarOption()->Cheryuk + GetSetItemStats()->wCheRyuk
		+ GetUniqueItemStats()->nCheRyuk) : 0; }

	WORD GetSimMek(){ return (int)(m_HeroInfo.wSimMek + GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek
		+ GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek + GetUniqueItemStats()->nSimMek) >= 0 ? (WORD)(m_HeroInfo.wSimMek
		+ GetAbilityStats()->StatSim + GetItemStats()->SimMaek + GetShopItemStats()->Simmek + GetAvatarOption()->Simmek + GetSetItemStats()->wSimMek
		+ GetUniqueItemStats()->nSimMek) : 0; }

	WORD GetGenGol(){ return (int)(m_HeroInfo.wGenGol + GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol
		+ GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol + GetUniqueItemStats()->nGengol) >= 0 ? (WORD)(m_HeroInfo.wGenGol
		+ GetAbilityStats()->StatGen + GetItemStats()->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + GetSetItemStats()->wGenGol
		+ GetUniqueItemStats()->nGengol) : 0; }

	WORD GetMinChub(){ return (int)(m_HeroInfo.wMinChub + GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub
		+ GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub + GetUniqueItemStats()->nMinChub) >= 0 ? (WORD)(m_HeroInfo.wMinChub
		+ GetAbilityStats()->StatMin + GetItemStats()->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + GetSetItemStats()->wMinChub
		+ GetUniqueItemStats()->nMinChub) : 0; }

	void SetCheRyuk(WORD val);
	void SetSimMek(WORD val);
	void SetGenGol(WORD val);
	void SetMinChub(WORD val);

	void AddGenGol(WORD val);
	void AddSimMek(WORD val);
	void AddMinChub(WORD val);
	void AddCheRyuk(WORD val);
	
	void SetStage( BYTE stage );
	BYTE GetStage()		{ return m_HeroCharacterInfo.Stage; }
	
	WORD GetPlayerLoginPoint(){return m_HeroCharacterInfo.LoginPoint_Index;}
	void SetLoginPoint(WORD LoginPoint){m_HeroCharacterInfo.LoginPoint_Index = LoginPoint;}
	void InitMapChangePoint(){m_HeroCharacterInfo.MapChangePoint_Index = 0;}
	void SetMapChangePoint(WORD point){m_HeroCharacterInfo.MapChangePoint_Index = point;}	// YH
	WORD GetMapChangePoint() { return m_HeroCharacterInfo.MapChangePoint_Index;	} //hs for party
	
	void CalcState();

	void SetMugongBase(POSTYPE abspos, MUGONGBASE * mugong)
	{
//#ifdef _JAPAN_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
// 			ASSERT(0);
//		POSTYPE pos = abspos - TP_MUGONG_START;
//#elif defined _HK_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
// 			ASSERT(0);
//
//		POSTYPE pos = abspos - TP_MUGONG_START;
//#elif defined _TL_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
// 			ASSERT(0);
//
//
//		POSTYPE pos = abspos - TP_MUGONG_START;
//#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
				ASSERT(0);
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;

		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		else
		{
			ASSERT(0);
			return;
		}
//#endif
		if( mugong->dwDBIdx != 0 && mugong->Position == 0)
			CRITICALASSERT(0);

            memcpy(&m_HeroMugongInfo.mugong[pos], mugong, sizeof(MUGONGBASE));
	}
	MUGONGBASE * GetMugongBase(POSTYPE abspos)
	{
//#ifdef _JAPAN_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
//   		{	
//			ASSERT(0);
// 			return NULL;
//   		}
//
//		POSTYPE pos = abspos - TP_MUGONG_START;
//
//#elif defined _HK_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
//   		{	
//			ASSERT(0);
// 			return NULL;
//   		}
//		POSTYPE pos = abspos - TP_MUGONG_START;
//#elif defined _TL_LOCAL_
//		if( abspos < TP_MUGONG_START && abspos >= TP_MUGONG_END )
//   		{	
//			ASSERT(0);
// 			return NULL;
//   		}
//		POSTYPE pos = abspos - TP_MUGONG_START;
//#else
		if( abspos < TP_MUGONG1_START && abspos >= TP_MUGONG2_END )
		{	
			if( abspos < TP_JINBUB_START && abspos >= TP_JINBUB_END )
			{
				ASSERT(0);
				return NULL;
			}
		}

		DWORD EndPos = TP_MUGONG1_END;
		if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MugongExtend ) )
			EndPos = TP_MUGONG2_END;


		POSTYPE pos;
		if( abspos >= TP_MUGONG1_START && abspos < TP_TITANMUGONG_END )
			pos = abspos - TP_MUGONG1_START;
		else
			return NULL;
//#endif
		return &m_HeroMugongInfo.mugong[pos];
	}	
	MUGONGBASE * GetMugongBaseByMugongIdx(WORD MugongIdx);
	int GetMugongLevel(WORD MugongIdx);
	int GetMugongNum();

	WORD GetSkillOptionIndex(WORD MugongIdx);
	//////////////////////////////////////////////////////////////////////////
	void GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo);
	void GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo);
	void GetMugongTotalInfo(MUGONG_TOTALINFO* pRtInfo);

	void GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag);
	ITEMBASE * GetItemInfoFromDBIdx(DWORD dwDBIdx);
	void GetPyogukItemInfo(PYOGUK_ITEM * pRtInfo);

	WORD GetWearedItemIdx(DWORD WearedPosition)	{	return m_HeroCharacterInfo.WearedItemIdx[WearedPosition];	}
	void SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx);
	BOOL UseItem(WORD abs_pos);
	BOOL UseConsumptionItem(WORD abs_pos);
	BOOL UseEquipItem(WORD abs_pos);
	BOOL UseMugongItem(WORD abs_pos);

	void SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin);
	
	virtual void SendMsg(MSGBASE* pMsg,int MsgLen);
	void	SendMsgToAllAgent(MSGBASE* pMsg,int MsgLen);
private:
	void MoneyUpdate( MONEYTYPE money ); 
	CYHHashTable<ITEM_RARE_OPTION_INFO> m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO> m_ItemOptionList;
public:
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	BOOL RemoveItemRareOption(DWORD dwRareIdx);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareIdx);

	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	BOOL RemoveItemOption(DWORD dwOptionIdx);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);


	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag = 0, eITEMTABLE tableIdx = eItemTable_Inventory, BYTE LogType = 0, DWORD TargetIdx = 0 );
	MONEYTYPE GetMoney(eITEMTABLE tableIdx = eItemTable_Inventory);

	MONEYTYPE GetMallMoney(){return  m_HeroInfo.MallMoney;}

	void SetMallMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx=0);

	MONEYTYPE GetGoldMoney(){return m_HeroInfo.GoldMoney;}

	void SetGoldMoney(MONEYTYPE ChangeValue,BYTE nFlag,WORD ItemIdx=0,WORD ItemDurability=0);

	MONEYTYPE GetSwMoney(){return m_HeroInfo.Fame;}

	void SetSwMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx=0);

	WORD GetReSet(){return m_HeroInfo.ReSet;}

	void SetReSet(DWORD Val,WORD Type);

	void SetReSetN(DWORD Val,WORD Type);

	void SetMarryName(char* pName);
	char* GetMarryName();
	bool  IsMarry();
	void SetWantMarryID(DWORD id){ m_dwWantMarryID = id; }
	DWORD GetWantMarryID(){ return m_dwWantMarryID; }
	void SetWantMarryName(char* name){ SafeStrCpy(m_szWantMarryName, name, MAX_NAME_LENGTH + 1); }
	char* GetWantMarryName(){ return m_szWantMarryName; }


	MONEYTYPE GetMaxPurseMoney(eITEMTABLE TableIdx);
	void SetMaxPurseMoney(eITEMTABLE TableIdx, DWORD MaxMoney);
	BOOL IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx = eItemTable_Inventory );

	player_calc_stats*	GetCharStats()			{	return &m_charStats;		}
	player_calc_stats*	GetItemStats()			{	return &m_itemStats;		}
	SET_ITEM_OPTION* GetSetItemStats()		{	return &m_setItemStats;		}	
	UNIQUE_ITEM_OPTION_INFO* GetUniqueItemStats(){	return &m_UniqueItemStats;	}	
	SHOPITEMOPTION*		GetShopItemStats()		{	return &m_ShopItemOption;	}
	AVATARITEMOPTION*	GetAvatarOption()		{	return &m_AvatarOption;		}

	//////////////////////////////////////////////////////////////////////////
	SKILLSTATSOPTION*	GetSkillStatsOption()		{	return &m_SkillStatsOption;	}
	//////////////////////////////////////////////////////////////////////////
	DWORD GetPartyIdx()	{	return m_HeroInfo.PartyID;	}
	void SetPartyIdx(DWORD PartyIDx) { m_HeroInfo.PartyID = PartyIDx; }

	DWORD GetMunpaIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetMunpaIdx(DWORD MunpaIDx) { m_HeroCharacterInfo.MunpaID = MunpaIDx; }
	
	BYTE GetMunpaMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetMunpaMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} 
	void SetMunpaName(char* MunpaName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, MunpaName, MAX_MUNPA_NAME+1); }

	DWORD GetGuildIdx()	{	return m_HeroCharacterInfo.MunpaID;	}
	void SetGuildIdx(DWORD GuildIdx) { m_HeroCharacterInfo.MunpaID = GuildIdx; }

	void SetGuildInfo(DWORD GuildIdx, BYTE Grade, char* GuildName, MARKNAMETYPE MarkName);
	BYTE GetGuildMemberRank() { return m_HeroCharacterInfo.PositionInMunpa; 	}
	void SetGuildMemberRank(BYTE rank){ m_HeroCharacterInfo.PositionInMunpa = rank; 	} 
	void SetGuildName(char* GuildName){ SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, MAX_MUNPA_NAME+1); }
	void SetGuildMarkName(MARKNAMETYPE MarkName);
	char* GetGuildCanEntryDate();
	void SetGuildCanEntryDate(char* date);
	void ClrGuildCanEntryDate();
	void SetNickName(char* NickName);

	BYTE GetLifePercent();
	BYTE GetShieldPercent();
	BYTE GetNaeRyukPercent();
	
	void OnStartObjectState(BYTE State,DWORD dwParam);
	void OnEndObjectState(BYTE State);

	WORD GetWearedWeapon() { return m_HeroCharacterInfo.WearedItemIdx[eWearedItem_Weapon]; }
	WORD GetWeaponEquipType();
	WORD GetTitanWeaponEquipType();

	void SetCurComboNum(DWORD combonum)	{	m_CurComboNum = combonum;	}
	DWORD GetCurComboNum()				{	return m_CurComboNum;	}

	void SetPeaceMode(BOOL bPeace)	{	m_HeroCharacterInfo.bPeace = bPeace?true:false;	}
	BOOL GetPeaceMode()				{	return m_HeroCharacterInfo.bPeace;		}
	
	void AddPyogukItem(ITEMBASE * pPyogukItem);
	void InitPyogukInfo(BYTE pyoguknum,MONEYTYPE money);
	BOOL IsPyogukInit() {return m_bPyogukInit; }
	void SetPyogukNum(BYTE n);
	BYTE GetPyogukNum();

	void RevivePresentSpot();
	void ReviveLogIn();
	void ReviveAfterVimu( BOOL bSendMsg = TRUE );

	void ClearWantedList();
	WANTEDTYPE GetWantedIdx()	{ return m_WantedIDX; }
	void SetWantedIdx(WANTEDTYPE val)	{ m_WantedIDX = val; }
		
	DWORD GetWantedNum()			{ return m_WantedList.GetCount(); }
	
	void ClearMurderIdx();
	DWORD GetMurderIDX()						{ return m_MurdererIDX;	}
	WORD GetMurderKind()						{ return m_MurdererKind;}
	BOOL IsReadyToRevive()						{ return m_bReadyToRevive;	}
	void SetReadyToRevive(BOOL val)				{ m_bReadyToRevive = val;	}
	void AddWanted(WANTEDTYPE WantedIDX);
	void DeleteWanted(WANTEDTYPE WantedIDX);
	BOOL IsWantedOwner(WANTEDTYPE WantedIDX);
	
	void SetExchangeContainer( sEXCHANGECONTAINER* ExchangeContainer )

											{ m_ExchangeContainer = *ExchangeContainer; }
	sEXCHANGECONTAINER* GetExchangeContainer() { return &m_ExchangeContainer; }

	void SetVimuing( BOOL bVimuing ) { m_bVimuing = bVimuing; }
	BOOL IsVimuing() { return m_bVimuing; }
	void SetVimuOpPlayerID( DWORD dwOpPlayerID ) { m_dwOpPlayerID = dwOpPlayerID; }
	DWORD GetVimuOpPlayerID()	{ return m_dwOpPlayerID; }

	sGAMEOPTION* GetGameOption() { return &m_GameOption; }
	void SetGameOption( sGAMEOPTION* pGameOption )	{ m_GameOption = *pGameOption; }
	void SetAvatarView( bool bNoAvatarView )		{ m_HeroCharacterInfo.bNoAvatarView = bNoAvatarView;	}
	bool GetAvatarView()		{ return m_HeroCharacterInfo.bNoAvatarView;	}

	int PKModeOn();
	BOOL PKModeOff();
	DWORD GetPKOffTime();
	void PKModeOffForce();
	BOOL IsPKMode()		{ return m_HeroCharacterInfo.bPKMode; }
	void SetPKStartTimeReset() { m_dwPKModeStartTime = gCurTime; }
	void AddPKContinueTime( DWORD dwTime ) { m_dwPKContinueTime += dwTime; }
	DWORD GetPKContinueTime(){return m_dwPKContinueTime;}
	void SetPKModeEndtime();

	BOOL IsExitStart() { return m_bExit; }
	int CanExitStart();
	void SetExitStart( BOOL bExit );
	int CanExit();
	void ExitCancel();
	void SetNormalExit() { m_bNormalExit = TRUE; }
	BOOL IsNormalExit() { return m_bNormalExit; }

	void SetUserLevel( DWORD dwUserLevel ) { m_dwUserLevel = dwUserLevel; }
	DWORD GetUserLevel() { return m_dwUserLevel; }

	void SetGuestStall( cStreetStall* pStall ) { m_pGuetStall = pStall; }
	cStreetStall* GetGuestStall() { return m_pGuetStall; }
	void SetStreetStallTitle(char* title);
	void GetStreetStallTitle(char* title);
/////////////////////////////////////////////////////////////////////////
	virtual void DoDie(CObject* pAttacker);
	virtual void DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife);
	virtual DWORD Damage(CObject* pAttacker,RESULTINFO* pDamageInfo);
	
	void SetMaxLife(DWORD val);
	void SetMaxShield(DWORD val);
	void SetMaxNaeRyuk(DWORD val);
	
	LEVELTYPE GetMaxLevel()		{	return m_HeroInfo.MaxLevel;	}

	void SetMaxLevel(LEVELTYPE val){ m_HeroInfo.MaxLevel = val; }

	//////////////////////////////////////////////////////////////////////////
	virtual LEVELTYPE GetLevel();
	virtual void SetLevel(LEVELTYPE level);

	virtual DWORD GetLife();
	virtual void SetLife(DWORD Life,BOOL bSendMsg = TRUE);
	virtual DWORD GetShield();
	virtual void SetShield(DWORD Shield,BOOL bSendMsg = TRUE);
	virtual DWORD GetNaeRyuk();
	virtual void SetNaeRyuk(DWORD valb,BOOL SendMsg = TRUE);	
	virtual DWORD DoGetMaxLife();
	virtual DWORD DoGetMaxShield();
	virtual DWORD DoGetMaxNaeRyuk();
	virtual DWORD DoGetPhyDefense();
	virtual float DoGetAttDefense(WORD Attrib);
	virtual float GetAttribPlusPercent(WORD Attrib);

	virtual DWORD DoGetPhyAttackPowerMin();
	virtual DWORD DoGetPhyAttackPowerMax();

	virtual float DoGetMoveSpeed();

//#ifdef _JAPAN_LOCAL_
//	virtual float DoGetAddAttackRange(){	return (float)GetMinChub() / 2.f;	}
//#else
	virtual float DoGetAddAttackRange(){	return GetMinChub() / 3.f;	}
//#endif

	virtual DWORD DoGetCritical();
	virtual DWORD DoGetDecisive();



	WORD GetKyungGongGrade()	{	return m_HeroInfo.KyungGongGrade;	}
	void SetKyungGongGrade(WORD grade)	{	m_HeroInfo.KyungGongGrade = grade;	}

	ITEMOBTAINARRAYINFO * GetArray(WORD id);
	void AddArray(ITEMOBTAINARRAYINFO * pInfo);
	void RemoveArray(DWORD id);
	
	CYHHashTable<ITEMOBTAINARRAYINFO> m_ItemArrayList;
	BOOL AddFollowList(CMonster * pMob);
	void RemoveFollowList(DWORD ID);
	BOOL RemoveFollowAsFarAs(DWORD GAmount, CObject* pObject );

	CYHHashTable<CMonster> m_FollowMonsterList;

	void AddQuest(CQuestBase* pQuest);
	void RemoveQuest(DWORD QuestIdx);
	BOOL SetQuestState(DWORD QuestIdx, QSTATETYPE value);
	CYHHashTable<CQuestBase>	m_QuestList;

	virtual void StateProcess();

	DWORD GetPlayTime()				{	return m_HeroInfo.Playtime;	}
	void SetLoginTime(int time)		{	m_LoginTime = time;	}

	void ReviveLogInPenelty();
protected:
	CQuestGroup		m_QuestGroup;
public:
	CQuestGroup*	GetQuestGroup()		{ return &m_QuestGroup; }
	void			QuestProcess();

	void			RSetMoney( MONEYTYPE money, BYTE flag );	

	DWORD			m_dwOldMapNum;
	WORD			m_wKillMonsterLevel;

	int m_LastRecvPingSYNTime;
	int m_PingSYNFastCount;
	
	void	SetKillMonsterLevel( WORD wMonLevel )	{ m_wKillMonsterLevel = wMonLevel; }
	WORD	GetKillMonsterLevel()					{ return m_wKillMonsterLevel; }

	CShopItemManager* GetShopItemManager()			{	return &m_ShopItemManager;	}
	void SetShopItemInit( BOOL bInit )				{	m_bShopInit = bInit;	}
	void InitShopItemInfo(SHOP_ITEM* pItemInfo);
	void SendShopItemInfo();

	void UpdateLogoutToDB();
	
	void SetInitPoint( DWORD Pos )		{	m_InitPosition = Pos;	}
	DWORD GetInitPoint()				{	return m_InitPosition;	}
	
	BOOL IsAbleReviveOther();
	BOOL ReviveShopItem( WORD ItemIdx );
	void SetReviveData( DWORD Id, WORD ItemIdx, POSTYPE ItemPos )
	{	m_ReviveData.TargetID=Id;	m_ReviveData.ItemIdx=ItemIdx;	m_ReviveData.ItemPos=ItemPos;	}
	REVIVEDATA* GetReviveData()			{	return &m_ReviveData;	}
	void SetReviveTime( DWORD time )	{	 m_ReviveTime = time;	}

	BOOL GetSavePointInit()					{	return m_bSavePointInited;	}
	void SetSavePointInit( BOOL val )		{	m_bSavePointInited = val;	}
	
	void InitShopInvenInfo(ITEMBASE* pShopWareHouse);

	CPetManager* GetPetManager()			{	return &m_PetManager;	}
	CPet*	GetCurPet()					{	return m_PetManager.GetCurSummonPet();	}
	
	BOOL GetKyungGongIdx()				{	return m_MoveInfo.KyungGongIdx;	}

protected:
	BOOL	m_bDieForBattleChannel;
	BOOL	m_bDieForGFW;	
	BOOL	m_bDieInSpecialMap;
	BOOL	m_bDieInUnionWarMap;//[盟战地图内死亡][BY:十里坡剑神][QQ:112582793][2019-10-31][20:36:35]
	WORD	m_wReturnMapNum;
	WORD	m_wGTReturnMapNum;
	int		m_ObserverBattleIdx;
public:
	void	SetDieForBattleChannel( BOOL bVal )	{	m_bDieForBattleChannel = bVal;	}	
	BOOL	IsDieForBattleChannel()		{	return m_bDieForBattleChannel;	}
	void	SetDieForGFW( BOOL bDie )	{ m_bDieForGFW = bDie; }
	BOOL	IsDieFromGFW()				{ return m_bDieForGFW; }
	void	SetDieInSpecialMap( BOOL bDie, WORD wReturnMap )	{ m_bDieInSpecialMap = bDie; m_wReturnMapNum = wReturnMap; }	
	//[设置盟战地图内死亡标志][BY:十里坡剑神][QQ:112582793][2019-10-31][20:42:36]
	void	SetDieInUnionWarMap(BOOL bDie){ m_bDieInUnionWarMap = bDie; }
	void	SendPlayerToMap();	
	
	void SetReturnMapNum( WORD num )	{	m_wGTReturnMapNum = num;	}
	WORD GetReturnMapNum()			{	return m_wGTReturnMapNum;	}
	void SetObserverBattleIdx( int Idx )		{	m_ObserverBattleIdx = Idx;	}
	int	GetObserverBattleIdx()				{	return m_ObserverBattleIdx;	}

	void		SetChangeAbilityKyungGongLevel( WORD wLevel );

	DWORD GetGuildUnionIdx()		{ return m_HeroCharacterInfo.dwGuildUnionIdx; }
	void InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx );
	void SetGuildUnionMarkIdx( DWORD dwMarkIdx )	{ m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx; }

	void SetRestraintMode( bool val )			{	m_HeroCharacterInfo.bRestraint = val;	}
	BOOL IsRestraintMode()						{	return m_HeroCharacterInfo.bRestraint;	}
	void ReviveVillage();

	void SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo );	

//#ifdef _JAPAN_LOCAL_
//	int		GetMainCharAttr()		{ return m_HeroCharacterInfo.nMainAttr; }	
//	int		GetSubCharAttr()		{ return m_HeroCharacterInfo.nSubAttr; }	
//	BOOL	ChangeMainCharAttr( int nMainAttr );
//	BOOL	ChangeSubCharAttr( int nSubAttr );	
//	int		WhatIsAttrRelation( int nOtherMainAttr );	
//
//	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
//	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
//	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
//	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
//	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
//	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
//#endif
//
//#ifdef _HK_LOCAL_
//	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
//	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
//	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
//	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
//	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
//	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
//#endif
//
//#ifdef _TL_LOCAL_
//	void	SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot );
//	DWORD	GetExtraInvenSlot()			{	return m_HeroCharacterInfo.ExtraInvenSlot;		}
//	DWORD	GetExtraPyogukSlot()		{	return m_HeroCharacterInfo.ExtraPyogukSlot;		}
//	DWORD	GetExtraMugongSlot()		{	return m_HeroCharacterInfo.ExtraMugongSlot;		}
//	void	SetExtraCharacterSlot( DWORD SlotNum )		{	m_dwCharacterSlotNum = SlotNum;	}
//	DWORD	GetExtraCharacterSlot()		{	return m_dwCharacterSlotNum;	}
//#endif



	void	InitMussangPoint()			{	m_dwMussangPoint = 0;		}
	void	SetMussangMode(BOOL bMode)	{	m_HeroCharacterInfo.bMussangMode = bMode;	}
	BOOL	IsMussangMode()				{	return m_HeroCharacterInfo.bMussangMode;	}
	BOOL	IsMussangReady()			{	return m_bIsMussangReady;	}
	DWORD	StartMussangMode();
	void	CheckMussangTime();
	void	SetMussangMaxPoint();
	DWORD	GetMussangMaxPoint();
	void	AddMussangPointFromLevel(DWORD dwMonsterLevel);
	void	SendMussangInfo();
	void	SendMussangEnd();	
	void	CheatMussangReady();

//#ifdef _JAPAN_LOCAL_
//	virtual DWORD CalcShieldDamage(DWORD ShieldDamage);
//#else
	virtual DWORD CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage);
	WORD	GetMussangStat() { return m_wMussangStat; }
//#endif

	BOOL IsSkipSkill();
	////////////////////////////////////////////////////////
	void AddSkillStatsOption(SKILLOPTION *pSkillOption);
	void RemoveSkillStatsOption(SKILLOPTION *pSkillOption);

	//////////////////////////////////////////////////////////////////////////
	BOOL IsHide() { return m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Hide]; } 
	BOOL IsDetect()
	{ 

		return (m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_Detect] ||
			m_BaseObjectInfo.SingleSpecialState[eSingleSpecialState_DetectItem]); 
	} 
	//////////////////////////////////////////////////////////////////////////
	CYHHashTable<CSkillObject>* GetSkillList() { return &m_SkillList; }
	//////////////////////////////////////////////////////////////////////////
	BOOL CanSkillState();
	////////////////////////////////////////////////////////////////////////////
	void IncreaseEventHammerCount();
	void SummonEventPetRndm();
	void SetEventSnowIdx(int snowIdx)	{ m_CristmasEventSnowIdx = snowIdx;	}
	DWORD GetEventSnowIdx()	{	return m_CristmasEventSnowIdx;	}

	void SendFlagNPCState();

	void InitTitanWearItemInfo( ITEMBASE* pTitanItemInfo );
	void InitTitanShopItemInfo( ITEMBASE* pTitanItemInfo );

	BOOL InTitan();
	BOOL CanUseTitanSkill();
	CTitanManager*	GetTitanManager()	{	return &m_TitanManager;	}
	CTitan* GetCurTitan()	{	return m_TitanManager.GetCurRidingTitan();	}
	void SetTitanWearInfo(POSTYPE slotPos, ITEMBASE* pItemBase, BOOL bIn);
	void ApplyTitanWearItem();

	void AddSetitemOption(SET_ITEM_OPTION* pSetItemOption);
	void ClearSetitemOption();
	///////////////////////////////////////////////////////

	void SetActionTime()				{	m_LastActionTime = gCurTime;	}
	BOOL IsActionPanelty()				{	return m_bActionPanelty;		}
	void SetActionPanelty()				{	m_bActionPanelty = TRUE;		}
	void SendNoActionPaneltyMsg();
	void InsetNoActionPaneltyLog( BYTE eLogKind );

	void AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption);
	void ClearUniqueItemOption()	{	ZeroMemory(&m_UniqueItemStats, sizeof(m_UniqueItemStats));	}
	//////////////////////////////////////////////////////////////////////////
	int GetSOSState()	{	return m_SOSState;	}
	void SetSOSState(int num)	{	m_SOSState = num;	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	void InitSkinDelay();		
	void StartSkinDelay();			
	BOOL GetSkinDelayResult();		
	DWORD GetSkinDelayTime();		
	/////////////////////////////////////////////////////////////////////////////////////////////////

protected:
	DWORD	m_dwAutoNoteAskPlayerIdx;		
	DWORD	m_dwLastAttackTimeForAutoNote;

public:
	void SetAutoAskPlayerIdx( DWORD dwPlayerIdx )	{ m_dwAutoNoteAskPlayerIdx = dwPlayerIdx; }
	DWORD GetAutoAskPlayerIdx()						{ return m_dwAutoNoteAskPlayerIdx; }

	void SetLastAttackTimeForAutoNote( DWORD dwTime )	{ m_dwLastAttackTimeForAutoNote = dwTime; }
	DWORD GetLastAttackTimeForAutoNote()				{ return m_dwLastAttackTimeForAutoNote; }


	void MallMoneyProcess();

	int     GetCurMapNum(){ return m_HeroCharacterInfo.CurMapNum; }
    void	SetPaoDianTime(UINT val){ m_LastPaoDianTime = val; }
	UINT	GetPaoDianTime(){ return m_LastPaoDianTime; }
	void	SetLastMapTime(UINT val){ m_LastMapTime = val; }
	UINT	GetLastMapTime(){ return m_LastMapTime; }
    void	SetConnection(DWORD nIndex){ m_dwConnection = nIndex; }
	DWORD	GetConnection(){ return m_dwConnection; }

	bool    IsFlashNameFlag(){return  m_HeroCharacterInfo.FlashNameFlag!=0;}
	void    SetFlashNameFlag(WORD Flag){ m_HeroCharacterInfo.FlashNameFlag=Flag;}
	WORD    GetFlashNameFlag(){return m_HeroCharacterInfo.FlashNameFlag;}
	
	void	GetDBGoldMoney();//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/28]
	void	UpdateGoldMoney(DWORD DBGoldMoney,DWORD ChangeMoney,BOOL bIsSend=FALSE);//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/28]
	void    UpdateMallMoney(DWORD DBMallMoney,DWORD ChangeMoney,BOOL bIsSend=FALSE);
	//[图片称号函数定义][By:十里坡剑神][QQ:112582793][2017/11/29]
	bool IsImageName(){return m_HeroCharacterInfo.ImageName != 0;}
	void SetImageName(WORD val){m_HeroCharacterInfo.ImageName = val;}
	WORD GetImageName(){return m_HeroCharacterInfo.ImageName;}
	//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
	void AddItemGrowKillMonster(CMonster * Monster);
	void SetBasicPoint(WORD val);
	WORD GetBasicPoint(){return m_HeroInfo.BasicPoint;}

	//[设置角色杀人计数][By:十里坡剑神][QQ:112582793][2018/4/8]
	void SetPlayerKillTimes(DWORD val){m_KillTime = val;}
	//自动加点服务端实现	by:十里坡剑神	QQ:112582793
	void	SetAddPointFlag(BYTE flag);
	void	AutoAddPoint(WORD val);
	void	SetAddPoint(BOOL val);
	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	//void  AddKillTimes(CPlayer* pAttacker);
	void  AddKillTimes();
	void ClearKillTimes();
	//void RemoveKillCheck();
	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	BOOL  IsSafeLock(){return m_bIsSafeLock;}
	void SetSafeLock(BOOL val){m_bIsSafeLock = val;}
	void SetKilledPlayerID(DWORD val){m_KilledPlayerID = val;}
	DWORD GetKilledPlayerID(){return m_KilledPlayerID;}
	void SetLootPos(DWORD val){m_LootingPos = val;}
	DWORD GetLootPos(){return m_LootingPos;}
	//[假人添加测试][By:十里坡剑神][QQ:112582793][2018/4/22]
	BOOL  IsRobot(){return m_bIsRobot;}
	void  SetRobot(BOOL val){m_bIsRobot = val;}
	void GetItemtotalInfo(ITEMBASE Inventory[]);
	//[背包整理获取物品][By:十里坡剑神][QQ:112582793][2018/6/11]
	void MoveInventPack(bool &bRet);
	void SetItemTotalInfo(ITEMBASE * pItemBaseAll);
	DWORD m_MovePackTime;
	DWORD IsCanMovePack();
	void SetMovePackTime(){m_MovePackTime = gCurTime;}

	void InitUnionWarKillTime(){ m_HeroInfo.dwUnionWarKill = 0; m_HeroInfo.dwUnionWarDie = 0; }

	void SetUnionWarKillSub(){ m_HeroInfo.dwUnionWarKill -= 1; }
	void SetUnionWarKillTimeAdd(){ m_HeroInfo.dwUnionWarKill +=1; }
	DWORD GetUnionWarKillTime(){ return m_HeroInfo.dwUnionWarKill; }

	void SetUnionWarDieSub(){ m_HeroInfo.dwUnionWarDie -= 1; }
	void SetUnionWarDieTimeAdd(){ m_HeroInfo.dwUnionWarDie += 1; }
	DWORD GetUnionWarDieTimes(){ return m_HeroInfo.dwUnionWarDie; }

	DWORD GetKillTimes(){ return m_KillTime; }
	void SetKillTimes(DWORD val){ m_KillTime = val; }
};

#endif // !defined(AFX_PLAYER_H__D36AF2E1_0FD9_4120_B257_64B0BE21D669__INCLUDED_)


