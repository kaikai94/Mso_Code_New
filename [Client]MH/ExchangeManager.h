#ifndef _EXCHANGEMANAGER_H
#define _EXCHANGEMANAGER_H


#include "IndexGenerator.h"


#define MAX_EXCHANGEITEM	10


#define EXCHANGEMGR USINGTON(CExchangeManager)

class cIcon;
class CItem;
class CExchangeItem;
class CPlayer;



enum eEXCHANGE_ERROR
{
	eEE_OK					= 0,
	eEE_USERCANCEL,
	eEE_USERLOGOUT,
	eEE_USERDIE,	
	eEE_DIE,	
	eEE_NOTENOUGHMONEY,
	eEE_NOTENOUGHSPACE,
	eEE_MAXMONEY,	
	eEE_ERROR,
};


class CExchangeManager
{

protected:

	int					m_nLinkedItemNum;
	CIndexGenerator		m_IconIndexCreator;
	
	BOOL				m_IsExchanging;

	BOOL				m_bLockSyncing;
	BOOL				m_bExchangeSyncing;
	BOOL				m_bInventoryActive;

public:

	CExchangeManager();
	virtual ~CExchangeManager();

	void Init();
	void ExchangeStart();
	void ExchangeEnd();
	BOOL LinkItem( CItem* pItem );
	void UnLinkItem( int nTypeIndex, cIcon* pItem );
	int GetLinkedItemNum() { return m_nLinkedItemNum; }
	
	BOOL ApplyExchange();

	void CancelApply();

	BOOL CanAcceptExchange( CPlayer* pAccepter );

	void AcceptExchange( BOOL bAccept );

	void Lock( BOOL bLock );
	void Exchange();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	BOOL IsLockSyncing()		{ return m_bLockSyncing; }
	BOOL IsExchangeSyncing()	{ return m_bExchangeSyncing; }
	
	BOOL IsExchanging()			{ return m_IsExchanging; }
};


EXTERNGLOBALTON(CExchangeManager)

#endif


