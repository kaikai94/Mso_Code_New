// MainTitle.cpp: implementation of the CMainTitle class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainTitle.h"
#include "MainGame.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "MHCamera.h"
#include "AppearanceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "ItemManager.h"
#include "KyungGongManager.h"
#include "ChatManager.h"
#include "MHTimeManager.h"
#include "FilteringTable.h"
#include "cMsgBox.h"
#include "SkillManager_client.h"
#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "MotionManager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_Interface.h"
#include "AbilityUpdater_ApplyData.h"
#include "ServerListDialog.h"
#include "CharSelect.h"
#include "DissolutionManager.h"
#include "cMonsterSpeechManager.h"
#include "PetSpeechManager.h"
#include "TitanManager.h"
#include "OptionManager.h"
#include "BillboardDialog.h"	//[个人排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "GuildTopList.h"		//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "BillBoardManager.h"	//[排行管理][By:十里坡剑神][QQ:112582793][2017/12/5]
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
//#ifdef TAIWAN_LOCAL
//#include "BillingManager.h"
//#endif
#include "GMNotifyManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "UserInfoManager.h"
#include "cScriptManager.h"
#include "./Input/UserInput.h"
#include "Intro.h"
#include "IntroReplayDlg.h"
#include "GameIn.h"
#include "ErrorMsg.h"
#include <time.h>
#include <shellapi.h>
#include "cKillTopList.h"		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
//#include "MaintainaceNoteDlg.h" //[维护公告][By:十里坡剑神][QQ:112582793][2018/6/14]
#include "ReConnect.h"
#include "AIManager.h"
#include "CheckResourceFile.h"
#define _LOGOWINDOW_

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char g_AgentAddr[16];
extern WORD g_AgentPort;
extern char g_szHeroIDName[];
extern int	g_nServerSetNum;
extern BOOL bSiegeWarMasterID;
extern DWORD bSiegeWarMasterChar;
extern char m_CryptKey[32];
#define ALPHA_PROCESS_TIME	1200
#define WAIT_CONNECT_TIME	60000	


void SendOutMsg()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_REQUEST_DISTOUT;
//#ifdef _HK_LOCAL_
//	msg.dwObjectID = 5434;	//5434
//#endif
	NETWORK->Send(&msg, sizeof(msg));
}

BOOL bFirst;
GLOBALTON(CMainTitle)
CMainTitle::CMainTitle()
{
//#ifdef TAIWAN_LOCAL
//	m_pAdvice		= NULL;
//#endif
	m_bInit			= FALSE;

	m_pServerListDlg = NULL;
	memset( m_DistributeAddr, 0, 16 );
	m_DistributePort = 0;
	m_bServerList = TRUE;

	m_bDisconntinToDist = FALSE;
	m_bDynamicDlg = FALSE;
//#ifdef _HK_LOCAL_
//	m_dwEnc = 0;
//#endif
	m_pIntroReplayDlg = NULL;
	LoadInitInfo();
	REDISCONNECT->ReadReDisInfo();
}


CMainTitle::~CMainTitle()
{	
}

static BOOL g_bResourceLoaded = FALSE;

void	CMainTitle::LoadInitInfo()
{
	char buff[64]={0,};
	CMHFile file;
	if(file.Init("./Resource/Client/MainTitle.bin","rb")==FALSE)
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
		COMPARE("#MAPNUM")
			sInitConfig.wMapNum = file.GetWord();
		COMPARE("#VPOINT")
			sInitConfig.vPivot.x = file.GetFloat();
			sInitConfig.vPivot.y = file.GetFloat();
			sInitConfig.vPivot.z = file.GetFloat();
		COMPARE("#CAMERANUM")
			sInitConfig.CameraNum = file.GetWord();
		COMPARE("#ANGLE")
			sInitConfig.AngleX = file.GetFloat();
			sInitConfig.AngleY = file.GetFloat();
		COMPARE("#DISTANCE")
			sInitConfig.fDistance = file.GetFloat();
		END_COMPARE
	}
	file.Release();
}
BOOL CMainTitle::Init(void* pInitParam)
{

	m_imageLoginBarTop.LoadSprite("image/2D/login_bar1.TIF");
	m_imageLoginBarBottom.LoadSprite("image/2D/login_bar01.TIF");
	m_imageLoginBackGround.LoadSprite("image/2D/login.tga");

	m_bInit			= FALSE;
	m_pServerListDlg = NULL;
	memset( m_DistributeAddr, 0, 16 );
	m_DistributePort = 0;
	
	m_bDisconntinToDist = FALSE;
	m_bNoDiconMsg		= FALSE;
	m_bDynamicDlg = FALSE;

	m_pIntroReplayDlg = NULL;

	NETWORK->SetCurState(this);

	if(MAP->IsInited() == FALSE)
	{
		MAP->InitMap(sInitConfig.wMapNum);//108//94
	}
	else
	{
		MAP->Release();
		MAP->InitMap(sInitConfig.wMapNum);		
	}

	if(g_bResourceLoaded == FALSE)
	{
		FILTERTABLE->Init();
		CHATMGR->Init();
		MON_SPEECHMGR->Init();		
		PET_SPEECHMGR->Init();
		MOTIONMGR->LoadMotionList();
		SKILLMGR->Init();
		ITEMMGR->LoadItemList();
		ITEMMGR->LoadShopItemTypeInfo();
		ITEMMGR->LoadShopItemInfo();
		ITEMMGR->LoadMixItemInfo();
		ITEMMGR->LoadValuablesList();//[贵重物品信息加载][BY:十里坡剑神][QQ:112582793][2019-8-28][12:59:37]
		ITEMMGR->LoadPacketInfoList();

		ITEMMGR->LoadItemToolTipList();
		ITEMMGR->LoadSetItemOption();
		ITEMMGR->LoadRareItemInfo();		
		GAMERESRCMNGR->LoadMonsterList();
		GAMERESRCMNGR->LoadMonsterInfoInMapList();
		GAMERESRCMNGR->LoadMapChangeArea();
		GAMERESRCMNGR->LoadUserCommendList();
		GAMERESRCMNGR->LoadNpcChxList();
		GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
		GAMERESRCMNGR->LoadItemChxList();
		GAMERESRCMNGR->LoadPetList();
		GAMERESRCMNGR->LoadPetRule();
		GAMERESRCMNGR->LoadPetBuffList();
		GAMERESRCMNGR->LoadTitanList();
		GAMERESRCMNGR->LoadExpPoint();
		GAMERESRCMNGR->LoadTacticStartInfoList();
		GAMERESRCMNGR->LoadPreDataTable();
		GAMERESRCMNGR->LoadServerList();
		GAMERESRCMNGR->LoadShopItemDupList();
		GAMERESRCMNGR->LoadFlagNpcInfo();
		KYUNGGONGMGR->Init();
		APPEARANCEMGR->Init();
		GAMERESRCMNGR->PreLoadData();
		ABILITYMGR->Init();
		ABILITYMGR->AddUpdater(CAbilityUpdater_Interface::GetInstance());
		ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance());
		DISSOLUTIONMGR->Init();
		TITANMGR->LoadTPMList();
		TITANMGR->LoadTitanPartsKindList();
		TITANMGR->LoadTitanUpgradeInfoList();
		TITANMGR->LoadTitanBreakInfoList();
		GAMERESRCMNGR->LoadTitanRule();	
		GAMERESRCMNGR->LoadUniqueItemOptionList();
		GAMERESRCMNGR->LoadUniqueItemMixList();
		GAMERESRCMNGR->LoadNomalClothesSkinList();
		GAMERESRCMNGR->LoadCostumeSkinList();
		GAMERESRCMNGR->LoadMapKindInfo();
		g_bResourceLoaded = TRUE;
		GAMERESRCMNGR->LoadPlusItemEffect();
		sCPlayerAI->LoadSetFile();
	}

	/*VECTOR3 vPivot;
	vPivot.x = 0;
	vPivot.y = 0;
	vPivot.z = 0;*/
	//CAMERA->InitCamera( 0, 0.6f, 360.f, -500, &vPivot);
	CAMERA->InitCamera( sInitConfig.CameraNum, sInitConfig.AngleX, sInitConfig.AngleY, sInitConfig.fDistance, &sInitConfig.vPivot);

	WINDOWMGR->SetcbDropProcess(cbDragDropProcess);

	CreateMainTitle_m();	

	m_pServerListDlg = (CServerListDialog*)WINDOWMGR->GetWindowForID( SL_SERVERLISTDLG );
	if( m_pServerListDlg )
	{
		m_pServerListDlg->Linking();
		m_pServerListDlg->SetActive( FALSE );

	}
	WINDOWMGR->AfterInit();

	m_bWaitConnectToAgent = FALSE;

	bFirst = TRUE;

	m_pIntroReplayDlg = (CIntroReplayDlg*)WINDOWMGR->GetWindowForID( INTRO_DLG );
	if( m_pIntroReplayDlg )
	{
		m_pIntroReplayDlg->SetAbsXY(GET_MAINWIN_W - 41, GET_MAINWIN_H - 120);
		m_pIntroReplayDlg->SetActive( FALSE );
	}
	InitMainTitleMonsters();
	return TRUE;
}

void CMainTitle::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	
	m_imageLoginBackGround.Release();
	RemoveMainTitleMonsters();
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(24);
	if(pNextGameState == NULL)
		MAP->Release();
}

void CameraRotate(CKeyboard& Keyboard,CMouse& Mouse);
void CameraWheelZoom(CKeyboard& Keyboard,CMouse& Mouse);
#include "ObjectGuagen.h"
void CMainTitle::Process()
{
	ProcessMainTitleMonsters();
	if(bFirst == TRUE)
	{
		bFirst = FALSE;
		CAMERA->MouseRotate(1,0);
		m_dwStartTime = MHTIMEMGR->GetNewCalcCurTime();
	}

	if( m_bServerList )
	{
		m_pServerListDlg->SetActive( TRUE );
		m_bServerList = FALSE;
		m_pIntroReplayDlg->SetActive( TRUE );	
	}

	if( m_bInit )
	{
		if( m_bDynamicDlg == FALSE )
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pDlg )
			if( !pDlg->IsActive() )
			{
				pDlg->SetDisable( FALSE );
				pDlg->SetActive( TRUE );
				cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID( MT_IDEDITBOX );
				pEdit->SetFocusEdit( TRUE );
				if(WINDOWMGR->GetUserCfg()->IsKeep)
				{
					pEdit->SetEditText(WINDOWMGR->GetUserCfg()->ID);
					cEditBox* pEdit1 = (cEditBox*)pDlg->GetWindowForID( MT_PWDEDITBOX );
					pEdit1->SetEditText(WINDOWMGR->GetUserCfg()->Pwd);
				}
//#ifdef _HK_LOCAL_
//				m_DistAuthKey = (m_DistAuthKey - 47)/3;
//				m_dwEnc = (m_dwEnc-m_DistAuthKey-23)/7;
//#endif

			}	
		}
		if( m_bWaitConnectToAgent == TRUE )
		{
			DWORD dwElapsed = gCurTime - m_dwWaitTime;
		
			if( dwElapsed > WAIT_CONNECT_TIME )
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
				if( pMsgBox )
					pMsgBox->ForceClose();

				m_bWaitConnectToAgent = FALSE;

				WINDOWMGR->MsgBox( MBI_TIMEOVER_TOCONNECT, MBT_OK, CHATMGR->GetChatMsg( 445 ) );
				m_pServerListDlg->SetActive( FALSE );
				m_bServerList = FALSE;

				m_pIntroReplayDlg->SetActive( FALSE );
			}
		}
	}	

}


void CMainTitle::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}
void CMainTitle::AfterRender()
{
	VECTOR2 v2Pos = { 0, 0 };
	VECTOR2 PosScale = { 1, 1 };

	VECTOR2 v2TopPos = { 0, 0 };
	VECTOR2 v2BottomPos = { 0, 640 	};

	PosScale.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	PosScale.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/MIDRSLTN_H;
	v2BottomPos.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-128;

	v2TopPos.y = -(50*PosScale.x);
	m_imageLoginBarTop.RenderSprite(&PosScale,0,0,&v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&PosScale,0,0,&v2BottomPos,0xffffffff);

	WINDOWMGR->Render();
}
void CMainTitle::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
#ifdef _GMTOOL_
			case MP_CHEAT_CHECKIP_ACK:
				{
					m_bInit = TRUE;
				}
				break;
			case MP_CHEAT_CHECKIP_NACK:
				{
					if( NETWORK->IsConnected() )
						NETWORK->Disconnect();
					WINDOWMGR->MsgBox( MBI_NOT_GM, MBT_NOBTN, CHATMGR->GetChatMsg( 162 ) );
				}
				break;
#endif
			case MP_USERCONN_USE_DYNAMIC_ACK:
				{
					StartWaitConnectToAgent( FALSE );
					cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
					if( pDlg )
					{
						pDlg->SetDisable( FALSE );
						pDlg->SetActive( FALSE );
					}
					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
					if( pMsgBox )
					{
						pMsgBox->SetDisable( FALSE );
						pMsgBox->SetActive( FALSE );
					}

					MSG_USE_DYNAMIC_ACK* pmsg = (MSG_USE_DYNAMIC_ACK*)pMsg;
					m_bDynamicDlg = TRUE;
					pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
					if( pDlg )
					{
						pDlg->SetDisable( FALSE );
						pDlg->SetActive( TRUE );

						cStatic* pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_DYNAMICID );
						pStc->SetStaticText( g_szHeroIDName );
						pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_COORD );
						pStc = (cStatic*)pDlg->GetWindowForID( MT_STC_COORD_MAT );
						pStc->SetStaticText( pmsg->mat );						
						((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetFocusEdit( TRUE );
						((cEditBox*)pDlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );
					}					
				}
				break;
			case MP_USERCONN_USE_DYNAMIC_NACK:
				{

				}
				break;

			case MP_USERCONN_DIST_CONNECTSUCCESS:
				{

//#ifndef _HK_LOCAL_
					MSG_CONNECACK* pmsg = (MSG_CONNECACK*)pMsg;
   					m_DistAuthKey = pmsg->dwObjectID;
   					SetBlock(FALSE);
   					m_bInit = TRUE;
					//NETWORK->SetKey( pmsg->eninit, pmsg->deinit );
//#else
//					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
//					m_DistAuthKey = pmsg->dwData2;
//					SetBlock(FALSE);
//					m_bInit = TRUE;
//					m_dwEnc = pmsg->dwData1;
//					m_dwOld = m_dwEnc;
//#endif
				}
				return;
			case MP_USERCONN_LOGIN_ACK:
				{
					MSG_LOGIN_ACK* pmsg = (MSG_LOGIN_ACK*)pMsg;

					strcpy(g_AgentAddr,pmsg->agentip);
					g_AgentPort = pmsg->agentport;
					m_UserIdx	= pmsg->userIdx;
					bSiegeWarMasterID = pmsg->bIsSiegeWar;
					bSiegeWarMasterChar = pmsg->dwSiegeWarChrIdx;
					REDISCONNECT->OnLoginAccountOK(g_AgentAddr,g_AgentPort);
					MAINGAME->SetUserLevel( (int)pmsg->cbUserLevel );
#ifdef _GMTOOL_
					if( pmsg->cbUserLevel <= eUSERLEVEL_GM )
						GMTOOLMGR->CanUse( TRUE );
#endif
					gUserID = m_UserIdx;
					USERINFOMGR->SetSaveFolderName( gUserID );

					m_bDisconntinToDist = TRUE;
					m_dwDiconWaitTime	= gCurTime;

					SendOutMsg();
				}
				break;
			case MP_USERCONN_SERVER_NOTREADY:
				{
					NoDisconMsg();
					OnLoginError( LOGIN_ERROR_NODISTRIBUTESERVER, 0 );
				}
				break;

			case MP_USERCONN_LOGIN_NACK:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					OnLoginError(pmsg->dwData1,pmsg->dwData2);
				}
				break;
			case MP_USERCONN_CHARACTERLIST_ACK:
				{
					SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;

					NETWORK->SetKey( pmsg->eninit, pmsg->deinit );

					if( NETWORK->IsConnected() ) 
						MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
				}
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(5));
					cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
					if( pIDDlg && pMsgBox )
					{
						pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
					}
				}
				return;
				
//#ifdef TAIWAN_LOCAL				
//			case MP_USERCONN_REMAINTIME_NOTIFY:
//				{
//					MSG_INT* pmsg = (MSG_INT*)pMsg;
//					BILLINGMGR->SetUseRemainTime(TRUE);
//					BILLINGMGR->SetRemainTime( pmsg->nData );	//
//				}
//				break;
//#endif
				
			case MP_USERCONN_AGENT_CONNECTSUCCESS:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					//NETWORK->SetKey(pmsg->eninit,pmsg->deinit);
					MSG_DWORD2 msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CHARACTERLIST_SYN;
					msg.dwData1 = m_UserIdx;
					msg.dwData2 = m_DistAuthKey;
					NETWORK->Send(&msg,sizeof(msg));
				}
				break;

			}
		}
		break;
	case MP_TOPLIST:
		{
			switch (Protocol)
			{
			case MP_TOPLIST_TO_CLIENT:
				{//[设置角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
					MSG_TOP_LIST_HERO * pmsg = (MSG_TOP_LIST_HERO*)pMsg;
					sBOARDMGR->SetTopList(pmsg);
				}
				break;
			default:
				break;
			}
		}
		break;
	case MP_CHEAT:
		{
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

					NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
					NOTIFYMGR->SetEventNotify( TRUE );

					NOTIFYMGR->SetEventNotifyChanged( TRUE );

					NOTIFYMGR->ResetEventApply();
					for(int i=0; i<eEvent_Max; ++i)
					{
						if( pmsg->EventList[i] )
							NOTIFYMGR->SetEventApply( i );
					}
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					NOTIFYMGR->SetEventNotify( FALSE );
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
				}	
				break;
			}
		}
		break;
	}
}



void CMainTitle::OnLoginError(DWORD errorcode,DWORD dwParam)
{
	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
	if( pMsgBox )
		WINDOWMGR->AddListDestroyWindow( pMsgBox );

	m_bWaitConnectToAgent = FALSE;
	
	ySWITCH(errorcode)
		yCASE(LOGIN_ERROR_INVALID_VERSION)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(9) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_OVERLAPPEDLOGIN)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_OVERLAPPEDLOGIN, MBT_YESNO, CHATMGR->GetChatMsg(8) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
		yCASE(LOGIN_ERROR_OVERLAPPEDLOGININOTHERSERVER)
			char serverName[128] = {0,};
			//for(int n=0;n<30;++n)
			//{
			//	if(GAMERESRCMNGR->m_ServerList[n].ServerNo == dwParam)
			//	{
			//		strcpy(serverName,GAMERESRCMNGR->m_ServerList[n].ServerName);
			//		break;
			//	}
			//}
			//SEVERLIST* pServerList = GAMERESRCMNGR->m_ServerList;
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_OVERLAPPEDLOGININOTHERSERVER, MBT_OK, CHATMGR->GetChatMsg(1),
												serverName, serverName);
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			NoDisconMsg();

		yCASE(LOGIN_ERROR_NOAGENTSERVER)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(7) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
			SendOutMsg();
			NoDisconMsg();
			
		yCASE(LOGIN_ERROR_NODISTRIBUTESERVER)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(7) );

		yCASE(LOGIN_ERROR_INVALIDUSERLEVEL)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(6));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_DISTSERVERISBUSY)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(674));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_WRONGIDPW)

			// 滚叼滚叼狼 版快 酒捞叼啊 绝促搁 辑厚胶荤侩沥焊 悼狼甫 救茄巴捞促.
			if( dwParam == 2 && OPTIONMGR->GetGameOption()->nLoginCombo == 3 )
			{
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_BUDDY_INFOAGREE, MBT_OK, CHATMGR->GetChatMsg(1647) );
				cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_IDEDITBOX );
				cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
				if( pEditId )
					pEditId->SetFocusEdit( FALSE );
				if( pEditPwd )
				{
					pEditPwd->SetEditText( "" );
					pEditPwd->SetFocusEdit( TRUE );
				}
				cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				pDlg1->SetDisable( FALSE );

				cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );

				if( pDlg2 )
				if( pDlg2->IsActive() )				
				{
					pDlg1->SetActive( TRUE );
					pDlg2->SetDisable( FALSE );
					pDlg2->SetActive( FALSE	);
					pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_DYNAMICPWDEDITBOX );
					pEditPwd->SetEditText( "" );
					//				pEditPwd->SetFocusEdit( TRUE );
				}
				
				return;
			}

			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
			pEdit->SetFocusEdit( FALSE );
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg(281) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

		yCASE(LOGIN_ERROR_BLOCKUSERLEVEL)
			char buf[256] = {0,};
			if( dwParam > 0 )
			{
				int hour = dwParam / 60;
				int min = dwParam - hour * 60;

				wsprintf( buf, CHATMGR->GetChatMsg( 1703 ), hour, min ) ;
			}
			else
			{
				wsprintf( buf, CHATMGR->GetChatMsg( 305 ) );
			}

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, buf );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

			SendOutMsg();
			NoDisconMsg();

			
		yCASE(LOGIN_ERROR_INVALID_IP)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_PASSERROR, MBT_OK, CHATMGR->GetChatMsg( 446 ) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		yCASE(LOGIN_ERROR_DISTCONNET_ERROR)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DISTCONNECT_ERROR, MBT_OK, CHATMGR->GetChatMsg(453) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

		yCASE(LOGIN_ERROR_MAXUSER)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(454) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
		yCASE(LOGIN_ERROR_MINOR_INADULTSERVER)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(443) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		yCASE(LOGIN_ERROR_SECEDEDACCOUNT)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(672) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
			
			SendOutMsg();
			NoDisconMsg();
			
		yCASE(LOGIN_ERROR_NOT_CLOSEBETAWINNER)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(438) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )

			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		
			SendOutMsg();
			NoDisconMsg();
		yCASE(LOGIN_ERROR_NOREMAINTIME)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, "您的充值卡里剩余，请尽快到《墨香》官方网站www.moxiang.com.cn充值。" );
		
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

			SendOutMsg();

			NoDisconMsg();
		yCASE(LOGIN_ERROR_NOIPREGEN)
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(1237) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_BUDDY_WEBFAILED)

			char msg[256] = { 0, };
			sprintf( msg, "%s (001)", CHATMGR->GetChatMsg(1645) );
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_BUDDY_IDAUTHFAILED)

			char msg[256] = { 0, };
			sprintf( msg, "%s (002)", CHATMGR->GetChatMsg(1645) );
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

			SendOutMsg();
			NoDisconMsg();

		yCASE(LOGIN_ERROR_BUDDY_AUTHCODEFAILED)

			if( dwParam == 99 )
			{
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_BUDDY_NAMECER, MBT_OK, CHATMGR->GetChatMsg(1646) );
			}
			else
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (003)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}

			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");

					editboxPwd->SetFocusEdit(FALSE);
				}
			}

			TITLE->NoDisconMsg();
			TITLE->ShowServerList();

		yCASE(LOGIN_ERROR_PROTECTIONNUMBER)

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 1690 ) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		
		yCASE(LOGIN_ERROR_PROTECTIONNUMBER_REGIST)

				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 1692 ) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}

		yCASE(LOGIN_ERROR_PROTECTIONNUMBER_INSERT)	

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg( 1693 ) );
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		yCASE(LOGIN_ERROR_DOUBLERUNCHECK)//[双开检测][By:十里坡剑神][QQ:112582793][2017/11/23]
			WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(2228));
			SendOutMsg();
			NoDisconMsg();
			TITLE->ShowServerList();		
	yENDSWITCH
}

void CMainTitle::OnDisconnect()
{
	if(m_bDisconntinToDist)
	{
		if(NETWORK->ConnectToServer(g_AgentAddr, g_AgentPort) == FALSE)
		//if(NETWORK->ConnectToServer("222.186.173.71", 14600) == FALSE)
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_VERCHKERROR, MBT_OK, CHATMGR->GetChatMsg(7));
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		}
		
		m_bDisconntinToDist = FALSE;
	}
	else if( !m_bNoDiconMsg )
	{
		OnLoginError(LOGIN_ERROR_DISTCONNET_ERROR, 0);

		cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pDlg )
		{
			pDlg->SetDisable( TRUE );
		}

		NETWORK->ReleaseKey();
	}

	m_bNoDiconMsg = FALSE;
}
void CMainTitle::ConnectToServer(int Type, int index )
{
	if( index > -1 )
	{
		SEVERLIST(* pServerList)[50] = GAMERESRCMNGR->m_ServerList;
		if( !pServerList[Type][index].bEnter )	return;

		m_pServerListDlg->SetDisable( TRUE );
		m_pIntroReplayDlg->SetDisable( TRUE );	
			
		if( NETWORK->ConnectToServer( pServerList[Type][index].DistributeIP, pServerList[Type][index].DistributePort ) == FALSE )
		{
			OnLoginError( LOGIN_ERROR_NODISTRIBUTESERVER, 0 );
		}
		//[为重连保存选择服务器索引][By:十里坡剑神][QQ:112582793][2017/12/5]
		REDISCONNECT->OnConnecToDistServer(pServerList[Type][index].DistributeIP,pServerList[Type][index].DistributePort);
		m_pServerListDlg->SetDisable( FALSE );
		m_pServerListDlg->SetActive( FALSE );

		m_pIntroReplayDlg->SetDisable( FALSE );
		m_pIntroReplayDlg->SetActive( FALSE );
		
		g_nServerSetNum = pServerList[Type][index].ServerNo;
	}
	m_ConnectionServerNo = index;
}

void CMainTitle::ShowServerList()
{
	m_bServerList = TRUE;


	cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( FALSE );
	}
	pDlg = WINDOWMGR->GetWindowForID(MT_DYNAMICDLG);
	if( pDlg )
	{
		pDlg->SetDisable( FALSE );
		pDlg->SetActive( FALSE );
	}

	m_bDynamicDlg = FALSE;

	pDlg = WINDOWMGR->GetWindowForID(NUMBERPAD_DLG);
	if(pDlg)
	{		
		pDlg->SetActive( FALSE );
	}

	if( NETWORK->IsConnected() )
		NETWORK->Disconnect();
	m_bInit = FALSE;
	FILECHECK->SetSendInit();//[初始化地域信息发送][BY:十里坡剑神][QQ:112582793][2019-9-17][10:08:04]
}

void CMainTitle::StartWaitConnectToAgent( BOOL bStart )
{
	m_dwWaitTime			= gCurTime;
	m_bWaitConnectToAgent	= bStart;
}


//#ifndef TAIWAN_LOCAL
void CMainTitle::InitMonster(SEND_MONSTER_TOTALINFO& info, DWORD& dwID, DWORD& dwIndex, VECTOR3& pos,char * Name)
{
	
	BASEMOVE_INFO				bminfo;
	CMonster*					pMonster = NULL;

	memset(&info, 0, sizeof(info));

	info.BaseObjectInfo.dwObjectID = dwID++;
	info.BaseObjectInfo.BattleID = gChannelNum + 1;

	sprintf(info.BaseObjectInfo.ObjectName, Name);

	m_av3LastPosition[dwIndex] = pos;

	info.MoveInfo.CurPos.Compress(&pos);

	bminfo.SetFrom(&info.MoveInfo);
	bminfo.SetCurTargetPosIdx(0);

	bminfo.SetMaxTargetPosIdx(0);
	bminfo.SetTargetPosition(0, pos);
	info.TotalInfo.MonsterKind = 143;

	m_apMonster[dwIndex] = OBJECTMGR->AddMonster(&info.BaseObjectInfo, &bminfo, &info.TotalInfo);
	m_adwLastActionTime[dwIndex] = gCurTime;

	MOVEMGR->InitMove(m_apMonster[dwIndex], &bminfo);
}
void CMainTitle::InitMainTitleMonsters(void)
{
	srand( (unsigned)time( NULL ) );

	SetRect(&m_rcMainTitleMonsterArea, 18000, 2400, 20000, 3600);

	memset(m_apMonster, 0, sizeof(m_apMonster));
	memset(m_av3Target, 0, sizeof(m_av3Target));	
	memset(m_adwLastActionTime, 0, sizeof(m_adwLastActionTime));
	memset(m_aiLastAction, 0, sizeof(m_aiLastAction));

	SEND_MONSTER_TOTALINFO		info;
	VECTOR3 pos;

	pos.x = 18447.0f;
	pos.y = 0.0f;
	pos.z = 3200.0f;
	DWORD dwID = 10;
	DWORD dwIndex = 0;	
	InitMonster(info, dwID, dwIndex,pos,"MainTitleMonster01");

	++dwIndex;
	pos.x += 250;	
	pos.z -= 100;
	InitMonster(info, dwID, dwIndex, pos, "MainTitleMonster02");

	++dwIndex;
	pos.x += 350;	
	InitMonster(info, dwID, dwIndex, pos, "MainTitleMonster03");
}


void CMainTitle::RemoveMainTitleMonsters(void)
{
	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)
	{
		OBJECTMGR->AddGarbageObject(m_apMonster[i]);
		m_apMonster[i] = NULL;
	}
//#ifdef _HK_LOCAL_
//	ZeroMemory( m_enkey, sizeof(m_enkey) );
//#endif
}

BOOL IsNear(VECTOR3* pv3Pos1, VECTOR3* pv3Pos2, int iMoveMode)
{
	float fCheckRange = 0.0f;

	if (iMoveMode == eMoveMode_Walk)
	{
		fCheckRange = 5.0f;
	}
	else
	{
		fCheckRange = 15.0f;
	}

	if (fCheckRange > fabs(pv3Pos1->x - pv3Pos2->x) &&
		fCheckRange > fabs(pv3Pos1->z - pv3Pos2->z))
	{
		return TRUE;
	}

	return FALSE;
}

void CMainTitle::ProcessMainTitleMonsters(void)
{	
	const int	c_aiReactionTime[_MAX_MAINTITLE_MONSTER_NUM] = { 900, 1300, 1500 };
	

	VECTOR3		pos = { 0.0f, 0.0f, 0.0f };	
	CMonster*	pMonster = NULL;
	BYTE		byState = 0;

	int i = 0;
	for (i = 0; i < _MAX_MAINTITLE_MONSTER_NUM; i++)	
	{
		pMonster = m_apMonster[i];	
		byState = pMonster->GetState();
		
		if (eObjectState_None == byState)
		{
			if (m_adwLastActionTime[i] + c_aiReactionTime[i] >= gCurTime)
			{
				continue;
			}			

			const int c_iMoveModeRun = 0;			

			int iRand = rand();
			int iAction = iRand / ( RAND_MAX / 5 );	

			if (2 < iAction)
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				continue;
			}

			if (0 != m_aiLastAction[i])
			{
				m_adwLastActionTime[i] = gCurTime;
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
				m_aiLastAction[i] = !m_aiLastAction[i];
				continue;				
			}

			int iMoveX = rand() / (RAND_MAX / 350);
			int iMoveZ = rand() / (RAND_MAX / 350);

			if (60 > iMoveX)
			{
				iMoveX = 60;
			}

			if (40 > iMoveZ)
			{
				iMoveZ = 40;
			}

			
			if (0== rand() / (RAND_MAX / 2) )
			{
				iMoveX = -iMoveX;
			}

			if (0 == rand() / (RAND_MAX /2) )
			{
				iMoveZ = -iMoveZ;
			}									
			
			pMonster->GetPosition(&pos);			

			if (0.0f == pos.x && 0.0f == pos.z)
			{
				pMonster->SetPosition(&m_av3LastPosition[i]);
			}

			if ( (m_rcMainTitleMonsterArea.left	>= (int)pos.x + iMoveX)  )
			{
				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.right <= (int)pos.x + iMoveX) )
			{
				iMoveX = 0;
			}

			if ( (m_rcMainTitleMonsterArea.top >= (int)pos.z + iMoveZ) )
			{			

				iMoveZ = 0;
			}

			if ( (m_rcMainTitleMonsterArea.bottom	<= (int)pos.z + iMoveZ) )
			{
				iMoveZ = 0;
			}			


			if (0 == iMoveX && 0 == iMoveZ)
			{
				continue;
			}

			pos.x += (float)iMoveX;		
			pos.z += (float)iMoveZ;
			
			m_av3Target[i] = pos;

			MOVEMGR->SetMonsterMoveType(pMonster, eMA_WALKAROUND);
						
			MOVEMGR->StartMoveEx(pMonster,gCurTime, &pos);
			
			m_adwLastActionTime[i] = gCurTime;
			m_aiLastAction[i] = !m_aiLastAction[i];
		}
		else		
		{
			VECTOR3 v3Pos;						
			pMonster->GetPosition(&v3Pos);

			if (TRUE == IsNear(&v3Pos, &m_av3Target[i], MOVEMGR->GetMoveMode(pMonster)))
			{
				MOVEMGR->MoveStop(pMonster, &v3Pos);
				MOVEMGR->SetMonsterMoveType(pMonster, eMA_STAND);
					
				BASEMOVE_INFO bmif;
				
				pMonster->GetBaseMoveInfo(&bmif);
				bmif.SetTargetPosition(&m_av3Target[i]);
				MOVEMGR->InitMove(pMonster, &bmif);				
				
				m_av3LastPosition[i] = v3Pos;				
			}
		}	
	}
	
}


//#endif





