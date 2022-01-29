#ifndef _CMSGBOX_H
#define _CMSGBOX_H


#include "./interface/cDialog.h"

class cStatic;
class cTextArea;

#define MAX_MB_MSG	128

enum eMBType
{
	MBT_NOBTN = 0,
	MBT_OK,
	MBT_YESNO,
	MBT_CANCEL,
	MBT_COUNT,
};



enum eMB_BTN_ID
{
	MB_BTN_OK = 0,
	MB_BTN_YES,
	MB_BTN_NO,
	MB_BTN_CANCEL,
	MB_BTN_COUNT,

};


class cMsgBox : public cDialog
{

protected:
	static cImage	m_DlgImg;
	static cImage	m_BtnImgBasic;
	static cImage	m_BtnImgOver;
	static cImage	m_BtnImgPress;
	static char		m_BtnText[MB_BTN_COUNT][32];

	static DWORD	m_dwBtnTextColor[3];
	static BOOL		m_bInit;
protected:
	cTextArea*		m_pMsg;
	DWORD			m_dwMsgColor;
	cButton*		m_pButton[2];

	void (*m_cbMBFunc)(LONG lId, void * p, DWORD we);
	
	int				m_nDefaultBtn;
	int				m_nMBType;

	DWORD			m_dwParam;
public:
	cMsgBox();
	~cMsgBox();


	virtual DWORD ActionEvent( CMouse * mouseInfo );
	virtual DWORD ActionKeyboardEvent( CKeyboard * keyInfo );
	void SetMBFunc( cbFUNC cbMBFunc ) { m_cbMBFunc = cbMBFunc; }

	static void InitMsgBox();
	static BOOL IsInit() { return m_bInit; }
	void MsgBox(LONG lId, int nMBType, char* strMsg, cbFUNC cbMBFunc = NULL, int DefaultBtn=0, bool bChangeColor=false);
	
	void SetDefaultBtn( int nDefaultBtn ) { m_nDefaultBtn = nDefaultBtn; }

	BOOL PressDefaultBtn();
	BOOL ForcePressButton( LONG lBtnID );
	void ForceClose();

	DWORD GetParam() { return m_dwParam; }
	void SetParam( DWORD dwParam )  { m_dwParam = dwParam; }
	void SetMsgBoxText(char* pStrMsg );
};



#endif








