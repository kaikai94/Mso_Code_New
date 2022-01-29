// ExchangeDialog.cpp: implementation of the CExchangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExchangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cStatic.h"

#include "ExchangeManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "Item.h"

#include "InventoryExDialog.h"
#include "./Input/UserInput.h"

#include "MainBarDialog.h"
#include "ItemManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExchangeDialog::CExchangeDialog()
{
	m_bInit				= FALSE;
	m_pMyIconGridDlg	= NULL;
	m_pOpIconGridDlg	= NULL;
	m_pMyMoneyEditBox	= NULL;
	m_pOpMoneyEditBox	= NULL;
	m_pMyGoldEditBox	= NULL;  
	m_pOpGoldEditBox	= NULL;   
	m_pLockBtn			= NULL;
	m_pExchangeBtn		= NULL;
	m_pInputMoneyBtn	= NULL;
	m_pInputGoldBtn		= NULL;  

	m_pMyNameStc		= NULL;
	m_pOpNameStc		= NULL;


	m_bMyLock			= FALSE;
	m_bOpLock			= FALSE;

	m_dwMoney			= 0;
	m_dwOpMoney			= 0;
	
	m_dwGold			=0;     
	m_dwOpGold			=0;  

	m_bExchangable		= TRUE;

	m_nMyItemNum		= 0;
	m_nOpItemNum		= 0;	
}

CExchangeDialog::~CExchangeDialog()
{

}

void CExchangeDialog::Linking()
{
	m_pMyIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
	m_pOpIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_OPPOSITE );
	m_pMyMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_MYMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );			
	m_pMyMoneyEditBox->SetAlign( TXT_RIGHT );
//	m_pMyMoneyEditBox->SetValidCheck( VCM_NUMBER );
	m_pOpMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_OPMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );
	m_pOpMoneyEditBox->SetAlign( TXT_RIGHT );

	m_pMyGoldEditBox    =(cEditBox*)GetWindowForID( XCI_EB_MYGOLD );

	m_pMyGoldEditBox->SetReadOnly(TRUE);

	m_pMyGoldEditBox->SetAlign(TXT_RIGHT);

	m_pOpGoldEditBox    =(cEditBox*)GetWindowForID( XCI_EB_OPGOLD );

	m_pOpGoldEditBox->SetReadOnly(TRUE);

	m_pOpGoldEditBox->SetAlign(TXT_RIGHT);

	m_pLockBtn			= (cPushupButton*)GetWindowForID( XCI_BTN_LOCK );
	m_pExchangeBtn		= (cPushupButton*)GetWindowForID( XCI_BTN_EXCHANGE );
	m_pInputMoneyBtn	= (cButton*)GetWindowForID( XCI_BTN_INPUTMONEY );
	m_pInputGoldBtn 	= (cButton*)GetWindowForID(  XCI_BTN_INPUTGOLD ); 
	
	m_pMyNameStc		= (cStatic*)GetWindowForID( XCI_INFO2 );
	m_pOpNameStc		= (cStatic*)GetWindowForID( XCI_INFO1 );
	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );

	m_bInit				= TRUE;
}

void CExchangeDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == XCI_BTN_EXIT || lId == XCI_BTN_CLOSE )

		{
			MSGBASE msg;
			msg.Category	= MP_EXCHANGE;
			msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
			msg.dwObjectID	= HERO->GetID();

			NETWORK->Send( &msg, sizeof( MSGBASE ) );
			SetDisable( TRUE );
		}
		else if( lId == XCI_BTN_INPUTMONEY )
		{
			if( m_bMyLock )
			{
			}
			else
			{
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_MONEY,
								x, y, OnInputMoney, OnInputCancel, this, NULL, CHATMGR->GetChatMsg(198) );
				SetDisable( TRUE );
			}
		}
		else if( lId == XCI_BTN_INPUTGOLD )
		{
			if( m_bMyLock )
			{
			}
			else
			{
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_GOLD,
								x, y, OnInputGold, OnInputGoldCancel, this, NULL, CHATMGR->GetChatMsg(2099) );
				SetDisable( TRUE );
			}
		}
	}
	if( we & WE_PUSHDOWN )
	{
		if( lId == XCI_BTN_LOCK )
		{
			if( EXCHANGEMGR->IsLockSyncing() )
			{
				m_pLockBtn->SetPush( FALSE );
			}
			else
			{
				MyLock( TRUE );
				EXCHANGEMGR->Lock( TRUE );	
			}
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			if( m_bMyLock && !EXCHANGEMGR->IsLockSyncing() && m_bOpLock )
			{
				int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
				if( nBlankNum < m_nOpItemNum - m_nMyItemNum )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 354 ) );
					m_pExchangeBtn->SetPush( FALSE );

				}
				else if( m_dwOpMoney > MAX_INVENTORY_MONEY - HERO->GetMoney() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(339) );
					m_pExchangeBtn->SetPush( FALSE );
				}
				else if( m_dwOpGold > MAX_INVENTORY_GOLD - HERO->GetGoldMoney())
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2100) );
					m_pExchangeBtn->SetPush( FALSE );
				}
				else
				{
					EXCHANGEMGR->Exchange();
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(330) );
				m_pExchangeBtn->SetPush( FALSE );
			} 
		}
	}
	if( we & WE_PUSHUP )

	{
		if( lId == XCI_BTN_LOCK )
		{
//#ifdef _HK_LOCAL_
//				m_pLockBtn->SetPush( TRUE );
//#else
			if( EXCHANGEMGR->IsLockSyncing() || EXCHANGEMGR->IsExchangeSyncing() )

			{
				m_pLockBtn->SetPush( TRUE );
			}
			else
			{
				EXCHANGEMGR->Lock( FALSE );	
			}
//#endif
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			m_pExchangeBtn->SetPush( TRUE );
		}
	}
	if( we & WE_LBTNCLICK )
	{
		if( lId == XCI_ICD_MINE )
		{
		}
	}
	
}

void CExchangeDialog::SetPlayerName( char* pMyName, char* pOpName )
{
	m_pMyNameStc->SetStaticText( pMyName );
	m_pOpNameStc->SetStaticText( pOpName );
}

void CExchangeDialog::SetActive( BOOL val )
{
	if( m_bDisable )		return;
	if( m_bActive == val )	return;

	cDialog::SetActive( val );
	
	if( val )
	{
		m_pMyMoneyEditBox->SetEditText( "0" );
		m_pOpMoneyEditBox->SetEditText( "0" );

		m_pMyGoldEditBox->SetEditText("0");
		m_pOpGoldEditBox->SetEditText("0");

		m_pLockBtn->SetPush( FALSE );
		m_pExchangeBtn->SetPush( FALSE );
	}

	else
	{
		if( !m_bInit ) return;	
		int i = 0;
		int nMyItemNum = m_nMyItemNum;
		int nOpItemNum = m_nOpItemNum;

		for( i = 0 ; i < nMyItemNum ; ++i )
			DelItem( 0, 1 );	
		for( i = 0 ; i < nOpItemNum ; ++i )
			DelItem( 1, 1 );	

		m_bMyLock	= FALSE;
		m_bOpLock	= FALSE;

		m_dwMoney	= 0;
		m_dwOpMoney = 0;

		m_dwGold	= 0;
        m_dwOpGold	= 0;

		m_bExchangable = TRUE;

		m_nMyItemNum		= 0;
		m_nOpItemNum		= 0;

		EXCHANGEMGR->Init();	

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}

		cDialog* pDlg2 = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_GOLD );
		if( pDlg2 )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg2 );
		}
	}
}

void CExchangeDialog::Render()
{
	cDialog::Render();

	if( !m_LockImage.IsNull() )
	{

		VECTOR2 vPos;

		if( m_bMyLock )
		{
			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pMyIconGridDlg->IsAddable( i ) ) continue;
				vPos.x = ( m_pMyIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pMyIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}

		if( m_bOpLock )
		{
			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pOpIconGridDlg->IsAddable( i ) ) continue;
				
				vPos.x = ( m_pOpIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pOpIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );


			}
		}
	}
}

BOOL CExchangeDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	if( m_bDisable ) return FALSE;

	if( icon->GetType() != WT_ITEM )
		return FALSE;

	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(315) );
		
		return FALSE;
	}

	if( (((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_PD))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2129) );
		return FALSE;
	}


	if( (((CItem*)icon)->GetItemBaseInfo()->ItemStatic == ITEM_STATIC_LUCK))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2124) );

		return FALSE;
	}

	if( ((CItem*)icon)->GetItemKind() & eSHOP_ITEM )
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pInfo )			return FALSE;
		if( pInfo->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		if( ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_EQUIP || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{

			if( pInfo->BuyPrice )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		else if( pInfo->MeleeAttackMax )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
			return FALSE;
		}
	}
	if( ITEMMGR->IsPetSummonItem( ((CItem*)icon)->GetItemIdx() ) )
	{
		const ITEMBASE* itembase = ((CItem*)icon)->GetItemBaseInfo();
		if(!PETMGR->CheckDefaultFriendship(itembase->dwDBIdx))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1275) );
			return FALSE;
		}
		if( PETMGR->IsCurPetSummonItem(itembase->dwDBIdx) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1247) );
			return FALSE;
		}
	}

	if( ((CItem*)icon)->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;

	}
	
	if( ((CItem*)icon)->GetItemKind() == eYOUNGYAK_ITEM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_GETIN_ITEM|| //副本卷	by:十里坡剑神	QQ:112582793
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_GROWITEM||	 //成长符	by:十里坡剑神	QQ:112582793
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_MARRYCHARM|| //同心符	by:十里坡剑神	QQ:112582793
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_HERB ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_CHARM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_SUNDRIES ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_GOLDITEM)
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}
	else
	{
		if( ((CItem*)icon)->GetQuickPosition() != 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}

	if( ((CItem*)icon)->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1583) );
		return FALSE;
	}

	eITEM_KINDBIT bits = ((CItem*)icon)->GetItemKind();
	if( bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(((CItem*)icon)->GetItemIdx())->ExchangeFlag == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1626) );
		return FALSE;
	}

	if( m_pMyIconGridDlg->PtInWindow( x, y ) && m_bMyLock == FALSE )
	{
		if( EXCHANGEMGR->LinkItem( (CItem*)icon ) )
			SetDisable( TRUE );
	}
	return FALSE;
}

void CExchangeDialog::FakeDeleteItem( POSTYPE pos )
{
	if( m_bDisable ) return; 

	MSG_WORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_DELITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.wData		= pos;

	NETWORK->Send( &msg, sizeof( MSG_WORD ) );

	SetDisable( TRUE );
}


void CExchangeDialog::AddItem( int nType, POSTYPE pos, cIcon* pItem )
{
	if( nType == 0 )
	{
		m_pMyIconGridDlg->AddIcon( pos-1, pItem );
		++m_nMyItemNum;
	}
	else
	{
		m_pOpIconGridDlg->AddIcon( pos-1, pItem );
		++m_nOpItemNum;
	}
}


void CExchangeDialog::DelItemOptionInfoAll()
{
	CExchangeItem*	pIcon = NULL;
	ITEM_INFO*		pInfo = NULL;
	for( int i = 1 ; i <= m_nOpItemNum ; ++i )
	{
		DelItemOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItemOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsOptionItem( (WORD)pIcon->GetData(), pIcon->GetDurability() ) )
	{
		ITEMMGR->RemoveItemOption( pIcon->GetDurability() );
	}
}

void CExchangeDialog::DelItemRareOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsRareOptionItem( (WORD)pIcon->GetData(), pIcon->GetRareness() ) )
	{
		ITEMMGR->RemoveItemRareOption( pIcon->GetRareness() );
	}
}

void CExchangeDialog::DelPetItemInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	DWORD itemDBIdx = pIcon->GetDBId();
	if( ITEMMGR->IsPetSummonItem( (WORD)pIcon->GetData() ) )
	{
		PETMGR->RemovePetInfo(itemDBIdx);
	}
}


void CExchangeDialog::DelPetItemInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelPetItemInfo( (POSTYPE)i );
	}
}
	
void CExchangeDialog::DelItemRareOptionInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelItemRareOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItem( int nType, POSTYPE pos )
{
	cIcon* pIcon = NULL;

	if( nType == 0 )
	{
		if( m_pMyIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;
			
			EXCHANGEMGR->UnLinkItem( 0, pIcon );

			for( int i = pos ; i < m_nMyItemNum ; ++i )
			{
				m_pMyIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				((CExchangeItem*)pIcon)->SetPosition( i );
				m_pMyIconGridDlg->AddIcon( i - 1, pIcon );
			}
			--m_nMyItemNum;
		}
	}
	else
	{

		if( m_pOpIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;

			EXCHANGEMGR->UnLinkItem( 1, pIcon );

			for( int i = pos ; i < m_nOpItemNum ; ++i )
			{
				m_pOpIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				m_pOpIconGridDlg->AddIcon( i - 1, pIcon );
			}
			--m_nOpItemNum;

		}
	}
}

MONEYTYPE CExchangeDialog::InputMoney(int nType, MONEYTYPE dwMoney)
{
	if( nType == 0 )
	{
		m_dwMoney = dwMoney;
		m_pMyMoneyEditBox->SetEditText( AddComma( dwMoney ) );
	}
	else
	{
		m_dwOpMoney = dwMoney;
		m_pOpMoneyEditBox->SetEditText( AddComma( dwMoney ) );
	}

	return dwMoney;
}

MONEYTYPE CExchangeDialog::InputGold(int nType, MONEYTYPE dwGold)
{
	if( nType == 0 )
	{
		m_dwGold = dwGold;
		m_pMyGoldEditBox->SetEditText( AddComma( dwGold ) );
	}
	else
	{
		m_dwOpGold = dwGold;
		m_pOpGoldEditBox->SetEditText( AddComma(  dwGold ) ); 
	}

	return dwGold;
}

void CExchangeDialog::OpLock( BOOL bLock )
{
	m_bOpLock = bLock;


	int i;
	for( i = 0 ; i < m_nOpItemNum ; ++i )
	{
		if( m_pOpIconGridDlg->GetIconForIdx( i ) )
			m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );

	}

	if( bLock == FALSE )
	{
		m_pExchangeBtn->SetPush( FALSE );

//#ifdef _HK_LOCAL_
//		m_pOpMoneyEditBox->SetEditText( "0" );
//		m_pOpGoldEditBox->SetEditText( "0" );  // 2014-10-29 
//#endif
		m_bMyLock = FALSE;

		for( i = 0 ; i < m_nMyItemNum ; ++i )
		{
			if( m_pMyIconGridDlg->GetIconForIdx( i ) )
				m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( FALSE );
		}
		
		m_pInputMoneyBtn->SetDisable( FALSE );

		m_pInputGoldBtn->SetDisable(FALSE); 

		m_pLockBtn->SetPush( FALSE );
	}
//#ifdef _HK_LOCAL_
//	else	//lock
//	{
//		m_pOpMoneyEditBox->SetEditText( AddComma( m_dwOpMoney ) );
//
//		m_pOpGoldEditBox->SetEditText( AddComma( m_dwOpGold ) ); // 2014-10-29 输入
//	}
//#endif
}

void CExchangeDialog::MyLock( BOOL bLock )
{
	m_bMyLock = bLock;

	int i;
	for( i = 0 ; i < m_nMyItemNum ; ++i )
	{
		if( m_pMyIconGridDlg->GetIconForIdx( i ) )
			m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
	}
	
	if( bLock )
	{
		m_pInputMoneyBtn->SetDisable( TRUE );

		m_pInputGoldBtn->SetDisable(TRUE);

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		cDialog* pDlg2 = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_GOLD );
		if( pDlg2 )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg2 );
		}
	}
	else
	{
		m_bOpLock = bLock;
		for( i = 0 ; i < m_nOpItemNum ; ++i )	
		{
			if( m_pOpIconGridDlg->GetIconForIdx( i ) )
				m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
		}

		m_pInputMoneyBtn->SetDisable( FALSE );

		m_pInputGoldBtn->SetDisable(FALSE);  
	}

	m_pLockBtn->SetPush( bLock );
}


void CExchangeDialog::OnInputMoney( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetMoney() )
	{
		pDlg->SetDisable( FALSE );
		return;
	}

	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() )
	{
		return;
	}

	DWORD dwCurMoney = HERO->GetMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTMONEY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );
}


void CExchangeDialog::OnInputCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}


void CExchangeDialog::OnInputGold( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetGold())
	{
		pDlg->SetDisable( FALSE );
		return;
	}

	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() )
	{
		return;
	}

	DWORD dwCurMoney = HERO->GetGoldMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTGOLD_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );
}


void CExchangeDialog::OnInputGoldCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}




