#include "stdafx.h"
#include "StreetStallManager.h"
#include "Item.h"
#include "itemmanager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "BuyItem.h"
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "objectmanager.h"
#include "objectstatemanager.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MoveManager.h"
#include "Vimumanager.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "StallFindDlg.h"
#include "StreetStall.h"
#include "InventoryExDialog.h"
#include "FilteringTable.h"
#include "MHCamera.h"
#include "Quickmanager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "StreetStallItemViewDlg.h"
#include "BattleSystem_Client.h"
#include "MHMap.h"
#include "cScriptManager.h"
#include "ObjectBalloon.h"
#include "StreetStallTitleTip.h"
#include "./Plug_inModule/Client.h"

GLOBALTON(CStreetStallManager);
CStreetStallManager::CStreetStallManager()
{
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;
	
	m_nLinkedItemNum = 0;
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_STREETSTALLITEM_STARTINDEX);
}

CStreetStallManager::~CStreetStallManager()
{
}

void CStreetStallManager::Init()
{
	m_bOpenMsgBox = TRUE;
	m_wStallKind = eSK_NULL;
	m_nLinkedItemNum = 0;
	m_dwWantBuyNum	= 0;
	m_dwWantSellNum	= 0;
}

BOOL CStreetStallManager::LinkItem( CItem* pItem, ITEMBASE* pBase )
{
	CExchangeItem* pExItem = new CExchangeItem;
	int pos;
	
	pExItem->InitItem( pItem, 0, m_IconIndexCreator.GenerateIndex() );
	pExItem->SetType( WT_STALLITEM );
	pExItem->SetDBId( pBase->dwDBIdx );
	pExItem->SetDurability( pBase->Durability );
	pExItem->SetRareness( pBase->RareIdx );
	pExItem->SetGrow(pBase->ItemGrow);  
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	pExItem->SetMaker(pBase->ItemMaker);
	pExItem->SetMaker1(pBase->ItemMaker1);
	pExItem->SetAbrasion(pBase->ItemAbrasion);
	pExItem->SetMaxItemAbrasion(pBase->MaxItemAbrasion);
	pos = GAMEIN->GetStreetStallDialog()->AddItem( pExItem );
	
	if( pos == -1 ) 
	{
		SAFE_DELETE(pExItem);
		return FALSE;
	}
	WINDOWMGR->AddWindow( pExItem );
	
	pExItem->SetPosition( pos );
	
	pItem->SetLock( TRUE );
	pItem->SetLinkPosition( pos );
	
	return TRUE;
}

BOOL CStreetStallManager::LinkItem(POSTYPE pos, ITEMBASE ItemInfo, WORD Volume, MONEYTYPE Money)
{
	if(pos < 0 || pos >= SLOT_STREETSTALL_NUM) return FALSE;

	CBuyItem* pBuyItem = new CBuyItem;

	pBuyItem->SetType( WT_BUYITE );

	pBuyItem->InitItem( ItemInfo, Volume, Money );
	
	if( !GAMEIN->GetStreetBuyStallDialog()->AddItem( pos, pBuyItem ) ) 
	{
		SAFE_DELETE(pBuyItem);
		return FALSE;
	}
	WINDOWMGR->AddWindow( pBuyItem );

	return TRUE;
}

void CStreetStallManager::UnlinkItem( CBuyItem* pItem )
{
	WINDOWMGR->AddListDestroyWindow( pItem );
}

void CStreetStallManager::UnlinkItem( CExchangeItem* pItem )
{
	if( pItem->GetLinkItem() == NULL )
		return;

	pItem->GetLinkItem()->SetLinkPosition( 0 );
	pItem->GetLinkItem()->SetLinkItem( NULL );
	pItem->GetLinkItem()->SetLock( FALSE );
	WINDOWMGR->AddListDestroyWindow( pItem );
}


void CStreetStallManager::ToggleHero_StreetStallMode( BOOL bOpen )
{	
	if( !bOpen )
	{
		if(OBJECTSTATEMGR->IsEndStateSetted(HERO) == TRUE)
			return;
		Toggle_StreetStallMode(HERO, FALSE);
		
		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);

		CAMERA->SetCharState( eCS_Normal, time );

		m_wStallKind = eSK_NULL;
	}
	else
	{
		DWORD time = HERO->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_Start);
		CAMERA->SetCharState( eCS_Sit, time );

		m_strOldTitle[0] = 0;
	}
}

void CStreetStallManager::Toggle_StreetStallMode(CObject* pObject, BOOL bStreet)
{
	if(bStreet == TRUE)
	{
		OBJECTSTATEMGR->StartObjectState(pObject, eObjectState_StreetStall_Owner);
	}
	else 
	{
		DWORD time = pObject->GetEngineObject()->GetAnimationTime(eMotion_StreetStall_End);
		OBJECTSTATEMGR->EndObjectState(pObject, eObjectState_StreetStall_Owner,time);
	}
}

void CStreetStallManager::OpenStreetStall()
{
	if( ( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Immortal )
		|| VIMUMGR->IsVimuing()		
		|| BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT 
		|| BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_SIEGEWAR 
		|| MAP->IsMapKind(eSurvivalMap) )
	{
		m_bOpenMsgBox = TRUE;
		m_wStallKind = eSK_NULL;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(353) );
		return;
	}
	
	if( HERO->InTitan() )
	{
		m_bOpenMsgBox = TRUE;
		m_wStallKind = eSK_NULL;

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1558) );
		TITANMGR->CheckNotifyMsg(TNM_CANNOT_DO);
		return;
	}

	MSG_STREETSTALL_TITLE msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_OPEN_SYN;
	msg.dwObjectID = HEROID;
	msg.StallKind = m_wStallKind;

	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->GetTitle( msg.Title );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->GetTitle( msg.Title );
	
	msg.StallKind = m_wStallKind;

	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_TITLE) );

	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_StreetStall_Owner);
}
BOOL CStreetStallManager::IsOpenState()
{
	return ((m_wStallKind == eSK_SELL)&&( GAMEIN->GetStreetStallDialog()->GetDlgState() ==  eSDS_OPENED ));
}
void CStreetStallManager::CloseStreetStall()
{
	if((m_wStallKind == eSK_SELL)&&( GAMEIN->GetStreetStallDialog()->GetDlgState() == eSDS_BUY ))
	{	
		MSG_DWORD msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_GUESTOUT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GAMEIN->GetStreetStallDialog()->GetStallOwnerId();
		NETWORK->Send( &msg, sizeof(MSG_DWORD) );
	}
	else if((m_wStallKind == eSK_BUY)&&( GAMEIN->GetStreetBuyStallDialog()->GetDlgState() == eBSDS_SELL ))
	{	
		MSG_DWORD msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_GUESTOUT_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData = GAMEIN->GetStreetBuyStallDialog()->GetStallOwnerId();
		NETWORK->Send( &msg, sizeof(MSG_DWORD) );
	}
	else if(((m_wStallKind == eSK_SELL)&&( GAMEIN->GetStreetStallDialog()->GetDlgState() ==  eSDS_OPENED )) ||
			((m_wStallKind == eSK_BUY)&&( GAMEIN->GetStreetBuyStallDialog()->GetDlgState() ==  eBSDS_OPENED )))
	{	
		MSGBASE msg;
		msg.Category = MP_STREETSTALL;
		msg.dwObjectID = HEROID;
		msg.Protocol = MP_STREETSTALL_CLOSE_SYN;
		NETWORK->Send( &msg, sizeof(MSGBASE) );
	}
}

void CStreetStallManager::EnterStreetStall( DWORD OwnerId )
{
	if( HERO->GetState() == eObjectState_StreetStall_Guest )
	{
		return;
	}
	
	if(GAMEIN->GetStallKindSelectDialog()->IsActive())
		return;
	
	OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_StreetStall_Guest );

	MSG_DWORD msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_GUESTIN_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = OwnerId;
	NETWORK->Send(&msg, sizeof(MSG_DWORD));
}
//////////////////////////////////////////////////////////////////////////
void CStreetStallManager::RegistItemEx(CItem* pItem, DWORD money, MONEYTYPE gold)
{
	if(m_wStallKind == eSK_BUY)
		return;		

	MSG_REGISTITEMEX msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_FAKEREGISTITEM_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemInfo = *pItem->GetItemBaseInfo();
	msg.dwData = m_nLinkedItemNum++;
	msg.dwMoney = money;
	msg.dwGold  = gold;   

	NETWORK->Send( &msg, sizeof(MSG_REGISTITEMEX) );
}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
void CStreetStallManager::RegistItemEx(ITEMBASE ItemInfo, WORD wAbsPosition, WORD Volume, MONEYTYPE money)
{
	if(m_wStallKind == eSK_SELL)
		return;		

	MSG_REGISTBUYITEMEX msg;
	msg.Category = MP_STREETSTALL;
	msg.Protocol = MP_STREETSTALL_FAKEREGISTBUYITEM_SYN;
	msg.dwObjectID = HEROID;
	msg.ItemInfo = ItemInfo;
	msg.dwData = m_nLinkedItemNum++;
	msg.dwMoney = money;
	msg.wVolume = Volume;
	msg.wAbsPosition = wAbsPosition; 
	
	NETWORK->Send( &msg, sizeof(MSG_REGISTBUYITEMEX) );
}
//////////////////////////////////////////////////////////////////////////

void CStreetStallManager::EditTitle()
{
	char strTitle[MAX_STREETSTALL_TITLELEN + 1] = {0,};
	
	if(m_wStallKind == eSK_SELL)
		GAMEIN->GetStreetStallDialog()->GetTitle( strTitle );
	else if(m_wStallKind == eSK_BUY)
		GAMEIN->GetStreetBuyStallDialog()->GetTitle( strTitle );
	
	if( strcmp( m_strOldTitle, strTitle ) == 0 )
		return;
	
	if( FILTERTABLE->FilterChat( strTitle ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(27) );
	}
	else
	{
		MSG_STREETSTALL_TITLE msg;
		msg.Category = MP_STREETSTALL;
		msg.Protocol = MP_STREETSTALL_EDITTITLE_SYN;
		msg.StallKind = m_wStallKind;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Title, strTitle, MAX_STREETSTALL_TITLELEN+1 );

		NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_TITLE) );

		SafeStrCpy( m_strOldTitle, strTitle, MAX_STREETSTALL_TITLELEN + 1 );
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL CStreetStallManager::SellItem()
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL || pDlg->IsActive() == FALSE )
		return FALSE;
		
	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();

	if( !pBuyItem || !pBuyItem->IsActive() ) 
		return FALSE;
	
	CItem* pItem = pDlg->GetSellItem();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}

	m_dwWantSellNum = 1;
	
	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETSELLMSG, MBT_YESNO, CHATMGR->GetChatMsg(164), pItem->GetItemInfo()->ItemName, AddComma( pBuyItem->GetMoney() ) );
	
	return TRUE;
}

BOOL CStreetStallManager::SellItemDur( DWORD dwNum )
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL || pDlg->IsActive() == FALSE )
		return FALSE;
	
	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();
	
	if( !pBuyItem || !pBuyItem->IsActive() ) 
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );
		
		return FALSE;
	}
	
	CItem* pItem = pDlg->GetSellItem();
	
	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}
	
	if( pItem->GetDurability() < dwNum )
		dwNum = pItem->GetDurability();

	m_dwWantSellNum = dwNum;

	char temp[32];
	SafeStrCpy( temp, AddComma( pDlg->GetItemMoney( pDlg->GetCurSelectedItemNum() ) ), 32 );
	
	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETSELLMSG, MBT_YESNO, CHATMGR->GetChatMsg(165), pItem->GetItemInfo()->ItemName, dwNum, temp, AddComma( pBuyItem->GetMoney() * dwNum) );

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

void CStreetStallManager::BuyItem()
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	POSTYPE pos = pDlg->GetCurSelectedItemNum();
	DWORD dwCheckDBIdx = pDlg->GetCheckDBIdx();

	m_dwWantBuyNum = 1;

	CExchangeItem* pItem = (CExchangeItem*)pDlg->GetCurSelectedItem();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pDlg->GetItemMoney( pos ) > HERO->GetMoney() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pDlg->GetItemGold( pos ) > HERO->GetGoldMoney() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2105) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETBUYMSG, MBT_YESNO,CHATMGR->GetChatMsg(2109),pItem->GetItemName(),pDlg->GetItemMoney( pos ),pDlg->GetItemGold( pos ));
}

void CStreetStallManager::BuyItemDur( POSTYPE pos, DWORD dwNum )
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	DWORD dwCheckDBIdx = pDlg->GetCheckDBIdx();

	m_dwWantBuyNum = dwNum;

	CExchangeItem* pItem = (CExchangeItem*)pDlg->GetItem( (POSTYPE)pos );
	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	
	if( pDlg->GetItemMoney( pos )*dwNum > HERO->GetMoney() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_STREETBUYMSG, MBT_YESNO, CHATMGR->GetChatMsg(2106), pItem->GetItemName(), dwNum,pDlg->GetItemMoney( pos ),pDlg->GetItemGold( pos ),pDlg->GetItemMoney( pos )*dwNum,pDlg->GetItemGold( pos )*dwNum);
}

void CStreetStallManager::ItemStatus( CExchangeItem* pItem, BOOL bLock )
{
	if( !pItem ) return;

	POSTYPE pos = pItem->GetPosition();

	GAMEIN->GetStreetStallDialog()->SetDisable( TRUE );

	MSG_STREETSTALL_ITEMSTATUS msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;

	if( bLock )
	{
		msg.Protocol = MP_STREETSTALL_LOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = *pItem->GetLinkItem()->GetItemBaseInfo();
		msg.nMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );
		msg.nGold  = GAMEIN->GetStreetStallDialog()->GetItemGold( pos ); 
	}
	else
	{
		msg.Protocol = MP_STREETSTALL_UNLOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = *pItem->GetLinkItem()->GetItemBaseInfo();
		msg.nMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );		
		msg.nGold  = GAMEIN->GetStreetStallDialog()->GetItemGold( pos );  
	}
	if (msg.nGold < 0 || msg.nMoney < 0)
		return;
	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_ITEMSTATUS) );
}

void CStreetStallManager::ItemStatus( POSTYPE pos, BOOL bLock )
{
	CExchangeItem* pItem;

	pItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetItem(pos);
	
	if( !pItem ) return;


	MSG_STREETSTALL_ITEMSTATUS msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;

	if( bLock )
	{
		msg.Protocol = MP_STREETSTALL_LOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = *pItem->GetLinkItem()->GetItemBaseInfo();
		msg.nMoney = GAMEIN->GetStreetStallDialog()->GetItemMoney( pos );
		msg.nGold  = GAMEIN->GetStreetStallDialog()->GetItemGold( pos ); 
	}
	else
	{
		msg.Protocol = MP_STREETSTALL_UNLOCKITEM_SYN;
		msg.wAbsPosition = pos;
		msg.ItemInfo = *pItem->GetLinkItem()->GetItemBaseInfo();
		msg.nMoney = 0;
		msg.nGold	= 0;  
	}
	if (msg.nGold < 0 || msg.nMoney < 0)
		return;
	NETWORK->Send( &msg, sizeof(MSG_STREETSTALL_ITEMSTATUS) );
}

void CStreetStallManager::ChangeDialogState()
{
	switch(m_wStallKind)
	{
	case eSK_NULL:
		{
			GAMEIN->GetStallKindSelectDialog()->Show();
			
			m_bOpenMsgBox = FALSE;
		}
		return;
	case eSK_SELL:
		{
			CStreetStall* pStall = GAMEIN->GetStreetStallDialog();
			DWORD state = pStall->GetDlgState();
			
			if( !m_bOpenMsgBox ) return;
			
			if( state == eSDS_NOT_OPENED )
			{
				GAMEIN->GetStallKindSelectDialog()->Show();
				
				m_bOpenMsgBox = FALSE;
			}
			
			if( state == eSDS_OPENED )
			{
				if( !pStall->IsActive() )

					GAMEIN->GetStreetStallDialog()->ShowSellStall();
			}
		}
		return;
	case eSK_BUY:
		{
			CStreetBuyStall* pBuyStall = GAMEIN->GetStreetBuyStallDialog();
			DWORD state = pBuyStall->GetDlgState();
			
			if( !m_bOpenMsgBox ) return;
			
			if( state == eBSDS_NOT_OPENED )
			{
				GAMEIN->GetStallKindSelectDialog()->Show();
				
				m_bOpenMsgBox = FALSE;
			}
			
			if( state == eBSDS_OPENED )
			{
				if( !pBuyStall->IsActive() )
					GAMEIN->GetStreetBuyStallDialog()->ShowBuyStall();
			}
		}
		return;
	default:
		return;
	}
}

void CStreetStallManager::LinkBuyItem( STREETSTALL_INFO* pStallInfo )
{
	CStreetStall* pDlg = GAMEIN->GetStreetStallDialog();
	int i;

	CAddableInfoIterator iter(&pStallInfo->AddableInfo);
	BYTE AddInfoKind;
	while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		switch(AddInfoKind)
		{
		case(CAddableInfoList::ItemOption):
			{
				ITEM_OPTION_INFO OptionInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&OptionInfo);
				ITEMMGR->InitItemOption(OptionInfo, pStallInfo->count);
			}
			break;
		case(CAddableInfoList::ItemRareOption):
			{
				ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&RareOptionInfo);
				ITEMMGR->InitItemRareOption(RareOptionInfo, pStallInfo->RareCount);
			}
			break;
		case(CAddableInfoList::PetTotalInfo):
			{
				PET_TOTALINFO PetInfo[SLOT_STREETSTALL_NUM];
				iter.GetInfoData(&PetInfo);
				PETMGR->InitPetInfo(PetInfo, pStallInfo->PetItemCount);
			}
			break;
		}
		iter.ShiftToNextData();
	}

	for( i=0;i<SLOT_STREETSTALL_NUM;++i)
	{
		if( pStallInfo->Item[i].Fill )
		{
			char buf[128] = {0,};
			char buf2[128] = {0,};

	        char buf3[128] = {0,};
	        char buf4[128] = {0,};

			CExchangeItem* pExItem = new CExchangeItem;
			cImage tmpImage;
			pExItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
				ITEMMGR->GetIconImage( pStallInfo->Item[i].wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
			pExItem->SetData( pStallInfo->Item[i].wIconIdx );
			pExItem->SetDBId( pStallInfo->Item[i].dwDBIdx );
			pExItem->SetDurability( pStallInfo->Item[i].Durability );
			pExItem->SetRareness( pStallInfo->Item[i].dwRareIdx );
			pExItem->SetItemParam( pStallInfo->Item[i].ItemParam );
			pExItem->SetGrow(pStallInfo->Item[i].dwGrow);         
			//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
			pExItem->SetAbrasion(pStallInfo->Item[i].wAbrasion);
			pExItem->SetMaxItemAbrasion(pStallInfo->Item[i].wMaxItemAbrasion);
			pExItem->SetItemIdx(pStallInfo->Item[i].wIconIdx);
			pExItem->SetMaker(pStallInfo->Item[i].cMaker);
			pExItem->SetMaker1(pStallInfo->Item[i].cMaker1);
			if( ITEMMGR->IsTitanEquipItem( pStallInfo->Item[i].wIconIdx ) )
			{
				ITEMMGR->SetToolTipIcon( pExItem, NULL, NULL, pStallInfo->Item[i].dwDBIdx );
			}
			else if( ITEMMGR->IsTitanCallItem( pStallInfo->Item[i].wIconIdx ) )
			{
				ITEMMGR->SetToolTipIcon( pExItem, NULL, NULL, pStallInfo->Item[i].dwDBIdx );
			}
			else if( ITEMMGR->IsPetSummonItem( pStallInfo->Item[i].wIconIdx ) )
			{
				ITEMMGR->SetToolTipIcon( pExItem, NULL, NULL, pStallInfo->Item[i].dwDBIdx );
			}
			else if( !ITEMMGR->IsDupItem(pStallInfo->Item[i].wIconIdx) && pStallInfo->Item[i].Durability !=0 )
			{
				ITEMMGR->SetToolTipIcon( pExItem, ITEMMGR->GetItemOption(pStallInfo->Item[i].Durability),
					ITEMMGR->GetItemRareOption(pStallInfo->Item[i].dwRareIdx),0);
			}
			else
				ITEMMGR->SetToolTipIcon( pExItem, NULL, ITEMMGR->GetItemRareOption(pStallInfo->Item[i].dwRareIdx),0);

			if( pStallInfo->Item[i].Locked  )
			{
				if( ITEMMGR->IsDupItem(pStallInfo->Item[i].wIconIdx) )
				{
					wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pStallInfo->Item[i].money ) );
					wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pStallInfo->Item[i].Durability * pStallInfo->Item[i].money ) );

					wsprintf( buf3, CHATMGR->GetChatMsg(2102), AddComma( pStallInfo->Item[i].gold ) );
					wsprintf( buf4, CHATMGR->GetChatMsg(2103), AddComma( pStallInfo->Item[i].Durability * pStallInfo->Item[i].gold ));
				}
				else
					wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pStallInfo->Item[i].money ) );
				wsprintf( buf3, CHATMGR->GetChatMsg(2104), AddComma( pStallInfo->Item[i].gold ) ); 
			}
			else
				wsprintf( buf, CHATMGR->GetChatMsg(114) );

			pExItem->AddToolTipLine( buf, TTTC_BUYPRICE );

			pExItem->AddToolTipLine( buf3, TTTC_BUYPRICE );

			if(buf2[0])
				pExItem->AddToolTipLine( buf2, TTTC_BUYPRICE );

			if(buf4[0])
		        pExItem->AddToolTipLine( buf4, TTTC_BUYPRICE ); 

			pExItem->SetLock( !pStallInfo->Item[i].Locked );
			pExItem->SetMovable( FALSE );

			int pos = pDlg->AddItem( pExItem );
			if( pos == -1 ) 
			{
				SAFE_DELETE(pExItem);
				continue;
			}

			WINDOWMGR->AddWindow( pExItem );
			pExItem->SetPosition( pos );

			pDlg->RegistMoney( i, pStallInfo->Item[i].money );

			pDlg->RegistGold( i,pStallInfo->Item[i].gold); 
			
			m_nLinkedItemNum++;
		}
	}

	pDlg->EditTitle( pStallInfo->Title );
	pDlg->SetStallOwnerId( pStallInfo->StallOwnerID );
}

void CStreetStallManager::LinkSellItem( STREETSTALL_INFO* pStallInfo )
{
	CStreetBuyStall* pDlg = GAMEIN->GetStreetBuyStallDialog();
	char buf[128] = {0,};
	char buf2[128] = {0,};
	int i;
	ITEMBASE ItemInfo;

	memset(&ItemInfo, 0, sizeof(ITEMBASE));
	
	for( i=0;i<SLOT_STREETSTALL_NUM;++i)
	{
		if( pStallInfo->Item[i].Fill )
		{
			CBuyItem* pBuyItem = new CBuyItem;

			cImage tmpImage;

			ItemInfo.wIconIdx = pStallInfo->Item[i].wIconIdx;
			ItemInfo.ItemParam = pStallInfo->Item[i].ItemParam;

			pBuyItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
				ITEMMGR->GetIconImage( pStallInfo->Item[i].wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
			pBuyItem->SetType(WT_BUYITE);
			pBuyItem->InitItem( ItemInfo, pStallInfo->Item[i].wVolume, pStallInfo->Item[i].money );
			pBuyItem->SetData( pStallInfo->Item[i].wIconIdx );
			
			ITEMMGR->SetToolTipIcon(pBuyItem);
			
			char buf[32];
			char buf2[32] = {0,};
			
			if(pStallInfo->Item[i].wVolume > 1)
			{
				wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pStallInfo->Item[i].money ) );
				wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pStallInfo->Item[i].money * pStallInfo->Item[i].wVolume ));
			}
			else
				wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pStallInfo->Item[i].money ) );	
			
			pBuyItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			if(buf2[0])
				pBuyItem->AddToolTipLine( buf2, TTTC_BUYPRICE );
			
			pBuyItem->SetMovable( FALSE );
			pBuyItem->SetLock(pStallInfo->Item[i].Locked);
					
			if( !pDlg->AddItem( i, pBuyItem ) ) 
			{
				SAFE_DELETE(pBuyItem);
				continue;
			}
			
			WINDOWMGR->AddWindow( pBuyItem );

			pDlg->RegistMoney( i, pStallInfo->Item[i].money );

			
			m_nLinkedItemNum++;
		}
	}
	
	pDlg->EditTitle( pStallInfo->Title );
	pDlg->SetStallOwnerId( pStallInfo->StallOwnerID );
	pDlg->SetCurSelectedItemNum( -1 );
}

void CStreetStallManager::NetworkMsgParse( BYTE Protocol, void* pMsg )

{
 	if(HERO == NULL)
		return;
	switch( Protocol ) 
	{
	case MP_STREETSTALL_OPEN_ACK:
		{
			MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;

			if( HERO->GetShopItemStats()->dwStreetStallDecoration > 0 )
				AddStreetStallDecoration(HERO, msg->StallKind);

			ToggleHero_StreetStallMode( TRUE );

			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			
			m_wStallKind = msg->StallKind;	
			
			if(m_wStallKind == eSK_SELL)
			{
				GAMEIN->GetStreetStallDialog()->RegistTitle( msg->Title, TRUE );
				GAMEIN->GetStreetStallDialog()->ShowSellStall();
			}
			else if(m_wStallKind == eSK_BUY)
			{
				GAMEIN->GetStreetBuyStallDialog()->RegistTitle( msg->Title, TRUE );
				GAMEIN->GetStreetBuyStallDialog()->ShowBuyStall();
			}

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(343) );
		}
		break;
	case MP_STREETSTALL_OPEN_NACK:
		{
			
			if(m_wStallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );
			else if(m_wStallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->RegistTitle( NULL, FALSE );

			if( HERO->GetState() == eObjectState_StreetStall_Owner )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Owner );
			
			m_bOpenMsgBox = TRUE;
			m_wStallKind = eSK_NULL;	
		}
		break;
	case MP_STREETSTALL_CLOSE_ACK:
		{
			OBJECTMGR->SetRemoveDecorationInfo(HERO);
			if(m_wStallKind == eSK_SELL)

			{
				GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);
				HERO->ShowStreetStallTitle( FALSE, NULL );
			}
			else if(m_wStallKind == eSK_BUY)
			{
				GAMEIN->GetStreetBuyStallDialog()->OnCloseStall();
				HERO->ShowStreetBuyStallTitle( FALSE, NULL );
			}
			
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);

			ToggleHero_StreetStallMode( FALSE );

			m_nLinkedItemNum = 0;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(344) );
			m_bOpenMsgBox = TRUE;
			m_wStallKind = eSK_NULL;
		}
		break;
	case MP_STREETSTALL_CLOSE_NACK:
		{
		}
		break;
	case MP_STREETSTALL_FAKEREGISTITEM_ACK:
		{
			MSG_LINKITEMEX* msg = (MSG_LINKITEMEX*)pMsg;

			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( !pItem )
			{
				ASSERT(0);
				return;
			}
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
			if( LinkItem( pItem, &msg->ItemInfo ) )
				GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney,msg->dwGold, FALSE, 1 );	//TRUE  2014-10-30 检查后注册
		}
		break;
	case MP_STREETSTALL_FAKEREGISTITEM_NACK:
		{
			MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;

			--m_nLinkedItemNum; 
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );

			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( (POSTYPE)msg->dwData1 );
			if(!pItem)
			{
				ASSERT(0);
				return;
			}
			
			if( msg->dwData2 != 1 ) 
				pItem->SetLock( FALSE );
			
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_FAKEREGISTITEM:
		{
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			SEND_LINKITEM_TOTALINFO* msg = (SEND_LINKITEM_TOTALINFO*)pMsg;

			if( msg->ItemInfo.wIconIdx == 0 )
			{
				ASSERT(msg->ItemInfo.wIconIdx);
				return;
			}


			CExchangeItem* pExItem = new CExchangeItem;
			cImage tmpImage;
			pExItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
				ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
			pExItem->SetData( msg->ItemInfo.wIconIdx );
			pExItem->SetDBId( msg->ItemInfo.dwDBIdx);
			pExItem->SetDurability( msg->ItemInfo.Durability );
			pExItem->SetItemParam( msg->ItemInfo.ItemParam );
			pExItem->SetRareness( msg->ItemInfo.RareIdx );
			pExItem->SetGrow(msg->ItemInfo.ItemGrow);      
			//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
			pExItem->SetMaker(msg->ItemInfo.ItemMaker);
			pExItem->SetMaker1(msg->ItemInfo.ItemMaker1);
			pExItem->SetAbrasion(msg->ItemInfo.ItemAbrasion);
			pExItem->SetMaxItemAbrasion(msg->ItemInfo.MaxItemAbrasion);
			pExItem->SetLock( TRUE );
			pExItem->SetMovable( FALSE );

			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pExItem, (MSG_LINKITEMOPTIONS*)msg);

			char buf[32];
			wsprintf( buf, CHATMGR->GetChatMsg(114) );
			
			pExItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			
			int pos = GAMEIN->GetStreetStallDialog()->AddItem( pExItem );
			if( pos == -1 ) 
			{
				SAFE_DELETE(pExItem);
				return;
			}
			
			WINDOWMGR->AddWindow( pExItem );
			pExItem->SetPosition( pos );
			GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->dwMoney,msg->dwGold, FALSE ); 
		}
		break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM_ACK:
		{
			MSG_LINKBUYITEMEX* msg = (MSG_LINKBUYITEMEX*)pMsg;

			GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
			if( LinkItem( msg->wAbsPosition, msg->ItemInfo, msg->wVolume, msg->dwMoney ) )
				GAMEIN->GetStreetBuyStallDialog()->ChangeItemStatus( msg->wAbsPosition, msg->wVolume, msg->dwMoney);
		}
		break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM_NACK:
		{
			MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;

			--m_nLinkedItemNum; 
			switch(msg->dwData2) 
			{
			case 0:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
				break;
			case 1:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1213) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1214) );
				break;
			}
			

			GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_FAKEREGISTBUYITEM:		
		{	
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			SEND_LINKBUYITEM_TOTALINFO* msg = (SEND_LINKBUYITEM_TOTALINFO*)pMsg;

			if( msg->ItemInfo.wIconIdx == 0 )

			{
				ASSERT(msg->ItemInfo.wIconIdx);
				return;
			}

			CBuyItem* pBuyItem = new CBuyItem;
			cImage tmpImage;

			ITEMBASE ItemInfo;
			
			memset(&ItemInfo, 0, sizeof(ITEMBASE));
			
			ItemInfo.wIconIdx = msg->ItemInfo.wIconIdx;
			ItemInfo.ItemParam = msg->ItemInfo.ItemParam;
			
			pBuyItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
			ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );
			pBuyItem->InitItem(ItemInfo, msg->wVolume, msg->dwMoney);
			pBuyItem->SetData( msg->ItemInfo.wIconIdx );
		
			pBuyItem->SetMovable( FALSE );
			ITEMMGR->SetToolTipIcon( pBuyItem );

			CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem(msg->wAbsPosition);

			if( !pItem )
				GAMEIN->GetStreetBuyStallDialog()->DeleteItem(msg->wAbsPosition);
			
			if( !GAMEIN->GetStreetBuyStallDialog()->AddItem( msg->wAbsPosition, pBuyItem ) ) 
			{
				SAFE_DELETE(pBuyItem);
				return;
			}
			
			WINDOWMGR->AddWindow( pBuyItem );
			GAMEIN->GetStreetBuyStallDialog()->ChangeItemStatus( msg->wAbsPosition, msg->wVolume, msg->dwMoney);
			GAMEIN->GetStreetBuyStallDialog()->SetCurSelectedItemNum(-1);
		}
		break;
	case MP_STREETSTALL_LOCKITEM_ACK:
		{
			MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
			GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->nMoney,msg->nGold ,TRUE ); 
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_LOCKITEM_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(350) );
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_LOCKITEM:
		{
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
			GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->nMoney,msg->nGold, TRUE ); 
		}
		break;
	case MP_STREETSTALL_UNLOCKITEM_ACK:
		{
			MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
			GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->nMoney,msg->nGold, FALSE, 2 ); 
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_UNLOCKITEM_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(351) );
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_UNLOCKITEM:
		{
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			cDialog* pDlg = WINDOWMGR->GetWindowForID(MBI_STREETBUYMSG);
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			MSG_STREETSTALL_ITEMSTATUS* msg = (MSG_STREETSTALL_ITEMSTATUS*)pMsg;
			GAMEIN->GetStreetStallDialog()->ChangeItemStatus( &msg->ItemInfo, msg->nMoney,msg->nGold, FALSE ); 
		}
		break;
	case MP_STREETSTALL_EDITTITLE_ACK:
		{
			MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;
			
			if(msg->StallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->RegistTitle( msg->Title, TRUE );
			else if(msg->StallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->RegistTitle( msg->Title, TRUE );

			m_wStallKind = msg->StallKind;
		}
		break;
	case MP_STREETSTALL_EDITTITLE_NACK:
		{
			if(m_wStallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->RegistTitle( NULL, FALSE );
			else if(m_wStallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->RegistTitle( NULL, FALSE );
		}
		break;
	case MP_STREETSTALL_EDITTITLE:
		{
			MSG_STREETSTALL_TITLE* msg = (MSG_STREETSTALL_TITLE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwObjectID );
			if(!pPlayer) return;
			if(msg->StallKind == eSK_SELL)
				pPlayer->SetStreetStallTitle( msg->Title );
			else if(msg->StallKind == eSK_BUY)
				pPlayer->SetStreetBuyStallTitle( msg->Title );

			if( HERO->GetState() == eObjectState_StreetStall_Guest )
			{
				if(msg->StallKind == eSK_SELL)
				{
					if( GAMEIN->GetStreetStallDialog()->GetStallOwnerId() == msg->dwObjectID )
					{
						GAMEIN->GetStreetStallDialog()->EditTitle( msg->Title );
					}
				}
				else if(msg->StallKind == eSK_BUY)
				{
					if( GAMEIN->GetStreetBuyStallDialog()->GetStallOwnerId() == msg->dwObjectID )
					{
						GAMEIN->GetStreetBuyStallDialog()->EditTitle( msg->Title );
					}
				}
			}
		}
		break;
	case MP_STREETSTALL_BUYITEM_ACK:
		{
			MSG_ITEMEX* msg = (MSG_ITEMEX*)pMsg;

			CItem* pItem = ITEMMGR->MakeNewItem(&msg->ItemInfo,"MP_STREETSTALL_BUYITEM_ACK");

			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, (MSG_LINKITEMOPTIONS*)msg);

			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
			QUICKMGR->RefreshQickItem();

			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
			GAMEIN->GetStreetStallDialog()->SetData(NULL);
		}
		break;

	case MP_STREETSTALL_DELETEITEM_ACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			if(m_wStallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
			else if(m_wStallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->DeleteItem( msg->wAbsPosition );

			--m_nLinkedItemNum;
		}
		break;
	case MP_STREETSTALL_DELETEITEM_NACK:
		break;
	case MP_STREETSTALL_DELETEITEM:
		{
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			--m_nLinkedItemNum;

			if( !ITEMMGR->IsDupItem( msg->ItemInfo.wIconIdx ) && msg->ItemInfo.Durability != 0 )
				ITEMMGR->RemoveItemOption(msg->ItemInfo.Durability);

			if(m_wStallKind == eSK_SELL)
			{
				GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
				GAMEIN->GetStreetStallDialog()->SetCurSelectedItemNum(-1);
				//GAMEIN->GetStreetStallDialog()->ResetDlgData();
			}
			else if(m_wStallKind == eSK_BUY)
			{
				GAMEIN->GetStreetBuyStallDialog()->DeleteItem( msg->wAbsPosition );
				GAMEIN->GetStreetBuyStallDialog()->SetCurSelectedItemNum(-1);
				//GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
			}
		}
		break;
	case MP_STREETSTALL_BUYITEM_NACK:
		{
			MSG_WORD* msg  = (MSG_WORD*)pMsg;

			switch(msg->wData)
			{
			case NOT_EXIST:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
				break;
			case NOT_MONEY:

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				break;
			case NOT_SPACE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
				break;
			case NOT_OWNERMONEY:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(364) );
				break;
			}

	
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
			GAMEIN->GetStreetStallDialog()->SetDisable( FALSE );
			GAMEIN->GetStreetStallDialog()->SetData(NULL);
		}
		break;
	case MP_STREETSTALL_SELLITEM_ACK:
		{
			MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;
			
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( !pItem )
			{
				ASSERT(0);
				return;
			}

			if( msg->count == 0 )
			{
				CItem* pOutItem = NULL;
				ITEMMGR->DeleteItem( msg->ItemInfo.Position, &pOutItem );
			}
			else 
			{
				pItem->SetDurability( msg->count );
				pItem->SetLock( FALSE );
			}

			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
			GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
			GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
		}
		break;
	case MP_STREETSTALL_SELLITEM_NACK:
		{
			MSG_WORD2* msg  = (MSG_WORD2*)pMsg;
			
			switch(msg->wData1)
			{
			case NOT_EXIST:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );
				break;
			case NOT_MONEY:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1217) );
				break;
			case NOT_SPACE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1216) );
				break;
			}
			
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->wData2 );
			if(pItem)	pItem->SetLock(FALSE);
			
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
			GAMEIN->GetStreetBuyStallDialog()->SetDisable( FALSE );
			GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
		}
		break;
	case MP_STREETSTALL_SELLITEM:
		{
			MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;
			
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( !pItem )
			{
				ASSERT(0);
				return;
			}
			
			if( msg->count == 0 )
			{
				CItem* pOutItem = NULL;
				ITEMMGR->DeleteItem( msg->ItemInfo.Position, &pOutItem );
				
				GAMEIN->GetStreetStallDialog()->DeleteItem( &msg->ItemInfo );
				--m_nLinkedItemNum;
			}
			else 
			{
				pItem->SetDurability( msg->count );
				GAMEIN->GetStreetStallDialog()->ResetItemInfo( msg->ItemInfo.dwDBIdx, msg->count );
			}
		}
		break;
	case MP_STREETSTALL_SELLITEM_ERROR:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(365) );
		}
		break;
	case MP_STREETSTALL_BUYITEM:
		{
			MSG_BUYITEM* msg = (MSG_BUYITEM*)pMsg;
			
			CItem* pItem = ITEMMGR->MakeNewItem(&msg->ItemInfo,"MP_STREETSTALL_BUYITEM");

			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, (MSG_LINKITEMOPTIONS*)msg);


			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(pItem);
			QUICKMGR->RefreshQickItem();

		}
		break;
	case MP_STREETSTALL_BUYITEM_ERROR:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			switch(msg->wData) 
			{
			case NOT_SPACE:
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
				break;
			case NOT_MONEY:
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				break;
			}
		}
		break;
	case MP_STREETSTALL_UPDATEITEM:
		{
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_ShopInven );
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest 
				&& OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Owner ) 
				return;

			MSG_SELLITEM* msg = (MSG_SELLITEM*)pMsg;

			if(m_wStallKind == eSK_SELL)
			{
				CExchangeItem* pExItem = (CExchangeItem*)GAMEIN->GetStreetStallDialog()->GetItem(msg->ItemInfo.dwDBIdx);
				ASSERT( pExItem );
				if( pExItem == NULL ) return;
				
				pExItem->SetDurability( msg->count );

				BOOL bState = FALSE;

				if( GAMEIN->GetStreetStallDialog()->GetDlgState() == eSDS_BUY ) 	
				{
					bState = TRUE;
				}
				
				GAMEIN->GetStreetStallDialog()->ChangeItemStatus(pExItem->GetPosition(),GAMEIN->GetStreetStallDialog()->GetItemMoney(pExItem->GetPosition()),GAMEIN->GetStreetStallDialog()->GetItemGold(pExItem->GetPosition()),bState);  // 摆摊 2014-10-30
					
				GAMEIN->GetStreetStallDialog()->ResetDlgData();
			}
			else if(m_wStallKind == eSK_BUY)
			{
				CBuyItem* pBuyItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem((POSTYPE)msg->dwData);
				ASSERT( pBuyItem );
				if( pBuyItem == NULL ) return;

				pBuyItem->SetVolume( (WORD)msg->count );
				GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
			}
		}
		break;
	case MP_STREETSTALL_GUESTIN_ACK:
		{
			STREETSTALL_INFO* msg = (STREETSTALL_INFO*)pMsg;

			m_wStallKind = msg->StallKind;
			
			if(m_wStallKind == eSK_SELL)
			{
				LinkBuyItem( msg );
				GAMEIN->GetStreetStallDialog()->ShowBuyStall();
			}
			else if(m_wStallKind == eSK_BUY)
			{
				LinkSellItem( msg );
				GAMEIN->GetStreetBuyStallDialog()->ShowSellStall();
			}

			GAMEIN->GetInventoryDialog()->SetActive(TRUE);
			
			if( HERO->GetState() != eObjectState_StreetStall_Guest )
				OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_StreetStall_Guest);

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->StallOwnerID );
			if( pPlayer )
			{
				pPlayer->SetStreetStallTitle( msg->Title );
				if(m_wStallKind == eSK_SELL)
				{
					pPlayer->SetStreetStallTitle( msg->Title );
				}
				else if(m_wStallKind == eSK_BUY)
				{
					pPlayer->SetStreetBuyStallTitle( msg->Title );
				}
				
			}
		}
		break;
	case MP_STREETSTALL_GUESTIN_NACK:
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Guest );
		}
		break;
	case MP_STREETSTALL_GUESTOUT_ACK:
		{
			if(m_wStallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);	
			else if(m_wStallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->OnCloseStall(TRUE);	

			GAMEIN->GetInventoryDialog()->SetActive(FALSE);

			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_StreetStall_Guest);
			
			m_wStallKind = eSK_NULL;
			m_nLinkedItemNum = 0;

			DWORD	dwObjectIndex = GAMEIN->GetStallFindDlg()->GetSelectedObjectIndex();
			DWORD	dwStallType = GAMEIN->GetStallFindDlg()->GetPrevSelectedType();
			if( dwObjectIndex > 0 )
			{

				CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( dwObjectIndex );
				if( pPlayer )
				{
					pPlayer->SetStreetStallBalloonImage( dwStallType, FALSE );
				}

				GAMEIN->GetStallFindDlg()->SetSelectedObjectIndex(0);
				GAMEIN->GetStallFindDlg()->SetPrevSelectedType(0);
			}
		}
		break;
	case MP_STREETSTALL_GUESTOUT_NACK:
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_StreetStall_Guest );
		}
		break;
	case MP_STREETSTALL_START:
		{
			CObject* pObject;
			MSG_STREETSTALL_TITLE* pmsg = (MSG_STREETSTALL_TITLE*)pMsg;
			pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pObject) return;
			Toggle_StreetStallMode(pObject, TRUE);

			if(((CPlayer*)pObject)->GetShopItemStats()->dwStreetStallDecoration > 0 )
                AddStreetStallDecoration((CPlayer*)pObject, pmsg->StallKind);
			
			if(pmsg->StallKind == eSK_SELL)
				((CPlayer*)pObject)->ShowStreetStallTitle( TRUE, pmsg->Title );
			else if(pmsg->StallKind == eSK_BUY)
				((CPlayer*)pObject)->ShowStreetBuyStallTitle( TRUE, pmsg->Title );
		}
		break;
	case MP_STREETSTALL_END:
		{
			CObject* pObject;
			MSG_STREETSTALL_TITLE* pmsg = (MSG_STREETSTALL_TITLE*)pMsg;
			pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			if(!pObject) return;
			OBJECTMGR->SetRemoveDecorationInfo((CPlayer*)pObject);

			if( pObject->GetID() != HERO->GetID() )
			{
				Toggle_StreetStallMode(pObject, FALSE);

				DWORD	dwObjectIndex = GAMEIN->GetStallFindDlg()->GetSelectedObjectIndex();
				DWORD	dwStallType = GAMEIN->GetStallFindDlg()->GetPrevSelectedType();
				if( dwObjectIndex == pObject->GetID() )
				{
					((CPlayer*)pObject)->SetStreetStallBalloonImage( dwStallType, FALSE );
					GAMEIN->GetStallFindDlg()->SetSelectedObjectIndex(0);
					GAMEIN->GetStallFindDlg()->SetPrevSelectedType(0);
				}
			}
			else
			{
				if(pmsg->StallKind == eSK_SELL)
					GAMEIN->GetStreetStallDialog()->OnCloseStall();
				else if(pmsg->StallKind == eSK_BUY)
					GAMEIN->GetStreetBuyStallDialog()->OnCloseStall();

				m_nLinkedItemNum = 0;
				m_bOpenMsgBox = TRUE;
				m_wStallKind = eSK_NULL;

				GAMEIN->GetInventoryDialog()->SetActive(FALSE);
			}
			if(pmsg->StallKind == eSK_SELL)
				((CPlayer*)pObject)->ShowStreetStallTitle( FALSE, NULL );
			else if(pmsg->StallKind == eSK_BUY)
				((CPlayer*)pObject)->ShowStreetBuyStallTitle( FALSE, NULL );
			
		}
		break;
	case MP_STREETSTALL_CLOSE:
		{
			if( OBJECTSTATEMGR->GetObjectState(HERO) != eObjectState_StreetStall_Guest) return;

			MSGBASE* msg = (MSGBASE*)pMsg;
			
			if(m_wStallKind == eSK_SELL)
				GAMEIN->GetStreetStallDialog()->OnCloseStall(TRUE);
			else if(m_wStallKind == eSK_BUY)
				GAMEIN->GetStreetBuyStallDialog()->OnCloseStall(TRUE);
			
			m_nLinkedItemNum = 0;

			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
			
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_StreetStall_Guest);

		}
		break;
	case MP_STREETSTALL_UPDATE:
		{

			MSG_WORD* msg = (MSG_WORD*)pMsg;

			CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem( msg->wData );

			if(pItem)
			{
				pItem->SetActive( FALSE );
				pItem->SetLock( TRUE );
			}

			GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
		}
		break;
	case MP_STREETSTALL_UPDATEEND:
		{

			for(int i = 0; i < SLOT_STREETBUYSTALL_NUM; i++)
			{
				CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetItem( i );
				
				if(pItem)
				{
					pItem->SetActive( TRUE );
					pItem->SetLock( FALSE );
				}
			}

			GAMEIN->GetStreetBuyStallDialog()->ResetDlgData();
		}
		break;
	case MP_STREETSTALL_UPDATE_ACK:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			
			POSTYPE pos = msg->wData;
			BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);


			GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
			ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();
			
			if(!pItem)
				return;

			char buf[32] = {0,};
			char buf2[32] = {0,};
			
			SafeStrCpy(buf,AddComma(RegInfo.Money), 32);
			SafeStrCpy(buf2,AddComma(RegInfo.Money * RegInfo.Volume), 32);
			
			if(RegInfo.Volume > 1)
			{
				WINDOWMGR->MsgBox(MBI_REGEDITMSG, MBT_YESNO, CHATMGR->GetChatMsg(1212), 
					pItem->ItemName,
					RegInfo.Volume,
					buf, buf2);
				
			}
			else
			{
				WINDOWMGR->MsgBox(MBI_REGEDITMSG, MBT_YESNO, CHATMGR->GetChatMsg(1211), 
					pItem->ItemName,
					RegInfo.Volume,
					buf);

			}

			GAMEIN->GetBuyRegDialog()->InitRegInfo();
		}
		break;
	case MP_STREETSTALL_UPDATE_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1224) );
		}
		break;
	case MP_STREETSTALL_UPDATEEND_ACK:
		{
			
		}
		break;

	case MP_STREETSTALL_UPDATEEND_NACK:
		{
			
		}
		break;
	case MP_STREETSTALL_MESSAGE:
		{
			MSG_STALLMSG* msg = (MSG_STALLMSG*)pMsg;

			ITEM_INFO* pItem = ITEMMGR->GetItemInfo( msg->ItemIdx );

			CPlayer* pPlayer = NULL;

			if( HEROID == msg->SellerID )
			{
				pPlayer = (CPlayer*) OBJECTMGR->GetObject( msg->BuyerID );

				if( pPlayer )
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2107),  pPlayer->GetObjectName(),  pItem->ItemName, msg->count, msg->money,msg->gold);
			}
			else if( HEROID == msg->BuyerID )
			{
				pPlayer = (CPlayer*) OBJECTMGR->GetObject( msg->SellerID );

				if( pPlayer ) 
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2108),  pPlayer->GetObjectName(),  pItem->ItemName, msg->count, msg->money,msg->gold);
			}
		}
		break;
	case MP_STREETSTALL_ITEMVIEW_ACK:
		{
			MSG_STREETSTALL_ITEMVIEW* pmsg = (MSG_STREETSTALL_ITEMVIEW*)pMsg;

			GAMEIN->GetStreetStallItemViewDlg()->LinkItem( pmsg );
			GAMEIN->GetStallFindDlg()->SetDisable( FALSE );
		}
		break;

	case MP_STREETSTALL_ITEMVIEW_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1694) );
			GAMEIN->GetStallFindDlg()->SetDisable( FALSE );
		}
		break;
	case MP_STREETSTALL_FINDITEM_ACK:
		{
			SEND_STREETSTALL_INFO *	msg = (SEND_STREETSTALL_INFO*)pMsg;
            CStallFindDlg * pDlg = GAMEIN->GetStallFindDlg();

			pDlg->SetDisable( FALSE );

			pDlg->SetStallPriceInfo( msg );
		}
		break;
	case MP_STREETSTALL_FINDITEM_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( 1 == pmsg->dwData )
			{
				WINDOWMGR->MsgBox( MBI_OK, MBT_OK, CHATMGR->GetChatMsg(1694) );
			}

			GAMEIN->GetStallFindDlg()->SetDisable( FALSE );
		}
		break;
	}
}


void CStreetStallManager::SendBuyItemMsg()
{
	CStreetStall* pDlg	= GAMEIN->GetStreetStallDialog();
	if( pDlg == NULL )				return;
	if( pDlg->IsActive() == FALSE ) return;

	DWORD dwCheckDBIdx	= pDlg->GetCheckDBIdx();
	DURTYPE dwNum			= m_dwWantBuyNum;

	CExchangeItem* pItem = pDlg->FindItem( dwCheckDBIdx );
	
	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	POSTYPE pos			= pItem->GetPosition();

	if( pDlg->GetItemMoney( pos )*dwNum > HERO->GetMoney() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pDlg->GetItemGold( pos )*dwNum > HERO->GetGoldMoney() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2105) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}

	if( GAMEIN->GetInventoryDialog()->GetBlankNum() == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	if( pItem->GetDBId() != dwCheckDBIdx )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(358) );
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return;
	}
	
	ITEMBASE base;
	base.dwDBIdx = pItem->GetDBId();
	base.wIconIdx = (WORD)pItem->GetData();
	base.Durability = dwNum;		

	STREETSTALL_BUYINFO BuyMsg;
	
	BuyMsg.Category = MP_STREETSTALL;
	BuyMsg.Protocol = MP_STREETSTALL_BUYITEM_SYN;
	BuyMsg.dwObjectID = HEROID;
	BuyMsg.StallOwnerID = pDlg->GetStallOwnerId();
	BuyMsg.StallPos = pos;
	BuyMsg.ItemInfo = base;
	BuyMsg.nBuyPrice = pDlg->GetItemMoney( pos );
	BuyMsg.nBuyGold  = pDlg->GetItemGold(pos);   
	
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_ShopInven );
	if (BuyMsg.nBuyGold < 0 || BuyMsg.nBuyPrice < 0)
	{
		sClient->FuckRubbish();
		return;
	}
		
	NETWORK->Send(&BuyMsg, sizeof(STREETSTALL_BUYINFO));
}

BOOL CStreetStallManager::SendSellItemMsg()
{
	CStreetBuyStall* pDlg	= GAMEIN->GetStreetBuyStallDialog();
	if( pDlg == NULL )				return FALSE;
	if( pDlg->IsActive() == FALSE ) return FALSE;
	
	DURTYPE dwNum = m_dwWantSellNum;
	
	CBuyItem* pBuyItem = (CBuyItem*)pDlg->GetCurSelectedItem();
	if( !pBuyItem )
		return FALSE;
	
	CItem* pItem = pDlg->GetSellItem();
	
	POSTYPE pos = pDlg->GetCurSelectedItemNum();

	if( pItem == NULL )
	{
		pDlg->SetData( NULL );
		pDlg->SetDisable( FALSE );
		return FALSE;
	}

	ITEMBASE base;

	memset(&base, 0, sizeof(ITEMBASE));

	base.dwDBIdx = pItem->GetDBIdx();
	base.wIconIdx = (WORD)pItem->GetData();
	base.Durability = dwNum;
	base.Position = pItem->GetPosition();
	base.ItemParam = pItem->GetItemParam();
	base.ItemGrow  = pItem->GetGrow();     
	
	STREETSTALL_BUYINFO SellMsg;
	
	SellMsg.Category = MP_STREETSTALL;
	SellMsg.Protocol = MP_STREETSTALL_SELLITEM_SYN;
	SellMsg.dwObjectID = HEROID;
	SellMsg.StallOwnerID = pDlg->GetStallOwnerId();
	SellMsg.StallPos = pos;
	SellMsg.ItemInfo = base;
	SellMsg.nBuyPrice = pDlg->GetItemMoney( pos );
	
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_ShopInven );

	if (SellMsg.nBuyPrice < 0)
		return FALSE;
	NETWORK->Send(&SellMsg, sizeof(STREETSTALL_BUYINFO));
	pDlg->DelSellItem();

	return TRUE;
}

CMapObject* CStreetStallManager::AddStreetStallDecoration(CPlayer* pPlayer, DWORD dwStreetStallKind)
{
	if( pPlayer->GetShopItemStats()->dwStreetStallDecoration == 0 )
        return NULL;

	CMapObject* pStreetStallDecoration = (CMapObject*)OBJECTMGR->AddDecoration(pPlayer, dwStreetStallKind);
	if( !pStreetStallDecoration )			return NULL;	

	return pStreetStallDecoration;
}




