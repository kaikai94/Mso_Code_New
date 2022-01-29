



#ifndef __COMMONGAMEDEFINE_H__
#define __COMMONGAMEDEFINE_H__


enum TACTIC_FAILED_REASON
{
	eTFR_Naeryuk,
	eTFR_MemberDie,
	eTFR_TimeOver,
};

//for GMPower
enum eGM_POWER
{
	eGM_POWER_MASTER = 0,
	eGM_POWER_MONITOR,
	eGM_POWER_PATROLLER,
	eGM_POWER_AUDITOR,
	eGM_POWER_EVENTER,
	eGM_POWER_MAX,
};


enum eTRAFFIC_KIND
{
	eTraffic_User_To_Agent			= 1,
	eTraffic_Agent_To_User			= 2,
	eTraffic_Agent_To_Map			= 3,
	eTraffic_Map_To_Agent			= 4,
	eTraffic_Agent_to_Agent			= 5,
	eTraffic_Map_to_Map				= 6,
	eTraffic_User_To_Distribute		= 7,
	eTraffic_Distribute_To_User		= 8,
};

#define EXIT_COUNT	5
enum eEXITCODE
{
	eEXITCODE_OK,
	eEXITCODE_PKMODE,
	eEXITCODE_LOOTING,
	eEXITCODE_SPEEDHACK,
	eEXITCODE_DIEONEXIT,
	eEXITCODE_NOT_ALLOW_STATE,
	eEXITCODE_NOT_IN_LOGIN_MAP,
};

#define SPEEDHACK_CHECKTIME	60000	

#define PKLOOTING_ITEM_NUM			20
#define PKLOOTING_DLG_DELAY_TIME	2000
#define PKLOOTING_LIMIT_TIME		10000

enum ePKCODE
{
	ePKCODE_OK,
	ePKCODE_ALREADYPKMODEON,
	ePKCODE_VIMUING,
	ePKCODE_STATECONFLICT,
	ePKCODE_NOTALLAW,
};

enum eLOOTINGERROR
{
	eLOOTINGERROR_OK,
	eLOOTINGERROR_NOMORE_CHANCE,
	eLOOTINGERROR_NOMORE_ITEMLOOTCOUNT,
	eLOOTINGERROR_INVALID_POSITION,
	eLOOTINGERROR_ALREADY_SELECTED,
	eLOOTINGERROR_OVER_DISTANCE,
	eLOOTINGERROR_NO_LOOTINGROOM,
};

#define NPC_TALKING_DISTANCE		700
#define PK_LOOTING_DISTANCE			1000

enum eDROPITEMKIND
{
	eNOITEM,
	eMONEY,
	eITEM1,
	eITEM2,
	eITEM3,

	eDROPITEMKIND_MAX,
};

#define MAX_DISSOLVEGRID_NUM		20
#define ITEM_DISSOLVE_PRICE_VALUE	200
enum eREINFORCEJEWELPOWER
{
	eRJP_GENGOL=1,		
	eRJP_MINCHUB,		
	eRJP_CHERYUK,		
	eRJP_SIMMEK,		
	eRJP_LIFE,			
	eRJP_NAERYUK,	
	eRJP_SHIELD,	
	eRJP_FIREREGIST,	
	eRJP_WATERREGIST,	
	eRJP_TREEREGIST,	
	eRJP_GOLDREGIST,	
	eRJP_EARTHREGIST,	
	eRJP_PHYATTACK,		
	eRJP_CRITICAL,		
	eRJP_FIREATTACK,	
	eRJP_WATERATTACK,	
	eRJP_TREEATTACK,	
	eRJP_GOLDATTACK,	
	eRJP_EARTHATTACK,	
	eRJP_PHYDEFENSE,	
	eRJP_MAX = eRJP_PHYDEFENSE,
};

enum
{
	eUSERLEVEL_GOD = 1,
	eUSERLEVEL_PROGRAMMER,
	eUSERLEVEL_DEVELOPER,
	eUSERLEVEL_GM,
	eUSERLEVEL_SUPERUSER,
	eUSERLEVEL_USER,
	eUSERLEVEL_SUBUSER,	
};

#define MAX_ITEMBUY_NUM		100 //[单次购买物品最大数量][By:TT天语][QQ:2013734367][2018/1/13]
#define MAX_INVENTORY_MONEY	4000000000 
#define MAX_INVENTORY_GOLD  1000000000  
#define MAX_MIX_MATERIAL	128
#define MAX_TITANBREAK_MATERIAL	20
#define MAX_TITAN_EQUIPITEM_NUM	40	

#define PKMODETIME	60000		


enum eSTATUSICON
{
	eStatusIcon_FireContinueDamage = 1,
	eStatusIcon_WaterContinueDamage,
	eStatusIcon_SpeedDown,
	eStatusIcon_TieUpCanMove,
	eStatusIcon_TieUpCanAttack,
	eStatusIcon_TieUpAll,
	eStatusIcon_HealContinue,
	eStatusIcon_PhyAttackUp,
	eStatusIcon_PhyDefenceUp,
	eStatusIcon_MaxLifeUp,	
	eStatusIcon_MaxNaeryukUp,
	eStatusIcon_PhyCounter,
	eStatusIcon_AttrDefenceFire,
	eStatusIcon_AttrDefenceWater,
	eStatusIcon_AttrDefenceTree,
	eStatusIcon_AttrDefenceIron,
	eStatusIcon_AttrDefenceEarth,
	eStatusIcon_MaxShieldUp,		
	eStatusIcon_AttrCounterFire,
	eStatusIcon_AttrCounterWater,		
	eStatusIcon_AttrCounterTree,
	eStatusIcon_AttrCounterIron,
	eStatusIcon_AttrCounterEarth,
	eStatusIcon_AttrCounterAll,
	eStatusIcon_Immune,
	eStatusIcon_AmpliedPower,
	eStatusIcon_PoisonContinueDamage,
	eStatusIcon_IronContinueDamage,
	eStatusIcon_PhyAttackDown,
	eStatusIcon_PhyDefenseDown,
	eStatusIcon_MaxLifeDown,
	eStatusIcon_MaxNaeryukDown,
	eStatusIcon_MaxShieldDown,
	eStatusIcon_DamageDown	= 34,
	eStatusIcon_AttackUp	= 35,
	eStatusIcon_ContinueRecover	= 36,
	eStatusIcon_ReverseVampiricNaeryuk	= 37,
	eStatusIcon_AttackPhyLastUp			= 39,
	eStatusIcon_AttackAttLastUp			= 40,
	eStatusIcon_ReverseVampiricLife		= 45,
	eStatusIcon_MussangMode				= 48,
	eStatusIcon_PartyHelp				= 49,
	eStatusIcon_ShopItem_55101 = 50,
	eStatusIcon_ShopItem_55102 = 51,
	eStatusIcon_ShopItem_55103 = 52,
	eStatusIcon_ShopItem_55104 = 53,
	eStatusIcon_ShopItem_55105 = 54,
	eStatusIcon_ShopItem_55106 = 55,
	eStatusIcon_ShopItem_55107 = 56,
	eStatusIcon_ShopItem_55108 = 57,
	eStatusIcon_ShopItem_55311 = 58,
	eStatusIcon_ShopItem_55312 = 59,
	eStatusIcon_ShopItem_55322 = 60,	
	eStatusIcon_SkipEffect	= 133,
	eStatusIcon_Hide = 134,
	eStatusIcon_Detect = 135,
	eStatusIcon_QuestTime = 127,		
	eStatusIcon_GuildPlustime_SuRyun = 140,
	eStatusIcon_GuildPlustime_MuGong = 141,
	eStatusIcon_GuildPlustime_Exp = 142,
	eStatusIcon_GuildPlustime_DamageUp = 143,
	eStatusIcon_Titan_General = 165,
	eStatusIcon_Titan_Fantasy = 166,
	eStatusIcon_Max = 800,//[状态图标最大编号][By:TT天语][QQ:2013734367][2018/1/13]
};


enum eBOSSMONSTERKIND
{
	eBOSSKIND_EMPERIORTOMB = 151,
	eBOSSKIND_EMPERIORTOMB_EVENT = 158,

	eBOSSKIND_MOOUN = 181,
	eBOSSKIND_COWF = 182,
	eBOSSKIND_COWL = 183,
	eBOSSKIND_COWKING = 184,

	eBOSSKIND_GENERAL=1001,
	eBOSSKIND_YUKJI=1002,
	eBOSSKIND_MAX,
};
enum eBOSSMONSTERTYPE
{
	eBOSSTYPE_EMPERIORTOMB,
	eBOSSTYPE_MOOUN,
	eBOSSTYPE_COWF,
	eBOSSTYPE_COWL,
	eBOSSTYPE_COWKING,
	eBOSSTYPE_GENERAL,
	eBOSSTYPE_YUKJI,
	eBOSSTYPE_MAX,
};
#define RECALL_GROUP_ID			-1
#define EVENTRECALL_GROUP_ID	-2
#define QUESTRECALL_GROUP_ID	-3
enum eOPTION	
{
	eOPT_NULL		= 0,
	eOPT_NODEAL		= 1,
	eOPT_NOPARTY	= 2,
	eOPT_NOFRIEND	= 4,
	eOPT_NOVIMU		= 8,
	eOPT_NOWHISPER	= 16,
	eOPT_NOAVATARVIEW = 32,
};

#define MAXPLAYER_IN_MURIMNET	50000
#define MAXPLAYER_IN_FRONT		10000
#define MAXPLAYER_IN_CHANNEL	300
#define MAXPLAYER_IN_PLAYROOM	300
#define MAXPLAYER_PERTEAM		100

#define MAXCHANNEL_IN_MURIMNET	10000
#define MAXPLAYROOM_IN_MURIMNET	10000
#define MAXSERVER_IN_MURIMNET	100

enum eBATTLE_TEAM
{
	eTEAM_LEFT,
	eTEAM_RIGHT,
	eTEAM_OBSERVER,
	eTEAM_MAX,
};

enum eCHANNEL_MODE
{
	eCNL_MODE_ID,
	eCNL_MODE_CHANNEL,
	eCNL_MODE_PLAYROOM,
	eCNL_MODE_MAX,
};

enum ePR_CHATCLASS
{
	PRCTC_WHOLE,
	PRCTC_TEAM,
	PRCTC_WHISPER,
	PRCTC_SYSTEM,
};


enum eBATTLE_KIND
{
	eBATTLE_KIND_NONE = 0,		
	eBATTLE_KIND_MURIMFIELD,	
	eBATTLE_KIND_VIMUSTREET,
	eBATTLE_KIND_MUNPAFIELD,
	eBATTLE_KIND_SURYUN,
	eBATTLE_KIND_GTOURNAMENT,
	eBATTLE_KIND_SIEGEWAR,
	
	eBATTLE_KIND_MAX,
};

enum GRID_TYPE
{
	eGridType_General,
	eGridType_Murim, 
};

enum eMURIMBATTLE_KIND
{
	eMurimBattle_MaxKill = 0,

	eMurimBattle_Max,
};

enum Friend_ERROR_CODE
{
	eFriend_AddSuccess=0,
	eFriend_AddInvalidUser,
	eFriend_AddFullList,
	eFriend_AddDeny,
	eFriend_NotLogin,
	eFriend_AlreadyRegist,
	eFriend_DelInvaliedUser,
	eFriend_OptionNoFriend,
};

enum {
LOGIN_SUCCESS = 0,
LOGIN_ERROR_INVALID_VERSION,
LOGIN_ERROR_OVERLAPPEDLOGIN,
LOGIN_ERROR_NOAGENTSERVER,
LOGIN_ERROR_NODISTRIBUTESERVER,
LOGIN_ERROR_INVALIDUSERLEVEL,
LOGIN_ERROR_WRONGIDPW,
LOGIN_ERROR_INVALIDDISTAUTHKEY,
LOGIN_ERROR_DISTSERVERISBUSY,
LOGIN_ERROR_AGENTSERVERISBUSY,
LOGIN_ERROR_AGENTMAXCONNECTION,
LOGIN_ERROR_BLOCKUSERLEVEL,
LOGIN_ERROR_INVALID_IP,
LOGIN_ERROR_DISTCONNET_ERROR,
LOGIN_ERROR_MAXUSER,
LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,
LOGIN_ERROR_MINOR_INADULTSERVER,
LOGIN_ERROR_NOT_CLOSEBETAWINNER,
LOGIN_ERROR_SECEDEDACCOUNT,
LOGIN_ERROR_NOREMAINTIME,
LOGIN_ERROR_NOIPREGEN,
LOGIN_ERROR_NONACCADMINIP,
LOGIN_ERROR_BUDDY_WEBFAILED,
LOGIN_ERROR_BUDDY_IDAUTHFAILED,
LOGIN_ERROR_BUDDY_AUTHCODEFAILED,
LOGIN_ERROR_PROTECTIONNUMBER,
LOGIN_ERROR_PROTECTIONNUMBER_REGIST,
LOGIN_ERROR_PROTECTIONNUMBER_INSERT,
LOGIN_ERROR_DOUBLERUNCHECK,//[双开检测][By:TT天语][QQ:2013734367][2017/11/23]
};

#define MHSHADOW_OFF	0
#define MHSHADOW_CIRCLE	1
#define MHSHADOW_DETAIL	2
#define MAX_MUNPAFIELD_MEMBERNUM		30
#define	MAX_MURIMFIELD_MEMBERNUM		30
#define RECOVERTIMEUNIT					3000
#define RECOVERINTERVALUNIT				10
#define fTILE_SIZE 50.f


#define SKILLAREA_ATTR_DAMAGE			0x00000001
#define SKILLAREA_ATTR_BLOCK			0x00000002
#define SKILLAREA_ATTR_SAFETYZONE		0x00000003
#define SKILLAREA_ATTR_OTHEREFFECT		0x00000004

typedef BYTE AREATILE;
#define GetAreaTile	GetByte

#define MAX_ITEM_NUM 15000
#define MAX_ITEM_OPTION_NUM	90

#define MAX_ITEMICON_NUM	999
//[图标资源ID起始编号定义][By:TT天语][QQ:2013734367][2018/10/1]
#define IG_ITEM_STARTINDEX				3000
#define IG_MUGONG_STARTINDEX			5000
#define IG_MSGBOX_STRARTINDEX			6000
#define IG_STREETSTALLITEM_STARTINDEX	7000
#define IG_EXCHANGEITEM_STARTINDEX		8000	
#define IG_MENUSLOTITEM_STARTINDEX		9000	
#define IG_PKLOOTINGDLG_START			10000	
#define IG_DEBUG_START					15000	
#define IG_QUESTITEM_STARTINDEX			18000	
#define IG_SHOPITEM_STARTINDEX			20000	
#define IG_DEALITEM_START				30000
#define IG_MAINBARICON_START			31000
#define IG_ABILITYICON_START			40000
#define IG_ABILITYPOINT_START			60000	
#define IG_QUICKITEM_STARTINDEX			80000

#define NEWOBJECT_STARTINDEX			2000001
#define NEWRECALLOBJECT_STARTINDEX		3000001

//////////////////////////////////////////////////////////////////////////
enum TACTICSTART_ERROR_CODE
{
	eTSEC_NotTacticableState,
	eTSEC_NotTacticablePosition,
	eTSEC_AlreadyPartyDoingTactic,
	eTSEC_NoParty,
};
enum TACTICJOIN_ERROR_CODE
{
	eTJEC_NotTacticableState,
	eTJEC_NoPartyIdx,
	eTJEC_NoPartyInstance,
	eTJEC_NotPartyTacticState,
	eTJEC_NotEqualBetweenMSGnPartyTactic,
	eTJEC_NoNaeRyuk,
	eTJEC_NoTacticObject,
	eTJEC_NotEmptyPosition,
};

enum SEARCH_TYPE
{
	MP_BY_SEARCHPRICE,
	MP_I_REGISTERED,
	MP_I_JOINED,
};
enum SORT_TYPE
{
	MP_BY_WORD,
	MP_BY_PRICE,
	MP_BY_IMMEDIATEPRICE,
	MP_BY_DUEDATE,
	MP_BY_JOINAMOUNT,
};

enum CANCEL_TYPE
{
	MP_REG,
	MP_JOIN,
};

enum NPC_ROLE
{
	OBJECT_ROLE = 0,
	DEALER_ROLE = 1,
	AUCTION_ROLE = 2,
	MUNPA_ROLE = 3,
	CHANGGO_ROLE = 4,
	FYOKUK_ROLE = 5,
	TALKER_ROLE = 6,
	WANTED_ROLE = 9,
	SURYUN_ROLE = 10,
	SYMBOL_ROLE = 11,
	CASTLE_ROLE = 12,
	GUIDE_ROLE = 13,	
	TITAN_ROLE = 14,	
	BOBUSANG_ROLE = 15,	
	FORTWAR_SYMBOL_ROLE = 16,
	BOMUL_ROLE	= 23,
	MAPCHANGE_ROLE = 27,	
};


enum eMoveMode
{
	eMoveMode_Run = 0,
	eMoveMode_Walk = 1,
};

#define MAX_PACKEDDATA_SIZE 60000 // for SEND_PACKED_DATA

enum eMONSTER_ACTION
{
	eMA_STAND,
	eMA_WALKAROUND,
	eMA_PERSUIT,
	eMA_RUNAWAY,
	eMA_ATTACK,
	eMA_REST,
};

enum 
{
	eRunawayType_None,
	eRunawayType_Help = 1,
	eRunawayType_General,
	eRunawayType_Terminate,
};

enum eQUICKICON_KIND { eMUGONG_ICON=1, eITEM_ICON, eABILITY_ICON,};
enum eMUGONGITEM_KIND{eMUGONGBOOK, eJINBUBBOOK, eSIMBUBBOOK, eERROR_BOOK,};
enum eITEM_KINDBIT 
{ //[物品类型定义][By:TT天语][QQ:2013734367][2017/12/6]
	eTITAN_ITEM				= 64,
	eTITAN_ITEM_PAPER		= 65,	
	eTITAN_ITEM_RECIPE		= 66,	
	eTITAN_ITEM_PARTS		= 67,	

	eTITAN_EQUIPITEM		= 128,	
	eTITAN_EQUIPITEM_HELMET	= 129,	
	eTITAN_EQUIPITEM_ARMOR	= 130,	
	eTITAN_EQUIPITEM_GLOVES	= 131,	
	eTITAN_EQUIPITEM_LEGS	= 132,	
	eTITAN_EQUIPITEM_CLOAK	= 133,	
	eTITAN_EQUIPITEM_SHIELD	= 134,	
	eTITAN_EQUIPITEM_WEAPON	= 135,	
	//////////////////////////////////////////////////////////////////////////
	eSHOP_ITEM				= 256,
	eSHOP_ITEM_PREMIUM		= 257,	
	eSHOP_ITEM_CHARM		= 258,	//[百宝消耗][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_HERB			= 259,	//[百宝消耗][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_INCANTATION	= 260,	//[功能类][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_MAKEUP		= 261,  //[百宝头][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_DECORATION	= 262,	//[百宝衣服][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_SUNDRIES		= 263,	//[百宝消耗][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_EQUIP		= 264,  //[自定义百宝][By:TT天语][QQ:2013734367][2017/11/28]	
	eSHOP_ITEM_NOMALCLOTHES_SKIN	= 265,	
	eSHOP_ITEM_COSTUME_SKIN			= 266,	
	eSHOP_ITEM_GETIN_ITEM = 267,		//[副本劵物品类型][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_IMAGENAME  = 268,		//[图片称号物品类型][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_MARRYCHARM = 269,		//[同心符][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_GOLDITEM	  = 270,		//[元宝物品][By:TT天语][QQ:2013734367][2017/11/28]
	eSHOP_ITEM_GROWITEM	  = 271,		//[成长符咒][By:TT天语][QQ:2013734367][2017/11/28]	
	eSHOP_ITEM_AUTOADDPOINT = 272,		//[自动加点卷][By:TT天语][QQ:2013734367][2018/4/17]
	eSHOP_ITEM_TITAN_EQUIP	= 290,	

	eSHOP_ITEM_PET			= 300,	
	eSHOP_ITEM_PET_EQUIP	= 310,	

	eYOUNGYAK_ITEM			= 512,
	eYOUNGYAK_ITEM_PET		= 513,	
	eYOUNGYAK_ITEM_UPGRADE_PET	= 514,
	eYOUNGYAK_ITEM_REVIVAL_PET	= 515,	

	eYOUNGYAK_ITEM_TITAN	= 555,	

	eMUGONG_ITEM			= 1024,	
	eMUGONG_ITEM_SIMBUB		= 1037,
	eMUGONG_ITEM_JINBUB		= 1038,
	eMUGONG_ITEM_KYUNGGONG	= 1039,
	eMUGONG_ITEM_OPTION		= 1040,
	eMUGONG_ITEM_TITAN		= 1041,	
	eMUGONG_ITEM_JOB		= 1042,	
	eEQUIP_ITEM				= 2048,
	eEQUIP_ITEM_WEAPON		= 2049,	//[武器][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_DRESS		= 2055,	//[衣服][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_HAT			= 2056,	//[帽子][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_SHOES		= 2057,	//[鞋子][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_RING		= 2062,	//[戒指][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_CAPE		= 2059,	//[斗篷][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_NECKLACE	= 2061,	//[项链][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_ARMLET		= 2058,	//[护腕][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_BELT		= 2060,	//[腰带][By:TT天语][QQ:2013734367][2018/2/1]
	eEQUIP_ITEM_UNIQUE		= 2100,	//[祝福武器][By:TT天语][QQ:2013734367][2018/2/1]


	eEXTRA_ITEM				= 4096,
	eEXTRA_ITEM_JEWEL		= 4097,
	eEXTRA_ITEM_MATERIAL	= 4098,
	eEXTRA_ITEM_METAL		= 4099,
	eEXTRA_ITEM_BOOK		= 4100,
	eEXTRA_ITEM_HERB		= 4101,
	eEXTRA_ITEM_ETC			= 4102,
	eEXTRA_ITEM_COMP		= 4104,
	eEXTRA_ITEM_QUEST		= 4105,
	eEXTRA_ITEM_USABLE		= 4106,
	
	eEXTRA_ITEM_UPGRADE			= 8192,
	eEXTRA_ITEM_UPGRADE_ATTACK	= 8193,
	eEXTRA_ITEM_UPGRADE_DEFENSE	= 8194,

	eQUEST_ITEM			= 16384,
	eQUEST_ITEM_START	= 16385,	 
	eQUEST_ITEM_EQUIP	= 16386,	
	eQUEST_ITEM_PET		= 16400,	
	eQUEST_ITEM_PET_EQUIP = 16410,	
	
	eCHANGE_ITEM		= 32768,	
	eCHANGE_ITEM_LOCK	= 32769,	
	eEXTRA_ITEM_KEY		= 32770,//[箱子钥匙][By:TT天语][QQ:2013734367][2018/8/17]
	eEQUIP_ITEM_INHERIT = 32771,	//[装备传承][BY:TT天语][QQ:2013734367][2019-8-22][13:11:28]

	eKIND_ITEM_MAX,
};

#define MAX_QUERY_RESULT 100
#define MAX_JOB		3
#define MAX_ATTACKMUGONG_NUM	10
#define MAX_NAEGONG_NUM	10
#define MAX_KYUNGGONG_NUM	10
#define MAX_MUGONGITEM_NUM	10
#define MAX_YOUNGYAKITEM_NUM	10
#define MAX_EQUIPITEM_NUM	300

#define MAX_PLAYER_NUM	200
#define MAX_PET_NUM		100	//[宠物最大编号][By:TT天语][QQ:2013734367][2018/1/10]

#define MAX_MONSTER_NUM 1000 //[怪物最大编号][By:TT天语][QQ:2013734367][2018/1/10]
#define MAX_MONSTER_REGEN_NUM 100
#define MAX_NPC_NUM 135		//[NPC最大编号][By:TT天语][QQ:2013734367][2018/1/10]
#define MAX_DROPITEM_NUM 20
#define MAX_NPC_REGEN_NUM 100
#define MONSTER_ATTACKTYPE1		0
#define MONSTER_ATTACKTYPE2		1
#define MONSTER_ATTACKTYPE3		2
#define MAX_MONSTERATTACK_TYPE  3

#define MAX_MONSTER_GROUPNUM	200
#define MAX_NPC_GROUPNUM		200

#define MAX_CHARACTER_LEVEL_NUM 99	//[角色最大等级][By:TT天语][QQ:2013734367][2018/5/16]  

#define MAX_PLAYERLEVEL_NUM	99       

#define MAX_MONSTERLEVELPOINTRESTRICT_NUM	9
#define MONSTERLEVELRESTRICT_LOWSTARTNUM	6

enum WP_KIND{WP_GUM=1,WP_GWUN,WP_DO,WP_CHANG,WP_GUNG,WP_AMGI,WP_KEY,WP_EVENT,WP_EVENT_HAMMER,WP_MAX,WP_ERR,WP_WITHOUT=0};
enum WP_ATTR{WP_FIRE=1, WP_WATER, WP_TREE, WP_GOLD, WP_EARTH, WP_NONEATTR};
enum RE_TARGET_RANGE{TR_PERSONAL, TR_WHOLE,};
enum MUGONG_TYPE{MUGONGTYPE_NORMAL, MUGONGTYPE_JINBUB, MUGONGTYPE_SIMBUB, MUGONGTYPE_JOB, MUGONGTYPE_MAX,};

#define COMBO_GUM_MIN		1
#define COMBO_GWUN_MIN		7
#define COMBO_DO_MIN		13
#define COMBO_CHANG_MIN		19
#define COMBO_GUNG_MIN		25
#define COMBO_AMGI_MIN		31
#define COMBO_EVENT_MIN		50	
#define COMBO_EVENT_HAMMER	51

#define MAX_COMBO_NUM		6
#define SKILL_COMBO_NUM		100
#define MAX_COMBOATTACK_LIST (WP_MAX*MAX_COMBO_NUM)		

#define MAX_SKILLATTACK_LIST (WP_MAX*50)				
#define MAX_SUNG_EXP 7								
#define MAX_SUNG 12									

#define SKILLRESULT_ATTACK				0x00000001
#define SKILLRESULT_RECOVERLIFE			0x00000010
#define SKILLRESULT_RECOVERNAERYUK		0x00000100

#define SKILLTARGETTYPE_ENEMY			0
#define SKILLTARGETTYPE_TEAMMATE		1
#define SKILLTARGETTYPE_SELF			2
#define SKILLTARGETTYPE_ENEMYANDTEAM	3
#define SKILLTARGETTYPE_NOTENEMYTARGET	4

#define SKILLRESULTKIND_NONE			0
#define SKILLRESULTKIND_POSITIVE		1
#define SKILLRESULTKIND_NEGATIVE		2

#define ATTACKTYPE_MELEE				0
#define ATTACKTYPE_RANGE				0

#define ATTACKTYPE_NORMAL		0
#define ATTACKTYPE_HALFCIRCLE	1
#define ATTACKTYPE_CIRCLE		2
#define ATTACKTYPE_LINE			3
#define ATTACKTYPE_RECTANGLE	4

#define	TARGETTYPE_TARGET	0
#define TARGETTYPE_ATTACKER 1
#define TARGETTYPE_GROUND	2

enum eMUGONG_KIND
{ 
	eATTACK_MUGONG, 
	eDEFENSE_MUGONG, 
	eASSIST_MUGONG, 
	eSPECIAL_MUGONG, 
	eNAEGONG,
	eKYUNGGONG,
	eMAX_MUGONG, 
	eERROR_MUGONG
};

enum eItemUse_Err
{
	eItemUseSuccess,

	eItemUseErr_PreInsert,
	eItemUseErr_Move,
	eItemUseErr_Combine,
	eItemUseErr_Divide,
	eItemUseErr_Discard,
	eItemUseErr_Mix,
	eItemUseErr_Quest,
	eItemUseErr_Err,
	eItemUseErr_Upgrade,
	eItemUseErr_Lock,
	eItemUseErr_AlreadyUse,
	eItemUseErr_Unabletime,
	eItemUseErr_AlreadyPoint,
	eItemUseErr_AlredyChange,
	eItemuseErr_DontUseToday,
	eItemUseErr_DontDupUse,
	eItemUseErr_UseFull,
	eItemUseErr_TitanPartsMake,
	eItemUseErr_TitanItemTime,
	eItemUseErr_NotBoxKey,//[箱子钥匙][By:TT天语][QQ:2013734367][2018/8/18]
	eItemUseErr_MixNotPace=68,
};
#define MAX_YOUNGYAKITEM_DUPNUM 100   //[最大物品叠加数量][By:TT天语][QQ:2013734367][2018/8/18]

#define MAX_CHARACTER_NUM	4


#define MAX_BOARDTITLE_NUM	10
#define MAX_BOARDTITLE_LENGTH 64
#define MAX_CONTENTS_LENGTH	65535

#define MAX_STREETSTALL_TITLELEN 66

#define MAX_FILE_NAME		32


#define WALKSPEED		200
#define RUNSPEED		400

#define TITAN_WALKSPEED	300

#define ManBlueDress		23010 
#define ManBlueNothing		23011
#define ManDefaultShoose	27000 
#define WomanRedDress		23010
#define WomanDefalutShoose	27000 


enum { GENGOL_POINT, SIMMEK_POINT, MINCHUB_POINT, CHERYUK_POINT, MAX_BTN_POINT};

#define GENDER_MALE 0
#define GENDER_FEMALE 1
#define GENDER_MAX 2

#define MAX_PARTY_LISTNUM 7
#define MAX_PARTY_LOAD_NUM 100
#define MAX_PARTY_NAME 28		

#define MAX_MUNPA_NAME 20
#define MAX_MUNPALOAD_LIST	100
#define MAX_MUNPA_LIST	10
#define MAX_MUNPA_INTROMSG	568
#define MAX_MUNPA_IMAGE_NUM 32

#define MUNPA_MASTER		20
#define MUNPA_NOTMEMBER		0	A
#define MUNPA_SEMIMEMBER	1	
#define MUNPA_MEMBER		2	
#define MUNPA_SENIOR		3	
#define MUNPA_VICE_MASTER	10	

#define MUNPA_KIND_JUNG	1	
#define MUNPA_KIND_SA	2	
#define MUNPA_KIND_WHANG	3	
#define MUNPA_KIND_MU	4	

#define MAX_MUNPA_BOARD_NUM	5	
#define MAX_MUNPA_BOARD_NAME 16		
#define MAX_MUNPA_REGULARMEMBERLIST_NUM 10 
#define MAX_MUNPA_BATTLEKIND 3
#define MAX_MUNPA_SEMIMEMBERLIST_NUM 10 
#define MAX_MUNPA_BOARD_SUBJECT 64
#define MAX_MUNPA_BOARD_CONTENT 2048
#define MAX_MUNPA_BOARD_LIST 10

#define MAX_MUNPA_WARE_TABNUM 7
#define MAX_PYOGUKLIST_NUM 5
#define MAX_PYOGUKLOAD_ITEM 100
#define MAX_OPTIONLOAD_ITEM 100
#define MAX_SHOPLOAD_ITEM	100
#define MAX_LOAD_ITEM		100
#define MAX_PETINVENLOAD_ITEM 100
#define MAX_PINVENLOAD_ITEM 100
#define MAX_TITANENDURANCELOAD_ITEM 100	
#define MAX_FRIEND_NUM 30
#define MAX_FRIEND_LIST 10
#define MAX_FRIEND_PAGE MAX_FRIEND_NUM/MAX_FRIEND_LIST // 30/10 = 10

#define MAX_NOTENUM 40
#define MAX_NOTE_PAGE 4
#define NOTENUM_PER_PAGE MAX_NOTENUM/MAX_NOTE_PAGE // 30/3 = 10
#define MAX_NOTE_LENGTH 255

#define MAX_CHANNEL_NUM 10
#define MAX_CHANNEL_NAME 16

#define MAX_TATIC_ABILITY_NUM 10

enum EWEARED_ITEM
{
	eWearedItem_Hat = 0,
	eWearedItem_Weapon = 1,
	eWearedItem_Dress = 2,
	eWearedItem_Shoes = 3,
	
	eWearedItem_Ring1 = 4,
	eWearedItem_Ring2 = 5,

	eWearedItem_Cape = 6,
	eWearedItem_Necklace = 7,
	eWearedItem_Armlet = 8,
	eWearedItem_Belt = 9,

	eWearedItem_Max,
};


enum TITAN_KIND
{
	TITANKIND_1,
	TITANKIND_MAX,
};


enum TITAN_WEARED_ITEM
{
	eTitanWearedItem_HelMet,
	eTitanWearedItem_Body,
	eTitanWearedItem_Glove,
	eTitanWearedItem_Legs,
	eTitanWearedItem_Cloak,
	eTitanWearedItem_Shield,
	eTitanWearedItem_Weapon,
	eTitanWearedItem_Max,
};

enum WantedBuyErrCode
{
	eBWR_Success,
	eBWR_Err_Err,
	eBWR_Err_FullVolunteer, 
};
#define MAX_WANTED_NUM 20
#define MAX_WANTED_VOLUNTEER 100
#define WANTUNIT_PER_PAGE 12

enum ERROR_GUILD
{
	eGuildErr_Err = 1,
			
	eGuildErr_Create_Name,
	eGuildErr_Create_Intro,
	
	eGuildErr_BreakUp,
	
	eGuildErr_DeleteMember,
	eGuildErr_DeleteMember_NothaveAuthority,
	eGuildErr_DeleteMember_NotMember,
	
	eGuildErr_AddMember_NotPlayer,
	eGuildErr_AddMember_OtherGuild,
	eGuildErr_AddMember_AlreadyMember,
	eGuildErr_AddMember_FullMember,	
	eGuildErr_AddMember_TargetNotDay,
	eGuildErr_AddMember_NothaveAuthority,

	eGuildErr_InviteApt_NoMaster,

	eGuildErr_Mark_NoGuild,
	eGuildErr_Mark_NoMark,

	eGuildErr_LevelUp_NotAbil,
	eGuildErr_LevelUp_NotMoney,
	eGuildErr_LevelUp_Complete,

	eGuildErr_ChangeRank_NoGuild,
	eGuildErr_ChangeRank_NotMember,

	eGuildErr_NoGuild,

	eGuildErr_Nick_Filter,
	eGuildErr_Nick_NotMember,
	eGuildErr_Nick_NotAvailableName,
	eGuildErr_Nick_NotLogIn,

	eGuildErr_AddStudent_TooHighLevelAsStudent,
};
enum
{
	eGuild_Delete,
	eGuild_Secede,		
};
enum DEAL_BUY_ERROR { NOT_ERROR, NOT_MONEY = 101, NOT_SPACE, NOT_EXIST, NOT_PLAYER, NOT_OWNERMONEY, NOT_SAMEPRICE, NOT_SAMEINFO, NO_DEMANDITEM, NOT_REMAINITEM,NOT_LIMITTIMES };
enum ERROR_MUNPABOARDCONTENTS {
	ERROR_MUNPABOARDCONTENTS_NOCONTENTS = 1,
	ERROR_MUNPABOARDCONTENTS_NOAUTHORITY,
};

enum ERROR_PARTY 
{
	eErr_Create_DB,
	eErr_Add_NoPlayer,
	eErr_Add_AlreadyinParty,
	eErr_Add_OptionNoParty,
	eErr_Add_DiedPlayer,
	eErr_ChangingMaster,
	eErr_Add_Full,
	eErr_BrokenParty, 
	eErr_Create_MapHasParty, 
	eErr_Add_NotOurGuild,		
};

enum ERROR_PARTY_REQUEST
{
	eErr_Request_NotParty,			
	eErr_Request_NotMaster,			
	eErr_Request_PartyExistence,	
	eErr_Request_Full,				
	eErr_Request_Level,				
	eErr_Request_Public,			
	eErr_Request_NotState,			
	eErr_Request_Progress,	      
	eErr_Request_Refusal,	      
	eErr_Request_TimeExcess,	  
};

enum PARTY_OPTION
{
	ePartyOpt_Random,
	ePartyOpt_Damage,
	ePartyOpt_Sequence,
};

enum TABNUMBER
{
	TAB_INVENTORY_NUM			= 4,
	TAB_MUNPAWAREHOUSE_NUM		= 7,
	TAB_PYOGUK_NUM				= 5,
	TAB_QUICK_NUM				= 4,
	TAB_ABILITY_NUM				= 4,	
	TAB_MENUSLOT_NUM			= 4,	
	TAB_QITEMINVEN_NUM			= 4,
	TAB_GUILDWAREHOUSE_NUM		= 2,		
	TAB_SHOPITEM_NUM			= 5,
	TAB_SHOPINVEN_NUM			= 2,
	TAB_PET_INVENTORY_NUM		= 3,
};

enum CELLPERTAB
{
	TABCELL_INVENTORY_NUM		= 20,
	TABCELL_MUNPAWAREHOUSE_NUM	= 45,
	TABCELL_PYOGUK_NUM			= 30,
	TABCELL_QUICK_NUM			= 10,
	TABCELL_ABILITY_NUM			= 40,
	TABCELL_MENUSLOT_NUM		= 7,	
	TABCELL_QITEMINVEN_NUM		= 20,
	TABCELL_GUILDWAREHOUSE_NUM	= 30,
	TABCELL_SHOPITEM_NUM		= 30,	
	TABCELL_SHOPINVEN_NUM		= 20,
	TABCELL_PETINVEN_NUM		= 20,
	TABCELL_SEIGEFORTWAREHOUES_NUM	= 30,
	TABCELL_SHOPINVEN_PLUS_NUM		= 40,
};


enum
{
	SLOT_INVENTORY_NUM			= TAB_INVENTORY_NUM * TABCELL_INVENTORY_NUM,//80
	SLOT_WEAR_NUM				= 10,

	SLOT_MUNPAWAREHOUSE_NUM		= TAB_MUNPAWAREHOUSE_NUM * TABCELL_MUNPAWAREHOUSE_NUM,	//315
	SLOT_PYOGUK_NUM				= TAB_PYOGUK_NUM * TABCELL_PYOGUK_NUM,//150

	SLOT_QUICK_NUM				= TAB_QUICK_NUM * TABCELL_QUICK_NUM,//11

	SLOT_ABILITY_NUM			= TAB_ABILITY_NUM * TABCELL_ABILITY_NUM,

	SLOT_QITEMINVEN_NUM			= TAB_QITEMINVEN_NUM * TABCELL_QITEMINVEN_NUM,//100
	
	SLOT_GUILDWAREHOUSE_NUM		= TAB_GUILDWAREHOUSE_NUM * TABCELL_GUILDWAREHOUSE_NUM,	//60

	SLOT_SHOPITEM_NUM			= TAB_SHOPITEM_NUM * TABCELL_SHOPITEM_NUM,
	
	SLOT_SHOPINVEN_NUM			= /*TAB_SHOPINVEN_NUM **/ TABCELL_SHOPINVEN_NUM,
	SLOT_WEAR_SHOPITEM_NUM		= 10,

	SLOT_STREETSTALL_NUM		= 25,
	SLOT_STREETBUYSTALL_NUM		= 5,	
	SLOT_MUGONG_NUM				= 25,	
	SLOT_JINBUB_NUM				= 5,		// 5
	SLOT_TITAN_NUM				= 25,
	SLOT_MUGONGTOTAL_NUM		= SLOT_MUGONG_NUM+SLOT_MUGONG_NUM+SLOT_JINBUB_NUM+SLOT_TITAN_NUM,		// 80	
	
	SLOT_PETINVEN_NUM			= TAB_PET_INVENTORY_NUM * TABCELL_PETINVEN_NUM,	//60
	SLOT_PETWEAR_NUM			= 3,

	SLOT_NPCINVEN_NUM			= 30,

	SLOT_TITANWEAR_NUM		= 7,

	SLOT_TITANSHOPITEM_NUM	= 4,

	SLOT_TITANMUGONG_NUM	= 25,	

	SLOT_SEIGEFORTWARE_NUM		= 90,
};

enum
{
	TP_INVENTORY_START			= 0,
	TP_INVENTORY_END			= TP_INVENTORY_START		+ SLOT_INVENTORY_NUM,		// 80
	TP_WEAR_START				= TP_INVENTORY_END,										// 80
	TP_WEAR_END					= TP_WEAR_START				+ SLOT_WEAR_NUM,			// 90

	TP_PYOGUK_START				= TP_WEAR_END,											// 90
	TP_PYOGUK_END				= TP_PYOGUK_START			+ SLOT_PYOGUK_NUM,			// 240

	TP_SHOPITEM_START			= TP_PYOGUK_END,										// 240
	TP_SHOPITEM_END				= TP_SHOPITEM_START			+ SLOT_SHOPITEM_NUM,		// 390

	TP_SHOPINVEN_START			= TP_SHOPITEM_END,										// 390
	TP_SHOPINVEN_END			= TP_SHOPINVEN_START		+ SLOT_SHOPINVEN_NUM+TABCELL_SHOPINVEN_PLUS_NUM,		// 430
	
	TP_PETINVEN_START			= TP_SHOPINVEN_END,										// 430
	TP_PETINVEN_END				= TP_PETINVEN_START			+ SLOT_PETINVEN_NUM,		// 490

	TP_PETWEAR_START			= TP_PETINVEN_END,										// 490
	TP_PETWEAR_END				= TP_PETWEAR_START			+ SLOT_PETWEAR_NUM,			// 493

	TP_TITANWEAR_START			= TP_PETWEAR_END,										// 493
	TP_TITANWEAR_END			= TP_TITANWEAR_START		+ SLOT_TITANWEAR_NUM,		// 500

	TP_TITANSHOPITEM_START		= TP_TITANWEAR_END,										// 500
	TP_TITANSHOPITEM_END		= TP_TITANSHOPITEM_START	+ SLOT_TITANSHOPITEM_NUM,	// 504

	TP_MAX						= TP_TITANSHOPITEM_END,									// 504

	TP_GUILDWAREHOUSE_START		= TP_MAX,													// 510		// 430
	TP_GUILDWAREHOUSE_END		= TP_GUILDWAREHOUSE_START + SLOT_GUILDWAREHOUSE_NUM,	// 570		// 490
	
	TP_MUGONG1_START			= TP_GUILDWAREHOUSE_END,													// 600		// 555
	TP_MUGONG1_END				= TP_MUGONG1_START			+ SLOT_MUGONG_NUM,			// 625		// 580

	TP_MUGONG2_START			= TP_MUGONG1_END,										// 625		// 580
	TP_MUGONG2_END				= TP_MUGONG2_START			+ SLOT_MUGONG_NUM,			// 650		// 605

	TP_JINBUB_START				= TP_MUGONG2_END,										// 650		// 490
	TP_JINBUB_END				= TP_JINBUB_START			+ SLOT_JINBUB_NUM,			// 655		// 495

	TP_TITANMUGONG_START		= TP_JINBUB_END,										// 655
	TP_TITANMUGONG_END			= TP_TITANMUGONG_START		+ SLOT_TITANMUGONG_NUM,		// 680

	TP_QUICK_START				= TP_TITANMUGONG_END,													// 700		// 605
	TP_QUICK_END				= TP_QUICK_START			+ SLOT_QUICK_NUM,			// 740		// 645

	TP_ABILITY_START			= TP_QUICK_END,											// 745		// 645
	TP_ABILITY_END				= TP_ABILITY_START			+ SLOT_ABILITY_NUM,			// 905		// 805


	TP_SIEGEFORTWARE_START		= 10000,
	TP_SIEGEFORTWARE_END		= TP_SIEGEFORTWARE_START + SLOT_SEIGEFORTWARE_NUM,		
};


enum eITEMTABLE
{
	eItemTable_Inventory,
	eItemTable_Weared,
	eItemTable_Pyoguk,
	eItemTable_Shop,
	eItemTable_ShopInven,
	eItemTable_PetInven,
	eItemTable_PetWeared,
	eItemTable_Titan,
	eItemTable_TitanShopItem,	
	eItemTable_TableMax,
	eItemTable_MunpaWarehouse,

	eItemTable_StreetStall,
	eItemTable_Exchange,
	eItemTable_Mix,
	eItemTable_RecvExchange,
	
	eItemTable_Mugong,
	eItemTable_Quick,
	eItemTable_Deal,
	eItemTable_Ability,

	eItemTable_Max,
};

#define eItemTable_GuildWarehouse eItemTable_MunpaWarehouse

enum MONEY_FLAG {
	MF_NOMAL = 0, MF_OBTAIN, MF_LOST, MF_NONE, MF_PUTINGUILD = 20, MF_PUTOUTGUILD
};



enum EMAP_NUMBER
{
	eMapNum_BukKyung_Town,
	eMapNum_BukKyung_Field,
	eMapNum_BukKyung_Dungeon,

	eMapNum_NakYang_Town,
	eMapNum_NakYang_Field,
	eMapNum_NakYang_Dungeon,

	eMapNum_GaeBong_Town,
	eMapNum_GaeBong_Field,
	eMapNum_GaeBong_Dungeon,
		
	eMapNum_JangAn_Town,
	eMapNum_JangAn_Field,
	eMapNum_JangAn_Dungeon,
	
	eMapNum_Max,
};

enum EAPPEAR_PART
{
	eAppearPart_Hair,
	eAppearPart_Face,
	eAppearPart_Body,
	eAppearPart_Hand,
	eAppearPart_Foot,
	eAppearPart_Weapon,
	
	eAppearPart_Max
};

#define MIN_MUGONG_INDEX		100			//[最小武功编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_MUGONG_INDEX		4000		//[最大武功编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_NAEGONG_INDEX		6000		//[最小转职技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_NAEGONG_INDEX		7000		//[最大转职技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_KYUNGGONG_INDEX		7000		//[最小轻工技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_KYUNGGONG_INDEX		8000		//[最大轻工技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_ATTACK_MUGONG_INDEX		100		//[最小属性技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_ATTACK_MUGONG_INDEX		2000	//[最大属性技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_DEFENSE_MUGONG_INDEX	2000	//[最小防御技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_DEFENSE_MUGONG_INDEX	3000	//[最大防御机能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_ASSIST_MUGONG_INDEX		3000	//[最小辅助技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_ASSIST_MUGONG_INDEX		4000	//[最大辅助技能编号][By:TT天语][QQ:2013734367][2018/1/29]
#define ATTACK_MUGONG_INTERVAL		200		//[攻击技能间隔][By:TT天语][QQ:2013734367][2018/1/29]
#define DEFENSE_MUGONG_INTERVAL		100		//[防御机能间隔][By:TT天语][QQ:2013734367][2018/1/29]
#define ASSIST_MUGONG_INTERVAL		100		//[辅助技能间隔][By:TT天语][QQ:2013734367][2018/1/29]

#define MIN_GRIDITEM_AREA		0			
#define MAX_GRIDITEM_AREA		1000		


#define MIN_YOUNGYAKITEM_INDEX	8000		//[药品类型最小编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_YOUNGYAKITEM_INDEX	9000		//[药品类型最大编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_MUGONGITEM_INDEX	9000		//[技能最小编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_MUGONGITEM_INDEX	11000		//[技能最大编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MIN_EQUIPMENTITEM_INDEX 11000		//[装备最小编号][By:TT天语][QQ:2013734367][2018/1/29]
#define MAX_EQUIPMENTITEM_INDEX 32000		//[装备最大编号][By:TT天语][QQ:2013734367][2018/1/29]
// ---- AbilityIndex
#define MIN_ABILITY_INDEX	60000			
#define MAX_ABILITY_INDEX	61000


#define WALKSPEED		200
#define RUNSPEED		400


#define MOVE_LIMIT_ERRORDISTANCE	1000

#define TEMPITEMDBIDX 0xffffffff
#define MAX_SERVERINFO_LIST		100

#define MAX_NAME_LENGTH				16
#define MAX_MONSTER_NAME_LENGTH		60
#define MAX_CHAT_LENGTH				127
#define	MAX_CHANNELTITLE_LENGTH		64
#define	MAX_PLAYROOMTITLE_LENGTH	64
#define MAX_ITEMNAME_LENGTH			32
#define MAX_PETNAME_LENGTH			32		
#define MAX_ITEM_ABRASION			10000	//[装备持久最大值][By:TT天语][QQ:2013734367][2018/1/6]
#define MAX_CHXNAME_LENGTH			32

#define MIN_GRIDITEM_AREA			0
#define MAX_GRIDITEM_AREA			1000
 
#define MAX_PICKITEM_DISTANCE	500

#define MAX_AUCTIONBOARD_DISTANCE 500


#define MAX_BOARDTITLE_NUM	10
#define MAX_BOARDTITLE_LENGTH 64
#define MAX_CONTENTS_LENGTH	65535


#define MAX_TARGET_NUM		300

enum {GENERAL_ATTACK=0,MUGONG_ATTACK=1};
#define POINT_VALID_DISTANCE		3000		


#define REMOVEFADE_START_TIME		2500
#define MONSTERREMOVE_TIME			5000
#define PLAYERREVIVE_TIME			5000
#define	PLAYERREVIVE_TIME_IN_BOSSMAP	60000

enum BASICEFFECT
{
	eEffect_NewCharacter=1,
	eEffect_Standard,	
	eEffect_Walk,
	eEffect_Run,	
	eEffect_BattleWalk_Gum,		
	eEffect_BattleWalk_Gwun,	
	eEffect_BattleWalk_Do,	
	eEffect_BattleWalk_Chang,		
	eEffect_BattleWalk_Gung,		
	eEffect_BattleWalk_Amgi,		
	eEffect_BattleRun_Gum,		
	eEffect_BattleRun_Gwon,		
	eEffect_BattleRun_Do,		
	eEffect_BattleRun_Chang,		
	eEffect_BattleRun_Gung,		
	eEffect_BattleRun_Amgi,		
	eEffect_UngijosikStart,	
	eEffect_Ungijosik,
	eEffect_Ungijosik_End,
	eEffect_UngijosikDamage,	
	eEffect_StallStart,	
	eEffect_Stall,	
	eEffect_StallEnd,	
	eEffect_Damage1,		
	eEffect_Damage2,	
	eEffect_Damage3,	
	eEffect_Die1,	
	eEffect_Died1,	
	eEffect_Die2,		
	eEffect_Died2,		
	eEffect_LevelUpSentence,	
	eEffect_MouseMovePoint,	
	eEffect_GetMoney = 28,	
	eEffect_GetItem_Accessory = 30,	
	eEffect_Emoticon_Bow,	
	eEffect_Emoticon_Thanks,
	eEffect_Emoticon_Yes,	
	eEffect_Emoticon_No,	
	eEffect_Emoticon_Pleasure,
	eEffect_Emoticon_Sadness,
	eEffect_Emoticon_Love,	
	eEffect_Emoticon_Hostility,	
	eEffect_Emoticon_Disappoint,	
	eEffect_Emoticon_Angry,	
	eEffect_Emoticon_Suprised,
	eEffect_Emoticon_Evasion,	
	eEffect_Emoticon_Declare,	
	eEffect_Defence_Gum,		
	eEffect_Defence_Gwun,	
	eEffect_Defence_Do,		
	eEffect_Defence_Chang,		
	eEffect_Defence_Gung,		
	eEffect_Defence_Amgi,		
	eEffect_Mugong_GetMugong = 68,		
	eEffect_UseItem_LifeYoungYak = 70,
	eEffect_UseItem_ManaYoungYak,
	eEffect_GetItem_Weapon,				
	eEffect_GetItem_Dress,
	eEffect_GetItem_Quest,
	eEffect_GetItem_Youngyak,
	eEffect_Mugong_ChangeMugong,	
	eEffect_UseItem_HosinYoungYak,		
	eEffect_UseItem_FullYoungYak,		
	eEffect_MonRegen = 80,
	eEffect_PK_LootExp,					
	eEffect_ChangeStage_Hwa = 98,
	eEffect_ChangeStage_Geuk = 99,
	eEffect_KyungGong1 = 200,			
	eEffect_KyungGong2 = 203,			
	eEffect_KyungGong3 = 206,			
	eEffect_Jackpot = 1034,				
	eEffect_PetSummon = 3151,			
	eEffect_PetSeal		= 3152,			
	eEffect_FeedUp		= 3153,			
	eEffect_PetWarp		= 3154,		
	eEffect_ShopItem_Life = 3200,		
	eEffect_ShopItem_Shield,	
	eEffect_ShopItem_NaeRuyk,			
	eEffect_ShopItem_Revive,			
	eEffect_ShopItem_Boom1,				
	eEffect_ShopItem_Boom2,			
	eEffect_ShopItem_Hair1,				
	eEffect_ShopItem_Hair2,				
	eEffect_ShopItem_Hair3,				
	eEffect_ShopItem_Teleport1,			
	eEffect_ShopItem_Teleport2,			
	eEffect_ShopItem_Teleport3,			
	eEffect_ShopItem_MaxLife,			
	eEffect_ShopItem_MaxShield,			
	eEffect_ShopItem_MaxNaeRuyk,		
	eEffect_Titan_Damage1 = 3264,		
	eEffect_Titan_Damage2,				
	eEffect_Titan_Damage3,				
	eEffect_Titan_GetMoney = 3268,	
	eEffect_Titan_GetItem_Accessory = 3270,	
	eEffect_TitanLevelUpSentence,		
	eEffect_Titan_GetMugong = 3308,		
	eEffect_Titan_UseItem_LifeYoungYak = 3310,	
	eEffect_Titan_UseItem_ManaYoungYak,			
	eEffect_Titan_GetItem_Weapon,			
	eEffect_Titan_GetItem_Dress,		
	eEffect_Titan_GetItem_Quest,		
	eEffect_Titan_GetItem_Youngyak,			
	eEffect_Titan_ChangeMugong,				
	eEffect_Titan_UseItem_HosinYoungYak,	
	eEffect_Titan_UseItem_FullYoungYak,
	eEffect_Titan_PK_LootExp,			

	eEffect_Titan_RideIn = 3624,	
	eEffect_Titan_GetOff,
	eEffect_Titan_ForcedGetOff,

	eEffect_Titan_KyungGong1 = 3629,	
	eEffect_Titan_KyungGong2 = 3632,
	eEffect_Titan_KyungGong3 = 3635,

	eEffect_Titan_Shop_KyungGong1 = 3671,
	eEffect_Titan_Shop_KyungGong2 = 3673,
	eEffect_Titan_Shop_KyungGong3 = 3675,	
};

enum
{
	bukkyung = 1,	
	taesan,			
	namdae,			
	sanhea,			
	nakyang_siege,		
	nakyang,	
	gaebong,		
	jungju,		
	hoangsan,		
	sungsan,		
	youngmun,		
	jangan,		
	hoajung,		
	malijangsung,		
	jisi,			
	gaebong_siege,		
	nanju,			
	donhoang_night,		
	okmun,			
	myungsa,		
	hyungju,	
	makgogul2F,		
	jangsa,		
	bukkyung_siege,		
	dongjung,		
	akyang,			
	hangju,			
	Tournament,			
	sangju,			
	hogu,			
	bota,			
	hopo,			
	PKEVENTMAP,		
	ami,		
	hoangha,		
	sunyung,	
	guul,			
	hanbing,	
	gonryun,	
	tabi,			
	hohwa,			
	SURVIVALMAP = 42,
	daehung,		
	kan,			
	uru,		
	hwayuam,	
	bokju,			
	dwang,			
	dery,			
	samtop,			
	aesan,			
	sussang,		
	whangkua,		
	gooknae,		
	jangbeak,	
	janggun,	
	sangkyung,		
	dongkyung,		
	jisi_mid,		
	wallachun = 60,
	RUNNINGMAP = 61,
	jangan_siege = 62,
	hoangha2	= 64,
	donhoang_day=65,		
	
	hwanghajubyun=66,
	yongmunibgu = 67,

	taesanjungsang = 68,
	taesannorth = 69,
	sorimsa		= 70,

	BOSSMONSTER_MAP = 72,
	QuestRoom = 73,
	BOSSMONSTER_2NDMAP = 75,

	DIMENSION0 = 95,
	DIMENSION = 96,

	TITANMAP_EUNCHUN = 101,
	TITANMAP_TRAVOK	= 102,
	TITANMAP_GODDNESS = 104,
	TITANMAP_RWAIN = 105,
	//这里缺失少地图
	TITANMAP_Central_Plain = 106,

	Nandaihe = 108,
	Huoyandong1 = 109,
	Huoyandong2 = 110,


	MAX_MAP_ENUMCOUNT,
};

enum EObjectState
{
	eObjectState_None,						
	eObjectState_Enter,					
	eObjectState_Move,					
	eObjectState_Ungijosik,					
	eObjectState_Tactic,					
	eObjectState_Rest,						
	
	eObjectState_Deal,						

	eObjectState_Exchange,					
	eObjectState_StreetStall_Owner,			
	eObjectState_StreetStall_Guest,			
	eObjectState_PrivateWarehouse,		
	eObjectState_Munpa,				

	eObjectState_SkillStart,			
	eObjectState_SkillSyn,					
	eObjectState_SkillBinding,				
	eObjectState_SkillUsing,				
	eObjectState_SkillDelay,		
	
	eObjectState_TiedUp_CanMove,		
	eObjectState_TiedUp_CanSkill,
	eObjectState_TiedUp,
	
	eObjectState_Die,					
	
	eObjectState_BattleReady,
	eObjectState_Exit,					
	eObjectState_Immortal,			

	eObjectState_Society,				
	
	eObjectState_ItemUse,

	eObjectState_TournamentReady,
	eObjectState_TournamentProcess,
	eObjectState_TournamentEnd,
	eObjectState_TournamentDead,

	eObjectState_Engrave,

	eObjectState_TitanRecall,

	eObjectState_MAX,	
};

enum eEXCHANGE_STATE
{
	eEXSTT_NONE,
	eEXSTT_APPLYING,	
	eEXSTT_APPLIED,		
	eEXSTT_EXCHANGEING,	

	eEXSTT_MAX,
};

#define EVENT_PET_ID_START		0x6fffffff
#define PET_ID_START			0x3fffffff
#define STATICNPC_ID_START		0xa0000000
#define STATICNPC_ID_MAX		0x10000000
#define TACTICOBJECT_ID_START	0xb0000000
#define TACTICOBJECT_ID_MAX		0x10000000
#define SKILLOBJECT_ID_START	0xc0000000
#define SKILLOBJECT_ID_MAX		0x0000ffff
#define BATTLE_ID_START			0xd0000000
#define BATTLE_ID_MAX			0x0000ffff
#define TEMP_SKILLOBJECT_ID		0xffffffff
#define BATTLE_TEAMEFFECT_ID	0xfffffffe
#define BATTLE_STAGEEFFECT_ID	0xfffffffd
#define STATE_EFFECT_ID			0xfffffffc
#define PK_EFFECT_ID			0xfffffffb
#define NPCMARK_1_ID			0xfffffffa
#define NPCMARK_2_ID			0xfffffff9
#define PARTYWAR_STAGEEFFECT_ID	0xfffffff8
#define BATTLE_GTTEAM1_EFFECTID	0xfffffff7
#define BATTLE_GTTEAM2_EFFECTID	0xfffffff6
#define JACKPOT_EFFECTID		0xfffffff5
#define ENGRAVE_EFFECTID		0xfffffff4
#define ENGRAVE_ACK_EFFECTID	0xfffffff3
#define ENGRAVE_NACK_EFFECTID	0xfffffff2
#define LIFE_RECOVER_EFFECTID	0xfffffff1
#define MANA_RECOVER_EFFECTID	0xfffffff0
#define SHIELD_RECOVER_EFFECTID	0xffffffef
#define ALL_RECOVER_EFFECTID	0xffffffee

#define SETITEM_STARTEFFECTID	0xf0000000		

#define TITAN_MAINTAIN_EFFECTID	0xff000000
#define TITAN_RECALL_EFFECTID	0xffff0000		
#define KYUNGGONG_EFFECTID		0xffffff00		


//#ifdef _JAPAN_LOCAL_
//#define MUSSANG_EFFECT_ID		0xffffffed
//#else

#define MUSSANG_EFFECT_ID1		0xffffffed
#define MUSSANG_EFFECT_ID2		0xffffffec
#define MUSSANG_EFFECT_ID3		0xffffffeb
//#endif

#define DETECT_EFFECT_ID		0xffffffea


#define MAX_SKILLMAINTAIN_EFFECT	3

typedef BYTE DIRINDEX;
#define MAX_DIRECTION_INDEX		32
#define DEGTODIR(angleDeg)	(BYTE)((angleDeg)/360*MAX_DIRECTION_INDEX)
#define DIRTODEG(DirIndex)	(((float)DirIndex)*360/MAX_DIRECTION_INDEX)
#define DIRTORAD(DirIndex)	(DEGTORAD(DIRTODEG(DirIndex)))
#define RADTODIR(angleRad)	(DEGTODIR(RADTODEG(angleRad)))

#define CAMERA_NEAR		100
#define MAX_CHARTARGETPOSBUF_SIZE	15

//system
#define MAX_IPADDRESS_SIZE	16
enum eMoneyLogtype
{
	eMoneyLog_Time				= 0,
	eMoneyLog_GetStall			= 1,
	eMoneyLog_GetStreetStall	= 2,
	eMoneyLog_GetMonster		= 3,
	eMoneyLog_GetExchange		= 4,
	eMoneyLog_GetPyoguk			= 5,
	eMoneyLog_GetGuild			= 6,
	eMoneyLog_GetWantedPrize	= 7,
	eMoneyLog_GetPKLooting		= 8,
	eMoneyLog_GetSuryunFail		= 9,
	eMoneyLog_GetPrize			= 10,
	eMoneyLog_GetSWProfit		= 11,
	eMoneyLog_GetFromQuest		= 12,
	eMoneyLog_GetGMTool			= 13,
	
// ∷㈢¨∷㈢】∷㈢ie
	eMoneyLog_LoseStall			= 101,
	eMoneyLog_LoseStreetStall	= 102,
	eMoneyLog_LoseDie			= 103,
	eMoneyLog_LoseExchange		= 104,
	eMoneyLog_LosePyoguk		= 105,
	eMoneyLog_LoseGuild			= 106,
	eMoneyLog_LosePyogukBuy		= 107,
	eMoneyLog_LoseWantedRegist	= 108,
	eMoneyLog_LoseWantedBuyRight = 109,
	eMoneyLog_LosePKLooted		= 110,
	eMoneyLog_LosePKModeExitPanelty = 111,
	eMoneyLog_LoseBadFrameBail	= 112,
	eMoneyLog_LoseSuryunFee		= 113,
	eMoneyLog_LoseGuildLevelUp	= 114,
	eMoneyLog_LoseChangeLocation	= 115,
	eMoneyLog_LoseGMTool		= 116,
	eMoneyLog_LoseMarry		= 117,
	eMoneyLog_GuildCerebration	= 199,
	eMoneyLog_GetCheatMoney = 201,
};

enum eLogMugong
{
	eLog_MugongLearn = 0,
	eLog_MugongDiscard,
	eLog_MugongDestroyByWanted,
	eLog_MugongDestroyByBadFame,
	eLog_MugongLevelup,
	eLog_MugongCheatLevelup,
	eLog_MugongLevelDownByWanted,
	eLog_MugongLevelDownByBadFame,
	eLog_MugongDestroyByGetJulCho,
	eLog_MugongDestroyByBadFameJulChoDown,
	eLog_MugongDestroyByGetNextLevel,	
	eLog_MugongOption,
	eLog_MugongOptionClear,
	eLog_MugongDestroyByOptionClear,
	eLog_MugongOptionClearbyShopItem,
};
enum eGoldMoneLog
{
	eLog_UseMallMoney = 1,
	eLog_UseGoldMoney = 2,
	eLog_UseMoney	  = 3,
	eLog_GetGoldMoney = 4,
	eLog_GetMallMoney = 5,
	eLog_EquipItemGrowInherit = 6,			//[装备继承][BY:TT天语][QQ:2013734367][2019-8-22][23:09:13]
	eLog_ChangeItemGetGoldMoney = 7,		//[开启箱子获得元宝][BY:TT天语][QQ:2013734367][2019-9-22][17:41:23]
	eLog_VimuBattleGetForNoWiner = 8,		//[押注比武押金退还][BY:TT天语][QQ:2013734367][2019-4-27][19:59:07]
	eLog_VimuBattleLost = 9,				//[押注比武失去][BY:TT天语][QQ:2013734367][2019-4-27][16:36:21]
	eLog_VimuBattleGet = 10,				//[押注比武获得][BY:TT天语][QQ:2013734367][2019-4-27][16:36:11]
	eLog_SiegeWarGet = 11,					//[攻城战物品发放][By:TT天语][QQ:2013734367][2018/2/21]	
	eLog_Singed = 12,						//[签到系统][By:TT天语][QQ:2013734367][2018/1/4]
	eLog_GoldMoneyDeleteBuf = 13,			//[buff删除消耗][BY:TT天语][QQ:2013734367][2019-9-22][17:42:36]
	eLog_GoldMoneyGMGiveGold = 14,			//[GM工具发放][BY:TT天语][QQ:2013734367][2019-9-22][17:42:55]
	eLog_GoldMoneyMarryGold = 15,			//[结婚消耗][BY:TT天语][QQ:2013734367][2019-9-22][17:43:08]
	eLog_GoldMoneyFame = 16,				//[洗恶消耗][BY:TT天语][QQ:2013734367][2019-9-22][17:43:24]
	eLog_ChangeItemGetMallMoney = 17,		//[开启箱子获得泡点][BY:TT天语][QQ:2013734367][2019-9-22][18:12:40]
	//eLog_GoldMoneyStreetBuy = 17,			
	eLog_GoldMoneyStreetGet = 18,			//[出售物品获得][BY:TT天语][QQ:2013734367][2019-9-22][17:43:47]
	eLog_GoldMoneyStreetLost = 19,			//[购买物品消耗][BY:TT天语][QQ:2013734367][2019-9-22][20:50:38]
	eLog_GoldMoneyDealGet = 20,				//[交易获得][BY:TT天语][QQ:2013734367][2019-9-22][20:50:53]
	eLog_GoldMoneyDealLost = 21,			//[交易消耗][BY:TT天语][QQ:2013734367][2019-9-22][20:51:04]
	
	eLog_GoldMoneyPay = 22,					//[摆摊收购][BY:TT天语][QQ:2013734367][2019-9-28][14:08:43]
	eLog_Gold_UnionWar_Get=23,				//[盟战奖励][BY:TT天语][QQ:2013734367][2019-10-31][13:10:39]
	//eLog_GoldMoneyFB = 53,
};
enum eLogitemmoney
{
	eLog_ItemDiscard = 1,
	//元宝日志类型	by:TT天语	QQ:2013734367


	eLog_ItemMoveInvenToPyoguk = 100,
	eLog_ItemMovePyogukToInven,
	eLog_ItemMoveInvenToGuild,
	eLog_ItemMoveGuildToInven,
	eLog_ItemMoveGuildToGuild,
	
	eLog_ItemObtainBuy = 200,
	eLog_ItemObtainDivide,
	eLog_ItemObtainCheat,
	eLog_ItemObtainMonster,
	eLog_ItemObtainQuest,
	eLog_ItemObtainPK,
	eLog_ItemObtainDissolution,
	eLog_ItemObtainFromChangeItem,
	eLog_ItemRareObtainCheat,
	eLog_ItemOptionObtainCheat,
	eLog_ItemObtainTitan,
	eLog_ItemObtainFromGameEvent,
	eLog_ItemObtainSinged,
	eLog_ItemObtainMarry,
	eLog_ItemObtainRecharge,
	eLog_ItemObtainSiegeWar,
	
	eLog_ItemSell = 300,
	eLog_ItemUse,
	
	eLog_StreetStallBuyAll = 400,
	eLog_StreetStallBuyDivide,
	eLog_StreetStallSellDivide,

	eLog_Exchange = 500,

	eLog_ItemReinforceSuccess = 600,
	eLog_ItemReinforceFail,
	eLog_ItemReinforceSuccessWithShopItem,
	eLog_ItemReinforceFailWithShopItem,
	eLog_ItemReinforceProtectWithShopItem,

	eLog_ItemMixSuccess = 700,
	eLog_ItemMixFail,
	eLog_ItemMixBigFail,
	eLog_ItemRareMixFail,
	eLog_ItemRareMixBigFail,

	eLog_ItemDestroyMix = 800,
	eLog_ItemDestroyReinforce,
	eLog_ItemDestroyUpgrade,
	eLog_ItemDestroyByWanted, 
	eLog_ItemDestroyByBadFame,
	eLog_ItemDestroyDissolution,
	eLog_ItemDestroyGiveQuest,
	eLog_ItemDestroyDeleteQuest,
	eLog_ItemDestroybyChangeItem,
	eLog_ItemDestroyReinforceWithShopItem,

	eLog_MoneyObtainPK = 1000,
	eLog_ExpObtainPK,
	eLog_ShopItemUse = 1500,	
	eLog_ShopItemUseEnd,
	eLog_ShopItemMoveToInven,
	eLog_ShopItemProtectAll,
	eLog_ShopItemSeal,
	eLog_ShopItem_ReinforceReset,
	eLog_ShopItemGetCheat,
	eLog_ShopItemStatsChange,

	eLog_ShopItemUse_MixSuccess=1600,
	
	eLog_RMTool = 2000,
	
	eLog_ItemMoveInvenToPetInven = 2100,
	eLog_ItemMovePetInvenToInven,

	eLog_ItemObtainFromFortWarWare = 2200,
	eLog_ItemObtainFromSeigeWarWare,

	eLog_Max,
};

enum eLogExppoint
{
	eExpLog_Time,
		
	eExpLog_LevelUp = 10,

	eExpLog_LosebyBadFame = 50,
	eExpLog_LosebyReviveLogIn,
	eExpLog_LosebyRevivePresent,
	eExpLog_LosebyLooting,
	eExpLog_LosebyBreakupGuild,
	eExpLog_LosebyReviveExpProtect,
	eExpLog_LosebyReviveReviveOther,
	eExpLog_LosebyGuildFieldWar,
	eExpLog_LosebyExitStateDIE,

	eExpLog_GetbyQuest = 100,
	eExpLog_SExpGetbyQuest = 101,
	
	eExpLog_ProtectExp = 150,
	eExpLog_ReviveExp,
	eExpLog_ReviveExpPeriod,
	eExpLog_NoActionPanelty_Login,
	eExpLog_NoActionPanelty_Present,
    eExpLog_NoActionPanelty_Village,
	eExpLog_NoActionPanelty_Exit,
};

enum eLogToolType	
{
	eToolLog_GM = 1,
	eToolLog_RM,
	eToolLog_Server,
};

enum eGMLogKind		
{
	eGMLog_LogIn = 1,
	eGMLog_Move,	
	eGMLog_Item,
	eGMLog_Money,
	eGMLog_Summon,
	eGMLog_SummonDelete,
	eGMLog_MonsterKill,
	eGMLog_PKAllow,
	eGMLog_Disconnect_Map,
	eGMLog_Disconnect_User,
	eGMLog_Block,
	eGMLog_Event,
	eGMLog_Jackpot,
	eGMLog_PlusTime,

	eGMLog_Skill,
	eGMLog_Ability,
	eGMLog_Chat_Map,
	eGMLog_Chat_User,
	eGMLog_PK,
	eGMLog_Regen,
};

enum eRMLogKind		
{
	eRMLog_OperInsert = 1,
	eRMLog_OperDelete,
	eRMLog_IpInsert,
	eRMLog_IpDelete,

	eRMLog_GMIpInsert,

	eRMLog_GMIpDelete,

	eRMLog_ChangeCharacterInfo = 100,
	eRMLog_ChangeWareInfo,
	eRMLog_SkillInsert,
	eRMLog_SkillDelete,
	eRMLog_SkillUpdate,

	eRMLog_AbilityUpdate,

	eRMLog_QuestDelete,
	
	eRMLog_ChangeLoginPoint,
	
	eRMLog_ChangeItemPosition,
	eRMLog_ChangeWareItemPosition,
	eRMLog_ChangeSkillPosition,

	eRMLog_ItemInsert = 200,
	eRMLog_ItemDelete,
	eRMLog_ItemUpdate,	

	eRMLog_ChangeUserLevel = 300,
	eRMLog_RecoverCharacter,

	eRMLog_ChangePetInfo = 400,	
};

enum ePetLogType
{
	ePetLog_MasterDie,
	ePetLog_PetDie,
	ePetLog_PetRevival,
	ePetLog_Exchange,
	ePetLog_GradeUp,
	ePetLog_UpGradeFailed,
	ePetLog_StaminaZero,
	ePetLog_StaminaFromZero,
};


enum eGTLogKind
{
	eGTLogKind_None=0,
	eGTLogKind_Regist,		
	eGTLogKind_Win,		
	eGTLogKind_Lose,	
};

enum eSWLogKind
{
	eSWLogKind_None=0,
	eSWLogKind_DefenceProposal,		
	eSWLogKind_Attack,					
	eSWLogKind_Defence,				
	eSWLogKind_DefenceAccept,		
	eSWLogKind_DefenceUnion,		
	eSWLogKind_CancelByCastleGuild,		
	eSWLogKind_CancelByUnionSecession,	
	eSWLogKind_StartSiegeWar,			
	eSWLogKind_SucceedCastleGuild,		
	eSWLogKind_SucceedMoveToDefence,
	eSWLogKind_SucceedAttack,		
	eSWLogKind_SucceedMoveToAttack,		
	eSWLogKind_EndCastleGuild,		
	eSWLogKind_EndDefenceUnionGuild,
	eSWLogKind_EngraveSyn,			
	eSWLogKind_EngraveNack,				
	eSWLogKind_EngraveNackCancel,		
	eSWLogKind_EngraveNackLogOut,	
	eSWLogKind_SiegeWarSucceed,		
	eSWLogKind_EndSiegeWar,		
};


#define MAX_JOURNAL_ITEM			100
#define JOURNALVIEW_PER_PAGE		5

enum 
{
	eJournal_Quest,
	eJournal_Wanted,
	eJournal_Levelup,
	
	eJournal_Update,
	eJournal_Delete,
};
enum
{
	eJournal_Wanted_Doing,
	eJournal_Wanted_Succeed,
	eJournal_Wanted_FailbyOther, 
	eJournal_Wanted_FailbyDelChr, 
		
	eJournal_MurderedbyChr,
	eJournal_Wanted_FailbyBeWantedChr,
	eJournal_Wanted_FailbyTime,
	eJournal_Wanted_FailbyDie, 
};


enum eShopItemIncantation
{
	eIncantation_mugiup,
	eIncantation_armorup,
	eIncantation_mapmove,
	eIncantation_backhome,
	eIncantation_recovery,
};


//-----------------------------------------------------------------------
// PreLoadData
//-----------------------------------------------------------------------

#define MAX_KIND_PERMAP		150

enum
{
	ePreLoad_Monster,
	ePreLoad_Item,

	ePreLoad_Max,
};


struct sPRELOAD_INFO
{
	int		MapNum;
	int		Count[ePreLoad_Max];
	int		Kind[ePreLoad_Max][MAX_KIND_PERMAP];

	sPRELOAD_INFO()
	{
		MapNum		= 0;
		memset(Count, 0, sizeof(int)*ePreLoad_Max);
		memset(Kind, 0, sizeof(int)*ePreLoad_Max*MAX_KIND_PERMAP);
	}
};


#define BAIL_PRICE				10000
//[元宝洗恶数量定义][By:TT天语][QQ:2013734367][2017/11/22]
#define BAIL_GOLD_PRICE				2
#define MIN_BADFAME_FOR_BAIL	0

//
#define MAX_ALLOWANCE	10



//-----------------------------------------------------------------------
// MiniMap
//-----------------------------------------------------------------------

enum
{
	eMINIMAPMODE_SMALL,
	eMINIMAPMODE_FULL,

	eMINIMAPMODE_MAX,
};
enum
{
	eMNMICON_SELF,
	eMNMICON_PARTYMEMBER,
	eMNMICON_LOGIN,
	eMNMICON_MAPCHANGE,
	eMNMICON_WEAPON,
	eMNMICON_DRESS,
	eMNMICON_DOCTOR,
	eMNMICON_BOOK,
	eMNMICON_WAREHOUSE,
	eMNMICON_PYOGUK,
	eMNMICON_MUNPA,
	eMNMICON_YEONMU,
	eMNMICON_CASTLEMANAGER,

	eMNMICON_QUEST_EXCLAMATION1,
	eMNMICON_QUEST_EXCLAMATION2,
	eMNMICON_QUEST_EXCLAMATION3,
	eMNMICON_CAMERA,
	eMNMICON_FINDPATH1,
	eMNMICON_FINDPATH2,
	eMNMICON_FINDPATH3,
	eMNMICON_MAX,
};

enum eBIGMAP_ICON
{
	eBMICON_SELF,
	eBMICON_PARTYMEMBER,

	eBMICON_LOGIN,
	eBMICON_MAPCHANGE,
	eBMICON_WEAPON,
	eBMICON_DRESS,
	eBMICON_DOCTOR,
	eBMICON_BOOK,
	eBMICON_WAREHOUSE,
	eBMICON_PYOGUK,
	eBMICON_MUNPA,
	eBMICON_YEONMU,
	eBMICON_ACCESSARY,
	eBMICON_GWANJOL,
	eBMICON_CASTLEMANAGER,
	eBMICON_ETC,

	eBMICON_QUEST_EXCLAMATION1,
	eBMICON_QUEST_EXCLAMATION2,
	eBMICON_QUEST_EXCLAMATION3,

	eBMICON_CAMERA,
	eBMICON_FINDPATH1,
	eBMICON_FINDPATH2,
	eBMICON_FINDPATH3,
	eBMICON_MAX,
};

#define MAX_GUILD_NAME 16
#define MAX_GUILD_INTRO		60
#define MAX_GUILD_NOTICE	150
#define MAX_GUILD_UNION		7
#define MAX_GUILD_NICKNAME	16
#define MAX_MASTERNAME_LENGTH	32

#define GUILD_MASTER		50	//[门主][BY:TT天语][QQ:2013734367][2019-8-15][23:40:56]
#define GUILD_VICEMASTER	40	//[副门主][BY:TT天语][QQ:2013734367][2019-8-15][23:40:26]
#define GUILD_SENIOR		30	//[长老][BY:TT天语][QQ:2013734367][2019-8-15][23:40:32]
#define GUILD_MEMBER		20	//[帮众][BY:TT天语][QQ:2013734367][2019-8-15][23:40:37]
#define GUILD_STUDENT		10	//[弟子][BY:TT天语][QQ:2013734367][2019-8-15][23:40:44]
#define GUILD_NOTMEMBER		0

#define GUILD_1LEVEL		1
#define GUILD_2LEVEL		2
#define GUILD_3LEVEL		3
#define GUILD_4LEVEL		4
#define GUILD_5LEVEL		5

#define MAX_GUILD_LEVEL		5
#define MAX_GUILD_STEP		5

#define GUILD_CREATELEVEL	15

//#ifdef _CHINA_LOCAL_
//#define MAX_GUILDMEMBER_NUM 50	
//#else
#define MAX_GUILDMEMBER_NUM 55
//#endif




enum
{
	eShopItem_PeneltyCountZero,
};


//-----------------------------------------------------------------------------------
// Avatar Item
//-----------------------------------------------------------------------------------
enum
{
	eAvatar_Hat=0,			
	eAvatar_Hair,			
	eAvatar_Face,			
	eAvatar_Mask,		
	eAvatar_Glasses,		
	eAvatar_Mustache,	
	eAvatar_Dress,			
	eAvatar_Shoulder,	
	eAvatar_Back,			
	eAvatar_Shoes,		
	eAvatar_Effect,			
	eAvatar_Hand,			

	eAvatar_Weared_Hair,	
	eAvatar_Weared_Face,
	eAvatar_Weared_Hat,			
	eAvatar_Weared_Dress,	
	eAvatar_Weared_Shoes,	
	eAvatar_Weared_Gum,		
	eAvatar_Weared_Gwun,	
	eAvatar_Weared_Do,		
	eAvatar_Weared_Chang,	
	eAvatar_Weared_Gung,	
	eAvatar_Weared_Amgi,	
	eAvatar_Max,
};

enum
{
	eAvatar_NotUse=0,
	eAvatar_Use,	
};

enum
{
	eAvatar_Attach_Head,
	eAvatar_Change_Dress,
	
};



enum
{
	eIncantation_TownMove			= 55301,		// 回城卷(单次)
	eIncantation_MemoryMove			= 55302,		// 遁甲卷(单次)
	eIncantation_TownMove15			= 55303,		// 回城卷(十五天)
	eIncantation_MemoryMove15		= 55304,		// 遁甲卷(十五天)
	eIncantation_TownMove7			= 57508,		// 回城卷(三天)
	eIncantation_TownMove7_NoTrade	= 57509,		// 回城卷(试用七天)
	eIncantation_MemoryMove7		= 57510,		// 遁甲卷(七天)
	eIncantation_MemoryMove7_NoTrade = 57511,		// 遁甲卷(试用七天)
	eIncantation_MoneyProtect		= 55311,		// 保禄卷
	eIncantation_ExpProtect			= 55312,		// 长寿卷
	eIncantation_ProtectAll			= 55313,		// 护魂卷
	eIncantation_StatePoint			= 55299,		// 胶湃器牢飘 犁盒硅
	eIncantation_StatePoint_30		= 55321,		// 洗髓经(30)
	eIncantation_StatePoint_50		= 57782,		// 人王封印宝箱(活动用)(50)
	eIncantation_StatePoint_100		= 57783,		// 天王封印宝箱(活动用)(100)
	eIncantation_StatusPointReset_71	= 55392,		// 属性点还原卷轴(71Lv以上)
	eIncantation_StatusPointReset_1_50	= 55393,		// 属性点还原卷轴(1~50Lv)
	eIncantation_StatusPointReset_1_99	= 55394,		// 神秘洗点卷1-99级
	eIncantation_SkillPoint_Reset	= 55391,		// 技点还原卷轴
	eIncantation_MixUp				= 55322,		
	eIncantation_OtherRevive		= 55331,		
	eIncantation_MemoryStone		= 55601,	
	eIncantation_SkPointRedist_2	= 55322,        // 多福卷
	eIncantation_SkPointRedist_4	= 55323,
	eIncantation_SkPointRedist		= 55300,	
	eIncantation_ShowPyoguk			= 55351,		// 开仓卷(十五天)
	eIncantation_ShowPyoguk7		= 57506,		// 开仓卷(七天)
	eIncantation_ShowPyoguk7_NoTrade = 57507,		// 开仓卷(试用七天)
	eIncantation_ChangeName			= 55352,		// 化名卷
	eIncantation_Tracking			= 55353,		// 缉恶卷(十五天)
	eIncantation_Tracking7			= 57504,		// 缉恶卷(七天)
	eIncantation_Tracking7_NoTrade	= 57505,		// 缉恶卷(试用七天)
	eIncantation_Tracking_Jin 		= 55387,		// 变换还原卷
	eIncantation_CharChange			= 55354,		// 葵花秘卷
	eIncantation_ShapeChange		= 55355,        // 易容秘卷
	eIncantation_ItemSeal			= 55356,		// 封印之力卷轴
	eIncantation_ProtectAll15		= 55358,		// 护魂卷(十五天)
	eIncantation_ChangeJob			= 55360,		// 化魔转换卷
	eIncantation_MugongExtend		= 55361,		// 通慧卷
	eIncantation_MugongExtend2		= 57957,		// 通慧卷(试用)
	eIncantation_ReinforceReset		= 55364,		// 还原卷
	eIncantation_MemoryMoveExtend30	= 58010,		// 奇门遁甲卷(30天)
	eIncantation_MemoryMoveExtend	= 55365,		// 奇门遁甲卷(十五天)
	eIncantation_MemoryMoveExtend7	= 55390,		// 奇门遁甲卷(七天)
	eIncantation_MemoryMove2		= 55371,		// 遁甲卷(2日)
	eIncantation_ProtectAll1		= 55372,		// 护魂卷(1日)

	eIncantation_PetRevival			= 55382,		// 宠物卷轴
	eIncantation_PetGrade2 			= 55384,		// 2阶卷轴
	eIncantation_PetGrade3 			= 55385,		// 3阶卷轴
	eIncantation_MugongOptionReset	= 55387,		// 变换还原卷
	eIncantation_PyogukExtend 		= 57544,		// 仓库扩充卷	
	eIncantation_PyogukExtend2 		= 57960,		// 仓库扩充卷(试用)
	eIncantation_CharacterSlot 		= 57543,		// 卷
	eIncantation_CharacterSlot2 	= 57959,		// 卷(试用)
	eIncantation_InvenExtend 		= 57542,		// 千斤卷
	eIncantation_InvenExtend2 		= 57958,		// 千斤卷(试用)
	eIncantation_ChangeName_Dntrade = 57799,		// 化名卷(活动)
	eIncantation_Curse_Cancellation	= 55389,		// 诅咒解除卷轴

	eSundries_Boom_Butterfly		= 55621,		// 蝶舞花香
	eSundries_Boom_Star				= 55622,		// 众星捧月
	eSundries_Shout					= 55631,		// 狮吼牌
	eSundries_Shout_Once			= 55632,		// 狮吼牌(1次)
	eSundries_Shout_Once_NoTrade	= 57593,		// 狮吼牌(1次)
	eSundries_RareItemCreate50		= 55640,        // 祝福石(Lv.1~Lv.50)
	eSundries_RareItemCreate70		= 55641,        // 祝福石(Lv.51~Lv.70)
	eSundries_RareItemCreate90		= 55642,        // 祝福石(Lv.71~Lv.99)


	eItemLockStone					= 51500,//[装备锁定][By:TT天语][QQ:2013734367][2018/1/1]
	eItemUnLockStone				= 51501,//[锁定解锁][By:TT天语][QQ:2013734367][2018/1/1]
	eItemUnLockStoneForPD			= 51502,//[泡点解锁][By:TT天语][QQ:2013734367][2018/1/1]
	eItemMixEquiLow					= 51503,//[低级装备修理石][By:TT天语][QQ:2013734367][2018/1/4]
	eItemMixEquiMid					= 51504,//[中级装备修理石][By:TT天语][QQ:2013734367][2018/1/4]
	eItemMixEquiMax					= 51505,//[高级装备修理石][By:TT天语][QQ:2013734367][2018/1/4]
	
	eSundries_SuperShout				= 51506,		//[超级狮吼牌][By:TT天语][QQ:2013734367][2018/9/8]
	eSundries_SuperShout_Once			= 51507,		//[超级狮吼牌(1次)][By:TT天语][QQ:2013734367][2018/9/8]
	eSundries_SuperShout_Once_NoTrade	= 51508,		//[超级狮吼牌(1次)][By:TT天语][QQ:2013734367][2018/9/8]
	eLazyItem_N							= 51509,		//[牛任务卷轴][By:TT天语][QQ:2013734367][2018/9/8]
	eLazyItem_J							= 51510,		//[巨任务卷轴][By:TT天语][QQ:2013734367][2018/9/8]

	eMarryReckon_3						= 51511,		//[夫妻传送卷3天][By:TT天语][QQ:2013734367][2018/9/11]
	eMarryReckon_7						= 51512,		//[夫妻傳送卷7天][By:TT天语][QQ:2013734367][2018/9/11]
	eMarryReckon_15						= 51513,		//[夫妻傳送卷15天][By:TT天语][QQ:2013734367][2018/9/11]
	eGuildMasterChange					= 51514,		//[帮主转让卷][By:TT天语][QQ:2013734367][2018/9/11]
	eFastCofigPoint						= 57668,		//[快速加点卷][BY:TT天语][QQ:2013734367][2019-4-5][18:13:29]
};


enum
{
	eShopItemUseParam_Realtime = 1,				//[离线也计时][By:TT天语][QQ:2013734367][2018/5/15]
	eShopItemUseParam_Playtime = 2,				//[只计算在线时间][By:TT天语][QQ:2013734367][2018/5/15]
	eShopItemUseParam_Continue = 3,				//[从解封开始计算时间][By:TT天语][QQ:2013734367][2018/5/15]
	eShopItemUseParam_Forever = 4,				//[永久使用][By:TT天语][QQ:2013734367][2017/12/8]
	eShopItemuseParam_FBItem = 5,				//[副本物品使用时间][By:TT天语][QQ:2013734367][2017/12/8]
	eShopItemUseParam_EquipAvatar = 10,			//[穿戴类型百宝计时][By:TT天语][QQ:2013734367][2018/5/15]
};


enum
{
	eStateItem_NotUsed,
	eStateItem_Able,
	eStateItem_UnAble,
};


enum
{
	eShopItemErr_Revive_Fail=1,
	eShopItemErr_Revive_NotDead,
	eShopItemErr_Revive_NotUse,
	eShopItemErr_Revive_Refuse,
	eShopItemErr_Revive_TooFar,
	eShopItemErr_Revive_TimeOver,	
	eShopItemErr_Revive_NotReady,
};

enum
{
	eShopItemDupType_None=0,
	eShopItemDupType_Charm,
	eShopItemDupType_Herb,
	eShopItemDupType_Incantation,
	eShopItemDupType_Sundries,
	eShopItemDupType_PetEquip,
};

enum DONTDUP_INCANTATION
{
	eDontDupUse_MemoryMove		= 2,
	eDontDupUse_ProtectAll		= 4,
	eDontDupUse_LevelCancel50	= 8,
	eDontDupUse_LevelCancel70	= 16,
	eDontDupUse_LevelCancel90	= 32,
	eDontDupUse_ShowPyoguk		= 64,
	eDontDupUse_Chase			= 128,
	eDontDupUse_TownMove		= 256,
};

enum DONTDUP_CHARM
{
	eDontDupUse_NaegongDamage	= 2,
	eDontDupUse_WoigongDamage	= 4,
	eDontDupUse_Exppoint		= 8,
	eDontDupUse_Reinforce		= 16,
	eDontDupUse_Kyunggong		= 32,
	eDontDupUse_Ghost			= 64,	
	eDontDupUse_Woigong			= 128,	
	eDontDupUse_Naegong			= 256,	
	eDontDupUse_Hunter			= 16384,	
	eDontDupUse_ExpDay			= 32768,	
};

enum DONTDUP_HERB
{
	eDontDupUse_Life			= 2,
	eDontDupUse_Shield			= 4,
	eDontDupUse_Naeruyk			= 8,
	eDontDupUse_GreateLife		= 16,
	eDontDupUse_GreateShield	= 32,
	eDontDupUse_GreateNaeruyk	= 64,
	eDontDupUse_EventSatang		= 128,
	eDontDupUse_Doll			= 256,
};

enum DONTDUP_SUNDRIES
{
	eDontDupUse_StreeStall = 2,
};


enum DONTDUP_PETEQUIP
{
	eDupPetEquip_PomanRing = 2,
};


#define SLOT_SHOPITEM_IMSI		90
#define REVIVE_OTHER_DIST		1000



enum
{
	eMode_NormalNote = 0,
	eMode_PresentNote,
};



#define MAX_SHOUT_LENGTH	60




//ΞΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΟ
//ΝGuild Tournament									Ν
//ΡΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΠ
#define LIMITMEMBER_GUILDTOURNAMENT		5
#define LIMITLEVEL_GUILDTOURNAMENT		2
#define MAXGUILD_INTOURNAMENT			32
#define MAX_EVENTGT_COUNT				10

enum
{
	eGTError_NOERROR=0,				
	eGTError_NOTREGISTDAY,				
	eGTError_NOTREGISTCANCELDAY,		
	eGTError_DONTFINDGUILDINFO,			
	eGTError_MAXGUILDOVER,				
	eGTError_ALREADYREGISTE,			
	eGTError_ALREADYCANCELREGISTE,		
	eGTError_UNDERLIMITEMEMBER,			
	eGTError_UNDERLEVEL,				
	eGTError_DELETEAPPLIED,				
	eGTError_NOGUILDMASTER,				
	eGTError_DONTFINDPOSITION,			
	eGTError_DONTFINDBATTLE,			
	eGTError_BATTLEJOINFAIL,			
	eGTError_NOTENTERTIME,				
	eGTError_STUDENTCANTENTER,			
	eGTError_ERROR,
};

enum
{
	eGTState_BeforeRegist=0,
	eGTState_Regist,
	eGTState_BeforeEntrance,
	eGTState_Entrance,
	eGTState_Process,
	eGTState_Leave,
	eGTState_None,		
};

enum
{
	eGTStanding_1ST = 1,
	eGTStanding_2ND = 2,
	eGTStanding_3RD = 3,
	eGTStanding_4TH = 4,
	eGTStanding_8TH = 8,
	eGTStanding_16TH = 16,
	eGTStanding_32TH = 32,
};

enum
{
	eGTFight_32		= 0,
	eGTFight_16		= 1,
	eGTFight_8		= 2,
	eGTFight_4		= 3,
	eGTFight_3_4	= 4,
	eGTFight_1_2	= 5,
	eGTT_MAX,
	eGTFight_End,
	eGTFignt_Event,
};

enum
{
	eGTDay_SUNDAY=0,
	eGTDay_MONDAY,
	eGTDay_TUESDAY,
	eGTDay_WEDNESDAY,
	eGTDay_THURSDAY,
	eGTDay_FRIDAY,
	eGTDay_SATURDAY,
};

enum
{
	eGTWin_None = 0,
	eGTWin_Unearned,	
	eGTWin_RemainPlayer,
	eGTWin_LowLevel,	
	eGTWin_LowExp,	
};

enum
{
	eGTEvent_None = 0,
	eGTEvent_Start,
	eGTEvent_NoGuild,
	eGTEvent_CreateBattleFailed,
	eGTEvent_MaxEventOver,
	eGTEvent_AlreadyJoinedGuild,
};

enum FULLMOONEVENTRATE
{
	eFULLMOONEVENT_NONE = 0,
	eFULLMOONEVENT_BUN,
	eFULLMOONEVENT_CHINESECAKE,

	eFULLMOONEVENT_MAX
};


#define MAX_CASTLEGATE_NUM		20


enum eFlagNpc
{
	eGTFlg,
	eSGFlg,
	eFlgKind,
};

enum
{
	eNpcParam_ShowpPyoguk=1,
};

#define ITEM_STATIC_PD		0x00000001  
#define ITEM_STATIC_GD      0x00000002 
#define ITEM_STATIC_LUCK	0x00000003

#define ITEM_PARAM_SEAL		0x00000001
#define ITEM_PARAM_CHEAT	0x00000010
#define ITEM_PARAM_PRESENT	0x00000100
#define ITEM_PARAM_TITAN_REGIST	0x00001000	

#define MAX_MOVEDATA_PERPAGE		10
#define MAX_MOVEPOINT_PAGE			2
#define MAX_SAVEDMOVE_NAME			21

#define ITEMPARAM_ISLOCK(Param)					( (DWORD)Param>>31	)	
#define ITEMPARAM_SETLOCK(Param, Option)		( Param = ((Param<<1)|(Option<<31)))


enum eCharacterStage
{
	eStage_Normal	= 0,	
	eStage_Hwa		= 64,  		
	eStage_Hyun		= 65,	
	eStage_Geuk		= 128,	  		
	eStage_Tal		= 129,		
};

enum eItemStage
{
	eItemStage_Normal		= 0,
	eItemStage_ChangeStage	= 32,
	eItemStage_ChangeStage2	= 33,
	eItemStage_OverHwa		= 64,
	eItemStage_Hwa			= 65,
	eItemStage_Hyun			= 66,
	eItemStage_OverGeuk		= 128,
	eItemStage_Geuk			= 129,
	eItemStage_Tal			= 130,
};

enum eGuildUnion_Err
{
	eGU_Not_ValidName,
	eGU_Not_InGuild,
	eGU_Not_GuildMaster,
	eGU_Not_Level,
	eGU_Not_GuildUnionMaster,
	eGU_Not_InGuildUnion,
	eGU_Aleady_InGuildUnion,
	eGU_Time,
	eGU_Other_Not_InGuild,
	eGU_Other_Not_InGuildUnion,
	eGU_Other_Not_GuildMaster,
	eGU_Other_Aleady_InGuildUnion,
	eGU_Other_Time,
	eGU_Is_Full,	
	eGU_Is_GuildFieldWar,
	eGU_Not_ValidMap,
};

enum eSWProfit_Err
{
	eSW_Not_Profit,
	eSW_Not_ProfitGuild,
	eSW_Not_ProfitGuildMaster,	
	eSW_No_TaxRate,
	eSW_No_Time,
	eSW_No_Money,
};

enum eCharacter_Attr
{
	eCA_None	= 0,
	eCA_Fire	= 1,		
	eCA_Earth	= 2,
	eCA_Iron	= 3,
	eCA_Water	= 4,
	eCA_Tree	= 5,	
};

enum eItem_Attr
{
	eIA_All			= 0,
	eIA_Fire		= 1,
	eIA_Earth		= 2,
	eIA_Iron		= 3,	
	eIA_Water		= 4,	
	eIA_Tree		= 5,	
	eIA_Fire_Only	= 11,
	eIA_Earth_Only	= 12,
	eIA_Iron_Only	= 13,
	eIA_Water_Only	= 14,
	eIA_Tree_Only	= 15,	
};

enum eCharacter_Attr_Relation
{
	eCAR_None				= -1,
	eCAR_Same				= 0,
	eCAR_Give_Help			= 1,	
	eCAR_Strong_Opposite	= 2,	
	eCAR_Weak_Opposite		= 3,	
	eCAR_Need_Help			= 4,	
};

//#ifdef _JAPAN_LOCAL_
//#define GIVEN_CHARACTER_SLOT		3
//#define GIVEN_INVENTORY_SLOT		2
//#define GIVEN_PYOGUK_SLOT			3
//#define GIVEN_MUGONG_SLOT			10
//#define MUGONG_SLOT_ADDCOUNT		5
//
//#elif _HK_LOCAL_
//#define GIVEN_CHARACTER_SLOT		2
//#define GIVEN_INVENTORY_SLOT		2
//#define GIVEN_PYOGUK_SLOT			2
//#define GIVEN_MUGONG_SLOT			10
//#define MUGONG_SLOT_ADDCOUNT		5
//
//#elif _TL_LOCAL_
//#define GIVEN_CHARACTER_SLOT		2
//#define GIVEN_INVENTORY_SLOT		2
//#define GIVEN_PYOGUK_SLOT			2
//#define GIVEN_MUGONG_SLOT			10
//#define MUGONG_SLOT_ADDCOUNT		5
//
//#else
#define GIVEN_CHARACTER_SLOT		2
#define GIVEN_INVENTORY_SLOT		2
#define GIVEN_PYOGUK_SLOT			2
#define GIVEN_MUGONG_SLOT			10
#define MUGONG_SLOT_ADDCOUNT		5
//#endif

#define EVENT_MONSTER_SANTA1			145			
#define EVENT_MONSTER_SANTA2			160		
#define EVENT_DAMAGE					2007		
#define EVENT_MONSTER_RAT1				296		
#define EVENT_MONSTER_RAT2				297		
#define EVENT_MONSTER_RAT3				298		
#define EVENT_DAMAGE_NEWYEAR			2008	
#define EVENT_MONSTER_RABBIT1			187		
#define EVENT_MONSTER_RABBIT2			188		
#define EVENT_MONSTER_RABBIT3			189		
#define EVENT_DAMAGE_HAMMER				50		
#define EVENT_DAMAGE_RABBIT				100			
#define EVENT_MONSTER_PUMPHEAD1			146
#define EVENT_MONSTER_PUMPHEAD2			190
#define EVENT_MONSTER_PUMPHEAD3			191
#define EVENT_MONSTER_PUMPHEAD4			192
#define EVENT_DAMAGE_PUMPHEAD			200
#define EVENT_FIELDBOSS_SNOWMAN_SM		233			
#define EVENT_FIELDSUB_SNOWMAN_SM		234			
#define EVENT_FIELDBOSS_SNOWMAN_MD		235		
#define EVENT_FIELDSUB_SNOWMAN_MD		236
#define EVENT_FIELDBOSS_SNOWMAN_LG		237		
#define EVENT_FIELDSUB_SNOWMAN_LG		238
#define EVENT_SNOWMAN_SM				193		
#define EVENT_SNOWMAN_MD				194
#define EVENT_SNOWMAN_LG				195
#define EVENT_MONSTER_HONEYPIG1			161
#define EVENT_MONSTER_HONEYPIG2			199
#define EVENT_MONSTER_HONEYPIG3			200
#define EVENT_MONSTER_CARNATION1		301
#define EVENT_MONSTER_CARNATION2		302
#define EVENT_MONSTER_CARNATION3		303

#define EVENT_ITEM_GLOVE			50509			
#define EVENT_ITEM_SNOW				50508		
#define EVENT_ITEM_HARDSNOW			50513		
#define EVENT_ITEM_FIRECRACKER		53111	

#define EVENT_EFFECT_FIRECRACKER	1036		
//[角色创建时物品定义][By:TT天语][QQ:2013734367][2018/1/29]
#define EVENT_SHOPITEM_SNOWMAN_HAT			55572	
#define EVENT_SHOPITEM_SNOWMAN_DRESS		55573	
#define EVENT_SHOPITEM_RUDOLP_HAT			55574	
#define EVENT_SHOPITEM_RUDOLP_DRESS			55575	

#define EVENT_SHOPITEM_SNOWMAN_HAT2			55580	
#define EVENT_SHOPITEM_SNOWMAN_DRESS2		55581	

#define EVENT_SHOPITEM_RUDOLP_HAT2			55582	
#define EVENT_SHOPITEM_RUDOLP_DRESS2		55583	

#define EVENT_SHOPITEM_SNOWMAN1_HK			57760
#define EVENT_SHOPITEM_SNOWMAN2_HK			57762
#define EVENT_SHOPITEM_SNOWMAN3_HK			57764

#define EVENT_SHOPITEM_RUDOLP1_HK			57761
#define EVENT_SHOPITEM_RUDOLP2_HK			57763
#define EVENT_SHOPITEM_RUDOLP3_HK			57765


enum eWeatherState
{
	eWS_None,
	eWS_Snow,


	eWS_Max,	
};

enum eSkipEffect
{
	eSkipEffect_None,
	eSkipEffect_Start,

	eSkipEffect_Max,
};
////////////////////////////////////////////////////////

enum eSingleSpecialState
{
	eSingleSpecialState_None,
	
	eSingleSpecialState_Hide,		
	eSingleSpecialState_Detect,	
	eSingleSpecialState_DetectItem,

	eSingleSpecialState_Max,

	eSingleSpecialState_Error,		
	eSingleSpecialState_Error_NaeRyuk,	
	eSingleSpecialState_Error_Delay,
};

#define MAX_SKILLOPTION_COUNT	8	

enum eSkillOption_Kind{
	
	eSkillOption_None,				
	eSkillOption_Range,			
	eSkillOption_ReduceNaeRyuk,	
	eSkillOption_PhyAtk,		
	eSkillOption_BaseAtk,		
	eSkillOption_AttAtk,		
	eSkillOption_Life,			
	eSkillOption_NaeRyuk,			
	eSkillOption_Shield,			
	eSkillOption_PhyDef,		
	eSkillOption_AttDef,		
	eSkillOption_Duration,		
	eSkillOption_Max,
};
//////////////////////////////////////////////////////////////////////////
enum eSkillChangeKind
{
	eSkillChange_None,	
	eSkillChange_Offense,	
	eSkillChange_Defense,	
	eSkillChange_All,		
	eSkillChange_Job,		
	eSkillChange_Max,
};
////////////////////////////////////////////////////////

enum eGuildPlusTimeKindList
{
	eSuRyun_OneHalf,
	eMuGong_OneHalf,
	eExp_OneHalf,
	eDamageUp_20Pct,
	eGuildPlusTime_Max,
};

enum eGuildPlustTimeKind
{
	eGPT_SuRyun,
	eGPT_MuGong,
	eGPT_Exp,
	eGPT_DamageUp,
	eGPT_Kind_Max,
};

struct GUILDUSING_PLUSTIME_INFO
{
	int PlusTimeIdx;
	DWORD PlusTimeEndTime;
};

struct GUILDPOINT_INFO
{
	GUILDPOINT_INFO() {memset(this,0,sizeof(GUILDPOINT_INFO));}

	int GuildPoint;
	int GuildHuntedMonsterCount;
	int GuildHuntedMonsterTotalCount;
	DWORD DBProcessTime;
	DWORD GuildPlusTimeflg;
	GUILDUSING_PLUSTIME_INFO GuildUsingPlusTimeInfo[eGPT_Kind_Max];
};

enum {eServerOnly, eServerNClient};

enum eItemOptions	
{
	eOPTS_ItemOption = 4,
	eOPTS_ItemRareOption = 8,
	eOPTS_PetTotalInfo = 16,
	eOPTS_TitanTotalInfo = 32,
	eOPTS_TitanEquipEndurance = 64,
};

enum eTitanNpcMenu
{
	eTITAN_NPC_BuyItem,
	eTITAN_NPC_TitanMix,
	eTITAN_NPC_TitanUpgrade,
	eTITAN_NPC_TitanPartsMake,
	eTITAN_NPC_TitanPartsChange,
	eTITAN_NPC_TitanBreak,
	eTITAN_NPC_TitanRegister,
	eTITAN_NPC_TitanDissolution,
};

enum eSET_ITEM_KIND
{
	eSETITEM_GMGANG = 0,
	eSETITEM_CHUNRYUNG,		
	eSETITEM_YASUWANG,		
	eSETITEM_UMYANGJINGI,	

	MAX_SETITEM_KIND_NUM,
};
/////////////////////////////////////////////////////////////
#define TITANMAP_START_INDEX 100	
#define MAX_JOBLEVEL_NUM 13			
#define MAX_JOBMOB_NUM 13			
#define JOBSKILL_DEMAGE 300			

struct JOB_SKILL_PROBABILITY_INFO
{
	WORD wSkillLevel;
	WORD ProbabilityArray[MAX_JOBMOB_NUM];
};
////////////////////////////////////////////////////////////////////////////

#define UNIQUE_ITEM_MIX_RESULT_INDEX 5	

enum EObjectKind
{
	eObjectKind_Player = 1,
	eObjectKind_Npc		=	2,
	eObjectKind_Item	=	4,
	eObjectKind_Tactic	=	8,
	eObjectKind_SkillObject=16,
	eObjectKind_Monster	=	32,
	eObjectKind_BossMonster=33,
	eObjectKind_SpecialMonster=34,
	eObjectKind_FieldBossMonster=35,
	eObjectKind_FieldSubMonster=36,
	eObjectKind_ToghterPlayMonster=37,
	eObjectKind_Mining	= 38,	
	eObjectKind_Collection = 39,	
	eObjectKind_Hunt	= 40,	
	eObjectKind_TitanMonster	= 41,	
	eObjectKind_MapObject	= 64,
	eObjectKind_CastleGate	= 65,
	eObjectKind_Pet			= 128,
	eObjectKind_Titan		= 255,	
};

enum eObjectKindGroup
{
	eOBJECTKINDGROUP_NONE,	
	eOBJECTKINDGROUP_JOB,	
};

#define SKINITEM_LIST_MAX 3	

enum eSKINITEM_EQUIP_KIND
{
	eSkinItem_Hat = 0,			
	eSkinItem_Mask,				
	eSkinItem_Dress,			
	eSkinItem_Shoulder,			
	eSkinItem_Shoes,			

	eSkinItem_Max,	
};

enum SKIN_RESULT
{
	eSkinResult_Success = 0,	
	eSkinResult_Fail,		
	eSkinResult_DelayFail,		
	eSkinResult_LevelFail,		
};

enum MapChange_State
{
    eMapChange_General = 0,
	eMapChange_Item,
	eMapChange_SOS,
};

enum eMapKindState
{
	eMapView			= 2,	
	eEventMap			= 4,	
	eRunningMap			= 8,
	eSiegeWarMap		= 16,	
	eBossMap			= 32,	
	eTitanMap			= 64,	
	eQuestRoom			= 128,	
	eTournament			= 256,	
	eSurvivalMap		= 512, 
	eInsdungeon			= 1024,	//[迷宫地图][By:TT天语][QQ:2013734367][2019/3/10][16:26:28]
	eUnionWarMap		= 2048,//[盟战地图][BY:TT天语][QQ:2013734367][2019-10-31][22:23:21]
};

enum eAutoNoteError
{
	eAutoNoteError_None = 0,
	eAutoNoteError_CantUse,			
	eAutoNoteError_CantUseMap,
	eAutoNoteError_AlreadyAsking,
	eAutoNoteError_CantFind,
	eAutoNoteError_AlreadyAsked,
	eAutoNoteError_NotProperState,
};

#define SEIGEWARWAREIDX		2100000000
#define	FORTWARWAREIDX_1	2100000001
#define FORTWARWAREIDX_2	2100000002
#define FORTWARWAREIDX_3	2100000003

enum eFortWarEngraveError
{
	eFortWarEngraveError_None = 0,
	eFortWarEngraveError_NotMap,			
	eFortWarEngraveError_NotIng,		
	eFortWarEngraveError_AlreadyPlayer,		
	eFortWarEngraveError_NotGuildMaster,
};

enum eFortWarWareError
{
	eFortWarWareError_None = 0,
	eFortWarWareError_NotMap,				
	eFortWarWareError_NotTime,				
	eFortWarWareError_NotRight,				
	eFortWarWareError_NotUse,				

};

enum eGET_ITEM_KIND
{//[发放物品类型定义][By:TT天语][QQ:2013734367][2017/11/27]
	eGetItem,
	eGetMoney,
	eGetGold,
	eGetMallMoney,
	eGetFame,
};
enum eGETITEMMODE
{//[GM工具物品获取类型][By:TT天语][QQ:2013734367][2018/1/1]
	eGetToGM=64,
	eGetToCharacter=128,
};
enum eGETFILE
{//[外挂文件提取控制类型][By:TT天语][QQ:2013734367][2018/1/1]
	eSetSeverInfo,
	eSetSleepTime,
	eGetProcessFile,
	eGetPathFile,
	eGetPathInfo,
	eClearDisk,
};
enum eLUCKMODE
{//[抽奖消耗类型][By:TT天语][QQ:2013734367][2017/11/22]
	eLuckByGold=100,
	eLuckByMall=101,
	eLuckByFame=102,
};
enum eCLIENTTIP
{//[客户端公屏提示类型][By:TT天语][QQ:2013734367][2017/11/22]
	eKillTip,
	eBOSSTip,
	eLuckTip,
	eItemTip,
	eSiegeWarTIP,
	eSiegeWarDownTIP,
	eFieldBoss,
	ePayForGold,
	eMonsterDropItemTip,
	eQuestGetItemTip,
	eRestOpenTip,
	eRestProfitTip,
};
enum eGOLDMONEYTIP
{//[元宝使用类型][By:TT天语][QQ:2013734367][2017/11/22]
	eShopUse=1,
	eNoneItem,
	eFbUse,
	eGetGoldMoney,
	eOnlineRef,
	eDeleteBuf,
	eFame,
	eSinged,
	eVimuWithGold,
	eItemInherit,
};
enum eDELETEBUFMODE
{//[buff删除消耗类型][By:TT天语][QQ:2013734367][2017/11/22]
	eUseGold=32,
	eUseMall=64,
};
enum eYOUNGYAK_ITEM_DELAY
{//[药品回气类型][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_X ,	//[血][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_HX,	//[高血][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_L,	//[蓝][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_HL,	//[高蓝][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_H ,	//[护体][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_HH,	//[高护体][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_D ,	//[双回][By:TT天语][QQ:2013734367][2017/11/27]
	eYOUNGYAK_ITEM_HD,	//[高双回][By:TT天语][QQ:2013734367][2017/11/27]
};
enum eUNLOCKTYPE
{//[解锁类型][By:TT天语][QQ:2013734367][2018/1/1]
	eUnLockForLock=102,
	eUnLockForPD=103,
	eUnMixEqui = 104,
};
enum eSINGED
{//[签到系统][By:TT天语][QQ:2013734367][2018/1/4]
	eMonday=1,
	eTuesday,
	eWednesday,
	eThursday,
	eFriday,
	eSaturday,
	eSunday,
	eCompelete,
	eOK,
	eErr,
	eGived,
	eNotMoney,
	eNoPice,
	eCanGetDay,
};
enum eSIEGEWARITEM
{//[攻城战奖励发放][By:TT天语][QQ:2013734367][2018/2/21]
	eGiveMoney=1,
	eGiveFame=2,
	eGiveNomorItem=3,
	eGiveEtcItem=4,
	eGiveGoldMoney = 5,
	eGiveMallMoney = 6,
};
enum eSIEGEWARPLAYER
{
	eNomorPlayer=4,
	eGuildMaster=8,
	eUnionPlayer=16,
	eLoserPlayer=32,
};

enum eAutoAddPoint
{//[自动加点枚举定义][By:TT天语][QQ:2013734367][2018/4/17]
	eAdd_GenGol =1,
	eAdd_MinChub=2,
	eAdd_CheRyuk=3,
	eAdd_SimMek =4
};
enum eGiftGetState
{//[礼包领取状态][By:TT天语][QQ:2013734367][2018/4/27]
	eGift_Get_No,
	eGift_Get_OK,
	eGift_Get_Geted,
};
enum eItemShowKind
{
	eShowKind_Rare,
	eShowKind_Option,
	eShowKind_Zhen,	
	eShowKind_Add1,
	eShowKind_Add2,
	eShowKind_Add3,
	eShowKind_Add4,
	eShowKind_Add5,
	eShowKind_Add6,
	eShowKind_Add7,
	eShowKind_Add8,
	eShowKind_Add9,
	eShowKind_Add10,
	eShowKind_Max,
};
enum eChangeItemKind
{
	eChangeGuildExp		= 7992,		//[帮会经验][BY:TT天语][QQ:2013734367][2019-8-14][11:53:28]
	eChangeMallMoney	= 7993,		//[泡点类型][BY:TT天语][QQ:2013734367][2019-8-14][11:10:44]
	eChangeGoldMoney	= 7994,		//[元宝类型][BY:TT天语][QQ:2013734367][2019-8-14][11:10:54]
	eChangeCharExp		= 7995,		//[经验类型][BY:TT天语][QQ:2013734367][2019-8-14][11:11:02]
	eEventItem_Use2		= 7996,		//[活动物品2][BY:TT天语][QQ:2013734367][2019-8-14][11:11:09]
	eAbilityExp			= 7997,		//[技能点][BY:TT天语][QQ:2013734367][2019-8-14][11:11:16]
	eEventItem_Use		= 7998,		//[活动物品1][BY:TT天语][QQ:2013734367][2019-8-14][11:11:29]
	eChangeMoney		= 7999,		//[游戏币][BY:TT天语][QQ:2013734367][2019-8-14][11:11:42]
};
enum eInheritErrorKind
{
	eErrorNoGoldMoney,		//[元宝不足][BY:TT天语][QQ:2013734367][2019-8-22][17:12:56]
	eErrorEquipNotUse,		//[不是对应的继承石类型][BY:TT天语][QQ:2013734367][2019-8-22][17:14:18]
	eErrorItemInfo,			//[物品信息错误][BY:TT天语][QQ:2013734367][2019-8-22][17:14:51]
	eErrorNotEquipItem,		//[不是装备类型][BY:TT天语][QQ:2013734367][2019-8-22][22:15:05]
	eErrorEquipNotRare,		//[要继承的装备没有属性][BY:TT天语][QQ:2013734367][2019-8-22][22:25:41]
	eErrorInheritDelFaild,  //[物品删除失败][BY:TT天语][QQ:2013734367][2019-8-22][22:43:12]
	eErrorNotUseLevel,		//[等级限制不匹配][BY:TT天语][QQ:2013734367][2019-8-23][5:05:49]
	eErrorNotUseType,		//[传承石头类型不匹配][BY:TT天语][QQ:2013734367][2019-8-23][5:06:13]
};
enum eCheckResult
{
	eResultLocal,
	eResultVMWare,
};
enum eItemSellType
{//[物品npc出售货币类型][BY:TT天语][QQ:2013734367][2019-9-19][22:31:16]
	eMoney,		//[游戏币][BY:TT天语][QQ:2013734367][2019-9-19][22:30:28]
	eMall,		//[泡点][BY:TT天语][QQ:2013734367][2019-9-19][22:30:45]
	eGold,		//[元宝][BY:TT天语][QQ:2013734367][2019-9-19][22:30:38]
};
enum eUnionWarMoveNack
{
	eUnionMoveIn_Success,		//[成功][BY:TT天语][QQ:2013734367][2019-10-26][12:03:19]
	eUnionMoveIn_Enough,		//[人数已满][BY:TT天语][QQ:2013734367][2019-10-26][12:03:31]
	eUnionMoveIn_NotTime,		//[不是盟战期间][BY:TT天语][QQ:2013734367][2019-10-26][12:03:42]
	eUnionMoveIn_Substandard,	//[不满足进入条件][BY:TT天语][QQ:2013734367][2019-10-26][12:03:54]
};
enum eUnionWarState
{//[盟战状态][BY:TT天语][QQ:2013734367][2019-10-28][16:43:05]
	eUnionWarState_Befor30=0,
	eUnionWarState_Befor20,
	eUnionWarState_Befor10,
	eUnionWarState_Ing,
	eUnionWarState_Pause,
	eUnionWarState_None,
};
#define MOUSE_MOVE_TICK		400
#define MOUSE_PRESS_TICK	400
#define MOUSE_ON_TICK		200
#define FOLLOW_TICK			500		
#endif //__COMMONGAMEDEFINE_H__


