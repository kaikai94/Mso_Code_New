// GMNotifyManager.cpp: implementation of the CGMNotifyManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GMNotifyManager.h"
#include "Interface/cFont.h"
#include "MainGame.h"
#include "GameResourceManager.h"
#include "cScriptManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CGMNotifyManager)

CGMNotifyManager::CGMNotifyManager()
{
	Init( MAX_NOTIFYMSG_LINE );
	SetPosition( NOTIFYMSG_DEFAULT_X, NOTIFYMSG_DEFAULT_Y, NOTIFYMSG_DEFAULT_WIDTH );
	SetLineSpace( NOTIFYMSG_DEFAULT_LINESPACE );
	SetFont( NOTIFYMSG_DEFAULT_FONT );

	memset( m_ApplyEventList, 0, sizeof(BOOL)*eEvent_Max );
}

CGMNotifyManager::~CGMNotifyManager()
{
	Release();
}

void CGMNotifyManager::Init( int nMaxLine )
{
	m_nMaxLine			= nMaxLine;

	m_bUseEventNotify	= FALSE;
	ZeroMemory( m_strEventNotifyTitle, 32 );
	ZeroMemory( m_strEventNotifyContext, 128 );

	m_bNotifyChanged	= FALSE;
}

void CGMNotifyManager::Release()
{
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		SAFE_DELETE(pNotify);
	}

	m_ListNotify.RemoveAll();
}

void CGMNotifyManager::PushMsg( char* pMsg, DWORD dwColor )
{
	sGMNOTIFY* pNotify = new sGMNOTIFY;
	
	pNotify->nStrLen = strlen(pMsg);

	if( pNotify->nStrLen > MAX_NOTIFYMSG_LENGTH )
	{
		strncpy( pNotify->strMsg, pMsg, MAX_NOTIFYMSG_LENGTH );
		pNotify->strMsg[MAX_NOTIFYMSG_LENGTH] = NULL;
		pNotify->nStrLen = MAX_NOTIFYMSG_LENGTH;
	}
	else
	{
		strcpy( pNotify->strMsg, pMsg );
	}

	pNotify->dwReceiveTime	= gCurTime;
	pNotify->dwColor		= dwColor;

	m_ListNotify.AddTail( pNotify );

	if( m_ListNotify.GetCount() > m_nMaxLine ) 
		PopMsg();
}

void CGMNotifyManager::PopMsg()
{
	if( m_ListNotify.GetCount() )
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
		SAFE_DELETE( pNotify );
		m_ListNotify.RemoveHead();
	}
}

void CGMNotifyManager::AddMsg( char* pMsg, int nClass )
{
	int nLen = strlen( pMsg );

	char buf[128];
	int nCpyNum = 0;

	while( nLen > MAX_NOTIFYMSG_PER_LINE )
	{
		if( ( pMsg + MAX_NOTIFYMSG_PER_LINE ) != CharNext( CharPrev( pMsg, pMsg + MAX_NOTIFYMSG_PER_LINE ) ) )
			nCpyNum = MAX_NOTIFYMSG_PER_LINE - 1;
		else
			nCpyNum = MAX_NOTIFYMSG_PER_LINE;

		strncpy( buf, pMsg, nCpyNum );
		buf[nCpyNum] = 0;
		PushMsg( buf, dwNotifyColor[nClass] );
		nLen -= nCpyNum;
		pMsg += nCpyNum;
		if( *pMsg == ' ' ) ++pMsg;
	}

	if( nLen > 0 )
		PushMsg( pMsg, dwNotifyColor[nClass] );	
}

void CGMNotifyManager::Render()
{
	if( m_ListNotify.GetCount() == 0 ) return;
	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
	{
		return;
	}

	sGMNOTIFY* pNotify = (sGMNOTIFY*)m_ListNotify.GetHead();
	if( gCurTime - pNotify->dwReceiveTime > NOTIFYMSG_LAST_TIME )
		PopMsg();

	LONG	lPosY = m_rcPos.top;
	LONG	lPosX;
	LONG	lTextWidth;
	RECT	rect;
	PTRLISTPOS pos = m_ListNotify.GetHeadPosition();
	while(pos)
	{
		sGMNOTIFY* pNotify = (sGMNOTIFY *)m_ListNotify.GetNext(pos);
		lTextWidth = CFONT_OBJ->GetTextExtentEx( 10, pNotify->strMsg, pNotify->nStrLen );
		lPosX = ((GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2)-lTextWidth/2);
		SetRect( &rect, lPosX, lPosY, lPosX + lTextWidth, lPosY + 1 );

		
		std::string TextBuff = pNotify->strMsg;
		MakeRenderText(TextBuff);

		CFONT_OBJ->RenderFont( 10, (char*)TextBuff.c_str(), TextBuff.length(), &rect, RGB_HALF( 30, 30, 30 ) );
		OffsetRect( &rect, -1, -1 );
		CFONT_OBJ->RenderFont( 10, (char*)TextBuff.c_str(), TextBuff.length(), &rect, pNotify->dwColor );
		lPosY += m_nLineSpace + 12;
		RenderFaceImage(rect.left,rect.top,pNotify->strMsg);
	}
}
void CGMNotifyManager::MakeRenderText(std::string& RenderMsg)
{
	if (RenderMsg.length() > 0)
	{
		int FindStartPos = 0;

		while (FindStartPos >= 0 && FindStartPos < RenderMsg.length())
		{
			int FindPos = RenderMsg.find('{', FindStartPos);

			if (FindPos >= 0)
			{
				RenderMsg.replace(FindPos, 3, "   ");
			}
			else
			{
				break;
			}

		}
		int FindPos1 = -1;
		int FindPos2 = -1;
		FindPos1 = RenderMsg.find('&', 0);
		if (FindPos1 != -1)
		{
			RenderMsg.replace(FindPos1, RenderMsg.length(), "");
		}
	}
}
void CGMNotifyManager::SetPosition( LONG lx, LONG ly, LONG lWidth )
{
	m_rcPos.left	= lx;
	m_rcPos.right	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-280;
	m_rcPos.top		= GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/10;
	m_rcPos.bottom	= m_rcPos.top + 1;
}

void CGMNotifyManager::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}

void CGMNotifyManager::SetEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;
}
void CGMNotifyManager::RenderFaceImage(LONG X,LONG Y,char * StaticText)
{
	std::string Buffer = StaticText;

	if (Buffer.length() > 0)
	{
		int FindStartPos = 0;

		while (FindStartPos >= 0 && FindStartPos < Buffer.length())
		{
			int FindPos = Buffer.find('{', FindStartPos);

			if (FindPos >= 0)
			{
				std::basic_string <char> ImgStr = Buffer.substr(FindPos + 1, 2);

				std::basic_string <char> FlagStr = Buffer.substr(0, FindPos);
				int lTextWidth = CFONT_OBJ->GetTextExtentEx(9, (char*)FlagStr.c_str(), FlagStr.length());
				int ImageIdx = atoi(ImgStr.c_str());

				if (ImageIdx >= 0 && ImageIdx < 56)
				{
					cImage m_Image;

					VECTOR2 p_Scal, p_Pos;

					p_Scal.x = p_Scal.y = 1;

					p_Pos.y = Y - 2;

					p_Pos.x = X + lTextWidth;

					SCRIPTMGR->GetImage(ImageIdx + 148, &m_Image, PFT_HARDPATH);

					m_Image.RenderSprite(&p_Scal, 0, 0, &p_Pos, 0xffffffff);
				}

				FindStartPos = FindPos + 2;
			}
			else
			{
				break;
			}

		}
		int FindPos1 = Buffer.find('&', FindStartPos);
		if (FindPos1 >= 0)
		{
			std::basic_string <char> ImgStr = Buffer.substr(FindPos1 + 1, 2);

			std::basic_string <char> FlagStr = Buffer.substr(0, FindPos1);
			int lTextWidth = CFONT_OBJ->GetTextExtentEx(9, (char*)FlagStr.c_str(), FlagStr.length());
			cImage m_Image;

			VECTOR2 p_Scal, p_Pos;

			p_Scal.x = p_Scal.y = 1;

			p_Pos.y = Y - 2;

			p_Pos.x = X + lTextWidth + 3;

			SCRIPTMGR->GetImage(238, &m_Image, PFT_HARDPATH);

			m_Image.RenderSprite(&p_Scal, 0, 0, &p_Pos, 0xffffffff);

		}
	}
}


