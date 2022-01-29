// ServerListDialog.h: interface for the CServerListDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_)
#define AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include ".\interface\cWindowHeader.h"
#include "cRITEMEx.h"

class cLsitCtrl;

class CServerListDialog : public cTabDialog  
{
	cButton*	m_pConnectBtn;
	cButton*	m_pExitBtn;

public:
	static cListCtrl*	m_pCTCListCtrl;
	static cListCtrl*	m_pCNCListCtrl;
	static cListCtrl*	m_pOTHListCtrl;
	static int			m_nIndex;
	static int			m_nType;
	CServerListDialog();
	virtual ~CServerListDialog();
	virtual void Add(cWindow * window);
	void	Linking();
	void	LoadServerList();
	void	ActionEvent( LONG lId, void* p, DWORD we );
	int		GetType(){return m_nType;}
	int		GetSelectedIndex()		{ return m_nIndex; }
	int		GetSelectIndex();
	cButton*    GetOkButton(){return m_pConnectBtn;}
};

#endif // !defined(AFX_SERVERLISTDIALOG_H__2827868C_735C_4097_85A8_F9A0693D724B__INCLUDED_)
