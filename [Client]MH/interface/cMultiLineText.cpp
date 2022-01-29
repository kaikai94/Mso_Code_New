







// cMultiLineText.cpp: implementation of the cMultiLineText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cMultiLineText.h"
#include "GameResourceManager.h"
#include "cFont.h"
#include "cScriptManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cImage  cMultiLineText::m_LUpRightImage;
cImage  cMultiLineText::m_RUpRightImage;
cImage  cMultiLineText::m_bLineImage;
cImage	cMultiLineText::m_LiftQuarter;
cImage	cMultiLineText::m_RightQuarter;
cMultiLineText::cMultiLineText()
{

	m_fgColor  =0x00ffffff;
	m_imgColor = 0x00ffffff;
	m_line_idx = 0;
	topLine = NULL;
	m_wFontIdx = 0;

	m_max_line_width = 0;
	m_fValid = FALSE;
	SetRect(&m_m_leftTopPos,0,0,0,0);
	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_bNamePannel = FALSE;
	m_LineCount =0;
	m_IsShowCur = FALSE;
	m_ItemDBIdx = 0;

}

cMultiLineText::~cMultiLineText()
{
	Release();
}

void cMultiLineText::Init(WORD fontIdx, DWORD fgColor, cImage * bgImage, DWORD imgColor )
{
	Release();
	m_wFontIdx = fontIdx;
	m_fgColor = fgColor;
	if( bgImage )
	m_bgImage = *bgImage;
	m_imgColor = imgColor;
	bIsEquipItem = FALSE;
}
void cMultiLineText::Init()
{
	//[物品信息边框][BY:十里坡剑神][QQ:112582793][2019-4-9][20:17:44]
	SCRIPTMGR->GetImage(239,&m_LUpRightImage,PFT_HARDPATH);
	SCRIPTMGR->GetImage(240,&m_bLineImage,PFT_HARDPATH);
	SCRIPTMGR->GetImage(241,&m_LiftQuarter,PFT_HARDPATH);
	SCRIPTMGR->GetImage(242,&m_RightQuarter,PFT_HARDPATH);
	SCRIPTMGR->GetImage(243,&m_RUpRightImage,PFT_HARDPATH);
}
void cMultiLineText::Release()
{
	LINE_NODE * curLineNode = topLine;
	LINE_NODE * tmpNode = NULL;

	while(curLineNode)
	{
		tmpNode = curLineNode;
		curLineNode = curLineNode->nextLine;
		SAFE_DELETE(tmpNode);		
	}
	curLineNode = NULL;
	topLine = NULL;
	m_fValid = FALSE;
	m_line_idx = 0;
	m_max_line_width = 0;
	m_bNamePannel = FALSE;
}
void cMultiLineText::GetLineHight(int line_idx,WORD & AddNum,WORD& MulNum)
{
	if (line_idx<=5)
	{
		AddNum = 0;
		MulNum = 7;
	}
	else if(line_idx<=10)
	{
		AddNum = 1;
		MulNum = 10;
	}
	else if(line_idx<=15)
	{
		AddNum = 3;
		MulNum = 10;
	}
	else if(line_idx<=18)
	{
		AddNum = 4;
		MulNum = 10;
	}
	else if(line_idx<=25)
	{
		AddNum = 4;
		MulNum = 11;
	}
	else if(line_idx <=35)
	{
		AddNum = 5;
		MulNum = 11;
	}
	else if(line_idx<=40)
	{
		AddNum = 6;
		MulNum = 11;
	}
	else if(line_idx<50)
	{
		AddNum = 11;
		MulNum = 12;
	}
}
void cMultiLineText::Render()
{
	LINE_NODE * curLineNode = topLine;
	RECT rect = m_m_leftTopPos;
	WORD LineCount = 0;
	VECTOR2 LinescaleRate, LinePos;
	LinescaleRate.x = 2;
	LinescaleRate.y = 1;
	if (!bIsEquipItem)
	{
		if ((DWORD)(rect.left + m_max_line_width) > GET_MAINWIN_W ||
			(DWORD)(rect.top + (m_line_idx + 1)*LINE_HEIGHT) > GET_MAINWIN_H)
		{
			if (rect.left - (TOOLTIP_MOUSESIZE_X + 4 + m_max_line_width) >= 0)
				rect.left -= TOOLTIP_MOUSESIZE_X + 4 + m_max_line_width;
			else
				rect.left -= TOOLTIP_MOUSESIZE_X;

			if (rect.top - (TOOLTIP_MOUSESIZE_Y + (m_line_idx + 1)*LINE_HEIGHT) >= 0)
				rect.top -= TOOLTIP_MOUSESIZE_Y + 4 + (m_line_idx + 1)*LINE_HEIGHT;
			else
				rect.top -= TOOLTIP_MOUSESIZE_Y - 4;
		}
	}

	LinePos.x = (float)rect.left + 20;

	if (!m_ImageTop.IsNull())
	{
		VECTOR2 scaleRate1, Pos1;
		scaleRate1.x = 1;

		scaleRate1.y = 1;

		Pos1.x = (float)rect.left - 8;
		Pos1.y = (float)rect.top - 36;
		m_ImageTop.RenderSprite(&scaleRate1, NULL, 0, &Pos1, 0xffffffff);
	}
	if (!m_bgImage.IsNull())
	{
		VECTOR2 scaleRate, Pos;

		if (m_bNamePannel)
		{
			scaleRate.x = (float)((m_max_line_width + 5) >> 2);
			if (!m_ImageTop.IsNull())
				scaleRate.x = 64;
			scaleRate.y = (float)(((m_line_idx + 1)*LINE_HEIGHT + 5) >> 2);

			Pos.x = (float)rect.left - 2;
			Pos.y = (float)rect.top - 3;

		}
		else if (!m_ImageTop.IsNull())
		{
			scaleRate.x = (float)((m_max_line_width + 10) >> 2);
			if (!m_ImageTop.IsNull())
				scaleRate.x = 64.5;
			WORD Addnum, MulNum;
			Addnum = 1;
			MulNum = 10;
			GetLineHight(m_line_idx, Addnum, MulNum);
			scaleRate.y = (float)(((m_line_idx + Addnum)*MulNum) >> 2);
			Pos.x = (float)rect.left - 6;
			Pos.y = (float)rect.top - 5;


		}
		else
		{
			scaleRate.x = (float)((m_max_line_width + 10) >> 2);
			if (!m_ImageTop.IsNull())
				scaleRate.x = 64;
			scaleRate.y = (float)(((m_line_idx + 1)*LINE_HEIGHT + 10) >> 2);
			Pos.x = (float)rect.left - 5;
			Pos.y = (float)rect.top - 5;
		}


		if (!m_ImageEquip.IsNull())
		{
			scaleRate.x = scaleRate.x + 10.0f;
			scaleRate.y = scaleRate.y + 20.0f;
		}

		m_bgImage.RenderSprite(&scaleRate, NULL, 0, &Pos, RGBA_MERGE(m_imgColor, m_alpha * m_dwOptionAlpha / 100));

		if (!m_ImageTop.IsNull())
		{
			VECTOR2 tScale, tpos;
			tpos.x = Pos.x + 4;
			tpos.y = Pos.y + scaleRate.y * 4;

			tScale.x = scaleRate.x - 2;
			tScale.y = 1;
			//[底边横线渲染][BY:十里坡剑神][QQ:112582793][2019-4-9][19:09:49]
			m_bLineImage.RenderSprite(&tScale, NULL, 0, &tpos, RGBA_MERGE(0xffffffff, 255));


			tpos.x = Pos.x - 8;
			tpos.y = Pos.y;

			tScale.x = 1;
			tScale.y = scaleRate.y;		//[左边竖线渲染][BY:十里坡剑神][QQ:112582793][2019-4-9][19:09:01]
			m_LUpRightImage.RenderSprite(&tScale, NULL, 0, &Pos, RGBA_MERGE(0xffffffff, 255));

			tpos.x = Pos.x + (scaleRate.x - 1) * 4;
			tpos.y = Pos.y;				//[右边竖线渲染][BY:十里坡剑神][QQ:112582793][2019-4-9][19:09:11]
			m_RUpRightImage.RenderSprite(&tScale, NULL, 0, &tpos, RGBA_MERGE(0xffffffff, 255));

			tpos.x = Pos.x;
			tpos.y = Pos.y + scaleRate.y * 4;
			tScale.x = 1;
			tScale.y = 1;
			//[左下角渲染][BY:十里坡剑神][QQ:112582793][2019-4-9][19:28:04]
			m_LiftQuarter.RenderSprite(&tScale, NULL, 0, &tpos, RGBA_MERGE(0xffffffff, 255));

			tpos.x = Pos.x + (scaleRate.x - 1) * 4;
			tpos.y = Pos.y + scaleRate.y * 4;
			tScale.x = 1;
			tScale.y = 1;
			//[右下角渲染][BY:十里坡剑神][QQ:112582793][2019-4-9][19:28:04]
			m_RightQuarter.RenderSprite(&tScale, NULL, 0, &tpos, RGBA_MERGE(0xffffffff, 255));

		}

		if (!m_ImageEquip.IsNull())
		{
			VECTOR2 scaleRate, Pos;

			scaleRate.x = scaleRate.y = 1;

			Pos.x = (float)rect.left;

			Pos.y = (float)rect.top;

			m_ImageEquip.RenderSprite(&scaleRate, NULL, 0, &Pos, 0xffffffff);

			rect.top = rect.top + 10;
		}



		if (!m_ImageEquip.IsNull())
		{
			rect.left = rect.left + 15;
		}
	}

	if (!m_ImageTop.IsNull())
	{
		while (curLineNode)
		{
			if (LineCount == 0)
			{
				RECT rectTmp;
				rectTmp.bottom = rect.top + 1;
				rectTmp.right = rect.left + 1;
				rectTmp.left = rect.left - 8;
				rectTmp.top = rect.top - 37;
				CFONT_OBJ->RenderFont(8, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
				if (m_IsShowCur)
				{
					rectTmp.left += CFONT_OBJ->GetTextWidth(8)*curLineNode->len + 10;
					CFONT_OBJ->RenderFont(8, "[当前穿戴]", strlen("[当前穿戴]"), &rectTmp, RGB(0, 255, 0));
				}
				rect.top += LINE_HEIGHT;
				curLineNode = curLineNode->nextLine;
				LineCount++;
				continue;
			}
			else if (LineCount == 1)
			{
				RECT rectTmp;
				rectTmp.bottom = rect.top + 1;
				rectTmp.right = rect.left + 1;
				rectTmp.left = rect.left + 60;
				rectTmp.top = rect.top - 22;
				CFONT_OBJ->RenderFont(8, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(RGB_HALF(255, 251, 150), m_alpha * m_dwOptionAlpha / 100));
				rect.top += LINE_HEIGHT;
				curLineNode = curLineNode->nextLine;
				LineCount++;
				continue;
			}
			else if (LineCount == 2)
			{
				RECT rectTmp;
				rectTmp.bottom = rect.top + 1;
				rectTmp.right = rect.left + 1;
				rectTmp.left = rect.left + 210;
				rectTmp.top = rect.top - LINE_HEIGHT * 2 - 6;
				CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
				//rect.top += LINE_HEIGHT;
				curLineNode = curLineNode->nextLine;
				LineCount++;
				//if(!m_ImageTopLine[LineCount].IsNull())
				//{
				//	LinePos.y = rect.top-15;
				//	m_ImageTopLine[LineCount].RenderSprite(&LinescaleRate,NULL,0,&LinePos,0xffffffff);
				//}
				continue;
			}
			else if (LineCount == 3)
			{
				RECT rectTmp;
				rectTmp.bottom = rect.top + 1;
				rectTmp.right = rect.left + 1;
				rectTmp.left = rect.left + 60;
				rectTmp.top = rect.top - 15;
				CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rectTmp, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
				rect.top += LINE_HEIGHT;
				curLineNode = curLineNode->nextLine;
				LineCount++;
				if (!m_ImageTopLine[LineCount].IsNull())
				{
					LinePos.y = rect.top - 15;
					m_ImageTopLine[LineCount].RenderSprite(&LinescaleRate, NULL, 0, &LinePos, 0xffffffff);
				}
				continue;
			}
			else
			{
				rect.right = rect.left + 1;
				rect.bottom = rect.top + 1;
				CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rect, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
				rect.top += LINE_HEIGHT;
				curLineNode = curLineNode->nextLine;
				LineCount++;
				if (!m_ImageTopLine[LineCount].IsNull())
				{
					LinePos.y = rect.top;
					m_ImageTopLine[LineCount].RenderSprite(&LinescaleRate, NULL, 0, &LinePos, 0xffffffff);
				}
			}

		}
	}
	else
	{
		while (curLineNode)
		{
			rect.right = rect.left + 1;
			rect.bottom = rect.top + 1;
			CFONT_OBJ->RenderFont(m_wFontIdx, curLineNode->line, curLineNode->len, &rect, RGBA_MERGE(curLineNode->color, m_alpha * m_dwOptionAlpha / 100));
			rect.top += LINE_HEIGHT;
			curLineNode = curLineNode->nextLine;
		}
	}
}

void cMultiLineText::SetText(char* text )
{
	if( text == NULL ) return;
	if( *text == 0 ) return;

	if( topLine != NULL ) Release();
	
	char * sp	= text;
	m_line_idx	= 0;
	m_max_line_width = 0; 

	LINE_NODE * curLineNode = topLine = new LINE_NODE;
	curLineNode->nextLine = NULL;
	curLineNode->color = m_fgColor;

	char * cur_line = topLine->line;

	while(*sp != 0)
	{
		if( IsDBCSLeadByte(*sp) )
		{

			*cur_line++ = *sp++;
			*cur_line++ = *sp++;

		}
		else
		{
			switch(*sp)
			{
			case TEXT_DELIMITER:
				{
					++sp;
					char tmp = *sp;
					switch(tmp)
					{
					case TEXT_NEWLINECHAR:
						{
							*cur_line = 0;
							curLineNode->len = strlen(curLineNode->line);

							int real_len = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, curLineNode->line, curLineNode->len);
							if(m_max_line_width < real_len)
							{
								m_max_line_width = real_len;
							}
							m_line_idx++;

							curLineNode = curLineNode->nextLine = new LINE_NODE;
							curLineNode->nextLine = NULL;
							cur_line = curLineNode->line;
							curLineNode->color = m_fgColor;
						}
						break;
					case TEXT_TABCHAR:
						{
						}
						break;
					case TEXT_SPACECHAR:
						{	
							*cur_line = ' ';
							++cur_line;
						}
						break;
					}
				}
				break;

			default:
				{
					*cur_line = *sp;
					++cur_line;
				}
				break;
			}
			++sp;
		}
	}
	curLineNode->len = strlen(curLineNode->line);
	curLineNode->nextLine = NULL;
	int real_len = CFONT_OBJ->GetTextExtentEx(m_wFontIdx, curLineNode->line, curLineNode->len);
	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}

	m_fValid = TRUE;
}

void cMultiLineText::AddLine( char* text, DWORD dwColor )
{
	if( text == NULL ) return;

	char* sp	= text;

	LINE_NODE* pLineNode	= new LINE_NODE;
	pLineNode->nextLine		= NULL;

	pLineNode->color		= dwColor;
	strcpy( pLineNode->line, text );
	pLineNode->len			= strlen(pLineNode->line);
	m_fValid = TRUE;

	LINE_NODE* pTail		= topLine;
	if( pTail )
	{
		while( pTail->nextLine )
			pTail = pTail->nextLine;

		pTail->nextLine = pLineNode;
		++m_line_idx;
	}
	else
	{
		topLine = pLineNode;
	}

	int	real_len = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, pLineNode->line, pLineNode->len );

	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}
	m_LineCount++;
}

void cMultiLineText::AddNamePannel( DWORD dwLength )
{
	LINE_NODE* pLineNode	= new LINE_NODE;
	pLineNode->nextLine		= NULL;
	pLineNode->color		= 0xffffffff;
	pLineNode->len			= strlen(pLineNode->line);
	m_fValid = TRUE;

	LINE_NODE* pTail		= topLine;
	if( pTail )

	{
		while( pTail->nextLine )
			pTail = pTail->nextLine;

		pTail->nextLine = pLineNode;

		++m_line_idx;

	}
	else
	{
		topLine = pLineNode;
	}
	
	char buf[MAX_NAME_LENGTH+1];
	strncpy( buf, "AAAAAAAAAAAAAAAA", MAX_NAME_LENGTH );
	int real_len = CFONT_OBJ->GetTextExtentEx( m_wFontIdx, buf, dwLength );

	if(m_max_line_width < real_len)
	{
		m_max_line_width = real_len;
	}

	m_bNamePannel = TRUE;
}

BOOL cMultiLineText::SetText(VECTOR3 * pPos3, char * szText)
{	

	VECTOR3 OutPos3;
	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, pPos3, &OutPos3);
	if(OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1)
		return FALSE;

	SetXY((long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x-30), (long)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y));

	SetText(szText);

	return TRUE;
}


void cMultiLineText::AddImageEquip(cImage * Image)
{
	m_ImageEquip=*Image;
}
void cMultiLineText::AddImageTop(cImage * Image)
{
	m_ImageTop=*Image;
}
void cMultiLineText::AddImageTopLine(cImage * Image,WORD Index)
{
	if(Index>49) return;
	if(m_ImageTopLine[Index].IsNull())
		m_ImageTopLine[Index] = * Image;
}
void cMultiLineText::ClearLineCount()
{
	m_LineCount=0;
	for(int i = 0;i<50;i++)
	{
		if (!m_ImageTopLine[i].IsNull())
		{
			m_ImageTopLine[i].SetNull();
		}
	}
		
}