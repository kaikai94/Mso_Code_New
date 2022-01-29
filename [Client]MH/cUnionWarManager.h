#pragma once
#include "./interface/cDialog.h"
#include "./interface/cStatic.h"
#include "./interface/cListCtrl.h"

class cTextArea;
class cUnionWarManager :
	public cDialog
{
	
	cStatic* m_pSPlayerKill;
	cStatic* m_pSPlayerDie;

	cListCtrl* m_pLUnionWarInfoList;
	
	cTextArea* m_TipMsg;

	DWORD m_dwWarTime;

	eUnionWarState	m_dwWarState;
	DWORD m_dwTimeCount;

public:
	cUnionWarManager();
	~cUnionWarManager();
	
	void Linking();
	virtual void SetActive(BOOL val);
	void ShowUnionWarInfo(DWORD dwKill,DWORD dwDie);
	

	void SetKillTimes(DWORD dwKillTimes);
	void SetDieTimes(DWORD dwDieTimes);
	void UpdateUnionInfo(DWORD dwUnionID,DWORD dwDieTimes,DWORD dwKillTimes);

	void UpdateUnionPlayerCount(DWORD dwUnionID, DWORD dwPlayerCount);

	void SetUnionWarTime(DWORD time){ m_dwWarTime = time; }
	void SetUnionState(eUnionWarState state){ m_dwWarState = state; }

	void UpdateTipMsg();
};
