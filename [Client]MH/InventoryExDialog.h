// InventoryExDialog.h: interface for the CInventoryExDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_)
#define AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cTabDialog.h"
#include "WearedExDialog.h"
#include "ItemShopInven.h"
#include "cShopItemInventPlusDlg.h"
#include <vector>
class CWearedExDialog;
class CItem;
class CMugongBase;
class CDealItem;
class cStatic;
class cMsgBox;
class cPushupButton;
//[背包界面管理类][By:十里坡剑神][QQ:112582793][2017/12/6]
enum { INV=0, WEAR=1, SHOPIVN=2,SHOPIVNPLUS=3, };
enum eINVENTORY_STATE { eITEMDEFAULT_STATE, eITEMUPGRADE_STATE, eITEMDEAL_STATE, };
class CInventoryExDialog : public cTabDialog  
{
	CItem*		m_pSelectedShopItem;
	DWORD m_CanMoveTimeCount;
	REVIVEDATA	m_ReviveData;
	MSG_SHOPITEM_USE_SYN m_UseMsg;
	cStatic * p_Money;
	WORD	m_dWidth;
public:
	CInventoryExDialog();
	virtual ~CInventoryExDialog();
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	BOOL	AddItem(ITEMBASE * itemBase);
	BOOL	AddItem(CItem* pItem);
	BOOL	DeleteItem(POSTYPE Pos,CItem** ppItem);
	void	MoveItem(POSTYPE FromPos, POSTYPE ToPos);
	virtual BOOL FakeMoveIcon(LONG x, LONG y, cIcon * icon);

	BOOL	GetBlankPositionRestrictRef(WORD & absPos);
	BOOL	CheckItemLockForItemIdx(WORD wItemIdx);

	void	Linking();
	BOOL	GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos);
	
	BOOL	IsExist(POSTYPE abs_pos);
	BOOL	CanBeMoved(CItem* pItem,POSTYPE pos);
	
	void	OnActionEvnet(LONG lId, void * p, DWORD we);
	

	void	UseItem(CItem* pItem);

	CItem * GetItemForPos(POSTYPE absPos);
	CItem * GetCurSelectedItem(BYTE type);								
	BOOL	UpdateItemDurabilityAdd(POSTYPE absPos, int dur);				
	void	SetState(eINVENTORY_STATE state){ m_iInventoryState = state; }
	WORD	GetTabIndex(POSTYPE absPos);
	
	int		GetBlankNum();
	void	ReleaseInventory();

	virtual void SetActive(BOOL val);


	DURTYPE	GetTotalItemDurability(WORD wItemIdx);	
	CItem * GetItemLike(WORD wItemIdx);
	void GetItemForCallPet(DWORD dwDBIdx);

	void OnShopItemBtn();
	void OffShopItemBtn(LONG lId);
	void ItemUnSealingSync();
	
	void ItemUnSealing(DWORD absPos);
	void SetReviveData( DWORD ID, DWORD ItemIdx, DWORD ItemPos )
	{	m_ReviveData.TargetID = ID;		m_ReviveData.ItemIdx = (WORD)ItemIdx;		m_ReviveData.ItemPos = (POSTYPE)ItemPos;		}
	REVIVEDATA GetReviveData()			{	return m_ReviveData;	}
	void SetShopItemInvenBtn( BOOL val );
	CItem*	GetSelectedShopItem()		{	return m_pSelectedShopItem;	}
	void ResetSelectedShopItem()		{	m_pSelectedShopItem = NULL;	}
	CItemShopInven* GetShopInven()		{	return m_pItemShopInven;	}
	cShopItemInventPlusDlg* GetShopInvenPlus(){return m_pItemShopInvenPlus;}
	void SetShopInvenPlus(cShopItemInventPlusDlg* pDlg){m_pItemShopInvenPlus = pDlg;}
	BOOL	FakeMoveItem(CItem * pFromItem,WORD ToPos);	//[背包整理函数重载][By:十里坡剑神][QQ:112582793][2018/2/2]
	void GetEmptyNum(BYTE type, std::vector<BYTE>& v);	//[获取空白背包格子][BY:十里坡剑神][QQ:112582793][2019-4-8][9:16:49]
	void SetDivideMsg();
	//[右键仓储][BY:十里坡剑神][QQ:112582793][2019-4-16][20:50:32]
	void GetItemFromPyoguk(CItem* pItem);
	void GetItemFromItemShop(CItem* pItem);
private:

	void	FakeBuyItem(LONG mouseX, LONG mouseY, CDealItem * icon);
	BOOL	IsDivideFunctionKey();
	BOOL	FakeMoveItem(LONG mouseX, LONG mouseY, CItem * icon);				
	BOOL	FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	BOOL	FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem );
	static void OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );
	static void OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 );

	CWearedExDialog		* m_pWearedDlg;
	CItemShopInven*		m_pItemShopInven;
	cPushupButton*		m_pItemShopBtn;
	cShopItemInventPlusDlg* m_pItemShopInvenPlus;
	BOOL				m_bShopInven;

	BOOL				m_fSpread;
	eINVENTORY_STATE	m_iInventoryState;


	MSG_ITEM_DIVIDE_SYN	m_divideMsg;

	DWORD	m_BtnPushstartTime;	
	DWORD	m_BtnPushDelayTime;
	BOOL	m_bPushTabWithMouseOver;

public:
	cMsgBox*			m_pQuestMsgBox;
	CItem*				m_pTempQuestItem;
	void	ItemUseForQuestStart();

	CItem*	GetPriorityItemForCristmasEvent();	
	DWORD	GetPriorityItemIdx();
	//[自动解除封印重载][By:十里坡剑神][QQ:112582793][2017/12/6]
	void ItemUnSealingSync(DWORD absPos);
	CItem * GetItemLike(WORD wItemIdx,WORD &OutPos);
	void SendUseDF();
	void ClearDFMsg();
	//[背包整理限制时间][By:十里坡剑神][QQ:112582793][2018/6/11]
	BOOL IsCanMovePack(WORD & Time);
	void SetMovePackTime(){m_CanMoveTimeCount = gCurTime;}
	void SetMoney(MONEYTYPE Val);
/******************************华丽的分界线******************************/
	void UseItem_InheritItemUse(CItem* pItem);
};

#endif // !defined(AFX_INVENTORYEXDIALOG_H__6F0EE0F4_67B8_4D9B_9988_6E5ABD8BA7FB__INCLUDED_)
														


