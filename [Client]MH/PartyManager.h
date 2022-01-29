



// PartyManager.h: interface for the CPartyManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _CPARTYMANAGER_H_
#define _CPARTYMANAGER_H_
#pragma once

#include "Party.h"
#include <map>

#define PARTYMGR USINGTON(CPartyManager)
struct VOICEMEMBER
{
	DWORD MemberID;
	DWORD CharID;
	std::string m_CharName;
};
class CPartyManager 
{
	CParty m_Party;
	void SetPartyInfo(PARTY_INFO* pmsg);
	DWORD m_RequestPartyID;

	BOOL m_bIsProcessing;

	PARTYEVENT m_PartyEventRate[ePartyEvent_Max];

	DWORD m_dwRequestPlayerID;
	std::map<DWORD,VOICEMEMBER> vc_MemberInfo;
public:
	CPartyManager();
	virtual ~CPartyManager();

	void Init();
	void NetworkMsgParse(BYTE Protocol,void* pMsg);	
	void AddPartyMemberSyn(DWORD AddPartymemeberID); 
	void DelPartyMemberSyn();
	void BanPartyMemberSyn(DWORD DelPartymemberID);
	void ChangeMasterPartySyn(DWORD FromMemberID, DWORD ToMemberID);
	void BreakUpSyn();
	void PartyInviteAccept();
	void PartyInviteDeny();

	void SetRequestPartyID(DWORD id)		{ m_RequestPartyID = id;	}
	DWORD GetRequestPartyID()				{ return m_RequestPartyID;	}

	void CreatePartySyn(PARTY_ADDOPTION* pAddOption);
	void CreatePartyResult(PARTY_ADDOPTION* pMsg);	

	void AddPartyMemberResult(SEND_PARTY_MEMBER_INFO* pMsg); 
	void DelPartyMemberResult(MSG_DWORD* pMsg);
	void BanPartyMemberResult(MSG_DWORD* pMsg);
	void ChangeMasterPartyResult(MSG_DWORD* pMsg);
	void BreakUpPartyResult();


	void UserLogIn(SEND_PARTY_MEMBER_INFO* pMsg);
	void UserLogOut(MSG_DWORD* pMsg);
	
	void SetPartyMemberLifePercent(DWORD PlayerID, DWORD LifePercent);
	BYTE GetPartyMemberLifePercent(DWORD PlayerID);


	void SetPartyMemberShieldPercent(DWORD PlayerID, DWORD ShieldPercent);
	BYTE GetPartyMemberShieldPercent(DWORD PlayerID);
	
	void SetPartyMemberNaeRyukPercent(DWORD PlayerID, DWORD NaeryukPercent);
	BYTE GetPartyMemberNaeRyukPercent(DWORD PlayerID);
	
	void SetPartyMemberLevel(DWORD PlayerID, LEVELTYPE lvl);
	LEVELTYPE GetPartyMemberLevel(DWORD PlayerID);


	PARTY_MEMBER* GetPartyMemberInfo(int i);

	DWORD GetMasterID()		{	return m_Party.GetMasterIdx();	}
	
	char* GetPartyMemberName(DWORD PlayerID) { return m_Party.GetMemberName(PlayerID); }
	DWORD GetPartyMemberID(int n) { return m_Party.GetMemberID(n); }


	BOOL IsMemberLogIn(int n)	{	return m_Party.IsLogIn(n);	}
		
	DWORD GetTacticObjectID()	{	return m_Party.GetTacticObjectID();	}
	void SetTacticObjectID(DWORD TObjID)	{	m_Party.SetTacticObjectID(TObjID);	}
	
	BOOL IsPartyMember(DWORD PlayerID);
	BOOL PartyChat(char* ChatMsg, char* SenderName);
	BOOL CanActivate();
	
	void SetIsProcessing(BOOL val) { m_bIsProcessing = val;	}
	BOOL IsProcessing() { return m_bIsProcessing;	}

	void LoadPartyEventInfo();
	void RefreshPartyEventBuffDesc();
    DWORD GetPartyEventRate( DWORD Index );

	void SetRequestPlayerID(DWORD dwPlayerID) { m_dwRequestPlayerID = dwPlayerID; }
	void PartyRequestResult(BOOL bConsent);
	void PartyError(DWORD dwkind);		
	void PartyDecition(MSG_NAME_DWORD* pMsg);
	void RefleshPartyMatchingList(LEVELTYPE wLevel);
#ifdef _XDYY_
	//[С������][By:ʮ���½���][QQ:112582793][2019/3/21][17:03:14]
	void DisableMemberVoice(DWORD CharID);
	void OpenMemberVoice(DWORD CharID);
	void SetMemberVoiceBtn(DWORD memberID);
#endif
private:
	BOOL m_MasterChanging;

private:
	void SetMasterChanging(BOOL val) { m_MasterChanging = val;	}
	BOOL IsMasterChanging();
};
EXTERNGLOBALTON(CPartyManager);

#endif


