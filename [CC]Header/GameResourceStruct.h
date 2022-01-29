



#ifndef __GAMERESOURCESTRUCT_H__
#define __GAMERESOURCESTRUCT_H__


class ITEM_MIX_INFO;
class ITEM_MIX_RES;
class ITEM_MIX_MATERIAL;

class CSkillInfo;
struct CharBasicStats
{
	WORD	wGenGol;					
	WORD	wMinChub;	
	WORD	wCheRyuk;	
	WORD	wSimMek;						
};
class RECALLINFO
{
public:
	DWORD	RecallType;
	DWORD	RecallID;
	DWORD	RecallMonsterNum;
	DWORD	RecallRange;
};
class ATTACKPATTERNINFO
{
public:
	WORD	AttackPatternNum;
	WORD	* AttackPattern;	
};
class CAttackPatternInfo
{
public:
	ATTACKPATTERNINFO * pAttackPatternInfo;
	virtual ~CAttackPatternInfo()
	{
		delete [] pAttackPatternInfo;
	}
	void Init(WORD wTotalNum)
	{
		MaxAttackNum = wTotalNum;
		pAttackPatternInfo = new ATTACKPATTERNINFO[MaxAttackNum];
		memset(pAttackPatternInfo, 0, sizeof(ATTACKPATTERNINFO)*MaxAttackNum);
	}
	void InitPattern(WORD PNum, WORD wTotalNum)
	{
		pAttackPatternInfo[PNum].AttackPatternNum = wTotalNum;
		pAttackPatternInfo[PNum].AttackPattern = new WORD[wTotalNum];
		memset(pAttackPatternInfo[PNum].AttackPattern, 0, sizeof(WORD)*wTotalNum);
	}
	void SetVal(WORD PNum, WORD PIdx, WORD PVal)
	{
		pAttackPatternInfo[PNum].AttackPattern[PIdx] = PVal;
	}
	WORD		MaxAttackNum;
};
class CRecallInfo
{
public:
	RECALLINFO * pRecallInfo;

	CRecallInfo(){ memset(this, 0, sizeof(CRecallInfo)); }
	virtual ~CRecallInfo()
	{
		if(pRecallInfo)
		delete [] pRecallInfo;
	}
	void Init(DWORD nRecall)
	{
		MaxRecallNum = nRecall;
		pRecallInfo = new RECALLINFO[MaxRecallNum];
		memset(pRecallInfo, 0, sizeof(RECALLINFO)*MaxRecallNum);
	}
	
	RECALLINFO * GetRecallInfo(DWORD dwIdx)
	{
		ASSERT(dwIdx < MaxRecallNum);
		return &pRecallInfo[dwIdx];
	}
	void SetRecallType(DWORD dwIdx, DWORD dwType)
	{
		pRecallInfo[dwIdx].RecallType = dwType;
	}
	void SetRecallID(DWORD dwIdx, DWORD dwID)
	{
		pRecallInfo[dwIdx].RecallID = dwID;
	}
	void SetRecallMonsterNum(DWORD dwIdx, DWORD dwNum)
	{
		pRecallInfo[dwIdx].RecallMonsterNum = dwNum;
	}
	void SetRecallRange(DWORD dwIdx, DWORD dwRange)
	{
		pRecallInfo[dwIdx].RecallRange = dwRange;
	}
protected:
	DWORD	MaxRecallNum;
	
};

class BASE_MONSTER_LIST
{
public:
	
	WORD		MonsterKind;		
	WORD		ObjectKind;			
	char		Name[MAX_MONSTER_NAME_LENGTH +1];				
	char		EngName[MAX_NAME_LENGTH+1];			
	LEVELTYPE	Level;					
	int			MoveDramaNum;				
	DWORD		MotionID;
	char		ChxName[MAX_CHXNAME_LENGTH];			
	float		Scale;				
	DWORD		Gravity;				
	float		Weight;						
	DWORD		MonsterRadius;
	DWORD		Life;				
	DWORD		Shield;					
	DWORD		ExpPoint;				
	WORD		Tall;					

	WORD		AttackPhysicalMin;	
	WORD		AttackPhysicalMax;
	WORD		CriticalPercent;
	WORD		PhysicalDefense;	
	ATTRIBUTEREGIST AttribResist;		

	WORD		WalkMove;	
	WORD		RunMove;	
	WORD		RunawayMove;

	int			Damage1DramaNum;
	int			Die1DramaNum;	
	int			Die2DramaNum;	

	DWORD		StandTime;
	BYTE		StandRate;
	BYTE		WalkRate;
	BYTE		RunRate;
	DWORD		DomainRange;			
	DWORD		PursuitForgiveTime;
	DWORD		PursuitForgiveDistance;	

	BOOL		bForeAttack;				
	DWORD		SearchPeriodicTime;			
	WORD		TargetSelect;					
	DWORD		SearchRange;
	DWORD		SearchAngle;
	WORD		TargetChange;

	WORD		AttackKind;
	DWORD		AttackNum;
	DWORD *		AttackIndex;
	DWORD *		AttackRate;
	CSkillInfo ** SkillInfo;	
	
	
	WORD		InitHelpType;	
	BOOL		bHearing;
	DWORD		HearingDistance;
};

class SMONSTER_LIST : public BASE_MONSTER_LIST
{
public:

	CAttackPatternInfo AttackPatternClass;

	CRecallInfo RecallInfoClass;
};

class MONSTEREX_LIST : public BASE_MONSTER_LIST
{
public:
	MONEYTYPE MinDropMoney;			
	MONEYTYPE MaxDropMoney;		

	
	WORD DropRate[eDROPITEMKIND_MAX];
	
	WORD CurDropRate[eDROPITEMKIND_MAX];


	WORD ItemListNum1;			
	WORD ItemListNum2;			
	WORD ItemListNum3;			


	BYTE MaxDropItemNum;	

	float fRunawayLifeRate;		
	WORD RunawayRate;		
	WORD RunawayHelpRate;	
	WORD RunawayDistance;		

	WORD Talk1;					
	WORD Talk2;				
	WORD Talk3;

	WORD HelpRequestCondition;
	WORD HelpRequestDistance;
	WORD HelpRequestTargetNum;

	WORD RecallCondition;
	WORD RecallObjectID;
	WORD RecallObjectNum;	
};
//////////////////////////////////////////////////////////////////////////
typedef struct _CHXLIST
{
	StaticString FileName;
}CHXLIST;

//-----------------------------------------------------------------------
#define MAX_PET_GRADE	3
struct BASE_PET_LIST
{
	DWORD	PetKind;
	char	Name[MAX_NAME_LENGTH+1];	
	DWORD	MotionID;	
	WORD	Pet2DIdx;
	char	ChxName_PerGrade[MAX_PET_GRADE][MAX_CHXNAME_LENGTH];
	float	Scale;		
	WORD	SummonLevel;	
	WORD	Tall[MAX_PET_GRADE];	
	float	SpeedFromHero;
	WORD	SkillIdx[MAX_PET_GRADE];	
	CSkillInfo*	SkillInfo[MAX_PET_GRADE];
	DWORD	SkillRecharge;	
	DWORD	StaminaDecreasePerSkill[MAX_PET_GRADE];	
	DWORD	StaminaMax[MAX_PET_GRADE];
	WORD	InventoryTapNum[MAX_PET_GRADE];
	int		DieDramaNum[MAX_PET_GRADE];		
	WORD	BuffList[MAX_PET_GRADE][3];
};

struct PET_RULE
{
	DWORD ResummonVilidTime;
	DWORD GradeUpProb1to2;
	DWORD GradeUpProb2to3;
	DWORD MaxFriendship;
	DWORD DefaultFriendship;
	DWORD RevivalFriendship;
	DWORD RevivalFriendship_Shop;
	int FriendShipPerSkill[3];
	int FriendShipPer10Sec[3];
	int FriendShipPerMasterDie;
	int FriendShipPerStaminaZero;
	int FriendShipPerTrade;
	int FriendShipPerUpgradeFail;
	int StaminaStand;
	int StaminaMove;
};

struct PET_BUFF_LIST
{
	WORD Idx;
	char BuffName[MAX_MONSTER_NAME_LENGTH + 1];
	BYTE BuffKind;
	DWORD BuffSuccessProb;
	DWORD BuffValueData;
	DWORD BuffAdditionalData;
};

//-----------------------------------------------------------------------
#define MAX_TITAN_GRADE	3

struct TITANINFO_GRADE
{
	DWORD	Speed;
	WORD	KyungGongSpeed[3];
	float	Scale;
	WORD	Tall;

	DWORD	MaxFuel;			
	DWORD	MaxSpell;			
	DWORD	Critical;
	float	AttackRange;

	WORD	MeleeAttackPowerMin;
	WORD	MeleeAttackPowerMax;
	WORD	RangeAttackPowerMin;
	WORD	RangeAttackPowerMax;

	float	PhyDef;	

	ATTRIBUTEATTACK	AttAtk;		
	ATTRIBUTEREGIST	AttReg;		

	DWORD	RecallReduceTime;	
	DWORD	dwRecallSpellTime;		
};

struct BASE_TITAN_LIST
{
	DWORD	TitanIdx;						
	char	TitanName[MAX_NAME_LENGTH+1];
	char	TitanBaseChxName[MAX_CHXNAME_LENGTH+1];
	char	TitanfaceModName[MAX_CHXNAME_LENGTH+1];
	WORD	Titan2DImage;					
	TITANINFO_GRADE	GradeInfo[MAX_TITAN_GRADE];
};

struct TITAN_SKILLLINK
{
	DWORD	CharSkillIdx;
	DWORD	TitanSkillIdx;
#ifdef _DEBUG
	char	skillName[MAX_NAME_LENGTH+1];
#endif

};

struct TITAN_EQUIPITEM_ENDURANCE
{
	DWORD titanEquipItemIdx;
	DWORD decreaseEndurance;
};

struct TITAN_RULE
{
	DWORD	TitanRecallValidTime_ZeroFuel;	
	DWORD	TitanRecallValidTime_Seal;		
	DWORD	TitanRecallValidTime_General;	
	DWORD	TitanVampValidTime;			
	DWORD	TitanFuelRate_ForVamp;			
	DWORD	MasterLifeRate_AllowForTitanVamp;		
	DWORD	MasterLifeRate_FromTitanVamp;		
	DWORD	FuelConvertRate_FromTitanVamp;		
	DWORD	MasterLifeRate_forTitanCall;		
	DWORD	TitanDamageShareRate;			
	DWORD	Prob_DecreaseEndurance;			
	DWORD	Prob_DecreaseArmorEndurance_Atk;	
	DWORD	Prob_DecreaseArmorEndurance_Def;
	DWORD	TitanMaintainTime;				
};

//-----------------------------------------------------------------------
typedef struct _MAPCHANGE_INFO
{
	WORD Kind;
	char CurMapName[MAX_NAME_LENGTH+1];
	char ObjectName[MAX_NAME_LENGTH+1];
	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
}MAPCHANGE_INFO;

//-----------------------------------------------------------------------
typedef struct _LOGINPOINT_INFO
{
	WORD Kind;
	char MapName[MAX_NAME_LENGTH+1];
	WORD MapNum;
	BYTE MapLoginPointNum;
	VECTOR3 CurPoint[10];
	WORD chx_num;
}LOGINPOINT_INFO;

//-----------------------------------------------------------------------
typedef struct _NPC_LIST
{
	WORD NpcKind;			//[NPC编号][By:十里坡剑神][QQ:112582793][2018/1/10]
	char Name[MAX_NAME_LENGTH+1];//[NPC名称][By:十里坡剑神][QQ:112582793][2018/1/10]

	WORD ModelNum;			//[模型编号][By:十里坡剑神][QQ:112582793][2018/1/10]
	WORD JobKind;			//[类型-传送点、售卖、任务等][By:十里坡剑神][QQ:112582793][2018/1/10]
	float Scale;			//[模型比例][By:十里坡剑神][QQ:112582793][2018/1/10]
	WORD Tall;				//[高度][By:十里坡剑神][QQ:112582793][2018/1/10]
	BOOL ShowJob;			
}NPC_LIST;

//-----------------------------------------------------------------------
typedef struct _NPC_REGEN
{

	_NPC_REGEN():dwObjectID(0),NpcKind(0)
	{
		Pos.x = Pos.y = Pos.z = 0;
	}
	DWORD	dwObjectID;		
	MAPTYPE	MapNum;			
	WORD	NpcKind;	
	char	Name[MAX_NAME_LENGTH+1];	
	WORD	NpcIndex;		
	VECTOR3 Pos;		
	float	Angle;			
}NPC_REGEN;


//-----------------------------------------------------------------------
typedef struct _MOD_LIST
{
	_MOD_LIST()
	{
		MaxModFile = 0;
		ModFile = NULL;
	}
	~_MOD_LIST()
	{
		if(MaxModFile == 0)
			return;
		SAFE_DELETE_ARRAY(ModFile);
	}
	DWORD MaxModFile;
	StaticString* ModFile;
	StaticString BaseObjectFile;
	
}MOD_LIST;
#ifdef _MAPSERVER_
struct MARRYGETITEM_INFO  //结婚赠送物品信息结构定义	by:十里坡剑神  QQ:112582793
{
	DWORD ItemIdx;
	WORD  Gender;
	WORD  ItemType;
	WORD  ItemNum;
};
#endif	//_MAPSERVER_
struct ITEM_INFO
{
	WORD ItemIdx;		
	char ItemName[MAX_ITEMNAME_LENGTH+1];	
	WORD ItemTooltipIdx;
	WORD Image2DNum;	
	WORD ItemKind;		
	DWORD BuyPrice;			
	DWORD SellPrice;	
	DWORD Rarity;		
	WORD WeaponType;	
	WORD GenGol;			
	WORD MinChub;		
	WORD CheRyuk;		
	WORD SimMek;		
	DWORD Life;				
	DWORD Shield;			
	WORD NaeRyuk;			
	ATTRIBUTEREGIST AttrRegist;		
	WORD LimitJob;		
	WORD LimitGender;		
	LEVELTYPE LimitLevel;	
	WORD LimitGenGol;		

	WORD LimitMinChub;	
	WORD LimitCheRyuk;		

	WORD LimitSimMek;	

	WORD ItemGrade;			
	WORD RangeType;			
	WORD EquipKind;			
	WORD Part3DType;		
	WORD Part3DModelNum;	
	WORD MeleeAttackMin;	
	WORD MeleeAttackMax;	
	WORD RangeAttackMin;	
	WORD RangeAttackMax;	
	WORD CriticalPercent;	
	ATTRIBUTEATTACK AttrAttack;		
	WORD PhyDef;			
	WORD Plus_MugongIdx;	
	WORD Plus_Value;		
	WORD AllPlus_Kind;	
	WORD AllPlus_Value;	

	WORD MugongNum;			
	WORD MugongType;		

	WORD LifeRecover;		
	float LifeRecoverRate;		
	WORD NaeRyukRecover;	
	float NaeRyukRecoverRate;

	WORD ItemType;		
	WORD wItemAttr;			
	WORD wAcquireSkillIdx1;
	WORD wAcquireSkillIdx2;	
	WORD wDeleteSkillIdx;	

	WORD wSetItemKind;
	WORD wPulsItemEffect;
	WORD wSellType;		//[出售货币类型][BY:十里坡剑神][QQ:112582793][2019-9-19][22:32:34]
};



struct SET_ITEM_OPTION
{
	WORD wIndex;									
	WORD wSetItemKind;								
	char szSetItemName[MAX_NAME_LENGTH+1];			
	
	WORD wGenGol;								
	WORD wMinChub;								
	WORD wCheRyuk;								
	WORD wSimMek;								
	DWORD dwLife;								
	DWORD dwShield;								
	DWORD dwNaeRyuk;									
	ATTRIBUTEREGIST AttrRegist;						

	LEVELTYPE wLimitLevel;						
	char szEffect[100];							
	WORD wLimitGenGol;							
	WORD wLimitMinChub;							
	WORD wLimitCheRyuk;							
	WORD wLimitSimMek;							
	
	WORD wMeleeAttackMin;							
	WORD wMeleeAttackMax;						
	WORD wRangeAttackMin;						
	WORD wRangeAttackMax;						
	WORD wCriticalPercent;						
	ATTRIBUTEATTACK AttrAttack;						
	
	WORD wPhyDef;								
	WORD wLifeRecover;							
	float fLifeRecoverRate;						
	WORD wNaeRyukRecover;							
	float fNaeRyukRecoverRate;					
	WORD wSetValue;								
	WORD wApplicationValue;					
};


struct SET_ITEM_INFO
{
	WORD wKind;
	WORD wCount;
};
///////////////////////////////////////////////////////////////////////////
struct PET_NAME
{
	WORD PetIdx;		
	char ItemName[MAX_PETNAME_LENGTH+1];	

};


struct sITEM_REINFORCE_INFO
{
	WORD			wItemIdx;
	WORD			ReinforceType;
	float			fMaxReinforce;
	float			fw;
};
//-----------------------------------
struct sITEM_RAREREINFORCE_INFO	: public sITEM_REINFORCE_INFO
{
	WORD			wMaterialGravity;
};

struct sITEM_DISSOLUTIONITEM
{
	WORD			wUnitItemIdx;				
	WORD			wUnitItemNum;						
};

struct sITEM_DISSOLUTION_INFO
{
	WORD					wDissolutionItemIdx;	
	WORD					wUnitKindNum;			
	WORD					wUnitTotalNum;		
	sITEM_DISSOLUTIONITEM*	pUnitItem;			
};
struct FB_MAP_INFO   //[副本地图信息][By:十里坡剑神][QQ:112582793][2017/11/27]
{
	WORD MapIDX;
	BYTE MapKind;
};
//[药品回气类型结构][By:十里坡剑神][QQ:112582793][2017/11/27]
struct YOUNGYAK_ITEM_DELAY_TYPE 
{
	WORD wtype;
	WORD ItemIdx;
	DWORD DelayTime;
};


class ITEM_MIX_INFO
{
public:
	WORD			wItemIdx;
	char			szItemName[MAX_ITEMNAME_LENGTH+1];
	
	WORD			wResultItemNum;
	ITEM_MIX_RES	* psResultItemInfo;
};

class ITEM_MIX_RES
{
public:
	WORD		wResItemIdx;
	char		szResItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		wMixKind;
	MONEYTYPE	Money;
	WORD		SuccessRatio;
	WORD		wMaterialItemNum;
	ITEM_MIX_MATERIAL * psMaterialItemInfo;
};

class ITEM_MIX_MATERIAL
{
public:
	WORD		wMatItemIdx;
	char		szMatItemName[MAX_ITEMNAME_LENGTH+1];
	WORD		wMatItemNum;
};

struct HILEVEL_ITEM_MIX_RATE_INFO	
{
	WORD	wItemLevel;
	DWORD	dwBaseRate;
	WORD	dwModulate;
};

struct TITAN_PARTS_KIND
{
	DWORD	dwPartsIdx;
	DWORD	dwPartsKind;
	DWORD	dwResultTitanIdx;
};

struct TITAN_STATSINFO
{
	DWORD	titanIdx;						
	DWORD	titanType;						
	DWORD	minAtt;						
	DWORD	maxAtt;						
	DWORD	woigongCri;					
	DWORD	naegongCri;					
	DWORD	minlongAtt;					
	DWORD	maxlongAtt;					
	DWORD	distance;					
	DWORD	Life;						
	DWORD	defense;					
	DWORD	Mana;						
	DWORD	Hosin;					
	WORD	AttrFire;					
	WORD	AttrWater;					
	WORD	AttrTree;					
	WORD	AttrIron;					
	WORD	AttrEarth;					
};
//////////////////////////////////////////////////////////////////////////

struct TITAN_UPGRADE_MATERIAL
{
    DWORD dwIndex;
	DWORD dwCount;
};

struct TITAN_UPGRADEINFO
{
    DWORD	dwTitanIdx;
	DWORD	dwNextTitanIdx;
	DWORD	dwMoney;
	WORD	wMaterial_Num;
	TITAN_UPGRADE_MATERIAL* pTitanUpgradeMaterial;

	TITAN_UPGRADEINFO::TITAN_UPGRADEINFO()
	{
		pTitanUpgradeMaterial = NULL;
	}
	TITAN_UPGRADEINFO::~TITAN_UPGRADEINFO()
	{
		if( pTitanUpgradeMaterial )
		{
			delete [] pTitanUpgradeMaterial;
			pTitanUpgradeMaterial = NULL;
		}
	}
};
//////////////////////////////////////////////////////////////////////////

struct TITAN_BREAK_MATERIAL
{
	DWORD	dwMaterialIdx;
	WORD	wMaterCnt;
	WORD	wRate;
};

struct TITAN_BREAKINFO
{
	DWORD	dwIdx;
	DWORD	dwMoney;
	WORD	wTotalCnt;
	WORD	wGetCnt;
	TITAN_BREAK_MATERIAL* pTitanBreakMaterial;

	TITAN_BREAKINFO::TITAN_BREAKINFO()
	{
		dwIdx = 0;
		wTotalCnt = 0;
		wGetCnt = 0;
		pTitanBreakMaterial = NULL;
	}
	TITAN_BREAKINFO::~TITAN_BREAKINFO()
	{
		if( pTitanBreakMaterial )
		{
			delete [] pTitanBreakMaterial;
			pTitanBreakMaterial = NULL;
		}
	}
};

struct BobusangPosPerMap
{
	VECTOR3	ApprPos;
	float	ApprDir;
};
//////////////////////////////////////////////////////////////////////////

struct UNIQUE_ITEM_OPTION_INFO
{
	DWORD	dwItemIdx;								
	char	strManEffectName[MAX_CHXNAME_LENGTH];	
	char	strWomanEffectName[MAX_CHXNAME_LENGTH];
	BYTE	ExchangeFlag;							
	BYTE	MixFlag;							
	DWORD	dwCurseCancellation;				

	int		nHp;
	int		nMp;
	int		nPhyDamage;
	int		nCriDamage;
	int		nCriRate;
	int		nGengol;
	int		nMinChub;
	int		nCheRyuk;
	int		nSimMek;
	int		nDefen;
	int		nRange;
	int		nAttR;
	int		nEnemyDefen;
	int		nShield;
    
	UNIQUE_ITEM_OPTION_INFO::UNIQUE_ITEM_OPTION_INFO()
	{
		dwItemIdx			= 0;
		ZeroMemory(strManEffectName, sizeof(strManEffectName));
		ZeroMemory(strWomanEffectName, sizeof(strWomanEffectName));
		ExchangeFlag		= 0;
		MixFlag				= 0;

		nHp					= 0;
		nMp					= 0;
		nPhyDamage			= 0;
		nCriDamage			= 0;
		nCriRate			= 0;
		nGengol				= 0;
		nMinChub			= 0;
		nCheRyuk			= 0;
		nSimMek				= 0;
		nDefen				= 0;
		nRange				= 0;
		nAttR				= 0;
		nEnemyDefen			= 0;
		nShield				= 0;
	}
};

struct UNIQUE_ITEM_MIX_MATERIAL_INFO
{
	DWORD	dwMaterialIdx;
	WORD	wMaterialNum;
};

struct UNIQUE_ITEM_MIX_RESULT_INFO
{
	DWORD	dwResultIdx;
	WORD	wResultRate;
};

struct UNIQUE_ITEM_MIX_INFO 
{
	DWORD	dwItemIdx;
	WORD	wMaterialKindNum;
	
	UNIQUE_ITEM_MIX_MATERIAL_INFO*	sUniqueItemMixMaterial;
	UNIQUE_ITEM_MIX_RESULT_INFO		sUniqueItemMixResult[UNIQUE_ITEM_MIX_RESULT_INDEX];
};

struct SKIN_SELECT_ITEM_INFO
{
	DWORD dwIndex;							
	char szSkinName[MAX_ITEMNAME_LENGTH+1];	
	DWORD dwLimitLevel;					
	WORD wEquipItem[SKINITEM_LIST_MAX];		

	SKIN_SELECT_ITEM_INFO::SKIN_SELECT_ITEM_INFO()
	{
		dwIndex = 0;	
		dwLimitLevel = 0;
		memset( wEquipItem, 0, sizeof(WORD)*SKINITEM_LIST_MAX );		
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//

typedef struct _PRELOAD
{
	StaticString FileName;
}PRELOAD;



struct SETSHOPITEM
{
	DWORD ItemIdx;
	char Name[MAX_ITEMNAME_LENGTH+1];
};
//-----------------------------------------------------------------------
struct GAMEDESC_INI
{
	GAMEDESC_INI()
	{
		strcpy(DistributeServerIP,"211.233.35.36");
		DistributeServerPort = 400;

		AgentServerPort = 100;
		
		bShadow = MHSHADOW_CIRCLE;
		
		dispInfo.dwWidth = 1024;		//WINDOW SIZE
		dispInfo.dwHeight = 768;
		dispInfo.dwBPS = 4;
		dispInfo.dispType = WINDOW_WITH_BLT;
		//dispInfo.bWindowed = TRUE;
		dispInfo.dwRefreshRate = 70;

		MaxShadowTexDetail = 256;
		MaxShadowNum = 4;

		FramePerSec = 30;
		TickPerFrame = 1000/(float)FramePerSec;

		MasterVolume = 1;
		SoundVolume = 1;
		BGMVolume = 1;
		
		CameraMinAngleX = 0;
		CameraMaxAngleX = 89.f;
		CameraMinDistance = 200.f;
		CameraMaxDistance = 1000.f;

		LimitDay = 0;
		LimitID[0] = 0;
		LimitPWD[0] = 0;

		strWindowTitle[0] = 0;
	}

	float MasterVolume;
	float SoundVolume;
	float BGMVolume;

	char DistributeServerIP[32];
	WORD DistributeServerPort;
	
	WORD AgentServerPort;
	
	DISPLAY_INFO dispInfo;

	BYTE bShadow;
	DWORD MaxShadowNum;
	DWORD MaxShadowTexDetail;

	DWORD FramePerSec;
	float TickPerFrame;

	char MovePoint[64];

	float CameraMinAngleX;
	float CameraMaxAngleX;
	float CameraMinDistance;
	float CameraMaxDistance;

	DWORD LimitDay;
	char LimitID[MAX_NAME_LENGTH+1];
	char LimitPWD[MAX_NAME_LENGTH+1];

	char strWindowTitle[128];
};

struct SEVERLIST
{
	WORD	LineType;
	char	DistributeIP[16];
	WORD	DistributePort;
	char	ServerName[64];
	WORD	ServerNo;
	DWORD	dwPing;
	BOOL	bEnter;

	SEVERLIST()
	{
		LineType = 0;
		ServerNo = 1;
		strcpy( DistributeIP, "211.233.35.36" );
		DistributePort = 400;
		strcpy( ServerName, "Test" );
		dwPing = 0;
		bEnter = TRUE;
	}
};


//-----------------------------------------------------------------------------------------------------------//
#endif //_CLIENT_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
#ifdef _SERVER_RESOURCE_FIELD_

//-----------------------------------------------------------------------------------------------------------//
#include "ServerTable.h"
//-----------------------------------------------------------------------
typedef struct _CHARACTERINOUTPOINT
{
	WORD MapNum;
	VECTOR3 MapInPoint[MAX_MAP_NUM];
	VECTOR3 MapOutPoint[MAX_MAP_NUM];
}CHARACTERINOUTPOINT;


//-----------------------------------------------------------------------------------------------------------//
#endif //_SERVER_RESOURCE_FIELD_
//-----------------------------------------------------------------------------------------------------------//
#endif //__GAMERESOURCESTRUCT_H__


