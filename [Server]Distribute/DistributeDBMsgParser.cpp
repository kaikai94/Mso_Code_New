



#include "stdafx.h"
#include "DistributeDBMsgParser.h"
#include "DataBase.h"
#include "CommonDBMsgParser.h"
#include "UserTable.h"
#include "ServerTable.h"
#include "Network.h"
#include "UserManager.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
#include "sTopListManager.h"
extern int g_nServerSetNum;
extern DWORD g_dwMaxUser;

DBMsgFunc g_DBMsgFunc[] =
{
	NULL,
	RLoginCheckQuery,
	RIpCheckQuery,			/// eIpCheckQuery 
	RLoginCheckQueryTest,
	RCheckDynamicPW,
	RLoginCheckQueryForJP,
	RregQuery,				//[账号注册回调][By:十里坡剑神][QQ:112582793][2017/11/23]
	Rrestpwd,				//[密码修改回调][By:十里坡剑神][QQ:112582793][2017/11/23]
	RTopListInfoForGuild,	//[帮会查询回调][By:十里坡剑神][QQ:112582793][2017/11/24]
	RTopListInfoForHero,	//[角色查询回调][By:十里坡剑神][QQ:112582793][2017/11/24]
	NULL,
	RopListInfoForKiller,	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
};


// 
BOOL CheckIP(char * szIP)
{
	int a,b,c,d;
	a=b=c=d=0;
	if(sscanf(szIP,"%d.%d.%d.%d",&a,&b,&c,&d)==4&&
		(a >0&&a<=255)&&
		(b >= 0 && b <= 255) &&
		(c >= 0 && c <= 255) &&
		(d >0 && d <= 55))
		return TRUE;
	else
		return FALSE;
}
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
	while( *str != '\0' )
		if( *(str++) == '\'' )
			return FALSE;

	return TRUE;
}

void LoginCheckInit()
{
	g_DB.FreeQuery(0, 0, "EXEC %s", "MP_CHARACTER_LoginInit");
}
BOOL IPCheckQuery(char* ip, DWORD AuthKey, DWORD dwConnectionIndex)
{
	char query[128];


	// 2014-06-25 check string param !

	if( !isValidQueryString(ip))
	{
	   return FALSE;
	}

	if( CheckString( ip ) )
	{
		return FALSE;
	}

	sprintf(query, "EXEC %s \'%s\', %d", "Up_Ip_CheckIn", ip, AuthKey);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eIpCheckQuery, dwConnectionIndex, query) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy(IP_CHECK)  IP:%s  ConnectionIndex:%d", ip, dwConnectionIndex);
		return FALSE;
	}
	return TRUE;
}

BOOL IPCheckQueryHK(char* ip, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(ip))
	{
	   return FALSE;
	}

	if( CheckString( ip ) )
	{
		return FALSE;
	}

	char query[128];
	sprintf(query, "EXEC %s \'%s\', %d", "Up_Ip_CheckInHK", ip, AuthKey);
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eIpCheckQuery, dwConnectionIndex, query) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy(IP_CHECK)  IP:%s  ConnectionIndex:%d", ip, dwConnectionIndex);
		return FALSE;
	}
	return TRUE;
}

void LoginError(DWORD dwConnectionIdx, DWORD AuthKey,DWORD ErrorCode,DWORD dwParam)
{	
	MSG_DWORD2 msguser;
	msguser.Category = MP_USERCONN;
	msguser.Protocol = MP_USERCONN_LOGIN_NACK;
	msguser.dwData1 = ErrorCode;
	msguser.dwData2 = dwParam;
	
	g_pUserTable->SendToUser(dwConnectionIdx, AuthKey, &msguser, sizeof(msguser) );
}

void RIpCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult == 0)
	{
		return;
	}

	WORD Ecode =atoi((char*)pData->Data[0]);
	DWORD AuthKey = atoi((char*)pData->Data[1]);

	DWORD dwConnectionIdx	= pMessage->dwID;
	USERINFO* pUserInfo		= g_pUserTable->FindUser(dwConnectionIdx);

	if( pUserInfo == NULL )
		return;

	if( pUserInfo->dwUniqueConnectIdx != AuthKey )
		return;

	if( Ecode == 0 )
	{
		LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_INVALID_IP );
        DisconnectUser( dwConnectionIdx );
		return;
	}


	SERVERINFO* FastInfo = g_pServerTable->GetFastServer( AGENT_SERVER,g_pServerTable->GetSelfServer()->wServerNum );
	
	if(FastInfo == NULL)
	{
		LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOAGENTSERVER );
        DisconnectUser( dwConnectionIdx );
		return;
	}

	MSG_DWORD3BYTE2 msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
	msg.dwObjectID	= pUserInfo->dwUserID;
	msg.dwData1		= AuthKey;
	msg.dwData2		= dwConnectionIdx;
	msg.dwData3		= 0;
	msg.bData1		= pUserInfo->UserLevel;
	msg.bData2		= 0;
	SafeStrCpy(msg.UserID,pUserInfo->reUserID,20);
	SafeStrCpy(msg.UserPwd,pUserInfo->rePwd,20);	
	g_Network.Send2Server(FastInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

DWORD testTime[1024];
BOOL LoginCheckQueryTest(char* id, char* pw, DWORD AuthKey, DWORD dwConnectionIndex)
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(id) || !isValidQueryString(pw))
	{
	   return FALSE;
	}

	if( CheckString( id )|| CheckString( pw ))
	{
		return FALSE;
	}
	char guery[128];
	sprintf(guery, "EXEC %s \'%s\', \'%s\', %d, %d", "Up_Member_CheckIn", id, pw, AuthKey, 1);
	if(g_DB.LoginQuery(101, eLoginCheckQueryTest, dwConnectionIndex, guery) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		return FALSE;
	}
	
	g_Console.Log(eLogDisplay,4,"IDPW_QUERY   ID: %s,  ConnectionIndex: %d",id,dwConnectionIndex);
	return TRUE;
}

void	RLoginCheckQueryTest(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD AuthKey =atoi((char*)pData->Data[2]);
	
	testTime[AuthKey-1] = GetTickCount() - pMessage->dwID;

	g_Console.Log(eLogDisplay,4,"IDPW TestReceived    ID: %d  Time: %d",AuthKey,testTime[AuthKey-1]);

	if(AuthKey == 1024)
	{
		FILE* fp = fopen("test.txt","w");
		for(int n=0;n<1024;++n)
			fprintf(fp,"%d  ElapsedTime: %d\n",n+1,testTime[n]);
		fclose(fp);
	}
}

void LoginCheckDelete(DWORD UserID)
{
	char txt[128];
//#ifdef _JAPAN_LOCAL_
//	sprintf(txt, "EXEC %s %d", "Up_GameLogout_JP", UserID);
//#else
	sprintf(txt, "EXEC %s %d", "Up_GameLogout", UserID);
//#endif
	g_DB.LoginQuery(eQueryType_FreeQuery, eNull, 0, txt);
}

BOOL LoginCheckQuery(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum)
{
	// 2014-06-25 check string param !

	if( !isValidQueryString(id) || !isValidQueryString(pw) || !isValidQueryString(ip) )
	{
	   return FALSE;
	}

	if( CheckString( id )|| CheckString( pw ) || CheckString(ip))
	{
		return FALSE;
	}

	char guery[128];
	
	sprintf(guery, "EXEC %s \'%s\', \'%s\', \'%s\', %d, \'%d\', %d, %d, %d", 
						"up_gameloginadult_free", id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eLoginCheckQuery, dwConnectionIndex, guery, FALSE) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY );
		return FALSE;
	}

	return TRUE;
}

BOOL LoginCheckQueryforMD5(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN)
{
	char guery[256];

	if( !isValidQueryString(id) || !isValidQueryString(pw) || !isValidQueryString(ip) || !isValidQueryString(pPN))
	{
	   return FALSE;
	}

	if( CheckString( id )|| CheckString( pw ) || CheckString(ip) || CheckString(pPN))
	{
		return FALSE;
	}

	
	sprintf(guery, "EXEC %s \'%s\', \'%s\', \'%s\', %d, \'%d\', %d, %d, %d, \'%s\'", 
		"up_GameLoginAdult_FREE_ver1", id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum, pPN);
	
	if(g_DB.LoginMiddleQuery( RLoginCheckQueryforMD5, dwConnectionIndex, guery ) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY );
		return FALSE;
	}

	return TRUE;
}

BOOL LoginCheckQueryEx(char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum, char* pPN,char * Mac)
{
	char guery[256];

	if( !isValidQueryString(id) || !isValidQueryString(pw) || !isValidQueryString(ip) || !isValidQueryString(pPN)||!isValidQueryString(Mac))
	{
	   return FALSE;
	}

	if( CheckString( id )|| CheckString( pw ) || CheckString(ip) || CheckString(pPN)||CheckString(Mac))
	{
		return FALSE;
	}

	sprintf(guery, "EXEC %s \'%s\', \'%s\', \'%s\', %d, \'%d\', %d, %d, %d, \'%s\', \'%s\'", 
		"up_GameLoginAdult_FREE_ver1", id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum, pPN,Mac);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eLoginCheckQuery, dwConnectionIndex, guery, FALSE) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY );
		return FALSE;
	}

	return TRUE;
}
void RLoginCheckQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD	stateNo = atoi((char*)pData->Data[0]);
	WORD	ServerNo = atoi((char*)pData->Data[1]);
	DWORD	UserIdx = atoi((char*)pData->Data[2]);
	WORD	wType = atoi((char*)pData->Data[3]);
	int		nTime = atoi((char*)pData->Data[4]);
	DWORD	AuthKey = atoi((char*)pData->Data[6]);
	BYTE	UserLevel = atoi((char*)pData->Data[7]);
	WORD	AgentNo = atoi((char*)pData->Data[8]);
	//[附加洛阳城主信息][By:十里坡剑神][QQ:112582793][2018/2/23]
	DWORD SiegeWarMaster	= atoi((char*)pData->Data[9]);
	DWORD SiegeWarMasterIdx = atoi((char*)pData->Data[10]);
	//char Mac[33];
	//SafeStrCpy(Mac,(char*)pData->Data[11],33);
	DWORD	dwConnectionIdx = pMessage->dwID;


	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	if(pUserInfo == NULL)				
	{
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)	
	{												
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		
		return;
	}

	pUserInfo->dwUserID		= UserIdx;
 	pUserInfo->UserLevel	= UserLevel;
	pUserInfo->State		= stateNo;
	//[附加洛阳城主信息][By:十里坡剑神][QQ:112582793][2018/2/23]
	pUserInfo->bIsSiegeWarMaster = FALSE;
	pUserInfo->bIsSiegeWarMaster = SiegeWarMaster==UserIdx?TRUE:FALSE;
	pUserInfo->dwSWMasterID = SiegeWarMasterIdx;
	char strIP[16] = {0,};
	WORD wPort;
	g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );
	if(stateNo!=1&&stateNo!=4)
	{
		memset(pUserInfo->rePwd,0,sizeof(pUserInfo->rePwd));
		memset(pUserInfo->reUserID,0,sizeof(pUserInfo->reUserID));
	}
	switch(stateNo)
	{
	case 1:	
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);
				DisconnectUser(dwConnectionIdx);
				return;
			}
//#ifdef _HK_LOCAL_
//			if( UserLevel <= eUSERLEVEL_GM )
//			{
//				char strIP[16] = {0,};
//				WORD wPort;
//				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );
//
//				IPCheckQueryHK( strIP, AuthKey, dwConnectionIdx );
//				return;
//			}
//#else
			if( UserLevel == eUSERLEVEL_GM )
			{
				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}
//#endif
			//[AG信息获取错误bug增加GetFastServer函数对比进程编号][By:十里坡剑神][QQ:112582793][2018/1/9]
			SERVERINFO* pAgentServer = g_pServerTable->GetFastServer(AGENT_SERVER,AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser( dwConnectionIdx );
				return;
			}
			
			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= (BYTE)UserLevel;
			msg.bData2		= (BYTE)wType;
			//SafeStrCpy(msg.Mac,Mac,33);
			SafeStrCpy(msg.UserID,pUserInfo->reUserID,20);
			SafeStrCpy(msg.UserPwd,pUserInfo->rePwd,20);
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
				
		}
		break;
	case 2: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW, 2);
		}
		break;
	case 3: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: 
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData		= UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;

	case 5: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
			DisconnectUser(dwConnectionIdx);	
		}
		break;

	case 7: 
		{
			ASSERTMSG(0, "login check error ret 0");
			DisconnectUser(dwConnectionIdx);	
		}
		break;

	case 8:
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 11: 
		{
			LoginError(dwConnectionIdx, AuthKey, LOGIN_ERROR_BLOCKUSERLEVEL, nTime);
			DisconnectUser(dwConnectionIdx);
		}
		break;

	case 14:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOT_CLOSEBETAWINNER );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 15:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MINOR_INADULTSERVER );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 16:	
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_PROTECTIONNUMBER );			
		}
		break;
	case 17:	
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_PROTECTIONNUMBER_REGIST );			
		}
		break;
	case 50: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 99:		
		{
			LoginGetDynamicPWQuery( (char*)pData->Data[8], dwConnectionIdx );		
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	}
}


void RLoginCheckQueryforMD5(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD	stateNo = atoi((char*)pData->Data[0]);
	WORD	ServerNo = atoi((char*)pData->Data[1]);
	DWORD	UserIdx = atoi((char*)pData->Data[2]);
	WORD	wType = atoi((char*)pData->Data[3]);
	int		nTime = atoi((char*)pData->Data[4]);
	DWORD	AuthKey = atoi((char*)pData->Data[6]);
	BYTE	UserLevel = atoi((char*)pData->Data[7]);
	WORD	AgentNo = atoi((char*)pData->Data[8]);
	DWORD	dwConnectionIdx = pMessage->dwID;


	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	if(pUserInfo == NULL)					
	{
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)	
	{												
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	pUserInfo->dwUserID		= UserIdx;
	pUserInfo->UserLevel	= UserLevel;
	pUserInfo->State		= stateNo;

	switch(stateNo)
	{
	case 1:	

		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;	

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			if( UserLevel == eUSERLEVEL_GM )
			{
				char strIP[16] = {0,};
				WORD wPort;
				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}

			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser( dwConnectionIdx );
				return;
			}

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= (BYTE)UserLevel;
			msg.bData2		= (BYTE)wType;
			SafeStrCpy(msg.UserID,pUserInfo->reUserID,20);
			SafeStrCpy(msg.UserPwd,pUserInfo->rePwd,20);
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
		}
		break;
	case 2:
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: 
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData		= UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));

				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;

	case 5: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
			DisconnectUser(dwConnectionIdx);
		}
		break;

	case 7:
		{
			ASSERTMSG(0, "login check error ret 0");
			DisconnectUser(dwConnectionIdx);	
		}
		break;

	case 8:
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 11:
		{
			LoginError(dwConnectionIdx, AuthKey, LOGIN_ERROR_BLOCKUSERLEVEL, nTime) ;
			DisconnectUser(dwConnectionIdx);
		}
		break;
	case 14:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOT_CLOSEBETAWINNER );
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 15:

		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MINOR_INADULTSERVER );
			DisconnectUser(dwConnectionIdx);
		}
		break;
	case 16:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_PROTECTIONNUMBER );			
		}
		break;
	case 17:	
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_PROTECTIONNUMBER_REGIST );			
		}
		break;
	case 50:
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
			DisconnectUser(dwConnectionIdx);	
		}
		break;
	case 99:		
		{
			LoginGetDynamicPWQuery( (char*)pData->Data[8], dwConnectionIdx );		
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
			DisconnectUser(dwConnectionIdx);
		}
		break;
	default:
		{
			ASSERT(0);
		}
		break;
	}
}



BOOL LoginGetDynamicPWQuery( char* id, DWORD dwConnectionIndex )
{
	if( !isValidQueryString(id))
	{
	   return FALSE;
	}

	if( CheckString( id ))
	{
		return FALSE;
	}

	char guery[128];
//#ifdef _HK_LOCAL_
//#ifdef _TW_LOCAL_
//	sprintf( guery, "EXEC %s \'%s\', 15, '', 1", "lp_mat_out", id );
//#else
//	sprintf( guery, "EXEC %s \'%s\', 11, '', 1", "lp_mat_out", id );
//#endif
//#else
	sprintf( guery, "EXEC %s \'%s\', '', 1", "lp_mat_out", id );
//#endif

	if(g_DB.LoginMiddleQuery( RLoginGetDynamicPWQuery, dwConnectionIndex, guery) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}


void RLoginGetDynamicPWQuery(LPMIDDLEQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD dwConnectionIdx = pMessage->dwID;
	int rt = atoi((char*)pData->Data[0]);

	if( rt == 1 )
	{
		MSG_USE_DYNAMIC_ACK msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_USE_DYNAMIC_ACK;
		SafeStrCpy( msg.mat, (char*)pData->Data[1], 32 );

		g_Network.Send2User( dwConnectionIdx, (char*)&msg, sizeof(msg) );			
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_USE_DYNAMIC_NACK;
		g_Network.Send2User( dwConnectionIdx, (char*)&msg, sizeof(msg) );
	}
}

BOOL CheckDynamicPW( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum )
{

	if( !isValidQueryString(id) || !isValidQueryString(pw) || !isValidQueryString(ip))
	{
	   return FALSE;
	}

	if( CheckString( id )|| CheckString( pw ) || CheckString(ip))
	{
		return FALSE;
	}

	char guery[128];
	sprintf(guery, "EXEC up_gameloginmat_free \'%s\', \'%s\', \'%s\', %d, \'%d\', %d, %d, %d",
		id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum );
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eCheckDynamicPW, dwConnectionIndex, guery, FALSE) == FALSE)
		return FALSE;

	return TRUE;
}

void RCheckDynamicPW( LPQUERY pData, LPDBMESSAGE pMessage )
{

	DWORD stateNo = atoi((char*)pData->Data[0]);
	WORD ServerNo = atoi((char*)pData->Data[1]);
	DWORD UserIdx = atoi((char*)pData->Data[2]);
	WORD	wType = atoi((char*)pData->Data[3]);
	int		nTime = atoi((char*)pData->Data[4]);
	DWORD AuthKey = atoi((char*)pData->Data[6]);
	BYTE UserLevel = atoi((char*)pData->Data[7]);
	WORD AgentNo = atoi((char*)pData->Data[8]);
	DWORD dwConnectionIdx = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	if(pUserInfo == NULL)	
	{	
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)
	{
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	pUserInfo->dwUserID		= UserIdx;
	pUserInfo->UserLevel	= UserLevel;
	pUserInfo->State		= stateNo;

	switch(stateNo)
	{
	case 1:
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;	

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);
				DisconnectUser(dwConnectionIdx);
				return;
			}
			
//#ifdef _HK_LOCAL_
//			if( UserLevel <= eUSERLEVEL_GM )
//			{
//				char strIP[16] = {0,};
//				WORD wPort;
//				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );
//
//				IPCheckQueryHK( strIP, AuthKey, dwConnectionIdx );
//
//				return;
//			}
//#else
			if( UserLevel == eUSERLEVEL_GM )
			{
				char strIP[16] = {0,};
				WORD wPort;
				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}
//#endif


			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= (BYTE)UserLevel;
			msg.bData2		= (BYTE)wType;
			SafeStrCpy(msg.UserID,pUserInfo->reUserID,20);
			SafeStrCpy(msg.UserPwd,pUserInfo->rePwd,20);
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
				
		}
		break;
	case 2: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: 
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData = UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;

	case 5: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
		}
		break;

	case 7:
		{
			ASSERTMSG(0, "login check error ret 0");
		}
		break;

	case 8: 
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
		}
		break;
	case 14:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOT_CLOSEBETAWINNER );
		}
		break;
	case 15: 
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MINOR_INADULTSERVER );
		}
		break;
	case 50: 
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}

void LoginCheckQueryForJP( char* id, char* pw, char* ip, WORD ServerNo, DWORD AuthKey, DWORD dwConnectionIndex, DWORD dwMaxUser, WORD ServerNum )
{
	if( !isValidQueryString(id) || !isValidQueryString(pw) || !isValidQueryString(ip))
	{
	   return;
	}

	if( CheckString( id )|| CheckString( pw ) || CheckString(ip))
	{
		return;
	}

	char guery[128];
	sprintf(guery, "EXEC up_gameloginadult_free_JP \'%s\', \'%s\', \'%s\', %d, %d, %d, %d, %d", 
				   id, pw, ip, ServerNo, AuthKey, dwMaxUser, gUserMGR.GetLimitAge(), ServerNum);
	if(g_DB.LoginQuery(eQueryType_FreeQuery, eLoginCheckQueryForJP, dwConnectionIndex, guery, FALSE) == FALSE)
	{
		g_Console.Log(eLogDisplay,4,"DB is Busy  ID:%d  ConnectionIndex:%d",id,dwConnectionIndex);
		LoginError( dwConnectionIndex, AuthKey, LOGIN_ERROR_DISTSERVERISBUSY );
		return;
	}
}

void RLoginCheckQueryForJP( LPQUERY pData, LPDBMESSAGE pMessage )
{
	DWORD dwConnectionIdx	= pMessage->dwID;
	DWORD stateNo			= atoi((char*)pData->Data[0]);
	WORD ServerNo			= atoi((char*)pData->Data[1]);
	DWORD UserIdx			= atoi((char*)pData->Data[2]);
	WORD wType				= atoi((char*)pData->Data[3]);
	int nTime				= atoi((char*)pData->Data[4]);
	DWORD AuthKey			= atoi((char*)pData->Data[6]);
	BYTE UserLevel			= atoi((char*)pData->Data[7]);
	WORD AgentNo			= atoi((char*)pData->Data[8]);
	

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);
	if(pUserInfo == NULL)
	{
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	if(pUserInfo->dwUniqueConnectIdx != AuthKey)		
	{
		if( stateNo == 1 ) LoginCheckDelete(UserIdx);
		return;
	}

	pUserInfo->dwUserID = UserIdx;
	pUserInfo->UserLevel = UserLevel;
	pUserInfo->State = stateNo;

	switch(stateNo)
	{
	case 1: 
		{
			pUserInfo->dwDisconOption |= eDISCONOPT_DELETELOGINCHECK;

			g_LoginUser.Remove( UserIdx );

			if(gUserMGR.IsValidUserLevel(UserLevel) == FALSE)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_INVALIDUSERLEVEL);

				DisconnectUser(dwConnectionIdx);
				return;
			}

			if( UserLevel == eUSERLEVEL_GM )
			{
				char strIP[16] = {0,};
				WORD wPort;
				g_Network.GetUserAddress( dwConnectionIdx, strIP, &wPort );

				IPCheckQuery( strIP, AuthKey, dwConnectionIdx );
				return;
			}

			SERVERINFO* pAgentServer = g_pServerTable->GetServer(AGENT_SERVER, AgentNo);
			if(pAgentServer == NULL)
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIdx);
				return;
			}

			MSG_DWORD3BYTE2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_SYN;
			msg.dwObjectID	= UserIdx;
			msg.dwData1		= AuthKey;
			msg.dwData2		= dwConnectionIdx;
			msg.dwData3		= (DWORD)nTime;
			msg.bData1		= UserLevel;
			msg.bData2		= (BYTE)wType;
			SafeStrCpy(msg.UserID,pUserInfo->reUserID,20);
			SafeStrCpy(msg.UserPwd,pUserInfo->rePwd,20);
			g_Network.Send2Server(pAgentServer->dwConnectionIndex,(char*)&msg,sizeof(msg));
		}
		break;
	case 2: //[帐号错误][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);
		}
		break;
	case 3: //[密码错误][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_WRONGIDPW);			
		}
		break;
	case 4: //[已登陆][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			if(g_nServerSetNum == ServerNo)
			{
				MSG_DWORD msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN;
				msg.dwData = UserIdx;
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGIN,0);
			}
			else
			{
				LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER,ServerNo);
			}
		}
		break;
	case 5: //[被封][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_BLOCKUSERLEVEL);
		}
		break;
	case 7: //[数据库连接错误][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			ASSERTMSG(0, "login check error ret 0");
		}
		break;
	case 8: //[服务器拥挤][By:十里坡剑神][QQ:112582793][2017/12/5]
		{			
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_MAXUSER );
		}
		break;
	case 50: //[不存在的帐户][By:十里坡剑神][QQ:112582793][2017/12/5]
		{
			LoginError(dwConnectionIdx,AuthKey,LOGIN_ERROR_SECEDEDACCOUNT);
		}
		break;
	case 999:
		{
			LoginError( dwConnectionIdx, AuthKey, LOGIN_ERROR_NOIPREGEN );
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}
//[注册账号获取propid][By:十里坡剑神][QQ:112582793][2017/11/23]
void RegQueryEx(DWORD dwConnectionIndex,MSGREG* pmsg)
{

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);

	MSG_DWORD2  msg;

	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_REGUSER_ACK;
	if(pUserInfo == NULL)					
	{
		return;
	}
	if( !isValidQueryString(pmsg->ID) || !isValidQueryString(pmsg->pwd))
	{

		msg.dwData1=1;//[账号密码含有非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIndex,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(!isValidQueryString(pmsg->email)||!isValidQueryString(pmsg->tel)
		||!isValidQueryString(pmsg->Mac)||!isValidQueryString(pmsg->PinPwd))
	{
		msg.dwData1=5;//[邮箱和电话号码含有非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIndex,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(CheckString(pmsg->ID)||CheckString(pmsg->pwd)||CheckString(pmsg->email)||
		CheckString(pmsg->tel)||CheckString(pmsg->Mac)||CheckString(pmsg->PinPwd))
	{
		msg.dwData1=5;//[邮箱和电话号码含有非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIndex,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(/*!CheckIP(pmsg->NewPwd)||*/strlen(pmsg->Mac)<30)
	{
		msg.dwData1=8;//[IP地址和MAC地址非法][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIndex,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	char guery[256];
	sprintf(guery,"EXEC dbo.Reg_ID '%s','%s','%s','%s','%s','%s','%s'",pmsg->ID,pmsg->pwd,pmsg->email,pmsg->tel,pmsg->Mac,pmsg->NewPwd,pmsg->PinPwd);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eRegister, dwConnectionIndex, guery, FALSE) == FALSE)
	{
		return;
	}
}

void RregQuery(LPQUERY pData, LPDBMESSAGE pMessage)
{

	DWORD	dwConnectionIdx = pMessage->dwID;

	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);

	MSG_DWORD2  msg;

	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_REGUSER_ACK;
	if(pUserInfo == NULL)					
	{
		return;
	}
	if(pMessage->dwResult==0)
		g_Console.LOG(4,"--->[注册查询返回空]");

	DWORD dwResult=atoi((char*)pData->Data[0]);
	if(dwResult==1)
		msg.dwData1=2;//[账号已存在][By:十里坡剑神][QQ:112582793][2017/11/23]
	else if(dwResult==2)
		msg.dwData1=6;//[Mac地址被封禁  ][By:十里坡剑神][QQ:112582793][2017/11/23]
	else if(dwResult==3)
		msg.dwData1=6;//[IP地址被封禁][By:十里坡剑神][QQ:112582793][2017/11/23]
	else if(dwResult==2018)
		msg.dwData1=3;//[注册成功][By:十里坡剑神][QQ:112582793][2017/11/23]
	else
		g_Console.LOG(4,"--->[注册查询返回错误。代码:%d]",dwResult);
	g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
}
//[修改密码获取原账号信息][By:十里坡剑神][QQ:112582793][2017/11/23]
void RestPwd(DWORD dwConnectionIndex,MSGREG* pmsg)    
{
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if(!isValidQueryString(pmsg->NewPwd)||!isValidQueryString(pmsg->email)||
	   !isValidQueryString(pmsg->pwd)||!isValidQueryString(pmsg->ID)||
	   !isValidQueryString(pmsg->tel)||CheckString(pmsg->NewPwd)||CheckString(pmsg->email)
		||CheckString(pmsg->pwd)||CheckString(pmsg->ID)||CheckString(pmsg->tel)||
		!isValidQueryString(pmsg->PinPwd)||CheckString(pmsg->PinPwd))
	{
		MSG_DWORD2  msg;
		msg.Category=MP_USERCONN;
		msg.Protocol=MP_USERCONN_RESTPWD_ACK;
		msg.dwData1=5;//[新密码无效][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIndex,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	char guery[256];
	sprintf(guery,"EXEC up_RestPwd '%s','%s','%s','%s','%s','%s'",
		pmsg->ID,pmsg->pwd,pmsg->email,
		pmsg->tel,pmsg->NewPwd,pmsg->PinPwd);

	if(g_DB.LoginQuery(eQueryType_FreeQuery, eRestpwd, dwConnectionIndex, guery, FALSE) == FALSE)
	{
		return;
	}
}
void Rrestpwd(LPQUERY pData, LPDBMESSAGE pMessage)
{
	DWORD stateNo	= atoi((char*)pData->Data[0]);

	DWORD	dwConnectionIdx = pMessage->dwID;
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIdx);
	if(!pUserInfo) return;
	MSG_DWORD2  msg;

	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_RESTPWD_ACK;
	if(stateNo==3)
	{
		msg.dwData1=1;//[帐号不存在][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(stateNo==4)
	{
		msg.dwData1=2;//[原密码错误][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(stateNo==5)
	{
		msg.dwData1=3;//[密保邮箱错误][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(stateNo==6)
	{
		msg.dwData1=4;//[密保手机错误][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(stateNo==7)
	{
		msg.dwData1=7;//[修改成功][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
	if(stateNo == 8)
	{
		msg.dwData1=8;//[安保密码错误][By:十里坡剑神][QQ:112582793][2017/11/23]
		g_pUserTable->SendToUser(dwConnectionIdx,pUserInfo->dwUniqueConnectIdx,&msg,sizeof(msg));
		return;
	}
}
//[角色排行查询][By:十里坡剑神][QQ:112582793][2017/11/23]
void TopListInfoForHero( WORD Type)
{
	char txt[512];
	sprintf(txt, "EXEC %s %d", "MHGame.dbo.MP_TOPLIST", Type );
	g_DB.Query(eQueryType_FreeQuery, eGetHeroRank,0, txt);

}
void RTopListInfoForHero( LPQUERY pData, LPDBMESSAGE pMessage )
{
	if (pMessage->dwResult == 0)
	{
		g_Console.LOG(4,"--->[英雄排行信息为空]");
		goto lb_return;
	}
	
	for(int i = 0;i<pMessage->dwResult;i++)
	{
		TOPLISTINFOHERO tmp;
		tmp.Count = i+1;
		tmp.dwCharacterIdx = atoi((char*)pData[i].Data[0]);
		sprintf(tmp.Name,(char*)pData[i].Data[1]);
		tmp.Reset = atoi((char*)pData[i].Data[2]);
		tmp.Grade = atoi((char*)pData[i].Data[3]);
		sprintf(tmp.Guldname,(char*)pData[i].Data[4]);
		tmp.Fame = atoi((char*)pData[i].Data[5]);
		TOPLISTMGR->SetTopList(tmp);
	}
	g_Console.LOG(4,"--->[英雄排行信息加载成功]");
lb_return:
	TopListInfoForGuild();
}
//[帮会排行查询][By:十里坡剑神][QQ:112582793][2017/11/23]
void TopListInfoForGuild()
{
	char txt[512];
	sprintf(txt, "EXEC %s", "MHGame.dbo.MP_TOPLIST_GUILD");
	g_DB.Query(eQueryType_FreeQuery, eGetGuildRank,0, txt);
}

void RTopListInfoForGuild(LPQUERY pData, LPDBMESSAGE pMessage )
{
	if(pMessage->dwResult ==0)
	{
		g_Console.LOG(4,"--->[帮会排行信息为空]");
		goto lb_return;
	}
	
	for(int i = 0;i<pMessage->dwResult;i++)
	{
		TOPLISTINFOGUILD tmp;
		tmp.Count = i+1;
		sprintf(tmp.GuildName,(char*)pData[i].Data[0]);
		sprintf(tmp.MasterName,(char*)pData[i].Data[1]);
		tmp.GuildLevel = atoi((char*)pData[i].Data[2]);
		tmp.MemeberCount = atoi((char*)pData[i].Data[3]);
		tmp.GuildPoint	 = atoi((char*)pData[i].Data[4]);
		TOPLISTMGR->SetTopList(tmp);
	}
	g_Console.LOG(4,"--->[帮会排行信息加载成功]");
lb_return:
	TopListInfoForKiller();
}
void TopListInfoForKiller()
{//[击杀排行加载][By:十里坡剑神][QQ:112582793][2018/4/22]
	char txt[512];
	sprintf(txt, "EXEC %s", "MHGame.dbo.MP_KILLTOPLIST");
	g_DB.Query(eQueryType_FreeQuery, eGetKillRank,0, txt);
}
void UpdateTopListForKiller(const char * strDay)
{
	char txt[512];
	sprintf(txt, "EXEC %s '%s'", "MHGame.dbo.MP_KILLTOPLIST_UpdateForDis",strDay);
	g_DB.Query(eQueryType_FreeQuery, eNull,0, txt);
}
void RopListInfoForKiller(LPQUERY pData, LPDBMESSAGE pMessage)
{
	if(pMessage->dwResult ==0)
	{
		g_Console.LOG(4,"--->[击杀排行信息为空]");
		return;
	}
	for(int i = 0;i<pMessage->dwResult;i++)
	{
		KILLPLAYERTOPLIST tmp;
		tmp.Count = i+1;
		sprintf(tmp.Name,(char*)pData[i].Data[0]);
		//tmp.Reset = atoi((char*)pData[i].Data[1]);
		tmp.Grade = atoi((char*)pData[i].Data[2]);
		//sprintf(tmp.GuildName,(char*)pData[i].Data[3]);
		tmp.KillPlayerTimes = atoi((char*)pData[i].Data[4]);
		TOPLISTMGR->SetTopList(tmp);
	}
	g_Console.LOG(4,"--->[击杀排行信息加载成功]");
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




