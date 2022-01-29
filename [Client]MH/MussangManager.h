




// MussangManager.h: interface for the CMussangManager class.
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)

#define AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MUSSANG_VALID_TIME		1000*60*10



#define MUSSANGMGR	USINGTON(CMussangManager)

enum eMussangStates{ eMSS_None=1, eMSS_Ready=2, eMSS_InMode=4, eMSS_LevelUp=8};


class CMussangManager  
{
	DWORD				m_dwMussangPoint;
	DWORD				m_dwMaxMussangPoint;

	BOOL				m_bIsMussangMode;
	BOOL				m_bIsMussangReady;
	BOOL				m_bValidBTNforSendMsg;	
	BOOL				m_bMussang;
	WORD				m_wMussangStat;

public:
	CMussangManager();
	virtual ~CMussangManager();

	void Init();
	void SetMussangPoint(DWORD dwMussangPoint);
	void SetMussangMaxPoint(DWORD dwPoint) { m_dwMaxMussangPoint = dwPoint;}
	void SetMussangReady(BOOL bVal);
	void SetMussangMode(BOOL bVal, DWORD time = 0);
	BOOL IsMussangReady() {return m_bIsMussangReady;}
	void SendMsgMussangOn();

	void SetValidMsg(BOOL bValid) {m_bValidBTNforSendMsg = bValid;}

	BOOL IsMussangMode() { return m_bMussang; }

	WORD GetMussangStat() { return m_wMussangStat; }
};
EXTERNGLOBALTON(CMussangManager)
#endif // !defined(AFX_MUSSANGMANAGER_H__4F0BBCDB_B978_4804_A05F_F72B21DF285F__INCLUDED_)


