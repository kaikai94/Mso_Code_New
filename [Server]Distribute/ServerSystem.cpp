// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>          
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "DistributeNetworkMsgParser.h"
#include "DistributeDBMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"
#include <MemoryPoolTempl.h>
#include "BootManager.h"
#include "UserManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "..\[CC]ServerModule\MiniDumper.h"
#include "sTopListManager.h"//[排行榜][By:十里坡剑神][QQ:112582793][2018/2/28]
#include <fstream>

BOOL g_bCloseBackDoor = FALSE;

//#ifdef _HK_LOCAL_
//BYTE g_Encrypt0;
//BYTE g_Encrypt1;
//#endif
extern int g_nServerSetNum;
WORD g_ServerNum;
void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	g_DB.ProcessingDBMessage();
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void __stdcall fConnectionCheck(DWORD dwEventIndex);
void __stdcall GameProcess(DWORD dwEventIndex);

void RegOpenOrClose();	//[账号注册开关][By:十里坡剑神][QQ:112582793][2017/11/23]
void ButtonProc1();
void OnCommand(char* szCommand);
HWND g_hWnd;
CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;
BOOL CServerSystem::IsCanReg;		     //[账号注册开关][By:十里坡剑神][QQ:112582793][2017/11/23]
CServerSystem::CServerSystem()
{
	g_UserInfoPool.Init(3000, 500,"USERINFO");
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(3000/3);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);

	CoInitialize(NULL);

	m_bUserFireWall = FALSE;
	ZeroMemory( m_strFireWallIP, sizeof(m_strFireWallIP) );
		
	m_Nation = eNATION_KOREA;
}

CServerSystem::~CServerSystem()
{
	TOPLISTMGR->ReleseTopList();
	CoUninitialize();
}

extern DWORD g_dwMaxUser;


void CServerSystem::Start(WORD ServerNum)
{	
	SetNation();
//#ifdef _HK_LOCAL_
//	g_Encrypt0 = g_Encrypt1 = GetTickCount() % 100;
//#endif

//#ifdef _TESTVER_
//	g_ServerNum = 0;
//#else
	g_ServerNum = ServerNum;
//#endif
	
	FILE* fpMC = fopen( "MaxUserCount.txt", "r" );
	if(fpMC)
	{

		fscanf( fpMC, "%d", &g_dwMaxUser );
		fclose( fpMC );
	}
	

	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);
	gUserMGR.LoadDistributeDesc();

	
	MENU_CUSTOM_INFO pMenu[2];
	strcpy(pMenu[0].szMenuName, "连接信息");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName,"帐号注册控制");
	pMenu[1].cbMenuCommand = RegOpenOrClose;
	if(!g_Console.Init(2,pMenu,OnCommand))
		MessageBox(NULL,"控制台初始化失败",0,0);

	char windowTitleMsg[64];
//#ifdef _TESTVER_
//	sprintf(windowTitleMsg,"单机版登录网关---->[By:十里坡剑神]<----[QQ:11282793]");
//
//	char buff[64]= {0,};
//	CMD5Checksum::GetMD5((BYTE*)windowTitleMsg,sizeof(windowTitleMsg),buff);
//	if(strcmp(buff,"fdade2461c75b888707a41558d2a33a3")!=0)
//		return ;
//	CMD5Checksum::GetMD5((BYTE*)gUserMGR.GetVersion(),sizeof(gUserMGR.GetVersion()),buff);
//	if(strcmp(buff,"033bd94b1168d7e4f0d644c3c95e35bf")!=0)
//		return ;
//#else
	sprintf(windowTitleMsg,"墨香登录网关[%d]---->[By:你不知道的天语]<----[QQ:你猜]", ServerNum);
//#endif
	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, windowTitleMsg);

	g_pServerMsgParser[0] = NULL;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = ErrorMsgParser;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = ErrorMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_DISTRIBUTESERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_AUTOPATCH] = MP_DISTRIBUTEAUTOPATCHMsgParser;
	g_pUserMsgParser[MP_TOPLIST] = MP_TopList_Client_MsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;
	g_pServerMsgParser[MP_TOPLIST] = MP_TopList_Sever_MsgParser;

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

#ifdef _DBTHREAD
	desc.dwCustomDefineEventNum = 1;
#else
	desc.dwCustomDefineEventNum = 3;
#endif

	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 100;
	desc.dwMaxUserNum = 3000;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 128000;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 3000;
	desc.dwFlag = 0;

	g_Network.Init(&desc);

	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		MessageBox(NULL,"服务初始化失败(StartServer)",NULL,NULL);
		return;
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("DistributeDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"数据库初始化失败",0,0);

	m_DBThreadTerminate = FALSE;
#ifdef _DBTHREAD
	CreateThread(0,0,ProcessDBMessageThread,0,0,&m_DBThreadID);
#endif
	LoginCheckInit();
//#ifdef _HK_LOCAL_
//	LoadSAT();
//	LoadAC();
//#endif
//	LoadFireWallIP();

	// Dump
//#ifdef _JAPAN_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _CHINA_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _HK_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _TL_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
//#endif

	g_bReady = TRUE;
	IsCanReg=TRUE;
	g_Console.LOG(4, "--->[登录网关启动完成]");

	if(IsCanReg)
		g_Console.LOG(4,"--->[账号注册开启]");
	else
		g_Console.LOG(4,"--->[账号注册关闭]");
	
	g_bReady = TRUE;
	//[DIS启动从数据库获取排行榜信息][By:十里坡剑神][QQ:112582793][2017/11/25]
	TOPLISTMGR->ReloadTopList();
	g_Console.WaitMessage();
}

//#ifdef _HK_LOCAL_
//void CServerSystem::LoadSAT()
//{
//	CMHFile file;
//	file.Init("Resource/SAT.bin","rb");
//	if(file.IsInited() == FALSE)
//	{
//		return;
//	}
//
//	file.GetDword();
//	for(int n=0;n<30;++n)
//	{
//		file.GetWord();
//		m_aniTimeMale[n] = file.GetDword();
//		file.GetDword();
//	}
//
//	file.Release();
//}
//
//void CServerSystem::LoadAC()
//{
//	CMHFile file;
//	file.Init("Resource/AbilityCalcInfo.bin","rb");
//	if(file.IsInited() == FALSE)
//	{
//		return;
//	}
//
//	for(int n=0;n<30;++n)
//	{
//		file.GetDword();
//		file.GetDword();
//		file.GetFloat();
//		file.GetFloat();
//		file.GetDword();
//		m_ac[n] = file.GetDword();
//		file.GetDword();
//		file.GetDword();
//		file.GetDword();
//		file.GetDword();
//		file.GetFloat();
//		file.GetFloat();
//		file.GetDword();
//		file.GetDword();
//	}
//
//	file.Release();
//}
//#endif

void CServerSystem::LoadFireWallIP()
{
	CMHFile file;
	if( !file.Init( "firewallip.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	while(1)
	{
		if( file.IsEOF() )
			break;

		if( file.GetInt() == g_nServerSetNum )
		{
			SafeStrCpy( m_strFireWallIP, file.GetString(), 16 );
			if( *m_strFireWallIP != 0 )
				m_bUserFireWall = TRUE;

			break;
		}

	}

	file.Release();	
}

void CServerSystem::ConnectionCheck()
{	
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;
	
//#ifdef _HK_LOCAL_
//	static DWORD nLastEncryptCheck = gCurTime;
//	if( gCurTime - nLastEncryptCheck >= 1000*60*60 )	//1矫埃
//	{
//		g_Encrypt0 = g_Encrypt1;
//		g_Encrypt1 = ( g_Encrypt1 + 1 ) % 100;
////		g_Console.LOG( 4, "Encrypt Changed : 0:%d, 1:%d", g_Encrypt0, g_Encrypt1 );
//		nLastEncryptCheck = gCurTime;
//	}
//#endif

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

	g_DB.ProcessingQueue();
	//[排行榜更新进程][By:十里坡剑神][QQ:112582793][2018/2/28]
	TOPLISTMGR->TopListPorcess();
}


void CServerSystem::End()
{
	m_DBThreadTerminate = TRUE;
	
	g_DB.Release();
	
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

void CServerSystem::SetNation()
{
	CMHFile file;
	if( !file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	if( strcmp( file.GetString(), "*NATION" ) == 0 )
	{
		if( strcmp( file.GetString(), "CHINA" ) == 0 )
		{
			m_Nation = eNATION_CHINA;
		}
	}

	file.Release();
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
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey;
	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
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
	g_pServerSystem->ConnectionCheck();
}

void __stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}

void ButtonProc1()
{
	g_Console.LOG(eLogDisplay, "--->[当前用户连接数量[%d]]", g_pUserTable->GetUserCount());
}
//[账号注册开关][By:十里坡剑神][QQ:112582793][2017/11/23]
void RegOpenOrClose()
{
	if(CServerSystem::IsCanReg)
	{
		CServerSystem::IsCanReg	=	FALSE;
		g_Console.LOG(4,"--->[注册已经关闭]");
	}
	else if(!CServerSystem::IsCanReg)
	{
		CServerSystem::IsCanReg	=	TRUE;
		g_Console.LOG(4,"--->[注册已经开启]");
	}
}


void OnCommand(char* szCommand)
{
	strupr(szCommand);
	char Header[256] = {0,};	
	sscanf(szCommand,"%s",Header);
	START_STRING_COMPARE(Header)
		COMPARE("SETBACKDOOR")
			g_bCloseBackDoor = !g_bCloseBackDoor;
		 if(g_bCloseBackDoor)
			g_Console.LOG(4,"--->[后门权限关闭]");
		 else
			 g_Console.LOG(4,"--->[后门权限开启]");
	END_COMPARE
}

BOOL CServerSystem::IsInvalidCharInclude( char* pStr )
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}



