//////////////////////////////////////////////////////////////////////
// class name : cStatic 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cSTATIC_H_

#define _cSTATIC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"

class cStatic : public cWindow  
{
public:
	cStatic();

	virtual ~cStatic();
	virtual void Render();
	void SetFontIdx(WORD fontIdx);
	void SetStaticText(char * text);
	LONG GetStaticValue()				{	return (LONG)atoi(m_szStaticText);	}
	char * GetStaticText()				{	return m_szStaticText;				}
	void SetStaticValue(LONG text)		{	itoa(text, m_szStaticText, 10);		}
	void SetMultiLine(BOOL val=TRUE)	{	m_fMultiLine = val;					}
	void InitMultiLine();
	void SetTextXY(LONG x, LONG y)		{	m_textXY.x = x; m_textXY.y = y;		}
	void SetFGColor(DWORD color)		{	m_fgColor = color;					}
	void SetShadowTextXY(LONG x, LONG y)	{	m_shadowTextXY.x = x; m_shadowTextXY.y = y;		}
	void SetShadowColor(DWORD color)		{	m_shadowColor = color;							}
	void SetShadow(BOOL val)				{	m_bShadow = val;								}
	void SetAlign( int nAlign )				{ m_nAlign = nAlign; }
	void SetRender(BOOL val){m_bIsRender = val;}
	BOOL IsRender(){return m_bIsRender;}
protected:
	cCoord			m_textXY;
	cMultiLineText	m_StaticText;
	char			m_szStaticText[MAX_TEXT_SIZE];
	BOOL			m_fMultiLine;
	DWORD			m_fgColor;
	BOOL			m_bShadow;
	cCoord			m_shadowTextXY;
	DWORD			m_shadowColor;
	int				m_nAlign;
	BOOL			m_bIsRender;
};

#endif // _cSTATIC_H_


