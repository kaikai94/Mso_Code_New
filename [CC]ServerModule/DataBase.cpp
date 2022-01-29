// DataBase.cpp: implementation of the CDataBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataBase.h"

#include "Console.h"
#include "Network.h"
#include "CommonDBMsgParser.h"

#include <stdio.h>

BOOL g_bWriteQuery = FALSE;

void ReadDBResultMessage1(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(0,ret,pMessage);	}
void ReadDBResultMessage2(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(1,ret,pMessage);	}
void ReadDBResultMessage3(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(2,ret,pMessage);	}
void ReadDBResultMessage4(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(3,ret,pMessage);	}
void ReadDBResultMessage5(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(4,ret,pMessage);	}
void ReadDBResultMessage6(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(5,ret,pMessage);	}
void ReadDBResultMessage7(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(6,ret,pMessage);	}
void ReadDBResultMessage8(DWORD ret, LPDBMESSAGE pMessage){	ReadDBResultMessage(7,ret,pMessage);	}

void castingdelete(LPVOID pTarget1, LPVOID pTarget2, int nNum);
void WriteMessage(char* pmsg);

#define	WM_DB_MESSAGE  WM_USER+231
extern HWND g_hWnd;


#define LOGINDBNAME			"MHCMember"
#define LOGINDBID			"sa"
#define LOGINDBPASSWORD		"1lFl%oeE5*MXncZbU4lnyXOu#noC0DIN"

#define LOGDBNAME			"MHLog"
#define LOGDBID				"sa"
#define LOGDBPASSWORD		"1lFl%oeE5*MXncZbU4lnyXOu#noC0DIN"	



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDataBase g_DB;
HINSTANCE g_hdIInst = NULL;

BOOL DBTH::Init(DESC_DBTHREAD* pDesc,BOOL bUsingLoginDB)
{
	DllGetClassObject_DB pDBFunc=NULL;
	g_hdIInst = LoadLibrary("DBTHREAD.dll");
	if(NULL==g_hdIInst)	goto libRt;

	pDBFunc = (DllGetClassObject_DB)GetProcAddress(g_hdIInst,"DllGetClassObject");
	if(NULL==pDBFunc)	goto libRt;

	IClassFactory * pcf = NULL;
	HRESULT hr = pDBFunc(CLSID_DBTHREAD_DLL,IID_IClassFactory,(void**)&pcf);
	if (FAILED(hr))	goto libRt;
	
	hr = pcf->CreateInstance(NULL,IID_DBTHREAD_DLL,(void**)&m_pIDB);
	if (FAILED(hr)) goto libRt;
		
	/*HRESULT hr = CoCreateInstance(CLSID_DBTHREAD_DLL, NULL, CLSCTX_INPROC_SERVER, IID_DBTHREAD_DLL, (void**)&m_pIDB);
	if (FAILED(hr))
	{
		MessageBox(NULL,"Cannot create IID_DBTHREAD_DLL  : unregistered Component","Error",NULL);
		return FALSE;
	}*/
	
	BOOL ret = m_pIDB->CreateDBThread(pDesc);
	if (!ret)
	{

		LPSTR pmsg = "can not init db class";
		WriteMessage(pmsg);
		return FALSE;
	} 

#ifndef _MONITORINGSERVER
	if (g_Network.GetINet())
		m_pIDB->SetDBMessageEvent( g_Network.GetINet()->GetCustomEventHandle(1) );
#endif
	if(bUsingLoginDB)
		m_LoginDB = m_pIDB->MakeNewConnection(LOGINDBNAME, LOGINDBID, LOGINDBPASSWORD);
	m_LogDB = m_pIDB->MakeNewConnection(LOGDBNAME, LOGDBID, LOGDBPASSWORD); 
	return TRUE;

libRt:
	MessageBox(NULL,"DBTHREAD.dll����ʧ��","����",NULL);
	return FALSE;
}
void DBTH::Release()
{

	if(m_pIDB)
	{
		m_pIDB->Release();
		m_pIDB = NULL;
		m_LoginDB = NULL;
		m_LogDB = NULL;
	}
	if(g_hdIInst)
	{
		FreeLibrary(g_hdIInst);
		g_hdIInst = NULL;
	}
}


BOOL DBTH::Query(DBQUERY* pQuery)
{
	LPDBFreeQuery pData = (LPDBFreeQuery) m_pIDB->GetDBMessageStackMemory();
	if(!pData)
	{
		return FALSE;
	}
	if(pQuery->dbcontype == eDBCON_LOGIN)
		pData->pCon = m_LoginDB;
	else if(pQuery->dbcontype == eDBCON_LOG)
		pData->pCon = m_LogDB;
	else if(pQuery->dbcontype == eDBCON_ADMIN)
		pData->pCon = m_pIDB->GetAdMinCon();
	else
	{
		ASSERT(0);
	}
	

	sprintf(pData->szBuffer, "%s", 	pQuery->strQuery);
	
	LPDBMESSAGE pTemp = (LPDBMESSAGE) m_pIDB->GetDBMessageMemory();
	if(!pTemp)
	{
		m_pIDB->FreeThisMemory( NULL, pData );
		return FALSE;
	}
	
	pTemp->bFNNo = pQuery->QueryType;									
	pTemp->bNotifyOnlyError = 0;
	pTemp->dwMessageID = pQuery->dwMessageID;							
	pTemp->dwID = pQuery->dwID;											
	pTemp->pData = (LPVOID) pData;
	
	
	m_pIDB->SendMessageDB(pTemp);	

	return TRUE;
}

void DBTH::DeletePointer(LPDBMESSAGE pMessage,DWORD ret)
{			
	m_pIDB->DeleteThisPointer(pMessage->bFNNo,(LPVOID)ret);
	m_pIDB->FreeThisMemory(pMessage,pMessage->pData);
}

			
CDataBase::CDataBase()
{
	m_pDB = NULL;
}

CDataBase::~CDataBase()
{
	Release();

}
BOOL CDataBase::AllCleared()
{
	if(m_QueryQueue.GetCount() != 0)
		return FALSE;

	for(DWORD n=0;n<m_MaxDBThreadNum;++n)
	{
		if(m_pDB[n].m_pIDB->GetUnusedDBMemoryNum() != m_SameTimeQueryPerThread)
			return FALSE;
		if(m_pDB[n].m_pIDB->GetUnusedStackMemoryNum() != m_SameTimeQueryPerThread)
			return FALSE;
	}

	return TRUE;
}
void CDataBase::Release()
{
	if(m_pDB)
	{
		while(AllCleared() == FALSE)
		{
			ProcessingQueue();
			ProcessingDBMessage();
			Sleep(50);
		}

		for(DWORD n=0;n<m_MaxDBThreadNum;++n)
			m_pDB[n].Release();

		m_MaxDBThreadNum = 0;
		delete [] m_pDB;
		m_pDB = NULL;
	}
}

DBTH* CDataBase::GetCurDB()
{
	DWORD maxunusedMem = 0;

	DWORD unusedMem;
	int idleThread = -1;

	for(DWORD n=0;n<m_MaxDBThreadNum;++n)
	{
		unusedMem = m_pDB[n].m_pIDB->GetUnusedDBMemoryNum();
		if(unusedMem > maxunusedMem)
		{
			maxunusedMem = unusedMem;
			idleThread = n;
		}
	}

	if(idleThread == -1)
		return NULL;
	return &m_pDB[idleThread];
}

BOOL CDataBase::Init(DWORD MaxDBThread,DWORD MaxSameTimeQuery,BOOL bUsingLoginDB)
{
	m_QueryPool.Init( MaxSameTimeQuery, MaxSameTimeQuery/4, "QueryPool" );

	DESC_DBTHREAD desc;
	desc.bUsingDLL = TRUE;							
	desc.bUsingThread = TRUE;						
	desc.bUsingEventObject = TRUE;					
	
	strcpy(desc.AdminCursor,"Cursor");
	strcpy(desc.AdminID,LOGINDBID);
	
	strcpy(desc.ClientCursor ,"Cursor");
	strcpy(desc.ClientID ,LOGINDBID);	

	strcpy(desc.AdminPW ,LOGINDBPASSWORD);
	strcpy(desc.ClientPW,LOGINDBPASSWORD);

	strcpy(desc.DBName,"MHGame");
	
	strcpy(desc.Default_User_account,"sa");
	strcpy(desc.Default_User_guid,"sa");
	strcpy(desc.Default_User_pw,"1lFl%oeE5*MXncZbU4lnyXOu#noC0DIN");
	strcpy(desc.Default_User_table,"test");

	desc.dwDBMessageID =	WM_DB_MESSAGE;			

	desc.hWndToPostMessage = g_hWnd;	

	desc.WriteMessage = WriteMessage;				
	
	desc.dwMessageMaxSize	= 30;	

	m_SameTimeQueryPerThread = 1024;

	desc.dwNumofMessage		= m_SameTimeQueryPerThread;				
	desc.dwNumofResult		= 100;
	desc.dwResultMaxSize	= sizeof(QUERYST)*MAX_ROW_NUM;	
	desc.dwSizeofStack		= sizeof(DBFreeQuery);


	m_MaxDBThreadNum = MaxDBThread;
	m_pDB = new DBTH[MaxDBThread];
	for(DWORD n=0;n<m_MaxDBThreadNum;++n)
	{
		if(n==0)	desc.ReadDBResult = ReadDBResultMessage1;
		else if(n==1)	desc.ReadDBResult = ReadDBResultMessage2;
		else if(n==2)	desc.ReadDBResult = ReadDBResultMessage3;
		else if(n==3)	desc.ReadDBResult = ReadDBResultMessage4;
		else if(n==4)	desc.ReadDBResult = ReadDBResultMessage5;
		else if(n==5)	desc.ReadDBResult = ReadDBResultMessage6;
		else if(n==6)	desc.ReadDBResult = ReadDBResultMessage7;
		else if(n==7)	desc.ReadDBResult = ReadDBResultMessage8;

		BOOL rt = m_pDB[n].Init(&desc,bUsingLoginDB);
		ASSERT(rt);
	}

	return TRUE;
}

BOOL CDataBase::AddQueryQueue(DBQUERY* pQuery, BOOL bAddQueueWhenFull)
{
	if(m_pDB == NULL)	return FALSE; 
#ifndef _MONITORINGSERVER
	if(g_bWriteQuery)
	{
		FILE* fp = fopen("DB_Query.txt","a+");
		if(fp)
		{
			fprintf(fp,"%s    %s",pQuery->strQuery,GetCurTimeToString());
			fclose(fp);
		}
	}
#endif

	DBTH* pDB = GetCurDB();

	if(pDB == NULL)
	{
		m_QueryQueue.AddTail(pQuery);
		return TRUE;
	}
	
	if( m_QueryQueue.GetCount() )
	{
		m_QueryQueue.AddTail(pQuery);
		return TRUE;
	}

	if(pDB->Query(pQuery) == FALSE)	
	{
		if(bAddQueueWhenFull)
			m_QueryQueue.AddTail(pQuery);	
		else
			return FALSE;
	}
	else

	{
		m_QueryPool.Free(pQuery);
	}
	return TRUE;
}

BOOL CDataBase::LoginQuery(BYTE QueryType,DWORD dwMessageID,DWORD ID,char* strQuery, BOOL bAddQueueWhenFull)
{
	if(m_pDB == NULL)	return FALSE;

	DBQUERY* pQuery = m_QueryPool.Alloc();	
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_LOGIN,QueryType,dwMessageID,strQuery,ID);

	return AddQueryQueue(pQuery);
}

BOOL CDataBase::LoginMiddleQuery(void* pReturnFunc,DWORD ID,char* strQuery,...)
{
	static char temp[4096];
	va_list vl;
	va_start(vl, strQuery);
	vsprintf(temp, strQuery, vl);
	va_end(vl);
	

	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_LOGIN,eQueryType_FreeMiddleQuery,(DWORD)pReturnFunc,temp,ID);

	AddQueryQueue(pQuery);
	return TRUE;

}

BOOL CDataBase::Query(BYTE QueryType,DWORD dwMessageID,DWORD ID,char* strQuery)
{
	if(m_pDB == NULL)	return FALSE; 

	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_ADMIN,QueryType,dwMessageID,strQuery,ID);

	AddQueryQueue(pQuery);
	return TRUE;
}

BOOL CDataBase::LogQuery(BYTE QueryType,DWORD dwMessageID,DWORD ID,char* strQuery)
{
	if(m_pDB == NULL)	return FALSE;

	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_LOG,QueryType,dwMessageID,strQuery,ID);

	AddQueryQueue(pQuery);
	return TRUE;
}

BOOL CDataBase::FreeQuery(DWORD dwMessageID, DWORD ID, char* strQuery,...)
{
	if(m_pDB == NULL)	return FALSE;


	static char temp[4096];
	va_list vl;
	va_start(vl, strQuery);
	vsprintf(temp, strQuery, vl);
	va_end(vl);
	
	
	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_ADMIN,eQueryType_FreeQuery,dwMessageID,temp,ID);

	AddQueryQueue(pQuery);
	return TRUE;
}

BOOL CDataBase::FreeLargeQuery(void* pReturnFunc,DWORD ID,char* strQuery,...)
{
	if(m_pDB == NULL)	return FALSE;

	static char temp[4096];
	va_list vl;
	va_start(vl, strQuery);
	vsprintf(temp, strQuery, vl);
	va_end(vl);
	
	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_ADMIN,eQueryType_FreeLargeQuery,(DWORD)pReturnFunc,temp,ID);

	AddQueryQueue(pQuery);
	return TRUE;
}

BOOL CDataBase::FreeMiddleQuery(void* pReturnFunc,DWORD ID,char* strQuery,...)
{

	if(m_pDB == NULL)	return FALSE;
	
	static char temp[4096];
	va_list vl;
	va_start(vl, strQuery);
	vsprintf(temp, strQuery, vl);
	va_end(vl);
	
	DBQUERY* pQuery = m_QueryPool.Alloc();
	ASSERT(pQuery);
	pQuery->SetQuery(DBTH::eDBCON_ADMIN,eQueryType_FreeMiddleQuery,(DWORD)pReturnFunc,temp,ID);

	AddQueryQueue(pQuery);
	return TRUE;
}
void CDataBase::ProcessingDBMessage()
{
	for(DWORD n=0;n<m_MaxDBThreadNum;++n)
	{
		if( m_pDB[n].m_pIDB )
			m_pDB[n].m_pIDB->ProcessingDBMessage();
	}

	ProcessingQueue();
}

void CDataBase::ProcessingQueue()
{
	while(m_QueryQueue.GetCount() != 0)
	{	


		DBQUERY* pQuery = (DBQUERY*)m_QueryQueue.GetHead();
		DBTH* pDB = GetCurDB();		
		if( pDB == NULL ) break;


		if( pDB->Query(pQuery) )
		{
			m_QueryQueue.RemoveHead();
			m_QueryPool.Free(pQuery);
		}
		else
		{
			break;
		}
	}
}

void ReadDBResultMessage(int ThreadNum,DWORD ret, LPDBMESSAGE pMessage)
{
	switch(pMessage->bFNNo)
	{
	case eQueryType_FreeQuery:
		{
			if( ret && g_DBMsgFunc[pMessage->dwMessageID])
				g_DBMsgFunc[pMessage->dwMessageID]((LPQUERY)ret, pMessage);
			g_DB.m_pDB[ThreadNum].DeletePointer(pMessage,ret);
		}
		break;
	case eQueryType_FreeMiddleQuery:
		{
			if( ret && pMessage->dwMessageID)
				((DBMiddleMsgFunc)pMessage->dwMessageID)((LPMIDDLEQUERY)ret, pMessage);
			g_DB.m_pDB[ThreadNum].DeletePointer(pMessage,ret);
		}
		break;
	case eQueryType_FreeLargeQuery:
		{
			if( ret && pMessage->dwMessageID)
				((DBLargeMsgFunc)pMessage->dwMessageID)((LPLARGEQUERY)ret, pMessage);
			g_DB.m_pDB[ThreadNum].DeletePointer(pMessage,ret);
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}

}

#ifdef _MONITORINGSERVER
char* GetCurTimeToString()
{
	static char t[2] = {0,};
	return t;
}
#endif

void  WriteMessage(char* pmsg)
{
	SYSTEMTIME time;
	char szFile[256] = {0,};

	GetLocalTime( &time );

#ifndef _RMTOOL_
	g_Console.LOG(2, pmsg);
#ifdef _MAPSERVER_	
	sprintf( szFile, "./Log/DB_ERROR_LOG_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
#else
	sprintf( szFile, "./Log/DB_ERROR_LOG_%4d%02d%02d.txt", time.wYear, time.wMonth, time.wDay );
#endif

	FILE* fp = fopen(szFile,"a+");
	fprintf(fp,"%s, time:%s\n",pmsg, GetCurTimeToString());
	fclose(fp);
#else
	sprintf( szFile, "./Log/DB_ERROR_LOG_FORTOOL_%02d_%4d%02d%02d.txt", g_pServerSystem->GetMapNum(), time.wYear, time.wMonth, time.wDay );
	FILE* fp = fopen(szFile,"a+");
	fprintf(fp,"[%s | %s] : %s\n", __DATE__, __TIME__, pmsg);
	fclose(fp);
#endif

	char cmp[64] = "SqlState: 08S01";
	if( strstr( pmsg, cmp ) )
	{

#ifndef _MONITORINGSERVER
		//SendMessage( g_hWnd, WM_CLOSE, 0, 0 );
#endif
		return;
	}
}



void castingdelete(LPVOID pTarget1, LPVOID pTarget2, int nNum)
{
	switch(nNum)
	{
	case 1:
		delete (DBDATA1*) pTarget1;
		delete (DBDATA1*) pTarget2;
		return;
	case 2:
		delete (DBDATA2*) pTarget1;
		delete (DBDATA2*) pTarget2;
		return;
	case 3:
		delete (DBDATA3*) pTarget1;
		delete (DBDATA3*) pTarget2;
		return;
	case 4:
		delete (DBDATA4*) pTarget1;
		delete (DBDATA4*) pTarget2;
		return;
	case 5:
		delete (DBDATA5*) pTarget1;
		delete (DBDATA5*) pTarget2;
		return;
	case 6:
		delete (DBDATA6*) pTarget1;
		delete (DBDATA6*) pTarget2;
		return;
	case 7:
		delete (DBDATA7*) pTarget1;
		delete (DBDATA7*) pTarget2;
		return;
	case 8:
		delete (DBDATA8*) pTarget1;
		delete (DBDATA8*) pTarget2;
		return;
	case 9:
		delete (DBDATA9*) pTarget1;
		delete (DBDATA9*) pTarget2;
		return;
	case 10:

		delete (DBDATA10*) pTarget1;
		delete (DBDATA10*) pTarget2;
		return;
		
	}
}




