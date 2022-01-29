




// StatsCalcManager.h: interface for the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)
#define AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000

#ifdef __MAPSERVER__
#define PLAYERTYPE CPlayer
class CPlayer;
#else
#define PLAYERTYPE CHero
class CHero;
#endif

#include "..\[CC]Header\GameResourceStruct.h"


#define STATSMGR	CStatsCalcManager::GetInstance()


class CStatsCalcManager  
{
	CStatsCalcManager();
public:	
	MAKESINGLETON(CStatsCalcManager);
	virtual ~CStatsCalcManager();
	void CalcItemStats(PLAYERTYPE* pPlayer);	
	void CalcCharStats(PLAYERTYPE* pPlayer);

	void Clear(player_calc_stats * pStats);

	void CalcSetItemStats();

	void CalcUniqueItemStats();
	
	void CalcTitanStats(DWORD dwDBIdx);	
};

#endif // !defined(AFX_STATSCALCMANAGER_H__1BA578DC_5092_4667_8FFC_6E3B15BF7B2E__INCLUDED_)


