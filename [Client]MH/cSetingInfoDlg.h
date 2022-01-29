#pragma once
#include "./interface/cDialog.h"
class cStatic;
//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
class cSetingInfoDlg: public cDialog
{
	cStatic * m_RestTimes;
	cStatic * m_RestLevel;
	cStatic * m_RestOpenTime;
	cStatic * m_SprofitTime;
	cStatic * m_SiegeWarState;
	DWORD m_dwGetInfo;
	WORD  m_wMinRestLevel;
	WORD  m_wRestTimes;
public:
	cSetingInfoDlg(void);
	~cSetingInfoDlg(void);
	void Linking();
	void NetWorkParser(MSGRESTINFO * msg);
	void SetSiegeWarInfo(WORD val);
	void SetAllInfo(MSGRESTINFO * msg);
	void Show(BOOL val);
	void SetRestLevel();
	void SetRestTimesAdd();
	
	WORD GetRestTimes(){return m_wRestTimes;}
	void SetRestOpenTime(DWORD val);
	void SetRestSproFitTime(DWORD val);
};
