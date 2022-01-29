



// ServerTable.h: interface for the CServerTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_)
#define AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <HashTable.h>



enum eSERVER_KIND

{
	eSK_NONE,
	eSK_DIST	= 1,
	eSK_AGENT	= 2,
	eSK_MAP		= 3,
};


class CServerTable  : public CYHHashTable<SERVERINFO>
{
	DWORD			m_MaxServerConnectionIndex;
	
public:
	CServerTable();
	virtual ~CServerTable();
	void Init(DWORD dwBucketNum);
	void Release();



	SERVERINFO * FindServer(WORD Port);
	SERVERINFO* FindServerForConnectionIndex(DWORD dwConnectionIndex);
	void AddServer(SERVERINFO * info, WORD Port);
	SERVERINFO * RemoveServer(DWORD dwConnectionIndex);			

	SERVERINFO * RemoveServer(WORD wKey);
	void RemoveServerForKind(WORD wSrvKind);				
	void RemoveAllServer();									

	SERVERINFO* GetNextServer();
	SERVERINFO* GetNextDistServer();
	SERVERINFO* GetNextMapServer();
	SERVERINFO* GetNextAgentServer();

	void AddSelfServer(SERVERINFO * info);
	void AddMSServer(SERVERINFO * info);
	SERVERINFO * GetSelfServer(){ return m_pSelfServerInfo; }
	SERVERINFO * GetMSServer(){ return m_pMSServerInfo; }

	WORD GetServerPort(WORD ServerKind, WORD ServerNum);
	WORD GetServerNum(WORD ServerPort);

	DWORD GetMaxServerConnectionIndex(){return m_MaxServerConnectionIndex;}
	void SetMaxServerConnectionIndex(DWORD val){ m_MaxServerConnectionIndex=val; }	

	
	DWORD GetFastServer(WORD ServerKind);
	BOOL GetFastServer(WORD ServerKind, char* pOutIP,WORD* pOutPort);

protected:
	SERVERINFO * m_pSelfServerInfo;
	SERVERINFO * m_pMSServerInfo;
};

//extern char * SERVER_KIND_STR[];
extern CServerTable * g_pServerTable;

#endif // !defined(AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_)


