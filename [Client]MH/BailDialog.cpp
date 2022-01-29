









// BailDialog.cpp: implementation of the CBailDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WindowIDEnum.h"
#include "BailDialog.h"

#include "./interface/cEditBox.h"
#include "cMsgBox.h"
#include "./interface/cTextArea.h"
#include "./interface/cWindowManager.h"
#include "ChatManager.h"
#include "cIMEex.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CBailDialog::CBailDialog()
{
	m_pBailEdtBox	= NULL;

	m_pBailText		= NULL;
	m_BadFame		= 0;
	m_IsGoldClearFAME = FALSE;
}

CBailDialog::~CBailDialog()
{

}



void CBailDialog::Linking()
{
	m_pBailEdtBox = (cEditBox*)GetWindowForID(BAIL_BAILEDITBOX);	
	m_pBailEdtBox->SetValidCheck(VCM_NUMBER);
	m_pBailEdtBox->SetAlign(TXT_RIGHT);
	m_pBailText = (cTextArea*)GetWindowForID(BAIL_TEXTAREA);
}

void CBailDialog::Open()
{
	char buf[256];
	char strBfame[256];
	if( HERO->GetBadFame() > MIN_BADFAME_FOR_BAIL )
	{
		char strBprice[256];
		strcpy(strBfame, AddComma((MONEYTYPE)MIN_BADFAME_FOR_BAIL));

		if(m_IsGoldClearFAME)
		{
			strcpy(strBprice, AddComma(MONEYTYPE(BAIL_GOLD_PRICE)));
			wsprintf( buf, CHATMGR->GetChatMsg(2200), strBfame, strBprice );
		}
		else
		{
			strcpy(strBprice, AddComma(MONEYTYPE(BAIL_PRICE)));
			wsprintf( buf, CHATMGR->GetChatMsg(644), strBfame, strBprice );
		}
		m_pBailText->SetScriptText(buf);
		m_pBailEdtBox->SetEditText( "0" );
		SetActive(TRUE);
	}
	else
	{

		WINDOWMGR->MsgBox(MBI_BAILLOWBADFAME, MBT_OK, CHATMGR->GetChatMsg(659), AddComma((MONEYTYPE)MIN_BADFAME_FOR_BAIL));
	}
}
void CBailDialog::Close()
{
	m_IsGoldClearFAME = FALSE;
}
void CBailDialog::SetActive( BOOL val )
{
	if(!val)
	{
		m_IsGoldClearFAME = FALSE;
		SetDisable(FALSE);
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
	cDialog::SetActive(val);
}
void CBailDialog::SetFame(DWORD BadFame)
{
	m_BadFame = BadFame;

	if(m_BadFame == 0)		return;

	if( m_BadFame+MIN_BADFAME_FOR_BAIL > (DWORD)HERO->GetBadFame() )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(648), AddComma((MONEYTYPE)MIN_BADFAME_FOR_BAIL));
		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}

	DWORD HeroMoney = 0;
	HeroMoney = HERO->GetGoldMoney();

	if( HeroMoney < m_BadFame*BAIL_GOLD_PRICE )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(117));

		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}

	char buf[256] = { 0, };

	char badfame[16] = { 0, };

	char money[16] = { 0, };
	strcpy(badfame, AddComma(m_BadFame));
	m_IsGoldClearFAME = TRUE;
	strcpy(money, AddComma(m_BadFame*BAIL_GOLD_PRICE));
	sprintf(buf, CHATMGR->GetChatMsg(2201), money, badfame );
	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEMSG, MBT_YESNO, buf);
	if( pMsgBox )
		SetDisable( TRUE );}

void CBailDialog::SetFame()
{
	m_BadFame = atoi( RemoveComma(m_pBailEdtBox->GetEditText()) );


	if(m_BadFame == 0)		return;

	if( m_BadFame+MIN_BADFAME_FOR_BAIL > (DWORD)HERO->GetBadFame() )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(648), AddComma((MONEYTYPE)MIN_BADFAME_FOR_BAIL));
		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}
	MONEYTYPE HeroMoney = 0;
	MONEYTYPE BailPrice = 0;
	if(m_IsGoldClearFAME)
	{
		HeroMoney = HERO->GetGoldMoney();
		BailPrice = BAIL_GOLD_PRICE;
	}
	else
	{
		HeroMoney =HERO->GetMoney();
		BailPrice = BAIL_PRICE;
	}

	if( HeroMoney < m_BadFame*BailPrice)
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(117));

		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}
	char buf[256] = { 0, };

	char badfame[16] = { 0, };

	char money[16] = { 0, };
	strcpy(badfame, AddComma(m_BadFame));
	if(m_IsGoldClearFAME)
	{
		strcpy(money, AddComma(m_BadFame*BailPrice));
		sprintf(buf, CHATMGR->GetChatMsg(2201), money, badfame );
	}
	else
	{
		strcpy(money, AddComma(m_BadFame*BailPrice));
		sprintf(buf, CHATMGR->GetChatMsg(645), money, badfame );
	}

	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEMSG, MBT_YESNO, buf);
	if( pMsgBox )
		SetDisable( TRUE );
}


void CBailDialog::SetBadFrameSync()
{
	MSG_FAME msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_BADFAME_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.IsUseGold = m_IsGoldClearFAME;
	if(m_BadFame <= 0)			return;

	msg.Fame = m_BadFame;

	NETWORK->Send(&msg, sizeof(msg));

	SetActive(FALSE);
	m_IsGoldClearFAME = FALSE;
	m_BadFame = 0;
}




