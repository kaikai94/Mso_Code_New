








// MapChange.cpp: implementation of the CMapChange class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapChange.h"

#include "MainGame.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cResourceManager.h"
#include "cWindowSystemFunc.h"

#include "mhMap.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "PathManager.h"
#include "LoadingDlg.h"
#include "./Interface/cScriptManager.h"

#include "./Input/UserInput.h"
#include "UserInfoManager.h"
#include "ChatManager.h"

#include "GMNotifyManager.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "GameResourceManager.h"
#include "MHAudioManager.h"
#include "SurvivalModeManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CMapChange)
CMapChange::CMapChange()
{
	m_pDlg = NULL;
	m_GameInInitKind = eGameInInitKind_Login;
	m_bThreadTerminated = FALSE;
	m_bMapLoadComplete = FALSE;
	m_bFirst = FALSE;

	hThread = NULL;

	m_MapNum = 0;
}

CMapChange::~CMapChange()
{


}
#include "ReConnect.h"
BOOL CMapChange::Init(void* pInitParam)
{
	AUDIOMGR->StopBGM();

	m_MapNum = *((MAPTYPE*)pInitParam);
	
//	CreateGameLoading_m();

	REDISCONNECT->OnEnterMap(m_MapNum);//[��ͼת����¼��ͼ���][BY:ʮ���½���][QQ:112582793][2019-4-11][13:47:28]
	char filename[256];
	sprintf(filename,"Image/LoadingMap/maploadingimage%02d.tga",m_MapNum);
	if(m_LoadingImage.LoadSprite(filename) == FALSE)
	{
		sprintf(filename,"Image/LoadingMap/maploadingimage00.tga");
		m_LoadingImage.LoadSprite(filename);
	}
	m_pDlg = (CLoadingDlg *)CreateGameLoading_m();

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

	m_bThreadTerminated=FALSE;
	m_bMapLoadComplete=FALSE;
	NETWORK->SetCurState(this);
	
	m_bFirst = TRUE;

	g_UserInput.SetInputFocus( FALSE ); 
	if(m_MapNum == 44)
		USERINFOMGR->SetMapChage( FALSE );
	else
		USERINFOMGR->SetMapChage( TRUE );
	
	return TRUE;
}
void CMapChange::Release(CGameState* pNextGameState)

{
	m_LoadingImage.Release();

	m_TipImage.Release();
	WINDOWMGR->DestroyWindowAll();
	m_pDlg = NULL;
	RESRCMGR->ReleaseResource(28);	
}

void CMapChange::Process()
{
	EnterGame();


	if(m_bFirst)

		m_bFirst = FALSE;
	else
	{
		if(m_bThreadTerminated == FALSE)
		{
			// Loading...
			
			m_bThreadTerminated = TRUE;
		}
		if(m_bMapLoadComplete == FALSE)
		{
			MAP->InitMap(m_MapNum);
			PATHMGR->SetMap(MAP, m_MapNum, MAP->GetTileWidth());
//			MINIMAP->InitMiniMap(m_MapNum);
			m_bMapLoadComplete = TRUE;
		}
	}
	
	return;
}

void CMapChange::BeforeRender()
{
	
}

void CMapChange::AfterRender()
{
	WINDOWMGR->Render();

	VECTOR2 tr;
	tr.x = tr.y = 0;
	VECTOR2 sc;


	sc.x  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	sc.y  = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / MIDRSLTN_H;
	m_LoadingImage.RenderSprite(&sc,NULL,0,&tr,0xffffffff);

	VECTOR2 tip, sz;
	tip.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-420;
	tip.y = GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-40;
	sz.x = sz.y=1;
    m_TipImage.RenderSprite(&sz,NULL,0,&tip,0xffffffff);
}

void CMapChange::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_FRIEND:
		break;
		
	case MP_PARTY:

		break;

	case MP_CHAT:
		CHATMGR->NetworkMsgParse(Protocol, pMsg);
		break;


	case MP_MUNPA:
		break;
	case MP_SIGNAL:
		break;
	case MP_GTOURNAMENT:
		GTMGR->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_JACKPOT:
		break;
	case MP_PET:
		break;
	case MP_SIEGEWAR:
		SIEGEMGR->NetworkMsgParse(Protocol, pMsg);
		break;
	case MP_SURVIVAL:
		//SVVMODEMGR->NetworkMsgParse(Protocol, pMsg);
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

//					if( GAMEIN->GetEventNotifyDialog() )

//					{
//						GAMEIN->GetEventNotifyDialog()->SetTitle( pmsg->strTitle );
//						GAMEIN->GetEventNotifyDialog()->SetContext( pmsg->strContext );

//						GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
//						NOTIFYMGR->SetEventNotifyChanged( FALSE );
//						//����?
//					}
//					else
//					{
						NOTIFYMGR->SetEventNotifyChanged( TRUE );
//					}

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
//					if( GAMEIN->GetEventNotifyDialog() )
//						GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
				}	
				break;

			}
		}
		break;

	default:
		{
			char temp[256];
			sprintf(temp,"Category:%d Protocol:%d",Category,Protocol);

			ASSERTMSG(0,temp);
		}
	}
}

void CMapChange::EnterGame()
{
	if(m_bMapLoadComplete && m_bThreadTerminated && !m_bDisconnected )
	{
		MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&m_GameInInitKind,sizeof(int));
	}
}




