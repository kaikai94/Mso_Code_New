#pragma once
#include <map>
#include <string>
struct CHECKINFO
{
	std::string strIP;
	WORD	wConnectTimes;
	BOOL	bIsdIsable;
};
class FireWallManager
{
	std::map<DWORD,CHECKINFO> m_NotInitUserTable;
	std::map<DWORD,WORD> m_ProtocolCheckTable;
public:
	FireWallManager(void);
	~FireWallManager(void);
	BOOL CheckConnectMsg(DWORD dwConnectionIndex);
	void RemoveCheckInfo(DWORD dwConnectionIndex);
	static FireWallManager* m_Instance;
	static FireWallManager * GetInstance();
	BOOL CheckProtocol(DWORD dwConnectionIndex);
	void RmoveProtocolCheck(DWORD dwConnectionIndex);
	void ClearDisableUser();
};
#define FIREWALLMGR FireWallManager::GetInstance()