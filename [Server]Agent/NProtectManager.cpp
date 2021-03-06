



#include "stdafx.h"

#ifdef _NPROTECT_

#include ".\nprotectmanager.h"

#include "UserTable.h"
#include "Network.h"
#include "AgentDBMsgParser.h"

GLOBALTON(CNProtectManager);

//DWORD CNProtectManager::m_dwHU = 0;

CNProtectManager::CNProtectManager(void)
{
}

CNProtectManager::~CNProtectManager(void)
{
}

void CNProtectManager::NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_NPROTECT_ANSWER:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);

		//NULLüũ!!
			if( pUserInfo == NULL ) return;
		//------

			pUserInfo->m_pCSA->m_AuthAnswer.dwIndex = pmsg->dwData1;
			pUserInfo->m_pCSA->m_AuthAnswer.dwValue1 = pmsg->dwData2;
			pUserInfo->m_pCSA->m_AuthAnswer.dwValue2 = pmsg->dwData3;
			pUserInfo->m_pCSA->m_AuthAnswer.dwValue3 = pmsg->dwData4;

			DWORD dwRet = pUserInfo->m_pCSA->CheckAuthAnswer();

			if(dwRet != ERROR_SUCCESS)
			{
				char buf[256] = {0,};

				SYSTEMTIME ti;
				GetLocalTime( &ti );

				sprintf(buf, "[ERRCODE:%d] Query : %08X %08X %08X %08X, Answer: %08X %08X %08X %08X, UserIdx: %d, CharIdx: %d, Time: %02d:%02d:%02d\n", 
					dwRet,
					pUserInfo->m_pCSA->m_AuthQuery.dwIndex,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue1,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue2,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue3,


					pUserInfo->m_pCSA->m_AuthAnswer.dwIndex,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue1,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue2,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue3,

					pUserInfo->dwUserID,
					pUserInfo->dwCharacterID,
					ti.wHour, ti.wMinute, ti.wSecond );

				char fname[256];

				sprintf(fname,"./Log/NProtect_%02d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(), ti.wYear, ti.wMonth, ti.wDay);
				FILE* pFile = fopen(fname, "a+");

				fprintf(pFile, buf);
				fclose(pFile);

				MSGBASE msg;
				msg.Category = MP_NPROTECT;
				msg.Protocol = MP_NPROTECT_DISCONNECT;
				g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
				DisconnectUser(dwConnectionIndex);
				NPROTECTMGR->Block(pUserInfo, dwRet);

				return;
			}

#ifdef _TW_LOCAL_
			if(pUserInfo->dwUserID == 1043448)
#else //HK

			if(pUserInfo->dwUserID == 292027)
#endif
			{
				char buf[256] = {0,};

				SYSTEMTIME ti;
				GetLocalTime( &ti );

				sprintf(buf, "[ERRCODE:%d] Query : %08X %08X %08X %08X, Answer: %08X %08X %08X %08X, UserIdx: %d, CharIdx: %d, Time: %02d:%02d:%02d\n", 
					dwRet,
					pUserInfo->m_pCSA->m_AuthQuery.dwIndex,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue1,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue2,
					pUserInfo->m_pCSA->m_AuthQuery.dwValue3,

					pUserInfo->m_pCSA->m_AuthAnswer.dwIndex,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue1,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue2,
					pUserInfo->m_pCSA->m_AuthAnswer.dwValue3,

					pUserInfo->dwUserID,
					pUserInfo->dwCharacterID,
					ti.wHour, ti.wMinute, ti.wSecond );

				char fname[256];

				sprintf(fname,"./Log/NProtect_TEST_%02d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(), ti.wYear, ti.wMonth, ti.wDay);
				FILE* pFile = fopen(fname, "a+");

				fprintf(pFile, buf);
				fclose(pFile);
			}

			pUserInfo->m_bCSA = FALSE;

			//

			if( pUserInfo->m_nCSAInit == 3 && pUserInfo->m_dwHUC != pmsg->dwObjectID )
			{
				pUserInfo->m_bCSA = TRUE;
			}

			// 1?? ????
			if(pUserInfo->m_nCSAInit == 1)
			{

				// 2?? ????
				pUserInfo->m_nCSAInit = 2;
				NPROTECTMGR->SendAuthQuery(pUserInfo);
			}
			else if(pUserInfo->m_nCSAInit == 2)
			{
				pUserInfo->m_nCSAInit = 3;	// ???Ŀ??? ?ֱ??????? ???????ش?
				pUserInfo->dwLastNProtectCheck += 1000*60*2;	//ó???? 1?и???.
			}
		}
		break;
	case MP_NPROTECT_USER_DISCONNECT:

		{

			// Ŭ???̾?Ʈ?? nProtect?? Ŭ???̾?Ʈ?? ???? ??????ų?? ?????? ?޼???
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

//			NPROTECTMGR->Block(pUserInfo, pmsg->dwData); --> Ŭ???̾?Ʈ ??ŷ?? ???????? ?ʴ´?.
		}
		break;

	case MP_NPROTECT_HACKTOOLUSER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );

			if( pInfo == NULL ) return;

			pInfo->bHackToolUser = TRUE;

		}
		break;
	default:
		break;
	}
}

void CNProtectManager::SendAuthQuery(USERINFO* pInfo)
{
	if(pInfo->m_bCSA)
	{
		MSGBASE msg;
		msg.Category = MP_NPROTECT;
		msg.Protocol = MP_NPROTECT_DISCONNECT;
		g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		if( pInfo->UserLevel >= eUSERLEVEL_GM )
			DisconnectUser(pInfo->dwConnectionIndex);
//		Block(pInfo, 0);

		return;
	}
	
	MSG_DWORD4 msg;
	DWORD dwRet = pInfo->m_pCSA->GetAuthQuery();

	if(dwRet != ERROR_SUCCESS)
	{
		char buf[256] = {0,};

		SYSTEMTIME ti;
		GetLocalTime( &ti );

		sprintf(buf, "Query : %08X %08X %08X %08X, UserIdx: %d, CharIdx: %d, Time: %02d:%02d:%02d\n", 
			pInfo->m_pCSA->m_AuthQuery.dwIndex,
			pInfo->m_pCSA->m_AuthQuery.dwValue1,
			pInfo->m_pCSA->m_AuthQuery.dwValue2,
			pInfo->m_pCSA->m_AuthQuery.dwValue3,
			pInfo->dwUserID,
			pInfo->dwCharacterID,
			ti.wHour, ti.wMinute, ti.wSecond);

		char fname[256];

		sprintf(fname,"./Log/NProtect_%02d_%02d%02d%02d.txt", g_pServerSystem->GetServerNum(), ti.wYear, ti.wMonth, ti.wDay);
		FILE* pFile = fopen(fname, "a+");

		fprintf(pFile, buf);
		fclose(pFile);

		return;

	}

	msg.Category = MP_NPROTECT;
	msg.Protocol = MP_NPROTECT_QUERY;
	msg.dwData1 = pInfo->m_pCSA->m_AuthQuery.dwIndex;
	msg.dwData2 = pInfo->m_pCSA->m_AuthQuery.dwValue1;
	msg.dwData3 = pInfo->m_pCSA->m_AuthQuery.dwValue2;
	msg.dwData4 = pInfo->m_pCSA->m_AuthQuery.dwValue3;
	msg.dwObjectID = msg.dwData3*2+msg.dwData1+msg.dwData2;

	pInfo->m_dwHUC = msg.dwData1/2 + msg.dwData2/2 + msg.dwData3 + msg.dwData4*2;

	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));

	pInfo->m_bCSA = TRUE;

	pInfo->dwLastNProtectCheck = gCurTime;
}

BOOL CNProtectManager::Init(WORD mapnum)
{
	BOOL bResult = InitGameguardAuth("./nProtect/", 50, true, 0x03);

	if(bResult)
		GGAuthUpdateTimer();
	
	m_MapNum = mapnum;

	return bResult;
}
void CNProtectManager::Release()
{
	CleanupGameguardAuth();
}

void CNProtectManager::NpLog(int mode, char* msg)
{
	if(mode & (NPLOG_DEBUG | NPLOG_ERROR))
	{
		SYSTEMTIME ti;
		GetLocalTime( &ti );

		char fname[256];

		sprintf(fname,"./Log/NProtect_%02d_%02d%02d%02d.txt", m_MapNum, ti.wYear, ti.wMonth, ti.wDay);
		FILE* pFile = fopen(fname, "a+");

		fprintf(pFile, msg);
		fprintf(pFile, "\n");


		fclose(pFile);
	}

}

void CNProtectManager::GGAuthUpdateCallback(PGG_UPREPORT report)
{
}

void CNProtectManager::Update()
{
	// ???? ?˰????? ??????Ʈ ?ֱ??? 5??(????)
	static DWORD dwCheckTime = gCurTime;

	if(gCurTime - dwCheckTime >= 300000)
	{
		GGAuthUpdateTimer();
		dwCheckTime = gCurTime;	//?׽?Ʈ?? ???? ???? ?ּ?ó??
	}
}

void CNProtectManager::Block(USERINFO* pInfo, DWORD BlockType)
{
	char IP[17] = {0,};
	WORD Port;

	g_Network.GetUserAddress(pInfo->dwConnectionIndex, IP, &Port);
	NProtectBlock(pInfo->dwUserID, pInfo->dwCharacterID, IP, BlockType);
}

#endif


