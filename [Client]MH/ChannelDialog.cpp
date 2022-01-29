// ChannelDialog.cpp: implementation of the CChannelDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelDialog.h"
#include "WindowIDEnum.h"
#include "CharSelect.h"
#include "cRItemEx.h"
#include "ChatManager.h"
#include "MainGame.h"
#include "ObjectManager.h"
#include "./Input/UserInput.h"
#include "MoveDialog.h"
#include "GameIn.h"
#include "ObjectStateManager.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
#include "MHTimeManager.h"	//[过图读秒引用][By:十里坡剑神][QQ:112582793][2018/5/1 20:53]
#include "AIManager.h"		//[过图读秒引用][By:十里坡剑神][QQ:112582793][2018/5/1 20:53]
int gChannelNum = -1;
bool IsBattleChannel[MAX_CHANNEL_NUM] = {0,};
//[增加安全线配置][By:十里坡剑神][QQ:112582793][2018/3/13]
bool IsPeaceChannel[MAX_CHANNEL_NUM] = {0,};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CChannelDialog)

CChannelDialog::CChannelDialog()
{
	m_bInit = FALSE;
	m_SelectRowIdx = 0;
	m_BaseChannelIndex = 0;
	m_wMoveMapNum = 0;
	m_dwChangeMapState = 0;
}

CChannelDialog::~CChannelDialog()
{
  if(m_pChannelLCtrl)
  {
     m_pChannelLCtrl->DeleteAllItems();
  }
}

void CChannelDialog::Linking()
{
	m_pChannelLCtrl = (cListCtrl*)GetWindowForID(CHA_CHANNELLIST);
}


DWORD CChannelDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);

	
	int rowidx = m_pChannelLCtrl->GetSelectedRowIdx();
	if(we & WE_ROWCLICK)
	{
		SelectChannel(rowidx);
	}
	else if( we & WE_ROWDBLCLICK )
	{
//#ifdef _KOR_LOCAL_
		if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			OnConnect();
		else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			MapChange();
//#else
//		OnConnect();
//#endif
	}
	return we;
}


void CChannelDialog::SetChannelList(MSG_CHANNEL_INFO* pInfo)
{
	m_pChannelLCtrl->DeleteAllItems();
	char temp[MAX_CHANNEL_NAME+4] = {0, };
	int len = strlen(temp);
	char num[2] = {0, };
	BYTE Count = 0;
	WORD LowCrowd = 1000; 
	int rowidx = 0;

	m_BaseChannelIndex = 0;

	for(int i=0; i<pInfo->Count; ++i)
	{
		IsBattleChannel[i]= pInfo->bBattleChannel[i];
		IsPeaceChannel[i] = pInfo->bIsPeaceChannel[i];
		cRITEMEx *ritem = new cRITEMEx(2);
//#ifdef _JAPAN_LOCAL_		
//		sprintf(temp, "%s%d", pInfo->ChannelName, i+1);
//#elif defined _HK_LOCAL_
//		sprintf(temp, pInfo->ChannelName, i+1);
//#elif defined _KOR_LOCAL_
		sprintf(temp, CHATMGR->GetChatMsg(1701), i+1);
//#else
//		sprintf(temp, "%s %d", pInfo->ChannelName, i+1);
//#endif
		strcpy(ritem->pString[0], temp);

//#ifdef _HK_LOCAL_
//		if(pInfo->PlayerNum[i] < 40)
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211) );
//
//		else if(pInfo->PlayerNum[i] < 140)	//MAX USER PER CHANNEL
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212) );
//		else 
//		{
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213) );
//			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);			
//		}
//#elif defined _TL_LOCAL_
//		if(pInfo->PlayerNum[i] < 50)
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211) );
//		else if(pInfo->PlayerNum[i] < 250)	//MAX USER PER CHANNEL
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212) );
//		else 
//		{
//			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213) );
//			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);			
//		}
//
//#else
		if(pInfo->PlayerNum[i] < 40)
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(211) );
		else if(pInfo->PlayerNum[i] < 100)
			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(212) );
		else 
		{

			strcpy(ritem->pString[1], CHATMGR->GetChatMsg(213) );
			ritem->rgb[1] = RGBA_MAKE(255,0,0,255);			
		}
		if( pInfo->bBattleChannel[i] )
		{
			char temp[64] = {0,};
			sprintf( temp, "(%s)", CHATMGR->GetChatMsg(1702) );
			strcat(ritem->pString[1], temp);
		}
		if(pInfo->bIsPeaceChannel[i])
		{
			char temp[64] = {0,};
			sprintf( temp, "(%s)", CHATMGR->GetChatMsg(2153) );
			strcat(ritem->pString[1], temp);
			ritem->rgb[1] = RGBA_MAKE(0,255,0,255);
		}
//#endif
		
		ritem->dwID = pInfo->PlayerNum[i];
		m_pChannelLCtrl->InsertItem(i, ritem);
		
		Count++;

		if(LowCrowd == 0)
			continue;
		if(LowCrowd > pInfo->PlayerNum[i])
		{
			LowCrowd = pInfo->PlayerNum[i];
			rowidx = i;
		}		
	}

	rowidx = 0;

	cRITEMEx* pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(rowidx);

	pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
	gChannelNum = rowidx+m_BaseChannelIndex;
	m_SelectRowIdx = rowidx;
	SetActive(TRUE);
#ifdef _GMTOOL_
	GMTOOLMGR->SetChannelCount( Count );
#endif

	m_wMoveMapNum = pInfo->wMoveMapNum;
	m_dwChangeMapState = pInfo->dwChangeMapState;
}

void CChannelDialog::SetActive(BOOL val)
{
	cDialog::SetActive(val);
}

void CChannelDialog::OnConnect()
{	
	cRITEMEx * pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(m_SelectRowIdx);
	if(pRItem)
	{
		if(gChannelNum == -1)
		{
			CHARSELECT->DisplayNotice(279);
		}
		else if( CHARSELECT->EnterGame() == FALSE )
		{
			CHARSELECT->DisplayNotice(18);
		}
	}
}

void CChannelDialog::SelectChannel(int rowidx)
{	
	cRITEMEx * pRItem = (cRITEMEx *)m_pChannelLCtrl->GetRItem(rowidx);
	
	if(pRItem != NULL)
	{
		if(m_SelectRowIdx != rowidx)
		{
			pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
			
			pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(m_SelectRowIdx);
			pRItem->rgb[0] = RGBA_MAKE( 255, 255 ,255 ,255);
			
			cRITEMEx* pRItem = (cRITEMEx*)m_pChannelLCtrl->GetRItem(rowidx);

			if(pRItem->dwID >= 300)
				gChannelNum = -1;
			else
				gChannelNum = rowidx + m_BaseChannelIndex;
			
			m_SelectRowIdx = rowidx;
		}
	}
}

void CChannelDialog::SendMapChannelInfoSYN(WORD wMapNum, DWORD dwState)
{
	MSG_DWORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData1 = wMapNum;
	msg.dwData2 = dwState;
	NETWORK->Send( &msg, sizeof(msg) );
}

void CChannelDialog::MapChange()

{
	if(m_wMoveMapNum != 0)
	{
		if(m_dwChangeMapState == eMapChange_General)
			g_UserInput.GetHeroMove()->MapChange( m_wMoveMapNum);

		else if(m_dwChangeMapState == eMapChange_Item)
		{
			CMoveDialog* pMoveDlg = GAMEIN->GetMoveDialog();
			if(pMoveDlg != NULL)
			{
				pMoveDlg->MapChange(m_wMoveMapNum);				
			}
		}		
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1699) );
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}

	SetActive( FALSE );

	m_wMoveMapNum = 0;
	m_dwChangeMapState = 0;
}



void CChannelDialog::InitDmove()  //[过图读秒开始][By:十里坡剑神][QQ:112582793][2018/5/1 20:54]
{
	nMcount = 3;
	CurTime = MHTIMEMGR->GetNewCalcCurTime();
	MoveTime = CurTime - 1000;
}
void CChannelDialog::DmoveMap() //[过图读秒计算][By:十里坡剑神][QQ:112582793][2018/5/1 20:54]
{
	if (gCurTime - MoveTime >= 1000)
	{
		MoveTime += 1000;
		if (nMcount <= 0)
		{
			NETWORK->Send(&moMsg, sizeof(moMsg));
			g_UserInput.SetInputFocus(FALSE);
			//sCPlayerAI->SaveConfig();   //[写入内挂配置][By:十里坡剑神][QQ:112582793][2018/5/1 20:54]
			IsMove = FALSE;
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2299), nMcount);
		}
		--nMcount;
	}
}

