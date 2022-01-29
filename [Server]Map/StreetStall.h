




#ifndef __STREETSTALL__
#define __STREETSTALL__


#define MAX_STREETSTALL_CELLNUM 25
#define MAX_STREETBUYSTALL_CELLNUM 5

enum STALL_KIND
{
	eSK_NULL,
	eSK_SELL,
	eSK_BUY,
};

//#define DEFAULT_USABLE_INVENTORY	3

struct sCELLINFO 
{

	void Init()

	{
		wVolume = 0;
		dwMoney = 0;
		bLock = FALSE;
		bFill = FALSE;
		memset(&sItemBase, 0, sizeof(ITEMBASE));
	}

	ITEMBASE	sItemBase;
	MONEYTYPE		dwMoney;
	MONEYTYPE		dwGold;
	WORD		wVolume;
	BOOL		bLock;
	BOOL		bFill;
};

class CPlayer;


class cStreetStall 
{

protected:
	sCELLINFO	m_sArticles[MAX_STREETSTALL_CELLNUM];		
	CPlayer*	m_pOwner;									
	cPtrList	m_GuestList;							
	int			m_nCurRegistItemNum;
	WORD		m_wStallKind;
	MONEYTYPE		m_nTotalMoney;

	MONEYTYPE       m_nTotalGold;

public:
	cStreetStall();
	virtual ~cStreetStall();
	void Init();

	MONEYTYPE	GetTotalMoney() { return m_nTotalMoney; };
	MONEYTYPE   GetTotalGold(){ return m_nTotalGold; }
	BOOL FillCell(ITEMBASE* pBase, MONEYTYPE money, MONEYTYPE gold, BOOL bLock = FALSE, DWORD Volume = 0, WORD wAbsPosition = 0);
	void EmptyCell( ITEMBASE* pBase, eITEMTABLE tableIdx );
	void EmptyCell( POSTYPE pos );
	void EmptyCellAll();
	void ChangeCellState( WORD pos, BOOL bLock );

	void UpdateCell( WORD pos, DURTYPE dur );


	void SetMoney(WORD pos, MONEYTYPE money);

	void SetGold(WORD pos, MONEYTYPE gold);

	void SetVolume( WORD pos, WORD Volume );

	WORD GetStallKind() { return m_wStallKind; }
	void SetStallKind(WORD wStallKind) { m_wStallKind = wStallKind; }

	CPlayer* GetOwner() { return m_pOwner; }
	void SetOwner( CPlayer* pOwner ) { m_pOwner = pOwner; }
	
	void GetStreetStallInfo( STREETSTALL_INFO& stall );
	sCELLINFO* GetCellInfo( POSTYPE pos ) { return &m_sArticles[pos];}

	void AddGuest( CPlayer* pGuest );
	void DeleteGuest( CPlayer* pGuest );
	void DeleteGuestAll();
	void SendMsgGuestAll( MSGBASE* pMsg, int nMsgLen, BOOL bChangeState = FALSE );

	int GetCurRegistItemNum() { return m_nCurRegistItemNum; }
	
	BOOL IsFull();

	BOOL CheckItemDBIdx(DWORD idx);
	BOOL CheckItemIdx(DWORD idx);
};


#endif //__STREETSTALL__


