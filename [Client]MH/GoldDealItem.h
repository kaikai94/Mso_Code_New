#pragma once
#include "./BaseItem.h"
class GoldDealItem: public CBaseItem  
{
public:
	GOLD_SHOP_ITEM_INFO m_ItemInfo;
	GoldDealItem(void);
	~GoldDealItem(void);
	char* GetItemName();
	void SetItemInfo(GOLD_SHOP_ITEM_INFO & info)
	{
		memcpy(&m_ItemInfo,&info,sizeof(GOLD_SHOP_ITEM_INFO));
	}
	GOLD_SHOP_ITEM_INFO & GetInfo(){return m_ItemInfo;}
};
