// Player.cpp: implementation of the CPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "Monster.h"
#include "StatsCalcManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "CharMove.h"
#include "PackedData.h"

#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "SkillManager_Server.h"
#include "KyungGongManager.h"
#include "ObjectEvent.h"
#include "ObjectStateManager.h"
#include "streetstallmanager.h"
#include "PeaceWarModManager.h"
#include "AbilityManager.h"
#include "CharacterCalcManager.h"
#include "Wantedmanager.h"
#include "GuildManager.h"
#include "MugongManager.h"
#include "VimuStreet/Battle_Vimustreet.h"
#include "StateMachinen.h"
#include "MHError.h"

#include "CQuestBase.h"
#include "Quest.h"

#include "TileManager.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "mhtimemanager.h"

#include "LootingManager.h"
#include "PKManager.h"
#include "QuestManager.h"
#include "PyogukManager.h"
#include "FameManager.h"
#include "EventMapMgr.h"
#include "UserTable.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "SiegeWarProfitMgr.h"
#include "StreetStall.h"
#include "Pet.h"
#include "PetManager.h"
#include "ServerSystem.h"
#include "SurvivalModeManager.h"
#include "Titan.h"
#include "ChannelSystem.h"
#include "WeatherManager.h"
#include "GameEventManager.h"
#include "FortWarManager.h"

#include "MapNetworkMsgParser.h" // include for mallmoney process! 2014-05-30
#include <time.h>

#include "../[CC]BattleSystem/BattleSystem_Server.h"
#include "../[CC]Suryun/SuryunManager_Server.h"
#include "SingedManager.h"			//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
#include "RechargeRecordManager.h"	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "../UnionWarManager.h"		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-23][15:06:46]
#define INSERTLOG_TIME			600000 // 10 min
#define INSERTLOG_TIME_CHINA	1800000 // 30 min
#define INSERTLOG_TIME_HK		1800000 // 30 min

extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;
extern int	g_nServerSetNum;
extern WORD g_MapNum;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ITEM_RARE_OPTION_INFO CPlayer::m_NULLRAREINFO;
ITEM_OPTION_INFO CPlayer::NULLOPTIONINFO;

CPlayer::CPlayer()
{
	m_AbilityGroup.SetOwenerObject(this);
	m_SkillList.Initialize(10);
	m_SkillFailCount = 0;
	memset(m_szWantMarryName, 0, MAX_NAME_LENGTH + 1);
	m_KillTime = 0;
}

CPlayer::~CPlayer()
{
	m_SkillList.RemoveAll();
}

void CPlayer::InitClearData()
{
	m_ItemContainer.Init();
	m_ItemContainer.SetInit(eItemTable_Inventory,	TP_INVENTORY_START,		SLOT_INVENTORY_NUM,		&m_InventorySlot);
	m_ItemContainer.SetInit(eItemTable_Weared,		TP_WEAR_START,			SLOT_WEAR_NUM,			&m_WearSlot);
	m_ItemContainer.SetInit(eItemTable_Pyoguk,		TP_PYOGUK_START,		SLOT_PYOGUK_NUM,		&m_PyogukSlot);
	m_ItemContainer.SetInit(eItemTable_Shop,		TP_SHOPITEM_START,		SLOT_SHOPITEM_NUM,		&m_ShopItemSlot);	
	m_ItemContainer.SetInit(eItemTable_ShopInven,	TP_SHOPINVEN_START,		SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM,	&m_ShopInvenSlot);
	m_ItemContainer.SetInit(eItemTable_PetInven,	TP_PETINVEN_START,		SLOT_PETINVEN_NUM,		&m_PetInvenSlot);
	m_ItemContainer.SetInit(eItemTable_PetWeared,	TP_PETWEAR_START,		SLOT_PETWEAR_NUM,		&m_PetWearSlot);
	m_ItemContainer.SetInit(eItemTable_Titan,		TP_TITANWEAR_START,		SLOT_TITANWEAR_NUM,		&m_TitanWearSlot);
	m_ItemContainer.SetInit(eItemTable_TitanShopItem,		TP_TITANSHOPITEM_START,		SLOT_TITANSHOPITEM_NUM,		&m_TitanShopItemSlot);
	//////////////////////////////////////////////////////////////////////////
	memset(&m_HeroCharacterInfo,0,sizeof(CHARACTER_TOTALINFO));
	memset(&m_HeroInfo,0,sizeof(HERO_TOTALINFO));
	memset(&m_HeroMugongInfo, 0, sizeof(MUGONG_TOTALINFO));
	memset(&m_itemStats,0,sizeof(m_itemStats));
	memset(&m_setItemStats,0,sizeof(m_setItemStats));		
	memset(&m_ShopItemOption,0,sizeof(SHOPITEMOPTION));
	for(int i=eAvatar_Weared_Hair; i<eAvatar_Max; i++)
		m_ShopItemOption.Avatar[i]=1;
	memset(&m_AvatarOption, 0 , sizeof(AVATARITEMOPTION));
		memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	memset(&(m_BaseObjectInfo.SingleSpecialState), 0, sizeof(bool) * eSingleSpecialState_Max);
	//////////////////////////////////////////////////////////////////////////
	m_ItemArrayList.Initialize(50);
	m_FollowMonsterList.Initialize(5);
	m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_QuestList.Initialize(30);
	m_QuestGroup.Initialize( this );
	m_ShopItemManager.Init( this );
	m_PetManager.Init( this );
	m_TitanManager.Init( this );
	m_InitPosition = 0;
	m_bSavePointInited = FALSE;
	m_bPetInvenInit = FALSE;
	m_wKillMonsterLevel = 0;
//#ifndef _JAPAN_LOCAL_
	m_dwMussangTime = 0;
	m_wMussangStat = 0;
//#endif

//#ifdef _JAPAN_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif
//#ifdef _HK_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif
//#ifdef _TL_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif


	m_bGotWarehouseItems = FALSE;
	m_SkillFailCount = 0;
	m_FullMoonEventHCt = 0;
	m_fFullMoonEventTime = 0.f;
	m_LastActionTime = 0;
	m_bActionPanelty = TRUE;

	m_dwAutoNoteAskPlayerIdx = 0;
	m_dwLastAttackTimeForAutoNote = 0;

	m_LastPaoDianTime = time(NULL);
	//[自动加点过图默认关闭][By:十里坡剑神][QQ:112582793][2018/4/17]
	m_IsAutoAddPoint = FALSE;
	m_AutoAddPointFlag = 0;
	m_KilledPlayerID = 0;
	m_LootingPos = 0;

	m_bIsRobot = FALSE;
	this->m_LastRecvPingSYNTime=0;
	this->m_PingSYNFastCount=0;
	//[背包整理限制时间][By:十里坡剑神][QQ:112582793][2018/6/11]
	m_MovePackTime = 0;
}

BOOL CPlayer::Init(EObjectKind kind,DWORD AgentNum, BASEOBJECT_INFO* pBaseObjectInfo)
{
	m_ItemGrowCount=0;
	m_DelayGroup.Init();
	m_StreetStallTitle[0] = 0;
	m_bPetInvenInit = FALSE;
	m_bPyogukInit = 0;
	m_bShopInit = FALSE;
	m_WantedIDX = 0;
	m_MurdererIDX = 0;
	m_MurdererKind = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;

	m_bReadyToRevive = FALSE;
	m_bSavePointInited = FALSE;
	m_bVimuing		= FALSE;
	m_bExit			= FALSE;
	m_bNormalExit	= FALSE;
	CObject::Init(kind, AgentNum, pBaseObjectInfo); 
	OBJECTSTATEMGR_OBJ->StartObjectState( this, eObjectState_Immortal, 0 );
//#ifdef _HK_LOCAL_
//	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 60000 );
//#else
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
//#endif 
	m_CurComboNum = 0;
	m_ChangeNpcIndex = 0;
	SetGravity(0);
	m_pGuetStall = NULL;
	m_bNeedRevive = TRUE;
	ZeroMemory( &m_GameOption, sizeof( m_GameOption ) );
	m_LoginTime = 0;
	m_ContinuePlayTime = 0;
	m_nHackCount = 0;
	m_dwHackStartTime = gCurTime;
	m_dwSkillDelayStartTime = gCurTime;	
	//m_dwLastSkillAnimationTime = 0;
	m_dwLastSocietyAction = 0;
	m_wKillMonsterLevel = 0;

	m_ReviveTime = 0;

	m_bDieForBattleChannel	= FALSE;
	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
	m_bDieInUnionWarMap = FALSE;
	InitMussangPoint();
	m_dwMussangStartTime = 0;
	m_bIsMussangReady = FALSE;
	m_bMussangFlag = 0;
	m_bWaitExitPlayer = TRUE;
	m_dwMussangTime = 0;
	m_wMussangStat = 0;
	m_bWaitExitPlayer = TRUE;
	memset(&m_SkillStatsOption, 0, sizeof(SKILLSTATSOPTION));
	memset(&(m_BaseObjectInfo.SingleSpecialState), 0, sizeof(bool) * eSingleSpecialState_Max);

//#ifdef _JAPAN_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif
//
//#ifdef _HK_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif
//
//#ifdef _TL_LOCAL_
//	m_dwCharacterSlotNum = 0;
//#endif

	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;
	m_FullMoonEventHCt = 0;
	m_fFullMoonEventTime = 0.f;
	m_dwHpMpBitFlag = 0x00;	

	m_CristmasEventSnowIdx = 0;

	InitSkinDelay();

	m_LastPaoDianTime = time(NULL);

	SetLastMapTime(0);
    this->m_LastRecvPingSYNTime=0;
    this->m_PingSYNFastCount=0;
	//[自动加点过图默认关闭][By:十里坡剑神][QQ:112582793][2018/4/17]
	m_IsAutoAddPoint = FALSE;
	m_AutoAddPointFlag = 0;
	m_KilledPlayerID = 0;
	m_LootingPos = 0;
	m_bIsRobot = FALSE;
	//[背包整理限制时间][By:十里坡剑神][QQ:112582793][2018/6/11]
	m_MovePackTime = 0;
	return TRUE;
}
void CPlayer::Release()
{
	//SAFE_DELETE(m_pItemContainer);
	m_DelayGroup.Release();
	m_AbilityGroup.Release();
	m_ItemContainer.Release();

	ITEMOBTAINARRAYINFO* pInfo;
	m_ItemArrayList.SetPositionHead();
	while (pInfo = m_ItemArrayList.GetData())
	{
		ITEMMGR->Free(this, pInfo);
	}
	m_ItemArrayList.RemoveAll();

	m_FollowMonsterList.RemoveAll();

	ITEM_RARE_OPTION_INFO* pRInfo = NULL;
	m_ItemRareOptionList.SetPositionHead();
	while(pRInfo = m_ItemRareOptionList.GetData())
		delete pRInfo;
	m_ItemRareOptionList.RemoveAll();
	
	ITEM_OPTION_INFO * pOInfo = NULL;
	m_ItemOptionList.SetPositionHead();
	while(pOInfo = m_ItemOptionList.GetData())
		delete pOInfo;
	m_ItemOptionList.RemoveAll();

	
	CQuestBase* pQuest;
	m_QuestList.SetPositionHead();
	while(pQuest = m_QuestList.GetData())
	{
		delete pQuest;
	}
	m_QuestList.RemoveAll();


	m_InventoryPurse.Release();
	m_PyogukPurse.Release();

	m_WantedList.RemoveAll();

	m_QuestGroup.Release();

	m_ShopItemManager.Release();

	m_PetManager.Release();

	m_TitanManager.Release();
	
	CObject::Release();

	m_SkillList.RemoveAll();
	m_SkillFailCount = 0;
}

void CPlayer::AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo)
{
	if( pInfo == 0 ) return;

	if( m_ItemRareOptionList.GetData(pInfo->dwRareOptionIdx) ) return;
	
	ITEM_RARE_OPTION_INFO* pNewRare = new ITEM_RARE_OPTION_INFO;
	memcpy(pNewRare, pInfo, sizeof(ITEM_RARE_OPTION_INFO));
	m_ItemRareOptionList.Add(pNewRare, pNewRare->dwRareOptionIdx);
}

BOOL CPlayer::RemoveItemRareOption(DWORD dwRareIdx)
{
	ASSERT(m_ItemRareOptionList.GetData(dwRareIdx));

	ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	if( pRareInfo = m_ItemRareOptionList.GetData(dwRareIdx) )
	{
		SAFE_DELETE( pRareInfo );
		m_ItemRareOptionList.Remove(dwRareIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
	
ITEM_RARE_OPTION_INFO* CPlayer::GetItemRareOption(DWORD dwRareIdx)
{
	ITEM_RARE_OPTION_INFO* pInfo = m_ItemRareOptionList.GetData(dwRareIdx);
	if( pInfo )
		return pInfo;
	memset(&m_NULLRAREINFO,0,sizeof(ITEM_RARE_OPTION_INFO));
	return &m_NULLRAREINFO;
}

void CPlayer::AddItemOption(ITEM_OPTION_INFO * pInfo)
{
	if(pInfo == 0)
	{
		return;
	}
	if(m_ItemOptionList.GetData(pInfo->dwOptionIdx))
	{
		return;
	}
	ITEM_OPTION_INFO * pNewInfo = new ITEM_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_OPTION_INFO));
	m_ItemOptionList.Add(pNewInfo, pNewInfo->dwOptionIdx);
}

BOOL CPlayer::RemoveItemOption(DWORD dwOptionIdx)
{	//需要修复
	ASSERT(m_ItemOptionList.GetData(dwOptionIdx));
	ITEM_OPTION_INFO * pNewInfo = m_ItemOptionList.GetData(dwOptionIdx);

	if( pNewInfo )
	{
		SAFE_DELETE(pNewInfo);
		m_ItemOptionList.Remove(dwOptionIdx);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

ITEM_OPTION_INFO * CPlayer::GetItemOption(DWORD dwOptionIdx)
{
	ITEM_OPTION_INFO* pInfo = m_ItemOptionList.GetData(dwOptionIdx);
	if(pInfo)
		return pInfo;
	memset(&NULLOPTIONINFO,0,sizeof(ITEM_OPTION_INFO));
	return &NULLOPTIONINFO;
}


DWORD CPlayer::GetGravity()
{
	DWORD totalG = 0;
	CMonster * pObject;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = m_FollowMonsterList.GetData())
	{
			totalG += pObject->GetGravity();
	}
	return totalG;
}

BOOL CPlayer::AddFollowList(CMonster * pMob)
{	
	if( m_FollowMonsterList.GetDataNum() < 50 )	
	{
		m_FollowMonsterList.Add(pMob, pMob->GetID());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CPlayer::RemoveFollowAsFarAs(DWORD GAmount, CObject* pMe )
{	
	VECTOR3 * ObjectPos	= CCharMove::GetPosition(this);
	BOOL bMe = FALSE;

	while(GAmount > 100)
	{	
		CMonster * pObject = NULL;
		CMonster * MaxObject = NULL;
		float	MaxDistance	= -1;
		float	Distance	= 0;

		m_FollowMonsterList.SetPositionHead();
		while(pObject = m_FollowMonsterList.GetData())
		{
			VECTOR3 * TObjectPos = CCharMove::GetPosition(pObject);
			if((Distance = CalcDistanceXZ( ObjectPos, TObjectPos )) > MaxDistance)
			{
				MaxDistance = Distance;
				MaxObject = pObject;
			}
		}
		if(MaxObject)
		{
			if(GAmount > MaxObject->GetGravity())
				GAmount -= MaxObject->GetGravity();
			else
				GAmount = 0;
			MaxObject->SetTObject(NULL);
			GSTATEMACHINE.SetState(MaxObject, eMA_WALKAROUND);
			if( pMe == MaxObject )
				bMe = TRUE;
		}
		else
		{
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("amount != 0"));
			GAmount = 0;
		}
	}

	return bMe;	
}

void CPlayer::RemoveFollowList(DWORD ID)
{
	m_FollowMonsterList.Remove(ID);
}

ITEMOBTAINARRAYINFO * CPlayer::GetArray(WORD id)
{
	return m_ItemArrayList.GetData(id);
}
void CPlayer::AddArray(ITEMOBTAINARRAYINFO * pInfo)
{
	m_ItemArrayList.Add(pInfo, pInfo->wObtainArrayID);
}
void CPlayer::RemoveArray(DWORD id)
{
	m_ItemArrayList.Remove(id);
}
void CPlayer::InitCharacterTotalInfo(CHARACTER_TOTALINFO* pTotalInfo)
{
	memcpy(&m_HeroCharacterInfo,pTotalInfo,sizeof(CHARACTER_TOTALINFO));
	if( GetUserLevel() == eUSERLEVEL_GM )
		m_HeroCharacterInfo.bVisible = FALSE;
	else
		m_HeroCharacterInfo.bVisible = TRUE;
}

void CPlayer::InitHeroTotalInfo(HERO_TOTALINFO* pHeroInfo)
{
	memcpy(&m_HeroInfo,pHeroInfo,sizeof(HERO_TOTALINFO));
	m_HeroInfo.MaxMussangPoint = GetMussangMaxPoint();
	m_ItemContainer.GetSlot(eItemTable_Inventory)->CreatePurse(&m_InventoryPurse, this, m_HeroInfo.Money, MAX_INVENTORY_MONEY);
	m_dwLastReduceHour = ( m_HeroInfo.Playtime - m_HeroInfo.LastPKModeEndTime ) / 3600; 
}

void CPlayer::InitMugongTotalInfo(MUGONG_TOTALINFO* pMugongInfo)
{
	memcpy(&m_HeroMugongInfo,pMugongInfo,sizeof(MUGONG_TOTALINFO));
}

void CPlayer::InitItemTotalInfo(ITEM_TOTALINFO* pItemInfo)
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemInfo->Inventory);	
	m_ItemContainer.GetSlot(eItemTable_Weared)->SetItemInfoAll(pItemInfo->WearedItem);
}

void CPlayer::InitShopInvenInfo(ITEMBASE* pShopWareHouse)
{
	m_ItemContainer.GetSlot(eItemTable_ShopInven)->SetItemInfoAll(pShopWareHouse);	
}

void CPlayer::AddPyogukItem(ITEMBASE * pPyogukItem)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->InsertItemAbs(this, pPyogukItem->Position, pPyogukItem);
}

void CPlayer::InitPyogukInfo(BYTE Pyoguknum,MONEYTYPE money)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->SetPyogukNum(Pyoguknum);

	MONEYTYPE maxmoney = 0;
	if(Pyoguknum)
	{		
		PYOGUKLISTINFO* pInfo = PYOGUKMGR->GetPyogukInfo(Pyoguknum);
		ASSERT(pInfo);
		maxmoney = pInfo ? pInfo->MaxMoney : 10;
	}
	else
	{
		ASSERT(money == 0);
		maxmoney = 0;
	}
	pSlot->CreatePurse(&m_PyogukPurse, this, money, maxmoney);	
}

void CPlayer::InitPyogukItemInfo(PYOGUK_ITEM* pItemInfo)
{
	int rt;
	CPyogukSlot * pSlot;
	pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	if(m_bPyogukInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Pyoguk);
		m_bPyogukInit = TRUE;
	}
	else
	{
		for(int i=0; i<SLOT_PYOGUK_NUM; ++i )
		{
			if(pItemInfo->Pyoguk[i].dwDBIdx != 0)
			{		
				rt = pSlot->InsertItemAbs(this, pItemInfo->Pyoguk[i].Position, &pItemInfo->Pyoguk[i]);
				if( rt != EI_TRUE)
				{
				}
			}
		}
	}
}

void CPlayer::InitPetInvenItemInfo(PETINVEN_ITEM* pItemInfo)
{
	int rt = 0;

	CItemSlot* pSlot = NULL;
	pSlot = m_ItemContainer.GetSlot(eItemTable_PetInven);
	
	pSlot->SetItemInfoAll(pItemInfo->PetInven);
}

void CPlayer::SendPetInvenItemInfo()
{
	SEND_PETINVEN_ITEM_INFO msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_PETINVEN_INFO_ACK;

	ITEM_OPTION_INFO OptionInfo[SLOT_PETINVEN_NUM];
	ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_PETINVEN_NUM];
	WORD OptionNum = 0;
	WORD RareOptionNum = 0;

	CItemSlot* pSlot = GetSlot(eItemTable_PetInven);
	POSTYPE StartPos = TP_PETINVEN_START;

	for( int i = 0; i < SLOT_PETINVEN_NUM; ++i )
	{
		if( pSlot->IsEmpty(StartPos+i) == FALSE )
		{
			msg.PetInvenItem[i] = *pSlot->GetItemInfoAbs(StartPos+i);

			if(ITEMMGR->IsOptionItem(msg.PetInvenItem[i].wIconIdx, msg.PetInvenItem[i].Durability))
			{
				memcpy(&OptionInfo[OptionNum], GetItemOption(msg.PetInvenItem[i].Durability), sizeof(ITEM_OPTION_INFO));
				++OptionNum;
			}
			if(ITEMMGR->IsRareOptionItem(msg.PetInvenItem[i].wIconIdx, msg.PetInvenItem[i].RareIdx))
			{
				memcpy(&RareOptionInfo[RareOptionNum], GetItemRareOption(msg.PetInvenItem[i].RareIdx), sizeof(ITEM_RARE_OPTION_INFO));
				++RareOptionNum;
			}
		}
	}

	if( OptionNum )
	{
		msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
		msg.wOptionCount = OptionNum;
	}
	if( RareOptionNum )
	{
		msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*RareOptionNum, RareOptionInfo );
		msg.wRareOptionCount = RareOptionNum;
	}

	SendMsg(&msg,msg.GetSize());
}

void CPlayer::InitPetWearItemInfo( ITEMBASE* pWearItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_PetWeared)->SetItemInfoAll(pWearItemInfo);	
}

void CPlayer::InitShopItemInfo(SHOP_ITEM* pItemInfo)
{
	CShopItemSlot* pSlot = (CShopItemSlot*)m_ItemContainer.GetSlot(eItemTable_Shop);
	if(m_bShopInit == FALSE)
	{
		pSlot->SetItemInfoAll(pItemInfo->Item);

		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )			
				pSlot->SetLock(pItemInfo->Item[i].Position, TRUE);

			ITEM_INFO * pItemListInfo = ITEMMGR->GetItemInfo(pItemInfo->Item[i].wIconIdx);

			if(pItemListInfo)
			{
				if( pItemListInfo->ItemKind == eSHOP_ITEM_PET )
				{
					PET_TOTALINFO* pPetInfo = m_PetManager.GetPetTotalInfo(pItemInfo->Item[i].dwDBIdx, eWithNULL );
					if( NULL == pPetInfo )
					{
						g_pServerSystem->CreateNewPet(this, pItemListInfo->ItemIdx, pItemInfo->Item[i].dwDBIdx,  pItemListInfo->Plus_MugongIdx);
					}
				}
			}
		}
		
		m_bShopInit = TRUE;
	}
	else
	{
		for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
		{
			if( pItemInfo->Item[i].dwDBIdx != 0 )
			{				
				if( pSlot->InsertItemAbs( this, pItemInfo->Item[i].Position, &pItemInfo->Item[i] ) != EI_TRUE )
				{
					ASSERT(0);
				}
			}
		}
	}
}

void CPlayer::SetPyogukNum(BYTE n)
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	pSlot->SetPyogukNum(n);
}

BYTE CPlayer::GetPyogukNum()
{
	CPyogukSlot * pSlot = (CPyogukSlot *)m_ItemContainer.GetSlot(eItemTable_Pyoguk);
	return pSlot->GetPyogukNum();
}

MONEYTYPE CPlayer::GetMoney(eITEMTABLE tableIdx)
{
	CItemSlot* pSlot = m_ItemContainer.GetSlot(tableIdx);
	return pSlot->GetMoney();
}

void CPlayer::GetCharacterTotalInfo(CHARACTER_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroCharacterInfo,sizeof(CHARACTER_TOTALINFO));

	pRtInfo->genGoal = GetGenGol();
	pRtInfo->simmak = GetSimMek();
	pRtInfo->minchub = GetMinChub();
	pRtInfo->cheryuk = GetCheRyuk();
	pRtInfo->Fame = m_HeroInfo.Fame;
	pRtInfo->defenseRate = GetPhyDefense();
	pRtInfo->critical = GetCritical();
	pRtInfo->minAttack = DoGetPhyAttackPowerMin();
	pRtInfo->maxAttack = DoGetPhyAttackPowerMax();
	pRtInfo->naeryuk = GetNaeRyuk();
}
HERO_TOTALINFO* CPlayer::GetHeroTotalInfo()
{
	return &m_HeroInfo;
}

void CPlayer::GetHeroTotalInfo(HERO_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroInfo,sizeof(HERO_TOTALINFO));
}

void CPlayer::GetMugongTotalInfo(MUGONG_TOTALINFO* pRtInfo)
{
	memcpy(pRtInfo,&m_HeroMugongInfo,sizeof(MUGONG_TOTALINFO));
}
ITEMBASE * CPlayer::GetItemInfoFromDBIdx(DWORD dwDBIdx)
{
	ITEM_TOTALINFO pRtInfo;
	m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(pRtInfo.Inventory);
	for(int i = 0;i<SLOT_INVENTORY_NUM;i++)
	{
		if(pRtInfo.Inventory[i].dwDBIdx==dwDBIdx)
			return &pRtInfo.Inventory[i];
	}
	m_ItemContainer.GetSlot(eItemTable_Weared)->GetItemInfoAll(pRtInfo.WearedItem);
	for(int i = 0;i<eWearedItem_Max;i++)
	{
		if(pRtInfo.WearedItem[i].dwDBIdx==dwDBIdx)
			return &pRtInfo.WearedItem[i];
	}
	m_ItemContainer.GetSlot(eItemTable_ShopInven)->GetItemInfoAll(pRtInfo.ShopInventory);
	for (int i = 0; i<TABCELL_SHOPINVEN_NUM + TABCELL_SHOPINVEN_PLUS_NUM; i++)
	{
		if(pRtInfo.ShopInventory[i].dwDBIdx==dwDBIdx)
			return &pRtInfo.ShopInventory[i];
	}
	m_ItemContainer.GetSlot(eItemTable_PetWeared)->GetItemInfoAll(pRtInfo.PetWearedItem);
	for(int i = 0;i<SLOT_PETWEAR_NUM;i++)
	{
		if(pRtInfo.PetWearedItem[i].dwDBIdx==dwDBIdx)
			return &pRtInfo.PetWearedItem[i];
	}
	memset(&pRtInfo,0,sizeof(ITEM_TOTALINFO));
	return &pRtInfo.Inventory[0];
}

void CPlayer::GetItemtotalInfo(ITEM_TOTALINFO* pRtInfo,DWORD dwFlag)
{
	if(dwFlag & GETITEM_FLAG_INVENTORY)
	{
		m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(pRtInfo->Inventory);
		m_ItemContainer.GetSlot(eItemTable_ShopInven)->GetItemInfoAll(pRtInfo->ShopInventory);
	}
	else
	{
		memset(pRtInfo->Inventory,0,sizeof(ITEMBASE)*(SLOT_INVENTORY_NUM+40));
		memset(pRtInfo->ShopInventory,0,sizeof(ITEMBASE)*(SLOT_SHOPINVEN_NUM+40));
	}
	if(dwFlag & GETITEM_FLAG_WEAR)
		m_ItemContainer.GetSlot(eItemTable_Weared)->GetItemInfoAll(pRtInfo->WearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*eWearedItem_Max);

	if(dwFlag & GETITEM_FLAG_PETWEAR)
		m_ItemContainer.GetSlot(eItemTable_PetWeared)->GetItemInfoAll(pRtInfo->PetWearedItem);
	else
		memset(pRtInfo->WearedItem,0,sizeof(ITEMBASE)*SLOT_PETWEAR_NUM);
	if(dwFlag & GETITEM_FLAG_TITANWEAR)
		m_ItemContainer.GetSlot(eItemTable_Titan)->GetItemInfoAll(pRtInfo->TitanWearedItem);
	else
		memset(pRtInfo->TitanWearedItem,0,sizeof(ITEMBASE)*SLOT_TITANWEAR_NUM);
	if(dwFlag & GETITEM_FLAG_TITANSHOP)
		m_ItemContainer.GetSlot(eItemTable_TitanShopItem)->GetItemInfoAll(pRtInfo->TitanShopItem);
	else
		memset(pRtInfo->TitanShopItem,0,sizeof(ITEMBASE)*SLOT_TITANSHOPITEM_NUM);
}

void CPlayer::GetPyogukItemInfo(PYOGUK_ITEM * pRtInfo)
{
	m_ItemContainer.GetSlot(eItemTable_Pyoguk)->GetItemInfoAll(pRtInfo->Pyoguk);
}

void CPlayer::SetAddMsg(char* pAddMsg,WORD* pMsgLen,DWORD dwReceiverID,BOOL bLogin)
{
	if( m_dwUserLevel <= eUSERLEVEL_GM && m_HeroCharacterInfo.bVisible == FALSE )
	{
		*pMsgLen = 0;
		return;
	}

	SEND_CHARACTER_TOTALINFO* pMsg = (SEND_CHARACTER_TOTALINFO*)pAddMsg;
	pMsg->AddableInfo.Init();

	pMsg->Category = MP_USERCONN;
	pMsg->Protocol = MP_USERCONN_CHARACTER_ADD;
	pMsg->dwObjectID = dwReceiverID;
	GetSendMoveInfo(&pMsg->MoveInfo,&pMsg->AddableInfo,TRUE);
	pMsg->MoveInfo.AbilityKyungGongLevel = GetAbilityStats()->KyunggongLevel;
	GetBaseObjectInfo(&pMsg->BaseObjectInfo);
	GetCharacterTotalInfo(&pMsg->TotalInfo);	
	memcpy(&pMsg->ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
	pMsg->bLogin = bLogin;

	cStreetStall* pStall = STREETSTALLMGR->FindStreetStall(this);
	if( pStall != NULL)
	{
		char StallTitle[MAX_STREETSTALL_TITLELEN+1];
		GetStreetStallTitle(StallTitle);
		StallTitle[MAX_STREETSTALL_TITLELEN] = 0;
		
		if( pStall->GetStallKind() == eSK_SELL)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetStall,strlen(StallTitle)+1,StallTitle);
		else if( pStall->GetStallKind() == eSK_BUY)
			pMsg->AddableInfo.AddInfo(CAddableInfoList::StreetBuyStall,strlen(StallTitle)+1,StallTitle);
	}

	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET )
	{
		COMPRESSEDPOS VimuPos;

		BATTLE_INFO_VIMUSTREET info;
		WORD wSize;
		GetBattle()->GetBattleInfo( (char*)&info, &wSize );

		VimuPos.Compress(&info.vStgPos);

		pMsg->AddableInfo.AddInfo(CAddableInfoList::VimuInfo,sizeof(COMPRESSEDPOS),&VimuPos);
	}
	
	int nTeam = PARTYWARMGR->IsMemberInPartyWar( this );
	if( nTeam != 0 )
	{
		pMsg->AddableInfo.AddInfo(CAddableInfoList::PartyWar, sizeof(nTeam), &nTeam);
	}

	if( GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
		nTeam = GetBattle()->GetBattleTeamID( this );		
		pMsg->AddableInfo.AddInfo(CAddableInfoList::GTournament, sizeof(nTeam), &nTeam);
	}
	else if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		SEIGEWAR_CHARADDINFO info;
		info.Team = (BYTE)GetBattle()->GetBattleTeamID( this );
		if( SIEGEWARMGR->GetEngraveIdx() == GetID() )
			info.bEngrave = 1;
		else
			info.bEngrave = 0;		

		pMsg->AddableInfo.AddInfo(CAddableInfoList::SiegeWar, sizeof(SEIGEWAR_CHARADDINFO), &info);
	}

	*pMsgLen = pMsg->GetMsgLength();
}

void CPlayer::SendMsg(MSGBASE* pMsg,int MsgLen)
{
	CObject::SendMsg(pMsg, MsgLen);
}
void CPlayer::SendMsgToAllAgent(MSGBASE* pMsg,int MsgLen)
{
	CObject::SendMsgToAllAgent(pMsg,MsgLen);
}
void CPlayer::SetStreetStallTitle(char* title)
{
	SafeStrCpy( m_StreetStallTitle, title, MAX_STREETSTALL_TITLELEN+1 );
}

void CPlayer::GetStreetStallTitle(char* title)
{
	SafeStrCpy( title, m_StreetStallTitle, MAX_STREETSTALL_TITLELEN+1);
}

void CPlayer::DoSetInited()
{
	STATSMGR->CalcItemStats(this);	
	
	SetLife(m_HeroCharacterInfo.Life);
	SetShield(m_HeroCharacterInfo.Shield);
	SetNaeRyuk(m_HeroInfo.wNaeRyuk);

	SetPlayerLevelUpPoint(GetPlayerLevelUpPoint());

	m_dwProgressTime = gCurTime;
}

void CPlayer::CalcState()
{
	STATSMGR->CalcItemStats(this);	

	SetLife(m_HeroCharacterInfo.Life);
	SetShield(m_HeroCharacterInfo.Shield);
	SetNaeRyuk(m_HeroInfo.wNaeRyuk);
}

void CPlayer::MoneyUpdate( MONEYTYPE money )
{	
	m_HeroInfo.Money = money;
}


void CPlayer::SetGenGol(WORD val)
{
	m_HeroInfo.wGenGol = val;

	STATSMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_GENGOL_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetMinChub(WORD val)
{
	m_HeroInfo.wMinChub = val;

	STATSMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MINCHUB_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetCheRyuk(WORD val)
{
	m_HeroInfo.wCheRyuk = val;

	STATSMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHERYUK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetSimMek(WORD val)
{
	m_HeroInfo.wSimMek = val;

	STATSMGR->CalcCharStats(this);

	CharacterHeroInfoUpdate(this);

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_SIMMEK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

BOOL CPlayer::UseItem(WORD abs_pos)
{
	return TRUE;
}

BOOL CPlayer::UseConsumptionItem(WORD abs_pos)
{
	return TRUE;
}

BOOL CPlayer::UseEquipItem(WORD abs_pos)
{
	return TRUE;
}

BOOL CPlayer::UseMugongItem(WORD abs_pos)
{
	return TRUE;
}


CItemSlot * CPlayer::GetSlot(POSTYPE absPos)
{
	if( TP_INVENTORY_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_GUILDWAREHOUSE_START <= absPos && absPos < TP_GUILDWAREHOUSE_END )
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	else if( TP_SHOPITEM_START <= absPos && absPos < TP_SHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_PETINVEN_START <= absPos && absPos < TP_PETINVEN_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_PETWEAR_START <= absPos && absPos < TP_PETWEAR_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_TITANWEAR_START <= absPos && absPos < TP_TITANWEAR_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	else if( TP_TITANSHOPITEM_START <= absPos && absPos < TP_TITANSHOPITEM_END )
	{
		return m_ItemContainer.GetSlot(absPos);
	}
	return NULL;
}

CItemSlot * CPlayer::GetSlot(eITEMTABLE tableIdx)
{
	if(tableIdx < eItemTable_TableMax)
		return m_ItemContainer.GetSlot(tableIdx);
	else if( tableIdx == eItemTable_GuildWarehouse)
	{
		return (CItemSlot*)GUILDMGR->GetSlot(GetGuildIdx());
	}
	return NULL;
}

void CPlayer::SetLife(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxlife = GetMaxLife();
	if(val > maxlife)
		val = maxlife;
	if(m_HeroCharacterInfo.Life != val)
	{
		if(bSendMsg == TRUE)
		{
			MSG_INT msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_LIFE_ACK;
			msg.dwObjectID = GetID();
			msg.nData = val - GetLife();
			SendMsg(&msg,sizeof(msg));
			

			//MSG_INT msg2;
			//msg2.Category = MP_CHAR;
			//msg2.Protocol = MP_CHAR_LIFECHANGE_ACK;
			//msg2.dwObjectID = GetID();
			//msg2.nData = val;
			//PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this, &msg2, sizeof(msg2));
		}
		
		if(GetPartyIdx())
			SendPlayerLifeToParty(val, GetPartyIdx());
	}
		
	m_HeroCharacterInfo.Life = val;
}

void CPlayer::SendPlayerLifeToParty(DWORD val, DWORD PartyID)
{	
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERLIFE;
	msg.PlayerID = GetID();
	if( GetMaxLife() )
		msg.GaugePercent = (float)(val*100/GetMaxLife());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}

void CPlayer::SetShield(DWORD val,BOOL bSendMsg) 
{
	if(GetState() == eObjectState_Die)
		return;

	DWORD maxShield = GetMaxShield();
	if(val > maxShield)
		val = maxShield;
	
	if(bSendMsg == TRUE)	
	{
		MSG_INT msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_SHIELD_ACK;
		msg.dwObjectID = GetID();
		msg.nData = val - m_HeroCharacterInfo.Shield;
		SendMsg(&msg,sizeof(msg));
	}

	m_HeroCharacterInfo.Shield = val;
}

void CPlayer::SendPlayerShieldToParty(DWORD val, DWORD PartyID)
{	

}

void CPlayer::SetNaeRyuk(DWORD val,BOOL bSendMsg)
{ 
	if(GetState() == eObjectState_Die)
		return;

	DWORD MaxNaeryuk = GetMaxNaeRyuk();
	if(val > MaxNaeryuk)
		val = MaxNaeryuk;

	if( m_HeroInfo.wNaeRyuk != val)
	{
		if(bSendMsg)
		{
			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.Protocol = MP_CHAR_NAERYUK_ACK;
			msg.dwObjectID = GetID();
			msg.dwData = val;
			SendMsg(&msg,sizeof(msg));
		}
		
		if(m_HeroInfo.PartyID)
			SendPlayerNaeRyukToParty(val, m_HeroInfo.PartyID);
	}
	
	m_HeroInfo.wNaeRyuk = val; 
}

void CPlayer::SendPlayerNaeRyukToParty(DWORD val, DWORD PartyID)
{
	CParty* pParty = PARTYMGR->GetParty(PartyID);
	if(pParty == NULL)
		return;
	
	SEND_PARTY_GAUGE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERNAERYUK;
	msg.PlayerID = GetID();
	if( GetMaxNaeRyuk() )
		msg.GaugePercent = (float)((val*100)/GetMaxNaeRyuk());
	else
	{
		ASSERT(0);
		msg.GaugePercent = 0;
	}
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), msg.PlayerID, GetGridID());
}

void CPlayer::SendMoveInfotoParty()
{

}

void CPlayer::SetMaxLife(DWORD val)
{
	m_HeroCharacterInfo.MaxLife = val;

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXLIFE_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetMaxShield(DWORD val)
{
	m_HeroCharacterInfo.MaxShield = val;

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXSHIELD_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetMaxNaeRyuk(DWORD val)
{
	m_HeroInfo.wMaxNaeRyuk= val;

	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MAXNAERYUK_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddGenGol(WORD val)
{
	SetGenGol(m_HeroInfo.wGenGol+val);
}
void CPlayer::AddSimMek(WORD val)
{
	SetSimMek(m_HeroInfo.wSimMek+val);
}
void CPlayer::AddMinChub(WORD val)
{
	SetMinChub(m_HeroInfo.wMinChub+val);
}
void CPlayer::AddCheRyuk(WORD val)
{
	SetCheRyuk(m_HeroInfo.wCheRyuk+val);
}
void CPlayer::SetPlayerLevelUpPoint(LEVELTYPE val) 
{ 
	if(m_IsAutoAddPoint&&val>0&&m_AutoAddPointFlag!=0)
	{//[自动加点拦截设置剩余属性点直接附加到相应属性上][By:十里坡剑神][QQ:112582793][2018/4/21]
		AutoAddPoint(val);
		return;
	}

	m_HeroInfo.LevelUpPoint=val;
	CharacterHeroInfoUpdate(this);
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_LEVELUPPOINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}

void CPlayer::SetBasicPoint(WORD val)
{
	m_HeroInfo.BasicPoint = val;
	MSG_DWORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_BASICPOINT_NOTIFY;
	msg.dwObjectID = GetID();
	msg.dwData = val;
	SendMsg(&msg,sizeof(msg));
}
void CPlayer::SetLevel(LEVELTYPE level)
{
	if(level > MAX_CHARACTER_LEVEL_NUM ) //MAX_PLAYERLEVEL_NUM 这里将等级限制去掉
		return;

	if(m_HeroCharacterInfo.Level == level) return;

	if(level > m_HeroCharacterInfo.Level)
	{
		if(m_HeroInfo.MaxLevel < level)
		{
			m_HeroInfo.MaxLevel = level;
			if( level > 70 )
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+4);
			else
				SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+2);
			if(level == 99)
				m_HeroInfo.ExpPoint=0;

			static LEVELTYPE lvtbl[8] = { 10, 15, 20, 25, 30, 35, 40, 45 };
			for( int i=0; i<8; ++i )
			{
				if( level == lvtbl[i] )
				{
					GMEvent01( GetUserID(), GetID(), m_HeroInfo.MaxLevel, g_nServerSetNum );
					break;
				}
			}

			GAMEEVENTMGR->ProcessLevelUpEvent( this, m_HeroInfo.MaxLevel );
			GUILDMGR->GuildMemberLevelUpToGuildPoint(this);
			GUILDMGR->GuildStudentLvUpPointDBUpdate(this);
		}
		
		m_HeroCharacterInfo.Level = level;
		
		STATSMGR->CalcItemStats(this);
		

		SetLife(GetMaxLife());
		SetShield(GetMaxShield());
		SetNaeRyuk(GetMaxNaeRyuk());
		SetMussangMaxPoint();
		
		JournalInsertLevel(GetID(), eJournal_Levelup, level);

		SEND_JOURNALINFO msg;
		msg.Category = MP_JOURNAL;
		msg.Protocol = MP_JOURNAL_ADD;
		msg.dwObjectID = GetID();
		msg.m_Info.Param = GetLevel();
		msg.m_Info.Kind = eJournal_Levelup;
		SendMsg(&msg, sizeof(msg));


		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));

		MSG_DWORD msg2;
		msg2.Category = MP_CHAR;
		msg2.Protocol = MP_CHAR_PLAYERLEVELUP_NOTIFY;
		msg2.dwObjectID = GetID();
		msg2.dwData = GetID();
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(this,&msg2,sizeof(msg2));	

		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Level, level, 1 ) );
	}
	else
	{
		m_HeroCharacterInfo.Level = level;

		STATSMGR->CalcItemStats(this);

		MSG_LEVEL msg1;
		msg1.Category = MP_CHAR;
		msg1.Protocol = MP_CHAR_LEVEL_NOTIFY;
		msg1.dwObjectID = GetID();
		msg1.Level = GetLevel();
		msg1.MaxExpPoint = GAMERESRCMNGR->GetMaxExpPoint(level);
		msg1.CurExpPoint = GetPlayerExpPoint();
		SendMsg(&msg1,sizeof(msg1));
	}
	
	ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,GetLevel(),GetAbilityGroup());

	if(GetPartyIdx())
		PARTYMGR->MemberLevelUp(GetPartyIdx(), GetID(), GetLevel());
	if(GetGuildIdx())
		GUILDMGR->MemberLevelUp(GetGuildIdx(), GetID(), GetLevel());
	//[记录99级升级时间][By:十里坡剑神][QQ:112582793][2018/6/10]
	stTIME LevelUpTime;
	SYSTEMTIME loclTime;
	GetLocalTime(&loclTime);
	LevelUpTime.SetTime(loclTime);
	CharacterLevelUp99Log(GetID(), LevelUpTime.value);
}

void CPlayer::SetPlayerExpPoint(EXPTYPE point,BYTE bDie) 
{
	EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());

	if(point >= pointForLevel)
	{
		m_HeroInfo.ExpPoint = point - pointForLevel;

		LEVELTYPE maxLevel = GetMaxLevel();

		SetLevel(m_HeroCharacterInfo.Level+1);
		LEVELTYPE NEWmaxLevel = GetMaxLevel();
		if( NEWmaxLevel > maxLevel)
		SetLevelUpStatPoint();
		CharacterHeroInfoUpdate(this);
		CharacterTotalInfoUpdate(this);
		
		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
		InsertLogExp( eExpLog_LevelUp, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, GetPlayerAbilityExpPoint());

		CObjectEvent::ObjectEvent(this, OE_LEVELUP, &m_HeroCharacterInfo.Level);
	}
	else
	{
		m_HeroInfo.ExpPoint = point;

		LEVELTYPE maxLevel = GetMaxLevel();
		LEVELTYPE curLevel = GetLevel();
		if( maxLevel == curLevel )
		{
			SetNormalStatPoint();	
		}

		MSG_EXPPOINT msg;
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_EXPPOINT_ACK;
		msg.dwObjectID = GetID();
		msg.ExpPoint = m_HeroInfo.ExpPoint;
		if(bDie == 0)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_ACQUIRE;
		else if(bDie == 1)
			msg.ExpKind = MSG_EXPPOINT::EXPPOINTKIND_DIE;

		SendMsg(&msg,sizeof(msg));
	}
}


void CPlayer::SetLevelUpStatPoint()
{
	for( int i = 0; i < 4; i++ )
	{
		if(!CheckBit(m_HeroInfo.ExpFlag, i))
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
	}

	m_HeroInfo.ExpFlag = 0x00;
	SetOnBit(&m_HeroInfo.ExpFlag, 4);	
	UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);
	m_dwHpMpBitFlag = 0x00;	

	EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()+1);

	int count = int(( ( (float)m_HeroInfo.ExpPoint / (float)maxexp ) * 100 ) / 20);

	for( int i = 0; i < count; i++ )
	{
		if( !CheckBit(m_HeroInfo.ExpFlag, i) && i < 4 )
		{
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
			SetOnBit(&m_HeroInfo.ExpFlag, i);
			UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);

			SetLife(GetMaxLife());
			SetShield(GetMaxShield());
			SetNaeRyuk(GetMaxNaeRyuk());
		}
	}
}

void CPlayer::SetNormalStatPoint()
{
	EXPTYPE maxexp = GAMERESRCMNGR->GetMaxExpPoint( GetLevel() );

	int count = int((((float)m_HeroInfo.ExpPoint / (float)maxexp)*100) / 20);

	for( int i = 0; i < count; i++ )
	{
        if( !CheckBit(m_HeroInfo.ExpFlag, i) && i < 4)
		{
			SetPlayerLevelUpPoint(GetPlayerLevelUpPoint()+1);
			SetOnBit(&m_HeroInfo.ExpFlag, i);
			UCharacterExpFlag(GetID(), m_HeroInfo.ExpFlag);
			CharacterHeroInfoUpdate(this);
		}
	}

	for( int i = 0; i < 4; i++ )
	{
		if(count == i+1 && !CheckBit(m_dwHpMpBitFlag, i))
		{
			SetLife(GetMaxLife());
			SetShield(GetMaxShield());
			SetNaeRyuk(GetMaxNaeRyuk());
			m_dwHpMpBitFlag = 0x00;
			for( int j = 0; j < count; j++ )
                SetOnBit(&m_dwHpMpBitFlag, i);
		}
		if(count == i && CheckBit(m_dwHpMpBitFlag, i))
		{
			m_dwHpMpBitFlag = 0x00;
			for( int j = 0; j < count; j++ )
				SetOnBit(&m_dwHpMpBitFlag, i);
		}
	}
}

void CPlayer::AddPlayerExpPoint(EXPTYPE Exp)
{
	if( Exp == 0 ) return;
	if(GetLevel()>=99)        //[99级不再增加经验][By:十里坡剑神][QQ:112582793][2017/12/5]
	{
		Exp = 0;
		return;
	}
	EXPTYPE NewExp = GetPlayerExpPoint() + Exp;
	if(NewExp>GAMERESRCMNGR->GetMaxExpPoint(GetLevel()))
	{
		if((NewExp-GAMERESRCMNGR->GetMaxExpPoint(GetLevel()))>GAMERESRCMNGR->GetMaxExpPoint(GetLevel()+1))
			NewExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
		SetPlayerExpPoint(NewExp,TRUE);
		CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
		return;
	}
	SetPlayerExpPoint(NewExp);
	SetPlayerExpPoint(NewExp,TRUE);
	CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
}

void CPlayer::ReduceExpPoint(EXPTYPE minusExp, BYTE bLogType)
{
	LEVELTYPE minuslevel = 0;
	EXPTYPE CurExp = GetPlayerExpPoint();

	if(GetLevel() <= 1 && CurExp < minusExp)	
		minusExp = CurExp;

	InsertLogExp( bLogType, GetID(), GetLevel(), (DWORD)minusExp, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
	
	while(1)
	{
		if(CurExp < minusExp)
		{
			minusExp -= CurExp;

			++minuslevel;
			CurExp = GAMERESRCMNGR->GetMaxExpPoint(GetLevel()-minuslevel) - 1;
			ASSERT(minuslevel<2);	
			if(minuslevel > 3)		
				break;
		}
		else
		{
			CurExp -= minusExp;
			break;
		}
	}

	if(minuslevel > 0)
	{
		SetLevel(m_HeroCharacterInfo.Level-minuslevel);
		
		m_dwHpMpBitFlag = 0x0F;	

		InsertLogCharacter( GetID(), m_HeroCharacterInfo.Level, &m_HeroInfo );
	}
	SetPlayerExpPoint(CurExp,TRUE);	

	CharacterLvlExpUpdate(GetID(), GetLevel(), GetPlayerExpPoint());
}

void CPlayer::SetPlayerAbilityExpPoint(DWORD point,BOOL bUse) 
{
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
		point += 10;

	DWORD FromAbilityExp = m_AbilityGroup.GetAbilityExp();
	DWORD ChangeAbilityExp = point - FromAbilityExp;
	if((point > 1000000000 || ChangeAbilityExp > 1000000000) && GetUserLevel() > eUSERLEVEL_GM )
	{
		AbilityLevelupLog(this,
			0,99,
			FromAbilityExp,point);
		
		return;
	}
	
	m_AbilityGroup.SetAbilityExp(point);

	MSG_ABILLITY_EXPPOINT msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_ABILITYEXPPOINT_ACK;
	msg.dwObjectID = GetID();
	msg.ExpPoint = GetPlayerAbilityExpPoint();
	if(bUse == 0)
		msg.ExpKind = MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ACQUIRE;
	else
		msg.ExpKind = MSG_ABILLITY_EXPPOINT::EXPPOINTKIND_ABILITYUSE;

	SendMsg(&msg,sizeof(msg));
}
void CPlayer::AddAbilityExp(DWORD AbilityExp)
{
	DWORD NewExp = GetPlayerAbilityExpPoint() + AbilityExp;

	SetPlayerAbilityExpPoint(NewExp);
}

DWORD CPlayer::AddExpPointToMugong(WORD MugongIdx,DWORD exp,BOOL bSuryunComplete)
{
	MUGONGBASE* pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return 0;

	if(pMugong->dwDBIdx == 0)
		return 0;
	DWORD point =0;
	pMugong->ExpPoint += exp;

	pMugong->bWear = TRUE;
	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugong->wIconIdx);

	if(pSkillInfo == NULL)
		return 0;
	
	point = pSkillInfo->GetNeedExp(pMugong->Sung);
	if(point == DWORD(-1))
		return 0;

	if(SURYUNMGR->NeedSuryun(pMugong->wIconIdx,pMugong->Sung,pMugong->ExpPoint))
		pMugong->ExpPoint = point - 1;

	if(bSuryunComplete)
	{
		pMugong->ExpPoint = 0;
		BYTE upSung = pMugong->Sung+1;
		if( pSkillInfo->GetNeedExp(pMugong->Sung+1) == DWORD(-1) )
		{
			upSung = 12;
		}
		SetPlayerMugongSung(pMugong->Position, upSung ,eLog_MugongLevelup);
	}
	else if(pMugong->ExpPoint >= point)
	{
		pMugong->ExpPoint = pMugong->ExpPoint - point;
		SetPlayerMugongSung(pMugong->Position, pMugong->Sung+1,eLog_MugongLevelup);
	}
	
	MSG_MUGONGEXP msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_EXPPOINT_NOTIFY;
	msg.absPos = pMugong->Position;
	msg.ExpPoint = pMugong->ExpPoint;
	SendMsg(&msg,sizeof(msg));
	
	return pMugong->ExpPoint;
}

void CPlayer::SetPlayerMugongSung(POSTYPE abspos, BYTE sung, WORD Reason)
{
	MUGONGBASE* pMugong = GetMugongBase(abspos);
	ASSERT(pMugong);
	if(pMugong == NULL)
		return;
	pMugong->Sung = sung;

	MSG_WORD3 msg;
	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_SUNG_NOTIFY;
	msg.dwObjectID = GetID();
	msg.wData1 = abspos;
	msg.wData2 = sung;
	msg.wData3 = Reason;
	SendMsg(&msg,sizeof(msg));

	MSGBASE msg1;
	SetProtocol( &msg1, MP_MUGONG, MP_MUGONG_SUNG_LEVELUP );
	msg1.dwObjectID = GetID();
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg1, sizeof(msg1) );

	MugongUpdateToDB(pMugong, "MUGONGSUNG");

	InsertLogMugong( Reason, GetID(), pMugong->wIconIdx, pMugong->dwDBIdx, sung, pMugong->ExpPoint );
}

void CPlayer::MugongLevelDown(WORD MugongIdx, WORD Reason)
{
	MUGONGBASE* pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
		return;

	if(pMugong->dwDBIdx == 0 || pMugong->Sung == 0)
		return;
	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pMugong->wIconIdx);
	if(pSkillInfo == NULL)
		return;
	
	SKILL_CHANGE_INFO* pSkillChangeInfo = SKILLMGR->GetSkillChangeInfobyDeadlyMugongIdx(pMugong->wIconIdx);
	if(pSkillChangeInfo) 
	{
		MUGONGMNGR->ChangeMugong(this, MugongIdx, pMugong->Position, pSkillChangeInfo->wMugongIdx, eLog_MugongDestroyByBadFameJulChoDown);
	}
	else
	{
		float fRate = pMugong->ExpPoint / (float)pSkillInfo->GetNeedExp(pMugong->Sung);
		
		BYTE newSung = pMugong->Sung - 1;
		DWORD newExp = newSung ? (DWORD)(pSkillInfo->GetNeedExp(newSung)*fRate) : 0;
		pMugong->ExpPoint = newExp;
		SetPlayerMugongSung(pMugong->Position,newSung,Reason);
	}
	pMugong->bWear = TRUE;
}

BYTE CPlayer::GetLifePercent()
{
	BYTE lifepercent = (BYTE)((GetLife() / (float)GetMaxLife()) * 100);
	return lifepercent;
}
BYTE CPlayer::GetShieldPercent()
{
	BYTE Shieldpercent = (BYTE)((GetShield() / (float)GetMaxShield()) * 100);
	return Shieldpercent;
}

BYTE CPlayer::GetNaeRyukPercent()
{
	BYTE naeryukpercent = (BYTE)((m_HeroInfo.wNaeRyuk / (float)GetMaxNaeRyuk()) * 100);
	return naeryukpercent;
}
void CPlayer::OnStartObjectState(BYTE State,DWORD dwParam)
{
	switch(State)
	{
	case eObjectState_Ungijosik:
		m_LifeRecoverTime.lastCheckTime = gCurTime;
		m_NaeRyukRecoverTime.lastCheckTime = gCurTime;
		m_ShieldRecoverTime.lastCheckTime = gCurTime;
		break;
	}
}

void CPlayer::OnEndObjectState(BYTE State)
{
	switch(State)
	{

	case eObjectState_Die:
		GetBattle()->OnTeamMemberRevive(GetBattleTeam(),this);
		m_bNeedRevive = TRUE;
		break;
	}

}

WORD CPlayer::GetWeaponEquipType()
{
	WORD WeaponItemIdx = GetWearedWeapon();
	if(WeaponItemIdx == 0)
		return WP_GWUN;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(WeaponItemIdx);

	if( pItemInfo )
		return pItemInfo->WeaponType;
	return 0;
}

WORD CPlayer::GetTitanWeaponEquipType()
{
	if( !InTitan() )
		return 0;

	CItemSlot * pSlot =GetSlot(eItemTable_Titan);
	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs(TP_TITANWEAR_START + eTitanWearedItem_Weapon);
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if(pInfo && pInfo->WeaponType != 0)
		return pInfo->WeaponType;
	else if( pItemBase->wIconIdx == 0 )
		return WP_GWUN;
	else
		return 0;
}

void CPlayer::ReviveAfterVimu( BOOL bSendMsg )
{
	ClearMurderIdx();
	m_bNeedRevive = TRUE;

	if( bSendMsg )
	{
		MOVE_POS msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
		msg.dwObjectID = GetID();
		msg.dwMoverID = GetID();
	
		msg.cpos.Compress(CCharMove::GetPosition(this));
		
		PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	SetLife( GetMaxLife() * 0.3 );	
}


void CPlayer::RevivePresentSpot()
{	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; 
		SendMsg( &msg, sizeof(msg) );
		return;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2; 
		SendMsg( &msg, sizeof(msg) );
		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4;
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	msg.cpos.Compress(CCharMove::GetPosition(this));
		
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	if( IsActionPanelty() == FALSE )
	{
		SetActionPanelty();
		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Present );
	}
	else
	{

	DWORD dwCheckBit = eSiegeWarMap | eTournament | eEventMap | eQuestRoom | eSurvivalMap;
	if( !(CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel()) )	
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				ShopItemUseParamUpdateToDB( 0,  GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
			}
			m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
		}
		else
		{
ProtectAll_UseFail:
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney((MONEYTYPE)(CurMoney*0.06), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
			}
			
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;

				float fpa = 3.0f;
				LEV_PENALTY* pTemp = GAMERESRCMNGR->GetLevPenalty(GetLevel());
				if( pTemp )
					fpa = pTemp->fNow;

				float fRate = fpa * 0.01f;

				if( !m_bDieForGFW )
					minusExp = (EXPTYPE)(pointForLevel * fRate);

				ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);

				m_PetManager.ReducePetFriendshipWithMastersRevive();
			}
		}
	}

	}

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	CalcState();	//卡血测试
	DWORD MaxLife = GetMaxLife();
	DWORD MaxNaeryuk = GetMaxNaeRyuk();
	DWORD MaxShield = GetMaxShield();
	SetLife((DWORD)(MaxLife*0.3));
	SetNaeRyuk((DWORD)(MaxLife*0.3));
	SetShield((DWORD)(MaxShield*0.3));
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	ClearMurderIdx();

	m_bDieForBattleChannel = FALSE;
	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
	m_bDieInUnionWarMap = FALSE;
}

void CPlayer::ReviveLogIn()
{	
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; 
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2;
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4; 
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	m_bNeedRevive = TRUE;
	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	
	WORD ServerMapNum = (WORD)GAMERESRCMNGR->GetLoadMapNum();
	
	VECTOR3 pos,RandPos;
	if( BATTLESYSTEM->GetBattle( this )->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		DWORD LoginIndex = 0;
		if( GetBattleTeam() == eBattleTeam1 )			LoginIndex = 1103;
		else											LoginIndex = 1102;
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,(WORD)LoginIndex);
	}
	else
		GAMERESRCMNGR->GetRegenPoint(pos,RandPos,m_HeroCharacterInfo.LoginPoint_Index,m_HeroCharacterInfo.MapChangePoint_Index);
	
	pos = RandPos;
	
	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition(this,&pos);
	CPet* pPet = m_PetManager.GetCurSummonPet();
	if(pPet)
	{
		CCharMove::SetPosition(pPet,&pos);
		MOVE_POS	Msg;
		Msg.Category = MP_MOVE;
		Msg.Protocol = MP_MOVE_PET_WARP_ACK;
		Msg.dwObjectID = GetID();
		Msg.dwMoverID = pPet->GetID();
		Msg.cpos.Compress(&pos);
		PACKEDDATA_OBJ->QuickSend(this,&Msg,sizeof(Msg));
	}

	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));
		
	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);

//#ifdef _CHINA_LOCAL_
//	if( BATTLESYSTEM->GetBattle( this )->GetBattleKind() != eBATTLE_KIND_SIEGEWAR )
//		ReviveLogInPenelty();
//#elif defined _KOR_LOCAL_
	if( IsActionPanelty() == FALSE )
	{
		SetActionPanelty();
		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Login );
	}
	else
		ReviveLogInPenelty();
//#else
//	ReviveLogInPenelty();
//#endif
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Immortal,0);
//#ifdef _HK_LOCAL_
//	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 60000 );
//#else
	OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal, 30000 );
//#endif // _HK_LOCAL_
	
	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;


	if(m_HeroInfo.PartyID)
	{	
		CParty* pParty = PARTYMGR->GetParty(m_HeroInfo.PartyID);
		if(!pParty)		return;

		SEND_PARTYICON_REVIVE msg;		
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_REVIVEPOS;
		msg.dwMoverID = m_BaseObjectInfo.dwObjectID;
		msg.Pos.posX = (WORD)pos.x;
		msg.Pos.posZ = (WORD)pos.z;
		pParty->SendMsgToAll(&msg, sizeof(msg));
	}
	CalcState();	//卡血测试
	ClearMurderIdx();
	DWORD MaxLife = GetMaxLife();
	DWORD MaxNaeryuk = GetMaxNaeRyuk();
	DWORD MaxShield = GetMaxShield();
	SetLife((DWORD)(MaxLife*0.3));
	SetNaeRyuk((DWORD)(MaxLife*0.3));
	SetShield((DWORD)(MaxShield*0.3));
	m_bDieInSpecialMap = FALSE;
	m_bDieInUnionWarMap = FALSE;
}

void CPlayer::ReviveLogInPenelty()
{
	DWORD dwCheckBit = eSiegeWarMap | eTournament | eEventMap | eQuestRoom | eSurvivalMap;
	if( !(CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel()) )	
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{		
		BOOL bExchanged = FALSE;
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{//[死亡护魂判断][By:十里坡剑神][QQ:112582793][2018/3/31]
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
			}
			m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
		}
		else
		{
ProtectAll_UseFail:
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney( (MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST , eItemTable_Inventory, eMoneyLog_LoseDie, 0 );
			}

			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );
			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;				

				float fpa = 2.0f;
				LEV_PENALTY* pTemp = GAMERESRCMNGR->GetLevPenalty(GetLevel());
				if( pTemp )
					fpa = pTemp->fSave;

				float fRate = fpa * 0.01f;

				if( GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
					fRate = 0.01f;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * fRate);				

				ReduceExpPoint(minusExp, eExpLog_LosebyReviveLogIn);

				m_PetManager.ReducePetFriendshipWithMastersRevive();
			}
		}

	}
	CalcState();	//卡血测试
	DWORD CurLife = GetMaxLife();
	DWORD CurNaeryuk = GetMaxNaeRyuk();
	DWORD CurShield = GetMaxShield();
	SetLife((DWORD)(CurLife*0.3));
	SetNaeRyuk(CurNaeryuk*0.3);
	SetShield((DWORD)(CurShield*0.3));
	ClearMurderIdx();

	m_bDieForBattleChannel = FALSE;
	m_bDieForGFW = FALSE;
}

void CPlayer::DoDie(CObject* pAttacker)
{
	OBJECTSTATEMGR_OBJ->StartObjectState(this,eObjectState_Die,pAttacker->GetID());
	for(WORD State = eSingleSpecialState_Hide; State < eSingleSpecialState_Max; State++)
	{
		if(!GetSingleSpecialState(State))
			continue;
	
		SetSingleSpecialStateUsedTime(State, gCurTime);
		SetSingleSpecialState(State, false);
		SendSingleSpeicalStateAck(State, false);
	}
	
	CPet* pPet = GetCurPet();
	if(pPet)
	{
		pPet->GetRandMotionNSpeech(ePM_MASTER_DIE, ePMF_ALWAYS);
	}
	
	m_PetManager.SetReduceAmountPetFriendship(pAttacker);

	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		CBattle* pBattle = pAttacker->GetBattle();

		

		WORD MapNum = g_pServerSystem->GetMapNum();
		DWORD dwCheckBit = eBossMap | eEventMap | eTournament | eSurvivalMap;
		if( GetGuildIdx() > 0 )
		{
			if( IsPKMode() == TRUE
				|| (GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pAttacker->GetBattleID() == GetBattleID())
				|| MapNum == SIEGEWARMGR->GetSiegeMapNum()
				|| g_pServerSystem->GetMap()->IsMapKind(dwCheckBit)
				|| PARTYWARMGR->IsEnemy(this, pAttackPlayer) || GUILDFIELDWARMGR->IsEnemy(this, pAttackPlayer)||
				UNIONWARMGR->IsUnionWarMap())
			{
				SetSOSState(0);
			}
			else
				SetSOSState(1);
		}

		if( CHANNELSYSTEM->IsBattleChannel(GetChannelID()) )
		{
			if( IsPKMode() == TRUE
				|| (GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET && pAttacker->GetBattleID() == GetBattleID())
				|| MapNum == SIEGEWARMGR->GetSiegeMapNum()
				|| g_pServerSystem->GetMap()->IsMapKind(dwCheckBit)
				|| PARTYWARMGR->IsEnemy(this, pAttackPlayer) || GUILDFIELDWARMGR->IsEnemy(this, pAttackPlayer) )
			{
				SetDieForBattleChannel(FALSE);
			}
			else
				SetDieForBattleChannel(TRUE);
		}

		if(pBattle->GetBattleKind() == eBATTLE_KIND_NONE || pAttacker->GetBattleID() != GetBattleID() )
		{
			if( LOOTINGMGR->IsLootingSituation( this, pAttacker ) )
			{
				if( PARTYWARMGR->IsinSamePartyWar( this, (CPlayer*)pAttacker ) == FALSE )
				{
					if(PARTYWARMGR->IsMemberInPartyWar( this ))
					{
						SetPenaltyByDie(TRUE);
					}

					LOOTINGMGR->CreateLootingRoom( this, (CPlayer*)pAttacker );
				}
			}
			if( IsPKMode() == FALSE && pAttackPlayer->IsPKMode() == TRUE && 
				WANTEDMGR->IsOwner((CPlayer*)pAttacker, GetWantedIdx()) == FALSE &&
				PARTYWARMGR->IsinSamePartyWar( this, (CPlayer*)pAttacker ) == FALSE )	
			{
				SetMurderIDX(pAttacker->GetID());
				SetMurderKind(pAttacker->GetObjectKind());
				WANTEDMGR->SendRegistWanted(this, (CPlayer*)pAttacker);
				JouranlInsertWanted(GetID(), eJournal_Wanted, eJournal_MurderedbyChr, ((CPlayer*)pAttacker)->GetObjectName());

				SetPenaltyByDie( TRUE );
				if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				{
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
				}
				WANTEDMGR->Complete((CPlayer*)pAttacker, this);
			}
			else
			{				
				SetReadyToRevive(TRUE);
				SetPenaltyByDie( TRUE );

				if( PARTYWARMGR->PlayerDie( this, (CPlayer*)pAttacker ) )
				{
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
				}
				else if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
				{
					SVVMODEMGR->RemoveAliveUser(this);
					SetReadyToRevive( FALSE );
					SetPenaltyByDie( FALSE );
					m_bNeedRevive = FALSE;
				}
				else
				{
					GUILDFIELDWARMGR->JudgeGuildFieldWar( this, (CPlayer*)pAttacker );

					WANTEDMGR->Complete((CPlayer*)pAttacker, this);
				}
			}
			if (UNIONWARMGR->IsUnionWarProcess())
				UNIONWARMGR->DiePlayer(this, pAttackPlayer);//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-29][1:52:23]
			//[杀人提示][By:十里坡剑神][QQ:112582793][2017/12/6]
			pAttackPlayer->SetKillTimes(pAttackPlayer->GetKillTimes() + 1);
			DWORD dwKillTimes = 0;
			dwKillTimes = pAttackPlayer->GetKillTimes();	
			if (dwKillTimes >= 300)
			{
				MSG_TIP msg;
				msg.Category = MP_CLIENT;
				msg.Protocol = MP_CLIENT_MSG_AGENT;
				msg.Flag = eKillTip;
				msg.KillTime = dwKillTimes;
				SafeStrCpy(msg.Name1, pAttackPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.Name2, GetObjectName(), MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.MapName, g_pServerSystem->GetMap()->GetMapName(), 128);
				msg.ChannelNum = pAttackPlayer->GetBattleID();
				pAttackPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
			}
			else if (dwKillTimes % 20 == 0)
			{
				MSG_TIP msg;
				msg.Category = MP_CLIENT;
				msg.Protocol = MP_CLIENT_MSG_AGENT;
				msg.Flag = eKillTip;
				msg.KillTime = dwKillTimes;
				SafeStrCpy(msg.Name1, pAttackPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.Name2, GetObjectName(), MAX_NAME_LENGTH + 1);
				SafeStrCpy(msg.MapName, g_pServerSystem->GetMap()->GetMapName(), 128);
				msg.ChannelNum = pAttackPlayer->GetBattleID();
				pAttackPlayer->SendMsgToAllAgent(&msg, sizeof(msg));
			}

			
			if (pAttackPlayer->IsPKMode())
			{
				if (pAttackPlayer->GetLevel() >= GetLevel())
				{
					if (pAttackPlayer->GetLevel() - GetLevel() <= 6)
						pAttackPlayer->AddKillTimes();//[击杀统计][By:十里坡剑神][QQ:112582793][2018/4/22]
				}
				else
					pAttackPlayer->AddKillTimes();//[击杀统计][By:十里坡剑神][QQ:112582793][2018/4/22]
			}
		}
	}
	else if(pAttacker->GetObjectKind() & eObjectKind_Monster )
	{	
		SetMurderKind(((CMonster*)pAttacker)->GetMonsterKind());
			
#ifdef _KOR_LOCAL_
		if( (gCurTime - m_LastActionTime) > GAMERESRCMNGR->GetPaneltyTime() )
		{
			m_bActionPanelty = FALSE;
			m_LastActionTime = gCurTime;
			SendNoActionPaneltyMsg();
		}
		else
#endif
			SetPenaltyByDie(TRUE);

		SetReadyToRevive(TRUE);

		QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 ) );
	}

	ExitCancel();
	EXCHANGEMGR->CancelExchange( this );
	STREETSTALLMGR->UserLogOut( this );
	LOOTINGMGR->LootingCancel( this );
	PKMGR->DiePanelty( this, pAttacker );

	CMonster * pObject = NULL;
	m_FollowMonsterList.SetPositionHead();
	while(pObject = (CMonster *)m_FollowMonsterList.GetData())
	{
		pObject->SetTObject(NULL);
	}
	m_FollowMonsterList.RemoveAll();

	InitMussangPoint();
	SendMussangInfo();

	if(IsMussangMode())
	{
		SetMussangMode(FALSE);
		STATSMGR->CalcItemStats(this);
		SendMussangEnd();
	}
	
	if( CCharMove::IsMoving(this) )
	{
		VECTOR3 pos;
		GetPosition( &pos );
		CCharMove::EndMove( this, gCurTime, &pos );
	}

	QUESTMAPMGR->DiePlayer( this );

	SKILLOPTION* pSkillOption = NULL;
	CSkillObject* pSkill = NULL;
	m_SkillList.SetPositionHead();
	while( pSkill = m_SkillList.GetData() )	
	{
		SKILLOBJECT_INFO* pSkillObjInfo = pSkill->GetSkillObjectInfo();
		if(pSkillObjInfo != NULL)
		{
			pSkillOption = SKILLMGR->GetSkillOption(pSkillObjInfo->Option);
			if(pSkillOption != NULL)
			{
				RemoveSkillStatsOption(pSkillOption);

				CCharacterCalcManager::CalcMaxLife(this);
				CCharacterCalcManager::CalcMaxShield(this);
				CCharacterCalcManager::CalcMaxNaeRyuk(this);
			}
		}
		pSkillObjInfo = NULL;
		pSkillOption = NULL;

		pSkill->DoDie(NULL);
	}
	m_SkillList.RemoveAll();

	if(this->GetTitanManager()->IsTitanRecall())
	{
		this->GetTitanManager()->InitTitanRecall();

		MSGBASE msg;
		msg.Category = MP_TITAN;
		msg.Protocol = MP_TITAN_RECALL_CANCEL_ACK;
		msg.dwObjectID = this->GetID();

		this->SendMsg(&msg, sizeof(msg));
	}	
	FORTWARMGR->DiePlayer( this );
}

float CPlayer::DoGetMoveSpeed()
{
	float speed = 0;
	if( InTitan() )
	{	
		if(m_MoveInfo.KyungGongIdx)
		{	
			TITANINFO_GRADE* pGradeInfo = GAMERESRCMNGR->GetTitanGradeInfo(m_TitanManager.GetCurRidingTitan()->GetTitanKind(), 
				m_TitanManager.GetCurRidingTitan()->GetTitanGrade());
			if( !pGradeInfo )
				return TITAN_WALKSPEED;

			WORD klev = 0;	
			if( m_MoveInfo.KyungGongIdx == 2602 )
				klev = 1;	
			else if( m_MoveInfo.KyungGongIdx == 2604 )
				klev = 2;	
			speed = pGradeInfo->KyungGongSpeed[klev];

			speed += GetAvatarOption()->KyunggongSpeed;
			speed += GetShopItemStats()->KyungGongSpeed;

			return speed;
		}
		else
		{
			if(m_MoveInfo.MoveMode == eMoveMode_Run)
				return m_TitanManager.GetTitanStats()->MoveSpeed;
			else
				return TITAN_WALKSPEED;
		}
	}
	else
	{

		if(m_MoveInfo.KyungGongIdx)
		{
			CKyungGongInfo* pKGInfo = KYUNGGONGMGR->GetKyungGongInfo(m_MoveInfo.KyungGongIdx);
			ASSERT(pKGInfo);
			if(pKGInfo == NULL)
				return 0;

			speed = pKGInfo->GetSpeed()+GetAbilityStats()->Kyunggong;

			speed += m_AvatarOption.KyunggongSpeed;
			speed += m_ShopItemOption.KyungGongSpeed;			
			return speed;
		}
		else
		{
			if(m_MoveInfo.MoveMode == eMoveMode_Run)
			{
				return RUNSPEED;
			}				
			else
			{
				return WALKSPEED;
			}
		}
	}

	return speed;
}

MUGONGBASE * CPlayer::GetMugongBaseByMugongIdx(WORD MugongIdx)
{
	MUGONGBASE* pMugong;
	for(int n=0;n<SLOT_MUGONGTOTAL_NUM;++n)
	{
        pMugong = &m_HeroMugongInfo.mugong[n];
		
		if(pMugong->dwDBIdx == 0)
			continue;
		if(pMugong->wIconIdx == MugongIdx)
			return pMugong;
	}
	return NULL;
}

int CPlayer::GetMugongLevel(WORD MugongIdx)
{
	if(MugongIdx < 100 || ( 10000 <= MugongIdx && MugongIdx < 10000 + 100 ))
		return 1;
	
	MUGONGBASE * pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
	{
		return -1;
	}

	return pMugong->Sung;
}

int CPlayer::GetMugongNum()
{
	int count = 0;
	MUGONGBASE* pMugong;
	for(int n=0;n<SLOT_MUGONGTOTAL_NUM;++n)
	{
		pMugong = &m_HeroMugongInfo.mugong[n];
		if(pMugong->dwDBIdx && pMugong->Sung > 0)
			++count;		
	}
	return count;
}

WORD CPlayer::GetSkillOptionIndex(WORD MugongIdx)
{
	MUGONGBASE * pMugong = GetMugongBaseByMugongIdx(MugongIdx);
	if(pMugong == NULL)
	{
		return 0;
	}

	return pMugong->OptionIndex;
}

void CPlayer::SetInitedGrid()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GRIDINIT;
	SendMsg(&msg,sizeof(msg));

	CGridUnit::SetInitedGrid();
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(this->GetBattleID());
	if(pBattle && pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, this);

	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_GameEnter, 0, 1 ) );
	QUESTMGR->AddQuestEvent( this, &CQuestEvent( eQuestEvent_MapChange, 0, g_pServerSystem->GetMapNum() ) );
}
DWORD CPlayer::DoGetCritical()
{
	DWORD cri = 0;
	if( InTitan() )
	{
		titan_calc_stats* pTitanStat = m_TitanManager.GetTitanStats();
		cri = pTitanStat->Critical;
	}
	else
	{
		cri = GetCharStats()->Critical + GetItemStats()->Critical + GetShopItemStats()->Critical + GetAvatarOption()->Critical;
	}

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1] )//[组队一击概率加成计算][By:十里坡剑神][QQ:112582793][2018/4/22]
				cri = (DWORD)(cri*gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1]);
		}
	}

	return cri;
}
DWORD CPlayer::DoGetDecisive()
{	
	DWORD cri = GetCharStats()->Decisive + GetItemStats()->Critical + GetShopItemStats()->Decisive + GetAvatarOption()->Decisive;

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1] )//[组队一击概率加成计算][By:十里坡剑神][QQ:112582793][2018/4/22]
				cri = (DWORD)(cri*gPartyEvent[ePartyEvent_CriticalRate].Rate[count-1]);
		}
	}

	return cri;
}
DWORD CPlayer::DoGetPhyAttackPowerMin()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();	
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); 
	AVATARITEMOPTION* pAvatarOption = GetAvatarOption();

	WORD WeaponKind = GetWeaponEquipType();

	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + pAvatarOption->Minchub + pSetItem_Stats->wMinChub);

//#ifdef _JAPAN_LOCAL_
//
//		WORD RareWeapon = pItem_stats->RangeAttackPowerMin;
//
//		WORD WeaponAttack = (WORD)(RareWeapon
//						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMin )
//						  + pAvatarOption->Attack);
//
//		return CalcRangeAttackPower( MinChub, WeaponAttack )
//			 + GetLevel() * 4
//			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
//#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMin+pAvatarOption->Attack+pSetItem_Stats->wRangeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMin));
//#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + pAvatarOption->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;	

//#ifdef _JAPAN_LOCAL_
//
//		WORD RareWeapon = pItem_stats->MeleeAttackPowerMin + DefaultPower;
//
//		WORD WeaponAttack = (WORD)(RareWeapon
//						  * ( 1.f + pItem_stats->MeleeAttackPowerAddRateMin )
//						  + pAvatarOption->Attack);
//
//		return CalcMeleeAttackPower( GenGol, WeaponAttack )
//			 + GetLevel() * 4
//			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
//#else
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMin+DefaultPower+pAvatarOption->Attack+pSetItem_Stats->wMeleeAttackMin)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMin));
//#endif
	}
}

DWORD CPlayer::DoGetPhyAttackPowerMax()
{
	const player_calc_stats * pChar_stats = GetCharStats();
	const player_calc_stats * pItem_stats = GetItemStats();
	const SET_ITEM_OPTION* pSetItem_Stats = GetSetItemStats(); 
	AVATARITEMOPTION* pAvatarOption = GetAvatarOption();

	WORD WeaponKind = GetWeaponEquipType();

	if( WeaponKind == WP_EVENT_HAMMER )
		return 0;

	if(WeaponKind == WP_GUNG || WeaponKind == WP_AMGI)
	{
		WORD MinChub = (WORD)(pChar_stats->MinChub + GetAbilityStats()->StatMin + pItem_stats->MinChub + GetShopItemStats()->Minchub + GetAvatarOption()->Minchub + pSetItem_Stats->wMinChub);

//#ifdef _JAPAN_LOCAL_
//		WORD RareWeapon = pItem_stats->RangeAttackPowerMax;
//		WORD WeaponAttack = (WORD)(RareWeapon
//						  * ( 1.f + pItem_stats->RangeAttackPowerAddRateMax )
//						  + pAvatarOption->Attack);
//
//		return CalcRangeAttackPower( MinChub, WeaponAttack )
//			 + GetLevel() * 4
//			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
//#else
		return (DWORD)((CalcRangeAttackPower(MinChub,pItem_stats->RangeAttackPowerMax+pAvatarOption->Attack+pSetItem_Stats->wRangeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->RangeAttackPowerAddRateMax));
//#endif
	}
	else
	{
		WORD GenGol = (WORD)(pChar_stats->GenGol + GetAbilityStats()->StatGen + pItem_stats->GenGol + GetShopItemStats()->Gengol + GetAvatarOption()->Gengol + pSetItem_Stats->wGenGol);

		WORD WeaponItemIdx = GetWearedItemIdx(eWearedItem_Weapon);
		WORD DefaultPower = 0;
		if( WeaponItemIdx == 0 )
			DefaultPower = 5;		
		
//#ifdef _JAPAN_LOCAL_
//
//		WORD RareWeapon = pItem_stats->MeleeAttackPowerMax + DefaultPower;
//
//		WORD WeaponAttack = (WORD)(RareWeapon
//						  * ( 1.f + pItem_stats->MeleeAttackPowerAddRateMax )
//						  + pAvatarOption->Attack);
//
//		return CalcMeleeAttackPower( GenGol, WeaponAttack )
//
//			 + GetLevel() * 4
//			 + GetAbilityStats()->GetPhyAttackUp( WeaponKind );
//#else
		return (DWORD)((CalcMeleeAttackPower(GenGol,pItem_stats->MeleeAttackPowerMax+DefaultPower+pAvatarOption->Attack+pSetItem_Stats->wMeleeAttackMax)
			+ GetAbilityStats()->GetPhyAttackUp(WeaponKind))
			* (1+GetItemStats()->MeleeAttackPowerAddRateMax));
//#endif
	}
}

void CPlayer::DoDamage(CObject* pAttacter,RESULTINFO* pDamageInfo,DWORD beforeLife)
{
	if(IsHide())
	{
		SetSingleSpecialStateUsedTime(eSingleSpecialState_Hide, gCurTime);
		SetSingleSpecialState(eSingleSpecialState_Hide, false);
		SendSingleSpeicalStateAck(eSingleSpecialState_Hide, false, true);
	}
}

void CPlayer::InitBaseObjectInfo(BASEOBJECT_INFO* pBaseInfo)
{
	ASSERT(GetInitState() == PLAYERINITSTATE_ONLY_ADDED);
	memcpy(&m_BaseObjectInfo,pBaseInfo,sizeof(BASEOBJECT_INFO));
}

MONEYTYPE CPlayer::SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag, eITEMTABLE tableIdx, BYTE LogType, DWORD TargetIdx )
{

	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if( !pPurse) return 0;

	MONEYTYPE Real = 0;
	if( bOper == MONEY_SUBTRACTION )
	{
		Real = pPurse->Subtraction( ChangeValue, MsgFlag );
	}
	else
	{
		Real = pPurse->Addition( ChangeValue, MsgFlag );
	}

	if( tableIdx == eItemTable_Inventory || tableIdx == eItemTable_ShopInven )
	{
		if( Real >= 10000 )
			InsertLogWorngMoney( LogType, GetID(), Real, GetMoney(), GetMoney(eItemTable_Pyoguk), TargetIdx );
	}

	return Real;
}

BOOL CPlayer::IsEnoughAdditionMoney(MONEYTYPE money, eITEMTABLE tableIdx )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(tableIdx);
	if(!pPurse) return FALSE;

	return pPurse->IsAdditionEnough( money );
}


MONEYTYPE CPlayer::GetMaxPurseMoney(eITEMTABLE TableIdx)
{
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return FALSE;
	return pPurse->GetMaxMoney();
}

void CPlayer::SetMaxPurseMoney(eITEMTABLE TableIdx, DWORD MaxMoney)
{
	if(TableIdx != eItemTable_Pyoguk)
	{
		ASSERT(0);
		return;
	}
	CPurse* pPurse = m_ItemContainer.GetPurse(TableIdx);
	if( !pPurse) return;
	pPurse->SetMaxMoney(MaxMoney);
}

void CPlayer::AddQuest(CQuestBase* pQuest)
{
	if(m_QuestList.GetData(pQuest->GetQuestIdx()))
	{
		ASSERT(0);
		return;
	}
	m_QuestList.Add( pQuest, pQuest->GetQuestIdx() );
}

void CPlayer::RemoveQuest(DWORD QuestIdx)
{
	m_QuestList.Remove(QuestIdx);
}

BOOL CPlayer::SetQuestState(DWORD QuestIdx, QSTATETYPE value)
{
	CQuestBase* pQuest;
	pQuest = m_QuestList.GetData(QuestIdx);

	if( !pQuest ) 
	{
		return FALSE;
	}

	pQuest->SetValue(value);
	BOOL bEnd = pQuest->IsComplete();

	QuestUpdateToDB( GetID(), QuestIdx, value, bEnd );

	if( bEnd )
	{
		m_QuestList.Remove(QuestIdx);

		MSG_DWORD msg;
		msg.Category = MP_QUEST;
		msg.Protocol = MP_QUEST_REMOVE_NOTIFY;
		msg.dwObjectID = GetID();
		msg.dwData = QuestIdx;
		SendMsg(&msg, sizeof(msg));
	}
	return TRUE;
}

void CPlayer::SetInitState(int initstate,DWORD protocol)
{
	if(initstate == PLAYERINITSTATE_ONLY_ADDED)
	{
		m_initState = PLAYERINITSTATE_ONLY_ADDED;
		InitClearData();
	}
	else
		m_initState |= initstate;

	
	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&       
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		!(m_initState & PLAYERINITSTATE_PYOGUK_INFO) )
	{
		CharacterPyogukInfo(GetUserID(), GetID());
		return;
	}

	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&     
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) && 
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		(m_initState & PLAYERINITSTATE_PYOGUK_INFO) &&
		!(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) )
	{
		UsingShopItemInfo( GetID() );
		SavedMovePointInfo( GetID() );
		return;
	}

	if ((m_initState & PLAYERINITSTATE_MUGONG_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_RARE_OPTION_INFO) &&
		(m_initState & PLAYERINITSTATE_ITEM_OPTION_INFO) &&         
		(m_initState & PLAYERINITSTATE_ABILITY_INFO) &&
		(m_initState & PLAYERINITSTATE_HERO_INFO) &&
		(m_initState & PLAYERINITSTATE_PET_INFO) &&
		(m_initState & PLAYERINITSTATE_TITAN_INFO) &&
		(m_initState & PLAYERINITSTATE_PYOGUK_INFO) &&
		(m_initState & PLAYERINITSTATE_SHOPITEM_USEDINFO) )
	{
		SetPeaceMode( g_pServerSystem->GetMap()->IsVillage() );

		GetPetManager()->CheckSummonningPet();

		CalcState(); 

		for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
		{
			const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(this, part);
			
			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = 0;
			
			if(pTargetItemBase == NULL)		continue;			
			if(pTargetItemBase->dwDBIdx == 0)	continue;

			m_HeroCharacterInfo.WearedItemIdx[part-TP_WEAR_START] = pTargetItemBase->wIconIdx;
		}
		if( g_pServerSystem->GetMap()->IsMapKind(eTournament) && m_ObserverBattleIdx )
			m_HeroCharacterInfo.bVisible = FALSE;
		if( (g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()) && 
			SIEGEWARMGR->IsProcessSiegewar() && m_ObserverBattleIdx )
			m_HeroCharacterInfo.bVisible = FALSE;

		SEND_HERO_TOTALINFO msg;
		memset( &msg, 0, sizeof(msg) );
		GetBaseObjectInfo(&msg.BaseObjectInfo);
		GetCharacterTotalInfo(&msg.ChrTotalInfo);
		GetHeroTotalInfo(&msg.HeroTotalInfo);
		GetMugongTotalInfo(&msg.MugongTotalInfo);
		DWORD getItem_Flag;
		if(protocol == MP_SURYUN_ENTER_SYN)
			getItem_Flag = GETITEM_FLAG_WEAR;
		else
			getItem_Flag = GETITEM_FLAG_INVENTORY | GETITEM_FLAG_WEAR | GETITEM_FLAG_PETWEAR | GETITEM_FLAG_TITANWEAR | GETITEM_FLAG_TITANSHOP;	// magi82 - Titan(070227)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
		GetItemtotalInfo(&msg.ItemTotalInfo,getItem_Flag);
		GetSendMoveInfo(&msg.SendMoveInfo,NULL);                                                                                                                                                                                                                                                                                                                                                           
		memcpy(&msg.ShopItemOption, GetShopItemStats(), sizeof(SHOPITEMOPTION));
		ASSERT(!(msg.SendMoveInfo.CurPos.wx == 0 && msg.SendMoveInfo.CurPos.wz == 0));

		ABILITYMGR->InitAbilityExp(GetAbilityGroup(),m_HeroInfo.AbilityExp);
		ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_CHARACTERLEVEL_CHANGED,msg.ChrTotalInfo.Level,GetAbilityGroup());

		ABILITYMGR->GetAbilityTotalInfo(GetAbilityGroup(),&msg.AbilityInfo);		
		
		msg.ChrTotalInfo.CurMapNum = GAMERESRCMNGR->GetLoadMapNum();
		msg.UniqueIDinAgent = GetUniqueIDinAgent();
	
		msg.PetNum = 0;
		WORD PetNum = 0;
		PET_TOTALINFO PetInfo[SLOT_INVENTORY_NUM + SLOT_SHOPINVEN_NUM+40];
		
		PetNum = m_PetManager.GetPetInfoList(PetInfo);

		if(PetNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO)*PetNum, PetInfo );
			msg.PetNum = PetNum;
		}

		WORD OptionNum = 0;
		ITEM_OPTION_INFO OptionInfo[MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM]; 
		m_ItemOptionList.SetPositionHead();
		ITEM_OPTION_INFO * pOInfo = NULL;
		while(pOInfo = m_ItemOptionList.GetData())
		{
			memcpy(&OptionInfo[OptionNum], pOInfo, sizeof(ITEM_OPTION_INFO));
			++OptionNum;		
			if( OptionNum >= MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM ) 
				break;
		}
		if(OptionNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
			msg.OptionNum = OptionNum;
		}
		msg.RareOptionNum = 0;	
		OptionNum = 0;
		ITEM_RARE_OPTION_INFO RareOptionInfo[MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM]; 
		m_ItemRareOptionList.SetPositionHead();
		ITEM_RARE_OPTION_INFO* pRInfo = NULL;
		while(pRInfo = m_ItemRareOptionList.GetData())
		{
			memcpy(&RareOptionInfo[OptionNum], pRInfo, sizeof(ITEM_RARE_OPTION_INFO));
			++OptionNum;
			if( OptionNum >= MAX_ITEM_OPTION_NUM+SLOT_PYOGUK_NUM ) 
				break;
		}
		if(OptionNum)
		{
			msg.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*OptionNum, RareOptionInfo);
			msg.RareOptionNum = OptionNum;
		}

		
		SetGotWarehouseItems( FALSE );

		GetLocalTime(&msg.ServerTime);
		
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_ACK;
		SendMsg((MSGBASE*)&msg, msg.GetMsgLength());

		WEATHERMGR->SendWeather( this );

		VECTOR3 pos;
		msg.SendMoveInfo.CurPos.Decompress(&pos);
		
		if(protocol != MP_SURYUN_ENTER_SYN)
		{
			BOOL bNotifyUserLogin = TRUE;
			if(protocol != MP_SURYUN_LEAVE_SYN && 
				protocol != MP_USERCONN_GAMEIN_SYN)
				bNotifyUserLogin = FALSE;
			PARTYMGR->UserLogIn(this,bNotifyUserLogin);
			
			GUILDMGR->UserLogIn(this);
		}

		WantedInfoByUserLogIn(GetID());

		MSG_WORD2 msgMapDesc;
		msgMapDesc.Category		= MP_USERCONN;
		msgMapDesc.Protocol		= MP_USERCONN_MAPDESC;
		msgMapDesc.wData1		= g_pServerSystem->GetMap()->IsVillage();
		msgMapDesc.wData2		= g_pServerSystem->GetMap()->IsPKAllow();
		SendMsg( &msgMapDesc, sizeof(msgMapDesc) );

		m_initState = PLAYERINITSTATE_ACKMSG_SEND;
	
		SERVERINFO* pserver = g_pServerTable->GetSelfServer();
		RegistLoginMapInfo(GetID(), GetObjectName(), (BYTE)GAMERESRCMNGR->GetLoadMapNum(), (WORD)pserver->wPortForServer);

		m_initState = PLAYERINITSTATE_INITED;
		SetInited();

		MSG_BYTE pymsg;
		pymsg.Category = MP_PYOGUK;
		pymsg.Protocol = MP_PYOGUK_INFO;
		pymsg.bData = GetPyogukNum();
		SendMsg(&pymsg,sizeof(pymsg));
		
		if(protocol == MP_SURYUN_LEAVE_SYN )
			m_ShopItemManager.SendShopItemUseInfo();

		GUILDFIELDWARMGR->AddPlayer( this );		

		GTMGR->AddPlayer( this );

		SIEGEWARMGR->AddPlayer( this );
		SWPROFITMGR->AddPlayer( this );

		FORTWARMGR->AddPlayer( this );
		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-23][15:07:44]
		if (UNIONWARMGR->IsUnionWarMap())
			UNIONWARMGR->AddPlayer(this);

		if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
			SVVMODEMGR->AddSVModeUser(this);

		SendFlagNPCState();

		ApplyTitanWearItem();	
		GetTitanManager()->CheckRidingTitan();	
		if(GAMERESRCMNGR->IsFBMap(GetCurMapNum()))  //副本地图进入消息发送	by:十里坡剑神	QQ:112582793
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_CHAR_FB_IN_ACK;
			msg.dwObjectID = GetID();
			SendMsg(&msg,sizeof(msg));
		}
		//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
		MSG_SINGEDINFO SingedMsg;
		SingedMsg.Category = MP_SINGED;
		SingedMsg.Protocol = MP_SINGED_INFO_SEND_ACK;
		SINGEDMGR->SetSingedMsgInfo(this,SingedMsg);
		SendMsg(&SingedMsg,sizeof(SingedMsg));
		//[加载充值信息][By:十里坡剑神][QQ:112582793][2018/4/22]
		LoadRechargeInfo(GetID());
	}
}

int CPlayer::CanExitStart()
{
	if( IsPKMode() )
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )	
		return eEXITCODE_LOOTING;

	if( GetState() == eObjectState_Exchange )
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest )
		return eEXITCODE_NOT_ALLOW_STATE;

	if( GetState() == eObjectState_Deal )
		return eEXITCODE_NOT_ALLOW_STATE;
	
	return eEXITCODE_OK;
}

void CPlayer::SetExitStart( BOOL bExit )
{
	m_bExit				= bExit;
	m_dwExitStartTime	= gCurTime;
}

int CPlayer::CanExit()
{
	DWORD lCurTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
	if( lCurTime - m_dwExitStartTime < EXIT_COUNT*1000 - 2000 )	
		return eEXITCODE_SPEEDHACK;

	if( IsPKMode() )						
		return eEXITCODE_PKMODE;
	if( LOOTINGMGR->IsLootedPlayer(GetID()) )
		return eEXITCODE_LOOTING;
	
	if( GetAutoAskPlayerIdx() )
		return eEXITCODE_NOT_ALLOW_STATE;
	return eEXITCODE_OK;
}

void CPlayer::ExitCancel()
{
	if( IsExitStart() )
	{
		SetExitStart( FALSE );
		MSG_BYTE msgNack;
		msgNack.Category	= MP_CHAR;
		msgNack.Protocol	= MP_CHAR_EXIT_NACK;
		msgNack.bData		= eEXITCODE_DIEONEXIT;
		SendMsg(&msgNack, sizeof(msgNack));		
	}
}

int CPlayer::PKModeOn()
{
	if( IsPKMode() ) return ePKCODE_ALREADYPKMODEON;
	if( IsVimuing() ) return ePKCODE_VIMUING;

	if( GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( this, eObjectState_Immortal );

	if( GetState() == eObjectState_Die )
		return ePKCODE_STATECONFLICT;
	
	m_HeroCharacterInfo.bPKMode = TRUE;
	m_dwPKModeStartTime			= gCurTime;
	m_dwPKContinueTime			= GetBadFame()/200 + PKMODETIME;
	return ePKCODE_OK;
}
DWORD CPlayer::GetPKOffTime()
{
	return (m_dwPKContinueTime-(gCurTime - m_dwPKModeStartTime))/1000;
}
BOOL CPlayer::PKModeOff()
{
	if (!IsPKMode() || UNIONWARMGR->IsUnionWarStart()) return FALSE;

	if( gCurTime - m_dwPKModeStartTime >= m_dwPKContinueTime )
	{
		m_HeroCharacterInfo.bPKMode = FALSE;
		SetPKModeEndtime();
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );
		return TRUE;
	}

	return FALSE;
}

void CPlayer::PKModeOffForce()
{
	m_HeroCharacterInfo.bPKMode = FALSE;
}

void CPlayer::StateProcess()
{
	switch( GetState() )
	{
	case eObjectState_Die:
		{
			DWORD dwElapsed = gCurTime - m_ObjectState.State_Start_Time;
			if( m_bNeedRevive )
			{
				DWORD reviveDelayTime = PLAYERREVIVE_TIME;
				if( CHANNELSYSTEM->IsBattleChannel(GetChannelID()) && IsDieForBattleChannel() )
					reviveDelayTime = PLAYERREVIVE_TIME * 2;

				if( dwElapsed > PLAYERREVIVE_TIME )
				{
					if( m_bDieInSpecialMap )
					{
						SendPlayerToMap();
						break;
					}
					if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
						break;
					if( IsReadyToRevive() != TRUE )
						break;
					
					if( GetSOSState() > 0 )
						break;
					
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_READY_TO_REVIVE;
					SendMsg( &msg, sizeof(msg) );

					m_bNeedRevive = FALSE;

					if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
					{
						m_bDieInSpecialMap = TRUE;
						m_ObjectState.State_Start_Time = gCurTime;
						break;
					}
					if (UNIONWARMGR->IsUnionWarMap())
					{//[盟战死亡复活超时惩罚][BY:十里坡剑神][QQ:112582793][2019-10-31][20:40:06]
						UNIONWARMGR->DiePlayerToLong(this);
						m_bDieInUnionWarMap = TRUE;
						m_ObjectState.State_Start_Time = gCurTime;
						break;
					}
				}
			}
			//if (m_bDieInUnionWarMap)
			//{
			//	if (UNIONWARMGR->IsUnionWarMap())
			//	{
			//		if (dwElapsed > 60000)
			//		{
			//			m_bDieInUnionWarMap = TRUE;
			//			UNIONWARMGR->DiePlayerToLong(this);
			//			m_ObjectState.State_Start_Time = gCurTime;
			//			break;
			//		}
			//	}
			//}
			if( m_bDieInSpecialMap )
			{
				if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				if( dwElapsed > 60000 )
				{
					m_bDieInSpecialMap = TRUE;
					m_wReturnMapNum = 17;
					m_bNeedRevive = TRUE;
					break;
				}
			}
		}
		break;
	}

	DWORD dwInsertLogTime = INSERTLOG_TIME;

	if( GetInited() )
	if( gCurTime - m_dwProgressTime >= dwInsertLogTime )
	{
		InsertLogExp( eExpLog_Time, GetID(), GetLevel(), 0, GetPlayerExpPoint(), 0, 0, GetPlayerAbilityExpPoint());
		InsertLogMoney(0, GetID(), GetMoney(), 0, GetUserID(), GetMoney(eItemTable_Pyoguk));
		
		if( IsPKMode() ) m_HeroInfo.LastPKModeEndTime = 0;
		UpdateCharacterInfoByTime(GetID(), GetPlayerExpPoint(), GetPlayerAbilityExpPoint(), GetMoney(), m_HeroInfo.Playtime, m_HeroInfo.LastPKModeEndTime );
		
		m_PetManager.UpdateCurPetInfo();

		m_TitanManager.UpDateCurTitanAndEquipItemInfo();

		for(int i=0; i<SLOT_MUGONGTOTAL_NUM;++i)
		{
			if( m_HeroMugongInfo.mugong[i].dwDBIdx != 0 )
			{
				InsertLogMugongExp(GetID(), 0, m_HeroMugongInfo.mugong[i].wIconIdx, m_HeroMugongInfo.mugong[i].dwDBIdx, 
									m_HeroMugongInfo.mugong[i].Sung, m_HeroMugongInfo.mugong[i].ExpPoint, m_HeroMugongInfo.mugong[i].Position );
				UpdateMugongExpByTime(GetID(), m_HeroMugongInfo.mugong[i].dwDBIdx, m_HeroMugongInfo.mugong[i].ExpPoint);				
			}
		}

		m_dwProgressTime = gCurTime;
	}

	m_ContinuePlayTime += gTickTime;
	DWORD dwPT = m_ContinuePlayTime/1000;
	if(dwPT)
	{
		m_HeroInfo.Playtime += dwPT;
		m_ContinuePlayTime -= dwPT*1000;
	}

	AddBadFameReduceTime();	
	m_ShopItemManager.CheckEndTime();

	m_PetManager.PetProcess();
	m_TitanManager.TitanProcess();

	if( m_ReviveTime )
	{
		m_ReviveTime -= gTickTime;

		MSG_WORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;

		CPlayer* pTarget  = (CPlayer*)g_pUserTable->FindUser(m_ReviveData.TargetID);
		if(!pTarget)
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}				
		else if( m_ReviveTime <= 0 )
		{
			msg.wData = eShopItemErr_Revive_TimeOver;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );
			
			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}
		else if( GetChannelID() != pTarget->GetChannelID() )
		{
			msg.wData = eShopItemErr_Revive_Fail;
			SendMsg( &msg, sizeof(msg) );
			pTarget->SendMsg( &msg, sizeof(msg) );

			m_ReviveTime = 0;
			memset( &m_ReviveData, 0, sizeof(m_ReviveData) );
		}
	}
	if( m_fFullMoonEventTime > 0 )
	{
		m_fFullMoonEventTime -= gTickTime;

		if( m_fFullMoonEventTime <= 0.f )
		{
			m_fFullMoonEventTime = 0.f;
			m_HeroCharacterInfo.EventIndex = eFULLMOONEVENT_NONE;

			MSG_BYTE msg;
			SetProtocol( &msg, MP_CHAR, MP_CHAR_FULLMOONEVENT_CHANGE );
			msg.bData = eFULLMOONEVENT_NONE;
			msg.dwObjectID = GetID();
			PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );
		}
	}

	SkinDelayTimeProcess();
}

void CPlayer::SetWearedItemIdx(DWORD WearedPosition,WORD ItemIdx)
{	
	if(WearedPosition == eWearedItem_Weapon)
	{
		GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
	}

	m_HeroCharacterInfo.WearedItemIdx[WearedPosition] = ItemIdx;
}

void CPlayer::AddWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.AddTail((void*)WantedIDX);	
}

void CPlayer::DeleteWanted(WANTEDTYPE WantedIDX)
{
	m_WantedList.Remove((void*)WantedIDX);
}

void CPlayer::ClearWantedList()
{
	m_WantedList.RemoveAll();
}

BOOL CPlayer::IsWantedOwner(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDTYPE, id)
		if( id == WantedIDX )
		{
			return TRUE;
		}
	PTRLISTSEARCHEND
		return FALSE;
}

void CPlayer::QuestProcess()
{
	m_QuestGroup.Process();
}


void CPlayer::SetPKModeEndtime()
{
	m_HeroInfo.LastPKModeEndTime = 0;
}


void CPlayer::AddBadFameReduceTime()
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return;
	if( GetState() == eObjectState_Immortal )		return;
	if( IsPKMode() )								return;
	if( GetBadFame() == 0 && GetWantedIdx() == 0 )	return;

	m_HeroInfo.LastPKModeEndTime += gTickTime;


	if( GetBadFame() == 0 )
	{
		if(m_HeroInfo.LastPKModeEndTime > WANTED_DESTROYED_TIME)
		{
			if(GetWantedIdx() != 0 )
			{	
				WANTEDMGR->DestroyWanted(this);
			}
		}     
	}
	else if( m_HeroInfo.LastPKModeEndTime > 600*1000 )
	{
		m_HeroInfo.LastPKModeEndTime -= 600*1000;
		PKCharacterUpdate( GetID(), m_HeroInfo.LastPKModeEndTime );

		SetBadFame( GetBadFame()-1 );
		if(GetBadFame() == 0)
			m_HeroInfo.LastPKModeEndTime = 0;
		BadFameCharacterUpdate( GetID(), GetBadFame() );
		FAMEMGR->SendBadFameMsg( this, GetBadFame() );
	}
}




BOOL CPlayer::SpeedHackCheck(WORD wTime)
{
	BOOL bIsHack = FALSE;
	++m_nHackCount;
	//g_Console.LOGEX(4, RGB(0, 255, 0), "角色名[%s],被检测次数[%d]", GetObjectName(), m_nHackCount);
	if( gCurTime - m_dwHackStartTime >= 60000 )
	{
		if( m_nHackCount >= g_nHackCheckWriteNum )
		{
			InsertSpeedHackCheck(GetID(), GetObjectName(), m_nHackCount, wTime);
			g_Console.LOGEX(4, RGB(255, 0, 0), "玩家[%s]使用连招", GetObjectName());
			g_Console.LOGEX(4, RGB(0, 255, 0), "检查间隔时间[%d]", wTime);
			bIsHack = TRUE;
		}
		if( m_nHackCount >= g_nHackCheckNum )
		{
			MSGBASE msg;
			msg.Category = MP_HACKCHECK;
			msg.Protocol = MP_HACKE_CHECKED_SYN;
			msg.dwObjectID = GetID();
			SendMsg(&msg, sizeof(msg));
			bIsHack = TRUE;
		}
		m_nHackCount		= 1;
		m_dwHackStartTime	= gCurTime;
	}
	return bIsHack;
}

void CPlayer::ClearMurderIdx()
{
	SetReadyToRevive(FALSE);
	m_MurdererKind = 0;
	m_MurdererIDX = 0;
	m_MurderObjKind = 0;
	m_bPenaltyByDie = FALSE;
}


DWORD CPlayer::Damage(CObject* pAttacker,RESULTINFO* pDamageInfo)
{
	DWORD realDamage = pDamageInfo->RealDamage;

	if( InTitan() )
	{
		DWORD titanObsorb = DWORD((pDamageInfo->RealDamage / 100.f)*90.f + 0.5f);

		if(titanObsorb)
		{
			pDamageInfo->TitanObsorbDamage = titanObsorb;
		}
		pDamageInfo->RealDamage =  realDamage - titanObsorb;

		GetCurTitan()->Damage(pAttacker, pDamageInfo);
	}

	DWORD life = GetLife();
	DWORD beforelife = life;

	if( life > pDamageInfo->RealDamage )
	{
		life -= pDamageInfo->RealDamage;
	}
	else
	{
		if( GetUserLevel() == eUSERLEVEL_GM ) 
		{
			life = 1;
		}
		else
		{
			life = 0;
		}
	}
	
	SetLife(life,FALSE);
	DoDamage(pAttacker,pDamageInfo,beforelife);

	return life;
}

void CPlayer::SetGuildInfo(DWORD GuildIdx, BYTE Grade, char* GuildName, MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MunpaID = GuildIdx;
	m_HeroCharacterInfo.PositionInMunpa = Grade;
	SafeStrCpy(m_HeroCharacterInfo.GuildName, GuildName, MAX_GUILD_NAME+1);
	m_HeroCharacterInfo.MarkName = MarkName;
}

void CPlayer::RSetMoney( MONEYTYPE money, BYTE flag )
{
	CPurse* pPurse = m_ItemContainer.GetPurse(eItemTable_Inventory);
	if( !pPurse ) return;

	pPurse->RSetMoney( money, flag );
	m_HeroInfo.Money = money;
}

void CPlayer::SetGuildMarkName(MARKNAMETYPE MarkName)
{
	m_HeroCharacterInfo.MarkName = MarkName;
}

char* CPlayer::GetGuildCanEntryDate()
{ 
	return m_HeroInfo.MunpaCanEntryDate;
}

void CPlayer::SetGuildCanEntryDate(char* date)
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, date, 11);
}

void CPlayer::ClrGuildCanEntryDate()
{
	SafeStrCpy(m_HeroInfo.MunpaCanEntryDate, "2004.01.01", 11);
}


void CPlayer::SendShopItemInfo()
{
	SEND_SHOPITEM_INFO ShopItem;
	ShopItem.Category = MP_ITEM;
	ShopItem.Protocol = MP_ITEM_SHOPITEM_INFO_ACK;	
	ShopItem.ItemCount = 0;


	POSTYPE startpos = TP_SHOPITEM_START;

	for(int i=0; i<SLOT_SHOPITEM_IMSI; i++)
	{
		if( m_ShopItemSlot.IsEmpty(startpos+i) == FALSE )
		{
			ShopItem.Item[ShopItem.ItemCount] = *m_ShopItemSlot.GetItemInfoAbs(startpos+i);
			if( ShopItem.Item[ShopItem.ItemCount].ItemParam & ITEM_PARAM_PRESENT )
			{
				((ITEMBASE*)m_ShopItemSlot.GetItemInfoAbs(startpos+i))->ItemParam ^= ITEM_PARAM_PRESENT;				
				ShopItemParamUpdateToDB( GetID(), ShopItem.Item[ShopItem.ItemCount].dwDBIdx, m_ShopItemSlot.GetItemInfoAbs(startpos+i)->ItemParam );
			}
			++ShopItem.ItemCount;
		}
	}

	SendMsg(&ShopItem, ShopItem.GetSize());
}


void CPlayer::UpdateLogoutToDB()
{
	m_ShopItemManager.UpdateLogoutToDB();
	m_PetManager.UpdateLogoutToDB();
}


BOOL CPlayer::IsAbleReviveOther()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; 
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2;
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}
	
	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4;
		SendMsg( &msg, sizeof(msg) );

		return FALSE;
	}

	return TRUE;
}


BOOL CPlayer::ReviveShopItem( WORD ItemIdx )
{
	ITEM_INFO* pItem = ITEMMGR->GetItemInfo( ItemIdx );
	if( !pItem )		return FALSE;

	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = GetID();
	msg.dwMoverID = GetID();
	msg.cpos.Compress(CCharMove::GetPosition(this));
	PACKEDDATA_OBJ->QuickSend(this,&msg,sizeof(msg));

	if( pItem->SellPrice )	
		InsertLogExp( eExpLog_ReviveExpPeriod, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
	else
		InsertLogExp( eExpLog_ReviveExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

	OBJECTSTATEMGR_OBJ->EndObjectState(this,eObjectState_Die);
	
	DWORD CurLife = GetMaxLife();
	DWORD CurNaeryuk = GetMaxNaeRyuk();
	DWORD CurShield = GetMaxShield();
	SetLife((DWORD)(CurLife));
	SetNaeRyuk((DWORD)(CurNaeryuk));
	SetShield((DWORD)(CurShield));

	m_YYLifeRecoverTime.bStart = FALSE;
	m_YYNaeRyukRecoverTime.bStart = FALSE;
	ClearMurderIdx();


	MSG_DWORD2	msg1;
	msg1.Category = MP_ITEM;
	msg1.Protocol = MP_ITEM_SHOPITEM_EFFECT;				
	msg1.dwData1 = GetID();
	msg1.dwData2 = eEffect_ShopItem_Revive;
	PACKEDDATA_OBJ->QuickSend( this, &msg1, sizeof(msg1) );

	m_bDieForGFW = FALSE;
	m_bDieInSpecialMap = FALSE;
	m_bDieInUnionWarMap = FALSE;
	return TRUE;
}

void CPlayer::ReviveVillage()
{
	if(GetState() != eObjectState_Die)
	{
		ASSERT(0);
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 1; 
		SendMsg( &msg, sizeof(msg) );

		return;
	}
	
	if( LOOTINGMGR->IsLootedPlayer( GetID() ) )	
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 2;
		SendMsg( &msg, sizeof(msg) );

		return;
	}

	if( IsExitStart() )
	{
		MSG_BYTE msg;
		msg.Category	= MP_USERCONN;
		msg.Protocol	= MP_USERCONN_CHARACTER_REVIVE_NACK;
		msg.bData		= 4;
		SendMsg( &msg, sizeof(msg) );

		return;
	}


//#ifdef _CHINA_LOCAL_
//
//#elif defined _KOR_LOCAL_
//	if( IsActionPanelty() == FALSE )
//	{
//		SetActionPanelty();
//		InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Village );
//	}
//	else
//#else
//	{

	if( !EVENTMAPMGR->IsEventMap() )
	if(GetLevel() >= 5 && m_bDieForGFW == FALSE )
	{
		DWORD ItemIdx = 0;
		SHOPITEMWITHTIME* pShopItem = NULL;
		if( GetShopItemStats()->ProtectCount )		
		{
			DWORD ItemIdx = m_ShopItemManager.GetProtectItemIdx();
			if( ItemIdx == 0 )
				goto ProtectAll_UseFail;
			pShopItem = m_ShopItemManager.GetUsingItemInfo( ItemIdx );
			if( !pShopItem )
				goto ProtectAll_UseFail;

			LogItemMoney( GetID(), GetObjectName(), 0, "",
				eLog_ShopItemProtectAll, GetMoney(eItemTable_Inventory), 0, 0,
				ItemIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, GetPlayerExpPoint());
			InsertLogExp( eExpLog_ProtectExp, GetID(), GetLevel(), GetShopItemStats()->ProtectCount, GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());

			--GetShopItemStats()->ProtectCount;
			if( GetShopItemStats()->ProtectCount <= 0 )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, ItemIdx);
				m_ShopItemManager.DeleteUsingShopItem( ItemIdx );
				m_ShopItemManager.SetProtectItemIdx( 0 );
			}
			else
			{
				ShopItemUseParamUpdateToDB( 0, GetID(), ItemIdx, GetShopItemStats()->ProtectCount );
			}
			m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_PROTECTALL, GetShopItemStats()->ProtectCount);
		}
		else
		{
ProtectAll_UseFail:
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_MoneyProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_MONEYPROTECT, eIncantation_MoneyProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_MoneyProtect );
			}
			else
			{
				CItemSlot* pSlot = m_ItemContainer.GetSlot(eItemTable_Inventory);
				DWORD CurMoney = pSlot->GetMoney();
				if( !m_bDieForGFW )
					SetMoney((MONEYTYPE)(CurMoney*0.04), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseDie, 0);
			}
			
			if( m_ShopItemManager.GetUsingItemInfo( eIncantation_ExpProtect ) )
			{
				m_ShopItemManager.SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_EXPPROTECT, eIncantation_ExpProtect);
				m_ShopItemManager.DeleteUsingShopItem( eIncantation_ExpProtect );

			}
			else
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(GetLevel());
				EXPTYPE minusExp = 0;

				if( !m_bDieForGFW )	minusExp = (EXPTYPE)(pointForLevel * 0.01);

				ReduceExpPoint(minusExp, eExpLog_LosebyRevivePresent);

				m_PetManager.ReducePetFriendshipWithMastersRevive();
			}
		}
	}

	//}		

//#endif // #ifdef _CHINA_LOCAL_

	if(GetState() == eObjectState_Die)
		OBJECTSTATEMGR_OBJ->EndObjectState( this ,eObjectState_Die );

	MSG_DWORD msg;
	msg.Category = MP_SIEGEWAR;
	msg.Protocol = MP_SIEGEWAR_RETURNTOMAP;
	msg.dwData = GetReturnMapNum();
	SendMsg( &msg, sizeof(msg) );
	g_pServerSystem->RemovePlayer( GetID() );
}

void CPlayer::SetNickName(char* NickName)
{
	SafeStrCpy(m_HeroCharacterInfo.NickName,NickName, MAX_GUILD_NICKNAME+1);
}

BOOL CPlayer::CanSocietyAction( DWORD curTime )
{
	DWORD dwElapsedTime = curTime - m_dwLastSocietyAction;

	if( dwElapsedTime > 2000 )	
	{
		m_dwLastSocietyAction = curTime;
		return TRUE;
	}

	return FALSE;
}

LEVELTYPE CPlayer::GetLevel()
{ 
	return m_HeroCharacterInfo.Level; 
}

DWORD CPlayer::GetLife() 
{ 
	return m_HeroCharacterInfo.Life; 
}

DWORD CPlayer::GetShield() 
{ 
	return m_HeroCharacterInfo.Shield; 
}

DWORD CPlayer::GetNaeRyuk()
{ 
	return m_HeroInfo.wNaeRyuk; 
}

DWORD CPlayer::DoGetMaxLife()
{ 
	return m_HeroCharacterInfo.MaxLife; 
}

DWORD CPlayer::DoGetMaxShield()
{ 
	return m_HeroCharacterInfo.MaxShield; 
}

DWORD CPlayer::DoGetMaxNaeRyuk()
{ 
	return m_HeroInfo.wMaxNaeRyuk; 
}

DWORD CPlayer::DoGetPhyDefense()
{ 
	int nPhyDefen = (int)(GetCharStats()->PhysicalDefense+GetItemStats()->PhysicalDefense+GetAbilityStats()->DefenceUp+GetSetItemStats()->wPhyDef+GetUniqueItemStats()->nDefen);
	if(nPhyDefen >= 0)
	{
		return (DWORD)nPhyDefen;
	}
	else
	{
		return 0;
	}
}

float CPlayer::DoGetAttDefense(WORD Attrib)
{
	if(InTitan())
		return GetCurTitan()->DoGetAttDefense(Attrib);
	else
		return GetItemStats()->AttributeResist.GetElement_Val(Attrib)+GetAbilityStats()->AttRegistUp.GetElement_Val(Attrib)+GetSetItemStats()->AttrRegist.GetElement_Val(Attrib); 
}	

float CPlayer::GetAttribPlusPercent(WORD Attrib)
{ 
	return (GetItemStats()->AttributeAttack.GetElement_Val(Attrib)+GetAbilityStats()->AttAttackUp.GetElement_Val(Attrib)+GetSetItemStats()->AttrAttack.GetElement_Val(Attrib));//[属性攻击力][By:十里坡剑神][QQ:112582793][2018/10/6]	
}

void CPlayer::SetStage( BYTE stage )
{
	if( m_HeroCharacterInfo.Stage == stage )	return;
	if( stage == eStage_Normal )
	{
		m_HeroCharacterInfo.Stage = stage;

		MSG_BYTE Msg;
		Msg.Category = MP_CHAR;
		Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
		Msg.dwObjectID = GetID();
		Msg.bData = stage;
		PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

		CharacterTotalInfoUpdate( this );	
	}
	else
	{
		{
			m_HeroCharacterInfo.Stage = stage;
			
			MSG_BYTE Msg;
			Msg.Category = MP_CHAR;
			Msg.Protocol = MP_CHAR_STAGE_NOTIFY;
			Msg.dwObjectID = GetID();
			Msg.bData = stage;
			PACKEDDATA_OBJ->QuickSend( this, &Msg, sizeof(Msg) );

			CharacterTotalInfoUpdate( this );	
		}
	}

//#ifndef _JAPAN_LOCAL_
	SetMussangMaxPoint();
//#endif
}

void CPlayer::SendPlayerToMap()
{
	if( m_wReturnMapNum==0 )	return;

	MSG_WORD2 Msg;
	Msg.Category = MP_USERCONN;
	Msg.Protocol = MP_USERCONN_BACKTOBEFOREMAP_SYN;
	Msg.wData1 = g_pServerSystem->GetMapNum();

	Msg.wData2 = m_wReturnMapNum;
	 
	SendMsg( &Msg, sizeof(Msg) );

	m_bDieInSpecialMap = FALSE;
	m_bDieInUnionWarMap = FALSE;
	m_wReturnMapNum = 0;
	m_bNeedRevive = FALSE;
}

DOUBLE CPlayer::GetPlayerTotalExpPoint()
{
	DOUBLE exp = 0;

	for(int i=1; i<GetLevel(); ++i)
	{
		exp += GAMERESRCMNGR->GetMaxExpPoint( i );
	}

	exp += GetPlayerExpPoint();

	return exp;
}

void CPlayer::SetChangeAbilityKyungGongLevel( WORD wLevel )
{
	MSG_DWORD2 msg;
	msg.Category = MP_KYUNGGONG;
	msg.Protocol = MP_KYUNGGONG_ABILITY_CHANGE_NOTIFY;
	msg.dwData1 = this->GetID();
	msg.dwData2 = wLevel;
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &msg, sizeof(msg) );	
}

void CPlayer::InitGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;
}

void CPlayer::SetGuildUnionInfo( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMarkIdx )
{
	m_HeroCharacterInfo.dwGuildUnionIdx = dwGuildUnionIdx;
	strncpy( m_HeroCharacterInfo.sGuildUnionName, pGuildUnionName, MAX_GUILD_NAME+1 );
	m_HeroCharacterInfo.dwGuildUnionMarkIdx = dwMarkIdx;

	MSG_NAME_DWORD3 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_PLAYER_INFO;
	Msg.dwData1 = GetID();
	Msg.dwData2 = dwGuildUnionIdx;
	Msg.dwData3 = dwMarkIdx;
	strncpy( Msg.Name, m_HeroCharacterInfo.sGuildUnionName, MAX_GUILD_NAME+1 );

	PACKEDDATA_OBJ->QuickSendExceptObjectSelf( this, &Msg, sizeof(Msg) );
}

void CPlayer::SetCharChangeInfo( CHARACTERCHANGE_INFO* pInfo )
{
	m_HeroCharacterInfo.Gender = pInfo->Gender;
	m_HeroCharacterInfo.HairType = pInfo->HairType;
	m_HeroCharacterInfo.FaceType = pInfo->FaceType;
	m_HeroCharacterInfo.Height = pInfo->Height;
	m_HeroCharacterInfo.Width = pInfo->Width;
}

//#ifdef _JAPAN_LOCAL_
//BOOL CPlayer::ChangeMainCharAttr( int nMainAttr )
//{
//	m_HeroCharacterInfo.nMainAttr = nMainAttr;
//	m_HeroCharacterInfo.nSubAttr = 0;
//
//	ChangeCharacterAttr( GetID(), MAKEDWORD(nMainAttr, 0) );
//	
//	return TRUE;
//}
//
//BOOL CPlayer::ChangeSubCharAttr( int nSubAttr )
//{
//	int nMainAttr = GetMainCharAttr();
//
//	if( nMainAttr == eCA_None )		return FALSE;
//	if( nMainAttr == nSubAttr )		return FALSE;
//
//	switch( nMainAttr )
//	{
//	case eCA_Fire:
//		{
//			if( nSubAttr != eCA_Earth && nSubAttr != eCA_Tree )		return FALSE;
//		}
//		break;
//	case eCA_Water:
//		{
//			if( nSubAttr != eCA_Tree && nSubAttr != eCA_Iron )		return FALSE;
//		}
//		break;
//	case eCA_Tree:
//		{
//			if( nSubAttr != eCA_Fire && nSubAttr != eCA_Water )		return FALSE;
//		}
//		break;
//	case eCA_Iron:
//		{
//			if( nSubAttr != eCA_Water && nSubAttr != eCA_Earth )	return FALSE;
//		}
//		break;
//	case eCA_Earth:
//		{
//			if( nSubAttr != eCA_Iron && nSubAttr != eCA_Fire )		return FALSE;
//		}
//		break;
//	}
//	
//	m_HeroCharacterInfo.nSubAttr = nSubAttr;
//
//	ChangeCharacterAttr( GetID(), MAKEDWORD(nMainAttr, nSubAttr) );
//
//	return TRUE;
//}
//
//int CPlayer::WhatIsAttrRelation( int nOtherMainAttr )
//{
//	if( m_HeroCharacterInfo.nMainAttr == eCA_None || nOtherMainAttr == eCA_None )	return eCAR_None;
//
//	int nRelation = nOtherMainAttr - GetMainCharAttr();
//	if( nRelation < 0 )	nRelation += 5;
//
//	return nRelation;
//}
//
//void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
//{
//	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
//	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
//	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;
//
//	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
//	m_InventorySlot.SetExtraSlotCount( InvenSlot );
//}
//
//#endif
//
//
//#ifdef _HK_LOCAL_
//void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
//{
//	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
//	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
//	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;
//
//	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
//	m_InventorySlot.SetExtraSlotCount( InvenSlot );
//}
//#endif
//
//#ifdef _TL_LOCAL_
//void CPlayer::SetExtraSlotCount( DWORD PyogukSlot, DWORD InvenSlot, DWORD MugongSlot )
//{
//	m_HeroCharacterInfo.ExtraInvenSlot = (char)InvenSlot;
//	m_HeroCharacterInfo.ExtraPyogukSlot = (char)PyogukSlot;
//	m_HeroCharacterInfo.ExtraMugongSlot = (char)MugongSlot;
//
//	m_PyogukSlot.SetExtraSlotCount( PyogukSlot );
//	m_InventorySlot.SetExtraSlotCount( InvenSlot );
//}
//#endif

void CPlayer::SetMussangMaxPoint()
{
	int iMaxMussangPoint = GetMussangMaxPoint();

	if(iMaxMussangPoint < 0)
	{
		iMaxMussangPoint = 0;
		ASSERT(0);
	}

	if( m_HeroInfo.MaxMussangPoint != (DWORD)iMaxMussangPoint )
	{
		m_HeroInfo.MaxMussangPoint = iMaxMussangPoint;

		MSG_DWORDBYTE msg;	
		msg.Category = MP_CHAR;
		msg.Protocol = MP_CHAR_MUSSANG_INFO;
		msg.dwObjectID = GetID();
		msg.dwData = m_HeroInfo.MaxMussangPoint;
		msg.bData = eMSS_LevelUp;
		
		SendMsg(&msg,sizeof(msg));
	}
}

void CPlayer::AddMussangPointFromLevel(DWORD dwMonsterLevel)
{
	if( m_bIsMussangReady ) return;

	int dwAddPoint = dwMonsterLevel + dwMonsterLevel - GetLevel();

	if( dwAddPoint < 1 )
		dwAddPoint = 1;
	if( m_AvatarOption.MussangCharge )
		dwAddPoint = int(dwAddPoint*(1+m_AvatarOption.MussangCharge*0.01f));

	if( GetPartyIdx() )
	{
		CParty* pParty = PARTYMGR->GetParty( GetPartyIdx() );
		if( pParty )
		{
			int count = pParty->GetMemberCountofMap( GetID() );
			if( count && gPartyEvent[ePartyEvent_MussangCharge].Rate[count-1] )//[组队无双恢复速度加成计算][By:十里坡剑神][QQ:112582793][2018/4/22]
				dwAddPoint = (DWORD)(dwAddPoint*gPartyEvent[ePartyEvent_MussangCharge].Rate[count-1]);
		}
	}

	m_dwMussangPoint += dwAddPoint;	

	if( m_dwMussangPoint >= m_HeroInfo.MaxMussangPoint )
	{
		m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	}

	if( !m_bIsMussangReady )
	{
		if( m_dwMussangPoint == m_HeroInfo.MaxMussangPoint )
			m_bIsMussangReady = TRUE;
		SendMussangInfo();
	}
}

DWORD CPlayer::GetMussangMaxPoint()	
{
//#ifdef _JAPAN_LOCAL_
//	return GetLevel()*130 + 60;
//#else
	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
		return GetLevel()*130 + 780;	//( GetLevel() + 9 ) * 120 ) - ( ( 30 - GetLevel() ) * 10 )
	case eStage_Hwa:		
	case eStage_Geuk:
		return GetLevel()*250 + 1620;	//( GetLevel() + 8 ) * 240 ) - ( ( 30 - GetLevel() ) * 10 )
	case eStage_Hyun:	  		
	case eStage_Tal:	
		return GetLevel()*370 + 2220;	//( GetLevel() + 7 ) * 360 ) - ( ( 30 - GetLevel() ) * 10 )
	}

	return 0;
//#endif
}

void CPlayer::SendMussangInfo()
{
	MSG_DWORDBYTE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_INFO;
	msg.dwObjectID = GetID();
	msg.dwData	= m_dwMussangPoint;

	if( m_bIsMussangReady )	m_bMussangFlag |= eMSS_Ready;

	msg.bData	= m_bMussangFlag;

	SendMsg(&msg,sizeof(msg));

	m_bMussangFlag = 0;
}

DWORD	CPlayer::StartMussangMode()
{
	if( !m_bIsMussangReady )
	{
		ASSERTMSG(0, "MussangPoint:Sync Err");
		return -99;
	}

	m_HeroCharacterInfo.bMussangMode = TRUE;

	m_bIsMussangReady = FALSE;	

	InitMussangPoint();

	SetMussangMode(TRUE);

	SendMussangInfo();
	
	m_dwMussangStartTime = gCurTime;

	m_dwMussangTime = 0;
#ifndef _JAPAN_LOCAL_
	switch(m_HeroCharacterInfo.Stage)
	{
	case eStage_Normal:	
		{
			m_wMussangStat = 0;
			m_dwMussangTime = 60000;
		}
		break;
	case eStage_Hwa:		
	case eStage_Geuk:
		{
			m_wMussangStat = 10;
			m_dwMussangTime = 90000;
		}
		break;
	case eStage_Hyun:
	case eStage_Tal:
		{
			m_wMussangStat = 15;
			m_dwMussangTime = 120000;
		}
		break;
	}

	m_dwMussangTime += (m_HeroCharacterInfo.Level / 10) * 10000;

	DWORD addTime = 0;

	GetPetManager()->GetPetBuffResultRt(ePB_MussangTimeIncrease, &addTime);

	if(addTime)
	{
		m_dwMussangTime += addTime;
	}

	CalcState();
#endif

	return m_dwMussangTime;
}

void CPlayer::CheckMussangTime()
{
	if( !m_HeroCharacterInfo.bMussangMode ) return;	

//#ifdef _JAPAN_LOCAL_
//	if( gCurTime - m_dwMussangStartTime > MUSSANG_VALID_TIME )
//	{
//		SetMussangMode(FALSE);
//		SendMussangEnd();
//	}
//#else
	if( gCurTime - m_dwMussangStartTime > m_dwMussangTime )
	{
		SetMussangMode(FALSE);
		SendMussangEnd();

		m_wMussangStat = 0;

	
		CalcState();
	}
//#endif
}

void CPlayer::SendMussangEnd()
{
	MSGBASE msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_MUSSANG_END;
	msg.dwObjectID = GetID();
	
	PACKEDDATA_OBJ->QuickSend(this, &msg, sizeof(MSGBASE));
}

void CPlayer::CheatMussangReady()
{
	m_dwMussangPoint = m_HeroInfo.MaxMussangPoint;
	m_bIsMussangReady = TRUE;

	SendMussangInfo();
}

//#ifdef _JAPAN_LOCAL_
//DWORD CPlayer::CalcShieldDamage(DWORD ShieldDamage)
//{
//	DWORD Shield = GetShield();
//
//	if(InTitan())	return 0;
//
//	if(Shield < ShieldDamage)
//	{
//		SetShield(0,FALSE);
//		return Shield;
//	}
//	else
//	{
//		SetShield(Shield-ShieldDamage,FALSE);
//		return ShieldDamage;
//	}
//}
//#else
DWORD CPlayer::CalcShieldDamage(DWORD ShieldDamage, DWORD &RealShieldDamage)
{
	DWORD Shield = GetShield();
	RealShieldDamage = ShieldDamage;

	if(IsMussangMode())
		RealShieldDamage = (DWORD)(RealShieldDamage * 0.7);

	if(Shield < RealShieldDamage)
	{
		SetShield(0,FALSE);
		RealShieldDamage = Shield;
		return (WORD)Shield;
	}
	else
	{
		SetShield(Shield-RealShieldDamage,FALSE);
		return ShieldDamage;
	}
}
//#endif

BOOL CPlayer::IsSkipSkill()
{
	if(	m_HeroCharacterInfo.Stage != eStage_Hyun &&
		m_HeroCharacterInfo.Stage != eStage_Tal )
		return FALSE;

	WORD SkipKind = 0;

	PTRLISTSEARCHSTART(m_StatusList,CStatus*,pSL)
		pSL->GetSkipEffectKind(SkipKind);
	PTRLISTSEARCHEND;

	if(SkipKind == 0)
		return FALSE;
	else
		return (BOOL)SkipKind;
}
////////////////////////////////////////////////////////
void CPlayer::AddSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life += pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk += pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield += pSkillOption->Shield;
	m_SkillStatsOption.PhyDef += pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef += pSkillOption->AttDef;

	m_SkillStatsOption.Range += pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk += pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk += pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk += pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk += pSkillOption->AttAtk;
}

void CPlayer::RemoveSkillStatsOption(SKILLOPTION *pSkillOption)
{
	m_SkillStatsOption.Life -= pSkillOption->Life;
	m_SkillStatsOption.NaeRyuk -= pSkillOption->NaeRyuk;
	m_SkillStatsOption.Shield -= pSkillOption->Shield;
	m_SkillStatsOption.PhyDef -= pSkillOption->PhyDef;
	m_SkillStatsOption.AttDef -= pSkillOption->AttDef;

	m_SkillStatsOption.Range -= pSkillOption->Range;
	m_SkillStatsOption.ReduceNaeRyuk -= pSkillOption->ReduceNaeRyuk;
	m_SkillStatsOption.PhyAtk -= pSkillOption->PhyAtk;
	m_SkillStatsOption.BaseAtk -= pSkillOption->BaseAtk;
	m_SkillStatsOption.AttAtk -= pSkillOption->AttAtk;
}
//////////////////////////////////////////////////////////////////////////
BOOL CPlayer::CanSkillState()
{
	if(m_SkillFailCount >= 5)
	{
		switch(m_BaseObjectInfo.ObjectState)
		{
		case eObjectState_None:
		case eObjectState_Move:
		case eObjectState_TiedUp_CanSkill:
			{
				m_SkillFailCount = 0;
				return TRUE;
			}
			break;
		case eObjectState_SkillStart:
		//case eObjectState_SkillSyn:	
		case eObjectState_SkillBinding:
		//case eObjectState_SkillUsing:
		//case eObjectState_SkillDelay:
		case eObjectState_Society:
		case eObjectState_Enter:
		case eObjectState_Ungijosik:
		case eObjectState_Tactic:
		case eObjectState_Rest:
			{
				m_SkillFailCount = 0;
				m_BaseObjectInfo.ObjectState = eObjectState_None;
				return TRUE;
			}
			break;
		default:
			{
				m_SkillFailCount = 0;
				return FALSE;
			}
			break;
		}
	}
	if(m_BaseObjectInfo.ObjectState != eObjectState_None &&
	   m_BaseObjectInfo.ObjectState != eObjectState_Move &&
	   m_BaseObjectInfo.ObjectState != eObjectState_TiedUp_CanSkill )
	{
		m_SkillFailCount++;
		return FALSE;
	}
	m_SkillFailCount = 0;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////

void CPlayer::IncreaseEventHammerCount()
{
	if( m_HeroCharacterInfo.EventIndex != eFULLMOONEVENT_NONE )
		return;

	++m_FullMoonEventHCt;

	if( m_FullMoonEventHCt >= 4 )
	{
		int rate = rand()%100;
		m_HeroCharacterInfo.EventIndex = GAMERESRCMNGR->GetFullMoonEventIdx( rate );

		if( m_HeroCharacterInfo.EventIndex != eFULLMOONEVENT_NONE )
		{
			m_fFullMoonEventTime = 60000.f;

			MSG_BYTE	msg;
			SetProtocol( &msg, MP_CHAR, MP_CHAR_FULLMOONEVENT_CHANGE );
			msg.bData = m_HeroCharacterInfo.EventIndex;
			msg.dwObjectID = GetID();
			PACKEDDATA_OBJ->QuickSend( this, &msg, sizeof(msg) );		
		}
		m_FullMoonEventHCt = 0;
	}
}

void CPlayer::SummonEventPetRndm()
{
	if( m_PetManager.GetCurSummonPet() != NULL )
		return;

#define SUMMONRATE_EVENTPET 10
	int rnd = rand()%100;

	if( SUMMONRATE_EVENTPET < rnd )
		return;
	else
	{
		m_PetManager.SummonEventPet();
	}


}

void CPlayer::SendFlagNPCState()
{
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_FLAGNPC_ONOFF;

	msg.dwData1 = GTMGR->GetGTState();
	msg.dwData2 = SIEGEWARMGR->GetSWState();

	SendMsg(&msg, sizeof(msg));
}

void CPlayer::InitTitanWearItemInfo( ITEMBASE* pTitanItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_Titan)->SetItemInfoAll(pTitanItemInfo);	
}

void CPlayer::InitTitanShopItemInfo( ITEMBASE* pTitanItemInfo )
{
	m_ItemContainer.GetSlot(eItemTable_TitanShopItem)->SetItemInfoAll(pTitanItemInfo);	
}

BOOL CPlayer::InTitan()
{
	if( NULL != m_TitanManager.GetCurRidingTitan() )
		return TRUE;
	else
		return FALSE;
}

BOOL CPlayer::CanUseTitanSkill()
{
	if( FALSE == InTitan() )
		return FALSE;
	return TRUE;
}

void CPlayer::SetTitanWearInfo( POSTYPE slotPos, ITEMBASE* pItemBase, BOOL bIn ) 
{
	m_TitanManager.SetWearedInfo(slotPos, pItemBase, bIn);
}

void CPlayer::ApplyTitanWearItem()
{
	STATSMGR->CalcTitanItemStats(this);

	m_TitanManager.SetTitanStats();

	if(GetInited())
	{
		m_TitanManager.SendTitanStats();
		m_TitanManager.SendTitanPartsChange();	
	}

	if(InTitan() == TRUE)
	{
		CTitan* pRidingTitan = m_TitanManager.GetCurRidingTitan();
		titan_calc_stats* pStats = pRidingTitan->GetTitanStats();
		if(pStats->MaxFuel < pRidingTitan->GetTotalInfo()->Fuel)
		{
			pRidingTitan->SetLife(pStats->MaxFuel);
		}	
	}	
}

void CPlayer::AddSetitemOption(SET_ITEM_OPTION* pSetItemOption)
{
	float attrvalue = 0;

	m_setItemStats.wGenGol += pSetItemOption->wGenGol;	

	m_setItemStats.wMinChub += pSetItemOption->wMinChub;

	m_setItemStats.wCheRyuk += pSetItemOption->wCheRyuk;

	m_setItemStats.wSimMek += pSetItemOption->wSimMek;

	m_setItemStats.dwLife += pSetItemOption->dwLife;

	m_setItemStats.dwShield += pSetItemOption->dwShield;

	m_setItemStats.dwNaeRyuk += pSetItemOption->dwNaeRyuk;

	m_setItemStats.AttrRegist.AddATTRIBUTE_VAL(pSetItemOption->AttrRegist,1);	

	m_setItemStats.wMeleeAttackMin += pSetItemOption->wMeleeAttackMin;

	m_setItemStats.wMeleeAttackMax += pSetItemOption->wMeleeAttackMax;

	m_setItemStats.wRangeAttackMin += pSetItemOption->wRangeAttackMin;

	m_setItemStats.wRangeAttackMax += pSetItemOption->wRangeAttackMax;

	m_setItemStats.wCriticalPercent += pSetItemOption->wCriticalPercent;	

	m_setItemStats.AttrAttack.AddATTRIBUTE_VAL(pSetItemOption->AttrAttack,1);		

	m_setItemStats.wPhyDef += pSetItemOption->wPhyDef;

	m_setItemStats.wLifeRecover += pSetItemOption->wLifeRecover;

	m_setItemStats.fLifeRecoverRate += pSetItemOption->fLifeRecoverRate;

	m_setItemStats.wNaeRyukRecover += pSetItemOption->wNaeRyukRecover;

	m_setItemStats.fNaeRyukRecoverRate += pSetItemOption->fNaeRyukRecoverRate;
}

void CPlayer::ClearSetitemOption()
{
	memset(&m_setItemStats, 0, sizeof(SET_ITEM_OPTION));
}

void CPlayer::SendNoActionPaneltyMsg()
{
	MSGBASE msg;
	SetProtocol( &msg, MP_CHAR, MP_CHAR_NOACTIONPANELTY_NOTIFY );
	SendMsg( &msg, sizeof(msg) );
}

void CPlayer::InsetNoActionPaneltyLog( BYTE eLogKind )
{
	InsertLogExp( eLogKind, GetID(), GetLevel(), 
		GetMoney(), GetPlayerExpPoint(), m_MurdererKind, m_MurdererIDX, GetPlayerAbilityExpPoint());
}


/////////////////////////////////////////////////////////////////////////////////////

void CPlayer::AddUniqueItemOption(UNIQUE_ITEM_OPTION_INFO* pUniqueOption)
{
	m_UniqueItemStats.nHp += pUniqueOption->nHp;
	m_UniqueItemStats.nMp += pUniqueOption->nMp;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
	m_UniqueItemStats.nPhyDamage += pUniqueOption->nPhyDamage;
	m_UniqueItemStats.nCriDamage += pUniqueOption->nCriDamage;
	m_UniqueItemStats.nCriRate += pUniqueOption->nCriRate;
	m_UniqueItemStats.nGengol += pUniqueOption->nGengol;
	m_UniqueItemStats.nMinChub += pUniqueOption->nMinChub;
	m_UniqueItemStats.nCheRyuk += pUniqueOption->nCheRyuk;
	m_UniqueItemStats.nSimMek += pUniqueOption->nSimMek;
	m_UniqueItemStats.nDefen += pUniqueOption->nDefen;
	m_UniqueItemStats.nRange += pUniqueOption->nRange;
	m_UniqueItemStats.nAttR += pUniqueOption->nAttR;
	m_UniqueItemStats.nEnemyDefen += pUniqueOption->nEnemyDefen;
	m_UniqueItemStats.nShield += pUniqueOption->nShield;
}
//////////////////////////////////////////////////////////////////////////
void CPlayer::InitSkinDelay()	
{
	m_dwSkinDelayTime = 0;
	m_bSkinDelayResult = FALSE;
}

void CPlayer::StartSkinDelay()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}

void CPlayer::SkinDelayTimeProcess()
{
	if(m_bSkinDelayResult == TRUE)
	{
		if(gCurTime > m_dwSkinDelayTime)
		{
			InitSkinDelay();
		}
	}
}

BOOL CPlayer::GetSkinDelayResult()
{
	return m_bSkinDelayResult;
}

DWORD CPlayer::GetSkinDelayTime()
{
	DWORD dwTime = m_dwSkinDelayTime - gCurTime;
	if(m_bSkinDelayResult == FALSE)
	{
		dwTime = 0;
	}

	return dwTime;
}


void CPlayer::SetMallMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx)
{

	long  HeroTotalMallMoney= m_HeroInfo.MallMoney;

	long  ChangeMallMoney=ChangeValue;

	long  LastMallMoney= m_HeroInfo.MallMoney;

	if(nFlag==1)
	{
		if (HeroTotalMallMoney < ChangeValue)
		{
			g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", 
				RGB(0, 0, 255), "泡点扣减数据异常 角色ID[%d] 角色名[%s] 元宝数[%d]",
				GetID(), GetObjectName(), ChangeValue);
			m_HeroInfo.MallMoney = 0;
			return;
		}
		else
			LastMallMoney = HeroTotalMallMoney-ChangeValue;
	}
	if(nFlag==2)
	{

		LastMallMoney = HeroTotalMallMoney+ChangeValue;
	}

	if( LastMallMoney<0)
	{
		LastMallMoney=0;
	}

	m_HeroInfo.MallMoney=LastMallMoney;

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_MALL_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney = ChangeValue;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));
	CharacterHeroMallInfoUpdate(GetID(),ChangeValue,nFlag);
}
void CPlayer::UpdateMallMoney(DWORD DBMallMoney,DWORD ChangeMoney,BOOL bIsSend)
{
	m_HeroInfo.MallMoney = DBMallMoney;
	if(bIsSend)
	{
		MSG_MONEY msgMoney;
		msgMoney.Category = MP_ITEM;
		msgMoney.Protocol = MP_ITEM_MALL_MONEY;
		msgMoney.dwObjectID = GetID();
		msgMoney.dwTotalMoney = ChangeMoney;
		msgMoney.ItemIdx=0;
		msgMoney.bFlag = 2;
		SendMsg(&msgMoney, sizeof(msgMoney));
	}
}
void CPlayer::SetGoldMoney(MONEYTYPE ChangeValue, BYTE nFlag, WORD ItemIdx, WORD ItemDurability)
{
	long  HeroTotalGoldMoney= m_HeroInfo.GoldMoney;

	long  ChangeGoldMoney=ChangeValue;

	long  LastGoldMoney=m_HeroInfo.GoldMoney;

	if (nFlag == eShopUse || nFlag == eNoneItem || nFlag == eFbUse || nFlag == eDeleteBuf || nFlag == eFame || nFlag == eSinged || nFlag == eVimuWithGold || nFlag == eItemInherit)
	{
		if (HeroTotalGoldMoney < ChangeValue)
		{
			g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", 
				RGB(0, 0, 255), "元宝扣减数据异常 角色ID[%d] 角色名[%s] 元宝数[%d]",
				GetID(), GetObjectName(),ChangeValue);

			MSG_MONEY msgMoney;
			msgMoney.Category = MP_ITEM;
			msgMoney.Protocol = MP_ITEM_GOLD_MONEY;
			msgMoney.dwObjectID = GetID();
			msgMoney.dwTotalMoney = m_HeroInfo.GoldMoney;
			msgMoney.dwRealMoney = 0;
			msgMoney.ItemIdx = ItemIdx;
			msgMoney.bFlag = eNoneItem;
			SendMsg(&msgMoney, sizeof(msgMoney));

			CharacterHeroGoldInfoUpdate(GetID(), m_HeroInfo.GoldMoney, eShopUse);//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/27]

			m_HeroInfo.GoldMoney = 0;
			return;
		}
		else
			LastGoldMoney  = HeroTotalGoldMoney-ChangeGoldMoney;
	}
	if(nFlag==eGetGoldMoney)
	{       
		LastGoldMoney  = HeroTotalGoldMoney+ChangeGoldMoney;
	}
	if(LastGoldMoney<0)
	{
		LastGoldMoney  =0;
	}

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_GOLD_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney= ChangeValue;
	msgMoney.dwRealMoney = LastGoldMoney;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));
	CharacterHeroGoldInfoUpdate(GetID(),ChangeValue,nFlag);//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/27]
}

void CPlayer::UpdateGoldMoney(DWORD DBGoldMoney,DWORD ChangeMoney,BOOL bIsSend)//[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/27]
{
	m_HeroInfo.GoldMoney = DBGoldMoney;
	if(bIsSend)
	{
		MSG_MONEY msgMoney;
		msgMoney.Category = MP_ITEM;
		msgMoney.Protocol = MP_ITEM_GOLD_MONEY;
		msgMoney.dwObjectID = GetID();
		msgMoney.dwTotalMoney= ChangeMoney;
		msgMoney.dwRealMoney = m_HeroInfo.GoldMoney;
		msgMoney.ItemIdx=0;
		msgMoney.bFlag = 5;
		SendMsg(&msgMoney, sizeof(msgMoney));
	}
}
void CPlayer::GetDBGoldMoney()  //[在线充值元宝刷新][By:十里坡剑神][QQ:112582793][2017/11/27]
{
	char txt[64];
	sprintf(txt,"EXEC %s %d", "dbo.MP_CHARACTER_GetGoldMoney", GetID());
	g_DB.Query(eQueryType_FreeQuery,eGetDBGoldMoney,GetID(),txt );
}

void CPlayer::SetSwMoney(MONEYTYPE ChangeValue,BYTE nFlag,DWORD ItemIdx)
{
	m_HeroInfo.Fame = m_HeroInfo.Fame -ChangeValue;

	if(m_HeroInfo.Fame<0)
	{
		m_HeroInfo.Fame=0;
	}

	MSG_MONEY msgMoney;
	msgMoney.Category = MP_ITEM;
	msgMoney.Protocol = MP_ITEM_SW_MONEY;
	msgMoney.dwObjectID = GetID();
	msgMoney.dwTotalMoney = ChangeValue;
	msgMoney.ItemIdx=ItemIdx;
	msgMoney.bFlag = nFlag;
	SendMsg(&msgMoney, sizeof(msgMoney));

	CharacterHeroInfoUpdate(this);
}

void CPlayer::SetReSet(DWORD Val,WORD Type)
{
	m_HeroInfo.ReSet = Val;

	if(m_HeroInfo.ReSet<0)
	{
		m_HeroInfo.ReSet=0;
	}

	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHANGELIFE_ACK;
	msg.dwObjectID=GetID();
	msg.dwData1=Val;
	msg.dwData2=Type;
	SendMsg(&msg, sizeof(msg));

	CharacterHeroInfoUpdate(this);
}




void CPlayer::SetReSetN(DWORD Val,WORD Type)
{

	MSG_DWORD2 msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_CHANGELIFE_NACK;
	msg.dwObjectID=GetID();
	msg.dwData1=Val;
	msg.dwData2=Type;
	SendMsg(&msg, sizeof(msg));

}

void CPlayer::SetMarryName(char* pName)
{
	if (pName == NULL)
		return;

	SafeStrCpy(m_HeroInfo.MarryName, pName, MAX_NAME_LENGTH + 1 );

	SafeStrCpy(m_HeroCharacterInfo.MarryName,pName,MAX_NAME_LENGTH + 1);

	CharacterHeroInfoUpdate(this);
}
char* CPlayer::GetMarryName()
{
	return m_HeroInfo.MarryName;
}
bool CPlayer::IsMarry()
{
	return strcmp("0", GetMarryName()) != 0;
}

void CPlayer::MallMoneyProcess()
{
	BOOL bIsGetMall = FALSE;
	if(g_pServerSystem->GetGameConfig().PaoDianReSet==0)
	{
		if(GetLevel() >= g_pServerSystem->GetGameConfig().PaoDianLevel)
			bIsGetMall=TRUE;
	}
	else if(GetReSet() >=g_pServerSystem->GetGameConfig().PaoDianReSet)
		bIsGetMall=TRUE;
	if (bIsGetMall)
	{
	
		if( GetState() != eObjectState_Die )
		{
			m_LastPaoDianTime -= g_pServerSystem->GetMainProcessTime();

			if (m_LastPaoDianTime <= 0)
			{
				DWORD MallMoney = 0;
				if(g_MapNum==17)
					MallMoney=g_pServerSystem->GetGameConfig().PaoDianMap_LZ;
				else
					MallMoney=g_pServerSystem->GetGameConfig().PaoDianMap;
				//if(m_bIsRobot)
				//	MallMoney *=0.3;
				SetMallMoney(MallMoney,2,0);
				m_LastPaoDianTime = 60 * g_pServerSystem->GetGameConfig().PaoDianTime * 1000;
			}
		}
	}
	if(GAMERESRCMNGR->IsFBMap(GetCurMapNum())&&!GetShopItemManager()->IsCanInPutFBMap())
	{//副本地图此处检查是否符合进入条件，防止遁甲飞进去	by:十里坡剑神	QQ:112582793
		if(gCurTime-m_dwMoveMapTime>=5000)
		{
			MSGBASE msg;
			msg.Category = MP_MOVE;
			msg.Protocol = MP_CHAR_FB_ACK;
			msg.dwObjectID = GetID();
			SendMsg(&msg,sizeof(msg));
			m_dwMoveMapTime = gCurTime;
		}
	}
}

//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
void CPlayer::AddItemGrowKillMonster(CMonster * Monster)
{
	if (NULL==Monster||(this->GetLevel())-(Monster->GetLevel())>6) return;

	if (m_HeroInfo.KillMonsterTimes>=100)
	{
		ITEMBASE * pTargetItem = (ITEMBASE *)ITEMMGR->GetItemInfoAbsIn(this, 81);
		if (pTargetItem)
		{
			if(pTargetItem->ItemGrow>=250000) 
			{
				m_HeroInfo.KillMonsterTimes=0;
				return;
			}
			else
			{
				m_ItemGrowCount = m_HeroInfo.KillMonsterTimes;
				WORD GrowRrot = 0;
				if(m_ShopItemManager.IsUseGrowItem(GrowRrot))
					m_ItemGrowCount *= GrowRrot;
				ItemGrowUpdate(this->GetID(),pTargetItem->dwDBIdx,m_ItemGrowCount); 
			}
		}
		m_HeroInfo.KillMonsterTimes=0;
	}
	else
		m_HeroInfo.KillMonsterTimes +=1;
	CharacterHeroInfoUpdate(this);
}
void CPlayer::AutoAddPoint(WORD val)
{
	switch(m_AutoAddPointFlag)
	{
	case eAdd_GenGol:
		AddGenGol(val);
		break;
	case eAdd_MinChub:
		AddMinChub(val);
		break;
	case eAdd_CheRyuk:
		AddCheRyuk(val);
		break;
	case eAdd_SimMek:
		AddSimMek(val);
		break;
	}
}

void CPlayer::SetAddPoint(BOOL val)
{
	m_IsAutoAddPoint = val;
}
void CPlayer::SetAddPointFlag(BYTE flag)
{
	m_AutoAddPointFlag=flag;
	if(flag!=0)
	{
		if(m_HeroInfo.LevelUpPoint>0)
		{
			SetPlayerLevelUpPoint(m_HeroInfo.LevelUpPoint);
			SetPlayerLevelUpPoint(0);
		}
	}
}

void  CPlayer::AddKillTimes()
{
	m_HeroInfo.dwKillPlayerTimes+=1;
	CharacterHeroInfoUpdate(this);
}
void CPlayer::ClearKillTimes()
{ 
	m_HeroInfo.dwKillPlayerTimes = 0; 
	CharacterHeroInfoUpdate(this);
}
void CPlayer::GetItemtotalInfo(ITEMBASE Inventory[])
{
	m_ItemContainer.GetSlot(eItemTable_Inventory)->GetItemInfoAll(Inventory);
}
void CPlayer::MoveInventPack(bool &bRet)
{//[背包整理获取物品][By:十里坡剑神][QQ:112582793][2018/6/11]
	m_ItemContainer.GetSlot(eItemTable_Inventory)->MovePack(GetID(), bRet);
}
void CPlayer::SetItemTotalInfo(ITEMBASE * pItemBaseAll)
{//[设置角色物品信息][By:十里坡剑神][QQ:112582793][2018/6/11]
	m_ItemContainer.GetSlot(eItemTable_Inventory)->SetItemInfoAll(pItemBaseAll);
}
DWORD CPlayer::IsCanMovePack()
{//[背包整理时间查询][By:十里坡剑神][QQ:112582793][2018/6/11]

	if (LOOTINGMGR->IsLootedPlayer(GetID()))
		return eEXITCODE_LOOTING;

	if (GetState() == eObjectState_Exchange)
		return eEXITCODE_NOT_ALLOW_STATE;

	if (GetState() == eObjectState_StreetStall_Owner ||
		GetState() == eObjectState_StreetStall_Guest)
		return eEXITCODE_NOT_ALLOW_STATE;
	if (gCurTime - m_MovePackTime < 5000)
		return 1;

	return 0;
}
//void CPlayer::SetLastSkillAnimationTime(DWORD dwTime)
//{
//	m_dwLastSkillAnimationTime = dwTime;
//}
