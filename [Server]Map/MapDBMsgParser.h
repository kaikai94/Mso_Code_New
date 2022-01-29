#ifndef __MAPDBMSGPARSER_H__
#define __MAPDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CPlayer;
class CParty;
class CAbilityGroup;

enum   DBMESSAGEIDFUNC 
{
	eNull,
	eCharacterInfoQuery,		
	eCharacterMugongQuery,		
	eCharacterItemSlotQuery,	
	eCharacterItemQuery,		
	eCharacterItemOptionQuery, 
	eCharacterItemOptionDeleteQuery,
	eCharacterItemRareOptionQuery,
	eCharacterItemRareOptionDeleteQuery,
	ePyogukItemOptionQuery,
	ePyogukItemRareOptionQuery,
	eShopItemInfoQuery,			                
	eSSItemInsert,				
	eItemOptionInsert,
	eItemRareInsert,		
	eItemUpdate,
	eItemCombineUpdate,
	eItemMoveUpdate,
	eItemMovePyogukUpdate,
	eItemMoveGuildUpdate,
	eItemMovePetInvenUpdate,
	eItemInsert,
	eItemDelete,
	eItemShopInvenInfo,
	eItemShopItemUseInfo,
	eItemShopItemUsing,
	eItemShopItemDelete,
	eItemShopItemUpdatetime,
	eItemShopItemUpdateParam,
	eItemShopItemUpdateUseParam,
	eItemShopItemUpdateUseInfo,
	eItemShopItemMPInfo,
	eItemShopMPUpdate,
	eItemShopMPDel,
	eItemShopGetItem,
	eItemMoveShopUpdate,
	eItemShopChangeName,
	eItemCharChange,
	eItemShopRareCreate,
	eItemShopIsUseableAvatar,
	eItemShopIsUseableEquip,
	eItemShopUpdatCharacterIdx,
	eItemShopUpdateAllInfo,
	eRegistLoginMapInfo,
	eUnRegistLoginMapInfo,
	eMugongUpdate2,
	eMugongMoveUpdate2,
	eMugongInsert,
	eMugongDelete,				
	eBaseEconomy,				
	eAuctionRegist,				
	eCharacterUpdate,			
	eHeroInfoUpdate,			
	eTotalInfoUpdate,		
	eBadFameUpdate,
	eAuctionCheck,
	eAuctionPageList,
	eAuctionConfirm,
	ePartyRegistMapServer,		
	ePartyInfoByUserLogin,		
	ePartyCreate,
	ePartyBreakup,
	ePartyCharacterTbUpdate,	
	ePartyAddMember,		
	ePartyDelMember,
	ePartyChangeMaster,
	eSavePoint,					
	eSaveMapChangePoint,
	ePyogukBuyPyoguk,
	eCharacterPyogukInfo,
	eCharacterPyogukItemInfo,
	ePyogukMoneyUpdate,
	eFriendNotifyLogout,
	eFameCharacterUpdate,
	eBadFameCharacterUpdate,
	ePKCharacterUpdate,
	eWantedLoad,
	eWantedGetList,
	eWantedBuyRight,
	eWantedRegist,
	eWantedGiveupRight,
	eWantedComplete,
	eWantedDestroyed,
	eWantedOrderList,
	eWantedMyList,
	eWantedSearch,
	eQuestTotalInfo,	
	eQuestUpdate,
	eQuestMainQuestLoad,
	eQuestSubQuestLoad,
	eQuestItemLoad,
	eQuestMainQuestInsert,
	eQuestSubQuestInsert,
	eQuestItemInsert,
	eQuestMainQuestUpdate,
	eQuestSubQuestUpdate,
	eQuestItemUpdate,
	eQuestMainQuestDelete,
	eQuestSubQuestDelete,
	eQuestSubQuestDeleteAll,
	eQuestItemDelete,
	eQuestItemDeleteAll,
	eQuestMainQuestUpdateCheckTime,
	eJournalGetList,
	eJournalInsertQuest,
	eJournalInsertWanted,
	eJournalInsertLevel,
	eJournalSaveUpdate,
	eJournalDelete,
	eLogCharacter,
	eLogExp,
	eLogMoney,
	eLogMoneyWrong,
	eLogMugong,
	eLogMugongExp,
	eLogItemMoney,
	eLogItemReinforce,
	eLogItemRare,
	eLogCheat,
	eInsertLogTool,
	eLogGuild,
	eLogPet,
	eLogGuildPoint,
	eLogGMToolUse,
	eCharacterInfoByTime,
	eMugongExpByTime,
	eSpeedHackCheck,
	eGameLogItem,
	eGuildLoadGuild,
	eGuildLoadMember,
	eGuildCreate,
	eGuildBreakUp,
	eGuildDeleteMember,
	eGuildAddMember,
	eGuildSecedeMember,
	eGuildLevelUp,
	eGuildChangeMemberRank,
	eGuildItemLoad,
	eGuildItemOption,
	eGuildItemRareOption,
	eGuildMoneyUpdate,
	eGuildGiveNickName,
	eConfirmUserOut,
	eSaveChrInfoBeforeOut,
	eCharacterInfoQueryToEventMap,
	eLoadGuildWarRecord,
	eDeleteGuildWarRecord,
	eLoadGuildFieldWar,
	eInsertGuildFieldWar,
	eDeleteGuildFieldWar,	
	eUpdateGuildFieldWarRecord,
	eGuildTournament_LoadAll,
	eGuildTournament_CancelLoad,
	eGuildTournament_CancelInsert,
	eGuildTournament_Insert,
	eGuildTournament_Delete,
	eGuildTournament_UpdateGuildInfo,
	eGuildTournament_UpdateTournamentInfo,	
	eJackpot_TotalMoney_Update,
	eJackpot_PrizeInfo_Update,
	eGuildUnionLoad,
	eGuildUnionCreate,
	eGuildUnionDestroy,
	eGuildUnionAddGuild,
	eGuildUnionRemoveGuild,
	eGuildUnionSecedeGuild,
	eSiegeWarInfoLoad,
	eSiegeWarInfoInsert,
	eSiegeWarInfoUpdate,
	eSiegeWarGuildInfoLoad,
	eSiegeWarGuildInsert,
	eSiegeWarGuildUpdate,
	eSiegeWarGuildDelete,
	eChangeCharacterAttr,
	eSiegeWarProfitInfoLoad,
	eSiegeWarProfitUpdateGuild,
	eSiegeWarProfitUpdateTaxRate,
	eSiegeWarProfitUpdateProfitMoney,
	eChangeGuildLocation,
	eChangeCharacterStageAbility,
	eCharacterPetInfoQuery,
	ePetInsert,
	ePetUpdate,
	ePetDelete,
	ePetInvenItemOptionInfoQuery,		
	ePetInvenItemRareOptionInfoQuery,	
	ePetInvenItemInfoQuery,		
	eGuildLoadNotice,
	eGuildUpdateNotice,
	eQuestEventCheck,
	eQuestEventEnd,
	ePetWearItemInfoQuery,		
	eGMEvent01,
	eGuildPointInfoLoad,
	eGuildPlustimeInfoLoad,
	eGuildAddHuntedMonsterCount,
	eGuildGetHuntedMonsterTotalCountWithInit,
	eGuildAddGuildPoint,
	eGuildUseGuildPoint,
	eGuildPlustimeEnd,
	eGuildTournamentAddLog,
	eSiegeWarAddLog,
	eGuildItemLoadInNeed,
	eGuildItemOptionInNeed,
	eGuildItemRareOptionInNeed,
	eCharacterExpFlag,	
	eGuildTraineeInfo,
	eGuildTraineeInsert,
	eGuildTraineeDelete,
	eGuildTraineeGuildInfo,
	eGuildTraineeGuildDelete,
	eGuildTraineeDeletebyGuild,
	eGuildAddStudent,
	eGuildStudentLvUpCount,	
	eGuildStudentLvUpInfoInit,
	eTitanWearItemInfo,	
	eCharacterTitanInfoQuery,	
	eCharacterTitanEquipItemEnduranceInfoQuery,	
	eTitanInsert,
	eTitanUpdate,
	eTitanDelete,
	eTitanEquipInfoInsert,
	eTitanEquipInfoUpdate,
	eTitanEquipInfoDelete,
	eTestGameQuery,
	eTestLogQuery,
	ePyogukTitanEnduranceOptionQuery,	
	eQuest_EndQuest_New,
	eQuest_DeleteQuest_New,
	eQuest_EndSubQuest_New,
	eTitanEquipInfoInsertException,	
	eTitanEquipInfoInsertExceptionPyoguk,	
	eCharacterSkinInfoUpdate,			
	eCharacterUpdateResetStatusPoint,	
	eCharacterSkinInfo,			
	eItemLimitInfoLoadAll,	
	eItemLimitInfoUpdate,
	eAutoNoteListLoad,		
	eAutoNoteListAdd,
	eMapItemDropListSelect,
	eMapItemDropListUpdate,
	eMapItemDropListInit,
	eFortWarInfoLoad,
	eFortWarInfoUpdate,
	eFortWarProfitMoneyUpdate,
	eFortWarItemLoad,
	eFortWarItemInsertToDB,
	eFortWarItemMoveToDB,         
	eItemUpdateLock,     
	eItemUpdateUnLock,   
	eItemUpdateGrow,  
	eGetDBGoldMoney,	//[元宝查询][By:十里坡剑神][QQ:112582793][2017/11/23]
	eItemUpdateAbrasion, //[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	eItemMakerUpdate,	 //[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	eItemMaker1Update,	 //[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	eLoadSingedInfo,	 //[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	eUpdateSingedInfo,	 //[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	eSingedInfoRest,	 //[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	eLoadGameConfig,	//[游戏设置加载][By:十里坡剑神][QQ:112582793][2017/11/23]
	eLoadRechargeInfo,	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	eLoadKillTopInfo,	//[角色击杀排行信息][By:十里坡剑神][QQ:112582793][2018/4/22]
	eUpdateMallMoney,	  //[泡点同步][By:十里坡剑神][QQ:112582793][2018/6/18]
	eMovePakLoadItemInfo,	//[背包整理回调][By:十里坡剑神][QQ:112582793][2019/3/17][14:18:59]
	MaxQuery
};
//////////////////////////////////////////////////////////////////////////
#define STORED_TEST_QUERY	"UP_GAME_TEST_QUERY"
#define STORED_CHARACTER_SELECT				"dbo.MP_CHARACTER_SelectByCharacterIDX"		
#define STORED_CHARACTER_KYUNGGONG			"dbo.MP_CHARACTER_KyungGong"				
#define STORED_CHARACTER_NAEGONG			"dbo.MP_CHARACTER_NaeGong"					
#define STORED_CHARACTER_MUGONGINFO			"dbo.MP_CHARACTER_MugongInfo"			
#define STORED_CHARACTER_ITEMSLOTINFO		"dbo.MP_CHARACTER_ItemSlotInfo_JP"			
#define STORED_CHARACTER_ITEMINFO			"dbo.MP_CHARACTER_ItemInfo"					
#define STORED_CHARACTER_ITEMRAREOPTIONINFO	"dbo.MP_CHARACTER_ItemRareOptionInfo"
#define STORED_CHARACTER_ITEMOPTIONINFO		"dbo.MP_CHARACTER_ItemOptionInfo"			
#define STORED_CHARACTER_SKILLINFO			"dbo.MP_CHARACTER_SkillInfo"				
#define STORED_CHARACTER_EXPFLAG			"dbo.MP_CHARACTER_UpdateExpFlag"
////////////////////////////////////////////////////////////////////////////////////
#define STORED_CHARACTER_BASEECONOMY		"dbo.MP_MAP_BaseEconomy"					
#define STORED_CHARACTER_AUCTIONREGIST		"dbo.MP_Auction_Regist"						
#define STORED_CHARACTER_AUCTIONONEPAGE		"dbo.MP_Auction_PageCalculate"			
#define STORED_CHARACTER_HEROINFOUPDATE		"dbo.MP_CHARACTER_HeroInfoUpdate"
#define STORED_CHARACTER_TOTALINFOUPDATE	"dbo.MP_CHARACTER_TotalInfoUpdate"
#define STORED_CHARACTER_BADFAMEUPDATE		"dbo.MP_CHARACTER_BadFameUpdate"
#define STORED_CHARACTER_REGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_Regist"				
#define STORED_CHARACTER_UNREGISTLOGINMAPINFO "dbo.MP_LogInMapInfo_UnRegist"			
#define STORED_CHARACTER_SAVEINFOBEFORELOGOUT "dbo.MP_CHARACTER_SaveInfoBeforeLogOut"
#define STORED_ITEM_UPDATE					"dbo.MP_ITEM_Update"
#define STORED_ITEM_COMBINEUPDATE			"dbo.MP_ITEM_CombineUpdate"
#define STORED_ITEM_MOVEUPDATE_INVEN		"dbo.MP_ITEM_MoveUpdate"
#define STORED_ITEM_MOVEUPDATE_PYOGUK		"dbo.MP_ITEM_MoveUpdatePyoguk"
#define STORED_ITEM_MOVEUPDATE_GUILD		"dbo.MP_ITEM_MoveUpdateMunpa"
#define STORED_ITEM_INSERT_INVEN			"dbo.MP_ITEM_Insert"
#define STORED_ITEM_DELETE					"dbo.MP_ITEM_Delete"
#define STORED_ITEM_RARE_OPTION_INSERT		"dbo.MP_ITEM_RARE_OPTION_Insert"
#define STORED_ITEM_RARE_OPTION_DELETE		"dbo.MP_ITEM_RARE_OPTION_Delete"
#define STORED_ITEM_OPTION_INSERT			"dbo.MP_ITEM_OPTION_Insert"
#define STORED_ITEM_OPTION_DELETE			"dbo.MP_ITEM_OPTION_Delete"        
#define STORED_ITEM_UPDATE_LOCK				"dbo.MP_ITEM_Update_Lock"
#define STORED_ITEM_UPDATE_UNLOCK			"dbo.MP_ITEM_Update_UnLock"
#define STORED_ITEM_UPDATE_GROW             "dbo.MP_ITEM_Update_Grow"
#define STORED_MUGONG_UPDATE				"dbo.MP_MUGONG_Update"
#define STORED_MUGONG_MOVEUPDATE			"dbo.MP_MUGONG_MoveUpdate"
#define STORED_MUGONG_INSERT				"dbo.MP_MUGONG_Insert"
#define STORED_MUGONG_DELETE				"dbo.MP_MUGONG_Delete"
#define STORED_AUCTION_CHECK		"dbo.MP_AUCTION_CHECK"
#define STORED_AUCTION_SEARCH		"dbo.MP_AUCTION_SEARCH"
#define STORED_AUCTION_SORT			"dbo.MP_AUCTION_SORT"
#define STORED_AUCTION_REGISTER		"dbo.MP_AUCTION_REGISTER"
#define STORED_AUCTION_JOIN			"dbo.MP_AUCTION_JOIN"
#define STORED_AUCTION_CANCEL		"dbo.MP_AUCTION__REGISTER_CANCEL"
#define STORED_PARTY_REGIST_MAPSERVER	"dbo.MP_PARTY_Regist"
#define STORED_PARTY_INFO_BY_USERLOGIN	"dbo.MP_PARTY_PartyInfoByUserLogin"
#define STORED_PARTY_CREATE				"dbo.MP_PARTY_CreateParty"
#define STORED_PARTY_BREAK				"dbo.MP_PARTY_BreakupParty"
#define STORED_PARTY_CHARACTERUPDATE	"dbo.MP_PARTY_DelPartyidxinCharacterTB"
#define STORED_PARTY_UPDATE				"dbo.MP_PARTY_UpdateMember"
#define STORED_PARTY_ADDMEMBER			"dbo.MP_PARTY_AddMember"
#define STORED_PARTY_DELMEMBER			"dbo.MP_PARTY_DelMember"
#define STORED_PARTY_CHANGEMASTER		"dbo.MP_PARTY_ChangeMaster"
#define STORED_GUILD_CREATE				"dbo.MP_GUILD_Create"
#define STORED_GUILD_BREAKUP			"dbo.MP_GUILD_BreakUp"
#define STORED_GUILD_DELETEMEMBER		"dbo.MP_GUILD_DeleteMember"
#define STORED_GUILD_ADDMEMBER			"dbo.MP_GUILD_AddMember"
#define STORED_GUILD_LOADGUILD			"dbo.MP_GUILD_LoadGuild"
#define STORED_GUILD_LOADNOTICE			"dbo.MP_GUILD_LoadNotice"
#define STORED_GUILD_UPDATENOTICE		"dbo.MP_GUILD_UpdateNotice"
#define STORED_GUILD_LOADMEMBER			"dbo.MP_GUILD_LoadMember"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"	
#define STORED_GUILD_MARKUPDATE			"dbo.MP_GUILD_MarkUpdate"
#define STORED_GUILD_LOADMARK			"dbo.MP_GUILD_LoadMark"
#define STORED_GUILD_LEVELUP			"dbo.MP_GUILD_LevelUp"
#define STORED_GUILD_CHANGRANK			"dbo.MP_GUILD_ChangeRank"
#define STORED_GUILD_LOADITEM			"dbo.MP_GUILD_LoadItem"
#define STORED_GUILD_GIVENICKNAME		"dbo.MP_GUILD_GiveMemberNickName"
#define STORED_GUILD_MONEYUPDATE		"dbo.MP_GUILD_MoneyUpdate"
#define STORED_GUILD_ITEMOPTION			"dbo.MP_GUILD_ItemOption_Info"
#define STORED_GUILD_MARKREGIST			"dbo.MP_GUILD_MarkRegist"
#define STORED_GUILD_TRAINEE_INFO		"dbo.MP_GUILD_TRAINEE_Info"
#define STORED_GUILD_TRAINEE_INSERT		"dbo.MP_GUILD_TRAINEE_Insert"
#define STORED_GUILD_TRAINEE_DELETE		"dbo.MP_GUILD_TRAINEE_Delete"
#define STORED_GUILD_TRAINEE_GUILDINFO			"dbo.MP_GUILD_TRAINEE_GuildInfo"
#define STORED_GUILD_TRAINEE_GUILDINSERT		"dbo.MP_GUILD_TRAINEE_GuildInsert"
#define STORED_GUILD_TRAINEE_GUILDDELETE		"dbo.MP_GUILD_TRAINEE_GuildDelete"
#define STORED_GUILD_TRAINEE_DELETEBYGUILD		"dbo.MP_GUILD_TRAINEE_DeletebyGuild"
#define STORED_GUILD_ADD_STUDENT				"dbo.MP_GUILD_AddStudent"
#define STORED_GUILD_TRAINEE_GUILDINTROINFO		"dbo.MP_GUILD_TRAINEE_GuildIntroInfo"
#define  STORED_GUILD_TRAINEE_GUILDINTROUPDATE	"dbo.MP_GUILD_TRAINEE_GuildIntroUpdate"
#define	STORED_TITAN_WEARITEMINFO	"dbo.MP_TITAN_WearItemInfo"
#define STORED_PYOGUK_BUYPYOGUK			"dbo.MP_PYOGUK_Buy"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"
#define STORED_PYOGUK_TITAN_ENDURANCE_INFO	"dbo.MP_PYOGUK_Titan_Endurance_Info"
#define STORED_PYOGUK_INFO				"dbo.MP_PYOGUK_Info"
#define STORED_PYOGUK_ITEMINFO			"dbo.MP_PYOGUK_ItemInfo"
#define STORED_PYOGUK_MONEYUPDATE		"dbo.MP_PYOGUK_MoneyUpdate"
#define STORED_FRIEND_NOTIFYLOGOUT		"dbo.MP_FRIEND_NotifyLogout"
#define STORED_SHOPINVEN_INFO			"dbo.MP_SHOPITEM_InvenInfo"
#define STORED_SHOPITEM_INFO			"dbo.MP_SHOPITEM_ItemInfo"
#define STORED_SHOPITEM_USEINFO			"dbo.MP_SHOPITEM_UseInfo"
#define STORED_SHOPITEM_USING			"dbo.MP_SHOPITEM_Using"
#define STORED_SHOPITEM_USING_JP		"dbo.MP_SHOPITEM_Using_JP"
#define STORED_SHOPITEM_DELETE			"dbo.MP_SHOPITEM_Delete"
#define STORED_SHOPITEM_UPDATETIME		"dbo.MP_SHOPITEM_Updatetime"
#define STORED_SHOPITEM_UPDATEPARAM		"dbo.MP_SHOPITEM_UpdateParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM	"dbo.MP_SHOPITEM_UpdateUseParam"
#define STORED_SHOPITEM_UPDATEUSEPARAM_JP	"dbo.MP_SHOPITEM_UpdateUseParam_JP"
#define STORED_SHOPITEM_GETITEM			"dbo.MP_SHOPITEM_GetItem"
#define STORED_ITEM_MOVEUPDATE_SHOP		"dbo.MP_ITEM_MoveUpdateShop"
#define STORED_SHOPITEM_CHANGENAME		"dbo.MP_character_rename"
#define STORED_SHOPITEM_CHARCHANGE		"dbo.MP_SHOPITEM_CharChange"
#define STORED_SHOPITEM_MPINFO			"dbo.MP_MOVEPOINT_GetInfo"
#define STORED_SHOPITEM_MPINSERT		"dbo.MP_MOVEPOINT_Insert"
#define STORED_SHOPITEM_MPUPDATE		"dbo.MP_MOVEPOINT_Update"
#define STORED_SHOPITEM_MPDEL			"dbo.MP_MOVEPOINT_Delete"
#define STORED_SHOPITEM_UPDATEUSEINFO	"dbo.MP_SHOPITEM_UpdateUseInfo"
#define STORED_SHOPITEM_AVATARABLE		"dbo.MP_SHOPITEM_IsAbleAvatar"
#define STORED_SHOPITEM_EQUIPABLE		"dbo.MP_SHOPITEM_IsAbleEquip"
#define STORED_SHOPITEM_UPDAETALL		"dbo.MP_SHOPITEM_UpdateAll"
#define STORED_FAME_CHARACTERUPDATE		"dbo.MP_FAME_CharacterUpdate"
#define STORED_BADFAME_CHARACTERUPDATE	"dbo.MP_BADFAME_CharacterUpdate"
#define STORED_PK_CHARACTERUPDATE		"dbo.MP_PK_CharacterUpdate"
#define STORED_WANTED_LOAD				"dbo.MP_WANTED_LoadList"
#define STORED_WANTED_INFO				"dbo.MP_WANTED_InfoByUserLogIn"
#define STORED_WANTED_BUYRIGHT			"dbo.MP_WANTED_BuyRight"
#define STORED_WANTED_REGIST			"dbo.MP_WANTED_Regist"
#define STORED_WANTED_GIVEUP			"dbo.MP_WANTED_GiveUpRight"
#define STORED_WANTED_COMPLETE			"dbo.MP_WANTED_Complete"
#define STORED_WANTED_DESTROYED			"dbo.MP_WANTED_Destroyed"
#define STORED_WANTED_ORDERLIST			"dbo.MP_WANTED_OrderList"
#define STORED_WANTED_MYLIST			"dbo.MP_WANTED_MyList"
#define STORED_WANTED_SEARCH			"dbo.MP_WANTED_Search"
#define STORED_QUEST_MAINQUEST_LOAD		"dbo.MP_MAINQUEST_Load"
#define STORED_QUEST_MAINQUEST_INSERT	"dbo.MP_MAINQUEST_Insert"
#define STORED_QUEST_MAINQUEST_DELETE	"dbo.MP_MAINQUEST_Delete"
#define STORED_QUEST_MAINQUEST_UPDATE	"dbo.MP_MAINQUEST_Update"
#define STORED_QUEST_SUBQUEST_LOAD		"dbo.MP_SUBQUEST_Load"
#define STORED_QUEST_SUBQUEST_INSERT	"dbo.MP_SUBQUEST_Insert"
#define STORED_QUEST_SUBQUEST_DELETE_ALL "dbo.MP_SUBQUEST_Delete_All"
#define STORED_QUEST_SUBQUEST_DELETE	"dbo.MP_SUBQUEST_Delete"
#define STORED_QUEST_SUBQUEST_UPDATE	"dbo.MP_SUBQUEST_Update"
#define STORED_QUEST_ITEM_LOAD			"dbo.MP_QUESTITEM_Load"
#define STORED_QUEST_ITEM_INSERT		"dbo.MP_QUESTITEM_Insert"
#define STORED_QUEST_ITEM_DELETE		"dbo.MP_QUESTITEM_Delete"
#define STORED_QUEST_ITEM_DELETE_ALL	"dbo.MP_QUESTITEM_Delete_All"
#define STORED_QUEST_ITEM_UPDATE		"dbo.MP_QUESTITEM_Update"
#define STORED_JOURNAL_GETLIST			"dbo.MP_JOURNAL_GetList"
#define STORED_JOURNAL_INSERTQUEST		"dbo.MP_JOURNAL_InsertQuest"
#define STORED_JOURNAL_INSERTWANTED		"dbo.MP_JOURNAL_InsertWanted"
#define STROED_JOURNAL_INSERTLEVEL		"dbo.MP_JOURNAL_InsertLevel"
#define STROED_JOURNAL_SAVEUPDATE		"dbo.MP_JOURNAL_SaveUpdate"
#define STROED_JOURNAL_DELETE			"dbo.MP_JOURNAL_Delete"
#define STORED_MAPCHANGE_CHARINFO		"dbo.MP_MAPCHANGE_CharInfo"
#define STORED_MAPCHANGE_MUGONGINFO		"dbo.MP_CHARACTER_MugongInfo"
#define STORED_SAVE_MAPCHAGEPOINT		"dbo.MP_CHARACTER_MapchangePointUpdate"
#define STORED_SAVE_MAPCHAGEPOINTRET	"dbo.MP_CHARACTER_MapchangePointReturn"
#define STORED_SAVE_LOGINPOINT			"dbo.MP_CHARACTER_LoginPointUpdate"
#define STORED_MAP_SAVEPOINT			"dbo.MP_MAP_SavePoint"
#define STORED_GT_LOADALL				"dbo.MP_GT_LoadAll"
#define STORED_GT_CANCELLOAD			"dbo.MP_GT_CancelLoad"
#define STORED_GT_CANCELINSERT			"dbo.MP_GT_CancelInsert"
#define STORED_GT_TOURNAMENTINFO_UPDATE	"dbo.MP_GT_TrInfoUpdate"
#define STORED_GT_GUILDINFO_INSERT		"dbo.MP_GT_Insert"
#define STORED_GT_GUILDINFO_DELETE		"dbo.MP_GT_Delete"
#define STORED_GT_GUILDINFO_UPDATE		"dbo.MP_GT_Update"
#define STORED_SIEGE_INFOLOAD			"dbo.MP_SIEGEWAR_InfoLoad"
#define STORED_SIEGE_INFOINSERT			"dbo.MP_SIEGEWAR_InfoInsert"
#define STORED_SIEGE_INFOUPDATE			"dbo.MP_SIEGEWAR_InfoUpdate"
#define STORED_SIEGE_GUILDLOAD			"dbo.MP_SIEGEWAR_GuildInfoLoad"
#define STORED_SIEGE_GUILDINSERT		"dbo.MP_SIEGEWAR_GuildInfoInsert"
#define STORED_SIEGE_GUILDUPDATE		"dbo.MP_SIEGEWAR_GuildInfoUpdate"
#define STORED_SIEGE_GUILDDELETE		"dbo.MP_SIEGEWAR_GuildInfoDelete"
#define STORED_CHARACTER_SKININFO		"dbo.MP_CHARACTER_UpdateSkinInfo"
#define STORED_CHARACTER_UPDATE_RESETSTATUSPOINT	"dbo.MP_CHARACTER_UpdateResetStatusPoint"
#define LOADGAMECONFIG "dbo.MP_LoadGameConfig"  //游戏设置加载  by:十里坡剑神	QQ:112582793
#define UPDATEGAMECONFIG "dbo.MP_UpdateGameConfig"  //游戏设置更新  by:十里坡剑神	QQ:112582793
//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#define LOADRECHARGEINFO "dbo.MP_Recharge_Record_Info_Load"		
#define UPDATERECHARGEINFO "dbo.MP_Recharge_Record_Info_update"
enum AuctionPage		
{
	eAT_DBIdx, eAT_ItemDBIdx, eAT_ItemIdx, eAT_SellNum,  eAT_SellerID, eAT_EndDate, eAT_EndTime,
	eAT_StartPrice, eAT_ImmediatePrice, eAT_CurrentPrice, eAT_BidderNum, eAT_CurrentBidder, eAT_RegDate
};
enum AuctionList
{
	ePRO_NAME, ePRO_Amount, eBID_High_Price, eBID_Immediate_Price, eBID_DueDate, eHigh_Price_Name, eJOIN_Amount, 
	eBID_Left_Time, eAuctioneer,ePRO_Index ,eBID_Starttime,eREG_Index, 
};

enum AuctionSuccess
{
	sePRO_NAME, sePRO_Amount, seAuctioneer,
	seREG_Index

};

enum AuctionConfirm
{
	bAuctionConfirm
};
enum CHSelectInfo	
{
	eCS_ObjectID = 0, eCS_UserID, eCS_PartyID,eCS_Gender, eCS_Gengoal, eCS_Life, eCS_Shield, eCS_Energe, eCS_Dex, eCS_Sta, eCS_Simmak, eCS_Fame, 
	eCS_Karma, eCS_Name, eCS_Repute, eCS_Stage, eCS_Grade, eCS_Expoint, eCS_GradeUpPoint, eCS_Money, 
	eCS_QuickPace, eCS_Vitality, eCS_Map, eCS_FaceType, eCS_HeadType, eCS_Hat, eCS_Weapon, eCS_Dress, eCS_Shoes, 
	eCS_Ring1, eCS_Ring2, eCS_Cape, eCS_Necklace, eCS_Armlet, eCS_Belt, eCS_MunpaIDX, eCS_GuildPositon, eCS_GuildName, eCS_BadFame,
	eCS_MapChangePoint, eCS_LoginPoint, eCS_AbilityExp, eCS_Height, eCS_Width, eCS_Playtime, eCS_LastPKEndTime, eCS_MaxLevel, eCS_MarkName, eCS_MunpaCanEntryDate,
	eCS_NickName, eCS_ExpFlag,eCS_SkinInfo,eCS_MallMoney,eCS_GoldMoney,eCS_ReSet,eCS_MarryName,eCS_PdTime,eCS_FNameFlag,eCS_KillMonsterTimes,
	eCS_DieTimes,eCS_KillPlayerTimes,eCS_UnionWarKill,eCS_UnionWarDie//[新增标志统计被杀次数和杀怪数量以及签到时间][By:十里坡剑神][QQ:112582793][2018/1/3]
};

enum CHMugong		
{
	eCM_ObjectID = 0, eCM_DBIDX, eCM_IDX, eCM_ExpPoint, eCM_Sung, eCM_Position, eCM_QPosition, eCM_Wear, eCM_Option
};

enum CHItem		
{
	eCI_ObjectID = 0, eCI_DBIDX, eCI_IDX, eCI_Position, eCI_QPosition, eCI_Durability, eCI_Param, eCI_RareIdx, eCI_Static,eCI_Grow,eCI_Abrasion,eCI_MaxAbrasion,eCI_Maker,eCI_Maker1 //[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
};

enum CHItemRare	
{
	eCIR_ObjectID, eCIR_ItemIdx, eCIR_Position, eCIR_RareID, eCIR_ItemDBID, 
	eCIR_GenGol, eCIR_MinChub, eCIR_CheRyuk, eCIR_SimMek, eCIR_Life, eCIR_NaeRyuk, eCIR_Shield, 
	eCIR_FireAttrRegist, eCIR_WaterAttrRegist, eCIR_TreeAttrRegist,  eCIR_GoldAttrRegist, eCIR_EarthAttrRegist,
	eCIR_PhyAttack, 
	eCIR_FireAttrAttack, eCIR_WaterAttrAttack, eCIR_TreeAttrAttack,  eCIR_GoldAttrAttack, eCIR_EarthAttrAttack,
	eCIR_PhyDefense 
};

enum CHItemOption	
{
	eCIO_ObjectID, eCIO_ItemIdx, eCIO_OptionID, eCIO_ItemDBID, eCIO_GenGol, eCIO_MinChub, eCIO_CheRyuk, eCIO_SimMek, eCIO_Life, eCIO_NaeRyuk, eCIO_Shield, 
	eCIO_FireAttrRegist, eCIO_WaterAttrRegist, eCIO_TreeAttrRegist,  eCIO_GoldAttrRegist, eCIO_EarthAttrRegist,
	eCIO_PhyAttack, eCIO_Critical, 
	eCIO_FireAttrAttack, eCIO_WaterAttrAttack, eCIO_TreeAttrAttack,  eCIO_GoldAttrAttack, eCIO_EarthAttrAttack,
	eCIO_PhyDefense 
};

enum CHItemRareOptionInfo
{
	eCIRO_ObjectID, eCIRO_RareOptionID, eCIRO_ItemDBID, eCIRO_GenGol, eCIRO_MinChub, eCIRO_CheRyuk, eCIRO_SimMek, eCIRO_Life, eCIRO_NaeRyuk, eCIRO_Shield, 
	eCIRO_FireAttrRegist, eCIRO_WaterAttrRegist, eCIRO_TreeAttrRegist,  eCIRO_GoldAttrRegist, eCIRO_EarthAttrRegist,
	eCIRO_PhyAttack,
	eCIRO_FireAttrAttack, eCIRO_WaterAttrAttack, eCIRO_TreeAttrAttack,  eCIRO_GoldAttrAttack, eCIRO_EarthAttrAttack,
	eCIRO_PhyDefense 
};
enum CHItemOptionInfo	
{
	eCIOI_ObjectID, eCIOI_OptionID, eCIOI_ItemDBID, eCIOI_GenGol, eCIOI_MinChub, eCIOI_CheRyuk, eCIOI_SimMek, eCIOI_Life, eCIOI_NaeRyuk, eCIOI_Shield, 
	eCIOI_FireAttrRegist, eCIOI_WaterAttrRegist, eCIOI_TreeAttrRegist,  eCIOI_GoldAttrRegist, eCIOI_EarthAttrRegist,
	eCIOI_PhyAttack, eCIOI_Critical, 
	eCIOI_FireAttrAttack, eCIOI_WaterAttrAttack, eCIOI_TreeAttrAttack,  eCIOI_GoldAttrAttack, eCIOI_EarthAttrAttack,
	eCIOI_PhyDefense 
};

enum CPyogukItemRareOptionInfo	
{

	ePROI_RareOptionID, ePROI_ItemDBID, ePROI_GenGol, ePROI_MinChub, ePROI_CheRyuk, ePROI_SimMek, ePROI_Life, ePROI_NaeRyuk, ePROI_Shield, 
	ePROI_FireAttrRegist, ePROI_WaterAttrRegist, ePROI_TreeAttrRegist,  ePROI_GoldAttrRegist, ePROI_EarthAttrRegist,
	ePROI_PhyAttack, 
	ePROI_FireAttrAttack, ePROI_WaterAttrAttack, ePROI_TreeAttrAttack,  ePROI_GoldAttrAttack, ePROI_EarthAttrAttack,
	ePROI_PhyDefense 
};

enum CPyogukItemOptionInfo	
{
	ePOI_OptionID, ePOI_ItemDBID, ePOI_GenGol, ePOI_MinChub, ePOI_CheRyuk, ePOI_SimMek, ePOI_Life, ePOI_NaeRyuk, ePOI_Shield, 
	ePOI_FireAttrRegist, ePOI_WaterAttrRegist, ePOI_TreeAttrRegist,  ePOI_GoldAttrRegist, ePOI_EarthAttrRegist,
	ePOI_PhyAttack, ePOI_Critical, 
	ePOI_FireAttrAttack, ePOI_WaterAttrAttack, ePOI_TreeAttrAttack,  ePOI_GoldAttrAttack, ePOI_EarthAttrAttack,
	ePOI_PhyDefense 
};
enum CMunpaItemOptionInfo	
{
	eMOI_OptionID, eMOI_ItemDBID, eMOI_GenGol, eMOI_MinChub, eMOI_CheRyuk, eMOI_SimMek, eMOI_Life, eMOI_NaeRyuk, eMOI_Shield, 
	eMOI_FireAttrRegist, eMOI_WaterAttrRegist, eMOI_TreeAttrRegist,  eMOI_GoldAttrRegist, eMOI_EarthAttrRegist,
	eMOI_PhyAttack, eMOI_Critical, 
	eMOI_FireAttrAttack, eMOI_WaterAttrAttack, eMOI_TreeAttrAttack,  eMOI_GoldAttrAttack, eMOI_EarthAttrAttack,
	eMOI_PhyDefense 
};

enum CGuildItemRareOptionInfo
{
	eGORI_RareOptionID, eGORI_ItemDBID, eGORI_GenGol, eGORI_MinChub, eGORI_CheRyuk, eGORI_SimMek, eGORI_Life, eGORI_NaeRyuk, eGORI_Shield, 
	eGORI_FireAttrRegist, eGORI_WaterAttrRegist, eGORI_TreeAttrRegist,  eGORI_GoldAttrRegist, eGORI_EarthAttrRegist,
	eGORI_PhyAttack, 
	eGORI_FireAttrAttack, eGORI_WaterAttrAttack, eGORI_TreeAttrAttack,  eGORI_GoldAttrAttack, eGORI_EarthAttrAttack,
	eGORI_PhyDefense
};

enum eGuildItemOptionInfo
{
	eGIOI_GuildIdx,
	eGIOI_OptionID, eGIOI_ItemDBID, eGIOI_GenGol, eGIOI_MinChub, eGIOI_CheRyuk, eGIOI_SimMek, eGIOI_Life, eGIOI_NaeRyuk, eGIOI_Shield, 
	eGIOI_FireAttrRegist, eGIOI_WaterAttrRegist, eGIOI_TreeAttrRegist,  eGIOI_GoldAttrRegist, eGIOI_EarthAttrRegist,

	eGIOI_PhyAttack, eGIOI_Critical, 
	eGIOI_FireAttrAttack, eGIOI_WaterAttrAttack, eGIOI_TreeAttrAttack,  eGIOI_GoldAttrAttack, eGIOI_EarthAttrAttack,
	eGIOI_PhyDefense
};

enum eGuildItemRareOptionInfo
{
	eGIROI_GuildIdx,
	eGIROI_RareOptionID, eGIROI_ItemDBID, eGIROI_GenGol, eGIROI_MinChub, eGIROI_CheRyuk, eGIROI_SimMek, eGIROI_Life, eGIROI_NaeRyuk, eGIROI_Shield, 
	eGIROI_FireAttrRegist, eGIROI_WaterAttrRegist, eGIROI_TreeAttrRegist,  eGIROI_GoldAttrRegist, eGIROI_EarthAttrRegist,
	eGIROI_PhyAttack, 
	eGIROI_FireAttrAttack, eGIROI_WaterAttrAttack, eGIROI_TreeAttrAttack,  eGIROI_GoldAttrAttack, eGIROI_EarthAttrAttack,
	eGIROI_PhyDefense
};

enum eGuildTraineeInfoIndex
{
    eGTII_DBIdx, eGTII_GuildIdx, eGTII_UserIdx, eGTII_UserName, eGTII_JoinTime
};

enum eGuildTraineeGuildInfoIndex
{
	eGTGII_DBIdx, eGTGII_GuildIdx, eGTGII_GuildRank
};

enum eGuildAddStudentIndex
{
	eGASI_CharIdx, eGASI_Level, eGASI_Name, eGASI_GuildIdx
};

enum eGuildTraineeGuildIntroInfoIndex
{
    eGTGIII_DBIdx, eGTGIII_Intro,
};
//////////////////////////////////////////////////////////////////////////
enum CPetInvenItemOptionInfo
{
	ePIOI_OptionID, ePIOI_ItemDBID, ePIOI_GenGol, ePIOI_MinChub, ePIOI_CheRyuk, ePIOI_SimMek, ePIOI_Life, ePIOI_NaeRyuk, ePIOI_Shield, 
	ePIOI_FireAttrRegist, ePIOI_WaterAttrRegist, ePIOI_TreeAttrRegist,  ePIOI_GoldAttrRegist, ePIOI_EarthAttrRegist,
	ePIOI_PhyAttack, ePIOI_Critical, 
	ePIOI_FireAttrAttack, ePIOI_WaterAttrAttack, ePIOI_TreeAttrAttack,  ePIOI_GoldAttrAttack, ePIOI_EarthAttrAttack,
	ePIOI_PhyDefense
};

enum CPetInvenItemRareOptionInfo
{
	ePIROI_RareOptionID, ePIROI_ItemDBID, ePIROI_GenGol, ePIROI_MinChub, ePIROI_CheRyuk, ePIROI_SimMek, ePIROI_Life, ePIROI_NaeRyuk, ePIROI_Shield, 
	ePIROI_FireAttrRegist, ePIROI_WaterAttrRegist, ePIROI_TreeAttrRegist,  ePIROI_GoldAttrRegist, ePIROI_EarthAttrRegist,
	ePIROI_PhyAttack,
	ePIROI_FireAttrAttack, ePIROI_WaterAttrAttack, ePIROI_TreeAttrAttack,  ePIROI_GoldAttrAttack, ePIROI_EarthAttrAttack,
	ePIROI_PhyDefense	
};

enum CPetTotalInfo
{
	ePTI_PetDBIdx, ePTI_UserID, ePTI_PetSummonItemDBIdx, ePTI_PetKind, ePTI_PetGrade, ePTI_PetStamina, ePTI_PetFriendship, 
	ePTI_Alive, ePTI_Summonning, ePTI_Rest,
};

enum CPetInsertInfo
{
	ePII_PetSummonItemIdx, ePII_PetDBIdx, ePII_MasterObjectID, ePII_UserIdx, ePII_PetSummonItemDBIdx, ePII_PetKind, ePII_PetGrade, ePII_PetStamina, ePII_PetFriendship, 
	ePII_Alive, ePII_Summonning, ePII_Rest,
};

enum GuildPointInfoInit
{
	eGPII_GuildDBIdx, eGPII_GuildIdx, eGPII_GuildPoint, eGPII_GuildHuntedMonsterTotalCount, eGPII_EndTimePerGuildPlustimeKind,
};

enum eGuildHuntedMonsterCount
{
	eGHMC_TotalCount, eGHMC_DB_Date,
};

enum GuildConvertHuntedMonsterCountWithInit
{
	eGCHMC_TotalCount, eGCHMC_DB_Date,
};

enum Guild_AddGuildPoint
{
	eGAGP_AddPoint, eGAGP_TotalPoint, eGAGP_AddKind, eGAGP_AdditionalData, 
};

enum Guild_UseGuildPoint
{
	eGUGP_UsePoint, eGUGP_TotalPoint, eGUGP_UseKind, eGUGP_KindIdx, eGUGP_Endtime,
};

enum CHSkill	
{
	eCK_ObjectID = 0, eCK_DBIDX, eCK_IDX, eCK_Position, eCK_QPosition, eCK_Durability
};

enum CHItemGroundDrop	
{
	eIG_ObjectID, eIG_DBIDX, eIG_IDX, eIG_POSX, eIG_POSZ
};

enum MMapBaseEconomy		
{
	eME_MEPNUM, eME_ORIGINNUM, eME_ORIGINPRICE, eME_ORIGINAMOUNT, eME_REQUIRENUM, eME_REQUIREPRICE, eME_REQUIREAMOUNT,
	eME_BUYRATES, eME_SELLRATES 
};

enum PartyInfoMap
{
	ePA_IDX, 
};
enum Partyinfobyuserlogin
{
	ePA_LIMemberId, ePA_LIMemberName, ePA_LIMemberCurrentMap, ePA_LIMemberLevel, ePA_LIOption,
	ePA_MinLevel, ePA_MaxLevel, ePA_Public, ePA_LimitCount, ePA_Theme,
};
enum Partychangemaster
{
	ePA_CMErr, ePA_CMTargetID,
};

enum PartyMember 
{
	 ePA_ERR,ePA_PARTYID, ePA_MASTER, ePA_OPTION,
	 ePA_MINLEVEL, ePA_MAXLEVEL, ePA_PUBLIC, ePA_LIMITCOUNT, ePA_THEME,
};
enum Partydelmember
{
	ePA_DMErr, ePA_DMTargetID,
};
enum Munpainfo
{
	eMu_MunpaID, eMunpaMasterID, eMu_MunpaName, eFamous, eMemberNum, eMunpaRank, eMunpaMoney
};

enum Munpaloadsyn
{
	eMu_MLStotalnum, eMu_MLSmunpaid, eMu_MLSmunpaname, eMu_MLSmastername, eMu_MLSmunpafamous, eMu_MLSmembernum, eMu_MLSKind
};
enum Munpacreate
{
	eMu_MCErr, eMu_MCMunpaid, eMu_MCMasterid, eMu_MCMunpaname, eMu_MCMunpaRank, eMu_MCMasterFame
};
enum Munpabreakup
{
	eMu_MBerr, eMu_MBmasterid, eMu_MBmasterfame,
};
enum Munpabreakupmember
{
	eMu_MBmemberid, eMu_MBmemberfame,
};
enum Munpahomeinfo
{
	eMu_MHIMunpaid, eMu_MHIIntroMsg, eMu_MHIMunpaKind, 
	eMu_MHIMunpaName, eMu_MHIMasterName, eMu_MHIFamous, 
	eMu_MHICreateDate, eMu_MHIMemberNum, eMu_MHIFieldWarWin, 
	eMu_MHIFieldWarLoss, eMu_MHIBuildingWarWin, eMu_MHIBuildingWarLoss, 
	eMu_MHICastleWarWin, eMu_MHICastleWarLoss, eMu_MHIBoardIDX, 
	eMu_MHIRSRank, eMu_MHIRCRank, eMu_MHIWCRank, 
	eMu_MHIDCRank, eMu_MHIBoardName, 
};
enum Munpamembersinfo
{
	eMu_MMTotalNum, eMu_MMunpaid, eMu_MMemberid, 
	eMu_MMemberName, eMu_MMemberRank, eMu_MMemberLevel, 
	eMu_MMemberFamous, eMu_MMember_LastVisitDay, eMu_MMember_EntryDay, 
	eMu_MMember_bLogin,
};
enum Munpachangememberank
{
	eMu_MCRchangeerr, eMu_MCRtargetid, eMu_MCRmunpaid, eMu_MCRposition, eMu_MCRtargetname, eMu_MCRfame, 
};
enum Munpabanmember
{
	eMu_MBMerr, eMu_MBMtargetid, eMu_MBMmunpaid, eMu_MBMblast, eMu_MBMtargetname, eMu_MBMfame, 
};
enum Munpadenymember
{
	eMu_MDMerr,  eMu_MDMtargetid, eMu_MDMmunpaid, eMu_MDMbLast, eMu_MDMtargetname,  
};
enum Munpaacceptmember
{
	eMu_MACerr, eMu_MACmunpaid, eMu_MACtargetid, eMu_MACblast, eMu_MACtargetname, eMu_MACMunpaname
};
enum MunpaBattleInfo
{
	eMu_MMunpaName, eMu_MMasterName, eMu_MMunpaFamous, eMu_MMemberNum, eMu_MCreateDate,
	eMu_MFBattleTotal, eMu_MBBattleTotal, eMu_MCBattleTotal, eMu_MTBattleTotal,
	eMu_MFBattleWin, eMu_MBBattleWin, eMu_MCBattleWin, eMu_MTBattleWin, 
	eMu_MFBattleLoss, eMu_MBBattleLoss, eMu_MCBattleLoss, eMu_MTBattleLoss
};
enum Munpajoinmemberinfo
{
	eMu_MJMTotalNum, eMu_MJMMunpaid, eMu_MJMCharacterID, 
	eMu_MJMCharacterName, eMu_MJMCharacterLevel, eMu_MJMCharacterFame, 
	eMu_MJMRequestJoinDay
};
enum Munpaboardinfo
{
	eMu_MCurBoardNum, eMu_MBoardsIdx, eMu_MBoardsName, eMu_MTotalNum,
};

enum Munpaitem
{
	eMu_IMunpaID, eMu_IDBIDX, eMu_IIDX, eMu_IPosition, eMu_IDurability, eMu_IRareIdx,eMu_IStatic,eMu_IGrow,eMu_Abrasion,eMu_MaxAbrasion,eMu_Maker,eMu_Maker1//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
};

enum Munpacreateboardinfo
{
	eMu_MBoardidx, eMu_MRSubjectRank, eMu_MRContentRank, eMu_MWContentRank, eMu_MDContentRank, eMu_MBoardName
};
enum Munpaboardlistinfo
{
	eMu_MBLITotalPage, eMu_MBLIContentIDx, eMu_MBLIContentNum, eMu_MBLISubject, eMu_MBLIWriter, eMu_MBLICount, eMu_MBLIRegDate, eMu_MBLIRegTime, eMu_MBLIDepth
};
enum Munpaboardcontentsinfo
{
	eMu_MBCErr, eMu_MBCIContent, eMu_MBCIContentIDx, eMu_MBCIContentNum, eMu_MBCISubject, eMu_MBCIWriter, eMu_MBCICount, eMu_MBCIRegDate, eMu_MBCDCRank, eMu_MBCWCRank
};
enum Munpamoney
{
	eMu_MWMunpaID, eMu_MWMunpaMoney, eMu_MWPutInOut,
};

enum Pyogukbuy
{
	ePyo_errbuy, ePyo_Kind
};

enum PyogukInfo
{
	ePyo_PyogukNum, ePyo_Money,
};

enum PyogukItemInfo
{
	ePI_DBIdx, ePI_IconIdx, ePI_Positon, ePI_QPosition, ePI_Durability, ePI_RareIdx,ePI_Static,ePI_Grow,ePI_Abrasion,ePI_MaxAbrasion,ePI_Maker,ePI_Maker1//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
};

enum Pyogukitem
{
	ePyo_Userid, ePyo_MaxPosition, ePyo_Durability, ePyo_Idx, ePyo_DBIdx, ePyo_Position, 
};

enum ePetInvenItemInfo
{
	ePIII_ObjectID = 0, ePIII_DBIDX, ePIII_IDX, ePIII_Position, ePIII_QPosition, ePIII_Durability, ePIII_MunpaIdx, ePIII_PyogukIdx, ePIII_ShopIdx, ePIII_Param, ePIII_RareIdx,ePIII_STATIC,ePIII_Grow,ePIII_Abrasion,ePIII_MaxAbrasion,ePII_Maker,ePII_Maker1//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
};

enum Friendlogout
{
	eFr_LToPlayerID, eFr_LLogoutName,
};
enum Friendnotelist
{
	eFr_NLFromName, eFr_NLNote, eFr_NLSendDate, eFr_NLSendTime, eFr_NLNoteID
};
enum Friendlist
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin,
};
enum Famemunpaupdate
{
	eFa_MunpaIDX, eFa_Fameval,
};
enum Wantedload
{
	eWa_LWantedIDX, eWa_LPlayerID, eWa_LPlayerName, eWa_LTotlaPrize, eWa_LRegistChridx, eWa_LTotalVolunteerNum,

};
enum Wantedregist
{
	eWa_RWanted_idx, eWa_RWantedChr_idx, eWa_RWanted_name, eWa_RTotalPrize, 
};
enum Wantedcomplete
{
	eWa_CCharacteridx, eWa_CCompleterName, eWa_CCompleteType,
};
enum Wantedorderlist
{
	eWa_OLTotal, eWa_OLWantedIDX, eWa_OLWantedChrName, eWa_OLPrize, eWa_OLVolunteerNum,
};
enum Wantedlogininfo
{
	eWa_LIMyWantedIDX, eWa_LIWantedIDX, eWa_LIWantedChrID, eWa_LIWantedName, eWa_LIRegistDate, 
};
enum QuestInfo
{
	eQi_QuestIdx, eQi_QuestState, eQi_EndParam, eQi_RegistTime, eQi_CheckType, eQi_CheckTime,
};
enum QuestSubDataInfo
{
	eQsi_QuestIdx, eQsi_SubQuestIdx, eQsi_Data, eQsi_RegistTime,
};
enum QuestItem
{
	eQit_ItemIdx, eQit_ItemCount, eQit_QuestIdx,
};
enum Journal
{
	eJnl_Index, eJnl_Kind, eJnl_Param, eJnl_Param_2, eJnl_Param_3, eJnl_bSaved, eJnl_ParamName, eJnl_RegDate,
};
enum Guildload
{
	eGu_LGIdx, eGu_LGMasterIdx, eGu_LGMasterName, eGu_LGName, eGu_LGLevel, eGu_LGLocation, eGu_LGUnionIdx, eGu_LGMoney, eGu_LGMarkName,
};
enum Guildloadmember
{
	eGu_LMIdx, eGu_LMName, eGu_LMLevel, eGu_LMGuildIdx, eGu_LMRank, 
};
enum Guilcreate
{
	eGu_CIsSuccess, eGu_CGuildIdx, eGu_CName, eGu_CMapNum, eGu_CMasterName, eGu_CMasterlvl,
};
enum ShopItem
{
	eMItm_ItemDBIdx, eMItm_ItemIdx, eMItm_Position, eMItm_Durability, eMItm_Param,eMItm_Static,eMItm_Grow,eMItm_Abrasion,eMItm_MaxAbrasion,eMItm_Maker,eMItm_Maker1//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
};
enum ShopItemUseInfo
{
	eUMtm_ItemIdx, eUMtm_DBIdx, eUMtm_Param, eUMtm_BTime, eUMtm_RTime, eUMtm_Position,
};
enum MovePointInfo
{
	eMPt_DBIdx, eMPt_Name, eMPt_MapNum, eMPt_MapPoint,
};
enum AvatarItemOption
{
	eAO_OptionIdx, 
};
enum eGTInfo
{
	eGT_GuildIdx, eGT_Position, eGT_Ranking, eGT_Process, eGT_WaitTime,
};
enum eSW_Info
{
	eSWI_SiegeWarIdx, eSWI_RegistTime, eSWI_SiegeWarTime, eSWI_SiegeWarLevel,
};
enum eSW_GuildInfo
{
	eSWGI_GuildDBIdx, eSWGI_GuildIdx, eSWGI_GuildType,
};

enum eGuildNotice
{
	eGu_GNDBIdx, eGu_GNGuildIdx, eGu_GNGuildNotice,
};

enum eMapDropItem	
{
	eMdi_MapNum, eMdi_Channel, eMdi_ItemIDX, eMdi_DropCount, eMdi_MaxDropCount
};
enum eMarryDelData
{
	eMarryDel_vtFromid , eMarryDel_vtTargetid , eMarryDel_vtvailderr, 
};

enum eSingedInfo
{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	eSING_UserID,eSING_Monday,eSING_Tuesday,eSING_Wednesday,eSING_Thursday,eSING_Friday,eSING_Saturday,eSING_Sunday,eSING_Compelet,
};
//[游戏配置加载][By:十里坡剑神][QQ:112582793][2017/11/30]
enum eGameConfigLoading
{
	eMaxReSetTime,eMinReSetLevel,eReSetMoney,eReSetPower,eFirstStageMoney,eSecondStageMoney,eReStagePower,
	eMarryLvSet,eMarryMoney,eMarryGold,ePaoDianLevel,ePaoDianReSet,
	ePaoDianTime,ePaoDianMap,ePaoDianMap_LZ,eReSetTime,eReSetInterval,
};
enum eRechargeInfoLoading
{
	eCharacter_idx,ePayMoney,eUsedMoney,ePayMoneyCount,
};
void TestQuery();
void CharacterNumSendAndCharacterInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterNumSendAndCharacterInfo2(DWORD CharacterIDX, DWORD dwTeam);
void CharacterMugongInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemSlotInfo(DWORD CharacterIDX);
void CharacterItemInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemRareOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void CharacterItemOptionInfo(DWORD CharacterIDX, DWORD Protocol);
void PyogukItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void PyogukItemOptionInfo(DWORD CharacterIDX, DWORD UserID, DWORD StartDBIdx);
void CharacterAbilityInfo(DWORD CharacterIDX, DWORD Protocol);
//SW051129 Pet
void CharacterPetInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanInfo(DWORD CharacterIDX, DWORD UserID);
void CharacterTitanEquipItemEnduranceInfo(DWORD CharacterIDX);
void PetInvenItemOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemRareOptionInfo(DWORD CharacterIDX, DWORD UserID, WORD StartPos, WORD EndPos);
void PetInvenItemInfo(DWORD CharacterIDX, WORD StartPos, WORD EndPos);
void PetWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );

void CharacterItemOptionDelete(DWORD OptionIdx, DWORD dwItemDBIdx);
void CharacterItemRareOptionDelete(DWORD RareIdx, DWORD dwItemDBIdx);
void AbilityLevelupLog(CPlayer* pPlayer,WORD AbilityIdx,LEVELTYPE AbilityLevel,EXPTYPE FromExp,EXPTYPE ToExp);

void MapBaseEconomy(BYTE MapNum);
void AuctionRegist(BYTE ItemIdx, DWORD SellerID, DWORD EndDate, DWORD EndTime, DWORD StartPrice, DWORD ImmediatePrice, char Memo[256]);
void AuctionPageQuery(DWORD CharacterIDX, BYTE PageNum);

void SSItemInsert(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD FromChrIdx, WORD bSeal=0,WORD bStatic=0,DWORD bGrow=0);
void ItemUpdateToDB(DWORD CharacterIdx, DWORD dwDBIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, WORD qPosition, DWORD RareIdx=0,WORD bStatic = 0);
void ItemCombineUpdateToDB(DWORD dwfromDBIdx, DURTYPE fromDur, DWORD dwToDBIdx, DURTYPE toDur);
void ItemMoveUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePyogukUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMoveGuildUpdateToDB(DWORD CharacterIDX, DWORD MunpaIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemMovePetInvenUpdateToDB(DWORD CharacterIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ItemInsertToDB(DWORD CharacterIdx, WORD wItemIdx, DURTYPE Durability, POSTYPE bPosition, DWORD dwKey, WORD bSeal=0,WORD bStatic=0,DWORD dwGrow=0);
void ItemDeleteToDB(DWORD dwDBIdx);
void ItemOptionInsertToDB(DWORD CharacterIdx, WORD wItemIdx, POSTYPE Pos, ITEM_OPTION_INFO * pOptionInfo);
void ItemRareInsertToDB(DWORD CharacterIdx, WORD wItemIdx,POSTYPE bPosition, DWORD dwKey, ITEM_RARE_OPTION_INFO* pRareOptionInfo );


void ShopItemRareInsertToDB(DWORD CharacterIdx, DWORD ItemIdx, DWORD ItemPos, DWORD ItemDBIdx, ITEM_RARE_OPTION_INFO* pRareOptionInfo,BOOL bIsInherit=FALSE);
void RShopItemRareInsertToDB(LPQUERY pData,  LPDBMESSAGE pMessage);

void PetInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, PET_TOTALINFO* pPetTotalInfo);
void PetUpdateToDB(DWORD UserIdx, const PET_TOTALINFO* pPetTotalInfo);
void PetDeleteToDB(DWORD dwPetDBIdx);

void TitanInsertToDB(DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, TITAN_TOTALINFO* pTitanTotalInfo);
void RTitanInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanUpdateToDB(DWORD MasterIdx, const TITAN_TOTALINFO* pTitanTotalInfo);
void TitanDeleteToDB(DWORD dwCallItemDBIdx);

void TitanEquipItemInfoInsertToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipItemInfoInsertToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipItemInfoUpdateToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void TitanEquipItemInfoDeleteToDB(DWORD dwTitanEquipDBIdx);

void TitanEquipInsertExceptionToDB(TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionToDB(LPQUERY pData, LPDBMESSAGE pMessage);
void TitanEquipInsertExceptionPyogukToDB(DWORD CharacterIdx, TITAN_ENDURANCE_ITEMINFO* pInfo);
void RTitanEquipInsertExceptionPyogukToDB(LPQUERY pData, LPDBMESSAGE pMessage);

void MugongUpdateToDB(MUGONGBASE* msg, char* type);
void MugongMoveUpdateToDB(DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos, char* type);
void MugongInsertToDB(DWORD CharacterIDX, WORD MugongIDX, POSTYPE MugongPos, BYTE bWeared, BYTE bSung, WORD Option = 0,WORD Qpotion=0);
void MugongDeleteToDB(DWORD dwDBIdx);

void CheckAuction(MSGBASE* msg);
void AuctionSearch(SEARCHLIST* msg);
void AuctionSort(SORTLIST* sort);
void AuctionRegister(REGISTERAUCTION* msg);
void AuctionJoin(JOINAUCTION* msg);
void AuctionCancel(AUCTIONCANCEL *msg);
//////////////////////////////////////////////////////////////////////////
void PartyLoad(DWORD PartyIDX);
void PartyInfoByUserLogin(DWORD PartyIDX, DWORD CharacterIDX);
void PartyBreakup(DWORD PartyIDX);
void PartyCreate(DWORD MasterIDX, PARTY_ADDOPTION* pAddOption);
///////////////////////////////////////////////////////////////
void PartyMemberUpdate(CParty* pParty, DWORD PartyIDx);
void PartyAddMember(DWORD PartyIDX, DWORD TargetID);
void PartyDelMember(DWORD PartyIDX, DWORD TargetID);
void PartyChangeMaster(DWORD PartyID, DWORD PlayerID, DWORD TargetPlayerID);

//////////////////////////////////////////////////////////////////////////
void ChangeMapGetCharacterInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
void ChangeMapGetCharacterMugongInfo(DWORD CharacterIDX, DWORD dwConnectionIndex);
//////////////////////////////////////////////////////////////////////////
void GuildLoadGuild(DWORD StartGuildIdx);
void GuildLoadMember(DWORD StartCharacterIdx);
void GuildCreate(DWORD CharacterIdx, char* CharacterName, LEVELTYPE CharacterLvl, char* GuildName, char* Intro);
void GuildBreakUp(DWORD GuildIdx, DWORD MasterIdx);
void GuildDeleteMember(DWORD GuildIdx, DWORD MemberIDX);
void GuildSecedeMember(DWORD GuildIdx, DWORD PlayerIdx);
void GuildAddMember(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void GuildMarkRegist(DWORD GuildIdx, char* ImgData, DWORD CharacterIdx);
void GuildLevelUp(DWORD GuildIdx, BYTE Level);
void GuildChangeMemberRank(DWORD GuildIdx, DWORD MemberIdx, BYTE Rank);
void GuildItemLoad(DWORD MapNum, DWORD StartItemDBIDx);
void GuildItemOption(DWORD MapNum, DWORD StartDBIdx);
void GuildItemRareOption(DWORD MapNum, DWORD StartDBIdx);
void GuildMoneyUpdate(DWORD GuildIdx, MONEYTYPE Money);
void GuildGiveMemberNickName(DWORD TargetId, char * NickName);

void GuildItemLoadInNeed(DWORD MapNum, DWORD GuildIdx);
void GuildItemOptionInNeed(DWORD MapNum, DWORD GuildIdx,DWORD StartDBIdx);
void GuildItemRareOptionInNeed(DWORD MapNum, DWORD GuildIdx, DWORD StartDBIdx);
void RGuildItemLoadInNeed( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOptionInNeed(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildLoadNotice(DWORD StartDBIdx);
void GuildUpdateNotice(DWORD GuildIdx, char* Notice);
void GuildLoadGuildPointInfo(DWORD startGuildDBIdx);	
void GuildLoadGuildPlustimeInfo(DWORD startDBIdx);
void RGuildLoadGuildPointInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadGuildPlustimeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildAddHuntedMonsterCount(DWORD GuildIdx, DWORD MonsterCount);	
void GuildGetHuntedMonsterTotalCountWithInit(DWORD GuildIdx);	

void RGuildAddHuntedMonsterCount(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildGetHuntedMonsterTotalCountWithInit(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildAddGuildPoint(DWORD GuildIdx, DWORD rAddPoint, DWORD rAddKind, DWORD rAdditionalData);	
void RGuildAddGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildUseGuildPoint(DWORD GuildIdx, DWORD rUsePoint, DWORD rUseKind, DWORD rKindIdx, DWORD rEndtime);
void RGuildUseGuildPoint(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildPlustimeEnd(DWORD GuildIdx, DWORD rPlusTimeIdx);
void RGuildPlustimeEnd(LPQUERY pData, LPDBMESSAGE pMessage);

void LogGuildPoint(DWORD GuildIdx, DWORD TotalGuildPoint, BOOL bAdd, int eKind, DWORD GuildPoint, DWORD AddData, DWORD CharacterIdx);
void PyogukBuyPyoguk(DWORD PlayerIDX);
void CharacterPyogukInfo(DWORD UserIdx, DWORD CharacterIdx);
void CharacterPyogukItemInfo(DWORD CharacterIdx, DWORD StartDBIdx);
void PyogukMoneyUpdateToDB(DWORD UserIdx, MONEYTYPE PyogukMoney);
void FriendNotifyLogouttoClient(DWORD PlayerID);
//////////////////////////////////////////////////////////////////////////
void FameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void BadFameCharacterUpdate(DWORD PlayerIDX, FAMETYPE val);
void PKCharacterUpdate(DWORD PlayerIDX, DWORD val);
//////////////////////////////////////////////////////////////////////////
void SavePointUpdate(DWORD CharacterIDX, WORD LoginPoint_Idx, WORD MapNum);
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
void SaveMapChangePointReturn(DWORD CharacterIDX, WORD MapChangePoint_Idx, DWORD dwConnectionIdx, LEVELTYPE lvl, EXPTYPE ExpPoint, MONEYTYPE PyogukMoney);
//////////////////////////////////////////////////////////////////////////
void WantedLoad(WANTEDTYPE StartWantedIDX);
void WantedInfoByUserLogIn(DWORD CharacterIDX);
void WantedBuyRight(DWORD CharacterIDX, WANTEDTYPE WantedIDX, MONEYTYPE AddPrize);
void WantedRegist(DWORD CharacterIDX, DWORD TargetIDX, MONEYTYPE Prize);
void WantedGiveUpRight(DWORD CharacterIDX, DWORD WantedIDX);
void WantedComplete(DWORD CharacterIDX, char* TargetName, WANTEDTYPE WantedIDX);
void WantedDestroyed(DWORD WantedIDX); 
void WantedOrderedList(DWORD CharacterIDX, DWORD Page, char* OrderType, BYTE PageUnit);
void WantedMyList(DWORD CharacterIDX, DWORD Page, BYTE PageUnit);
void WantedSearch(DWORD CharacterIDX, char* WantedName, DWORD Page, BYTE PageUnit);
//////////////////////////////////////////////////////////////////////////
void CharacterHeroInfoUpdate(CPlayer* pPlayer);
void CharacterTotalInfoUpdate(CPlayer* pPlayer);
void CharacterBadFameUpdate(DWORD PlayerID, int BadFame, DWORD Money);
//void CharacterQuickInfoUpdate(CPlayer * pPlayer);
void CharacterMugongInfoUpdate(CPlayer* pPlayer);
void CharacterTablePartyIdxUpdate(DWORD PlayerID);
void PyogukMoneyUpdate(DWORD PlayerID, BYTE PyogukNum, DWORD Money);
void RegistLoginMapInfo(DWORD CharacterIDX, char* CharacterName, BYTE MapNum, WORD MapPort);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
void CharacterLvlExpUpdate(DWORD PlayerID, LEVELTYPE level, EXPTYPE exppoint);
//////////////////////////////////////////////////////////////////////////
void QuestTotalInfo(DWORD PlayerID);
void QuestUpdateToDB(DWORD PlayerID, DWORD QuestIdx, QSTATETYPE State, BYTE bEnd);
void QuestItemload(DWORD PlayerID, int QuestIdx = -1 );
void QuestItemInsert(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD dwItemNum );
void QuestItemDelete(DWORD PlayerID, DWORD ItemIdx);
void QuestItemDeleteAll(DWORD PlayerID, DWORD dwQuestIdx);
void QuestItemUpdate(DWORD PlayerID, DWORD dwQuestIdx, DWORD ItemIdx, DWORD ItemNum);

void QuestMainQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestMainQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time);
void QuestMainQuestDelete(DWORD PlayerID, DWORD mQuestIdx);
void QuestMainQuestInsert(DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD Time);

void QuestSubQuestLoad(DWORD PlayerID, int QuestIdx = -1 );
void QuestSubQuestInsert(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestUpdateToDB(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time);
void QuestSubQuestDelete(DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx);
void QuestSubQuestDeleteAll(DWORD PlayerID, DWORD mQuestIdx);

void QuestMainQuestUpdateCheckTime(DWORD dwCharIdx, DWORD dwQuestIdx, DWORD dwCheckType, DWORD dwCheckTime );

void Quest_EndQuest_New( DWORD PlayerID, DWORD mQuestIdx, QSTATETYPE State, DWORD EndParam, DWORD Time );
void Quest_DeleteQuest_New( DWORD PlayerID, DWORD mQuestIdx );
void Quest_EndSubQuest_New( DWORD PlayerID, DWORD mQuestIdx, DWORD sQuestIdx, QSTATETYPE Data, QSTATETYPE Time );

//////////////////////////////////////////////////////////////////////////
//journal
void JournalGetList(DWORD CharacterIDX);
void JouranlInsertQuest(DWORD CharacterIDX, BYTE Kind, DWORD QuestIDX, DWORD SubQuestIDX, int bCompleted);
void JouranlInsertWanted(DWORD CharacterIDX, BYTE Kind, DWORD Param, char* ParamName);
void JournalInsertLevel(DWORD CharacterIDX, BYTE Kind, DWORD Level);
void JournalSaveUpdate(DWORD CharacterIDX, DWORD JournalIndex);
void JournalDelete(DWORD CharacterIDX, DWORD JournalIndex);
//////////////////////////////////////////////////////////////////////////
// ShopItem
void ShopItemInvenInfo( DWORD CharacterIdx );
void CharacterShopItemInfo(DWORD CharacterIDX, DWORD StartDBIdx);
void UsingShopItemInfo( DWORD CharacterIdx );
void SavedMovePointInfo( DWORD CharacterIdx );
void SavedMovePointInsert( DWORD CharacterIdx, char* Name, DWORD MapIdx, DWORD Position );
void SavedMovePointUpdate( DWORD CharacterIdx, DWORD DBIdx, char* Name );
void SavedMovePointDelete( DWORD CharacterIdx, DWORD DBIdx );
void UsingShopItemUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD dwDBIdx, DWORD Param, QSTATETYPE StartTime, QSTATETYPE RemainTime );
void ShopItemDeleteToDB( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdatetimeToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD RemainTime );
void ShopItemParamUpdateToDB( DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemUseParamUpdateToDB( DWORD UserIdx, DWORD CharacterIdx, DWORD dwItemIdx, DWORD Param );
void ShopItemGetItem( DWORD CharacterIdx, WORD wItemIdx, DWORD Dur );
void ItemMoveShopUpdateToDB(DWORD CharacterIDX, DWORD UserIDX, DWORD dwfromDBIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
void ShopItemUpdateCharacterIdx( DWORD CharacterIdx, DWORD dwDBIdx );
void ShopItemUpdateUseInfoToDB( DWORD CharacterIdx, DWORD dwDBIdx, DWORD Param, DWORD RemainTime );
void CharacterChangeName( DWORD CharacterIdx, char* Name, DWORD DBIdx );
void CharacterChangeInfoToDB( DWORD CharacterIdx, DWORD Gender, DWORD HairType, DWORD FaceType, float Height, float Width );
void IsUseAbleShopAvatarItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD ItemIdx, DWORD ItemPos );
void IsAbleShopEquipItem( DWORD CharacterIdx, DWORD ItemDBIdx, DWORD FromItemIdx, DWORD FromPos, DWORD ToItemIdx, DWORD ToPos );
void ShopItemAllUseInfoUpdateToDB( DWORD ItemDBIdx, DWORD CharacterIdx, WORD ItemIdx, DWORD Param, DWORD Remaintime );

//////////////////////////////////////////////////////////////////////////
void InsertLogCharacter( DWORD charIdx, LEVELTYPE level, HERO_TOTALINFO* pTotalInfo );
void InsertLogExp( BYTE bType, DWORD charIdx, LEVELTYPE level, DWORD changeValue, EXPTYPE NowExp, WORD MurdererKind, DWORD MurdererIdx, DWORD CurAbilPoint);
void InsertLogMoney( BYTE type, DWORD charIdx, MONEYTYPE InvenMoney, MONEYTYPE ChangeValue, DWORD TargetID, MONEYTYPE TargetMoney );
void InsertLogWorngMoney( BYTE type, DWORD charIdx, MONEYTYPE changeValue, MONEYTYPE InvenMoney, MONEYTYPE PyogukMoney, DWORD targetIdx );
void InsertLogMugong( WORD type, DWORD charIdx, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp );
void InsertLogMugongExp( DWORD charIdx, EXPTYPE changeValue, DWORD mugongIdx, DWORD mugongDBIdx, LEVELTYPE mugongSung, EXPTYPE mugongExp, POSTYPE mugongPos );
void LogItemMoney(DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
				WORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
				DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
				DURTYPE ItemDur, EXPTYPE ExpPoint);
void LogItemOption(DWORD CharacterIdx, DWORD ItemDBIdx, ITEM_OPTION_INFO* pOptionInfo);
void LogItemRareOption(DWORD CharacterIdx, DWORD ItemDBidx, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
void LogCheat(DWORD Character_Idx, BYTE LogType, MONEYTYPE Param, DWORD ItemIdx, DWORD ItemDBIdx);
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );
void LogGuild(DWORD CharacterIdx, DWORD GuildIdx, WORD LogType, DWORD Param1 = 0, DWORD Param2 = 0);
void LogPet(DWORD CharacterIdx, DWORD UserIdx, BYTE LogType, DWORD PetDBIdx, DWORD PetSummonItemDBIdx, DWORD PetGrade, DWORD PetStamina, DWORD PetFriendship, BYTE PetAlive);
void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );
//////////////////////////////////////////////////////////////////////////
void UpdateCharacterInfoByTime(DWORD charIdx, EXPTYPE exp, DWORD abilityExp, MONEYTYPE money, DWORD time, DWORD LastPKEndTime );
void UpdateMugongExpByTime(DWORD charIdx, DWORD DBIdx, EXPTYPE exp);

void InsertSpeedHackCheck(DWORD charIdx, char* charName, DWORD Distance, DWORD skillDelay);
//////////////////////////////////////////////////////////////////////////
void ReceiveMessageFromDB(LPQUERY pData,  LPDBMESSAGE pMessage);
void RTestQuery(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNumSendAndCharacterInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterInfoQueryToEventMap(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterMugongInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemSlotInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterAbilityInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPetInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterTitanEquipItemEnduranceInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetInvenItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPetWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RPetInsert__(DWORD CharacterIdx, PET_TOTALINFO* pPetTotalInfo);

void RMapBaseEconomy(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageQuery(LPQUERY pData, LPDBMESSAGE pMessage);

void RCharacterItemGetBuyIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RSSItemInsert(LPQUERY pData, LPDBMESSAGE pMessage); 
void RCharacterItemGetCheatIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetDivideIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDivideNewItemIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterItemGetHuntIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterDupItemInsertIDX(LPQUERY pData, LPDBMESSAGE pMessage);
void RShopItemInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RSavedMovePointInsert( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void RCharacterChangeName( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsUseAbleShopAvatarItem( LPQUERY pData, LPDBMESSAGE pMessage );
void RIsAbleShopEquipItem( LPQUERY pData, LPDBMESSAGE pMessage );

void RItemInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemOptionInsert(LPQUERY pData, LPDBMESSAGE pMessage);
void RItemRareInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void RPetInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void RMugongInsert(LPQUERY pData, LPDBMESSAGE pMessage);

void RAuctionCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionPageList(LPQUERY pData, LPDBMESSAGE pMessage);
void RAuctionConfirm(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RPartyLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyCreate(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void RPartyInfoByUserLogin(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//-----------------------------------------------------------------
void RPartyChangeMaster(LPQUERY pData, LPDBMESSAGE pMessage);
void RPartyDelMember(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RCharacterStatsHistory(LPQUERY pData, LPDBMESSAGE pMessage);
void RSavePointUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadGuild(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMember(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadMark(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildCreate(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildBreakUp( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildDeleteMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildSecedeMember( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildItemOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildItemRareOption(LPQUERY pData, LPDBMESSAGE pMessage);
void RGuildMarkRegist(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RGuildLoadNotice(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RCharacterPyogukInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterPyogukItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RPyogukItemRareOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RPyogukItemOptionInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void RFriendNotifyLogouttoClient(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RWantedLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedInfoByUserLogIn(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedRegist(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedComplete(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDestroyed(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedOrderedList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedMyList(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedSearch(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RQuestTotalInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestMainQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestSubQuestLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void RQuestItemLoad(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RJournalGetList(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RSaveMapChangePointReturn(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void RCharacterShopItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void ConfirmUserOut( DWORD dwConnectIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD dwKind );
void RConfirmUserOut( LPQUERY pData, LPDBMESSAGE pMessage );

void SaveCharInfoBeforeLogout(DWORD PlayerIdx, DWORD dwConnectionIndex, DWORD UserIdx, MONEYTYPE PyogukMoney);
void RSaveCharInfoBeforeLogout( LPQUERY pData, LPDBMESSAGE pMessage );

void LoadGuildWarRecord( DWORD dwIdx );
void RLoadGuildWarRecord( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteGuildWarRecord( DWORD dwGuildIdx );
void LoadGuildFieldWar( DWORD dwIdx );
void RLoadGuildFieldWar( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
void DeleteGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
void UpdateGuildFieldWarRecordToDB( DWORD dwGuildIdx, DWORD dwVitory, DWORD dwDraw, DWORD dwLoose );

void GuildTournamentInfoLoad();
void GuildTournamentCancelIdxLoad( DWORD TournamentIdx );
void GuildTournamentCancelIdxInsert( DWORD TournamentIdx, DWORD GuildIdx );
void GuildTournamentGuildInfoInsert( DWORD GuildIdx, char* GuildName, DWORD Position );
void GuildTournamentGuildInfoDelete( DWORD GuildIdx );
void GuildTournamentGuildInfoUpdate( DWORD GuildIdx, DWORD Ranking, DWORD ProcessTournament );
void GuildTournamentInfoUpdate( DWORD TournamentIdx, DWORD TournamentFight, DWORD WaitTime );
void RGuildTournamentInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void RGuildTournamentCancelIdxLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildTournamentAddLog( DWORD TournamentIdx, DWORD GuildIdx, DWORD Logkind, DWORD Ranking );

void JackpotAddTotalMoney( MONEYTYPE MapMoney );
void JackpotPrizeInfo( MONEYTYPE MapMoney, DWORD PlayerID, DWORD PrizeKind, DWORD PrizePercentage, DWORD dwAbsMoney = 0);
void RJackpotPrizeInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void GuildUnionLoad( DWORD dwStartGuildUnionIdx );
void RGuildUnionLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionLoadMark( DWORD dwMarkIdx );
void RGuildUnionLoadMark( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

void GuildUnionCreate( DWORD dwCharacterIdx, DWORD dwGuildIdx, char* sUnionName );
void RGuildUnionCreate( LPQUERY pData, LPDBMESSAGE pMessage );
void GuildUnionDestroy( DWORD dwGuildUnionIdx, DWORD dwMarkIdx, DWORD dwMasterIdx, DWORD dwTime );
void GuildUnionAddGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionRemoveGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx );
void GuildUnionSecedeGuild( DWORD dwGuildUnionIdx, int nIndex, DWORD dwGuildIdx, DWORD dwTime );

void GuildUnionMarkRegist( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );
void RGuildUnionMarkRegist( LPLARGEQUERY pData, LPDBMESSAGE pMessage );

//ΞΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΟ
//ΝSiegeWar													  Ν
//ΡΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΜΠ
void SiegeWarInfoLoad( DWORD MapNum );
void RSiegeWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarInfoInsert( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarInfoUpdate( DWORD SiegeWarIdx, DWORD RegistTime, DWORD SiegeWarTime, DWORD Level, DWORD MapNum );
void SiegeWarGuildInfoLoad( DWORD StartDBIdx, DWORD SiegeWarIdx, DWORD MapNum );
void RSiegeWarGuildInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarGuildInsert( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildUpdate( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD Type, DWORD MapNum );
void SiegeWarGuildDelete( DWORD SiegeWarIdx, DWORD GuildIdx, DWORD MapNum );
void SiegeWarAddLog( DWORD SiegeWarIdx, DWORD MapNum, DWORD GuildIdx, DWORD Logkind, DWORD Value );

void ChangeCharacterAttr( DWORD dwPlayerIdx, DWORD dwAttr );

void SiegeWarProfitInfoLoad();
void RSiegeWarProfitInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void SiegeWarProfitUpdateGuild( int nMapNum, DWORD dwGuildIdx );
void SiegeWarProfitUpdateTaxRate( int nMapNum, int nTaxRate, DWORD dwRegDate );
void SiegeWarProfitUpdateProfitMoney( int nMapNum, DWORD dwMoney, int nType = 0 );
void RSiegeWarProfitUpdateProfitMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void ChangeGuildLocation( DWORD dwGuildIdx, WORD wMapNum );

void ChangeCharacterStageAbility( DWORD dwCharacterIdx, BYTE stage, CAbilityGroup* pGroup );

void QuestEventCheck( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwNpcIdx, DWORD dwQuestIdx, char* pTime );
void RQuestEventCheck( LPQUERY pData, LPDBMESSAGE pMessage );

void QuestEventEnd( DWORD dwUserIdx, DWORD dwCharacterIdx, char* pTime );

void GMEvent01( DWORD dwUserIdx, DWORD dwCharacterIdx, DWORD dwLevel, int nServerSetNum );

void UCharacterExpFlag(DWORD CharacterIDX, BYTE ExpFlag);
////////////////////////////////////////////////////////////////////////////////////
void GuildTraineeInfo(DWORD StartDBIdx);
void RGuildTraineeInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeInsert(DWORD GuildIdx, DWORD UserIdx, char UserName[], DWORD JoinTime);
void GuildTraineeDelete(DWORD UserIdx);

void GuildTraineeGuildInfo(DWORD StartDBIdx);
void RGuildTraineeGuildInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildInsert(DWORD GuildIdx);
void RGuildTraineeGuildInsert(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
void GuildTraineeGuildDelete(DWORD GuildIdx);

void GuildTraineeDeletebyGuild(DWORD GuildIdx);

void GuildAddStudent(DWORD GuildIdx, DWORD MemberIDX, char* GuildName, BYTE Rank);
void RGuildAddStudent(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroInfo(DWORD StartDBIdx);
void RGuildTraineeGuildIntroInfo(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeGuildIntroUpdate(DWORD GuildIdx, char* GuildIntro);
////////////////////////////////////////////////////////////////////////////////////
void GuildStudentLvUpCtUpdate(DWORD GuildIdx, DWORD AddPoint, DWORD ToPoint);	
void RGuildStudentLvUpCtUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildStudentLvUpCtInit(DWORD GuildIdx, DWORD MasterIDX);
void RGuildStudentLvUpCtInit(LPQUERY pData, LPDBMESSAGE pMessage);

void TitanWearItemInfo( DWORD CharacterIdx, POSTYPE FromPos, POSTYPE ToPos );
void RTitanWearItemInfo(LPQUERY pData, LPDBMESSAGE pMessage);
//////////////////////////////////////////////////////////////////////////
void TestGameQuery( DWORD CharacterIdx );
void RTestGameQuery( LPQUERY pData, LPDBMESSAGE pMessage );
void TestLogQuery( DWORD CharacterIdx );
void RTestLogQuery( LPQUERY pData, LPDBMESSAGE pMessage );


void PyogukTitanEnduranceInfo(DWORD CharacterIdx, DWORD UserIdx, DWORD StartDBIdx);
void RPyogukTitanEnduranceInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void CharacterSkinInfoUpdate(CPlayer* pPlayer);
void CharacterSkinInfo(DWORD dwCharacterIndex);
void RCharacterSkinInfo(LPQUERY pData, LPDBMESSAGE pMessage);

void CharacterUpdateResetStatusPoint(DWORD dwCharacterIDX, DWORD dwStatusPoint);

void ItemLimitInfoLoadAll( DWORD dwStartDBIdx );
void RItemLimitInfoLoadAll( LPQUERY pData, LPDBMESSAGE pMessage );
void ItemLimitInfoUpdate( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount );
void RItemLimitInfoUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

void AutoNoteListLoad( DWORD dwCharacterIdx );
void RAutoNoteListLoad( LPQUERY pData, LPDBMESSAGE pMessage );	
void AutoNoteListAdd( DWORD dwCharacterIdx, DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pStrAutoName );
void RAutoNoteListAdd( LPQUERY pData, LPDBMESSAGE pMessage );

void MapItemDropLog( WORD wUserIDX, WORD wCharacterIDX, WORD wItemIDX, WORD wMap, WORD wChannel, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListSelect( WORD wMapNum );
void MapItemDropListUpdate( DWORD dwCharacterIdx, WORD wMapNum, WORD wChannel, WORD wItemIDX, WORD wDropCount, WORD wMaxDropCount );
void MapItemDropListInit( WORD wMapNum );
void RMapItemDropListSelect( LPQUERY pData, LPDBMESSAGE pMessage );
void RMapItemDropListUpdate( LPQUERY pData, LPDBMESSAGE pMessage );

void FortWarInfoLoad();
void RFortWarInfoLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarInfoUpdate( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx );
void FortWarProfitMoneyUpdate( int nWarIDX, DWORD dwMoney, int nType );
void RFortWarProfitMoneyUpdate( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemLoad( int nWarIDX );
void RFortWarItemLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemInsertToDB( int nWarIDX, DWORD dwItemIdx, DURTYPE Durability, POSTYPE wPos, DWORD dwParam );
void RFortWarItemInsertToDB( LPQUERY pData, LPDBMESSAGE pMessage );
void FortWarItemMoveToDB( DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD wPos, int nWarIDX );

void ItemLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam); 

void RItemLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

void ItemUnLockUpdate(DWORD dwCharacterIdx,DWORD wdItemDBIdx,WORD wParam); 

void RItemUnLockUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

void ItemGrowUpdate(DWORD dwCharacterIdx, DWORD dwItemDBIdx, DWORD dwGrow,BOOL bIsInherit = FALSE);

void RItemGrowUpdate(LPQUERY pData, LPDBMESSAGE pMessage);

BOOL isValidQueryString( char * str );

BOOL CheckString( const char *strChk );
void RUpdateUserMallMoney(LPQUERY pData, LPDBMESSAGE pMessage);
void RUpdateUserCredit(LPQUERY pData, LPDBMESSAGE pMessage); //[在线充值元宝刷新数据库回调处理函数定义][By:十里坡剑神][QQ:112582793][2017/11/21]
void CharacterHeroGoldInfoUpdate(DWORD dwID,DWORD dwChangeValue,WORD type);	 //[在线充值元宝刷新数据库回调处理函数定义][By:十里坡剑神][QQ:112582793][2017/11/21]
void CharacterHeroMallInfoUpdate(DWORD ID,DWORD dwChangeValue,WORD type);  
//[元宝交易日志][By:十里坡剑神][QQ:112582793][2017/11/21]
void LogGoldMoney(WORD LogType, DWORD FromChrID, DWORD FromTotalGold, DWORD ToChrID, DWORD ToChrTotalGold, DWORD ChangeGold, DWORD BuyItemIdx, DWORD ItemDBIdx, DWORD Durability = 0);
//[商城使用日志][BY:十里坡剑神][QQ:112582793][2019-9-15][3:32:38]
void ItemShopUseLog(WORD Type, DWORD dwChrID, char* CharName, WORD wItemIdx, char* ItemName, WORD wItemNum, DWORD TotalMall, DWORD UseMall, DWORD LastMall, DWORD TotalGold, DWORD UseGold, DWORD LastGold, DWORD TotalMoney, DWORD UseMoney, DWORD LastMoney);
//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
void ItemAbrasionUpdate(DWORD dwCharacterIdx, DWORD dwItemDBIdx, WORD Abrasion,WORD MaxAbrasion);
void RItemAbrasionUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void ItemMakerUpdate(DWORD dwCharacterIdx, DWORD dwItemDBIdx, char* Maker);
void RItemMakerUpdate(LPQUERY pData, LPDBMESSAGE pMessage);
void ItemMaker1Update(DWORD dwCharacterIdx, DWORD dwItemDBIdx, char *Maker1);
void RItemMaker1Update(LPQUERY pData, LPDBMESSAGE pMessage);
//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
void SingedInfoLoad(DWORD UserIdx);
void RSingedInfoLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void UpdateSingedInfo(DWORD UserIdx,const char* day);
//[游戏配置加载函数][By:十里坡剑神][QQ:112582793][2017/11/21]
void LoadGameConfig();
void RGameConfig(LPQUERY pData, LPDBMESSAGE pMessage );
void UpdateGameConfig(GAMECONFIG &GameCfg);
//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
void LoadRechargeInfo(DWORD character_idx);
void RLoadRechargeInfo(LPQUERY pData, LPDBMESSAGE pMessage );
void UpdateRechargeInfo(DWORD character_idx, DWORD PayMoney, DWORD UsedMoney);
//[冲榜奖励][By:十里坡剑神][QQ:112582793][2018/3/9]
void SendSprofit(SPROFIT &Config);
void UpdateRestSetTime(DWORD RestTime,DWORD SprofitTime);
//[牛巨任务][By:十里坡剑神][QQ:112582793][2018/4/18]
void UpdateQuestN(DWORD characterIdx);
void UpdateQuestJ(DWORD characterIdx);
//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
void KillTopInfoLoad(DWORD CharacterIdx);
void RKillTopInfoLoad(LPQUERY pData, LPDBMESSAGE pMessage);
void UpDateKillTopInfo(DWORD characterIdx,const char* day);
//[记录99级升级时间][By:十里坡剑神][QQ:112582793][2018/6/10]
void CharacterLevelUp99Log(DWORD characterIdx,DWORD dwTime);

//[整理背包物品信息查询][By:十里坡剑神][QQ:112582793][2018/6/11]
void LoadCharacterItemInfoForMovePak(DWORD characterIdx);
void RCharacterItemInfoForMovePak(LPQUERY pData, LPDBMESSAGE pMessage);

//[离婚结婚名更新][BY:十里坡剑神][QQ:112582793][2019-8-27][23:30:46]
void MarryDelUpdate(char* Name);

//[盟战个人击杀更新][BY:十里坡剑神][QQ:112582793][2019-10-30][16:41:39]
void CharacterUnionWarInfoUpdate(CPlayer* pPlayer);
#endif //__MAPBMSGPARSER_H__



