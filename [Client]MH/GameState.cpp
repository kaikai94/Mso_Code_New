







// GameState.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameState.h"
#include "MainGame.h"
#include "..\interface\cwindowmanager.h"
#include "windowidenum.h"
#include "cmsgbox.h"
#include "chatmanager.h"
#include "CharSelect.h"
#include "RegID.h"			//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "RestPwd.h"		//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "GameIn.h"			//[注册帐号引用][By:十里坡剑神][QQ:112582793][2017/12/5]
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND _g_hWnd;
extern BOOL g_IsExit;

CGameState::CGameState()
{
	m_bBlockScreen	= FALSE;
	m_bDisconnected = FALSE;
}

CGameState::~CGameState()
{

}
//#include "ReDisConnManager.h"
BOOL CGameState::CommonNetworkParser(BYTE Category,BYTE Protocol,void* pMsg)
{
	BOOL rt = FALSE;

	if(rt)return rt;
	switch(Category) 
	{
	case MP_HACKCHECK:
		{
			g_IsExit = TRUE;
			PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
		}
		break;
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY:
				{
					WINDOWMGR->MsgBox( MBI_OTHERUSER_CONNECTTRY, MBT_OK, CHATMGR->GetChatMsg(2) );
					rt = TRUE;
				}
				break;
			case MP_USERCONN_CONNECTION_CHECK:
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CONNECTION_CHECK_OK;


					NETWORK->Send(&msg,sizeof(msg));
					rt = TRUE;
				}
				break;
			case MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN:
				{
					WINDOWMGR->MsgBox( MBI_DISCONNECTED_OVERLAPPEDLOGIN, 
						MBT_OK, CHATMGR->GetChatMsg(3) );

					rt = TRUE;
				}
				break;
			case MP_USERCONN_CHARACTERSLOT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					rt = TRUE;
				}
				break;	
				//[账号注册消息解析][By:十里坡剑神][QQ:112582793][2017/11/23]
			case MP_USERCONN_REGUSER_ACK:
				{
					cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
					cEditBox* UserName=(cEditBox*)pIDDlg->GetWindowForID( MT_IDEDITBOX );
					cEditBox* PassWord=(cEditBox*)pIDDlg->GetWindowForID( MT_PWDEDITBOX );
					cDialog* pREDlg = WINDOWMGR->GetWindowForID( REGDLG );
					cEditBox * editboxId = (cEditBox *)pREDlg->GetWindowForID(REGID);
					cEditBox * editboxPwd = (cEditBox *)pREDlg->GetWindowForID(REGPWD);	
					cEditBox * editboxPwd1 = (cEditBox *)pREDlg->GetWindowForID(REGPWD1);
					cEditBox * editboxTel = (cEditBox *)pREDlg->GetWindowForID(REGTEL);	
					cEditBox * editboxEmail = (cEditBox *)pREDlg->GetWindowForID(REGEMAIL);
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					if(pmsg->dwData1==1)	//[账号密码非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2210) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==2)	//[账号已存在][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2211) );
						editboxId->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==3)	//[注册成功][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2212) );
						UserName->SetEditText(editboxId->GetEditText());
						PassWord->SetEditText(editboxPwd->GetEditText());
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return TRUE;
					}
					if(pmsg->dwData1==4)	//[数据库错误][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2213) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
					if(pmsg->dwData1==5)	//[手机邮箱非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2214) );
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						return FALSE;
					}
					if(pmsg->dwData1==6)	//[黑名单用户][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2215) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
					if(pmsg->dwData1 ==7)		//[注册关闭][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2216) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
					if(pmsg->dwData1==8)//[注册信息不全][By:十里坡剑神][QQ:112582793][2019/4/1][12:19:24]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2337) );
						editboxId->SetEditText("");
						editboxPwd->SetEditText("");
						editboxPwd1->SetEditText("");
						editboxTel->SetEditText("");
						editboxEmail->SetEditText("");
						GAMEIN->GetRegID()->Close();
						return FALSE;
					}
				}
				break;
				//[修改密码应答协议处理][By:十里坡剑神][QQ:112582793][2017/11/23]
			case MP_USERCONN_RESTPWD_ACK:   
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					if(pmsg->dwData1==1)	//[账号不存在][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2217) );
					}
					else if(pmsg->dwData1==2)	//[原密码错误][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2218) );
					}
					else if(pmsg->dwData1==3)	//[密保邮箱错误][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2219) );
					}
					else if(pmsg->dwData1==4)	//[密保手机错误][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2220) );
					}
					else if(pmsg->dwData1==5)	//[新密码非法字符][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2221) );
					}
					else if(pmsg->dwData1==6)	//[数据库查询错误][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2222) );
					}
					else if(pmsg->dwData1==7)	//[修改成功][By:十里坡剑神][QQ:112582793][2017/11/23]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2223) );
						GAMEIN->GetRestPwdDlg()->Close();
					}
					else if(pmsg->dwData1==8)//[安保密码错误][By:十里坡剑神][QQ:112582793][2018/4/30]
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2283) );
					}
				}
				break;
			}
		}
		break;


#ifdef _HACK_SHIELD_
	case MP_HACKSHIELD:
		{

			HACKSHIELDMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}
		break;
#endif
#ifdef _NPROTECT_
	case MP_NPROTECT:
		{
			NPROTECTMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}


		break;

#endif
	}


	return rt;
}

void CGameState::SetBlock(BOOL val)
{
	m_bBlockScreen = val;
}

void CGameState::OnDisconnect()
{
	m_bDisconnected = TRUE;
	if( cMsgBox::IsInit() )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 448 ) );
	}
	else
	{
		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}




