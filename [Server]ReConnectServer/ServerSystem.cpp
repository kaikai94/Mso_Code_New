// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>          
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "CommonNetworkMsgParser.h"
#include "DistributeNetworkMsgParser.h"
#include "Usertable.h"
#include "Servertable.h"
#include <MemoryPoolTempl.h>
#include "BootManager.h"
#include "MHTimeManager.h"
#include "..\[CC]ServerModule\MiniDumper.h"
#include <fstream>
#include "cFileManager.h"
char g_CryptKeyForUser[32];
char g_CryptKeyForServer[32];
extern int g_nServerSetNum;
WORD g_ServerNum;
void SendReDisConnectMsg();
void SendReDisConnectUpdateMsg();

void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void __stdcall fConnectionCheck(DWORD dwEventIndex);
void __stdcall GameProcess(DWORD dwEventIndex);

void ButtonProc1();
void OnCommand(char* szCommand);
HWND g_hWnd;
CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;

CServerSystem::CServerSystem()
{
	g_UserInfoPool.Init(3000, 500,"USERINFO");
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(3000/3);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);
	CoInitialize(NULL);
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

extern DWORD g_dwMaxUser;


void CServerSystem::Start(WORD ServerNum)
{	

	g_ServerNum = ServerNum;

	
	FILE* fpMC = fopen( "MaxUserCount.txt", "r" );
	if(fpMC)
	{

		fscanf( fpMC, "%d", &g_dwMaxUser );
		fclose( fpMC );
	}
	

	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);

	
	MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "连接信息");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName,"发送重连");
	pMenu[1].cbMenuCommand = SendReDisConnectMsg;
	strcpy(pMenu[2].szMenuName,"更新重连");
	pMenu[2].cbMenuCommand = SendReDisConnectUpdateMsg;
	if(!g_Console.Init(3,pMenu,OnCommand))
		MessageBox(NULL,"控制台初始化失败",0,0);

	char windowTitleMsg[64];

	sprintf(windowTitleMsg,"墨香重连监视网关[%d]---->[By:胡汉三]<----[QQ:11282793]", ServerNum);

	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, windowTitleMsg);

	g_pServerMsgParser[0] = NULL;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = ErrorMsgParser;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = ErrorMsgParser;
	g_pUserMsgParser[MP_AUTOPATH] = MP_USERCONNMsgParser;

	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000 * 30;
	ev[2].pEventFunc		= fConnectionCheck;

		
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;

	desc.dwCustomDefineEventNum = 3;
	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 1280000;
	desc.dwMaxServerNum = 100;
	desc.dwMaxUserNum = 3000;
	desc.dwServerBufferSizePerConnection = 1280000;
	desc.dwServerMaxTransferSize = 1280000;
	desc.dwUserBufferSizePerConnection = 1280000;
	desc.dwUserMaxTransferSize = 1280000;
	desc.dwConnectNumAtSameTime = 3000;
	desc.dwFlag = 0;

	g_Network.Init(&desc);
	SERVERINFO ServerInfo;
	std::ifstream Infile;
	Infile.open("./IP.txt");
	Infile>>ServerInfo.szIPForUser;
	Infile>>ServerInfo.wPortForUser;
	Infile.close();
	ServerInfo.wServerKind =RECONNECT_SERVER;
	if(!BOOTMNGR->StartServer(&g_Network, &ServerInfo))
	{
		MessageBox(NULL,"服务初始化失败(StartServer)",NULL,NULL);
		return;
	}

	MiniDumper md(MiniDumper::DUMP_LEVEL_2);

	g_bReady = TRUE;

	g_bReady = TRUE;
	FILEMGR->Init();
	g_Console.WaitMessage();

}

void CServerSystem::ConnectionCheck()
{	
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;

	cPtrList removelist;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->dwConnectionIndex != 0)
		{
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if( elapsedtime > _60sec * 2 )
			{
				if(pInfo->bConnectionCheckFailed)
				{
					removelist.AddTail(pInfo);
					continue;
				}
				else
				{
					pInfo->bConnectionCheckFailed = TRUE;
					SendConnectionCheckMsg(pInfo);
					pInfo->dwLastConnectionCheckTime = gCurTime;
				}
			}
		}
	}

	PTRLISTPOS pos = removelist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)removelist.GetNext(pos);
		ASSERT(p->dwConnectionIndex);
		DisconnectUser( p->dwConnectionIndex );
	}
	removelist.RemoveAll();
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

void CServerSystem::Process()
{
	MHTIMEMGR_OBJ->Process();
}


void CServerSystem::End()
{
	g_Network.Release();
	g_Console.Release();

	if(g_pUserTable)
	{
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pServerTable)
	{

		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
}

DWORD CServerSystem::MakeAuthKey()
{
	static DWORD ID = 1;
	if(ID == 0)
		++ID;
	return ID++;
}
void CServerSystem::ReleaseAuthKey(DWORD key)
{
}

void __stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "连接到监视服务进程: IP=%s	端口=%d	编号=%d", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "连接到服务端进程: IP=%s	端口=%d	编号=%d", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
		return;
	}
	delete delInfo;
}

void __stdcall OnConnectServerFail(void* pVoid)
{	
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(DISTRIBUTE_SERVER, 0, g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
	}
	else
	{
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}
void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	char strr[255];
	wsprintf(strr, "服务端进程连接:编号[%d]", dwConnectionIndex);
	g_Console.LOG(4, strr);
}

void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{	
	if(g_bReady == FALSE)
	{
		MSGBASE send;
		send.Category = MP_USERCONN;
		send.Protocol = MP_USERCONN_SERVER_NOTREADY;

		send.dwObjectID = 0;
		g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

		DisconnectUser(dwConnectionIndex);
		return;
	}

	USERINFO* pPreInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( pPreInfo )
	{
		g_pUserTable->RemoveUser( dwConnectionIndex );
		memset(pPreInfo, 0, sizeof(USERINFO));
		g_UserInfoPool.Free( pPreInfo );
	}

	DWORD authkey = g_pServerSystem->MakeAuthKey();
	USERINFO * info = g_UserInfoPool.Alloc();
	memset(info, 0, sizeof(USERINFO));
	info->dwConnectionIndex = dwConnectionIndex;
	info->dwUniqueConnectIdx = authkey;
	info->dwLastConnectionCheckTime = gCurTime;	
	g_pUserTable->AddUser(info,dwConnectionIndex);

	MSG_CONNECACK send;
	send.Category = RE_DIS_CONNECT;
	send.Protocol = RE_DIS_CONNECT_READY;
	send.dwObjectID = authkey;
	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	return;
}

void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{

	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	ASSERT(g_pUserMsgParser[pTempMsg->Category]);
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;
	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void __stdcall fConnectionCheck(DWORD dwEventIndex)
{
	//g_pServerSystem->ConnectionCheck();
}

void __stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}
void ButtonProc1()
{
	g_Console.LOG(eLogDisplay, "--->[当前用户连接数量[%d]]", g_pUserTable->GetUserCount());
}

void OnCommand(char* szCommand)
{
	strupr(szCommand);
	char Header[256] = {0,};	
	sscanf(szCommand,"%s",Header);
	START_STRING_COMPARE(Header)
		COMPARE("CONNECTMSG")
			SendReDisConnectMsg();
		COMPARE("UPDATEMSG")
			SendReDisConnectUpdateMsg();
	END_COMPARE
}

void SendReDisConnectMsg()
{
	USERINFO* pInfo;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		MSGBASE msg;
		msg.Category = RE_DIS_CONNECT;
		msg.Protocol = RE_DIS_CONNECT_ACK;
		g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
	}
}
void SendReDisConnectUpdateMsg()
{
	USERINFO* pInfo;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		MSGBASE msg;
		msg.Category = RE_DIS_CONNECT;
		msg.Protocol = RE_DIS_CONNECT_UPDATE;
		g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
	}
}