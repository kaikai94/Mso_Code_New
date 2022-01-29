
#ifndef __MSDBMSGPARSER_H__
#define __MSDBMSGPARSER_H__

#include "db.h"


typedef	void (*DBMsgFunc)(LPQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBMiddleMsgFunc)(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage);
typedef	void (*DBLargeMsgFunc)(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
extern DBMsgFunc g_DBMsgFunc[];

#ifdef DBID
#undef DBID
#endif
#define DBID(a) a, 

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a)


enum   DBMESSAGEIDFUNC 
{
	#include "MSDBMsgID.h"
	MaxQuery,
};

/*
#ifdef DBID
#undef DBID
#endif
#define DBID(a)

#ifdef DBRETID
#undef DBRETID
#endif
#define DBRETID(a) void a(LPQUERY pData, LPDBMESSAGE pMessage); 

#include "MSDBMsgID.h"
*/

enum eOperInfo
{
	eOI_ErroCode = 0, eOI_OperIdx, eOI_OperID, eOI_OperName, eOI_OperPwd, eOI_OperPower, eOI_Date, eOI_Time, 
	eOI_IPIdx, eOI_IPAdress, eOI_IPDate, eOI_IPTime, 
};

#define eOI_MyIdx	eOI_ErroCode

enum eIPInfo
{
	eII_IPIdx, eII_IPAdress, eII_Date, eII_Time,
};

enum eUserTotalInfo 
{
	eUI_IsLogin, eUI_User_Idx, eUI_Id, eUI_Pwd, eUI_Name, eUI_Idcard, eUI_Phone, eUI_Email, eUI_Regdate, eUI_LastLogintime, eUI_LastLogouttime, eUI_Totaltime, eUI_UserLevel,
	eUI_Char_Server,
};

enum eCharacterBaseInfo 
{
	eCS_CharacterIdx = 0, eCS_UserIdx, eCS_PartyIdx, eCS_Gender, eCS_Gengoal, eCS_Life, eCS_MaxLife, eCS_Energe,
	eCS_Dex, eCS_Sta, eCS_Simmak, eCS_Fame, eCS_Karma, eCS_Name, eCS_Repute, eCS_Grade, eCS_MaxGrade, eCS_Expoint,
	eCS_GradeUpPoint, eCS_Money, eCS_QuickPace, eCS_Vitality, eCS_Map, eCS_FaceType, eCS_HeadType, eCS_Hat, eCS_Weapon,
	eCS_Dress, eCS_Shoes, eCS_Ring1, eCS_Ring2, eCS_Cape, eCS_Necklace, eCS_Armlet, eCS_Belt, eCS_MunpaIdx, eCS_MunpaPositon,
	eCS_MunpaName, eCS_MapChangePoint, eCS_LoginPoint, eCS_Shield, eCS_AbilityExp, eCS_BadFame, eCS_Stage,
};

enum eCharacterPyogukInfo
{
	eCP_UserIdx = 0, eCP_PyogukNum, eCP_PyogukMoney,
};

enum eCharacterMugong		// Ä³¸¯¹«°øÁ¤º¸¸¦ ¹ÞÀ»¶§ ¾¸
{
	eCM_ObjectID = 0, eCM_DBIDX, eCM_IDX, eCM_ExpPoint, eCM_Sung, eCM_Position, eCM_QPosition, eCM_Wear
};

enum eCharacterItem			// Ä³¸¯ÅÍ ¾ÆÀÌÅÛ Á¤º¸¸¦ ¹ÞÀ»¶§ »ç¿ë
{
	eCI_ObjectID = 0, eCI_DBIDX, eCI_IDX, eCI_Position, eCI_QPosition, eCI_Durability, eCI_Param,
};

enum ePyogukItem			// Ä³¸¯ÅÍ ¾ÆÀÌÅÛ Á¤º¸¸¦ ¹ÞÀ»¶§ »ç¿ë
{
	ePI_UserId = 0, ePI_DBIDX, ePI_IDX, ePI_Position, ePI_QPosition, ePI_Durability
};

enum eCharacterSkill		// Ä³¸¯ÅÍ ½ºÅ³ Á¤º¸¸¦ ¹ÞÀ»¶§ »ç¿ë
{
	eCK_ObjectID = 0, eCK_DBIDX, eCK_IDX, eCK_Position, eCK_QPosition, eCK_Durability
};

enum eCharacterAbility
{
	eCA_ObjectID = 0, eCA_Battle, eCA_KyungGong, eCA_Society, eCA_Job
};

enum eCharacterItemOptionInfo	// ¾ÆÀÌÅÛ ¿É¼Ç
{
	eCIOI_UserID, eCIOI_CharID, eCIOI_OptionID, eCIOI_ItemDBID, eCIOI_GenGol, eCIOI_MinChub, eCIOI_CheRyuk, eCIOI_SimMek, eCIOI_Life, eCIOI_NaeRyuk, eCIOI_Shield, 
	eCIOI_FireAttrRegist, eCIOI_WaterAttrRegist, eCIOI_TreeAttrRegist,  eCIOI_GoldAttrRegist, eCIOI_EarthAttrRegist,
	eCIOI_PhyAttack, eCIOI_Critical, 
	eCIOI_FireAttrAttack, eCIOI_WaterAttrAttack, eCIOI_TreeAttrAttack,  eCIOI_GoldAttrAttack, eCIOI_EarthAttrAttack,
	eCIOI_PhyDefense 
};

enum eMunpaBaseInfo
{
	eMB_MunpaId = 0, eMB_MunpaName, eMB_MunjuName, eMB_RegDate, eMB_RegTime, eMB_MemberNum, eMB_Location, eMB_MunpaLevel, 
	eMB_Famous,	eMB_Kind, eMB_MunpaMoney
};

enum eMunpaMemberInfo
{
	eMM_MunpaId = 0, eMM_MemberIdx, eMM_MemberName, eMM_MemberLevel, eMM_MemberJikChek, eMM_MemberDate, eMM_MemberTime,
};

enum eGameLogItemInfo
{
	eGI_CharName = 0, eGI_Dayonly, eGI_StartDate, eGI_EndDate, eGI_LogIdx, eGI_LogType, eGI_CharIdx, eGI_TargetIdx, 
	eGI_TargetName, eGI_ItemIdx, eGI_ItemDBIdx, eGI_Logdate, eGI_Logtime, eGI_Money,
};

enum eGameLogMoney
{
	eGLM_CharName = 0, eGLM_Dayonly, eGLM_StartDate, eGLM_EndDate,
	eGLM_LogIdx, eGLM_LogType, eGLM_CharIdx, eGLM_InvenMoney, eGLM_PyogukMoney, eGLM_LogDate, eGLM_LogTime,
};

enum eGameLogItem
{
	eGLI_ChrName = 0, eGLI_DayOnly, eGLI_StartDate, eGLI_EndDate, eGLI_LogIdx, eGLI_LogType, eGLI_ItemIdx, eGLI_ItemDBIdx,
	eGLI_CurChrIdx, eGLI_BeforeChrIdx, eGLI_Money, eGLI_LogDate, eGLI_LogTime, eGLI_CurChrName, eGLI_BeforeChrName,
};

enum eItemMoneyLog
{
	eIML_Type = 0, eIML_ChrName, eIML_Item, eIML_DayOnly, eIML_SDate, eIML_EDate,
	eIML_LogIdx, eIML_LogType, eIML_FromChrName, eIML_FromChrIdx, eIML_ToChrName, eIML_ToChrIdx,
	eIML_FromTotalMoney, eIML_ToTotalMoney, eIML_ChangeMoney, 
	eIML_ItemIdx, eIML_ItemDBIdx, eIML_ItemDur, eIML_ItemFromPosition, eIML_ItemToPosition, eIML_RegDate, eIML_RegTime,
};

enum ePetLog
{
	ePL_Type = 0, ePL_ChrName, ePL_Item, ePL_DayOnly, ePL_SDate, ePL_EDate,
	ePL_Idx, ePL_LogType, ePL_PetIdx, ePL_PetSummonItemDBIdx, ePL_UserIdx, ePL_CharacterIdx,
	ePL_Grade, ePL_FriendShip, ePL_Stamina, 
	ePL_Alive, ePL_LogDate, ePL_LogTime,
};

// pet
enum ePetBaseInfo
{
	ePB_PetDBIdx = 0, ePB_MasterIdx, ePB_SummonItemDBIdx, ePB_Kind, ePB_Grade, ePB_Stamina, ePB_FriendShip, ePB_Alive, 
	ePB_Summon,	ePB_Rest, ePB_Loginid, ePB_CharIdx
};

// Admin Info
void InsertIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp );
void RInsertIpAdress( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertOperator( WORD wConnectIdx, WORD wTempIdx, char* strId, char* strName, char* strPwd, WORD power );
void RInsertOperator( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteIpAdress( WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx );
void DeleteOperator( WORD wConnectIdx, WORD wTempIdx, DWORD operIdx );
void TotalIpAdressInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx );
void RTotalIpAdressInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void TotalOperatorInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, DWORD operIdx );
void RTotalOperatorInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void OperatorLoginCheck( DWORD dwConnectIdx, char* strId, char* strPwd, char* strIp );
void ROperatorLoginCheck( LPQUERY pData, LPDBMESSAGE pMessage );
/// 060829 PKH - GM툴 허용 아이피

void TotalGMIpAdressInfo( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx );
void RTotalGMIpAdressInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void InsertGMIpAdress( WORD wConnectIdx, WORD wTempIdx, char* strIp );
void RInsertGMIpAdress( LPQUERY pData, LPDBMESSAGE pMessage );
void DeleteGMIpAdress( WORD wConnectIdx, WORD wTempIdx, DWORD ipIdx );


// User Info
void SearchUserInfoById(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strId);
void SearchUserInfoByIdCard(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strIdCard);
void SearchUserInfoByCharName(WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strCharName);
void RSearchUserInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );

// Character Info
void CharacterBaseInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterBaseInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterInvenInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterInvenInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterPyogukInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx );
void RCharacterPyogukInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterPyogukItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, POSTYPE startPos );
void RCharacterPyogukItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterMugongInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterMugongInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterAbilityInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterAbilityInfo( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void CharacterItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx );
void RCharacterItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void UpdateCharacterBaseInfo( WORD wConnectIdx, WORD wTempIdx, TMSG_CHARACTER_UPDATEBASEINFO* Info );
void UpdateCharacterPyogukInfo( WORD wConnectIdx, WORD wTempIdx, DWORD UserIdx, WORD PyogukNum, MONEYTYPE Money );

// Item
void CharacterWearItemUpdate( DWORD dwCharIdx, BYTE type, WORD wItemIdx );
void InvenItemInsert( WORD wConnectIdx, WORD wTempIdx, char* sCharName, DWORD dwCharIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo, DWORD dwUserIdx = 0, DWORD dwItemKind = 0, TITAN_TOTALINFO * pTitanTotalInfo = NULL );
void RInvenItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );
void PyogukItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo );
void RPyogukItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwMunpaIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo );
void RMunpaItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

void ItemDelete( DWORD dwItemDBIdx, DWORD dwOptionDBIdx );
void ItemUpdate( DWORD dwDBIdx, ITEMBASE* pItem );
void ItemOptionUpdate( DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo );
void ItemOptionInsert( DWORD dwItemDBIdx, ITEM_OPTION_INFO* pOptionInfo );
void ItemOptionDelete( DWORD dwItemDBIdx );

// Mugong
void MugongInsert( DWORD dwCharIdx, MUGONGBASE* pMugong );
void MugongUpdate( DWORD dwMugongDBIdx, MUGONGBASE* pMugong );
void MugongDelete( DWORD dwMugongDBIdx );

// Ability 
void AbilityUpdate( DWORD dwCharIdx, ABILITY_TOTALINFO* pAbility );

// Munpa
void SearchMunpaIdxByName( WORD wConnectIdx, WORD wTempIdx, WORD wServer, char* strName );
void RSearchMunpaIdx( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaBaseInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx );
void RMunpaBaseInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, POSTYPE startPos );
void RMunpaItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RMunpaMemberInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MunpaMemberInfo( WORD wConnectIdx, WORD wTempIdx, DWORD MunpaIdx, DWORD startIdx );
void MunpaMoneyUpdate( DWORD GuildIdx, DWORD Money );

// log
void GameLogItemMoveLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogItemMoveLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemObtainLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogItemObtainLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogBaseInfoLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogBaseInfoLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogMoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD startIdx, WORD wServer, char* strId, BYTE bDay, char* strStart, char* strEnd );
void RGameLogMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemFromChrName( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, char* strChrName );
void RGameLogItemFromChrName( LPQUERY pData, LPDBMESSAGE pMessage );
void GameLogItemFromItemDBIndex( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wServer, BYTE bDay, char* strStart, char* strEnd, DWORD dwItemDBIdx );
void RGameLogItemFromItemDBIndex( LPQUERY pData, LPDBMESSAGE pMessage );

// pet
void PetInfoUpdate( DWORD dwPetDBIdx, TMSG_PET_UPDATEBASEINFO* PetInfo);

// jsd add 04.08.25 ---------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

// IP Check 
void TotalIpInfo( DWORD startIdx );							// ip
void RTotalIpInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// Log
void ItemMoneyLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RItemMoneyLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RCharacterLog( LPQUERY pData, LPDBMESSAGE pMessage );
void ExpPointLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RExpPointLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MugongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMugongLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MugongExpLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMugongExpLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MoneyLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMoneyLog( LPQUERY pData, LPDBMESSAGE pMessage );
void MoneyWrongLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RMoneyWrongLog( LPQUERY pData, LPDBMESSAGE pMessage );
void AbilityLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RAbilityLog( LPQUERY pData, LPDBMESSAGE pMessage );
void ItemOptionLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void ItemOptionLog2( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwDBIdx, int nDay, char* sSDate, char* sEDate );
void RItemOptionLog( LPQUERY pData, LPDBMESSAGE pMessage );

void CheatLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RCheatLog( LPQUERY pData, LPDBMESSAGE pMessage );
void HackingLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RHackingLog( LPQUERY pData, LPDBMESSAGE pMessage );

void ToolLog( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwLogType, char* sName, int nDay, char* sSDate, char* sEDate );
void RToolLog( LPQUERY pData, LPDBMESSAGE pMessage );
void PetLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RPetLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 문파토너먼트로그정보
void GuildTNLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RGuildTNLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 공성전로그정보
void SiegeWarLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, int nDay, char* sSDate, char* sEDate );
void RSiegeWarLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060915 PKH GMTOOL
void GMToolLogFromType( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, WORD wLogType, DWORD dwCharIdx, DWORD dwItem, int nDay, char* sSDate, char* sEDate );
void RGMToolLogFromType( LPQUERY pData, LPDBMESSAGE pMessage );


// user info
void SearchMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID );
void RSearchMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchMemberNumFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchMemberNumFromCharName( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchCharIdxFromCharName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchCharIdxFromCharName( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 문파정보
void SearchGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage );

/// 060831 PKH 공성전
void SearchSWGuildIdxFromGuildName( WORD wConnectIdx, WORD wTempIdx, char* sCharName );
void RSearchSWGuildIdxFromGuildName( LPQUERY pData, LPDBMESSAGE pMessage );

void SearchUserInfoFromMemberNum( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum, WORD wServer );
void RSearchUserInfoFromMemberNum( LPMIDDLEQUERY pData, LPDBMESSAGE pMessage );
void SimpleCharacterInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum );
void RSimpleCharacterInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterCreateInfoFromMN( WORD wConnectIdx, WORD wTempIdx, DWORD dwMemberNum, DWORD dwStartIdx );
void RCharacterCreateInfoFromMN( LPQUERY pData, LPDBMESSAGE pMessage );

void CheckCharacterName( WORD wConnectIdx, WORD wTempIdx, char* sName );
void RCheckCharacterName( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterRecover( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, char* sName );
void RCharacterRecover( LPQUERY pData, LPDBMESSAGE pMessage );

// quest
void CharacterMainQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterMainQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void MainQuestDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx );
void CharacterSubQuestInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwQuestIdx );
void RCharacterSubQuestInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// log
void InsertItemMoneyLog( DWORD FromChrIdx, char* FromChrName, DWORD ToChrIdx, char* ToChrName,
					     DWORD LogType, MONEYTYPE FromTotalMoney, MONEYTYPE ToTotalMoney, MONEYTYPE ChangeMoney,
						 DWORD ItemIdx, DWORD ItemDBIdx, POSTYPE ItemFromPos, POSTYPE ItemToPos,
						 DURTYPE ItemDur, DWORD ExpPoint );
void InsertLogTool( DWORD dwLogType, DWORD dwLogKind, DWORD dwOperIdx, char* sOperName, DWORD dwTargetIdx, char* sTargetName,
				    DWORD dwParam1, DWORD dwParam2, DWORD dwParam3, DWORD dwParam4 );

// userlevel
void ChangeUserLevel( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwUserLevel );
void RChangeUserLevel( LPQUERY pData, LPDBMESSAGE pMessage );

// loginpoint
void ChangeLoginPoint( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharacterIdx, DWORD dwMapNum );
void RChangeLoginPoint( LPQUERY pData, LPDBMESSAGE pMessage );

// characterinveninfo2 
void CharacterInvenInfo2(WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwCharIdx);
void RCharacterInvenInfo2(LPQUERY pData, LPDBMESSAGE pMessage);

void ChangeItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );
void ChangeWareItemPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );
void ChangeMugongPosition( WORD wConnectIdx, WORD wTempIdx, DWORD dwChrIdx, DWORD dwDBIdx, DWORD dwPos );

void CharacterShopWareItemInfo( WORD wConnectionIdx, WORD wTempIdx, DWORD dwUserIdx );
void RCharacterShopWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );

void ShopWareItemInsert( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, ITEMBASE* pItem, ITEM_OPTION_INFO* pOptionInfo );
void RShopWareItemInsert( LPQUERY pData, LPDBMESSAGE pMessage );

// Guild
void SearchGuild( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx );
void RSearchGuild( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildInfo( WORD wConnectIdx, WORD wTempIdx, char* pGuildName );
void RSearchGuildInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildMemberInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildMemberInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchGuildWareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwStartIdx, DWORD dwGuildIdx );
void RSearchGuildWareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// Rare Item
void CharacterRareItemOptionInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwUserIdx, DWORD dwCharIdx, DWORD StartIdx );
void RCharacterRareItemOptionInfo( LPQUERY pData, LPDBMESSAGE pMessage );

// shop item
void CharacterUsingShopItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void RCharacterUsingShopItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterUsingShopItemInfoDelete( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwDelete );			
void CharacterUsingShopItemInfoEdit( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx, DWORD dwItemDBIdx, DWORD dwParam );			


// pet
void SearchPetByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD dwPetIdx);
void RSearchPetInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchPetList(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchPetWareItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void RSearchPetAccItemInfo( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchPetWareItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwcharidx, DWORD dwStartIdx, DWORD dwEndIdx );
void SearchPetListByidx(WORD wConnectIdx, WORD wTempIdx, WORD wServer, DWORD loginidx);
void SearchPetAccItemInfo( WORD wConnectIdx, WORD wTempIdx, DWORD dwPetIdx, DWORD dwStartIdx, DWORD dwEndIdx );
void RSearchUserLoginChk( LPQUERY pData, LPDBMESSAGE pMessage );
void SearchUserLoginChk( WORD wConnectIdx, WORD wTempIdx, DWORD user_num );
void RCharacterMainPetList( LPQUERY pData, LPDBMESSAGE pMessage );
void CharacterMainPetList( WORD wConnectIdx, WORD wTempIdx, DWORD dwCharIdx );
void SearchPetMemberNumFromID( WORD wConnectIdx, WORD wTempIdx, char* sID );
void RSearchPetMemberNumFromID( LPQUERY pData, LPDBMESSAGE pMessage );

//delete character list
void SearchDelCharList( WORD wConnectIdx, WORD wTempIdx, WORD WServer, char* delcharname );
void RSearchDelCharList( LPQUERY pData, LPDBMESSAGE pMessage );
#endif
