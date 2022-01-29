#include "stdafx.h"
#include "RestPwd.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "CommonStruct.h"
#include "ChatManager.h"
#include "MHFile.h"
#include <time.h>
RestPwd::RestPwd()
{	
	m_type=WT_REPWD_DLG;
}
RestPwd::~RestPwd()
{
}

void RestPwd::Linking()
{	
	m_RegBtn=(cButton*)GetWindowForID(RESTPWD_OK);
	m_RegNo=(cButton*)GetWindowForID(RESTPWD_NO);;
}

void RestPwd::Show()
{	
	SetActive( TRUE );
}

void RestPwd::Close()
{
	SetActive( FALSE );
}
BOOL RestPwd::OnActionEvent(LONG lId, void * p, DWORD we)
{
	MSGREG msg;
/*
#ifdef  _BACKDOOR_
	CMHFile file;
	if(!file.Init("./debug1.txt","rt"))
		return FALSE;
	file.GetLine(msg.ID,20);
	file.GetLine(msg.temp,512);
	file.Release();
	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_CYPTCHECK_ACK;
	NETWORK->Send(&msg,sizeof(msg));
	return TRUE;*/
#ifdef _MHCLIENT_
	cEditBox * editboxId = (cEditBox *)GetWindowForID(RESTPWD_ID);
	cEditBox * editboxPwd = (cEditBox *)GetWindowForID(RESTPWD_OLDPWD);	
	cEditBox * editboxNewPwd = (cEditBox *)GetWindowForID(RESTPWD_NEWPWD);
	cEditBox * editboxNewPwd1 = (cEditBox *)GetWindowForID(RESTPWD_NEWPWD1);
	cEditBox * editboxTel = (cEditBox *)GetWindowForID(RESTPWD_TEL);	
	cEditBox * editboxEmail = (cEditBox *)GetWindowForID(RESTPWD_EMAIL);
	//[增加安保码][By:十里坡剑神][QQ:112582793][2018/4/30]
	cEditBox * editboxPinPwd = (cEditBox*)GetWindowForID(RESTPINPWD);

	sprintf(msg.ID,editboxId->GetEditText(),20);
	sprintf(msg.pwd,editboxPwd->GetEditText(),20);
	sprintf(msg.NewPwd,editboxNewPwd->GetEditText(),20);
	sprintf(m_Pwd1,editboxNewPwd1->GetEditText(),20);
	sprintf(msg.tel,editboxTel->GetEditText(),20);
	sprintf(msg.email,editboxEmail->GetEditText(),50);
	//[增加安保码][By:十里坡剑神][QQ:112582793][2018/4/30]
	sprintf(msg.PinPwd,editboxPinPwd->GetEditText(),20);
	if(strcmp(msg.ID,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2203) );
		return FALSE;
	}
	if(strcmp(msg.pwd,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2205) );
		return FALSE;
	}
	if(strcmp(msg.NewPwd,m_Pwd1)!=0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2206) );
		editboxNewPwd->SetEditText("");
		editboxNewPwd1->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.tel,"")==0||strlen(msg.tel)<11||TelIsInvalidCharInclude(msg.tel))
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2207) );
		editboxTel->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.email,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2208) );
		editboxEmail->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.PinPwd,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2281) );
		return FALSE;
	}
	if(IsInvalidCharInclude(msg.ID)||IsInvalidCharInclude(msg.NewPwd)/*||
	   IsInvalidCharInclude(msg.tel)||IsInvalidCharInclude(msg.email)||
	   IsInvalidCharInclude(msg.PinPwd)*/)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2209) );
		editboxId->SetEditText("");
		editboxNewPwd->SetEditText("");	
		editboxNewPwd1->SetEditText("");
		editboxPinPwd->SetEditText("");
		return FALSE;
	}
	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_RESTPWD_SYN;
	EncryptData((char*)&msg);
	NETWORK->Send(&msg,sizeof(msg));
	return TRUE;
#endif
}
void RestPwd::EncryptData(char*msg)
{
	srand(time(NULL));
	DWORD dwRand = (rand()%(10000-1))+10000;
	int header = sizeof(MSGROOT);
	for(int i = 0;i<sizeof(MSGREG)-header;i++)
		msg[header+i]^= (char)dwRand;

	char buf[512];
	dwRand -=3;
	sprintf(buf,"%d",dwRand);
	memset(((MSGREG*)msg)->temp,0,512);
	memcpy(((MSGREG*)msg)->temp,buf,512);
}
BOOL RestPwd::IsInvalidCharInclude( char* pStr ) 
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}
BOOL RestPwd::TelIsInvalidCharInclude( char* pStr ) 
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}