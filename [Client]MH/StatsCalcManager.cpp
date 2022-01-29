// StatsCalcManager.cpp: implementation of the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatsCalcManager.h"
#include "..\[CC]Header\GameResourceManager.h"

#ifdef _MHCLIENT_
#include "Hero.h"
#include "GameIn.h"
#include "ObjectManager.h"
#endif

#ifdef _MAPSERVER_
#include "Player.h"
#endif

#include "CharacterCalcManager.h"
#include "ItemManager.h"
#include "MussangManager.h"
#include "PetManager.h"

#include "TitanManager.h"

#include "TitanInventoryDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CStatsCalcManager::CStatsCalcManager()
{
}

CStatsCalcManager::~CStatsCalcManager()
{

}

void CStatsCalcManager::Clear(player_calc_stats * pStats)
{
	memset(pStats, 0, sizeof(player_calc_stats));
}

//#ifdef _JAPAN_LOCAL_
//float ItemApplyRate[] = 
//{
//		1,
//		0.5f,
//		0.4f,
//		0.3f,
//		0.2f,
//		0.1f,
//		0.0f,
//};
//
//#else

float ItemApplyRate[] = 
{
		1,
		0.6f,
		0.55f,
		0.5f,
		0.45f,
		0.4f,
		0.35f,
		0.30f,
		0.3f,

		0.25f,
		0.2f,
		0.15f,
		0.1f,
		0.05f,

		0.f,
};

//#endif

void CStatsCalcManager::CalcItemStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * item_stats = pPlayer->GetItemStats();
	Clear(item_stats);

	ASSERT(pPlayer->GetMaxLevel() >= pPlayer->GetLevel());
	
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		if(pTargetItemBase == NULL)
			continue;

		if(pTargetItemBase->dwDBIdx == 0)
			continue;

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if(pItemInfo == NULL)
			continue;

		float ApplyRate = 1.f;
		int gap = pItemInfo->LimitLevel - pPlayer->GetLevel();
		if(gap > 0)
		{
			if( pPlayer->GetShopItemStats()->EquipLevelFree )
				gap -= pPlayer->GetShopItemStats()->EquipLevelFree;
			

			if(gap < 0 )	gap = 0;
//#ifdef _JAPAN_LOCAL_			
//			if(gap > 6)		gap = 6;
//#else
			if(gap > 13)	gap = 13;
//#endif 
			ApplyRate = ItemApplyRate[gap];
		}

		if(pItemInfo->ItemKind == eEQUIP_ITEM_ARMLET)
		{//[护手][By:十里坡剑神][QQ:112582793][2018/2/28]
			item_stats->MeleeAttackPowerAddRateMin += pItemInfo->MeleeAttackMin*ApplyRate * 0.01f;
			item_stats->MeleeAttackPowerAddRateMax += pItemInfo->MeleeAttackMax*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMin += pItemInfo->RangeAttackMin*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMax += pItemInfo->RangeAttackMax*ApplyRate * 0.01f;
		}
		else
		{
			item_stats->MeleeAttackPowerMin += (WORD)(pItemInfo->MeleeAttackMin*ApplyRate);		
			item_stats->MeleeAttackPowerMax += (WORD)(pItemInfo->MeleeAttackMax*ApplyRate);
			item_stats->RangeAttackPowerMin += (WORD)(pItemInfo->RangeAttackMin*ApplyRate);
			item_stats->RangeAttackPowerMax += (WORD)(pItemInfo->RangeAttackMax*ApplyRate);
		}

		item_stats->PhysicalDefense += (WORD)(pItemInfo->PhyDef*ApplyRate);
		item_stats->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,ApplyRate);
		item_stats->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,ApplyRate);
		item_stats->GenGol += (WORD)(pItemInfo->GenGol*ApplyRate);
		item_stats->MinChub += (WORD)(pItemInfo->MinChub*ApplyRate);
		item_stats->CheRyuk += (WORD)(pItemInfo->CheRyuk*ApplyRate);
		item_stats->SimMaek += (WORD)(pItemInfo->SimMek*ApplyRate);
		item_stats->MaxLife += (DWORD)(pItemInfo->Life*ApplyRate);
		item_stats->MaxShield += (DWORD)(pItemInfo->Shield*ApplyRate);
		item_stats->MaxNaeRyuk += (DWORD)(pItemInfo->NaeRyuk*ApplyRate);
		
		if(ITEMMGR->IsOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->Durability))
		{
#ifdef _MHCLIENT_
			ITEM_OPTION_INFO * pOptionInfo = ITEMMGR->GetItemOption(pTargetItemBase->Durability);
#else
			ITEM_OPTION_INFO * pOptionInfo = pPlayer->GetItemOption(pTargetItemBase->Durability);
#endif
			if(pOptionInfo)
			{
				float rate = ApplyRate;
				if( pPlayer->GetShopItemStats()->ReinforceAmp )
					rate += (pPlayer->GetShopItemStats()->ReinforceAmp/100.f);

				item_stats->MeleeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);



				item_stats->PhysicalDefense += (WORD)(pOptionInfo->PhyDefense*rate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pOptionInfo->AttrAttack,rate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pOptionInfo->AttrRegist,rate);
				item_stats->GenGol += (WORD)(pOptionInfo->GenGol*rate);
				item_stats->MinChub += (WORD)(pOptionInfo->MinChub*rate);
				item_stats->CheRyuk += (WORD)(pOptionInfo->CheRyuk*rate);
				item_stats->SimMaek += (WORD)(pOptionInfo->SimMek*rate);
				item_stats->MaxLife += (DWORD)(pOptionInfo->Life*rate);
				item_stats->MaxShield += (DWORD)(pOptionInfo->Shield*rate);
				item_stats->MaxNaeRyuk += (DWORD)(pOptionInfo->NaeRyuk*rate);
				item_stats->Critical += (DWORD)(pOptionInfo->CriticalPercent*rate);
			}
		}
		if(ITEMMGR->IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))
		{
#ifdef _MHCLIENT_
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = ITEMMGR->GetItemRareOption(pTargetItemBase->RareIdx);
#else
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = pPlayer->GetItemRareOption(pTargetItemBase->RareIdx);
#endif
			if(pRareOptionInfo)
			{
				item_stats->MeleeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);

				item_stats->PhysicalDefense += (WORD)(pRareOptionInfo->PhyDefense*ApplyRate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pRareOptionInfo->AttrAttack,ApplyRate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pRareOptionInfo->AttrRegist,ApplyRate);
				item_stats->GenGol += (WORD)(pRareOptionInfo->GenGol*ApplyRate);
				item_stats->MinChub += (WORD)(pRareOptionInfo->MinChub*ApplyRate);
				item_stats->CheRyuk += (WORD)(pRareOptionInfo->CheRyuk*ApplyRate);
				item_stats->SimMaek += (WORD)(pRareOptionInfo->SimMek*ApplyRate);
				item_stats->MaxLife += (DWORD)(pRareOptionInfo->Life*ApplyRate);
				item_stats->MaxShield += (DWORD)(pRareOptionInfo->Shield*ApplyRate);
				item_stats->MaxNaeRyuk += (DWORD)(pRareOptionInfo->NaeRyuk*ApplyRate);
			}
		}
	}

	item_stats->AttributeResist.CheckLimit(1);	

	if(MUSSANGMGR->IsMussangMode())
	{
		WORD stat = MUSSANGMGR->GetMussangStat();

		item_stats->GenGol += stat;
		item_stats->MinChub += stat;
		item_stats->CheRyuk += stat;
		item_stats->SimMaek += stat;
	}

	PETMGR->AddMasterStatFromPetBuff(item_stats);
	CalcSetItemStats();
	CalcUniqueItemStats();
	CalcCharStats(pPlayer);
}

void CStatsCalcManager::CalcCharStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * char_stats = pPlayer->GetCharStats();
	Clear(char_stats);

	HERO_TOTALINFO pHeroInfo;
	pPlayer->GetHeroTotalInfo(&pHeroInfo);

	char_stats->MinChub = pHeroInfo.wMinChub;
	char_stats->GenGol = pHeroInfo.wGenGol;


//#ifdef _JAPAN_LOCAL_
//	char_stats->PhysicalDefense = pPlayer->GetCheRyuk() * 10 / 15;
//
//	char_stats->Critical = ( pPlayer->GetGenGol() / 4 ) + ( pPlayer->GetMinChub() / 6 );
//
//#else
	char_stats->PhysicalDefense = (WORD)(((pPlayer->GetCheRyuk())/1.5)*0.6);
	char_stats->Critical = (pPlayer->GetGenGol()) / 4;
	char_stats->Decisive = (pPlayer->GetSimMek()) / 4;
//#endif

#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
	CCharacterCalcManager::CalcMaxShield(pPlayer);
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}

void CStatsCalcManager::CalcSetItemStats()
{
	HERO->ClearSetitemOption();
	
	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];

	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);

	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(HERO, part);

		if(pTargetItemBase == NULL)
		{
			continue;
		}

		if(pTargetItemBase->dwDBIdx == 0)
		{
			continue;
		}

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if(pItemInfo == NULL)
			continue;

		if(pItemInfo->wSetItemKind != 0)
		{
			for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
			{
				if(setItemInfo[ui].wKind == pItemInfo->wSetItemKind)

				{
					setItemInfo[ui].wCount++;
					break;
				}
				else
				{
					if(setItemInfo[ui].wKind == 0)
					{
						setItemInfo[ui].wKind = pItemInfo->wSetItemKind;
						setItemInfo[ui].wCount++;
						break;
					}					
				}				
			}
		}		
	}

	CYHHashTable<SET_ITEM_OPTION>* setItemOptionList = ITEMMGR->GetSetItemOptionList();

	for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
	{
		if(setItemInfo[ui].wKind != 0)
		{			
			setItemOptionList->SetPositionHead();

			SET_ITEM_OPTION* pSetItemOption = NULL;
			while(pSetItemOption = setItemOptionList->GetData())
			{			
				if(pSetItemOption->wSetItemKind == setItemInfo[ui].wKind)
				{
					WORD wSetValue = pSetItemOption->wSetValue;					

					if(setItemInfo[ui].wCount >= wSetValue)
					{
						HERO->AddSetitemOption( pSetItemOption );
					}					
				}				
			}			
		}

	}
}

void CStatsCalcManager::CalcUniqueItemStats()
{
	HERO->ClearUniqueItemOption();

	for( POSTYPE pos = TP_WEAR_START; pos < TP_WEAR_END; pos++ )
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn(HERO, pos);
		if( !pItemBase )
		{
			continue;
		}

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if(!pInfo)
		{
			continue;
		}

		if( pInfo->ItemKind == eEQUIP_ITEM_UNIQUE )
		{
			UNIQUE_ITEM_OPTION_INFO* pUniqueItemInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pInfo->ItemIdx);
			if( !pUniqueItemInfo )
			{
				continue;
			}

			HERO->AddUniqueItemOption(pUniqueItemInfo);
		}
	}
}

void CStatsCalcManager::CalcTitanStats(DWORD dwDBIdx)
{
	CItem* pItem = ITEMMGR->GetItem(dwDBIdx);

	if(!pItem)
	{
		return;
	}

	titan_calc_stats* pTitanStats = TITANMGR->GetTitanStats();
	ZeroMemory(pTitanStats, sizeof(titan_calc_stats));

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
	BASE_TITAN_LIST* pTitanBaseInfo = GAMERESRCMNGR->GetTitanListInfo(pInfo->Plus_MugongIdx);	// 鸥捞藕 府胶飘

	pTitanStats->MaxFuel				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MaxFuel;
	pTitanStats->MaxSpell				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MaxSpell;
	pTitanStats->PhysicalDefense		= (WORD)pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].PhyDef;
	pTitanStats->Critical				= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].Critical;
	pTitanStats->MeleeAttackPowerMin	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MeleeAttackPowerMin;
	pTitanStats->MeleeAttackPowerMax	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].MeleeAttackPowerMax;
	pTitanStats->RangeAttackPowerMin	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].RangeAttackPowerMin;
	pTitanStats->RangeAttackPowerMax	= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].RangeAttackPowerMax;
	pTitanStats->AttackRange			= pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttackRange;
	pTitanStats->MoveSpeed				= (float)pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].Speed;
	pTitanStats->AttributeAttack.AddATTRIBUTE_VAL(pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttAtk, ATTR_FIRE);
	pTitanStats->AttributeResist.AddATTRIBUTE_VAL(pTitanBaseInfo->GradeInfo[pInfo->Plus_Value - 1].AttReg, ATTR_FIRE);

	for( POSTYPE pos = TP_TITANWEAR_START; pos < TP_TITANWEAR_END; pos++ )
	{
		CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_Titan, pos );
		if(!pItem)
		{
			continue;
		}

		TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = NULL;
		pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
		if(NULL == pEnduranceInfo || pEnduranceInfo->Endurance == 0)
			continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(NULL == pItemInfo)
			continue;

		pTitanStats->MaxFuel				+= pItemInfo->Life;
		pTitanStats->MaxSpell				+= pItemInfo->NaeRyuk;
		pTitanStats->PhysicalDefense		+= pItemInfo->PhyDef;
		pTitanStats->Critical				+= pItemInfo->CriticalPercent;
		pTitanStats->MeleeAttackPowerMin	+= pItemInfo->MeleeAttackMin;
		pTitanStats->MeleeAttackPowerMax	+= pItemInfo->MeleeAttackMax;
		pTitanStats->RangeAttackPowerMin	+= pItemInfo->RangeAttackMin;
		pTitanStats->RangeAttackPowerMax	+= pItemInfo->RangeAttackMax;
		pTitanStats->AttackRange			+= pItemInfo->MugongNum;
		pTitanStats->MoveSpeed				+= pItemInfo->AllPlus_Value;

		pTitanStats->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack, ATTR_FIRE);
		pTitanStats->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist, ATTR_FIRE);
	}

	GAMEIN->GetTitanInventoryDlg()->SetTitanInvenInfo();
}
//////////////////////////////////////////////////////////////////////////




