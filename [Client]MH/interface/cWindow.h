



//////////////////////////////////////////////////////////////////////
// class name : cWindow 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////

#ifndef _cWINDOW_H_
#define _cWINDOW_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cObject.h"
#include "cImage.h"
#include "cMultiLineText.h"
#include "cWindowDef.h"


class CMouse;
class CKeyboard;


void defaultWindowFunc(LONG lId, void * p, DWORD we);

class cWindow : public cObject
{
public:

	cWindow();
	virtual ~cWindow();

	
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	virtual void Render();
	virtual void ToolTipRender();
	virtual DWORD ActionEvent( CMouse * mouseInfo );			
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo ) { return WE_NULL; }	

	virtual void Add(cWindow * wnd){}


	BOOL PtInWindow(LONG x, LONG y);
	virtual void SetAbsXY(LONG x, LONG y){ m_absPos.x=(float)x; m_absPos.y=(float)y; }
	void SetRelXY(LONG x, LONG y){ m_relPos.x=(float)x; m_relPos.y=(float)y; }
	void SetValidXY(LONG x, LONG y) { m_validPos.x = (float)x; m_validPos.y=(float)y; }
	
	virtual void SetAutoClose(BOOL val){}
	virtual BOOL IsAutoClose(){ return FALSE; }


	void SetWH(LONG x, LONG y){ m_width=(WORD)x; m_height=(WORD)y; }
	void SetFontIdx(WORD wIdx){ m_wFontIdx=wIdx; }
	void SetFocus(BOOL val){ m_bFocus=val; }
	void SetDepend(BOOL val){ m_bDepend=val; }
	void SetMovable(BOOL val){ m_bMovable=val; }

	void SetBasicImage(cImage * image){ if(image) { m_BasicImage = *image; m_pCurImage = &m_BasicImage; } }
	void SetToolTip(char * msg, DWORD color=RGB_HALF(255,255,0), cImage * image=NULL, DWORD imgColor = TTCLR_DEFAULT );
	void AddToolTipLine( char* msg, DWORD color= TTTC_DEFAULT );
	cImage * GetBasicImage() { return &m_BasicImage; }
	void AddImageTop(cImage * image);
	void AddImagTopLine(cImage* image,WORD Index);
	WORD GetTipLineCount();
	void IniTipLineCount();
	
	BOOL IsMovable(){ return m_bMovable; }
	BOOL IsDepend(){ return m_bDepend; }
	BOOL IsFocus(){ return m_bFocus; }
	WORD GetWidth(){ return m_width; } 
	WORD GetHeight(){ return m_height; }
	virtual void SetHeight(WORD hei) { m_height = hei; }
	float GetAbsX(){ return m_absPos.x;	}
	float GetAbsY(){ return m_absPos.y;	}
	void SetAbsX(LONG x){ m_absPos.x=(float)x;	}
	void SetAbsY(LONG y){ m_absPos.y=(float)y;	}
	float GetRelX(){ return m_relPos.x;	}
	float GetRelY(){ return m_relPos.y;	}
	virtual void SetAlpha(BYTE al){ m_alpha = al; }
	virtual void SetOptionAlpha(DWORD dwAlpha) { m_dwOptionAlpha = dwAlpha; }
	WORD GetFontIdx() { return m_wFontIdx; }

	void SetToolTipImageRGB( DWORD color ) { toolTip.SetImageRGB( color ); }
	void SetImageRGB(DWORD val) { m_dwImageRGB = val; }
	void SetcbFunc(cbFUNC Func=defaultWindowFunc){ if(Func) cbWindowFunc=Func; }
	void ExcuteCBFunc( DWORD we );

	BOOL IsAlwaysTop() { return m_bAlwaysTop; }
	void SetAlwaysTop( BOOL bAlwaysTop ) { m_bAlwaysTop = bAlwaysTop; }
	void SetTopOnActive( BOOL bTop ) { m_bSetTopOnActive = bTop; }
	void SetDestroy() { m_bDestroy = TRUE; }
	void SetImageSrcRect(RECT * pRect){ CopyRect(&m_imgSrcRect, pRect); }
	void SetScale(cImageScale * scale)	{
											m_scale.x = scale->x;
											m_scale.y = scale->y;
										}

	virtual void SetFocusEdit(BOOL val) {}

   void  AddToolTipImageEquip(cImage * image);  
   
   cMultiLineText* GetToolTip(){return &toolTip;}
   void SetEquipItem();
   //[װ���Ա�][BY:ʮ���½���][QQ:112582793][2019-4-12][15:40:17]
   void SetRelationToolTip(cMultiLineText* toolTip){m_pRelationTip = toolTip;}
   void SetRelationToolTip1(cMultiLineText* toolTip){m_pRelationTip1 = toolTip;}
protected:
	void SetCurImage(cImage * image){ m_pCurImage = image; }			
	cbFUNC cbWindowFunc;
	WORD m_wFontIdx;
	BOOL m_bFocus;
	BOOL m_bMovable;
	BOOL m_bDepend;				
	BOOL m_bDestroy;
	BOOL m_bAlwaysTop;
	BOOL m_bSetTopOnActive;
	cImage * m_pToolTipImage;	
	DWORD m_toolTipFgColor;
	DWORD m_dwViewCurTime;
	DWORD m_dwViewLastTime;
	LONG m_ttX, m_ttY;
	cMultiLineText toolTip;
	cImage m_BasicImage;
	cImage * m_pCurImage;
	DWORD m_dwImageRGB;
	DWORD m_alpha;
	DWORD m_dwOptionAlpha; 
public:
	VECTOR2 m_absPos;
	VECTOR2 m_relPos;
	RECT	m_imgSrcRect;
private:
	VECTOR2 m_validPos;
protected:
	WORD m_width;
	WORD m_height;
	cImageScale		m_scale;
	//[װ���Ա�][BY:ʮ���½���][QQ:112582793][2019-4-12][15:40:31]
	cMultiLineText* m_pRelationTip;
	cMultiLineText* m_pRelationTip1;
};

#endif // _cWINDOW_H_


