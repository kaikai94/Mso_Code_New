#include "stdafx.h"
#include "PartyManager.h"
#include "Party.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Battle.h"
#include "PackedData.h"

CPartyManager::CPartyManager() 
{
	m_partyRoomHashTable.Initialize(256);	
}

CPartyManager::~CPartyManager() 
{
	void* pInfo;
	m_partyRoomHashTable.SetPositionHead();
	while(pInfo = m_partyRoomHashTable.GetData())
		delete pInfo;
	m_partyRoomHashTable.RemoveAll();
}


CParty* CPartyManager::RegistParty(DWORD PartyIDX)
{
	CParty* pParty = m_partyRoomHashTable.GetData(PartyIDX);

	if(pParty == NULL)
	{
		PARTY_ADDOPTION AddOption;
		memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));
		pParty = new CParty(PartyIDX, 0, "", NULL, &AddOption);
		m_partyRoomHashTable.Add(pParty,PartyIDX);
	}

	ASSERT(pParty);

	return pParty;
}

void CPartyManager::CreatePartyQuery(CPlayer * pMaster, PARTY_ADDOPTION* pAddOption)
{
	DWORD PartyIdx = pMaster->GetPartyIdx();
	if(PartyIdx != 0)
	{
		int errorFlag = 0;
		CParty* pParty = GetParty( PartyIdx );

		if( pParty )
		{
			if( pParty->GetMasterID() == pMaster->GetID() )
			{
				errorFlag = 1;
				BreakupParty( PartyIdx, pMaster->GetID() );
			}
			else if( pParty->IsPartyMember(pMaster->GetID()) )
			{
				errorFlag = 2;
				DelMember( pMaster->GetID(), PartyIdx );
			}
		}
		else
		{
			errorFlag = 3;
		}

		char temp[128] = {0,};
		sprintf(temp, "PartyLogicError(%d)!! :: CharaterID: %d\tPartyIdx: %d\n", errorFlag, pMaster->GetID(), PartyIdx );
		ASSERTMSG(0, temp);

		pMaster->SetPartyIdx(0);
	}		
	PartyCreate(pMaster->GetID(), pAddOption); 
}

void CPartyManager::AddPartyInvite(DWORD MasterID, DWORD TargetID)
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterID);
	if(!pMaster)
		return;
	CParty* pParty = GetParty(pMaster->GetPartyIdx());
	if(!pParty)
	{
		return;
	}

	if(pParty->IsPartyMember(MasterID) == FALSE)
		return;
	if(pParty->IsPartyMember(TargetID) == TRUE)
		return;

	
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_ADD_NACK;
	msg.dwData = 0;

	CPlayer* pTargetPlayer = (CPlayer*)g_pUserTable->FindUser(TargetID);	
	if(!pTargetPlayer)
	{
		msg.dwData = eErr_Add_NoPlayer;
	}
	else if(pTargetPlayer->GetPartyIdx() != 0)
		msg.dwData		= eErr_Add_AlreadyinParty;
	else if(pTargetPlayer->GetGameOption()->bNoParty)
		msg.dwData		= eErr_Add_OptionNoParty;
	else if(pTargetPlayer->GetState() == eObjectState_Die)
		msg.dwData		= eErr_Add_DiedPlayer;	
	else if( pMaster->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
		if(	pTargetPlayer->GetPartyIdx() != pMaster->GetPartyIdx() )
			msg.dwData = eErr_Add_NotOurGuild;
	}	
	if(msg.dwData > 0)
		pMaster->SendMsg(&msg, sizeof(msg));
	else
	{
		MSG_NAME_DWORD2 invitemsg;
		invitemsg.Category = MP_PARTY;
		invitemsg.Protocol = MP_PARTY_ADD_INVITE;
		invitemsg.dwData1	= pParty->GetPartyIdx();
		invitemsg.dwData2	= pParty->GetOption();
		SafeStrCpy(invitemsg.Name,pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
		pTargetPlayer->SendMsg(&invitemsg,sizeof(invitemsg));

		MSGBASE rtmsg;
		rtmsg.Category = MP_PARTY;
		rtmsg.Protocol = MP_PARTY_INVITE_ACCEPT_ACK;
		pMaster->SendMsg(&rtmsg, sizeof(rtmsg));
	}
}

void CPartyManager::AddMember(DWORD PartyID, DWORD TargetPlayerID)
{
	CPlayer* pNewMember = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
	if(!pNewMember)
		return;
	if(pNewMember->GetPartyIdx() != 0) 
	{
		ASSERT(0);
		return;
	}
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		MSG_BYTE msg;
		msg.Category = MP_PARTY;
		msg.Protocol = MP_PARTY_INVITE_ACCEPT_NACK;
		msg.bData = eErr_BrokenParty;
		pNewMember->SendMsg(&msg, sizeof(msg));
		return;
	}
	if(pParty->IsPartyMember(pNewMember->GetID()) == TRUE)
		return;

	PartyAddMember(PartyID,TargetPlayerID); //DB update

	
	AddMemberResult(pParty, pNewMember);
}

void CPartyManager::AddMemberResult(CParty* pParty, CPlayer* pNewMember)
{
	pNewMember->SetPartyIdx(pParty->GetPartyIdx());

	SEND_PARTY_MEMBER_INFO smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_ADD_ACK;
	smsg.MemberInfo.dwMemberID = pNewMember->GetID();	
	SafeStrCpy(smsg.MemberInfo.Name,pNewMember->GetObjectName(), MAX_NAME_LENGTH+1);
	smsg.MemberInfo.bLogged = TRUE;
	smsg.MemberInfo.LifePercent = pNewMember->GetLifePercent();
	smsg.MemberInfo.ShieldPercent = pNewMember->GetShieldPercent();
	smsg.MemberInfo.NaeRyukPercent = pNewMember->GetNaeRyukPercent();
	smsg.MemberInfo.Level = pNewMember->GetLevel();	
	VECTOR3 vPos;
	pNewMember->GetPosition(&vPos);
	smsg.MemberInfo.posX = (WORD)vPos.x;
	smsg.MemberInfo.posZ = (WORD)vPos.z;
	smsg.PartyID = pParty->GetPartyIdx();
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
	
	if(pParty->AddPartyMember(pNewMember->GetID(),pNewMember->GetObjectName(),pNewMember,smsg.MemberInfo.Level))
		NotifyChangesOtherMapServer(pNewMember->GetID(),pParty,MP_PARTY_NOTIFYADD_TO_MAPSERVER, pParty->GetPartyIdx(), smsg.MemberInfo.Level);
}

void CPartyManager::AddMemberNack(DWORD PartyID, DWORD TargetPlayerID)
{
	CParty* pParty = GetParty(PartyID);
	if(!pParty)
		return;
	MSGBASE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INVITE_DENY_ACK;
	pParty->SendMsgToAll(&msg, sizeof(msg));
}

void CPartyManager::NotifyAddParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty ==NULL)
		return;
	
	if(pParty->GetMasterID() == 0) 
		return;

	if(pParty->IsPartyMember(pmsg->dwObjectID) == TRUE)
		return;

	pParty->AddPartyMember(pmsg->dwObjectID,pmsg->Name,NULL, pmsg->Level);

	SEND_PARTY_MEMBER_INFO smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_ADD_ACK;
	smsg.MemberInfo.dwMemberID = pmsg->dwObjectID;
	SafeStrCpy(smsg.MemberInfo.Name,pmsg->Name, MAX_NAME_LENGTH+1);
	smsg.MemberInfo.bLogged = TRUE;
	smsg.MemberInfo.LifePercent = 0;
	smsg.MemberInfo.NaeRyukPercent = 0;
	smsg.MemberInfo.Level = pmsg->Level;
	smsg.PartyID = pmsg->PartyID;

	pParty->SendMsgToAll(&smsg,sizeof(smsg));
}

void CPartyManager::DelMember(DWORD TargetPlayerID,DWORD PartyID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		char buff[512] = {0,};
		sprintf(buff,"[成员踢出失败]队伍不存在，队伍编号:[%u]",PartyID);
		ASSERTMSG(0,buff);
		return;
	}

	
	if(pParty->IsPartyMember(TargetPlayerID) == FALSE)
		return;
	if(pParty->GetMasterID() == 0) 
		return;

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_DEL_ACK;
	smsg.dwData = TargetPlayerID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));

	NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYDELETE_TO_MAPSERVER, PartyID);
	
	pParty->RemovePartyMember(TargetPlayerID);
	
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);
}

void CPartyManager::NotifyDelParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty ==NULL)
		return;
	
	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_DEL_ACK;
	smsg.dwData = pmsg->dwObjectID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
	
	pParty->RemovePartyMember(pmsg->dwObjectID);
	
	CPlayer * pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);
}

void CPartyManager::BanParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID) 
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	DWORD PartyIdx = pPlayer->GetPartyIdx();
	CParty* pParty = GetParty(PartyIdx);
	if(pParty == NULL)
	{

		char buff[512] = {0,};
		sprintf(buff,"[成员踢出失败]队伍不存在，队伍编号:[%u]",PartyID);
		ASSERTMSG(0,buff);
		return;
	}
	if(pParty->IsMasterChanging(pPlayer, MP_PARTY_BAN_NACK) == TRUE)
		return;
	
	if(pParty->IsPartyMember(pPlayer->GetID()) && pParty->IsPartyMember(TargetPlayerID))
	{		
		MSG_DWORD smsg;
		smsg.Category = MP_PARTY;
		smsg.Protocol = MP_PARTY_BAN_ACK;
		smsg.dwData = TargetPlayerID;
		pParty->SendMsgToAll(&smsg,sizeof(smsg));
		
		CharacterTablePartyIdxUpdate(TargetPlayerID); 
		
		CPlayer* pBanedPlayer = (CPlayer *)g_pUserTable->FindUser(TargetPlayerID);
		if(pBanedPlayer)
			pBanedPlayer->SetPartyIdx(0);

		NotifyChangesOtherMapServer(TargetPlayerID,pParty,MP_PARTY_NOTIFYBAN_TO_MAPSERVER, PartyID);

		pParty->RemovePartyMember(TargetPlayerID);	
		PartyDelMember(PartyID, TargetPlayerID);
	}
	else
	{
		char buff[512] = {0,};
		sprintf(buff,"[成员踢出失败]操作成员ID:[%u],被踢出成员ID:[%u],队伍编号:[%u]",pPlayer->GetID(),TargetPlayerID,PartyID);
		ASSERTMSG(0,buff);
	}
}
void CPartyManager::NotifyBanParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0)
		return;
	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_BAN_ACK;
	smsg.dwData = pmsg->dwObjectID;
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
	
	pParty->RemovePartyMember(pmsg->dwObjectID);

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if(pPlayer)
		pPlayer->SetPartyIdx(0);
}
void CPartyManager::ChangeMasterParty(DWORD PartyID,DWORD PlayerID,DWORD TargetPlayerID) 
{
	if(PlayerID == TargetPlayerID)
		return;
	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	CParty* pParty = GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
	{
		char buff[512] = {0,};
		sprintf(buff,"[队长改变失败,队伍不存在],角色ID:[%u],队伍编号:[%u]",pPlayer->GetID(),PartyID);
		ASSERTMSG(0,buff);
		return;
	}

	if(pParty->IsPartyMember(pPlayer->GetID()) == FALSE)
		return;
	if(pParty->IsPartyMember(TargetPlayerID) == FALSE)
		return;
	if(pParty->GetMemberID(0) != pPlayer->GetID())	
		return;
	if(pParty->GetMasterID() == TargetPlayerID)
		return;

	PartyChangeMaster(pPlayer->GetPartyIdx(), PlayerID, TargetPlayerID);
	pParty->SetMasterChanging(TRUE);
}
void CPartyManager::NotifyChangeMasterParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{	
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0) 
		return;
	DoChangeMasterParty(pParty, pmsg->dwObjectID);
}
void CPartyManager::DoChangeMasterParty(CParty* pParty, DWORD TargetID)
{
	pParty->ChangeMaster(pParty->GetMasterID(),TargetID);

	MSG_DWORD smsg;
	smsg.Category = MP_PARTY;
	smsg.Protocol = MP_PARTY_CHANGEMASTER_ACK;
	smsg.dwData = TargetID;
	
	pParty->SendMsgToAll(&smsg,sizeof(smsg));
}
void CPartyManager::BreakupParty(DWORD PartyID,DWORD PlayerID) 
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	if(pPlayer->GetPartyIdx() == 0)
	{
		char buff[512] = {0,};
		sprintf(buff,"队伍解散失败，角色队伍编号为0。角色ID[%u]",PlayerID);
		ASSERTMSG(0,buff);
		return;
	}
		

	CParty* pParty = GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
	{
		char buff[512] = {0,};
		sprintf(buff,"队伍解散失败，不存在的队伍。队伍编号[%u],角色ID[%u]",pPlayer->GetPartyIdx(),PlayerID);
		ASSERTMSG(0,buff);
		pPlayer->SetPartyIdx(0);
		return;
	}

	if(pParty->IsMasterChanging(pPlayer, MP_PARTY_BREAKUP_NACK) == TRUE)
		return;
		
	MSGBASE send_msg;
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_BREAKUP_ACK;
	send_msg.dwObjectID = pPlayer->GetID();

	pParty->SendMsgToAll(&send_msg,sizeof(send_msg));
	
	PartyBreakup(pPlayer->GetPartyIdx());
	
	NotifyChangesOtherMapServer(PlayerID,pParty,MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER, PartyID);
	
    pParty->BreakUp();

	m_partyRoomHashTable.Remove(PartyID);
	delete pParty;
}
#ifdef _XDYY_
void CPartyManager::PartVoiceNotify(MSG_DWORD3* pMsg,int MsgSize)
{
	CParty* pParty = GetParty(pMsg->dwData2);
	if(pParty == NULL)
		return;
	pParty->SendMsgExceptOne(pMsg,MsgSize,pMsg->dwObjectID);
}
#endif
void CPartyManager::NotifyBreakupParty(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{	
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;

	MSGBASE send_msg;
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_BREAKUP_ACK;
		
	pParty->SendMsgToAll(&send_msg,sizeof(send_msg));

	pParty->BreakUp();

	m_partyRoomHashTable.Remove(pmsg->PartyID);
	delete pParty;
}
void CPartyManager::CreatePartyResult(CPlayer * pMaster, PARTY_ADDOPTION* pAddOption)
{
	DWORD dwPartyIDX = pAddOption->dwPartyIndex;

	if(m_partyRoomHashTable.GetData(dwPartyIDX))
	{
		ASSERTMSG(0, "[队伍创建]已经存在的队伍编号");
		return;
	}

	CParty* pParty = new CParty(dwPartyIDX,pMaster->GetID(),pMaster->GetObjectName(),pMaster, pAddOption);

	m_partyRoomHashTable.Add(pParty,dwPartyIDX);

	pMaster->SetPartyIdx(dwPartyIDX);

	PARTY_ADDOPTION send_msg;
	send_msg = (*pAddOption);
	send_msg.Category = MP_PARTY;
	send_msg.Protocol = MP_PARTY_CREATE_ACK;
	send_msg.dwObjectID = pMaster->GetID();	
	pMaster->SendMsg(&send_msg,sizeof(send_msg));

	pParty->SetMaster(pMaster);

	PARTY_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_NOTIFYCREATE_TO_MAPSERVER;
	msg.dwObjectID = pMaster->GetID();
	msg.PartyAddOption = (*pAddOption);
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
	{

		msg.Member[i].bLogged = pParty->m_Member[i].bLogged;
		msg.Member[i].dwMemberID = pParty->m_Member[i].MemberID;
		msg.Member[i].Level = pParty->m_Member[i].Level;
		SafeStrCpy(msg.Member[i].Name, pParty->m_Member[i].Name, MAX_NAME_LENGTH+1);
	}

	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
}

void CPartyManager::NotifyCreateParty(PARTY_INFO* pPartyInfo)
{
	DWORD dwPartyIDX = pPartyInfo->PartyAddOption.dwPartyIndex;	

	if(m_partyRoomHashTable.GetData(dwPartyIDX))
	{
		ASSERTMSG(0, "[队伍创建]已经存在的队伍编号");
		return;
	}

	PARTY_ADDOPTION AddOption;
	memset(&AddOption, 0, sizeof(PARTY_ADDOPTION));
	AddOption = pPartyInfo->PartyAddOption;
	
	DWORD dwMasterID = pPartyInfo->Member[0].dwMemberID;
	char* pMasterName = pPartyInfo->Member[0].Name;
	CParty* pParty = new CParty(dwPartyIDX, dwMasterID, pMasterName, NULL, &AddOption);	

	for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
	{
		pParty->m_Member[i].bLogged = pPartyInfo->Member[i].bLogged;
		pParty->m_Member[i].MemberID = pPartyInfo->Member[i].dwMemberID;
		pParty->m_Member[i].Level = pPartyInfo->Member[i].Level;
		SafeStrCpy(pParty->m_Member[i].Name, pPartyInfo->Member[i].Name, MAX_NAME_LENGTH+1);
	}	

	m_partyRoomHashTable.Add(pParty,dwPartyIDX);	
}

CParty* CPartyManager::GetParty(DWORD partyIDx)
{
	CParty* pParty = (CParty*)m_partyRoomHashTable.GetData(partyIDx);
	return pParty;
}

void CPartyManager::UserLogIn(CPlayer* pPlayer,BOOL bNotifyUserLogin)
{
	DWORD PartyID = pPlayer->GetPartyIdx();
	if(PartyID == 0)
		return;
	
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		PartyInfoByUserLogin(PartyID,pPlayer->GetID());
		return;
	}
	if(pParty->GetMasterID() != 0)
		pParty->UserLogIn(pPlayer,bNotifyUserLogin);
	
	else 
	{	
		PartyInfoByUserLogin(PartyID,pPlayer->GetID());
	}	
	
	if(!pPlayer->GetMapChangePoint()) 
	{
		pParty->SendMemberLogInMsg(pPlayer->GetID());
		pParty->NotifyMemberLogin(pPlayer->GetID());
	}	
}
void CPartyManager::NotifyUserLogIn(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);

	if(pParty ==NULL)
		return;
	if(pParty->m_Member[0].MemberID == 0)
		return;
	pParty->NotifyUserLogIn(pmsg);
}
void CPartyManager::UserLogOut(CPlayer* pPlayer)
{
	DWORD PartyID = pPlayer->GetPartyIdx();
	if(PartyID == 0)
		return;

	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		char buf[128];
		sprintf(buf, "队伍不存在. 队伍编号:[%d]", PartyID);
		ASSERTMSG(0,buf);
		return;
	}
	pParty->UserLogOut(pPlayer->GetID()); 
	pParty->SendMsgUserLogOut(pPlayer->GetID());

	NotifyChangesOtherMapServer(pPlayer->GetID(),pParty,MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER, PartyID);
}

void CPartyManager::NotifyUserLogOut(SEND_CHANGE_PARTY_MEMBER_INFO* pmsg)
{
	CParty* pParty = GetParty(pmsg->PartyID);
	if(pParty == NULL)
		return;
	if(pParty->GetMasterID() == 0) 
		return;
	pParty->UserLogOut(pmsg->dwObjectID);
	pParty->SendMsgUserLogOut(pmsg->dwObjectID);
}
void CPartyManager::UserLogInAfterDBQuery(DWORD CharacterID,DWORD PartyIdx,PARTYMEMBER* PartyInfo, PARTY_ADDOPTION AddOption)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(CharacterID);
	if(pPlayer == NULL)
		return;

	CParty* pParty;
	pParty = GetParty(PartyIdx);
	if(!pParty)
	{
		pParty = RegistParty(PartyIdx);
	}

	pParty->SetAddOption(&AddOption);
	for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
	{
		pParty->m_Member[i].bLogged = PartyInfo[i].bLogged;
		pParty->m_Member[i].MemberID = PartyInfo[i].MemberID;
		SafeStrCpy(pParty->m_Member[i].Name, PartyInfo[i].Name, MAX_NAME_LENGTH+1);

		pParty->m_Member[i].Level = PartyInfo[i].Level;
	}

	pParty->UserLogIn(pPlayer,TRUE);
}

void CPartyManager::DelMemberSyn(DWORD PlayerID, DWORD PartyID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
	{
		ASSERT(0);
		return;
	}

	PartyDelMember(PartyID, PlayerID);
}

void CPartyManager::NotifyDelSyn(MSG_DWORD2* pmsg)
{	
	DelMember(pmsg->dwData1,pmsg->dwData2);
}
void CPartyManager::NotifyChangesOtherMapServer(DWORD TargetPlayerID,CParty* pParty, BYTE Protocol, DWORD PartyIDX, LEVELTYPE Level)
{
	SEND_CHANGE_PARTY_MEMBER_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = Protocol;
	msg.dwObjectID = TargetPlayerID;
	msg.PartyID = PartyIDX;
	SafeStrCpy(msg.Name,pParty->GetMemberName(TargetPlayerID), MAX_NAME_LENGTH+1);
	msg.Level = Level;

	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));
}


void CPartyManager::RemoveParty(DWORD DeletedPlayerID, DWORD PartyID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return;
	pParty->RemovePartyMember(DeletedPlayerID);
}


void CPartyManager::SendErrMsg(DWORD PlayerID, int ErrKind, BYTE Protocol)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
	if(pPlayer == NULL)
		return;

	char buf[128];
	sprintf(buf, "PartyErrNum: %d, CharacterIdx: %d", ErrKind, PlayerID);
	ASSERTMSG(0, buf);

	MSG_BYTE msg;
	msg.Category = MP_PARTY;
	msg.Protocol = Protocol;
	msg.bData = ErrKind;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

BOOL CPartyManager::SendPartyMsg(DWORD PartyID, MSGBASE* pmsg, int msglen)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return FALSE;

	pParty->SendMsgToAll(pmsg,msglen);
	return TRUE;
}

void CPartyManager::SetTacticObjectID(DWORD PartyID,DWORD TacticObjectID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return;

	pParty->SetTacticObjectID(TacticObjectID);
}

void CPartyManager::NotifyMemberLoginMsg(DWORD PartyID, DWORD PlayerID)
{
	CParty* pParty = GetParty(PartyID);
	if(pParty == NULL)
		return;

	pParty->SendMemberLogInMsg(PlayerID);
}

void CPartyManager::MemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl)
{
	DoMemberLevelUp(PartyIDX, PlayerIDX, lvl);
	
	MSG_DWORD3 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_NOTIFYMEMBER_LEVEL;
	msg.dwData1 = PartyIDX;
	msg.dwData2 = PlayerIDX;
	msg.dwData3 = lvl;	
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));	
}

void CPartyManager::DoMemberLevelUp(DWORD PartyIDX, DWORD PlayerIDX, LEVELTYPE lvl)
{
	CParty* pParty = PARTYMGR->GetParty(PartyIDX);
	if(pParty == NULL)
		return;
	
	if(pParty->GetMasterID() == 0) 
		return;
	pParty->SetMemberLevel(PlayerIDX, lvl);

	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MEMBERLEVEL;
	msg.dwData1 = PlayerIDX;
	msg.dwData2 = lvl;
	pParty->SendMsgExceptOne(&msg, sizeof(msg), PlayerIDX);
}

void CPartyManager::SendObtainItemMsg(CPlayer* pPlayer, WORD ObtainIdx)
{
	CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
	if(pParty == NULL)
		return;
	MSG_DWORD_WORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MONSTER_OBTAIN_NOTIFY;
	msg.dwData = pPlayer->GetID();

	msg.wData = ObtainIdx;
	pParty->SendMsgExceptOneinChannel(&msg, sizeof(msg), pPlayer->GetID(), pPlayer->GetGridID());
}

void CPartyManager::MasterToPartyRequest(CPlayer* pPlayer, MSG_NAME_DWORD2* pMsg)
{
	DWORD dwPartyIDX, dwMasterID, dwPlayerID = 0;

	dwPartyIDX = pMsg->dwData1;
	dwPlayerID = pMsg->dwObjectID;

	CParty* pParty = GetParty(dwPartyIDX);
	if(pParty == NULL)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotParty);
		return;
	}	

	dwMasterID = pParty->GetMasterID();	
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(dwMasterID);
	if(pMaster == NULL) 
	{		
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotMaster);			
		return;
	}

	if(pParty->IsPartyMember(pMaster->GetID()) == FALSE)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotMaster);		
		return;	
	}
	
	if(pParty->IsPartyMember(dwPlayerID) == TRUE)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_PartyExistence);
		return;
	}

	PARTY_ADDOPTION* pAddOption = pParty->GetAddOption();

	if(pAddOption->dwPartyIndex != dwPartyIDX)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotParty);
		return;
	}

	if(pAddOption->wLimitCount <= pParty->GetMemberNum())
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_Full);
		return;
	}

	LEVELTYPE wPlayerLevel = pPlayer->GetLevel();
	if( (pAddOption->wMinLevel > wPlayerLevel) || (pAddOption->wMaxLevel < wPlayerLevel) )
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_Level);
		return;
	}

	if(pAddOption->bPublic == FALSE)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_Public);
		return;
	}

	if(pMaster->GetState() == eObjectState_Die)
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotState);
		return;
	}

	if( (pMaster->GetBattle()->GetBattleKind() != eBATTLE_KIND_NONE) &&
		(pMaster->GetBattle()->GetBattleKind() != eBATTLE_KIND_MUNPAFIELD) )
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_NotState);
		return;		
	}	

	if(pParty->GetRequestPlayerID() == 0)
	{
		pParty->SetRequestPlayerID(dwPlayerID);
		pParty->StartRequestProcessTime();		
	}	
	else
	{
		MasterToPartyRequestErr(dwPlayerID, eErr_Request_Progress);
		return;
	}

	MSG_NAME_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MASTERTOREQUEST_ACK;
	msg.dwObjectID = pMaster->GetID();
	msg.dwData = dwPlayerID;
	SafeStrCpy(msg.Name, pMsg->Name, MAX_NAME_LENGTH+1);
	pMaster->SendMsg(&msg, sizeof(msg));
}

void CPartyManager::MasterToPartyRequestErr(DWORD dwPlayerID, DWORD dwErrIndex)
{	
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_ERROR;
	msg.dwObjectID = dwPlayerID;
	msg.dwData = dwErrIndex;
	g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );		
}

void CPartyManager::NotifyPartyInfoSyn(DWORD dwPartyID)
{
	PARTY_INFO msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_NOTIFY_INFO;

	CParty* pParty = GetParty(dwPartyID);
	if(pParty == NULL)
		return;

	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; ++i)
	{
		pParty->GetMemberInfo(i, &msg.Member[i]);		
	}

	msg.Option = pParty->GetOption();

	PARTY_ADDOPTION* pAddOption =  pParty->GetAddOption();
	memcpy(&msg.PartyAddOption, pAddOption, sizeof(msg.PartyAddOption));	
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
}

void CPartyManager::NotifyPartyInfo(PARTY_INFO* pPartyInfo)
{
	CParty* pParty = GetParty(pPartyInfo->PartyAddOption.dwPartyIndex);
	
	if(pParty != NULL)
		return;

	pParty = RegistParty(pPartyInfo->PartyAddOption.dwPartyIndex);
	
	for(int i = 0 ; i < MAX_PARTY_LISTNUM ; ++i)
	{
		pParty->m_Member[i].bLogged = pPartyInfo->Member[i].bLogged;
		pParty->m_Member[i].MemberID = pPartyInfo->Member[i].dwMemberID;
		SafeStrCpy(pParty->m_Member[i].Name, pPartyInfo->Member[i].Name, MAX_NAME_LENGTH+1);

		pParty->m_Member[i].Level = pPartyInfo->Member[i].Level;
	}

	pParty->SetOption(pPartyInfo->Option);
	pParty->SetAddOption(&pPartyInfo->PartyAddOption);
	
}

void CPartyManager::PartyMatchingInfo(CPlayer* pPlayer, MSG_WORD* pMsg)
{
	MSG_PARTYMATCHING_INFO msg;	
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_MATCHING_INFO;
	msg.dwObjectID = pMsg->dwObjectID;

	WORD wCount = 0;
	LEVELTYPE wLevel = pMsg->wData;

	m_partyRoomHashTable.SetPositionHead();
	while (CParty* pParty = m_partyRoomHashTable.GetData())
	{
		if(wCount >= MAX_PARTYMATCHING_INFO_NUM)
			break;

		if(pParty->IsMemberLogIn(0) == FALSE)
			continue;

		PARTY_ADDOPTION* pAddOption = pParty->GetAddOption();
		if( ((pAddOption->wMinLevel > wLevel) && (wLevel != 0)) || ((pAddOption->wMaxLevel < wLevel) && (wLevel != 0)) )
			continue;
		
		for(int i = 0 ; i < MAX_PARTY_LISTNUM ; ++i)
		{
			msg.PartyMatchingInfo[wCount].dwMemberID[i] = pParty->GetMemberID(i);			

		}

		SafeStrCpy(msg.PartyMatchingInfo[wCount].szMasterName, pParty->GetMemberName(pParty->GetMemberID(0)), MAX_NAME_LENGTH+1);

		memcpy(&msg.PartyMatchingInfo[wCount].PartyAddOption, pAddOption, sizeof(msg.PartyMatchingInfo[wCount].PartyAddOption));	
		
		wCount++;		
	}

	if(wCount > MAX_PARTYMATCHING_INFO_NUM)
		msg.wCount = MAX_PARTYMATCHING_INFO_NUM;
	else
		msg.wCount = wCount;
	
	ASSERT(pPlayer != NULL);

	pPlayer->SendMsg( &msg, msg.GetMsgSize() );
}

void CPartyManager::Process()
{
	m_partyRoomHashTable.SetPositionHead();
	while(CParty* pParty = m_partyRoomHashTable.GetData())
	{
		if(pParty->GetRequestPlayerID() == 0)
			return;

		pParty->Process();
	}
}


