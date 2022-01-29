#include "stdafx.h"
#include "UnionWarInfo.h"
#include "cUnionWarManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "ChannelDialog.h"
#include "ObjectStateManager.h"
#include "MoveDialog.h"
#include "GuildManager.h"
#include "./input/UserInput.h"
#include "MHMap.h"
#include "cMsgBox.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"

GLOBALTON(UnionWarInfo)

UnionWarInfo::UnionWarInfo()
{
	m_UnionInfo.Initialize(20);
	m_dwKillTimes = 0;
	m_dwDieTimes = 0;
	m_UnionMapNum = 0;
	m_bIsWarStart = FALSE;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
}


UnionWarInfo::~UnionWarInfo()
{
	m_dwKillTimes = 0;
	m_dwDieTimes = 0;
	m_UnionMapNum = 0;
	m_bIsWarStart = FALSE;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
	Release();
}
void UnionWarInfo::Release()
{
	sUnionWarInfo * sInfo = NULL;
	m_UnionInfo.SetPositionHead();
	while (sInfo = m_UnionInfo.GetData())
	{
		if (sInfo)
			delete sInfo;
		sInfo = NULL;
	}
	m_UnionInfo.RemoveAll();
}
void UnionWarInfo::InitUnionWarInfo(UNIONWARINFO* pInfo)
{
	Release();
	sUnionWarInfo * sInfo = NULL;
	for (int i = 0; i < 10; i++)
	{
		if (pInfo[i].wUnionPlayerCount == 0)
			break;
		sInfo = new sUnionWarInfo;
		sInfo->dwUnionID = pInfo[i].dwUnionID;
		sInfo->dwDieTimeCount = pInfo[i].wUnionDieCount;
		sInfo->dwKillTimeCount = pInfo[i].wUnionKillCount;
		sInfo->wUnionPlayerCount = pInfo[i].wUnionPlayerCount;
		SafeStrCpy(sInfo->sUnionName, pInfo[i].sUnionName, MAX_GUILD_NAME + 1);
		m_UnionInfo.Add(sInfo, sInfo->dwUnionID);
	}
}
void UnionWarInfo::EndUnionWarInfo()
{
	Release();
	m_dwKillTimes = 0;
	m_dwDieTimes = 0;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
	//SetActive(FALSE);
	GAMEIN->GetUnionWarInfoDlg()->SetActive(FALSE);
}
sUnionWarInfo * UnionWarInfo::GetUnionWarInfo(DWORD dwUnionID)
{
	sUnionWarInfo* pInfo = NULL;
	pInfo = m_UnionInfo.GetData(dwUnionID);
	if (pInfo)
		return pInfo;
	return NULL;
}
void UnionWarInfo::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_UNIONWAR_PLAYERCOUNT_NOTIFY:
		{//[联盟人数变化更新消息][BY:十里坡剑神][QQ:112582793][2019-10-29][23:55:53]
			MSG_DWORD2 *pmsg = (MSG_DWORD2*)pMsg;
			GAMEIN->GetUnionWarInfoDlg()->UpdateUnionPlayerCount(pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_UNIONWAR_KILL_NOTIFY:
		{//[击杀广播消息][BY:十里坡剑神][QQ:112582793][2019-10-25][20:12:24]
			MSG_UNIONWARPLAYERKILLINFO* pmsg = (MSG_UNIONWARPLAYERKILLINFO*)pMsg;

			if (pmsg->dwObjectID == HEROID)
				GAMEIN->GetUnionWarInfoDlg()->SetKillTimes(pmsg->dwPlayerKillTimes);
			else if (pmsg->dwDiePlayerID == HEROID)
				GAMEIN->GetUnionWarInfoDlg()->SetDieTimes(pmsg->dwPlayerDieTimes);

			GAMEIN->GetUnionWarInfoDlg()->UpdateUnionInfo(pmsg->dwKillUnionID, 0, pmsg->dwUnionKillTimes);
			GAMEIN->GetUnionWarInfoDlg()->UpdateUnionInfo(pmsg->dwDieUnionID, pmsg->dwUnionDieTimes, 0);
		}
		break;
	case MP_UNIONWAR_PLAYERDIETOLONG_NOTIFY:
		{//[死亡复活超时惩罚][BY:十里坡剑神][QQ:112582793][2019-10-31][21:26:10]
			MSG_UNIONWARPLAYERDIE* pmsg = (MSG_UNIONWARPLAYERDIE*)pMsg;
			GAMEIN->GetUnionWarInfoDlg()->UpdateUnionInfo(pmsg->dwUnionID, pmsg->dwUnionDie, pmsg->dwUnionKill);
			if (pmsg->dwPlayerID == HEROID)
			{
				GAMEIN->GetUnionWarInfoDlg()->SetKillTimes(pmsg->dwPlayerKill);
			}
			CHATMGR->AddMsg(CTC_TIP_KILL, CHATMGR->GetChatMsg(2429), pmsg->UnionName, pmsg->PlayerName);
		}
		break;
	case MP_UNIONWAR_BEFOR_TIME_NOTIFY:
		{//[盟战开始前时间提示][BY:十里坡剑神][QQ:112582793][2019-10-25][20:10:20]
			MSG_DWORD5* pmsg = (MSG_DWORD5*)pMsg;
			m_UnionMapNum = pmsg->dwData2;
			switch (pmsg->dwData1)
			{
			case eUnionWarState_Befor30:	CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2425), 30);	break;
			case eUnionWarState_Befor20:	CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2425), 20);	break;
			case eUnionWarState_Befor10:	CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2426), 10);	break;
			default:
				break;
			}
			m_dwWarState = eUnionWarState(pmsg->dwData1);
			m_dwWarTime = pmsg->dwData5;
			GAMEIN->GetUnionWarInfoDlg()->SetUnionWarTime(m_dwWarTime);
			GAMEIN->GetUnionWarInfoDlg()->SetUnionState(m_dwWarState);
			if (pmsg->dwData1 < eUnionWarState_Befor10)
			{
				if (HERO->GetGuildUnionIdx() != 0 &&
					HERO->GetLevel() > pmsg->dwData4 &&
					HERO->GetReset() >= pmsg->dwData3 &&
					m_UnionMapNum != MAP->GetMapNum())
				{
					cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MSBOX_UNIONENTER, MBT_YESNO,
						CHATMGR->GetChatMsg(2428));
				}
			}
		}
		break;
	case MP_UNIONWAR_WARINFO_NOTIFY:
		{//[盟战开始服务端发送的联盟信息][BY:十里坡剑神][QQ:112582793][2019-10-25][20:13:56]
			MSG_UNIONWARINFO* pmsg = (MSG_UNIONWARINFO*)pMsg;

			InitUnionWarInfo(pmsg->sUnionInfo);
			m_dwKillTimes = pmsg->dwPlayerKillTimes;
			m_dwDieTimes = pmsg->dwPlayerDieTimes;
			GAMEIN->GetUnionWarInfoDlg()->ShowUnionWarInfo(m_dwKillTimes, m_dwDieTimes);
			m_dwWarState = pmsg->eWarState;
			m_dwWarTime = pmsg->dwUnionWarTime;
			GAMEIN->GetUnionWarInfoDlg()->SetUnionWarTime(m_dwWarTime);
			GAMEIN->GetUnionWarInfoDlg()->SetUnionState(m_dwWarState);
			//if (m_dwWarState==eUnionWarState_Ing)
			m_bIsWarStart = TRUE;
		}
		break;
	case MP_UNIONWAR_MOVEIN_NACK:
		{//[进入地图失败消息][BY:十里坡剑神][QQ:112582793][2019-10-25][20:11:42]
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			switch (pmsg->bData)
			{
			case eUnionMoveIn_Enough:
				{//[人数已满][BY:十里坡剑神][QQ:112582793][2019-10-25][20:55:57]
					CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
					if (pChannelDlg != NULL)
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2421));
						pChannelDlg->SetActive(FALSE);
					}

					if (HERO->GetState() == eObjectState_Deal)
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					if (GAMEIN->GetMoveDialog()->IsActive())
					{//[解决地图关闭卡回城][By:十里坡剑神][QQ:112582793][2017/12/9]
						GAMEIN->GetMoveDialog()->SetTownMoveView(FALSE);
						GAMEIN->GetMoveDialog()->SetActive(FALSE);
					}
				}
				break;
			case eUnionMoveIn_NotTime:
				{//[盟战已经结束][BY:十里坡剑神][QQ:112582793][2019-10-25][20:56:05]
					CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
					if (pChannelDlg != NULL)
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2422));
						pChannelDlg->SetActive(FALSE);
					}

					if (HERO->GetState() == eObjectState_Deal)
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					if (GAMEIN->GetMoveDialog()->IsActive())
					{//[解决地图关闭卡回城][By:十里坡剑神][QQ:112582793][2017/12/9]
						GAMEIN->GetMoveDialog()->SetTownMoveView(FALSE);
						GAMEIN->GetMoveDialog()->SetActive(FALSE);
					}
				}
				break;
			case eUnionMoveIn_Substandard:
				{//[不符合进入条件][BY:十里坡剑神][QQ:112582793][2019-10-25][23:49:22]
					if (HERO->GetState() == eObjectState_Deal)
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2423));
					g_UserInput.SetInputFocus(TRUE);

					if (GUILDMGR->GetSOSTargetPos())
					{
						GUILDMGR->SetSOSTargetPos(0);
					}

					if (GUILDMGR->GetSOSChannelIdx())
					{
						GUILDMGR->SetSOSChannelIdx(-1);
					}

					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
					ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

					GAMEIN->GetMoveDialog()->SetItemToMapServer(0, 0, FALSE);
				}
				break;
			default:
				break;
			}
		}
		break;
	case MP_UNIONWAR_END_NOTIFY:
		{//[结束广播消息][BY:十里坡剑神][QQ:112582793][2019-10-25][20:13:06]
			UNIONEND *pmsg = (UNIONEND*)pmsg;
			CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2433), pmsg->sUnionName, pmsg->wUnionKillCount);
			EndUnionWarInfo();
			m_dwWarState = eUnionWarState_None;
			m_dwWarTime = 0;
			m_bIsWarStart = FALSE;
		}
		break;
	case MP_UNIONWAR_RETURNTOMAP:
		{//[返回进入地图][BY:十里坡剑神][QQ:112582793][2019-10-25][20:37:33]
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			gChannelNum = 0;
			m_UnionMapNum = 0;
			GAMEIN->SetMoveMap(pmsg->dwData);
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MSBOX_UNIONRETURN, MBT_YESNO,
				CHATMGR->GetChatMsg(2424));
		}
		break;
	default:
		break;
	}
}