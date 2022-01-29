




#ifndef __AGENTDBMSGPARSER_H__
#define __AGENTDBMSGPARSER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "db.h"
#include "CommonDBMsgParser.h"


//////////////////////////////////////////////////////////////////
//#ifdef _JAPAN_LOCAL_
//#define dMAXCHARACTER 5
//#elif defined _HK_LOCAL_
//#define dMAXCHARACTER 5
//#elif defined _TL_LOCAL_
//#define dMAXCHARACTER 5
//#else
#define dMAXCHARACTER 4
//#endif

enum BasicCharacterInfo
{
	eCL_ObjectID = 0, eCL_StandIndex, eCL_ObjectName, eCL_BodyType, eCL_HeadType, 
	eCL_Hat, eCL_Dress, eCL_shoes, eCL_Weapon, 
	eCL_Grade, eCL_Map, eCL_Gender, eCL_Height, eCL_Width, eCL_Stage, eCL_AuthKey,
};

enum MakeCharacterInfo
{
	eMC_UserID = 0, eMC_Gengoal, eMC_DEX, eMC_STA, eMC_Simmak, eMC_Name, eMC_Job, eMC_BodyType, eMC_HeadType
};

enum Friendinfo
{
	eFr_TargetID=0, eFr_FromID, eFr_ToPlayerName, eFr_FromPlayerName, eFr_Err, 
};
enum Friendisvalidtarget
{
	eFr_vtFromid , eFr_vtTargetid , eFr_vtFromname , eFr_vtToname, eFr_vtvailderr, 
};
enum Frienddel
{
	eFr_targetname, eFr_IsSuccess, 
};
enum Friendadd 
{
	eFr_addToErr, eFr_addToIDX, eFr_addToName, eFr_addFromErr, eFr_addFromIDX, eFr_addFromName, 
};
enum Friendlist
{
	eFr_FriendName, eFr_IsLoggin,
};
enum Friendlogin
{

	eFr_LTargetID, eFr_LLoggedname,
};
enum Friendloginedfriend
{
	eFr_LFFriendID, eFr_LFFriendName,
};
enum 
{
	eFr_FLFriendid, eFr_FLFriendname, eFr_FLIsLogin, eFr_FLTotalPage,
};
enum Friendsendnote
{
	eFr_NErr, eFr_NToName, eFr_NToId,
};
enum FriendNotelist
{
	eFr_NNoteID, eFr_NSender, eFr_NSentDate, eFr_NSentTime, eFr_NTotalpage, eFr_NbIsRead,
};
enum FriendReadnote
{
	eFr_NRNNoteID, eFr_NRNSender, eFr_NRNNote, eFr_NRNItemIdx,
};
enum FriendDeletenote
{
	eFr_NdNoteID, eFr_NdbLast,
};

enum 
{
	eCharacterBaseQuery = 1,	
	eCreateCharacter,			
	eLoginCheckDelete,			
	eDeleteCharacter,		
	eNewCharacterCheckName,   
	eForcedDisconnect,          
	eSetCurMaxCount,			
	eLoginMapInfoSearchForName, 
	eSavePoint,					
	eUnRegistLoginMapInfo,		
	eMapUserUnRegistMapInfo,	
	eFriendGetTargetMemeberIDX,	
	eFriendAdd,					
	eFriendIsValidTarget,		
	eFriendDel,					
	eFriendDelID,				
	eFriendDelList,				
	eFriendNotifyLogin,			
	eFriendGetFriendList,		
	eFriendGetLoginFriends,		
	eNoteIsNewNote,				
	eNoteDelAll,				
	eNoteList,				
	eNoteDelete,				
	eWantedDelete,				
	eWantedDelWantInfo,			
//For GM-Tool	
	eGM_BanCharacter,
	eGM_UpdateUserLevel,
	eGM_WhereIsCharacter,
	eGM_Login,
	eGM_GetGMPowerList,
	eAgentDown,				
	eBilling_RemainTimeUpdate,
	eBilling_UpdateAll,
	eEventItemUse050106,		
	eCheckGuildMasterLogin,		
	eEventItemUse050203,		
	eInsertBillingTable,		
	eDeleteBillingTable,
	eEventItemUse050322,		
	eJackpot_TotalMoney_Load,	
	eCheckGuildFieldWarMoney,	
	eAddGuildFieldWarMoney,		
	eChaseFindUser,
	eJP_CharacterSlot,			
	eEventItemUse051108,		
	eEventItemUse2,					
	eGM_UpdateUserState,
	eLogGMToolUse,					
	eNProtectBlock,
	eHackToolUser,
	eGuildTraineeDelete,
	ePunishListLoad,		
	ePunishListAdd,	
	ePunishCountAdd,
	eInsertLog_AutoBlock,
	eSafeLockCheck,//[安全锁解锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	eSafeLockRestPwd,//[安全锁修改密码][By:十里坡剑神][QQ:112582793][2018/4/30]
	eMarryLoginNotice, //[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	MaxQuery
};

BOOL isValidQueryString( char * str );
BOOL CheckString( const char *strChk );
BOOL CheckMessageString(const char *strChk);
void CheatLog(DWORD CharacterIDX,DWORD CheatKind);
void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex);
void LoginCheckDelete(DWORD UserID);
void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex);
void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex);
void CharacterNameCheck(char* pName, DWORD dwConnectionIndex);
void SetUserCurAndMaxCount(DWORD CurCount, DWORD MaxCount);
void SearchWhisperUserAndSend(DWORD dwPlayerID, char* CharacterName, DWORD dwKey );
void SearchPartyChatSendOnPlayer(DWORD dwPlayerID, DWORD dwKey );
void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx);
void UnRegistLoginMapInfo(DWORD CharacterIDX);
void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName);
void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID);
void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName);
void FriendDelFriend(DWORD CharacterIDX, char* TargetName);
void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast);
void FriendNotifyLogintoClient(DWORD CharacterIDX);
void FriendGetLoginFriends(DWORD CharacterIDX);
void FriendGetFriendList(DWORD CharacterIDX);
void NoteIsNewNote(DWORD PlayerID);
void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note);
void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note);
void NoteDelAll(DWORD CharacterIDX);
void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode);
void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront);
void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast);
void WantedDelete(DWORD CharacterIDX);
void WantedDelWantInfo(DWORD WantedIDX);
void ChaseFindUser(DWORD UserIdx, char* WantedName, DWORD dwItemIdx);
void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID );
void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel);
void GM_GetGMPowerList(DWORD dwStartIdx, DWORD dwFlag = 0 );
void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP );
void BillingRemainTimeUpdate(DWORD dwUserIdx, int nRemainTime );
void BillingUpdateAll( WORD wAgentNum );
// Query Result function
void RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage);
void RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RLoginCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteList(LPQUERY pData, LPDBMESSAGE pMessage);
void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage);
void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage);
void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage);
void RWantedDelete(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage);
void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage);
void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol );
void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage );
void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney );
void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );
void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney );
void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void EventItemUse050106( DWORD dwUserIdx );
void EventItemUse050203( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
void EventItemUse050322( DWORD dwUserIdx );
void EventItemUse051108( DWORD dwObjectIdx, DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
void REventItemUse051108( LPQUERY pData, LPDBMESSAGE pMessage );
void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum );
void JackpotLoadTotalMoney();
void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage );

void RChaseFindUser( LPQUERY pData, LPDBMESSAGE pMessage );

void InsertBillingTable( DWORD dwUserIdx, DWORD dwBillType, DWORD dwAgentNo, DWORD dwServerSetNum );
void DeleteBillingTable( DWORD dwUserIdx, DWORD dwServerSetNum );

void GetCharacterSlotCount( DWORD UserIdx, DWORD AuthKey, DWORD dwConnectionIndex );
void RGetCharacterSlotCount( LPQUERY pData, LPDBMESSAGE pMessage );

void GM_UpdateUserState(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState);
void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage);

void GuildTraineeDelete(DWORD UserIdx);

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 );

void PunishListLoad( DWORD dwUserIdx );
void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage );
void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime );

void PunishCountAdd( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, int nPunishKind, int nPeriodDay, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName );
void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage );

void InsertLog_AutoBlock( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, DWORD dwBlockTime, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName, int nServerSet );
//断线重连登录地图修改	by:十里坡剑神	QQ:112582793
void TopListCheck(DWORD dwConnectionIndex,char * msg);
void RTopListCheck(LPLARGEQUERY pData, LPDBMESSAGE pMessage);
//[重置签到系统数据库信息][By:十里坡剑神][QQ:112582793][2018/1/7]
void RestSingedInfo();
void SafeLockUnLockCheck(DWORD dwConnectionIndex,DWORD propid,char* pinpwd);
void RSafeLockUnLockCheck(LPQUERY pData, LPDBMESSAGE pMessage);
void SafeLockRestPwd(DWORD dwConnectionIndex,DWORD propid,char* NewPinPwd,char * PinPwd);
void RSafeLockRestPwd(LPQUERY pData, LPDBMESSAGE pMessage);
//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
void MarryNoticeLogin(DWORD characterIDX);
void RMarryNoticeLogin(LPQUERY pData, LPDBMESSAGE pMessage);
#endif //__AGENTBMSGPARSER_H__


