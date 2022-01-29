



// TrafficLog.cpp: implementation of the CTrafficLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrafficLog.h"
#include "UserTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CTrafficLog);
CTrafficLog::CTrafficLog()
{
//	m_CPLogBuffer = 0;

	m_pUserTrafficPool = new CMemoryPoolTempl<USERTRAFFIC>;
	m_pUserTrafficPool->Init( MAX_USER_IN_A_AGENT, 500, "UserTrafficPool");
	m_UserTrafficTable.Initialize( MAX_USER_IN_A_AGENT );

	m_dwCheckTime = 0;
	m_dwLastPacketCheckTime = 0;
	m_dwLastPacketWriteTime = 0;
	m_dwUnValuedCount = 0;
	m_dwValuedCount = 0;

}


CTrafficLog::~CTrafficLog()
{
//	BatchWriteLog();

	m_UserTrafficTable.SetPositionHead();

	USERTRAFFIC* pData = NULL;

	while( pData = m_UserTrafficTable.GetData() )
	{
		m_pUserTrafficPool->Free(pData);
	}

	m_UserTrafficTable.RemoveAll();

	SAFE_DELETE(m_pUserTrafficPool);
}
/*
void CTrafficLog::BatchWriteLog()
{
	FILE* fp = fopen("./Log/TrafficLog.txt","a+");
	if(fp)
	{
		fprintf(fp,m_LogBuffer);
		m_CPLogBuffer = 0;
		fclose(fp);
	}
}
*/
/*
void CTrafficLog::LogBuffer(char * buffer,...)
{
	char msg2[256];
	va_list argList;
	va_start(argList, buffer);
	vsprintf(msg2,buffer,argList);
	va_end(argList);

	int strsize = strlen(msg2);
	if(m_CPLogBuffer + strsize >= MAX_LOG_BUFFER_SIZE)
	{
		BatchWriteLog();
	}
	strncpy(&m_LogBuffer[m_CPLogBuffer], msg2, strsize);
	m_LogBuffer[m_CPLogBuffer+strsize]=0;
	m_CPLogBuffer += strsize;
}
*/
////KES
void CTrafficLog::StartRecord()
{
#ifndef _IGNORE_ASSERT_
	m_dwStartTime		= gCurTime;

	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];
	sprintf(fname,"./Log/RTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp = NULL;
	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "기록시간\t총받은크기\t총받은수\t경과시간\t현재동접\n" );
		fclose( fp );
	}	

	sprintf(fname,"./Log/STrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "기록시간\t총보낸크기\t총보낸수\t경과시간\t현재동접\n" );
		fclose( fp );
	}

	sprintf(fname,"./Log/RMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION\n" );
		fclose( fp );
	}	

	sprintf(fname,"./Log/SRMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION\n" );
		fclose( fp );
	}

	Clear();
#endif
}


void CTrafficLog::EndRecord()
{
#ifndef _IGNORE_ASSERT_
	WriteTrafficLogFile();
	Clear();
#endif
}

void CTrafficLog::AddReceivePacket( DWORD dwCategory, DWORD dwLength )
{
	m_dwTotalReceiveSize[dwCategory] += (dwLength + 40);
	++m_dwTotalReceiveNum[dwCategory];
}

void CTrafficLog::AddSendPacket( DWORD dwCategory, DWORD dwLength )
{
	m_dwTotalSendSize[dwCategory] += (dwLength + 40); // TCP/IP Header bytes
	++m_dwTotalSendNum[dwCategory];
}


//0: player, 1:monster ; 0: MP_TARGET, 1:MP_ONETARGET, 2:MP_COLLECTION
void CTrafficLog::AddReceiveMovePacket( DWORD dwObjectID, WORD Protocol, DWORD dwLength )
{
	int nMP = 3;
	if( Protocol == MP_MOVE_TARGET )
		nMP = 0;
	else if( Protocol == MP_MOVE_ONETARGET )

		nMP = 1;
	else if( Protocol == MP_MOVE_CORRECTION )
		nMP = 2;

	if( dwObjectID < 2000000 ) //player
	{
		m_dwMoveSizeReceive[0][nMP] += dwLength;
		++m_dwMoveNumReceive[0][nMP];
	}
	else	//monster
	{
		m_dwMoveSizeReceive[1][nMP] += dwLength;
		++m_dwMoveNumReceive[1][nMP];
	}
}

void CTrafficLog::AddSendMovePacket( DWORD dwObjectID, WORD Protocol, DWORD dwLength )
{
	int nMP = 3;
	if( Protocol == MP_MOVE_TARGET )
		nMP = 0;
	else if( Protocol == MP_MOVE_ONETARGET )
		nMP = 1;
	else if( Protocol == MP_MOVE_CORRECTION )
		nMP = 2;

	if( dwObjectID < 2000000 ) //player
	{
		m_dwMoveSizeSend[0][nMP] += dwLength;
		++m_dwMoveNumSend[0][nMP];
	}
	else	//monster
	{
		m_dwMoveSizeSend[1][nMP] += dwLength;
		++m_dwMoveNumSend[1][nMP];
	}
}

#define TRAFFIC_LOG_WRITE_TIME	600000

void CTrafficLog::Process()
{
#ifndef _IGNORE_ASSERT_
	if(gCurTime - m_dwLastPacketCheckTime >= m_dwCheckTime)
	{
		ResetUserPacket(gCurTime - m_dwLastPacketCheckTime);
		m_dwLastPacketCheckTime = gCurTime;
	}

	if( gCurTime - m_dwLastPacketWriteTime >= TRAFFIC_LOG_WRITE_TIME )
	{
//		WriteUserTrafficLogFile();
		m_dwLastPacketWriteTime = gCurTime;
	}
/*
	if( gCurTime - m_dwStartTime >= TRAFFIC_LOG_WRITE_TIME )
	{
		WriteTrafficLogFile();
		Clear();
		m_dwStartTime = gCurTime;
	}
*/
#endif
}

void CTrafficLog::Clear()
{
	ZeroMemory( m_dwTotalReceiveSize, sizeof(m_dwTotalReceiveSize) );
	ZeroMemory( m_dwTotalReceiveNum, sizeof(m_dwTotalReceiveNum) );
	ZeroMemory( m_dwTotalSendSize, sizeof(m_dwTotalSendSize) );
	ZeroMemory( m_dwTotalSendNum, sizeof(m_dwTotalSendNum) );

	ZeroMemory( m_dwMoveSizeReceive, sizeof(m_dwMoveSizeReceive) );
	ZeroMemory( m_dwMoveNumReceive, sizeof(m_dwMoveNumReceive) );
	ZeroMemory( m_dwMoveSizeSend, sizeof(m_dwMoveSizeSend) );
	ZeroMemory( m_dwMoveNumSend, sizeof(m_dwMoveNumSend) );
}

void CTrafficLog::WriteTrafficLogFile()	
{
//
	DWORD dwReceiveTotalSize	= 0;
	DWORD dwSendTotalSize		= 0;
	DWORD dwReceiveTotalNum		= 0;
	DWORD dwSendTotalNum		= 0;

	for( int i = 0 ; i < MP_MAX ; ++i )
	{
		dwReceiveTotalSize += m_dwTotalReceiveSize[i];
		dwReceiveTotalNum += m_dwTotalReceiveNum[i];
		dwSendTotalSize += m_dwTotalSendSize[i];
		dwSendTotalNum += m_dwTotalSendNum[i];
	}

	SYSTEMTIME ti;
	GetLocalTime( &ti );

	char fname[256];

	sprintf(fname,"./Log/RTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp;
	
	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );

		fprintf( fp, "%u\t%u", dwReceiveTotalSize, dwReceiveTotalNum );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			fprintf( fp, "\t%u\t%u", m_dwTotalReceiveSize[i], m_dwTotalReceiveNum[i] );
		}
		fprintf( fp, "\n");

		fclose(fp);
	}	

	sprintf(fname,"./Log/STrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "%u\t%u", dwSendTotalSize, dwSendTotalNum );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		for( int i = 0 ; i < MP_MAX ; ++i )
		{
			fprintf( fp, "\t%u\t%u", m_dwTotalSendSize[i], m_dwTotalSendNum[i] );
		}
		fprintf( fp, "\n");


		fclose(fp);
	}


	sprintf(fname,"./Log/RMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );
	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );		

		int i, j;
		for( i = 0 ; i < 2 ; ++i )
		{
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveSizeReceive[i][j] );
			}
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveNumReceive[i][j] );
			}
			fprintf( fp, "\n" );
		}

		fclose( fp );
	}	


	sprintf(fname,"./Log/SRMove_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	fp = fopen( fname, "a+" );
	if(fp)
	{
		fprintf( fp, "%02d:%02d:%02d\t", ti.wHour, ti.wMinute, ti.wSecond );
		fprintf( fp, "\t%u", g_pUserTable->GetDataNum() );
		fprintf( fp, "\t%u", gCurTime - m_dwStartTime );

		int i, j;
		for( i = 0 ; i < 2 ; ++i )
		{
			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveSizeSend[i][j] );
			}

			for( j = 0 ; j < 4 ; ++j )
			{
				fprintf( fp, "\t%u", m_dwMoveNumSend[i][j] );
			}
		}

		fclose( fp );
	}




}

void CTrafficLog::AddUser(DWORD dwUserID, DWORD dwConnectionIndex)
{
	USERTRAFFIC* pData = NULL;

	pData = m_UserTrafficTable.GetData(dwUserID);

	if( pData )
	{
		if( pData->bLogin )
		{
			ASSERT(0);
		}
		else
		{
			pData->bLogin = true;
		}
	}
	else
	{
		pData = m_pUserTrafficPool->Alloc();

		pData->dwConnectionIndex = dwConnectionIndex;
		pData->dwUserID = dwUserID;
		pData->dwLoginTime = 0;
		pData->dwTotalPacketCount = 0;
		pData->dwValuedCount = 0;
		pData->dwUnValuedCount = 0;
		pData->bLogin = true;

		m_UserTrafficTable.Add( pData, dwUserID );
	}
}

void CTrafficLog::RemoveUser(DWORD dwUserID)
{
    USERTRAFFIC* pData = NULL;

	pData = m_UserTrafficTable.GetData(dwUserID);

	if( pData && pData->bLogin )
	{
		DWORD spend = gCurTime - m_dwLastPacketCheckTime;
		
		pData->dwLoginTime += spend;
		
		pData->dwUnValuedCount = 0;
		pData->dwValuedCount = 0;

		pData->bLogin = false;

		return;
	}
	ASSERT(0);
}

void CTrafficLog::AddUserPacket(DWORD dwUserID, BYTE Category)
{
	USERTRAFFIC* pData = NULL;

	pData = m_UserTrafficTable.GetData(dwUserID);

	if( pData && pData->bLogin )
	{
		if(Category >= MP_MAX || Category <= 0)
			pData->dwUnValuedCount++;
		else
			pData->dwValuedCount++;

		pData->dwTotalPacketCount++;
		
		if(pData->dwUnValuedCount >= m_dwUnValuedCount)
			DisconnectUser( pData->dwConnectionIndex );

		if(pData->dwValuedCount >= m_dwValuedCount)
			DisconnectUser( pData->dwConnectionIndex );
		return;
	}
}

void CTrafficLog::ResetUserPacket(DWORD dwTime)
{
	USERTRAFFIC* pData = NULL;

	m_UserTrafficTable.SetPositionHead();
	while( pData = m_UserTrafficTable.GetData() )
	{
		if( pData->bLogin )
		{
			pData->dwLoginTime += dwTime;

			pData->dwValuedCount = 0;
		}
	}
}


void CTrafficLog::WriteUserTrafficLogFile()
{
#ifndef _IGNORE_ASSERT_
	SYSTEMTIME ti;
	GetLocalTime( &ti );
	char fname[256];

	sprintf(fname,"./Log/UserTrafficLog_Agent%d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(),
														ti.wYear, ti.wMonth, ti.wDay );

	FILE* fp;
	
	fp = fopen( fname, "a+" );

	if(fp)
	{
		fprintf( fp, "%02d : %02d : %02d 현재 현황\n기록된 유저수 : %d\n", ti.wHour, ti.wMinute, ti.wSecond, m_UserTrafficTable.GetDataNum() );

		USERTRAFFIC* pData = NULL;
		WORD count = 0;
		
		m_UserTrafficTable.SetPositionHead();
		while( pData = m_UserTrafficTable.GetData() )
		{
			double time = pData->dwLoginTime / 1000;
			double average = pData->dwTotalPacketCount / time;
			
			fprintf( fp, "%04d USERID : %d , 접속시간: %f, 총패킷양 : %d, 평균 : %f\n", ++count, pData->dwUserID, time, pData->dwTotalPacketCount, average );
			if( !pData->bLogin )
			{
				m_UserTrafficTable.Remove( pData->dwUserID );
				m_pUserTrafficPool->Free(pData);
			}
		}

		fclose( fp );
	}
#endif
}

void CTrafficLog::Init()
{
	char fname[] = "./Traffic.txt";

	FILE* fp;
	
	fp = fopen( fname, "r" );

	if(fp)
	{
		fscanf(fp, "%d %d %d", &m_dwUnValuedCount, &m_dwCheckTime, &m_dwValuedCount);
		fclose( fp );
	}
	else
	{
		m_dwUnValuedCount = 10;
		m_dwCheckTime = 1;
		m_dwValuedCount = 1000;
	}

	m_dwCheckTime *= 1000;
}

void CTrafficLog::End()
{
}


