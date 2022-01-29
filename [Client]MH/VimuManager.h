




#ifndef _VIMUMANAGER_H
#define _VIMUMANAGER_H


#define VIMUMGR USINGTON(CVimuManager)

class CPlayer;




class CVimuManager 
{


protected:


	BOOL m_bIsVimuing;
	//[Ѻע����][BY:ʮ���½���][QQ:112582793][2019-4-27][17:59:14]
	DWORD m_dwGoldMoney;
public:

	//MAKESINGLETON(CVimuManager);

	CVimuManager();

	virtual ~CVimuManager();

	void Init();
	void ApplyVimu(BOOL bIsGoldMoney = FALSE);		
	void CancelApply();
	void AcceptVimu( BOOL bAccept );
	
	BOOL CanAcceptVimu( CPlayer* pAccepter );

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	

	void SetVimuing( BOOL bVimuing );
	BOOL IsVimuing() { return m_bIsVimuing; }
	//[Ѻע����][BY:ʮ���½���][QQ:112582793][2019-4-27][17:59:06]
	void ApplyVimuWithGold(DWORD dwGold);
	void SetGoldMoney(DWORD dwMoney){m_dwGoldMoney = dwMoney;}
	void AcceptVimuWithGold(BOOL bAccept);
};

EXTERNGLOBALTON(CVimuManager);
#endif


