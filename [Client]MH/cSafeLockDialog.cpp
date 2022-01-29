#include "stdafx.h"
#include "cSafeLockDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cStatic.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
#include "cMainDlgSafeBtn.h"
#include "GameIn.h"
#include "ReConnect.h"
cSafeLockDialog::cSafeLockDialog(void)
{
	m_pStPwd			= NULL;
	m_pStNewPwd			= NULL;
	m_pStCheckNewPwd	= NULL;
	m_btnOK				= NULL;
	m_edPwd				= NULL;
	m_edNewPwd			= NULL;
	m_edCheckNewPwd		= NULL;
	m_IsRestPwd	 = FALSE;
	m_ChangeDialog = FALSE;
}

cSafeLockDialog::~cSafeLockDialog(void)
{

}
void cSafeLockDialog::Linking()
{
	m_pStPwd			= new cStatic;
	if(m_pStPwd)
	{
		m_pStPwd->Init(101,33,0,0,NULL,-1);
		m_pStPwd->SetShadow(TRUE);
		m_pStPwd->SetFontIdx(2);
		m_pStPwd->SetAlign(TXT_CENTER);
		m_pStPwd->SetStaticText(CHATMGR->GetChatMsg(2285));
		Add(m_pStPwd);
	}
	m_pStNewPwd			= new cStatic;
	if(m_pStNewPwd)
	{
		m_pStNewPwd->Init(101,74,0,0,NULL,-1);
		m_pStNewPwd->SetShadow(TRUE);
		m_pStNewPwd->SetFontIdx(2);
		m_pStNewPwd->SetAlign(TXT_CENTER);
		m_pStNewPwd->SetStaticText(CHATMGR->GetChatMsg(2286));
		Add(m_pStNewPwd);
	}
	m_pStCheckNewPwd	= new cStatic;
	if(m_pStCheckNewPwd)
	{
		m_pStCheckNewPwd->Init(101,112,0,0,NULL,-1);
		m_pStCheckNewPwd->SetShadow(TRUE);
		m_pStCheckNewPwd->SetFontIdx(2);
		m_pStCheckNewPwd->SetAlign(TXT_CENTER);
		m_pStCheckNewPwd->SetStaticText(CHATMGR->GetChatMsg(2287));
		Add(m_pStCheckNewPwd);
	}
	m_btnOK				= (cButton*)GetWindowForID(SAFELOCK_ULOCK_BTN);
	m_edPwd				= (cEditBox*)GetWindowForID(SAFELOCK_EDIT_PWD);
	m_edNewPwd			= (cEditBox*)GetWindowForID(SAFELOCK_EDIT_NEWPWD);
	m_edCheckNewPwd		= (cEditBox*)GetWindowForID(SAFELOCK_EDIT_CHECKNEWPWD);

	DWORD dwColor = RGB(255,255,255);
	m_pStNewPwd->SetActive(FALSE);
	m_pStCheckNewPwd->SetActive(FALSE);
	m_edNewPwd->SetActive(FALSE);
	m_edCheckNewPwd->SetActive(FALSE);
	m_btnOK->SetText(CHATMGR->GetChatMsg(2288),dwColor,dwColor,dwColor);
}
void cSafeLockDialog::Show(BOOL IsRestPwd)
{
	DWORD dwColor = RGB(255,255,255);
	if(IsRestPwd)
	{
		m_pStNewPwd->SetActive(TRUE);
		m_pStCheckNewPwd->SetActive(TRUE);
		m_edNewPwd->SetActive(TRUE);
		m_edCheckNewPwd->SetActive(TRUE);
		m_btnOK->SetText(CHATMGR->GetChatMsg(2289),dwColor,dwColor,dwColor);
		if(m_ChangeDialog)
		{
			m_pStPwd->SetAbsY(m_pStPwd->GetAbsY()-30);
			m_edPwd->SetAbsY(m_edPwd->GetAbsY()-40);
			m_btnOK->SetAbsY(m_btnOK->GetAbsY()+10);
			m_ChangeDialog = FALSE;
		}
	}
	else
	{
		m_pStNewPwd->SetActive(FALSE);
		m_pStCheckNewPwd->SetActive(FALSE);
		m_edNewPwd->SetActive(FALSE);
		m_edCheckNewPwd->SetActive(FALSE);
		m_btnOK->SetText(CHATMGR->GetChatMsg(2288),dwColor,dwColor,dwColor);
		if(!m_ChangeDialog)
		{
			m_pStPwd->SetAbsY(m_pStPwd->GetAbsY()+30);
			m_edPwd->SetAbsY(m_edPwd->GetAbsY()+40);
			m_btnOK->SetAbsY(m_btnOK->GetAbsY()-10);
			m_ChangeDialog = TRUE;
		}
	}
	m_IsRestPwd = IsRestPwd;
	SetActive(TRUE);
}
void cSafeLockDialog::Close(BOOL IsOpen)
{
	if(m_ChangeDialog)
	{
		m_pStPwd->SetAbsY(m_pStPwd->GetAbsY()-30);
		m_edPwd->SetAbsY(m_edPwd->GetAbsY()-40);
		m_btnOK->SetAbsY(m_btnOK->GetAbsY()+10);
		m_ChangeDialog = FALSE;
	}

	m_IsRestPwd = FALSE;
	if(IsOpen)
	{
		GAMEIN->GetMainDlgSafeBtn()->Show(IsOpen);
		GAMEIN->GetMainDlgSafeBtn()->SetSafeLockState(IsOpen);
	}
	SetActive(FALSE);
}
void cSafeLockDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	MSG_SAFELOCK msg;
	memset(&msg,0,sizeof(msg));
	msg.Category = MP_SAFELOCK;
	msg.Protocol = MP_SAFELOCK_UNLOCK_SYN;
	msg.dwObjectID = HEROID;
	SafeStrCpy(msg.Pwd,m_edPwd->GetEditText(),20);
	REDISCONNECT->OnOpenSafeLock(msg.Pwd);
	if(strlen(msg.Pwd)==0)
	{//[未输入安全码][By:十里坡剑神][QQ:112582793][2018/4/30]
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2281) );
		return;
	}
	if(m_IsRestPwd)
	{
		msg.Protocol = MP_SAFELOCK_RESTPWD_SYN;
		SafeStrCpy(msg.NewPwd,m_edNewPwd->GetEditText(),20);
		if(strlen(msg.NewPwd)==0)
		{//[未输入新安全码][By:十里坡剑神][QQ:112582793][2018/4/30]
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2281) );
			return;
		}
		if(strcmp(msg.NewPwd,m_edCheckNewPwd->GetEditText())!=0)
		{//[两次输入的新安全码不一致][By:十里坡剑神][QQ:112582793][2018/4/30]
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2282) );
			return;
		}
		REDISCONNECT->OnOpenSafeLock(msg.NewPwd);
	}
	NETWORK->Send(&msg,sizeof(msg));
	m_edPwd->SetEditText("");
	m_edNewPwd->SetEditText("");
	m_edCheckNewPwd->SetEditText("");
}