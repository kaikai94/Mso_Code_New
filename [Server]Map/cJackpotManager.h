



// cJackpotManager.h: interface for the cJackpotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_)
#define AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PROBABILITY_LIMIT	100000000

#define JACKPOTMGR	USINGTON(cJackpotManager)

enum ePrizeKind{ NO_PRIZE = -1, FST_PRIZE, SND_PRIZE, THR_PRIZE, FTH_PRIZE, NUM_PRIZE };
enum {EXCP_MAP_COUNT = 8};

struct stPrize
{
	stPrize():m_dwProbability(0),m_dwPercent(0),AbsMoney(0){};
	DWORD	m_dwProbability;
	DWORD	m_dwPercent;	
	DWORD	AbsMoney;
};


class cJackpotManager  
{
	DWORD	m_dwTotalJPMoney;	
	DWORD	m_dwMapJPMoney;
	DWORD	m_dwPrizeMoney;	
	DWORD	m_dwPercentage;


	CPlayer*	m_pPlayer;
	CMonster*	m_pMonster;

	int		m_nPrizeKind;
	
	DWORD	m_dwChipPerMon;	
	DWORD	m_dwUpDateTimeLen;	
	DWORD	m_dwLastDBUpdateTime;	
	stPrize	m_stPrize[NUM_PRIZE];	

	BOOL	m_bDoPrize;	
	BOOL	m_bIsAppMon;	
	BOOL	m_bIsAppMap;	

public:
	cJackpotManager();
	virtual ~cJackpotManager();


	BOOL	IsAppMap(WORD MapNum);
	BOOL	IsAppMon(CPlayer* pPlayer, CMonster* pMonster);
	BOOL	IsUserIn();
	BOOL	GetIsAppMap() {return m_bIsAppMap;}
	void	SetTotalMoney(DWORD TotalMoney) {m_dwTotalJPMoney = TotalMoney;}
	void	SetPrizeOnOff(BOOL bOn) {m_bDoPrize = bOn;}
	void	SetChipPerMon(DWORD Chip) {m_dwChipPerMon = Chip;}
	void	SetPrizeProb(DWORD PrizeKind, DWORD Prob, DWORD Percent)
	{
		m_stPrize[PrizeKind].m_dwProbability = Prob;
		m_stPrize[PrizeKind].m_dwPercent = Percent;
	}
	void	SetDBUpdateTimeLen(DWORD TimeLength) {m_dwUpDateTimeLen = TimeLength;}

	BOOL	LoadJackpotInfo();
	void	Process();
	DWORD	SetProbToDword(DWORD* atom, DWORD* denomi);
	DWORD	MakeRndValue();
	int		CheckPrize(int kind);
	int		CheckPrize();

	void	DoJackpot(CPlayer* pPlayer, CMonster* pMonster);
	void	SendMsgDBMapMoney();
	void	SendMsgDBPrizeInfo(CPlayer* pPlayer, int PrizeKind);
	void	SendMsgAllUserPrizeNotify(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind );
	void	SetPrizeInfo(DWORD TotalMoney, DWORD PrizeMoney, DWORD PlayerID, DWORD PrizeKind);
	DWORD	CalcAddProb(int kind);

	void	Init(WORD Mapnum);

	void	Release();
};

EXTERNGLOBALTON(cJackpotManager)
#endif // !defined(AFX_CJACKPOTMANAGER_H__8F994787_2618_43B5_B144_32070D18B413__INCLUDED_)



