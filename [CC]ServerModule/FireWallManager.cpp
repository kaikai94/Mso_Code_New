#include "stdafx.h"
#include "FireWallManager.h"
#include "Network.h"
#include "Console.h"
#include "shellapi.h"
FireWallManager* FireWallManager::m_Instance;
FireWallManager::FireWallManager(void)
{
	m_Instance = NULL;
}

FireWallManager::~FireWallManager(void)
{
	if(!m_Instance)
		delete m_Instance;
	m_Instance = NULL;
}
void FireWallManager::RemoveCheckInfo(DWORD dwConnectionIndex)
{
	std::map<DWORD,CHECKINFO>::iterator iter;
	iter = m_NotInitUserTable.find(dwConnectionIndex);
	if(iter!=m_NotInitUserTable.end())
		m_NotInitUserTable.erase(iter);
}
void FireWallManager::RmoveProtocolCheck(DWORD dwConnectionIndex)
{
	std::map<DWORD,WORD>::iterator iter;
	iter = m_ProtocolCheckTable.find(dwConnectionIndex);
	if(iter!=m_ProtocolCheckTable.end())
		m_ProtocolCheckTable.erase(iter);
}
BOOL FireWallManager::CheckProtocol(DWORD dwConnectionIndex)
{
	char IP[16]= {0,};
	WORD port = 0;
	g_Network.GetUserAddress(dwConnectionIndex,IP,&port);
	std::map<DWORD,WORD>::iterator iter;
	iter = m_ProtocolCheckTable.find(dwConnectionIndex);
	if(iter!=m_ProtocolCheckTable.end())
	{
		if (iter->second>=1)
		{
			char buf[512];
			memset(buf,0,512);
			sprintf(buf,"/c netsh ipsec static add filter filterlist=关闭的 srcaddr=%s dstaddr=Me dstport=0 protocol=any",IP);
			ShellExecute(NULL,"runas","cmd.exe",buf,NULL,SW_HIDE);
			memset(buf,0,512);
			sprintf(buf,"[协议检查]IP[%s]已被添加到黑名单",IP);
			g_Console.LOGEX(4,RGB(255,0,0),buf);
			return TRUE;
		}
		else
		{
			iter->second++;
			return FALSE;
		}
	}
	m_ProtocolCheckTable[dwConnectionIndex] = 0;
}
void FireWallManager::ClearDisableUser()
{
	if(m_NotInitUserTable.size()<=0)
		return;
	std::map<DWORD,CHECKINFO>::iterator iter;
	iter = m_NotInitUserTable.begin();
	while(1)
	{
		if(iter==m_NotInitUserTable.end())
			break;
		else if(iter->second.bIsdIsable = TRUE)
			iter = m_NotInitUserTable.erase(iter);
		else
			iter++;
	}
}
BOOL FireWallManager::CheckConnectMsg(DWORD dwConnectionIndex)
{
	char IP[16]= {0,};
	WORD port = 0;
	g_Network.GetUserAddress(dwConnectionIndex,IP,&port);
	std::map<DWORD,CHECKINFO>::iterator iter;
	iter = m_NotInitUserTable.find(dwConnectionIndex);
	if(iter!=m_NotInitUserTable.end())
	{
		if(iter->second.wConnectTimes>=1)
		{
			if(!iter->second.bIsdIsable)
			{
				char buf[512];
				memset(buf,0,512);
				sprintf(buf,"/c netsh ipsec static add filter filterlist=关闭的 srcaddr=%s dstaddr=Me dstport=0 protocol=any",IP);
				ShellExecute(NULL,"runas","cmd.exe",buf,NULL,SW_HIDE);
				memset(buf,0,512);
				sprintf(buf,"[连接检查]IP[%s]已被添加到黑名单",IP);
				g_Console.LOGEX(4,RGB(255,0,0),buf);
				iter->second.bIsdIsable = TRUE;
			}
			return TRUE;
		}
		else
		{
			iter->second.wConnectTimes++;
			return FALSE;
		}
	}
	CHECKINFO info;
	info.strIP = IP;
	info.wConnectTimes = 1;
	info.bIsdIsable = FALSE;
	m_NotInitUserTable[dwConnectionIndex] = info;
	return FALSE;
}

FireWallManager * FireWallManager::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new FireWallManager;
	}
	return m_Instance;
}