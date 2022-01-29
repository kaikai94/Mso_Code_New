// ServerConnect.cpp: implementation of the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerConnect.h"
#include "../Engine/GraphicEngine.h"
#include "mmsystem.h"

#include "MainGame.h"


#include ".\Interface\cWindowManager.h"
#include "cWindowSystemFunc.h"

#include "MHTimeManager.h"

#include <vfw.h>
#include "GameResourceManager.h"

#define _LOGOWINDOW_


//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////


GLOBALTON(CServerConnect);
extern HINSTANCE g_hInst;

extern HWND _g_hWnd;
BOOL g_bCanEnterGame = FALSE;

CServerConnect::CServerConnect()
{

}


CServerConnect::~CServerConnect()
{
}


BOOL CServerConnect::Init(void* pInitParam)
{
	m_imageLoginBarTop.LoadSprite("image/2D/login_bar1.TIF");
	m_imageLoginBarBottom.LoadSprite("image/2D/login_bar01.tif");
	m_imageLoginBackGround.LoadSprite("image/2D/Titanlogo.tif");

	WINDOWMGR->AfterInit();
	return TRUE;
}
void CServerConnect::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	
	m_imageLoginBackGround.Release();
}

void CServerConnect::Process()
{
	if(g_bCanEnterGame)
	{
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
	}
}
void CServerConnect::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
}
void CServerConnect::AfterRender()
{
	VECTOR2 v2Pos = { 0, 0 };
	VECTOR2 PosScale = { 1, 1 };

	VECTOR2 v2TopPos = { 0, 0 };
	VECTOR2 v2BottomPos = { 0, 640 	};
	PosScale.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	PosScale.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/MIDRSLTN_H;
	v2BottomPos.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-128;

	m_imageLoginBackGround.RenderSprite(&PosScale,0,0,&v2TopPos,0xffffffff);
	v2TopPos.y = -(50*PosScale.x);
	m_imageLoginBarTop.RenderSprite(&PosScale,0,0,&v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&PosScale,0,0,&v2BottomPos,0xffffffff);
	
	WINDOWMGR->Render();
}

void CServerConnect::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{	
}
