



#include "stdafx.h"
#include "UnionWarManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "Player.h"
#include "UserTable.h"
#include "ObjectStateManager.h"
#include "PackedData.h"
#include "Network.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "GuildUnionManager.h"
#include "GuildUnion.h"
#include "PKManager.h"
#include <map>
extern WORD  g_wUnionMapNum;

GLOBALTON(CUnionWarManager)
template<class _Ty = void>
class MapCompare
{ 
public:
	bool operator () (const _Ty& _Left, const _Ty& _Right) const
	{   
		return (_Left > _Right);
	}
};
CUnionWarManager::CUnionWarManager(void)
{
	MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum(m_UnionSet.wBackToMapNum);
	if (ChangeInfo)
	{
		m_UnionSet.wBackToMapKind = ChangeInfo->Kind;
	}
}

CUnionWarManager::~CUnionWarManager(void)
{
	
}

void CUnionWarManager::Init()
{//[��ʼ����ս��Ϣ����ʼǰ�����ȵ���][BY:ʮ���½���][QQ:112582793][2019-10-23][10:21:35]
	m_UnionSet.bUnionWarMap = FALSE;
	m_dwWarState = eUnionWarState_None;
	m_dwWarTime = 0;
	memset( m_UnionWarTimeInfo, 0, sizeof(sUnionWarTimeInfo)*eUnionWarDay_Max);

	if (!LoadUnionWarInfo())
		return;
	m_UnionWarUnionTable.Initialize(m_UnionSet.wMaxTeamNum);
	m_UnionLeavePlayer.Initialize(m_UnionSet.wMaxTeamPlayerNum*m_UnionSet.wMaxTeamNum);
}

void CUnionWarManager::Release()
{//[�ͷ���ս��Դ��������������][BY:ʮ���½���][QQ:112582793][2019-10-23][10:21:59]
	sUnionWarInfo* pInfo = NULL;
	m_UnionWarUnionTable.SetPositionHead();
	while (pInfo = m_UnionWarUnionTable.GetData())
	{
		delete pInfo;
		pInfo = NULL;
	}
	m_UnionWarUnionTable.RemoveAll();
	m_UnionWarUnionTable.~CYHHashTable();
	m_UnionLeavePlayer.~CYHHashTable();
}

BOOL CUnionWarManager::LoadUnionWarInfo()
{//[������ս������Ϣ��ʼ��ʱ�Զ�����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:22:25]
	CMHFile file;
	char buf[256] = { 0, };
	char line[512] = { 0,};
	if (file.Init("./Resource/Server/UnionWarInfo.bin","rb") != TRUE)
	{
		g_Console.LOGEX(4, RGB(255,0,0),"��ս��Ϣ����ʧ��");
		return FALSE;
	}
	while (!file.IsEOF())
	{
		file.GetString(buf);
		if (buf[0] == '/')
		{
			file.GetLineX(line, 512);
			continue;
		}
		if (strcmp(buf, "*UNIONWAR") == 0)
		{
			g_wUnionMapNum = file.GetWord();
			if (g_wUnionMapNum == g_pServerSystem->GetMapNum())
			{
				m_UnionSet.bUnionWarMap = TRUE;
				for (int i = eUnionWarDay_Sun; i < eUnionWarDay_Max; ++i)
				{
					int nDayOfWeek = file.GetInt();
					m_UnionWarTimeInfo[nDayOfWeek].bOn = file.GetBool();
					m_UnionWarTimeInfo[nDayOfWeek].UnionWarStartTime.wHour = file.GetWord();
					m_UnionWarTimeInfo[nDayOfWeek].UnionWarStartTime.wMinute = file.GetWord();
					m_UnionWarTimeInfo[nDayOfWeek].dwWarTime = file.GetDword() * 60;
				}
			}
		}
		else if (strcmp(buf, "*MAXTEAMNUM")==0)
		{
			m_UnionSet.wMaxTeamNum = file.GetWord();
		}
		else if (strcmp(buf, "*MAXTEAMPLAYERNUM")==0)
		{
			m_UnionSet.wMaxTeamPlayerNum = file.GetWord();
		}
		else if (strcmp(buf, "*LIMITLEVEL") == 0)
		{
			m_UnionSet.wLimitLevel = file.GetWord();
		}
		else if (strcmp(buf, "*REWARDRANK") == 0)
		{
			m_UnionSet.wRewardRank = file.GetWord();
		}
		else if (strcmp(buf, "*REWARDRATIO") == 0)
		{
			m_UnionSet.wRewardRatio = file.GetWord();
		}
		else if (strcmp(buf, "*BASICREWARD") == 0)
		{
			m_UnionSet.wBasicReward = file.GetWord();
		}
		else if (strcmp(buf, "*MINPLAYER") == 0)
		{
			m_UnionSet.wMinPlayer = file.GetWord();
		}
		else if (strcmp(buf, "*LIMITPLAYERCOUNT") == 0)
		{
			m_UnionSet.bIsClear = file.GetBool();
		}
		else if (strcmp(buf, "*BACKTOMAPNUM") == 0)
		{
			m_UnionSet.wBackToMapNum = file.GetWord();
			MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum(m_UnionSet.wBackToMapNum);
			if (ChangeInfo)
			{
				m_UnionSet.wBackToMapKind = ChangeInfo->Kind;
			}
		}
	}
	file.Release();
	return m_UnionSet.bUnionWarMap;
}
int CUnionWarManager::GetSecondInterval(SYSTEMTIME curTime, SYSTEMTIME basicTime)
{//[���㿪ʼʱ������][BY:ʮ���½���][QQ:112582793][2019-10-23][14:41:42]
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;
	if( basicTime.wHour > curTime.wHour )
	{
		nHour = basicTime.wHour - curTime.wHour;

		if( basicTime.wMinute > curTime.wMinute )
		{
			nMin = basicTime.wMinute - curTime.wMinute;
		}
		else
		{
			nHour--;
			nMin = basicTime.wMinute + 60 - curTime.wMinute;
		}

		if( basicTime.wSecond >= curTime.wSecond )
		{
			nSec = basicTime.wSecond - curTime.wSecond;
		}
		else
		{
			nMin--;
			nSec = basicTime.wSecond + 60 - curTime.wSecond;
		}
		return nHour*60*60 + nMin*60 + nSec;
	}
	else
	{
		nHour = basicTime.wHour - curTime.wHour;
		nMin = basicTime.wMinute - curTime.wMinute;
		nSec = basicTime.wSecond - curTime.wSecond;
		return nHour*60*60 + nMin*60 + nSec;        
	}
}
BOOL CUnionWarManager::IsUnionWarStart()
{
	if (FALSE == m_UnionSet.bUnionWarMap/* || m_dwWarState >= eUnionWarState_Befor10*/)
		return FALSE;
	else if (m_dwWarState == eUnionWarState_Ing)
		return TRUE;
	return FALSE;
}
BOOL CUnionWarManager::IsUnionWarProcess()
{//[�����ж��Ƿ�Ϊ��ս��ͼ����ս�رտ���״̬][BY:ʮ���½���][QQ:112582793][2019-10-23][10:24:26]
	if (FALSE == m_UnionSet.bUnionWarMap/* || m_dwWarState >= eUnionWarState_Befor10*/)
		return FALSE;

	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	if (FALSE == m_UnionWarTimeInfo[curTime.wDayOfWeek].bOn)
		return FALSE;

	return TRUE;
}
void CUnionWarManager::ProcessUnionWar()
{//[��ս����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:00]
	if (!IsUnionWarProcess())
		return;

	SYSTEMTIME curTime;
	GetLocalTime( &curTime );	
	int nSec = GetSecondInterval(curTime, m_UnionWarTimeInfo[curTime.wDayOfWeek].UnionWarStartTime);
	switch( m_dwWarState )
	{
	case eUnionWarState_None:
		{	
			if (nSec > 1200 && nSec <= 1800)
			{//[��ʼǰ30����][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor30;
				 
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec > 600 && nSec <= 1200)
			{//[��ʼǰ20����][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor20;
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec > 0 && nSec <= 600)
			{//[��ʼǰ10����,�����������������][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor10;
				//[�����������������][BY:ʮ���½���][QQ:112582793][2019-10-25][20:02:45]
				if (m_UnionSet.bIsClear)
					ClearPlayerNotEnough();
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec <= 0)
			{
				m_dwWarState = eUnionWarState_Ing;
				if (abs(nSec) >= m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime)
					m_dwWarState = eUnionWarState_None;
				if (m_dwWarState == eUnionWarState_Ing)
				{
					PKMGR->SetPKAllowWithMsg(TRUE);
					m_dwWarTime = (m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec) * 1000 + gCurTime;
					//[���ø��˻�ɱ����][BY:ʮ���½���][QQ:112582793][2019-10-30][16:54:43]
					CharacterUnionWarInfoUpdate(NULL);
					//[ȫͼ�㲥��ս��ʼ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-23][0:47:48]
					SendUnionWarInfoToAllPlayer();
				}
			}
		}
		break;
	case eUnionWarState_Befor30:
		{
			if (nSec > 600 && nSec <= 1200)
			{//[��ʼǰ20����][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor20;
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec > 0 && nSec <= 600)
			{//[��ʼǰ10����,�����������������][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor10;
				//[�����������������][BY:ʮ���½���][QQ:112582793][2019-10-25][20:02:45]
				if (m_UnionSet.bIsClear)
					ClearPlayerNotEnough();
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec <= 0)
			{
				m_dwWarState = eUnionWarState_Ing;
				if (abs(nSec) >= m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime)
					m_dwWarState = eUnionWarState_None;
				if (m_dwWarState == eUnionWarState_Ing)
				{
					PKMGR->SetPKAllowWithMsg(TRUE);
					m_dwWarTime = (m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec) * 1000 + gCurTime;
					//[���ø��˻�ɱ����][BY:ʮ���½���][QQ:112582793][2019-10-30][16:54:43]
					CharacterUnionWarInfoUpdate(NULL);
					//[ȫͼ�㲥��ս��ʼ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-23][0:47:48]
					SendUnionWarInfoToAllPlayer();
				}
			}
		}
		break;
	case eUnionWarState_Befor20:
		{
			if (nSec > 0 && nSec <= 600)
			{//[��ʼǰ10����,�����������������][BY:ʮ���½���][QQ:112582793][2019-10-22][17:20:49]
				m_dwWarState = eUnionWarState_Befor10;
				//[�����������������][BY:ʮ���½���][QQ:112582793][2019-10-25][20:02:45]
				if (m_UnionSet.bIsClear)
					ClearPlayerNotEnough();
				MSG_DWORD5 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_BEFOR_TIME_NOTIFY;
				msg.dwData1 = m_dwWarState;
				msg.dwData2 = g_wUnionMapNum;
				msg.dwData3 = g_pServerSystem->GetGameConfig().MaxReSetTime;
				msg.dwData4 = m_UnionSet.wLimitLevel;
				msg.dwData5 = nSec;
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
			else if (nSec <= 0)
			{
				m_dwWarState = eUnionWarState_Ing;
				if (abs(nSec) >= m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime)
					m_dwWarState = eUnionWarState_None;
				if (m_dwWarState == eUnionWarState_Ing)
				{
					PKMGR->SetPKAllowWithMsg(TRUE);
					m_dwWarTime = (m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec) * 1000 + gCurTime;
					//[���ø��˻�ɱ����][BY:ʮ���½���][QQ:112582793][2019-10-30][16:54:43]
					CharacterUnionWarInfoUpdate(NULL);
					//[ȫͼ�㲥��ս��ʼ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-23][0:47:48]
					SendUnionWarInfoToAllPlayer();
				}
			}
		}
		break;
	case eUnionWarState_Befor10:
		{
			if (nSec <= 0)
			{
				m_dwWarState = eUnionWarState_Ing;
				if (abs(nSec) >= m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime)
					m_dwWarState = eUnionWarState_None;
				if (m_dwWarState == eUnionWarState_Ing)
				{
					PKMGR->SetPKAllowWithMsg(TRUE);
					m_dwWarTime = (m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec) * 1000 + gCurTime;
					//[���ø��˻�ɱ����][BY:ʮ���½���][QQ:112582793][2019-10-30][16:54:43]
					CharacterUnionWarInfoUpdate(NULL);
					//[ȫͼ�㲥��ս��ʼ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-23][0:47:48]
					SendUnionWarInfoToAllPlayer();

				}
			}
		}
		break;
	case eUnionWarState_Ing:
		{
			if( gCurTime >= m_dwWarTime )
			{
				EndUnionWar(TRUE);
				//[����][BY:ʮ���½���][QQ:112582793][2019-10-22][17:21:50]
				PKMGR->SetPKAllowWithMsg(FALSE);
				return;
			}
		}
		break;
	case eUnionWarState_Pause:
		{
			if (gCurTime >= m_dwWarTime)
			{
				m_dwWarState = eUnionWarState_None;
			}
		}
		break;
	}
}
void CUnionWarManager::CalcReward()
{//[���㽱��][BY:ʮ���½���][QQ:112582793][2019-10-31][13:22:46]

	sUnionWarInfo * pUnionInfo = NULL;
	sUnionWarInfo * pWinInfo = NULL;

	m_UnionWarUnionTable.SetPositionHead();

	while (pUnionInfo = m_UnionWarUnionTable.GetData())
	{//[����ʤ��������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:18:45]
		if (pUnionInfo)
		{
			if (pWinInfo)
			{
				if (pUnionInfo->dwKillTimeCount > pWinInfo->dwKillTimeCount)
				{
					pWinInfo = pUnionInfo;
				}
				else if (pUnionInfo->dwKillTimeCount == pWinInfo->dwKillTimeCount)
				{
					int kUnionKill = pUnionInfo->dwKillTimeCount - pUnionInfo->dwDieTimeCount;
					int kWinKill = pWinInfo->dwKillTimeCount - pWinInfo->dwDieTimeCount;
					if (kUnionKill > kWinKill)
					{
						pWinInfo = pUnionInfo;
					}
				}
				else if (pUnionInfo->UnionWarPlayerTable.GetDataNum() > pWinInfo->UnionWarPlayerTable.GetDataNum())
				{
					pWinInfo = pUnionInfo;
				}
			}
			else
			{
				pWinInfo = pUnionInfo;
			}
		}
		pUnionInfo = NULL;
	}
	//[������������map����][BY:ʮ���½���][QQ:112582793][2019-10-31][12:28:57]
	std::map<DWORD, CPlayer*, MapCompare<DWORD>> mPlayerInfo;
	if (pWinInfo)
	{//[����ʤ������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:19:14]
		UNIONEND msg;
		msg.Category = MP_UNIONWAR;
		msg.Protocol = MP_UNIONWAR_END_NOTIFY;
		SafeStrCpy(msg.sUnionName, pWinInfo->sUnionName, MAX_GUILD_NAME + 1);
		msg.wUnionKillCount = pWinInfo->dwKillTimeCount;
		g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		//[��ʤ�����������������ӵ���������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:19:25]
		CPlayer* pPlayer = NULL;
		pWinInfo->UnionWarPlayerTable.SetPositionHead();
		while (pPlayer = pWinInfo->UnionWarPlayerTable.GetData())
		{
			if (pPlayer)
			{
				mPlayerInfo[pPlayer->GetUnionWarKillTime()] = pPlayer;
			}
			pPlayer = NULL;
		}
		m_UnionWarUnionTable.Remove(pWinInfo->dwUnionID);
		pWinInfo = NULL;
	}

	int nCount = m_UnionSet.wRewardRank;
	std::map<DWORD, CPlayer*, MapCompare<DWORD>>::iterator iter;
	iter = mPlayerInfo.begin();
	for (; iter != mPlayerInfo.end(); ++iter, --nCount)
	{//[����ʤ�����˽���][BY:ʮ���½���][QQ:112582793][2019-10-31][13:19:44]
		if (iter == mPlayerInfo.end()) break;
		DWORD dwGold = 0;
		int dwKill = iter->second->GetUnionWarKillTime();
		int dwDie = iter->second->GetUnionWarDieTimes();
		if (nCount >= 0)
		{//[����ǰnCount����ҽ�������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:19:55]
			if (dwKill - dwDie > 0)
			{
				dwGold = m_UnionSet.wBasicReward + (dwKill - dwDie) * 2;
			}
		}
		else //[����nCount����ҽ�������][BY:ʮ���½���][QQ:112582793][2019-10-31][13:20:22]
			dwGold = m_UnionSet.wBasicReward;

		//iter->second->SetGoldMoney();
		ItemShopUseLog(eLog_Gold_UnionWar_Get, iter->second->GetID(), iter->second->GetObjectName(),
			0, "��ս����", dwKill,
			dwDie, dwKill - dwDie, (dwKill - dwDie) * 2,
			iter->second->GetGoldMoney(), dwGold, iter->second->GetGoldMoney() + dwGold,
			0, 0, 0);
		CharacterUnionWarInfoUpdate(iter->second);
	}
	m_UnionWarUnionTable.SetPositionHead();
	while (pUnionInfo = m_UnionWarUnionTable.GetData())
	{//[����ʣ��������˽���][BY:ʮ���½���][QQ:112582793][2019-10-31][13:20:46]
		if (pUnionInfo)
		{
			SendLostReward(pUnionInfo);
		}
		pUnionInfo = NULL;
	}
}
void CUnionWarManager::EndUnionWar(BOOL bIsComplete)
{
	if (bIsComplete)
	{
		m_dwWarState = eUnionWarState_None;
		CalcReward();
	}
	else
		m_dwWarState = eUnionWarState_Pause;

	Release();
}

void CUnionWarManager::SendLostReward(sUnionWarInfo * pUnionInfo)
{//[���Ų������˽���][BY:ʮ���½���][QQ:112582793][2019-10-31][13:14:06]
	CPlayer * pPlayer = NULL;
	pUnionInfo->UnionWarPlayerTable.SetPositionHead();
	while (pPlayer = pUnionInfo->UnionWarPlayerTable.GetData())
	{
		if (pPlayer)
		{
			//pPlayer->SetGoldMoney();
			ItemShopUseLog(eLog_Gold_UnionWar_Get, pPlayer->GetID(), pPlayer->GetObjectName(),
				0, "��ս����", 0,
				0, 0, 0,
				pPlayer->GetGoldMoney(), m_UnionSet.wBasicReward, pPlayer->GetGoldMoney() + m_UnionSet.wBasicReward,
				0, 0, 0);
			CharacterUnionWarInfoUpdate(pPlayer);
		}
		pPlayer = NULL;
	}
}
BOOL CUnionWarManager::IsUnionWaring(CPlayer* pPlayer)
{//[�ж���ս�Ƿ�ʼ][BY:ʮ���½���][QQ:112582793][2019-10-23][10:22:50]
	if (m_dwWarState == eUnionWarState_Ing )
		return TRUE;
	else 
		return FALSE;
}
void CUnionWarManager::ClearPlayerNotEnough()
{//[��������������������][BY:ʮ���½���][QQ:112582793][2019-10-25][20:21:11]

	sUnionWarInfo * pInfo = NULL;
	m_UnionWarUnionTable.SetPositionHead();
	while (pInfo = m_UnionWarUnionTable.GetData())
	{
		if (pInfo)
		{
			if (pInfo->UnionWarPlayerTable.GetDataNum() < m_UnionSet.wMinPlayer)
			{
				CPlayer* pPlayer = NULL;
				pInfo->UnionWarPlayerTable.SetPositionHead();
				while (pPlayer = pInfo->UnionWarPlayerTable.GetData())
				{
					SaveMapChangePointUpdate(pPlayer->GetID(), m_UnionSet.wBackToMapKind);
					pPlayer->UpdateLogoutToDB();
					g_pServerSystem->RemovePlayer(pPlayer->GetID());

					pInfo->UnionWarPlayerTable.Remove(pPlayer->GetID());
					
					MSG_DWORD msg;

					SetProtocol(&msg, MP_UNIONWAR, MP_UNIONWAR_RETURNTOMAP_SYN);
					msg.dwData = m_UnionSet.wBackToMapNum;
					pPlayer->SendMsg(&msg, sizeof(msg));
				}
				m_UnionWarUnionTable.Remove(pInfo->dwUnionID);
			}
		}			
	}
}
void CUnionWarManager::AddPlayer(CPlayer* pPlayer)
{//[��ӽ�ɫ����ս][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:12]
	if (!IsUnionWarProcess())
		return;

	sUnionWarInfo* pInfo = NULL;
	pInfo = m_UnionWarUnionTable.GetData(pPlayer->GetGuildUnionIdx());
	if (NULL == pInfo)
	{
		if (m_UnionWarUnionTable.GetDataNum() < m_UnionSet.wMaxTeamNum)
		{
			CGuildUnion* pUnionInfo = NULL;
			pUnionInfo = GUILDUNIONMGR->GetUnionInfo(pPlayer->GetGuildUnionIdx());
			if (NULL == pUnionInfo)
			{
				return;
			}

			pInfo = new sUnionWarInfo(m_UnionSet.wMaxTeamPlayerNum);
			pInfo->UnionWarPlayerTable.Add(pPlayer, pPlayer->GetID());
			pInfo->dwUnionID = pPlayer->GetGuildUnionIdx();
			SafeStrCpy(pInfo->sUnionName, pUnionInfo->GetGuildUnionName(), MAX_GUILD_NAME + 1);
			m_UnionWarUnionTable.Add(pInfo, pInfo->dwUnionID);
			pPlayer->InitUnionWarKillTime();
			if (m_dwWarState < eUnionWarState_Pause)
			{
				SendUnionWarInfo(pPlayer);
				MSG_DWORD2 msg;
				msg.Category = MP_UNIONWAR;
				msg.Protocol = MP_UNIONWAR_PLAYERCOUNT_NOTIFY;
				msg.dwData1 = pInfo->dwUnionID;
				msg.dwData2 = pInfo->UnionWarPlayerTable.GetDataNum();
				g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
			}
		}
		else
		{//[������������][BY:ʮ���½���][QQ:112582793][2019-10-22][17:36:21]
			return;
		}
	}
	else if (pInfo->UnionWarPlayerTable.GetDataNum() < m_UnionSet.wMaxTeamPlayerNum)
	{
		pInfo->UnionWarPlayerTable.Add(pPlayer, pPlayer->GetID());
		//pPlayer->InitUnionWarKillTime();
		if (m_dwWarState < eUnionWarState_Pause)
		{
			SendUnionWarInfo(pPlayer);
			MSG_DWORD2 msg;
			msg.Category = MP_UNIONWAR;
			msg.Protocol = MP_UNIONWAR_PLAYERCOUNT_NOTIFY;
			msg.dwData1 = pInfo->dwUnionID;
			msg.dwData2 = pInfo->UnionWarPlayerTable.GetDataNum();
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
	}
	else
	{//[������������][BY:ʮ���½���][QQ:112582793][2019-10-22][17:37:46]

	}
}

void CUnionWarManager::RemovePlayer(CPlayer* pPlayer)
{//[��ɫ�˳���ս��ͼ���Ƴ���ɫ��Ϣ������ָ��ʧЧ��ɵ�ͼ����][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:24]
	if (!IsUnionWarProcess())
		return;
	sUnionWarInfo* pInfo = NULL;
	pInfo = m_UnionWarUnionTable.GetData(pPlayer->GetGuildUnionIdx());
	if (pInfo != NULL)
	{
		pInfo->UnionWarPlayerTable.Remove(pPlayer->GetID());
		if (m_dwWarState == eUnionWarState_Ing)
		{
			m_UnionLeavePlayer.Add(pPlayer, pPlayer->GetID());
			MSG_DWORD2 msg;
			msg.Category = MP_UNIONWAR;
			msg.Protocol = MP_UNIONWAR_PLAYERCOUNT_NOTIFY;
			msg.dwData1 = pInfo->dwUnionID;
			msg.dwData2 = pInfo->UnionWarPlayerTable.GetDataNum();
			g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
		}
		//[��ս��ʼ�����Ƴ�������ս��������][BY:ʮ���½���][QQ:112582793][2019-10-29][23:32:45]
		if (pInfo->UnionWarPlayerTable.GetDataNum() <= 0 && m_dwWarState != eUnionWarState_Ing)
			m_UnionWarUnionTable.Remove(pPlayer->GetGuildUnionIdx());
	}
}
//[������ʱ�ͷ�][BY:ʮ���½���][QQ:112582793][2019-10-31][20:43:41]
void CUnionWarManager::DiePlayerToLong(CPlayer* pPlayer)
{
	sUnionWarInfo* pDieInfo = NULL;
	pDieInfo = m_UnionWarUnionTable.GetData(pPlayer->GetGuildUnionIdx());
	if (pDieInfo)
	{
		if (pDieInfo->dwKillTimeCount > 0)
			pDieInfo->dwKillTimeCount -= 1;
		else
			pDieInfo->dwDieTimeCount += 1;

		pPlayer->SetUnionWarKillSub();
		MSG_UNIONWARPLAYERDIE msg;
		msg.Category = MP_UNIONWAR;
		msg.Protocol = MP_UNIONWAR_PLAYERDIETOLONG_NOTIFY;

		msg.dwUnionID = pDieInfo->dwUnionID;
		msg.dwPlayerID = pPlayer->GetID();

		msg.dwUnionKill = pDieInfo->dwKillTimeCount;
		msg.dwUnionDie = pDieInfo->dwDieTimeCount;
		msg.dwPlayerKill = pPlayer->GetUnionWarKillTime();

		SafeStrCpy(msg.UnionName, pDieInfo->sUnionName, MAX_GUILD_NAME + 1);
		SafeStrCpy(msg.PlayerName, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);

		PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
	}
}
void CUnionWarManager::DiePlayer(CPlayer* pDiePlayer, CPlayer* pAttacker)
{//[�������������Ϣ���ͻ���][BY:ʮ���½���][QQ:112582793][2019-10-23][10:23:56]
	if (!IsUnionWarProcess())
		return;
	sUnionWarInfo* pInfo = NULL;	//[��ɱ������Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-29][1:08:04]
	sUnionWarInfo* pDieInfo = NULL;	//[����������Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-29][1:08:11]
	pDieInfo = m_UnionWarUnionTable.GetData(pDiePlayer->GetGuildUnionIdx());
	pInfo   = m_UnionWarUnionTable.GetData(pAttacker->GetGuildUnionIdx());
	if (pInfo != NULL&&pDieInfo!=NULL)
	{
		pInfo->dwKillTimeCount += 1;		//[��ɱ��1][BY:ʮ���½���][QQ:112582793][2019-10-29][1:07:45]
		pDieInfo->dwDieTimeCount += 1;		//[������1][BY:ʮ���½���][QQ:112582793][2019-10-29][1:07:53]

		pAttacker->SetUnionWarKillTimeAdd();	//[��ɱ��һ�ɱ+1][BY:ʮ���½���][QQ:112582793][2019-10-29][1:08:40]
		pDiePlayer->SetUnionWarDieTimeAdd();	//[�����������+1][BY:ʮ���½���][QQ:112582793][2019-10-29][1:08:48]
		pDiePlayer->SetDieInUnionWarMap(TRUE);

		//[����ɱ��Ϣ�㲥����ǰ��ͼ���������][BY:ʮ���½���][QQ:112582793][2019-10-22][18:32:42]
		MSG_UNIONWARPLAYERKILLINFO msg;
		msg.Category = MP_UNIONWAR;
		msg.Protocol = MP_UNIONWAR_KILL_NOTIFY;
		//[��ɱ��Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-29][1:09:04]
		msg.dwObjectID = pAttacker->GetID();
		msg.dwKillUnionID = pAttacker->GetGuildUnionIdx();
		msg.dwUnionKillTimes = pInfo->dwKillTimeCount;
		msg.dwPlayerKillTimes = pAttacker->GetUnionWarKillTime();
		//[������Ϣ][BY:ʮ���½���][QQ:112582793][2019-10-29][1:09:10]
		msg.dwDiePlayerID = pDiePlayer->GetID();
		msg.dwDieUnionID = pDiePlayer->GetGuildUnionIdx();
		msg.dwUnionDieTimes = pDieInfo->dwDieTimeCount;
		msg.dwPlayerDieTimes = pDiePlayer->GetUnionWarDieTimes();

		PACKEDDATA_OBJ->QuickSend(pAttacker, &msg, sizeof(msg));
	}
}
WORD CUnionWarManager::IsCanMoveIn(DWORD dwPlayerID)
{
	if (m_dwWarState < eUnionWarState_Befor10)
	{//[��ս��ʼǰ��Сʱ��ǰ20���ӿ��Խ���][BY:ʮ���½���][QQ:112582793][2019-10-23][14:57:17]
		return eUnionMoveIn_Success;
	}
	if (m_dwWarState == eUnionWarState_Ing)
	{//[ս����ʼ���ж�֮ǰ�Ƿ��������������ͨ����û��������������][BY:ʮ���½���][QQ:112582793][2019-10-23][15:03:56]
		CPlayer* pInfo = m_UnionLeavePlayer.GetData(dwPlayerID);
		if (pInfo != NULL)
		{
			m_UnionLeavePlayer.Remove(dwPlayerID);
			return eUnionMoveIn_Success;
		}
		else
			return eUnionMoveIn_Enough;
	}
	else
		return eUnionMoveIn_NotTime;
}
WORD CUnionWarManager::IsCanMoveIn(CPlayer* pPlayer)
{//[�ж��Ƿ�ɽ�����ս��ͼ][BY:ʮ���½���][QQ:112582793][2019-10-23][12:36:31]
	DWORD dwUnionID = pPlayer->GetGuildUnionIdx();
	WORD wLevel = pPlayer->GetLevel();
	WORD wReSet = pPlayer->GetReSet();
	if (dwUnionID == 0 || wLevel < 90 || wReSet < g_pServerSystem->GetGameConfig().MaxReSetTime)
		return eUnionMoveIn_Substandard;
	else
		return eUnionMoveIn_Success;
}
void CUnionWarManager::SendUnionWarInfoToAllPlayer()
{//[������ս��Ϣ�����в������][BY:ʮ���½���][QQ:112582793][2019-10-28][16:58:33]
	WORD wUnionCount = m_UnionWarUnionTable.GetDataNum();
	if (wUnionCount <= 1)
	{
		EndUnionWar(FALSE);
		return;
	}

	WORD wCount = 0;
	
	sUnionWarInfo* pInfo = NULL;
	m_UnionWarUnionTable.SetPositionHead();

	while (pInfo = m_UnionWarUnionTable.GetData())
	{
		if (pInfo)
		{
			UnionInfo[wCount].dwUnionID = pInfo->dwUnionID;
			SafeStrCpy(UnionInfo[wCount].sUnionName, pInfo->sUnionName, MAX_GUILD_NAME + 1);
			UnionInfo[wCount].wUnionKillCount = pInfo->dwKillTimeCount;
			UnionInfo[wCount].wUnionDieCount = pInfo->dwDieTimeCount;
			UnionInfo[wCount].wUnionPlayerCount = pInfo->UnionWarPlayerTable.GetDataNum();
		}
		pInfo = NULL;
		wCount++;
		if (wCount >= 10)
			break;
	}
	pInfo = NULL;
	m_UnionWarUnionTable.SetPositionHead();
	while (pInfo = m_UnionWarUnionTable.GetData())
	{
		if (pInfo)
			SendUnionWarInfo(pInfo);
		pInfo = NULL;
	}
}
void CUnionWarManager::SendUnionWarInfo(CPlayer* pPlayer)
{
	WORD wUnionCount = m_UnionWarUnionTable.GetDataNum();
	WORD wCount = 0;

	sUnionWarInfo* pInfo = NULL;
	m_UnionWarUnionTable.SetPositionHead();

	while (pInfo = m_UnionWarUnionTable.GetData())
	{
		if (pInfo)
		{
			UnionInfo[wCount].dwUnionID = pInfo->dwUnionID;
			SafeStrCpy(UnionInfo[wCount].sUnionName, pInfo->sUnionName, MAX_GUILD_NAME + 1);
			UnionInfo[wCount].wUnionKillCount = pInfo->dwKillTimeCount;
			UnionInfo[wCount].wUnionDieCount = pInfo->dwDieTimeCount;
			UnionInfo[wCount].wUnionPlayerCount = pInfo->UnionWarPlayerTable.GetDataNum();
		}
		pInfo = NULL;
		wCount++;
		if (wCount >= 10)
			break;
	}
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	int  nSec = GetSecondInterval(curTime, m_UnionWarTimeInfo[curTime.wDayOfWeek].UnionWarStartTime);
	MSG_UNIONWARINFO msg;
	msg.Category = MP_UNIONWAR;
	msg.Protocol = MP_UNIONWAR_WARINFO_NOTIFY;
	if (m_dwWarState == eUnionWarState_Ing)
	{
		msg.dwUnionWarTime = m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec;
	}
	else
	{
		msg.dwUnionWarTime = nSec;
	}
	memcpy(msg.sUnionInfo, UnionInfo, sizeof(msg.sUnionInfo));
	msg.dwPlayerKillTimes = pPlayer->GetUnionWarKillTime();
	msg.dwPlayerDieTimes = pPlayer->GetUnionWarDieTimes();

	msg.eWarState = m_dwWarState;
	pPlayer->SendMsg(&msg, sizeof(msg));
	if (m_dwWarState == eUnionWarState_Ing)
	{
		if (pPlayer->PKModeOn() == ePKCODE_OK)
		{//[ǿ�ƿ���][BY:ʮ���½���][QQ:112582793][2019-10-29][14:34:49]
			MSG_DWORD msg1;
			msg1.Category = MP_PK;
			msg1.Protocol = MP_PK_PKON_ACK;
			msg1.dwObjectID = pPlayer->GetID();
			msg1.dwData = msg1.dwObjectID;
			pPlayer->SendMsg(&msg1, sizeof(msg1));
			//PACKEDDATA_OBJ->QuickSend(pPlayer, &msg1, sizeof(msg1));
		}
	}

}
void CUnionWarManager::SendUnionWarInfo(sUnionWarInfo* pInfo)
{
	CPlayer * pPlayer = NULL;
	pInfo->UnionWarPlayerTable.SetPositionHead();
	while (pPlayer = pInfo->UnionWarPlayerTable.GetData())
	{
		if (pPlayer)
		{
			SYSTEMTIME curTime;
			GetLocalTime(&curTime);
			int nSec = GetSecondInterval(curTime, m_UnionWarTimeInfo[curTime.wDayOfWeek].UnionWarStartTime);
			MSG_UNIONWARINFO msg;
			msg.Category = MP_UNIONWAR;
			msg.Protocol = MP_UNIONWAR_WARINFO_NOTIFY;
			if (m_dwWarState == eUnionWarState_Ing)
			{
				msg.dwUnionWarTime = m_UnionWarTimeInfo[curTime.wDayOfWeek].dwWarTime + nSec;
			}	
			else
			{
				msg.dwUnionWarTime = nSec;
			}		

			memcpy(msg.sUnionInfo, UnionInfo, sizeof(msg.sUnionInfo));
			msg.dwPlayerKillTimes = pPlayer->GetUnionWarKillTime();
			msg.dwPlayerDieTimes = pPlayer->GetUnionWarDieTimes();
			
			msg.eWarState = m_dwWarState;
			pPlayer->SendMsg(&msg, sizeof(msg));

			if (m_dwWarState == eUnionWarState_Ing)
			{
				if (pPlayer->PKModeOn() == ePKCODE_OK)
				{//[ǿ�ƿ���][BY:ʮ���½���][QQ:112582793][2019-10-29][14:34:49]
					MSG_DWORD msg1;
					msg1.Category = MP_PK;
					msg1.Protocol = MP_PK_PKON_ACK;
					msg1.dwObjectID = pPlayer->GetID();
					msg1.dwData = msg1.dwObjectID;

					PACKEDDATA_OBJ->QuickSend(pPlayer, &msg1, sizeof(msg1));
				}
			}

		}
		pPlayer = NULL;
	}
}


