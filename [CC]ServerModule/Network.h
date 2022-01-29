




// Network.h: interface for the CNetwork class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__19C7FECD_93EA_4809_9B83_4E0330849642__INCLUDED_)
#define AFX_NETWORK_H__19C7FECD_93EA_4809_9B83_4E0330849642__INCLUDED_

#if _MSC_VER > 1000
#pragma once


#endif // _MSC_VER > 1000


extern BOOL g_bReady;

#include "inetwork.h"
//#include "ServerSystem.h"
#ifdef __AGENTSERVER__
#include "UserTable.h"
#endif




class CNetwork  
{
	I4DyuchiNET* m_pINet;
public:
	CNetwork();
	virtual ~CNetwork();
	
	I4DyuchiNET* GetINet()	{	return m_pINet;	}

	BOOL Init(DESC_NETWORK * desc);
	void Release();

	BOOL StartServerServer(char * szIP, int port);

	BOOL StartUserServer(char * szIP, int port);
	BOOL ConnectToServer(char * szIP, int port, void * pParam);

	void GetUserAddress(DWORD dwConnectionIndex, char* ip, WORD * port);
	

	//--------- send, recv process --------------------------------
	void Send2Server(DWORD dwConnectionIndex, char * msg, DWORD size);
	void Send2AgentServer(char * msg, DWORD size);
	void SendAll2AgentServer(char * msg, DWORD size);
	void Send2User(DWORD dwConnectionIndex, char * msg, DWORD size);
	
	void BroadCastDisServer(char * msg, DWORD size);
	void Broadcast2Server(char * msg, DWORD size);
	void Broadcast2User(MSGBASE * msg, DWORD size);
	void Broad2User(MSGBASE* msg, DWORD size);
	void Broadcast2MapServer(char * msg, DWORD size);
	void Broadcast2MapServerExceptOne(DWORD dwConnectionIndex, char * msg, DWORD size);
	void Broadcast2AgentServer(char* msg, DWORD size);
	void Broadcast2AgentServerExceptOne(DWORD dwConnectionIndex, char* msg, DWORD size);
	void Broadcast2AgentServerExceptSelf(char* msg, DWORD size);

	void DisconnectUser(DWORD dwConnectionIndex);
#ifdef __AGENTSERVER__
	void Send2User(MSGBASE * msg, DWORD size);
	void EnCrypt(USERINFO * userInfo, char* msg,DWORD size);
#endif
};

extern CNetwork g_Network;
#endif // !defined(AFX_NETWORK_H__19C7FECD_93EA_4809_9B83_4E0330849642__INCLUDED_)


