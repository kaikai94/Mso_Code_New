// ServerListDialog.cpp: implementation of the CServerListDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerListDialog.h"

#include "WindowIDEnum.h"
#include "GameResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "./Interface/cListCtrl.h"
#include "MainTitle.h"
#include "ErrorMsg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cListCtrl*	CServerListDialog::m_pCTCListCtrl;
cListCtrl*	CServerListDialog::m_pCNCListCtrl;
cListCtrl*	CServerListDialog::m_pOTHListCtrl;
int	CServerListDialog::m_nIndex;
int	CServerListDialog::m_nType;
CServerListDialog::CServerListDialog()
{
	m_pCTCListCtrl = NULL;
	m_pCNCListCtrl = NULL;
	m_pOTHListCtrl = NULL;
	m_nIndex = -1;
	m_nType =-1;
}
void CServerListDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
CServerListDialog::~CServerListDialog()
{
	if(m_pCTCListCtrl)
	{
	   m_pCTCListCtrl->DeleteAllItems();
	}
	if(m_pCNCListCtrl)
	{
		m_pCNCListCtrl->DeleteAllItems();
	}	
	if(m_pOTHListCtrl)
	{
		m_pOTHListCtrl->DeleteAllItems();
	}
}

void CServerListDialog::Linking()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 0 );
	m_pCTCListCtrl = (cListCtrl*)pDlg->GetWindowForID(SL_LISTCTRL_CTC);
	cDialog* pDlg1 = (cDialog*)GetTabSheet( 1 );
	m_pCNCListCtrl = (cListCtrl*)pDlg1->GetWindowForID(SL_LISTCTRL_CNC);
	cDialog* pDlg2 = (cDialog*)GetTabSheet( 2 );
	m_pOTHListCtrl = (cListCtrl*)pDlg2->GetWindowForID(SL_LISTCTRL_OTHER);
	m_pConnectBtn = (cButton*)GetWindowForID(SL_BTN_CONNECT);
	m_pExitBtn = (cButton*)GetWindowForID(SL_BTN_EXIT);

	LoadServerList();
}

void CServerListDialog::LoadServerList()
{
	SEVERLIST(* pServerList)[50] = GAMERESRCMNGR->m_ServerList;
	for( int i = 0; i < GAMERESRCMNGR->m_nMaxServerList1; ++i )
	{
		cRITEMEx* ritem = new cRITEMEx(3);
		ritem->dwID = i+1;
		sprintf( ritem->pString[0], "%d", i+1 );
		strcpy( ritem->pString[1], pServerList[0][i].ServerName );
		//sprintf( ritem->pString[2], "%d ms", pServerList[0][i].dwPing );
		strcpy(ritem->pString[2], "正常");
		ritem->rgb[2] = RGBA_MAKE(0,255,0,255);
		if( !pServerList[0][i].bEnter|| pServerList[0][i].dwPing==2018)
		{
			strcpy( ritem->pString[2], "关闭" );
			ritem->rgb[0] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[2] = RGBA_MAKE(255,0,0,255);
		}
		
		m_pCTCListCtrl->InsertItem( i, ritem );
	}
	for( int i = 0; i < GAMERESRCMNGR->m_nMaxServerList2; ++i )
	{
		cRITEMEx* ritem = new cRITEMEx(3);
		ritem->dwID = i+1;
		sprintf( ritem->pString[0], "%d", i+1 );
		strcpy( ritem->pString[1], pServerList[1][i].ServerName );
		//sprintf( ritem->pString[2], "%d ms", pServerList[1][i].dwPing );
		strcpy(ritem->pString[2], "正常");
		ritem->rgb[2] = RGBA_MAKE(0,255,0,255);
		if( !pServerList[1][i].bEnter|| pServerList[1][i].dwPing==2018)
		{
			strcpy( ritem->pString[2], "关闭" );
			ritem->rgb[0] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[2] = RGBA_MAKE(255,0,0,255);
		}

		m_pCNCListCtrl->InsertItem( i, ritem );
	}
	for( int i = 0; i < GAMERESRCMNGR->m_nMaxServerList3; ++i )
	{
		cRITEMEx* ritem = new cRITEMEx(3);
		ritem->dwID = i+1;
		sprintf( ritem->pString[0], "%d", i+1 );
		strcpy( ritem->pString[1], pServerList[2][i].ServerName );
		//sprintf( ritem->pString[2], "%d ms", pServerList[2][i].dwPing );
		strcpy(ritem->pString[2], "正常");
		ritem->rgb[2] = RGBA_MAKE(0,255,0,255);
		if( !pServerList[2][i].bEnter|| pServerList[2][i].dwPing==2018)
		{
			strcpy( ritem->pString[2], "关闭" );
			ritem->rgb[0] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);
			ritem->rgb[2] = RGBA_MAKE(255,0,0,255);
		}
		m_pOTHListCtrl->InsertItem( i, ritem );
	}
}

void CServerListDialog::ActionEvent( LONG lId, void* p, DWORD we )
{
	
	cRITEMEx* pRItem = NULL;

	if( we & WE_ROWCLICK )
	{
		CServerListDialog * pdl=(CServerListDialog *)p;
		SEVERLIST(* pServerList)[50] = GAMERESRCMNGR->m_ServerList;
		if (lId==SL_LISTCTRL_CTC)
		{
			int i = m_pCTCListCtrl->GetSelectedRowIdx();
			if(i<GAMERESRCMNGR->m_nMaxServerList1)
			{
				pRItem = (cRITEMEx*)m_pCTCListCtrl->GetRItem(i);
				if( m_nIndex != i )
				{
					pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
					pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
					if( !pServerList[0][i].bEnter )
					{
						pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
						pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

					}	
					if( m_nIndex > -1 )
					{
						pRItem = (cRITEMEx*)m_pCTCListCtrl->GetRItem(m_nIndex);
						pRItem->rgb[0] = 0xffffffff;
						pRItem->rgb[1] = 0xffffffff;

						if( !pServerList[0][m_nIndex].bEnter )
						{
							pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
							pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

						}
					}					
				}
				m_nIndex = i;
				m_nType =0;
			}
		}
		if (lId==SL_LISTCTRL_CNC)
		{
			int i = m_pCNCListCtrl->GetSelectedRowIdx();
			if(i<GAMERESRCMNGR->m_nMaxServerList2)
			{
				pRItem = (cRITEMEx*)m_pCNCListCtrl->GetRItem(i);
				if( m_nIndex != i )
				{
					pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
					pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
					if( !pServerList[0][i].bEnter )
					{
						pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
						pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

					}	
					if( m_nIndex > -1 )
					{
						pRItem = (cRITEMEx*)m_pCNCListCtrl->GetRItem(m_nIndex);
						pRItem->rgb[0] = 0xffffffff;
						pRItem->rgb[1] = 0xffffffff;

						if( !pServerList[0][m_nIndex].bEnter )
						{
							pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
							pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

						}
					}					
				}
				m_nIndex = i;
				m_nType=1;
			}
		}
		if (lId==SL_LISTCTRL_OTHER)
		{
			int i = m_pOTHListCtrl->GetSelectedRowIdx();
			if(i<GAMERESRCMNGR->m_nMaxServerList3)
			{
				pRItem = (cRITEMEx*)m_pOTHListCtrl->GetRItem(i);
				if( m_nIndex != i )
				{
					pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
					pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
					if( !pServerList[0][i].bEnter )
					{
						pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
						pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

					}	
					if( m_nIndex > -1 )
					{
						pRItem = (cRITEMEx*)m_pOTHListCtrl->GetRItem(m_nIndex);
						pRItem->rgb[0] = 0xffffffff;
						pRItem->rgb[1] = 0xffffffff;

						if( !pServerList[0][m_nIndex].bEnter )
						{
							pRItem->rgb[0] = RGBA_MAKE(255,0,0,255);
							pRItem->rgb[1] = RGBA_MAKE(255,0,0,255);

						}
					}					
				}
				m_nIndex = i;
				m_nType =2;
			}
		}
	}
	else if( we & WE_ROWDBLCLICK )
	{
		if (lId==SL_LISTCTRL_CTC)
		{
			int i = m_pCTCListCtrl->GetSelectedRowIdx();

			if( i < GAMERESRCMNGR->m_nMaxServerList1 )
			{
				TITLE->ConnectToServer(0,i);
				m_nType=0;
			}	
		}
		if (lId==SL_LISTCTRL_CNC)
		{
			int i = m_pCNCListCtrl->GetSelectedRowIdx();

			if( i < GAMERESRCMNGR->m_nMaxServerList2 )
			{
				TITLE->ConnectToServer(1,i);
				m_nType=1;
			}	
		}
		if (lId==SL_LISTCTRL_OTHER)
		{
			int i = m_pOTHListCtrl->GetSelectedRowIdx();

			if( i < GAMERESRCMNGR->m_nMaxServerList3 )
			{
				TITLE->ConnectToServer(2,i);
				m_nType=2;
			}	
		}
	}
}
int	CServerListDialog::GetSelectIndex()
{
	if (m_nType ==0)
		return m_pCTCListCtrl->GetSelectedRowIdx();
	else if(m_nType ==1)
		return m_pCNCListCtrl->GetSelectedRowIdx();
	else if(m_nType ==2)
		return m_pOTHListCtrl->GetSelectedRowIdx();
	else
		return -1;
}