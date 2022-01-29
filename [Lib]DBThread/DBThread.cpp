







//////////////////////////////////////////////////////////////////////
// Coolwater.Fire version 2001.1.15
// Ultimate Database Gasoline Engine
// If it does not work correctly, plz enter www.coolwater.fire.com 
// 
// programed by Park Sung Jun 
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500


#include "stdafx.h"
#include "DB.h"
#include "dbclass.h"

#include <process.h>
BOOL CDB::InitDBThread()
{
	InitializeCriticalSectionAndSpinCount(&m_DBCS,200);           //[数据库线程初始化][By:十里坡剑神][QQ:112582793][2018/3/9]
	InitializeCriticalSectionAndSpinCount(&m_ReturnMessage,200); 
	m_hDBEvent				= CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hKillDBEvent			= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hKillReturnEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hDBThread				= (HANDLE)_beginthread(DBThread,0,this);
	
	return 1;
}



BOOL	CDB::CloseDBThread()
{	
	if (m_hDBThread && m_SucceedInit)
	{
		SetEvent(m_hKillDBEvent);
		WaitForSingleObject(m_hKillReturnEvent, INFINITE);
		m_hDBThread = NULL;
	}
	if(!m_bUsingDLL)
		CloseHandle(m_hResultMessageEvent);

	if (m_SucceedInit)
	{
		DeleteCriticalSection(&m_DBCS);
		DeleteCriticalSection(&m_ReturnMessage);
		
		CloseHandle(m_hDBEvent);
		CloseHandle(m_hKillDBEvent);
		CloseHandle(m_hKillReturnEvent);
	}
	return 1;
}

void CDB::SendMessageDB(LPDBMESSAGE pPara)
{
	EnterCriticalSection(&m_DBCS);
	(GetDBProcessAdd())->AddTail(pPara);
	LeaveCriticalSection(&m_DBCS);
	SetEvent(m_hDBEvent);
}
////////////////////////////////////////////////////////////
void CDB::DispatchDBMessage(DWORD pRet,LPDBMESSAGE pBuffer)
{
	if (m_UsingEventObject)
	{
		LPDBRESULTMESSAGE pMessage = (LPDBRESULTMESSAGE) m_pDBMessageResultMemory->Alloc();
		if (!pMessage) 	m_pWriteMessage("DispatchDBMessage : memory allo fail");
		pMessage->lpDBMessage = pBuffer;
		pMessage->Ret = pRet;

		EnterCriticalSection(&m_ReturnMessage);
		GetDBMessageAdd()->AddTail(pMessage);
		LeaveCriticalSection(&m_ReturnMessage);

		SetEvent(m_hResultMessageEvent);
	}
	else
	::PostMessage(m_hWndToPostMessage, m_dwDBMessageID,(WPARAM) pRet, (LPARAM) pBuffer);
}
////////////////////////////////////////////////////////////////////////////////////////////
void CDB::ProcessingDBMessage()
{
	POSITION_	pos, posbuffer;
	EnterCriticalSection(&m_ReturnMessage);
	
	SwitchList2();
	
	LeaveCriticalSection(&m_ReturnMessage);
	pos = GetDBMessagePop()->GetHeadPosition();

	LPDBRESULTMESSAGE lpBuffer;
	
	while(pos != NULL)
	{
		posbuffer = pos;
		lpBuffer = (LPDBRESULTMESSAGE) GetDBMessagePop()->GetNext(pos);
		if (lpBuffer != NULL)
		{
			GetDBMessagePop()->RemoveAtFromList(posbuffer);
			m_pReadDBResult(lpBuffer->Ret,  lpBuffer->lpDBMessage);
			m_pDBMessageResultMemory->Free((char*)lpBuffer);
		}
	}

	EnterCriticalSection(&m_ReturnMessage);
		
		SwitchList2();
		if (GetDBMessagePop()->IsEmpty() == FALSE)
		{
			SwitchList2();
			SetEvent(m_hResultMessageEvent);
		}
	LeaveCriticalSection(&m_ReturnMessage);

}
////////////////////////////////////////////////////////////////////////////////////////////
void DBThread(LPVOID pPara)
{
	
	CDB* pDB = (CDB*) pPara;
	HANDLE		eventHandles[2];
	POSITION_	pos, posbuffer;
	eventHandles[0] = pDB->GetDBEvent();
	eventHandles[1] = pDB->GetDBKillEvent();
	DWORD dwRet;

	LPDBMESSAGE lpSend;
	while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
	{
		EnterCriticalSection(pDB->GetDBCS());
			pDB->SwitchList();
		LeaveCriticalSection(pDB->GetDBCS());

		pos = (pDB->GetDBProcessPop())->GetHeadPosition();
		while(pos != NULL)
		{
			posbuffer = pos;
			lpSend = (LPDBMESSAGE) (pDB->GetDBProcessPop())->GetNext(pos);

			
			if (lpSend != NULL)

			{
				(pDB->GetDBProcessPop())->RemoveAtFromList(posbuffer);
				if (lpSend->bFNNo > 100)
				{
					LPVOID pRet;
					pRet = pDB->ExcuteThreadFN(lpSend);
					pDB->DispatchDBMessage((DWORD)pRet,lpSend);
				}
				else 
				{
					dwRet = pDB->ExcuteThreadFNbool(lpSend);
					if (dwRet)
					{
						if (lpSend->bNotifyOnlyError)
						{
							if (lpSend->bFNNo == 2)
							{
								pDB->DispatchDBMessage(dwRet,lpSend);					
							}
							else if (lpSend->bFNNo == 14 || lpSend->bFNNo == 8 || lpSend->bFNNo == 10 ||lpSend->bFNNo == 11 ||lpSend->bFNNo == 12 ||lpSend->bFNNo == 13)
							{
								pDB->FreeDBMessageMemory((char*)lpSend);

							}
							else if (lpSend->bFNNo == 5 || lpSend->bFNNo == 6 || lpSend->bFNNo == 15 || lpSend->bFNNo == 16)
							{
								pDB->DispatchDBMessage(dwRet,lpSend);
							}
							else 
							{								
								pDB->FreeDBStackMemory((char*)lpSend->pData);
								pDB->FreeDBMessageMemory((char*)lpSend);
							}
						}
						else
							pDB->DispatchDBMessage(dwRet,lpSend);					
					}
					else pDB->DispatchDBMessage(dwRet,lpSend);
				}
			}
		}	

		EnterCriticalSection(pDB->GetDBCS());

			
			pDB->SwitchList();
			if ((pDB->GetDBProcessPop())->IsEmpty() == FALSE)
			{
				pDB->SwitchList();

				SetEvent(pDB->GetDBEvent());
			}
		LeaveCriticalSection(pDB->GetDBCS());
	}
	SetEvent(pDB->GetKillReturnEvent());
	_endthread();
	return;
}




LPVOID	CDB::ExcuteThreadFN(LPDBMESSAGE pBuffer)
{
	switch (pBuffer->bFNNo)

	{

	case 101:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 104:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeLargeQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 105:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeMiddleQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 102:
	{
		LPDBSingleQuery lpMessage = (LPDBSingleQuery) pBuffer->pData;
		return SingleQuery(lpMessage->pCon, lpMessage->szTable,lpMessage->dwValue,lpMessage->lpCol,lpMessage->sqltype);
	}
	case 103:
	{
		LPDBSingleQuery lpMessage = (LPDBSingleQuery) pBuffer->pData;
		return SingleQueryDA(lpMessage->pCon, lpMessage->szTable,lpMessage->dwValue,lpMessage->lpCol,lpMessage->sqltype);
	}
	}
	return NULL;
}


BOOL	CDB::ExcuteThreadFNbool(LPDBMESSAGE pBuffer)
{
	switch (pBuffer->bFNNo)
	{
		case 1:
		{
			LPDBExcuteIUD pMessage = (LPDBExcuteIUD) pBuffer->pData;
			return ExcuteIUD(pMessage->pCon,pMessage->szQuery);
		}
		case 2:
		{
			LPDBInsertRecord pMessage = (LPDBInsertRecord) pBuffer->pData;
			return InsertRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue,pMessage->bNumData);
		}
		case 3:
		{
			LPDBDeleteRecord pMessage = (LPDBDeleteRecord) pBuffer->pData;
			return DeleteRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue);
		}
		case 4:
		{
			LPDBUpdateRecord pMessage = (LPDBUpdateRecord) pBuffer->pData;
			return UpdateRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue,pMessage->DestCol,pMessage->DestValue);
		}
		case 5:
		{
			LPDBLogin pMessage = (LPDBLogin) pBuffer->pData;
			return Login(pMessage->pCon,pMessage->szTableName,pMessage->pColID,pMessage->pColPW,pMessage->pID,pMessage->pPW,pMessage->pRetCol,pMessage->pRetValue,pMessage->iLength,pMessage->sqlType);
		}
		case 6:
		{
			LPDBSimpleLogin pMessage = (LPDBSimpleLogin) pBuffer->pData;
			return SimpleLogin(pMessage->pCon,pMessage->pID,pMessage->pPW,&(pBuffer->dwResult));
		}
		case 7:
		{
			LPDBGenerateResultSet pMessage = (LPDBGenerateResultSet) pBuffer->pData;
			return BindQuery(pMessage->pCon,pMessage->szBuffer,pMessage->lpBind,pMessage->rowset,pMessage->StSize,pMessage->sqlType);
		}
		case 8:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return DeleteRecordInCursor(pMessage);

		}
		case 9:
		{
			LPDBGenerateResultSet pMessage = (LPDBGenerateResultSet) pBuffer->pData;
			return GenerateResultSet(pMessage->pCon,pMessage->szBuffer,pMessage->lpBind,pMessage->StSize,pMessage->sqlType,pMessage->rowset);
		}
		case 10:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetNextRecordSet(pMessage);
		}
		case 11:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetLastRecordSet(pMessage);
		}
		case 12:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetFirstRecordSet(pMessage);
		}
		case 13:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetPriorRecordSet(pMessage);
		}
		case 14:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return CloseRecordSet(pMessage);
		}
		case 15:
		{
			LPDBSaveDataByPtr pMessge = (LPDBSaveDataByPtr) pBuffer->pData;
			return SaveDataByPtr(pMessge->pCon,pMessge->szQuery,pMessge->nSize,pMessge->pPara,pMessge->nBuffersize,pMessge->pSettingData);
		}
		case 16:
		{
			LPDBLoadDataByPtr lpMessage = (LPDBLoadDataByPtr) pBuffer->pData;
			return LoadDataByPtr(lpMessage->pCon, lpMessage->szQuery,lpMessage->nSize,lpMessage->pData);
		}
		case 17:
		{
			LPDBExcuteIUD pMessage = (LPDBExcuteIUD) pBuffer->pData;
			return ExcuteIUDForProcedure(pMessage->pCon,pMessage->szQuery);
		}
			

	}
	return NULL;
}





