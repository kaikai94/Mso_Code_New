







#include "stdafx.h"
#include "VimuManager.h"
#include "./interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "objectstatemanager.h"

#include "BattleSystem_Client.h"

GLOBALTON(CVimuManager);

CVimuManager::CVimuManager()

{
	m_bIsVimuing = FALSE;
	m_dwGoldMoney = 0;
}

CVimuManager::~CVimuManager()
{

}

void CVimuManager::Init()
{
	m_bIsVimuing = FALSE;
}

void CVimuManager::ApplyVimuWithGold(DWORD dwGold)
{//[Ñº×¢±ÈÎä][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-4-21][0:39:35]
	m_dwGoldMoney = dwGold;
	ApplyVimu(TRUE);
}
void CVimuManager::ApplyVimu(BOOL bIsGoldMoney)
{
	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE || m_bIsVimuing )
	{
		return;
	}
	if( HERO->IsPKMode() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
		return;
	}
	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
		return;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();

	if( pOpPlayer == NULL || pOpPlayer == HERO )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(380) );
		return;
	}

	if( pOpPlayer->GetObjectKind() != eObjectKind_Player )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(380) );
		return;
	}
	if( pOpPlayer->GetState() == eObjectState_Die )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(368) );
		return;
	}
	
	if( pOpPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1329) );
		return;
	}

	SetVimuing( TRUE );
	HERO->DisableAutoAttack();
	HERO->SetNextAction(NULL);	
	MOVEMGR->HeroMoveStop();
//	HERO->SetState( eObjectState_BattleReady );
	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_BattleReady);
	
	
	MSG_DWORD2 msg;
	msg.Category	= MP_BATTLE;
	if(bIsGoldMoney)
		msg.Protocol	= MP_BATTLE_VIMUWITHGOLD_APPLY_SYN;
	else
		msg.Protocol	= MP_BATTLE_VIMU_APPLY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData1		= pOpPlayer->GetID();
	msg.dwData2		= m_dwGoldMoney;
	NETWORK->Send( &msg, sizeof(MSG_DWORD2) );
}


void CVimuManager::CancelApply()
{
	MSGBASE msg;
	msg.Category	= MP_BATTLE;
	msg.Protocol	= MP_BATTLE_VIMU_WAITING_CANCEL_SYN;
	msg.dwObjectID	= HERO->GetID();
	NETWORK->Send( &msg, sizeof(MSGBASE) );	
	m_dwGoldMoney = 0;
}
void CVimuManager::AcceptVimuWithGold(BOOL bAccept)
{
	if( bAccept )
	{
		if( CanAcceptVimu( HERO ) &&HERO->GetGoldMoney()>=m_dwGoldMoney)
		{
			HERO->DisableAutoAttack();					
			HERO->SetNextAction(NULL);	
			MOVEMGR->HeroMoveStop();
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_BattleReady);

			MSG_DWORD msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMUWITHGOLD_ACCEPT_SYN;
			msg.dwObjectID	= HERO->GetID();
			msg.dwData = m_dwGoldMoney;
			NETWORK->Send( &msg, sizeof(MSG_DWORD) );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(378) );
		}
		else
		{
			MSG_BYTE msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMU_ERROR;
			msg.dwObjectID	= HERO->GetID();
			msg.bData		= 1;	
			NETWORK->Send( &msg, sizeof(MSG_BYTE) );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
			SetVimuing( FALSE );
			m_dwGoldMoney = 0;
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_BATTLE;
		msg.Protocol	= MP_BATTLE_VIMU_REJECT_SYN;
		msg.dwObjectID	= HERO->GetID();
		NETWORK->Send( &msg, sizeof(MSGBASE) );
	}

}
void CVimuManager::AcceptVimu( BOOL bAccept )
{

	if( bAccept )
	{
		if( CanAcceptVimu( HERO ) )
		{
			HERO->DisableAutoAttack();					
			HERO->SetNextAction(NULL);					
			MOVEMGR->HeroMoveStop();
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_BattleReady);
			MSGBASE msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMU_ACCEPT_SYN;
			msg.dwObjectID	= HERO->GetID();
			NETWORK->Send( &msg, sizeof(MSGBASE) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(378) );
		}
		else
		{
			MSG_BYTE msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMU_ERROR;
			msg.dwObjectID	= HERO->GetID();
			msg.bData		= 1;	
			NETWORK->Send( &msg, sizeof(MSG_BYTE) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
			SetVimuing( FALSE );
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_BATTLE;
		msg.Protocol	= MP_BATTLE_VIMU_REJECT_SYN;
		msg.dwObjectID	= HERO->GetID();
		NETWORK->Send( &msg, sizeof(MSGBASE) );
	}

}

BOOL CVimuManager::CanAcceptVimu( CPlayer* pAccepter )
{
	if( pAccepter == NULL )					return FALSE;

	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )	 return FALSE;
	if( pAccepter->IsPKMode() )				return FALSE;
//	if( m_bIsVimuing == TRUE )				return FALSE; //Ã‡Ã¶Ã€Ã§ Ã€ÃŒ Ã‡Ã”Â¼Ã¶Â´Ã‚ HEROÂ¸Â¸ Ã€ÃŽÃ€ÃšÂ·ÃŽ Â³Ã‘Â¾Ã®Â¿Ã‚Â´Ã™!
	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move
		&& pAccepter->GetState() != eObjectState_Immortal )
		return FALSE;
	
	return TRUE;
}

void CVimuManager::SetVimuing( BOOL bVimuing )
{
	m_bIsVimuing = bVimuing;
	
//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_VIMUICON, m_bIsVimuing );

}

void CVimuManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_BATTLE_VIMU_APPLY:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pApplyer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pApplyer == NULL || m_bIsVimuing == TRUE )
			{
				MSG_BYTE msg;
				msg.Category	= MP_BATTLE;
				msg.Protocol	= MP_BATTLE_VIMU_ERROR;
				msg.dwObjectID	= HERO->GetID();
				msg.bData		= 1;
				NETWORK->Send( &msg, sizeof(MSG_BYTE) );
				return;
			}

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_VIMUAPPLY, MBT_YESNO,
									CHATMGR->GetChatMsg(373), pApplyer->GetObjectName() );
			if( pMsgBox ) pMsgBox->SetDefaultBtn( -1 );
			
			SetVimuing( TRUE );
		}
		break;
	case MP_BATTLE_VIMUWITHGOLD_APPLY_NACK:
		{//[Ñº×¢±ÈÎäÑûÇëÊ§°Ü][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-4-27][18:13:10]
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );

			if( pPlayer )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2357), pPlayer->GetObjectName() );
			m_dwGoldMoney = 0;
		}
		break;
	case MP_BATTLE_VIMUWITHGOLD_APPLY:
		{//[Ñº×¢±ÈÎäÊÕµ½ÑûÇë][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-4-27][18:17:49]
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pApplyer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if(pApplyer==NULL||m_bIsVimuing==TRUE)
			{
				MSG_BYTE msg;
				msg.Category	= MP_BATTLE;
				msg.Protocol	= MP_BATTLE_VIMU_ERROR;
				msg.dwObjectID	= HERO->GetID();
				msg.bData		= 1;
				NETWORK->Send( &msg, sizeof(MSG_BYTE) );
				return;
			}
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_VIMUAPPLYWITHGOLD, MBT_YESNO,
				CHATMGR->GetChatMsg(2358), pApplyer->GetObjectName(),pmsg->dwData2 );
			if( pMsgBox ) pMsgBox->SetDefaultBtn( -1 );

			SetVimuing( TRUE );
			m_dwGoldMoney = pmsg->dwData2;
		}
		break;
	case MP_BATTLE_VIMU_APPLY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );

			if( pPlayer )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(371), pPlayer->GetObjectName() );
			
			WINDOWMGR->MsgBox( MBI_VIMUWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(372) );
		}

		break;
	case MP_BATTLE_VIMU_APPLY_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );

			SetVimuing( FALSE );
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(368) );
		}
		break;

	case MP_BATTLE_VIMU_ACCEPT:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );

			if( pPlayer )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(374), pPlayer->GetObjectName() );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}

		break;

	case MP_BATTLE_VIMU_ACCEPT_ACK:
		{
			
		}
		break;

	case MP_BATTLE_VIMU_ACCEPT_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )				
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
		}
		break;

	case MP_BATTLE_VIMU_ERROR:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if( pmsg->bData == 0 )
			{

			}
			else if( pmsg->bData == 1 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(381) );
			}

			else if( pmsg->bData == 2 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(369) );
			}
			else if( pmsg->bData == 3 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(367) );
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );


			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
		}
		break;

	case MP_BATTLE_VIMU_REJECT:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(375) );
			
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_BATTLE_VIMU_REJECT_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(379) );
			SetVimuing( FALSE );
		}
		break;

	case MP_BATTLE_VIMU_REJECT_NACK:
		{

		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL:

		{

			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )

				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(376) );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUAPPLY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			//[Ñº×¢±ÈÎä][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-4-27][18:21:17]
			cDialog* pDlg1 = WINDOWMGR->GetWindowForID(MBI_VIMUAPPLYWITHGOLD);
			if(pDlg1)
				WINDOWMGR->AddListDestroyWindow(pDlg1);
		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL_ACK:
		{
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(377) );
		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL_NACK:
		{

		}
		break;

	case MP_BATTLE_DESTROY_NOTIFY:

		{
			SetVimuing( FALSE );			
		}

		break;
	}
}




