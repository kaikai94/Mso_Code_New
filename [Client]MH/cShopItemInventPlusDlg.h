#pragma once
#include "./interface/cIconGridDialog.h"
class CItem;
class cShopItemInventPlusDlg :public cIconGridDialog
{
public:
	cShopItemInventPlusDlg(void);
	~cShopItemInventPlusDlg(void);
	
	BOOL AddItem(CItem* pItem);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);
	POSTYPE GetRelativePosition( POSTYPE absPos);
	BOOL InPt(POSTYPE absPos);
	BOOL DeleteItem(POSTYPE Pos,CItem** ppItem);

	BOOL FakeMoveItem(LONG MouseX, LONG MouseY, CItem* pSrcItem);
	CItem* GetItemForPos(POSTYPE absPos);
	BOOL FakeGeneralItemMove(POSTYPE ToPos, CItem * pFromItem, CItem * pToItem);
	BOOL CanBeMoved(CItem* pItem,POSTYPE pos);

	BOOL GetBlankPositionRestrictRef(WORD& absPos);

	BOOL GetBlankNum();
};
