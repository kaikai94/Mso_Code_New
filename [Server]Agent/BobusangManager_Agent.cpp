




#include "stdafx.h"
#include ".\bobusangmanager_agent.h"
#include "ServerSystem.h"
#include "Network.h"
#include "MHTimeManager.h"
#include "MHFile.h"
#include "AgentDBMsgParser.h"
#include "ServerTable.h"

GLOBALTON(BobusangManager_Agent)

BobusangManager_Agent::BobusangManager_Agent(void)
{
	m_bManager				=	FALSE;
	m_bOnProcessing			=	TRUE;
	m_dwBobusangCheckTime	=	0;
	m_piAppearedState		=	NULL;
	m_nChannelTotalNum		=	0;

	m_dwAppearTimeMin		=	0;
	m_dwAppearTimeMax		=	0;
	m_dwDurationTimeMin		=	0;
	m_dwDurationTimeMax		=	0;

	//memset( &m_PosPerMap, 0, sizeof(BobusangPosPerMap) * MAX_MAP_ENUMCOUNT );
	m_mapInfo.Initialize( 64 );
	
	m_pBobusangInfo		=	NULL;
}

BobusangManager_Agent::~BobusangManager_Agent(void)
{
	BobusangMgr_Release();

	m_mapInfo.SetPositionHead();
	BOBUSANG_MAPINFO* pInfo = NULL;
	while( pInfo = m_mapInfo.GetData() )
	{
		SAFE_DELETE(pInfo);
	}
	m_mapInfo.RemoveAll();
}

void BobusangManager_Agent::CheckManager()
{
	if( g_pServerSystem->GetServerNum() == 0 )
		m_bManager = TRUE;
	else m_bManager = FALSE;
}

void BobusangManager_Agent::BobusangMgr_Start()
{
	CheckManager();
}

void BobusangManager_Agent::BobusangMgr_Init()
{
	LoadChannelInfo();

	LoadBobusangInfo();
}

void BobusangManager_Agent::BobusangMgr_Process()
{
	if( !m_bManager )	return;
	if( gCurTime > m_dwBobusangCheckTime + BOBUSANG_CHECKTIME )
	{
		int n = m_nChannelTotalNum;
		if(n < 1)	return;

		stTIME curTime;
		curTime.SetTime(GetCurTime());

		BOBUSANGINFO* pInfo = NULL;

		while( n-- )
		{

			if( eBBSAS_APPEAR == m_piAppearedState[n] )
			{
				pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_CUR ];
				
				if(!pInfo)	continue;

				static stTIME notifyFromTime, tmp;
				notifyFromTime.SetTime(0,0,0,0,1,0);
				tmp.SetTime(pInfo->DisappearanceTime);
				tmp -= notifyFromTime;

				if( curTime.value > tmp.value )
				{
					SendBobusangLeaveNotifyToMap(pInfo);
				}
				
				if( curTime.value > pInfo->DisappearanceTime )
				{
					SendBobusangLeaveMsgToMap(pInfo);
					m_piAppearedState[n] = eBBSAS_DISAPPEAR_DELAYED;
				}
			}
			else if( eBBSAS_DISAPPEAR == m_piAppearedState[n] )
			{
				pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_NEXT ];
				
				if(!pInfo)	continue;
				
				if( curTime.value > pInfo->AppearanceTime )
				{
					if(FALSE == m_bOnProcessing)
						return;

					SendBobusangInfoMsgToMap(pInfo);
					m_piAppearedState[n] = eBBSAS_APPEAR_DELAYED;
				}
			}
			else if( eBBSAS_APPEAR_DELAYED == m_piAppearedState[n] 
			|| eBBSAS_DISAPPEAR_DELAYED == m_piAppearedState[n] )
			{
				pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_NEXT ];

				if(!pInfo)	continue;

				stTIME elapsedTime, tmp;
				elapsedTime.SetTime(pInfo->AppearanceTime);
				tmp.SetTime(0,0,0,0,m_dwAppearTimeMax,0);
				elapsedTime += tmp;

				if( curTime.value > elapsedTime.value )
				{
					SetBobusangInfo(n);
				}
			}
			else if( eBBSAS_NONE == m_piAppearedState[n] )
			{
				pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_NEXT ];

				if(!pInfo)	continue;

				SetBobusangInfo(n);
			}
		}

		m_dwBobusangCheckTime = gCurTime;
	}
}

void BobusangManager_Agent::BobusangMgr_Release()
{
	//SAFE_DELETE(m_piAppearedState);
	if(m_piAppearedState)
	{
		delete m_piAppearedState;
		m_piAppearedState = NULL;
	}
	//SAFE_DELETE(m_pBobusangInfo);
	if(m_pBobusangInfo)
	{
		delete [] m_pBobusangInfo;
		m_pBobusangInfo = NULL;
	}
}

BOOL BobusangManager_Agent::LoadChannelInfo()
{
	//CMHFile file;

	//DWORD	serverSetNum = 0;
	//if( FALSE == file.Init("serverset.txt","rt") )
	//{
	//	serverSetNum = 1;
	//}
	//else
	//{
	//	serverSetNum = file.GetDword();
	//}
	//file.Release();

	//char filePath[256] = {0,};
	//sprintf(filePath, "serverset/%d/ChannelInfo.bin",serverSetNum);

	//if( FALSE == file.Init(filePath, "rb") )
	//{
	//	MessageBox(NULL,"Can't Open ChannelInfo.bin", NULL, NULL);
	//	return FALSE;
	//}

	//while(FALSE == file.IsEOF())
	//{
	//	WORD wMapNum = file.GetWord();
	//	WORD wKind = file.GetWord();
 //       DWORD dwMaxChannelNum = file.GetDword();
	//	
	//	if(dwMaxChannelNum > m_nChannelTotalNum)
	//		m_nChannelTotalNum = dwMaxChannelNum;
	//}


	//if(m_nChannelTotalNum)
	//{
		m_nChannelTotalNum = 1;
		m_piAppearedState	= new int;
		//for(DWORD i = 0; i < m_nChannelTotalNum; ++i)
			*m_piAppearedState = eBBSAS_NONE;


		m_pBobusangInfo		= new BOBUSANGINFO[m_nChannelTotalNum * eBBSIT_KINDNUM];
	//}

	//file.Release();

	return TRUE;
}
void BobusangManager_Agent::RloadBobusangInfo()
{
	m_mapInfo.SetPositionHead();
	BOBUSANG_MAPINFO* pInfo = NULL;
	while( pInfo = m_mapInfo.GetData() )
	{
		SAFE_DELETE(pInfo);
	}
	m_mapInfo.RemoveAll();
	LoadBobusangInfo();
}
BOOL BobusangManager_Agent::LoadBobusangInfo()
{
	CMHFile file;

	if( FALSE == file.Init("resource/Server/BobusangInfo.bin", "rb") )
	{

		MessageBox(NULL,"Can't Open BobusangInfo File", NULL, NULL);
		return FALSE;
	}

	char paserTmp[256] = {0,};

	file.GetString();	
	m_dwAppearTimeMin = file.GetDword();
	m_dwAppearTimeMax = file.GetDword();
	
	file.GetString();	
	m_dwDurationTimeMin = file.GetDword();
	m_dwDurationTimeMax = file.GetDword();

	int PosNum, MapCount;
	char* pMapNum = NULL;
	MapCount = 0;
    
	BOBUSANG_MAPINFO* pInfo = NULL;

	while( FALSE == file.IsEOF() )
	{
		file.GetString(paserTmp);

		if( 0 > MapCount|| MAX_MAP_ENUMCOUNT < MapCount )
		{
			return FALSE;
		}

		if( '#' == paserTmp[0] )
		{
			pInfo = new BOBUSANG_MAPINFO;

			pMapNum = paserTmp;
			++pMapNum;
			

			pInfo->dataIdx = MapCount;
			pInfo->mapNum = atoi(pMapNum);
			pInfo->strMap = file.GetString();
			PosNum = 0;
			while( PosNum < BOBUSANG_POSNUM_MAX )
			{
				pInfo->Pos[PosNum].ApprPos.x	= file.GetFloat();
				pInfo->Pos[PosNum].ApprPos.z	= file.GetFloat();
				pInfo->Pos[PosNum].ApprDir		= file.GetFloat();

				++PosNum;
			}
			++MapCount;
			if(!m_mapInfo.GetData(pInfo->dataIdx))
				m_mapInfo.Add(pInfo,pInfo->dataIdx);
		}
	}

	if(m_bManager)
		InitBobusangInfo();

	return TRUE;

}

void BobusangManager_Agent::InitBobusangInfo()
{
	for( DWORD i = 0; i < m_nChannelTotalNum; ++i )
	{
		SetBobusangInfo(i);
	}
}

void BobusangManager_Agent::SetBobusangInfo( DWORD iChannel )
{
	BOBUSANGINFO* pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*iChannel + eBBSIT_NEXT ];
	BOBUSANGINFO* pBeforeInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*iChannel + eBBSIT_CUR ];

	pInfo->AppearanceChannel = iChannel;

	DWORD totalMapcount = m_mapInfo.GetDataNum();
	DWORD rndIdx = rand() % totalMapcount;


	BOBUSANG_MAPINFO* pRndInfo = m_mapInfo.GetData(rndIdx);

	if(!pRndInfo)
	{
		ASSERT(0);
		return;
	}

	SERVERINFO* pServerInfo = g_pServerTable->FindMapServer((WORD)pRndInfo->mapNum);
	if(pServerInfo)
	{
		pInfo->AppearanceMapNum = pRndInfo->mapNum;
	}
	else
	{
		m_piAppearedState[iChannel] = eBBSAS_NONE;
		return;
	}
	m_wMapIndex = rndIdx;
	pInfo->strMap = pRndInfo->strMap;
	DWORD rndMin = m_dwAppearTimeMin + rand()%(m_dwAppearTimeMax - m_dwAppearTimeMin);
	stTIME startTime, gapTime;
	
	gapTime = *(SetstTIMEfromMinValue(rndMin));

	if( pBeforeInfo->DisappearanceTime && pBeforeInfo->DisappearanceTime > GetCurTime() )	
	{
		startTime.SetTime(pBeforeInfo->DisappearanceTime);
	}
	else	
	{
		startTime.SetTime(GetCurTime());
	}
	startTime += gapTime;

	pInfo->AppearanceTime = startTime.value;

	rndMin = m_dwDurationTimeMin + rand()%(m_dwDurationTimeMax - m_dwDurationTimeMin);

	gapTime = *(SetstTIMEfromMinValue(rndMin));

	startTime += gapTime;
	pInfo->DisappearanceTime = startTime.value;
	pInfo->AppearancePosIdx = rand()% BOBUSANG_POSNUM_MAX;

	pInfo->SellingListIndex = rand()% DEALITEM_BIN_TABNUM;	

	if( eBBSAS_APPEAR !=m_piAppearedState[iChannel] )
		m_piAppearedState[iChannel] = eBBSAS_DISAPPEAR;

}
#include "Console.h"
void BobusangManager_Agent::OutPutBobusangInfo()
{//[包袱商人刷新日志][BY:十里坡剑神][QQ:112582793][2019-4-17][18:54:25]
	BOBUSANGINFO* pInfo = &m_pBobusangInfo[1];
	BOBUSANG_MAPINFO* pRndInfo = m_mapInfo.GetData(m_wMapIndex);
	if(!pInfo||!pRndInfo) return;
	stTIME StartTime;
	StartTime.SetTime(pInfo->AppearanceTime);
	g_Console.LOGEX(4,RGB(0,255,0),"*******************包袱商人刷新信息*******************");
	g_Console.LOGEX(4,RGB(0,255,0),"刷新地图[%s]",pInfo->strMap.c_str());
	g_Console.LOGEX(4,RGB(0,255,0),"刷新坐标[%d,%d]",(DWORD)(pRndInfo->Pos[pInfo->AppearancePosIdx].ApprPos.x/100),(DWORD)(pRndInfo->Pos[pInfo->AppearancePosIdx].ApprPos.z/100));
	g_Console.LOGEX(4,RGB(0,255,0),"刷新时间[%d-%d-%d %d:%d:%d]",StartTime.GetYear(),StartTime.GetMonth(),StartTime.GetDay(),StartTime.GetHour(),StartTime.GetMinute(),StartTime.GetSecond());
	g_Console.LOGEX(4,RGB(0,255,0),"出售列表编号[%d]",pInfo->SellingListIndex);
	g_Console.LOGEX(4,RGB(0,255,0),"******************************************************");
}
void BobusangManager_Agent::SendBobusangInfoMsgToMap( BOBUSANGINFO* pInfo )
{
	SERVERINFO* pServerInfo = g_pServerTable->FindMapServer( WORD(pInfo->AppearanceMapNum) );

	if( !pServerInfo )	return;

	if(0 == pServerInfo->dwConnectionIndex)	
	{
		return;
	}

	MSG_BOBUSANG_INFO msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_INFO_AGENT_TO_MAP;
	msg.bobusangInfo = *pInfo;

	g_Network.Send2Server( pServerInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

	int n = pInfo->AppearanceChannel;

	BOBUSANGINFO* pBobusangInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM * n + eBBSIT_CUR ];
	*pBobusangInfo = *pInfo;
	OutPutBobusangInfo();
	SetBobusangInfo( n );

}

void BobusangManager_Agent::SendBobusangLeaveMsgToMap( BOBUSANGINFO* pInfo )
{
	SERVERINFO* pServerInfo = g_pServerTable->FindMapServer( WORD(pInfo->AppearanceMapNum) );

	if( !pServerInfo )	return;

	if(0 == pServerInfo->dwConnectionIndex)	
	{
		return;
	}

	MSG_DWORD2	msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_DISAPPEAR_AGENT_TO_MAP;
	msg.dwData1	= pInfo->AppearanceChannel;
	msg.dwData2 = pInfo->DisappearanceTime;

	g_Network.Send2Server( pServerInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}

void BobusangManager_Agent::SendBobusangLeaveNotifyToMap( BOBUSANGINFO* pInfo )
{
	SERVERINFO* pServerInfo = g_pServerTable->FindMapServer( WORD(pInfo->AppearanceMapNum) );

	if( !pServerInfo )	return;

	if(0 == pServerInfo->dwConnectionIndex)	
	{
		return;
	}

	MSG_DWORD2	msg;
	msg.Category = MP_BOBUSANG;
	msg.Protocol = MP_BOBUSANG_NOTIFY_FOR_DISAPPEARANCE;
	msg.dwData1	= pInfo->AppearanceChannel;
	msg.dwData2 = 0;

	g_Network.Send2Server( pServerInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
}

void BobusangManager_Agent::SetChannelState( DWORD iChannel, int bobusangAppearedState )
{
	if(!m_bManager)	return;

	m_piAppearedState[iChannel] = bobusangAppearedState;
}

void BobusangManager_Agent::DeveloperRequest( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_BOBUSANGINFO_GET_SYN:
		{//[客户端获取包袱商人信息][BY:十里坡剑神][QQ:112582793][2019-6-12][13:40:03]
			BOBUSANGINFO* pInfo = &m_pBobusangInfo[1];
			BOBUSANG_MAPINFO* pRndInfo = m_mapInfo.GetData(m_wMapIndex);
			if (!pInfo || !pRndInfo) return;

			MSGBASE * pmsg = (MSGBASE*)pMsg;
			MSG_BOBUSANG_INFO_GET msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_BOBUSANGINFO_GET_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			SafeStrCpy(msg.strMapName, pRndInfo->strMap.c_str(), pRndInfo->strMap.length()+1);
			msg.dwAppearanceTime = pInfo->AppearanceTime;
			msg.vPos.x = pRndInfo->Pos[pInfo->AppearancePosIdx].ApprPos.x / 100;
			msg.vPos.y = pRndInfo->Pos[pInfo->AppearancePosIdx].ApprPos.z / 100;
			g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			MSG_BOBUSANG_INFO_REQUEST msg;
			msg.Category = MP_CHEAT;
			msg.dwObjectID = pmsg->dwObjectID;

			if( pmsg->dwData >= m_nChannelTotalNum )
			{
				msg.Protocol = MP_CHEAT_BOBUSANGINFO_REQUEST_NACK;
				g_Network.Send2User( (MSGBASE*)&msg, sizeof(msg) );
				return;
			}
			
			msg.Protocol = MP_CHEAT_BOBUSANGINFO_REQUEST_ACK;
			DWORD n = pmsg->dwData;

			msg.appearanceState = m_piAppearedState[n];
			msg.bobusangInfo[0] = m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_CUR ];
			msg.bobusangInfo[1] = m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_NEXT ];
			g_Network.Send2User( (MSGBASE*)&msg, sizeof(msg) );

		}
		break;
	case MP_CHEAT_BOBUSANG_LEAVE_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( pmsg->dwData >= m_nChannelTotalNum )
			{
				pmsg->Protocol = MP_CHEAT_BOBUSANG_LEAVE_NACK;
				pmsg->dwData = 99;
				g_Network.Send2User( (MSGBASE*)pmsg, sizeof(MSG_DWORD) );
				return;
			}

			DWORD n = pmsg->dwData;

			BOBUSANGINFO* pInfo = NULL;
			if( eBBSAS_APPEAR == m_piAppearedState[n] )
			{
				pInfo = &m_pBobusangInfo[ eBBSIT_KINDNUM*n + eBBSIT_CUR ];

				if( 0 == pInfo->AppearanceMapNum )
				{
					pmsg->Protocol = MP_CHEAT_BOBUSANG_LEAVE_NACK;
					pmsg->dwData = 999;
					g_Network.Send2User( (MSGBASE*)pmsg, sizeof(MSG_DWORD) );
					return;
				}

				SendBobusangLeaveNotifyToMap(pInfo);
				SendBobusangLeaveMsgToMap(pInfo);
				m_piAppearedState[n] = eBBSAS_DISAPPEAR_DELAYED;


				pmsg->Protocol = MP_CHEAT_BOBUSANG_LEAVE_ACK;
			}
			else
			{

				pmsg->Protocol = MP_CHEAT_BOBUSANG_LEAVE_NACK;
				pmsg->dwData = 9999;
			}

			g_Network.Send2User( (MSGBASE*)pmsg, sizeof(MSG_DWORD) );

		}
		break;
	case MP_CHEAT_BOBUSANGINFO_CHANGE_SYN:
		{
			MSG_BOBUSANG_INFO* pmsg = (MSG_BOBUSANG_INFO*)pMsg;

			DWORD channel = pmsg->bobusangInfo.AppearanceChannel;
			
			MSGBASE msg;
			msg.Category = MP_CHEAT;
			msg.dwObjectID = pmsg->dwObjectID;

			if( channel >= m_nChannelTotalNum )
			{
				msg.Protocol = MP_CHEAT_BOBUSANGINFO_CHANGE_NACK;
				
			}
			else
			{
				m_pBobusangInfo[ eBBSIT_KINDNUM*channel + eBBSIT_NEXT ] = pmsg->bobusangInfo;
			}

			msg.Protocol = MP_CHEAT_BOBUSANGINFO_CHANGE_ACK;
			g_Network.Send2User( (MSGBASE*)&msg, sizeof(msg) );

		}
		break;
	case MP_CHEAT_BOBUSANG_ONOFF_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( TRUE == SetProcessing(pmsg->dwData) )
				pmsg->Protocol = MP_CHEAT_BOBUSANG_ONOFF_ACK;
			else
				pmsg->Protocol = MP_CHEAT_BOBUSANG_ONOFF_NACK;

			g_Network.Send2User( (MSGBASE*)pmsg, sizeof(*pmsg) );
		}
		break;
	default:
		break;
	}
}

BOOL BobusangManager_Agent::SetProcessing( BOOL bVal )
{
	if( m_bOnProcessing ==  bVal )
		return FALSE;
	else

	{
		m_bOnProcessing = bVal;
		return TRUE;
	}
}

stTIME*	BobusangManager_Agent::SetstTIMEfromMinValue( DWORD min )
{
	static stTIME conv;

#define HOUR_PER_DAY 24
#define MIN_PER_HOUR 60

	DWORD d = min / (HOUR_PER_DAY*MIN_PER_HOUR);
	DWORD h = (min % (HOUR_PER_DAY*MIN_PER_HOUR)) / MIN_PER_HOUR;
	DWORD n = (min % (HOUR_PER_DAY*MIN_PER_HOUR)) % MIN_PER_HOUR;

	conv.SetTime(0,0,d,h,n,0);

	return &conv;
}


