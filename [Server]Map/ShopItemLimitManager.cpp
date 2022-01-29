#include "stdafx.h"
#include "ShopItemLimitManager.h"
#include "MapDBMsgParser.h"

ShopItemLimitManager::ShopItemLimitManager(void)
{
}

ShopItemLimitManager::~ShopItemLimitManager(void)
{

}

BOOL ShopItemLimitManager::CanBuyItem(DWORD dwType)
{
	stTIME Time;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	Time.SetTime(sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return Time.value < dwType;
}