







// MHNetwork.cpp: implementation of the CMHNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>          //
#include "MHNetwork.h"
#include "GameState.h"
#include "mmsystem.h"
#include "MainGame.h"
#include "GameResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 
GLOBALTON(CMHNetwork)
CMHNetwork* gNetwork = NULL;
HMODULE g_hBaseNetworkHandle = NULL;
CMHNetwork::CMHNetwork()
{
	m_ConnectionIndex = NULL;
	m_CheckSum = 0;

	gNetwork = this;

	m_bDisconnecting = FALSE;
}

CMHNetwork::~CMHNetwork()
{

	Release();
}



void CMHNetwork::Release()

{
	if(m_pBaseNetwork)
	{
		Disconnect();
	}
	
	SAFE_RELEASE(m_pBaseNetwork);


	if(g_hBaseNetworkHandle)
	{
		FreeLibrary( g_hBaseNetworkHandle );
		g_hBaseNetworkHandle = NULL;
	}

}

#define WM_SOCKEVENTMSG WM_USER+155

BOOL CMHNetwork::Init(HWND hWnd)
{
	m_hMainWnd = hWnd;
	
	HRESULT hr;
	
	g_hBaseNetworkHandle = LoadLibrary("BaseNetwork.dll");

	if (NULL == g_hBaseNetworkHandle) return FALSE;

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");

	if (NULL == pNetFunc) return FALSE;

	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&m_pBaseNetwork);		
	if (hr != S_OK)
	{

		MessageBox(NULL,"BASENETWORK.DLL is Not Exist.","Error",MB_OK);
		return FALSE;
	}
	
	DESC_BASENETWORK Desc;	memset(&Desc,0,sizeof(Desc));
	Desc.wSockEventWinMsgID = WM_SOCKEVENTMSG;	
	Desc.OnConnect			= OnCon;
	Desc.OnDisconnect		= OnDisCon;
	Desc.ReceivedMsg		= OnRecv;
	
	m_pBaseNetwork->InitNetwork(m_hMainWnd,5,&Desc);

	if(m_pBaseNetwork == NULL)
		return FALSE;

	
	return TRUE;
}


BOOL CMHNetwork::ConnectToServer(char* ip,WORD port)
{	
	SEVERLIST(* pServerList)[50] = GAMERESRCMNGR->m_ServerList;
	char hostname[256];
	strcpy(hostname,ip);
	// 输入的是域名还是ip地址，如是域名就去获取其对应的ip
	if (isalpha(ip[0]))
	{  //域名解析 BY:十里坡剑神	QQ:112582793
		hostent* pHostent = gethostbyname(hostname);
		if (pHostent==NULL) {
			printf("Error: %u\n", WSAGetLastError());
			return FALSE;
		}

		hostent& he = *pHostent;
		/*printf("name=%s\naliases=%s\naddrtype=%d\nlength=%d\n",
			he.h_name, he.h_aliases, he.h_addrtype, he.h_length);*/

		sockaddr_in sa;
		for (int nAdapter=0; he.h_addr_list[nAdapter]; nAdapter++) {

			memcpy ( &sa.sin_addr.s_addr, he.h_addr_list[nAdapter],he.h_length);
			sprintf(hostname,"%s", inet_ntoa(sa.sin_addr)); 
		}

	}
	m_ConnectionIndex = m_pBaseNetwork->ConnectToServer(hostname,port);
	if(!m_ConnectionIndex)
		return FALSE;

	return TRUE;
}

void CMHNetwork::Disconnect()
{
	if(m_ConnectionIndex)
	{
		m_pBaseNetwork->CompulsiveDisconnect(m_ConnectionIndex);
		m_ConnectionIndex = 0;	
		m_bDisconnecting = TRUE;
	}
}

void CMHNetwork::SetCurState(CGameState* pState)
{
	m_pCurState = pState;
}

void CMHNetwork::Send(MSGROOT* pMsg,int MsgLen)
{
	ASSERT(pMsg->Category < MP_MAX);

	int ssize = sizeof( MSGROOT );

	if( !m_crypt.Encrypt( ( char * )pMsg + ssize, MsgLen - ssize ) )
	{
		ASSERTMSG(0,"Encrypt Error");
		return;
	}
	pMsg->Code = m_crypt.GetEnCRCConvertChar();

	pMsg->CheckSum = m_CheckSum++;

	if(m_ConnectionIndex)
	{  
		m_pBaseNetwork->Send(m_ConnectionIndex,(char*)pMsg,MsgLen);
	}

}

//////////////////////////////////////////////////////////////////////////


void OnCon(DWORD dwConIndex)
{
}

void OnDisCon(DWORD dwConIndex)
{
	if(gNetwork->m_bDisconnecting == FALSE)
	{
		gNetwork->m_pCurState->OnDisconnect();
	}
	else
	{
		gNetwork->m_bDisconnecting = FALSE;
	}
}

void OnRecv(DWORD dwConIndex,char* pMsg,DWORD msglen)
{	
	ASSERT(gNetwork->m_pCurState);

	MSGROOT* pTemp = (MSGROOT*)pMsg;

	int headerSize = sizeof( MSGROOT );
	if( !gNetwork->m_crypt.Decrypt( ( char * )pTemp + headerSize, msglen - headerSize ) )
	{
		ASSERTMSG(0,"Decrypt Error");
		return;
	}
	if( pTemp->Code != gNetwork->m_crypt.GetDeCRCConvertChar() )
	{
		ASSERTMSG(0,"Decrypt CRC Error");
		return;
	}

	if(CGameState::CommonNetworkParser(pTemp->Category,pTemp->Protocol,pMsg) == FALSE)
		gNetwork->m_pCurState->NetworkMsgParse(pTemp->Category,pTemp->Protocol,pMsg);

}




