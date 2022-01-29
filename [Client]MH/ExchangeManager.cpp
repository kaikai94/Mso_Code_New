







#include "stdafx.h"
#include "ExchangeManager.h"
#include "Item.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "VimuManager.h"

#include "ExchangeDialog.h"
#include "InventoryExDialog.h"
#include "MainBarDialog.h"
#include "ObjectStateManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

#include "Battle.h"
#include "BattleSystem_Client.h"

#include "MHMap.h"

GLOBALTON(CExchangeManager)

CExchangeManager::CExchangeManager()
{
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_EXCHANGEITEM_STARTINDEX);
	
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_bInventoryActive	= FALSE;
//	m_nExchangeState	= eEXSTT_NONE;
}

CExchangeManager::~CExchangeManager()
{
	m_IconIndexCreator.Release();
}

void CExchangeManager::Init()	//exchangedialog¿¡¼­ active false ÀÏ¶§ È£ÃâÇÑ´Ù. CONFIRM
{
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_IsExchanging		= FALSE;
}

void CExchangeManager::ExchangeStart()
{
//---±³È¯½ÅÃ»»óÅÂ·Î ¸¸µé±â
	HERO->DisableAutoAttack();					//ÀÚµ¿ °ø°Ý Ãë¼Ò
	HERO->SetNextAction(NULL);					//½ºÅ³ Ãë¼Ò
	MOVEMGR->HeroMoveStop();					//Á¦ÀÚ¸® ¸ØÃã ¸Þ¼¼Áö

	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = TRUE;
//------------------------
}

void CExchangeManager::ExchangeEnd()
{
	if( HERO->GetState() == eObjectState_Exchange )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = FALSE;
}

BOOL CExchangeManager::ApplyExchange()
{
	if( m_IsExchanging  )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(335) );
		return FALSE;
	}

	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( VIMUMGR->IsVimuing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( HERO->InTitan() )
	{
		TITANMGR->CheckNotifyMsg(TNM_CANNOT_DO);
		return FALSE;
	}

	if( MAP->IsMapKind(eSurvivalMap) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();
	if( pOpPlayer == NULL ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	if( pOpPlayer->GetObjectKind() != eObjectKind_Player || pOpPlayer == HERO ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	if( pOpPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )

	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );

	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( HERO->GetBattleTeam() == 2 )
		return FALSE;
	
	ExchangeStart();

	MSG_DWORD msgApply;
	msgApply.Category	= MP_EXCHANGE;
	msgApply.Protocol	= MP_EXCHANGE_APPLY_SYN;
	msgApply.dwObjectID	= HERO->GetID();
	msgApply.dwData		= pOpPlayer->GetID();

	NETWORK->Send( &msgApply, sizeof( msgApply ) );

	return TRUE;
}

BOOL CExchangeManager::CanAcceptExchange( CPlayer* pAccepter )
{
	if( pAccepter == NULL )					return FALSE;
	if( VIMUMGR->IsVimuing() )				return FALSE;

	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move
		&& pAccepter->GetState() != eObjectState_Immortal )
		return FALSE;
	
	return TRUE;	
}

void CExchangeManager::AcceptExchange( BOOL bAccept )
{
	if( bAccept )
	{
		if( CanAcceptExchange( HERO ) )
		{
			ExchangeStart();
			
			MSGBASE msgAccept;
			msgAccept.Category		= MP_EXCHANGE;
			msgAccept.dwObjectID	= HERO->GetID();

			msgAccept.Protocol		= MP_EXCHANGE_ACCEPT_SYN;
			NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(334) );

		}
		else
		{
			MSGBASE msgError;		
			msgError.Category		= MP_EXCHANGE;
			msgError.dwObjectID		= HERO->GetID();
			msgError.Protocol		= MP_EXCHANGE_CANTAPPLY;
			NETWORK->Send( &msgError, sizeof(MSGBASE) );

			m_IsExchanging			 = FALSE;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		}
	}
	else
	{
		MSGBASE msgAccept;
		msgAccept.Category		= MP_EXCHANGE;
		msgAccept.dwObjectID	= HERO->GetID();
		msgAccept.Protocol		= MP_EXCHANGE_REJECT_SYN;
		NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(327) );

		ExchangeEnd();
	}	
}

void CExchangeManager::CancelApply()
{
	MSGBASE msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_WAITING_CANCEL_SYN;
	msg.dwObjectID	= HERO->GetID();

	NETWORK->Send( &msg, sizeof(MSGBASE) );
}

BOOL CExchangeManager::LinkItem( CItem* pItem )
{
	if( pItem->GetType() != WT_ITEM ) return FALSE;		

	if( pItem->GetLinkItem() != NULL ) return FALSE;		

	if( m_nLinkedItemNum >= MAX_EXCHANGEITEM ) return FALSE;

	++m_nLinkedItemNum;
	
	pItem->SetLock( TRUE );


	MSG_LINKITEM msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_ADDITEM_SYN;

	msg.dwObjectID	= HERO->GetID();
	msg.ItemInfo	= *pItem->GetItemBaseInfo();
	msg.wAbsPosition = m_nLinkedItemNum;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );

	return TRUE;
}

void CExchangeManager::UnLinkItem( int nTypeIndex, cIcon* pItem )
{
	if( nTypeIndex == 0 )
	{
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkPosition( 0 );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkItem( NULL );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLock( FALSE );		
	}

	m_IconIndexCreator.ReleaseIndex( pItem->GetID() );
	WINDOWMGR->AddListDestroyWindow( pItem );
}

void CExchangeManager::Lock( BOOL bLock )
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();

	if( bLock )
	{
		msg.Protocol = MP_EXCHANGE_LOCK_SYN;
	}
	else
	{
		msg.Protocol = MP_EXCHANGE_UNLOCK_SYN;
	}

	NETWORK->Send( &msg, sizeof( MSGBASE ) );

	m_bLockSyncing = TRUE;
}


void CExchangeManager::Exchange()
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();
	msg.Protocol	= MP_EXCHANGE_EXCHANGE_SYN;

	NETWORK->Send( &msg, sizeof( MSGBASE ) );
	
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);

	m_bExchangeSyncing = TRUE;
}


void CExchangeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_EXCHANGE_APPLY:	
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			if( pPlayer == NULL || m_IsExchanging == TRUE )
			{
				MSGBASE msgReject;
				msgReject.Category		= MP_EXCHANGE;
				msgReject.dwObjectID	= HERO->GetID();
				msgReject.Protocol		= MP_EXCHANGE_CANTAPPLY;
				NETWORK->Send( &msgReject, sizeof(MSGBASE) );

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
				break;
			}

			WINDOWMGR->MsgBox( MBI_EXCHANGEAPPLY, MBT_YESNO,
						CHATMGR->GetChatMsg(321), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			m_IsExchanging = TRUE;
		}
		break;

	case MP_EXCHANGE_APPLY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL )	
			{
				ExchangeEnd();
				break;
			}
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(322), pPlayer->GetObjectName() );

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_EXCHANGEWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(337) );
			if( pMsgBox )
				pMsgBox->SetDefaultBtn(-1);
		}
		break;

	case MP_EXCHANGE_APPLY_NACK:
		{			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT_NACK:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );			
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL ) return;

			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_EXCHANGE_REJECT:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_CANTAPPLY:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL:
		{
			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_ACK:
		{
			ExchangeEnd();


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(316) );
		}
		break;	

	case MP_EXCHANGE_START:
		{
			GAMEIN->GetExchangeDialog()->SetActive( TRUE );

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
			{
				GAMEIN->GetInventoryDialog()->SetActive( TRUE );
				m_bInventoryActive = FALSE;
			}
			else
			{
				m_bInventoryActive = TRUE;
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_ACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			ASSERT( pItem );
			if( !pItem ) break;

			CExchangeItem* pExItem = new CExchangeItem;
			pExItem->SetDBId(msg->ItemInfo.dwDBIdx);
			pExItem->InitItem( pItem, msg->wAbsPosition, m_IconIndexCreator.GenerateIndex() );
			pExItem->SetType( WT_EXCHANGEITEM );
			pExItem->SetDurability( msg->ItemInfo.Durability );
			pExItem->SetRareness( msg->ItemInfo.RareIdx );
			pExItem->SetGrow(msg->ItemInfo.ItemGrow);   
			//[ÄÍ¾Ã£¬ÖÆÔìÕß¸½¼Ó][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/1/2]
			pExItem->SetMaker(msg->ItemInfo.ItemMaker);
			pExItem->SetMaker1(msg->ItemInfo.ItemMaker1);
			pExItem->SetAbrasion(msg->ItemInfo.ItemAbrasion);
			pExItem->SetMaxItemAbrasion(msg->ItemInfo.MaxItemAbrasion);
			WINDOWMGR->AddWindow( pExItem );

			GAMEIN->GetExchangeDialog()->AddItem( 0, msg->wAbsPosition, pExItem );

			pItem->SetLinkPosition( m_nLinkedItemNum );
			pItem->SetLinkItem( pExItem );

			int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();

			if( nBlankNum < (int)msg->wAbsPosition - EXCHANGEMGR->GetLinkedItemNum() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_NACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	

			--m_nLinkedItemNum;

			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( pItem )
				pItem->SetLock( FALSE );			
		}
		break;

	case MP_EXCHANGE_ADDITEM:	
		{
			MSG_LINKITEMOPTIONS* msg = (MSG_LINKITEMOPTIONS*)pMsg;

			CExchangeItem* pItem = new CExchangeItem;
			cImage tmpImage;
			pItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
						 ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );

			pItem->SetType( WT_EXCHANGEITEM );
			pItem->SetDurability( msg->ItemInfo.Durability );
			pItem->SetRareness( msg->ItemInfo.RareIdx );
			pItem->SetMovable( FALSE );
			pItem->SetData( msg->ItemInfo.wIconIdx );
			pItem->SetItemParam( msg->ItemInfo.ItemParam );
			pItem->SetDBId( msg->ItemInfo.dwDBIdx );
			pItem->SetGrow(msg->ItemInfo.ItemGrow);
			//[ÄÍ¾Ã£¬ÖÆÔìÕß¸½¼Ó][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/1/2]
			pItem->SetAbrasion(msg->ItemInfo.ItemAbrasion);
			pItem->SetMaxItemAbrasion(msg->ItemInfo.MaxItemAbrasion);
			pItem->SetMaker(msg->ItemInfo.ItemMaker);
			pItem->SetMaker1(msg->ItemInfo.ItemMaker1);
			
			pItem->SetItemIdx(msg->ItemInfo.wIconIdx);

			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, msg);

			WINDOWMGR->AddWindow( pItem );

			GAMEIN->GetExchangeDialog()->AddItem( 1, msg->wAbsPosition, pItem );
		}
		break;
		
	case MP_EXCHANGE_DELITEM_ACK:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->DelItem( 0, msg->wData );
			--m_nLinkedItemNum;
		}
		break;

	case MP_EXCHANGE_DELITEM_NACK:
		{

			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_DELITEM:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			GAMEIN->GetExchangeDialog()->DelItemOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelPetItemInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelItem( 1, msg->wData );			
		}
		break;
		
	case MP_EXCHANGE_INPUTMONEY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwMoney = GAMEIN->GetExchangeDialog()->InputMoney( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTGOLD_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwGold = GAMEIN->GetExchangeDialog()->InputGold( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY_NACK:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;
	case MP_EXCHANGE_INPUTGOLD_NACK: 
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY:
		{

			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputMoney( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_INPUTGOLD: 
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputGold( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_LOCK_ACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );
		}
		break;
	case MP_EXCHANGE_LOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_LOCK:
		{
			GAMEIN->GetExchangeDialog()->OpLock( TRUE );			
		}
		break;

	case MP_EXCHANGE_UNLOCK_ACK:
		{
			m_bLockSyncing = FALSE;			
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_UNLOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );
		}
		break;

	case MP_EXCHANGE_UNLOCK:
		{
			GAMEIN->GetExchangeDialog()->OpLock( FALSE );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(332) );

			if( m_bExchangeSyncing )
			{
				m_bExchangeSyncing = FALSE;
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );				
			}			
		}
		break;

	case MP_EXCHANGE_EXCHANGE_ACK:
		{
		}
		break;

	case MP_EXCHANGE_EXCHANGE:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );			
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325) );

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_EXCHANGE_NACK:		
		{
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
		}
		break;

	case MP_EXCHANGE_CANCEL_ACK:
		{
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelPetItemInfoAll();

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );

			if( !m_bInventoryActive )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );

			ExchangeEnd();
		}
		break;
	
	case MP_EXCHANGE_CANCEL_NACK:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_CANCEL:
		{
			MSG_BYTE* msg = (MSG_BYTE*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	

			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();

			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
			if( !m_bInventoryActive )
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			
			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY);
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );			

			switch( msg->bData )
			{
				case eEE_USERCANCEL:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );

					break;
				case eEE_USERLOGOUT:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(314) );
					break;
				case eEE_USERDIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(313) );
					break;
				case eEE_DIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(312) );
					break;
				case eEE_NOTENOUGHMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(311) );
					break;
				case eEE_NOTENOUGHSPACE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(310) );
					break;
				case eEE_MAXMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(309) );
					break;
				default:
					break;
			}

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_SETMONEY:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			HERO->SetMoney( msg->dwData );	
		}
		break;
		
	case MP_EXCHANGE_INSERT:
		{
			MSG_ITEM* msg = (MSG_ITEM*)pMsg;
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(&msg->ItemInfo);
			ASSERT(rt);

			QUICKMGR->RefreshQickItem();
		}
		break;

	case MP_EXCHANGE_REMOVE:
		{
			MSG_EXCHANGE_REMOVEITEM* msg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( msg->wAbsPosition, &pOutItem );
		}
		break;
	}
}





