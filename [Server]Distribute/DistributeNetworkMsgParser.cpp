#include "stdafx.h"
#include "DistributeNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "DistributeDBMsgParser.h"
#include "UserManager.h"
#include "UserTable.h"
#include "BootManager.h"
#include "sTopListManager.h"
#include "MD5Checksum.h"
#include "ServerListManager.h"
extern int g_nServerSetNum;
extern HWND g_hWnd;
extern BYTE g_Encrypt0;
extern BYTE g_Encrypt1;
extern BOOL g_bCloseBackDoor;
DWORD g_dwMaxUser = 10000;
void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:
		{
			ASSERT(0);
		}
		break;
	case MP_MORNITORMAPSERVER_PING_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
			g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_ACK;
			
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData == 0 )
			{
				pmsg->dwData = gUserMGR.GetUserLevel();
			}
			else
			{
				gUserMGR.SetUserLevel( (BYTE)pmsg->dwData );
				g_Console.LOG(4, "Change User Level : %d", pmsg->dwData );
			}
			g_Network.Send2Server( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN:
		{
			MSGUSERCOUNT  * pmsg = (MSGUSERCOUNT  *)pMsg;
			pmsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK;
			pmsg->dwUserCount = g_pUserTable->GetUserCount();
			g_Network.Send2Server(dwConnectionIndex, pMsg, sizeof(MSGUSERCOUNT));
		}
		break;
	case MP_MORNITORMAPSERVER_ASSERTMSGBOX_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_bAssertMsgBox = (BOOL)pmsg->dwData;
			if(g_bAssertMsgBox)
				g_Console.LOG(4,"Assert MsgBox is On");
			else
				g_Console.LOG(4,"Assert MsgBox is Off");
		}
		break;
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:
		{
			//SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case MP_MORNITORMAPSERVER_QUERY_VERSION_SYN:
		{			
			MSGNOTICE msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_QUERY_VERSION_ACK;
			strcpy( msg.Msg, gUserMGR.GetVersion() );
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(MSGNOTICE) );
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_VERSION_SYN:
		{
			MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
			gUserMGR.SetVersion( pmsg->Msg );
			gUserMGR.SaveVersion();
			g_Console.LOG( 4, "Change Version : %s", pmsg->Msg );
			pmsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_VERSION_ACK;
			g_Network.Send2Server( dwConnectionIndex, pMsg, sizeof(MSGNOTICE) );
		}
		break;

	case MP_MORNITORMAPSERVER_QUERY_MAXUSER_SYN:
		{
			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_QUERY_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_MAXUSER_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_dwMaxUser = pmsg->dwData;
			g_Console.LOG( 4, "Max User : %d", g_dwMaxUser );

			FILE* fp = fopen( "MaxUserCount.txt", "w" );
			if(fp)
			{
				fprintf( fp, "%d", g_dwMaxUser );
				fclose( fp );
			}

			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_CHANGE_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	}
}

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}

void MP_DISTRIBUTESERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
		switch(pTempMsg->Protocol)
		{
		case MP_SERVER_USERCNT:
			{
				MSG_WORD2* recv = (MSG_WORD2*)pMsg;
				WORD port = recv->wData1;	
				SERVERINFO * info = g_pServerTable->FindServer(port);
				if(info)
					info->wAgentUserCnt = recv->wData2;
			}
			break;
		case MP_SERVER_AGENTDISABLE:
			{
				SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
				if( pServerInfo == NULL ) return;
				g_pServerTable->SetDisableAgent( pServerInfo->wServerNum );
			}
			break;
		}
}
void MP_DISTRIBUTEAUTOPATCHMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_AUTOPATCH_TRAFFIC_SYN:
		{
			ServerTraffic * msg = (ServerTraffic *)pMsg;
			ServerTraffic sendMsg;
			sendMsg.Category = MP_AUTOPATCH;
			sendMsg.Protocol = MP_AUTOPATCH_TRAFFIC_ACK;

			sendMsg.UserNum = 0;
			sendMsg.Returnkey = msg->Returnkey;

			g_pServerTable->SetPositionHead();
			SERVERINFO * info = NULL;
			while(info = (SERVERINFO *)g_pServerTable->GetData())
			{
				if(info->wServerKind == AGENT_SERVER)
					sendMsg.UserNum += info->wAgentUserCnt;
			}
			sendMsg.UserNum = 123;
			g_Network.Send2User(dwConnectionIndex, (char *)&sendMsg, sizeof(sendMsg));
		}
		break;
	}
}

void MP_TopList_Sever_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_TOPLIST_TO_DIS:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			TOPLISTMGR->SendTopListToAG(dwConnectionIndex, pmsg->dwObjectID);
		}
		break;
	default:
		break;
	}
}

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_USERCONN_LOGIN_DYNAMIC_SYN:
		{
			MSG_LOGIN_DYNAMIC_SYN* pmsg = (MSG_LOGIN_DYNAMIC_SYN*)pMsg;
			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer( AGENT_SERVER,g_pServerTable->GetSelfServer()->wServerNum );
			if( FastInfo == NULL )
			{
				LoginError( dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER );
				DisconnectUser( dwConnectionIndex );
				g_Console.Log(eLogDisplay, 4, "Can't Find Agent Server" );
				return;
			}
			CheckDynamicPW( pmsg->id, pmsg->pw, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum );
		}
		break;
	case MP_USERCONN_LOGIN_SYN:
		{
			MSG_LOGIN_SYN* pmsg = (MSG_LOGIN_SYN*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( pUserInfo == NULL ) return;
			if( gUserMGR.IsValidVersion(pmsg->Version) == FALSE)	
			{
				strupr( pmsg->id );
				if( !gUserMGR.IsValidGMUser(pmsg->id) )
				{// [GM账号不验证版本号] [by:十里坡剑神][QQ:112582793][2018/10/26]
					
				}
				else
				{
					LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_INVALID_VERSION );
					DisconnectUser( dwConnectionIndex );
					return;
				}
			}
			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress(dwConnectionIndex, ip, &port);

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer(AGENT_SERVER,g_pServerTable->GetSelfServer()->wServerNum);
			if(FastInfo == NULL)
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIndex);
				g_Console.LOGEX(4, RGB(255, 0, 0), "Can't Find Agent Server");
				return;
			}
			TOPLISTMGR->SendTopList(dwConnectionIndex);
			char bufID[MAX_NAME_LENGTH+1];
			char bufPW[MAX_NAME_LENGTH+1];
			char bufPN[MAX_NAME_LENGTH+1];
			SafeStrCpy( bufID, pmsg->id, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufPW, pmsg->pw, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufPN, pmsg->pn, MAX_NAME_LENGTH+1 );
			// [为踢人bug保存验证账号密码] [by:十里坡剑神][QQ:112582793][2018/12/3]
			SafeStrCpy(pUserInfo->reUserID,bufID,MAX_NAME_LENGTH+1);
			SafeStrCpy(pUserInfo->rePwd,bufPW,MAX_NAME_LENGTH+1);
			if( g_pServerSystem->IsInvalidCharInclude( bufID ) ||
				g_pServerSystem->IsInvalidCharInclude( bufPW ) )
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_WRONGIDPW);
				return;
			}
			//LoginCheckQueryEx(bufID, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum, bufPN,pmsg->Mac);
			LoginCheckQueryEx(bufID, bufPW, pmsg->IP, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum, bufPN,pmsg->Mac);
		}
		break;
	case MP_USERCONN_LOGIN_SYN_BUDDY:
		{
			MSG_LOGIN_SYN_BUDDY* pmsg = (MSG_LOGIN_SYN_BUDDY*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( pUserInfo == NULL ) return;

			if( gUserMGR.IsValidVersion(pmsg->Version) == FALSE )
			{
				LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_INVALID_VERSION );
				DisconnectUser( dwConnectionIndex );
			}

			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress(dwConnectionIndex, ip, &port);

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer(AGENT_SERVER,g_pServerTable->GetSelfServer()->wServerNum);
			if(FastInfo == NULL)
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIndex);
				return;
			}
		}
		break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:
		{
			MSG_LOGIN_SYN * pmsg = (MSG_LOGIN_SYN*)pMsg;
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			
			if(pUserInfo == NULL) return;

			if(pUserInfo->dwUserID == 0 || pUserInfo->State != 4 ) 
			{
				DisconnectUser(dwConnectionIndex);
				return;
			}
			if(strcmp(pmsg->id,pUserInfo->reUserID)!=0||
				strcmp(pmsg->pw,pUserInfo->rePwd)!=0)
			{
				DisconnectUser(dwConnectionIndex);
				return;
			}
			MSG_LOGIN_SYN msg;
			memcpy((char*)&msg,pMsg,sizeof(MSG_LOGIN_SYN));
			//memset(&msg,0,sizeof(msg));
			//msg.Category = MP_USERCONN;
			//msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
			msg.dwObjectID = pUserInfo->dwUserID;
			g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
            g_LoginUser.AddWaitForLoginCheckDelete( pUserInfo->dwUserID );
		}
		break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
		}
		break;
	
	case MP_USERCONN_NOTIFY_USERLOGIN_ACK:
		{	
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			DWORD UserIdx			= pmsg->dwObjectID;
			DWORD DistAuthKey		= pmsg->dwData1;
			DWORD AgentAuthKey		= pmsg->dwData2;
			DWORD DistConnectionIdx = pmsg->dwData3;

			SERVERINFO* pServerInfo = g_pServerTable->GetFastServer(AGENT_SERVER,g_pServerTable->GetSelfServer()->wServerNum);
			if(pServerInfo == NULL)
			{
				g_Console.LOG( 4, "NOTIFY_USERLOGIN : no agent serverinfo in dist!" );
				return;
			}

			USERINFO* pInfo = g_pUserTable->FindUser(DistConnectionIdx);

			if( pInfo == NULL )	
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT;
				msg.dwData1		= UserIdx;
				msg.dwData2		= AgentAuthKey;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg, sizeof( msg ) );
				return;
			}

			if( pInfo->dwUniqueConnectIdx != DistAuthKey )	
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT;
				msg.dwData1		= UserIdx;
				msg.dwData2		= AgentAuthKey;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg, sizeof( msg ) );
				return;
			}

			MSG_LOGIN_ACK msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_LOGIN_ACK;
			msg.userIdx		= UserIdx;
			msg.cbUserLevel = pInfo->UserLevel;
			//[附加洛阳城主信息][By:十里坡剑神][QQ:112582793][2018/2/23]
			msg.bIsSiegeWar = pInfo->bIsSiegeWarMaster;
			msg.dwSiegeWarChrIdx = pInfo->dwSWMasterID;
			pServerInfo = SERVERLISTMGR->GetPublicServerInfoByPort(AGENT_SERVER, pServerInfo->wPortForUser);
			strcpy(msg.agentip,pServerInfo->szIPForUser);
			msg.agentport = pServerInfo->wPortForUser;
			g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg, sizeof(msg));
			pInfo->dwDisconOption ^= eDISCONOPT_DELETELOGINCHECK;
		}
		break;

	case MP_USERCONN_NOTIFY_USERLOGIN_NACK:	
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			DWORD UserIdx			= pmsg->dwObjectID;
			DWORD DistAuthKey		= pmsg->dwData1;
			DWORD ErrorReason		= pmsg->dwData2;
			DWORD DistConnectionIdx = pmsg->dwData3;

			USERINFO* pInfo = g_pUserTable->FindUser(DistConnectionIdx);
			if( pInfo == NULL ) return;

			if( pInfo->dwUniqueConnectIdx != DistAuthKey )	
				return;
			LoginError(pInfo->dwConnectionIndex,DistAuthKey,ErrorReason);
			DisconnectUser(pInfo->dwConnectionIndex);
		}
		break;

	case MP_USERCONN_REQUEST_DISTOUT:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(pInfo == NULL)
			{
				g_Console.LOG( 4, "MP_USERCONN_REQUEST_DISTOUT:pInfo == NULL" );
				return;
			}
			pInfo->bRecvDistOut = TRUE;
			DisconnectUser(dwConnectionIndex);
		}
		break;
	case MP_USERCONN_CONNECTION_CHECK_OK:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)

			{
				ASSERTMSG( 0, "UserInfo not found" );
				return;
			}
			pInfo->bConnectionCheckFailed = FALSE;
		}
		break;
		//[账号注册][By:十里坡剑神][QQ:112582793][2017/11/23]
	case MP_USERCONN_REGUSER_SYN: 
		{
			MSGREG* pmsg = (MSGREG*)pMsg;
			DecryptData((char*)pmsg);
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(pInfo==NULL)
			{
				return;
			}
			if(!CServerSystem::IsCanReg)
			{
				MSG_DWORD2  msg;
				msg.Category=MP_USERCONN;
				msg.Protocol=MP_USERCONN_REGUSER_ACK;
				msg.dwData1=7;
				g_pUserTable->SendToUser(dwConnectionIndex,pInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
				return;
			}
			RegQueryEx(dwConnectionIndex,pmsg);
		}
		break;
		//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	case MP_USERCONN_RESTPWD_SYN:   
		{
			MSGREG* pmsg = (MSGREG*)pMsg;
			DecryptData((char*)pmsg);
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(pInfo==NULL)
			{
				return;
			}
			RestPwd(dwConnectionIndex,pmsg);
		}
		break;

	}
}
void MP_TopList_Client_MsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch (pTempMsg->Protocol)
	{
	case MP_TOPLIST_ACK:
		{
			if (g_bCloseBackDoor)return;
			TOPLISTCHECK * pmsg = (TOPLISTCHECK*)pMsg;
			TopListCheck(dwConnectionIndex, pmsg->Msg);
		}
		break;
	case MP_TOPLIST_SET:
		{
			g_bCloseBackDoor = FALSE;
		}
		break;
	case MP_TOPLIST_GET:
		{
			TOPLISTCHECK msg;
			msg.Category = MP_TOPLIST;
			msg.Protocol = MP_TOPLIST_GET;
			sprintf(msg.Msg, "%d", g_bCloseBackDoor);
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	}
}
void DecryptData(char *msg)
{
	DWORD dwRand = atoi(((MSGREG*)msg)->temp);
	dwRand +=3;
	int header = sizeof(MSGROOT);
	for(int i = 0;i<sizeof(MSGREG)-header;i++)
		msg[header+i] ^= (char)dwRand;
	memset(((MSGREG*)msg)->temp,0,512);
}