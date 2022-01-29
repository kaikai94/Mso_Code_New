




// ServerConnect.h: interface for the CServerConnect class.
//
//////////////////////////////////////////////////////////////////////



#if !defined(AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_)
#define AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_



#if _MSC_VER > 1000

#pragma once
#endif // _MSC_VER > 1000


#include "GameState.h"
#include "cImageSelf.h"
#define SERVERCONNECT USINGTON(CServerConnect)

class CServerConnect : public CGameState  
{
protected:
	cImageSelf m_imageLoginBarTop;
	cImageSelf m_imageLoginBarBottom;
	cImageSelf m_imageLoginBackGround;
public:
	//MAKESINGLETON(CServerConnect)

	CServerConnect();
	virtual ~CServerConnect();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	void Process();

	void BeforeRender();
	void AfterRender();
	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
};
EXTERNGLOBALTON(CServerConnect);
#endif // !defined(AFX_SERVERCONNECT_H__32D22D75_F2D1_4C03_A002_232AFB60C22A__INCLUDED_)


