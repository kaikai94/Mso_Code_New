// StreetStall.cpp: implementation of the CStreetStall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StallKindSelectDlg.h"
#include "StreetStall.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cSpin.h"
#include "./Interface\cIcon.h"
#include "./Interface\cIconGridDialog.h"
#include "./Interface\cEditBox.h"
#include "./Interface\cTextArea.h"
#include "./Interface\cButton.h"
#include "StreetStallManager.h"
#include "item.h"
#include "itemmanager.h"
#include "ExchangeItem.h"
#include "StreetStallTitleTip.h"
#include "cIMEex.h"
#include "cDivideBox.h"
#include "cDivideBoxEx.h"
#include "ChatManager.h"
#include "./Input/Mouse.h"
#include "MainBarDialog.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "AppearanceManager.h"
#include "cScriptManager.h" 
#include "cChangeCharCode.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CStreetStall::CStreetStall()
{
	m_type = WT_STREETSTALLDIALOG;
	m_DlgState = eSDS_NOT_OPENED;

	memset(m_MoneyArray, 0, SLOT_STREETSTALL_NUM*sizeof(DWORD));

	memset(m_GoldArray, 0, SLOT_STREETSTALL_NUM*sizeof(DWORD));
	
	m_nCurSelectedItem = -1;
	m_dwCurSelectedItemDBIdx = 0;
	m_pData = NULL;
}

CStreetStall::~CStreetStall()
{

}

void CStreetStall::Linking()
{
	m_pStallGrid = (cIconGridDialog*)GetWindowForID(SSI_ICONGRID);
	m_ImageGrid =(cIconGridDialog*)GetWindowForID(SSI_ICONGRID_IMAGE);
	m_pImgBtn   =(cButton *)GetWindowForID(SSI_BTN_IMAGE);    
	m_pTitleEdit = (cEditBox *)GetWindowForID(SSI_TITLEEDIT);
	m_pTitleEdit->GetIME()->SetEnterAllow(FALSE);
	m_pTitleEdit->GetIME()->SetLimitLine(2);
	m_pEnterBtn	= (cButton*)GetWindowForID(SSI_ENTER);
	m_pMoneyEdit = (cEditBox *)GetWindowForID(SSI_MONEYEDIT);
	m_pMoneyEdit->SetValidCheck( VCM_NUMBER );
	m_pMoneyEdit->SetAlign( TXT_RIGHT );
	m_pMoneyEdit->SetReadOnly( TRUE );
	m_pGoldEdit = (cEditBox *)GetWindowForID(SSI_GOLDEDIT);
	m_pGoldEdit->SetValidCheck( VCM_NUMBER );
	m_pGoldEdit->SetAlign( TXT_RIGHT );
	m_pGoldEdit->SetReadOnly( TRUE );
	m_pBuyBtn = (cButton *)GetWindowForID(SSI_BUYBTN);
	m_pEditBtn = (cButton *)GetWindowForID(SSI_EDITBTN);
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );
	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pGoldEdit->SetEditText(DEFAULT_MONEY_TEXT);
	LoadFaceList();  
	m_ImageGrid->SetActive(FALSE);  
	m_pTitleEdit->SetRenderFaceImg(TRUE); 
}

void CStreetStall::LoadFaceList() 
{
	for(int i=0;i<CHARFACELIST; i++)
	{
		SCRIPTMGR->GetImage(i+FACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);

		cIcon * pIcon;

		pIcon=new cIcon;

		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+6000+i);
				
		pIcon->SetData(i+FACELISTSTARTIDX);

		pIcon->SetDisable(TRUE);

		pIcon->SetDepend(FALSE);
        
		m_ImageGrid->AddIcon(i,pIcon);
	}

	m_ImageGrid->SetActive(FALSE);

}
void CStreetStall::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );


	if( m_DlgState == eSDS_BUY )
	{
		m_pEnterBtn->SetDisable( TRUE );
	}
}

DWORD CStreetStall::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;


	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
	{
		we |= ActionEventWindow(mouseInfo);
		we |= ActionEventComponent(mouseInfo);
	}
	return we;	
}

void CStreetStall::OnCloseStall(BOOL bDelOption)
{
	STREETSTALLMGR->SetStallKind(eSK_NULL);

	SetDisable(FALSE);
	cDialog::SetActive(FALSE);


	m_pMoneyEdit->SetReadOnly( TRUE );
	m_pMoneyEdit->SetFocusEdit( FALSE );


	m_pGoldEdit->SetReadOnly( TRUE );
	m_pGoldEdit->SetFocusEdit( FALSE );

	WORD num = m_pStallGrid->GetCellNum();
	DeleteItemAll(bDelOption);

	m_pStallGrid->SetCurSelCellPos(-1);
	m_nCurSelectedItem = -1;
	m_dwCurSelectedItemDBIdx = 0;
	memset(m_MoneyArray, 0, SLOT_STREETSTALL_NUM*sizeof(DWORD));
	memset(m_GoldArray, 0, SLOT_STREETSTALL_NUM*sizeof(DWORD));
	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );
    m_pGoldEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pStallGrid->SetCurSelCellPos( -1 );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );

	m_DlgState = eSDS_NOT_OPENED;
	m_dwOwnnerId = HERO->GetID();

	if(m_pData)
	{
		CItem* item = (CItem*)m_pData;
		item->SetLock( FALSE );
	}
	m_pData = NULL;

	cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETSTALL );
	if( pDivideBox )
	{
		pDivideBox->SetDisable(TRUE);
		pDivideBox->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow( pDivideBox );
	}

	cDivideBoxEx * pDivideBox2 = (cDivideBoxEx *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY_GOLD );
	if( pDivideBox2 )
	{
		pDivideBox2->SetDisable(TRUE);
		pDivideBox2->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow( pDivideBox2 );
	}
}

void CStreetStall::ShowSellStall()
{
	SetActive(TRUE);

	m_DlgState = eSDS_OPENED;

	m_pBuyBtn->SetActive(FALSE);

	m_pEditBtn->SetActive(TRUE);

	m_pTitleEdit->SetReadOnly(FALSE);

	m_pEnterBtn->SetDisable(FALSE);

	if( m_pStallGrid->GetCurSelCellPos() == -1 ) 
	{
	}

	else
	{
	}

	return;
}

void CStreetStall::ShowBuyStall()
{
	SetActive(TRUE);

	m_DlgState = eSDS_BUY;
	m_pBuyBtn->SetActive(TRUE);
	m_pEditBtn->SetActive(FALSE);
	m_pTitleEdit->SetReadOnly(TRUE);
	m_pEnterBtn->SetDisable( TRUE );
	ResetDlgData();

	return;
}

int CStreetStall::AddItem( cIcon* pItem )
{
	POSTYPE pos;
	int i=0;
	for(;i<m_pStallGrid->GetCellNum();++i)
	{
		if( !m_pStallGrid->m_pIconGridCell[i].use )
			break;
	}
	pos = i;
	if( pos == m_pStallGrid->GetCellNum() ) return -1;

	m_pStallGrid->AddIcon( pos, pItem );

	return pos;
}

cIcon* CStreetStall::GetItem( DWORD dwDBIdx )
{
	CExchangeItem* pItem;
	for(int i=0;i<m_pStallGrid->GetCellNum();++i)
	{
		pItem = ( CExchangeItem* )m_pStallGrid->m_pIconGridCell[i].icon;
		if(pItem == NULL)
			return NULL; 
		if( pItem->GetDBId() == dwDBIdx )
			return pItem;
	}

	return NULL;
}

void CStreetStall::DeleteItem( ITEMBASE* pbase )
{
	CExchangeItem* pItem;
	POSTYPE pos;

	int i=0;
	for(;i<m_pStallGrid->GetCellNum();++i)
	{
		pItem = (CExchangeItem*)m_pStallGrid->m_pIconGridCell[i].icon;

		if( pItem == NULL ) 
		{
			continue;
		}

		if( pbase->dwDBIdx == pItem->GetDBId() )
			break;
	}
	
	pos = i;
	
	if( pos == m_pStallGrid->GetCellNum() ) return;

	cIcon* pIcon = NULL;
	if( m_pStallGrid->DeleteIcon( pos, &pIcon ) )
	{
		if( m_pData && pIcon )
		{
			if( ((CItem*)m_pData)->GetItemBaseInfo()->dwDBIdx == ((CExchangeItem*)pIcon)->GetDBId() )
			{
				m_pData = NULL;
				m_dwCurSelectedItemDBIdx = 0;
			}
		}

		if( m_DlgState == eSDS_BUY )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CExchangeItem*)pIcon );
		}

		m_pStallGrid->DeleteIcon( pos, &pIcon );

		
		for( int i = pos ; i < SLOT_STREETSTALL_NUM-1 ; ++i )
		{
			m_pStallGrid->DeleteIcon( i+1, &pIcon );
			if( !pIcon ) break;

			((CExchangeItem*)pIcon)->SetPosition( i );


			if( ((CExchangeItem*)pIcon)->GetLinkItem() )
				((CExchangeItem*)pIcon)->GetLinkItem()->SetLinkPosition( i );

			m_pStallGrid->AddIcon( i, pIcon );
			RegistMoney(i, m_MoneyArray[i + 1]);
			RegistGold(i, m_GoldArray[i + 1]);
			//m_MoneyArray[i] = m_MoneyArray[i+1];
           // m_GoldArray[i] = m_GoldArray[i+1];
		}
		RegistMoney(i, 0);
		RegistGold(i, 0);
		//m_MoneyArray[i] = 0;
		//m_GoldArray[i] = 0;
	}

	ResetDlgData();
}


CExchangeItem* CStreetStall::FindItem( DWORD dwDBIdx )
{
	CExchangeItem* pItem = NULL;
	CExchangeItem* pFindItem = NULL;


	for( int i = 0 ; i < m_pStallGrid->GetCellNum() ; ++i )
	{
		pItem = (CExchangeItem*)m_pStallGrid->m_pIconGridCell[i].icon;

		if( pItem == NULL )
			break;

		if( dwDBIdx == pItem->GetDBId() )
		{
			pFindItem = pItem;
			break;
		}
	}
	
	return pFindItem;	
}

void CStreetStall::ResetItemInfo( DWORD dwDBIdx, DWORD count )
{
	CExchangeItem* pItem = NULL;
	POSTYPE pos;
	int i=0;
	for(;i<m_pStallGrid->GetCellNum();++i)
	{
		pItem = (CExchangeItem*)m_pStallGrid->m_pIconGridCell[i].icon;
		
		if( pItem == NULL ) 
		{
			ASSERTMSG(0, "서버에 있는 정보가 클라이언트에는 없음..");
			continue;
		}

		if( dwDBIdx == pItem->GetDBId() )
			break;
	}


	pos = i;
	if( pos == m_pStallGrid->GetCellNum() ) return;

	pItem->SetDurability( count );
}


void CStreetStall::DeleteItemAll(BOOL bDelOption)
{
	cIcon* pIcon;

	for( int i=0;i<SLOT_STREETSTALL_NUM;++i)
	{
		m_pStallGrid->DeleteIcon( i, &pIcon );
		
		if( bDelOption && pIcon )
		{
			CItem* pItem = ((CExchangeItem*)pIcon)->GetLinkItem();
			if( pItem )
			{
				if( !( pItem->GetItemInfo()->ItemKind & eSHOP_ITEM ) )				
				if( !ITEMMGR->IsDupItem( (WORD)((CExchangeItem*)pIcon)->GetData() ) && ((CExchangeItem*)pIcon)->GetDurability() != 0 )
					ITEMMGR->RemoveItemOption( ((CExchangeItem*)pIcon)->GetDurability() );			
				else if( ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness() ) )
					ITEMMGR->RemoveItemRareOption(pItem->GetRareness());

				if( ITEMMGR->IsPetSummonItem(pItem->GetItemIdx()) )
					PETMGR->RemovePetInfo(pItem->GetDBIdx());
				
			}
		}
		if(pIcon)
			STREETSTALLMGR->ReleaselItemIndex(pIcon->GetID());
		if( m_DlgState == eSDS_BUY )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CExchangeItem*)pIcon );
		}
	}
}


BOOL CStreetStall::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	if( m_bDisable == TRUE || m_pData ) return FALSE;
	if( GetDlgState() != eSDS_OPENED ) return FALSE;

	if( icon->GetType() != WT_ITEM )
		return FALSE;
	
	if( ((CItem*)icon)->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;
	}
	
	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(359) );

		return FALSE;
	}

	eITEM_KINDBIT bits = ((CItem*)icon)->GetItemKind();
	if( bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(((CItem*)icon)->GetItemIdx())->ExchangeFlag == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1673) );
		return FALSE;
	}

	if( bits == eTITAN_ITEM_PAPER )
	{
		BOOL bFlag = TITANMGR->IsRegistedTitan(((CItem*)icon)->GetDBIdx());
		if( bFlag )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1674) );
			return FALSE;
		}
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
		if( ((CItem*)icon)->GetItemInfo()->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		if( ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_EQUIP || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{
			if( ((CItem*)icon)->GetItemInfo()->BuyPrice )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}	 
		else if( ((CItem*)icon)->GetItemInfo()->MeleeAttackMax )
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
	if( ((CItem*)icon)->GetItemKind() == eYOUNGYAK_ITEM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_HERB ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_CHARM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_SUNDRIES
		||((CItem*)icon)->GetItemKind() == eSHOP_ITEM_AUTOADDPOINT 
		)
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(348) );
			return FALSE;
		}
	}
	else
	{
		if( ((CItem*)icon)->GetQuickPosition() != 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(348) );
			return FALSE;
		}
	}

	if( icon->GetIconType() == eIconType_LinkedItem ) return FALSE;
	if( !m_pStallGrid->PtInWindow( x, y ) ) return FALSE;

	CItem* item = (CItem*)icon;
	if( item->GetLinkPosition() ) return FALSE; 
	item->SetLock( TRUE );

	m_pData = item;

	if( STREETSTALLMGR->GetLinkedItemNum() >= SLOT_STREETSTALL_NUM )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
		item->SetLock( FALSE );
		m_pData = NULL;
	}
	else
	{
		STREETSTALLMGR->RegistItemEx( item, 0, 0 );
		m_dwCurSelectedItemDBIdx = item->GetDBIdx();
		SetDisable( TRUE );
	}


	return FALSE;
}

void CStreetStall::FakeDeleteItem( POSTYPE pos )
{
	CExchangeItem* pExItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx( pos );
	if( pExItem == NULL ) return;
	if( pExItem->GetLinkItem() == NULL ) return;

	MSG_LINKITEM msg;
	msg.Category	= MP_STREETSTALL;
	msg.Protocol	= MP_STREETSTALL_DELETEITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.ItemInfo	= *pExItem->GetLinkItem()->GetItemBaseInfo();
	msg.wAbsPosition = pos;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );
}

DWORD CStreetStall::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;
	we = cDialog::ActionEventWindow( mouseInfo );

	if( mouseInfo->LButtonDown() )
	if( !PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
	{
		m_pStallGrid->SetShowGrid( FALSE );
	}

	return we;
}

void CStreetStall::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we == WE_LBTNCLICK || we == WE_LBTNDBLCLICK )
	{
		if(lId== SSI_ICONGRID_IMAGE)
		{
			int Pos=m_ImageGrid->GetCurSelCellPos();

			if(Pos!=0)
			{
				char ImgCode[10];

				sprintf(ImgCode,"{%02d",Pos);

				m_pTitleEdit->SetEditText(strcat(m_pTitleEdit->GetEditText(),ImgCode));
			}

			return;
		}

		POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
		CExchangeItem * pItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx( pos );

		if( !pItem ) 
		{
			m_nCurSelectedItem = -1;
			m_dwCurSelectedItemDBIdx = 0;
			return;
		}
		else
		{
			m_nCurSelectedItem = pos;
			m_dwCurSelectedItemDBIdx = GetCurSelectedItemDBidx();

			if( m_DlgState == eSDS_BUY )
			{
				if( !pItem->IsLocked() && we == WE_LBTNDBLCLICK )
				{
					if( ITEMMGR->IsDupItem( (WORD)pItem->GetData() ) )
					{
						if( pItem->GetDurability() == 1)
							STREETSTALLMGR->BuyItem();
						else
							ShowDivideBox();
					}
					else
					{
						STREETSTALLMGR->BuyItem();
					}
				}
			}

			m_pStallGrid->SetShowGrid( TRUE );
		}
	}
	else if( we & WE_BTNCLICK && lId == SSI_ENTER )
	{
		STREETSTALLMGR->EditTitle();
	}
	else if( we & WE_BTNCLICK && lId == SSI_CLOSEBTN )
	{
		SetActive( FALSE );	
	}
	else if( we & WE_BTNCLICK && lId == SSI_BTN_IMAGE ) 
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
}

void CStreetStall::SetActive( BOOL val )
{
	if( !m_bDisable )
	if( m_bActive != val )
	{
		if(!val) STREETSTALLMGR->CloseStreetStall();

		cDialog::SetActiveRecursive( val );
	}
}

void CStreetStall::RegistMoney()
{
	m_MoneyArray[m_nCurSelectedItem] = (DWORD)atoi( RemoveComma( m_pMoneyEdit->GetEditText() ));
}

void CStreetStall::RegistMoney( POSTYPE pos, DWORD dwMoney )

{
	m_MoneyArray[pos] = dwMoney;
}
void CStreetStall::RegistGold()
{
	m_GoldArray[m_nCurSelectedItem] = (DWORD)atoi( RemoveComma(  m_pGoldEdit->GetEditText() ));
}
void CStreetStall::RegistGold( POSTYPE pos, DWORD dwGold )
{
	m_GoldArray[pos] = dwGold;
}

void CStreetStall::RegistTitle( char* title, BOOL bSucess )
{
	if( bSucess )
	{
		HERO->ShowStreetStallTitle( TRUE, title );
		strcpy( m_OldTitle, title );
	}
	else
	{
		m_pTitleEdit->SetEditText( m_OldTitle );
	}
}

void CStreetStall::OnMoneyEditClick()
{
}


void CStreetStall::OnGoldEditClick()
{
}

void CStreetStall::OnTitleEditClick()
{
}


void CStreetStall::ChangeItemStatus( POSTYPE pos, DWORD money,DWORD gold, BOOL bLock )
{
	CExchangeItem * pItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx( pos );
	if( !pItem ) return;
	char buf[128] = {0,};
	char buf2[128] = {0,};

	char buf3[128] = {0,};
	char buf4[128] = {0,};

	m_MoneyArray[pos] = money;

	m_GoldArray[pos] = gold; 

	if( m_DlgState == eSDS_BUY )
	{
		pItem->SetLock( !bLock );
		pItem->SetMovable( FALSE );
	}
	else
	{
		pItem->SetLock( bLock );
		pItem->SetMovable( !bLock );
	}

	if( ITEMMGR->IsPetSummonItem( (WORD)pItem->GetData() ) )
	{
		ITEMMGR->SetToolTipIcon( pItem, NULL, NULL, pItem->GetDBId() );
	}
	else if( (pItem->GetDurability() != 0 || pItem->GetRareness())
		&& !ITEMMGR->IsDupItem((WORD)pItem->GetData()) )
	{
		ITEMMGR->SetToolTipIcon( pItem, ITEMMGR->GetItemOption(pItem->GetDurability()),
		ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
	}
	else
		ITEMMGR->SetToolTipIcon( pItem );

	if( bLock )
	{
		if( ITEMMGR->IsDupItem((WORD)pItem->GetData()) )
		{
			wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( money ) );
			wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( money * pItem->GetDurability() ));
			wsprintf( buf3, CHATMGR->GetChatMsg(2102), AddComma( gold ) );
			wsprintf( buf4, CHATMGR->GetChatMsg(2103), AddComma( gold * pItem->GetDurability() ));
		}
		else
		{
			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( money ) );
			wsprintf( buf3, CHATMGR->GetChatMsg(2104), AddComma( gold ) );
		}
	}
	else
	{
		wsprintf( buf, CHATMGR->GetChatMsg(114) );
	}
	pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
	pItem->AddToolTipLine( buf3, TTTC_BUYPRICE );

	if(buf2[0])
		pItem->AddToolTipLine( buf2, TTTC_BUYPRICE ); 

	if(buf4[0])
		pItem->AddToolTipLine( buf4, TTTC_BUYPRICE );
	ResetDlgData();
}

void CStreetStall::ChangeItemStatus( ITEMBASE* pbase, DWORD money, DWORD gold, BOOL bLock, int nDivideKind )
{
	CExchangeItem* pItem = NULL;
	POSTYPE pos;
	char buf[128] = {0,};
	char buf2[128] = {0,};


	char buf3[128] = {0,};
	char buf4[128] = {0,};
	int i=0;
	for(;i<m_pStallGrid->GetCellNum();++i)
	{
		if( m_pStallGrid->m_pIconGridCell[i].use )
		{
			pItem = (CExchangeItem*)m_pStallGrid->m_pIconGridCell[i].icon;
			if( pItem->GetDBId() == pbase->dwDBIdx )
				break;
		}
	}

	pos = i;
	if( pos == m_pStallGrid->GetCellNum() ) return;

	m_nCurSelectedItem = pos;
	m_pStallGrid->SetCurSelCellPos( pos );

	m_MoneyArray[pos] = money;

	m_GoldArray[pos] = gold ;

	if( m_DlgState == eSDS_BUY )
	{
		pItem->SetLock( !bLock );
		pItem->SetMovable( FALSE );
	}
	else
	{
		if( !bLock )
		{
			m_pData = pItem->GetLinkItem();
			if( ITEMMGR->IsDupItem( pItem->GetLinkItem()->GetItemIdx() ) )
				ShowDivideBox( eSO_INPUTMONEY_DUP, (int)(pItem->GetAbsX() + 20), (int)(pItem->GetAbsY() + 20), nDivideKind );
			else
				ShowDivideBox( eSO_INPUTMONEY, (int)(pItem->GetAbsX() + 20), (int)(pItem->GetAbsY() + 20), nDivideKind );
		
			pItem->SetLock( TRUE );
			pItem->SetMovable( FALSE );
		}
		else
		{
			pItem->SetLock( FALSE );
			pItem->SetMovable( FALSE );
			m_pData = NULL;
		}
	}
	if( ITEMMGR->IsPetSummonItem( (WORD)pItem->GetData() ) )
	{
		ITEMMGR->SetToolTipIcon( pItem, NULL, NULL, pItem->GetDBId() );
	}
	else if( !ITEMMGR->IsDupItem((WORD)pItem->GetData()) && pItem->GetDurability() != 0 )
		ITEMMGR->SetToolTipIcon( pItem, ITEMMGR->GetItemOption(pItem->GetDurability()), ITEMMGR->GetItemRareOption(pItem->GetRareness()),0 );
	else
		ITEMMGR->SetToolTipIcon( pItem, NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);

	if( bLock )
	{
		if( ITEMMGR->IsDupItem((WORD)pItem->GetData()) )
		{
			wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( money ) );
			wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( money * pItem->GetDurability() ));
			wsprintf( buf3, CHATMGR->GetChatMsg(2102), AddComma( gold ) );
			wsprintf( buf4, CHATMGR->GetChatMsg(2103), AddComma( gold * pItem->GetDurability() ));
		}
		else
		{
			wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( money ) );
		    wsprintf( buf3, CHATMGR->GetChatMsg(2104), AddComma( gold ) );
		}
	
	}
	else
	{
		wsprintf( buf, CHATMGR->GetChatMsg(114) );
	}
	pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
	pItem->AddToolTipLine( buf3, TTTC_BUYPRICE );
	
	if(buf2[0])
		pItem->AddToolTipLine( buf2, TTTC_BUYPRICE );

	if(buf4[0])
		pItem->AddToolTipLine( buf4, TTTC_BUYPRICE );
	ResetDlgData();
}

BOOL CStreetStall::SelectedItemCheck()
{

	m_pStallGrid->SetShowGrid( TRUE );

	CExchangeItem * pLinkItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );

	return !pLinkItem->IsLocked();
}

void CStreetStall::ResetDlgData()
{
	m_pStallGrid->SetCurSelCellPos( m_nCurSelectedItem );
	m_pStallGrid->SetShowGrid( TRUE );
	m_TotalMoney = 0;
	m_TotalGold  = 0;
	
	for(int i=0;i<m_pStallGrid->GetCellNum();++i)
	{
		CExchangeItem* pItem = (CExchangeItem*)m_pStallGrid->m_pIconGridCell[i].icon;
		DWORD Durability;
		
		if(pItem == NULL)
			continue;

		if( ITEMMGR->IsDupItem((WORD)pItem->GetData()) )
			Durability = pItem->GetDurability();
		else
			Durability = 1;
		
		if(Durability == 0)
			Durability = 1;
		
		m_TotalMoney += (m_MoneyArray[i] * Durability);

		m_TotalGold  += (m_GoldArray[i] * Durability); 
	}
	
	m_pMoneyEdit->SetEditText( AddComma( m_TotalMoney ) ); 

	m_pGoldEdit->SetEditText( AddComma( m_TotalGold ) ); 
}

DWORD CStreetStall::GetCurSelectedItemDBidx()
{
	return ((CExchangeItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem ))->GetDBId();
}

DWORD CStreetStall::GetCurSelectedItemDur()
{
	return ((CExchangeItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem ))->GetDurability();
}


void CStreetStall::ShowDivideBox(DWORD dwOption, int x, int y, int nDivideKind )
{
	switch( dwOption )
	{
	case eSO_DIVE:
		{
			CExchangeItem* pExItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );

			if( pExItem == NULL ) return;

			cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_STREETSTALL, (LONG)pExItem->GetAbsX(), (LONG)pExItem->GetAbsY(), OnDivideItem, OnDivideItemCancel, this, NULL, CHATMGR->GetChatMsg(638) );
			if( !pDivideBox )
			{
				pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETSTALL );
			}
			pDivideBox->ChangeKind( nDivideKind );

			pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
			pDivideBox->SetValue(1);
			m_pData = pExItem;
			pDivideBox->m_pSpin->SetFocusEdit(TRUE);
		}
		break;
	case eSO_INPUTMONEY:
		{   
			cDivideBoxEx * pDivideBox = WINDOWMGR->DivideBoxEx( DBOX_INPUT_MONEY_GOLD, x, y, OnFakeRegistItem, OnRegistItemCancel, this, NULL, CHATMGR->GetChatMsg(639) );
			if( !pDivideBox )
				pDivideBox = (cDivideBoxEx *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY_GOLD);

			pDivideBox->ChangeKind( nDivideKind );
			pDivideBox->m_pSpin1->SetFocusEdit( TRUE );
			pDivideBox->m_pSpin1->SetEditText( AddComma( m_MoneyArray[m_nCurSelectedItem] ) );
			pDivideBox->m_pSpin2->SetFocusEdit( FALSE );
			pDivideBox->m_pSpin2->SetEditText( AddComma( m_GoldArray[m_nCurSelectedItem] ) );
			SetDisable( TRUE );
		}
		break;
	case eSO_INPUTMONEY_DUP:
		{
			cDivideBoxEx * pDivideBox = WINDOWMGR->DivideBoxEx( DBOX_INPUT_MONEY_GOLD, x, y, OnFakeRegistItem, OnRegistItemCancel, this, NULL, CHATMGR->GetChatMsg(642) );
			if( !pDivideBox )
				pDivideBox = (cDivideBoxEx *)WINDOWMGR->GetWindowForID( DBOX_INPUT_MONEY_GOLD );
			pDivideBox->ChangeKind( nDivideKind );
			pDivideBox->m_pSpin1->SetFocusEdit( TRUE );
			pDivideBox->m_pSpin1->SetEditText( AddComma( m_MoneyArray[m_nCurSelectedItem] ) );
			pDivideBox->m_pSpin2->SetFocusEdit( FALSE );
			pDivideBox->m_pSpin2->SetEditText( AddComma( m_GoldArray[m_nCurSelectedItem] ) );
			SetDisable( TRUE );
		}
		break;
	}
}
void CStreetStall::OnDivideItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetStall* pStall = (CStreetStall*)vData1;

	if( param1 == 0 )
	{
		pStall->SetDisable(FALSE);
		return; 
	}

	cDivideBox* pDivideBox = (cDivideBox*)p;

	if( pDivideBox == NULL )
	{
		pStall->SetDisable(FALSE);
		return;
	}

	CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
	if( pExItem == NULL )
	{
		OnDivideItemCancel( iId, p, param1, vData1, vData2 );
		return;		
	}

	if( pExItem->GetDurability() < param1 )
	{
		OnDivideItemCancel( iId, p, param1, vData1, vData2 );
		
		return;
	}

	POSTYPE pos = pExItem->GetPosition();

	STREETSTALLMGR->BuyItemDur( pos, param1 );
}

void CStreetStall::OnDivideItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetStall* pStall = (CStreetStall*)vData1;
	if( pStall == NULL ) return;

	pStall->SetData( NULL );
	pStall->SetDisable( FALSE );
}

void CStreetStall::OnFakeRegistItem( LONG iId, void* p, DWORD param1,DWORD param2, void * vData1, void * vData2 )
{
	CStreetStall* pStall = (CStreetStall*)vData1;

	pStall->SetDisable( FALSE );

	if( param1 == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(637) );
		OnRegistItemCancel( iId, p, param1,param2,vData1, vData2 );
		return;
	}
	else
	{
		CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
		if( pExItem == NULL )
		{
			OnRegistItemCancel( iId, p, param1,param2, vData1, vData2 );
			return;
		}

		pStall->RegistMoney( pExItem->GetPosition(), param1 );
		pStall->RegistGold(pExItem->GetPosition(), param2);
		STREETSTALLMGR->ItemStatus( pExItem, TRUE );
	}
}

void CStreetStall::OnRegistItemCancel( LONG iId, void* p, DWORD param1,DWORD param2, void * vData1, void * vData2 )
{
	CStreetStall* pStall = (CStreetStall*)vData1;
	CItem* item = (CItem*)pStall->GetData();

	item->SetLock( FALSE );
	pStall->SetData( NULL );
	pStall->SetDisable( FALSE );

	CExchangeItem* pExItem = (CExchangeItem*)pStall->GetGridDialog()->GetIconForIdx( item->GetLinkPosition() );
	if( pExItem == NULL ) return;
	if( pExItem->GetLinkItem() == NULL ) return;
		
	MSG_LINKITEM msg;
	msg.Category	= MP_STREETSTALL;
	msg.Protocol	= MP_STREETSTALL_DELETEITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.ItemInfo	= *pExItem->GetLinkItem()->GetItemBaseInfo();
	msg.wAbsPosition = item->GetLinkPosition();
		
	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );
}

int CStreetStall::GetGridposForItemIdx(WORD wIdx)
{
	int num = m_pStallGrid->GetCellNum();

	for(int i=0;i<num;++i)
	{
		CExchangeItem* pItem = (CExchangeItem*)m_pStallGrid->GetIconForIdx(i);
		if( wIdx ==  pItem->GetDBId() )
			return i;
	}
	return -1;
}


void CStreetStall::GetTitle( char* pStrTitle )
{
	strcpy(pStrTitle, m_pTitleEdit->GetEditText());
}

void CStreetStall::EditTitle( char* title )
{
#ifdef _BIG5SYSTEM_
	if (strlen(title)>=2)
	{
		CODEMGR->ChangeCharCode(title);
	}
#endif
	m_pTitleEdit->SetEditText( title );
}

cIcon* CStreetStall::GetCurSelectedItem() 
{ 
	if(m_nCurSelectedItem == -1) return NULL; 
	return m_pStallGrid->GetIconForIdx( m_nCurSelectedItem ); 
}


DWORD CStreetStall::GetCurSelectedItemIdx() 
{ 
	if(m_nCurSelectedItem == -1) return 0; 
	return m_pStallGrid->GetIconForIdx(m_nCurSelectedItem)->GetData(); 
}


cIcon* CStreetStall::GetItem( POSTYPE pos )
{
	return m_pStallGrid->GetIconForIdx( pos );
}




