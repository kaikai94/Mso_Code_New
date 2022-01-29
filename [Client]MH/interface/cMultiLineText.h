




// cMultiLineText.h: interface for the cMultiLineText class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _cMULTILINETEXT_H_
#define _cMULTILINETEXT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cImage.h"


#define TOOLTIP_MOUSESIZE_X	34
#define TOOLTIP_MOUSESIZE_Y	28


class LINE_NODE
{
public:
	LINE_NODE()
	{
		memset(line, 0, 256);
		nextLine	= NULL;

		len			= 0;
		color		= 0xffffffff;
	}
	~LINE_NODE(){
		memset(line,0,256);
		nextLine = NULL;
		len			= 0;
		color		= 0xffffffff;
	}

	DWORD	color;
	char	line[256];
	DWORD	len;
	LINE_NODE * nextLine;
};

class cMultiLineText  
{

public:
	cMultiLineText();
	virtual ~cMultiLineText();
	void Init(WORD fontIdx, DWORD fgColor, cImage * bgImage=NULL, DWORD imgColor = 0xffffffff);

	void Release();
	void Render();
	
	BOOL IsValid() { return m_fValid; }
	
	void SetText( char* text );
	virtual BOOL SetText(VECTOR3* pPos3, char* szText);

	void AddLine( char* text, DWORD dwColor = 0xffffffff );	
	void AddNamePannel( DWORD dwLength );

	void SetFontIdx(WORD fontIdx){ m_wFontIdx = fontIdx; }
	void SetXY(LONG x, LONG y){	m_m_leftTopPos.left = x; m_m_leftTopPos.top = y; }
	void operator=(char* text){ SetText(text); }

	void SetImageRGB( DWORD color )				{ m_imgColor = color; }
	void SetImageAlpha( DWORD dwAlpha )			{ m_alpha = dwAlpha; }
	void SetOptionAlpha( DWORD dwOptionAlpha )	{ m_dwOptionAlpha = dwOptionAlpha; }

    void AddImageEquip(cImage * Image);
	void AddImageTop(cImage * Image);
	
	void AddImageTopLine(cImage * Image,WORD Index);
	WORD GetLineCount(){return m_LineCount;}
	void ClearLineCount();
	void GetLineHight(int line_idx,WORD & AddNum,WORD& MulNum);
	void SetShowCurItem(BOOL val){m_IsShowCur = val;}
	WORD GetHight(){return (m_line_idx+3)*LINE_HEIGHT;}
	void SetItemDBIdx(DWORD val){m_ItemDBIdx = val;}
	DWORD GetItemDBIdx(){return m_ItemDBIdx;}
	void SetEquipItem(BOOL val){bIsEquipItem = val;}
	static void Init();
private:
	LINE_NODE* topLine;
	BOOL bIsEquipItem;
	BOOL	m_fValid;
	WORD	m_wFontIdx;
	cImage  m_bgImage;
	//[物品信息边框][BY:十里坡剑神][QQ:112582793][2019-4-9][20:17:57]
	//[这几个Image是共用资源所以声明成静态成员，全局只需一个对象][BY:十里坡剑神][QQ:112582793][2019-4-10][0:41:15]
	static cImage  m_LUpRightImage;
	static cImage  m_RUpRightImage;
	static cImage  m_bLineImage;
	static cImage	m_LiftQuarter;
	static cImage	m_RightQuarter;

	DWORD	m_fgColor;
	DWORD	m_imgColor;

	RECT	m_m_leftTopPos;
	int		m_line_idx;
	int		m_max_line_width;

	BOOL	m_IsShowCur;
	DWORD	m_alpha;
	DWORD	m_dwOptionAlpha;
	WORD	m_LineCount;

	cImage  m_ImageEquip;      
	cImage  m_ImageTop;      
	cImage  m_ImageTopLine[50];
	BOOL	m_bNamePannel;
	DWORD   m_ItemDBIdx;
};

#endif // _cMULTILINETEXT_H_




