#include "stdafx.h"
#include "ExchangeRoom.h"
#include "ExchangeManager.h"
#include "mapdbmsgparser.h"
#include "Player.h"
#include "ItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"

CExchangeRoom::CExchangeRoom()
{
	
}

CExchangeRoom::~CExchangeRoom()
{

}

void CExchangeRoom::Init( CPlayer* pPlayer1, CPlayer* pPlayer2 )
{
	sEXCHANGECONTAINER ExchangeContainer;

	m_ExchangeData[0].pPlayer		= pPlayer1;
	m_ExchangeData[0].bExchange		= FALSE;
	m_ExchangeData[0].bLock			= FALSE;
	m_ExchangeData[0].dwMoney		= 0;
	m_ExchangeData[0].dwGold		= 0; // 2014-10-29 ÊýÁ¿
	m_ExchangeData[0].nAddItemNum	= 0;
	ZeroMemory( m_ExchangeData[0].ItemInfo, sizeof(ITEMBASE)*MAX_EXCHANGEITEM );

	m_ExchangeData[1].pPlayer		= pPlayer2;
	m_ExchangeData[1].bExchange		= FALSE;
	m_ExchangeData[1].bLock			= FALSE;
	m_ExchangeData[1].dwMoney		= 0;
	m_ExchangeData[1].dwGold		= 0; // 2014-10-29 ÊýÁ¿
	m_ExchangeData[1].nAddItemNum	= 0;
	ZeroMemory( m_ExchangeData[1].ItemInfo, sizeof(ITEMBASE)*MAX_EXCHANGEITEM );

	ExchangeContainer.pRoom		= this;

	ExchangeContainer.nMyIndex	= 0;
	ExchangeContainer.pOpPlayer = pPlayer2;
	pPlayer1->SetExchangeContainer( &ExchangeContainer );

	ExchangeContainer.nMyIndex	= 1;
	ExchangeContainer.pOpPlayer = pPlayer1;
	pPlayer2->SetExchangeContainer( &ExchangeContainer );


	m_nExchangeState = EXCS_WAITING;
}

void CExchangeRoom::Exit()
{
//item lock Ç®±â
	CItemSlot* pItemSlot	= NULL;
	POSTYPE	ItemPos;
	
	for( int j = 0 ; j < 2 ; ++j )
	for( int i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
	{
		ItemPos		= m_ExchangeData[j].ItemInfo[i].Position;
		pItemSlot	= m_ExchangeData[j].pPlayer->GetSlot( ItemPos );
		if( pItemSlot )
			pItemSlot->SetLock( ItemPos, FALSE );
	}	

//player data ÃÊ±âÈ­
	sEXCHANGECONTAINER ExchangeContainer;	//ÃÊ±âÈ­ÇÔ
	if( m_ExchangeData[0].pPlayer )
		m_ExchangeData[0].pPlayer->SetExchangeContainer( &ExchangeContainer );
	if( m_ExchangeData[1].pPlayer )
		m_ExchangeData[1].pPlayer->SetExchangeContainer( &ExchangeContainer );
//
}

void CExchangeRoom::Lock( int nIndex, BOOL bLock )
{
	if( bLock )
		m_ExchangeData[nIndex].bLock = TRUE;
	else
	{
		m_ExchangeData[0].bLock = FALSE;
		m_ExchangeData[1].bLock = FALSE;

		m_ExchangeData[0].bExchange = FALSE;
		m_ExchangeData[1].bExchange = FALSE;		
	}
}

BOOL CExchangeRoom::IsAllLock()
{
	if( m_ExchangeData[0].bLock && m_ExchangeData[1].bLock )
		return TRUE;

	return FALSE;
}

void CExchangeRoom::SetExchange( int nIndex )
{
	m_ExchangeData[nIndex].bExchange = TRUE;
}

BOOL CExchangeRoom::IsAllExchange()
{
	if( m_ExchangeData[0].bExchange && m_ExchangeData[1].bExchange )
		return TRUE;

	return FALSE;
}


BOOL CExchangeRoom::AddItem( int nPlayerIndex, ITEMBASE* pItemInfo )
{
	if( IsLocked( nPlayerIndex ) ) return FALSE;
	if( m_ExchangeData[nPlayerIndex].nAddItemNum >= MAX_EXCHANGEITEM )
		return FALSE;

	if( pItemInfo->QuickPosition != 0 ) return FALSE;

//¼­¹ö¿¡µµ ¾ÆÀÌÅÛ LOCK
	// RaMa
	eITEMTABLE Table = eItemTable_Inventory;
	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItemInfo->wIconIdx );
	if(!pInfo)		return FALSE;

	CItemSlot* pItemSlot = NULL;	
	if( pInfo->ItemKind & eSHOP_ITEM )
	{
		Table = eItemTable_ShopInven;
		pItemSlot = (CShopInvenSlot*)m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( eItemTable_ShopInven );

		if( pInfo->ItemType == 11 )
		if( !(pItemInfo->ItemParam & ITEM_PARAM_SEAL) )
			return FALSE;
		
		// 2014-11-20 ÅÝµãÎÞ·¨ 

		if(pItemInfo->ItemStatic == ITEM_STATIC_PD)
		{
			return FALSE;
		}

        // 2014-12-08 ±»ËøÎÞ·¨ 
		if(pItemInfo->ItemStatic ==ITEM_STATIC_LUCK)
		{
            return FALSE;
		}
		
		// °Å·¡ºÒ°¡ ¾ÆÀÌÅÛ
		if( pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
		{
			if( pInfo->BuyPrice )
				return FALSE;
		}
		else if( pInfo->MeleeAttackMax )
			return FALSE;
	}
	else
		pItemSlot = (CInventorySlot*)m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( eItemTable_Inventory );	 
	

	if( pItemSlot )
	{
		if( GetItemKind( pItemInfo->wIconIdx ) == eYOUNGYAK_ITEM )
		{
			if( pItemSlot->CheckQPosForItemIdx( pItemInfo->wIconIdx ) == FALSE )
				return FALSE;
		}
		else
		{
			if( pItemSlot->GetItemInfoAbs( pItemInfo->Position )->QuickPosition != 0 )
				return FALSE;
		}


		if( pItemSlot->IsLock( pItemInfo->Position ) ) return FALSE;
		pItemSlot->SetLock( pItemInfo->Position, TRUE );
	}
//
	m_ExchangeData[nPlayerIndex].ItemInfo[m_ExchangeData[nPlayerIndex].nAddItemNum] = *pItemInfo;
	++m_ExchangeData[nPlayerIndex].nAddItemNum;

	return TRUE;
}

BOOL CExchangeRoom::DelItem( int nPlayerIndex, POSTYPE pos )
{
//	ASSERT( pos >=0 && pos < m_ExchangeData[nPlayerIndex].nAddItemNum );
	// ¿¡·¯Ã³¸®
	if( pos < 0 || pos >= m_ExchangeData[nPlayerIndex].nAddItemNum )
		return FALSE;
	
	if( IsLocked( nPlayerIndex ) ) return FALSE;
	if( m_ExchangeData[nPlayerIndex].nAddItemNum <= 0 )
		return FALSE;

	
//¼­¹ö¿¡µµ ¾ÆÀÌÅÛ UNLOCK
	POSTYPE ItemAbsPos = m_ExchangeData[nPlayerIndex].ItemInfo[pos].Position;

	CItemSlot* pItemSlot = m_ExchangeData[nPlayerIndex].pPlayer->GetSlot( ItemAbsPos );
	if( pItemSlot )
		pItemSlot->SetLock( ItemAbsPos, FALSE );
//

	--m_ExchangeData[nPlayerIndex].nAddItemNum;

	for( int i = pos ; i < m_ExchangeData[nPlayerIndex].nAddItemNum ; ++i )
	{
		m_ExchangeData[nPlayerIndex].ItemInfo[i] = m_ExchangeData[nPlayerIndex].ItemInfo[i+1];
	}

	return TRUE;

}

DWORD CExchangeRoom::InputMoney( int nPlayerIndex, DWORD dwMoney )
{
	if( dwMoney > m_ExchangeData[nPlayerIndex].pPlayer->GetMoney() )
		dwMoney = m_ExchangeData[nPlayerIndex].pPlayer->GetMoney();

	m_ExchangeData[nPlayerIndex].dwMoney = dwMoney;

	return dwMoney;
}

// 2014-10-29 ÊäÈë·þÎñ¶ËÐ­Òé

DWORD CExchangeRoom::InputGold( int nPlayerIndex, DWORD dwGold )
{
	if( dwGold > m_ExchangeData[nPlayerIndex].pPlayer->GetGoldMoney() )
		dwGold = m_ExchangeData[nPlayerIndex].pPlayer->GetGoldMoney();

	m_ExchangeData[nPlayerIndex].dwGold = dwGold;

	return dwGold;
}

int CExchangeRoom::DoExchange()
{
	CInventorySlot* pInventory[2];
	CShopInvenSlot* pShopInventory[2];
	pInventory[0] = (CInventorySlot*)m_ExchangeData[0].pPlayer->GetSlot( eItemTable_Inventory );
	pInventory[1] = (CInventorySlot*)m_ExchangeData[1].pPlayer->GetSlot( eItemTable_Inventory );
	pShopInventory[0] = (CShopInvenSlot*)m_ExchangeData[0].pPlayer->GetSlot( eItemTable_ShopInven );
	pShopInventory[1] = (CShopInvenSlot*)m_ExchangeData[1].pPlayer->GetSlot( eItemTable_ShopInven );

	if( pInventory[0] == NULL || pInventory[1] == NULL || pShopInventory[0] == NULL || pShopInventory[1] == NULL )
		return eEE_ERROR;

	//ºóÀÚ¸®Ã¼Å©	//Áö¿öÁú ÀÚ¸®µµ °í·ÁÇÑ´Ù?(ok)
	WORD EmptySellPos[2][MAX_EXCHANGEITEM];
	WORD ShopEmptySellPos[2][MAX_EXCHANGEITEM];
	WORD sOut;

	// RaMa
	int i, j;
	ITEM_INFO* pItemInfo = NULL;
	WORD shopitemcount[2] = { 0, };
	WORD itemcount[2] = { 0, };

	for( j=0; j<2; j++ )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; i++ )
		{
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
			if(!pItemInfo)
				return eEE_ERROR;

			if( pItemInfo->ItemKind & eSHOP_ITEM )
				++shopitemcount[j];
			else
				++itemcount[j];
		}
	}
	//

	if( itemcount[1] > itemcount[0] )
	{
		sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], itemcount[1] - itemcount[0] );
		if( sOut != itemcount[1] - itemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( itemcount[0] > itemcount[1] )
	{
		sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], itemcount[0] - itemcount[1] );
		if( sOut != itemcount[0] - itemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}
	if( shopitemcount[1] > shopitemcount[0] )
	{
		sOut = pShopInventory[0]->GetEmptyCell( ShopEmptySellPos[0], shopitemcount[1] - shopitemcount[0] );
		if( sOut != shopitemcount[1] - shopitemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( shopitemcount[0] > shopitemcount[1] )
	{
		sOut = pShopInventory[1]->GetEmptyCell( ShopEmptySellPos[1], shopitemcount[0] - shopitemcount[1] );
		if( sOut != shopitemcount[0] - shopitemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}


/*
	if( m_ExchangeData[1].nAddItemNum > m_ExchangeData[0].nAddItemNum )
	{
		sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], itemcount[1] - itemcount[0] );

		if( sOut != itemcount[1] - itemcount[0] )
			return eEE_NOTENOUGHSPACE;
		sOut = pShopInventory[0]->GetEmptyCell( ShopEmptySellPos[0], shopitemcount[1] - shopitemcount[0] );
		if( sOut != shopitemcount[1] - shopitemcount[0] )
			return eEE_NOTENOUGHSPACE;
	}
	if( m_ExchangeData[0].nAddItemNum > m_ExchangeData[1].nAddItemNum )
	{
		sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], itemcount[0] - itemcount[1] );
		if( sOut != itemcount[0] - itemcount[1] )
			return eEE_NOTENOUGHSPACE;
		sOut = pShopInventory[1]->GetEmptyCell( ShopEmptySellPos[1], shopitemcount[0] - shopitemcount[1] );
		if( sOut != shopitemcount[0] - shopitemcount[1] )
			return eEE_NOTENOUGHSPACE;
	}
*/
	
	//µ·Ã¼Å©
	if( m_ExchangeData[0].dwMoney > m_ExchangeData[0].pPlayer->GetMoney() )
		return eEE_NOTENOUGHMONEY;
	if( m_ExchangeData[1].dwMoney > m_ExchangeData[1].pPlayer->GetMoney() )
		return eEE_NOTENOUGHMONEY;

	// 2014-10-29 Ìá½»ºó£¬¶Ô½á¹û½øÐÐÈ·ÈÏ!

	if( m_ExchangeData[0].dwGold > m_ExchangeData[0].pPlayer->GetGoldMoney() )
		return eEE_NOTENOUGHMONEY;
	if( m_ExchangeData[1].dwGold > m_ExchangeData[1].pPlayer->GetGoldMoney() )
		return eEE_NOTENOUGHMONEY;


	//2007. 8. 29. CBH - ±Ý¾× ÃÖ´ë°ª °è»ê ÀÚ·áÇü ¿À¹öÇÃ·Î¿ì °³¼±
	if( m_ExchangeData[1].dwMoney > MAX_INVENTORY_MONEY - m_ExchangeData[0].pPlayer->GetMoney() )
		return eEE_MAXMONEY;
	if( m_ExchangeData[0].dwMoney > MAX_INVENTORY_MONEY - m_ExchangeData[1].pPlayer->GetMoney() )
		return eEE_MAXMONEY;

	// 2014-10-29 Ìá½»ºó£¬¶ÔÊý¾Ý½øÐÐºÏ·¨ÐÔ¼ì²é!

	if( m_ExchangeData[1].dwGold > MAX_INVENTORY_GOLD - m_ExchangeData[0].pPlayer->GetGoldMoney() )
		return eEE_MAXMONEY;
	if( m_ExchangeData[0].dwGold > MAX_INVENTORY_GOLD - m_ExchangeData[1].pPlayer->GetGoldMoney() )
		return eEE_MAXMONEY;
	
///////////////////////////////////////////////
//¾ÆÀÌÅÛ °Ë»ç ÄÚµå ³ÖÀÚ.. Á¦ÀÚ¸®¿¡ ´Ù µé¾îÀÖ³ª?	Å×½ºÆ® ÇÊ¿ä.
	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			//const ITEMBASE* pItemBase = pInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );
			// RaMa
			const ITEMBASE* pItemBase = NULL;
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );			
			if( pItemInfo->ItemKind & eSHOP_ITEM )
				pItemBase = pShopInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );
			else
				pItemBase = pInventory[j]->GetItemInfoAbs( m_ExchangeData[j].ItemInfo[i].Position );

			if( pItemBase->dwDBIdx != m_ExchangeData[j].ItemInfo[i].dwDBIdx ||
				pItemBase->wIconIdx != m_ExchangeData[j].ItemInfo[i].wIconIdx ||
				pItemBase->Durability != m_ExchangeData[j].ItemInfo[i].Durability ||
				pItemBase->Position != m_ExchangeData[j].ItemInfo[i].Position )
				return eEE_ERROR;			//½ÇÁ¦ ÀÎº¥Åä¸®¿Í ±³È¯Ã¢ÀÇ Á¤º¸°¡ ´Ù¸£´Ù.

			if( ITEMMGR->IsDupItem(pItemBase->wIconIdx) )
			if( pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM/*MAX_DURABILITY*/ )
				return eEE_ERROR;
		}
	}

	
	
//º»»óÅÂ·Î º¹±¸°¡ °¡´ÉÇÑ°¡?
/////////////
//EXCHANGE!!!
	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			ITEMBASE ItemBase;
//			pInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//Áß¿ä! Ã¼Å©.			
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
			if( pItemInfo->ItemKind & eSHOP_ITEM )
				pShopInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//Áß¿ä! Ã¼Å©.
			else
				pInventory[j]->DeleteItemAbs( NULL, m_ExchangeData[j].ItemInfo[i].Position, &ItemBase, SS_LOCKOMIT );	//Áß¿ä! Ã¼Å©.


			//»èÁ¦ ¸Þ¼¼Áö..	//¸ð¾Æ¼­ ÇÑ²¨¹ø¿¡ Á¦°ÅÇÏÀÚ.
			MSG_EXCHANGE_REMOVEITEM RemoveMsg;
			RemoveMsg.Category = MP_EXCHANGE;
			RemoveMsg.Protocol = MP_EXCHANGE_REMOVE;
			RemoveMsg.wAbsPosition = m_ExchangeData[j].ItemInfo[i].Position;
			m_ExchangeData[j].pPlayer->SendMsg( &RemoveMsg, sizeof( RemoveMsg ) );
		}
	}

/*
	sOut = pInventory[0]->GetEmptyCell( EmptySellPos[0], m_ExchangeData[1].nAddItemNum );
	if( sOut != m_ExchangeData[1].nAddItemNum )
		return FALSE;

	sOut = pInventory[1]->GetEmptyCell( EmptySellPos[1], m_ExchangeData[0].nAddItemNum );
	if( sOut != m_ExchangeData[0].nAddItemNum  )
		return FALSE;
*/	

	for( j = 0 ; j < 2 ; ++ j )
	{
		for( i = 0 ; i < m_ExchangeData[j].nAddItemNum ; ++i )
		{
			POSTYPE EmptyPos;
			/*if( pInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //ºó°ø°£ ¾øÀ»¶§ ¿¡·¯Ã¼Å©ÇÏ±â
			{
				ASSERT(0);
				continue;
			}*/
			// RaMa
			pItemInfo = ITEMMGR->GetItemInfo( m_ExchangeData[j].ItemInfo[i].wIconIdx );
			if( pItemInfo->ItemKind & eSHOP_ITEM )
			{
				if( pShopInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //ºó°ø°£ ¾øÀ»¶§ ¿¡·¯Ã¼Å©ÇÏ±â
				{
					ASSERT(0);
					continue;
				}
			}
			else
			{
				if( pInventory[!j]->GetEmptyCell( &EmptyPos ) == 0 ) //ºó°ø°£ ¾øÀ»¶§ ¿¡·¯Ã¼Å©ÇÏ±â
				{
					ASSERT(0);
					continue;
				}
			}


			ItemUpdateToDB( m_ExchangeData[!j].pPlayer->GetID(), m_ExchangeData[j].ItemInfo[i].dwDBIdx,
							m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].Durability,
							EmptyPos, 0, m_ExchangeData[j].ItemInfo[i].RareIdx ,m_ExchangeData[j].ItemInfo[i].ItemStatic);


			m_ExchangeData[j].ItemInfo[i].Position = EmptyPos;

//itemoption
			if( ITEMMGR->IsOptionItem( m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].Durability) )
			{
				m_ExchangeData[!j].pPlayer->AddItemOption(
					m_ExchangeData[j].pPlayer->GetItemOption( m_ExchangeData[j].ItemInfo[i].Durability ) );
				//m_ExchangeData[j].pPlayer->RemoveItemOption( m_ExchangeData[j].ItemInfo[i].Durability );
				if( FALSE == m_ExchangeData[j].pPlayer->RemoveItemOption( m_ExchangeData[j].ItemInfo[i].Durability ) )
				{
					char szBuf[128];
					sprintf( szBuf, "ExchangeOptItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", m_ExchangeData[j].pPlayer->GetID()
						,m_ExchangeData[j].ItemInfo[i].wIconIdx );
					ASSERTMSG(0, szBuf);
				}
//				ItemOptionUpdateToDB( m_ExchangeData[j].ItemInfo[i].Durability, m_ExchangeData[!j].pPlayer->GetID() );
			}
			
			//SW050920 RareOptionItem
			if( ITEMMGR->IsRareOptionItem( m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].RareIdx ) )
			{
				m_ExchangeData[!j].pPlayer->AddItemRareOption(
					m_ExchangeData[j].pPlayer->GetItemRareOption( m_ExchangeData[j].ItemInfo[i].RareIdx ) );
				//m_ExchangeData[j].pPlayer->RemoveItemRareOption( m_ExchangeData[j].ItemInfo[i].RareIdx );
				if( FALSE == m_ExchangeData[j].pPlayer->RemoveItemRareOption( m_ExchangeData[j].ItemInfo[i].RareIdx ) )
				{
					char szBuf[128];
					sprintf( szBuf, "ExchangeRareItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", m_ExchangeData[j].pPlayer->GetID()
						,m_ExchangeData[j].ItemInfo[i].wIconIdx );
					ASSERTMSG(0, szBuf);
				}
			}

			if( ITEMMGR->IsPetSummonItem(m_ExchangeData[j].ItemInfo[i].wIconIdx) )
			{

				BOOL bReduceFriendly = !(m_ExchangeData[j].ItemInfo[i].ItemParam & ITEM_PARAM_SEAL);
				m_ExchangeData[j].pPlayer->GetPetManager()->ExchangePet(m_ExchangeData[j].ItemInfo[i].dwDBIdx,
					m_ExchangeData[!j].pPlayer, bReduceFriendly);
			}

			if( ITEMMGR->IsTitanCallItem(m_ExchangeData[j].ItemInfo[i].wIconIdx) )
			{

				m_ExchangeData[j].pPlayer->GetTitanManager()->ExchangeTitan(m_ExchangeData[j].ItemInfo[i].dwDBIdx,
					m_ExchangeData[!j].pPlayer);
			}

			if( ITEMMGR->IsTitanEquipItem(m_ExchangeData[j].ItemInfo[i].wIconIdx) )
			{
				m_ExchangeData[j].pPlayer->GetTitanManager()->ExchangeTitanEquipItem(m_ExchangeData[j].ItemInfo[i].dwDBIdx,
					m_ExchangeData[!j].pPlayer);

			}


			// RaMa
			int rt;
			if( pItemInfo->ItemKind & eSHOP_ITEM )
				rt = pShopInventory[!j]->InsertItemAbs( NULL, EmptyPos, &m_ExchangeData[j].ItemInfo[i], SS_LOCKOMIT );
			else
				rt = pInventory[!j]->InsertItemAbs( NULL, EmptyPos, &m_ExchangeData[j].ItemInfo[i], SS_LOCKOMIT );
			ASSERT( rt == EI_TRUE );

			//Ãß°¡ ¸Þ¼¼Áö...	//¸ð¾Æ¼­ ÇÑ¹ø¿¡ º¸³»´Â °É·Î ±³Ã¼ÇÏÀÚ. confirm
			MSG_ITEM InsertMsg;
			InsertMsg.Category = MP_EXCHANGE;
			InsertMsg.Protocol = MP_EXCHANGE_INSERT;
			InsertMsg.ItemInfo = m_ExchangeData[j].ItemInfo[i];
			m_ExchangeData[!j].pPlayer->SendMsg( &InsertMsg, sizeof( MSG_ITEM ) );

			LogItemMoney(m_ExchangeData[j].pPlayer->GetID(), m_ExchangeData[j].pPlayer->GetObjectName(), 
						m_ExchangeData[1-j].pPlayer->GetID(), m_ExchangeData[1-j].pPlayer->GetObjectName(),
						eLog_Exchange, m_ExchangeData[j].pPlayer->GetMoney(), m_ExchangeData[1-j].pPlayer->GetMoney(), 
						0, m_ExchangeData[j].ItemInfo[i].wIconIdx, m_ExchangeData[j].ItemInfo[i].dwDBIdx, m_ExchangeData[j].ItemInfo[i].Position, 0, m_ExchangeData[j].ItemInfo[i].Durability,
						m_ExchangeData[j].pPlayer->GetPlayerExpPoint());
		}
	}

	DWORD	dwMoney = 0;
	DWORD   dwGold=0;    // 2014-10-29 ÖÐ¼ä
	MSG_DWORD MoneyMsg;
	MoneyMsg.Category	= MP_EXCHANGE;
	MoneyMsg.Protocol	= MP_EXCHANGE_SETMONEY;

	int FromNum =0;
	for( j = 0 ; j < 2 ; ++j )
	{
		if( m_ExchangeData[j].dwMoney > m_ExchangeData[!j].dwMoney )	//³»°¡ °Çµ·ÀÌ ´õ ¸¹´Ù.
		{
			dwMoney = m_ExchangeData[j].dwMoney - m_ExchangeData[!j].dwMoney;
			m_ExchangeData[j].pPlayer->SetMoney( dwMoney, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseExchange, m_ExchangeData[1-j].pPlayer->GetID() );
			FromNum = j;
		}
		else if( m_ExchangeData[j].dwMoney < m_ExchangeData[!j].dwMoney )
		{
			dwMoney = m_ExchangeData[!j].dwMoney - m_ExchangeData[j].dwMoney;
			m_ExchangeData[j].pPlayer->SetMoney( dwMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetExchange, m_ExchangeData[1-j].pPlayer->GetID() );
		}
		else
		{
			continue;
		}

	}

	FromNum =0;
	for( j = 0 ; j < 2 ; ++j )
	{
		if( m_ExchangeData[j].dwGold > m_ExchangeData[!j].dwGold )	// 2014-10-29 
		{
			dwGold = m_ExchangeData[j].dwGold - m_ExchangeData[!j].dwGold;
			m_ExchangeData[j].pPlayer->SetGoldMoney( dwGold,2);
			FromNum = j;
			ItemShopUseLog(eLog_GoldMoneyDealLost, m_ExchangeData[j].pPlayer->GetID(), m_ExchangeData[j].pPlayer->GetObjectName(), 0, m_ExchangeData[!j].pPlayer->GetObjectName(), 0,
				0, 0, 0,
				m_ExchangeData[j].pPlayer->GetGoldMoney(), dwGold, m_ExchangeData[j].pPlayer->GetGoldMoney() - dwGold,
				0, 0,0);
		}
		else if( m_ExchangeData[j].dwGold < m_ExchangeData[!j].dwGold ) // 2014-10-29 
		{
			dwGold = m_ExchangeData[!j].dwGold - m_ExchangeData[j].dwGold;
			m_ExchangeData[j].pPlayer->SetGoldMoney(dwGold, eGetGoldMoney);
			ItemShopUseLog(eLog_GoldMoneyDealGet, m_ExchangeData[j].pPlayer->GetID(), m_ExchangeData[j].pPlayer->GetObjectName(), 0, m_ExchangeData[!j].pPlayer->GetObjectName(),0,
				0,0,0,
				m_ExchangeData[j].pPlayer->GetGoldMoney(), dwGold, m_ExchangeData[j].pPlayer->GetGoldMoney() + dwGold, 
				0,0,0);
		}
		else
		{
			continue;
		}

	}
	//[Ôª±¦½»Ò×ÈÕÖ¾][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/21]
	/*LogGoldMoney(eLog_GoldMoneyDeal,m_ExchangeData[FromNum].pPlayer->GetID(),m_ExchangeData[FromNum].pPlayer->GetGoldMoney(),
		m_ExchangeData[!FromNum].pPlayer->GetID(),m_ExchangeData[!FromNum].pPlayer->GetGoldMoney(),dwGold,
		m_ExchangeData[FromNum].ItemInfo->wIconIdx, m_ExchangeData[FromNum].ItemInfo->dwDBIdx, m_ExchangeData[FromNum].ItemInfo->Durability);*/

	LogItemMoney(m_ExchangeData[FromNum].pPlayer->GetID(), m_ExchangeData[FromNum].pPlayer->GetObjectName(),
		m_ExchangeData[!FromNum].pPlayer->GetID(), m_ExchangeData[!FromNum].pPlayer->GetObjectName(),
		eLog_Exchange, m_ExchangeData[FromNum].pPlayer->GetMoney(), m_ExchangeData[!FromNum].pPlayer->GetMoney(), 
		dwMoney, 0, 0, 0, 0, 0,
		m_ExchangeData[FromNum].pPlayer->GetPlayerExpPoint());

	return eEE_OK;
}


