// StreetBuyStall.cpp: implementation of the CStreetBuyStall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BuyRegDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cSpin.h"
#include "./Interface\cIcon.h"
#include "./Interface\cIconGridDialog.h"
#include "./Interface\cEditBox.h"
#include "./Interface\cTextArea.h"
#include "./Interface\cButton.h"
#include "InventoryExDialog.h"

#include "StreetStallManager.h"

#include "item.h"
#include "itemmanager.h"
#include "BuyItem.h"
#include "StreetStallTitleTip.h"
#include "cIMEex.h"
#include "cDivideBox.h"
#include "ChatManager.h"
#include "./Input/Mouse.h"

#include "MainBarDialog.h"
#include "QuickManager.h"
#include "AppearanceManager.h"
#include "cScriptManager.h" 
#include "cChangeCharCode.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStreetBuyStall::CStreetBuyStall()
{
	m_type = WT_STREETBUYSTALLDIALOG;
	m_DlgState = eBSDS_NOT_OPENED;

	memset(m_MoneyArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));
	memset(&m_FakeRegInfo, 0, sizeof(BUY_REG_INFO));

	m_nCurSelectedItem = -1;
	m_pData = NULL;
	m_pSellItem = NULL;
}

CStreetBuyStall::~CStreetBuyStall()
{

}

void CStreetBuyStall::Linking()
{
	m_pStallGrid = (cIconGridDialog*)GetWindowForID(BS_ICONGRID);

	m_ImageGrid =(cIconGridDialog*)GetWindowForID(BS_ICONGRID_IMAGE);

	m_pImgBtn   =(cButton *)GetWindowForID(BS_BTN_IMAGE);     

	m_pTitleEdit = (cEditBox *)GetWindowForID(BS_TITLEEDIT);
	m_pTitleEdit->GetIME()->SetEnterAllow(FALSE);
	m_pTitleEdit->GetIME()->SetLimitLine(2);

	m_pEnterBtn	= (cButton*)GetWindowForID(BS_ENTER);
	m_pMoneyEdit = (cEditBox *)GetWindowForID(BS_MONEYEDIT);
	m_pMoneyEdit->SetValidCheck( VCM_NUMBER );
	m_pMoneyEdit->SetAlign( TXT_RIGHT );
	m_pMoneyEdit->SetReadOnly( TRUE );
	m_pRegistBtn = (cButton *)GetWindowForID(BS_REGBTN);
	m_pSellBtn = (cButton *)GetWindowForID(BS_SELLBTN);
	
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );
	
	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );

	LoadFaceList();  

	m_ImageGrid->SetActive(FALSE); 

	m_pTitleEdit->SetRenderFaceImg(TRUE); 
}

DWORD CStreetBuyStall::ActionEvent(CMouse * mouseInfo)
{
	if( !m_bActive ) return WE_NULL;

	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable )
	{
		we |= ActionEventWindow(mouseInfo);

		WORD pos;

		m_pStallGrid->GetPositionForXYRef(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY(), pos);
		we |= ActionEventComponent(mouseInfo);
	}
	return we;	

}

void CStreetBuyStall::OnCloseStall(BOOL bDelOption)
{
	STREETSTALLMGR->SetStallKind(eSK_NULL);

	SetDisable(FALSE);
	cDialog::SetActive(FALSE);

	m_pMoneyEdit->SetReadOnly( TRUE );
	m_pMoneyEdit->SetFocusEdit( FALSE );

	WORD num = m_pStallGrid->GetCellNum();
	DeleteItemAll(bDelOption);

	m_pStallGrid->SetCurSelCellPos(-1);
	m_nCurSelectedItem = -1;
	memset(m_MoneyArray, 0, SLOT_STREETBUYSTALL_NUM*sizeof(DWORD));

	m_pMoneyEdit->SetEditText( DEFAULT_MONEY_TEXT );
	m_pTitleEdit->SetEditText( DEFAULT_TITLE_TEXT );

	m_pStallGrid->SetShowGrid( FALSE );
	strcpy( m_OldTitle, m_pTitleEdit->GetEditText() );

	m_DlgState = eBSDS_NOT_OPENED;

	m_dwOwnnerId = HERO->GetID();

	if(m_pData)
	{
		CItem* item = (CItem*)m_pData;
		item->SetLock( FALSE );
	}
	m_pData = NULL;

	cDivideBox * pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETBUYSTALL );
	if( pDivideBox )
	{
		pDivideBox->SetDisable(TRUE);
		pDivideBox->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow( pDivideBox );
	}
}

void CStreetBuyStall::ShowBuyStall()
{
	SetActive(TRUE);

	m_DlgState = eBSDS_OPENED;

	m_pTitleEdit->SetReadOnly(FALSE);

	m_pEnterBtn->SetDisable(FALSE);
	m_pRegistBtn->SetDisable(FALSE);
	m_pRegistBtn->SetActive(TRUE);
	m_pSellBtn->SetActive(FALSE);
	return;
}

void CStreetBuyStall::SetDisable(BOOL val)
{
	cDialog::SetDisable(val);

	if(m_DlgState == eBSDS_SELL)
	{
		m_pEnterBtn->SetDisable( TRUE );
		
		m_pRegistBtn->SetActive(FALSE);
	}
	else if(m_DlgState == eBSDS_OPENED)

	{
		m_pSellBtn->SetActive(FALSE);
	}
}

void CStreetBuyStall::ShowSellStall()
{
	SetActive(TRUE);
	m_DlgState = eBSDS_SELL;
	m_pTitleEdit->SetReadOnly(TRUE);
	m_pEnterBtn->SetDisable( TRUE );
	m_pRegistBtn->SetActive(FALSE);
	m_pSellBtn->SetActive(TRUE);

	for(int i = 0; i < SLOT_STREETBUYSTALL_NUM; i++)
	{
		cIcon* pIcon = m_pStallGrid->GetIconForIdx( i );

		if( !pIcon )
			continue;

		if( pIcon->IsLocked() )
			pIcon->SetActive( FALSE );
	}

	ResetDlgData();
	return;
}

BOOL CStreetBuyStall::AddItem( POSTYPE pos, cIcon* pItem )
{
	if( pos == m_pStallGrid->GetCellNum() ) return FALSE;

	BUY_REG_INFO temp;
	DWORD money = m_MoneyArray[pos];
	memcpy( &temp, &m_RegInfoArray[pos], sizeof(BUY_REG_INFO) );
	
	DeleteItem(pos);

	m_MoneyArray[pos] = money;
	memcpy( &m_RegInfoArray[pos], &temp, sizeof(BUY_REG_INFO) );

	return m_pStallGrid->AddIcon( pos, pItem );
}

void CStreetBuyStall::DeleteItem( POSTYPE pos )
{
	if( pos == m_pStallGrid->GetCellNum() ) return;

	cIcon* pIcon = NULL;
	if( m_pStallGrid->DeleteIcon( pos, &pIcon ) )
	{
		if( m_DlgState == eBSDS_SELL )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CBuyItem*)pIcon );
		}

		m_pStallGrid->DeleteIcon( pos, &pIcon );
	
		m_MoneyArray[pos] = 0;
		memset( &m_RegInfoArray[pos], 0, sizeof(BUY_REG_INFO) );
	}

	ResetDlgData();
}

void CStreetBuyStall::ResetItemInfo( POSTYPE pos, WORD volume )
{
	CBuyItem* pItem = (CBuyItem*)m_pStallGrid->m_pIconGridCell[pos].icon;

	if( !pItem )	return;
	if( pos == m_pStallGrid->GetCellNum() ) return;
	
	pItem->SetVolume( volume );
}

void CStreetBuyStall::DeleteItemAll(BOOL bDelOption)
{
	cIcon* pIcon;

	for( int i=0;i<SLOT_STREETBUYSTALL_NUM;++i)
	{
		m_pStallGrid->DeleteIcon( i, &pIcon );
		
		if( m_DlgState == eBSDS_SELL )
		{
			if( pIcon ) WINDOWMGR->AddListDestroyWindow( pIcon );
		}
		else
		{
			if( pIcon ) STREETSTALLMGR->UnlinkItem( (CBuyItem*)pIcon );
		}
	}
}

void CStreetBuyStall::FakeRegistItem( BUY_REG_INFO RegInfo, ITEM_INFO* pItemInfo)
{
	ITEMBASE Item;

	memset(&Item, 0, sizeof(ITEMBASE));

	Item.wIconIdx = pItemInfo->ItemIdx;
	
	if(m_nCurSelectedItem == -1)
		m_nCurSelectedItem = 0;

	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(Item.wIconIdx);
	
	if(!pInfo)
		return;
	if( pInfo->ItemKind <= eSHOP_ITEM_EQUIP )
	{
		Item.ItemParam = ITEM_PARAM_SEAL;
	}
	
	memcpy( &m_FakeRegInfo, &RegInfo, sizeof(BUY_REG_INFO) );
	STREETSTALLMGR->RegistItemEx(Item, m_nCurSelectedItem, RegInfo.Volume, RegInfo.Money);
	SetDisable( TRUE );

}

BOOL CStreetBuyStall::SellError(WORD errCode)

{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(errCode) );
	
	return FALSE;
}

BOOL CStreetBuyStall::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return FALSE;
}

BOOL CStreetBuyStall::ShowDivideBox(WORD Volume)
{
	CBuyItem* pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );
	
	if( pBuyItem == NULL ) return FALSE;
	
	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_STREETBUYSTALL, (LONG)pBuyItem->GetAbsX(), (LONG)pBuyItem->GetAbsY(), OnDivideItem, OnDivideItemCancel, this, NULL, CHATMGR->GetChatMsg(186) );
	if( !pDivideBox )
	{
		pDivideBox = (cDivideBox *)WINDOWMGR->GetWindowForID( DBOX_DIVIDE_STREETBUYSTALL );
	}
	pDivideBox->ChangeKind( 0 );
	
	pDivideBox->SetMaxValue( Volume );

	pDivideBox->SetValue(0);	
	
	m_pData = pBuyItem;
	
	pDivideBox->m_pSpin->SetFocusEdit(TRUE);

	return TRUE;
}

void CStreetBuyStall::OnDivideItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetBuyStall* pStall = (CStreetBuyStall*)vData1;
	CBuyItem* pBuyItem = (CBuyItem*)pStall->GetCurSelectedItem();
	
	if(!pStall || !pStall->IsActive() || !pBuyItem || !pBuyItem->IsActive() || param1 == 0)
	{
		pStall->SellError();
				
		OnDivideItemCancel(iId, p, param1, vData1, vData2);
		return; 
	}
	
	cDivideBox* pDivideBox = (cDivideBox*)p;

	if( pDivideBox == NULL )
	{
		pStall->SetDisable(FALSE);
		return;
	}

	if( !STREETSTALLMGR->SellItemDur( param1 ) )
	{		
		pStall->SellError();
		
		CItem* pItem = pStall->GetSellItem();
		if(pItem)	pItem->SetLock(FALSE);
	}
}

void CStreetBuyStall::OnDivideItemCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CStreetBuyStall* pStall = (CStreetBuyStall*)vData1;
	if( pStall == NULL ) return;

	pStall->SetData( NULL );
	pStall->SetDisable( FALSE );

	CItem* pItem = pStall->GetSellItem();
	if(pItem)	pItem->SetLock(FALSE);
}

void CStreetBuyStall::FakeDeleteItem( POSTYPE pos )
{
	CBuyItem* pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );

	if( pBuyItem == NULL ) return;

	MSG_LINKITEM msg;
	msg.Category	= MP_STREETSTALL;
	msg.Protocol	= MP_STREETSTALL_DELETEITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	

	memset(&msg.ItemInfo, 0, sizeof(msg.ItemInfo));

	msg.ItemInfo.wIconIdx = pBuyItem->GetItemIdx();
	msg.wAbsPosition = pos;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );
}

DWORD CStreetBuyStall::ActionEventWindow(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;
	we = cDialog::ActionEventWindow( mouseInfo );

	if( mouseInfo->LButtonDown() )
	if( !PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )

	{
		m_pStallGrid->SetShowGrid( FALSE );
	}
	else
	{
	}

	return we;
}

void CStreetBuyStall::OnRegBtn()
{
	if(m_DlgState == eBSDS_SELL)
		return;

	POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
	
	if(pos < 0 || pos >= SLOT_STREETBUYSTALL_NUM)
		pos = 0;
	
	m_nCurSelectedItem = pos;
	
	CBuyItem * pItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );
	
	if( !pItem )
	{
		GAMEIN->GetBuyRegDialog()->Show();
		return;
	}
	else
	{
		MSG_WORD msg;
		msg.Category = MP_STREETSTALL;
		msg.dwObjectID = HEROID;

		msg.Protocol = MP_STREETSTALL_UPDATE_SYN;
		msg.wData = pos;
		
		NETWORK->Send( &msg, sizeof(MSG_WORD) );
		
		return;
	}
}

BOOL CStreetBuyStall::OnSellBtn()
{
	if(m_DlgState == eBSDS_OPENED)
		return FALSE;
	
	POSTYPE pos = (POSTYPE)m_pStallGrid->GetCurSelCellPos();
	
	if(pos < 0 || pos >= SLOT_STREETBUYSTALL_NUM)
		return SellError(eSE_NOT_SELECT);

	m_nCurSelectedItem = pos;
	
	CBuyItem * pBuyItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );

	if(( !pBuyItem ) || ( !pBuyItem->IsActive() ))
		return SellError(eSE_NOT_EXIST);	

	CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( pBuyItem->GetItemIdx() );
	
	if( !pItem )
		return SellError(eSE_NOT_HAVE);

	if( pItem->GetItemKind() & eSHOP_ITEM )
	{
		if( pItem->GetItemInfo()->ItemType == 11 )
		{
			if( !(pItem->GetItemParam() & ITEM_PARAM_SEAL) )
			{
				return SellError(eSE_NOT_ALLOW);
			}
		}
		if( pItem->GetItemKind() == eSHOP_ITEM_EQUIP || pItem->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{
			if( pItem->GetItemInfo()->BuyPrice )
				return SellError(eSE_NOT_ALLOW);
		}
		else if( pItem->GetItemInfo()->MeleeAttackMax )
			return SellError(eSE_NOT_ALLOW);
	}
	else
	{
		if( ITEMMGR->IsRareOptionItem(pItem->GetItemIdx(), pItem->GetRareness()) )
			return SellError(eSE_RARE);

		if( ITEMMGR->IsOptionItem(pItem->GetItemIdx(), pItem->GetDurability()) )
			return SellError(eSE_OPTION);
	}

	if( pItem->GetIconType() == eIconType_LinkedItem ) return SellError();
	if( pItem->GetLinkPosition() ) return SellError();

	if( pItem->GetItemKind() == eYOUNGYAK_ITEM ||
		pItem->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		pItem->GetItemKind() == eSHOP_ITEM_HERB ||
		pItem->GetItemKind() == eSHOP_ITEM_CHARM ||
		pItem->GetItemKind() == eSHOP_ITEM_SUNDRIES
		||pItem->GetItemKind() == eSHOP_ITEM_AUTOADDPOINT 
		)
	{
		if( QUICKMGR->CheckQPosForItemIdx(pItem->GetItemIdx()) == FALSE )
		{
			return SellError(eSE_QUICK);
		}
	}
	else
	{
		if( pItem->GetQuickPosition() != 0 )
		{
			return SellError(eSE_QUICK);
		}
	}

	if( pItem->IsLocked() ) return SellError();

	pItem->SetLock( TRUE );
	
	m_pSellItem = pItem;
	
	if( ITEMMGR->IsDupItem( pItem->GetItemIdx() ) )
	{
		if( pItem->GetDurability() == 1)
			pItem->SetLock( STREETSTALLMGR->SellItem() );
		else
			pItem->SetLock( ShowDivideBox( (WORD)pItem->GetDurability() ) );
	}
	else
	{
		pItem->SetLock( STREETSTALLMGR->SellItem() );
	}

	return TRUE;
}

void CStreetBuyStall::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we == WE_LBTNCLICK )
	{
		if(lId== BS_ICONGRID_IMAGE)
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
		
		m_nCurSelectedItem = pos;
	}
	else if( ( ( we == WE_LBTNDBLCLICK ) && (m_DlgState == eBSDS_OPENED) )
			|| ( we & WE_BTNCLICK && lId == BS_REGBTN ) )
		OnRegBtn();

	
	else if( ( ( we == WE_LBTNDBLCLICK ) && (m_DlgState == eBSDS_SELL) )
			|| ( we & WE_BTNCLICK && lId == BS_SELLBTN ) )
		OnSellBtn();
	
	else if( we & WE_BTNCLICK && lId == BS_ENTER )
		STREETSTALLMGR->EditTitle();
	
	else if( we & WE_BTNCLICK && lId == BS_CLOSEBTN )
	{
		GAMEIN->GetBuyRegDialog()->Close();
		SetActive( FALSE );	
	}
	else if( we & WE_BTNCLICK && lId == BS_BTN_IMAGE ) 
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

void CStreetBuyStall::SetActive( BOOL val )
{
	if( !m_bDisable )
	if( m_bActive != val )
	{
		if(!val) STREETSTALLMGR->CloseStreetStall();

		cDialog::SetActiveRecursive( val );
	}
}

void CStreetBuyStall::RegistMoney()
{
	m_MoneyArray[m_nCurSelectedItem] = (MONEYTYPE)_atoi64(RemoveComma(m_pMoneyEdit->GetEditText()));
}

void CStreetBuyStall::RegistMoney(POSTYPE pos, MONEYTYPE dwMoney)
{
	m_MoneyArray[pos] = dwMoney;
}

void CStreetBuyStall::RegistTitle( char* title, BOOL bSucess )
{
	if( bSucess )
	{
		HERO->ShowStreetBuyStallTitle( TRUE, title );
		strcpy( m_OldTitle, title );
	}
	else
	{
		m_pTitleEdit->SetEditText( m_OldTitle );
	}
}

void CStreetBuyStall::OnMoneyEditClick()
{
}

void CStreetBuyStall::OnTitleEditClick()
{
}

void CStreetBuyStall::ChangeItemStatus(POSTYPE pos, WORD volume, MONEYTYPE money)
{
	CBuyItem * pItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( pos );
	if( !pItem ) return;
	char buf[128] = {0,};
	char buf2[128] = {0,};

	m_MoneyArray[pos] = money;

	memcpy(&m_RegInfoArray[pos], &m_FakeRegInfo, sizeof(BUY_REG_INFO));

	pItem->SetMovable( FALSE );

	if(pItem->GetVolume() > 1)
	{
		wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pItem->GetMoney() ) );
		wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pItem->GetMoney() * pItem->GetVolume() ));
	}
	else
		wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pItem->GetMoney() ) );	
	
	pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
	
	if(buf2[0])
		pItem->AddToolTipLine( buf2, TTTC_BUYPRICE );
	ResetDlgData();
}

BOOL CStreetBuyStall::SelectedItemCheck()
{
	m_pStallGrid->SetShowGrid( TRUE );	

	CBuyItem * pLinkItem = (CBuyItem*)m_pStallGrid->GetIconForIdx( m_nCurSelectedItem );

	if( !pLinkItem ) return FALSE;


	return !pLinkItem->IsLocked();
}

void CStreetBuyStall::ResetDlgData()
{
	m_pStallGrid->SetCurSelCellPos( m_nCurSelectedItem );
	m_pStallGrid->SetShowGrid( TRUE );
	
	m_TotalMoney = 0;
	
	for(int i=0;i<m_pStallGrid->GetCellNum();++i)
	{
		CBuyItem* pItem = (CBuyItem*)m_pStallGrid->m_pIconGridCell[i].icon;
		DWORD Volume;
		
		if(pItem == NULL)
			continue;
		
		if( !pItem->IsActive() )
		{
			pItem->SetActive(TRUE);
		}

		Volume = pItem->GetVolume();
		
		if(Volume == 0)
			Volume = 1;

		if(m_RegInfoArray[i].Volume != Volume)
		{
			m_RegInfoArray[i].Volume = (WORD)Volume;

			char buf[128] = {0,};
			char buf2[128] = {0,};

			ITEMMGR->SetToolTipIcon(pItem);
			
			if(pItem->GetVolume() > 1)
			{
				wsprintf( buf, CHATMGR->GetChatMsg(199), AddComma( pItem->GetMoney() ) );
				wsprintf( buf2, CHATMGR->GetChatMsg(1206), AddComma( pItem->GetMoney() * pItem->GetVolume() ));
			}
			else
				wsprintf( buf, CHATMGR->GetChatMsg(214), AddComma( pItem->GetMoney() ) );	
			
			pItem->AddToolTipLine( buf, TTTC_BUYPRICE );
			
			if(buf2[0])
				pItem->AddToolTipLine( buf2, TTTC_BUYPRICE );
		}
		m_TotalMoney += MONEYTYPE(m_MoneyArray[i] * Volume);
	}
	
	m_pMoneyEdit->SetEditText( AddComma( m_TotalMoney ) ); 
}

void CStreetBuyStall::GetTitle( char* pStrTitle )
{
	strcpy(pStrTitle, m_pTitleEdit->GetEditText());
}

void CStreetBuyStall::EditTitle( char* title )
{
#ifdef _BIG5SYSTEM_
	if (CODEMGR->GetIsSimple()&&strlen(title)>=2)
	{
		CODEMGR->ChangeCharCode(title);
	}
#endif
	m_pTitleEdit->SetEditText( title );
}

cIcon* CStreetBuyStall::GetCurSelectedItem() 
{ 
	if(m_nCurSelectedItem == -1) return NULL; 
	return m_pStallGrid->GetIconForIdx( m_nCurSelectedItem ); 
}


DWORD CStreetBuyStall::GetCurSelectedItemIdx() 
{ 
	if(m_nCurSelectedItem == -1) return 0; 
	return m_pStallGrid->GetIconForIdx(m_nCurSelectedItem)->GetData(); 
}

cIcon* CStreetBuyStall::GetItem( POSTYPE pos )
{
	return m_pStallGrid->GetIconForIdx( pos );
}
void CStreetBuyStall::LoadFaceList()
{
	for(int i=0;i<CHARFACELIST; i++)
	{
		SCRIPTMGR->GetImage(i+FACELISTSTARTIDX , &m_ImageFace[i], PFT_HARDPATH);

		cIcon * pIcon;

		pIcon=new cIcon;

		pIcon->Init(0,0,16,16,&m_ImageFace[i] ,IG_DEALITEM_START+7000+i);
				
		pIcon->SetData(i+FACELISTSTARTIDX);

		pIcon->SetDisable(TRUE);

		pIcon->SetDepend(FALSE);
        
		m_ImageGrid->AddIcon(i,pIcon);
	}
	m_ImageGrid->SetActive(FALSE);
}




















