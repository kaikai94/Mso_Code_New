




// ShoutDialog.h: interface for the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)
#define AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cDialog.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconGridDialog.h"  

#define CHARFACELIST        56      
#define FACELISTSTARTIDX	148      

class cEditBox;
class cListDialog;
class cIcon;      
class cButton;


class CShoutDialog : public cDialog
{
	DWORD			m_dwItemIdx;
	DWORD			m_dwItemPos;

	cEditBox*		m_pMsgBox;

	cButton*		m_ShoutOk;

	cButton*		m_SuperShoutOk;

	cButton*        m_FaceBtn;
	cImage          m_ImageFace[CHARFACELIST]; 

	cIcon           m_IconFace[CHARFACELIST];  

	cIconGridDialog * m_ImageGrid;  

	BOOL bIsSuperMsg;
public:
	CShoutDialog();
	virtual ~CShoutDialog();



	void Linking();

	BOOL SendShoutMsgSyn();

	void SendItemInfoSyn(char * buff);

	void SetItemInfo( DWORD ItemIdx, DWORD ItemPos )			{	m_dwItemIdx = ItemIdx;	m_dwItemPos = ItemPos;	}

	void LoadFaceList(); 

	void OnActionEvent(LONG lId, void * p, DWORD we); 
	void Show(BOOL val = FALSE);

};



#endif // !defined(AFX_SHOUTDIALOG_H__2851FDE7_5AC8_4835_8F46_900FC49814D7__INCLUDED_)



