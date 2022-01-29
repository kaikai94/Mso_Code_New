










// ShoutchatDialog.cpp: implementation of the CShoutchatDialog class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"

#include "ShoutchatDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cListDialog.h"


#include "GameIn.h"
#include "ChatDialog.h"
#include "GameResourceManager.h"
#include "GMNotifyManager.h"
static const DWORD SuperMsgColor[2] = {0xffa300,0xff1600/*,0xe400ff,0xffde00*/};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShoutchatDialog::CShoutchatDialog()
{

	m_type = WT_SHOUTCHAT_DLG;
	m_LastMsgTime = 0;
	m_SuperMsgTime = 0;
	SafeStrCpy(m_SuperMsgBuf[0],"",60);
	SafeStrCpy(m_SuperMsgBuf[1],"",60);
	SafeStrCpy(m_SuperMsgBuf[2],"",60);
}

CShoutchatDialog::~CShoutchatDialog()
{


}


void CShoutchatDialog::Linking()
{
	m_pMsgListDlg = (cListDialog*)GetWindowForID( CHA_LIST );
	m_pSuperMsgListDlg = (cListDialog*)GetWindowForID( CHA_SUPLIST );
	m_pMsgListDlg->SetRenderFaceImg(TRUE); 
	m_pSuperMsgListDlg->SetRenderFaceImg(TRUE);
	//m_pSuperMsgListDlg->SetAlwaysTop(TRUE);
	//m_pSuperMsgListDlg->SetAbsY(m_pSuperMsgListDlg->GetHeight()-m_pMsgListDlg->GetHeight());
	if( GAMERESRCMNGR->GetPrintScreenIndex()==0)
	{
		GAMEIN->GetShoutchatDlg()->RefreshPosition();
	}
}


void CShoutchatDialog::Process()
{
	if( gCurTime - m_LastMsgTime < 5000 )
		return;
	
	m_LastMsgTime = gCurTime;
}

void CShoutchatDialog::SetActive( BOOL val )
{
	if( val )
		RefreshPosition();
	cDialog::SetActive( val );
}
void CShoutchatDialog::SuperShoutProcess()
{
	if(strlen(m_SuperMsgBuf[2])==0)return;
	if(gCurTime-m_SuperMsgTime>=200)
		AddSuperMsg(m_SuperMsgBuf[2]);
	if(gCurTime-m_SuperMsgTime1>=4000)
	{
		strncpy(m_SuperMsgBuf[0],m_SuperMsgBuf[1],60);
		strncpy(m_SuperMsgBuf[1],m_SuperMsgBuf[2],60);
		strncpy(m_SuperMsgBuf[2],"",60);
	}
}
void CShoutchatDialog::AddSuperMsg(char* str)
{
	char buf[3][61] = { 0, };
	strncpy( buf[2], str, 60 );
	if(m_SuperColorIdx<1&&m_SuperColorIdx>=0)
		m_SuperColorIdx++;
	else
		m_SuperColorIdx = 0;
	strncpy(buf[0],m_SuperMsgBuf[0],60);
	strncpy(buf[1],m_SuperMsgBuf[1],60);
	
	if(strcmp(buf[2],m_SuperMsgBuf[2])!=0)
	{
		strncpy( m_SuperMsgBuf[2], str, 60 );
		NOTIFYMGR->AddMsg( buf[2],8);
		m_SuperMsgTime1 = gCurTime;
	}
	if( m_pSuperMsgListDlg )
	{
		m_pSuperMsgListDlg->AddItem(buf[0],SuperMsgColor[0]);
		m_pSuperMsgListDlg->AddItem(buf[1],SuperMsgColor[0]);
		m_pSuperMsgListDlg->AddItem( buf[2], SuperMsgColor[m_SuperColorIdx] );
		m_SuperMsgTime = gCurTime;
	}
}
void CShoutchatDialog::AddMsg( char* pstr )
{
	char buf[61] = { 0, };
	strncpy( buf, pstr, 60 );
	
	if( m_pMsgListDlg )
		m_pMsgListDlg->AddItem( buf, RGBA_MAKE(217, 206, 247, 255) );
}



void CShoutchatDialog::RefreshPosition()
{

	float absX = GAMEIN->GetChatDialog()->GetAbsX();

	float absY = GAMEIN->GetChatDialog()->GetSheetPosY();

	SetAbsXY( (LONG)absX, (LONG)absY-GetHeight() );
	m_pSuperMsgListDlg->SetAbsY((LONG)absY-(GetHeight()+59));
}




