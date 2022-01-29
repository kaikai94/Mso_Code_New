#include "stdafx.h"
#include "ReConnect.h"
#include "MainGame.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "MHCamera.h"
#include "AppearanceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "MHTimeManager.h"
#include "cMsgBox.h"
#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "MotionManager.h"
#include "cScriptManager.h"
#include "./Input/UserInput.h"
#include "GameIn.h"
#include "MainTitle.h"
#include "GameCheckManager.h"
extern HWND _g_hWnd;
extern char g_szHeroIDName[];

extern char g_CLIENTVERSION[];
extern char g_AgentAddr[16];
extern WORD g_AgentPort;
extern VECTOR3 gHeroPos;
extern BOOL g_SafeLockIsOpen;
GLOBALTON(CReConnect)
CReConnect::CReConnect(void)
{
	dwTimeCount = 10000;
}

CReConnect::~CReConnect(void)
{
}
#include "ObjectManager.h"
#include "MoveManager.h"
#include "PetManager.h"
#include "Pet.h"
#include "SkillManager_client.h"
#include "AIManager.h"
#include "ObjectStateManager.h"
BOOL CReConnect::Init(void* pInitParam)
{
	SKILLMGR->ReLoadSkillInfo();
	GAMEIN->InitInternetIP();
	m_imageLoginBarTop.LoadSprite("image/2D/login_bar00.TIF");
	m_imageLoginBarBottom.LoadSprite("image/2D/login_bar01.TIF");
	m_imageLoginBackGround.LoadSprite("image/2D/login.tga");

	NETWORK->m_crypt.DisableEncy();
	NETWORK->SetCurState(this);

	if(MAP->IsInited() == FALSE)
	{
		MAP->InitMap(TITLE->sInitConfig.wMapNum);
	}
	else
	{
		MAP->Release();
		MAP->InitMap(TITLE->sInitConfig.wMapNum);		
	}
	CAMERA->InitCamera( TITLE->sInitConfig.CameraNum, TITLE->sInitConfig.AngleX, TITLE->sInitConfig.AngleY, TITLE->sInitConfig.fDistance, &TITLE->sInitConfig.vPivot);

	WINDOWMGR->SetcbDropProcess(cbDragDropProcess);
	WINDOWMGR->AfterInit();
	WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, "");
	sprintf(m_MaintainaceNoteMsg,"网络连接中断^n^n");
	dwStepTime = gCurTime;
	//if(!sGAMECHECK->CheckAutoPatch())
	//	RunAutoPatch(FALSE);
	return TRUE;
}
void CReConnect::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	
	m_imageLoginBackGround.Release();
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(24);
	if(pNextGameState == NULL)
		MAP->Release();
}

void CReConnect::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}
void CReConnect::AfterRender()
{
	VECTOR2 v2Pos = { 0, 0 };
	VECTOR2 PosScale = { 1, 1 };

	VECTOR2 v2TopPos = { 0, 0 };
	VECTOR2 v2BottomPos = { 0, 640 	};

	float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	v2BottomPos.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-128;
	PosScale.x =scaleRate; 

	m_imageLoginBarTop.RenderSprite(&PosScale,0,0,&v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&PosScale,0,0,&v2BottomPos,0xffffffff);

	WINDOWMGR->Render();
}
void CReConnect::Process()
{
	if(m_bIsWaiting) return;

	if(gCurTime-dwStepTime<=dwTimeCount) 
	{
		cMsgBox * pMsgBox = (cMsgBox *)WINDOWMGR->GetWindowForID(MBI_NOBTNMSGBOX);
		if(pMsgBox)
		{
			char buff[512]={0,};
			sprintf(buff,"^n   %s   请等待[%d]秒....",m_MaintainaceNoteMsg,(dwTimeCount-(gCurTime-dwStepTime))/1000);
			pMsgBox->SetMsgBoxText(buff);
		}
		return;
	}

	switch(m_Step)
	{
	case eToConnectDis:ConnectDisServer();break;
	case eSendLoginMsg:SendLoginSYN();break;
	case eSendGetCharList:SendCharListSYN();break;
	case eSendGetMapInfo:SendMapInfoSYN();break;
	}
	if(m_Step==eNull)
	{
		dwTimeCount = 30000;
		sprintf(m_MaintainaceNoteMsg,"等待服务端确认^n^n");
	}
	if(gCurTime-dwSendMsgTime>=30000)
	{//[30秒未收到消息回复返回第一步][BY:十里坡剑神][QQ:112582793][2019-4-11][17:07:33]
		sprintf(m_MaintainaceNoteMsg,"等待超时尝试重新连接^n^n");
		RetrunStep(eToConnectDis);
		NETWORK->Disconnect();
	}
}
void CReConnect::ConnectDisServer()
{//[链接登陆网关][BY:十里坡剑神][QQ:112582793][2019-4-11][10:35:08]
	if(NETWORK->ConnectToServer(m_ReDisInfo.ReConncetDistrIP,m_ReDisInfo.wDistrPort)==FALSE)
	{
		sprintf(m_MaintainaceNoteMsg,"连接登陆网关失败^n^n");
		dwTimeCount = 30000;
		dwStepTime = gCurTime;
	}
	else
	{
		dwTimeCount = 10000;
		dwStepTime = gCurTime;
	}
	dwSendMsgTime = gCurTime;
}
void CReConnect::RetrunStep(BYTE step)
{//[重置重连进程][BY:十里坡剑神][QQ:112582793][2019-4-11][15:43:50]
	m_Step = step;
	
	dwStepTime = gCurTime;
	m_bIsWaiting = FALSE;
	if(eToConnectDis==step)
	{
		dwTimeCount = 30000;
		NETWORK->m_crypt.DisableEncy();
	}
	else
		dwTimeCount = 10000;
}
#include "UserInfoManager.h"
void CReConnect::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	dwSendMsgTime = gCurTime;
	switch(Category) 
	{	
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
				//[登陆网关连接成功][BY:十里坡剑神][QQ:112582793][2019-4-11][15:37:05]
				case MP_USERCONN_DIST_CONNECTSUCCESS:DisConnectSuccess(pMsg);break;
					//[登陆失败][BY:十里坡剑神][QQ:112582793][2019-4-11][15:36:08]
				case MP_USERCONN_LOGIN_NACK:LoginNAck(pMsg);	break;
					//[账号登陆成功断开登陆网关连接，开始连接角色网关][BY:十里坡剑神][QQ:112582793][2019-4-11][15:35:26]
				case MP_USERCONN_LOGIN_ACK:LoginAck(pMsg);break;
					//[连接角色网关成功发送消息获取角色信息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:35:01]
				case MP_USERCONN_AGENT_CONNECTSUCCESS:SendCharListSYN();break;
					//[查询角色信息成功开始获取地图频道信息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:34:20]
				case MP_USERCONN_CHARACTERLIST_ACK:CharListAck(pMsg);break;
					//[角色信息查询失败][BY:十里坡剑神][QQ:112582793][2019-4-11][15:32:49]
				case MP_USERCONN_CHARACTERLIST_NACK:CharListNAck();break;
					//[频道信息获取成功，初始化地图进入游戏][BY:十里坡剑神][QQ:112582793][2019-4-11][15:34:37]
				case MP_USERCONN_CHANNELINFO_ACK:MapChannelInfoAck(pMsg);break;
					//[频道信息获取失败][BY:十里坡剑神][QQ:112582793][2019-4-11][20:16:59]
				case MP_USERCONN_CHANNELINFO_NACK:MapChannelInfoNack();break;
			}//end switch(Protocol)
		}//end case MP_USERCONN:
		break;

	default:
		break;
	}//end switch Category
}
//[网络消息解析函数][BY:十里坡剑神][QQ:112582793][2019-4-11][20:06:18]
void CReConnect::DisConnectSuccess(void*pMsg)
{
	sprintf(m_MaintainaceNoteMsg,"连接登陆网关开始验证登陆^n^n");
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	m_DistAuthKey = pmsg->dwObjectID;
	TITLE->m_DistAuthKey = m_DistAuthKey;
	TITLE->m_bInit = TRUE;
	SendLoginSYN();
}
void CReConnect::SendLoginSYN()
{
	//[发送登录信息][BY:十里坡剑神][QQ:112582793][2019-4-11][11:18:13]
	MSG_LOGIN_SYN msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_LOGIN_SYN;
	SafeStrCpy(msg.id, m_ReDisInfo.UserAccount, MAX_NAME_LENGTH + 1 );
	SafeStrCpy(msg.pw, m_ReDisInfo.UserPwd, MAX_NAME_LENGTH + 1 );
	SafeStrCpy(msg.pn, "1111", MAX_NAME_LENGTH + 1 );
	SafeStrCpy(msg.Version,g_CLIENTVERSION, 33 );
	SafeStrCpy(msg.IP,GAMEIN->GetInternetIP().c_str(),18);
	msg.AuthKey = m_DistAuthKey;;
	NETWORK->Send(&msg,sizeof(msg));
	dwTimeCount = 10000;
	dwStepTime = gCurTime;
	m_Step=eNull;
}
void CReConnect::LoginNAck(void*pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	sprintf(m_MaintainaceNoteMsg,"登陆失败^n^n");
	if(pmsg->dwData1==LOGIN_ERROR_OVERLAPPEDLOGIN||pmsg->dwData1==LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER)
	{//[账号重复登陆发送踢出登陆账号消息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:36:15]
		MSG_LOGIN_SYN msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
		SafeStrCpy(msg.id, m_ReDisInfo.UserAccount, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pw, m_ReDisInfo.UserPwd, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pn, "1111", MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.Version,g_CLIENTVERSION, 33 );
		SafeStrCpy(msg.IP,GAMEIN->GetInternetIP().c_str(),18);
		msg.dwObjectID = 0;
		NETWORK->Send(&msg,sizeof(msg));
	}

	RetrunStep(eSendLoginMsg);
	//NETWORK->Disconnect();
}
void CReConnect::LoginAck(void*pMsg)
{
	sprintf(m_MaintainaceNoteMsg,"尝试连接角色网关^n^n");
	MSG_LOGIN_ACK* pmsg = (MSG_LOGIN_ACK*)pMsg;
	strcpy(g_AgentAddr,pmsg->agentip);
	g_AgentPort = pmsg->agentport;
	OnLoginAccountOK(g_AgentAddr,g_AgentPort);
	TITLE->m_UserIdx	= pmsg->userIdx;
	MAINGAME->SetUserLevel( (int)pmsg->cbUserLevel );

	gUserID = pmsg->userIdx;
	USERINFOMGR->SetSaveFolderName( gUserID );

	TITLE->m_bDisconntinToDist = TRUE;
	TITLE->m_dwDiconWaitTime	= gCurTime;
	dwTimeCount = 10000;
	dwStepTime = gCurTime;
	NETWORK->m_bDisconnecting = FALSE;
	SendOutMsg();
	//NETWORK->Disconnect();
	m_Step=eToConnectAG;
}
void CReConnect::SendCharListSYN()
{
	sprintf(m_MaintainaceNoteMsg,"获取角色信息^n^n");	
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTERLIST_SYN;
	msg.dwData1 = TITLE->m_UserIdx;
	msg.dwData2 = m_DistAuthKey;
	NETWORK->Send(&msg,sizeof(msg));
	dwTimeCount = 10000;
	dwStepTime = gCurTime;
	m_Step=eNull;
}
void CReConnect::CharListAck(void*pMsg)
{
	sprintf(m_MaintainaceNoteMsg,"获取地图信息^n^n");
	SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
	NETWORK->SetKey( pmsg->eninit, pmsg->deinit );
	NETWORK->m_crypt.SetInit(TRUE);
	SendMapInfoSYN();
}
void CReConnect::SendMapInfoSYN()
{
	//[发送获取地图频道信息消息][BY:十里坡剑神][QQ:112582793][2019-4-11][15:39:23]
	MSGSHANNELINFOSYN msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
	msg.dwObjectID = m_ReDisInfo.dwPlayerID;
	msg.bIsReConn = TRUE;
	msg.wMapNum = m_ReDisInfo.wMapNum;
	NETWORK->Send( &msg, sizeof(msg) );
	dwTimeCount = 10000;
	dwStepTime = gCurTime;
	m_Step=eNull;
}
void CReConnect::CharListNAck()
{
	sprintf(m_MaintainaceNoteMsg,"角色信息获取失败^n^n");	
	cMsgBox* MsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_NOBTNMSGBOX);
	RetrunStep(eSendGetCharList);
	//NETWORK->Disconnect();
}
void CReConnect::MapChannelInfoAck(void*pMsg)
{
	sprintf(m_MaintainaceNoteMsg,"同步数据进入游戏^n^n");
	GameLoadingParam loadingParam;
	loadingParam.selectedCharNum = m_ReDisInfo.dwPlayerID;
	loadingParam.m_MapNum = m_ReDisInfo.wMapNum;

	MAINGAME->SetGameState(eGAMESTATE_GAMELOADING, (void *)&loadingParam, sizeof(GameLoadingParam));
	dwTimeCount = 10000;
	dwStepTime = gCurTime;
	m_bIsWaiting = TRUE;
	m_Step=eNull;

}
void CReConnect::MapChannelInfoNack()
{
	sprintf(m_MaintainaceNoteMsg,"地图信息获取失败^n^n");	
	RetrunStep(eSendGetMapInfo);
	//NETWORK->Disconnect();
}
#include "ChatManager.h"
void CReConnect::OnDisconnect()
{
	if(m_Step==eToConnectAG)
	{
		if(NETWORK->ConnectToServer(m_ReDisInfo.ReConnectAgentIP,m_ReDisInfo.wAgentPort) == FALSE)
		{
			dwTimeCount = 30000;
			m_Step = eToConnectDis;
			sprintf(m_MaintainaceNoteMsg,"角色网关连接失败^n^n");	
		}
	}
	else
	{
		dwTimeCount = 30000;
		sprintf(m_MaintainaceNoteMsg,"网络连接中断^n^n");	
	}
}

void CReConnect::OnConnecToDistServer(char* ip,WORD port)
{//[记录DIS端口，IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:45:58]
	strcpy(m_ReDisInfo.ReConncetDistrIP,ip);
	m_ReDisInfo.wDistrPort=port;
}

void CReConnect::OnLoginAccount(char *pAccoutn, char *pPswrd)
{//[记录账号密码][BY:十里坡剑神][QQ:112582793][2019-4-11][9:46:29]
	strcpy(m_ReDisInfo.UserAccount,pAccoutn);
	strcpy(m_ReDisInfo.UserPwd,pPswrd);
}

void CReConnect::OnLoginAccountOK(char* ip,WORD port)
{//[记录AG端口,IP][BY:十里坡剑神][QQ:112582793][2019-4-11][9:47:08]
	strcpy(m_ReDisInfo.ReConnectAgentIP,ip);
	m_ReDisInfo.wAgentPort=port;
}
void CReConnect::OnOpenSafeLock(char *Pwd)
{//[记录安全码][BY:十里坡剑神][QQ:112582793][2019-4-11][9:47:52]
	strcpy(m_ReDisInfo.SafePwd,Pwd);
}

void CReConnect::InitForReConnect(BOOL bIsWait/* = FALSE*/)
{
	m_bIsWaiting = bIsWait;
	m_bReConnecting = TRUE;
	if(m_ReDisInfo.dwHeroPos==0&&HERO)
	{
		
		VECTOR3 vPos = {0,};
		HERO->GetPosition(&vPos);
		stMOVEPOINT Mp;
		Mp.SetMovePoint(vPos.x,vPos.z);
		m_ReDisInfo.dwHeroPos = Mp.value;
	}
	m_ReDisInfo.bAIWorking=sCPlayerAI->IsOpen();
	sCPlayerAI->SetOpen(FALSE);
	sCPlayerAI->SaveConfig();
	if (HERO)
	{
		CPet * pPet = HERO->GetPet();
		if (pPet)
		{//[保存宠物信息][BY:十里坡剑神][QQ:112582793][2019-4-11][10:01:43]
			REDISCONNECT->SetPetDBIndex(pPet->GetPetSommonItemDBIdx());
		}
	}
}

void CReConnect::SendOpenSafeLock()
{
	if(!m_bReConnecting||strlen(m_ReDisInfo.SafePwd)==0||!g_SafeLockIsOpen) return;
	MSG_SAFELOCK msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_SAFELOCK;
	msg.Protocol = MP_SAFELOCK_UNLOCK_SYN;
	msg.dwObjectID = HEROID;
	SafeStrCpy(msg.Pwd,m_ReDisInfo.SafePwd,20);
	NETWORK->Send(&msg,sizeof(msg));
}


void CReConnect::WirteReDisInfo()
{
	FILE * fp;
	char buf[64];
	fopen_s(&fp,"./Data/ReDisInfo.dat","w+");
	if(fp)
	{
		fwrite(&m_ReDisInfo,sizeof(m_ReDisInfo),1,fp);
		fclose(fp);
	}
}
void CReConnect::ReadReDisInfo()
{
	FILE * fp;
	memset(&m_ReDisInfo,0,sizeof(m_ReDisInfo));
	fopen_s(&fp,"./Data/ReDisInfo.dat","r");
	if(fp)
	{
		fread(&m_ReDisInfo,sizeof(m_ReDisInfo),1,fp);
		fclose(fp);
		DeleteFile("./Data/ReDisInfo.dat");
		m_bIsWaiting = TRUE;
		
		int initKind = eGameInInitKind_Login;
		MAINGAME->SetGameState(eGAMESTATE_REDISCONNECT,&initKind,sizeof(int));
	}
}

void CReConnect::RunAutoPatch(BOOL bVal)
{
	char pCurPath[MAX_PATH];
	std::string Patch;
	GetCurrentDirectory( MAX_PATH, pCurPath);
	Patch = pCurPath;
	Patch += "\\";
	Patch += "MHAutoPatch.exe";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	if(bVal)
	{
		Patch += " ";
		Patch += "RunClient";
	}
	if(CreateProcess(NULL, (LPSTR)Patch.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
		if(bVal)
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}
#include "ItemManager.h"
void CReConnect::StopReCnnMode()
{

	m_bReConnecting=FALSE;
	m_bIsWaiting = FALSE;
	m_ReDisInfo.dwHeroPos = 0;
	m_Step = 0;
	sCPlayerAI->SetOpen(m_ReDisInfo.bAIWorking);
	if(m_ReDisInfo.dwPetDBIndex!=0)
	{
		CItem * pItem = ITEMMGR->GetItem(m_ReDisInfo.dwPetDBIndex);
		if(pItem&&GAMEIN->GetInventoryDialog())
			GAMEIN->GetInventoryDialog()->UseItem(pItem);
	}
	if(m_ReDisInfo.AddType!=64)
	{
		MSGAUTOPOINT msg;
		msg.dwObjectID = HEROID;
		msg.Category=MP_CHAR;
		msg.Protocol = MP_CHAR_AUTOADD_SYN;
		msg.AddType = m_ReDisInfo.AddType;
		NETWORK->Send(&msg,sizeof(msg));
	}
	VECTOR3 Pos;
	Pos.x=gHeroPos.x+2;
	Pos.z=gHeroPos.z+2;
	Pos.y=gHeroPos.y;
	MOVEMGR->SetHeroTarget(&Pos,1.0f);
}