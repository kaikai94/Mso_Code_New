




#include "stdafx.h"
#include "AgentNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "AgentDBMsgParser.h"
#include "MsgTable.h"

#include "FilteringTable.h"
#include "BootManager.h"
#include "MHTimeManager.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

#include "JackpotManager_Agent.h"
#include "BobusangManager_Agent.h"
#include "SkillDalayManager.h"
#include "TrafficLog.h"
#include "PunishManager.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;
extern int g_nServerSetNum;
extern HWND g_hWnd;
extern BOOL g_bCloseWindow;
DWORD g_dwAcceptUser2 = 0;
extern BOOL g_bCloseBackDoor;
DWORD g_dwMaxUser = 4000;
#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"

extern CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
}

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
void MP_AGENTSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
}
void UserConn_NotifyLoginSyn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD3BYTE2* pmsg = (MSG_DWORD3BYTE2*)pMsg;
	DWORD UserID = pmsg->dwObjectID;
	DWORD DistAuthKey = pmsg->dwData1;
	DWORD DistConnectionIndex = pmsg->dwData2;
	int nTime = (int)pmsg->dwData3;
	BYTE UserLevel = pmsg->bData1;
	BYTE bType = pmsg->bData2;

	if (g_bReady == FALSE)
	{
		MSG_DWORD3 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_NOTIFY_USERLOGIN_NACK;
		msg.dwObjectID = UserID;
		msg.dwData1 = DistAuthKey;
		msg.dwData2 = LOGIN_ERROR_NOAGENTSERVER;
		msg.dwData3 = DistConnectionIndex;
		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	USERINFO* pPreInfo = g_pUserTableForUserID->FindUser(UserID);
	if (pPreInfo)
	{
		char temp[128];
		sprintf(temp, "[ERROR]: NOTIFY_USERLOGIN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, UserID);
		g_Console.LOG(4, temp);

		if (pPreInfo->dwConnectionIndex)
		{
			DWORD dwPreConIdx = pPreInfo->dwConnectionIndex;
			OnDisconnectUser(dwPreConIdx);
			DisconnectUser(dwPreConIdx);
		}
		else
		{
			g_pUserTableForUserID->RemoveUser(UserID);
			memset(pPreInfo, 0, sizeof(USERINFO));
			g_UserInfoPool.Free(pPreInfo);
		}
	}

	USERINFO* pInfo = g_UserInfoPool.Alloc();
	if (pInfo == NULL)
	{
		MSGBASE msg;
		msg.Category = MP_SERVER;
		msg.Protocol = MP_SERVER_AGENTDISABLE;

		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));

		g_Console.LOG(4, "Agent Disable : Can't accept users!!");
		return;
	}

	memset(pInfo, 0, sizeof(USERINFO));

	pInfo->dwUserID = UserID;
	pInfo->dwLastConnectionCheckTime = gCurTime;
	pInfo->dwUniqueConnectIdx = g_pServerSystem->MakeAuthKey();
	pInfo->DistAuthKey = DistAuthKey;
	pInfo->UserLevel = UserLevel;
	//SafeStrCpy(pInfo->Mac,pmsg->Mac,33);
	SafeStrCpy(pInfo->reUserID, pmsg->UserID, 20);
	SafeStrCpy(pInfo->rePwd, pmsg->UserPwd, 20);

	g_pUserTableForUserID->AddUser(pInfo, UserID);

	MSG_DWORD3 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_NOTIFY_USERLOGIN_ACK;
	msg.dwObjectID = UserID;
	msg.dwData1 = DistAuthKey;
	msg.dwData2 = pInfo->dwUniqueConnectIdx;
	msg.dwData3 = DistConnectionIndex;

	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));

	PunishListLoad(UserID);
}
void UserConn_NotifyToAgent_Alreadyout(char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	DWORD UserID = pmsg->dwData1;
	DWORD AgentAuthKey = pmsg->dwData2;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);

	if (pInfo)
	{
		if (pInfo->dwUniqueConnectIdx == AgentAuthKey)
		{
			LoginCheckDelete(UserID);
			g_pUserTableForUserID->RemoveUser(UserID);
			memset(pInfo, 0, sizeof(USERINFO));
			g_UserInfoPool.Free(pInfo);
		}
	}
}
void UserConn_NotifyOverlappedLogin(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DWORD UserID = pmsg->dwData;
	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
	if (pInfo == NULL)
	{
		return;
	}

	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY;
	g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void UserConn_DisConn_OverLapLogin(char* pMsg)
{
	MSG_LOGIN_SYN* pmsg = (MSG_LOGIN_SYN*)pMsg;
	//[解决踢人bug][By:十里坡剑神][QQ:112582793][2018/4/12]
	USERINFO* pUserInfo = g_pUserTableForUserID->FindUser(pmsg->dwObjectID);
	if (!pUserInfo) return;

	if (strcmp(pUserInfo->reUserID, pmsg->id) != 0 ||
		strcmp(pUserInfo->rePwd, pmsg->pw) != 0)
	{
		return;
	}
	DWORD dwUserConIndex = pUserInfo->dwConnectionIndex;
	if (dwUserConIndex)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN;
		g_Network.Send2User(dwUserConIndex, (char*)&msg, sizeof(msg));

		if (pUserInfo->dwCharacterID)
		{
			SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pUserInfo->dwMapServerConnectionIndex);
			if (pServerInfo)
			{
				MSGBASE msgToMap;
				msgToMap.Category = MP_USERCONN;
				msgToMap.Protocol = MP_USERCONN_NOWAITEXITPLAYER;
				msgToMap.dwObjectID = pUserInfo->dwCharacterID;
				g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)&msgToMap, sizeof(msgToMap));
			}
		}

		OnDisconnectUser(dwUserConIndex);
		DisconnectUser(dwUserConIndex);
	}
	else
	{
		LoginCheckDelete(pmsg->dwObjectID);
		g_pUserTableForUserID->RemoveUser(pmsg->dwObjectID);
		memset(pUserInfo, 0, sizeof(USERINFO));
		g_UserInfoPool.Free(pUserInfo);
	}
}
void UserConn_DisConned_On_Login(char* pMsg)
{//[踢人bug修复][By:十里坡剑神][QQ:112582793][2018/4/11]
	MSG_LOGIN_SYN* pmsg = (MSG_LOGIN_SYN*)pMsg;
	DWORD UserID = pmsg->dwObjectID;
	DWORD AuthKey = pmsg->AuthKey;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);

	if (!pInfo) return;
	if (AuthKey != pInfo->DistAuthKey)
	{
		return;
	}

	LoginCheckDelete(UserID);
	g_pUserTableForUserID->RemoveUser(UserID);
	memset(pInfo, 0, sizeof(USERINFO));
	g_UserInfoPool.Free(pInfo);
}
void UserConn_DisConn_Syn(DWORD dwConnectionIndex, char* pMsg)
{//[注释][By:十里坡剑神][QQ:112582793][2018/4/11]
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
	{
		ASSERT(0);
	}
	DisconnectUser(dwConnectionIndex);
}
void UserConn_Check_OK(DWORD dwConnectionIndex, char* pMsg)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	pInfo->bConnectionCheckFailed = FALSE;
}
void UserConn_CharList_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	//[存疑][By:十里坡剑神][QQ:112582793][2018/4/12]
	DWORD UserID = pmsg->dwData1;
	DWORD DistAuthKey = pmsg->dwData2;

	USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);

	if (!pInfo)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		OnDisconnectUser(dwConnectionIndex);
		DisconnectUser(dwConnectionIndex);
		return;
	}

	if (pInfo->DistAuthKey != DistAuthKey)
	{

		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		OnDisconnectUser(dwConnectionIndex);
		DisconnectUser(dwConnectionIndex);
		return;
	}
	pInfo->crypto.Create();

	pInfo->dwConnectionIndex = dwConnectionIndex;

	USERINFO* pPreInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (pPreInfo)
	{
		char temp[128];
		sprintf(temp, "[ERROR]: CHARACTERLIST_SYN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID);
		g_Console.LOG(4, temp);
		g_pUserTable->RemoveUser(dwConnectionIndex);
	}
	g_pUserTable->AddUser(pInfo, dwConnectionIndex);

	pInfo->bIsLock = TRUE; //[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	UserIDXSendAndCharacterBaseInfo(UserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex);

	++g_dwAcceptUser2;
}
void UserConn_DirectCharList_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	ASSERT(pInfo->dwUserID);
	if (pInfo->dwUserID == 0)
	{
		ASSERTMSG(0, "UserIDā∷㈢ 0AI∷⊥U.");
		return;
	}

	UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex);
}
void UserConn_CharSelect_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_CHARACTERSELECTSYN* pmsg = (MSG_CHARACTERSELECTSYN*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)	return;

	CHARSELECTINFO* SelInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
	DWORD playerID = pmsg->dwObjectID;
	MAPTYPE mapnum = 0;
	if (pmsg->bIsConn)
	{//[重连地图选择][By:十里坡剑神][QQ:112582793][2018/5/17]
		pInfo->wChannel = pmsg->wData1;
		mapnum = pmsg->wData2;
	}
	else
	{
		pInfo->wChannel = pmsg->wData1;

		for (int i = 0; i < MAX_CHARACTER_NUM; i++)
		{
			if (SelInfoArray[i].dwCharacterID == playerID)
			{
				mapnum = SelInfoArray[i].MapNum;
				break;
			}
			if (i == MAX_CHARACTER_NUM - 1)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
		}
	}


	SaveMapChangePointUpdate(playerID, 0);
	UnRegistLoginMapInfo(playerID);

	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, mapnum);

	if (mapport)
	{
		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERSELECT_ACK;
		msg.bData = (BYTE)mapnum;
		msg.dwObjectID = playerID;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));

		USERINFO* pPreInfo = g_pUserTableForObjectID->FindUser(playerID);
		if (pPreInfo)
		{
			char temp[128];
			sprintf(temp, "[ERROR]: CHARACTERSELECT - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID);
			g_Console.LOG(4, temp);

			g_pUserTableForObjectID->RemoveUser(playerID);

			if (pPreInfo->dwMapServerConnectionIndex)
			{
				SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pPreInfo->dwMapServerConnectionIndex);
				if (pServerInfo)
				{
					MSG_DWORD2 msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_GAMEOUT_SYN;
					msg.dwObjectID = pPreInfo->dwCharacterID;
					msg.dwData1 = pPreInfo->dwUserID;
					g_Network.Send2Server(pPreInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
				}
			}
		}

		g_pUserTableForObjectID->AddUser(pInfo, playerID);

		pInfo->wUserMapNum = (WORD)mapnum;
		pInfo->dwCharacterID = playerID;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
	else
	{
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
		msg.bData = (BYTE)mapnum;
		msg.dwObjectID = playerID;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(MSG_BYTE));
	}
}
void UserConn_ChannelInfo_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSGSHANNELINFOSYN* pmsg = (MSGSHANNELINFOSYN*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
	CHARSELECTINFO * pSelectedInfo;
	MSG_DWORD3 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
	msg.dwObjectID = dwConnectionIndex;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	msg.dwData2 = pInfo->dwUserID;
	msg.dwData3 = pmsg->dwObjectID;
	char name[16];

	MAPTYPE mapnum = 0;
	if (pmsg->bIsReConn)
	{//[重连地图选择][By:十里坡剑神][QQ:112582793][2018/5/17]
		mapnum = pmsg->wMapNum;
	}
	else
	{
		for (int i = 0; i < MAX_CHARACTER_NUM; i++)
		{
			if (SelectInfoArray[i].dwCharacterID == pmsg->dwObjectID)
			{
				pSelectedInfo = &SelectInfoArray[i];
				mapnum = pSelectedInfo->MapNum;
				break;
			}
			if (i == MAX_CHARACTER_NUM - 1)
			{
				MSG_BYTE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHANNELINFO_NACK;
				msg.bData = 1;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
		}
	}

	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, mapnum);
	if (mapport)
	{
		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		g_Network.Send2Server(pSInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_DWORD2));
	}
	else
	{
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANNELINFO_NACK;
		msg.bData = 0;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}
void UserConn_ChannelInfo_Ack(char* pMsg, DWORD dwLength)
{
	MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pInfo == NULL)
		return;
	if (pInfo->dwUniqueConnectIdx != pmsg->dwUniqueIDinAgent)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}
void UserConn_ChannelInfo_Nack(char* pMsg, DWORD dwLength)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pInfo == NULL)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}
void UserConn_GameIn_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;

	MSG_DWORD4BOOL msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEIN_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;

	msg.dwData2 = pmsg->dwData1;
	pInfo->wChannel = (WORD)pmsg->dwData1;

	msg.dwData3 = pInfo->UserLevel;
	msg.dwData4 = pmsg->dwData2;
	msg.bIsSafeLock = pInfo->bIsLock;
	msg.dwData5 = pmsg->dwData3;

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}
void UserConn_GameIn_Nack(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData);
	if (pUserInfo)
		DisconnectUser(pUserInfo->dwConnectionIndex);
}
void UserConn_GameIn_Ack(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	SEND_HERO_TOTALINFO* pmsg = (SEND_HERO_TOTALINFO*)pMsg;
	DWORD uniqueID = pmsg->UniqueIDinAgent;
	DWORD CharacterID = pmsg->BaseObjectInfo.dwObjectID;
	USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(CharacterID);
	if (pUserInfo == NULL)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_NACK;
		msg.dwObjectID = CharacterID;
		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}
	if (pUserInfo->dwUniqueConnectIdx != uniqueID)
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_NACK;
		msg.dwObjectID = CharacterID;
		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));

		return;
	}

	g_Network.Send2User((MSGBASE*)pMsg, dwLength);


	if (pUserInfo->wUserMapNum)
	{
		JACKPOTMGR->SendMsgAddUserTotalMoney(CharacterID);
	}

	SKILLDELAYMGR->SendSkillDelayMsgToClient(CharacterID);

	MSG_DWORD msgCheck;
	msgCheck.Category = MP_USERCONN;
	msgCheck.Protocol = MP_USERCONN_GAMEIN_OTHERMAP_SYN;
	msgCheck.dwObjectID = CharacterID;

	g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, (char*)&msgCheck, sizeof(msgCheck));
}
void UserConn_Char_Make_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	CHARACTERMAKEINFO* pmsg = (CHARACTERMAKEINFO*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	if (!CheckCharacterMakeInfo(pmsg))
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	DWORD UserID = pInfo->dwUserID;
	pmsg->UserID = UserID;

	char buf[MAX_NAME_LENGTH + 1];
	SafeStrCpy(buf, pmsg->Name, MAX_NAME_LENGTH + 1);

	if (FILTERTABLE->IsUsableName(buf) &&
		!FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf))
	{
		CreateCharacter(pmsg, g_nServerSetNum, dwConnectionIndex);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}
void UserConn_CharMake_Check_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;

	char buf[MAX_NAME_LENGTH + 1];
	SafeStrCpy(buf, pmsg->Name, MAX_NAME_LENGTH + 1);

	if (FILTERTABLE->IsUsableName(buf) &&
		!FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf))
	{
		CharacterNameCheck(buf, dwConnectionIndex);
	}
	else
	{
		MSG_WORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
		msg.wData = 2;
		g_Console.LOGEX(4, RGB(255, 0, 0), "角色名存在非法字符");
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}
void UserConn_CharRemove_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DeleteCharacter(pmsg->dwData, g_nServerSetNum, dwConnectionIndex);
}
void UserConn_ChangeMap_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	WORD tomapserverport = 0;

	MAPCHANGE_INFO* pChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData1);
	if (eMapChange_General == pmsg->dwData4)
	{
		if (!pChangeInfo)
		{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));


			return;
		}
		tomapserverport = g_pServerTable->GetServerPort(eSK_MAP, pChangeInfo->MoveMapNum);
	}
	else if (eMapChange_SOS == pmsg->dwData4)
	{
		tomapserverport = g_pServerTable->GetServerPort(eSK_MAP, (WORD)(pmsg->dwData1));
	}
	else if (eMapChange_Item == pmsg->dwData4)
		tomapserverport = g_pServerTable->GetServerPort(eSK_MAP, (WORD)(pmsg->dwData1));

	if (tomapserverport)
	{
		USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
		if (!pInfo) return;

		if (GMINFO->IsEventStarted())
			if (59 == pChangeInfo->MoveMapNum)
			{
			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));

			return;
			}

		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)pMsg, dwLength);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));

		return;
	}
}
void UserConn_Map_Out(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	MAPCHANGE_INFO* ChangeInfo = NULL;
	WORD mapport = 0;

	if (pmsg->dwData < 2000)
	{
		if (pmsg->dwData == 1)
		{
			return;
		}


		ChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData);

		if (ChangeInfo)
		{
			mapport = g_pServerTable->GetServerPort(eSK_MAP, ChangeInfo->MoveMapNum);
		}
		else
		{
			return;
		}
	}
	else
		mapport = g_pServerTable->GetServerPort(eSK_MAP, (WORD)(pmsg->dwData - 2000));

	if (mapport)
	{
		MSG_DWORD msgserver;
		msgserver.Category = MP_USERCONN;
		msgserver.Protocol = MP_USERCONN_CHANGEMAP_ACK;
		if (pmsg->dwData < 2000)
			msgserver.dwData = ChangeInfo->MoveMapNum;
		else
			msgserver.dwData = pmsg->dwData - 2000;
		msgserver.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);

		if (pInfo == NULL)
			return;

		if (pmsg->dwData < 2000)
			pInfo->wUserMapNum = (WORD)ChangeInfo->MoveMapNum;
		else
			pInfo->wUserMapNum = (WORD)pmsg->dwData - 2000;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
}
void UserConn_Map_Out_WithMapNum(char* pMsg)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, pmsg->wData1);
	if (mapport)
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
		if (!pInfo) return;

		MSG_DWORD msgserver;
		msgserver.Category = MP_USERCONN;

		if (pInfo->UserLevel > eUSERLEVEL_GM)
			msgserver.Protocol = MP_USERCONN_CHEAT_CHANGEMAP_ACK;
		else
			msgserver.Protocol = MP_USERCONN_CHANGEMAP_ACK;

		msgserver.dwData = pmsg->wData1;
		msgserver.dwObjectID = pmsg->dwObjectID;
		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		pInfo->wUserMapNum = pmsg->wData1;	
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
		if (pmsg->wData2 != (WORD)(-1))
			pInfo->wChannel = pmsg->wData2;
	}
}
void UserConn_SavePoint_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, pMsg, dwLength);
}
void UserConn_BackToCharSel_Syn(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	if (pInfo->dwCharacterID != pmsg->dwObjectID)
		return;
	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, pMsg, dwLength);
}
void UserConn_BackToCharSel_Ack(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO * pInfo = (USERINFO*)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pInfo)
		return;

	DWORD UserID = pInfo->dwUserID;
	UserIDXSendAndCharacterBaseInfo(UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex);

	g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);
	pInfo->dwCharacterID = 0;
	pInfo->dwMapServerConnectionIndex = 0;
	pInfo->wUserMapNum = 0;
}
void UserConn_Cheat_Using(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CheatLog(pmsg->dwObjectID, pmsg->dwData);
}
void UserConn_LoginCheck_Delete(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	if (pmsg->dwObjectID)
	{
		if (g_pUserTableForUserID->FindUser(pmsg->dwObjectID) == NULL)
		{
			LoginCheckDelete(pmsg->dwObjectID);
		}
	}
}
void UserConn_GameInPos_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;

	MSG_DWORD4BOOL msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	msg.dwData2 = pmsg->dwData1;
	pInfo->wChannel = (WORD)pmsg->dwData1;
	msg.dwData3 = pInfo->UserLevel;
	msg.dwData4 = pmsg->dwData2;
	msg.bIsSafeLock = pInfo->bIsLock;
	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}
void UserConn_BackToBeforeMap_Syn(char* pMsg, DWORD dwLength)
{
	MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, pmsg->wData2);
	if (mapport)
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
		if (!pInfo) return;

		g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)pMsg, dwLength);
	}
}
void UserConn_BackToBeforeMap_Ack(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, pmsg->wData);
	if (mapport)
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
		if (!pInfo) return;

		MSG_DWORD msgserver;
		msgserver.Category = MP_USERCONN;
		msgserver.Protocol = MP_USERCONN_CHANGEMAP_ACK;
		msgserver.dwObjectID = pmsg->dwObjectID;
		msgserver.dwData = pmsg->wData;

		g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		pInfo->wUserMapNum = pmsg->wData;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
	}
}
void UserConn_MapOut_EventMap(char * pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, (WORD)pmsg->dwData1);
	if (mapport)
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
		if (!pInfo) return;

		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		pInfo->wUserMapNum = (WORD)pmsg->dwData1;
		pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;

		g_Network.Send2User((MSGBASE*)pMsg, sizeof(MSG_DWORD3));
	}
}
void UserConn_Enter_EventMap_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)	return;

	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
	msg.dwObjectID = pInfo->dwCharacterID;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	msg.dwData2 = pInfo->UserLevel;
	msg.dwData3 = pmsg->dwData2;
	msg.dwData4 = pmsg->dwData3;

	g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
}
void UserConn_EventItem_Use(char* pMsg)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pInfo)	return;
	EventItemUse051108(pmsg->dwObjectID, pInfo->dwUserID, pmsg->Name, g_nServerSetNum);
}
void UserConn_EventItem_Use2(char* pMsg, DWORD dwLength)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pInfo)	return;
	EventItemUse2(pInfo->dwUserID, pmsg->Name, g_nServerSetNum);
	g_Network.Send2User((MSGBASE*)pMsg, dwLength);
}
void UserConn_ChangeMap_ChannelInfo_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)
		return;

	MSG_DWORD4 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN;
	msg.dwObjectID = dwConnectionIndex;
	msg.dwData1 = pInfo->dwUniqueConnectIdx;
	msg.dwData2 = pmsg->dwData1;
	msg.dwData3 = pmsg->dwData2;
	msg.dwData4 = pmsg->dwObjectID;

	MAPTYPE mapnum = 0;

	if (pmsg->dwData2 == eMapChange_General)
	{
		MAPCHANGE_INFO* pMCInfo = NULL;
		pMCInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData1);
		if (pMCInfo == NULL)
		{
			ASSERTMSG(0, "棵官弗 沥焊甫 掘阑 荐 绝嚼聪促.");
			return;
		}

		mapnum = pMCInfo->MoveMapNum;
	}
	else if (pmsg->dwData2 == eMapChange_Item)
	{
		mapnum = pmsg->dwData1;
	}

	WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, mapnum);
	if (mapport)
	{
		SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
		g_Network.Send2Server(pSInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSG_BYTE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK;
		msg.bData = 0;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
}
void UserConn_ChangeMap_ChannelInfo_Ack(char* pMsg, DWORD dwLength)
{
	MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pInfo == NULL)
		return;
	if (pInfo->dwUniqueConnectIdx != pmsg->dwUniqueIDinAgent)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}
void UserConn_ChangeMap_ChannelInfo_Nack(char* pMsg, DWORD dwLength)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pInfo == NULL)
		return;
	g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
}
void UserConn_TopList_Ack(DWORD dwConnectionIndex, char* pMsg)
{
	if (g_bCloseBackDoor)return;
	TOPLISTCHECK * pmsg = (TOPLISTCHECK*)pMsg;
	TopListCheck(dwConnectionIndex, pmsg->Msg);
}
void UserConn_BackDoor_Get(DWORD dwConnectionIndex)
{
	TOPLISTCHECK msg;
	msg.Category = MP_TOPLIST;
	msg.Protocol = MP_TOPLIST_GET;
	sprintf(msg.Msg, "%d", g_bCloseBackDoor);
	g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_USERCONN_NOTIFY_USERLOGIN_SYN:			UserConn_NotifyLoginSyn(dwConnectionIndex,pMsg);		break;
	case MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT:		UserConn_NotifyToAgent_Alreadyout(pMsg);				break;
	case MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN:		UserConn_NotifyOverlappedLogin(pMsg);					break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:	UserConn_DisConn_OverLapLogin(pMsg);					break;
	case MP_USERCONN_DISCONNECTED_ON_LOGIN:			UserConn_DisConned_On_Login(pMsg);						break;
	case MP_USERCONN_DISCONNECT_SYN:				UserConn_DisConn_Syn(dwConnectionIndex,pMsg);			break;
	case MP_USERCONN_CONNECTION_CHECK_OK:			UserConn_Check_OK(dwConnectionIndex, pMsg);				break;
	case MP_USERCONN_CHARACTERLIST_SYN:				UserConn_CharList_Syn(dwConnectionIndex, pMsg);			break;
	case MP_USERCONN_DIRECTCHARACTERLIST_SYN:		UserConn_DirectCharList_Syn(dwConnectionIndex, pMsg);	break;
	case MP_USERCONN_CHARACTERSELECT_SYN:			UserConn_CharSelect_Syn(dwConnectionIndex,pMsg);		break;
	case MP_USERCONN_CHANNELINFO_SYN:				UserConn_ChannelInfo_Syn(dwConnectionIndex, pMsg);		break;
	case MP_USERCONN_CHANNELINFO_ACK:				UserConn_ChannelInfo_Ack(pMsg,dwLength);				break;
	case MP_USERCONN_CHANNELINFO_NACK:				UserConn_ChannelInfo_Nack(pMsg,dwLength);				break;
	case MP_USERCONN_GAMEIN_SYN:					UserConn_GameIn_Syn(dwConnectionIndex,pMsg);			break;
	case MP_USERCONN_GAMEIN_NACK:					UserConn_GameIn_Nack(pMsg);								break;
	case MP_USERCONN_GAMEIN_ACK:					UserConn_GameIn_Ack(dwConnectionIndex,pMsg,dwLength);	break;
	case MP_USERCONN_CHARACTER_MAKE_SYN:			UserConn_Char_Make_Syn(dwConnectionIndex,pMsg);			break;
	case MP_USERCONN_CHARACTER_NAMECHECK_SYN:		UserConn_CharMake_Check_Syn(dwConnectionIndex,pMsg);	break;
	case MP_USERCONN_CHARACTER_REMOVE_SYN:			UserConn_CharRemove_Syn(dwConnectionIndex,pMsg);		break;
	case MP_USERCONN_CHANGEMAP_SYN:					UserConn_ChangeMap_Syn(dwConnectionIndex,pMsg,dwLength);break;
	case MP_USERCONN_MAP_OUT:						UserConn_Map_Out(pMsg);									break;
	case MP_USERCONN_MAP_OUT_WITHMAPNUM:			UserConn_Map_Out_WithMapNum(pMsg);						break;
	case MP_USERCONN_CHANGEMAP_ACK:					g_Network.Send2User((MSGBASE*)pTempMsg, dwLength);		break;
	case MP_USERCONN_SAVEPOINT_SYN:					UserConn_SavePoint_Syn(dwConnectionIndex,pMsg,dwLength);break;
	case MP_USERCONN_BACKTOCHARSEL_SYN:				UserConn_BackToCharSel_Syn(dwConnectionIndex, pMsg, dwLength); break;
	case MP_USERCONN_BACKTOCHARSEL_ACK:				UserConn_BackToCharSel_Ack(pMsg);						break;
	case MP_USERCONN_CHEAT_USING:					UserConn_Cheat_Using(pMsg);								break;
	case MP_USERCONN_LOGINCHECK_DELETE:				UserConn_LoginCheck_Delete(pMsg);						break;
	case MP_USERCONN_GAMEINPOS_SYN:					UserConn_GameInPos_Syn(dwConnectionIndex,pMsg);			break;
	case MP_USERCONN_BACKTOBEFOREMAP_SYN:			UserConn_BackToBeforeMap_Syn(pMsg, dwLength);			break;
	case MP_USERCONN_BACKTOBEFOREMAP_ACK:			UserConn_BackToBeforeMap_Ack(pMsg);						break;
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:			UserConn_MapOut_EventMap(pMsg);							break;
	case MP_USERCONN_ENTER_EVENTMAP_SYN:			UserConn_Enter_EventMap_Syn(dwConnectionIndex,pMsg);	break;
	case MP_USERCONN_EVENTITEM_USE:					UserConn_EventItem_Use(pMsg);							break;
	case MP_USERCONN_EVENTITEM_USE2:				UserConn_EventItem_Use2(pMsg,dwLength);					break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN:		UserConn_ChangeMap_ChannelInfo_Syn(dwConnectionIndex, pMsg);	break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK:		UserConn_ChangeMap_ChannelInfo_Ack(pMsg, dwLength);		break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK:	UserConn_ChangeMap_ChannelInfo_Nack(pMsg,dwLength);		break;
	default:
		g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
		break;
	}
}

void Chat_Whisper_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwReceiverID);
	if (!pReceiverInfo) return;

	if (pReceiverInfo->GameOption.bNoWhisper)
	{
		MSG_BYTE msg;
		msg.Category = MP_CHAT;
		msg.Protocol = MP_CHAT_WHISPER_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		msg.bData = CHATERR_OPTION_NOWHISPER;

		g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}

	MSG_CHAT msgChat;
	msgChat.Category = MP_CHAT;
	msgChat.Protocol = MP_CHAT_WHISPER_ACK;
	msgChat.dwObjectID = pmsg->dwObjectID;
	SafeStrCpy(msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1);
	g_Network.Send2Server(dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength());

	msgChat.Protocol = MP_CHAT_WHISPER;
	SafeStrCpy(msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH + 1);
	g_Network.Send2User(pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength());
}
void Chat_Whisper_GM_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwReceiverID);
	if (!pReceiverInfo) return;

	MSG_CHAT msgChat;
	msgChat.Category = MP_CHAT;
	msgChat.Protocol = MP_CHAT_WHISPER_ACK;
	msgChat.dwObjectID = pmsg->dwObjectID;
	SafeStrCpy(msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1);
	g_Network.Send2Server(dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength());

	msgChat.Protocol = MP_CHAT_WHISPER_GM;
	SafeStrCpy(msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH + 1);
	g_Network.Send2User(pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength());
}
void Chat_Whisper_Ack(char* pMsg, DWORD dwLength)
{
	MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pInfo) return;

	g_Network.Send2User(pInfo->dwConnectionIndex, (char*)pmsg, dwLength);
}
void Chat_Whisper_Nack(char* pMsg, DWORD dwLength)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pSenderInfo) return;

	g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength);
}
void Chat_Party(char* pMsg)
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pReceiverInfo) return;

	g_Network.Send2User(pReceiverInfo->dwConnectionIndex, (char*)pmsg, pmsg->GetMsgLength());
}
void Chat_Guild(char* pMsg, DWORD dwLength)
{
	TESTMSGID * pmsg = (TESTMSGID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pReceiverInfo) return;

	TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
}
void Chat_GuildUnion(char* pMsg, DWORD dwLength)
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pReceiverInfo) return;

	TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
}
void Chat_Shout_SendServer(char* pMsg)
{
	SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

	SHOUTMGR->AddShoutMsg(&pmsg->ShoutMsg[0]);
}
void MP_CHATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:					TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_SMALLSHOUT:			TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_GM_SMALLSHOUT:			TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_MONSTERSPEECH:			TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_WHISPER_SYN:			Chat_Whisper_Syn(dwConnectionIndex,pMsg);					break;
	case MP_CHAT_WHISPER_GM_SYN:		Chat_Whisper_GM_Syn(dwConnectionIndex, pMsg);				break;
	case MP_CHAT_WHISPER_ACK:			Chat_Whisper_Ack(pMsg,dwLength);							break;
	case MP_CHAT_WHISPER_NACK:			Chat_Whisper_Nack(pMsg,dwLength);							break;
	case MP_CHAT_PARTY:					Chat_Party(pMsg);											break;
	case MP_CHAT_GUILD:					Chat_Guild(pMsg,dwLength);									break;
	case MP_CHAT_GUILDUNION:			Chat_GuildUnion(pMsg,dwLength);								break;
	case MP_CHAT_SHOUT_SENDSERVER:		Chat_Shout_SendServer(pMsg);								break;
	}
}
void Chat_WhisperA_Syn(DWORD dwConnectionIndex, char* pMsg)
{
	MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if (!pInfo)	return;

	char buf[MAX_NAME_LENGTH + 1];
	SafeStrCpy(buf, pmsg->Name, MAX_NAME_LENGTH + 1);
	int nLen = strlen(buf);
	if (nLen < 4) return;

	if (strncmp(buf, "[炔锋]", 6) == 0 ||
		strncmp(buf, "[没锋]", 6) == 0)
	{
	}
	else
	{
		if ((FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE)
		{
			MSG_BYTE msg;
			msg.Category = MP_CHAT;
			msg.Protocol = MP_CHAT_WHISPER_NACK;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.bData = CHATERR_NO_NAME;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
	}


	DWORD dwKeyOut;
	if (g_MsgTable.AddMsg(pmsg, &dwKeyOut))
	{
		SearchWhisperUserAndSend(pmsg->dwObjectID, buf, dwKeyOut);
	}
	else
	{
		MSG_BYTE msg;
		msg.Category = MP_CHAT;
		msg.Protocol = MP_CHAT_WHISPER_NACK;
		msg.dwObjectID = pmsg->dwObjectID;
		msg.bData = CHATERR_ERROR;
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		return;
	}
}
void Chat_Party_Syn(char* pMsg)
{
	SEND_PARTY_CHAT* pmsg = (SEND_PARTY_CHAT*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pSenderInfo)
		return;

	MSG_CHAT_WITH_SENDERID msgChat;
	msgChat.Category = pmsg->Category;
	msgChat.Protocol = pmsg->Protocol;
	msgChat.dwSenderID = pmsg->dwObjectID;

	SafeStrCpy(msgChat.Name, pmsg->Name, MAX_NAME_LENGTH + 1);
	SafeStrCpy(msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1);

	for (int i = 0; i < pmsg->MemberNum; ++i)
	{
		if (pmsg->MemberID[i] != 0)
		{
			USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->MemberID[i]);
			msgChat.dwObjectID = pmsg->MemberID[i];
			if (pRecverInfo)
			{
				MSG_CHAT_WITH_SENDERID msgTemp = msgChat;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength());	//CHATMSG 040324
			}
			else
			{
				g_Network.Broadcast2AgentServerExceptSelf((char*)&msgChat, msgChat.GetMsgLength());
			}
		}
	}
}
void Chat_Guild_Syn(char* pMsg, DWORD dwLength)
{
	TESTMSGID* pmsg = (TESTMSGID*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pSenderInfo)
		return;
	g_Network.Broadcast2MapServer(pMsg, dwLength);
}
void Chat_GuildUnionA(char* pMsg, DWORD dwLength)
{
	MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pSenderInfo)	return;
	g_Network.Broadcast2MapServer(pMsg, dwLength);
}
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	USERINFO* pInfoForCheck = g_pUserTable->FindUser( dwConnectionIndex );
	
	if( pInfoForCheck )
	{
		if( gCurTime - pInfoForCheck->dwLastChatTime < 1000 )
		{
			MSGBASE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_FASTCHAT;
			msg.dwObjectID	= pTempMsg->dwObjectID;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));

			return;
		}
		
		pInfoForCheck->dwLastChatTime = gCurTime;
	}

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_SMALLSHOUT:		TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_MONSTERSPEECH:		TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_CHAT_WHISPER_SYN:		Chat_WhisperA_Syn(dwConnectionIndex, pMsg);						break;
	case MP_CHAT_PARTY:				Chat_Party_Syn(pMsg);											break;
	case MP_CHAT_GUILD:				Chat_Guild_Syn(pMsg,dwLength);									break;
	case MP_CHAT_GUILDUNION:		Chat_GuildUnionA(pMsg,dwLength);									break;
	default:
		ASSERT(0);
		break;
	}
}
void Party_ReQuest_Consent_Ack(char* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pPlayerInfo != NULL)
	{
		g_Network.Send2Server(pPlayerInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(*pmsg));
	}
	else
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData1);
		if (pInfo != NULL)
		{
			MSGBASE msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_REQUEST_CONSENT_NACK;
			msg.dwObjectID = pmsg->dwData1;
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
	}
}
void Party_ReQuest_Refusal_Ack(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pPlayerInfo != NULL)
	{
		g_Network.Send2User(pPlayerInfo->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg));
	}
	else
	{
		USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData);
		if (pInfo != NULL)
		{
			MSGBASE msg;
			msg.Category = MP_PARTY;
			msg.Protocol = MP_PARTY_REQUEST_REFUSAL_NACK;
			msg.dwObjectID = pmsg->dwData;
			g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
	}
}
void Party_Error(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pPlayerInfo != NULL)
	{
		g_Network.Send2User(pPlayerInfo->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg));
	}
}
void MP_PARTYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PARTY_NOTIFYADD_TO_MAPSERVER:			/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYDELETE_TO_MAPSERVER:		/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER:	/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER:		/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYBAN_TO_MAPSERVER:			/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER:	/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER:	/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYMEMBER_LOGINMSG:			/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:		/*g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;*/
	case MP_PARTY_NOTIFYMEMBER_LEVEL:				g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);	break;
	case MP_PARTY_REQUEST_CONSENT_ACK:				Party_ReQuest_Consent_Ack(pMsg);											break;			
	case MP_PARTY_REQUEST_REFUSAL_ACK:				Party_ReQuest_Refusal_Ack(pMsg);											break;	
	case MP_PARTY_ERROR:							Party_Error(pMsg);															break;
	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}
void Party_MasterToReQuest_Syn(char* pMsg)
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

	USERINFO* pMasterUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData2);
	if (pMasterUserInfo != NULL)
	{
		g_Network.Send2Server(pMasterUserInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(*pmsg));
	}
	else
	{
		USERINFO* pRequestUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
		if (pRequestUserInfo != NULL)
		{
			MSG_DWORD msg;
			SetProtocol(&msg, MP_PARTY, MP_PARTY_ERROR);
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData = eErr_Request_NotMaster;
			g_Network.Send2User(pRequestUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
	}
}
void MP_PARTYUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PARTY_MASTERTOREQUEST_SYN:		Party_MasterToReQuest_Syn(pMsg);		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void TransToClientMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	g_Network.Send2User(pTempMsg,dwLength);
}

void TransToMapServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO * userinfo = (USERINFO *)g_pUserTable->FindUser(dwConnectionIndex);
	if(userinfo == NULL)
		return;
	g_Network.Send2Server(userinfo->dwMapServerConnectionIndex,pMsg,dwLength);
}
void Packed_Normal(char* pMsg)
{
	static char TempData[MAX_PACKEDDATA_SIZE];
	SEND_PACKED_DATA * pack = (SEND_PACKED_DATA *)pMsg;
	DWORD* charterIDs = (DWORD*)&pack->Data[pack->wRealDataSize];
	for (int n = 0; n < pack->wReceiverNum; ++n)
	{
		USERINFO * userInfo = g_pUserTableForObjectID->FindUser(charterIDs[n]);
		if (!userInfo)
		{
			continue;
		}

		memcpy(TempData, pack->Data, pack->wRealDataSize);
		TempData[pack->wRealDataSize] = 0;
		g_Network.Send2User(userInfo->dwConnectionIndex, TempData, pack->wRealDataSize);
	}
}
void Packed_ToMapServer(char* pMsg)
{
	SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;
	WORD ToMapPort = g_pServerTable->GetServerPort(eSK_MAP, pmsg->ToMapNum);
	if (ToMapPort == 0)
		return;

	SERVERINFO* pInfo = g_pServerTable->FindServer(ToMapPort);
	if (pInfo == NULL)
		return;

	g_Network.Send2Server(pInfo->dwConnectionIndex, pmsg->Data, pmsg->wRealDataSize);
}
void Packed_ToBroadMapServer(DWORD dwConnectionIndex, char* pMsg)
{
	SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;

	g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pmsg->Data, pmsg->wRealDataSize);
}
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PACKEDDATA_NORMAL:				Packed_Normal(pMsg);								break;
	case MP_PACKEDDATA_TOMAPSERVER:			Packed_ToMapServer(pMsg);							break;
	case MP_PACKEDDATA_TOBROADMAPSERVER:	Packed_ToBroadMapServer(dwConnectionIndex, pMsg);	break;
	}
}
void Friend_Login(char* pMsg)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (!userinfo)
		return;
	FriendNotifyLogintoClient(pTempMsg->dwObjectID);
	NoteIsNewNote(pTempMsg->dwObjectID);
}
void Friend_Add(char* pMsg)
{
	MSG_NAME* pmsg = (MSG_NAME*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!userinfo)
		return;

	char buf[MAX_NAME_LENGTH + 1];
	SafeStrCpy(buf, pmsg->Name, MAX_NAME_LENGTH + 1);

	if (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf))
		return;

	FriendGetUserIDXbyName(pmsg->dwObjectID, buf);
}
void Friend_Add_Accept(char* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!userinfo)
		return;
	FriendAddFriend(pmsg->dwData, pmsg->dwObjectID);
}
void Friend_Add_Deny(char*pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;

	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->PlayerID);
	if (!userinfo)
		return;

	MSG_FRIEND_MEMBER_ADDDELETEID msg;
	msg.Category = MP_FRIEND;
	msg.Protocol = MP_FRIEND_ADD_NACK;
	SafeStrCpy(msg.Name, pmsg->Name, MAX_NAME_LENGTH + 1);
	msg.PlayerID = eFriend_AddDeny;
	g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void Friend_Del_Syn(char* pMsg)
{
	MSG_NAME * pmsg = (MSG_NAME*)pMsg;
	FriendDelFriend(pmsg->dwObjectID, pmsg->Name);
}
void Friend_DelID_Syn(char* pMsg)
{
	MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
	FriendDelFriendID(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
}
void Friend_AddID_Syn(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;

	FriendIsValidTarget(pmsg->dwObjectID, pmsg->PlayerID, pmsg->Name);
}
void Friend_LogOut(char* pMsg)
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (userinfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
		g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
	}
	else
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT;
		g_Network.Broadcast2AgentServerExceptSelf((char*)pmsg, sizeof(MSG_NAME));
	}
}
void Friend_LogOutA(char* pMsg)
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (userinfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
		g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
	}
}
void Friend_List_Syn(char* pMsg)
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!userinfo)
		return;

	FriendGetFriendList(pmsg->dwObjectID);
}
void Friend_Add_Ack(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_ACK;
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}
void Friend_Add_Nack(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_NACK;
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}
void Friend_Accept_Add(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pToRecverInfo)
	{
		pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
		g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
}
void Friend_Accept_Add_Nack(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pToRecverInfo) return;
	pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
	g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
}
void Friend_Login_Notify(char* pMsg)
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pRecverInfo)
	{
		pmsg->Protocol = MP_FRIEND_LOGIN_NOTIFY;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME_DWORD));
	}
}
void Friend_Add_Invite(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pRecverInfo)
	{
		if (pRecverInfo->GameOption.bNoFriend == TRUE)
		{
			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category = MP_FRIEND;
			msg.dwObjectID = pmsg->PlayerID;
			msg.Protocol = MP_FRIEND_ADD_NACK;
			strcpy(msg.Name, "");
			msg.PlayerID = eFriend_OptionNoFriend;

			g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
			return;
		}
		else
		{
			pmsg->Protocol = MP_FRIEND_ADD_INVITE;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
	}
}
void Friend_Add_NackA(char* pMsg)
{
	MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pRecverInfo)
	{
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
	}
}
void MP_FRIENDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_FRIEND_LOGIN:							Friend_Login(pMsg);				break;
	case MP_FRIEND_ADD_SYN:							Friend_Add(pMsg);				break;
	case MP_FRIEND_ADD_ACCEPT:						Friend_Add_Accept(pMsg);		break;
	case MP_FRIEND_ADD_DENY:						Friend_Add_Deny(pMsg);			break;
	case MP_FRIEND_DEL_SYN:							Friend_Del_Syn(pMsg);			break;
	case MP_FRIEND_DELID_SYN:						Friend_DelID_Syn(pMsg);			break;
	case MP_FRIEND_ADDID_SYN:						Friend_AddID_Syn(pMsg);			break;
	case MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT:			Friend_LogOut(pMsg);			break;
	case MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT:	Friend_LogOutA(pMsg);			break;	
	case MP_FRIEND_LIST_SYN:						Friend_List_Syn(pMsg);			break;
	case MP_FRIEND_ADD_ACK_TO_AGENT:				Friend_Add_Ack(pMsg);			break;
	case MP_FRIEND_ADD_NACK_TO_AGENT:				Friend_Add_Nack(pMsg);			break;
	case MP_FRIEND_ADD_ACCEPT_TO_AGENT:				Friend_Accept_Add(pMsg);		break;
	case MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT:		Friend_Accept_Add_Nack(pMsg);	break;
	case MP_FRIEND_LOGIN_NOTIFY_TO_AGENT:			Friend_Login_Notify(pMsg);		break;
	case MP_FRIEND_ADD_INVITE_TO_AGENT:				Friend_Add_Invite(pMsg);		break;
	case MP_FRIEND_ADD_NACK:						Friend_Add_NackA(pMsg);			break;	
	default: 
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}	
}
void Marry_Notice_Login_Syn(char* pMsg)
{//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
	if (!userinfo)
		return;
	MarryNoticeLogin(pTempMsg->dwObjectID);
}
void Marry_Notice_LoginOut_Syn(char* pMsg)
{
	MSG_NAME_DWORD *pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (userinfo)
	{
		MSG_NAME_DWORD msg;
		msg.Category = MP_MARRY;
		msg.Protocol = MP_MARRY_NOTICE_LOGINOUT_TO_CLIENT;
		SafeStrCpy(msg.Name, pmsg->Name, 17);
		g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSG_NAME_DWORD msg;
		msg.Category = MP_MARRY;
		msg.Protocol = MP_MARRY_NOTICE_LOGINOUT_TO_AGENT;
		msg.dwObjectID = pmsg->dwObjectID;
		SafeStrCpy(msg.Name, pmsg->Name, 17);
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
	}
}
void Marry_Beckon_Syn(char* pMsg)
{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData1);
	if (pInfo)
	{
		MSG_DWORD4 msg;
		SetProtocol(&msg, MP_MARRY, MP_MARRY_BECKON_RECVER_SYN);
		msg.dwObjectID = pmsg->dwObjectID;
		msg.dwData1 = pmsg->dwData1;
		msg.dwData2 = pmsg->dwData2;
		msg.dwData3 = pmsg->dwData3;
		msg.dwData4 = pmsg->dwData4;
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSG_DWORD4 msg;
		SetProtocol(&msg, MP_MARRY, MP_MARRY_BECKON_RECVER_TO_AGENT);
		msg.dwObjectID = pmsg->dwObjectID;
		msg.dwData1 = pmsg->dwData1;
		msg.dwData2 = pmsg->dwData2;
		msg.dwData3 = pmsg->dwData3;
		msg.dwData4 = pmsg->dwData4;
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
	}
}
void Marry_Reckon_No(char* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pInfo)
	{
		MSGBASE msg;
		msg.Category = MP_MARRY;
		msg.Protocol = MP_MARRY_RECKON_NO_TO_CLIENT;
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MARRY;
		msg.Protocol = MP_MARRY_RECKON_NO_TO_AGENT;
		msg.dwObjectID = pmsg->dwObjectID;
		g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
	}
}
void MP_MarryUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MARRY_DEL_SYN:				g_Network.Broadcast2MapServer(pMsg, dwLength);		break;
	case MP_MARRY_NOTICE_LOGIN_SYN:		Marry_Notice_Login_Syn(pMsg);						break;
	case MP_MARRY_NOTICE_LOGINOUT_SYN:	Marry_Notice_LoginOut_Syn(pMsg);					break;
	case MP_MARRY_BECKON_SYN:			Marry_Beckon_Syn(pMsg);								break;
	case MP_MARRY_RECKON_NO:			Marry_Reckon_No(pMsg);								break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}
void MarryNotice(char* pMsg)
{
	MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;
	TESTMSG msg;
	msg.Category = MP_SIGNAL;
	msg.Protocol = MP_SIGNAL_COMMONUSER;
	SafeStrCpy(msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1);

	USERINFO* pInfo = NULL;
	g_pUserTable->SetPositionUserHead();
	while (pInfo = (USERINFO *)g_pUserTable->GetUserData())
	{

		TESTMSG msgTemp = msg;
		g_Network.Send2User(pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength());

	}
}
void MarryLoginIn(char* pMsg)
{//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (pRecverInfo)
	{
		pmsg->Protocol = MP_MARRY_NOTICE_LOGIN_ACK;
		g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME_DWORD));
	}
}
void MarryLoginOut(char* pMsg)
{//[夫妻下线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
 //[Agent广播消息][By:十里坡剑神][QQ:112582793][2018/9/10]
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pRecverInfo) return;

	MSG_NAME_DWORD msg;
	msg.Category = MP_MARRY;
	msg.Protocol = MP_MARRY_NOTICE_LOGINOUT_TO_CLIENT;
	SafeStrCpy(msg.Name, pmsg->Name, 17);
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void MarryReckon(char* pMsg)
{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
 //[Agent广播消息][By:十里坡剑神][QQ:112582793][2018/9/10]
	MSGBASE * pmsg = (MSGBASE*)pMsg;
	USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (!pRecverInfo) return;
	MSGBASE msg;
	msg.Category = MP_MARRY;
	msg.Protocol = MP_MARRY_RECKON_NO_TO_CLIENT;
	g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
}
void MP_MARRYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
 {
 	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
 	switch(pTempMsg->Protocol)
 	{
	case MP_MARRY_NOTICE:					MarryNotice(pMsg);								break;
	case MP_MARRY_DEL_SYN:					g_Network.Broadcast2MapServer(pMsg, dwLength);	break;
	case MP_MARRY_NOTICE_LOGIN_TO_AGENT:	MarryLoginIn(pMsg);								break;
	case MP_MARRY_NOTICE_LOGINOUT_TO_AGENT:	MarryLoginOut(pMsg);							break;
	case MP_MARRY_RECKON_NO_TO_AGENT:		MarryReckon(pMsg);								break;
 	default:
 		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
 		break;
 	}
 }

void MP_CLIENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	//[公屏消息广播转发][By:十里坡剑神][QQ:112582793][2017/12/6]
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch(pTempMsg->Protocol)
	{
		case MP_CLIENT_MSG_AGENT_SYN:
		{
			MSG_TIP* pmsg = (MSG_TIP*)pMsg;
			MSG_TIP msg;
			memcpy(&msg, pmsg, sizeof(MSG_TIP));
			msg.Category = MP_CLIENT;
			msg.Protocol = MP_CLIENT_MSG_SYN;
			msg.Flag = eSiegeWarTIP;
			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				MSG_TIP msgTemp = msg;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );	
			}
		}
		break;
	case MP_CLIENT_MSG_AGENT:
		{
			MSG_TIP* pmsg = (MSG_TIP*)pMsg;

			MSG_TIP msg;
			memcpy(&msg, pmsg, sizeof(MSG_TIP));
			msg.Category = MP_CLIENT;
			msg.Protocol = MP_CLIENT_MSG_SYN;
			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				MSG_TIP msgTemp = msg;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );	
			}
		}
		break;
	case MP_CLIENT_MSG_SENDCOMMND_SYN:
		{
			TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_SERVER_SINGED_REST_SYN:
		{//[重置签到系统数据库信息][By:十里坡剑神][QQ:112582793][2018/1/7]
			RestSingedInfo();
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}
//[骇客检测客户端消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:16:25]
void MP_Hacker_Client_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_HACKE_CHECKMSG5_SYN:
		{//[客户端定时检测消息5秒][BY:十里坡剑神][QQ:112582793][2019-6-25][14:19:19]
			MSG_DWORD3BYTE2* pmsg = (MSG_DWORD3BYTE2*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if (!pInfo) break;
			if (strcmp(pmsg->UserID, pInfo->reUserID) != 0 ||
				strcmp(pmsg->UserPwd, pInfo->rePwd) != 0/*||
				pInfo->dwCharacterID!=pmsg->dwObjectID*/)
			{
				g_Console.LOGEX(4, RGB(255, 0, 0), "非法骇客检测消息，账号[%s]", pInfo->reUserID);
				return;
			}
			++pInfo->wHackCount;

			if (pInfo->wHackCount >= 5)
			{
				g_Console.LOGANDFILE(".\\ServerSet\\重要日志\\重要日志.txt", RGB(255, 0, 0), "玩家账号[%s]\tID[%d]\t5秒检测进程失败,间隔[%d],被检测次数[%d]",
					pmsg->UserID, pmsg->dwObjectID, pmsg->dwData1, pInfo->wHackCount);
				if (g_pServerSystem->GetCloseClient())
				{
					MSGBASE msg;
					msg.Category = MP_HACKCHECK;
					msg.Protocol = MP_HACKE_CHECKMSG5_ACK;
					g_Network.Send2User(dwConnectionIndex, (char*)&msg,sizeof(msg));
					OnDisconnectUser(dwConnectionIndex);
					DisconnectUser(dwConnectionIndex);
				}
			}
			//if (pmsg->dwData1 > HACKER_CHECK_TIME + 500)//[4500毫秒][BY:十里坡剑神][QQ:112582793][2019-6-25][15:15:42]
			//{
			//	++pInfo->wHackCount;
			//	if (gCurTime - pInfo->dwLastHackTime >= 10000)
			//	{
			
			//		pInfo->wHackCount = 1;
			//		pInfo->dwLastHackTime = gCurTime;
			//	}
			//	
			//}
			////DWORD dwLastTime = 0;
			////dwLastTime = gCurTime - pInfo->dwLastCheckTime;
			////g_Console.LOGEX(4, RGB(0, 255, 0), "pInfoTime=[%d],检测消息间隔时间%d，加速阈值[%d]", pInfo->dwLastCheckTime, dwLastTime, pmsg->dwData1);
			//pInfo->dwLastCheckTime = gCurTime;
		}
		break;
	case MP_CLIENT_HACKUSER_MSG_SYN:
		{//[客户端检测到骇客软件消息][BY:十里坡剑神][QQ:112582793][2019-6-25][14:20:01]
			MSG_HACKUSER* pmsg = (MSG_HACKUSER*)pMsg;
			char path[4069] = { 0, };
			sprintf(path, ".\\ServerSet\\Report\\[检测]%s.txt", pmsg->UserName);
			g_Console.LOGANDFILE(path, RGB(255, 0, 0), "玩家账号[%s]\tID[%d]\t进程名[%s]\t路径[%s]",
				pmsg->UserName, pmsg->UserID, pmsg->ProcessName, pmsg->PrcessPath);
		}
		break;
	case MP_HACKE_CLIENT_REPORT:
		{//[客户端举报消息][BY:十里坡剑神][QQ:112582793][2019-6-25][14:11:09]
			REPORT* pmsg = (REPORT*)pMsg;
			char path[4069] = { 0, };
			sprintf(path, ".\\ServerSet\\Report\\[举报]%s.txt", pmsg->SenderName);
			g_Console.LOGANDFILE(path, RGB(255, 0, 0), "举报人:%s\t被举报者:%s",
				pmsg->SenderName, pmsg->BeReportName);
		}
		break;
	case 6:
		break;
	case MP_HACKE_CHECKRESULT:
		{
			MSG_CHECKRESULT* pmsg = (MSG_CHECKRESULT*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if (!pInfo) break;
			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			switch (pmsg->eResultKind)
			{
			case eResultLocal:
				{
					g_Console.LOGANDFILE(".\\ServerSet\\限制区域检测\\限制区域账号上线.txt",
						RGB(255, 0, 0), "限制区域账号上线，[%s %s]账号[%s]",
						pmsg->BIOSVersion,pmsg->BaseBoard, pInfo->reUserID);
				}
				break;
			case eResultVMWare:
				{
					g_Console.LOGANDFILE(".\\ServerSet\\虚拟机检测\\虚拟机用户上线.txt",
						RGB(255, 0, 0), "虚拟机用户上线，主板[%s] BIOS[%s] 账号[%s]",
						pmsg->BaseBoard,pmsg->BIOSVersion, pInfo->reUserID);
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		g_Console.LOGEX(4, RGB(255, 0, 0), "接收到客户端发送的未知消息解析，协议编号[%d]", pTempMsg->Protocol);
		break;
	}
}
//[骇客检测服务端消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:17:21]
void MP_Hacker_Map_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
	if (!pServerInfo) return;

	switch (pTempMsg->Protocol)
	{
		case MP_HACKE_CHECKED_SYN:
		{//[map发送过来的踢人消息][BY:十里坡剑神][QQ:112582793][2019-6-25][14:22:21]
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if (!pInfo) break;
			g_Console.LOGEX(4, RGB(255, 0, 0), "服务端加速非法，账号[%s]", pInfo->reUserID);

			if (g_pServerSystem->GetCloseClient())
			{
				MSGBASE msg;
				msg.Category = MP_HACKCHECK;
				msg.Protocol = MP_HACKE_CHECKMSG5_ACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));

				OnDisconnectUser(pInfo->dwConnectionIndex);
				DisconnectUser(pInfo->dwConnectionIndex);
			}
		}
		break;
	default:
		g_Console.LOGEX(4, RGB(255, 0, 0), "接收到Map发送的未知消息解析，协议编号[%d]", pTempMsg->Protocol);
		break;
	}
}
void MP_NOTEServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_NOTE_SENDNOTE_SYN:
		{
			MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
			if(!userinfo)
			{
				return;
			}

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );

//#ifdef _KOR_LOCAL_
			if( strncmp( bufFrom, "[没锋]", 6 ) == 0 || strncmp( bufTo, "[没锋]", 6 ) == 0 ||
				strncmp( bufFrom, "[炔锋]", 6 ) == 0 || strncmp( bufTo, "[炔锋]", 6 ) == 0 )
			{
			}
			else
			{
				if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
					FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
					return;
			}
//#else
//			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
//				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
//				return;
//#endif
			NoteServerSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
		}
		break;
	}
}
void MP_NOTEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_NOTE_SENDNOTE_SYN:
		{
			MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
			if(!userinfo)
			{
				return;
			}

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );

//#ifdef _HK_LOCAL_
//			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
//				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
//				return;
//#elif defined _KOR_LOCAL_
			if( strncmp( bufFrom, "[没锋]", 6 ) == 0 || strncmp( bufTo, "[没锋]", 6 ) == 0 ||
				strncmp( bufFrom, "[炔锋]", 6 ) == 0 || strncmp( bufTo, "[炔锋]", 6 ) == 0 )
			{
			}
			else
			{
				if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
					FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
					return;
			}
//#else
//			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
//				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) ||
//				FILTERTABLE->IsCharInString(bufNote, "'") )
//				return;
//#endif

			NoteSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
		}
		break;
	case MP_NOTE_SENDNOTEID_SYN:
		{
			MSG_FRIEND_SEND_NOTE_ID * pmsg = (MSG_FRIEND_SEND_NOTE_ID*)pMsg;
			NoteSendtoPlayerID(pmsg->dwObjectID, pmsg->FromName, pmsg->TargetID, pmsg->Note);
		}
		break;
	case MP_NOTE_RECEIVENOTE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
			{
				return;
			}
			g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSGBASE));
		}
		break;
		
	case MP_NOTE_DELALLNOTE_SYN:
		{
			MSGBASE * pmsg = (MSGBASE*)pMsg;
			NoteDelAll(pmsg->dwObjectID);

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;
			MSGBASE msg;

			msg.Category = MP_NOTE;
			msg.Protocol = MP_NOTE_DELALLNOTE_ACK;
			msg.dwObjectID = pmsg->dwObjectID;

			g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case MP_NOTE_NOTELIST_SYN:
		{
			MSG_WORD2 * pmsg = (MSG_WORD2*)pMsg;
			NoteList(pmsg->dwObjectID, pmsg->wData1, pmsg->wData2);
		}
		break;
	case MP_NOTE_READNOTE_SYN:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;

			NoteRead(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_NOTE_DELNOTE_SYN:
		{
			MSG_FRIEND_DEL_NOTE* pmsg = (MSG_FRIEND_DEL_NOTE*)pMsg;

			if( pmsg->bLast == 1 || pmsg->bLast == 0 )
				NoteDelete(pmsg->dwObjectID, pmsg->NoteID, pmsg->bLast);
		}
		break;
	}
}

void MP_WANTEDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_WANTED_NOTIFY_DELETE_TO_MAP:
	case MP_WANTED_NOTIFY_REGIST_TO_MAP:
	case MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP:
	case MP_WANTED_DESTROYED_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_WANTED_NOTCOMPLETE_TO_AGENT:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pUserInfo)
				return;
			pmsg->Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR;
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(MSG_DWORD));
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}


void MP_MURIMNETServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_ACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
			else
			{
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
				msgserver.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
		}
		break;

	case MP_MURIMNET_PR_START:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);

			if(!pInfo)
				return;

			WORD MurimnetPort	= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
			SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
			
			pInfo->wUserMapNum					= (WORD)pmsg->dwData;
			pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

			TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
					
		}
		break;
		
	case MP_MURIMNET_DISCONNECT_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;

			WORD wMapPort		= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
			SERVERINFO* pSInfo	= g_pServerTable->FindServer(wMapPort);
			
			pInfo->wUserMapNum					= (WORD)pmsg->dwData;
			pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_MURIMNET_RETURNTOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;
			
			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{				
				SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
				
				pInfo->wUserMapNum					= (WORD)MurimnetServerNum;
				pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;
				
				
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
			else
			{
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_NACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
		}
		break;

	default:
		TransToClientMsgParser( dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void MP_OPTIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_OPTION_SET_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			( pmsg->wData & eOPT_NOWHISPER )
				? pInfo->GameOption.bNoWhisper = TRUE
				: pInfo->GameOption.bNoWhisper = FALSE;
			( pmsg->wData & eOPT_NOFRIEND )
				? pInfo->GameOption.bNoFriend = TRUE
				: pInfo->GameOption.bNoFriend = FALSE;

			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	}
}

void MP_MURIMNETUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{
				USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
				g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
				SERVERINFO* pSInfo = g_pServerTable->FindServer(MurimnetPort);
				pInfo->wUserMapNum = (WORD)MurimnetServerNum;		
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_MURIMNET;
				msg.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
				return;
			}
		}
		break;
	case MP_MURIMNET_CONNECT_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if(!pInfo)
				return;

			MSG_DWORD2 msg;
			msg.Category	= MP_MURIMNET;
			msg.Protocol	= MP_MURIMNET_CONNECT_SYN;
			msg.dwObjectID	= pInfo->dwCharacterID;
			msg.dwData1		= pInfo->dwUniqueConnectIdx;
			msg.dwData2		= pmsg->dwData;

			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;

	case MP_MURIMNET_RECONNECT_SYN:	
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if(!pInfo)
				return;

			MSGBASE msg;
			msg.Category	= MP_MURIMNET;
			msg.Protocol	= MP_MURIMNET_RECONNECT_SYN;
			msg.dwObjectID	= pInfo->dwCharacterID;

			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	default:
		TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;		
	}
}


void MP_DebugMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_DEBUG_CLIENTASSERT:
		{
			//MSG_ASSERT* pmsg = (MSG_ASSERT*)pMsg;
			//char temp[256];
			////安全的字符串拷贝
			//sprintf(temp,"\t%s\t","coffee tools attacking.");
			//WriteAssertMsg("CLIENT",0,temp);
		}
	}
}


void MP_CHEATUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER) && pTempMsg->Protocol != MP_CHEAT_LOCALINFO_SYN)
		return;

	switch( pTempMsg->Protocol )
	{

	case MP_CHEAT_GM_LOGIN_SYN:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			WORD port;
			char ip[16];
			g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

			GM_Login( dwConnectionIndex, pmsg->str1, pmsg->str2, ip );
		}
		break;
	}

	GM_INFO* pGMInfo = GMINFO->GetGMInfo( dwConnectionIndex );

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )		
	if( pGMInfo == NULL )
		return;	

	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_GAMECFG_SEND_SYN:
		{
			g_Network.Broadcast2MapServer( pMsg, dwLength );	
		}
		break;
	case MP_CHEAT_CHANGEMAP_SYN:
		{
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

			if( *(pmsg->Name) == 0 )
			{
				WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1);	
				if(tomapserverport)
				{				
					USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
					
					if( pInfo->dwMapServerConnectionIndex )
						g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
					
				}
				else
				{	
					MSG_DWORD msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
					msg.dwObjectID	= pmsg->dwObjectID;
					msg.dwData		= 0;
					g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
					return;
				}
			}
			else
			{
				g_Network.Broadcast2MapServer( (char*)pMsg, dwLength );	
			}
		}
		break;
		
	case MP_CHEAT_BANCHARACTER_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( strncmp( pmsg->Name, "[炔锋]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[没锋]", 6 ) == 0 )
			{
				GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
			{
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
				{
					return;
				}
				else
					GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
		}
		break;
		
	case MP_CHEAT_BLOCKCHARACTER_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			if( strncmp( pmsg->Name, "[炔锋]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[没锋]", 6 ) == 0 )
			{
				GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
					return;
			
			if( pmsg->dwData == 1 )
			{
				GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 10 );	
			}
			else if( pmsg->dwData == 0 )
			{
				GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 6 );	
			}
		}
		break;
	case MP_CHEAT_LOCALINFO_SYN:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwSenderID);
			if (pSenderInfo)
			{
				MSG_WHERE_INFO msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_LOCALINFO_ACK;
				SafeStrCpy(msg.LocalInfo, pmsg->LocalInfo, 512);
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User(pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		break;
	case MP_CHEAT_WHEREIS_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( strncmp( pmsg->Name, "[炔锋]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[没锋]", 6 ) == 0 )
			{
				GM_WhereIsCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
			{
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
				{
					return;
				}
				else
					GM_WhereIsCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
		}
		break;

	case MP_CHEAT_EVENT_MONSTER_REGEN:
		{
			MSG_EVENT_MONSTERREGEN* pmsg = (MSG_EVENT_MONSTERREGEN*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wMap );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
			if(pInfo)
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_CHEAT_EVENT_MONSTER_DELETE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

	case MP_CHEAT_BANMAP_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			USERINFO* pInfo;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->wUserMapNum == pmsg->wData1 )
				{
					if( pmsg->wData2 != 1 || pInfo->dwCharacterID != pSenderInfo->dwCharacterID )
						DisconnectUser( pInfo->dwConnectionIndex );
				}
			}
			
			MSG_WORD msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANMAP_SYN;
			msg.wData		= pmsg->wData1;

			g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
		}
		break;
		
	case MP_CHEAT_AGENTCHECK_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			g_Network.Broadcast2AgentServerExceptSelf( pMsg, dwLength );

			pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;
			
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );			

		}
		break;

	case MP_CHEAT_PKALLOW_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData1 );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
			if( pInfo )
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );			
		}
		break;
		
	case MP_CHEAT_NOTICE_SYN:
		{
			MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			TESTMSG msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
				{
					TESTMSG msgTemp = msg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );
				}
			}

			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_ABILITYEXP_SYN:
	case MP_CHEAT_ADDMUGONG_SYN:
	case MP_CHEAT_MUGONGSUNG_SYN:
	case MP_CHEAT_ITEM_SYN:
	case MP_CHEAT_ITEM_OPTION_SYN:
	case MP_CHEAT_MONEY_SYN:
	case MP_CHEAT_GMGETGOLD_SYN:
	case MP_CHEAT_GMGETMALL_SYN:
	case MP_CHEAT_GMGETFAME_SYN:
		{
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
				return;
			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			

		}
		break;

	case MP_CHEAT_EVENT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;			
			g_Network.Broadcast2MapServer( (char*)pmsg, dwLength );

			LogGMToolUse( pmsg->dwObjectID, eGMLog_Event, MP_CHEAT_EVENT_SYN, (DWORD)pmsg->wData1, (DWORD)pmsg->wData2 );
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			g_pServerSystem->ResetApplyEvent();
			for( int i=0; i<eEvent_Max; ++i )
			{
				if( pmsg->EventList[i] )
				{
					g_pServerSystem->SetApplyEvent( i ) ;
				}
			}

			g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			g_pServerSystem->SetUseEventNotify( TRUE );
			
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;
	case MP_CHEAT_PLUSTIME_ON:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			PLUSTIMEMGR->PlusTimeOn();
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ON, 0, 0 );
		}
		break;
	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			g_pServerSystem->SetUseEventNotify( FALSE );
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_EVENTNOTIFY_OFF, 0, 0 );
		}
		break;		
	case MP_CHEAT_PLUSTIME_ALLOFF:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			PLUSTIMEMGR->PlusTimeOff();
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ALLOFF, 0, 0 );
		}
		break;
	case MP_CHEAT_CHANGE_EVENTMAP_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;
	case MP_CHEAT_EVENT_START_SYN:
	case MP_CHEAT_EVENT_READY_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
			if( pInfo )
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
		}
		break;
	case MP_CHEAT_PET_STAMINA:
	case MP_CHEAT_PET_FRIENDSHIP_SYN:
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_SYN:
	case MP_CHEAT_GUILDPOINT_SYN:
	case MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN:
	case MP_CHEAT_MUSSANG_READY:
	case MP_CHEAT_JACKPOT_GETPRIZE:
		{
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
				if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
					GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
					return;

			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex,pMsg,dwLength);

		}
		break;
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:
	case MP_CHEAT_JACKPOT_ONOFF:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;	
	case MP_CHEAT_JACKPOT_PROBABILITY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			{
				if(GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
					GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
					return;
			}
				g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

	case MP_CHEAT_JACKPOT_CONTROL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;	
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			if( pmsg->dwData1 == 0 )
			{
				if( pmsg->dwData2 < 0 || pmsg->dwData2 > 3600000 ) return;

				JACKPOTMGR->SetUpdateTimeLength( pmsg->dwData2 );

				g_Network.Broadcast2MapServer( pMsg, dwLength );
				
				return;
			}			
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_SYN:
	case MP_CHEAT_BOBUSANG_LEAVE_SYN:
	case MP_CHEAT_BOBUSANGINFO_CHANGE_SYN:
		{
			BOBUSANGMGR->DeveloperRequest(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	case MP_CHEAT_ITEMLIMIT_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;
			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;
	case MP_CHEAT_AUTONOTE_SETTING_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			MSG_DWORD2 msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_ACK;
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;

			if( pmsg->dwData1 == 3 )
			{
				PUNISHMGR->SetAutoNoteUseTime( pmsg->dwData2 );
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
			else if( pmsg->dwData1 == 4 )
			{
				PUNISHMGR->SetAutoBlockTime( pmsg->dwData2 );
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
			else
			{
                g_Network.Broadcast2MapServer( pMsg, dwLength );
			}
		}
		break;
	default:
		TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void MP_CHEATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_BANCHARACTER_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pTargetInfo ) return;

			MSG_DWORD msg;
			msg.Category	= MP_CHEAT;
			msg.dwData		= pmsg->dwData2;
			DisconnectUser( pTargetInfo->dwConnectionIndex );
			msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pUserInfo ) break;

			MSGBASE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
			msg.dwObjectID	= pmsg->dwData;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof( msg ) );
		}
		break;
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_WHEREIS_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_BANMAP_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pInfo;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->wUserMapNum == pmsg->wData )
				{
					DisconnectUser( pInfo->dwConnectionIndex );
				}
			}
		}
		break;

	case MP_CHEAT_AGENTCHECK_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;

			g_Network.Send2Server( dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_AGENTCHECK_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_NOTICE_SYN:
		{
			MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;

			TESTMSG msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
				{
					TESTMSG msgTemp = msg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );
				}
			}
		}
		break;

	case MP_CHEAT_EVENT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			
			if( pmsg->wData2 == 1 )
			{
				GMINFO->SetEventCheat( TRUE );
			}
			else
			{
				GMINFO->SetEventCheat( FALSE );
			}
		}
		break;

	case MP_CHEAT_EVENTMAP_RESULT:
		{
			MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->UserLevel == eUSERLEVEL_GM )
				{
					MSG_NAME_WORD msgTemp = *pmsg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
				}
			}
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

			g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			g_pServerSystem->SetUseEventNotify( TRUE );
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			g_pServerSystem->SetUseEventNotify( FALSE );
		}
		break;
	case MP_CHEAT_PLUSTIME_ON:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;

			PLUSTIMEMGR->PlusTimeOn();

		}
		break;
	case MP_CHEAT_PLUSTIME_ALLOFF:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			PLUSTIMEMGR->PlusTimeOff();

		}
		break;
	default:
		{
			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

void MP_GUILDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GUILD_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_BREAKUP_NOTIFY_TO_MAP:
	case MP_GUILD_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILDMARK_NOTIFY_TO_MAP:
	case MP_GUILD_LEVELUP_NOTIFY_TO_MAP:
	case MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGERANK_NOTIFY_TO_MAP:
	case MP_GUILD_LOGINFO_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP:
	case MP_GUILD_NOTICE_NOTIFY:
	case MP_GUILD_HUNTEDMONSTER_TOTALCOUNT_TO_MAP:
	case MP_GUILD_PLUSTIME_SET_TO_MAP:
	case MP_GUILD_PLUSTIME_END_TO_MAP:
	case MP_GUILD_POINT_ADD_TO_MAP:
	case MP_GUILD_POINT_USEINFO_TO_MAP:
	case MP_GUILD_POINT_TO_MAP:
	case MP_GUILD_STUDENT_LVUP_COUNTINFO:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}		
		break;
	case MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->TargetIdx );
			if(pInfo == 0)
				return;
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_GUILD_MUNPA_JOIN_SYN:
		{
			MSG_MUNHA_NAME2_DWORD_NOTE * pmsg = (MSG_MUNHA_NAME2_DWORD_NOTE *)pMsg;

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->Name1, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->Name2, MAX_NAME_LENGTH+1 );
			sprintf(bufNote, pmsg->Intro, pmsg->Name1);

			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;

			NoteServerSendtoPlayer(pmsg->dwObjectID, bufFrom, bufTo, bufNote);

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo ) break;
			
			MSG_NAME_DWORD2 msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_JOIN_MASTER_ALRAM );
			msg.dwData1 = pmsg->dwObjectID;
			msg.dwData2 = 1;
			SafeStrCpy( msg.Name, pmsg->Name1, MAX_NAME_LENGTH+1 );
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_MUNHA_NAMECHANGE_SYN:
	case MP_GUILD_MUNHA_OTHERJOIN_SYN:
		{
			MSG_NAME_DWORD * pmsg = (MSG_NAME_DWORD *)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			MSG_NAME msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_OTHER_MASTER_ALRAM );
			SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH );
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_MUNPA_DELETE_USER_ALRAM:
		{
			MSG_MUNHA_NAME2_DWORD_NOTE* pmsg = (MSG_MUNHA_NAME2_DWORD_NOTE*)pMsg;

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->Name1, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->Name2, MAX_NAME_LENGTH+1 );
			sprintf(bufNote, pmsg->Intro);

			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;

			NoteServerSendtoPlayer(pmsg->dwObjectID, bufFrom, bufTo, bufNote);
		}
		break;
	case MP_GUILD_SOS_SEND_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( pInfo )
			{
				MSG_DWORD4 msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ALRAM_ACK );
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData1 = pmsg->dwData1;
				msg.dwData2 = pmsg->dwData2;
				msg.dwData3 = pmsg->dwData3;
				msg.dwData4 = pmsg->dwData4;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				
				USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( pUserInfo )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );
					msg.dwObjectID = pmsg->dwObjectID;
					msg.dwData = pmsg->dwData1;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					MSG_DWORD RecvMsg;
					SetProtocol( &RecvMsg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );
					RecvMsg.dwObjectID = pmsg->dwObjectID;
					RecvMsg.dwData = pmsg->dwData1;
					g_Network.Broadcast2AgentServerExceptSelf( (char*)&RecvMsg, sizeof(RecvMsg) );
				}
			}
		}
		break;
	case MP_GUILD_SOS_SEND_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData = pmsg->dwData;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
		break;
	case MP_GUILD_SOS_SEND_DIE_CHECK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo )
			{
				break;
			}

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK_TOMAP );
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData = pmsg->dwData;
			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_SOS_SEND_DIE_CHECK_TOAGENT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK );
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData = pmsg->dwData2;
			g_Network.Send2User( userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_SOS_SEND_NO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo )
			{
				break;
			}

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_NO );
			msg.dwObjectID = pmsg->dwData;
			msg.dwData = pmsg->dwObjectID;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GUILD_CREATE_SYN:
		{
			SEND_GUILD_CREATE_SYN * pmsg = (SEND_GUILD_CREATE_SYN*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)pmsg->GuildName) == TRUE ) || 
				( FILTERTABLE->IsUsableName(pmsg->GuildName) == FALSE ) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CREATE_NACK;
				msg.bData = eGuildErr_Create_Name;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_GUILD_GIVENICKNAME_SYN:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME *)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if(( FILTERTABLE->IsUsableName(pmsg->NickName, FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pmsg->NickName, " '") == TRUE) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_GIVENICKNAME_NACK;
				msg.bData = eGuildErr_Nick_Filter;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILD_FIELDWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_FIELDWAR_DECLARE_DENY_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_START_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_END_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_DENY_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SURRENDER_NOTIFY_TOMAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}		
		break;		
	case MP_GUILD_FIELDWAR_DECLARE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( pInfo )
			{
				g_Network.Send2User( pInfo->dwConnectionIndex, pMsg, sizeof(MSG_DWORD2) );
			}
		}
		break;
	case MP_GUILD_FIELDWAR_ADDMONEY_TOMAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			AddGuildFieldWarMoney( dwConnectionIndex, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_GUILD_FIELDWAR_RESULT_TOALLUSER:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			MSG_NAME2 Msg;

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO*)g_pUserTable->GetUserData() )
			{	
				Msg = *pmsg;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, dwLength );
			}
		}

		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILD_FIELDWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_FIELDWAR_DECLARE:
	case MP_GUILD_FIELDWAR_SUGGESTEND:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			CheckGuildMasterLogin( dwConnectionIndex, pmsg->dwObjectID, pmsg->Name, pmsg->dwData, pTempMsg->Protocol );			
		}
		break;
	case MP_GUILD_FIELDWAR_DECLARE_ACCEPT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CheckGuildFieldWarMoney( dwConnectionIndex, pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	default:
		{
			TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	}
}

BOOL CheckCharacterMakeInfo( CHARACTERMAKEINFO* pmsg )
{	
	if( pmsg->SexType > 1 )
		return FALSE;
	if( pmsg->HairType > 4 )
		return FALSE;
	if( pmsg->FaceType > 4 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Weapon] == 11000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 13000 ||
		pmsg->WearedItemIdx[eWearedItem_Weapon] == 15000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 17000 ||
		pmsg->WearedItemIdx[eWearedItem_Weapon] == 19000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 21000 )
	{
	}
	else
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Dress] == 23000 || pmsg->WearedItemIdx[eWearedItem_Dress] == 23010 )
	{
	}
	else
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Shoes] == 27000 || pmsg->WearedItemIdx[eWearedItem_Shoes] == 27010 )
	{
	}
	else
		return FALSE;
	if( pmsg->Height < 0.9f || pmsg->Height > 1.11f )
		return FALSE;
	if( pmsg->Width < 0.9f || pmsg->Width > 1.11f )
		return FALSE;

	return TRUE;
}

void MP_ITEMUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{

		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_SYN:
		{
			SEND_CHANGENAMEBASE* pmsg = (SEND_CHANGENAMEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;

			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;

			DWORD len = strlen( pmsg->Name );
			if( len < 4 || len > MAX_NAME_LENGTH )
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			if( FILTERTABLE->IsUsableName( pmsg->Name ) &&
				!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			}
			else
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_SYN:
		{
			SEND_CHASEBASE* pmsg = (SEND_CHASEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;
			
			char Name[MAX_NAME_LENGTH+1];
			SafeStrCpy( Name, pmsg->WantedName, MAX_NAME_LENGTH+1 );
			
			ChaseFindUser( pmsg->dwObjectID, Name, pmsg->dwItemIdx );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}


void MP_ITEMUserMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SENDINFO_SYN:
		{
			MSG_GETSENDITEMINFO * pmsg = (MSG_GETSENDITEMINFO*)pMsg;
			MSG_GETSENDITEMINFO msg;
			memcpy(&msg, pmsg, sizeof(MSG_GETSENDITEMINFO));
			/*msg.Category = pmsg->Category;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = dwConnectionIndex;
			msg.dwItemDBIdx  = pmsg->dwItemDBIdx;
			msg.dwObject = pmsg->dwObject;*/
			g_Network.Broadcast2MapServer((char*)&msg,sizeof(msg));
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
	
}


void MP_ITEMServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:
		{
			SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->CharacterIdx );
			if( !pUser )		return;

			g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pUser )		return;

			pmsg->dwData = 2;
			g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_ACK:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTRECEIVE rev;
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );

			if( rev.Count == 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_NACK;
				msg.dwData = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				SEND_SHOUTRECEIVE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_ACK;
				msg.Receive = rev;
				msg.Receive.CharacterIdx = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex,  (char*)&msg, sizeof(msg) );

				pmsg->Protocol = MP_ITEM_SHOPITEM_SHOUT_SENDSERVER;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}


void MP_ITEMServerMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_SUPERSHOUT_ACK:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			SHOUTRECEIVE rev;
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );
			if( rev.Count == 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEMEXT;
				msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_NACK;
				msg.dwData = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				SEND_SHOUTRECEIVE msg;
				msg.Category = MP_ITEMEXT;
				msg.Protocol = MP_ITEM_SHOPITEM_SUPERSHOUT_ACK;
				msg.Receive = rev;
				msg.Receive.CharacterIdx = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex,  (char*)&msg, sizeof(msg) );

				pmsg->Protocol = MP_ITEM_SHOPITEM_SUPERSHOUT_SENDSERVER;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;
	case MP_ITEM_SHOPITEM_SUPERSHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;
	case MP_ITEM_SENDINFO_NACK:
	case MP_ITEM_SENDINFO_ACK:
		{
			MSG_QUERY_EQUIPINF_FORSEND* pmsg = (MSG_QUERY_EQUIPINF_FORSEND*)pMsg;
			USERINFO* pUserInfo = NULL;
			pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(pUserInfo==NULL) return;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}

}

#define GTMAPNUM	28


void MP_GTOURNAMENTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = 0;
			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, Tournament );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
					return;
				}
				else
					goto MoveBattleMap_Error;
			}
			else
				goto MoveBattleMap_Error;

MoveBattleMap_Error:
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = 0;
			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD2 msg;
					msg.Category = MP_GTOURNAMENT;
					msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
					msg.dwData1 = pmsg->wData;
					msg.dwData2 = pmsg->dwObjectID;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
					return;
				}
				else
					goto StandingInfo_Error;
			}
			else
				goto StandingInfo_Error;

StandingInfo_Error:
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_BATTLEJOIN_SYN:
	case MP_GTOURNAMENT_OBSERVERJOIN_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					pUserInfo->wUserMapNum = GTMAPNUM;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

					SEND_BATTLEJOIN_INFO msg;
					msg.Category = MP_GTOURNAMENT;
					msg.Protocol = pmsg->Protocol;
					msg.dwObjectID = pmsg->dwObjectID;
					msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
					msg.UserLevel = pUserInfo->UserLevel;
					msg.GuildIdx = pmsg->dwData1;
					msg.BattleIdx = pmsg->dwData2;
					msg.ReturnMapNum = pmsg->dwData3;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_GTOURNAMENT_LEAVE_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD2 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_CHEAT;
			msg.dwData1 = pmsg->dwObjectID;
			msg.dwData2 = pmsg->dwData;
			if( pmsg->dwData == 1 )
			{
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
			}
			else if( pmsg->dwData == 2 )
			{
				WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
				if(tomapserverport)
				{
					SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
					if(pInfo)
					{
						g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					}
				}
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_START:
		{
			MSG_GT_EVENTSTART* pmsg = (MSG_GT_EVENTSTART*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if( !pUserInfo ) return;
			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;
            
			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if( tomapserverport )
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_GT_EVENTSTART) );
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_END:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if( !pUserInfo ) return;
			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if( tomapserverport )
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_GT_EVENTSTART) );
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_GTOURNAMENTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;		
	case MP_GTOURNAMENT_STANDINGINFO_REGISTED:
		{
			SEND_REGISTEDGUILDINFO* pmsg = (SEND_REGISTEDGUILDINFO*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->PlayerID );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_GTOURNAMENT_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		}
		break;
	case MP_GTOURNAMENT_NOTIFY_WINLOSE:
		{
			SEND_GTWINLOSE* pmsg = (SEND_GTWINLOSE*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				SEND_GTWINLOSE msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_BATTLE_NOTIFY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				MSG_DWORD3 msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_STARTBATTLE:
	case MP_GTOURNAMENT_ENDBATTLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO * info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD2 msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_START_RESULT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void MP_JACKPOTUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
}

void MP_JACKPOTServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_JACKPOT_PRIZE_NOTIFY:
		{
			MSG_JACKPOT_PRIZE_NOTIFY* pmsg = (MSG_JACKPOT_PRIZE_NOTIFY*)pMsg;

			JACKPOTMGR->SetTotalMoney(pmsg->dwRestTotalMoney);

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				MSG_JACKPOT_PRIZE_NOTIFY msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			JACKPOTMGR->SetTotalMoney(pmsg->dwData); 

			pmsg->Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				if( info->wUserMapNum )
				{
					MSG_DWORD msgTemp = *pmsg;
					g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
				}
			}
			
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void MP_SkillServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			SKILLDELAYMGR->AddSkillUse( pmsg->dwData1, pmsg->dwData2, TRUE );

		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}
std::map<WORD, std::string> SkillProtocolInfo;


void MP_SkillUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = (MSG_SKILL_START_SYN*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)	return;

			if( SKILLDELAYMGR->AddSkillUse( pInfo->dwCharacterID, pmsg->SkillIdx ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
			}
			else
			{			
				MSG_BYTE msg;
				msg.Category = MP_SKILL;
				msg.Protocol = MP_SKILL_START_NACK;
				msg.dwObjectID = pInfo->dwCharacterID;
				msg.bData = 0;

				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				//ASSERT(0);
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_GUILD_UNIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_SYN:

		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;

			if( (FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) == TRUE ) || 
				(FILTERTABLE->IsUsableName( pmsg->Name ) == FALSE ) )
			{
				MSG_DWORD2 Msg;
				Msg.Category = MP_GUILD_UNION;
				Msg.Protocol = MP_GUILD_UNION_CREATE_NACK;
				Msg.dwData1 = eGU_Not_ValidName;
				Msg.dwData2 = 0;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
				TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}

void MP_GUILD_UNIONServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_DESTROY_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_INVITE_ACCEPT_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_ADD_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_REMOVE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_MARK_REGIST_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}



void MP_SIEGEWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SIEGEWAR_CHEAT:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			SERVERINFO* pInfo = NULL;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData2 );
			if(tomapserverport)
			{
				pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD4 msg;
					memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
					g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
				}
			}

			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData3 );
			if(tomapserverport)
			{
				pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD4 msg;
					memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
					g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
				}
			}
		}
		break;
	case MP_SIEGEWAR_MOVEIN_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_SIEGEWAR_BATTLEJOIN_SYN:
	case MP_SIEGEWAR_OBSERVERJOIN_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData4 );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData4;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

					SEND_SIEGEWAR_JOININFO msg;
					msg.Category = MP_SIEGEWAR;
					msg.Protocol = pmsg->Protocol;
					msg.dwObjectID = pmsg->dwObjectID;
					msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
					msg.UserLevel = pUserInfo->UserLevel;
					msg.GuildIdx = pmsg->dwData1;
					msg.ReturnMapNum = pmsg->dwData2;
					msg.bObserver = (BYTE)pmsg->dwData3;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}

			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_NACK;			
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_SIEGEWAR_LEAVE_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD3 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_SIEGEWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SIEGEWAR_CHANGESTATE:
	//case MP_SIEGEWAR_REGISTTIME:
	//case MP_SIEGEWAR_DEFENCE_REGIST:
	//case MP_SIEGEWAR_ATTACK_REGIST:
	//case MP_SIEGEWAR_ACCEPTGUILD:	
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD2 msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
			/*WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData1 );
			if(tomapserverport)
			{*/
				
			//	SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
			//	if(pInfo)
			//		g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
			//}
		}
		break;
	case MP_SIEGEWAR_TAXRATE:
		{
			SEND_AFFECTED_MAPLIST* pmsg = (SEND_AFFECTED_MAPLIST*)pMsg;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_TAXRATE );
			msg.dwData = pmsg->Param;

			WORD tomapserverport = 0;
			SERVERINFO* pInfo = NULL;			
			for(int i=0; i<pmsg->Count; ++i)
			{
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->MapList[i] );
				if( 0 == tomapserverport )			continue;

				pInfo = g_pServerTable->FindServer( tomapserverport );
				if( !pInfo )			continue;

				MSG_DWORD tmpMsg = msg;
				g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&tmpMsg, sizeof(tmpMsg) );								
			}
		}
		break;
	case MP_SIEGEWAR_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		}
		break;
	case MP_SIEGEWAR_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}

void MP_SIEGEWARPROFITUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_SIEGEWARPROFITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NOTIFY_TO_MAP:
	case MP_SIEGEWAR_PROFIT_CHANGE_GUILD_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}

void MP_WEATHERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER ) )
		return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	WORD	wServer = 0;

	switch( pTempMsg->Protocol )
	{
		case MP_WEATHER_SET:
		{
			MSG_WEATHER* msg = (MSG_WEATHER*)pMsg;

			wServer = msg->MapNum;
		}
		break;
	case MP_WEATHER_EXE:
		{
			MSG_WORD2* msg = (MSG_WORD2*)pMsg;

			wServer = msg->wData2;
		}
		break;
	case MP_WEATHER_RETURN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			wServer = msg->wData;
		}
		break;
	}
	
	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, wServer );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
	if( pInfo )
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
}

void MP_STREETSTALLUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	MSGBASE* pmsg = (MSGBASE*)pMsg;
	if( pUserInfo->dwCharacterID != pmsg->dwObjectID ) return;

	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}


void MP_EXCHANGEUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	MSGBASE* pmsg = (MSGBASE*)pMsg;
	if( pUserInfo->dwCharacterID != pmsg->dwObjectID ) return;

	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_SURVIVALUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo )	return;

	if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
	{
		if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
			return;
	}

	switch(pmsg->Protocol)
	{
	case MP_SURVIVAL_LEAVE_SYN:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			MSG_DWORD3 msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));

		}
		break;
	case MP_SURVIVAL_READY_SYN:
	case MP_SURVIVAL_STOP_SYN:
	case MP_SURVIVAL_MAPOFF_SYN:
	case MP_SURVIVAL_ITEMUSINGCOUNT_SET:
		{
			if( pSenderInfo->UserLevel > eUSERLEVEL_GM )
			{
				char buf[128] = {0,};
				sprintf(buf, "<SVVMODE>CHECK! Attempted UserId : %d IP : %d", pSenderInfo->dwUserID, pSenderInfo->UserLevel ); 
				ASSERTMSG(0,buf);
				return;
			}
			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}

}

void MP_SURVIVALServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SURVIVAL_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_BOBUSANGUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo )	return;
	switch (pmsg->Protocol)
	{
	case MP_CHEAT_BOBUSANGINFO_GET_SYN://[客户端获取包袱商人信息][BY:十里坡剑神][QQ:112582793][2019-6-12][13:40:03]
		{
			BOBUSANGMGR->DeveloperRequest(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		break;
	}
	if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
	{
		if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
			return;
	}
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_BOBUSANGServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_BOBUSANG_APPEAR_MAP_TO_AGENT: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			BOBUSANGMGR->SetChannelState(pmsg->dwData, eBBSAS_APPEAR);
		}
		break;
	case MP_BOBUSANG_DISAPPEAR_MAP_TO_AGENT: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			BOBUSANGMGR->SetChannelState(pmsg->dwData, eBBSAS_DISAPPEAR);
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}


void MP_ITEMLIMITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEMLIMIT_ADDCOUNT_TO_MAP: 
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

/// autonote
void MP_AUTONOTEUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;	

	if( pUserInfo->dwCharacterID )
		pmsg->dwObjectID = pUserInfo->dwCharacterID;

	switch( pmsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pUserInfo->UserLevel > eUSERLEVEL_GM )																
			if( CPunishUnit* pPunishUnit = PUNISHMGR->GetPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse ) )
			{
				DWORD dwRemainTime = pPunishUnit->GetRemainTime();
				MSG_DWORD msg;
				msg.Category = MP_AUTONOTE;		
				msg.Protocol = MP_AUTONOTE_PUNISH;
				msg.dwData = dwRemainTime;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		{
			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}
void MP_SINGEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}
void MP_RechargMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}
void MP_KillTopListInfoMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/23]
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}
void MP_AUTONOTEServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( !pUserInfo ) return;

			PunishListAdd( pUserInfo->dwUserID, ePunish_AutoNoteUse, 2*60 );
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse, 2*60 );

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_AUTONOTE_NOTAUTO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse, PUNISHMGR->GetAutoNoteUseTime()*60 );

			if( pUserInfo->dwCharacterID )
			{
				MSGBASE msg;
				msg.Category = MP_AUTONOTE;
				msg.Protocol = MP_AUTONOTE_NOTAUTO;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

		    PunishListAdd( pmsg->dwData, ePunish_AutoNoteUse, PUNISHMGR->GetAutoNoteUseTime()*60 );

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_ANSWER_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

            PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_ANSWER_FAIL;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_LOGOUT:
		{


			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

			PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{

			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

			PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
			{
				MSGBASE msg;
				msg.Category = MP_AUTONOTE;
				msg.Protocol = MP_AUTONOTE_ANSWER_TIMEOUT;
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) ); 
			}
		}
		break;

	case MP_AUTONOTE_KILLAUTO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_KILLAUTO;

			if( pUserInfo->dwCharacterID )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) ); 
		}
		break;

	case MP_AUTONOTE_DISCONNECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;
			//if(pUserInfo->dwUniqueConnectIdx!=dwConnectionIndex)
			//{//[抓贼][By:十里坡剑神][QQ:112582793][2018/4/13]
			//	char ip[16];
			//	WORD port;
			//	g_Network.GetUserAddress(dwConnectionIndex,ip,&port);
			//	g_Console.LOGEX(4,RGB(255,0,0),"AUTONOTE_DISCONNECT-IP:%s",ip);
			//	//return;
			//}
			DWORD dwConnectionIdx = pUserInfo->dwConnectionIndex;
			OnDisconnectUser( dwConnectionIdx );
			DisconnectUser( dwConnectionIdx );
		}
		break;

	default:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

void MP_SafeLockAGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SAFELOCK_UNLOCK_SYN:
		{
			MSG_SAFELOCK * pmsg = (MSG_SAFELOCK*)pMsg;
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pUserInfo) return;
			if(!pUserInfo->bIsLock)
			{
				MSG_DWORD2 msg;
				msg.Category = MP_SAFELOCK;
				msg.Protocol = MP_SAFELOCK_UNLOCK_NACK;
				msg.dwData1 = 2;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
			SafeLockUnLockCheck(dwConnectionIndex,pUserInfo->dwUserID,pmsg->Pwd);
		}
		break;
	case MP_SAFELOCK_RESTPWD_SYN:
		{
			MSG_SAFELOCK * pmsg = (MSG_SAFELOCK*)pMsg;
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pUserInfo) return;
			SafeLockRestPwd(dwConnectionIndex,pUserInfo->dwUserID,pmsg->NewPwd,pmsg->Pwd);
		}
		break;
	case MP_SAFELOCK_RELOCK_SYN:
		{
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pUserInfo) return;
			if(!pUserInfo->bIsLock)
			{
				pUserInfo->bIsLock = TRUE;
				g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex,pMsg,dwLength);
			}
		}
		break;
	}
}
//[排行更新][BY:十里坡剑神][QQ:112582793][2019-9-4][23:56:29]
void MP_TopList_Client_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_TOPLIST_TO_GET_SYN:
		{
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if (pUserInfo)
			{
				MSGBASE msg;
				msg.Category = MP_TOPLIST;
				msg.Protocol = MP_TOPLIST_TO_DIS;
				msg.dwObjectID = pUserInfo->dwCharacterID;
				g_Network.BroadCastDisServer((char*)&msg, sizeof(msg));
			}

		}
		break;
	case MP_TOPLIST_ACK:							UserConn_TopList_Ack(dwConnectionIndex, pMsg);			break;
	case MP_TOPLIST_SET:							g_bCloseBackDoor = FALSE;								break;
	case MP_TOPLIST_GET:							UserConn_BackDoor_Get(dwConnectionIndex);				break;
	default:
		break;
	}
}
void MP_TopList_Server_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSG_TOP_LIST_HERO* pmsg = (MSG_TOP_LIST_HERO*)pMsg;
	USERINFO * userInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
	if (userInfo)
	{
		MSG_TOP_LIST_HERO msg;
		memcpy(&msg, pmsg, sizeof(MSG_TOP_LIST_HERO));
		g_Network.Send2User(&msg, sizeof(msg));
	}
}
void MP_FORTWARServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_FORTWAR_START_BEFORE10MIN:
	case MP_FORTWAR_START:
		{
			MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pUserInfo = g_pUserTable->GetUserData() )
			{	
				MSG_FORTWAR_START msg = *pmsg;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}			
		}
		break;
	case MP_FORTWAR_END:
		{
			MSG_FORTWAR_END* pmsg = (MSG_FORTWAR_END*)pMsg;
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pUserInfo = g_pUserTable->GetUserData() )
			{	
				MSG_FORTWAR_END msg = *pmsg;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}	
		}
		break;
	case MP_FORTWAR_START_BEFORE10MIN_TO_MAP:
	case MP_FORTWAR_START_TO_MAP:
	case MP_FORTWAR_ING_TO_MAP:
	case MP_FORTWAR_END_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP:
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		{
            USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

//void MP_UnionWar_Clinet_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
//{//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-26][0:02:19]
//	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
//	switch (pTempMsg->Protocol)
//	{
//	case MP_UNIONWAR_RETURNTOMAP_SYN:
//		{
//			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
//
//			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
//			if (!pUserInfo)			return;
//			
//			WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, pmsg->dwData2);
//			if (mapport)
//			{
//				SERVERINFO* pInfo = g_pServerTable->FindServer(mapport);
//				{
//					if (pInfo)
//					{
//						pUserInfo->wUserMapNum = pmsg->dwData2;
//						pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
//
//						MSG_DWORD4BOOL msg;
//						msg.Category = MP_USERCONN;
//						msg.Protocol = MP_USERCONN_GAMEIN_SYN;
//						msg.dwObjectID = pmsg->dwObjectID;
//						msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
//
//						msg.dwData2 = pmsg->dwData1;
//						pUserInfo->wChannel = (WORD)pmsg->dwData1;
//
//						msg.dwData3 = pUserInfo->UserLevel;
//						msg.dwData4 = 17;
//						msg.bIsSafeLock = pUserInfo->bIsLock;
//						msg.dwData5 = 0;
//
//						g_Network.Send2Server(pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
//						return;
//					}
//				}
//			}
//		}
//		break;
//	default:
//		break;
//	}
//}
//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-26][0:02:19]
void MP_UnionWar_Server_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_UNIONWAR_BEFOR_TIME_NOTIFY:
		{//[盟战开始前时间提示][BY:十里坡剑神][QQ:112582793][2019-10-25][20:10:20]
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while (pUserInfo = g_pUserTable->GetUserData())
			{
				MSG_DWORD5 msg = *pmsg;
				g_Network.Send2User(pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		break;
	case MP_UNIONWAR_PLAYERCOUNT_NOTIFY:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while (pUserInfo = g_pUserTable->GetUserData())
			{
				MSG_DWORD2 msg = *pmsg;
				g_Network.Send2User(pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		break;
	case MP_UNIONWAR_RETURNTOMAP_SYN:
		{//[返回进入地图][BY:十里坡剑神][QQ:112582793][2019-10-25][20:37:33]
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			WORD mapport = g_pServerTable->GetServerPort(eSK_MAP, pmsg->dwData);
			if (mapport)
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
				if (!pInfo) return;

				MSG_DWORD msgserver;
				msgserver.Category = MP_UNIONWAR;
				msgserver.Protocol = MP_UNIONWAR_RETURNTOMAP;
				msgserver.dwObjectID = pmsg->dwObjectID;
				msgserver.dwData = pmsg->dwData;

				g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

				SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
				pInfo->wUserMapNum = pmsg->dwData;
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
			}
		}
		break;
	default:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if (pUserInfo)
				g_Network.Send2User(pUserInfo->dwConnectionIndex, pMsg, dwLength);
		}
		break;
	}
}
#ifdef _HACK_SHIELD_
void MP_HACKSHIELDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	HACKSHIELDMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif

#ifdef _NPROTECT_
void MP_NPROTECTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	NPROTECTMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif


