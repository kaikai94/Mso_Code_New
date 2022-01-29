




// ShopItemManager.h: interface for the CShopItemManager class.
//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)
#define AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CPlayer;
struct ITEM_INFO;



class CShopItemManager  
{
	CYHHashTable<SHOPITEMWITHTIME>			m_UsingItemTable;
	CMemoryPoolTempl<SHOPITEMWITHTIME>*		m_UsingItemPool;
	CYHHashTable<MOVEDATA>					m_MovePointTable;
	CMemoryPoolTempl<MOVEDATA>*				m_MovePointPool;

	CPlayer*	m_pPlayer;
	DWORD		m_Checktime;
	DWORD		m_Updatetime;

	//
	DWORD		m_DupIncantation;
	DWORD		m_DupCharm;
	DWORD		m_DupHerb;
	DWORD		m_DupSundries;
	DWORD		m_DupPetEquip;
	DWORD		m_ProtectItemIdx;
private:	
	void AddUsingShopItem(SHOPITEMWITHTIME* pShopItem, WORD dwItemIndex);
public:
	CShopItemManager();
	virtual ~CShopItemManager();

	void Init(CPlayer* pPlayer);
	void Release();
	void CheckEndTime(BOOL IsCheck=FALSE);
	int UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem );
	BOOL UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime );
	BOOL CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param=0 );

	void DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete=FALSE );
	void DeleteUsingShopItemInfo( DWORD ItemIdx );
	void SendMsgDwordToPlayer( DWORD Msg, DWORD Param );
	void SendShopItemUseInfo();
	void UpdateLogoutToDB();

	BOOL AddMovePoint( MOVEDATA* pData );
	BOOL DeleteMovePoint( DWORD DBIdx );
	BOOL ReNameMovePoint( DWORD DBIdx, char* newName );
	void ReleseMovePoint();

	SHOPITEMWITHTIME* GetUsingItemInfo( DWORD ItemIdx );

	DWORD GetSavedMPNum()		{	return m_MovePointTable.GetDataNum();	}

	BOOL PutOnAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	BOOL TakeOffAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	void AddShopItemUse( SHOPITEMBASE* pShopItem );
	void DiscardAvatarItem( WORD ItemIdx, WORD ItemPos );
	void CheckAvatarEndtime();

	void CalcAvatarOption( BOOL bCalcStats=TRUE );
	void CalcPlusTime( DWORD dwEventIdx, DWORD dwType );

	void AddDupParam( ITEM_INFO* pItemInfo );
	void DeleteDupParam( ITEM_INFO* pItemInfo );
	BOOL IsDupAble( ITEM_INFO* pItemInfo );

	DWORD GetProtectItemIdx()					{	return m_ProtectItemIdx;	}
	void SetProtectItemIdx( DWORD ItemIdx )		{	m_ProtectItemIdx = ItemIdx;		}

	
	DWORD PutSkinSelectItem(MSG_DWORD2* pMsg);	
	void DiscardSkinItem(DWORD dwItemIndex);	
	void RemoveEquipSkin(DWORD dwSkinKind);			

	CYHHashTable<SHOPITEMWITHTIME> * GetUsingItemInfoAll();  // 2014-11-10  获取角色增益信息
	BOOL IsCanInPutFBMap();  //副本进入物品是否使用检查	by:十里坡剑神	QQ:112582793
	BOOL IsUseGrowItem(WORD & wVal);	 //成长符咒使用检查	by:十里坡剑神	QQ:112582793
	bool AddFBItemTime(WORD wItemIdx,DWORD dwTime);
};





#endif // !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)


