#include "stdafx.h"
#include "cShopItemInventPlusDlg.h"

#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
cShopItemInventPlusDlg::cShopItemInventPlusDlg(void)
{
	m_type		= WT_ICONGRIDDIALOG;
	m_nIconType = WT_ITEM;	
}

cShopItemInventPlusDlg::~cShopItemInventPlusDlg(void)
{
}


BOOL cShopItemInventPlusDlg::InPt(POSTYPE absPos)
{
	return (TP_SHOPINVEN_START+20<=absPos&&absPos<TP_SHOPINVEN_END);
}
BOOL cShopItemInventPlusDlg::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	return DeleteIcon(Pos-(TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM), (cIcon **)ppItem);
}

BOOL cShopItemInventPlusDlg::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	return GAMEIN->GetInventoryDialog()->FakeMoveIcon(x,y,icon);
}
BOOL cShopItemInventPlusDlg::AddItem(CItem* pItem)
{
	if(!pItem)		return FALSE;	

	if( pItem->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL )
	{
		pItem->SetMovable(TRUE);
	}

	ITEMMGR->RefreshItem( pItem );

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( !pItemInfo )		return FALSE;


	if( pItemInfo->ItemKind==eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind==eSHOP_ITEM_DECORATION )
	{
		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
		if( pUsedInfo )
			ITEMMGR->AddUsedAvatarItemToolTip( pUsedInfo );

	}

	else if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
	{

		SHOPITEMBASE* pUsedInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
		if( pUsedInfo )
			ITEMMGR->AddUsedShopEquipItemToolTip( pUsedInfo );		
	}


	POSTYPE relPos = GetRelativePosition(pItem->GetPosition());


	return AddIcon(relPos, pItem);
}




POSTYPE cShopItemInventPlusDlg::GetRelativePosition( POSTYPE absPos)
{
	return ( absPos - (TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM) ) % 80;
}



BOOL cShopItemInventPlusDlg::FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem)
{
	WORD ToPos=0;


	if( !GetPositionForXYRef(MouseX, MouseY, ToPos) )
		return FALSE;	

	ToPos = ToPos+TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM;

	CItem * pToItem = GetItemForPos( ToPos );

	if(pToItem && pToItem->IsLocked() == TRUE)
		return FALSE;


	return FakeGeneralItemMove( ToPos, pSrcItem, pToItem );
}




CItem* cShopItemInventPlusDlg::GetItemForPos(POSTYPE absPos)
{
	if(InPt(absPos)/*TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM <= absPos && absPos < TP_SHOPINVEN_END*/)
	{
		return (CItem*)GetIconForIdx(absPos-(TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM));
	}

	return NULL;

}


BOOL cShopItemInventPlusDlg::FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem)
{
	if(CanBeMoved(pFromItem, ToPos) == FALSE)
		return FALSE;
	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;


	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if (ToIdx >= eItemTable_Max) return FALSE;

	if(pFromItem->GetPosition() == ToPos)
		return FALSE;

	if( pToItem && pToItem->GetUseParam() == 1 )
	{
		if( FromIdx != ToIdx )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(748) );
			return FALSE;
		}
	}


	if(pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;

	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}


BOOL cShopItemInventPlusDlg::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	if( TP_SHOPITEM_START <= pos && pos < TP_SHOPINVEN_END )
	{
		POSTYPE fromPos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= fromPos && fromPos < TP_SHOPINVEN_END )
			return TRUE;
		else
			return FALSE;

	}
	else
		return FALSE;
}


BOOL cShopItemInventPlusDlg::GetBlankPositionRestrictRef(WORD& absPos)
{	
	for(int j = 0 ; j < GetCellNum() ; ++j)
	{
		if(IsAddable(j))

		{
			absPos = TP_SHOPINVEN_START+j+TABCELL_SHOPINVEN_NUM;

			return TRUE;
		}
	}

	return FALSE;
}


BOOL cShopItemInventPlusDlg::GetBlankNum()
{
	int nBlankNum = 0;

	for( int i = 0; i < GetCellNum(); ++i )
	{
		if( IsAddable(i) )
		{
			++nBlankNum;
		}
	}

	return nBlankNum;
}