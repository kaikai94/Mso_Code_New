
// cDivideBox.cpp: implementation of the cDivideBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cDivideBox.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "./Interface/cSpin.h"

#include "WindowIDEnum.h"
#include "cIMEEX.h"
#include "ChatManager.h"

#include "./Input/Keyboard.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cDivideBox::cDivideBox()
{
	m_pOKBtn		= NULL;
	m_pCancelBtn	= NULL;
	m_pSpin			= NULL;
	m_cbDivideFunc	= NULL;
	m_cbCancelFunc	= NULL;
	m_vData1		= NULL;
	m_vData2		= NULL;
}

cDivideBox::~cDivideBox()
{

}


void cDivideBox::CreateDivideBox( LONG x, LONG y, LONG ID, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char* strTitle )
{
	m_cbDivideFunc = cbFc1;
	m_cbCancelFunc = cbFc2;
	m_vData1 = vData1;
	m_vData2 = vData2;
	SetAbsXY(x,y);
	SetID(ID);

	m_pOKBtn = (cButton *)GetWindowForID(CMI_MONEYOK);
	m_pCancelBtn = (cButton *)GetWindowForID(CMI_MONEYCANCEL);

	m_pSpin = (cSpin *)GetWindowForID(CMI_MONEYSPIN);
	m_pSpin->SetValidCheck(VCM_NUMBER);
	m_pSpin->SetAlign( TXT_RIGHT );		

	m_pSpin->SetFocusEdit( TRUE );
	m_pSpin->SetUnit( 1 );

	if( strTitle )
	{
		cStatic* pStatic = new cStatic;
		
		pStatic->Init( 13, 2, 0, 0, NULL );
		pStatic->SetShadow( TRUE );
		pStatic->SetFontIdx( 2 );
		pStatic->SetAlign( TXT_LEFT );
		pStatic->SetStaticText( strTitle );
		
		Add( pStatic );
	}
}

void cDivideBox::ChangeKind( int nKind )
{
	if( nKind == 0 )
	{
		m_pOKBtn->SetText( CHATMGR->GetChatMsg(418), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
		m_pCancelBtn->SetText( CHATMGR->GetChatMsg(419), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
	else if( nKind == 1 )
	{
		m_pOKBtn->SetText( CHATMGR->GetChatMsg(658), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
	else if( nKind == 2 )
	{
		m_pCancelBtn->SetText( CHATMGR->GetChatMsg(632), RGB_HALF(225,225,225), 0xffffffff, 0xffffffff );
	}
}

DWORD cDivideBox::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	DWORD weBtn = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;
	
	we |= cWindow::ActionEvent( mouseInfo );
	we |= cDialog::ActionEventWindow( mouseInfo );

	BOOL bClicked = FALSE;

	if( we & WE_CLOSEWINDOW )
	{
		bClicked = TRUE;
	}

	we |= m_pSpin->ActionEvent( mouseInfo);

	weBtn = m_pOKBtn->ActionEvent( mouseInfo);
	if( weBtn  & WE_BTNCLICK )
	{
		if( m_cbDivideFunc )
			m_cbDivideFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
		bClicked = TRUE;
	}
	weBtn = m_pCancelBtn->ActionEvent( mouseInfo );
	if( weBtn  & WE_BTNCLICK )
	{
		if( m_cbCancelFunc )
			m_cbCancelFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
		bClicked = TRUE;
	}

	if( bClicked )
	{
		m_pSpin->SetFocusEdit( FALSE );

		SetActive( FALSE );
		SetDisable( TRUE );
		WINDOWMGR->AddListDestroyWindow( this );

	}
	

	return we;
}



DWORD cDivideBox::ActionKeyboardEvent( CKeyboard* keyInfo )
{
	return WE_NULL;
}

void cDivideBox::ExcuteDBFunc( DWORD we )
{
	if( we == WE_RETURN )
	{

		if( m_cbDivideFunc )
		{
			m_cbDivideFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
			m_pSpin->SetFocusEdit( FALSE );
			SetActive( FALSE );
			SetDisable( TRUE );


			WINDOWMGR->AddListDestroyWindow( this );
		}
	}
	else if( we == 0 )	
	{
		if( m_cbCancelFunc )
		{
			m_cbCancelFunc( m_ID, this, m_pSpin->GetValue(), m_vData1, m_vData2 );
			m_pSpin->SetFocusEdit( FALSE );
			SetActive( FALSE );
			SetDisable( TRUE );

			WINDOWMGR->AddListDestroyWindow( this );
		}
	}
}

void cDivideBox::SetValue( DWORD val )
{
	m_pSpin->SetValue(val);

}

LONG cDivideBox::GetValue()
{	
	if(m_pSpin)

		return m_pSpin->GetValue();
	else 

		return 0;
}

void cDivideBox::SetMaxValue( DWORD val )
{
	m_pSpin->SetMax( val );
}

void cDivideBox::SetMinValue( DWORD val )
{
	m_pSpin->SetMin( val );
}




