// SkillInfo.cpp: implementation of the CSkillInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillInfo.h"
#include "SkillObjectFactory.h"
#include "ItemManager.h"
#ifdef _MAPSERVER_
#include "SkillObject_server.h"
#include "CharMove.h"
#else
#include "SkillObject_Client.h"
#include "ActionTarget.h"
#include "GameIn.h" 
#include "Hero.h"
#include "MoveManager.h"
#include "ObjectStateManager.h"
#include "ChatManager.h"
#include "BattleSystem_client.h"
#include "ObjectManager.h"
#include "SkillManager_client.h"
#include "MussangManager.h"
#include "VimuManager.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CSkillInfo::CSkillInfo()
{
	m_SkillOperateAnimationTime[GENDER_MALE] = 10000;
	m_SkillOperateAnimationTime[GENDER_FEMALE] = 0;

	m_BeforeSkill = 0;
	m_AfterSkill = 0;
	memset(m_SkillTree,0,sizeof(WORD)*5);
}

CSkillInfo::~CSkillInfo()
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,CSkillObject*,pSObj)
		delete pSObj;
	PTRLISTSEARCHEND;
	m_SkillObjectPool.RemoveAll();
}

BOOL CSkillInfo::InitSkillInfo(CMHFile* pFile)
{
	ASSERT(pFile->IsInited());
	ASSERT(pFile->IsEOF() == FALSE);

	memset(&m_SkillInfo,0,sizeof(m_SkillInfo));

	m_SkillInfo.SkillIdx = pFile->GetWord();									//[A]
	SafeStrCpy( m_SkillInfo.SkillName, pFile->GetString(), MAX_NAME_LENGTH+1 );	//[B]
	m_SkillInfo.SkillTooltipIdx = pFile->GetWord();								//[C]
	m_SkillInfo.RestrictLevel = pFile->GetWord();								//[D]
	m_SkillInfo.LowImage = pFile->GetInt();										//[E]
	m_SkillInfo.HighImage = pFile->GetInt();									//[F]
	m_SkillInfo.SkillKind = pFile->GetWord();									//[G]
	m_SkillInfo.WeaponKind = pFile->GetWord();									//[H]
	m_SkillInfo.SkillRange = pFile->GetWord();									//[I]
	m_SkillInfo.TargetKind = pFile->GetWord();									//[J]
	m_SkillInfo.TargetRange = pFile->GetWord();									//[K]
	m_SkillInfo.TargetAreaIdx = pFile->GetWord();								//[L]
	m_SkillInfo.TargetAreaPivot = pFile->GetWord();								//[M]
	m_SkillInfo.TargetAreaFix = pFile->GetWord();								//[N]
	m_SkillInfo.MoveTargetArea = pFile->GetWord();								//[O]
	m_SkillInfo.MoveTargetAreaDirection = pFile->GetWord();						//[P]
	m_SkillInfo.MoveTargetAreaVelocity = pFile->GetFloat();						//[Q]
	m_SkillInfo.Duration = pFile->GetDword();									//[R]
	m_SkillInfo.Interval = pFile->GetWord();									//[S]
	m_SkillInfo.DelaySingleEffect = pFile->GetWord();							//[T]
	m_SkillInfo.ComboNum = pFile->GetWord();									//[U]
	m_SkillInfo.Life = pFile->GetWord();										//[V]
	m_SkillInfo.BindOperator = pFile->GetWord();								//[W]
	m_SkillInfo.EffectStartTime = pFile->GetWord();								//[X]
	m_SkillInfo.EffectStart = FindEffectNum(pFile->GetString());				//[Y]
	m_SkillInfo.EffectUse = FindEffectNum(pFile->GetString());					//[Z]
	m_SkillInfo.EffectSelf = FindEffectNum(pFile->GetString());					//[AA]
	m_SkillInfo.EffectMapObjectCreate = FindEffectNum(pFile->GetString());		//[AB]
	m_SkillInfo.EffectMineOperate = FindEffectNum(pFile->GetString());			//[AC]
	m_SkillInfo.DelayTime = pFile->GetDword();									//[AD]
	m_SkillInfo.FatalDamage = pFile->GetWord();									//[AE]
	for(int n=0;n<12;++n)
		m_SkillInfo.NeedExp[n] = pFile->GetExpPoint();							//[AF-AQ]
	pFile->GetWord(m_SkillInfo.NeedNaeRyuk,12);									//[AR-BC]
	m_SkillInfo.Attrib = pFile->GetWord();										//[BD]
	m_SkillInfo.NegativeResultTargetType = pFile->GetWord();					//[BE]
	m_SkillInfo.TieUpType = pFile->GetWord();									//[BF]
	m_SkillInfo.ChangeSpeedType = pFile->GetWord();								//[BG]
	m_SkillInfo.ChangeSpeedRate = pFile->GetWord();								//[BH]
	m_SkillInfo.Dispel = pFile->GetWord();										//[BI]
	m_SkillInfo.PositiveResultTargetType = pFile->GetWord();					//[BJ]
	m_SkillInfo.Immune = pFile->GetWord();										//[BK]
	m_SkillInfo.AIObject = pFile->GetWord();									//[BL]
	m_SkillInfo.MineCheckRange = pFile->GetWord();								//[BM]
	m_SkillInfo.MineCheckStartTime = pFile->GetWord();							//[BN]
	
	m_SkillInfo.CounterDodgeKind = pFile->GetWord();							//[BO]
	m_SkillInfo.CounterEffect = FindEffectNum(pFile->GetString());				//[BP]

	m_SkillInfo.DamageDecreaseForDist = pFile->GetWord();						//[BQ]
	
	WORD AdditiveAttr;
	WORD temp[12];
	
	for(int n=0;n<6;++n)		//[BR-EQ]  此处循环读取数值，第一个数值是技能选项编号，根据技能选项编号判断需要读取到哪个变量数组，每个数组读取12个数值
	{							//一共设置6组选项，每组选项13个数，第一个数代表技能选项编号（注:每个技能此处必须满足78列数值，不设置字段也需要设置0补上）
		AdditiveAttr = pFile->GetWord();
		ySWITCH(AdditiveAttr)
			yCASE(0)	pFile->GetWord(temp,12);
			yCASE(11)	pFile->GetFloat(m_SkillInfo.FirstPhyAttack,12);
			yCASE(12)	pFile->GetFloat(m_SkillInfo.UpPhyAttack,12);
			yCASE(13)	pFile->GetWord(m_SkillInfo.FirstAttAttackMin,12);
			yCASE(14)	pFile->GetWord(m_SkillInfo.FirstAttAttackMax,12);
			yCASE(15)	pFile->GetWord(m_SkillInfo.ContinueAttAttack,12);

			yCASE(20)	pFile->GetFloat(m_SkillInfo.CounterPhyAttack,12);
			yCASE(21)	pFile->GetFloat(m_SkillInfo.CounterAttAttack,12);
			yCASE(22)	pFile->GetFloat(m_SkillInfo.UpPhyDefence,12);
			yCASE(23)	pFile->GetFloat(m_SkillInfo.UpAttDefence,12);
			yCASE(24)	pFile->GetWord(m_SkillInfo.UpMaxNaeRyuk,12);
			yCASE(25)	pFile->GetWord(m_SkillInfo.UpMaxLife,12);
			yCASE(26)	pFile->GetWord(m_SkillInfo.ContinueRecoverNaeRyuk,12);
			yCASE(27)	pFile->GetWord(m_SkillInfo.FirstRecoverNaeRyuk,12);
			yCASE(28)	pFile->GetWord(m_SkillInfo.ContinueRecoverLife,12);
			yCASE(29)	pFile->GetWord(m_SkillInfo.FirstRecoverLife,12);
			yCASE(30)	pFile->GetFloat(m_SkillInfo.CounterDodgeRate,12);

			yCASE(31)	pFile->GetFloat(m_SkillInfo.CriticalRate,12);
			yCASE(32)	pFile->GetFloat(m_SkillInfo.StunRate,12);
			yCASE(33)	pFile->GetWord(m_SkillInfo.StunTime,12);
			
			
			yCASE(34)	pFile->GetWord(m_SkillInfo.UpMaxShield,12);			
			yCASE(35)	pFile->GetWord(m_SkillInfo.AmplifiedPowerPhy,12);			
			yCASE(36)	pFile->GetFloat(m_SkillInfo.VampiricLife,12);
			yCASE(37)	pFile->GetFloat(m_SkillInfo.VampiricNaeryuk,12);			
			yCASE(38)	pFile->GetFloat(m_SkillInfo.RecoverStateAbnormal,12);
					
			yCASE(39)	pFile->GetFloat(m_SkillInfo.DispelAttackFeelRate,12);
			yCASE(40)	pFile->GetFloat(m_SkillInfo.ChangeSpeedProbability,12);
			
			yCASE(41)	pFile->GetWord(m_SkillInfo.DownMaxLife,12);			
			yCASE(42)	pFile->GetWord(m_SkillInfo.DownMaxNaeRyuk,12);			
			yCASE(43)	pFile->GetWord(m_SkillInfo.DownMaxShield,12);
			yCASE(44)	pFile->GetFloat(m_SkillInfo.DownPhyDefence,12);			
			yCASE(45)	pFile->GetFloat(m_SkillInfo.DownAttDefence,12);	
			yCASE(46)	pFile->GetFloat(m_SkillInfo.DownPhyAttack,12);
	
			yCASE(47)	pFile->GetDword(m_SkillInfo.SkillAdditionalTime,12);
			yCASE(48)	pFile->GetWord(m_SkillInfo.AmplifiedPowerAtt,12);
			yCASE(49)	pFile->GetFloat(m_SkillInfo.AmplifiedPowerAttRate,12);

			yCASE(50)	pFile->GetFloat(m_SkillInfo.FirstAttAttack,12);
			yCASE(51)	pFile->GetFloat(m_SkillInfo.ContinueAttAttackRate,12);
			
			yCASE(52)	pFile->GetFloat(m_SkillInfo.DamageRate,12);
			yCASE(53)	pFile->GetFloat(m_SkillInfo.AttackRate,12);
			yCASE(54)	pFile->GetWord(m_SkillInfo.ContinueRecoverShield,12);
			yCASE(55)	pFile->GetFloat(m_SkillInfo.AttackLifeRate,12);
			yCASE(56)	pFile->GetFloat(m_SkillInfo.AttackShieldRate,12);
			yCASE(57)	pFile->GetFloat(m_SkillInfo.AttackSuccessRate,12);
			yCASE(58)	pFile->GetFloat(m_SkillInfo.VampiricReverseLife,12);
			yCASE(59)	pFile->GetFloat(m_SkillInfo.VampiricReverseNaeryuk,12);
			yCASE(60)	pFile->GetDword(m_SkillInfo.AttackPhyLastUp,12);
			yCASE(61)	pFile->GetDword(m_SkillInfo.AttackAttLastUp,12);
			yCASE(100)	
				m_SkillInfo.SkipEffect = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
			yCASE(101)	
				m_SkillInfo.SpecialState = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
			yCASE(200)	
				m_SkillInfo.AddDegree = pFile->GetInt();
				for(int i = 0; i < 11; i++)
					pFile->GetInt();
			yCASE(201)	
				m_SkillInfo.SafeRange = pFile->GetWord();
				for(int i = 0; i < 11; i++)
					pFile->GetWord();
		yENDSWITCH
	}	
	m_SkillInfo.CanSkipEffect = pFile->GetBool();

	m_SkillInfo.ChangeKind = pFile->GetWord();

	if( m_SkillInfo.TargetAreaPivot == 1 && 
		m_SkillInfo.TargetRange != 0 &&
		m_SkillInfo.SkillRange > m_SkillInfo.TargetRange )
		m_SkillInfo.TargetRange = m_SkillInfo.SkillRange;
	m_SkillInfo.LinkSkillIdx = pFile->GetWord();
	m_SkillInfo.bIsCheck = pFile->GetBool();
	return TRUE;
}

#define SKILLDUMMYCHECK 0x74
BOOL CSkillInfo::InitDummySkillInfo(CSkillInfo* pOriginalSkillInfo)
{
	memcpy(&m_SkillInfo,&pOriginalSkillInfo->m_SkillInfo,sizeof(SKILLINFO));
	char* p = (char*)&m_SkillInfo;
	for(int n=0;n<sizeof(SKILLINFO);++n)
	{
		p[n] ^= SKILLDUMMYCHECK;
	}

	return TRUE;
}

BOOL CSkillInfo::CheckOriginal(CSkillInfo* pSkillInfo)
{
	char* pOriginal = (char*)&pSkillInfo->m_SkillInfo;
	char* pDummy = (char*)&m_SkillInfo;

	for(int n=0;n<sizeof(SKILLINFO);++n)
	{
		if(pOriginal[n] != ( pDummy[n] ^ SKILLDUMMYCHECK ))
			return FALSE;
	}
	return TRUE;
}


#ifdef _MAPSERVER_
#include "Battle.h"
#include "Player.h"
#include "Titan.h"
#include "PartyManager.h"
#include "Party.h"

BOOL CSkillInfo::IsExcutableSkillState(CObject* pObject,int SkillLevel, SKILLOPTION* pOption)
{	
	if(pObject->GetLevel() < m_SkillInfo.RestrictLevel)
	{
		return FALSE;
	}
	if(m_SkillInfo.SpecialState)
	{
		if(m_SkillInfo.SpecialState == eSingleSpecialState_Hide)
		{
			if(pObject->GetObjectKind() == eObjectKind_Player)
			{
				CPlayer* pPlayer = (CPlayer*)pObject;
				if( pPlayer->GetPetManager()->GetCurSummonPet() )
					return FALSE;

				if( pPlayer->IsVimuing() )
					return FALSE;
			}
		}
		else if(m_SkillInfo.SpecialState == eSingleSpecialState_Detect)
		{
			if(pObject->GetSingleSpecialState(eSingleSpecialState_DetectItem))
				return FALSE;
		}
	}
	else if(pObject->GetSingleSpecialState(eSingleSpecialState_Hide))
	{
		return FALSE;
	}

	if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->InTitan() )
	{
		CPlayer* pPlayer = (CPlayer*)pObject;

		if( !pPlayer->CanUseTitanSkill() )
			return FALSE;

		if(pOption)
		{
			if((int)((pPlayer->GetCurTitan())->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	return FALSE;
			if((int)((pPlayer->GetCurTitan())->GetMaxLife()) < pOption->Life * -1) return FALSE;
		}
	}
	else
	{
		if(pOption)
		{
			if((int)(pObject->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	return FALSE;
			if((int)(pObject->GetMaxLife()) < pOption->Life * -1) return FALSE;
			if((int)(pObject->GetMaxShield()) < pOption->Shield * -1) return FALSE;
		}
		////////////////////////////////////////////////////////////////////////////////
	}
	if(pObject->IsInSpecialState(eSpecialState_Stun) == TRUE)
		return FALSE;

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		if(!((CPlayer*)pObject)->CanSkillState())
			return FALSE;


		if( ((CPlayer*)pObject)->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill,GetSkillIndex()) != 0)
		{
			return FALSE;
		}
	}

	WORD MogongNaeRyuk = GetNeedNaeRyuk(SkillLevel);
	DWORD NeedNaeRyuk = (DWORD)(MogongNaeRyuk*gEventRate[eEvent_NaeRuykRate]);

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		if( ((CPlayer*)pObject)->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( ((CPlayer*)pObject)->GetPartyIdx() );
			if( pParty )
			{
				int count = pParty->GetMemberCountofMap( pObject->GetID() );
				if( count && gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1] )//[组队内力恢复加成计算][By:十里坡剑神][QQ:112582793][2018/4/22]
					NeedNaeRyuk = (DWORD)(NeedNaeRyuk*gPartyEvent[ePartyEvent_NaeRyukRate].Rate[count-1]);
			}
		}
		WORD NaeryukSpend = ((CPlayer*)pObject)->GetShopItemStats()->NeaRyukSpend + ((CPlayer*)pObject)->GetAvatarOption()->NeaRyukSpend 
			+ ((CPlayer*)pObject)->GetShopItemStats()->PlustimeNaeruyk;
		if( NaeryukSpend >= 100 )
			NeedNaeRyuk = 0;
		else
			NeedNaeRyuk = (DWORD)(NeedNaeRyuk*(1.0f - NaeryukSpend*0.01f));


//#ifdef _JAPAN_LOCAL_
//		if( ((CPlayer*)pObject)->IsMussangMode() )
//		{
//			NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.25f);
//		}
//#else
		if( ((CPlayer*)pObject)->IsMussangMode() )
		{
			switch(((CPlayer*)pObject)->GetStage())
			{
			case eStage_Normal:	
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.95f);
			case eStage_Hwa:		
			case eStage_Geuk:
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.90f);
			case eStage_Hyun:	  		
			case eStage_Tal:	
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.85f);
			}
		}
//#endif //_JAPAN_LOCAL_

		if(pOption)
		{
			float ReduceNaeRyuk = 0.0f;

			ReduceNaeRyuk += (((CPlayer*)pObject)->GetSkillStatsOption()->ReduceNaeRyuk + pOption->ReduceNaeRyuk);

			NeedNaeRyuk += (DWORD)((NeedNaeRyuk * ReduceNaeRyuk) + 0.5);

			if(NeedNaeRyuk < 0)
				NeedNaeRyuk = 0;
		}	
	}

	//if( pObject->GetObjectKind() == eObjectKind_Player && ((CPlayer*)pObject)->InTitan() )
	//{
	//	if( NeedNaeRyuk )
	//	{
	//		CPlayer* pPlayer = (CPlayer*)pObject;

	//		CTitan* pTitan = pPlayer->GetCurTitan();
	//		if( pTitan->GetTotalInfo()->Spell < NeedNaeRyuk )
	//			return FALSE;

	//		pTitan->ReduceSpell( NeedNaeRyuk );
	//	}
	//}
	//else
	{
		if(NeedNaeRyuk)
		{
			if(pObject->GetNaeRyuk() < NeedNaeRyuk)
			{
				return FALSE;
			}

			pObject->ReduceNaeRyuk(NeedNaeRyuk);

		}	
		/*
		if( pObject->GetObjectKind() == eObjectKind_Player && 
		g_pServerSystem->GetMap()->IsVillage() == FALSE &&
		pObject->GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;

			DWORD MugongExp = MogongNaeRyuk;

			if(g_pServerSystem->GetNation() == eNATION_CHINA)
			{
			if(pPlayer->GetBadFame() >= 60)
			MugongExp = MogongNaeRyuk/4;
			else if(pPlayer->GetBadFame() >= 30)
			MugongExp = MogongNaeRyuk/2;
			else
			MugongExp = MogongNaeRyuk;
		}

		pPlayer->AddExpPointToMugong(GetSkillIndex(),MugongExp);
		*/
	}
	
	return TRUE;
}

BOOL CSkillInfo::IsInSkillRange(CObject* pObject,VECTOR3* pTargetPos,float TargetRadius, SKILLOPTION* pOption)
{
	VECTOR3* p1 = CCharMove::GetPosition(pObject);
	VECTOR3* p2 = pTargetPos;
	p1->y = p2->y = 0;

	float dist = CalcDistanceXZ(p1,p2) - TargetRadius;
	float addRange = 0.f;
	
	if(m_SkillInfo.ComboNum != SKILL_COMBO_NUM &&
		(m_SkillInfo.WeaponKind == WP_AMGI || m_SkillInfo.WeaponKind == WP_GUNG))	
		addRange = pObject->GetAddAttackRange();

	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		dist -= 300.f;

		if(pOption)
			addRange += (((CPlayer*)pObject)->GetSkillStatsOption()->Range + pOption->Range);

		addRange += ((CPlayer*)pObject)->GetUniqueItemStats()->nRange;

		if( ((CPlayer*)pObject)->InTitan() == TRUE )
		{
			CItemSlot* pSlot = ((CPlayer*)pObject)->GetSlot(eItemTable_Titan);
			ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs(TP_TITANWEAR_START + eTitanWearedItem_Weapon);
			WORD wTitanWeaponIdx = pItemBase->wIconIdx;
			if( wTitanWeaponIdx )
			{
				ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wTitanWeaponIdx);
				if( pItemInfo && pItemInfo->MugongNum > 0 )
				{
					addRange += pItemInfo->MugongNum;
				}
			}
		}
	}

	float dis = (float)m_SkillInfo.SkillRange+addRange;
	if(dis < 0)
		dis = 0;

	if( dis < dist)	
		return FALSE;
		
	return TRUE;
}

BOOL CSkillInfo::ComboTest(CObject* pObject)
{
	return TRUE;
}

BOOL CSkillInfo::CheckEvade(MAINTARGET* pMainTarget)
{
	return FALSE;
}
#else //_MAPSERVER_

BOOL CSkillInfo::IsExcutableSkillState(CHero* pHero,int SkillLevel, SKILLOPTION* pOption)
{
	if(pHero->GetLevel() < m_SkillInfo.RestrictLevel)
	{// [级别太低无法施展武功] [by:十里坡剑神][QQ:112582793][2018/11/24]
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(999) );
		return FALSE;
	}

	if(m_SkillInfo.SpecialState)
	{
        DWORD time = pHero->GetSingleSpecialStateUsedTime(m_SkillInfo.SpecialState);
 		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(m_SkillInfo.SpecialState);

		if(!pInfo)
			return FALSE;

		int delay = (time + pInfo->DelayTime) - gCurTime;
		if(time != 0 && delay > 0)
		{// [技能使用间隔时间检查] [by:十里坡剑神][QQ:112582793][2018/11/24]
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1355), delay / 1000);
			return FALSE;
		}

		if(m_SkillInfo.SpecialState == eSingleSpecialState_Hide)
		{
			if(pHero->GetPet())
			{// [宠物为召唤状态下不能使用隐身] [by:十里坡剑神][QQ:112582793][2018/11/24]
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}

			if(VIMUMGR->IsVimuing())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}

			if(pHero->IsPKMode())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1330));
				return FALSE;
			}
		}
		else if(m_SkillInfo.SpecialState == eSingleSpecialState_Detect)
		{
			if(pHero->GetSingleSpecialState(eSingleSpecialState_DetectItem))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1332));
				return FALSE;
			}
		}

	}
	else if(pHero->GetSingleSpecialState(eSingleSpecialState_Hide))
	{// [隐身状态不能使用技能] [by:十里坡剑神][QQ:112582793][2018/11/24]
		return FALSE;
	}

	if(pOption)
	{
		BOOL bVal = FALSE;

		if((int)(pHero->GetMaxNaeRyuk()) < pOption->NaeRyuk * -1)	bVal = TRUE;
		if((int)(pHero->GetMaxLife()) < pOption->Life * -1)	bVal = TRUE;
		if((int)(pHero->GetMaxShield()) < pOption->Shield * -1)	bVal = TRUE;
		
		if(bVal)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(798));
			return FALSE;
		}
	}

	if (pHero->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Skill, GetSkillIndex()) != 0)
		return FALSE;
	if (pHero->IsSkillCooling())
		return FALSE;
	DWORD NeedNaeRyuk = GetNeedNaeRyuk(SkillLevel);
	
	if(NeedNaeRyuk)
	{
		WORD NaeryukSpend = HERO->GetShopItemStats()->NeaRyukSpend + HERO->GetAvatarOption()->NeaRyukSpend 
			+ HERO->GetShopItemStats()->PlustimeNaeruyk;
		NeedNaeRyuk = (DWORD)(NeedNaeRyuk*(1.0f - NaeryukSpend*0.01f));

		if( MUSSANGMGR->IsMussangMode() )
		{
			switch(HERO->GetStage())
			{
			case eStage_Normal:	
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.95f);
			case eStage_Hwa:		
			case eStage_Geuk:
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.90f);
			case eStage_Hyun:	  		
			case eStage_Tal:	
				NeedNaeRyuk = (DWORD)(NeedNaeRyuk * 0.85f);
			}
		}
		if(pOption)
		{
			float ReduceNaeRyuk = 0.0f;
			ReduceNaeRyuk += (pHero->GetSkillStatsOption()->ReduceNaeRyuk + pOption->ReduceNaeRyuk);
			NeedNaeRyuk += (DWORD)((NeedNaeRyuk * ReduceNaeRyuk) + 0.5);
			if(NeedNaeRyuk < 0)
			NeedNaeRyuk = 0;
		}
		if( pHero->GetNaeRyuk() < NeedNaeRyuk )	
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(401) );
			return FALSE;
		}
	}
	if( pHero->IsInSpecialState(eSpecialState_Stun) == TRUE)
		return FALSE;

	if( pHero->GetState() != eObjectState_None &&
		pHero->GetState() != eObjectState_Move &&
		pHero->GetState() != eObjectState_TiedUp_CanSkill &&
		pHero->GetState() != eObjectState_Society
		) 
	{

		if( pHero->GetState() != eObjectState_SkillSyn &&
			pHero->GetState() != eObjectState_SkillUsing &&
			pHero->GetState() != eObjectState_SkillBinding &&
			pHero->GetState() != eObjectState_SkillStart &&			
			pHero->GetState() != eObjectState_SkillDelay )
		{
			//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(141) );
			pHero->DisableAutoAttack();
		}

		return FALSE;
	}

	if( pHero->GetState() == eObjectState_Society )
	{
		if( pHero->EndSocietyAct() == FALSE )
		{
			pHero->DisableAutoAttack();
			return FALSE;
		}
	}

	if( pHero->GetLevel() < m_SkillInfo.RestrictLevel &&
		m_SkillInfo.ComboNum != SKILL_COMBO_NUM)
		return FALSE;

	return TRUE;
}

BOOL CSkillInfo::IsInSkillRange(CHero* pHero,CActionTarget* pTarget, SKILLOPTION* pOption)
{
	VECTOR3 p1;
	pHero->GetPosition(&p1);
	VECTOR3* p2 = pTarget->GetTargetPosition();
	VECTOR3 hpos;
	pHero->GetPosition(&hpos);
	if(p2 == NULL)
		return FALSE;

	p1.y = p2->y = 0;
	
	float dist = CalcDistanceXZ(&p1,p2) - pTarget->GetRadius();
	float addRange = 0;

	if( m_SkillInfo.ComboNum != SKILL_COMBO_NUM &&
		(m_SkillInfo.WeaponKind == WP_AMGI || m_SkillInfo.WeaponKind == WP_GUNG))	
		addRange = pHero->GetAddAttackRange();

	if(pOption)
		addRange += (pHero->GetSkillStatsOption()->Range + pOption->Range);

	addRange += pHero->GetUniqueItemStats()->nRange;

	if( pHero->InTitan() == TRUE )
	{
		WORD wTitanWeaponIdx = pHero->GetTitanWearedItemIdx(eTitanWearedItem_Weapon);
		if( wTitanWeaponIdx )
		{
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wTitanWeaponIdx);
			if( pItemInfo && pItemInfo->MugongNum > 0 )
			{
				addRange += pItemInfo->MugongNum;
			}
		}
	}

	float dis = m_SkillInfo.SkillRange+addRange;
	if(dis < 0)
		dis = 0;

	if( dis < dist)
		return FALSE;
		
	return TRUE;
}

BOOL CSkillInfo::ComboTest(CHero* pHero)
{

	if( GetSkillIndex() == COMBO_EVENT_MIN || GetSkillIndex() == COMBO_EVENT_HAMMER )
		return TRUE;
	
	DWORD curcombo = pHero->GetCurComboNum();
	if( m_SkillInfo.ComboNum == SKILL_COMBO_NUM &&
		curcombo != SKILL_COMBO_NUM )
		return TRUE;
	if( m_SkillInfo.ComboNum == curcombo +1 )
		return TRUE;

	return FALSE;
}

void CSkillInfo::SendMsgToServer(CHero* pOperator,CActionTarget* pTarget)
{
	MSG_SKILL_START_SYN msg;
	MAINTARGET MainTarget;	
	pTarget->GetMainTarget(&MainTarget);

	WORD wSkillIndex = GetSkillIndex();
	if( (wSkillIndex < 1) || (wSkillIndex > 10036) )
	{
		char str[256] ={0,};
		sprintf( str, "技能编号超出10036 %d", wSkillIndex );
		WRITEDEBUGFILE( str );
		return;
	}
	
	msg.InitMsg(GetSkillIndex(),&MainTarget,pOperator->GetAngleDeg(),pOperator->GetID());
	pTarget->GetTargetArray(&msg.TargetList);	
	NETWORK->Send(&msg,msg.GetMsgLength());
}

BOOL CSkillInfo::IsValidTarget(CHero* pHero,CActionTarget* pTarget)
{
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pHero);
	CObject* pTargetObj = NULL;
	if(pTarget->GetTargetID() != 0)
		pTargetObj = OBJECTMGR->GetObject(pTarget->GetTargetID());

	if(pTargetObj == NULL)
		return TRUE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Npc )
		return FALSE;

	if( pTargetObj->GetObjectKind() == eObjectKind_Pet )
		return FALSE;

	if( SKILLMGR->CheckSkillKind(m_SkillInfo.SkillKind) == TRUE )
	{
		if( GetObjectKindGroup(pTargetObj->GetObjectKind()) == eOBJECTKINDGROUP_NONE )
			return FALSE;
	}

	if( m_SkillInfo.FirstPhyAttack[11] != 0 ||
		m_SkillInfo.FirstAttAttackMin[11] != 0 ||
		m_SkillInfo.FirstAttAttackMax[11] != 0 )
	{
		if( pBattle->IsEnemy(pHero,pTargetObj) == FALSE )
			return FALSE;
	}
	
	return TRUE;
}

BOOL CSkillInfo::ConvertTargetToSkill(CHero* pHero,CActionTarget* pTarget)
{
	if(GetSkillInfo()->TargetKind == 1)
	{
		pTarget->SetOneTarget(pHero);
		return TRUE;
	}
	else if(GetSkillInfo()->TargetKind == 0)
	{
		pTarget->SetOneTarget(NULL);
		return TRUE;
	}
	else
	{
		return TRUE;
	}
}

#endif

CSkillObject* CSkillInfo::GetSkillObject()
{
	CSkillObject* pSObj;
	if(m_SkillObjectPool.GetCount() == 0)
		pSObj = CSkillObjectFactory::MakeNewSkillObject(this);
	else
		pSObj = (CSkillObject*)m_SkillObjectPool.RemoveTail();
	return pSObj;
}

void CSkillInfo::ReleaseSkillObject(CSkillObject* pSObj,int kind)
{
	PTRLISTSEARCHSTART(m_SkillObjectPool,CSkillObject*,pPreObj)
		if(pSObj == pPreObj)
		{			
			return;
		}
	PTRLISTSEARCHEND

#ifdef _MAPSERVER_
	pSObj->releaseKind = kind;
#endif
	
	m_SkillObjectPool.AddHead(pSObj);
}

VECTOR3* CSkillInfo::GetTargetAreaPivotPos(VECTOR3* pOperatorPos,VECTOR3* pTargetPos)
{	
	if(IsTargetAreaFitToHero())
		return pOperatorPos;
	else
		return pTargetPos;
}

BOOL CSkillInfo::IsMultipleTargetSkill()
{
	if( m_SkillInfo.TargetRange != 0 ||
		m_SkillInfo.TargetAreaIdx != 0)
		return TRUE;
	else
		return FALSE;
}

DWORD CSkillInfo::GetSkillOperateAnimationTime(int gender)
{
	if(gender >= GENDER_MAX)
	{
		ASSERT(0);
		gender = GENDER_MALE;
	}

	return m_SkillOperateAnimationTime[gender];
}

void CSkillInfo::SetSkillTree(WORD Before,WORD After,WORD* pArray)
{
	m_AfterSkill = After;
	m_BeforeSkill = Before;
	memcpy(&m_SkillTree,pArray,sizeof(WORD)*5);
}
