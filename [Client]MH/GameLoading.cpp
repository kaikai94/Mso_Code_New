// GameLoading.cpp: implementation of the CGameLoading class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameLoading.h"
#include "MainGame.h"
#include "GameIn.h"
#include "mhMap.h"
#include "cWindowSystemFunc.h"

#include "./WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "SkillManager_Client.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GameResourceManager.h"
#include "MHAudioManager.h"
#include "ReConnect.h"
extern HWND _g_hWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGameLoading)
CGameLoading::CGameLoading()
{
	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	m_MapNum = -1;

	m_pDlg	= NULL;
}

CGameLoading::~CGameLoading()
{

}

BOOL CGameLoading::Init(void* pInitParam)
{
	char filename[256];
	AUDIOMGR->StopBGM();
	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);

	GameLoadingParam * lParam = (GameLoadingParam *)pInitParam;

	MSG_CHARACTERSELECTSYN	msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CHARACTERSELECT_SYN;	
	msg.wData1		= gChannelNum;
	msg.dwObjectID	= lParam->selectedCharNum;
	if(!REDISCONNECT->IsInReCnnMode())
	{
		msg.wData2		=	0;
		msg.bIsConn		= FALSE;
	}
	else
	{
		msg.wData2		= lParam->m_MapNum;
		msg.bIsConn		= TRUE;
	}
	NETWORK->Send(&msg, sizeof(msg));

	sprintf(filename,"Image/LoadingMap/maploadingimage%02d.tga",lParam->m_MapNum);

	
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Image/LoadingMap/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();
	
	///////////////////////////////////////////////////////////////////////
	CMHFile fp;

	if( !fp.Init( ".\\Image\\LoadingTip.bin", "rb" ) )	return FALSE;	

	WORD TipCount = fp.GetWord();

	WORD CurTip = (rand() % TipCount) + 1;

	sprintf(filename,"Image/LoadingMap/LoadingTip%02d.tga",CurTip);
	m_TipImage.LoadSprite(filename);

	RECT rect;
	rect.top = 0;
	rect.bottom = 28;
	rect.left = 0;
	rect.right = 619;

	m_TipImage.SetImageSrcRect(&rect);

	fp.Release();

	WINDOWMGR->AfterInit();

	m_bFirst = TRUE;
	m_MapNum = -1;

	g_UserInput.SetInputFocus( FALSE );

	USERINFOMGR->SetMapChage( FALSE );

	return TRUE;
}
void CGameLoading::Release(CGameState* pNextGameState)
{
	m_LoadingImage.Release();

	m_TipImage.Release();
	WINDOWMGR->DestroyWindowAll();
	m_pDlg = NULL;
	RESRCMGR->ReleaseResource(28);
}

void CGameLoading::Process()
{
	EnterGame();
	if(m_bFirst)
		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
			//LOGFILE("Init EffectManager");
			EFFECTMGR->Init();
			
			m_bThreadTerminated = TRUE;
		}
		if(m_MapNum != (MAPTYPE)(-1) && m_bMapLoadComplete == FALSE && m_bThreadTerminated)
		{
			//LOGFILE("MAP->InitMap(m_MapNum);");
			MAP->InitMap(m_MapNum);

			//LOGFILE("PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());");
			PATHMGR->SetMap(MAP, m_MapNum,MAP->GetTileWidth());	
			m_bMapLoadComplete = TRUE;
		}
	}

	return;
}
void CGameLoading::BeforeRender()
{
}
void CGameLoading::AfterRender()
{
	WINDOWMGR->Render();
	VECTOR2 tr;
	tr.x = tr.y = 0;
	VECTOR2 sc;
	sc.x  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	sc.y  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/ MIDRSLTN_H;
	m_LoadingImage.RenderSprite(&sc,NULL,0,&tr,0xffffffff);

	VECTOR2 tip, sz;

	tip.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-420;
	tip.y = GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-40;
	sz.x = sz.y=1;

    m_TipImage.RenderSprite(&sz,NULL,0,&tip,0xffffffff);	
}

void CGameLoading::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTERSELECT_ACK:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
					m_MapNum = pmsg->bData;
					REDISCONNECT->OnEnterMap(m_MapNum);//[第一次进入地图记录][BY:十里坡剑神][QQ:112582793][2019-4-11][13:47:28]
				}
				return;

			case MP_USERCONN_CHARACTERSELECT_NACK:
				{
					LOG(EC_MAPSERVER_CLOSED);
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
				}
				return;
			}
		}
		break;
	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
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
	case MP_JACKPOT:
		break;
	}
}

void CGameLoading::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		int initKind = eGameInInitKind_Login;
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&initKind,sizeof(int));
	}
}

