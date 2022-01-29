#include "stdafx.h"

#include "ItemSlot.h"
#include "Purse.h"
#include "ItemManager.h"
#include <algorithm>
#include <vector>
#include "MapDBMsgParser.h"
#define ClearItemBase(a)		a.dwDBIdx = 0;	\
								a.wIconIdx = 0;	\
								a.Position = 0;	\
								a.QuickPosition = 0;	\
								a.Durability = 0;	\
								a.RareIdx = 0; \
								a.ItemGrow=0; \
								a.ItemAbrasion=0; \
								a.MaxItemAbrasion = 0;  \
								strcpy(a.ItemMaker,"无");  \
								strcpy(a.ItemMaker1,"无");

#define ClearSlotInfo(b)		b.bLock = 0;	\
								b.wPassword = 0;	\
								b.wState = 0;
bool cmp(const ITEMBASE& item1, const ITEMBASE& item2)
{//[背包整理排序比较函数][By:十里坡剑神][QQ:112582793][2018/6/11]
	  return item1.wIconIdx > item2.wIconIdx;
}

CItemSlot::CItemSlot()
{
	m_ItemBaseArray = NULL;
	m_SlotInfoArray = NULL;
	m_pPurse		= NULL;
}

CItemSlot::~CItemSlot()
{
	Release();
}

char * CItemSlot::_ToStringSlot()
{
	static char slotBuf[2512];
	slotBuf[0] = 0;
	char tmp[64];
	for( POSTYPE i = 0 ; i < m_SlotNum ; ++i )
	{
		sprintf(tmp, "DBIDX:%d ITEMIDX:%d POS:%d QPOS:%d DURA:%d LOCK:%d PWD:%d\n", 
							m_ItemBaseArray[i].dwDBIdx,
							m_ItemBaseArray[i].wIconIdx, 
							m_ItemBaseArray[i].Position, 
							m_ItemBaseArray[i].QuickPosition, 
							m_ItemBaseArray[i].Durability, 
							m_SlotInfoArray[i].bLock, 
							m_SlotInfoArray[i].wPassword);
		strcat(slotBuf, tmp);
		if( strlen(slotBuf) > 512 ) break;
	}
	return slotBuf;
}

void CItemSlot::Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo)
{
	m_StartAbsPos		= startAbsPos;
	m_SlotNum			= slotNum;
	m_ItemBaseArray		= pInfo;
	m_SlotInfoArray		= pSlotInfo;
	bIsMovePack = false;
}

BOOL CItemSlot::CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max)
{
	if(!m_pPurse)
	m_pPurse = pNewPurse;
	return m_pPurse->InitPurse( pOwner, money, max );
}

void CItemSlot::Release()
{
	m_pPurse		= NULL;
}

ITEMBASE /*const*/ * CItemSlot::GetItemInfoAbs(POSTYPE absPos)
{ 
	if( !IsPosIn(absPos) )	return NULL;

	return  &m_ItemBaseArray[absPos];	
}
void CItemSlot::MovePack(DWORD dwID, bool& bRet)
{
	if (bIsMovePack)
	{
		bRet = false;
		return;
	}
		
	WORD wItemCount = GetItemCount();

	std::sort(m_ItemBaseArray, m_ItemBaseArray + m_SlotNum, cmp);
	for (int i = 0; i < wItemCount; i++)
	{
		if (!ITEMMGR->IsDupItem(m_ItemBaseArray[i].wIconIdx)||
			m_ItemBaseArray[i].Durability >= MAX_YOUNGYAKITEM_DUPNUM)
			continue;
		for (int j = 0; j < wItemCount; j++)
		{
			if (j==i) continue;
			if (m_ItemBaseArray[j].Durability >= MAX_YOUNGYAKITEM_DUPNUM) continue;
			if (m_ItemBaseArray[i].wIconIdx != m_ItemBaseArray[j].wIconIdx) continue;
			DURTYPE Dur = m_ItemBaseArray[i].Durability + m_ItemBaseArray[j].Durability;
			if (Dur > MAX_YOUNGYAKITEM_DUPNUM)
			{
				m_ItemBaseArray[i].Durability = MAX_YOUNGYAKITEM_DUPNUM;
				m_ItemBaseArray[j].Durability = Dur - MAX_YOUNGYAKITEM_DUPNUM;
				m_ItemBaseArray[i].QuickPosition |= m_ItemBaseArray[j].QuickPosition;
				ItemCombineUpdateToDB(m_ItemBaseArray[j].dwDBIdx, m_ItemBaseArray[j].Durability, m_ItemBaseArray[i].dwDBIdx, m_ItemBaseArray[i].Durability);
			}
			else
			{
				m_ItemBaseArray[i].Durability = Dur;

				m_ItemBaseArray[i].QuickPosition |= m_ItemBaseArray[j].QuickPosition;
				ItemCombineUpdateToDB(m_ItemBaseArray[j].dwDBIdx, 0, m_ItemBaseArray[i].dwDBIdx, m_ItemBaseArray[i].Durability);			

				memset(&m_ItemBaseArray[j], 0, sizeof(ITEMBASE));
			}
			break;
		}
	}
	std::sort(m_ItemBaseArray, m_ItemBaseArray + m_SlotNum, cmp);
	for (int i = 0; i < m_SlotNum; i++)
	{
		if (m_ItemBaseArray[i].dwDBIdx == 0) break;
		m_ItemBaseArray[i].Position = i;
		ItemUpdateToDB(dwID, m_ItemBaseArray[i].dwDBIdx, m_ItemBaseArray[i].wIconIdx, m_ItemBaseArray[i].Durability,
			m_ItemBaseArray[i].Position, m_ItemBaseArray[i].QuickPosition, m_ItemBaseArray[i].RareIdx, m_ItemBaseArray[i].ItemStatic);
	}
	memset(m_SlotInfoArray, 0, sizeof(SLOTINFO)*m_SlotNum);
	bRet = true;
	bIsMovePack = bRet;
}
void CItemSlot::GetItemInfoAll(ITEMBASE * pItemBaseAll)
{
	memcpy(pItemBaseAll, &m_ItemBaseArray[m_StartAbsPos], sizeof(ITEMBASE)*m_SlotNum);
}
void CItemSlot::SetItemInfoAll(ITEMBASE * pItemBaseAll)
{
	memcpy(&m_ItemBaseArray[m_StartAbsPos], pItemBaseAll, sizeof(ITEMBASE)*m_SlotNum);
}

ERROR_ITEM CItemSlot::InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state)
{
	bIsMovePack = false;
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	if( pItem->Position != absPos )
	{
		pItem->Position = absPos;
	}
	if( !IsEmptyInner(absPos) )	return EI_EXISTED;
	if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) ) return EI_PASSWD;

	if((state & SS_PREINSERT) == FALSE)	
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[absPos].wIconIdx) && m_ItemBaseArray[absPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;

	m_ItemBaseArray[absPos] = *pItem;
	m_SlotInfoArray[absPos].wState = ( state & ~SS_LOCKOMIT );
	m_SlotInfoArray[absPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur,WORD itemstatic, WORD flag, WORD state, DWORD RareDBIdx,DWORD Grow,WORD Abrasion,WORD MaxAbrasion,char * Maker,char *Maker1)
{
	bIsMovePack = false;
	if( !IsPosIn(whatAbsPos) )	return EI_OUTOFPOS;
	if( (flag & UB_ABSPOS) && (position != whatAbsPos) )
	{
		position = whatAbsPos;
	}
	if( !( SS_LOCKOMIT & state ) && IsLock(whatAbsPos) )  return EI_LOCKED;
	if( SS_CHKDBIDX & state )
	{
		if( dwDBIdx != m_ItemBaseArray[whatAbsPos].dwDBIdx )
			return EI_NOTEQUALDATA;
	}
	if( IsPassword(whatAbsPos) ) return EI_PASSWD;

	if(flag & UB_ICONIDX)
	m_ItemBaseArray[whatAbsPos].wIconIdx = wItemIdx;
	if(flag & UB_QABSPOS)

	m_ItemBaseArray[whatAbsPos].QuickPosition = quickPosition;
	if(flag & UB_ABSPOS)
	m_ItemBaseArray[whatAbsPos].Position = position;
	
	if( ITEMMGR->IsDupItem(m_ItemBaseArray[whatAbsPos].wIconIdx) && m_ItemBaseArray[whatAbsPos].Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return EI_NOTEQUALDATA;

	if(flag & UB_DURA)
		m_ItemBaseArray[whatAbsPos].Durability = Dur;
	
	if(flag & UB_RARE)
		m_ItemBaseArray[whatAbsPos].RareIdx = RareDBIdx;


	if(flag & UB_LOCK)
		m_ItemBaseArray[whatAbsPos].ItemStatic = itemstatic;

	if(flag & UB_UNLOCK)
		m_ItemBaseArray[whatAbsPos].ItemStatic = 0; 

	if(flag & UB_GROW)
		m_ItemBaseArray[whatAbsPos].ItemGrow  =Grow; 
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	if(flag&UB_ABRASION)
		m_ItemBaseArray[whatAbsPos].ItemAbrasion=Abrasion;
	if(flag&UB_MAXABRASION)
		m_ItemBaseArray[whatAbsPos].MaxItemAbrasion=MaxAbrasion;
	if (flag&UB_MAKER)
		strcpy(m_ItemBaseArray[whatAbsPos].ItemMaker,Maker);
	if(flag&UB_MAKER1)
		strcpy(m_ItemBaseArray[whatAbsPos].ItemMaker1,Maker1);

	m_SlotInfoArray[whatAbsPos].wState = ( state & ~(SS_LOCKOMIT|SS_CHKDBIDX) );
	m_SlotInfoArray[whatAbsPos].bLock = FALSE;

	return EI_TRUE;
}

ERROR_ITEM CItemSlot::DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state)
{
	bIsMovePack = false;
	if( !IsPosIn(absPos) )	return EI_OUTOFPOS;
	if( IsEmptyInner(absPos) )	return EI_NOTEXIST;
	if( !(absPos >= TP_SHOPITEM_START && absPos < TP_SHOPITEM_END) &&
		!(absPos >= TP_SHOPINVEN_START && absPos < TP_SHOPINVEN_END) )
		if( !( SS_LOCKOMIT & state ) && IsLock(absPos) )  return EI_LOCKED;
	if( IsPassword(absPos) ) return EI_PASSWD;

	if(pItemOut)
		*pItemOut = m_ItemBaseArray[absPos];

	ClearItemBase(m_ItemBaseArray[absPos]);
	ClearSlotInfo(m_SlotInfoArray[absPos]);
	return EI_TRUE;
}

BOOL CItemSlot::IsPosIn(POSTYPE absPos)
{
	if( m_StartAbsPos > absPos || absPos >= m_StartAbsPos + m_SlotNum )
		return FALSE;
	else

		return TRUE;
}
BOOL CItemSlot::IsEmptyInner(POSTYPE absPos)
{
	if( m_SlotInfoArray[absPos].wState != SS_NONE )
		return TRUE;
	else if( m_ItemBaseArray[absPos].dwDBIdx == 0 ) 
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsEmpty(POSTYPE absPos)
{
	if( !m_SlotInfoArray[absPos].bLock && m_SlotInfoArray[absPos].wState == SS_NONE && m_ItemBaseArray[absPos].dwDBIdx == 0)
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsLock(POSTYPE absPos)
{
	if( m_SlotInfoArray[absPos].bLock )
		return TRUE;
	else
		return FALSE;
}
BOOL CItemSlot::IsPassword(POSTYPE absPos)
{
	if( m_SlotInfoArray[absPos].wPassword ) 
		return TRUE;
	else
		return FALSE;
}
void CItemSlot::SetLock(POSTYPE absPos, BOOL val)
{
	m_SlotInfoArray[absPos].bLock = val;
}

MONEYTYPE CItemSlot::GetMoney()
{
	if( !m_pPurse ) return 0;
	return m_pPurse->GetPurseCurMoney();
}

WORD CItemSlot::GetItemCount()
{
	POSTYPE EndPos = m_StartAbsPos + m_SlotNum;
	WORD ItemNum = 0;

	for( WORD i = m_StartAbsPos ; i < EndPos ; ++i )
	{
		if( IsEmpty(i) ) continue;

		++ItemNum;
	}

	return ItemNum;	
}


