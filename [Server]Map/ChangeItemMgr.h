// ChangeItemMgr.h: interface for the CChangeItemMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)
#define AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_

#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000

#define CHANGEITEMMGR CChangeItemMgr::GetInstance()



#define MAX_CHANGE_RATE	30001

class CChangeItemMgr  
{

	CYHHashTable<sCHANGEITEM>		m_ChangeItemList;
	CYHHashTable<sMULTICHANGEITEM>	m_MultiChangeItemList;	
	
public:
	MAKESINGLETON( CChangeItemMgr );
	CChangeItemMgr();
	virtual ~CChangeItemMgr();

	void	Init();
	void	Release();
	BOOL	LoadItemChange();

	int			 UseChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx );	
	int			 UseNormalChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem );
	int			 UseMultiChangeItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx, ITEMBASE* pItem );

	unsigned int ChangedTotalItemNum(sMULTICHANGEITEM* pMultiChangeItem,WORD wMaxSet);

	int		UseChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx );
	int		UseNormalChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx );
	int		UseMultiChangeItemFromShopItem( CPlayer* pPlayer, WORD wItemIdx );

	sCHANGEITEMUNIT*	GetItemUnitFromCalPercent( WORD wItemIdx );
	sCHANGEITEMUNIT*	GetMultiItemUnitFromCalPercent( sCHANGEITEM* pSet );
	sCHANGEITEMUNIT*    GetMaxSpaceItemRef(sCHANGEITEM* pSet , DWORD& nMin );
	//[箱子物品是否加锁][By:十里坡剑神][QQ:112582793][2018/1/5]
	WORD GetToItemTypeForNormal(WORD wItemIdx);
	//[箱子钥匙][By:十里坡剑神][QQ:112582793][2018/8/17]
	BOOL CheckBoxKey(WORD wCurNeedBoxKey, CPlayer* pPlayer,WORD &wKeyItemPos,DWORD & dwKeyItemIdx);
	WORD BoxKeyType(DWORD dwItemIdx);
};

#endif // !defined(AFX_CHANGEITEMMGR_H__94A4A82B_1B66_44EE_9965_C0BAFC2B306C__INCLUDED_)


