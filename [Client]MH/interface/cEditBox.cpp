







// cEditBox.cpp: implementation of the cEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cEditBox.h"
#include <string>   
#include "ctype.h" 
#include "cScriptManager.h" 
#include "cWindowManager.h"
#include "cFont.h"
#include "../input/cIMEWnd.h"
#include "../cIMEex.h"
#include "../Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "cChangeCharCode.h"
//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cEditBox::cEditBox()
{
	m_nonactiveTextColor=0xffffff;
	m_activeTextColor=0xffffff;
	m_bInitEdit=FALSE;
//	m_bCaret=TRUE;

	m_bSecret=FALSE;
	m_bTextChanged=0;
	m_dwCaretCurTick=m_dwCaretLastTick=0;
	m_type = WT_EDITBOX;
	m_bReadOnly = FALSE;
	m_bFaceImage= FALSE;
	
	m_pIMEex	= new cIMEex;
	m_pIMEex->SetParentEdit( this );

	m_bCaret	= FALSE;
	m_pEditText	= NULL;

	m_bShowCaretInReadOnly = FALSE;

	m_nAlign			= TXT_LEFT;
	m_lTextLeftOffset	= 5;
	m_lTextRightOffset	= 5;
	m_lTextTopOffset	= 6;
	
	m_cbEditFunc = NULL;
}

cEditBox::~cEditBox()
{
	CIMEWND->SetDockingEx(m_pIMEex, FALSE);
	WINDOWMGR->SetFocusedEdit( this, FALSE );

	if(m_pIMEex)
	{
		m_pIMEex->Release();
		delete m_pIMEex;
		m_pIMEex = NULL;
	}

	if( m_pEditText )
	{
		delete[] m_pEditText;
		m_pEditText = NULL;
	}
}

void cEditBox::SetTextOffset( LONG Left, LONG Right, LONG Top )
{
	m_lTextLeftOffset	= Left;
	m_lTextRightOffset	= Right;
	m_lTextTopOffset	= Top;
}

void cEditBox::Init(LONG x, LONG y, WORD wid, WORD hei, cImage* basicImage, cImage* focusImage, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);

	if( focusImage )

	{
		if( focusImage->IsNull() )
		{
			if( basicImage )
				m_FocusImage = *basicImage;
		}
		else
		{
			m_FocusImage = *focusImage;
		}
	}
	else
	{
		if( basicImage )

			m_FocusImage = *basicImage;
	}

	SetFocusEdit(FALSE);
}

void cEditBox::InitEditbox(WORD editBoxPixel, WORD BufBytes)
{
	m_bInitEdit = TRUE;
	m_pIMEex->Init( BufBytes, editBoxPixel, TRUE, 1 );
	m_pEditText = new char[BufBytes + 1];
	ZeroMemory( m_pEditText, BufBytes+1 );
}

DWORD cEditBox::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cWindow::ActionEvent(mouseInfo);

	if( m_bDisable ) return we;

	if( mouseInfo->LButtonDown() )
	{
		if( PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY()) && ( we & WE_LBTNCLICK ) )
		{
			SetFocusEdit(TRUE);
			if(cbWindowFunc)
				(*cbWindowFunc)(m_ID, m_pParent, WE_SETFOCUSON);

			we |= WE_SETFOCUSON;
		}
		else
		{
			SetFocusEdit(FALSE);
		}

	}


	if(m_bTextChanged)
	{
		if(cbWindowFunc)
			(*cbWindowFunc)(m_ID, m_pParent, WE_CHANGETEXT);
		we |= WE_CHANGETEXT;

	}

	return we;
}


DWORD cEditBox::ActionKeyboardEvent(CKeyboard * keyInfo)
{
	
/*	//RETURN키 처리는 cimewnd에서 한다.

	DWORD we = WE_NULL;

	if( !m_bActive || m_bDestroy ) return we;

	if( m_bFocus  )
	{
		if( keyInfo->GetKeyDown(KEY_RETURN) || keyInfo->GetKeyDown(KEY_PADENTER) )
		{
			if(cbWindowFunc)
				(*cbWindowFunc)(m_ID, m_pParent, WE_RETURN);

			we |= WE_RETURN;
		}
	}

	return we;
*/
	return WE_NULL;
}


void cEditBox::SetActive(BOOL val)
{

	if( m_bDisable ) return;

	cWindow::SetActive( val );
	
	if( !val )
		SetFocusEdit( FALSE );
}


void cEditBox::SetFocus( BOOL val )
{
	cWindow::SetFocus( val );
}

void cEditBox::SetFocusEdit(BOOL val)
{
	if( m_bFocus == val ) return;

	if( val && !m_bActive ) return;

	SetFocus(val);

	if(val)
	{
		if( !m_bReadOnly )

			CIMEWND->SetDockingEx(m_pIMEex, TRUE);
		else if( !m_bShowCaretInReadOnly )
			return;

		m_bCaret = TRUE;
		m_dwCaretLastTick = m_dwCaretCurTick = GetTickCount();
		WINDOWMGR->SetFocusedEdit( this, TRUE );

		m_pCurImage = &m_FocusImage;

		IMEEvent( eIMEE_SETFOCUS );
	}
	else
	{
		m_bCaret = FALSE;
		CIMEWND->SetDockingEx(m_pIMEex, FALSE);
		WINDOWMGR->SetFocusedEdit( this, FALSE );
		m_pCurImage = &m_BasicImage;

		IMEEvent( eIMEE_KILLFOCUS );
	}
}
void cEditBox::MakeRenderText(std::string& RenderMsg)
{
	if(RenderMsg.length()>0)
	{
		int FindStartPos=0;

		while(FindStartPos>=0 && FindStartPos< RenderMsg.length())
		{
			int FindPos= RenderMsg.find('{',FindStartPos);

			if(FindPos>=0)
			{
				RenderMsg.replace(FindPos,3,"   ");
			}
			else
			{
				break;
			}

		}
	}
}

void cEditBox::Render()
{
	cWindow::Render();
	char strBuf[256];
	int nStrLen;
	RECT rcText;

	LONG TxtPosX, TxtPosY;

	TxtPosY = (LONG)m_absPos.y + m_lTextTopOffset;

	if(m_bFocus && ( !m_bReadOnly || m_bShowCaretInReadOnly) )
	{
		for( int i = 0 ; i < m_pIMEex->GetTotalLine() ; ++i )
		{
			m_pIMEex->GetLineText( i, strBuf, &nStrLen );
			
			if( m_bSecret )
			{
				memset( strBuf, '*', nStrLen );
			}
			
			if( m_nAlign & TXT_LEFT )
			{
				TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
			}
			else if( m_nAlign & TXT_RIGHT )
			{
				TxtPosX = (LONG)m_absPos.x + m_width 
						  - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) - m_lTextRightOffset;
			}
			else
			{
				TxtPosX = (LONG)m_absPos.x + ( m_width 
						  - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) ) / 2;
			}

			SetRect( &rcText, TxtPosX,
				TxtPosY + LINE_HEIGHT * i, 1, 1 );
			std::string MsgBuff = strBuf;
			MakeRenderText(MsgBuff);
#ifdef _BIG5SYSTEM_
			if (MsgBuff.length()>=2)
				CODEMGR->ChangeCharCode((char*)MsgBuff.c_str());
#endif
			CFONT_OBJ->RenderFont( m_wFontIdx, (char*)MsgBuff.c_str(), MsgBuff.length(), &rcText, 
				RGBA_MERGE( m_activeTextColor, m_alpha * m_dwOptionAlpha / 100 ) );

			if(m_bFaceImage)
			{
			   RenderFaceImage(TxtPosX,TxtPosY+ LINE_HEIGHT * i,strBuf);
			}

			if( m_bCaret )
			{
				if( i == m_pIMEex->GetInsertPos()->nLine )
				{
					LONG lExtent = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex );
					
					SetRect( &rcText, TxtPosX + lExtent - 2,
						TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT, 1, 1 );
					
					CFONT_OBJ->RenderFont( m_wFontIdx, "|", 1, &rcText, 
						RGBA_MERGE( 0x00ffffff, m_alpha * m_dwOptionAlpha / 100 ) );

					if(m_bFaceImage)
					{
				    	RenderFaceImage(TxtPosX,TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT,strBuf); 
					}
					
				}
			}			
		}

		
		m_dwCaretCurTick = GetTickCount();
		
		if( m_dwCaretCurTick - m_dwCaretLastTick >= 500 )
		{
			m_bCaret = !m_bCaret;
			while( m_dwCaretCurTick - m_dwCaretLastTick >= 500 )
				m_dwCaretLastTick += 500;
		}

	}
	else
	{
		for( int i = 0 ; i < m_pIMEex->GetTotalLine() ; ++i )
		{

			m_pIMEex->GetLineText( i, strBuf, &nStrLen );

			if( m_bSecret )
			{
				memset( strBuf, '*', nStrLen );
			}

			if( m_nAlign & TXT_LEFT )
			{
				TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
			}
			else if( m_nAlign & TXT_RIGHT )
			{
				TxtPosX = (LONG)m_absPos.x + m_width 
						  - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) - m_lTextLeftOffset;
			}
			else	//TXT_CENTER
			{
				TxtPosX = (LONG)m_absPos.x + ( m_width 
						  - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) ) / 2;
			}

			SetRect( &rcText, TxtPosX,
				TxtPosY + LINE_HEIGHT * i, 1, 1 );
			std::string MsgBuff = strBuf;
			MakeRenderText(MsgBuff);
#ifdef _BIG5SYSTEM_
			if (MsgBuff.length()>=2)
				CODEMGR->ChangeCharCode((char*)MsgBuff.c_str());
#endif
			CFONT_OBJ->RenderFont( m_wFontIdx, (char*)MsgBuff.c_str(),MsgBuff.length(), &rcText, 
				RGBA_MERGE( m_nonactiveTextColor, m_alpha * m_dwOptionAlpha / 100 ) );

			if(m_bFaceImage)
			{
			  RenderFaceImage(TxtPosX,TxtPosY+ LINE_HEIGHT * i,strBuf); 
			}
		}

	}
}

void cEditBox::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);
}

char* cEditBox::GetEditText()
{ 
	if(m_pIMEex)
	{
		if(m_pIMEex->GetTotalLine() > 1)
			m_pIMEex->GetScriptText( m_pEditText );
		else
			m_pIMEex->GetLineText( 0, m_pEditText );
	}
	else
		m_pEditText[0] = 0;
#ifdef _BIG5SYSTEM_
	if (strlen(m_pEditText)>=2)
	{
		CODEMGR->ChangeCharCode(m_pEditText);
	}
#endif
	return m_pEditText;
}

void cEditBox::SetEditText(char * text)
{ 
	if(m_bInitEdit)
	{
		m_pIMEex->SetScriptText( text );
	}
}


void cEditBox::SetValidCheck( int nSetNum )
{
	if( m_pIMEex )
		m_pIMEex->SetValidCheckMethod( nSetNum );
}

int cEditBox::GetValidCheckMethod()
{
	if( m_pIMEex )
		return m_pIMEex->GetValidCheckMethod();

	return 0;
}


void cEditBox::SetAlign( int nAlign )

{
	m_nAlign = nAlign;
}


void cEditBox::SetDisable( BOOL val )
{
	m_bDisable = val;
	if( val )
		SetFocusEdit( FALSE );
}

void cEditBox::IMEEvent( DWORD dwEvent )
{
	if( m_cbEditFunc )
		m_cbEditFunc( m_ID, this, dwEvent );
}

void cEditBox::GetCaretPos( LONG* X, LONG* Y )
{
	LONG TxtPosY = (LONG)m_absPos.y + m_lTextTopOffset;
	LONG TxtPosX = 0;

	char strBuf[256] = {0,};
	int nStrLen = 0;

	if( m_pIMEex->GetLineText( m_pIMEex->GetInsertPos()->nLine, strBuf, &nStrLen ) == FALSE )
	{
		*X = 0;
		*Y = 0;
		return;
	}

	if( m_nAlign & TXT_LEFT )
	{
		TxtPosX = (LONG)m_absPos.x + m_lTextLeftOffset;
	}
	else if( m_nAlign & TXT_RIGHT )
	{
		TxtPosX = (LONG)m_absPos.x + m_width 
			- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) - m_lTextRightOffset;
		

	}
	else	//TXT_CENTER

	{
		TxtPosX = (LONG)m_absPos.x + ( m_width 
			- CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, nStrLen ) ) / 2;
	}


	LONG lExtent = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, strBuf, m_pIMEex->GetInsertPos()->nIndex );

	*X = TxtPosX + lExtent - 2;
	*Y = TxtPosY + m_pIMEex->GetInsertPos()->nLine * LINE_HEIGHT;			


}

void cEditBox::RenderFaceImage(LONG X,LONG Y,char * StaticText)
{
	std::string Buffer=StaticText;

	if(Buffer.length()>0)
	{
		int FindStartPos=0;

		while(FindStartPos>=0 && FindStartPos< Buffer.length())
		{
			int FindPos= Buffer.find('{',FindStartPos);

			if(FindPos>=0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos+1,2);

				std::basic_string <char> FlagStr= Buffer.substr(0,FindPos);

				int ImageIdx= atoi(ImgStr.c_str());

				if(ImageIdx>=0 && ImageIdx<56)
				{
					cImage m_Image;

					VECTOR2 p_Scal,p_Pos;

					p_Scal.x=p_Scal.y=1;

					p_Pos.y = Y-2;

					p_Pos.x = X+1.2f+(FlagStr.length())*6;
			
					SCRIPTMGR->GetImage(ImageIdx+148 , &m_Image, PFT_HARDPATH);

					m_Image.RenderSprite(&p_Scal,0,0,&p_Pos,0xffffffff);
				}

                FindStartPos = FindPos+2;
			}
			else
			{
				break;
			}
			
		}
	}
	
}
void cEditBox::SetRenderFaceImg(BOOL val)
{
	m_bFaceImage=val; 
}




