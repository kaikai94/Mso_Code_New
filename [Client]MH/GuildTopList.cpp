#include "stdafx.h"
#include "GuildTopList.h"
#include "WindowIDEnum.h"
#include "cRITEMEx.h"
#include "./Interface/cPushupButton.h"
#include "BillBoardManager.h"
#include "ChatManager.h"
GuildTopList::GuildTopList(void)
{

}

GuildTopList::~GuildTopList(void)
{

}
void GuildTopList::ClearList()
{
	for (int i  = 0;i<5;i++)
	{
		m_pList[i]->DeleteAllItems();
	}
}
void GuildTopList::OpenGuildTopList()
{
	ClearList();
	ShowList();
	SetActive( TRUE );
}
void GuildTopList::SetActive( BOOL val )
{
	if( m_bActive == val ) return;

	if(!val) 
	{
		for (int i  = 0;i<5;i++)
		{
			m_pList[i]->DeleteAllItems();
		}
	}
	cDialog::SetActiveRecursive( val );
}
void GuildTopList::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
void GuildTopList::Linking()
{
	cDialog* pDlg	= (cDialog*)GetTabSheet(0 );
	m_pList[0]		= (cListCtrl*)pDlg->GetWindowForID(BILL_GUILD_MEMBERLIST);
	cDialog* pDlg1	= (cDialog*)GetTabSheet(1 );
	m_pList[1]		= (cListCtrl*)pDlg1->GetWindowForID(BILL_GUILD_MEMBERLIST2);
	cDialog* pDlg2	= (cDialog*)GetTabSheet( 2 );
	m_pList[2]		= (cListCtrl*)pDlg2->GetWindowForID(BILL_GUILD_MEMBERLIST3);
	cDialog* pDlg3	= (cDialog*)GetTabSheet( 3 );
	m_pList[3]		= (cListCtrl*)pDlg3->GetWindowForID(BILL_GUILD_MEMBERLIST4);
	cDialog* pDlg4	= (cDialog*)GetTabSheet( 4 );
	m_pList[4]		= (cListCtrl*)pDlg4->GetWindowForID(BILL_GUILD_MEMBERLIST5);
	m_sTime			= (cStatic*)this->GetWindowForID(BILL_GUILD_TIME);
}
void GuildTopList::SetTableState(WORD count)
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
void GuildTopList::ShowList()
{
	char text[128];
	sprintf(text,CHATMGR->GetChatMsg(2229),
		sBOARDMGR->GetUpdateTime().wYear,
		sBOARDMGR->GetUpdateTime().wMonth,
		sBOARDMGR->GetUpdateTime().wDay,
		sBOARDMGR->GetUpdateTime().wHour,
		sBOARDMGR->GetUpdateTime().wMinute);
	m_sTime->SetStaticText(text);
	TOPLISTINFOGUILD GuildList[MAX_TOP_LIST_COUNT];
	memset(GuildList, 0, sizeof(TOPLISTINFOGUILD) * MAX_TOP_LIST_COUNT);
	memcpy(GuildList, sBOARDMGR->GetGuildList(), sizeof(TOPLISTINFOGUILD)*MAX_TOP_LIST_COUNT);

	int j = 1;
	int i = 0;
	int k = 0;
	WORD GuildCount = sBOARDMGR->GetTopListGuildCount();
	SetTableState(GuildCount);
	for (; k < GuildCount; k++)
	{
		if (GuildList[k].GuildLevel == 0)
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
		SafeStrCpy(ritem->pString[1], GuildList[k].GuildName, 17);
		sprintf(text, "%d", GuildList[k].GuildLevel);
		SafeStrCpy(ritem->pString[2], text, 4);
		sprintf(text, "%d", GuildList[k].MemeberCount);
		SafeStrCpy(ritem->pString[3], text, 4);
		SafeStrCpy(ritem->pString[4], GuildList[k].MasterName, 17);
		sprintf(text, "%d", GuildList[k].GuildPoint);
		SafeStrCpy(ritem->pString[5], text, 17);
		ritem->dwID = j-1;
		m_pList[i]->InsertItem(j-1, ritem);
		j++;
		if (j % 20 == 1)
			i++;		
	}
}

