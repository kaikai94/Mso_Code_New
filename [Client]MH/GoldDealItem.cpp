#include "stdafx.h"
#include "GoldDealItem.h"
#include "ItemManager.h"
GoldDealItem::GoldDealItem(void)
{
	m_type = WT_DEALITEM;
}

GoldDealItem::~GoldDealItem(void)
{
}
char* GoldDealItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(m_ItemInfo.wItemIndex);
	if(pItemInfo)

		return pItemInfo->ItemName;
	return NULL;

}