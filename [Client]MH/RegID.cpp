#include "stdafx.h"
#include "RegID.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameResourceManager.h"
#include "cWindowManager.h"
#include "CommonStruct.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "MHFile.h"
#include "cMsgBox.h"
#include <time.h>
#include <fstream>
RegID::RegID()
{	
	m_type=WT_REG_DLG;
}
RegID::~RegID()
{
}

void RegID::Linking()
{	
	m_RegBtn=(cButton*)GetWindowForID(REGOK);
	m_RegNo=(cButton*)GetWindowForID(REGNO);;
}

void RegID::Show()
{	
	SetActive( TRUE );
}

void RegID::Close()
{
	SetActive( FALSE );
}

BOOL RegID::OnActionEvent(LONG lId, void * p, DWORD we)
{
	MSGREG msg;
	memset(&msg,0,sizeof(MSGREG));
	cEditBox * editboxId = (cEditBox *)GetWindowForID(REGID);
	cEditBox * editboxPwd = (cEditBox *)GetWindowForID(REGPWD);	
	cEditBox * editboxPwd1 = (cEditBox *)GetWindowForID(REGPWD1);
	cEditBox * editboxTel = (cEditBox *)GetWindowForID(REGTEL);	
	cEditBox * editboxEmail = (cEditBox *)GetWindowForID(REGEMAIL);
	//[增加安全码验证][By:十里坡剑神][QQ:112582793][2018/4/30]
	cEditBox * editboxPin	= (cEditBox *)GetWindowForID(REGPINPWD);
	cEditBox * editboxPin1	= (cEditBox *)GetWindowForID(REGPINPWD1);

	sprintf(msg.ID,editboxId->GetEditText(),20);
	sprintf(msg.pwd,editboxPwd->GetEditText(),20);
	sprintf(m_Pwd1,editboxPwd1->GetEditText(),20);
	sprintf(msg.tel,editboxTel->GetEditText(),20);
	sprintf(msg.email,editboxEmail->GetEditText(),50);
	//[增加安全码验证][By:十里坡剑神][QQ:112582793][2018/4/30]
	sprintf(msg.PinPwd,editboxPin->GetEditText(),20);
	sprintf(msg.rPinPwd,editboxPin1->GetEditText(),20);
	if(strcmp(msg.ID,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2203) );
		return FALSE;
	}
	if(strlen(msg.ID)<6||strlen(msg.pwd)<6||strlen(msg.ID)>19||strlen(msg.pwd)>19)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2204) );
		return FALSE;
	}
	if(strcmp(msg.pwd,"")==0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2205) );
		return FALSE;
	}
	if(strcmp(msg.pwd,m_Pwd1)!=0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2206) );
		editboxPwd->SetEditText("");
		editboxPwd1->SetEditText("");
		return FALSE;
	}
	if(strcmp(msg.tel,"")==0||strlen(msg.tel)<8||TelIsInvalidCharInclude( msg.tel ) )
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
	if(strcmp(msg.PinPwd,msg.rPinPwd)!=0)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2281) );
		return FALSE;
	}
	if(IsInvalidCharInclude(msg.ID)||IsInvalidCharInclude(msg.pwd)/*||
		IsInvalidCharInclude(msg.PinPwd)||IsInvalidCharInclude(msg.rPinPwd)||
		IsInvalidCharInclude(msg.tel)||IsInvalidCharInclude(msg.email)*/)
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2209) );
		editboxId->SetEditText("");
		editboxPwd->SetEditText("");	
		editboxPwd1->SetEditText("");
		return FALSE;
	}
	//if(GAMEIN->GetMacPlus(msg.Mac)!=1)
	//{
	//	cMsgBox * MsgBox = WINDOWMGR->MsgBox(MBI_OK, MBT_OK,CHATMGR->GetChatMsg(2261));
	//	return FALSE;
	//}

	GAMEIN->GetHWID(msg.Mac);
	sprintf(msg.NewPwd,GAMEIN->GetInternetIP().c_str());

	msg.Category=MP_USERCONN;
	msg.Protocol=MP_USERCONN_REGUSER_SYN;
	EncryptData((char*)&msg);
	NETWORK->Send(&msg,sizeof(msg));
	return TRUE;
}
void RegID::EncryptData(char* msg)
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
BOOL RegID::IsInvalidCharInclude( char* pStr ) 
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
BOOL RegID::TelIsInvalidCharInclude( char* pStr ) 
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
