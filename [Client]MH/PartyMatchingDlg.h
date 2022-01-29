




#ifndef _PARTY_MATCHING_DLG_H
#define _PARTY_MATCHING_DLG_H

#include "interface/cDialog.h"
#include "interface/cEditBox.h"
#include "interface/cListDialog.h"
#include "GameResourceStruct.h"
#include "../input/Mouse.h"

class CPartyMatchingDlg : public cDialog
{

private:
	enum
	{
		eREFLESH_DELAYTIME = 5000,
	};

private:	
	cEditBox* m_pLimitLevelEdit;
	cListDialog* m_pPartyList;	



	DWORD m_dwSelectIdx;
	DWORD m_dwReflashDelayTime;	

	CYHHashTable<PARTY_MATCHINGLIST_INFO> m_PartyInfoTable;
	PARTY_MATCHINGLIST_INFO pTemp[MAX_PARTYMATCHING_INFO_NUM];
public:

	CPartyMatchingDlg();
	virtual ~CPartyMatchingDlg();


	void Linking();
	virtual void SetActive( BOOL val );
	virtual DWORD ActionEvent(CMouse* mouseInfo);	
	virtual BOOL OnActionEvent(LONG lId, void * p, DWORD we);
	
	void RemovePartyInfoTable();
	void SetPartyMatchingList(MSG_PARTYMATCHING_INFO* pPartyListInfo);	
	void PartyListSort(PARTY_MATCHINGLIST_INFO* pPartyListInfo, WORD wMaxNum);	

	void PartyRandomSelect();

	WORD GetPartyMemberNum(PARTY_MATCHINGLIST_INFO* pPartyInfo);
	BOOL CheckPartyRequest(DWORD dwPartyIDX);	
	void MasterToPartyRequestSyn(DWORD dwPartyIDX, DWORD dwMasterID);
	void SetPartyInfo(PARTY_MATCHINGLIST_INFO* pPartyInfo, PARTY_MATCHINGLIST_INFO Info);
	void StartReflashDelayTime();

	PARTY_MATCHINGLIST_INFO* GetPartyInfo(DWORD dwPartyIDX);
};

#endif _PARTY_MATCHING_DLG_H


