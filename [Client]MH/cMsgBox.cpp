








#include "stdafx.h"
#include "cMsgBox.h"
#include "./interface/cButton.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cTextArea.h"
#include "./interface/cScriptManager.h"
#include "./Interface/cResourceManager.h"
#include "./input/cImeWnd.h"

#include "./Input/Keyboard.h"
#include "ChatManager.h"
#include "GameResourceManager.h"


//¸Þ¼¼Áö¹Ú½º Å©±â
#define MB_WIDTH	197//248
#define MB_HEIGHT	150

//¹öÆ°À§Ä¡Å©±â
#define MB_BTN1_X	131
#define MB_BTN1_Y	110
#define MB_BTN2_X	69
#define MB_BTN2_Y	110
#define MB_BTN_W	56
#define MB_BTN_H	19
//ÅØ½ºÆ®À§Ä¡

#define MB_TEXT_X	15
#define MB_TEXT_Y	50

//////////////////

cImage	cMsgBox::m_DlgImg;
cImage	cMsgBox::m_BtnImgBasic;
cImage	cMsgBox::m_BtnImgOver;
cImage	cMsgBox::m_BtnImgPress;
char	cMsgBox::m_BtnText[MB_BTN_COUNT][32]		= {0, };
BOOL	cMsgBox::m_bInit = FALSE;
DWORD	cMsgBox::m_dwBtnTextColor[3] = { 0x00ffffff, 0x00ffff00, 0x00ffff00 };

cMsgBox::cMsgBox()
{
	m_pMsg			= NULL;
	m_pButton[0]	= NULL;
	m_pButton[1]	= NULL;

	m_dwMsgColor = 0x00ffffff;

	m_nDefaultBtn	= 0;
	m_dwParam  = 0;
	m_type = WT_MSGBOX;

	m_nMBType = MBT_NOBTN;
}

cMsgBox::~cMsgBox()
{
}



void cMsgBox::InitMsgBox()
{
	char imagePath[256]={0,};
	char imageMsg[32]={0,};

	SCRIPTMGR->GetImage( 30, &m_DlgImg );
	SCRIPTMGR->GetImage( 31, &m_BtnImgBasic );
	SCRIPTMGR->GetImage( 32, &m_BtnImgOver );
	SCRIPTMGR->GetImage( 33, &m_BtnImgPress );


	strcpy( m_BtnText[MB_BTN_OK], RESRCMGR->GetMsg( 221 ) );
	strcpy( m_BtnText[MB_BTN_YES], RESRCMGR->GetMsg( 222 ) );
	strcpy( m_BtnText[MB_BTN_NO], RESRCMGR->GetMsg( 223 ) );

	strcpy( m_BtnText[MB_BTN_CANCEL], RESRCMGR->GetMsg( 224 ) );

	m_bInit = TRUE;
}


void cMsgBox::MsgBox(LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc, int DefaultBtn, bool bChangeColor)
{
	RECT rtCaption = { 0, 0, MB_WIDTH, 20 };


	cDialog::Init( ( GET_MAINWIN_W - MB_WIDTH ) / 2, ( GET_MAINWIN_H - MB_HEIGHT ) / 2,
		MB_WIDTH, MB_HEIGHT, &m_DlgImg, lId );

	SetMovable(TRUE);
	SetCaptionRect(&rtCaption);
	m_cbMBFunc = cbMBFunc;
	
	cStatic* pStatic = new cStatic;
	pStatic->Init( 82, 10, 0, 0, NULL, -1 );
	if (bChangeColor)
	{
		pStatic->SetStaticText(CHATMGR->GetChatMsg(2405));
		pStatic->SetRelXY(60, 8);
	}
	else
		pStatic->SetStaticText( CHATMGR->GetChatMsg(210) );
	pStatic->SetShadow( TRUE );
	pStatic->SetFontIdx( 2 );
	pStatic->SetAlign( TXT_LEFT );
	Add( pStatic );

	m_nMBType = nMBType;
	switch( nMBType )
	{
	case MBT_NOBTN:
		{
		}
		break;

	case MBT_OK:
		{
			LONG btn1x = (MB_WIDTH - MB_BTN_W)/2;


			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_OK );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_OK], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
		}
		break;

	case MBT_YESNO:
		{
			LONG btn1x = (MB_WIDTH - (MB_BTN_W*2) - 6)/2;
			LONG btn2x = btn1x + MB_BTN_W + 6;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN2_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_YES );

			m_pButton[0]->SetText( m_BtnText[MB_BTN_YES], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );

			m_pButton[1] = new cButton;

			m_pButton[1]->Init( btn2x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_NO );

			m_pButton[1]->SetText( m_BtnText[MB_BTN_NO], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );
			m_pButton[1]->SetAlign( TXT_CENTER );
			m_pButton[1]->SetActive( m_bActive );

			m_pButton[1]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[1] );
		}
		break;

	case MBT_CANCEL:
		{
			LONG btn1x = (MB_WIDTH - MB_BTN_W)/2;

			m_pButton[0] = new cButton;
			m_pButton[0]->Init( btn1x, MB_BTN1_Y, MB_BTN_W, MB_BTN_H, &m_BtnImgBasic, &m_BtnImgOver, &m_BtnImgPress, NULL, MBI_CANCEL );


			m_pButton[0]->SetText( m_BtnText[MB_BTN_CANCEL], m_dwBtnTextColor[0], m_dwBtnTextColor[1], m_dwBtnTextColor[2] );

			m_pButton[0]->SetAlign( TXT_CENTER );
			m_pButton[0]->SetActive( m_bActive );
			m_pButton[0]->SetAlpha( (BYTE)m_alpha );
			Add( m_pButton[0] );
		}
		break;


	}

	m_pMsg = new cTextArea;

	RECT rtText = { 0, 0, 196 - MB_TEXT_X * 2, 90 };

	m_pMsg->Init(MB_TEXT_X, MB_TEXT_Y, MB_WIDTH - MB_TEXT_X, 90, NULL );
	m_pMsg->InitTextArea( &rtText, 256 );
	m_pMsg->SetAlpha((BYTE)m_alpha);
	m_pMsg->SetScriptText( strMsg );
	m_pMsg->SetActive(m_bActive);	
	Add( m_pMsg );
	m_type = WT_MSGBOX;
	m_nDefaultBtn = DefaultBtn;
	m_pMsg->SetChangeColor(bChangeColor);
}

void cMsgBox::SetMsgBoxText(char* pStrMsg)
{
	m_pMsg->SetScriptText( pStrMsg );
}
DWORD cMsgBox::ActionEvent( CMouse * mouseInfo )
{

	DWORD we = WE_NULL;
	DWORD weBtn = WE_NULL;
	
	if( !m_bActive ) return we;

	we |= cWindow::ActionEvent( mouseInfo );
	we |= cDialog::ActionEventWindow( mouseInfo);


	for( int i = 0 ; i < 2 ; ++ i )
	{
		if( m_pButton[i] )
		{
			weBtn = m_pButton[i]->ActionEvent( mouseInfo );
			if( weBtn & WE_BTNCLICK )
			{
				ForcePressButton(m_pButton[i]->GetID());
				break;
			}
		}		
	}
	return we;
}


DWORD cMsgBox::ActionKeyboardEvent( CKeyboard * keyInfo )
{
	DWORD we = WE_NULL;
	return we;
}

BOOL cMsgBox::PressDefaultBtn()
{
	if( m_nDefaultBtn == 0 || m_nDefaultBtn == 1 )
	if( m_pButton[m_nDefaultBtn] )
	{
		return ForcePressButton( m_pButton[m_nDefaultBtn]->GetID() );
	}

	return FALSE;
}

BOOL cMsgBox::ForcePressButton( LONG lBtnID )
{
	if( lBtnID <= 0 ) return FALSE;
	if( !m_cbMBFunc ) return FALSE;

	m_cbMBFunc( m_ID, this, lBtnID );

	SetActive( FALSE );
	SetDisable( TRUE );
	WINDOWMGR->AddListDestroyWindow( this );

	return TRUE;

}

void cMsgBox::ForceClose()
{
	switch( m_nMBType )
	{
	case MBT_NOBTN:
		{

			ForcePressButton( 0 );
		}
		break;

	case MBT_OK:
		{
			ForcePressButton( MBI_OK );
		}
		break;

	case MBT_YESNO:
		{
			ForcePressButton( MBI_NO );
		}
		break;

	case MBT_CANCEL:
		{
			ForcePressButton( MBI_CANCEL );
		}
		break;
	}
}
