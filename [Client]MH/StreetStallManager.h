



#ifndef __STREETSTALLMANAGER__
#define __STREETSTALLMANAGER__


#include "IndexGenerator.h"

#include "StallKindSelectDlg.h"

#define STREETSTALLMGR USINGTON(CStreetStallManager)

#define STREETSTALL_MAX_LINKITEM 5

class CItem;
class CExchangeItem;
class CBuyItem;
class CObject;
class CStallKindSelectDlg;
class CMapObject;
class CPlayer;

class CStreetStallManager 
{
	CIndexGenerator m_IconIndexCreator;
	WORD m_wStallKind;
protected:
	int m_nLinkedItemNum;	

	BOOL m_bOpenMsgBox;
	char m_strOldTitle[MAX_STREETSTALL_TITLELEN + 1];

	DWORD	m_dwWantBuyNum;
	DWORD	m_dwWantSellNum;

public:

	CStreetStallManager();
	virtual ~CStreetStallManager();
	
	void Init();

	void SetOpenMsgBox(BOOL bOpen) { m_bOpenMsgBox = bOpen; }
	
	BOOL LinkItem( CItem* pItem, ITEMBASE* pBase );

	BOOL LinkItem(POSTYPE pos, ITEMBASE ItemInfo, WORD Volume, MONEYTYPE Money);
	void UnlinkItem( CExchangeItem* pItem );
	void UnlinkItem( CBuyItem* pItem );
	void LinkBuyItem( STREETSTALL_INFO* pStallInfo );
	void LinkSellItem( STREETSTALL_INFO* pStallInfo );
	int GetLinkedItemNum() { return m_nLinkedItemNum; }
	
	void ToggleHero_StreetStallMode( BOOL bOpen );
	void Toggle_StreetStallMode( CObject* pObject, BOOL bStreet);


	void ChangeDialogState();

	void OpenStreetStall();
	void CloseStreetStall();

	void EnterStreetStall( DWORD OwnerId );


	void RegistItemEx(CItem* pItem, DWORD money, MONEYTYPE gold);
	void RegistItemEx(ITEMBASE ItemInfo, WORD wAbsPosition, WORD Volume, MONEYTYPE money);

	void ItemStatus( CExchangeItem* pItem, BOOL bLock );
	void ItemStatus( POSTYPE pos, BOOL bLock );

	void EditTitle();
	void NetworkMsgParse( BYTE Protocol, void* pMsg );

	void BuyItem();
	void BuyItemDur( POSTYPE pos, DWORD dwNum );

	void SendBuyItemMsg();


	BOOL SellItem();
	BOOL SellItemDur( DWORD dwNum );
	
	BOOL SendSellItemMsg();


	WORD GetStallKind() { return m_wStallKind; }
	void SetStallKind(WORD wStallKind) { m_wStallKind = wStallKind; }

	CMapObject* AddStreetStallDecoration(CPlayer* pPlayer, DWORD dwStreetStallKind);
	BOOL IsOpenState();
	void ReleaselItemIndex(DWORD Index){m_IconIndexCreator.ReleaseIndex(Index);}
};
EXTERNGLOBALTON(CStreetStallManager);

#endif //__STREETSTALLMANAGER__


