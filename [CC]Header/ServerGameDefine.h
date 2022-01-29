



#ifndef __SERVERGAMEDEFINE_H__
#define __SERVERGAMEDEFINE_H__



#define MAX_TOTAL_PLAYER_NUM		800
#define MAX_TOTAL_TITAN_NUM			800

#define MAX_TITANINFO_NUM			800

#define MAX_TOTAL_PET_NUM			800
#define MAX_TOTAL_MONSTER_NUM		1000
#define MAX_TOTAL_BOSSMONSTER_NUM	10
#define MAX_TOTAL_NPC_NUM			50
#define MAX_TOTAL_TACTIC_NUM		200
#define MAX_MAPOBJECT_NUM			20




#define GRID_BIT 5


extern DWORD gCurTime;
extern DWORD gTickTime;

// °æÇèÄ¡ & ¾Æ?ÌÅÛ & µ· µå¶ø?² Á¶?ý - RaMa 04.10.16 
extern float gExpRate;
extern float gAbilRate;
extern float gItemRate;
extern float gMoneyRate;
//
extern float gDamageReciveRate;		// ¹Þ´Â µ¥¹ÌÁö
extern float gDamageRate;			// ÁÖ´Â µ¥¹ÌÁö
extern float gNaeRuykRate;			// ³»·Â¼Ò¸ð
extern float gUngiSpeed;			// ¿î±âÁ¶½Ä ½ºÇÇµå
extern float gPartyExpRate;			// ÆÄÆ¼°æÇèÄ¡
extern float gGetMoney;				// ¾ò´Âµ·?Ç ¹è¼ö
extern float gMugongExpRate;	// ��������ġ ���
// Etc
extern float gShield;				// È£½Å°­±â
extern float gDefence;				// ¹æ¾î·Â

extern float gEventRate[];
extern float gEventRateFile[];
struct PARTYEVENT;
extern PARTYEVENT gPartyEvent[];

#define _SERVER_RESOURCE_FIELD_			// ¼­¹ö¸¸?» ±¸ºÐÇÏ´Â ¿µ¿ª

#define START_LOGIN			0
#define CHANGE_LOGIN		1

#define MONSTERGROUPUNIT	2
#define NPCGROUPUINT		1


#define OBJ_REGEN_START_INDEX	100001

enum SERVER_KIND
{
	ERROR_SERVER,
	DISTRIBUTE_SERVER,

	AGENT_SERVER,
	MAP_SERVER,
	CHAT_SERVER,
	MURIM_SERVER,
	MONITOR_AGENT_SERVER,
	MONITOR_SERVER,
	BUDDYAUTH_SERVER,
	RECONNECT_SERVER,
	MAX_SERVER_KIND,
};
#define MAX_IPADDRESS_SIZE	16
#define MAX_MAP_NUM		100
#define MAX_SERVER_COUNT	30

enum CHEAT_LOG
{
	eCHT_Item,
	eCHT_Money,
	eCHT_Hide,
	eCHT_AddMugong,
	eCHT_MugongSung,
	eCHT_LevelUp,
	eCHT_AbilityExp,
	eCHT_Gengol,
	eCHT_Minchub,
	eCHT_Cheryuk,
	eCHT_Simmak,
};

enum eBOSSEVENT
{
	eBOSSEVENT_LIFE,
};

enum ebossstate
{	
	//////////////////////////////////////////////////////////////////////////
	//NORMAL STATE
	eBossState_Stand,
	eBossState_WalkAround,
	eBossState_Pursuit,
	eBossState_RunAway,
	eBossState_Attack,

	//////////////////////////////////////////////////////////////////////////
	//EVENT STATE
	eBossState_Recover,
	eBossState_Summon,
			
	eBossState_Max,
};



enum ebossaction
{
	eBOSSACTION_RECOVER = 1,
	eBOSSACTION_SUMMON,
};

enum ebosscondition
{
	eBOSSCONDITION_LIFE = 1,	
};

enum eGuildLog

{
	// member
	eGuildLog_MemberSecede = 1,
	eGuildLog_MemberBan,

	eGuildLog_MemberAdd,

	// master 
	eGuildLog_MasterChangeRank = 100,
	

	// guild
	eLogGuild_GuildCreate = 200,
	eGuildLog_GuildBreakUp,
	eGuildLog_GuildLevelUp,
	eGuildLog_GuildLevelDown,
	
	// guildunion
	eGuildLog_CreateUnion = 400,
	eGuildLog_DestroyUnion,
	eGuildLog_AddUnion,
	eGuildLog_RemoveUnion,	
	eGuildLog_SecedeUnion,
};

enum ePunishKind			
{
	ePunish_Login = 0,
	ePunish_AutoNoteUse,	
	ePunish_Chat,
	ePunish_Trade,	
	ePunish_Max,
};

enum ePunishCountKind
{
	ePunishCount_AutoUser = 0,
	ePunishCount_NoManner,
	ePunishCount_TradeCheat,	
	ePunishCount_Max,
};

#endif //__SERVERGAMEDEFINE_H__


