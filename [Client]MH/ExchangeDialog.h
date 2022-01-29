




// ExchangeDialog.h: interface for the CExchangeDialog class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_EXCHANGEDIALOG_H__93FF55CE_8E05_4501_A0E2_85D133275E30__INCLUDED_)
#define AFX_EXCHANGEDIALOG_H__93FF55CE_8E05_4501_A0E2_85D133275E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"
#include "./Interface/cImage.h"

class cIconGridDialog;
class cEditBox;
class cPushupButton;
class cButton;
class cStatic;

class CExchangeDialog  : public cDialog
{
protected:

//for linking
	BOOL				m_bInit;
	cIconGridDialog*	m_pMyIconGridDlg;
	cIconGridDialog*	m_pOpIconGridDlg;
	cEditBox*			m_pMyMoneyEditBox;
	cEditBox*			m_pOpMoneyEditBox;
	cEditBox*			m_pMyGoldEditBox;  // 2014-10-29 ±‡º≠øÚ
	cEditBox*			m_pOpGoldEditBox;  // 2014-10-29 ±‡º≠øÚ
	cPushupButton*		m_pLockBtn;
	cPushupButton*		m_pExchangeBtn;
	cButton*			m_pInputMoneyBtn;
	cButton*			m_pInputGoldBtn;   // 2014-10-29  ‰»Î∞¥≈•

	cStatic*			m_pMyNameStc;
	cStatic*			m_pOpNameStc;


	cImage				m_LockImage;
	
	BOOL				m_bMyLock;

	BOOL				m_bOpLock;

	
	MONEYTYPE				m_dwMoney;
	MONEYTYPE				m_dwOpMoney;

	MONEYTYPE			    m_dwGold;      //  2014-10-29
	MONEYTYPE				m_dwOpGold;    //  2014-10-29

	
	BOOL				m_bExchangable;

	int					m_nMyItemNum;		//±≥»Ø«“ ≥ª æ∆¿Ã≈€ ºˆ
	int					m_nOpItemNum;		//±≥»Ø«“ ªÛ¥Î æ∆¿Ã≈€ ºˆ

public:
	CExchangeDialog();
	virtual ~CExchangeDialog();
	
	void Linking();
	virtual void SetActive( BOOL val );
//	virtual DWORD ActionEvent( CMouse* mouseInfo, CKeyboard* keyInfo );
	virtual void Render();
	void OnActionEvent(LONG lId, void * p, DWORD we);

	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon* icon );
	void FakeDeleteItem( POSTYPE pos );

	void AddItem( int nType, POSTYPE pos, cIcon* pItem );
	void DelItem( int nType, POSTYPE pos );
	void DelItemOptionInfo( POSTYPE pos );
	void DelItemOptionInfoAll();
	void DelItemRareOptionInfo( POSTYPE pos );
	void DelItemRareOptionInfoAll();
	void DelPetItemInfo( POSTYPE pos );
	void DelPetItemInfoAll();

	MONEYTYPE InputMoney(int nType, MONEYTYPE dwMoney);

	MONEYTYPE InputGold(int nType, MONEYTYPE dwGold);  //  ‰»Î 2014-10-29



	void OpLock( BOOL bLock );	//ªÛ¥Î ∂Ù∞…±‚
	void MyLock( BOOL bLock );

	MONEYTYPE GetMoney() { return m_dwMoney; }
	BOOL IsMyLock()	{ return m_bMyLock; }

	// 2014-10-29 

	MONEYTYPE GetGold(){ return m_dwGold; }
	
	void SetPlayerName( char* pMyName, char* pOpName );



	static void OnInputMoney( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnInputCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	static void OnInputGold( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnInputGoldCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

};

#endif // !defined(AFX_EXCHANGEDIALOG_H__93FF55CE_8E05_4501_A0E2_85D133275E30__INCLUDED_)


