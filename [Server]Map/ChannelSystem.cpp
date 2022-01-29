// ChannelSystem.cpp: implementation of the CChannelSystem class.
#include "stdafx.h"
#include "ChannelSystem.h"
#include "MHFile.h"
#include "BattleSystem_Server.h"
#include "Object.h"
#include "NetWork.h"
#include "EventMapMgr.h"
#include "QuestMapMgr.h"
#include "../UnionWarManager.h"//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-25][12:54:49]
extern int g_nServerSetNum;

CChannelSystem::CChannelSystem()
{
	m_dwCount = 0;
	m_bQuestMap = FALSE;
	memset( m_QMChannel, 0, sizeof(CChannel)*1000 );
	memset( m_Channel, 0, sizeof(CChannel*)*MAX_CHANNEL_NUM );
}

CChannelSystem::~CChannelSystem()
{
	for(DWORD i=0; i<m_dwCount; ++i)
	{
		if(m_Channel[i])
		{		
			delete m_Channel[i];
			m_Channel[i] = NULL;
		}
	}
}

void CChannelSystem::LoadChannelInfo()
{
	CMHFile file;
	BYTE Kind, num = 0;
	char value[64] = {0,};
	WORD wMapNum = 0;

	
	char filename[256];
	sprintf(filename,"serverset/%d/ChannelInfo.bin",g_nServerSetNum);

	file.Init(filename, "rb");

	if(file.IsInited() == FALSE)
	{
		MessageBox(NULL,"Can't Open ChannelInfo File", NULL, NULL);
		return;
	}

	char buf[1024];
	file.GetLine(buf, 1024);	
	CHANNEL_INFO defaultInfo;
	defaultInfo.RegenRate = file.GetFloat();
	defaultInfo.DropRate = file.GetFloat();
	defaultInfo.ExpRate = file.GetFloat();
	defaultInfo.AbilityExpRate = file.GetFloat();
	defaultInfo.MoneyRate  =file.GetFloat();
	defaultInfo.bBattleChannel = file.GetBool();
	defaultInfo.bPeaceChannel = file.GetBool();

	file.GetLine(buf, 1024);

	char temp[512] = {0,};
	while(!file.IsEOF())
	{
		file.GetString(temp);

		if( '#' == temp[0] )
		{
			if( atoi(temp+1) == g_pServerSystem->GetMapNum() )
			{
				Kind = file.GetByte();
				num = file.GetByte();
				CreateChannel(Kind, num);

				for( int i = 0; i < num; ++i)
				{
					m_Channel[i]->m_ChannelInfo.RegenRate = defaultInfo.RegenRate;
					m_Channel[i]->m_ChannelInfo.DropRate = defaultInfo.DropRate;
					m_Channel[i]->m_ChannelInfo.ExpRate = defaultInfo.ExpRate;
					m_Channel[i]->m_ChannelInfo.AbilityExpRate = defaultInfo.AbilityExpRate;
					m_Channel[i]->m_ChannelInfo.MoneyRate = defaultInfo.MoneyRate;
					m_Channel[i]->m_ChannelInfo.bBattleChannel = defaultInfo.bBattleChannel;
					m_Channel[i]->m_ChannelInfo.bPeaceChannel = defaultInfo.bPeaceChannel;
				}

				DWORD ChannelCnt = 0;
				DWORD ChannelIDX = 0;
				while(!file.IsLineEnd())
				{
					++ChannelCnt;

					ChannelIDX = file.GetDword();

					if( m_dwCount >= ChannelIDX && ChannelCnt == ChannelIDX )
					{
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.RegenRate = file.GetFloat();
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.DropRate = file.GetFloat();
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.ExpRate = file.GetFloat();
						// [增加线路经验、技能点、游戏币倍率] [by:十里坡剑神][QQ:112582793][2018/11/22]
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.AbilityExpRate = file.GetFloat();
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.MoneyRate = file.GetFloat();
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.bBattleChannel = file.GetBool();
						//[增加安全线配置][By:十里坡剑神][QQ:112582793][2018/3/13]
						m_Channel[ChannelIDX - 1]->m_ChannelInfo.bPeaceChannel = file.GetBool();
					}
				}
				break;
			}
		}
	}
	file.Release();

	if(m_dwCount == 0)
		CreateChannel(0, 1);
}

void CChannelSystem::CreateChannel(BYTE Kind, BYTE num)
{
	for(BYTE i=0; i<num; ++i)
	{			
		 DWORD ChannelIDX = BATTLESYSTEM->CreateChannel(); 

		 m_Channel[i] = new CChannel;
		 m_Channel[i]->SetChannelID(ChannelIDX);
	} 
	m_dwCount = num;
	m_bQuestMap = FALSE;
}

void CChannelSystem::IncreasePlayerNum(DWORD ChannelID)
{
	if( m_bQuestMap )	return;

	ASSERT(ChannelID);
	if(ChannelID > m_dwCount)
	{
		ASSERT(0); return;
	}
	m_Channel[ChannelID-1]->IncreaseNum();
}

void CChannelSystem::DecreasePlayerNum(DWORD ChannelID)
{
	if( m_bQuestMap )	return;

	ASSERT(ChannelID);
	if(ChannelID > m_dwCount)
	{
		return;
	}
	m_Channel[ChannelID-1]->DecreaseNum();
}


void CChannelSystem::SendChannelInfo(MSG_DWORD3* pInfo, DWORD dwConnectionIndex)
{
	if(g_pServerSystem->GetStart() == FALSE || m_bQuestMap )
	{
		MSG_BYTE nmsg;
		nmsg.Category = MP_USERCONN;
		nmsg.Protocol = MP_USERCONN_CHANNELINFO_NACK;
		nmsg.bData = 0;
		nmsg.dwObjectID = pInfo->dwObjectID;
		g_Network.Send2Server(dwConnectionIndex,(char*)&nmsg,sizeof(nmsg));
		return;
	}
	if (UNIONWARMGR->IsUnionWarMap())
	{//[盟战进入条件检查][BY:十里坡剑神][QQ:112582793][2019-10-25][12:55:46]
		WORD wRet = UNIONWARMGR->IsCanMoveIn(pInfo->dwData3);
		if (wRet!=0)
		{
			MSG_BYTE nmsg;
			nmsg.Category = MP_UNIONWAR;
			nmsg.Protocol = MP_UNIONWAR_MOVEIN_NACK;
			nmsg.bData = wRet;
			nmsg.dwObjectID = pInfo->dwData3;
			g_Network.Send2Server(dwConnectionIndex, (char*)&nmsg, sizeof(nmsg));
			return;
		}
	}
	MSG_CHANNEL_INFO msg;
	memset(&msg, 0, sizeof(MSG_CHANNEL_INFO));
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_ACK;
//#ifndef _KOR_LOCAL_
//	SafeStrCpy(msg.ChannelName, m_ChannelName, MAX_CHANNEL_NAME+1);
//#endif
	for(DWORD i=0; i<m_dwCount; ++i)
	{
		msg.PlayerNum[i] = m_Channel[i]->GetPlayerNum();
		msg.bBattleChannel[i] = m_Channel[i]->IsBattleChannel();
		msg.bIsPeaceChannel[i] = m_Channel[i]->IsPeaceChannel();
	}
	msg.dwObjectID = pInfo->dwObjectID;
	msg.dwUniqueIDinAgent = pInfo->dwData1;
	msg.Count = (BYTE)m_dwCount;

	g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
}

DWORD CChannelSystem::GetChannelID(int num)
{
	if( m_bQuestMap )	return 0;

	if(((DWORD)num >= m_dwCount) || (num < 0))
	{
		ASSERT(0);
		return 0;
	}
	return m_Channel[num]->GetChannelID();
}

void CChannelSystem::InitQuestMapChannel()
{
	m_bQuestMap = TRUE;
}

DWORD CChannelSystem::CreateQuestMapChannel() 
{
	if( !m_bQuestMap )	return 0;

	DWORD ChannelIDX = BATTLESYSTEM->CreateChannel();
	int i = 0;
	for(i = 0; i < 1000; ++i )
	{
		if( m_QMChannel[i].GetChannelID() == 0 )
			break;
	}
	if( i > 999 )	return 0;

	m_QMChannel[i].SetChannelID( ChannelIDX );
	++m_dwCount;
	return ChannelIDX;
}

void CChannelSystem::DestroyQuestMapChannel( DWORD dwChannel )
{
	if( !m_bQuestMap )	return;

	for( int i = 0; i < 1000; ++i )
	{
		if( m_QMChannel[i].GetChannelID() == dwChannel )
		{
			m_QMChannel[i].SetChannelID( 0 );
			break;
		}
	}
	--m_dwCount;

	BATTLESYSTEM->DestroyChannel( dwChannel );	
}

void CChannelSystem::Init( WORD wMapNum )
{
	if( QUESTMAPMGR->Init( wMapNum ) )	return;
	if( wMapNum == EVENTMAPNUM )
	{
		EVENTMAPMGR->Init( wMapNum, 10 );
	}
	else
	{
		LoadChannelInfo();
	}
}

DWORD CChannelSystem::GetRandomChannelID()
{
	if( m_bQuestMap )	return 0;

	int num = 0;
	if( m_dwCount > 1 )
		num = random(0, GetChannelCount()-1);
	return m_Channel[num]->GetChannelID();
}

DWORD CChannelSystem::GetLastChannelID()
{
	if( m_bQuestMap )	return 0;

	return m_Channel[m_dwCount-1]->GetChannelID();
}
void CChannelSystem::SendChannelInfoToMS( DWORD dwConnectionIndex )
{
	if( m_bQuestMap )	return;

	MSG_CHANNEL_INFO msg;
	memset(&msg, 0, sizeof(MSG_CHANNEL_INFO));
	msg.Category = MP_MORNITORMAPSERVER;
	msg.Protocol = MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_ACK;
//#ifndef _KOR_LOCAL_
//	SafeStrCpy(msg.ChannelName, m_ChannelName, MAX_CHANNEL_NAME+1);
//#endif
	for(DWORD i=0; i<m_dwCount; ++i)
	{
		msg.PlayerNum[i] = m_Channel[i]->GetPlayerNum();
	}
	msg.Count = (BYTE)m_dwCount;
	g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
}
void CChannelSystem::SendMapChangeChannelInfo(MSG_DWORD4* pInfo, DWORD dwConnectionIndex)
{
	if(g_pServerSystem->GetStart() == FALSE || m_bQuestMap )
	{
		MSG_BYTE nmsg;
		nmsg.Category = MP_USERCONN;
		nmsg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK;
		nmsg.bData = 0;
		nmsg.dwObjectID = pInfo->dwObjectID;
		g_Network.Send2Server(dwConnectionIndex,(char*)&nmsg,sizeof(nmsg));
		return;
	}
	if (UNIONWARMGR->IsUnionWarMap())
	{//[盟战进入条件检查][BY:十里坡剑神][QQ:112582793][2019-10-25][12:55:46]
		WORD wRet = 0;
		wRet = UNIONWARMGR->IsCanMoveIn(pInfo->dwData4);
		if (wRet!=0)
		{
			MSG_BYTE nmsg;
			nmsg.Category = MP_UNIONWAR;
			nmsg.Protocol = MP_UNIONWAR_MOVEIN_NACK;
			nmsg.bData = wRet;
			nmsg.dwObjectID = pInfo->dwData4;
			g_Network.Send2Server(dwConnectionIndex, (char*)&nmsg, sizeof(nmsg));
			return;
		}
	}
	MSG_CHANNEL_INFO msg;
	memset(&msg, 0, sizeof(MSG_CHANNEL_INFO));
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK;

	for(DWORD i=0; i<m_dwCount; ++i)
	{
		msg.PlayerNum[i] = m_Channel[i]->GetPlayerNum();
		msg.bBattleChannel[i] = m_Channel[i]->IsBattleChannel();
		msg.bIsPeaceChannel[i] = m_Channel[i]->IsPeaceChannel();
	}
	msg.dwObjectID = pInfo->dwObjectID;
	msg.dwUniqueIDinAgent = pInfo->dwData1;
	msg.Count = (BYTE)m_dwCount;
	msg.wMoveMapNum = pInfo->dwData2;
	msg.dwChangeMapState = pInfo->dwData3;
	g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
}

float CChannelSystem::GetRegenRate( DWORD ChannelID )
{
	if( m_bQuestMap )	return 1.0f;

	if( !ChannelID || ChannelID > m_dwCount )
	{
		ASSERT(0);

		return 1.0f;
	}

	if( m_Channel[ChannelID - 1] == NULL )
		return 1.0f;

	return m_Channel[ChannelID - 1]->GetRegenRate();
}

float CChannelSystem::GetDropRate( DWORD ChannelID )
{
	if( m_bQuestMap )	return 1.0f;

	if( !ChannelID || ChannelID > m_dwCount )
	{
		ASSERT(0);
		return 1.0f;
	}

	if( m_Channel[ChannelID - 1] == NULL )
		return 1.0f;

	return m_Channel[ChannelID - 1]->GetDropRate();
}

BOOL CChannelSystem::IsBattleChannel( DWORD ChannelID )
{
	if( m_bQuestMap )	return FALSE;

	if( !ChannelID || ChannelID > m_dwCount )
	{
		ASSERT(0);
		return FALSE;
	}

	if( m_Channel[ChannelID - 1] == NULL )
		return FALSE;

	return m_Channel[ChannelID - 1]->IsBattleChannel();
}

void CChannelSystem::GetChannelInfo( MSG_CHANNEL_INFO* pInfo )
{
	if( m_bQuestMap )	return;

	for( DWORD i = 0; i < m_dwCount; ++i )
	{
		pInfo->bBattleChannel[i] = m_Channel[i]->IsBattleChannel();
		//[增加安全线路配置发送到客户端][By:十里坡剑神][QQ:112582793][2018/3/13]
		pInfo->bIsPeaceChannel[i] = m_Channel[i]->IsPeaceChannel();
	}
	pInfo->Count = m_dwCount;
}


