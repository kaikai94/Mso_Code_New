// GMPowerList.h: interface for the CGMPowerList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_)
#define AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GMINFO	USINGTON(CGMPowerList)

struct GM_INFO
{
	DWORD	dwConnectionIndex;
	DWORD	dwGMIndex;
	char	szGMID[MAX_NAME_LENGTH+1];
	int		nPower;
};

class CGMPowerList  
{
protected:
	CYHHashTable<GM_INFO> m_CheatUserList;	//치트 사용가능한 사용자 체크	

public:
	CGMPowerList();
	virtual ~CGMPowerList();	

	void AddCheatUserList( GM_INFO* pGMInfo );
	void RemoveCheatUserList();
	GM_INFO* GetCheatUserInfo(DWORD dwUserIdx);
};

EXTERNGLOBALTON(CGMPowerList);
#endif // !defined(AFX_GMPOWERLIST_H__1298A9AD_BA4C_43FC_AB80_75DB880CA4A1__INCLUDED_)
