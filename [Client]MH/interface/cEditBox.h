//////////////////////////////////////////////////////////////////////
// class name : cEditBox 
// Lastest update : 2002.10.29. by taiyo
//////////////////////////////////////////////////////////////////////
#ifndef _cEDITBOX_H_
#define _cEDITBOX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindow.h"


class cIMEex;

class cEditBox : public cWindow  
{

public:
	cEditBox();
	virtual ~cEditBox();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage* basicImage, cImage* focusImage, LONG ID=0);
	virtual void Render();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	virtual DWORD ActionKeyboardEvent(CKeyboard * keyInfo);
	virtual void SetAbsXY(LONG x, LONG y);
	char * GetEditText();
	void SetEditText(char * text);
	inline void SetActiveTextColor(DWORD color){ m_activeTextColor = color; }
	inline void SetNonactiveTextColor(DWORD color){ m_nonactiveTextColor = color; }
	inline void SetCaret(BOOL val){ m_bCaret = val; }
	inline void SetSecret(BOOL val){ m_bSecret = val; }
	virtual void SetFocusEdit(BOOL val);
	void SetReadOnly(BOOL val){ m_bReadOnly = val; }
	void ShowCaretInReadOnly(BOOL val)	{ m_bShowCaretInReadOnly = val; }
	void InitEditbox(WORD editBoxPixel, 
					WORD BufBytes);
	void SetTextOffset( LONG Left, LONG Right, LONG Top );
	void SetAlign( int nAlign );
	void SetValidCheck( int nSetNum );
	int GetValidCheckMethod();
	void SetFocus( BOOL val );
	virtual void SetActive(BOOL val);
	cIMEex* GetIME() { return m_pIMEex;	}
	BOOL IsReadOnly() { return m_bReadOnly; }
	virtual void SetDisable( BOOL val );
	void SetEditFunc( cbEditFUNC cbEditFunc ) { m_cbEditFunc = cbEditFunc; }
	void IMEEvent( DWORD dwEvent );
	void GetCaretPos( LONG* X, LONG* Y );
	void RenderFaceImage(LONG X,LONG Y,char * StaticText); 
	void SetRenderFaceImg(BOOL val); 
	void MakeRenderText(std::string& RenderMsg);

protected:
	cbEditFUNC	m_cbEditFunc;
	LONG		m_bTextChanged;			
	DWORD		m_nonactiveTextColor;
	DWORD		m_activeTextColor;
	BOOL		m_bInitEdit;			
	BOOL		m_bSecret;
	DWORD		m_dwCaretCurTick;
	DWORD		m_dwCaretLastTick;
	int			m_nAlign;
	LONG		m_lTextLeftOffset;
	LONG		m_lTextRightOffset;
	LONG		m_lTextTopOffset;
	cImage		m_FocusImage;
	cIMEex*		m_pIMEex;
	char*		m_pEditText;
	BOOL		m_bCaret;
	BOOL		m_bReadOnly;
	BOOL		m_bShowCaretInReadOnly;
	BOOL        m_bFaceImage;
};

#endif // _cEDITBOX_H_


