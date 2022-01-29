








#include "stdafx.h"
#include <string>   
#include "ctype.h" 
#include "cScriptManager.h"   
#include "StreetStallTitleTip.h"
#include "./interface/cFont.h"

#define BALLOON_BORDER		10

CStreetStallTitleTip::CStreetStallTitleTip()
{
	m_fgColor	= RGB_HALF(0,200,0);
	m_TitleLen	= 0;
	m_lTall		= 0;
	m_wFontIdx	= 0;
	m_bActive	= TRUE;
	m_bShow		= FALSE;
}

CStreetStallTitleTip::~CStreetStallTitleTip()

{


}
/*
void CStreetStallTitleTip::SetTitle(char * szTitle)



{
	if( szTitle == NULL )	return;
	if( *szTitle == 0 )		return;

//	m_TitleLen = strlen( szTitle );
//	ASSERTMSG(m_TitleLen < 39, "object name is very long");

//	strncpy( m_sztitle, szTitle, m_TitleLen+1 );
	SafeStrCpy( m_sztitle, szTitle, MAX_STREETSTALL_TITLELEN );
	m_TitleLen = strlen( m_sztitle );

	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_sztitle, m_TitleLen);
	SetPosX(-strPixelLen/2);

	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30);
}
*/
void CStreetStallTitleTip::SetTitle(char * szTitle)
{
	if( szTitle == NULL )	return;
	if( *szTitle == 0 )		return;

	int nLen = strlen( szTitle );
	if( nLen <= 0 || nLen > MAX_STREETSTALL_TITLELEN ) return;

//	char buf[128];
	int nCpyNum = 0;

	m_nLineNum = 0;

	while( nLen > SS_TITLE_LENGTH )
	{
		if( *szTitle == 0 ) break;
		if( ( szTitle + SS_TITLE_LENGTH ) != CharNext( CharPrev( szTitle, szTitle + SS_TITLE_LENGTH ) ) )
			nCpyNum = SS_TITLE_LENGTH - 1;
		else
			nCpyNum = SS_TITLE_LENGTH;


		strncpy( m_szChatMsg[m_nLineNum], szTitle, nCpyNum );
		m_szChatMsg[m_nLineNum][nCpyNum] = 0;

		m_ChatMsgLen[m_nLineNum] = nCpyNum;

		nLen -= nCpyNum;
		szTitle += nCpyNum;

		if( *szTitle == ' ' ) ++szTitle;	//¶óÀÎ Ã¹±ÛÀÚ°¡ ½ºÆäÀÌ½º°¡ ¾Æ´Ïµµ·Ï...
		if( ++m_nLineNum >= SS_TITLE_LINE-1 ) break;
	}

	if( nLen > 0 && m_nLineNum < SS_TITLE_LINE )
	{
		strncpy( m_szChatMsg[m_nLineNum], szTitle, SS_TITLE_LENGTH );
		m_szChatMsg[m_nLineNum][SS_TITLE_LENGTH] = 0;
		if( nLen > SS_TITLE_LENGTH )
			m_ChatMsgLen[m_nLineNum] = nCpyNum;
		else
			m_ChatMsgLen[m_nLineNum] = nLen;
		++m_nLineNum;
	}


	int strPixelLen = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szChatMsg[0], m_ChatMsgLen[0]);
	SetPosX(-strPixelLen/2);

	cBalloonOutline::SetBalloonSize(strPixelLen+8, 30, m_nLineNum);
}
/*

BOOL CStreetStallTitleTip::Render(LONG absX, LONG absY)
{
	if(!m_bActive)	return FALSE;
	if(!m_bShow)	return FALSE;
	
	cBalloonOutline::RenderOutline( float(absX+m_lPosX), float(absY+m_lTall-17), float(absX), 17);


	// ÅØ½ºÆ®
	RECT rect = {(long)(absX + m_lPosX+BALLOON_BORDER), (long)(absY + m_lTall+BALLOON_BORDER-17), 1, 1};
	if( *m_sztitle != 0 )
	{
		CFONT_OBJ->RenderFont(m_wFontIdx, m_sztitle, m_TitleLen, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 255 ));
	}



	return TRUE;
}
*/
void CStreetStallTitleTip::MakeRenderText(std::string& RenderMsg)
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

BOOL CStreetStallTitleTip::Render(LONG absX, LONG absY)
{
	if(!m_bActive) return FALSE;

	if(!m_bShow) return FALSE;

	if( m_nLineNum <= 0 || m_nLineNum > 2 ) return FALSE;

	LONG lLineHeight = 15;
	LONG lMiddleTotalHeight = 2 + lLineHeight * m_nLineNum;

	LONG lPosX = absX + m_lPosX;
	LONG lPosY = absY + m_lTall - lMiddleTotalHeight - 10;

	cBalloonOutline::RenderOutlineSimple( (float)lPosX, (float)lPosY, (float)absX, lMiddleTotalHeight );


	RECT rect;
	for( int i = 0 ; i < m_nLineNum ; ++i )
	{

		if( m_szChatMsg[i][0] != 0 )
		{
			SetRect( &rect, (long)(lPosX + BALLOON_BORDER),
							(long)(lPosY + BALLOON_BORDER + lLineHeight * i ), 1, 1 );
			std::string MsgBuff = m_szChatMsg[i];
			MakeRenderText(MsgBuff);
			CFONT_OBJ->RenderFont(m_wFontIdx, (char*)MsgBuff.c_str(), MsgBuff.length(), &rect, RGBA_MERGE( /*m_fgColor,*/RGB_HALF(70,70,70), 255 ) );

			RenderFaceImage( (long)(lPosX + BALLOON_BORDER),(long)(lPosY + BALLOON_BORDER + lLineHeight * i ),m_szChatMsg[i]);  // °ÚÌ¯±íÇéäÖÈ¾ 2014-01-02
		}	
	}

	return TRUE;
}

void CStreetStallTitleTip::RenderFaceImage(LONG X,LONG Y,char * ChatMsg)
{
	std::string Buffer=ChatMsg;

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
				int lTextWidth = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, (char*)FlagStr.c_str(), FlagStr.length() );
				int ImageIdx= atoi(ImgStr.c_str());

				if(ImageIdx>=0 && ImageIdx<56)
				{
					cImage m_Image;

					VECTOR2 p_Scal,p_Pos;

					p_Scal.x=p_Scal.y=1;

					p_Pos.y = Y-2;

					p_Pos.x = X+lTextWidth;
			
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


