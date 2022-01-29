





#pragma once
#include "itemslot.h"




class CPetWearSlot : public CItemSlot
{

public:
	CPetWearSlot(void);
	virtual ~CPetWearSlot(void);

	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0);

	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);




	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase);

	void ApplyPetEquipItem(CPlayer* pPlayer, DWORD ItemIdx, BOOL bAdd);
};


