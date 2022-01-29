#pragma once
#include "ItemManager.h"
#define BOBUSANG_NPCIDX	74
#define BOBUSANG_wNpcUniqueIdx	300	
#define AutoClearItmeSell		301
#define BOBUSANGMGR	USINGTON(BobusangManager)


class CNpc;

struct BOBUSANGTOTALINFO
{
	BOBUSANGTOTALINFO() : pBobusang(NULL), pDealItemInfo(NULL) {};	
	BOBUSANGINFO		AppearanceInfo;
	CNpc*				pBobusang;

	DealerData*			pDealItemInfo;
	cPtrList			SellingItemList;
	cPtrList			pCustomerList;
};

class BobusangManager
{
	BOBUSANGTOTALINFO*	m_pBobusang;


public:
	BobusangManager(void);
	~BobusangManager(void);

	void	BobusangMgr_Init();
	void	BobusangMgr_Process();
	void	BobusangMgr_Release();

	BOOL	MakeNewBobusangNpc(BOBUSANGINFO* pBobusangInfo);

	BOOL	RemoveBobusangNpc(DWORD channelNum);

	void	SetBobusanInfo(BOBUSANGINFO* pBobusangInfo);
	BOBUSANGTOTALINFO* GetBobusang(DWORD mapChannelNum);
	int		GetBobusangSellingRt(DWORD channelNum, DealerItem* pItemList);

	void	AddGuest(CPlayer* pPlayer);
	void	LeaveGuest(CPlayer* pPlayer);
	void	SendLeaveGuestMsg(CPlayer* pPlayer);

	BOOL	BuyItem(CPlayer* pPlayer, WORD buyItemIdx, WORD BuyItemNum);

	DealerItem*	GetSellingItem(cPtrList* pSellingItemList, WORD buyItemIdx);

	void	AppearBobusang(MSG_BOBUSANG_INFO* pmsg);
	void	DisappearBobusang(MSG_DWORD2* pmsg);

	BOOL	CheckHackBobusangDist(CPlayer* pGuest);

	void	SendDealerItemMsgToGuestAll(DWORD channelNum, MSGBASE* pmsg, int size);
	void	NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
};

EXTERNGLOBALTON(BobusangManager)


