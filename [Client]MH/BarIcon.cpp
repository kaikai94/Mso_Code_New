







// BarIcon.cpp: implementation of the CBarIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarIcon.h"

#include "GameIn.h"



#include "MainBarDialog.h"
#include "./Input/Mouse.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"


#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarIcon::CBarIcon()
{
	m_bCaptionHit = FALSE;
}

CBarIcon::~CBarIcon()
{

}

void CBarIcon::InitInfo(BARICON_INFO * info)
{
	memcpy(&m_Info, info, sizeof(BARICON_INFO));
	m_bAlram = FALSE;

	SCRIPTMGR->GetImage( 92, &m_ImgAlram[0] );
	SCRIPTMGR->GetImage( 93, &m_ImgAlram[1] );
	SCRIPTMGR->GetImage( 94, &m_ImgAlram[2] );
	SCRIPTMGR->GetImage( 95, &m_ImgAlram[3] );
	SCRIPTMGR->GetImage( 96, &m_ImgAlram[4] );
	SCRIPTMGR->GetImage( 97, &m_ImgAlram[5] );

	SCRIPTMGR->GetImage( 108, &m_ImgSpecialAlram[0] );
	SCRIPTMGR->GetImage( 109, &m_ImgSpecialAlram[1] );
	SCRIPTMGR->GetImage( 110, &m_ImgSpecialAlram[2] );
	
	
}

DWORD CBarIcon::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	we = cWindow::ActionEvent(mouseInfo);


	long x = mouseInfo->GetMouseX();
	long y = mouseInfo->GetMouseY();
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();


	if( ( mouseInfo->LButtonDown() || mouseInfo->LButtonDoubleClick() ) && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{

			if( m_bMovable ) m_bCaptionHit = TRUE;
			we |= WE_TOPWINDOW;
		}
	}

	if( mouseInfo->LButtonUp() )
	{
		if( m_bCaptionHit )
		if( GAMEIN->GetMainInterfaceDialog()->m_OldDragFlag == FALSE )
		if( PtInWindow( x2, y2 ) )
		if( !WINDOWMGR->IsMouseInputProcessed() )
		{
			m_fPushed ^= TRUE;
			GAMEIN->GetMainInterfaceDialog()->PushMenu( GetInfo()->kind );

		}

		m_bCaptionHit = FALSE;
	}

	if( !m_fPushed )
	{
		if( we & WE_MOUSEOVER )
		{
			m_pCurImage = &m_OverImage;
			m_fgCurColor = m_fgOverColor;		
		}
		else
		{
			m_pCurImage = &m_BasicImage;
			m_fgCurColor = m_fgBasicColor;
		}
	}
	else
	{
		m_pCurImage = &m_PressImage;
		m_fgCurColor = m_fgPressColor;		
	}
	return we;
}

void CBarIcon::Render()
{
	static DWORD dwTime = gCurTime;
	static DWORD dwSpecialTime = gCurTime;
	static int n = 0;
	static int nSpecial = 0;
	cPushupButton::Render();

	if( m_bAlram == FALSE ) return;
	VECTOR2 vAlramPos;
	vAlramPos.x = m_absPos.x + 2;
	vAlramPos.y = m_absPos.y + 5;
	m_ImgAlram[n].RenderSprite( &m_scale, NULL, 0, &vAlramPos, 0xffffffff );

	if( gCurTime - dwTime > 150 )
	{
		n = (n+1)%6;
		dwTime = gCurTime;
	}

	if( !HERO ) return;
	
	/*if( HERO->GetLevel() <= 10 )
	{
		VECTOR2 vPos;
		vPos.x = m_absPos.x + 5;
		vPos.y = m_absPos.y - 30;

		m_ImgSpecialAlram[nSpecial].RenderSprite( &m_scale, NULL, 0, &vPos, 0xffffffff );
		if( gCurTime - dwSpecialTime > 100 )
		{
			nSpecial = (nSpecial+1)%3;
			dwSpecialTime = gCurTime;
		}
	}*/
}




