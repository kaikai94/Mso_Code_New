



// ItemSlot.h: interface for the CItemSlot class.

// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)
#define AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum UPDATE_BIT{ UB_DBIDX = 1, UB_ICONIDX = 2, UB_ABSPOS = 4, UB_QABSPOS = 8, UB_DURA = 16, UB_RARE = 32, UB_LOCK = 64, UB_UNLOCK = 128, UB_GROW = 256, UB_ABRASION = 512, UB_MAXABRASION = 1024, UB_MAKER = 2048, UB_MAKER1 = 4096, UB_ALL = 8192 };//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
enum ERROR_ITEM { EI_TRUE = 0, EI_OUTOFPOS = 1, EI_NOTEQUALDATA, EI_EXISTED, EI_NOTEXIST, EI_LOCKED, EI_PASSWD, EI_NOTENOUGHMONEY, EI_NOSPACE, EI_MAXMONEY };

enum SLOT_STATE { SS_NONE = 0, SS_PREINSERT=1, SS_LOCKOMIT = 2, SS_CHKDBIDX = 4, };

class CPlayer;
class CPurse;

class CItemSlot  
{
	bool bIsMovePack;
public:
	CItemSlot();
	virtual ~CItemSlot();

	virtual void Init(POSTYPE startAbsPos, POSTYPE slotNum, ITEMBASE * pInfo, SLOTINFO * pSlotInfo);
	void Release();
	
	virtual ITEMBASE /*const*/ * GetItemInfoAbs(POSTYPE absPos);

	void GetItemInfoAll(ITEMBASE * pItemBaseAll);
	void SetItemInfoAll(ITEMBASE * pItemBaseAll);
	char * _ToStringSlot();

	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur,WORD itemstatic, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx = 0,DWORD Grow=0,WORD Abrasion=0,WORD MaxAbrasion=0,char * Maker="无",char *Maker1 = "无");
	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);
		
	POSTYPE GetStartPos() { return m_StartAbsPos; }
	POSTYPE GetSlotNum() { return m_SlotNum; }

	inline virtual BOOL IsEmpty(POSTYPE absPos);

	CPurse*	GetPurse() { return m_pPurse; }
	MONEYTYPE GetMoney();

	BOOL CreatePurse( CPurse* pNewPurse, void* pOwner, MONEYTYPE money, MONEYTYPE max = MAXMONEY );

	void MovePack(DWORD dwID,bool& bRet);

	virtual BOOL IsAddable(POSTYPE absPos, ITEMBASE * pItemBase){	return TRUE;	}
	virtual WORD GetEmptyCell(WORD * EmptyCellPos, WORD NeedCount = 1) { return 0; }
	virtual void SetLock(POSTYPE absPos, BOOL val);
	virtual BOOL IsLock(POSTYPE absPos);

	virtual WORD GetItemCount();

	virtual BOOL CheckQPosForItemIdx( WORD ItemIdx )		{	return TRUE;	}
	virtual BOOL CheckItemLockForItemIdx( WORD ItemIdx )	{	return TRUE;	}

protected:
	virtual inline BOOL IsEmptyInner(POSTYPE absPos);
	virtual inline BOOL IsPosIn(POSTYPE absPos);
	virtual inline BOOL IsPassword(POSTYPE absPos);

	ITEMBASE *		m_ItemBaseArray;
	SLOTINFO *		m_SlotInfoArray;
	POSTYPE			m_StartAbsPos;
	POSTYPE			m_SlotNum;						
	CPurse*			m_pPurse;
};

#endif // !defined(AFX_ITEMSLOT_H__8056E436_4AA0_474D_AFFB_BA0ED84B6697__INCLUDED_)


