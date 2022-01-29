



// cDivideBoxEx.h: interface for the cDivideBoxEx class.

//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDIVIDEBOXEX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_)
#define AFX_CDIVIDEBOXEX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "./Interface/cDialog.h"

//#include "INTERFACE\cSpin.h"

class cSpin;

class cDivideBoxEx : public cDialog  
{
public:
	cDivideBoxEx();
	virtual ~cDivideBoxEx();

	void CreateDivideBox( LONG x, LONG y, LONG ID, cbDivideExFUNC cbFc1, cbDivideExFUNC cbFc2, void * vData1, void * vData2, char* strTitle );
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	virtual DWORD ActionKeyboardEvent( CKeyboard* keyInfo );
	void SetValue( DWORD val1,DWORD val2 );

//	void SetPos( int nPos ) { m_nPos = nPos; }

	LONG GetValue(int  SpinIndex);


	void SetMaxValue( DWORD val1 ,DWORD val2);
	void SetMinValue( DWORD val1 ,DWORD val2);

	void ExcuteDBFunc( DWORD we );

	void ChangeKind( int nKind );
	
public:
	

	cbDivideExFUNC	m_cbDivideFunc;
	cbDivideExFUNC	m_cbCancelFunc;
	cButton			* m_pOKBtn;
	cButton			* m_pCancelBtn;
	cSpin			* m_pSpin1;	
	cSpin			* m_pSpin2;
	void			* m_vData1;
	void			* m_vData2;
	
//	int				m_nPos;				 // 나눠지는 아이템의 위치를 가진다.
	

};

#endif // !defined(AFX_CDIVIDEBOX_H__E7789D66_4B67_41B1_8CAC_AFB48DC1B0D4__INCLUDED_)


