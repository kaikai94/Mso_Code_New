#pragma once
#include "./interface/cDialog.h"
#include "Item.h"
class FastConfigPointDlg:public cDialog
{
	WORD m_wItemIdx;
	DWORD m_dwItemDBIdx;
	DWORD m_dwItemPos;
public:
	FastConfigPointDlg(void);
	~FastConfigPointDlg(void);
	void AddPoint();
	DWORD ActionKeyboardEvent( CKeyboard* keyInfo );
	void OnActionEvent(LONG lId, void * p, DWORD we);
	void SetActive( BOOL val );
	void SetItemInfo(CItem * pItem);
	void ClearData();
};
