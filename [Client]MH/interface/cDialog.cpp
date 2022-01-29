







// cDialog.cpp: implementation of the cDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cDialog.h"

#include "ptrList.h"
#include "cWindowManager.h"
#include "cbutton.h"
#include "../WindowIDEnum.h"
#include "../Input/Mouse.h"

#include "GameResourceManager.h"
#include "cScriptManager.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cDialog::cDialog()
{
	m_type = WT_DIALOG;
	m_pComponentList=NULL;
	m_pComponentList = new cPtrList;
	m_bMovable = TRUE;
	///m_pGuageBar = NULL;
	m_pCloseBtn = NULL;
	m_absPos.x = 0;
	m_absPos.y = 0;
	m_pCloseBtn = 0;
	m_bAutoClose = FALSE;

	m_bCaptionHit = FALSE;
}

cDialog::~cDialog()
{
	DestroyCtrl();
	SAFE_DELETE(m_pComponentList);
}

void cDialog::DestroyCtrl()
{
	if(!m_pComponentList) return;
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);
		SAFE_DELETE(window);
	}	
	m_pComponentList->RemoveAll();
}

void cDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)

{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
}

DWORD cDialog::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
		we |= ActionEventWindow(mouseInfo);


	we |= ActionEventComponent(mouseInfo);
	return we;
}

DWORD cDialog::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;

	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();

	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftPress = mouseInfo->LButtonPressed();

	BOOL click = mouseInfo->LButtonDown();

	if( m_pCloseBtn )
	{
		we = m_pCloseBtn->ActionEvent(mouseInfo);

		if(we & WE_BTNCLICK)
		{
			cbWindowFunc(m_ID, m_pParent, WE_CLOSEWINDOW);
			cWindowManager::OnWindowEvent( m_ID, this, WE_CLOSEWINDOW );
			return (we |= WE_CLOSEWINDOW);

		}

		if( m_pCloseBtn->IsClickInside() )

			return we;
	}

	if( mouseInfo->LButtonDown() && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{
			if( m_bMovable && PtInCaption( x2, y2 ) )

				m_bCaptionHit = TRUE;
			
			we |= WE_TOPWINDOW;
		}
	}

	if( mouseInfo->LButtonUp() )
		m_bCaptionHit = FALSE;

	if( m_bCaptionHit )
	if( mouseInfo->LButtonDrag() )
	if(WINDOWMGR->m_OldDragFlag == FALSE)
	{					
		SetDepend(FALSE);
		
		float X = (float)x2;
		float Y = (float)y2;
		LONG OldX = (LONG)(X>m_absPos.x?X-m_absPos.x:m_absPos.x-X);
		LONG OldY = (LONG)(Y>m_absPos.y?Y-m_absPos.y:m_absPos.y-Y);
		WINDOWMGR->SetDragStart((LONG)m_absPos.x, (LONG)m_absPos.y, OldX, OldY, m_ID);
		m_bCaptionHit = FALSE;
	}

	return we;
}

DWORD cDialog::ActionEventComponent(CMouse * mouseInfo)
{

	DWORD we = WE_NULL;
	if(!m_bActive) return we;
	if(!m_pComponentList) return we;
	if((int)m_pComponentList<0x100)
	{
		g_CErrorMsg.PrintErrorFile("cDialog AEC m_pComponentList:%d",m_pComponentList);
	}
	PTRLISTPOS pos = m_pComponentList->GetTailPosition();
	cWindow * window;

	while(pos)
	{
		window = (cWindow *)m_pComponentList->GetPrev(pos);

		if( window->IsActive() )
			we |= window->ActionEvent(mouseInfo);
	}
	return we;
}


DWORD cDialog::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	DWORD we = WE_NULL;

	if( !m_bActive ) return we;

	we |= cWindow::ActionKeyboardEvent(keyInfo);
	we |= ActionKeyboardEventComponent(keyInfo);

	return we;
}


DWORD cDialog::ActionKeyboardEventComponent(CKeyboard * keyInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;
	PTRLISTPOS pos = m_pComponentList->GetTailPosition();
	cWindow * window;

	while(pos)
	{
		window = (cWindow *)m_pComponentList->GetPrev(pos);
		if( window->IsActive() )
			we = window->ActionKeyboardEvent(keyInfo);

	}


	return we;
}


void cDialog::Add(cWindow * window)
{
	if(!m_pCloseBtn && window->GetID()== CMI_CLOSEBTN)
	{
		m_pCloseBtn = (cButton *)window;
	}

	window->SetAbsXY((LONG)(m_absPos.x+window->GetRelX()), (LONG)(m_absPos.y+window->GetRelY()));

	window->SetParent((cObject *)this);
	m_pComponentList->AddTail((void *)window);
}


void cDialog::SetActive( BOOL val )
{
	if( val == TRUE && m_bSetTopOnActive )
		WINDOWMGR->SetWindowTop( this );

	if( m_bDisable ) return;

	cWindow::SetActive(val);
}

void cDialog::SetActiveRecursive(BOOL val)
{
	if( m_bDisable ) return;
	
	cWindow::SetActive(val);

	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);

		window->SetActive(val);

	}
	
	if( val == TRUE )
		WINDOWMGR->SetWindowTop( this );
}

void cDialog::SetAlpha(BYTE al)

{ 
	cWindow::SetAlpha(al);
	

	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);
		window->SetAlpha(al);
	}
}

void cDialog::SetOptionAlpha( DWORD dwAlpha )
{ 
	cWindow::SetOptionAlpha( dwAlpha );
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);
		window->SetOptionAlpha( dwAlpha );
	}
}


void cDialog::Render()
{
	if(!m_bActive) return;	
	cDialog::RenderWindow();
	cDialog::RenderComponent();
}
void cDialog::RenderWindow()
{
	if(!m_bActive) return;	
	cWindow::Render();
}
void cDialog::RenderComponent()
{
	if (!m_bActive || m_pComponentList==NULL) return;
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pComponentList->GetNext(pos);
		if(window->IsActive())
		{
			window->Render();
		}
	}
}

BOOL cDialog::PtInCaption(LONG x, LONG y)
{
	if(m_absPos.x + m_captionRect.left > x || m_absPos.y + m_captionRect.top > y || m_absPos.x + m_captionRect.right < x || m_absPos.y + m_captionRect.bottom < y)
		return FALSE;
	else
		return TRUE;
}

cWindow * cDialog::GetWindowForID(LONG id)
{
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList->GetNext(pos);
		if(win->GetID() == id)
			return win;
	}
	return NULL;
}

void cDialog::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);

	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList->GetNext(pos);
		win->SetAbsXY(x+(LONG)win->GetRelX(), y+(LONG)win->GetRelY());

	}
}

void cDialog::SetDisable( BOOL val )
{
	m_bDisable = val;
	PTRLISTPOS pos = m_pComponentList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pComponentList->GetNext(pos);
		win->SetDisable( val );
	}
}




