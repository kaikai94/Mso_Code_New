#include "stdafx.h"
#include "CommonDBMsgParser.h"
#include "AgentDBMsgParser.h"
#include "DataBase.h"
#include "UserTable.h"
#include "ServerTable.h"
#include "Console.h"
#include "Network.h"
#include "MsgTable.h"
#include "ServerSystem.h"
#include "GMPowerList.h"

#include "JackpotManager_Agent.h"
#include "PunishManager.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif

extern int g_nServerSetNum;

BOOL CheckString( const char *strChk )
{

	if(StrStrI(strChk,"select")!=NULL ||
	 StrStrI(strChk,"truncate")!=NULL || 
	   StrStrI(strChk,"delete")!=NULL || 
	   StrStrI(strChk,"insert")!=NULL ||
	   StrStrI(strChk,"update")!=NULL ||
	     StrStrI(strChk,"drop")!=NULL ||
	   StrStrI(strChk,"create")!=NULL ||
	     StrStrI(strChk,"exec")!=NULL ||
	    StrStrI(strChk,"alter")!=NULL ||
		 StrStrI(strChk,"like")!=NULL ||
		 StrStrI(strChk,"join")!=NULL ||
		StrStrI(strChk,"union")!=NULL ||
		StrStrI(strChk,"where")!=NULL ||
		  StrStrI(strChk,"and")!=NULL ||
		     StrStrI(strChk,"or")!=NULL)

	{
		return TRUE;
	}

	return FALSE;
}


BOOL isValidQueryString( char * str )
{
	WORD wCount = 0;
	while( *str != '\0' )
	{
		if( *str == '\'' )
			wCount++;
		str++;
	}
	return (wCount%2==0);
}

BOOL CheckMessageString(const char *strChk)
{
  	if(StrStrI(strChk,"select")!=NULL ||
	 StrStrI(strChk,"truncate")!=NULL || 
	   StrStrI(strChk,"delete")!=NULL || 
	   StrStrI(strChk,"insert")!=NULL ||
	   StrStrI(strChk,"update")!=NULL ||
	     StrStrI(strChk,"drop")!=NULL ||
	   StrStrI(strChk,"create")!=NULL ||
	     StrStrI(strChk,"exec")!=NULL ||
	    StrStrI(strChk,"alter")!=NULL ||
		 StrStrI(strChk,"like")!=NULL ||
		 StrStrI(strChk,"join")!=NULL ||
		StrStrI(strChk,"union")!=NULL ||
		StrStrI(strChk,"where")!=NULL ||
		  StrStrI(strChk,"and")!=NULL ||
		     StrStrI(strChk,"or")!=NULL)

	{
		return TRUE;
	}

	return FALSE;
}

DBMsgFunc g_DBMsgFunc[MaxQuery] =
{
	NULL,
	RUserIDXSendAndCharacterBaseInfo,
	RCreateCharacter,
	NULL,
	RDeleteCharacter,
	RCharacterNameCheck,
	NULL,
	NULL,
	RSearchWhisperUserAndSend, 
	NULL,						
	NULL,							
	NULL,							
	RFriendIsValidTarget,		
	RFriendAddFriend,
	RFriendIsValidTarget,
	RFriendDelFriend,
	RFriendDelFriendID,
	NULL,
	RFriendNotifyLogintoClient,
	RFriendGetFriendList,
	RFriendGetLoginFriends,
	RNoteIsNewNote,
	NULL,
	RNoteList,
	RNoteDelete,
	RWantedDelete,
	NULL,
	RGM_BanCharacter,
	RGM_UpdateUserLevel,			
	RGM_WhereIsCharacter,
	RGM_Login,
	RGM_GetGMPowerList,
	NULL,
	NULL,
	NULL,
	NULL,							
	RCheckGuildMasterLogin,			
	NULL,							
	NULL,							
	NULL,
	NULL,							
	RJackpotLoadTotalMoney,			
	RCheckGuildFieldWarMoney,		
	RAddGuildFieldWarMoney,			
	RChaseFindUser,					
	RGetCharacterSlotCount,			
	REventItemUse051108,			
	NULL,							
	RGM_UpdateUserState,			
	NULL,						
	NULL,							
	NULL,							
	NULL,							
	RPunishListLoad,			
	NULL,							
	RPunishCountAdd,				
	NULL,							
	RSafeLockUnLockCheck,		//[安全锁解锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	RSafeLockRestPwd,			//[安全锁修改密码][By:十里坡剑神][QQ:112582793][2018/4/30]
	RMarryNoticeLogin,			//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
};	

void UserIDXSendAndCharacterBaseInfo(DWORD UserIDX, DWORD AuthKey, DWORD dwConnectionIndex)
{
	g_DB.FreeQuery(eCharacterBaseQuery, dwConnectionIndex, "EXEC MP_CHARACTER_SelectByUserIDX %d, %d", UserIDX, AuthKey);
}

void CheatLog(DWORD CharacterIDX,DWORD CheatKind)
{
	char txt[256];
	sprintf(txt,"INSERT TB_LogHacking (Character_idx,CheatKind,LogDate) values(%d,%d,getdate())",
				CharacterIDX,CheatKind);
	g_DB.LogQuery(eQueryType_FreeQuery,0,0,txt);
}

void CreateCharacter(CHARACTERMAKEINFO* pMChar, WORD ServerNo, DWORD dwConnectionIndex)
{
	CHARACTERMAKEINFO* pMsg = pMChar;


	if( !isValidQueryString(pMsg->Name))
	{
	   return;
	}

	if( CheckString( pMsg->Name ) )
	{
		return;
	}

	char txt[512];

	WORD CheRyuk = 12;
	WORD SimMek = 12;
	WORD GenGol = 12;
	WORD MinChub = 12;
	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	int LoginPoint = 2017;	

	BYTE bStartMap = 17;
	LoginPoint = 2017;

	sprintf(txt, "EXEC %s %d, %d, %d, %d, %d, \'%s\', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %f, %f, \'%s\'", 
		"MP_CHARACTER_CreateCharacter", pMsg->UserID, GenGol, MinChub, CheRyuk, SimMek, 
		pMsg->Name,	pMsg->FaceType, pMsg->HairType, bStartMap, pMsg->SexType,
		pMsg->WearedItemIdx[eWearedItem_Hat], pMsg->WearedItemIdx[eWearedItem_Weapon], pMsg->WearedItemIdx[eWearedItem_Dress], 
		pMsg->WearedItemIdx[eWearedItem_Shoes], pMsg->WearedItemIdx[eWearedItem_Ring1], pMsg->WearedItemIdx[eWearedItem_Ring2],
		pMsg->WearedItemIdx[eWearedItem_Cape], pMsg->WearedItemIdx[eWearedItem_Necklace],pMsg->WearedItemIdx[eWearedItem_Armlet], pMsg->WearedItemIdx[eWearedItem_Belt],
		LoginPoint, ServerNo, pMsg->Height, pMsg->Width, ip);
		
	if(g_DB.Query(eQueryType_FreeQuery, eCreateCharacter, dwConnectionIndex, txt) == FALSE)
	{
	}
}

void CharacterNameCheck(char* pName, DWORD dwConnectionIndex)
{
	if( !isValidQueryString(pName))
	{
	   return;
	}

	if( CheckString( pName ) )
	{
		return;
	}

	g_DB.FreeQuery(eNewCharacterCheckName, dwConnectionIndex, "EXEC %s \'%s\'", "MP_CHARACTER_NameCheck", pName);
}

void LoginCheckDelete(DWORD UserID)//, DWORD dwConnectionIndex)
{

	static char txt[128];
	
//#ifdef _JAPAN_LOCAL_
//	sprintf(txt, "EXEC %s %d", "Up_GameLogOut_JP", UserID);
//#else
	sprintf(txt, "EXEC %s %d", "Up_GameLogOut", UserID);
//#endif
	
	g_DB.LoginQuery(101, eLoginCheckDelete, 0, txt);
}

void SetUserCurAndMaxCount(DWORD CurCount, DWORD MaxCount)
{
	g_DB.FreeQuery(eSetCurMaxCount, 0, "EXEC %s %d, %d", "SetUserLogCount", CurCount, MaxCount);
}
void DeleteCharacter(DWORD dwPlayerID, WORD ServerNo, DWORD dwConnectionIndex)
{
	USERINFO* pinfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!pinfo)
		return;
	if(pinfo->bIsLock)
	{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
		MSG_DWORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_NACK;
		msg.dwData = 4;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}
	CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pinfo->SelectInfoArray;
	
	for(int i = 0; i < MAX_CHARACTER_NUM; i++)
	{
		if(SelectInfoArray[i].dwCharacterID == dwPlayerID)
			break;
		if(i == MAX_CHARACTER_NUM - 1)	
			return;
	}
	
	char ip[16];
	WORD port;
	g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

	g_DB.FreeQuery(eDeleteCharacter, dwConnectionIndex, "EXEC %s %d, %d, \'%s\'", "MP_CHARACTER_DeleteCharacter", dwPlayerID, ServerNo, ip );
}

void SearchWhisperUserAndSend( DWORD dwPlayerID, char* CharacterName, DWORD dwKey )
{
	if( !isValidQueryString(CharacterName))
	{
	   return;
	}

	if( CheckString( CharacterName ) )
	{
		return;
	}

	g_DB.FreeQuery(eLoginMapInfoSearchForName, dwKey, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwPlayerID );
}

void SaveMapChangePointUpdate(DWORD CharacterIDX, WORD MapChangePoint_Idx)
{
	g_DB.FreeQuery(eSavePoint, CharacterIDX, "EXEC  %s %d, %d", "MP_CHARACTER_MapchangePointUpdate", CharacterIDX, MapChangePoint_Idx);
	
}

void UnRegistLoginMapInfo(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eUnRegistLoginMapInfo, CharacterIDX, "EXEC %s %d", "MP_LoginMapInfo_UnRegist", CharacterIDX);
}

void FriendGetUserIDXbyName(DWORD CharacterIDX, char* TargetName)
{
	if( !isValidQueryString(TargetName))
	{
	   return;
	}

	if( CheckString( TargetName ) )
	{
		return;
	}
	g_DB.FreeQuery(eFriendGetTargetMemeberIDX, CharacterIDX, "EXEC %s \'%s\', %u", "MP_FRIEND_GetTargetIDX", TargetName, CharacterIDX);
}

void FriendAddFriend(DWORD CharacterIDX, DWORD TargetID) 
{
	g_DB.FreeQuery(eFriendAdd, CharacterIDX, "EXEC %s %u, %u", "MP_FRIEND_AddFriend", CharacterIDX, TargetID);
}

void FriendIsValidTarget(DWORD CharacterIDX, DWORD TargetID, char* FromName)
{
	if( !isValidQueryString(FromName))
	{
	   return;
	}

	if( CheckString( FromName ) )
	{
		return;
	}
	g_DB.FreeQuery(eFriendIsValidTarget, CharacterIDX, "EXEC %s %d, %d, \'%s\'", "MP_FRIEND_IsValidTarget", CharacterIDX, TargetID, FromName);
}

void FriendDelFriend(DWORD CharacterIDX, char* TargetName)
{
	if( !isValidQueryString(TargetName))
	{
	   return;
	}

	if( CheckString(TargetName))
	{
		return;
	}
	g_DB.FreeQuery(eFriendDel, CharacterIDX, "EXEC %s %u, \'%s\'", "MP_FRIEND_DelFriend", CharacterIDX, TargetName);
}

void FriendDelFriendID(DWORD CharacterIDX, DWORD TargetID, DWORD bLast)
{
	g_DB.FreeQuery(eFriendDelID, CharacterIDX, "EXEC %s %u, %u, %d", "MP_FRIEND_DelFriendID", CharacterIDX, TargetID, bLast);
}

void FriendNotifyLogintoClient(DWORD CharacterIDX)
{

	g_DB.FreeQuery(eFriendNotifyLogin, CharacterIDX, "EXEC %s %u", "MP_FRIEND_NotifyLogin", CharacterIDX); //鲁陋赂娄 碌卯路脧脟脩 禄莽露梅碌茅
}

void FriendGetLoginFriends(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendGetLoginFriends, CharacterIDX, "EXEC %s %u", "MP_FRIEND_LoginFriend", CharacterIDX);//鲁禄掳隆 碌卯路脧脟脩 禄莽露梅碌茅
}

void FriendGetFriendList(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eFriendGetFriendList, CharacterIDX, "EXEC %s %u", "MP_FRIEND_GetFriendList", CharacterIDX);
}

void NoteIsNewNote(DWORD PlayerID)
{
	g_DB.FreeQuery(eNoteIsNewNote, PlayerID, "EXEC %s %u", "MP_NOTE_IsNewNote", PlayerID);
}

void NoteSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note)
{
	if( !isValidQueryString(FromName) || !isValidQueryString(ToName))
	{
	   return;
	}

	if( CheckString(FromName) || CheckString( ToName) || CheckMessageString(Note))
	{
		return;
	}

	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\'", "MP_NOTE_SendNote", ToName, FromName, Note);	
}

void NoteServerSendtoPlayer(DWORD FromIDX, char* FromName, char* ToName, char* Note)
{
	if( !isValidQueryString(FromName) || !isValidQueryString(ToName))
	{
	   return;
	}

	if( CheckString(FromName) || CheckString( ToName) || CheckMessageString(Note))
	{
		return;
	}
	g_DB.FreeLargeQuery(RNoteServerSendtoPlayer, FromIDX, "EXEC %s \'%s\', \'%s\', \'%s\'", "MP_NOTE_SendNote", ToName, FromName, Note);	
}

void NoteSendtoPlayerID(DWORD FromIDX, char* FromName, DWORD ToIDX, char* Note)
{
	if( !isValidQueryString(FromName))
	{
	   return;
	}

	if( CheckString(FromName) || CheckMessageString(Note))
	{
		return;
	}
	g_DB.FreeLargeQuery(RNoteSendtoPlayer, FromIDX, "EXEC %s %u, \'%s\', %u, \'%s\'", "MP_NOTE_SendNoteID", FromIDX, FromName, ToIDX, Note);
}

void NoteDelAll(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eNoteDelAll, 0, "EXEC %s %u", "MP_NOTE_DelAllNote", CharacterIDX);
}

void NoteList(DWORD CharacterIDX, WORD Page, WORD Mode)
{	
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(CharacterIDX);
	if(!userinfo)
		return;
	g_DB.FreeQuery(eNoteList, CharacterIDX, "EXEC %s %u, %d, %u, %d", "MP_NOTE_GetNoteList", CharacterIDX, 10, Page, Mode); //10掳鲁戮驴 鹿脼戮脝驴脗麓脵. 
}

void NoteRead(DWORD CharacterIDX, DWORD NoteIDX, DWORD IsFront)
{
	g_DB.FreeLargeQuery(RNoteRead, CharacterIDX, "EXEC %s %u, %u, %u", "MP_NOTE_ReadNote", CharacterIDX, NoteIDX, IsFront);
}

void NoteDelete(DWORD PlayerID, DWORD NoteID, BOOL bLast)
{	
	g_DB.FreeQuery(eNoteDelete, PlayerID, "EXEC %s %u, %u, %d", "MP_NOTE_DelNote", PlayerID, NoteID, bLast);
}

void WantedDelete(DWORD CharacterIDX)
{
	g_DB.FreeQuery(eWantedDelete, CharacterIDX, "EXEC %s %u", "MP_WANTED_Delete", CharacterIDX);
}

void WantedDelWantInfo(DWORD WantedIDX)
{
	g_DB.FreeQuery(eWantedDelWantInfo, 0, "EXEC %s %u", "MP_WANTED_DeleteWantedInfo", WantedIDX);
}

void GM_WhereIsCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	if( !isValidQueryString(CharacterName))
	{
	   return;
	}

	if( CheckString(CharacterName))
	{
		return;
	}
	g_DB.FreeQuery(eGM_WhereIsCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwSeacherID );
}

void GM_BanCharacter(DWORD dwID, char* CharacterName, DWORD dwSeacherID )
{
	if( !isValidQueryString(CharacterName))
	{
	   return;
	}

	if( CheckString(CharacterName))
	{
		return;
	}

	g_DB.FreeQuery(eGM_BanCharacter, dwID, "EXEC %s \'%s\', %d", "MP_LoginCharacterSearchForName", CharacterName, dwSeacherID );
}

void GM_UpdateUserLevel(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserLevel)
{
	if( !isValidQueryString(Charactername))
	{
	   return;
	}

	if( CheckString(Charactername))
	{
		return;
	}
//#ifdef _TL_LOCAL_
//	GM_UpdateUserState(dwID, dwServerGroup, Charactername, UserLevel);
//#elif _HK_LOCAL_
//	GM_UpdateUserState(dwID, dwServerGroup, Charactername, UserLevel);
//#else
    char txt[128];
	sprintf(txt, "EXEC %s  \'%s\', %d", "GMTool_UpdateUserLevel", Charactername, UserLevel);
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_UpdateUserLevel, dwID, txt);
//#endif
}

void GM_Login( DWORD dwConnectionIdx, char* strID, char* strPW, char* strIP )
{
	// 此处无法注入字符串不用过滤 !
	char txt[128];
	sprintf(txt, "EXEC RP_OperatorLoginCheck \'%s\', \'%s\', \'%s\'", strID, strPW, strIP);	
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_Login, dwConnectionIdx, txt);
}

void GM_GetGMPowerList( DWORD dwStartIdx, DWORD dwFlag )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", "RP_OperatorInfo", dwStartIdx, dwFlag );
	g_DB.LoginQuery(eQueryType_FreeQuery, eGM_GetGMPowerList, 0, txt);
}

void BillingRemainTimeUpdate(DWORD dwUserIdx, int nRemainTime )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d, %d", "MP_REMAINTIME_Update", dwUserIdx, nRemainTime );
	g_DB.LoginQuery(eQueryType_FreeQuery, eBilling_RemainTimeUpdate, 0, txt);
}

void BillingUpdateAll( WORD wAgentNum )
{
	char txt[128];
	sprintf(txt, "EXEC %s %d", "up_AgentLastTime", wAgentNum );
	g_DB.LoginQuery(eQueryType_FreeQuery, eBilling_UpdateAll, 0, txt );
}

void RUserIDXSendAndCharacterBaseInfo(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(atoi((char*)pData[0].Data[0]) == 0)
		count = 0;
	DWORD AgentAuthKey = atoi((char*)pData[0].Data[eCL_AuthKey]);
	USERINFO* pInfo = g_pUserTable->FindUser(pMessage->dwID);
	if(pInfo == NULL)		
		return;
	if(pInfo->dwUniqueConnectIdx != AgentAuthKey)	
		return;

	if(count > dMAXCHARACTER)
	{
		count = dMAXCHARACTER;
		char temp[256];
		sprintf(temp,"脛脡赂炉脜脥掳隆 5赂铆 ?脤禄贸?脤戮煤?陆!!! useridx : %d", pInfo->dwUserID );
		ASSERTMSG(0,temp);
	}


	SEND_CHARSELECT_INFO msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTERLIST_ACK;

	msg.eninit = *pInfo->crypto.GetEnKey(); 
	msg.deinit = *pInfo->crypto.GetDeKey();
	if( !count ) 
	{
		msg.CharNum = 0;	
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));
		pInfo->crypto.SetInit( TRUE );	
		
		return;
	}

	msg.CharNum = (BYTE)(count);


	for( DWORD i=0; i<count; i++)
	{
		msg.BaseObjectInfo[i].dwObjectID = atoi((char*)pData[i].Data[eCL_ObjectID]);
		msg.StandingArrayNum[i] = atoi((char*)pData[i].Data[eCL_StandIndex]);
		SafeStrCpy( msg.BaseObjectInfo[i].ObjectName, (char*)pData[i].Data[eCL_ObjectName], MAX_NAME_LENGTH+1 );
		msg.BaseObjectInfo[i].ObjectState = 0;
		msg.ChrTotalInfo[i].FaceType = atoi((char*)pData[i].Data[eCL_BodyType]);
		msg.ChrTotalInfo[i].HairType = atoi((char*)pData[i].Data[eCL_HeadType]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Hat] = atoi((char*)pData[i].Data[eCL_Hat]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Weapon] = atoi((char*)pData[i].Data[eCL_Weapon]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Dress] = atoi((char*)pData[i].Data[eCL_Dress]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Shoes] = atoi((char*)pData[i].Data[eCL_shoes]);
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring1] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Ring2] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Cape] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Necklace] = 0;

		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Armlet] = 0;
		msg.ChrTotalInfo[i].WearedItemIdx[eWearedItem_Belt] = 0;	
		msg.ChrTotalInfo[i].Level = atoi((char*)pData[i].Data[eCL_Grade]);
		msg.ChrTotalInfo[i].LoginMapNum = atoi((char*)pData[i].Data[eCL_Map]);
		msg.ChrTotalInfo[i].Gender = atoi((char*)pData[i].Data[eCL_Gender]);
		msg.ChrTotalInfo[i].Height = (float)atof((char*)pData[i].Data[eCL_Height]);
		msg.ChrTotalInfo[i].Width = (float)atof((char*)pData[i].Data[eCL_Width]);
		msg.ChrTotalInfo[i].Stage = atoi((char*)pData[i].Data[eCL_Stage]);
			
		pInfo->SelectInfoArray[i].dwCharacterID = msg.BaseObjectInfo[i].dwObjectID;
		pInfo->SelectInfoArray[i].Level = msg.ChrTotalInfo[i].Level;
		pInfo->SelectInfoArray[i].MapNum = msg.ChrTotalInfo[i].LoginMapNum;
		pInfo->SelectInfoArray[i].Gender = msg.ChrTotalInfo[i].Gender;
		SafeStrCpy( pInfo->SelectInfoArray[i].CharacterName, msg.BaseObjectInfo[i].ObjectName, MAX_NAME_LENGTH + 1);

    

	}	
	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(SEND_CHARSELECT_INFO));
	pInfo->crypto.SetInit( TRUE );		
}

void	RCreateCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	int result = atoi((char*)pData->Data[0]);
	DWORD dwConnectionIndex = pMessage->dwID;

	switch(result)
	{
	case 0:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Console.LOGEX(4,RGB(255,0,0),"获取链接信息失败");
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

			ASSERT(pInfo->dwUserID);

			if(pInfo->dwUserID == 0)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Console.LOGEX(4,RGB(255,0,0),"玩家账号propid错误");
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
			
			UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);
		}
		break;
	case 1:
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
			g_Console.LOGEX(4,RGB(255,0,0),"玩家账号角色满");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 2:
		{
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Console.LOGEX(4,RGB(255,0,0),"角色名重复");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 3:
		{
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Console.LOGEX(4,RGB(255,0,0),"玩家角色名和帮会名重复");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 4:
		{
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Console.LOGEX(4,RGB(255,0,0),"玩家账号propid错误");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 5:
		{
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Console.LOGEX(4,RGB(255,0,0),"空角色名");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case 6:
		{
			MSG_WORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
			msg.wData = result;
			g_Console.LOGEX(4,RGB(255,0,0),"角色名存在非法字符");
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	default:
		ASSERT(0);
		return;
	}
}
void	RCharacterNameCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[0])==0)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_ACK;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSG_WORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
		msg.wData = 2;
		g_Console.LOGEX(4,RGB(255,0,0),"角色名检查失败返回值[%d]",atoi((char*)pData->Data[0]));
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
}

void RSearchWhisperUserAndSend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{
		ASSERT(0);
	}
	else
	{
		MSG_CHAT* pMsgChat = g_MsgTable.GetMsg( pMessage->dwID );
		if( pMsgChat == NULL ) return;

		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMsgChat->dwObjectID );
		if( !pSenderInfo ) 
		{
			g_MsgTable.RemoveMsg( pMessage->dwID );
			return;
		}

		int nError	= atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	
		if( nLenEr == 1 && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_WHISPER_NACK;
			msg.dwObjectID	= pMsgChat->dwObjectID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwReceiverID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( dwReceiverID );
			
			if( pReceiverInfo )
			{
				if( pReceiverInfo->GameOption.bNoWhisper && pSenderInfo->UserLevel != eUSERLEVEL_GM )
				{
					MSG_BYTE msg;
					msg.Category	= MP_CHAT;
					msg.Protocol	= MP_CHAT_WHISPER_NACK;
					msg.dwObjectID	= pMsgChat->dwObjectID;	
					msg.bData		= CHATERR_OPTION_NOWHISPER;

					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					MSG_CHAT msgToSender = *pMsgChat;
					msgToSender.Category = MP_CHAT;
					msgToSender.Protocol = MP_CHAT_WHISPER_ACK;
					g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msgToSender, msgToSender.GetMsgLength() );	//CHATMSG 040324

					MSG_CHAT msgToReceiver = *pMsgChat;
					msgToReceiver.Category = MP_CHAT;
					if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
						msgToReceiver.Protocol = MP_CHAT_WHISPER_GM;
					else
						msgToReceiver.Protocol = MP_CHAT_WHISPER;
					SafeStrCpy( msgToReceiver.Name, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 );	//潞赂鲁陆禄莽露梅?脟 ?脤赂搂?赂路脦 鹿脵虏脼
					g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgToReceiver, msgToReceiver.GetMsgLength() );
				}
			}
			else
			{
				MSG_WHISPER msgWhisper;
				msgWhisper.Category		= MP_CHAT;
				if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
					msgWhisper.Protocol		= MP_CHAT_WHISPER_GM_SYN;
				else
					msgWhisper.Protocol		= MP_CHAT_WHISPER_SYN;

				msgWhisper.dwObjectID	= pMsgChat->dwObjectID;				
				msgWhisper.dwReceiverID	= (DWORD)atoi((char*)pData->Data[1]);	
				SafeStrCpy( msgWhisper.SenderName, (char*)pData->Data[0], MAX_NAME_LENGTH + 1 );
				SafeStrCpy( msgWhisper.ReceiverName, pMsgChat->Name, MAX_NAME_LENGTH + 1 );
				SafeStrCpy( msgWhisper.Msg, pMsgChat->Msg, MAX_CHAT_LENGTH + 1 );

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgWhisper, msgWhisper.GetMsgLength() );	
			}
		}
	}

	g_MsgTable.RemoveMsg( pMessage->dwID );
}

void RFriendDelFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_IsSuccess]) != 0)
		{
			MSG_NAME_DWORD msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_DEL_ACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwData = atoi((char*)pData->Data[eFr_IsSuccess]); 
			msg.dwObjectID = pMessage->dwID;

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_NAME_DWORD));
		}
		else
		{
			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category	= MP_FRIEND;
			msg.Protocol	= MP_FRIEND_DEL_NACK;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_targetname], MAX_NAME_LENGTH + 1 );
			msg.dwObjectID	= pMessage->dwID;
			msg.PlayerID	= atoi((char*)pData->Data[eFr_IsSuccess]);

			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
	}
}


void RFriendAddFriend(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	msg.Category = MP_FRIEND;
	SafeStrCpy( msg.Name, (char*)pData->Data[eFr_addToName], MAX_NAME_LENGTH + 1 );
	msg.PlayerID = atoi((char*)pData->Data[eFr_addToIDX]);
	msg.dwObjectID = atoi((char*)pData->Data[eFr_addFromIDX]);

	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_addFromIDX]));
	if(pRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_addFromErr]) == 0)
			msg.Protocol = MP_FRIEND_ADD_ACK;
		else
		{
			msg.PlayerID = atoi((char*)pData->Data[eFr_addFromErr]);
			msg.Protocol = MP_FRIEND_ADD_NACK;
		}
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}		
	else 
	{
		if(atoi((char*)pData->Data[eFr_addFromErr]) == 0)
			msg.Protocol = MP_FRIEND_ADD_ACK_TO_AGENT;
		else
		{
			msg.PlayerID = atoi((char*)pData->Data[eFr_addFromErr]);
			msg.Protocol = MP_FRIEND_ADD_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
	
	MSG_FRIEND_MEMBER_ADDDELETEID bmsg;
	bmsg.Category = MP_FRIEND;
	SafeStrCpy(bmsg.Name, (char*)pData->Data[eFr_addFromName], MAX_NAME_LENGTH+1);
	bmsg.dwObjectID = atoi((char*)pData->Data[eFr_addToIDX]);
	bmsg.PlayerID = atoi((char*)pData->Data[eFr_addFromIDX]);
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_addToIDX]));
	if(pToRecverInfo)
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0)
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
		else	
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
		}
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&bmsg, sizeof(bmsg));
	}
	else
	{
		if(atoi((char*)pData->Data[eFr_addToErr]) == 0)
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_TO_AGENT;
		else 
		{
			bmsg.PlayerID = atoi((char*)pData->Data[eFr_addToErr]);
			bmsg.Protocol = MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT;
		}
		g_Network.Broadcast2AgentServerExceptSelf((char*)&bmsg, sizeof(bmsg));
	}
}

void RFriendIsValidTarget(LPQUERY pData, LPDBMESSAGE pMessage)
{

	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	memset(&msg, 0, sizeof(msg));

	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[eFr_vtTargetid]));
	if(!pSenderInfo)
		return;

	if(atoi((char*)pData->Data[eFr_Err]) != 0)
	{		
		msg.Category = MP_FRIEND;
		msg.dwObjectID = pMessage->dwID;
		msg.Protocol = MP_FRIEND_ADD_NACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
		msg.PlayerID = atoi((char*)pData->Data[eFr_Err]);	

		g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		if(pRecverInfo)
		{
			if(pRecverInfo->GameOption.bNoFriend == TRUE)
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = pMessage->dwID;
				msg.Protocol = MP_FRIEND_ADD_NACK;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtToname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = eFriend_OptionNoFriend;

				g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
			else
			{
				msg.Category = MP_FRIEND;
				msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);
				msg.Protocol = MP_FRIEND_ADD_INVITE;
				SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
				msg.PlayerID = pMessage->dwID;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		else
		{
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_ADD_INVITE_TO_AGENT;
			SafeStrCpy( msg.Name, (char*)pData->Data[eFr_vtFromname], MAX_NAME_LENGTH + 1 );
			msg.PlayerID = pMessage->dwID;
			
			msg.dwObjectID = atoi((char*)pData->Data[eFr_vtTargetid]);

			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendDelFriendID(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pRecverInfo)
	{
		MSG_DWORD_WORD msg;
		msg.Category = MP_FRIEND;
		msg.Protocol = MP_FRIEND_DELID_ACK;
		msg.wData = atoi((char*)pData->Data[0]); 
		msg.dwData = atoi((char*)pData->Data[1]); 
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RFriendNotifyLogintoClient(LPQUERY pData, LPDBMESSAGE pMessage)
{
	MSG_NAME_DWORD msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_NOTIFY;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[eFr_LLoggedname], MAX_NAME_LENGTH + 1 );
	msg.dwData = pMessage->dwID;

	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData[i].Data[eFr_LTargetID]));
		msg.dwObjectID = atoi((char*)pData[i].Data[eFr_LTargetID]);
		if(pRecverInfo)
		{
			MSG_NAME_DWORD msgTemp = msg;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
		}
		else
		{
			msg.Protocol = MP_FRIEND_LOGIN_NOTIFY_TO_AGENT;
			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
	}
}

void RFriendGetLoginFriends(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_NAME msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LOGIN_FRIEND;
	msg.dwObjectID = pMessage->dwID;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		SafeStrCpy( msg.Name, (char*)pData[i].Data[eFr_LFFriendName], MAX_NAME_LENGTH + 1 );
		
		MSG_NAME msgTemp = msg;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp));
	}
}

void RFriendGetFriendList(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;	

	MSG_FRIEND_LIST_DLG msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_LIST_ACK;

	msg.count = pMessage->dwResult <= MAX_FRIEND_NUM ? (BYTE)pMessage->dwResult : MAX_FRIEND_NUM;
	
	if(pMessage->dwResult > MAX_FRIEND_NUM)
	{
		ASSERT(pMessage->dwResult <= MAX_FRIEND_NUM);
		msg.count = MAX_FRIEND_NUM;
	}

	for(DWORD i=0; i< msg.count; ++i)
	{
		msg.FriendList[i].Id = atol((char*)pData[i].Data[eFr_FLFriendid]);

		msg.FriendList[i].IsLoggIn = atoi((char*)pData[i].Data[eFr_FLIsLogin]);
		SafeStrCpy( msg.FriendList[i].Name, (char*)pData[i].Data[eFr_FLFriendname], MAX_NAME_LENGTH + 1 );
	}
	msg.dwObjectID = pMessage->dwID;
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
}

void RNoteIsNewNote(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;
	
	if(atoi((char*)pData->Data[0]) == 1)
	{
		MSGBASE msg;
		msg.Category = MP_NOTE;

		msg.Protocol = MP_NOTE_NEW_NOTE;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}

void RNoteSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) 
	{
		MSG_NAME msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_ACK;
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) 
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteServerSendtoPlayer(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	if(atoi((char*)pData->Data[eFr_NErr]) == 0) 
	{

	}
	else
	{
		MSG_NAME_WORD msg;
		msg.Category = MP_NOTE;
		msg.Protocol = MP_NOTE_SENDNOTE_NACK;
		msg.wData = atoi((char*)pData->Data[eFr_NErr]); // 2:invalid user, 3: full space
		SafeStrCpy( msg.Name, (char*)pData->Data[eFr_NToName], MAX_NAME_LENGTH + 1 );
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;

	}

	DWORD Toidx = atoi((char*)pData->Data[eFr_NToId]);
	if(Toidx == 0) 
		return;
	MSGBASE rmsg;
	rmsg.Category = MP_NOTE;
	rmsg.Protocol = MP_NOTE_RECEIVENOTE;
	rmsg.dwObjectID = Toidx;

	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(Toidx);
	if(pToRecverInfo)
	{
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)&rmsg, sizeof(rmsg));
	}
	else 
	{
		g_Network.Broadcast2AgentServerExceptSelf( (char*)&rmsg, sizeof(rmsg) );
	}
}

void RNoteList(LPQUERY pData, LPDBMESSAGE pMessage)
{	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	
	char tempDate[64];

	static MSG_FRIEND_NOTE_LIST msg;
	memset(&msg,0,sizeof(MSG_FRIEND_NOTE_LIST));
	
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_NOTELIST_ACK;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		SafeStrCpy( tempDate, (char*)pData[i].Data[eFr_NSentDate], 64 );

		char buf1[64] = { 0, };
		char buf2[64] = { 0, };
		strncpy( buf1, &tempDate[5], strlen(tempDate)-5);
		strcpy( tempDate, (char*)pData[i].Data[eFr_NSentTime] );
		strncpy( buf2, tempDate, strlen(tempDate)-3 );
		sprintf( msg.NoteList[i].SendDate, "%s - %s", buf1, buf2 );
				
		SafeStrCpy( msg.NoteList[i].FromName, (char*)pData[i].Data[eFr_NSender], MAX_NAME_LENGTH + 1 );
		
		msg.NoteList[i].NoteID = atoi((char*)pData[i].Data[eFr_NNoteID]);
		msg.NoteList[i].bIsRead = atoi((char*)pData[i].Data[eFr_NbIsRead]);
		msg.dwObjectID = pMessage->dwID; 
		
	}
	msg.TotalPage = atoi((char*)pData[0].Data[eFr_NTotalpage]);
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	
}

void RNoteRead(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_FRIEND_READ_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_READNOTE_ACK;
	SafeStrCpy( msg.FromName, (char*)pData->Data[eFr_NRNSender], MAX_NAME_LENGTH + 1 );
	SafeStrCpy( msg.Note, (char*)pData->Data[eFr_NRNNote], MAX_NOTE_LENGTH + 1 );
	msg.NoteID = atoi((char*)pData->Data[eFr_NRNNoteID]);
	msg.ItemIdx = atoi((char*)pData->Data[eFr_NRNItemIdx]);
	
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, msg.GetMsgLength());
}

void RNoteDelete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(!pRecverInfo)
		return;

	MSG_FRIEND_DEL_NOTE msg;
	msg.Category = MP_NOTE;
	msg.Protocol = MP_NOTE_DELNOTE_ACK;
	msg.bLast = atoi((char*)pData->Data[eFr_NdbLast]);
	msg.NoteID 	= atoi((char*)pData->Data[eFr_NdNoteID]);
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}

void RDeleteCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
	if( !pInfo )			return;

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;

	if(atoi((char*)pData->Data[0]) != 0)
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_NACK;
		msg.dwData = atoi((char*)pData->Data[0]);
	}
	else
	{
		msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_ACK;
		DWORD CharacterIdx = atoi((char*)pData->Data[1]);
		WantedDelete( CharacterIdx );

		for(int i=0; i<MAX_CHARACTER_NUM; ++i)
		{
			if( pInfo->SelectInfoArray[i].dwCharacterID == CharacterIdx )
				memset( &pInfo->SelectInfoArray[i], 0, sizeof(CHARSELECTINFO) );
		}

		GuildTraineeDelete(CharacterIdx);

		MSG_DWORD msg2;
		ZeroMemory( &msg2, sizeof(MSG_DWORD) );
		SetProtocol( &msg2, MP_GUILD, MP_GUILD_MUNHA_USERDELETE_SYN );
		msg2.dwObjectID = CharacterIdx;
		msg2.dwData = CharacterIdx;
		g_Network.Broadcast2MapServer((char*)&msg2, sizeof(msg2));
	}

	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
}

void RWantedDelete(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(atoi((char*)pData->Data[0]) == 0) 
		return;
	//FOR SERVER TO SERVER
	MSGBASE msg;
	msg.Category = MP_WANTED;
	msg.Protocol = MP_WANTED_DELETE_BY_DELETECHR;
	msg.dwObjectID = atoi((char*)pData[0].Data[0]);  //wanted_idx 
	g_Network.Broadcast2MapServer((char*)&msg, sizeof(msg));

	//FOR CLIENT TO SERVER
	USERINFO* pUserInfo = NULL;
	for(DWORD i=0; i<pMessage->dwResult; ++i)
	{
		MSG_WORD msg;
		msg.Category = MP_WANTED;
		msg.dwObjectID = atoi((char*)pData[i].Data[1]); //character_idx
		msg.wData = atoi((char*)pData[0].Data[0]); //wanted_idx 

		pUserInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData->Data[1]));
		if(!pUserInfo)
		{
			//broadcasting to another agent
			msg.Protocol = MP_WANTED_NOTCOMPLETE_TO_AGENT;
			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
		}
		else
		{			
			msg.Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR;
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
	}
	if(pMessage->dwResult < MAX_QUERY_RESULT)
	{
		WantedDelWantInfo(atoi((char*)pData[0].Data[0]));
	}
	else
	{
	}
}

//---for GM_Tool
void RGM_WhereIsCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_WHEREIS_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID	= (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMapNum		= (DWORD)atoi((char*)pData->Data[2]);

			if( dwMapNum > 0 )
			{
				MSG_DWORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
				msg.dwObjectID	= pMessage->dwID;
				msg.dwData		= dwTargetID;	

				WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMapNum );
				SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );				

				if( pInfo )
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				MSG_WORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_ACK;
				msg.dwObjectID	= pMessage->dwID;
				msg.wData		= (WORD)dwMapNum;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_BanCharacter(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	if(!count)
	{

	}
	else
	{
		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( !pSenderInfo ) return;

		int nError = atoi((char*)pData->Data[0]);
		int nLenEr	= strlen((char*)pData->Data[0]);	
		if( nLenEr && ( nError == CHATERR_NO_NAME || nError == CHATERR_NOT_CONNECTED ) )
		{
			MSG_BYTE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANCHARACTER_NACK;
			msg.dwObjectID	= pMessage->dwID;
			msg.bData		= nError;
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		else
		{
			DWORD dwTargetID = (DWORD)atoi((char*)pData->Data[1]);
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( dwTargetID );

			if( pTargetInfo )
			{
				DisconnectUser( pTargetInfo->dwConnectionIndex );

				MSGBASE msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
				msg.dwObjectID	= pMessage->dwID;
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				LogGMToolUse( pMessage->dwID, eGMLog_Disconnect_User, MP_CHEAT_BANCHARACTER_ACK, dwTargetID, 0 );
			}
			else
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
				msg.dwData1		= dwTargetID;
				msg.dwData2		= pMessage->dwID;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
			}
		}
	}
}

void RGM_UpdateUserLevel(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
		DWORD useridx = atoi((char*)pData->Data[1]);
		DWORD state = atoi((char*)pData->Data[2]);

		LogGMToolUse( pMessage->dwID, eGMLog_Block, useridx, state, 0 );
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}


void RGM_Login(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD count = pMessage->dwResult;
	DWORD dwConnectionIndex = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );

	if( count == 0 || pUserInfo == NULL )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 0;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}
	WORD check = atoi((char*)pData[0].Data[0]);

	if( check != 0 )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 1;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;
	}

	int nPower = atoi((char*)pData[0].Data[5]);

	if( nPower < 0 || nPower >= eGM_POWER_MAX )
	{
		MSG_BYTE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_GM_LOGIN_NACK;
		msg.bData		= 2;

		g_Network.Send2User( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		return;		
	}

	DWORD dwIdx = (DWORD)atoi((char*)pData[0].Data[1]);
	char szName[MAX_NAME_LENGTH+1];

	SafeStrCpy( szName, (char*)pData[0].Data[2], MAX_NAME_LENGTH+1 );

	GMINFO->AddGMList( dwConnectionIndex, nPower, dwIdx, szName );

	MSG_DWORD Msg;
	Msg.Category	= MP_CHEAT;
	Msg.Protocol	= MP_CHEAT_GM_LOGIN_ACK;
	Msg.dwData		= nPower;

	g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
}


void RGM_GetGMPowerList(LPQUERY pData, LPDBMESSAGE pMessage)
{

}

void CheckGuildMasterLogin( DWORD dwConnectionIdx, DWORD dwPlayerIdx, char* pSearchName, DWORD dwMoney, BYTE Protocol )
{
	g_DB.FreeQuery( eCheckGuildMasterLogin, dwConnectionIdx, "EXEC MP_GUILDFIELDWAR_CheckMasterLogin %d, \'%s\', %d, %d",
		dwPlayerIdx, pSearchName, dwMoney, Protocol );
}

void RCheckGuildMasterLogin( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:	
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMasterIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[3]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[4]);
			BYTE Protocol = (BYTE)atoi((char*)pData->Data[5]);

			MSG_DWORD3 Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = Protocol;
			Msg.dwData1 = dwSenderIdx;
			Msg.dwData2 = dwMasterIdx;
			Msg.dwData3 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( !wPort )	return;
			SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
			if( !pSInfo )	return;

			if( userinfo->dwMapServerConnectionIndex == pSInfo->dwConnectionIndex )
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
			{
				g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
		}
		break;
	case 1:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 3:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 4:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = nFlag;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	}
}

void CheckGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwSenderIdx, DWORD dwEnemyGuildIdx, DWORD dwMoney )
{
	g_DB.FreeQuery( eCheckGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_CheckMoney %d, %d, %d",
		dwSenderIdx, dwEnemyGuildIdx, dwMoney );
}

void RCheckGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:		
		{
			DWORD dwSenderIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwEnemyGuildIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_DECLARE_ACCEPT;
			Msg.dwObjectID = dwSenderIdx;
			Msg.dwData1 = dwEnemyGuildIdx;
			Msg.dwData2 = dwMoney;

			USERINFO* userinfo = (USERINFO*)g_pUserTable->FindUser( dwConnectionIndex );
			if( userinfo == NULL )	return;
			g_Network.Send2Server( userinfo->dwMapServerConnectionIndex, (char*)&Msg, sizeof(Msg) );
		}
		break;
	case 1:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	case 2:		
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = 5;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_FIELDWAR;
			Msg1.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 6;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	case 3:	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = 6;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );

			DWORD dwReceiverIdx = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[2]);

			MSG_BYTE Msg1;
			Msg1.Category = MP_GUILD_FIELDWAR;
			Msg1.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg1.dwObjectID = dwReceiverIdx;
			Msg1.bData = 5;			

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg1, sizeof(Msg1) );
			}
		}
		break;
	}
}

void AddGuildFieldWarMoney( DWORD dwConnectionIndex, DWORD dwPlayerIdx, DWORD dwMoney )
{
	g_DB.FreeQuery( eAddGuildFieldWarMoney, dwConnectionIndex, "EXEC dbo.MP_GUILDFIELDWAR_AddMoney %d, %d",
		dwPlayerIdx, dwMoney );
}

void RAddGuildFieldWarMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIndex = pMessage->dwID;
	int nFlag = atoi( (char*)pData->Data[0] );

	switch( nFlag )
	{
	case 0:	
		{
			DWORD dwMap = (DWORD)atoi((char*)pData->Data[1]);
			DWORD dwPlayerIdx = (DWORD)atoi((char*)pData->Data[2]);
			DWORD dwMoney = (DWORD)atoi((char*)pData->Data[3]);

			MSG_DWORD2 Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_ADDMONEY_TOMAP;
			Msg.dwData1 = dwPlayerIdx;
			Msg.dwData2 = dwMoney;

			WORD wPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)dwMap );
			if( wPort )
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer( wPort );
				g_Network.Send2Server( pSInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
		}
		break;

	case 1:	
		{
			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_NACK;
			Msg.bData = 4;
			g_Network.Send2User( dwConnectionIndex, (char*)&Msg, sizeof(Msg) );			
		}
		break;
	}
}

void JackpotLoadTotalMoney()
{
	char txt[128];
	sprintf( txt, "EXEC dbo.MP_JACKPOT_LoadMoney" );
	g_DB.Query( eQueryType_FreeQuery, eJackpot_TotalMoney_Load, 0, txt );
}

void RJackpotLoadTotalMoney( LPQUERY pData, LPDBMESSAGE pMessage )
{	
	if( pMessage->dwResult == 0 ) return;

	DWORD TotalMoney = (DWORD)atoi((char*)pData->Data[0]);
	JACKPOTMGR->SetTotalMoney( TotalMoney );
	JACKPOTMGR->SendMsgUserTotalMoney();
}

void EventItemUse050106( DWORD dwUserIdx )
{
	char txt[128];
	sprintf( txt, "EXEC up_event_insert %d", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050106, 0, txt);
}

void EventItemUse050203( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	if( !isValidQueryString(sCharName))
	{
	   return;
	}

	if( CheckString(sCharName))
	{
		return;
	}
	char txt[128];
	sprintf( txt, "EXEC up_event0203_Ginsert %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050106, 0, txt);
}

void EventItemUse050322( DWORD dwUserIdx )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.up_event0322_insert %d", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse050322, 0, txt);
}

void EventItemUse051108( DWORD dwObjectIdx, DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	if( !isValidQueryString(sCharName))
	{
	   return;
	}

	if( CheckString(sCharName))
	{
		return;
	}
	char txt[128];
	sprintf( txt, "EXEC up_event051108_insert %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse051108, dwObjectIdx, txt);
}

void REventItemUse051108( LPQUERY pData, LPDBMESSAGE pMessage )
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSG_DWORD msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_EVENTITEM_USE;
	msg.dwData = (DWORD)atoi((char*)pData->Data[0]);

	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}

void EventItemUse2( DWORD dwUserIdx, char* sCharName, DWORD dwServerNum )
{
	if( !isValidQueryString(sCharName))
	{
	   return;
	}

	if( CheckString(sCharName))
	{
		return;
	}
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_EventItemUse2 %d, \'%s\', %d", dwUserIdx, sCharName, dwServerNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eEventItemUse2, 0, txt);
}

void InsertBillingTable( DWORD dwUserIdx, DWORD dwBillType, DWORD dwAgentNo, DWORD dwServerSetNum )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_InsertBillingTable %d, %d, %d, %d", dwUserIdx, dwBillType, dwAgentNo, dwServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eInsertBillingTable, 0, txt);
}

void DeleteBillingTable( DWORD dwUserIdx, DWORD dwServerSetNum )
{
	char txt[128];
	sprintf( txt, "EXEC dbo.Up_DeleteBillingTable %d, %d", dwUserIdx, dwServerSetNum );
	g_DB.LoginQuery( eQueryType_FreeQuery, eDeleteBillingTable, 0, txt);
}


void ChaseFindUser( DWORD UserIdx, char* WantedName, DWORD dwItemIdx )
{	
	if( !isValidQueryString(WantedName))
	{
	   return;
	}

	if( CheckString(WantedName))
	{
		return;
	}
	g_DB.FreeQuery(eChaseFindUser, UserIdx, "EXEC %s \'%s\', %d", "MP_character_name_chk", WantedName, dwItemIdx );
}
void RChaseFindUser( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(atoi((char*)pData->Data[0]) == 0)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
		msg.dwData = 1;
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( pInfo )
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else if(atoi((char*)pData->Data[0]) == 1)
	{
		MSG_DWORD msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
		msg.dwData = 3;
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
		if( pInfo )
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		DWORD CharacterIdx = atoi((char*)pData->Data[0]);
		DWORD MapNum = atoi((char*)pData->Data[1]);
		DWORD ItemIdx = atoi((char*)pData->Data[2]);

		SERVERINFO* pInfo = g_pServerTable->FindMapServer( (WORD)MapNum );
		if( pInfo )
		{
			MSG_DWORD3 msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_CHASE_SYN;
			msg.dwData1 = pMessage->dwID;
			msg.dwData2 = CharacterIdx;
			msg.dwData3 = ItemIdx;

			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
	}
}



void GetCharacterSlotCount( DWORD UserIdx, DWORD AuthKey, DWORD dwConnectionIndex )
{
	g_DB.FreeQuery(eJP_CharacterSlot, dwConnectionIndex, "EXEC MP_CHARACTER_GetCharacterSlot_JP %d, %d", UserIdx, AuthKey);
}

void RGetCharacterSlotCount( LPQUERY pData, LPDBMESSAGE pMessage )
{
//#ifdef _JAPAN_LOCAL_
//	
//	DWORD AgentAuthKey = atoi((char*)pData->Data[0]);
//	DWORD SlotCount = atoi((char*)pData->Data[1]);
//	
//	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
//	if( !pInfo )		return;
//	if( pInfo->dwUniqueConnectIdx != AgentAuthKey )		return;
//
//	pInfo->dwExtraUserSlot = SlotCount;
//
//	MSG_DWORD msg;
//	SetProtocol( &msg, MP_USERCONN, MP_USERCONN_CHARACTERSLOT );
//	msg.dwData = SlotCount;
//	g_Network.Send2User( pMessage->dwID, (char*)&msg, sizeof(msg) );
//
//#endif	// _JAPAN_LOCAL_
//
//#ifdef _HK_LOCAL_
//	
//	DWORD AgentAuthKey = atoi((char*)pData->Data[0]);
//	DWORD SlotCount = atoi((char*)pData->Data[1]);
//	
//	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
//	if( !pInfo )		return;
//	if( pInfo->dwUniqueConnectIdx != AgentAuthKey )		return;
//
//	pInfo->dwExtraUserSlot = SlotCount;
//
//	MSG_DWORD msg;
//	SetProtocol( &msg, MP_USERCONN, MP_USERCONN_CHARACTERSLOT );
//	msg.dwData = SlotCount;
//	g_Network.Send2User( pMessage->dwID, (char*)&msg, sizeof(msg) );
//
//#endif	// _HK_LOCAL_
//#ifdef _TL_LOCAL_
//	
//	DWORD AgentAuthKey = atoi((char*)pData->Data[0]);
//	DWORD SlotCount = atoi((char*)pData->Data[1]);
//	
//	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
//	if( !pInfo )		return;
//	if( pInfo->dwUniqueConnectIdx != AgentAuthKey )		return;
//
//	//
//	pInfo->dwExtraUserSlot = SlotCount;
//
//	MSG_DWORD msg;
//	SetProtocol( &msg, MP_USERCONN, MP_USERCONN_CHARACTERSLOT );
//	msg.dwData = SlotCount;
//	g_Network.Send2User( pMessage->dwID, (char*)&msg, sizeof(msg) );
//
//#endif	// _HK_LOCAL_
}

void GM_UpdateUserState(DWORD dwID, DWORD dwServerGroup, char* Charactername, BYTE UserState)
{
	if( !isValidQueryString(Charactername))
	{
	   return;
	}

	if( CheckString(Charactername))
	{
		return;
	}
	g_DB.FreeQuery( eGM_UpdateUserState, dwID, "EXEC dbo.MP_GMTOOL_UpdateUserState %d, \'%s\', %d", dwServerGroup, Charactername, UserState );
}

void RGM_UpdateUserState(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pMessage->dwID );
	if( !pUserInfo ) return;

	MSGBASE msg;
	msg.Category	= MP_CHEAT;
	msg.dwObjectID	= pMessage->dwID;

	if(atoi((char*)pData->Data[0])==0)
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_NACK;
	}
	else
	{
		msg.Protocol = MP_CHEAT_BLOCKCHARACTER_ACK;
	}
	g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

}

void GuildTraineeDelete(DWORD UserIdx) 
{
	g_DB.FreeQuery(eGuildTraineeDelete, 0, "EXEC %s %d", "MP_GUILD_TRAINEE_Delete", UserIdx);
}

void LogGMToolUse( DWORD CharacterIdx, DWORD GMLogtype, DWORD Logkind, DWORD Param1, DWORD Param2 )
{
	char txt[128] = { 0, };
	sprintf( txt, "EXEC dbo.Up_GMToolUseLog %d, %d, %d, %d, %d",
		CharacterIdx,
		GMLogtype,
		Logkind,
		Param1,
		Param2
	);
	g_DB.LogQuery( eQueryType_FreeQuery, eLogGMToolUse, 0, txt );
}

void PunishListLoad( DWORD dwUserIdx )
{
	char txt[128] = {0,};
	sprintf(txt, "EXEC dbo.MP_PUNISHLIST_LOAD %u", dwUserIdx );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishListLoad, dwUserIdx, txt);
}

void RPunishListLoad( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pMessage->dwID );
	if( pUserInfo == NULL ) return;

	for( DWORD i = 0; i < pMessage->dwResult; ++i )
	{
		int nPunishTime = atoi( (char*)pData[i].Data[1] );
		if( nPunishTime > 0 ) 
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, atoi((char*)pData[i].Data[0]), nPunishTime );

	}
}

void PunishListAdd( DWORD dwUserIdx, int nPunishKind, DWORD dwPunishTime )
{
	char txt[128] = {0,};
	sprintf(txt, "EXEC dbo.MP_PUNISHLIST_ADD %u, %d, %u", dwUserIdx, nPunishKind, dwPunishTime );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishListAdd, dwUserIdx, txt);
}

void PunishCountAdd( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, int nPunishKind, int nPeriodDay, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName )
{

	if( !isValidQueryString(pAutoCharacterName) || !isValidQueryString(pAskCharacterName))
	{
	   return;
	}

	if( CheckString(pAutoCharacterName)|| CheckString(pAskCharacterName))
	{
		return;
	}
	char txt[128] = {0,};
	sprintf(txt, "EXEC dbo.MP_PUNISHCOUNT_ADD %u, %u, \'%s\', %d, %d, %u, %u, \'%s\'", dwAutoUserIdx, dwAutoCharacterIdx, pAutoCharacterName, nPunishKind, nPeriodDay, dwAskUserIdx, dwAskCharacterIdx, pAskCharacterName );
	g_DB.LoginQuery( eQueryType_FreeQuery, ePunishCountAdd, dwAutoUserIdx, txt);
}

void RPunishCountAdd( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if( 0 == pMessage->dwResult )
		return;

	int nPunishKind	= atoi((char*)pData[0].Data[0]);
	int nCount = atoi((char*)pData[0].Data[1]);
	
	if( nCount <= 1 ) 
		return;

	DWORD dwAutoUserIdx = (DWORD)atoi((char*)pData[0].Data[2]);
	DWORD dwAutoCharacterIdx = (DWORD)atoi((char*)pData[0].Data[3]);
	char AutoCharacterName[MAX_NAME_LENGTH+1] = {0,};
	SafeStrCpy( AutoCharacterName, (char*)pData[0].Data[4], MAX_NAME_LENGTH+1 );
	DWORD dwAskUserIdx = (DWORD)atoi((char*)pData[0].Data[5]);
	DWORD dwAskCharacterIdx = (DWORD)atoi((char*)pData[0].Data[6]);
	char AskCharacterName[MAX_NAME_LENGTH+1] = {0,};
	SafeStrCpy( AskCharacterName, (char*)pData[0].Data[7], MAX_NAME_LENGTH+1 );

	switch( nPunishKind )
	{
	case ePunishCount_AutoUser:
		{
			DWORD dwBlockTime = (DWORD)POW((double)2,(int)( nCount-2)) * 1 * 60 * PUNISHMGR->GetAutoBlockTime();
			PunishListAdd( pMessage->dwID, ePunish_Login, dwBlockTime );

			InsertLog_AutoBlock( dwAutoUserIdx, dwAutoCharacterIdx, AutoCharacterName, dwBlockTime, dwAskUserIdx, dwAskCharacterIdx, AskCharacterName, g_nServerSetNum );

		}
		break;
	};
}

void InsertLog_AutoBlock( DWORD dwAutoUserIdx, DWORD dwAutoCharacterIdx, char* pAutoCharacterName, DWORD dwBlockTime, DWORD dwAskUserIdx, DWORD dwAskCharacterIdx, char* pAskCharacterName, int nServerSet )
{
	if( !isValidQueryString(pAutoCharacterName) || !isValidQueryString(pAskCharacterName))
	{
	   return;
	}

	if( CheckString(pAutoCharacterName)|| CheckString(pAskCharacterName))
	{
		return;
	}
	char txt[128] = {0,};
	sprintf(txt, "EXEC dbo.MP_INSERTLOG_AUTOBLOCK %u, %u, \'%s\', %d, %u, %u, \'%s\', %d", dwAutoUserIdx, dwAutoCharacterIdx, pAutoCharacterName, dwBlockTime, dwAskUserIdx, dwAskCharacterIdx, pAskCharacterName, nServerSet );
	g_DB.LoginQuery( eQueryType_FreeQuery, eInsertLog_AutoBlock, dwAutoUserIdx, txt);
}

void TopListCheck(DWORD dwConnectionIndex,char * msg)
{
	g_DB.FreeLargeQuery(RTopListCheck,dwConnectionIndex, msg);
}
void RTopListCheck(LPLARGEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD	dwConnectionIdx = pMessage->dwID;
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);
	if(!pUserInfo) return;
	if (pMessage->dwResult!=0)
	{
		for(int i = 0;i<pMessage->dwResult;i++)
		{
			TOPLISTCHECK msg;
			msg.Category = MP_TOPLIST;
			msg.Protocol = MP_TOPLIST_ACK;
			SafeStrCpy(msg.Msg,(char*)pData->Data[i],sizeof(msg.Msg));
			g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		}
	}
}
void RestSingedInfo()
{//[重置签到系统数据库信息][By:十里坡剑神][QQ:112582793][2018/1/7]
	char txt[128] = {0,};
	sprintf(txt, "EXEC  %s","dbo.MP_SingedInfo_Rest");
	g_DB.FreeQuery(eQueryType_FreeQuery,0, txt);
}
void SafeLockUnLockCheck(DWORD dwConnectionIndex,DWORD propid,char* pinpwd)
{//[安全锁解锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	if( !isValidQueryString(pinpwd) )
	{
		return;
	}
	if( CheckString(pinpwd))
	{
		return;
	}
	char txt[128] = {0,};
	sprintf(txt, "EXEC  %s %d,\'%s\'","dbo.up_UnLockSafeLock",propid,pinpwd);
	g_DB.FreeQuery(eSafeLockCheck, dwConnectionIndex,txt );
}
void RSafeLockUnLockCheck(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
	if( !pInfo )			return;
	if(pMessage->dwResult==0)return;

	DWORD UserIdx = atoi((char*)pData->Data[0]);
	DWORD  IsSafeLock = atoi((char*)pData->Data[1]);
	
	if(IsSafeLock==1)
	{//[解锁失败][By:十里坡剑神][QQ:112582793][2018/4/30]
		MSG_DWORD2 msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_UNLOCK_NACK;
		msg.dwData1 = 1;
		g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
	}
	else
	{
		//[解锁成功通知map][By:十里坡剑神][QQ:112582793][2018/4/30]
		MSGBASE msg;
		msg.Category = MP_SAFELOCK;
		msg.Protocol = MP_SAFELOCK_UNLOCK_AGTOMAP_SYN;
		msg.dwObjectID = pInfo->dwCharacterID;
		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)&msg,sizeof(msg));
		pInfo->bIsLock = FALSE;
	}
}
void SafeLockRestPwd(DWORD dwConnectionIndex,DWORD propid,char* NewPinPwd,char * PinPwd)
{//[安全锁修改密码][By:十里坡剑神][QQ:112582793][2018/4/30]
	if( !isValidQueryString(PinPwd)||!isValidQueryString(NewPinPwd) )
	{
		return;
	}
	if( CheckString(PinPwd)||CheckString(NewPinPwd))
	{
		return;
	}
	char txt[128] = {0,};
	sprintf(txt, "EXEC  %s %d,'%s','%s'","dbo.up_RestSafeLockPwd",propid,NewPinPwd,PinPwd);
	g_DB.FreeQuery(eSafeLockRestPwd, dwConnectionIndex,txt );
}
void RSafeLockRestPwd(LPQUERY pData, LPDBMESSAGE pMessage)
{
	USERINFO* pInfo = g_pUserTable->FindUser( pMessage->dwID );
	if( !pInfo )			return;
	if(pMessage->dwResult==0)return;
	DWORD code = atoi((char*)pData->Data[0]);
	MSG_DWORD2 msg;
	msg.Category = MP_SAFELOCK;
	if(code==1||code==2)
	{
		msg.Protocol = MP_SAFELOCK_RESTPWD_NACK;
	}
	else
		msg.Protocol = MP_SAFELOCK_RESTPWD_ACK;
	msg.dwData1 = code;
	g_Network.Send2User(pMessage->dwID, (char*)&msg, sizeof(msg));
}

//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
void MarryNoticeLogin(DWORD characterIDX)
{
	g_DB.FreeQuery(eMarryLoginNotice, characterIDX, "EXEC %s %u", "MP_Marry_NotifyLogin", characterIDX); 
}
void RMarryNoticeLogin(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult==0) return;
	MSG_NAME_DWORD msg;
	msg.Category = MP_MARRY;
	msg.Protocol = MP_MARRY_NOTICE_LOGIN_ACK;
	SafeStrCpy( msg.Name, (char*)pData[0].Data[1], MAX_NAME_LENGTH + 1 );
	msg.dwData = pMessage->dwID;
	
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(atoi((char*)pData[0].Data[0]));
	msg.dwObjectID = atoi((char*)pData[0].Data[0]);
	if(pRecverInfo)
	{
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		msg.Protocol = MP_MARRY_NOTICE_LOGIN_TO_AGENT;
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
	}

	MSGBASE msg1;
	msg1.Category = MP_MARRY;
	msg1.Protocol = MP_MARRY_NOTICE_LOGIN_SELF;
	msg1.dwObjectID = atoi((char*)pData[0].Data[0]);
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pMessage->dwID);
	if(pSenderInfo)
	{
		g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg1, sizeof(msg1));
	}
}

