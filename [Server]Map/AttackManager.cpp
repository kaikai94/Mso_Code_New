// AttackManager.cpp: implementation of the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackManager.h"
#include "Network.h"
#include "PackedData.h"
#include "ServerSystem.h"
#include "CharMove.h"
#include "RegenNPC.h"
#include "UserTable.h"
#include "ObjectStateManager.h"
#include "PartyManager.h"
#include "Party.h"
#include "SiegeWarMgr.h"
#include "Player.h"
#include "PetManager.h"
#include "GuildManager.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "ItemManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SHIELD_COMBO_DAMAGE			0.5f
#define SHIELD_OUT_MUGONG_DAMAGE	0.7f
#define SHIELD_IN_MUGONG_DAMAGE		0.7f
extern float g_fAttribPower;
CAttackManager::CAttackManager()
{
	m_nDamageRate = 100.0f;
}

CAttackManager::~CAttackManager()
{

}

void CAttackManager::sendDieMsg(CObject * pAttacker,CObject* pTarget)
{
	MSG_DWORD2 m2c;
	m2c.Category = MP_USERCONN;


	if(pTarget->GetObjectKind() & eObjectKind_Monster)
		m2c.Protocol = MP_USERCONN_MONSTER_DIE;
	else if(pTarget->GetObjectKind() == eObjectKind_Player)
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

	m2c.dwObjectID = pAttacker->GetID();
	m2c.dwData1 = pAttacker->GetID();
	m2c.dwData2 = pTarget->GetID();

	PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
}

//[物理伤害计算][By:十里坡剑神][QQ:112582793][2017/12/7]
DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
										RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical);

	if(pDamageInfo->bCritical)
	{
		attackPhyDamage += attackPhyDamage*gEventRateFile[eEvent_PhyAttack];

		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}
		if ((pAttacker->GetObjectKind() & eObjectKind_Player) && pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmgPlayer, &attackPhyDamage);
		}
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}
	}

	attackPhyDamage += AmplifiedPower;

	attackPhyDamage *= fDecreaseDamageRate;

	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
			attackPhyDamage *= gEventRate[eEvent_PVPD];


//#ifdef _JAPAN_LOCAL_
//
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE));
//	attackPhyDamage -= ShieldDamage;
//#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
//#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);

	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)	
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	float fdam = 0.0f;
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage*0.01f+1.0f);

		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );

		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);

		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
			attackPhyDamage *= (fdam+1.0f);
		}
		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		if(attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
			attackPhyDamage *= (1.0f-fdam);
		}
	}

CalcEnd:
	return (DWORD)attackPhyDamage;
}

//[武功物理伤害计算][By:十里坡剑神][QQ:112582793][2017/12/7]
DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
										 RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical );

	if(pDamageInfo->bCritical)
	{
		attackPhyDamage += attackPhyDamage*gEventRateFile[eEvent_PhyAttack];

		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}
		if ((pAttacker->GetObjectKind() & eObjectKind_Player) && pTargetObject->GetObjectKind() == eObjectKind_Player)
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmgPlayer, &attackPhyDamage);
		}
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}
	}

	attackPhyDamage += AmplifiedPower;

	attackPhyDamage *= fDecreaseDamageRate;

	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
			attackPhyDamage *= gEventRate[eEvent_PVPD];


//#ifdef _JAPAN_LOCAL_
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
//	attackPhyDamage -= ShieldDamage;
//#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
//#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);

	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;


	float fdam = 0.0f;
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage*0.01f) + 
			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage*0.01f);
		attackPhyDamage *= (fdam+1.0f);

		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );

		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);

		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			{
				fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
				attackPhyDamage *= (fdam+1.0f);
			}
		}
		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		if(attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )

		{
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			{
				fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
				attackPhyDamage *= (1.0f-fdam);
			}
		}
	}

	return (DWORD)attackPhyDamage;
}
//[属性攻击伤害计算][By:十里坡剑神][QQ:112582793][2017/12/7]
DWORD CAttackManager::GetMugongAttrDamage(CObject* pAttacker,CObject* pTargetObject,
										  WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
										  float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate )
{

	pDamageInfo->bDecisive = m_ATTACKCALC.getDecisive(pAttacker,pTargetObject,fCriticalRate);

	double attackAttrDamage = m_ATTACKCALC.getAttributeAttackPower(pAttacker,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
	attackAttrDamage *= fDecreaseDamageRate;

	if(pDamageInfo->bDecisive)
	{
		attackAttrDamage += attackAttrDamage*gEventRateFile[eEvent_Attrib];

		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackAttrDamage);
		}

		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackAttrDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackAttrDamage < 0.f)
				attackAttrDamage = 1.f;
		}
	}
	
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
	{
		attackAttrDamage *= gEventRate[eEvent_PVPA];
	}


//#ifdef _JAPAN_LOCAL_
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE));
//	attackAttrDamage -= ShieldDamage;
//#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE), ShieldDamage);
	attackAttrDamage -= ReduceDamage;
//#endif

	pDamageInfo->ShieldDamage += ShieldDamage;	

//#ifdef _JAPAN_LOCAL_
//	float RegVal = ( pTargetObject->GetAttDefense(Attrib) * 3.f ) / ( attackAttrDamage * 2.f );	
//
//
//	if( RegVal > 0.99f )		RegVal = 0.99f;
//	else if( RegVal < 0.f )		RegVal = 0.f;
//
//#else
	float RegVal = pTargetObject->GetAttDefense(Attrib);

	if(pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		float val = 1 + ((CPlayer*)pTargetObject)->GetSkillStatsOption()->AttDef;

		if( val < 0 )
			val = 0.0f;

		RegVal = RegVal * val;
	}

	if( RegVal >= gEventRateFile[eEvent_RegDefence] )
		RegVal = gEventRateFile[eEvent_RegDefence];
	if( RegVal < 0 )		RegVal = 0;

	//RegVal *= 0.7f;
//#endif
	double resAttrDamage = (attackAttrDamage * (1 - RegVal) );

	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	float fdam = 0.0f;
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage*0.01f)+
			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage*0.01f);
		resAttrDamage = (resAttrDamage*(fdam+1.0f));

		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage );

		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);

		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown*0.01f;
			resAttrDamage = (resAttrDamage*(fdam+1.0f));
		}

		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
			resAttrDamage = (resAttrDamage*(1.0f-fdam));
		}
	}

CalcEnd:	

	return (DWORD)resAttrDamage;
}

void CAttackManager::Attack(BOOL bMugong, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
							float PhyAttackRate,
							WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
							float fCriticalRate,
							RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
							WORD AmplifiedPowerAttrib, BOOL bContinueAttack )
{	
	pDamageInfo->Clear();

	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{// [伤害闪避] [by:十里坡剑神][QQ:112582793][2018/10/24]
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
		if( ((CPlayer*)pTarget)->GetUserLevel() <= eUSERLEVEL_GM && ((CPlayer*)pTarget)->IsVisible() == FALSE )
		{
			return;
		}
	}

	if(( pAttacker->GetObjectKind() == eObjectKind_Player ) && ( pTarget->GetObjectKind() & eObjectKind_Monster ))
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		CMonster* pMonster = (CMonster*)pTarget;

		if( pPlayer->GetWeaponEquipType() == WP_EVENT || pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			if(bMugong)
				return;

			if(bContinueAttack)
				return;
		}
		else if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG 
			)
			return;
	}

	if(pTarget->GetInited() == FALSE)
		return;

	DWORD AttrDamage =0,PhyDamage =0;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}

	if(PhyAttackRate > 0.000001f)
	{
		if(bMugong)
		{
			PhyDamage = GetMugongPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
		else
		{
			PhyDamage = GetComboPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
	}

	if(AttAttackMax != 0)
	{
		AttrDamage = GetMugongAttrDamage(pAttacker,pTarget,Attrib,AttAttackMin,AttAttackMax,AttAttackRate,fCriticalRate,pDamageInfo,fDecreaseDamageRate );
	}
	if (pTarget->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &PhyDamage);
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceDemageRate, &AttrDamage);
	}
//#ifdef _JAPAN_LOCAL_
//	if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
//	{
//		if( ((CPlayer*)pTarget)->WhatIsAttrRelation(Attrib) == eCAR_Weak_Opposite )
//		{
//			PhyDamage = PhyDamage + PhyDamage / 2;
//		}
//		if( ((CPlayer*)pTarget)->WhatIsAttrRelation(AmplifiedPowerAttrib) == eCAR_Weak_Opposite )
//		{
//			AttrDamage = AttrDamage + AttrDamage /2;
//		}
//	}		
//
//#endif

	pAttacker->CalcRealAttack(pTarget, PhyDamage, AttrDamage, pDamageInfo, bContinueAttack );

	DWORD UpAttackDamage = 0;
	if( PhyDamage + AttrDamage < pDamageInfo->RealDamage )
		UpAttackDamage = ( pDamageInfo->RealDamage - ( PhyDamage + AttrDamage ) ) / 2;

	//float fsiegedamage = 1.f;
	//fsiegedamage = 0.5f;

	//if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
	//{//[攻城战攻击力计算][BY:十里坡剑神][QQ:112582793][2019-9-4][16:04:36]
	//	if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
	//	{
	//		if( PhyDamage )
	//			PhyDamage = (DWORD)(PhyDamage*fsiegedamage + 1);
	//		if( AttrDamage )
	//			AttrDamage = (DWORD)(AttrDamage*fsiegedamage + 1);
	//		if( UpAttackDamage )

	//			UpAttackDamage = (DWORD)(UpAttackDamage*fsiegedamage + 1);
	//		if( pDamageInfo->RealDamage )
	//			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage*fsiegedamage + 1);
	//		if( pDamageInfo->ShieldDamage )
	//			pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage*fsiegedamage + 1);
	//	}
	//}

	pTarget->CalcRealDamage(pAttacker,PhyDamage+UpAttackDamage,AttrDamage+UpAttackDamage,pDamageInfo);

	if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pDamageInfo->RealDamage > 1000000 )
	{
		char buf[256] = { 0, };
		sprintf( buf, "[DAMAGE] Name: %s, bCritical: %d, Damage: %d, ShieldDamage : %d, PhyDamage: %d, AttrDamage : %d, UpAttackDamage : %d ", 
			pAttacker->GetObjectName(), pDamageInfo->bCritical, pDamageInfo->RealDamage, pDamageInfo->ShieldDamage, 
			PhyDamage, AttrDamage, UpAttackDamage );
		ASSERTMSG( 0, buf );
		return;
	}

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
//#ifdef _JAPAN_LOCAL_
//
//		if( pAttackPlayer->GetPartyIdx() )
//		{
//			CParty* pParty = PARTYMGR->GetParty( pAttackPlayer->GetPartyIdx() );
//
//			if( pParty )
//				if( pParty->IsHelpPartyMember( pAttackPlayer->GetID() ) )
//				{
//					pDamageInfo->RealDamage = pDamageInfo->RealDamage + pDamageInfo->RealDamage / 2;
//				}
//		}
//
//#endif

		if( pTarget->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pTargetPlayer = (CPlayer*)pTarget;

			if( pAttackPlayer->IsPKMode() )
			{
				if( AttrDamage || PhyDamage )
					if( pTargetPlayer->IsPKMode() == FALSE )
						pAttackPlayer->SetPKStartTimeReset();
			}

			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
				pTargetPlayer->IncreaseEventHammerCount();
			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT )
			{
				pTargetPlayer->SummonEventPetRndm();
			}
		}
	}

	if(bCounter)
		pTarget->CalcCounterDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo);
	else
		pDamageInfo->CounterDamage = 0;

	pTarget->CalcReverseVampiric(pDamageInfo);

	pAttacker->CalcVampiric(pDamageInfo);

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pAttacker)->IsMussangMode())
			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * 1.05);
	}

	float fRate = m_nDamageRate * 0.01f;

	if(pDamageInfo->RealDamage > 0 && ((pDamageInfo->RealDamage * fRate)*10) <= 10)
		pDamageInfo->RealDamage = 1;
	else
		pDamageInfo->RealDamage *= (DWORD)fRate;

	DWORD dwRealDemage = pDamageInfo->RealDamage;

	pDamageInfo->RealDamage = GetPenaltyDemege(pAttacker, pTarget, dwRealDemage);	

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		if( pPlayer->InTitan() )
		{
			pPlayer->GetTitanManager()->DoRandomDecrease(AtAtk);
		}
	}
	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

	if(pDamageInfo->CounterDamage != 0)
	{
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
			ATTACKMGR->sendDieMsg(pTarget,pAttacker);
			pAttacker->Die(pTarget);
		}
	}

	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

void CAttackManager::RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverLifeVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
//#ifdef _JAPAN_LOCAL_
//	pObject->AddLife(RecoverLifeVal,&realAddVal,FALSE);
//#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverLifeVal * (1 + fPlus));
	pObject->AddLife(val,&realAddVal,FALSE);
//#endif
	pResultInfo->HealLife = (WORD)realAddVal;
}
void CAttackManager::RecoverNaeRyuk(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverNaeRyukVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
//#ifdef _JAPAN_LOCAL_
//	pObject->AddNaeRyuk(RecoverNaeRyukVal,&realAddVal);
//#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverNaeRyukVal * (1 + fPlus));
	pObject->AddNaeRyuk(val,&realAddVal);
//#endif
	pResultInfo->RechargeNaeryuk = (WORD)realAddVal;
}


void CAttackManager::AttackAbs( CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo )
{

	if( nAbsKind == 0 ) return;

	pDamageInfo->Clear();

	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}
	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}


	if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pAttacker)->IsPKMode() )
		{
			if( pTarget->GetObjectKind() == eObjectKind_Player )
				if( AttackRate > 0.0f  )
					if( ((CPlayer*)pTarget)->IsPKMode() == FALSE )
						((CPlayer*)pAttacker)->SetPKStartTimeReset();
		}

		DWORD TargetLife = pTarget->GetLife();
		DWORD MinusLife = 0;
		DWORD TargetShield = pTarget->GetShield();
		DWORD MinusShield = 0;

		if( nAbsKind & eAAK_LIFE )
		{
			MinusLife = (DWORD)(TargetLife * AttackRate);
		}
		else if( nAbsKind & eAAK_SHIELD )
		{
			MinusShield = (DWORD)(TargetShield * AttackRate);
		}

		pDamageInfo->RealDamage = MinusLife;
		pDamageInfo->ShieldDamage = MinusShield;

		DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

		if(newLife == 0)
		{
			ATTACKMGR->sendDieMsg(pAttacker,pTarget);
			pTarget->Die(pAttacker);
		}
}



void CAttackManager::AttackJinbub( CObject* pAttacker, CObject* pTarget, DWORD AttackPower, 
								  DWORD AttackMin,DWORD AttackMax, RESULTINFO* pDamageInfo,
								  float fDecreaseDamageRate )
{
	pDamageInfo->Clear();

	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}


	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}

	if( AttackPower == 0 )
		return;

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pAttacker)->IsPKMode() )
		{
			if( pTarget )
				if( pTarget->GetObjectKind() == eObjectKind_Player )
					if( ((CPlayer*)pTarget)->IsPKMode() == FALSE )
						((CPlayer*)pAttacker)->SetPKStartTimeReset();
		}

		if( AttackMax < AttackMin )
			AttackMin = AttackMax;

		DWORD PlusAttack = random( AttackMin, AttackMax );

		DWORD RealAttack = GetJinbubDamage( pAttacker, pTarget, AttackPower+PlusAttack,
			pDamageInfo, fDecreaseDamageRate );


		float fsiegedamage = 0.2f;
//#ifdef _HK_LOCAL_
		fsiegedamage = 0.5f;
		if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
		{
			if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
			{
				if( RealAttack )
					RealAttack = (WORD)(RealAttack*fsiegedamage + 1);
				if( pDamageInfo->RealDamage )
					pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage*fsiegedamage + 1);
				if( pDamageInfo->ShieldDamage )

					pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage*fsiegedamage + 1);
			}
		}

		pTarget->CalcRealDamage( pAttacker, (WORD)RealAttack, 0, pDamageInfo );

		pTarget->CalcReverseVampiric(pDamageInfo);

		DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

		if(newLife == 0)
		{
			ATTACKMGR->sendDieMsg(pAttacker,pTarget);
			pTarget->Die(pAttacker);
		}
}

DWORD CAttackManager::GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
									  RESULTINFO* pDamageInfo,float fDecreaseDamageRate)
{
	DWORD attackPhyDamage = (DWORD)(AttackPower * fDecreaseDamageRate);

	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
//#ifdef _JAPAN_LOCAL_
//		attackPhyDamage = (DWORD)(attackPhyDamage*0.25f);
//#else
		attackPhyDamage = (DWORD)(attackPhyDamage*0.5f);
//#endif

//#ifdef _JAPAN_LOCAL_
//	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
//	attackPhyDamage -= ShieldDamage;
//#else
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
//#endif

	pDamageInfo->ShieldDamage += (DWORD)ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage = (DWORD)(attackPhyDamage*(1-defencePhyLevel));

	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage = (DWORD)( attackPhyDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage = (DWORD)(attackPhyDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	return attackPhyDamage;
}

DWORD CAttackManager::GetPenaltyDemege(CObject* pAttacker,CObject* pTargetObject, DWORD dwDemage)
{
	DWORD dwResultDemege = dwDemage;

	BYTE attackerKind = pAttacker->GetObjectKind();
	BYTE targetKind = pTargetObject->GetObjectKind();

	if(targetKind & eObjectKind_Monster)
	{
		if(attackerKind == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pAttacker;
			if(targetKind == eObjectKind_TitanMonster)
			{
				if(pPlayer->InTitan() == FALSE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.05f );
				}				
			}
			else
			{
				if(pPlayer->InTitan() == TRUE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.5f );
				}				
			}
		}
	}	

	return dwResultDemege;
}


