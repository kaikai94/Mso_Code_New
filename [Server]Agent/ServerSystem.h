////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\  =  /O                              //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//             ."" '<  `.___\_<|>_/___.'  >'"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
//     ========`-.____`-.___\_____/___.-`____.-'========          //
//                          `=---='                               //
//     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^         //
//         ���汣��       ����BUG     �����޸�					  //
////////////////////////////////////////////////////////////////////



// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERLISTNAME "Agentserver_info.txt"

#define __AGENTSERVER__
#include "ServerTable.h"
#include "UserTable.h"
#include "..\4dyuchigxgfunc/global.h"
#include <map>
#include <string>
//--- max user&server define ------
#define MAX_USER_IN_A_AGENT		4000
#define MAX_SERVER_CONNECTION	100
//---------------------------------

typedef struct _MAPCHANGE_INFO
{
	DWORD Kind;
	char CurMapName[MAX_NAME_LENGTH];
	char ObjectName[MAX_NAME_LENGTH];
	WORD CurMapNum;
	WORD MoveMapNum;
	VECTOR3 CurPoint;
	VECTOR3 MovePoint;
	WORD chx_num;
}MAPCHANGE_INFO;

struct ROBOTINFO
{
	DWORD dwUserID;
	DWORD dwMapNum;
	char  Name[MAX_NAME_LENGTH+1];
	DWORD dwConnectionIndex;
	std::string strMac;
	std::string strIP;
};
enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};


class CServerSystem  
{
	MAPCHANGE_INFO	m_MapChange[MAX_POINT_NUM];

	WORD			m_wServerNum;
	
	BOOL			m_bUseCrypt;

	BOOL			m_bTestServer;
	int		m_Nation;

	char	m_strEventNotifyTitle[32];
	char	m_strEventNotifyContext[128];

	BOOL	m_bUseEventNotify;
	BOOL	m_bEvenList[eEvent_Max];
	BOOL	m_CloseClient;
	BOOL	m_CloseServer;
	DWORD   m_dwCheckTime;
public:
	CServerSystem();
	virtual ~CServerSystem();
	
	WORD GetServerNum() { return m_wServerNum; }
	void ConnectionCheck();
	void SendConnectionCheckMsg(USERINFO* pInfo);

	void Process();
	void Start(WORD ServerNum);
	void End();		

	void SetMapChangeInfo();
	MAPCHANGE_INFO* GetMapChangeInfo(DWORD Index);
	
	void SendMsgMapServerDown( WORD wServerNum );
	void SetEnableCrypt( BOOL bEnable )		{ m_bUseCrypt = bEnable; }
	BOOL IsEnableCrypt()					{ return m_bUseCrypt; }

	void SetNation();
	int GetNation() { return m_Nation; }

	BOOL IsTestServer() { return m_bTestServer; }
	
	void SetEventNotifyStr( char* pStrTitle, char* pStrContext );
	void SetUseEventNotify( BOOL bUse );
	BOOL IsUseEventNotify()			{ return m_bUseEventNotify; }

	char* GetEventNotifyTitle()		{ return m_strEventNotifyTitle; }

	char* GetEventNotifyContext()	{ return m_strEventNotifyContext; }
	void ResetApplyEvent()			{	memset( m_bEvenList, 0, sizeof(BOOL)*eEvent_Max );	}
	void SetApplyEvent( DWORD EventIdx )	{	m_bEvenList[EventIdx] = TRUE;	}

	BOOL* GetApplyEvent()	{	return m_bEvenList;	}
	DWORD MakeAuthKey();
	void ReleaseAuthKey(DWORD key);

	void SetCloseClient(){ m_CloseClient = !m_CloseClient; }
	void SetCloseServer(){ m_CloseServer = !m_CloseServer; }
	BOOL GetCloseClient(){ return m_CloseClient; }
	BOOL GetCloseServer(){ return m_CloseServer; }
};



extern CServerSystem * g_pServerSystem;
void _stdcall OnConnectServerSuccess(DWORD dwIndex, void* pVoid);

void _stdcall OnConnectServerFail(void* pVoid);

void _stdcall ProcessDBMessage(DWORD dwEventIndex);
void _stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void _stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

void _stdcall OnAcceptServer(DWORD dwConnectionIndex);
void _stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void _stdcall OnAcceptUser(DWORD dwConnectionIndex);
void _stdcall OnDisconnectUser(DWORD dwConnectionIndex);


void _stdcall GameProcess(DWORD dwEventIndex);

void ButtonProc1();
void OnCommand(char* szCommand);

#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)


