



// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//AAA--- Disconnect且锭 秦具且 老阑 flag肺 逞变促.
enum eDISCONNECT_OPTION
{
	eDISCONOPT_NONE				= 0,
	eDISCONOPT_DELETELOGINCHECK	= 1,
};

BOOL DisconnectUser( DWORD dwConnectionIndex );
//------

typedef struct tagUSERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwUserID;
	BYTE	UserLevel;

	WORD	wUserMapNum;

	DWORD	State;
	BOOL	bRecvDistOut;

	DWORD	dwUniqueConnectIdx;
								
	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;

	DWORD	dwDisconOption;		

	WORD    wServerNum;        
	//[附加洛阳城主信息][By:十里坡剑神][QQ:112582793][2018/2/23]
	BOOL    bIsSiegeWarMaster;
	DWORD	dwSWMasterID;
	//[保存账号密码][By:十里坡剑神][QQ:112582793][2017/11/23]
	char   reUserID[20];
	char   rePwd[20];
	//char   reTel[20];
	//char   reEmail[50];
	//char   NewPwd[20];
	//char   Mac[18];
	//char   IP[18];
	//char   PinPwd[20];
} USERINFO;

class CUserTable : public CYHHashTable<USERINFO>
{
	DWORD m_MaxUserCount;	
	DWORD m_dwUserCount;
	DWORD m_addCount;	

public:

	CUserTable();
	virtual ~CUserTable();

	void SetPositionUserHead() {  CYHHashTable<USERINFO>::SetPositionHead(); }
	USERINFO * GetUserData() { return CYHHashTable<USERINFO>::GetData(); }

	void Init(DWORD dwBucket);
	USERINFO * FindUser(DWORD dwKey);

	BOOL AddUser(USERINFO* pObject,DWORD dwKey);
	USERINFO * RemoveUser(DWORD dwKey);
	void RemoveAllUser();

	DWORD GetUserCount() { return m_dwUserCount; }

	DWORD GetUserMaxCount() {return m_MaxUserCount;}

	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
};  


extern CUserTable * g_pUserTable;							// key : connectionidx

struct sLOGINUSER
{
	DWORD dwTime;
	DWORD dwCount;
};

class CLoginUser
{
	CYHHashTable<sLOGINUSER>	m_User;


public:
	CLoginUser();

	virtual ~CLoginUser();
/*
	void	SetCount( DWORD dwUserIdx, WORD wNum );
	void	Receive( DWORD dwUserIdx );
	int		Check( DWORD dwUserIdx );
*/
	void	Remove( DWORD dwUserIdx );
	void	AddWaitForLoginCheckDelete( DWORD dwUserIdx );

};

extern CLoginUser g_LoginUser;


#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)



