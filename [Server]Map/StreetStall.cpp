



#include "stdafx.h"
#include "StreetStall.h"
#include "usertable.h"
#include "Player.h"
#include "objectstatemanager.h"
#include "itemmanager.h"


cStreetStall::cStreetStall()
{
	Init();
}

cStreetStall::~cStreetStall()
{
	m_pOwner = NULL;
	m_GuestList.RemoveAll();
//	EmptyCellAll();
}

void cStreetStall::Init()
{
	m_pOwner = NULL;
	m_nCurRegistItemNum = 0;
	EmptyCellAll();
	DeleteGuestAll();

	m_wStallKind = eSK_NULL;
//	m_nUsable = DEFAULT_USABLE_INVENTORY;	
	m_nTotalMoney = 0;
	
	m_nTotalGold =  0;  // 2014-10-30 ×Ü

}

// 2014-10-30 

BOOL cStreetStall::FillCell(ITEMBASE* pBase, MONEYTYPE money, MONEYTYPE gold, BOOL bLock, DWORD Volume, WORD wAbsPosition)
{
	WORD pos = 0;
	
	if(!m_pOwner) 
	{
		return FALSE;
	}
	
	if(m_wStallKind == eSK_SELL)
	{
		CItemSlot* pSlot = m_pOwner->GetSlot(pBase->Position);
		
		if( !pSlot )
		{
			ASSERT(0);
			return TRUE;
		}
		
		pSlot->SetLock(pBase->Position, bLock);
		
		int n = 0; 
		for(n = 0; n<MAX_STREETSTALL_CELLNUM;++n)
		{
			if( !m_sArticles[n].bFill )
				break;
		}
		pos = n;
		if( pos == MAX_STREETSTALL_CELLNUM ) return FALSE;
		if( pBase->wIconIdx == 0 ) return FALSE;
	}
	else if(m_wStallKind == eSK_BUY)
	{	// ±¸¸Å ³ëÁ¡ÀÇ °æ¿ì µ¥ÀÌÅÍ°¡ ÀÖÀ¸¸é µ¤¾î ¾º¿î´Ù
		pos = wAbsPosition;
		if(m_sArticles[pos].bFill)
			EmptyCell( pos );

	}

	m_sArticles[pos].bFill = TRUE;
	m_sArticles[pos].bLock = (WORD)Volume;
	m_sArticles[pos].sItemBase = *pBase;
	m_sArticles[pos].dwMoney = money; 
	m_sArticles[pos].dwGold = gold;           // 2014-10-30 !
	m_sArticles[pos].wVolume = bLock;
	++m_nCurRegistItemNum;	


	if(m_wStallKind == eSK_BUY)
	{
		m_nTotalMoney = 0;

		m_nTotalGold  = 0; // 2014-10-30 ×Ü
		
		for(int n = 0; n<MAX_STREETBUYSTALL_CELLNUM;++n)
		{
			if( !m_sArticles[n].bFill )

				continue;
			
			m_nTotalMoney += m_sArticles[n].dwMoney * m_sArticles[n].wVolume;

			m_nTotalGold  += m_sArticles[n].dwGold * m_sArticles[n].wVolume;
		}
	}
	return TRUE;
}


void cStreetStall::EmptyCell( ITEMBASE* pBase, eITEMTABLE tableIdx )
{

	WORD pos;
	BOOL bSetItemSlot = TRUE;
	CItemSlot* pSlot;

	if( m_pOwner )
	{
		pSlot = m_pOwner->GetSlot(tableIdx);
		if( !pSlot )
		{
			ASSERT(0);
			bSetItemSlot = FALSE;
		}
	}
	else 
		bSetItemSlot = FALSE;

	int n = 0;
	for(n = 0; n<MAX_STREETSTALL_CELLNUM;++n)
	{
		if( m_sArticles[n].bFill )
		{
			if(m_sArticles[n].sItemBase.dwDBIdx == pBase->dwDBIdx)
				break;
		}
	}

	pos = n;
	if(  pos == MAX_STREETSTALL_CELLNUM ) return;
	if( !m_sArticles[pos].bFill ) return; // ÀÌ¹Ì ºñ¿öÁ® ÀÖ´Ù..

	pSlot->SetLock(m_sArticles[pos].sItemBase.Position, FALSE);
	
	m_sArticles[pos].Init();

/*	Bugs code!!!!!!
	for( int i = pos ; i < MAX_STREETSTALL_CELLNUM-1 ; ++i )
	{
		if( !m_sArticles[i+1].bFill )
		{
			m_sArticles[i].Init();
			break;
		}
		m_sArticles[i] = m_sArticles[i+1];
	}
*/

	for( int i = pos ; i < MAX_STREETSTALL_CELLNUM-1 ; ++i )
	{
		if( !m_sArticles[i+1].bFill )
			break;

		m_sArticles[i] = m_sArticles[i+1];
		m_sArticles[i+1].Init();
	}

	--m_nCurRegistItemNum;
}

void cStreetStall::EmptyCell( POSTYPE pos )
{
	if(  pos == MAX_STREETSTALL_CELLNUM ) return;
	if( !m_sArticles[pos].bFill ) return; 


	m_nTotalMoney -= (m_sArticles[pos].dwMoney * m_sArticles[pos].wVolume);
	m_nTotalGold -= (m_sArticles[pos].dwGold * m_sArticles[pos].wVolume);
	m_sArticles[pos].Init();

	--m_nCurRegistItemNum;
}


void cStreetStall::UpdateCell( WORD pos, DURTYPE dur )
{
	if( pos > MAX_STREETSTALL_CELLNUM ) return;
	if( !m_sArticles[pos].bFill ) return;
	
	if(m_wStallKind == eSK_SELL)
		m_sArticles[pos].sItemBase.Durability = dur;
	else if(m_wStallKind == eSK_BUY)
		m_sArticles[pos].wVolume = (WORD)dur;
}


void cStreetStall::EmptyCellAll()
{
	BOOL bSetItemSlot = TRUE;
	CItemSlot* pSlot;
	CItemSlot* pSlotShop;

	if( m_pOwner )
	{
		pSlot = m_pOwner->GetSlot(eItemTable_Inventory);
		pSlotShop = m_pOwner->GetSlot(eItemTable_ShopInven);
		if( !pSlot || !pSlotShop)
		{
			ASSERT(0);
			bSetItemSlot = FALSE;
		}
	}
	else
		bSetItemSlot = FALSE;

	for(int i=0;i<MAX_STREETSTALL_CELLNUM;++i)
	{

		if( !m_sArticles[i].bFill )
		{
			if(m_wStallKind == eSK_SELL)
				break;
			if(m_wStallKind == eSK_BUY)
				continue;
		}

		if( bSetItemSlot )
		{
			if(m_wStallKind == eSK_SELL)
			{
				CItemSlot* pItemSlot = m_pOwner->GetSlot( m_sArticles[i].sItemBase.Position );
				if( pItemSlot )
					pSlot->SetLock(m_sArticles[i].sItemBase.Position, FALSE);
			}
//			pSlot->SetLock(m_sArticles[i].sItemBase.Position, FALSE);
//			pSlotShop->SetLock(m_sArticles[i].sItemBase.Position, FALSE);
		}
		m_sArticles[i].Init();
		m_sArticles[i].dwMoney = 0;
	}
	m_nCurRegistItemNum = 0;
}

void cStreetStall::SetMoney(WORD pos, MONEYTYPE money)
{
	if( pos > MAX_STREETSTALL_CELLNUM ) return; // °¹¼ö¸¦ ÃÊ°úÇÑ´Ù??
	if( !m_sArticles[pos].bFill ) return; // ºñ¿öÁ® ÀÖ´Ù..
	if( m_sArticles[pos].bLock ) return;

	m_sArticles[pos].dwMoney = money;
}
// 2014-10-30 ÉèÖÃÊý¾Ý
void cStreetStall::SetGold(WORD pos, MONEYTYPE gold)
{
	if( pos > MAX_STREETSTALL_CELLNUM ) return; // °¹¼ö¸¦ ÃÊ°úÇÑ´Ù??
	if( !m_sArticles[pos].bFill ) return; // ºñ¿öÁ® ÀÖ´Ù..
	if( m_sArticles[pos].bLock ) return;

	m_sArticles[pos].dwGold = gold;
}

void cStreetStall::SetVolume( WORD pos, WORD Volume )
{
	if( pos > MAX_STREETSTALL_CELLNUM ) return; // °¹¼ö¸¦ ÃÊ°úÇÑ´Ù??
	if( !m_sArticles[pos].bFill ) return; // ºñ¿öÁ® ÀÖ´Ù..
	if( m_sArticles[pos].bLock ) return;
	
	m_sArticles[pos].wVolume = Volume;
}

void cStreetStall::ChangeCellState( WORD pos, BOOL bLock ) // (ÁÖÀÇ)µî·Ï°ú´Â ´Ù¸£°Ô ¹è¿­ÀÇ ÀÎµ¦½º°¡ ³Ñ¾î¿Â´Ù.
{
	if(!m_pOwner) 
	{
		return;
	}

	if( pos > MAX_STREETSTALL_CELLNUM ) return; // °¹¼ö¸¦ ÃÊ°úÇÑ´Ù??
	if( !m_sArticles[pos].bFill ) return; // ºñ¿öÁ® ÀÖ´Ù..
	if( m_sArticles[pos].bLock == bLock ) return;
	if( m_wStallKind == eSK_BUY ) return;

	CItemSlot* pSlot = m_pOwner->GetSlot(m_sArticles[pos].sItemBase.Position);
	if( !pSlot )
	{
		ASSERT(0);
		return;
	}

	pSlot->SetLock(m_sArticles[pos].sItemBase.Position, bLock);
	m_sArticles[pos].bLock = bLock;
}

void cStreetStall::AddGuest( CPlayer* pGuest )
{
	if( m_GuestList.Find(pGuest) ) return;

	pGuest->SetGuestStall( this );
	m_GuestList.AddTail( pGuest );
}

void cStreetStall::DeleteGuest( CPlayer* pGuest )
{
	pGuest->SetGuestStall( NULL );

	if( m_GuestList.Find(pGuest) == FALSE ) return;
	m_GuestList.Remove( pGuest );
}

void cStreetStall::DeleteGuestAll()
{
	PTRLISTPOS pos =  m_GuestList.GetHeadPosition();

	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)m_GuestList.GetNext( pos );
		pGuest->SetGuestStall( NULL );
	}
	m_GuestList.RemoveAll();
}

void cStreetStall::GetStreetStallInfo( STREETSTALL_INFO& stall )
{
	ITEM_OPTION_INFO OptionInfo[SLOT_STREETSTALL_NUM];
	ITEM_RARE_OPTION_INFO RareOptionInfo[SLOT_STREETSTALL_NUM];
	PET_TOTALINFO PetInfo[SLOT_STREETSTALL_NUM];
	TITAN_TOTALINFO TitanInfo[SLOT_STREETSTALL_NUM];
	TITAN_ENDURANCE_ITEMINFO TitanEquipInfo[SLOT_STREETSTALL_NUM];

	WORD OptionNum = 0;
	WORD RareOptionNum = 0;
	WORD PetInfoNum = 0;


	stall.StallKind = m_wStallKind;
	
	for(int i=0;i<MAX_STREETSTALL_CELLNUM;++i)
	{
		stall.Item[i].wIconIdx = m_sArticles[i].sItemBase.wIconIdx;
		stall.Item[i].dwDBIdx = m_sArticles[i].sItemBase.dwDBIdx;
		stall.Item[i].Durability = m_sArticles[i].sItemBase.Durability;
		stall.Item[i].money = m_sArticles[i].dwMoney;
		stall.Item[i].gold = m_sArticles[i].dwGold; 
		stall.Item[i].Locked = m_sArticles[i].bLock;
		stall.Item[i].Fill = m_sArticles[i].bFill;
		stall.Item[i].ItemParam = m_sArticles[i].sItemBase.ItemParam;
		stall.Item[i].dwRareIdx = m_sArticles[i].sItemBase.RareIdx;
		stall.Item[i].wVolume = m_sArticles[i].wVolume;
		stall.Item[i].dwGrow     = m_sArticles[i].sItemBase.ItemGrow; 
		//[ÄÍ¾Ã£¬ÖÆÔìÕß¸½¼Ó][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/1/2]
		stall.Item[i].wAbrasion = m_sArticles[i].sItemBase.ItemAbrasion;
		stall.Item[i].wMaxItemAbrasion=m_sArticles[i].sItemBase.MaxItemAbrasion;
		strcpy(stall.Item[i].cMaker,m_sArticles[i].sItemBase.ItemMaker);
		strcpy(stall.Item[i].cMaker1,m_sArticles[i].sItemBase.ItemMaker1);
		if( stall.Item[i].Fill )
		{

			if( ITEMMGR->IsOptionItem(stall.Item[i].wIconIdx, stall.Item[i].Durability) )
			{
				memcpy(&OptionInfo[OptionNum], GetOwner()->GetItemOption(stall.Item[i].Durability), sizeof(ITEM_OPTION_INFO));
				++OptionNum;
			}
			if( ITEMMGR->IsRareOptionItem(stall.Item[i].wIconIdx, stall.Item[i].dwRareIdx) )
			{
				memcpy(&RareOptionInfo[RareOptionNum], GetOwner()->GetItemRareOption(stall.Item[i].dwRareIdx), sizeof(ITEM_RARE_OPTION_INFO));
				++RareOptionNum;
			}
		
			if( ITEMMGR->IsPetSummonItem((WORD)stall.Item[i].wIconIdx))
			{
				memcpy(&PetInfo[PetInfoNum], GetOwner()->GetPetManager()->GetPetTotalInfo(stall.Item[i].dwDBIdx), sizeof(PET_TOTALINFO));
				++PetInfoNum;
			}
		}
	}

	if( OptionNum )
	{
		stall.AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO)*OptionNum, OptionInfo );
		stall.count = OptionNum;
	}
	if( RareOptionNum )
	{
		stall.AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO)*RareOptionNum, RareOptionInfo );
		stall.RareCount = RareOptionNum;
	}
	if( PetInfoNum )
	{
		stall.AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO)*PetInfoNum, PetInfo );
		stall.PetItemCount = PetInfoNum;
	}
	if( m_pOwner )
		m_pOwner->GetStreetStallTitle(stall.Title);
}

BOOL cStreetStall::CheckItemDBIdx(DWORD idx)
{
	for(int i=0;i<MAX_STREETSTALL_CELLNUM;++i)
	{
		if( m_sArticles[i].bFill )
		{
			if( m_sArticles[i].sItemBase.dwDBIdx == idx)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL cStreetStall::CheckItemIdx(DWORD idx)
{
	for(int i=0;i<MAX_STREETSTALL_CELLNUM;++i)
	{
		if( m_sArticles[i].bFill )
		{

			if( m_sArticles[i].sItemBase.wIconIdx == idx)
				return FALSE;
		}
	}
	
	return TRUE;
}

BOOL cStreetStall::IsFull()
{
	for(int i=0;i<MAX_STREETSTALL_CELLNUM;++i)
	{
		if( !m_sArticles[i].bFill )
			return FALSE;
	}
	
	return TRUE;
}


/* ¼Õ´ÔÀÇ »óÅÂ º¯°æ Flag Ãß°¡ 03.12.26 */
void cStreetStall::SendMsgGuestAll( MSGBASE* pMsg, int nMsgLen, BOOL bChangeState )
{
	PTRLISTPOS pos =  m_GuestList.GetHeadPosition();

	while(pos)
	{
		CPlayer* pGuest = (CPlayer*)m_GuestList.GetNext( pos );
		if( !pGuest )
			continue;

		pGuest->SendMsg( pMsg, nMsgLen);

		if( bChangeState )
		{
			if( pGuest->GetState() == eObjectState_StreetStall_Guest)
				OBJECTSTATEMGR_OBJ->EndObjectState(pGuest, eObjectState_StreetStall_Guest);
		}
	}
}


