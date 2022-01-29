



// UserTable.cpp: implementation of the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"

#include "Network.h"
#include "DistributeDBMsgParser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CUserTable * g_pUserTable = NULL;
CLoginUser g_LoginUser;
extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;

CUserTable::CUserTable()
{
}

CUserTable::~CUserTable()
{
}

void CUserTable::Init(DWORD dwBucket)
{
	CYHHashTable<USERINFO>::Initialize(dwBucket);

	m_MaxUserCount = 0;
	m_dwUserCount = 0;
	m_addCount = 0;
}

USERINFO* CUserTable::FindUser(DWORD dwKey)
{
	return CYHHashTable<USERINFO>::GetData(dwKey);

}


BOOL CUserTable::AddUser(USERINFO* pObject,DWORD dwKey)
{
	ASSERT(!FindUser(dwKey));
	++m_dwUserCount;

	++ m_addCount;

	return CYHHashTable<USERINFO>::Add(pObject,dwKey);	
}

USERINFO * CUserTable::RemoveUser(DWORD dwKey)
{
	USERINFO * info = NULL;

	info = (USERINFO *)GetData(dwKey);
	Remove(dwKey);
	--m_dwUserCount;

	return info;
}

void CUserTable::RemoveAllUser()
{
	m_dwUserCount = 0;
	CYHHashTable<USERINFO>::RemoveAll();
}
BOOL CUserTable::SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size)
{
	USERINFO* pUserInfo = FindUser(dwKey);
	if(pUserInfo == NULL)
		return FALSE;
	if(pUserInfo->dwUniqueConnectIdx != dwUniqueConnectIdx)
		return FALSE;

	g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)pmsg,size);
	return TRUE;
}

BOOL CUserTable::OnDisconnectUser(DWORD dwKey)
{
	USERINFO* pUserInfo = FindUser(dwKey);
	if(pUserInfo == NULL)	return FALSE;

	if( pUserInfo->dwDisconOption & eDISCONOPT_DELETELOGINCHECK )
	if( pUserInfo->dwUserID )
		LoginCheckDelete( pUserInfo->dwUserID );

	if( pUserInfo->dwUserID != 0 &&
		pUserInfo->bRecvDistOut == FALSE)
	{
		MSG_LOGIN_SYN msg;
		memset(&msg,0,sizeof(msg));
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_DISCONNECTED_ON_LOGIN;
		msg.dwObjectID = pUserInfo->dwUserID;
		msg.AuthKey = pUserInfo->dwUniqueConnectIdx;
		g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
	}


	RemoveUser(dwKey);
	memset(pUserInfo,0,sizeof(USERINFO));	
	g_UserInfoPool.Free(pUserInfo);

	return TRUE;
}

BOOL DisconnectUser(DWORD dwConnectionIndex)
{
	g_Network.DisconnectUser(dwConnectionIndex);

	return TRUE;
}




//===========================================================

CLoginUser::CLoginUser()
{
	m_User.Initialize( 100 );
}

CLoginUser::~CLoginUser()
{
	sLOGINUSER* pData = NULL;
	m_User.SetPositionHead();
	while( pData = m_User.GetData() )
		delete pData;
	m_User.RemoveAll();
}
void CLoginUser::Remove( DWORD dwUserIdx )
{
	sLOGINUSER* pData = m_User.GetData( dwUserIdx );
	if( pData )
	{
		delete pData;
		m_User.Remove( dwUserIdx );		
	}
}

void CLoginUser::AddWaitForLoginCheckDelete( DWORD dwUserIdx )
{
	sLOGINUSER* pData = m_User.GetData( dwUserIdx );
	
	if( pData )
	{
		if( gCurTime - pData->dwTime >= 10*1000 ) //10sec
		{

			m_User.Remove( dwUserIdx );
			delete pData;			
			LoginCheckDelete( dwUserIdx );
		}
	}


	else
	{
		sLOGINUSER* pData = new sLOGINUSER;
		pData->dwTime = gCurTime;
		m_User.Add( pData, dwUserIdx ); 
	}
}


