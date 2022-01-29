



#pragma once
#include "DealDialog.h"

#define BOBUSANG_NPCIDX	74
#define BOBUSANG_wNpcUniqueIdx	300		//"StaticNpcList.bin �� ������"


#define BOBUSANGMGR USINGTON(BobusangManager)
#define SVVMODEMGR USINGTON(CSurvivalModeManager)

#define BOBUSANG_POSNUM_MAX	4	// �ʺ� ���� �ִ� ��ġ����
#define DEALITEM_BIN_TABNUM	7	// deallist.bin ���� �� ���� // ���λ��� ���� �Ѱ��� �����ϰ� �Ǹ��Ѵ�.


#define BOBUSANG_ITEM_TICKET	8014


enum bobusangAppearedState{
	eBBSAS_NONE = -1,
	eBBSAS_DISAPPEAR			= 0,	// ���� ����

	eBBSAS_APPEAR				= 2,	// ���� ����
	eBBSAS_APPEAR_DELAYED		= 4,	// ���� �޽����� �� ���� ����
	eBBSAS_DISAPPEAR_DELAYED	= 8,	// ���� �޽����� �� ���� ����
	eBBSAS_TIME_DELAYED			= 16,	// �ð� üũ �ʰ�
};
#define BBS_STATE_KINDMAX	6

enum BobusangValidityData{
	BVD_NONE = 0,
	BVD_MAP = 1,
	BVD_CNL = 2,
	BVD_POS = 4,
	BVD_SEL = 8,
	BVD_APR = 16,
	BVD_DSAPR = 32,

	BVD_ALL = BVD_MAP|BVD_CNL|BVD_POS|BVD_SEL|BVD_APR|BVD_DSAPR,
};

class BobusangManager
{
	DealerData		m_Dealer;
	BOOL			m_bOnDealWithBobusang;
	//DealerItem		m_SellingItem[SLOT_NPCINVEN_NUM];

public:
	BobusangManager(void);

	~BobusangManager(void);

	void	InitDealItemList();
	void	InitDealItemList(DealerItem* pItem, DWORD Num);		// ���λ� ��ȭ�� �����κ��� ���� �Ǹ����� ����.
	void	ResetDealItemInfo(DWORD itemIdx, DWORD itemPos, DWORD itemCount);
	DealerData*		GetBobusangDealer()
	{
		return &m_Dealer;
	}

	void	SendEndDealMsg();


	void	NetworkMsgParse(BYTE Protocol, void* pMsg);

	// ���λ� �ŷ� ���¸� ��Ÿ����. �ŷ��� ���λ� NPC ����� ��� ���.
	void	SetOnDealing(BOOL bVal)	{	m_bOnDealWithBobusang = bVal;	}
	BOOL	GetOnDealing()	{	return m_bOnDealWithBobusang;	}

#ifdef _GMTOOL_ 
//for cheat
private:
	BOOL			m_bCheatReady;
	int				m_flgValidity;
	BOBUSANGINFO	m_BobusangInfo;
public:
	void	InitCheatData();
	void	SetCheatReady(BOOL bVal)	{	m_bCheatReady = bVal; }

	BOOL	CheckCheatReady();
	void	SetValidity(int flg)		{	m_flgValidity |= flg;	}

	int		GetValidity()	{	return m_flgValidity;	}

	BOBUSANGINFO*	GetNewBobusangInfo()	{	return &m_BobusangInfo;	}
	void	PrintInfo( BOBUSANGINFO* pInfo, char* pStr = "NEW", int state = eBBSAS_NONE );
#endif
};


EXTERNGLOBALTON(BobusangManager);


