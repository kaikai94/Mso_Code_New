
// Network.cpp: implementation of the CNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Network.h"
#include "Network_guid.h"
#include "ServerTable.h"
#ifdef __AGENTSERVER__
#include "TrafficLog.h"
#include "UserTable.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL g_bReady = FALSE;

CNetwork g_Network;
HINSTANCE g_hdNetInst = NULL;
CNetwork::CNetwork()
{
//	CoInitialize(NULL);
	m_pINet = NULL;

}

CNetwork::~CNetwork()
{
	//Release();
//	CoUninitialize();
}
void CNetwork::Release()
{

	if(m_pINet)
	{
		m_pINet->Release();

		m_pINet = NULL;
	}
	if(g_hdNetInst)
	{
		FreeLibrary(g_hdNetInst);
		g_hdNetInst = NULL;
	}
}


BOOL CNetwork::Init(DESC_NETWORK * desc)
{
	if(!g_pServerTable) return FALSE;
	
	/*HRESULT hr;
	hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&m_pINet);
	if (FAILED(hr))
		return;*/
	DllGetClassObject_NetWork pNetWokFunc = NULL;
	g_hdNetInst = LoadLibrary("Inetwork.dll");
	if(NULL==g_hdNetInst) goto libRt;
	
	pNetWokFunc = (DllGetClassObject_NetWork)GetProcAddress(g_hdNetInst,"DllGetClassObject");
	if(NULL==pNetWokFunc) goto libRt;

	IClassFactory * pcf = NULL;
	HRESULT hr = pNetWokFunc(CLSID_4DyuchiNET,IID_IClassFactory,(void**)&pcf);
	if(FAILED(hr)) goto libRt;
	hr = pcf->CreateInstance(NULL,IID_4DyuchiNET,(void**)&m_pINet);
	if (FAILED(hr))  goto libRt;

	if(!m_pINet->CreateNetwork(desc,0,0,0))
		return FALSE;

	return TRUE;
libRt:
	MessageBox(NULL,"Inetwork.dll¼ÓÔØÊ§°Ü","´íÎó",NULL);
	return FALSE;
}

BOOL CNetwork::StartServerServer(char * szIP, int port)
{
	return m_pINet->StartServerWithServerSide(szIP,port);
}
BOOL CNetwork::StartUserServer(char * szIP, int port)
{
	return m_pINet->StartServerWithUserSide(szIP, port);
}
BOOL CNetwork::ConnectToServer(char * szIP, int port, void * pParam)
{
	return m_pINet->ConnectToServerWithServerSide(szIP,port, OnConnectServerSuccess, OnConnectServerFail, pParam);
}

void CNetwork::Send2Server(DWORD dwConnectionIndex, char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category != 0);
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->SendToServer(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::Send2AgentServer(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
		break;
	}
}
void CNetwork::SendAll2AgentServer(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}
void CNetwork::Send2User(DWORD dwConnectionIndex, char * msg, DWORD size)
{
#ifdef __AGENTSERVER__
	MSGBASE* pMsg = (MSGBASE*)msg;
	ASSERT(pMsg->Category != 0);
	ASSERT(pMsg->Category < MP_MAX);
	ASSERT(size < 512000);

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);

	if( g_pServerSystem->IsEnableCrypt() )
	{
		EnCrypt(pInfo,msg,size);
	}

	BOOL rt = m_pINet->SendToUser(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);

	TRAFFIC->AddSendPacket(((MSGROOT*)msg)->Category, size);

#else	
	BOOL rt = m_pINet->SendToUser(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
#endif
}


#ifdef __AGENTSERVER__

void CNetwork::Send2User(MSGBASE * msg, DWORD size)
{
	ASSERT(msg->Category != 0);
	ASSERT(msg->Category < MP_MAX);
	ASSERT(size < 512000);
	USERINFO * userInfo = g_pUserTableForObjectID->FindUser(msg->dwObjectID);
	
	if( g_pServerSystem->IsEnableCrypt() )
	{
		EnCrypt(userInfo,(char*)msg,size);
	}

	if(userInfo)
		m_pINet->SendToUser(userInfo->dwConnectionIndex, (char *)msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::EnCrypt(USERINFO * userInfo, char* msg,DWORD size)
{
	if( !userInfo ) return;
	int headerSize = sizeof( MSGROOT );
	if( !userInfo->crypto.Encrypt( ( char *)msg + headerSize, size - headerSize ) )
	{
		ASSERTMSG(0,"Encrypt Error");
		return;

	}

	((MSGBASE*)msg)->Code = userInfo->crypto.GetEnCRCConvertChar();


}
#endif

void CNetwork::Broadcast2Server(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category != 0);
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->BroadcastServer(msg, size, FLAG_SEND_NOT_ENCRYPTION);

}
void CNetwork::BroadCastDisServer(char * msg, DWORD size)
{
	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextDistServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}
void CNetwork::Broadcast2User(MSGBASE * msg, DWORD size)

{
	ASSERT(msg->Category != 0);
	ASSERT(msg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->BroadcastUser((char*)msg, size, FLAG_SEND_NOT_ENCRYPTION);
}
void CNetwork::Broad2User(MSGBASE* msg, DWORD size)
{
	ASSERT(msg->Category != 0);
	ASSERT(msg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->BroadcastUser((char*)msg, size, FLAG_SEND_ENCRYPTION);
}
void CNetwork::Broadcast2MapServer(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextMapServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CNetwork::Broadcast2MapServerExceptOne(DWORD dwConnectionIndex, char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextMapServer())
	{
		if(dwConnectionIndex == pInfo->dwConnectionIndex)
			continue;

		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CNetwork::Broadcast2AgentServer(char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);

	}
}

void CNetwork::Broadcast2AgentServerExceptSelf(char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CNetwork::Broadcast2AgentServerExceptOne(DWORD dwConnectionIndex, char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;

	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{

		if(dwConnectionIndex == pInfo->dwConnectionIndex)
			continue;

		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}	
	
}

void CNetwork::GetUserAddress(DWORD dwConnectionIndex, char* ip, WORD * port)
{
	m_pINet->GetUserAddress(dwConnectionIndex, ip, port);
}

void CNetwork::DisconnectUser(DWORD dwConnectionIndex)
{
	m_pINet->CompulsiveDisconnectUser(dwConnectionIndex);
}

