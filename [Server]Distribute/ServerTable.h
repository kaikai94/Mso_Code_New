





// ServerTable.h: interface for the CServerTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_)

#define AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//--------------------------------------------------------------------------
// ¼­¹ö Á¤º¸¸¦ °¡Áö°í ÀÖ´Â Å¬·¡½º
// Á¤º¸¸¦ ·ÎµåÇÏÁö ¾Ê´Â´Ù.
// ¼­¹ö°ü·Ã½ºÅ¸Æ®µµ ÇÏÁö ¾Ê´Â´Ù.
//--------------------------------------------------------------------------
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
/*
#ifdef __AGENTSERVER__

	WORD				m_MapIDMapNum[MAX_MAP_NUM];		// MapIDµî·ÏÇÏ´Â º¯¼ö.
#endif //__AGENTSERVER__*/


	//DWORD		m_dwMaxBucketNum;
	DWORD		m_MaxServerConnectionIndex;

	int			m_nDisableAgent[8];

	
public:
	CServerTable();
	virtual ~CServerTable();
	void Init(DWORD dwBucketNum);
	void Release();

	void SetDisableAgent( int nAgentNum )
	{
		if( nAgentNum >= 0 && nAgentNum < 8 )
			m_nDisableAgent[nAgentNum] = TRUE;
	}

	SERVERINFO * FindServer(WORD Port);

	SERVERINFO* FindServerForConnectionIndex(DWORD dwConnectionIndex);
	void AddServer(SERVERINFO * info, WORD Port);
	SERVERINFO * RemoveServer(DWORD dwConnectionIndex);		
	SERVERINFO * RemoveServer(WORD wKey);
	void RemoveAllServer();										

	SERVERINFO* GetNextServer();
	SERVERINFO* GetNextDistServer();
	SERVERINFO* GetNextMapServer();
	SERVERINFO* GetNextAgentServer();
	
	void AddSelfServer(SERVERINFO * info);

	void AddMSServer(SERVERINFO * info);
	SERVERINFO * GetSelfServer(){ return m_pSelfServerInfo; }
	SERVERINFO * GetMSServer(){ return m_pMSServerInfo; }

	
	SERVERINFO* GetServer(WORD ServerKind, WORD ServerNum);

	WORD GetServerPort(WORD ServerKind, WORD ServerNum);
	WORD GetServerNum(WORD ServerPort);

	DWORD GetMaxServerConnectionIndex(){return m_MaxServerConnectionIndex;}
	void SetMaxServerConnectionIndex(DWORD val){ m_MaxServerConnectionIndex=val; }	
	
	SERVERINFO* GetFastServer(WORD ServerKind,WORD SeverNum);
	BOOL GetFastServer(WORD ServerKind, char* pOutIP,WORD* pOutPort);

protected:
	SERVERINFO * m_pSelfServerInfo;
	SERVERINFO * m_pMSServerInfo;
};
extern CServerTable * g_pServerTable;

#endif // !defined(AFX_SERVERTABLE_H__C42B519F_767C_4291_B2DA_5F077251C100__INCLUDED_)


