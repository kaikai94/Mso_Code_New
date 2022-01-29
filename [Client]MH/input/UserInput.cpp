// UserInput.cpp: implementation of the CUserInput class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "UserInput.h"
#include "../Gamein.h"
#include "../MHCamera.h"
#include "../Object.h"
#include "../MoveManager.h"
#include "../Monster.h"
#include "../Player.h"
#include "../Npc.h"
#include "../MHNetwork.h"
#include "../ObjectManager.h"

#include "../ItemGround.h"
#include "../Interface\cWindowManager.h"
#include "../Interface\cEditBox.h"
#include "../WindowIDEnum.h"

#include "CameraRotate.inl"
#include "CameraWheelZoom.inl"

#include "MacroManager.h"
#include "MainGame.h"




CUserInput	g_UserInput;

extern HWND g_hWnd;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserInput::CUserInput()
{
	m_MouseEventThrower.SetDefaultReceiver(&m_HeroMove);
	m_MouseEventThrower.SetReceiverToDefault();
	
	m_bFocus = TRUE;
}

CUserInput::~CUserInput()
{


}


void CUserInput::Init()
{
	HRESULT hr;

	hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
								IID_IDirectInput8, (VOID**)&m_pdi, NULL );
	ASSERT(hr==S_OK);

	m_Keyboard.Init( m_pdi );
	m_Mouse.Init();
}


void CUserInput::Release()
{
	SAFE_RELEASE( m_pdi );

	m_Keyboard.Release();

	m_Mouse.Release();
}



//#include "ReDisConnManager.h"
void CUserInput::Process()
{
	m_Keyboard.UpdateKeyboardState();
	StartGetKeyState();

	//if(RECONNMGR->IsInReCnnMode())
	//{
		WINDOWMGR->KeyboardInput( &m_Keyboard );
	//	return;
	//}
	if( m_bFocus )
	{
		while( GetNextKeyState() )
		{
			MACROMGR->KeyboardInput( &m_Keyboard );
			WINDOWMGR->KeyboardInput( &m_Keyboard );
		}

		MACROMGR->KeyboardPressInput( &m_Keyboard );
	}

	do
	{
		m_Mouse.GetNextMouseEvent();	

		if( m_bFocus )
		{
			WINDOWMGR->MouseInput( &m_Mouse );
			if( MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN )
				m_MouseEventThrower.Process( &m_Mouse );
		}

	}
	while( !m_Mouse.IsEmptyMouseEvent() );

	CameraRotate( &m_Keyboard, &m_Mouse );
	CameraWheelZoom( &m_Keyboard, &m_Mouse );
}


void CUserInput::StartGetKeyState()
{
	m_Keyboard.StartGetKeyState();
}


BOOL CUserInput::GetNextKeyState()
{
	return m_Keyboard.GetNextKeyState();
}






