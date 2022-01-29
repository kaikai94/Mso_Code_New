// MainGame.cpp: implementation of the CMainGame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainGame.h"
#include "MainTitle.h"
#include "Intro.h"
#include "ServerConnect.h"
#include "CharSelect.h"
#include "CharMake.h"
#include "GameLoading.h"
#include "GameIn.h"
#include "MapChange.h"
#include "MurimNet.h"
#include "MHTimeManager.h"
#include "mhnetwork.h"
#include "./Engine/Engine.h"
#include "./Interface/cResourceManager.h"
#include "./Interface/cWindowManager.h"

#include "../[CC]Header/GameResourceManager.h"
#include "AppearanceManager.h"
#include "ObjectManager.h"
#include "OptionManager.h"

#include "StringLoader.h"
#include "MovePoint.h"
#include "./Input/cIMEWnd.h"
#include "./Audio/MHAudioManager.h"

#include "mhMap.h"
#include "./Input/UserInput.h"

#include "CQuestManager.h"
#include "MacroManager.h"
#include "MixManager.h"
#include "NpcScriptManager.h"
#include "HelpDicManager.h"

#include "AbilityManager.h"
#include "CheatMsgparser.h"

#include "MouseCursor.h"
#include "ChatManager.h"
//050630SW
#include "cChatTipManager.h"
//-
#include "ReinforceManager.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "GuildManager.h"
#include "SkillDelayManager.h"

#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "./Engine/EngineObject.h"
#include "./Engine/EngineObjectCache.h"
#endif
#include "cScriptManager.h"
#include "VideoCaptureManager.h"
#include "ReConnect.h"


GLOBALTON(CMainGame);
CMainGame* pTempMainGame = NULL;
HWND gHwnd = NULL;



CMainGame::CMainGame()
{
	ASSERT(!pTempMainGame);


	m_bChangeState = FALSE;
	m_ChangeState = -1;
	m_ppGameState = NULL;
	m_pCurrentGameState = NULL;
	m_pEngine = NULL;
	m_bEndGame = FALSE;
	m_pStateInitParam = NULL;

	pTempMainGame = this;
	m_nUserLevel = eUSERLEVEL_USER;	
}

CMainGame::~CMainGame()
{
}

void CMainGame::Init(HWND hMainWnd)
{
	srand( GetTickCount() );

#ifndef _HACK_SHIELD_
	DIRECTORYMGR->Init();
#endif

	DIRECTORYMGR->SetLoadMode(eLM_EffectScript);
	LoadEffectFileTable("list_m.befl");
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	
	m_pEngine = new CEngine;
	m_pEngine->Init(hMainWnd,&GAMERESRCMNGR->m_GameDesc.dispInfo,
		GAMERESRCMNGR->m_GameDesc.MaxShadowNum,GAMERESRCMNGR->m_GameDesc.MaxShadowTexDetail,0,
		GAMERESRCMNGR->m_GameDesc.FramePerSec);

	RESRCMGR->Init(FILE_IMAGE_PATH, FILE_IMAGE_MSG);

	SCRIPTMGR->InitParseTypeData(); 

	NETWORK->Init(hMainWnd);

	WINDOWMGR->Init();

	gHwnd = hMainWnd;
	STRINGLOADER->Init(".\\Resource\\string.mhs");

	OPTIONMGR->Init();	
	OPTIONMGR->ApplySettings();	
	
	MACROMGR->Init();		
	MIXMGR->LoadMixList();

	REINFORCEMGR->LoadReinforceItemInfo();
	REINFORCEMGR->LoadRareReinnforceItemInfo();

	NPCSCRIPTMGR->Init();
	HELPDICMGR->Init();
	AUDIOMGR->Initialize( hMainWnd );
	MOVEPOINT->Init();
	g_UserInput.Init();
	GUILDMGR->Init();
	CHATTIPMGR->Init();
	SKILLDELAYMGR->LoadSkillUseInfo();

#ifndef _RESOURCE_WORK_NOT_APPLY_
	g_pEngineObjectCache = new CEngineObjectCache;
	g_pEngineObjectCache->Init(g_pExecutive, CEngineObjectCache::MAX_ENGINE_OBJECT_CACHE_SIZE);
#endif
}




void CMainGame::Release()
{
	MOVEPOINT->Release();

	if(m_pCurrentGameState)
	{
		m_pCurrentGameState->Release(NULL);
		m_pCurrentGameState = NULL;
	}

	CHATTIPMGR->Release();

	EFFECTMGR->Release();
	
	WINDOWMGR->Release();

	RESRCMGR->Release();

	NETWORK->Release();

	SAFE_DELETE_ARRAY(m_pStateInitParam);

	AUDIOMGR->Terminate();

	g_UserInput.Release();

	CURSOR->Release();

	GUILDMGR->Release();

#ifndef _RESOURCE_WORK_NOT_APPLY_
	g_pEngineObjectCache->Release();
	delete g_pEngineObjectCache;
	g_pEngineObjectCache = NULL;
#endif
	if(m_pEngine)
	{
		m_pEngine->Release();
		delete m_pEngine;
		m_pEngine = NULL;
	}
}

void CMainGame::SetGameState(int StateNum,void* pStateInitParam,int ParamLen)
{
	m_bChangeState = TRUE;
	m_ChangeState = StateNum;
	if(m_ChangeState == eGAMESTATE_END)
	{
		m_bEndGame = TRUE;
#ifdef _GMTOOL_
	GMTOOLMGR->DestroyGMDialog();
#endif
	}

	//ASSERT(m_pStateInitParam == NULL);
	if(pStateInitParam && ParamLen)
	{ 
		m_pStateInitParam = new char[ParamLen];
		memcpy(m_pStateInitParam,pStateInitParam,ParamLen);
	}
}

extern BOOL g_bActiveApp;
//#include "ReDisConnManager.h"
int CMainGame::Process()
{
	if(g_bActiveApp == FALSE)
		Sleep( 10 );

	Sleep(1);	

	if(m_bEndGame)
	{
		Release();
		return -1;
	}

	BeforeRenderFunc();
	BOOL rt = m_pEngine->BeginProcess(NULL,AfterRenderFunc);
	if(rt)
	{
		if(m_pCurrentGameState)
		{
			if(m_pCurrentGameState->IsBlock() == FALSE)
			{
				MHTIMEMGR->Process();
				g_UserInput.Process();
				WINDOWMGR->Process();
				m_pCurrentGameState->Process();
			}
			AUDIOMGR->Process();
		}
	}

	if(m_pCurrentGameState == GAMEIN && GAMEIN->IsGameInAcked() == FALSE)
	{
		g_pExecutive->GetRenderer()->BeginRender(0,0,0);
		g_pExecutive->GetRenderer()->EndRender();
	}
	VIDEOCAPTUREMGR->Process();
	m_pEngine->EndProcess();
	

	if(m_bChangeState)
	{
		if(m_pCurrentGameState)
		{
			m_pCurrentGameState->Release(GetGameState(m_ChangeState));
			m_pCurrentGameState->SetBlock(FALSE);
		}
		
		m_pCurrentGameState = GetGameState(m_ChangeState);

		if(!m_pCurrentGameState)
		{
			Release(); 

			return -1;
		}

		m_nCurStateNum = m_ChangeState; 
		CIMEWND->SetDockingEx( NULL, TRUE ); 
	}


#ifndef _RESOURCE_WORK_NOT_APPLY_
	APPEARANCEMGR->ProcessReservation();
	APPEARANCEMGR->ProcessAlphaAppearance();	
#endif

	APPEARANCEMGR->ProcessAppearance();
	OBJECTMGR->ProcessGarbage();
#ifndef _RESOURCE_WORK_NOT_APPLY_
	g_pEngineObjectCache->ProcessCacheDelete();
#endif
	if(m_bChangeState)
	{
		m_pCurrentGameState->SetBlock(FALSE);
		if( m_pCurrentGameState->Init(m_pStateInitParam) == FALSE )
		{

			if( m_nCurStateNum == eGAMESTATE_TITLE )	
			{
				m_pCurrentGameState->SetBlock( FALSE );
				TITLE->OnLoginError( LOGIN_ERROR_NODISTRIBUTESERVER, 0 );
			}
			else if(m_nCurStateNum == eGAMESTATE_INTRO)
			{
                MAINGAME->SetGameState(eGAMESTATE_CONNECT);
				return 0;
			}
			else
			{
				MessageBox( 0, CHATMGR->GetChatMsg(444), 0, 0 );
				SAFE_DELETE_ARRAY(m_pStateInitParam);
				Release();
				return -1;				
			}
		}
		SAFE_DELETE_ARRAY(m_pStateInitParam);

		m_bChangeState = FALSE;
	}
	return 0;
}

void CMainGame::BeforeRender()
{
	if(m_pCurrentGameState)
		m_pCurrentGameState->BeforeRender();
}


void CMainGame::AfterRender()
{
	if(m_pCurrentGameState)
		m_pCurrentGameState->AfterRender();
}


void CMainGame::PauseRender( BOOL bPause )
{
	m_pEngine->PauseRender( bPause);
}


CGameState* CMainGame::GetGameState(int StateNum)
{
	CGameState* pState=NULL;
	switch(StateNum) 
	{
	case eGAMESTATE_INTRO:
		pState = INTRO;
		break;
	case eGAMESTATE_CONNECT:
		pState = SERVERCONNECT;
		break;
	case eGAMESTATE_TITLE:
		pState = TITLE;
		break;
	case eGAMESTATE_CHARSELECT:
		pState = CHARSELECT;
		break;
	case eGAMESTATE_CHARMAKE:
		pState = CHARMAKE;
		break;
	case eGAMESTATE_GAMELOADING:
		pState = GAMELOADING;
		break;
	case eGAMESTATE_GAMEIN:
		pState = GAMEIN;
		break;
	case eGAMESTATE_MAPCHANGE:
		pState = MAPCHANGE;
		break;
	case eGAMESTATE_MURIMNET:
		pState = MURIMNET;
		break;
	case eGAMESTATE_END:
		pState = NULL;
		break; 
	case eGAMESTATE_REDISCONNECT:
		pState = REDISCONNECT;
	default:
		break;
	}
	return pState;
}


DWORD _stdcall BeforeRenderFunc()
{
	pTempMainGame->BeforeRender();
	return 0;
}

DWORD _stdcall AfterRenderFunc()
{
	pTempMainGame->AfterRender();
	return 0;
}
DWORD _stdcall AfterRenderFunc__()
{
	WINDOWMGR->Render();
	return 0;
}


DWORD __stdcall MapLoadCallback(DWORD dwCurCount,DWORD dwTotalCount,void* pArg)
{
	g_pExecutive->Run(0xff000000,NULL,AfterRenderFunc__,0);
	g_pExecutive->GetGeometry()->Present(NULL);
	return 0;
}




