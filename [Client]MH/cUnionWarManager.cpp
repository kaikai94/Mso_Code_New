#include "stdafx.h"
#include "cUnionWarManager.h"
#include "WindowIDEnum.h"

#include "cWindowManager.h"
#include "./input/UserInput.h"
#include "cRITEMEx.h"
#include "UnionWarInfo.h"
#include "ChatManager.h"
#include "cTextArea.h"
cUnionWarManager::cUnionWarManager()
{
	m_pSPlayerKill = NULL;
	m_pSPlayerDie = NULL;
	m_pLUnionWarInfoList = NULL;
	m_TipMsg = NULL;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
	m_dwTimeCount = 0;
}


cUnionWarManager::~cUnionWarManager()
{
	if (m_pLUnionWarInfoList)
		m_pLUnionWarInfoList->DeleteAllItems();
	m_pSPlayerKill = NULL;
	m_pSPlayerDie = NULL;
	m_pLUnionWarInfoList = NULL;
	m_TipMsg = NULL;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
	m_dwTimeCount = 0;
}
void cUnionWarManager::Linking()
{
	m_pSPlayerKill = (cStatic*)GetWindowForID(UNIONWAR_PLAYER_KILL);
	m_pSPlayerDie = (cStatic*)GetWindowForID(UNIONWAR_PLAYER_DIE);
	m_pLUnionWarInfoList = (cListCtrl*)GetWindowForID(UNIONWAR_INFO_LIST);
	m_TipMsg = (cTextArea*)GetWindowForID(UNIONWAR_TIPMSG);
	m_TipMsg->SetTextColor(RGB(250, 206, 135));
	//m_TipMsg->SetChangeColor(true);
}
void cUnionWarManager::ShowUnionWarInfo(DWORD dwKill, DWORD dwDie)
{
	char buff[512] = { 0, };
	sprintf(buff, "%d", dwKill);
	m_pSPlayerKill->SetStaticText(buff);

	sprintf(buff, "%d", dwDie);
	m_pSPlayerDie->SetStaticText(buff);

	WORD wCount = 0;
	sUnionWarInfo * sInfo = NULL;
	cUNIONWARMGR->GetUnionWarInfoTable().SetPositionHead();
	//m_UnionInfo.SetPositionHead();
	m_pLUnionWarInfoList->DeleteAllItems();
	while (sInfo = cUNIONWARMGR->GetUnionWarInfoTable().GetData())
	{
		if (sInfo)
		{
			cRITEMEx *ritem = new cRITEMEx(4);
			ritem->rgb[0] = RGBA_MAKE(135, 206, 250, 255);
			ritem->rgb[1] = RGBA_MAKE(135, 206, 250, 255);
			ritem->rgb[2] = RGBA_MAKE(135, 206, 250, 255);
			ritem->rgb[3] = RGBA_MAKE(135, 206, 250, 255);

			SafeStrCpy(ritem->pString[0], sInfo->sUnionName, 17);
			sprintf(buff, "%d", sInfo->dwKillTimeCount);
			SafeStrCpy(ritem->pString[1], buff, 8);

			sprintf(buff, "%d", sInfo->dwDieTimeCount);
			SafeStrCpy(ritem->pString[2], buff, 8);

			sprintf(buff, "%d", sInfo->wUnionPlayerCount);
			SafeStrCpy(ritem->pString[3], buff, 8);
			ritem->dwID = wCount;
			m_pLUnionWarInfoList->InsertItem(wCount, ritem);
			wCount++;
		}
	}
	//SetActive(TRUE);
}
void cUnionWarManager::SetKillTimes(DWORD dwKillTimes)
{
	char buff[32] = { 0, };
	sprintf(buff, "%d", dwKillTimes);
	m_pSPlayerKill->SetStaticText(buff);
}
void cUnionWarManager::SetDieTimes(DWORD dwDieTimes)
{
	char buff[32] = { 0, };
	sprintf(buff, "%d", dwDieTimes);
	m_pSPlayerDie->SetStaticText(buff);
}
void cUnionWarManager::UpdateUnionInfo(DWORD dwUnionID, DWORD dwDieTimes, DWORD dwKillTimes)
{
	sUnionWarInfo * sInfo = NULL;
	sInfo = cUNIONWARMGR->GetUnionWarInfo(dwUnionID);
	if (sInfo)
	{
		if (dwDieTimes!=0)
			sInfo->dwDieTimeCount = dwDieTimes;
		if (dwKillTimes!=0)
			sInfo->dwKillTimeCount = dwKillTimes;
		RITEM * item = NULL;
		for (WORD wIndex = m_pLUnionWarInfoList->GetItemCount(); wIndex >=0; wIndex--)
		{
			item = m_pLUnionWarInfoList->GetRItem(wIndex-1);
			if (strcmp(item->pString[0], sInfo->sUnionName)==0)
				break;
		}
		char buff[8] = { 0 };
		sprintf(buff, "%d", sInfo->dwKillTimeCount);
		SafeStrCpy(item->pString[1], buff, 8);

		sprintf(buff, "%d", sInfo->dwDieTimeCount);
		SafeStrCpy(item->pString[2], buff, 8);
	}
}
void cUnionWarManager::SetActive(BOOL val)
{
	if (!val)
		m_pLUnionWarInfoList->DeleteAllItems();
	cDialog::SetActive(val);
}
void cUnionWarManager::UpdateUnionPlayerCount(DWORD dwUnionID, DWORD dwPlayerCount)
{
	sUnionWarInfo* pInfo = NULL;
	pInfo = cUNIONWARMGR->GetUnionWarInfo(dwUnionID);
	if (NULL != pInfo)
	{
		pInfo->wUnionPlayerCount = dwPlayerCount;
		RITEM * item = NULL;
		for (WORD wIndex = m_pLUnionWarInfoList->GetItemCount(); wIndex >= 0; wIndex--)
		{
			item = m_pLUnionWarInfoList->GetRItem(wIndex - 1);
			if (strcmp(item->pString[0], pInfo->sUnionName) == 0)
				break;
		}
		char buff[8] = { 0 };
		sprintf(buff, "%d", pInfo->wUnionPlayerCount);
		SafeStrCpy(item->pString[3], buff, 8);
	}
}
void cUnionWarManager::UpdateTipMsg()
{
	char buff[1024] = { 0 };
	DWORD dwMinute = m_dwWarTime / 60 + 1;

	switch (m_dwWarState)
	{
	case eUnionWarState_Befor30:
	case eUnionWarState_Befor20:
		{		
			if (gCurTime - m_dwTimeCount >= 60000)
			{
				if (m_dwTimeCount != 0)
					m_dwWarTime -= 60;
				m_dwTimeCount = gCurTime;
			}
			sprintf(buff, CHATMGR->GetChatMsg(2430), dwMinute, dwMinute - 10);			
		}
		break;
	case eUnionWarState_Befor10:
		{
			if (gCurTime - m_dwTimeCount >= 60000)
			{
				if (m_dwTimeCount != 0)
					m_dwWarTime -= 60;
				m_dwTimeCount = gCurTime;
			}
			sprintf(buff, CHATMGR->GetChatMsg(2431), dwMinute);	
		}
		break;
	case eUnionWarState_Ing:
		{
			DWORD wSecond = 60- (dwMinute * 60 - m_dwWarTime);
			sprintf(buff, CHATMGR->GetChatMsg(2432), dwMinute, wSecond);
			if (gCurTime - m_dwTimeCount >= 1000)
			{
				if (m_dwTimeCount != 0)
					m_dwWarTime -= 1;
				m_dwTimeCount = gCurTime;
			}
		}
		break;
	case eUnionWarState_Pause:
		break;
	case eUnionWarState_None:
		break;
	default:
		break;
	}
	
	m_TipMsg->SetScriptText(buff);
}