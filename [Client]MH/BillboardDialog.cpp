#include "stdafx.h"
#include "BillboardDialog.h"
#include "cDialogueList.h"
#include "WindowIDEnum.h"
#include "HelpDicManager.h"
#include "ChatManager.h"
#include "cRITEMEx.h"
#include "./Interface/cPushupButton.h"
#include "BillBoardManager.h"
#include "ChatManager.h"
cBillBoardDialog::cBillBoardDialog()
{
}

cBillBoardDialog::~cBillBoardDialog()
{
	
}

void cBillBoardDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
void cBillBoardDialog::Linking()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 0 );
	m_memdlg[0] = (cListCtrl*)pDlg->GetWindowForID(BILL_MEMBERLIST);
	cDialog* pDlg1 = (cDialog*)GetTabSheet( 1 );
	m_memdlg[1] = (cListCtrl*)pDlg1->GetWindowForID(BILL_MEMBERLIST2);
	cDialog* pDlg2 = (cDialog*)GetTabSheet( 2 );
	m_memdlg[2] = (cListCtrl*)pDlg2->GetWindowForID(BILL_MEMBERLIST3);
	cDialog* pDlg3 = (cDialog*)GetTabSheet( 3 );
	m_memdlg[3] = (cListCtrl*)pDlg3->GetWindowForID(BILL_MEMBERLIST4);
	cDialog* pDlg4 = (cDialog*)GetTabSheet( 4 );
	m_memdlg[4] = (cListCtrl*)pDlg4->GetWindowForID(BILL_MEMBERLIST5);
	m_sTime			= (cStatic*)this->GetWindowForID(BILL_TIME);
}


void cBillBoardDialog::SetTableState(WORD count)
{
	cPushupButton* BtnBtn;
	for(int i = 0;i<5;i++)
	{
		BtnBtn = GetTabBtn(i);
		BtnBtn->SetActive(FALSE);
	}
	if(count<=20) return;
	if(count>80)
	{
		for(int i = 0;i<5;i++)
		{
			BtnBtn = GetTabBtn(i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if(count >60)
	{
		for(int i = 0;i<4;i++)
		{
			BtnBtn = GetTabBtn(i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if(count >40)
	{
		for(int i = 0;i<3;i++)
		{
			BtnBtn = GetTabBtn(i);
			BtnBtn->SetActive(TRUE);
		}
	}
	else if(count >20)
	{
		for(int i = 0;i<2;i++)
		{
			BtnBtn = GetTabBtn(i);
			BtnBtn->SetActive(TRUE);
		}
	}
}

void cBillBoardDialog::EndDialog()
{
	for (int i  = 0;i<5;i++)
	{
		m_memdlg[i]->DeleteAllItems();
	}
}


void cBillBoardDialog::OpenBillBoard()
{
	EndDialog();	
	ShowHeroTopList();
	SetActive(TRUE);
}
void cBillBoardDialog::SetActive( BOOL val )
{
	if( m_bActive == val ) return;
	if(!val)EndDialog();	
	cDialog::SetActiveRecursive( val );
}

void cBillBoardDialog::ShowHeroTopList()
{
	char text[128];
	sprintf(text,CHATMGR->GetChatMsg(2229),
		sBOARDMGR->GetUpdateTime().wYear,
		sBOARDMGR->GetUpdateTime().wMonth,
		sBOARDMGR->GetUpdateTime().wDay,
		sBOARDMGR->GetUpdateTime().wHour,
		sBOARDMGR->GetUpdateTime().wMinute);
	m_sTime->SetStaticText(text);
	TOPLISTINFOHERO tmp[MAX_TOP_LIST_COUNT];
	memset(tmp, 0, sizeof(TOPLISTINFOHERO) * MAX_TOP_LIST_COUNT);
	memcpy(tmp, sBOARDMGR->GetHeroList(), sizeof(TOPLISTINFOHERO)*MAX_TOP_LIST_COUNT);
	stTIME sTime;
	int j = 1;
	int k = 0;
	int i = 0;
	WORD TopCount = sBOARDMGR->GetTopListCount();
	SetTableState(TopCount);
	for (; k < TopCount; k++)
	{
		if (tmp[k].dwCharacterIdx == 0)
			break;
		cRITEMEx *ritem = new cRITEMEx(6);
		ritem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
		ritem->rgb[1] = RGBA_MAKE(255, 255, 255, 255);
		ritem->rgb[2] = RGBA_MAKE(255, 255, 255, 255);
		ritem->rgb[3] = RGBA_MAKE(255, 255, 255, 255);
		ritem->rgb[4] = RGBA_MAKE(255, 255, 255, 255);
		ritem->rgb[5] = RGBA_MAKE(255, 255, 255, 255);

		sprintf(text, "%d", j);
		SafeStrCpy(ritem->pString[0], text, 4);
		SafeStrCpy(ritem->pString[1], tmp[k].Name, 17);
		sprintf(text, "%d", tmp[k].Grade);
		SafeStrCpy(ritem->pString[2], text, 4);
		sprintf(text, "%d", tmp[k].Reset);
		SafeStrCpy(ritem->pString[3], text, 4);
		SafeStrCpy(ritem->pString[4], tmp[k].Guldname, 17);
		sTime.SetTime(tmp[k].Fame);
		sprintf(text, "%dÈÕ %02d:%02d:%02d", sTime.GetDay(), sTime.GetHour(), sTime.GetMinute(), sTime.GetSecond());
		SafeStrCpy(ritem->pString[5], text, 17);
		ritem->dwID = j-1;
		m_memdlg[i]->InsertItem(j - 1, ritem);
		j++;
		if (j % 20 == 1)
			i++;
	}
}

