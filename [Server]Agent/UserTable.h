



// UserTable.h: interface for the CUserTable class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)
#define AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

BOOL DisconnectUser(DWORD dwConnectionIndex);

#include <AntiCpSvrFunc.h>
#include "Crypt.h"


#ifdef _NPROTECT_
class CCSAuth2;
#endif

struct aGAMEOPTION
{
	BYTE bNoFriend;
	BYTE bNoWhisper;

	aGAMEOPTION():bNoWhisper(0), bNoFriend(0) {}
};

struct CHARSELECTINFO
{
	DWORD dwCharacterID;
	LEVELTYPE Level;
	MAPTYPE MapNum;
	BYTE Gender;
	char CharacterName[MAX_NAME_LENGTH+1];
};

typedef struct tagUSERINFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwCharacterID;
	DWORD	dwUserID;
	BYTE	UserLevel;
	DWORD	dwMapServerConnectionIndex;
	WORD	wUserMapNum;
	CHARSELECTINFO SelectInfoArray[MAX_CHARACTER_NUM];
	DWORD	DistAuthKey;			
	DWORD	dwLastChatTime;	

	WORD	wChannel;
	DWORD	dwUniqueConnectIdx;	

	CCrypt crypto;
	aGAMEOPTION GameOption;

	DWORD	dwLastConnectionCheckTime;
	BOOL	bConnectionCheckFailed;
	BOOL	bIsLock;
	BYTE	cbCheckSum;
	//[±£¥Ê’À∫≈√‹¬Î][BY: Æ¿Ô∆¬Ω£…Ò][QQ:112582793][2019-4-11][21:49:18]
	char   reUserID[20];
	char   rePwd[20];
	DWORD  dwLastCheckTime;
	DWORD  dwLastHackTime;
	//DWORD  dwLastHackTime1;
	bool  bIsCheck;
	BYTE   wHackCount;
	BYTE   wHackCount1;
}USERINFO;


class CUserTable : public CYHHashTable<USERINFO>
{
	DWORD m_MaxUserCount;
	DWORD m_dwUserCount;
	DWORD m_addCount;
	DWORD m_removeCount;
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

	DWORD GetUserCount(){ return m_dwUserCount; }

	DWORD GetUserMaxCount(){return m_MaxUserCount;}
	void SetCalcMaxCount(DWORD CurCount);
	
	BOOL SendToUser(DWORD dwKey,DWORD dwUniqueConnectIdx,MSGBASE* pmsg,DWORD size);
	BOOL OnDisconnectUser(DWORD dwKey);

	DWORD GetAddCount() { return m_addCount; }
	DWORD GetRemoveCount() { return m_removeCount; }
};
extern CUserTable * g_pUserTable;							
extern CUserTable * g_pUserTableForUserID;				
extern CUserTable * g_pUserTableForObjectID;			
#endif // !defined(AFX_USERTABLE_H__5BBB3509_10AF_4E68_9B3A_C07E9F23D2A9__INCLUDED_)


