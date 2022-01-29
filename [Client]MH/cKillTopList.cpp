
#include "stdafx.h"
#include "cKillTopList.h"
#include "cDialogueList.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "cRITEMEx.h"
#include "BillBoardManager.h"
#include "DealItem.h"
#include "ItemManager.h"
#include "Item.h"
#include "cImage.h"
#include "ObjectManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cListCtrl.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"

cKillTopList::cKillTopList(void)
{
	m_GetType = eNoPice;
	m_DayInfo[eMonday] = "星期一";
	m_DayInfo[eTuesday] = "星期二";
	m_DayInfo[eWednesday] = "星期三";
	m_DayInfo[eThursday] = "星期四";
	m_DayInfo[eFriday] = "星期五";
	m_DayInfo[eSaturday] = "星期六";
	m_DayInfo[eSunday] = "星期日";
	LoadGiftInfo();
}

cKillTopList::~cKillTopList(void)
{
	m_GiftInfo.clear();
	m_DayInfo.clear();
}
void cKillTopList::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
void cKillTopList::Linking()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 0 );
	m_memdlg[0] = (cListCtrl*)pDlg->GetWindowForID(BILL_KILL_MEMBERLIST);
	cDialog* pDlg1 = (cDialog*)GetTabSheet( 1 );
	m_memdlg[1] = (cListCtrl*)pDlg1->GetWindowForID(BILL_KILL_MEMBERLIST2);
	cDialog* pDlg2 = (cDialog*)GetTabSheet( 2 );
	m_memdlg[2] = (cListCtrl*)pDlg2->GetWindowForID(BILL_KILL_MEMBERLIST3);
	cDialog* pDlg3 = (cDialog*)GetTabSheet( 3 );
	m_memdlg[3] = (cListCtrl*)pDlg3->GetWindowForID(BILL_KILL_MEMBERLIST4);
	cDialog* pDlg4 = (cDialog*)GetTabSheet( 4 );
	m_memdlg[4] = (cListCtrl*)pDlg4->GetWindowForID(BILL_KILL_MEMBERLIST5);
	m_sTime			= (cStatic*)this->GetWindowForID(BILL_KILL_TIME);
	m_IconDlg	= (cIconGridDialog*) this->GetWindowForID(BILL_KILL_ICON_ITEM);
}


void cKillTopList::SetTableState(WORD count)
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

void cKillTopList::EndDialog()
{
	for (int i = 0;i<5;i++)
	{
		m_memdlg[i]->DeleteAllItems();
	}
	cIcon* pIcon;
	pIcon = m_IconDlg->GetIconForIdx(0);
	if(pIcon != NULL)
	{
		m_IconDlg->DeleteIcon(0,&pIcon);
		pIcon->SetActive( FALSE );
		WINDOWMGR->AddListDestroyWindow( pIcon );
		pIcon = NULL;
	}
}


void cKillTopList::OpenKillBoard()
{
	EndDialog();	
	ShowKillTopList();
	SetActive(TRUE);
}
void cKillTopList::SetActive( BOOL val )
{
	if( m_bActive == val ) return;
	if(!val)EndDialog();	
	cDialog::SetActiveRecursive( val );
}

void cKillTopList::ShowKillTopList()
{
	char text[128];
	sprintf(text,CHATMGR->GetChatMsg(2229),
		sBOARDMGR->GetUpdateTime().wYear,
		sBOARDMGR->GetUpdateTime().wMonth,
		sBOARDMGR->GetUpdateTime().wDay,
		sBOARDMGR->GetUpdateTime().wHour,
		sBOARDMGR->GetUpdateTime().wMinute);
	m_sTime->SetStaticText(text);

	KILLPLAYERTOPLIST tmp[MAX_TOP_LIST_COUNT];
	memset(tmp, 0, sizeof(KILLPLAYERTOPLIST) * MAX_TOP_LIST_COUNT);
	memcpy(tmp, sBOARDMGR->GetKillList(), sizeof(KILLPLAYERTOPLIST)*MAX_TOP_LIST_COUNT);
	int j = 1;
	int i = 0;
	int k = 0;
	WORD KillerCount = sBOARDMGR->GetTopListKillerCount();
	SetTableState(KillerCount);
	for (; k < KillerCount; k++)
	{
		if (tmp[k].Grade == 0)
			break;
		else
		{
			cRITEMEx *ritem = new cRITEMEx(4);

			ritem->rgb[0] = RGBA_MAKE(255, 255, 255, 255);
			ritem->rgb[1] = RGBA_MAKE(255, 255, 255, 255);
			ritem->rgb[2] = RGBA_MAKE(255, 255, 255, 255);
			ritem->rgb[3] = RGBA_MAKE(255, 255, 255, 255);

			sprintf(text, "%d", j);
			SafeStrCpy(ritem->pString[0], text, 4);
			SafeStrCpy(ritem->pString[1], tmp[k].Name, 17);
			sprintf(text, "%d", tmp[k].Grade);
			SafeStrCpy(ritem->pString[2], text, 4);
			sprintf(text, "%d", tmp[k].KillPlayerTimes);
			SafeStrCpy(ritem->pString[3], text, 7);
			ritem->dwID = j-1;
			m_memdlg[i]->InsertItem(j-1, ritem);
			j++;
			if (j % 20 == 1)
				i++;
		}
	}


	std::map<eSINGED,std::string>::iterator iter;
	iter = m_DayInfo.find(m_GetType);
	if(iter==m_DayInfo.end())
	{
		m_IconDlg->SetDisable(TRUE);
		return;
	}
	std::map<std::string,SINGEDGIFTINFO>::iterator iter2;
	iter2 = m_GiftInfo.find(iter->second);
	if(iter2==m_GiftInfo.end())
	{
		m_IconDlg->SetDisable(TRUE);
		return;
	}
	
	CDealItem* pItem = NULL;
	cImage tmpImage;
	pItem = new CDealItem;
	pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(iter2->second.wItemIdx, &tmpImage),0);
	pItem->SetData(iter2->second.wItemIdx);
	pItem->SetItemIdx(iter2->second.wItemIdx);
	pItem->m_wIdx=i;
	pItem->SetMovable(FALSE);
	ITEMMGR->SetGoldShopToolTipIcon(pItem);
	WINDOWMGR->AddWindow(pItem);
	m_IconDlg->AddIcon(0,pItem);
	if(m_IconDlg->IsDisable())
		m_IconDlg->SetDisable(FALSE);
}
void cKillTopList::SetHeroKillTopInfo(SINGEDINFO& info)
{
	memcpy(&m_HeroKillTopInfo,&info,sizeof(m_HeroKillTopInfo));
	InitDlgForHero();
	if(this->IsActive())
	{
		SetActive(FALSE);
		OpenKillBoard();
	}
}
void cKillTopList::InitDlgForHero()
{
	if(m_HeroKillTopInfo.Mon==eGift_Get_OK)
		m_GetType = eMonday;
	else if(m_HeroKillTopInfo.Tues==eGift_Get_OK)
		m_GetType =eTuesday;
	else if(m_HeroKillTopInfo.Wed==eGift_Get_OK)
		m_GetType = eWednesday;
	else if(m_HeroKillTopInfo.Thur==eGift_Get_OK)
		m_GetType = eThursday;
	else if(m_HeroKillTopInfo.Fri == eGift_Get_OK)
		m_GetType = eFriday;
	else if(m_HeroKillTopInfo.Sat==eGift_Get_OK)
		m_GetType = eSaturday;
	else if(m_HeroKillTopInfo.Mon==eGift_Get_Geted&&m_HeroKillTopInfo.Thur==eGift_Get_Geted&&
			m_HeroKillTopInfo.Wed==eGift_Get_Geted&&m_HeroKillTopInfo.Tues==eGift_Get_Geted&&
			m_HeroKillTopInfo.Fri==eGift_Get_Geted&&m_HeroKillTopInfo.Sat==eGift_Get_Geted&&
			m_HeroKillTopInfo.Sun==eGift_Get_OK)
			m_GetType = eSunday;
	else
		m_GetType= eNoPice;
}
void cKillTopList::LoadGiftInfo()
{
	CMHFile file;
	if (!file.Init("./Resource/KillTopListGiftInfo.bin","rb"))
		return;
	char Day[16];
	while(1)
	{
		if(file.IsEOF())
			break;
		SINGEDGIFTINFO info;

		file.GetString(Day);
		std::string temp = Day;
		info.wCount = file.GetWord();
		info.wItemIdx = file.GetWord();
		m_GiftInfo[temp]=info;
	}
	file.Release();
}
void cKillTopList::OnActionEvent(LONG lId, void * p, DWORD we)
{
	//[屏蔽击杀排行礼包][By:十里坡剑神][QQ:112582793][2018/5/3 12:39]
	//if(m_GetType==eNoPice) return;
	//MSG_SINGED msg;
	//msg.Category = MP_KILLTOPLIST;
	//msg.Protocol = MP_KILLTOPLIST_SYN;
	//msg.dwObjectID = HEROID;
	//msg.eType = m_GetType;
	//NETWORK->Send(&msg,sizeof(msg));
}
