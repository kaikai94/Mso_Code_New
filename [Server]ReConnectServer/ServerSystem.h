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
//         佛祖保佑       永无BUG     永不修改				      //
////////////////////////////////////////////////////////////////////




// ServerSystem.h: interface for the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)
#define AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVERLISTNAME "DistributeServer_info.txt"
#define __DISTRIBUTESERVER__


#include <string>    
#include "../4dyuchigxgfunc/global.h"
#include "UserTable.h"


enum eNATION
{
	eNATION_KOREA,
	eNATION_CHINA,
};

class CServerSystem  
{

public:
	CServerSystem();

	virtual ~CServerSystem();

	void Process();
	void Start(WORD ServerNum);
	void End();		

	DWORD MakeAuthKey();
	void ReleaseAuthKey(DWORD key);
	
	void ConnectionCheck();
	void SendConnectionCheckMsg(USERINFO* pInfo);

};

extern CServerSystem * g_pServerSystem;
void __stdcall  OnConnectServerSuccess(DWORD dwIndex, void* pVoid);
void __stdcall  OnConnectServerFail(void* pVoid);

#endif // !defined(AFX_SERVERSYSTEM_H__FD3EBFC3_EE3D_4505_A5A1_24DA471D20AB__INCLUDED_)


