
#include "stdafx.h"
#include "GameIn.h"
#include "PartyManager.h"

#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "objectmanager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "ItemManager.h"
#include "AbilityManager.h"
#include "GameEventManager.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "StatusIconDlg.h"
#include "CharacterDialog.h"

#include "MHMap.h"
#include "PartyMatchingDlg.h"

#include "./interface/cDialog.h"
#include "BattleSystem_Client.h"
#include "AIManager.h"
#ifdef _XDYY_
#include "OnlineVoiceManager.h"
#endif
GLOBALTON(CPartyManager);

CPartyManager::CPartyManager()
{
}

CPartyManager::~CPartyManager()
{
}

void CPartyManager::Init()
{
	m_MasterChanging = FALSE;
	m_Party.Clear();
	m_bIsProcessing = FALSE;
	m_RequestPartyID = 0;

	m_dwRequestPlayerID = 0;
	LoadPartyEventInfo();
	vc_MemberInfo.clear();
}

void CPartyManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	if( GAMEIN->GetPartyDialog() == NULL )
		return;
	if(HERO == NULL)
		return;

	switch(Protocol) 
	{
	case MP_PARTY_INFO: 			
		{
			PARTY_INFO* pmsg = (PARTY_INFO*)pMsg;
			SetPartyInfo(pmsg);
			GAMEIN->GetPartyDialog()->SetActive(TRUE);
			GAMEIN->GetPartyDialog()->RefreshDlg();

			for(int i=0; i<MAX_PARTY_LISTNUM; i++)
			{
				if(pmsg->Member[i].bLogged && pmsg->Member[i].dwMemberID != HERO->GetID() &&
					pmsg->Member[i].LifePercent)
				{
					PICONMGR->CreatePartyIcon(pmsg->Member[i].dwMemberID, &pmsg->Member[i]);
				}
			}

			RefreshPartyEventBuffDesc();
			SetIsProcessing(FALSE);
		}
		break;
		
	case MP_PARTY_ADD_INVITE:
		{	
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
			SetRequestPartyID(pmsg->dwData1);
			GAMEIN->GetPartyInviteDialog()->SetMsg(pmsg->Name, (BYTE)pmsg->dwData2);
			GAMEIN->GetPartyInviteDialog()->SetActive(TRUE);
			if(sCPlayerAI->sAiConfig.AutoSwitch[eOpen])
			{
				if(sCPlayerAI->sAiConfig.AutoSwitch[eAutoJoinGroup])
				{
					GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
					PARTYMGR->PartyInviteAccept();
				}
			}
		}
		break;
	
	case MP_PARTY_ADD_ACK: 
		{
			SEND_PARTY_MEMBER_INFO* pmsg = (SEND_PARTY_MEMBER_INFO*)pMsg;
			AddPartyMemberResult(pmsg);	

			RefreshPartyEventBuffDesc();

			SetIsProcessing(FALSE);
		} 
		break; 
		
	case MP_PARTY_ADD_NACK: 
		{	
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			switch(pmsg->dwData)
			{
			case eErr_Add_NoPlayer:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(67)); break;
			case eErr_Add_AlreadyinParty:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(76)); break;
			case eErr_Add_OptionNoParty:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(79)); break;
			case eErr_Add_DiedPlayer:
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(158)); break;
			}			
		}
		break;
		
	case MP_PARTY_DEL_ACK: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DelPartyMemberResult(pmsg);
			
			if(pmsg->dwData == HERO->GetID())			
				PICONMGR->DeleteAllPartyIcon();			
			else			
				PICONMGR->DeletePartyIcon(pmsg->dwData);

			RefreshPartyEventBuffDesc();
		}
		break;
		
	case MP_PARTY_DEL_NACK: 
		{
			MSG_INT* pmsg = (MSG_INT*)pMsg;
			switch(pmsg->nData)
			{
			case eErr_ChangingMaster:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(159) ); break;
			}
		}
		break;
		
	case MP_PARTY_CREATE_ACK: 
		{
			PARTY_ADDOPTION* pmsg = (PARTY_ADDOPTION*)pMsg;
			CreatePartyResult(pmsg);			

			GAMEEVENTMGR->AddEvent( eGameEvent_Ability, 403 );

			RefreshPartyEventBuffDesc();
		}
		break;
		
	case MP_PARTY_CREATE_NACK: 
		{
			ASSERT(0);
			MSG_BYTE* pmsg = (MSG_BYTE* )pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(89), pmsg->bData);
			SetIsProcessing(FALSE);
		}
		break;
		
	case MP_PARTY_BAN_ACK: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			BanPartyMemberResult(pmsg);		


			RefreshPartyEventBuffDesc();
		} 
		break;
	case MP_PARTY_BAN_NACK: 
		{
			
		}
		break;
		
	case MP_PARTY_MEMBER_LOGIN: 
		{
			SEND_PARTY_MEMBER_INFO* pmsg = (SEND_PARTY_MEMBER_INFO*)pMsg;
			UserLogIn(pmsg);
			
			if(pmsg->MemberInfo.bLogged && pmsg->MemberInfo.dwMemberID != HERO->GetID())
			{
				if(pmsg->MemberInfo.LifePercent)
					PICONMGR->CreatePartyIcon(pmsg->MemberInfo.dwMemberID, &pmsg->MemberInfo);
				else	
					PICONMGR->DeletePartyIcon(pmsg->MemberInfo.dwMemberID);
			}

			RefreshPartyEventBuffDesc();
		}
		break;
		
	case MP_PARTY_MEMBER_LOGOUT: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			UserLogOut(pmsg);
			
			PICONMGR->DeletePartyIcon(pmsg->dwData);

			RefreshPartyEventBuffDesc();
		 }
		break;
		
	case MP_PARTY_CHANGEMASTER_ACK: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			ChangeMasterPartyResult(pmsg);
			SetMasterChanging(FALSE);

			RefreshPartyEventBuffDesc();
		}
		break;
		
	case MP_PARTY_CHANGEMASTER_NACK: 
		{
			ASSERT(0);
			SetMasterChanging(FALSE);
		}
		break;
		
	case MP_PARTY_BREAKUP_ACK: 
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			BreakUpPartyResult();

			PICONMGR->DeleteAllPartyIcon();

			RefreshPartyEventBuffDesc();
		} 
		break;
		
	case MP_PARTY_BREAKUP_NACK: 
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(159) );
			SetIsProcessing(FALSE);
		}
		break;

	case MP_PARTY_MEMBERLIFE:
		{
			SEND_PARTY_GAUGE* pmsg = (SEND_PARTY_GAUGE*)pMsg;
			SetPartyMemberLifePercent(pmsg->PlayerID, (DWORD)pmsg->GaugePercent); //data1:playerid, data2:val
			GAMEIN->GetPartyDialog()->RefreshDlg();
		}
		break;

	case MP_PARTY_MEMBERSHIELD:
		{
			SEND_PARTY_GAUGE* pmsg = (SEND_PARTY_GAUGE*)pMsg;
			SetPartyMemberShieldPercent(pmsg->PlayerID, (DWORD)pmsg->GaugePercent); //data1:playerid, data2:val
			GAMEIN->GetPartyDialog()->RefreshDlg();
		}
		break;
		
	case MP_PARTY_MEMBERNAERYUK:
		{
			SEND_PARTY_GAUGE* pmsg = (SEND_PARTY_GAUGE*)pMsg;
			SetPartyMemberNaeRyukPercent(pmsg->PlayerID, (DWORD)pmsg->GaugePercent);			
			GAMEIN->GetPartyDialog()->RefreshDlg();
		}
		break;
	case MP_PARTY_MEMBERLEVEL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			SetPartyMemberLevel(pmsg->dwData1, (LEVELTYPE)pmsg->dwData2);			
			GAMEIN->GetPartyDialog()->RefreshDlg();
		}
		break;
	case MP_PARTY_SENDPOS:
		{
			SEND_PARTYICON_MOVEINFO* pmsg = (SEND_PARTYICON_MOVEINFO*)pMsg;
			if(pmsg->MoveInfo.dwMoverID != HERO->GetID())
				PICONMGR->SetMoveInfo(&pmsg->MoveInfo);
		}
		break;
	case MP_PARTY_REVIVEPOS:
		{
			SEND_PARTYICON_REVIVE* pmsg = (SEND_PARTYICON_REVIVE*)pMsg;
			PICONMGR->SetCurPosition(pmsg->dwMoverID, pmsg->Pos);
		}
		break;
	case MP_PARTY_INVITE_DENY_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(75) );
		}
		break;
	case MP_PARTY_INVITE_ACCEPT_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(26) );
		}
		break;
	case MP_PARTY_INVITE_ACCEPT_NACK:
		{
			MSG_BYTE * pmsg = (MSG_BYTE*)pMsg;
			if(pmsg->bData == eErr_BrokenParty)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(66) );
			else if(pmsg->bData == eErr_Add_Full)
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(77) );
		}
		break;

	case MP_PARTY_MEMBER_LOGINMSG:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(pmsg->dwData != HEROID)
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(64), m_Party.GetMemberName(pmsg->dwData));
		}
		break;
	case MP_PARTY_MONSTER_OBTAIN_NOTIFY:
		{
			MSG_DWORD_WORD* pmsg = (MSG_DWORD_WORD*)pMsg;
			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pmsg->wData);
			if(pItemInfo == 0)
				return;
			CHATMGR->AddMsg( CTC_GETITEM, CHATMGR->GetChatMsg( 150 ), GetPartyMemberName(pmsg->dwData), pItemInfo->ItemName );
		}
		break;
	case MP_PARTY_MASTERTOREQUEST_ACK:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;

			PartyDecition(pmsg);
			SetIsProcessing(TRUE);			
		}
		break;
	case MP_PARTY_ERROR:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			PartyError(pmsg->dwData);
			SetIsProcessing(FALSE);
		}
		break;		
	case MP_PARTY_REQUEST_CONSENT_NACK:	
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1736) );
			SetIsProcessing(FALSE);
		}
		break;
	case MP_PARTY_REQUEST_REFUSAL_NACK:	
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1736) );
			SetIsProcessing(FALSE);
		}
		break;
	case MP_PARTY_REQUEST_REFUSAL_ACK: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1727) );
			SetIsProcessing(FALSE);
		}
		break;
	case MP_PARTY_MATCHING_INFO:
		{
			MSG_PARTYMATCHING_INFO* pmsg = (MSG_PARTYMATCHING_INFO*)pMsg;

			CPartyMatchingDlg* pPartyMatchingDlg = GAMEIN->GetPartyMatchingDlg();
			if(pPartyMatchingDlg == NULL)
				break;

			pPartyMatchingDlg->SetPartyMatchingList(pmsg);
		}
		break;
#ifdef _XDYY_
	case MP_PARTY_VOICE_ADD:
		{//[小队语音_成员加入聊天室][By:十里坡剑神][QQ:112582793][2019/3/20][14:27:15]
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			if(!m_Party.IsPartyMember(pmsg->dwData3))
				return;
			VOICEMEMBER info;
			info.CharID = pmsg->dwData3;
			info.MemberID = pmsg->dwData1;
			info.m_CharName = m_Party.GetMemberName(pmsg->dwData3);
			vc_MemberInfo[info.CharID]  =info;
			if(VOICEMGR->IsInint())
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2332),info.m_CharName.c_str());
			GAMEIN->GetPartyDialog()->SetMemberVoiceBtn(info.CharID,TRUE);
		}
		break;
	case MP_PARTY_VOICE_QUIT:
		{//[小队语音_成员退出聊天室][By:十里坡剑神][QQ:112582793][2019/3/20][14:27:27]
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			

			if(!m_Party.IsPartyMember(pmsg->dwData3))
				return;
			std::map<DWORD,VOICEMEMBER>::iterator iter;
			iter = vc_MemberInfo.find(pmsg->dwData3);
			if(iter!=vc_MemberInfo.end())
			{
				GAMEIN->GetPartyDialog()->SetMemberVoiceBtn(iter->second.CharID,FALSE);
				if(VOICEMGR->IsInint())
					CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2333),iter->second.m_CharName.c_str());
				vc_MemberInfo.erase(iter);
			}
		}
		break;
#endif
	} 
}
#ifdef _XDYY_
void CPartyManager::SetMemberVoiceBtn(DWORD memberID)
{//[小队语音_聊天室成员说话时设置喇叭颜色][By:十里坡剑神][QQ:112582793][2019/3/21][17:02:43]
	std::map<DWORD,VOICEMEMBER>::iterator iter;
	iter = vc_MemberInfo.begin();
	while(1)
	{
		if(iter==vc_MemberInfo.end())
			break;
		if(iter->second.MemberID==memberID)
		{
			GAMEIN->GetPartyDialog()->SetMemberVoiceBtnRGB(iter->second.CharID);
			break;
		}
	}
}
void CPartyManager::DisableMemberVoice(DWORD CharID)
{//[小队语音_禁止某个聊天室成员说话][By:十里坡剑神][QQ:112582793][2019/3/21][17:01:50]
	std::map<DWORD,VOICEMEMBER>::iterator iter;
	iter = vc_MemberInfo.find(CharID);
	if(iter!=vc_MemberInfo.end())
	{
		VOICEMGR->CloseSomeOneMic(iter->second.MemberID);
	}
}
void CPartyManager::OpenMemberVoice(DWORD CharID)
{//[小队语音_开启某个聊天室成员说话][By:十里坡剑神][QQ:112582793][2019/3/21][17:02:08]
	std::map<DWORD,VOICEMEMBER>::iterator iter;
	iter = vc_MemberInfo.find(CharID);
	if(iter!=vc_MemberInfo.end())
	{
		VOICEMGR->OpneSomeOneMic(iter->second.MemberID);
	}
}
#endif
void CPartyManager::CreatePartySyn(PARTY_ADDOPTION* pAddOption) 
{
	if(HERO->GetPartyIdx() != 0)
	{
		ASSERT(0);
		return;
	}

	PARTY_ADDOPTION msg;
	msg = (*pAddOption);
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CREATE_SYN;
	msg.dwObjectID = HEROID;	
	NETWORK->Send(&msg,sizeof(msg));

	SetIsProcessing(TRUE);
}

void CPartyManager::AddPartyMemberSyn(DWORD AddPartymemeberID) 
{
	if(AddPartymemeberID == 0)
	{
		ASSERT(0);
		return;
	}
	if(m_Party.IsPartyMember(AddPartymemeberID) == TRUE)
		return;

	if(m_Party.GetMemberNum() != MAX_PARTY_LISTNUM)
	{
		MSG_DWORD2 msg;

		msg.Category	= MP_PARTY;
		msg.Protocol	= MP_PARTY_ADD_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= AddPartymemeberID;			
		msg.dwData2		= HERO->GetPartyIdx();
		NETWORK->Send(&msg,sizeof(msg));
	}
	else 
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(77) );
}

void CPartyManager::DelPartyMemberSyn() 
{
	if(HEROID == GetMasterID())
		ASSERTMSG(0,"规厘篮 呕硼甫 且 荐 绝促.");
	
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_DEL_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));
}

void CPartyManager::ChangeMasterPartySyn(DWORD FromMemberID, DWORD ToMemberID) 
{
	if(ToMemberID == 0)
		return;
	if(ToMemberID == FromMemberID)
		return;
	if(m_Party.IsPartyMember(ToMemberID) == FALSE)
		return;
	if(m_Party.GetMasterIdx() != FromMemberID)	
		return;
	if(m_Party.GetMasterIdx() == ToMemberID)	
		return;
	
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_CHANGEMASTER_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = ToMemberID;
	msg.dwData2 = HERO->GetPartyIdx();
	NETWORK->Send(&msg, sizeof(msg));
	
	SetMasterChanging(TRUE);	
}

void CPartyManager::BreakUpSyn() 
{
	if(IsMasterChanging() == TRUE)
		return;
	if(IsProcessing() == TRUE)
		return;

	if( 0 == HERO->GetPartyIdx() ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(66) );
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_BREAKUP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));

	SetIsProcessing(TRUE);
}

void CPartyManager::BanPartyMemberSyn(DWORD DelPartymemberID) 
{
	if(DelPartymemberID == 0)
		return;
	if(DelPartymemberID == HEROID)
		return;

	if(IsMasterChanging() == TRUE)
		return;

	if(m_Party.IsPartyMember(DelPartymemberID) == FALSE)
		return;
	MSG_DWORD2 msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_BAN_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData1 = DelPartymemberID;
	msg.dwData2 = HERO->GetPartyIdx();
	NETWORK->Send(&msg,sizeof(msg));
}

void CPartyManager::CreatePartyResult(PARTY_ADDOPTION* pMsg)
{
	m_Party.Clear();
	HERO->SetPartyIdx(pMsg->dwPartyIndex);
	m_Party.SetAddOption((*pMsg));
	m_Party.SetMaster(HEROID,HERO->GetObjectName(),TRUE,HERO->GetLifePercent(), HERO->GetShieldPercent(), HERO->GetNaeRyukPercent());
	SetIsProcessing(FALSE);
	GAMEIN->GetPartyDialog()->RefreshDlg();

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(68) );
	GAMEIN->GetPartyDialog()->SetActive(TRUE);
	OBJECTMGR->ApplyOverInfoOption( HERO );
#ifdef _XDYY_
	//[小队语音_设置房间信息][By:十里坡剑神][QQ:112582793][2019/3/20][13:24:20]
	VOICEMGR->SetVoiceRoomInfo(pMsg->dwPartyIndex);
#endif
}

void CPartyManager::AddPartyMemberResult(SEND_PARTY_MEMBER_INFO* pMsg) 
{
	if(pMsg->MemberInfo.dwMemberID == HEROID) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(69) );
		HERO->SetPartyIdx(pMsg->PartyID);

		OBJECTMGR->ApplyOverInfoOptionToAll();
	}
	else 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(70), pMsg->MemberInfo.Name);
		m_Party.AddMember(&pMsg->MemberInfo);

		CObject* pMember = OBJECTMGR->GetObject( pMsg->MemberInfo.dwMemberID );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

		if(pMsg->MemberInfo.bLogged && pMsg->MemberInfo.LifePercent)
		{
			PICONMGR->CreatePartyIcon(pMsg->MemberInfo.dwMemberID, &pMsg->MemberInfo);
		}
	}
	
	GAMEIN->GetPartyDialog()->RefreshDlg();

}

void CPartyManager::DelPartyMemberResult(MSG_DWORD* pMsg) 
{
	if(pMsg->dwData == HEROID) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(71) );
		HERO->SetPartyIdx(0);
		m_Party.Clear();

		GAMEIN->GetPartyDialog()->SetActive(FALSE);
		OBJECTMGR->ApplyOverInfoOptionToAll();
#ifdef _XDYY_
		//[小队语音_退出队伍][By:十里坡剑神][QQ:112582793][2019/3/21][15:20:14]
		VOICEMGR->QuiteRoom();
		VOICEMGR->ClearVoice();
#endif
	}
	else 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(163), m_Party.GetMemberName(pMsg->dwData));
		m_Party.DelMember(pMsg->dwData);		
		CObject* pMember = OBJECTMGR->GetObject( pMsg->dwData );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

	}
	GAMEIN->GetPartyDialog()->RefreshDlg();

}

void CPartyManager::BanPartyMemberResult(MSG_DWORD* pMsg) 
{
	if(pMsg->dwData == HEROID)
	{
		GAMEIN->GetPartyDialog()->SetActive(FALSE);
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(72) );
		HERO->SetPartyIdx(0);
		m_Party.Clear();
		
		OBJECTMGR->ApplyOverInfoOptionToAll();

		PICONMGR->DeleteAllPartyIcon();
#ifdef _XDYY_
		//[小队语音_退出队伍][By:十里坡剑神][QQ:112582793][2019/3/21][15:20:14]
		VOICEMGR->QuiteRoom();
		VOICEMGR->ClearVoice();
#endif
	}
	else 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(73), m_Party.GetMemberName(pMsg->dwData));
		
		m_Party.DelMember(pMsg->dwData);

		CObject* pMember = OBJECTMGR->GetObject( pMsg->dwData );
		OBJECTMGR->ApplyOverInfoOption( pMember );//

		PICONMGR->DeletePartyIcon(pMsg->dwData);
	}
	GAMEIN->GetPartyDialog()->SetClickedMemberID(0);
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::UserLogIn(SEND_PARTY_MEMBER_INFO* pMsg) 

{
	m_Party.LogIn(&pMsg->MemberInfo);
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::UserLogOut(MSG_DWORD* pMsg) 
{
	m_Party.LogOut(pMsg->dwData);
	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(65), m_Party.GetMemberName(pMsg->dwData));

	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::ChangeMasterPartyResult(MSG_DWORD* pMsg) 
{
	m_Party.ChangeMaster(pMsg->dwData);
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(74), m_Party.GetMemberName(pMsg->dwData) );
	GAMEIN->GetPartyDialog()->RefreshDlg();
}

void CPartyManager::BreakUpPartyResult()
{
	HERO->SetPartyIdx(0);
	m_Party.Clear();
	m_Party.BreakUpResult();
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 66 ) );
	GAMEIN->GetPartyDialog()->RefreshDlg();
	GAMEIN->GetPartyDialog()->SetActive(FALSE);
	SetIsProcessing(FALSE);
	OBJECTMGR->ApplyOverInfoOptionToAll();
#ifdef _XDYY_
	//[小队语音_退出队伍][By:十里坡剑神][QQ:112582793][2019/3/21][15:20:14]
	VOICEMGR->QuiteRoom();
	VOICEMGR->ClearVoice();
#endif
}

void CPartyManager::SetPartyInfo(PARTY_INFO* pmsg) 
{
	m_Party.InitParty(pmsg);
	OBJECTMGR->ApplyOverInfoOptionToAll();
#ifdef _XDYY_
	//[小队语音_设置房间信息][By:十里坡剑神][QQ:112582793][2019/3/20][13:24:20]
	VOICEMGR->SetVoiceRoomInfo(pmsg->PartyDBIdx);
#endif
}

void CPartyManager::SetPartyMemberLifePercent(DWORD PalyerID, DWORD LifePercent)
{
	m_Party.SetMemberLifePercent(PalyerID, LifePercent);
}

void CPartyManager::SetPartyMemberShieldPercent(DWORD PalyerID, DWORD ShieldPercent)
{
	m_Party.SetMemberShieldPercent(PalyerID, ShieldPercent);
}

void CPartyManager::SetPartyMemberNaeRyukPercent(DWORD PlayerID, DWORD NaeryukPercent)
{
	m_Party.SetMemberNaeRyukPercent(PlayerID, NaeryukPercent);
}

void CPartyManager::SetPartyMemberLevel(DWORD PlayerID, LEVELTYPE lvl)
{
	m_Party.SetMemberLevel(PlayerID, lvl);
}

BYTE CPartyManager::GetPartyMemberLifePercent(DWORD PlayerID)
{
	return m_Party.GetMemberLifePercent(PlayerID);
}

BYTE CPartyManager::GetPartyMemberShieldPercent(DWORD PlayerID)
{
	return m_Party.GetMemberShieldPercent(PlayerID);
}

BYTE CPartyManager::GetPartyMemberNaeRyukPercent(DWORD PlayerID)
{
	return m_Party.GetMemberNaeRyukPercent(PlayerID);
}

LEVELTYPE CPartyManager::GetPartyMemberLevel(DWORD PlayerID)
{
	return m_Party.GetMemberLevel(PlayerID);
}

void CPartyManager::PartyInviteAccept()
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INVITE_ACCEPT_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = GetRequestPartyID();
	
	NETWORK->Send(&msg,sizeof(msg));
}

void CPartyManager::PartyInviteDeny()
{
	MSG_DWORD msg;
	msg.Category = MP_PARTY;
	msg.Protocol = MP_PARTY_INVITE_DENY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = GetRequestPartyID();
	NETWORK->Send(&msg,sizeof(msg));

	SetRequestPartyID(0);
}

BOOL CPartyManager::IsPartyMember(DWORD PlayerID)
{
	return m_Party.IsPartyMember(PlayerID);
}

PARTY_MEMBER* CPartyManager::GetPartyMemberInfo(int i)
{
	return m_Party.GetPartyMemberInfo(i);
}

BOOL CPartyManager::PartyChat(char* ChatMsg, char* SenderName)
{
	if( HERO->GetPartyIdx() == 0 )
		return FALSE;
	
	SEND_PARTY_CHAT msg;
	memset(&msg, 0, sizeof(msg));
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_PARTY;
	msg.dwObjectID = HEROID;
	strcpy(msg.Name, SenderName);
	strcpy(msg.Msg, ChatMsg);
	for(int n=0; n<MAX_PARTY_LISTNUM; ++n)
	{
		if(m_Party.IsLogIn(n))
		{
			msg.MemberID[msg.MemberNum] = m_Party.GetMemberID(n);
			msg.MemberNum++;
		}
	}
	NETWORK->Send(&msg, msg.GetMsgLength()); 

	return TRUE;
}

BOOL CPartyManager::IsMasterChanging()
{
	if(m_MasterChanging == TRUE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(159) );
		return TRUE;
	}
	return FALSE;
}

BOOL CPartyManager::CanActivate()
{
	if(HERO->GetPartyIdx() == 0)
	{
		if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_PARTY,HERO->GetAbilityGroup(),1) == FALSE)
		{
			CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
			return FALSE;
		}

		if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
			return FALSE;
		}

		if(m_bIsProcessing == FALSE)
		{
			GAMEIN->GetPartyCreateDialog()->SetActive(TRUE);
			return FALSE;
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(78) );
			return FALSE;
		}
	}
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		return FALSE;
	}

	return TRUE;
}


void CPartyManager::LoadPartyEventInfo()
{
	memset( m_PartyEventRate, 0, sizeof(PARTYEVENT)*ePartyEvent_Max );

	CMHFile file;
	if( !file.Init( "./Resource/PartyPlustimeInfo.bin", "rb") )
		return;

	int count = file.GetInt();	

	for( int i=0; i<count; ++i )
	{
		int idx = file.GetInt();
		m_PartyEventRate[idx].Rate[0] = file.GetFloat();
		m_PartyEventRate[idx].Rate[1] = file.GetFloat();
		m_PartyEventRate[idx].Rate[2] = file.GetFloat();
		m_PartyEventRate[idx].Rate[3] = file.GetFloat();
		m_PartyEventRate[idx].Rate[4] = file.GetFloat();
		m_PartyEventRate[idx].Rate[5] = file.GetFloat();
		m_PartyEventRate[idx].Rate[6] = file.GetFloat();
	}
}


void CPartyManager::RefreshPartyEventBuffDesc()
{
	int eventcount = 0;
	int count = PICONMGR->GetPartyMemberCountofMap();

    for(int i=0; i<ePartyEvent_Max; ++i )
	{
		if( m_PartyEventRate[i].Rate[count] )
			++eventcount;
	}

	if( eventcount == 0 )
		STATUSICONDLG->RemoveIcon( HERO, 148, 1 );
	else
	{
		STATUSICONDLG->AddIcon( HERO, 148 );
	}
}


DWORD CPartyManager::GetPartyEventRate( DWORD Index )
{
	int count = PICONMGR->GetPartyMemberCountofMap();

	if( m_PartyEventRate[Index].Rate[count] == 0 )
		return 0;

	float fr = 1.f;
	fr = fr - m_PartyEventRate[Index].Rate[count];	
	fr = abs( fr );

	return (DWORD)(fr*100);
}

void CPartyManager::PartyRequestResult(BOOL bConsent)
{
	if(m_dwRequestPlayerID == 0)
		return;

	if(bConsent == TRUE)
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_PARTY;
		msg.Protocol	= MP_PARTY_REQUEST_CONSENT_SYN;
		msg.dwObjectID	= m_dwRequestPlayerID;
		msg.dwData1		= HEROID;
		msg.dwData2		= HERO->GetPartyIdx();		
		NETWORK->Send(&msg,sizeof(msg));
	}
	else
	{
		MSG_DWORD2 msg;
		msg.Category	= MP_PARTY;
		msg.Protocol	= MP_PARTY_REQUEST_REFUSAL_SYN;

		msg.dwObjectID	= m_dwRequestPlayerID;
		msg.dwData1 = HEROID;
		msg.dwData2 = HERO->GetPartyIdx();		
		NETWORK->Send(&msg,sizeof(msg));
	}

	m_dwRequestPlayerID = 0;
	SetIsProcessing(FALSE);


	cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_PARTYDECISION );		
	if(pMsgBox == NULL)
		return;

	WINDOWMGR->AddListDestroyWindow(pMsgBox);
}

void CPartyManager::PartyError(DWORD dwkind)
{
	switch(dwkind)
	{
	case eErr_Request_NotParty:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1725) );	
		break;	
	case eErr_Request_PartyExistence:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1732) );
		break;
	case eErr_Request_Full:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1733) );
		break;
	case eErr_Request_Level:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1731) );
		break;
	case eErr_Request_Public:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1729) );
		break;
	case eErr_Request_NotMaster:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1730) );	
		break;
	case eErr_Request_NotState:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1728) );	
		break;
	case eErr_Request_Progress:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
		break;
	case eErr_Request_Refusal:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1727) );
		break;	
	case eErr_Request_TimeExcess:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1739) );
		break;	
	}

	if(dwkind == eErr_Request_TimeExcess)	
	{
		cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_PARTYDECISION );		
		if(pMsgBox == NULL)
			return;
		pMsgBox->SetActive(FALSE);
		WINDOWMGR->AddListDestroyWindow(pMsgBox);
	}	
}

void CPartyManager::PartyDecition(MSG_NAME_DWORD* pMsg)
{
	if(IsProcessing() == TRUE)
		return;
	
	PARTYMGR->SetRequestPlayerID(pMsg->dwData);

	char szStr[256];
	memset(szStr, 0, sizeof(szStr));
	sprintf(szStr, CHATMGR->GetChatMsg(1735), pMsg->Name);
	WINDOWMGR->MsgBox(MBI_PARTYDECISION, MBT_YESNO, szStr);

	cDialog* pDlg = (cDialog*)WINDOWMGR->GetWindowForID( MBI_PARTYDECISION );
	if(pDlg == NULL)
		return;

	pDlg->SetAbsXY(432, 570);	
	pDlg->SetActive(TRUE);
}

void CPartyManager::RefleshPartyMatchingList(LEVELTYPE wLevel)
{
	MSG_WORD msg;
	msg.Category	= MP_PARTY;
	msg.Protocol	= MP_PARTY_MATCHING_INFO;
	msg.dwObjectID	= HEROID;
	msg.wData = wLevel;
	NETWORK->Send(&msg,sizeof(msg));	
}