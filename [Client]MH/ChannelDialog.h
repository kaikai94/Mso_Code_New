





// ChannelDialog.h: interface for the CChannelDialog class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_)

#define AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include ".\interface\cWindowHeader.h"

#define CHANNEL USINGTON(CChannelDialog)

class cListCtrl;

class CChannelDialog  : public cDialog
{

	int m_BaseChannelIndex;
	cListCtrl * m_pChannelLCtrl;
	BOOL m_bInit;
	int m_SelectRowIdx;

	WORD m_wMoveMapNum;	
	DWORD m_dwChangeMapState;	
	//[过图读秒变量声明][By:十里坡剑神][QQ:112582793][2018/5/1 20:55]
	MSG_DWORD4 moMsg;
	BOOL	  IsMove;
	DWORD	  CurTime;
	DWORD	  MoveTime;
	int		  nMcount;
public:
	CChannelDialog();
	virtual ~CChannelDialog();


	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void Linking();
	void SetChannelList(MSG_CHANNEL_INFO* pInfo);
	virtual void SetActive(BOOL val);

	void SelectChannel(int rowidx);
	void OnConnect();

	void SendMapChannelInfoSYN(WORD wMapNum, DWORD dwState = eMapChange_General);
	void MapChange();
	//[过图读秒函数声明][By:十里坡剑神][QQ:112582793][2018/5/1 20:55]
	void SetMoveMsg(MSG_DWORD4 msg){moMsg = msg;}
	BOOL IsMoveMap(){return IsMove;}
	void SetMove(BOOL val){IsMove = val;}
	void DmoveMap();
	void InitDmove();
};

EXTERNGLOBALTON(CChannelDialog)

#endif // !defined(AFX_CHANNELDIALOG_H__2799AAF3_2A8B_402D_86D4_EB2547D1888B__INCLUDED_)


