#ifndef _ABILITYHEADER_
#define _ABILITYHEADER_

WORD GetAbilityIndex_WithKindPosition(BYTE Kind,BYTE Position);
void GetKindPosition_WithAbilityIndex(WORD AbilityIndex,BYTE* pOutKind,BYTE* pOutPosition);
BYTE GetAbilityLevel_WithDBValue(BYTE DBValue);

#define ABILITYUPDATE_CHARACTERLEVEL_CHANGED			0x00000001
#define ABILITYUPDATE_ABILITYUPITEM_USE					0x00000002
#define ABILITYUPDATE_ABILITYLEVEL_CHANGED				0x00000004
#define ABILITYUPDATE_ABILITYEXP_CHANGED				0x00000008
#define ABILITYUPDATE_ABILITYQUICK_CHANGED				0x00000010
#define ABILITYUPDATE_ABILITYINITED						0x00000020
#define ABILITYUPDATE_ABILITYLEVEL_CHANGEDBYSHOPITEM	0x00000100
#define ABILITYUPDATE_CHARACTERSTAGE_CHANGED			0x00000200

enum eAbilityGradeUpMethod
{
	eAGM_OnOff,		//0: 1È¸¼º ¼ºÀåÇü : ÇÑ¹ø ¼ö·ÃÇÏ¿© ÀÍÈ÷¸é È¿°ú¸¦ °¡Áö°Ô µÇ¸ç ±× ÀÌ»óÀÇ È¿°úÁõÁøÀº ¾ø´Ù. (On/OffÇü)
	eAGM_GradeUp,	//1: µî±Þ ¼ºÀåÇü : Æ¯±âÄ¡¸¦ »ç¿ëÇÏ¿© °èÇØ¼­ ¼ºÀåÇØ ³ª°¥¼ö ÀÖ´Â ÇüÅÂ
	eAGM_Restrict,	//2: Á¦ÇÑ ¼ºÀåÇü : Æ¯Á¤ Æ¯±â°¡ ÇØ´ç ·¹º§¿¡ µµ´ÞÇÏ±â Àü¿¡´Â Æ¯±â¸¦ ¼ºÀå½ÃÅ³¼ö ¾ø´Â ÇüÅÂ
};

enum eAbilityUseType
{
	eAUT_Use,		//0: 1È¸¼º Å¸ÀÔ
	eAUT_Passive,	//1: ÆÐ½Ãºê Å¸ÀÔ
	eAUT_Active,	//2: ON/OFF
};

enum eAbilityGroupKind
{
	eAGK_Battle,
	eAGK_KyungGong,
	eAGK_Society,
	eAGK_Job,
	eAGK_Max,
};

#define ABILITYLEVEL_DB_KEY		'A'

#define ABILITYQUICKPOSITION_NULL	' '
#define ABILITYQUICKPOSITION_KEY	'A'

#define ABILITY_MAX_LEVEL		30

#define MAX_TOTAL_ABILITY	(MAX_ABILITY_NUM_PER_GROUP*eAGK_Max)
#define MAX_ABILITY_NUM_PER_GROUP	40
#define MAX_ABILITY_LEVEL 30

#define ABILITY_STARTINDEX_BATTLE		100
#define ABILITY_STARTINDEX_KYUNGGONG	200
#define ABILITY_STARTINDEX_SOCIETY		300
#define ABILITY_STARTINDEX_JOB			400
#define ABILITY_STARTINDEX_INTERVAL		100


struct ABILITY_STATS
{
	DWORD PhyAttackUp[WP_MAX];
	ATTRIBUTE_VAL<float> AttAttackUp;
	ATTRIBUTE_VAL<float> AttRegistUp;
	DWORD DefenceUp;

	DWORD LifeUp;
	DWORD NaeRyukUp;
	DWORD ShieldUp;
	
	DWORD UngiUpVal;

	DWORD StatGen;	
	DWORD StatMin;	
	DWORD StatChe;	
	DWORD StatSim;	
	float Kyunggong;	
	WORD  KyunggongLevel;
	float fNoAttrib;	
	DWORD SkillDamage;	
	DWORD CriticalDamage;	
	DWORD dwTitanRidingPlusTime;	

	DWORD GetPhyAttackUp(WORD WeaponKind)
	{
		return PhyAttackUp[WeaponKind-1];
	}
	void SetPhyAttackUp(WORD WeaponKind,DWORD UpVal)
	{
		PhyAttackUp[WeaponKind-1] = UpVal;
	}
	void Clear()
	{
		memset(this,0,sizeof(ABILITY_STATS));
	}
};

struct ABILITY_CALC
{
	DWORD	dwPhyAttack;	
	float	fAttribAttack;
	float	fAttribRegist;	
	DWORD	dwLife;		
	DWORD	dwDeffence;		
	DWORD	dwNearyuk;		
	DWORD	dwShield;		
	DWORD	dwUngi;			
	DWORD	dwStat;		
	float	fKyunggong;		
	float	fNoAttrib;		
	DWORD	dwSkillDamage;	
	DWORD	dwCriticalDamage;	
	DWORD	dwTitanRidingPlusTime;	
};

enum eABILITY_USE_KIND
{
	eAUK_KYUNGGONG = 1,
	eAUK_JOB,

	eAUK_PHYATTACK,
	eAUK_ATTRIB,

	eAUK_MAXLIFE,
	eAUK_DEFENCE,
	eAUK_MAXNEARYUK,
	eAUK_MAXSHIELD,
	eAUK_UNGISPEED,

	eAUK_SOCIETY = 10,	
	eAUK_PET	= 11,	//11 abilitybaseinfo.bin -> effect_kind
};

enum eABILITY_USE_KIND_STAGE	
{
	eAUK_STAT1				= 11,	
	eAUK_STAT2				= 12,	
	eAUK_STAT3				= 13,	
	eAUK_STAT4				= 14,	
	eAUK_KYUNGGONG_SPEED	= 15,	
	eAUK_JUSOOL				= 16,	
	eAUK_NOATTRIB			= 17,	
	eAUK_SKILL_DAMAGE		= 18,	
	eAUK_CRITICAL_DAMAGE	= 19,	
};

enum eABILITY_USE_JOB
{
	eAUKJOB_Ungijosik = 101,	
	eAUKJOB_Vimu = 102,			
	eAUKJOB_Party = 103,		
	eAUKJOB_Guild = 104,		
	eAUKJOB_Deal = 105,			
	eAUKJOB_StreetStall = 106,  

	eAUKJOB_Restraint = 108,	
	eAUKJOB_Attack = 109,		

	eAUKJOB_StallFind = 110,	
	eAUKJOB_AutoNote = 111,	
	eAUKJOB_PartyFind = 112,
	eAUKJOB_VimuWithGold	=	113,//[Ñº×¢±ÈÎä][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-4-20][22:20:05]

	eAUKJOB_TitanExpert = 120,	

	eAUKJOB_Upgrade = 201,		
	eAUKJOB_Mix = 202,			
	eAUKJOB_Reinforce = 203,	
	eAUKJOB_Dissolve = 204,		
};

enum eABILITY_USE_PET
{
	eAUKPET_State	= 150,
	eAUKPET_Inven	= 151,
	eAUKPET_Skill	= 152,
	eAUKPET_Rest	= 153,
	eAUKPET_Seal	= 154,
};

enum eABILITY_USE_SOCIETY
{
	eAUKJOB_Bow		= 301,		
	eAUKJOB_Happy	= 302,		
	eAUKJOB_Sad		= 303,		
	eAUKJOB_Yes		= 304,		
	eAUKJOB_No		= 305,		
	eAUKJOB_Sit		= 306,		
	eAUKJOB_BigBow	= 307,		
};

enum eAbilityIconState
{
	eAIS_NotUsable,
	eAIS_NotAvailable,
	eAIS_OnlyVisible,
	eAIS_Usable,
};


enum AbilityAcquireKind
{
	eAAK_Item,				
	eAAK_Quest,				
	eAAK_OtherAbility,		
	eAAK_CharacterLevel,	
	eAAK_CharacterStage,	
	eAAK_Max,
};


#define ABILITYINDEX_UNGI	401	
#define ABILITYINDEX_VIMU	402	
#define ABILITYINDEX_PARTY	403	
#define ABILITYINDEX_GUILD	404	
#define ABILITYINDEX_EXCHANGE	405	
#define ABILITYINDEX_STALL	406	


#define ABILITYINDEX_ITEMUPGRADE	407	
#define ABILITYINDEX_ITEMMIX		408
#define ABILITYINDEX_ITEMREINFORCE	409	
#define ABILITYINDEX_ITEMDISSOLVE	410	
#define ABILITYINDEX_LEADERSHIP		411	
#define	ABILITYINDEX_RESTRAINT		412	

#define ABILITYINDEX_PETSTATE		450	
#define ABILITYINDEX_PETINVEN		451	
#define ABILITYINDEX_PETSKILL		452	
#define ABILITYINDEX_PETREST		453	
#define ABILITYINDEX_PETSEAL		454	

#define ABILITYINDEX_TACTIC_HWA		122	
#define ABILITYINDEX_TACTIC_GEUK	128	
#define ABILITYINDEX_TACTIC_MUSSANG	129 


#endif
