







// ShoutDialog.cpp: implementation of the CShoutDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ShoutDialog.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "ChatManager.h"

#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "FilteringTable.h"
#include "cScriptManager.h"                // 2014-12-15 表情资源使用头
#include "cButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////

CShoutDialog::CShoutDialog()
{
	m_type = WT_SHOUT_DLG;
	m_dwItemIdx = m_dwItemPos = 0;
	bIsSuperMsg = FALSE;
}

CShoutDialog::~CShoutDialog()


{


}


void CShoutDialog::Linking()
{

	m_pMsgBox = (cEditBox*)GetWindowForID( CHA_MSG );

	m_ShoutOk = (cButton*)GetWindowForID(CHA_CREATEOKBTN);

	m_SuperShoutOk = (cButton*)GetWindowForID(CHA_CREATEOKBTNSUPER);

	m_FaceBtn = (cButton*)GetWindowForID(CHA_BTN_IMAGE);

	m_ImageGrid =(cIconGridDialog*)GetWindowForID(CHA_ICONGRID_IMAGE);

	m_pMsgBox->SetRenderFaceImg(TRUE);  

	LoadFaceList(); 
	m_SuperShoutOk->SetActive(FALSE);

	m_ShoutOk->SetActive(FALSE);

}


void CShoutDialog::LoadFaceList()
{
	// 2014-12-15 表情到内存!

	for(int i=0;i<CHARFACELIST; i++)
	{
		SCRIPTMGR->GetImage(i+FACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);

		cIcon * pIcon;

		pIcon=new cIcon;

		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+8000+i);

		pIcon->SetData(i+FACELISTSTARTIDX);

		pIcon->SetDisable(TRUE);

		pIcon->SetDepend(FALSE);

		m_ImageGrid->AddIcon(i,pIcon);
	}

	m_ImageGrid->SetActive(FALSE);

}
void CShoutDialog::Show(BOOL val)
{
	if(val)
	{
		m_ShoutOk->SetActive(FALSE);
		m_SuperShoutOk->SetActive(TRUE);
		m_FaceBtn->SetActive(TRUE);
	}
	else
	{
		m_ShoutOk->SetActive(TRUE);
		m_SuperShoutOk->SetActive(FALSE);
		m_FaceBtn->SetActive(TRUE);
	}
	bIsSuperMsg = val;
	SetActive(TRUE);
}
void CShoutDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
		case CHA_BTN_IMAGE:  
			{
				if(m_ImageGrid->IsActive())
				{
					m_ImageGrid->SetActive(FALSE);
				}
				else
				{
					m_ImageGrid->SetActive(TRUE);
				}
			}
			break;
		}
	}
	else if( we & WE_LBTNCLICK)  // 2014-12-21 点击表情面版事件 鼠标左键点击
	{
		switch(lId)
		{
		case CHA_ICONGRID_IMAGE:
			{
				int Pos=m_ImageGrid->GetCurSelCellPos();

				if(Pos>=0)
				{
					char ImgCode[10];

					sprintf(ImgCode,"{%02d",Pos);

					m_pMsgBox->SetEditText(strcat(m_pMsgBox->GetEditText(),ImgCode));
				}
			}
		}
	}
}
void CShoutDialog::SendItemInfoSyn(char * buff)
{
	if (strlen(buff) == 0)
		return ;
	char buf[MAX_SHOUT_LENGTH + 1] = { 0, };
	char SendMsg[MAX_SHOUT_LENGTH + 1] = { 0, };
	strcpy(buf, m_pMsgBox->GetEditText());
	if (*buf != 0)
	{
		if (FILTERTABLE->FilterChat(buf))
			return;
		sprintf(SendMsg, "%s %s", buf, buff);
	}
	else
		sprintf(SendMsg, "%s", buff);

	SEND_SHOUTBASE_ITEMINFO msg;

	sprintf(msg.Msg, "%s : %s", HERO->GetObjectName(), SendMsg);

	if (bIsSuperMsg)
	{
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEM_SHOPITEM_SUPERSHOUT_SYN;
	}
	else
	{
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_SYN;
	}
	msg.dwObjectID = HERO->GetID();

	msg.ItemIdx = m_dwItemIdx;

	msg.ItemPos = (WORD)m_dwItemPos;

	NETWORK->Send(&msg, sizeof(msg));

	SetActive(FALSE);

	m_dwItemIdx = m_dwItemPos = 0;
	bIsSuperMsg = FALSE;
}
BOOL CShoutDialog::SendShoutMsgSyn()
{
	char buf[MAX_SHOUT_LENGTH+1] = { 0, };
	strcpy( buf, m_pMsgBox->GetEditText() );

	if( strlen( buf ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(903) );
		return FALSE;

	}

	m_pMsgBox->SetEditText("");

	if( FILTERTABLE->FilterChat( buf ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(27) );
		return FALSE;
	}


	SEND_SHOUTBASE_ITEMINFO msg;	

	sprintf( msg.Msg, "%s : %s", HERO->GetObjectName(), buf );	
	
	if (bIsSuperMsg)
	{
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEM_SHOPITEM_SUPERSHOUT_SYN;
	}
	else
	{
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_SYN;
	}
	msg.dwObjectID = HERO->GetID();

	msg.ItemIdx = m_dwItemIdx;

	msg.ItemPos = (WORD)m_dwItemPos;

	NETWORK->Send( &msg, sizeof(msg) );



	SetActive( FALSE );

	m_dwItemIdx = m_dwItemPos = 0;
	bIsSuperMsg = FALSE;
	return TRUE;
}





