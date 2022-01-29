#pragma once

#include "..\[CC]Header\GameResourceStruct.h"

#define SHOPITEMLIMITMGR ShopItemLimitManager::GetInstance()

class ShopItemLimitManager
{
public:
	ShopItemLimitManager(void);
	~ShopItemLimitManager(void);
	GETINSTANCE(ShopItemLimitManager);
	BOOL CanBuyItem(DWORD dwType);
};
