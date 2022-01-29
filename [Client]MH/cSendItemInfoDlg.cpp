#include "stdafx.h"
#include "cSendItemInfoDlg.h"
#include "cWindowManager.h"
#include "./input/UserInput.h"
#include "./input/Mouse.h"
#include "./WindowIDEnum.h"
#include "./interface/cButton.h"
#include "ItemManager.h"
cSendItemInfoDlg::cSendItemInfoDlg(void)
{
	m_pSendItemInfoTip = NULL;
	//m_CloseBtn = NULL;
	m_dwItemDBidx=-1;
}

cSendItemInfoDlg::~cSendItemInfoDlg(void)
{
	ClearUseItem();
}
void cSendItemInfoDlg::Linking()
{
	//m_CloseBtn = (cButton *)GetWindowForID(CMI_CLOSEBTN);
}
void cSendItemInfoDlg::Show()
{
	if(!m_pSendItemInfoTip)return;
	CMouse * pMouseInfo = g_UserInput.GetMouse();
	m_pSendItemInfoTip->SetXY(pMouseInfo->GetMouseX(),pMouseInfo->GetMouseY()-m_pSendItemInfoTip->GetHight());
	//m_CloseBtn->SetAbsXY(pMouseInfo->GetMouseX()+275,pMouseInfo->GetMouseY()-(m_pSendItemInfoTip->GetHight()+53));
	WINDOWMGR->SetSendItemInfoTip(m_pSendItemInfoTip);
	SetActive(TRUE);
}
void cSendItemInfoDlg::SetActive( BOOL val )
{
	if(!val)
		WINDOWMGR->SetSendItemInfoTip(NULL);
	cDialog::SetActive(val);
}
void cSendItemInfoDlg::SetItem(DWORD ItemDBidx,BOOL bNeedDelete)
{
	m_dwItemDBidx =ItemDBidx; 
	if(bNeedDelete)
		m_NeedDeleteItem.push_back(ItemDBidx);
}

void cSendItemInfoDlg::SetItemTip(cMultiLineText *Tip)
{
	if(m_pSendItemInfoTip)
		m_pSendItemInfoTip->SetItemDBIdx(0);
	m_pSendItemInfoTip = Tip;
}
BOOL cSendItemInfoDlg::IsSameItem(DWORD dwItemDBIdx)
{
	if(m_dwItemDBidx!=-1)
		return (m_dwItemDBidx==dwItemDBIdx);
	else
		return FALSE;
}
void cSendItemInfoDlg::ClearUseItem()
{
	if(IsActive())
		SetActive(FALSE);
	if(m_NeedDeleteItem.size()==0) return;
	std::vector<DWORD>::iterator iter;
	iter = m_NeedDeleteItem.begin();
	for(;iter!=m_NeedDeleteItem.end();iter++)
	{
		CItem * pItem = ITEMMGR->GetItem(*iter);
		if(pItem)
			ITEMMGR->ItemDelete(pItem);
		//iter = m_NeedDeleteItem.erase(iter);
		//if()
		//	break;
	}
	if (m_NeedDeleteItem.size() > 0)
	{
		m_NeedDeleteItem.clear();
		std::vector<DWORD>().swap(m_NeedDeleteItem);
	}
		
}