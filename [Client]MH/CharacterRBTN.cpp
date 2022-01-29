#include "stdafx.h"
#include "CharacterRBTN.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "cButton.h"
CharacterRBTN::CharacterRBTN(void)
{
	m_type=WT_CHARRBTN_DLG;
}


CharacterRBTN::~CharacterRBTN(void)
{
}

void CharacterRBTN::Linking()   //对话框组件绑定   by:十里坡剑神 QQ:112582793
{
	AddFriend		=(cButton*)GetWindowForID(FRI_ADDFRIENDBTN);
	InviteMember	=(cButton*)GetWindowForID(GD_INVITEMEMBER);
	ViewTargeteQ	=(cButton*)GetWindowForID(CG_VIEWTARGETEQ);
	Report			=(cButton*)GetWindowForID(CG_REPORT);
	AddPartyMember	=(cButton*)GetWindowForID(PA_ADDPARTYMEMBERBTN);
	ApplyDeal		=(cButton*)GetWindowForID(APPLYDEAL);
	ApplyPK			=(cButton*)GetWindowForID(APPLYPK);
	MiniNote		=(cButton*)GetWindowForID(MININOTE);
	CharName		=(cStatic*)GetWindowForID(CG_CHARNAME);
	SendWhisper		=(cButton*)GetWindowForID(CG_WHISPERSEND);
	MarryMe			=(cButton*)GetWindowForID(CG_MARRYME);
	DelMarry		=(cButton*)GetWindowForID(CG_DELMARRY);
}
void CharacterRBTN::SetCharName(char *buf)
{
	CharName->SetStaticText(buf);
	CharName->SetAlign(TXT_CENTER);
}
void CharacterRBTN::Show()
{
	if(HERO->IsMarry())
		DelMarry->SetActive(TRUE);
	else
		DelMarry->SetActive(FALSE);
	SetActive(TRUE);
}


void CharacterRBTN::Close()
{
	SetActive(FALSE);
}
void CharacterRBTN::SendReportMsg()
{
	REPORT msg;
	msg.Category = MP_HACKCHECK;
	msg.Protocol = MP_HACKE_CLIENT_REPORT;
	sprintf(msg.BeReportName,CharName->GetStaticText());
	sprintf(msg.SenderName,HERO->GetObjectName());
	NETWORK->Send(&msg,sizeof(msg));
}
