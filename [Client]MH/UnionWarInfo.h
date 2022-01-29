#pragma once
#define cUNIONWARMGR USINGTON(UnionWarInfo)
class UnionWarInfo
{
	CYHHashTable<sUnionWarInfo> m_UnionInfo;
	DWORD m_dwKillTimes;
	DWORD m_dwDieTimes;
	BOOL m_bIsWarStart;
	DWORD m_UnionMapNum;
	eUnionWarState	m_dwWarState;
	DWORD m_dwWarTime;
public:
	UnionWarInfo();
	~UnionWarInfo();

	void NetworkMsgParse(BYTE Protocol, void* pMsg);

	void InitUnionWarInfo(UNIONWARINFO* pInfo);
	void Release();
	BOOL IsUnionWarStart(){ return m_bIsWarStart; }

	DWORD GetUnionMapNum(){ return m_UnionMapNum; }

	void EndUnionWarInfo();

	sUnionWarInfo * GetUnionWarInfo(DWORD dwUnionID);

	CYHHashTable<sUnionWarInfo>& GetUnionWarInfoTable(){ return m_UnionInfo; }
};

EXTERNGLOBALTON(UnionWarInfo)