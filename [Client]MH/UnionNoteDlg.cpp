









#include "stdafx.h"
#include "UnionNoteDlg.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Item.h"
#include "./Interface\cTextArea.h"


#include "ChatManager.h"
#include "ItemManager.h"

CUnionNoteDlg::CUnionNoteDlg(void)
{
	m_bUse = FALSE;
}

CUnionNoteDlg::~CUnionNoteDlg(void)
{
}

void CUnionNoteDlg::Linking()
{
	m_pNoteText = (cTextArea*)GetWindowForID(AN_TEXTREA);

	m_pNoteText->SetEnterAllow(FALSE);
	m_pNoteText->SetScriptText("");
}

void CUnionNoteDlg::Show(CItem* pItem)
{

	if( !HERO->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(35) );
		return;
	}
	if( HERO->GetGuildMemberRank() != GUILD_MASTER && HERO->GetGuildMemberRank() != GUILD_VICEMASTER )

	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
		return;
	}
	if( !HERO->GetGuildUnionIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
		return;
	}


	if(pItem == NULL)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
		//786 "¾ÆÀÌÅÛ »ç¿ë¿¡ ½ÇÆÐÇÏ¿´½À´Ï´Ù."
		return;
	}
	if(m_bUse)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
		//752 "ÀÌ¹Ì »ç¿ëÁßÀÎ ¾ÆÀÌÅÛ ÀÔ´Ï´Ù."
		return;
	}

	m_pItem = pItem;


	SetActive(TRUE);
}



void CUnionNoteDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{

		switch(lId)
		{
		case AN_SENDOKBTN:
			{
				MSG_GUILD_SEND_NOTE msg;
				msg.Category = MP_GUILD_UNION;

				msg.Protocol = MP_GUILD_UNION_NOTE_SYN;
				msg.FromId = HERO->GetGuildUnionIdx();

				msg.dwObjectID = HEROID;


				SafeStrCpy(msg.FromName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
				m_pNoteText->GetScriptText(msg.Note);

				NETWORK->Send(&msg, msg.GetMsgLength());


				SetActive(FALSE);
			}
		case AN_CANCELBTN:
			{
				SetActive(FALSE);

			}
		}
	}
}

void CUnionNoteDlg::Use()
{
	m_bUse = FALSE;
	m_pNoteText->SetScriptText("");


	MSG_ITEM_USE_SYN msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_USE_SYN;

	msg.dwObjectID = HEROID;
	msg.wItemIdx = m_pItem->GetItemIdx();
	msg.TargetPos = m_pItem->GetPosition();
	msg.dwItemDBIdx = m_pItem->GetDBIdx();
	//[Ôö¼ÓÎïÆ·Ê¹ÓÃÊý¾Ý¿â±àºÅÅÐ¶Ï][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/4/13]

	NETWORK->Send(&msg,sizeof(msg));
}




