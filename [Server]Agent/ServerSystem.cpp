// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "AgentNetworkMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"

#include <MemoryPoolTempl.h>

#include "AgentDBMsgParser.h"
#include "Crypt.h"

#include "FilteringTable.h"
#include "MHTimeManager.h"
#include "BootManager.h"
#include "MHFile.h"
#include "AgentDBMsgParser.h"
#include "TrafficLog.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

#include "SkillDalayManager.h"
#include "JackpotManager_Agent.h"
#include "BobusangManager_Agent.h"
#include "PunishManager.h"

#include "..\[CC]ServerModule\MiniDumper.h"
#include <fstream>
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"
#endif

#ifdef _NETWORKTEST
DWORD RecvServerMsgAmount[MP_MAX] = {0,};
DWORD RecvUserMsgAmount[MP_MAX] = {0,};
extern DWORD SendServerMsgAmount[MP_MAX];
extern DWORD SendUserMsgAmount[MP_MAX];
#endif
//#ifdef _TESTVER_
//#include "../[Server]Distribute/MD5Checksum.h"
//#endif
void _stdcall fConnectionCheck(DWORD dwEventIndex);

DWORD g_dwAcceptUser = 0;
BOOL g_bCloseWindow = FALSE;
extern DWORD g_dwAcceptUser2;
extern int	g_nServerSetNum;
BOOL g_bCloseBackDoor = TRUE;
HWND g_hWnd;

void RloadGoldShopInfo();

CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;
#ifdef _NPROTECT_
CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif
void RloadDropRate();
void RloadBobuSangInfo();
CServerSystem::CServerSystem()
{
	//CoInitialize(NULL);
	m_bUseCrypt = TRUE;
	m_Nation = eNATION_KOREA;
	m_bUseEventNotify = FALSE;
	ZeroMemory( m_strEventNotifyTitle, sizeof( m_strEventNotifyTitle ) );
	ZeroMemory( m_strEventNotifyContext, sizeof( m_strEventNotifyContext ) );
	memset( m_bEvenList, 0, sizeof(BOOL)*eEvent_Max );
	m_bTestServer = FALSE;
	m_CloseClient = FALSE;
	m_CloseServer = FALSE;
	m_dwCheckTime = 0;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

void CServerSystem::Start(WORD ServerNum)
{
	SetNation();
//#ifdef _TESTVER_
	//m_wServerNum = 0;
//#else
	m_wServerNum = ServerNum;
//#endif
	srand(GetTickCount());
	JACKPOTMGR->Start();
	BOBUSANGMGR->BobusangMgr_Start();

	MENU_CUSTOM_INFO pMenu[4] = {0,};
	strcpy(pMenu[0].szMenuName, "连接检查");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName,"更新商城信息");
	pMenu[1].cbMenuCommand = RloadGoldShopInfo;

	strcpy(pMenu[2].szMenuName,"重加载比例设置");
	pMenu[2].cbMenuCommand = RloadDropRate;
	
	strcpy(pMenu[3].szMenuName,"重加载商人信息");
	pMenu[3].cbMenuCommand = RloadBobuSangInfo;

	if(!g_Console.Init(4,pMenu,OnCommand))
		MessageBox(NULL,"控制台初始化失败",0,0);

	char TitleText[255];

	sprintf(TitleText, "墨香角色网关[%d]----[By:你不知道的天语]----[QQ:你猜]", ServerNum);
	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, TitleText);
	
	SetMapChangeInfo();
	g_pServerMsgParser[0] = NULL;	
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = TransToClientMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_AGENTSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_MAP] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMServerMsgParser;
	g_pServerMsgParser[MP_ITEMEXT] = MP_ITEMServerMsgParserExt;
	g_pServerMsgParser[MP_CHAT] = MP_CHATServerMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MUGONG] = TransToClientMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = TransToClientMsgParser;
	g_pServerMsgParser[MP_CHEAT] = TransToClientMsgParser;
	g_pServerMsgParser[MP_QUICK] = TransToClientMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = MP_PACKEDMsgParser;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYServerMsgParser;	

	g_pServerMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pServerMsgParser[MP_NOTE] = MP_NOTEServerMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDServerMsgParser;
	
	g_pServerMsgParser[MP_AUCTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;
	g_pServerMsgParser[MP_MURIMNET] = MP_MURIMNETServerMsgParser;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATServerMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDServerMsgParser;
	g_pServerMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARServerMsgParser;	
	g_pServerMsgParser[MP_JACKPOT] = MP_JACKPOTServerMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillServerMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONServerMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITServerMsgParser;
	g_pServerMsgParser[MP_SURVIVAL] = MP_SURVIVALServerMsgParser;
	g_pServerMsgParser[MP_BOBUSANG] = MP_BOBUSANGServerMsgParser;
	g_pServerMsgParser[MP_OPTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEMLIMIT] = MP_ITEMLIMITServerMsgParser;
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTEServerMsgParser;
	g_pServerMsgParser[MP_FORTWAR] = MP_FORTWARServerMsgParser;
	g_pServerMsgParser[MP_MARRY] = MP_MARRYMsgParser;          
	g_pServerMsgParser[MP_CLIENT] = MP_CLIENTMsgParser;    
	//[服务端骇客消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:26:20]
	g_pServerMsgParser[MP_HACKCHECK] = MP_Hacker_Map_MsgParser;
	//[排行更新][BY:十里坡剑神][QQ:112582793][2019-9-4][23:56:29]
	g_pServerMsgParser[MP_TOPLIST] = MP_TopList_Server_MsgParser;
	g_pServerMsgParser[MP_UNIONWAR] = MP_UnionWar_Server_MsgParser;
#ifdef _NPROTECT_
	g_pServerMsgParser[MP_NPROTECT] = MP_NPROTECTUserMsgParser;
#endif
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = TransToMapServerMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pUserMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pUserMsgParser[MP_MURIMNET] = MP_MURIMNETUserMsgParser;
	g_pUserMsgParser[MP_OPTION] = MP_OPTIONUserMsgParser;
	g_pUserMsgParser[MP_DEBUG] = MP_DebugMsgParser;
	g_pUserMsgParser[MP_CHEAT] = MP_CHEATUserMsgParser;
	g_pUserMsgParser[MP_GUILD] = MP_GUILDUserMsgParser;
	g_pUserMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARUserMsgParser;
	g_pUserMsgParser[MP_ITEM] = MP_ITEMUserMsgParser;
	g_pUserMsgParser[MP_ITEMEXT] = MP_ITEMUserMsgParserExt;
	g_pUserMsgParser[MP_PARTY] = MP_PARTYUserMsgParser;		
	g_pUserMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTUserMsgParser;
	g_pUserMsgParser[MP_JACKPOT] = MP_JACKPOTUserMsgParser;
	g_pUserMsgParser[MP_SKILL] = MP_SkillUserMsgParser;
	g_pUserMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR] = MP_SIEGEWARUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITUserMsgParser;
	g_pUserMsgParser[MP_SURVIVAL] = MP_SURVIVALUserMsgParser;
	g_pUserMsgParser[MP_BOBUSANG] = MP_BOBUSANGUserMsgParser;

	g_pUserMsgParser[MP_EXCHANGE] = MP_EXCHANGEUserMsgParser;
	g_pUserMsgParser[MP_STREETSTALL] = MP_STREETSTALLUserMsgParser;

	g_pUserMsgParser[MP_WEATHER] = MP_WEATHERUserMsgParser;
	g_pUserMsgParser[MP_AUTONOTE] = MP_AUTONOTEUserMsgParser;
	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	g_pUserMsgParser[MP_SINGED] = MP_SINGEDMsgParser;
	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	g_pUserMsgParser[MP_RECHARGE]=MP_RechargMsgParser;
	g_pUserMsgParser[MP_MARRY] = MP_MarryUserMsgParser;
	g_pUserMsgParser[MP_CLIENT] = MP_CLIENTMsgParser;  
	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/23]
	g_pUserMsgParser[MP_KILLTOPLIST] = MP_KillTopListInfoMsgParser;
	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	g_pUserMsgParser[MP_SAFELOCK] = MP_SafeLockAGMsgParser;
	//[客户端骇客消息解析][BY:十里坡剑神][QQ:112582793][2019-6-25][14:25:18]
	g_pUserMsgParser[MP_HACKCHECK] = MP_Hacker_Client_MsgParser;
	//[排行更新][BY:十里坡剑神][QQ:112582793][2019-9-4][23:56:29]
	g_pUserMsgParser[MP_TOPLIST] = MP_TopList_Client_MsgParser;
	//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-27][12:23:34]
	//g_pUserMsgParser[MP_UNIONWAR] = MP_UnionWar_Clinet_MsgParser;

	g_UserInfoPool.Init(MAX_USER_IN_A_AGENT/2, 500,"UserInfoPool");
	g_pUserTable = new CUserTable;					
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForUserID = new CUserTable;			
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForObjectID = new CUserTable;		
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2);
	g_pServerTable = new CServerTable;				
	g_pServerTable->Init(MAX_SERVER_CONNECTION);
	
	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000*30;	
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

	desc.dwMainMsgQueMaxBufferSize = 20480000;		
	desc.dwMaxServerNum = MAX_SERVER_CONNECTION;		
	desc.dwMaxUserNum = MAX_USER_IN_A_AGENT;			
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;				

	desc.dwUserBufferSizePerConnection = 128000;
	desc.dwUserMaxTransferSize = 65535;					
	desc.dwConnectNumAtSameTime = MAX_USER_IN_A_AGENT/4;	
	desc.dwFlag = 0;

	g_Network.Init(&desc);

	BOOTMNGR->AddSelfBootList(AGENT_SERVER, ServerNum, g_pServerTable);
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		ASSERT(0);
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;

		OnConnectServerFail(&info);		
	}


	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("AgentDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"数据库初始化失败",0,0);

	FILTERTABLE->Init();
	GMINFO->Init();
	PLUSTIMEMGR->Init();
	//TRAFFIC->Init();
	SKILLDELAYMGR->LoadSkillUseInfo();
	BOBUSANGMGR->BobusangMgr_Init();
	PUNISHMGR->Init();
//#ifdef _HK_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _TL_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _CHINA_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#elif defined _JAPAN_LOCAL_
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
//#endif
	
	g_Console.LOG(4, "-----------   角色网关启动完成  -----------------");
	g_bReady = TRUE;

	char txt[128];
	sprintf(txt, "EXEC up_Agent_Down %d, %d", g_nServerSetNum , g_pServerTable->GetSelfServer()->wServerNum);
	g_DB.LoginQuery(eQueryType_FreeQuery, eAgentDown, 0, txt);

	g_Console.WaitMessage();
}
void CServerSystem::Process()
{	
	/*if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		g_bCloseWindow = FALSE;
		g_bReady = FALSE;
	}*/

	MHTIMEMGR_OBJ->Process();

	g_DB.ProcessingQueue();
	//TRAFFIC->Process();
	SHOUTMGR->Process();
	JACKPOTMGR->Process();
	BOBUSANGMGR->BobusangMgr_Process();
	PUNISHMGR->Process();
}

void CServerSystem::ConnectionCheck()
{	
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;
	
	cPtrList removelist;
	cPtrList disconnectlist;

	g_pUserTableForUserID->SetPositionHead();
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		if(pInfo->dwConnectionIndex == 0)
		{
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if(elapsedtime > _60sec*2)
			{
				removelist.AddTail(pInfo);
			}
		}
		else
		{
			//if(pInfo->UserLevel == eUSERLEVEL_PROGRAMMER)
			//	continue;

			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			
			if( elapsedtime > _60sec*10 )	
			{
				if(pInfo->bConnectionCheckFailed)		
				{
					pInfo->bIsCheck = false;
					disconnectlist.AddTail(pInfo);
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

		LoginCheckDelete(p->dwUserID);		
		g_pUserTableForUserID->RemoveUser(p->dwUserID);
		//TRAFFIC->RemoveUser(p->dwUserID);

		memset(p,0,sizeof(USERINFO));
		g_UserInfoPool.Free(p);
	}
	removelist.RemoveAll();

	pos = disconnectlist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)disconnectlist.GetNext(pos);
		DisconnectUser(p->dwConnectionIndex);
	}
	disconnectlist.RemoveAll();

	PLUSTIMEMGR->Process();
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}


void CServerSystem::End()
{
	g_bReady = FALSE;
	//TRAFFIC->End();	
	g_pUserTableForUserID->SetPositionHead();
	USERINFO* pInfo;
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		LoginCheckDelete(pInfo->dwUserID);
		g_pServerSystem->ReleaseAuthKey(pInfo->dwUniqueConnectIdx);
		g_UserInfoPool.Free(pInfo);
	}
	g_pUserTableForUserID->RemoveAll();
	PUNISHMGR->Release();
	
	g_DB.Release();
	g_Network.Release();
	g_Console.Release();

	if(g_pUserTable)
	{
		g_pUserTable->RemoveAllUser();
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pUserTableForObjectID)
	{
		g_pUserTableForObjectID->RemoveAllUser();
		delete g_pUserTableForObjectID;
		g_pUserTableForObjectID = NULL;
	}
	if(g_pServerTable)
	{
		g_pServerTable->RemoveAllServer();
		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
}

void CServerSystem::SetMapChangeInfo()
{
	char filename[64];
	CMHFile file;
	sprintf(filename,"./Resource/MapChange.bin");
	if(!file.Init(filename, "rb"))
		return;

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		
		m_MapChange[n].Kind = file.GetWord();
		SafeStrCpy( m_MapChange[n].CurMapName, file.GetString(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( m_MapChange[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		m_MapChange[n].CurMapNum = file.GetWord();
		m_MapChange[n].MoveMapNum = file.GetWord();
		m_MapChange[n].CurPoint.x = file.GetFloat();
		m_MapChange[n].CurPoint.y = 0;

		m_MapChange[n].CurPoint.z = file.GetFloat();
		m_MapChange[n].MovePoint.x = file.GetFloat();
		m_MapChange[n].MovePoint.y = 0;
		m_MapChange[n].MovePoint.z = file.GetFloat();
		m_MapChange[n].chx_num = file.GetWord();
		
		++n;
	}
	
	file.Release();
}

MAPCHANGE_INFO* CServerSystem::GetMapChangeInfo(DWORD Index)
{
	for(int i=0; i<MAX_POINT_NUM; i++)
	{
		if(m_MapChange[i].Kind == Index)
			return &m_MapChange[i];
	}

	return NULL;
}
void CServerSystem::SendMsgMapServerDown( WORD wServerNum )
{
	USERINFO* pInfo;
	g_pUserTable->SetPositionUserHead();

	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->wUserMapNum == wServerNum )
		{
			DisconnectUser(pInfo->dwConnectionIndex);
		}
	}
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
	if( file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{

		if( strcmp( file.GetString(), "*NATION" ) == 0 )
		{
			if( strcmp( file.GetString(), "CHINA" ) == 0 )
			{
				m_Nation = eNATION_CHINA;
			}
		}

		file.Release();
	}

	CMHFile file2;
	if( file2.Init( "_TESTSERVER", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		m_bTestServer = TRUE;
		file2.Release();
	}
}


void CServerSystem::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}

void CServerSystem::SetUseEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;

	if( bUse )
	{
		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, m_strEventNotifyTitle, 32 );
		SafeStrCpy( msg.strContext, m_strEventNotifyContext, 128 );

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( m_bEvenList[i] )				msg.EventList[i] = 1;
			else								msg.EventList[i] = 0;
		}

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSG_EVENTNOTIFY_ON msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_OFF;

		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSGBASE msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
}

void _stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "连接到监控服务进程: IP=%s	端口=%d	编号=%d", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "连接到服务端进程: IP=%s	端口=%d	编号=%d", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}

	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)			
		g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);	
}

void _stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	char strr[255];
	
	SERVERINFO* serverinfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);

	wsprintf(strr, "服务端进程链接断开:编号[%]", dwConnectionIndex);
	g_Console.LOG(4, strr);


	if(serverinfo)
	if( serverinfo->wServerKind == 3 )
	{
		g_pServerSystem->SendMsgMapServerDown(serverinfo->wServerNum);
	}

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;
}


void _stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(AGENT_SERVER, g_pServerSystem->GetServerNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
	}
	else
	{
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}			
}

void _stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "服务端进程连接:编号[%d]", dwConnectionIndex );
}

void _stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
	{
		g_Network.DisconnectUser( dwConnectionIndex );
		return;
	}
	
	++g_dwAcceptUser;

	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_AGENT_CONNECTSUCCESS;
	g_Network.Send2User(dwConnectionIndex, (char*)&send, sizeof(send));

	if( g_pServerSystem->IsUseEventNotify() )
	{
		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, g_pServerSystem->GetEventNotifyTitle(), 32 );
		SafeStrCpy( msg.strContext, g_pServerSystem->GetEventNotifyContext(), 128 );


		BOOL* pEventlist = g_pServerSystem->GetApplyEvent();

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( pEventlist[i] )				msg.EventList[i] = 1;
			else							msg.EventList[i] = 0;
		}
				
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}	
	
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	++myInfo->wAgentUserCnt;

	MSG_WORD2 msg2;
	msg2.Category = MP_SERVER;
	msg2.Protocol = MP_SERVER_USERCNT;
	msg2.wData1 = myInfo->wPortForServer;
	msg2.wData2 = myInfo->wAgentUserCnt;
	msg2.dwObjectID = 0;

	g_pServerTable->SetPositionHead();
	SERVERINFO* pOtherAgentInfo = NULL;
	while(pOtherAgentInfo = g_pServerTable->GetNextDistServer())
	{
		if(myInfo == pOtherAgentInfo)
			continue;

		g_Network.Send2Server(pOtherAgentInfo->dwConnectionIndex, (char *)&msg2, sizeof(msg2));
	}

	DWORD CurUserCount = g_pUserTable->GetUserCount();
	g_pUserTable->SetCalcMaxCount(CurUserCount);
	DWORD MaxUserCount = g_pUserTable->GetUserMaxCount();
	SetUserCurAndMaxCount(CurUserCount, MaxUserCount);
}

void _stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
		return;
	
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
}

void _stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	printf("Agent : ProcessDBMessage\n");
	g_DB.ProcessingDBMessage();
}

void _stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	ASSERT(g_pServerMsgParser[pTempMsg->Category] != NULL);
	
	if( pTempMsg->Category <= 0 || pTempMsg->Category >= MP_MAX ||
		g_pServerMsgParser[pTempMsg->Category] == NULL )
	{
		char buf[128];
		sprintf( buf, "Category error : %d", pTempMsg->Category );
		ASSERTMSG( 0, buf );
		return;
	}
	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void _stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);

	if( pInfo )
		if( g_pServerSystem->IsEnableCrypt() )
		{

			int headerSize = sizeof( MSGROOT );

			if( !pInfo->crypto.Decrypt( ( char * )pTempMsg +  headerSize, dwLength - headerSize ) )
			{
				char buff[128] = { 0, };
				sprintf_s(buff, "Decrypt Error CharID=%d", pInfo->dwCharacterID);
				ASSERTMSG(0, buff);
				return;
			}
			char aaa = pInfo->crypto.GetDeCRCConvertChar();
			if( pTempMsg->Code !=  aaa )
			{
				char buff[128] = { 0, };
				sprintf_s(buff, "Decrypt CRC Error CharID=%d", pInfo->dwCharacterID);
				ASSERTMSG(0, buff);
				return;
			}

			if (pInfo->cbCheckSum == 0)
			{
				pInfo->cbCheckSum = pTempMsg->CheckSum;
			}
			else if (pInfo->cbCheckSum != pTempMsg->CheckSum)
			{
				char buff[128] = { 0, };
				sprintf_s(buff, "CheckSum Error,MsgSum=%d,InfoSum=%d,CharID=%d", pTempMsg->CheckSum, pInfo->cbCheckSum, pInfo->dwCharacterID);
				ASSERTMSG(0, buff);
				pInfo->cbCheckSum = pTempMsg->CheckSum + 1;
				//return;
			}

			++pInfo->cbCheckSum;
			//TRAFFIC->AddUserPacket( pInfo->dwUserID, pTempMsg->Category );
		}

	ASSERT(g_pUserMsgParser[pTempMsg->Category] != NULL);
	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(pTempMsg->Category != 0);
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void _stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}

void _stdcall fConnectionCheck(DWORD dwEventIndex)
{
	g_pServerSystem->ConnectionCheck();
}




void ButtonProc1()
{
	int servercount = g_pServerTable->GetDataNum();
	g_Console.LOG(4, "服务端连接数量:[%d]", servercount);
	g_Console.LOG(4, "当前网关网络连接数量:[%d]", g_Network.GetINet()->GetConnectedUserNum());
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
		COMPARE("SHOWBOBUSANG")
			BOBUSANGMGR->OutPutBobusangInfo();
		COMPARE("SETCLIENT")
				g_pServerSystem->SetCloseClient();
				if (g_pServerSystem->GetCloseClient())
					g_Console.LOGEX(4, RGB(0, 255, 0), "设置成功，当前状态[踢人开启]");
				else
					g_Console.LOGEX(4, RGB(0, 255, 0), "设置成功，当前状态[踢人关闭]");
	END_COMPARE
}




void RloadDropRate()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_RLOAD;
	g_Network.Broadcast2MapServer((char*)&msg,sizeof(msg));
}
void RloadGoldShopInfo()
{
	MSGBASE msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEX_UPDATE_GOLDSHOP_SYN;
	g_Network.Broadcast2MapServer((char*)&msg,sizeof(msg));
}
void RloadBobuSangInfo()
{
	BOBUSANGMGR->RloadBobusangInfo();
}



