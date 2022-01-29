



// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildManager.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "Network.h"
#include "Player.h"
#include "Guild.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "AbilityManager.h"
#include "GuildMark.h"
#include "PackedData.h"
#include "GuildFieldWarMgr.h"
#include "ItemManager.h"
#include "GuildTournamentMgr.h"
#include "GuildUnionManager.h"
#include "SiegeWarMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int CGuildManager::m_gPlustimeKindNum  = 0;

CGuildManager::CGuildManager()
{
	m_GuildRoomHashTable.Initialize(256);
	m_GuildMarkTable.Initialize(128);
	m_ItemOptionList.Initialize(MAX_ITEM_OPTION_NUM);
	m_ItemRareOptionList.Initialize(MAX_ITEM_OPTION_NUM); //¹öÄÏÀ» Å©°Ô Àâ´Â ÀÌÀ¯´Â?MAX_ITEM_OPTION_NUM*100-->MAX_ITEM_OPTION_NUM ÀÌÀ¯°¡ ¾ø´Â °Í°°¾Æ ÁÙ¿´´Ù.
	LoadLevelUpInfo();

	memset(&m_GuildPointRule,0,sizeof(GUILDPOINT_RULE));
	m_CHECKTIMELEN_GuildHuntedMonsterCounter = 0;
	m_dwLastDBUpdateTime = gCurTime;

	LoadGuildPointRule();
	SYSTEMTIME st;
	GetLocalTime(&st);
	m_wCurDateCheckForConverCount = st.wDay;
	m_CheckTimeForGuildPlustimeEnd = gCurTime;
}

CGuildManager::~CGuildManager()
{
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while(pGuild = m_GuildRoomHashTable.GetData())
		delete pGuild;
	m_GuildRoomHashTable.RemoveAll();

	CGuildMark* pMarkInfo;
	m_GuildMarkTable.SetPositionHead();
	while(pMarkInfo = m_GuildMarkTable.GetData())
		delete pMarkInfo;
	m_GuildMarkTable.RemoveAll();

	ITEM_OPTION_INFO * pOInfo = NULL;
	m_ItemOptionList.SetPositionHead();
	while(pOInfo = m_ItemOptionList.GetData())
		delete pOInfo;
	m_ItemOptionList.RemoveAll();

	ITEM_RARE_OPTION_INFO* pRareInfo = NULL;
	m_ItemRareOptionList.SetPositionHead();
	while(pRareInfo = m_ItemRareOptionList.GetData())
		delete pRareInfo;
	m_ItemRareOptionList.RemoveAll();

	if(m_pGuildPlustimeInfo)
	{
		delete[] m_pGuildPlustimeInfo;
		m_pGuildPlustimeInfo = NULL;
	}

	//magi82 - ¹®ÇÏ»ý °¡ÀÔÆí¸®½Ã½ºÅÛ /////////////////////////////////////////

	m_GuildTraineeList.RemoveAll();
	m_GuildTraineeList = NULL;
	
	m_GuildTraineeGuildList.RemoveAll();
	m_GuildTraineeGuildList = NULL;

	m_GuildTraineeGuildIntroInfoList.RemoveAll();
	m_GuildTraineeGuildIntroInfoList = NULL;
	//////////////////////////////////////////////////////////////////////////
}
void CGuildManager::Guild_Create_Syn(void* pMsg)
{
	SEND_GUILD_CREATE_SYN* pmsg = (SEND_GUILD_CREATE_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	CreateGuildSyn(pPlayer, pmsg->GuildName, pmsg->Intro);
}
void CGuildManager::Guild_BreakUp_Syn(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	if (GUILDFIELDWARMGR->IsGuildFieldWar(pPlayer->GetGuildIdx()))
		return;
	BreakUpGuildSyn(pPlayer);
}
void CGuildManager::Guild_DeleteMember_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pMaster == 0)
		return;
	DeleteMemberSyn(pMaster, pmsg->dwData);
}
void CGuildManager::Guild_AddMember_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pMaster == 0)
		return;
	AddMemberSyn(pMaster, pmsg->dwData);
}
void CGuildManager::Guild_Invite_Accept(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	AddMemberResult(pPlayer, pmsg->dwData);
}
void CGuildManager::Guild_Invite_Deny(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData);
	if (pMaster == 0)
		return;
	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_INVITE_DENY;
	pMaster->SendMsg(&msg, sizeof(msg));
}
void CGuildManager::Guild_Secede_Syn(void* pMsg)
{
	MSGBASE * pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	SecedeSyn(pPlayer);
}
void CGuildManager::Guild_LevelUp_Syn(void* pMsg)
{
	MSGBASE * pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	LevelUpSyn(pPlayer);
}
void CGuildManager::Guild_ChangeRank_Syn(void* pMsg)
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	ChangeMemberRank(pPlayer, pmsg->dwData, pmsg->bData);
}
void CGuildManager::Guild_GiveNickName_Syn(void* pMsg)
{
	SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	GiveNickNameSyn(pPlayer, pmsg->TargetIdx, pmsg->NickName);
}
void CGuildManager::Guild_PutIn_Money_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)	return;

	if (!CheckHack(pPlayer))	return;

	PutInMoneyGuild(pPlayer, pmsg->dwData);
}
void CGuildManager::Guild_PutOut_Money_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)	return;

	if (!CheckHack(pPlayer))	return;

	PutOutMoneyGuild(pPlayer, pmsg->dwData);
}
void CGuildManager::Guild_Create_Notify_To_Map(void* pMsg)
{
	SEND_GUILD_CREATE_NOTIFY* pmsg = (SEND_GUILD_CREATE_NOTIFY*)pMsg;
	CGuild* pGuild = RegistGuild(&pmsg->GuildInfo, 0);
	if (pGuild == 0)
		return;
	GUILDMEMBERINFO Memberinfo;
	Memberinfo.InitInfo(pmsg->GuildInfo.MasterIdx, pmsg->GuildInfo.MasterName, pmsg->MasterLvl, GUILD_MASTER);
	pGuild->AddMember(&Memberinfo);
}
void CGuildManager::Guild_BreakUp_Notify_To_Map(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	DoBreakUp(pmsg->dwObjectID);
}
void CGuildManager::Guild_DeleteMember_Notify_To_Map(void* pMsg)
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
	DoDeleteMember(pmsg->dwObjectID, pmsg->dwData, eGuild_Delete, pmsg->Name);
}
void CGuildManager::Guild_AddMember_Notify_To_Map(void* pMsg)
{
	SEND_GUILD_MEMBER_INFO* pmsg = (SEND_GUILD_MEMBER_INFO*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->GuildIdx);
	if (pGuild == 0)
	{
		ASSERTMSG(0, "Different Guild Info!");
		return;
	}

	//SW070824 ¼öÁ¤
	/*if(pGuild->CanAddMember() == FALSE )
	{
	ASSERTMSG(0, "Different Guild Member Info!");
	return;
	}*/
	if (pmsg->MemberInfo.Rank == GUILD_STUDENT)
	{
		if (pGuild->CanAddStudent() == FALSE)
		{
			ASSERTMSG(0, "Different Guild Member_Student Info!");
			return;
		}
	}
	else
	{
		if (pGuild->CanAddMember() == FALSE)
		{
			ASSERTMSG(0, "Different Guild Member Info!");
			return;
		}
	}

	GUILDMEMBERINFO member;
	member.InitInfo(pmsg->MemberInfo.MemberIdx, pmsg->MemberInfo.MemberName, pmsg->MemberInfo.Memberlvl, pmsg->MemberInfo.Rank, TRUE);
	if (pGuild->AddMember(&member) == FALSE)
	{
		char buf[128];
		sprintf(buf, "Add Notify Failed! GuildIdx : %u, MemberIdx : %u", pmsg->GuildIdx, pmsg->MemberInfo.MemberIdx);
		ASSERTMSG(0, buf);
	}
}
void CGuildManager::Guild_Secede_Notify_To_Map(void* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	DoDeleteMember(pmsg->dwObjectID, pmsg->dwData, eGuild_Secede, "2000.01.01");
}
void CGuildManager::Guild_LevelUp_Notify_To_Map(void* pMsg)
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild == 0)
	{
		char buf[128];
		sprintf(buf, "LevelUp Notify Failed! GuildIdx: %d", pmsg->dwObjectID);
		ASSERTMSG(0, buf);
	}
	pGuild->LevelUp();
}
void CGuildManager::Guild_ChangeRank_Notify_To_Map(void* pMsg)
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild == 0)
	{
		char buf[128];
		sprintf(buf, "ChangeRank Notify Failed! GuildIdx: %d", pmsg->dwObjectID);
		ASSERTMSG(0, buf);
		return;
	}
	if (pGuild->ChangeMemberRank(pmsg->dwData, pmsg->bData) == FALSE)
	{
		ASSERTMSG(0, "Notify Change Rank Err, Must Check.");
		return;
	}
}
void CGuildManager::Guild_MemberLevel_Notify_To_Map(void* pMsg)
{
	MSG_DWORD3 * pmsg = (MSG_DWORD3*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwData1);
	if (pGuild == 0)
		return;
	pGuild->SetMemberLevel(pmsg->dwData2, (LEVELTYPE)pmsg->dwData3);
}
void CGuildManager::Guild_GiveNickName_Notify_To_Map(void* pMsg)
{
	SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->TargetIdx);
	if (pPlayer == 0)
		return;

	pPlayer->SetNickName(pmsg->NickName);

	// SEND TO 
	SEND_GUILD_NICKNAME msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_GIVENICKNAME_ACK;
	msg.TargetIdx = pmsg->TargetIdx;
	SafeStrCpy(msg.NickName, pmsg->NickName, MAX_GUILD_NICKNAME + 1);
	PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
}
void CGuildManager::Guild_LogInfo_Notify_To_Map(void* pMsg)
{
	SEND_GUILDMEMBER_LOGININFO * pmsg = (SEND_GUILDMEMBER_LOGININFO *)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild == 0)
		return;
	if (pGuild->IsMember(pmsg->MemberIdx) == FALSE)
		return;
	pGuild->SetLogInfo(pmsg->MemberIdx, pmsg->bLogIn);
}
void CGuildManager::Guild_Mark_UpDate_Syn(void* pMsg)
{
	MSG_GUILDMARK_IMG * pmsg = (MSG_GUILDMARK_IMG*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	CGuild* pGuild = GetGuild(pmsg->GuildIdx);
	if (pGuild == 0)
	{
		char buf[256];
		sprintf(buf, "GuildIdx: %u, CharacterIdx: %u", pmsg->GuildIdx, pmsg->dwObjectID);
		ASSERTMSG(0, buf);
		OnErr(pPlayer, MP_GUILDMARK_UPDATE_NACK, eGuildErr_Mark_NoGuild);
	}
	RegistMarkSyn(pmsg->GuildIdx, pmsg->imgData, pmsg->dwObjectID);
}
void CGuildManager::Guild_Mark_Request_Syn(void* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;

	RequestMark(pPlayer, pmsg->dwData);
}
void CGuildManager::Guild_Mark_Notify_To_Map(void* pMsg)
{
	MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->GuildIdx);
	if (pGuild == 0)
		return;
	DoRegistMark(pGuild, pmsg->MarkName, pmsg->imgData);
}
void CGuildManager::Guild_AddStudent_Syn(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (NULL == pMaster){ return; }

	AddStudentSyn(pMaster, pmsg->dwData);
}
void CGuildManager::Guild_Invite_As_Student_Accept(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;
	AddStudentResult(pPlayer, pmsg->dwData);
}
void CGuildManager::Guild_HuntedMonster_TotalCount_To_Map(void* pMsg)
{
	SEND_GUILD_HUNTEDMOB_INFO* pmsg = (SEND_GUILD_HUNTEDMOB_INFO*)pMsg;
	SetGuildHuntedMonsterTotalCountInfo(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
}
void CGuildManager::Guild_Point_UseInfo_To_Map(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild)
	{
		pGuild->SendGuildPointUseInfoToAll(pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4);
	}
}
void CGuildManager::Guild_Point_To_Map(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild)
	{
		pGuild->SetGuildPoint(pmsg->dwData);
	}
}
void CGuildManager::Guild_Point_Add_To_Map(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwObjectID);
	if (pGuild)
	{
		pGuild->SendGuildPointAddInfoToAll(pmsg->dwData1, pmsg->dwData2, pmsg->dwData3, pmsg->dwData4);
	}
}
void CGuildManager::Guild_PlusTime_Set_To_Map(void* pMsg)
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwData1);
	if (pGuild)
	{
		AddGuildPlustimeUsingGuildList(pGuild);
		pGuild->AddGuildPlustime(pmsg->dwData2, pmsg->dwData3);
	}
}
void CGuildManager::Guild_PlusTime_End_To_Map(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	DWORD GuildIdx = pmsg->dwObjectID;
	if (GuildIdx)
	{
		EndGuildPlustime(pmsg->dwObjectID, pmsg->dwData);
	}
}
void CGuildManager::Guild_PointUse_Syn(void* pMsg)
{
	MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;

	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());

	if (NULL == pGuild)
		return;

	GUILDMGR->UseGuildPoint(pPlayer, DWORD(pmsg->bData), pmsg->dwData);
}
void CGuildManager::Guild_Get_HuntedMonster_TotalCount_Syn(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)	return;

	DWORD GuildIdx = pPlayer->GetGuildIdx();

	MSG_DWORD2 rtMsg;
	rtMsg.Category = MP_GUILD;

	CGuild* pGuild = GetGuild(GuildIdx);
	if (NULL == pGuild)
	{
		rtMsg.Protocol = MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_NACK;
	}
	else
	{
		GUILDPOINT_INFO* pGuildPointInfo = pGuild->GetGuildPointInfo();
		rtMsg.dwData1 = pGuildPointInfo->GuildHuntedMonsterTotalCount;
		rtMsg.dwData2 = pGuildPointInfo->DBProcessTime;
		rtMsg.Protocol = MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_ACK;
	}
	pPlayer->SendMsg(&rtMsg, sizeof(MSG_DWORD2));
}
void CGuildManager::Guild_Munpa_Regist_Syn(void* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	PTRLISTPOS pos = m_GuildTraineeGuildList.GetHeadPosition();
	GUILD_TRAINEE_GUILDLIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_GUILDLIST*)m_GuildTraineeGuildList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->dwData)
			{
				MSG_DWORD ErrMsg;
				ZeroMemory(&ErrMsg, sizeof(MSG_DWORD));

				SetProtocol(&ErrMsg, MP_GUILD, MP_GUILD_MUNPA_REGIST_NACK);
				ErrMsg.dwData = 0;
				pPlayer->SendMsg(&ErrMsg, sizeof(ErrMsg));
				return;
			}
		}
	}

	GuildTraineeGuildInsert(pmsg->dwData);

	GUILD_TRAINEE_GUILDLIST* temp = new GUILD_TRAINEE_GUILDLIST;
	ZeroMemory(temp, sizeof(GUILD_TRAINEE_GUILDLIST));
	temp->dwGuildIdx = pmsg->dwData;
	temp->dwRank = rand() % 10;
	AddGuildTraineeGuildInfo(temp);

	MSG_GUILD_TRAINEE_GUILDLIST msg;
	SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNPA_REGIST_BROD);
	msg.sTraineeGuildList.dwGuildIdx = pmsg->dwData;
	msg.sTraineeGuildList.dwRank = temp->dwRank;
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&msg, sizeof(msg));

	MSG_DWORD OkMsg;
	ZeroMemory(&OkMsg, sizeof(MSG_DWORD));
	SetProtocol(&OkMsg, MP_GUILD, MP_GUILD_MUNPA_REGIST_ACK);
	OkMsg.dwData = 0;
	pPlayer->SendMsg(&OkMsg, sizeof(OkMsg));
}
void CGuildManager::Guild_Munpa_Regist_Brod(void* pMsg)
{
	MSG_GUILD_TRAINEE_GUILDLIST* pmsg = (MSG_GUILD_TRAINEE_GUILDLIST*)pMsg;
	if (!pmsg)
		return;

	PTRLISTPOS pos = m_GuildTraineeGuildList.GetHeadPosition();
	GUILD_TRAINEE_GUILDLIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_GUILDLIST*)m_GuildTraineeGuildList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->sTraineeGuildList.dwGuildIdx)
				return;
		}
	}

	GUILD_TRAINEE_GUILDLIST* temp = new GUILD_TRAINEE_GUILDLIST;
	ZeroMemory(temp, sizeof(GUILD_TRAINEE_GUILDLIST));
	temp->dwGuildIdx = pmsg->sTraineeGuildList.dwGuildIdx;
	temp->dwRank = pmsg->sTraineeGuildList.dwRank;
	AddGuildTraineeGuildInfo(temp);
}
void CGuildManager::Guild_Munpa_Regist_Intro_Brod(void* pMsg)
{
	MSG_MUNHAINFOUPDATE* pmsg = (MSG_MUNHAINFOUPDATE*)pMsg;

	GUILD_INTRO_INFO* temp;
	temp = new GUILD_INTRO_INFO;
	ZeroMemory(temp, sizeof(GUILD_INTRO_INFO));

	temp->dwGuildIdx = pmsg->Index;
	SafeStrCpy(temp->strIntro, pmsg->Intro, MAX_GUILD_INTRO + 1);

	AddGuildTraineeGuildIntroInfo(temp);
}
void CGuildManager::Guild_Munpa_Delete_Syn(void* pMsg)
{
	MSG_MUNHANOTEINFO * pmsg = (MSG_MUNHANOTEINFO*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	BOOL bFlag = FALSE;
	PTRLISTPOS pos = m_GuildTraineeGuildList.GetHeadPosition();
	GUILD_TRAINEE_GUILDLIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_GUILDLIST*)m_GuildTraineeGuildList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->dwData)
			{
				bFlag = TRUE;

				m_GuildTraineeGuildList.Remove(pTrainee);
			}
		}
	}

	pos = m_GuildTraineeGuildIntroInfoList.GetHeadPosition();
	GUILD_INTRO_INFO* pGuildIntro = NULL;
	while (pos)
	{
		pGuildIntro = (GUILD_INTRO_INFO*)m_GuildTraineeGuildIntroInfoList.GetNext(pos);
		if (pGuildIntro)
		{
			if (pGuildIntro->dwGuildIdx == pmsg->dwData)
			{
				bFlag = TRUE;

				m_GuildTraineeGuildIntroInfoList.Remove(pGuildIntro);
			}
		}
	}

	if (!bFlag)
	{
		MSG_DWORD ErrMsg;
		ZeroMemory(&ErrMsg, sizeof(MSG_DWORD));
		SetProtocol(&ErrMsg, MP_GUILD, MP_GUILD_MUNPA_DELETE_NACK);
		ErrMsg.dwData = 0;
		pPlayer->SendMsg(&ErrMsg, sizeof(ErrMsg));
		return;
	}

	pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pUserTrainee = NULL;
	while (pos)
	{
		pUserTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pUserTrainee)
		{
			if (pUserTrainee->dwGuildIdx == pmsg->dwData)
			{
				CGuild* pJoinGuild = (CGuild*)GetGuild(pUserTrainee->dwGuildIdx);

				MSG_MUNHA_NAME2_DWORD_NOTE Sendmsg;
				ZeroMemory(&Sendmsg, sizeof(MSG_MUNHA_NAME2_DWORD_NOTE));
				SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_MUNPA_DELETE_USER_ALRAM);
				Sendmsg.dwObjectID = pmsg->dwObjectID;
				SafeStrCpy(Sendmsg.Name1, pJoinGuild->GetGuildInfo()->MasterName, MAX_NAME_LENGTH + 1);
				SafeStrCpy(Sendmsg.Name2, pUserTrainee->strName, MAX_NAME_LENGTH + 1);
				SafeStrCpy(Sendmsg.Intro, pmsg->Intro, MAX_CHAT_LENGTH + 1);
				g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));

				m_GuildTraineeList.Remove(pUserTrainee);
			}
		}
	}

	GuildTraineeGuildDelete(pmsg->dwData);
	GuildTraineeDeletebyGuild(pmsg->dwData);

	MSG_DWORD msg;
	SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNPA_DELETE_BROD);
	msg.dwData = pmsg->dwData;
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&msg, sizeof(msg));

	MSG_DWORD OkMsg;
	ZeroMemory(&OkMsg, sizeof(MSG_DWORD));
	SetProtocol(&OkMsg, MP_GUILD, MP_GUILD_MUNPA_DELETE_ACK);
	OkMsg.dwData = 0;
	pPlayer->SendMsg(&OkMsg, sizeof(OkMsg));
}
void CGuildManager::Guild_Munpa_Delete_Brod(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	if (!pmsg)
		return;

	PTRLISTPOS pos = m_GuildTraineeGuildList.GetHeadPosition();
	GUILD_TRAINEE_GUILDLIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_GUILDLIST*)m_GuildTraineeGuildList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->dwData)
			{
				m_GuildTraineeGuildList.Remove(pTrainee);
			}
		}
	}

	pos = m_GuildTraineeGuildIntroInfoList.GetHeadPosition();
	GUILD_INTRO_INFO* pGuildIntro = NULL;
	while (pos)
	{
		pGuildIntro = (GUILD_INTRO_INFO*)m_GuildTraineeGuildIntroInfoList.GetNext(pos);
		if (pGuildIntro)
		{
			if (pGuildIntro->dwGuildIdx == pmsg->dwData)

			{
				m_GuildTraineeGuildIntroInfoList.Remove(pGuildIntro);
			}
		}
	}

	pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pUserTrainee = NULL;
	while (pos)
	{
		pUserTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pUserTrainee->dwGuildIdx == pmsg->dwData)
			{
				m_GuildTraineeList.Remove(pUserTrainee);
			}
		}
	}
}
void CGuildManager::Guild_Munpa_Catalog_Syn(void* pMsg)
{
	MSG_WORD * pmsg = (MSG_WORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	WORD wIndex = pmsg->wData;
	if (wIndex <= 0)
		wIndex = 1;

	MSG_MUNHAINFO SendMsg;
	ZeroMemory(&SendMsg, sizeof(MSG_MUNHAINFO));
	SetProtocol(&SendMsg, MP_GUILD, MP_GUILD_MUNPA_CATALOG_ACK);
	SendMsg.Index = wIndex;
	SendMsg.MaxCount = m_GuildTraineeGuildList.GetCount();

	PTRLISTPOS pos = NULL;
	GUILD_TRAINEE_GUILDLIST* pTemp = NULL;

	int count = (wIndex - 1) * 4;
	for (int i = 0; i < 4; i++)
	{
		if (count + i < SendMsg.MaxCount)
		{
			pos = m_GuildTraineeGuildList.FindIndex(count + i);
			pTemp = (GUILD_TRAINEE_GUILDLIST*)m_GuildTraineeGuildList.GetAt(pos);

			CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild(pTemp->dwGuildIdx);
			if (!pGuild)
				return;

			SendMsg.sMunHa_Info[i].nMunpaIdx = pTemp->dwGuildIdx;
			SendMsg.sMunHa_Info[i].nMunhaCount = pGuild->GetStudentNum();
			//					SendMsg.sMunHa_Info[i].nMunpaRank = pTemp->dwRank;
			SendMsg.sMunHa_Info[i].nMunhaGuildMark = pGuild->GetMarkName();
			SafeStrCpy(SendMsg.sMunHa_Info[i].strName, pGuild->GetGuildInfo()->GuildName, MAX_GUILD_NAME + 1);

			PTRLISTPOS pos = m_GuildTraineeGuildIntroInfoList.GetHeadPosition();
			GUILD_INTRO_INFO* pGuildIntro = NULL;
			while (pos)
			{
				pGuildIntro = (GUILD_INTRO_INFO*)m_GuildTraineeGuildIntroInfoList.GetNext(pos);
				if (pGuildIntro)
				{
					if (pGuildIntro->dwGuildIdx == pTemp->dwGuildIdx)
					{
						SafeStrCpy(SendMsg.sMunHa_Info[i].strIntro, pGuildIntro->strIntro, MAX_GUILD_INTRO + 1);
						break;
					}
				}
			}
		}
	}
	pPlayer->SendMsg(&SendMsg, sizeof(MSG_MUNHAINFO));
}
void CGuildManager::Guild_InfoUpdate_Syn(void* pMsg)
{
	MSG_MUNHAINFOUPDATE * pmsg = (MSG_MUNHAINFOUPDATE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	MSG_WORD Errmsg;
	SetProtocol(&Errmsg, MP_GUILD, MP_GUILD_INFOUPDATE_NACK);

	if (pPlayer->GetGuildMemberRank() != GUILD_MASTER)
	{
		Errmsg.wData = 726;
		pPlayer->SendMsg(&Errmsg, sizeof(Errmsg));
		return;
	}

	int len = strlen(pmsg->Intro);
	if (len > MAX_GUILD_INTRO)
	{
		Errmsg.wData = 32;
		pPlayer->SendMsg(&Errmsg, sizeof(Errmsg));
		return;
	}

	GuildTraineeGuildIntroUpdate(pmsg->Index, pmsg->Intro);

	PTRLISTPOS pos = m_GuildTraineeGuildIntroInfoList.GetHeadPosition();
	GUILD_INTRO_INFO* pGuildIntro = NULL;
	while (pos)
	{
		pGuildIntro = (GUILD_INTRO_INFO*)m_GuildTraineeGuildIntroInfoList.GetNext(pos);
		if (pGuildIntro)

		{
			if (pGuildIntro->dwGuildIdx == pmsg->Index)
			{
				SafeStrCpy(pGuildIntro->strIntro, pmsg->Intro, MAX_GUILD_INTRO + 1);
				break;
			}
		}
	}

	MSG_MUNHAINFOUPDATE brodmsg;
	SetProtocol(&brodmsg, MP_GUILD, MP_GUILD_INFOUPDATE_BROD);
	brodmsg.Index = pmsg->Index;
	SafeStrCpy(brodmsg.Intro, pmsg->Intro, MAX_GUILD_INTRO + 1);
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&brodmsg, sizeof(brodmsg));


	MSG_WORD okmsg;
	SetProtocol(&okmsg, MP_GUILD, MP_GUILD_INFOUPDATE_ACK);
	okmsg.wData = 0;
	pPlayer->SendMsg(&okmsg, sizeof(okmsg));
}
void CGuildManager::Guild_InfoUpdate_Brod(void* pMsg)
{
	MSG_MUNHAINFOUPDATE* pmsg = (MSG_MUNHAINFOUPDATE*)pMsg;
	if (!pmsg)
		return;

	PTRLISTPOS pos = m_GuildTraineeGuildIntroInfoList.GetHeadPosition();
	GUILD_INTRO_INFO* pGuildIntro = NULL;
	while (pos)
	{
		pGuildIntro = (GUILD_INTRO_INFO*)m_GuildTraineeGuildIntroInfoList.GetNext(pos);
		if (pGuildIntro)
		{
			if (pGuildIntro->dwGuildIdx == pmsg->Index)
			{
				SafeStrCpy(pGuildIntro->strIntro, pmsg->Intro, MAX_GUILD_INTRO + 1);
				break;
			}
		}
	}
}
void CGuildManager::Guild_Munpa_Join_Syn(void* pMsg)
{
	MSG_MUNHANOTEINFO * pmsg = (MSG_MUNHANOTEINFO*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild(pmsg->dwData);
	if (!pGuild)
		return;

	int errorCode = 0;
	if (pPlayer->GetLevel() >= 40)
		errorCode = 1;
	if (pPlayer->GetGuildMemberRank() > GUILD_NOTMEMBER)
		errorCode = 2;

	int MunpaCount = 0;
	DWORD dwJoinTime = 0;
	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->dwData)
			{
				if (++MunpaCount >= 50)
				{
					errorCode = 3;
					break;
				}
			}
			if (pTrainee->dwUserIdx == pmsg->dwObjectID)
			{
				errorCode = 4;
				dwJoinTime = pTrainee->dwTime;
				break;
			}
		}
	}

	if (errorCode)
	{
		MSG_MUNPAJOINERROR ErrMsg;
		ZeroMemory(&ErrMsg, sizeof(MSG_MUNPAJOINERROR));
		SetProtocol(&ErrMsg, MP_GUILD, MP_GUILD_MUNPA_JOIN_NACK);
		ErrMsg.errorCode = errorCode;
		ErrMsg.errorTime = dwJoinTime;
		SafeStrCpy(ErrMsg.strName, pGuild->GetGuildName(), MAX_GUILD_NAME + 1);
		pPlayer->SendMsg(&ErrMsg, sizeof(ErrMsg));
		return;
	}

	MSG_NAME rtMsg;
	SetProtocol(&rtMsg, MP_GUILD, MP_GUILD_MUNPA_JOIN_ACK);
	SafeStrCpy(rtMsg.Name, pGuild->GetGuildName(), MAX_GUILD_NAME + 1);
	pPlayer->SendMsg(&rtMsg, sizeof(rtMsg));

	MSG_MUNHA_NAME2_DWORD_NOTE Sendmsg;
	SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_MUNPA_JOIN_SYN);
	Sendmsg.dwObjectID = pmsg->dwObjectID;
	Sendmsg.dwData = pGuild->GetMasterIdx();
	SafeStrCpy(Sendmsg.Name1, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
	SafeStrCpy(Sendmsg.Name2, pGuild->GetGuildInfo()->MasterName, MAX_NAME_LENGTH + 1);
	SafeStrCpy(Sendmsg.Intro, pmsg->Intro, MAX_CHAT_LENGTH + 1);
	g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));

	pGuild->SendMsgAgentStudentJoin(pmsg->dwObjectID, pPlayer->GetObjectName(), pmsg->Intro);

	stTIME curTime;
	curTime.value = GetCurTime();
	curTime.SetTime(0, curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	GuildTraineeInsert(pmsg->dwData, pmsg->dwObjectID, pPlayer->GetObjectName(), curTime.value);

	GUILD_TRAINEE_LIST* temp = new GUILD_TRAINEE_LIST;
	ZeroMemory(temp, sizeof(GUILD_TRAINEE_LIST));
	temp->dwGuildIdx = pmsg->dwData;
	temp->dwUserIdx = pmsg->dwObjectID;
	SafeStrCpy(temp->strName, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
	temp->dwTime = curTime.value;
	AddGuildTraineeInfo(temp);

	MSG_GUILD_TRAINEE_LIST msg;
	SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNPA_JOIN_BROD);
	msg.sTraineeList.dwGuildIdx = pmsg->dwData;
	msg.sTraineeList.dwUserIdx = pmsg->dwObjectID;
	SafeStrCpy(msg.sTraineeList.strName, pPlayer->GetObjectName(), MAX_NAME_LENGTH + 1);
	msg.sTraineeList.dwTime = curTime.value;
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&msg, sizeof(msg));
}
void CGuildManager::Guild_Munpa_Join_Brod(void* pMsg)
{
	MSG_GUILD_TRAINEE_LIST* pmsg = (MSG_GUILD_TRAINEE_LIST*)pMsg;
	if (!pmsg)
		return;

	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee = NULL;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwUserIdx == pmsg->sTraineeList.dwUserIdx)
				return;
		}
	}

	GUILD_TRAINEE_LIST* temp = new GUILD_TRAINEE_LIST;
	ZeroMemory(temp, sizeof(GUILD_TRAINEE_LIST));
	temp->dwGuildIdx = pmsg->sTraineeList.dwGuildIdx;
	temp->dwUserIdx = pmsg->sTraineeList.dwUserIdx;
	SafeStrCpy(temp->strName, pmsg->sTraineeList.strName, MAX_NAME_LENGTH + 1);
	temp->dwTime = pmsg->sTraineeList.dwTime;
	AddGuildTraineeInfo(temp);
}
void CGuildManager::Guild_Munha_Catalog_Syn(void* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	MSG_MUNHACATALOG rtMsg;
	ZeroMemory(&rtMsg, sizeof(MSG_MUNHACATALOG));
	SetProtocol(&rtMsg, MP_GUILD, MP_GUILD_MUNHA_CATALOG_ACK);
	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwGuildIdx == pmsg->dwData)
			{
				rtMsg.Value[rtMsg.dwCnt].dwMunha_Index = pTrainee->dwUserIdx;
				SafeStrCpy(rtMsg.Value[rtMsg.dwCnt].strName, pTrainee->strName, MAX_NAME_LENGTH + 1);

				rtMsg.dwCnt++;
			}
		}
	}

	if (rtMsg.dwCnt <= 50)
	{
		pPlayer->SendMsg(&rtMsg, rtMsg.GetSize());
	}
	else
	{
		MSG_DWORD ErrMsg;
		ZeroMemory(&ErrMsg, sizeof(MSG_DWORD));
		SetProtocol(&ErrMsg, MP_GUILD, MP_GUILD_MUNHA_CATALOG_NACK);
		ErrMsg.dwData = 0;
		pPlayer->SendMsg(&ErrMsg, sizeof(ErrMsg));
		return;
	}
}
void CGuildManager::Guild_Munha_Delete_Syn(void* pMsg)
{
	MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	GuildTraineeDelete(pmsg->dwData1);

	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwUserIdx == pmsg->dwData1)
			{
				m_GuildTraineeList.Remove(pTrainee);
			}
		}
	}

	MSG_DWORD msg;
	SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_BROD);
	msg.dwData = pmsg->dwData1;
	PACKEDDATA_OBJ->SendToBroadCastMapServer(&msg, sizeof(msg));

	MSG_DWORD2 rtmsg;
	SetProtocol(&rtmsg, MP_GUILD, MP_GUILD_MUNHA_DELETE_ACK);
	rtmsg.dwData1 = pmsg->dwData1;
	rtmsg.dwData2 = pmsg->dwData2;
	pPlayer->SendMsg(&rtmsg, sizeof(rtmsg));
}
void CGuildManager::Guild_Munha_Delete_Brod(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	if (!pmsg)
		return;

	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwUserIdx == pmsg->dwData)
			{
				m_GuildTraineeList.Remove(pTrainee);
			}
		}
	}
}
void CGuildManager::Guild_Munha_UserDelete_Syb(void* pMsg)
{
	MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
	DWORD dwGuildIdx = 0;

	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwUserIdx == pmsg->dwData)
			{
				dwGuildIdx = pTrainee->dwGuildIdx;
				m_GuildTraineeList.Remove(pTrainee);
				break;
			}
		}
	}

	CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild(dwGuildIdx);
	if (!pGuild)
		return;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pGuild->GetGuildInfo()->MasterIdx);
	if (pPlayer)
	{
		MSG_NAME msg;
		SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNHA_USERDELETE_MASTER_ALRAM);
		SafeStrCpy(msg.Name, pTrainee->strName, MAX_NAME_LENGTH);
		pPlayer->SendMsg(&msg, sizeof(MSG_NAME));
	}

	pos = pGuild->GetMemberList()->GetHeadPosition();
	while (pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)pGuild->GetMemberList()->GetNext(pos);

		if (pInfo)
		{
			if (pInfo->Rank == GUILD_SENIOR || pInfo->Rank == GUILD_VICEMASTER)
			{
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pInfo->MemberIdx);
				if (!pPlayer)
					return;

				MSG_NAME msg;
				SetProtocol(&msg, MP_GUILD, MP_GUILD_MUNHA_USERDELETE_MASTER_ALRAM);
				SafeStrCpy(msg.Name, pTrainee->strName, MAX_NAME_LENGTH);
				pPlayer->SendMsg(&msg, sizeof(MSG_NAME));
			}
		}
	}
}
void CGuildManager::Guild_Munha_Accept_Syn(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (pPlayer == 0)
		return;


	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	BOOL bCheck = FALSE;
	while (pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if (pTrainee)
		{
			if (pTrainee->dwUserIdx == pmsg->dwData1)
				bCheck = TRUE;
		}
	}
	if (!bCheck)
	{
		MSG_DWORD rtMsg;
		ZeroMemory(&rtMsg, sizeof(MSG_DWORD));
		SetProtocol(&rtMsg, MP_GUILD, MP_GUILD_MUNHA_ACCEPT_NACK);
		rtMsg.dwObjectID = pmsg->dwObjectID;
		rtMsg.dwData = 0;
		pPlayer->SendMsg(&rtMsg, sizeof(MSG_DWORD));
	}

	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData1);
	if (pTarget != 0)
	{
		AddStudentResult(pTarget, pmsg->dwObjectID);
	}
	else
	{
		AddStudentResult(pmsg->dwData1, pmsg->dwObjectID);
	}

	MSG_DWORD2 rtMsg;
	ZeroMemory(&rtMsg, sizeof(MSG_DWORD2));
	SetProtocol(&rtMsg, MP_GUILD, MP_GUILD_MUNHA_ACCEPT_ACK);
	rtMsg.dwObjectID = pmsg->dwObjectID;
	rtMsg.dwData1 = pmsg->dwData1;
	rtMsg.dwData2 = pmsg->dwData2;
	pPlayer->SendMsg(&rtMsg, sizeof(MSG_DWORD2));
}
void CGuildManager::Guild_Student_LvUp_CountInfo(void* pMsg)
{
	MSG_DWORD2BYTE* pmsg = (MSG_DWORD2BYTE*)pMsg;
	CGuild* pGuild = GetGuild(pmsg->dwData1);
	if (!pGuild)
	{
		char buf[128] = { 0, };
		sprintf(buf, "Err! MP_GUILD_STUDENT_LVUP_COUNTINFO: pGuild is NULL. GuildIdx %d", pmsg->dwData1);
		ASSERTMSG(0, buf);
		return;
	}

	pGuild->SetStudentLvUpCountInfo(pmsg->dwData2, pmsg->bData);
}
void CGuildManager::Guild_CelebrationMoney_Master_Checked(void* pMsg)
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	DWORD GuildIdx = pmsg->dwData1;
	DWORD MasterID = pmsg->dwData2;

	CGuild* pGuild = GetGuild(pmsg->dwData1);
	if (NULL == pGuild)
	{
		ASSERT(0);
		return;
	}
	GuildStudentLvUpCtInit(GuildIdx, MasterID);
}
void CGuildManager::Guild_CelebrationMoney_Notify(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CGuild* pGuild = GetGuild(pmsg->dwData);
	if (NULL == pGuild)
	{
		ASSERT(0);
		return;
	}
	pGuild->SetStudentLvUpCountInfo(0, 0);

	pGuild->SendMsgToAll(pmsg, sizeof(*pmsg));
}
void CGuildManager::Guild_CelebrationMoney_To_Guildwear(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CGuild* pGuild = GetGuild(pmsg->dwData);
	if (!pGuild)
	{
		ASSERT(0);
		return;
	}

	PutInGuildCerabrationMoney(pGuild, m_GuildPointRule.GUILD_GIFTMONEY);
}
void CGuildManager::Guild_SOS_Send_Syn(void* pMsg)
{
	MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	if (pmsg->dwData1 == pPlayer->GetID())
	{
		MSG_DWORD rtMsg;
		SetProtocol(&rtMsg, MP_GUILD, MP_GUILD_SOS_SEND_ALRAM_NACK);
		rtMsg.dwData = 3;
		pPlayer->SendMsg(&rtMsg, sizeof(rtMsg));
		return;
	}

	MSG_DWORD4 Sendmsg;
	SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_SOS_SEND_SYN);
	Sendmsg.dwObjectID = pmsg->dwObjectID;
	Sendmsg.dwData1 = pmsg->dwData1;
	Sendmsg.dwData2 = pmsg->dwData2;
	Sendmsg.dwData3 = pmsg->dwData3;
	Sendmsg.dwData4 = pmsg->dwData4;	
	g_Network.Broadcast2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
}
void CGuildManager::Guild_SOS_Send_Cancel(void* pMsg)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	pPlayer->SetSOSState(0);
}
void CGuildManager::Guild_SOS_Send_Die_Check(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	MSG_DWORD Sendmsg;
	SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK);
	Sendmsg.dwObjectID = pmsg->dwObjectID;
	Sendmsg.dwData = pmsg->dwData;
	g_Network.Broadcast2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
}
void CGuildManager::Guild_SOS_Send_Die_Check_ToMap(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwData);
	if (!pPlayer)
		return;

	MSG_DWORD2 Sendmsg;
	SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK_TOAGENT);
	Sendmsg.dwObjectID = pmsg->dwObjectID;
	Sendmsg.dwData1 = pmsg->dwData;

	if (pPlayer->GetState() == eObjectState_Die)
	{
		Sendmsg.dwData2 = 1;
	}
	else
	{
		Sendmsg.dwData2 = 2;
	}
	g_Network.Broadcast2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
}
void CGuildManager::Guild_SOS_Send_No(void* pMsg)
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if (!pPlayer)
		return;

	MSG_DWORD Sendmsg;
	SetProtocol(&Sendmsg, MP_GUILD, MP_GUILD_SOS_SEND_NO);
	Sendmsg.dwObjectID = pmsg->dwObjectID;
	Sendmsg.dwData = pmsg->dwData;
	g_Network.Broadcast2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
}
void CGuildManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_GUILD_CREATE_SYN:						Guild_Create_Syn(pMsg);					break;
	case MP_GUILD_BREAKUP_SYN:						Guild_BreakUp_Syn(pMsg);				break;
	case MP_GUILD_DELETEMEMBER_SYN:					Guild_DeleteMember_Syn(pMsg);			break;
	case MP_GUILD_ADDMEMBER_SYN:					Guild_AddMember_Syn(pMsg);				break;
	case MP_GUILD_INVITE_ACCEPT:					Guild_Invite_Accept(pMsg);				break;
	case MP_GUILD_INVITE_DENY:						Guild_Invite_Deny(pMsg);				break;
	case MP_GUILD_SECEDE_SYN:						Guild_Secede_Syn(pMsg);					break;
	case MP_GUILD_LEVELUP_SYN:						Guild_LevelUp_Syn(pMsg);				break;
	case MP_GUILD_CHANGERANK_SYN:					Guild_ChangeRank_Syn(pMsg);				break;
	case MP_GUILD_GIVENICKNAME_SYN:					Guild_GiveNickName_Syn(pMsg);			break;
	case MP_GUILD_PUTIN_MONEY_SYN:					Guild_PutIn_Money_Syn(pMsg);			break;
	case MP_GUILD_PUTOUT_MONEY_SYN:					Guild_PutOut_Money_Syn(pMsg);			break;
	case MP_GUILD_CREATE_NOTIFY_TO_MAP:				Guild_Create_Notify_To_Map(pMsg);		break;
	case MP_GUILD_BREAKUP_NOTIFY_TO_MAP:			Guild_BreakUp_Notify_To_Map(pMsg);		break;
	case MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP:		Guild_DeleteMember_Notify_To_Map(pMsg);	break;
	case MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP:			Guild_AddMember_Notify_To_Map(pMsg);	break;
	case MP_GUILD_SECEDE_NOTIFY_TO_MAP:				Guild_Secede_Notify_To_Map(pMsg);		break;
	case MP_GUILD_LEVELUP_NOTIFY_TO_MAP:			Guild_LevelUp_Notify_To_Map(pMsg);		break;
	case MP_GUILD_CHANGERANK_NOTIFY_TO_MAP:			Guild_ChangeRank_Notify_To_Map(pMsg);	break;
	case MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP:		Guild_MemberLevel_Notify_To_Map(pMsg);	break;
	case MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP:		Guild_GiveNickName_Notify_To_Map(pMsg);	break;
	case MP_GUILD_LOGINFO_NOTIFY_TO_MAP:			Guild_LogInfo_Notify_To_Map(pMsg);		break;
	case MP_GUILDMARK_UPDATE_SYN:					Guild_Mark_UpDate_Syn(pMsg);			break;
	case MP_GUILDMARK_REQUEST_SYN:					Guild_Mark_Request_Syn(pMsg);			break;
	case MP_GUILDMARK_NOTIFY_TO_MAP:				Guild_Mark_Notify_To_Map(pMsg);			break;
	case MP_GUILD_CHANGE_LOCATION_SYN:				MsgChangeLocationSyn(pMsg);				break;
	case MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP:	MsgChangeLocationNotifyToMap(pMsg);		break;
	case MP_GUILD_NOTE_SYN:							MsgGuildNoteSyn(pMsg);					break;
	case MP_GUILD_NOTICE_SYN:						MsgGuildNoticeSyn(pMsg);				break;
	case MP_GUILD_NOTICE_NOTIFY:					MsgGuildNoticeNotify(pMsg);				break;
#ifndef _HK_LOCAL_	
	case MP_GUILD_ADDSTUDENT_SYN:					Guild_AddStudent_Syn(pMsg);				break;
	case MP_GUILD_INVITE_AS_STUDENT_ACCEPT:			Guild_Invite_As_Student_Accept(pMsg);	break;
	case MP_GUILD_HUNTEDMONSTER_TOTALCOUNT_TO_MAP:	Guild_HuntedMonster_TotalCount_To_Map(pMsg); break;
	case MP_GUILD_POINT_USEINFO_TO_MAP:				Guild_Point_UseInfo_To_Map(pMsg);		break;
	case MP_GUILD_POINT_TO_MAP:						Guild_Point_To_Map(pMsg);				break;
	case MP_GUILD_POINT_ADD_TO_MAP:					Guild_Point_Add_To_Map(pMsg);			break;
	case MP_GUILD_PLUSTIME_SET_TO_MAP:				Guild_PlusTime_Set_To_Map(pMsg);		break;
	case MP_GUILD_PLUSTIME_END_TO_MAP:				Guild_PlusTime_End_To_Map(pMsg);		break;
	case MP_GUILD_POINTUSE_SYN:						Guild_PointUse_Syn(pMsg);				break;
	case MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_SYN:	Guild_Get_HuntedMonster_TotalCount_Syn(pMsg); break;
	case MP_GUILD_MUNPA_REGIST_SYN:					Guild_Munpa_Regist_Syn(pMsg);			break;
	case MP_GUILD_MUNPA_REGIST_BROD:				Guild_Munpa_Regist_Brod(pMsg);			break;
	case MP_GUILD_MUNPA_REGIST_INTRO_BROD:			Guild_Munpa_Regist_Intro_Brod(pMsg);	break;
	case MP_GUILD_MUNPA_DELETE_SYN:					Guild_Munpa_Delete_Syn(pMsg);			break;
	case MP_GUILD_MUNPA_DELETE_BROD:				Guild_Munpa_Delete_Brod(pMsg);			break;
	case MP_GUILD_MUNPA_CATALOG_SYN:				Guild_Munpa_Catalog_Syn(pMsg);			break;
	case MP_GUILD_INFOUPDATE_SYN:					Guild_InfoUpdate_Syn(pMsg);				break;
	case MP_GUILD_INFOUPDATE_BROD:					Guild_InfoUpdate_Brod(pMsg);			break;
	case MP_GUILD_MUNPA_JOIN_SYN:					Guild_Munpa_Join_Syn(pMsg);				break;
	case MP_GUILD_MUNPA_JOIN_BROD:					Guild_Munpa_Join_Brod(pMsg);			break;
	case MP_GUILD_MUNHA_CATALOG_SYN:				Guild_Munha_Catalog_Syn(pMsg);			break;
	case MP_GUILD_MUNHA_DELETE_SYN:					Guild_Munha_Delete_Syn(pMsg);			break;
	case MP_GUILD_MUNHA_DELETE_BROD:				Guild_Munha_Delete_Brod(pMsg);			break;
	case MP_GUILD_MUNHA_USERDELETE_SYN:				Guild_Munha_UserDelete_Syb(pMsg);		break;
	case MP_GUILD_MUNHA_ACCEPT_SYN:					Guild_Munha_Accept_Syn(pMsg);			break;
	case MP_GUILD_STUDENT_LVUP_COUNTINFO:			Guild_Student_LvUp_CountInfo(pMsg);		break;
	case MP_GUILD_CELEBRATIONMONEY_MASTER_CHECKED:	Guild_CelebrationMoney_Master_Checked(pMsg);	break;
	case MP_GUILD_CELEBRATIONMONEY_NOTIFY:			Guild_CelebrationMoney_Notify(pMsg);	break;
	case MP_GUILD_CELEBRATIONMONEY_TO_GUILDWEAR:	Guild_CelebrationMoney_To_Guildwear(pMsg);	break;
	case MP_GUILD_SOS_SEND_SYN:						Guild_SOS_Send_Syn(pMsg);				break;
	case MP_GUILD_SOS_SEND_CANCEL:					Guild_SOS_Send_Cancel(pMsg);			break;
	case MP_GUILD_SOS_SEND_DIE_CHECK:				Guild_SOS_Send_Die_Check(pMsg);			break;
	case MP_GUILD_SOS_SEND_DIE_CHECK_TOMAP:			Guild_SOS_Send_Die_Check_ToMap(pMsg);	break;
	case MP_GUILD_SOS_SEND_NO:						Guild_SOS_Send_No(pMsg);				break;
#endif
	default:
		{
			char buf[32];
			sprintf(buf, "Protocol : %d", Protocol);
			ASSERTMSG(0, buf);
		}
		break;
	}
}

CGuild* CGuildManager::RegistGuild(GUILDINFO* pGuildInfo, MONEYTYPE GuildMoney)
{

	if(m_GuildRoomHashTable.GetData(pGuildInfo->GuildIdx))
	{
		char buf[64];
		sprintf(buf, "Error! GuildIdx : %u", pGuildInfo->GuildIdx);
		ASSERTMSG(0, buf);
		return NULL;
	}
	CGuild* pGuild = new CGuild(pGuildInfo, GuildMoney);
	m_GuildRoomHashTable.Add(pGuild, pGuildInfo->GuildIdx);
	return pGuild;
}

void CGuildManager::CreateGuildSyn(CPlayer* pPlayer, char* GuildName, char* Intro)
{
	if(pPlayer->GetGuildIdx())
	{
		ASSERT(0);
		OnErr(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Err);
	}
	if((ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, pPlayer->GetAbilityGroup())  < 1) || (pPlayer->GetLevel() < GUILD_CREATELEVEL))
	{
		ASSERT(0);
		OnErr(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Err);
	}
	if(CanEntryGuild(pPlayer->GetGuildCanEntryDate()) == FALSE)
	{
		ASSERT(0);
		OnErr(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Err);
	}
	if(IsVaildGuildName(GuildName) == FALSE)
	{
		ASSERT(0);
		OnErr(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Create_Name);
	}
	if(strlen(Intro) > MAX_GUILD_INTRO)
	{
		ASSERT(0);
		OnErr(pPlayer, MP_GUILD_CREATE_NACK, eGuildErr_Create_Intro);
	}

	GuildCreate(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetLevel(), GuildName, Intro);
}

void CGuildManager::CreateGuildResult(LEVELTYPE MasterLvl, GUILDINFO* pGuildInfo)
{
	CGuild* pGuild = RegistGuild(pGuildInfo, 0);
	if(pGuild == 0)
		return;
	
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pGuildInfo->MasterIdx);
	if(pPlayer)
	{
		pPlayer->SetGuildInfo(pGuildInfo->GuildIdx, GUILD_MASTER, pGuildInfo->GuildName, 0);
		
		//ack
		SEND_GUILD_CREATE_ACK msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_CREATE_ACK;
		msg.GuildIdx = pGuildInfo->GuildIdx;
		SafeStrCpy(msg.GuildName, pGuildInfo->GuildName, MAX_GUILD_NAME+1);
		pPlayer->SendMsg(&msg, sizeof(msg));

		SendGuildName(pPlayer, pGuildInfo->GuildIdx, pGuildInfo->GuildName);
	}
	
	GUILDMEMBERINFO Memberinfo;
	Memberinfo.InitInfo(pGuildInfo->MasterIdx, pGuildInfo->MasterName, MasterLvl, GUILD_MASTER, TRUE);
	pGuild->AddMember(&Memberinfo);

	SEND_GUILD_CREATE_NOTIFY notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_CREATE_NOTIFY_TO_MAP;
	notify.GuildInfo.GuildIdx = pGuildInfo->GuildIdx;
	notify.GuildInfo.GuildLevel = GUILD_1LEVEL;
	SafeStrCpy(notify.GuildInfo.MasterName, pGuildInfo->MasterName, MAX_NAME_LENGTH+1);
	SafeStrCpy(notify.GuildInfo.GuildName, pGuildInfo->GuildName, MAX_GUILD_NAME+1);
	memset(notify.GuildInfo.GuildNotice, 0, MAX_GUILD_NOTICE+1);
	notify.GuildInfo.MapNum = GAMERESRCMNGR->GetLoadMapNum();
	notify.GuildInfo.MasterIdx = pGuildInfo->MasterIdx;
	notify.GuildInfo.UnionIdx = 0;
	notify.MasterLvl = MasterLvl;
	notify.GuildInfo.MarkName = pGuildInfo->MarkName;

	notify.GuildInfo.LvUpCounter = 0;
	notify.GuildInfo.bNeedMasterChecking = FALSE;
	
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));

	LogGuild(pGuild->GetMasterIdx(), pGuild->GetIdx(), eLogGuild_GuildCreate);
}

void CGuildManager::BreakUpGuildSyn(CPlayer* pMaster)
{
	if(pMaster->GetGuildMemberRank() != GUILD_MASTER)
		OnErr(pMaster, MP_GUILD_BREAKUP_NACK, eGuildErr_BreakUp);

	GuildBreakUp(pMaster->GetGuildIdx(), pMaster->GetID());

	GTMGR->DeleteAddedGuild( pMaster->GetGuildIdx() );

	SIEGEWARMGR->BreakUpGuild( pMaster->GetGuildIdx() );
}

void CGuildManager::BreakUpGuildResult(CPlayer* pMaster, DWORD GuildIdx, char* EntryDate)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
	{
		ASSERTMSG(0, "Guild BreakUpResult - NoGuild Err");
		return;
	}
	DWORD MasterIdx = pGuild->GetMasterIdx();
	BYTE rt = DoBreakUp(GuildIdx);
	if(rt)
	{
		if(pMaster == 0)
			return;
		OnErr(pMaster, MP_GUILD_BREAKUP_NACK, rt);
	}

	LogGuild(MasterIdx, GuildIdx, eGuildLog_GuildBreakUp);

	if(pMaster)
	{
		EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint(pMaster->GetLevel());
		EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.05);
		pMaster->ReduceExpPoint(minusExp, eExpLog_LosebyBreakupGuild);
		
		SendGuildEntryDate(pMaster, EntryDate);

		MSGBASE notify;
		notify.Category = MP_GUILD;
		notify.Protocol = MP_GUILD_BREAKUP_NOTIFY_TO_MAP;
		notify.dwObjectID = pMaster->GetGuildIdx();
		g_Network.Send2AgentServer((char*)&notify, sizeof(notify));
	}
}

BYTE CGuildManager::DoBreakUp(DWORD GuildIdx)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(!pGuild)
	{
		ASSERT(0);
		return eGuildErr_BreakUp; 
	}

	if(pGuild->GetMarkName())
		DeleteMark(pGuild->GetMarkName());

	pGuild->BreakUp();

	delete pGuild;

	m_GuildRoomHashTable.Remove(GuildIdx);
	return 0;
}

CGuild* CGuildManager::GetGuild(DWORD GuildIdx)
{
	return m_GuildRoomHashTable.GetData(GuildIdx);
}

CGuild* CGuildManager::GetGuildFromMasterIdx( DWORD dwMasterIdx )
{
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while( pGuild = m_GuildRoomHashTable.GetData() )
	{
		if( pGuild->GetMasterIdx() == dwMasterIdx )
			return pGuild;
	}
	return NULL;
}

CGuildMark* CGuildManager::GetMark(MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = NULL;
	pMark = m_GuildMarkTable.GetData(MarkName);
	return pMark;
}

CGuild* CGuildManager::GetGuildIndex( char* GuildName )
{
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while( pGuild = m_GuildRoomHashTable.GetData() )
	{
		if( strcmp( pGuild->GetGuildName(), GuildName ) == 0 )
			return pGuild;
	}
	return NULL;
}

void CGuildManager::SendNackMsg(CPlayer* pPlayer, BYTE Protocol, BYTE errstate)
{
	MSG_BYTE nmsg;
	nmsg.Category = MP_GUILD;
	nmsg.Protocol = Protocol;
	nmsg.bData = errstate;
	pPlayer->SendMsg(&nmsg, sizeof(nmsg));
}

BOOL CGuildManager::IsVaildGuildName(char* GuildName)
{
	CGuild * pInfo;
	m_GuildRoomHashTable.SetPositionHead();
	while(pInfo = m_GuildRoomHashTable.GetData())
	{
		if(strcmp(pInfo->GetGuildName(), GuildName) == 0)
			return FALSE;
	}
	if(strlen(GuildName) > MAX_GUILD_NAME)
		return FALSE;
	return TRUE;
}

void CGuildManager::DeleteMemberSyn(CPlayer* pMaster, DWORD MemberIDX)
{
	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		OnErr(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember);
	}
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(!pGuild)
		OnErr(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember);
	if(pMaster->GetGuildMemberRank() < GUILD_MASTER)
		OnErr(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NothaveAuthority);
	if(!pGuild->IsMember(MemberIDX))
		OnErr(pMaster, MP_GUILD_DELETEMEMBER_NACK, eGuildErr_DeleteMember_NotMember);

	GuildDeleteMember(pMaster->GetGuildIdx(), MemberIDX);
}

void CGuildManager::DeleteMemberResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;

	BYTE err = DoDeleteMember(GuildIdx, MemberIDX, eGuild_Delete, EntryDay);
	if(err)
	{
		ASSERT(0);
		CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(pGuild->GetMasterIdx());
		if(pMaster == 0)
			return;
		OnErr(pMaster, MP_GUILD_DELETEMEMBER_NACK, err);
	}
	
	MSG_NAME_DWORD notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP;
	notify.dwObjectID = GuildIdx;
	notify.dwData = MemberIDX;
	SafeStrCpy(notify.Name, EntryDay, 11);
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));	

	
	LogGuild(MemberIDX, GuildIdx, eGuildLog_MemberBan);
}

BYTE CGuildManager::DoDeleteMember(DWORD GuildIDX, DWORD MemberIDX, BYTE bType, char* EntryDay)
{
	CGuild* pGuild = m_GuildRoomHashTable.GetData(GuildIDX);
	if(pGuild == 0)
	{
		ASSERT(0);
		return eGuildErr_Err;
	}
	if(pGuild->IsMember(MemberIDX) == FALSE)
	{
		ASSERT(0);
		return eGuildErr_DeleteMember;
	}

	if(pGuild->DeleteMember(MemberIDX, bType) == FALSE)
	{
		ASSERT(0);
		return eGuildErr_Err;
	}	

	CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(MemberIDX);
	if(pMember)
	{
		pMember->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
		pMember->SetNickName("");
		SendGuildEntryDate(pMember, EntryDay);	
		pMember->SetGuildUnionInfo(0,"",0);//[ÍË°ïºó²»ÄÜ¹¥»÷ÐÞ¸´][by:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019/1/10]
		pMember->ClearKillTimes();		   //[ÍË°ïºóÇå¿Õ»÷É±¼ÆÊý][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-9-4][17:13:13]
		MSG_DWORDBYTE msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_DELETEMEMBER_ACK;
		msg.dwData = MemberIDX;
		msg.bData = eGuild_Delete;
		pMember->SendMsg(&msg, sizeof(msg));

		MarkChange(pMember, 0, 0);
	}
	
	pGuild->SendDeleteMsg(MemberIDX, eGuild_Delete);
	return 0;
}

void CGuildManager::LoadMembers(GUILDMEMBERINFO_ID* pInfo)
{
	CGuild * pGuild = GetGuild(pInfo->GuildIdx);
	if(pGuild == 0)
	{
		char buf[64];
		sprintf(buf, "GuildIdx : %d, CharacterIdx : %d", pInfo->GuildIdx, pInfo->MemberInfo.MemberIdx);
		ASSERTMSG(0, buf);
		return;
	}
	pGuild->AddMember(&pInfo->MemberInfo);
}

void CGuildManager::LoadMark(MARKNAMETYPE MarkName, char* pImgData)
{
	if(m_GuildMarkTable.GetData(MarkName))
	{
		ASSERTMSG(0, "LoadMark");
		return;
	}
	char Img[GUILDMARK_BUFSIZE];
	char *pStr = (char*)pImgData;
	char tempBuf[3] = {0,};
	int curpos = 0;
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		strncpy(tempBuf,&pStr[curpos],2);
		Img[n] = HexToByte(tempBuf);
		curpos += 2;
	}
	CGuildMark* pMark = new CGuildMark;
	pMark->Init(MarkName, Img);
	
	m_GuildMarkTable.Add(pMark, MarkName);
}

void CGuildManager::AddMemberSyn(CPlayer* pMaster, DWORD TargetIdx)
{
	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_Err);
	}
	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(TargetIdx);
	if(pTarget == 0)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_NotPlayer);
	if(pTarget->GetGuildIdx())
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_OtherGuild);
	
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_Err);
	if(pMaster->GetGuildMemberRank() < GUILD_VICEMASTER)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_NothaveAuthority);
	if(pGuild->IsMember(TargetIdx) == TRUE)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_AlreadyMember);
	if(pGuild->CanAddMember() == FALSE)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_FullMember);
	if(CanEntryGuild(pTarget->GetGuildCanEntryDate()) == FALSE)
		OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_TargetNotDay);

	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_ADDMEMBER_INVITE;
	pMaster->SendMsg(&msg, sizeof(msg));

	SEND_GUILD_INVITE invite;
	invite.Category = MP_GUILD;
	invite.Protocol = MP_GUILD_INVITE;
	invite.MasterIdx = pMaster->GetID();
	SafeStrCpy(invite.MasterName, pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(invite.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	pTarget->SendMsg(&invite, sizeof(invite));
}

void CGuildManager::AddMemberResult(CPlayer* pTarget, DWORD MasterIdx)
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterIdx);
	if(pMaster == 0)
		OnErr(pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_InviteApt_NoMaster);
	
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_Err);
	
	if(pGuild->IsMember(pTarget->GetID()) == TRUE)
		OnErr(pTarget, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_Err);

	if(pGuild->CanAddMember() == FALSE)
		OnErr(pMaster, MP_GUILD_INVITE_ACCEPT_NACK, eGuildErr_AddMember_FullMember);

	GuildAddMember(pMaster->GetGuildIdx(), pTarget->GetID(), pGuild->GetGuildName(), GUILD_MEMBER);

	pTarget->SetGuildInfo(pMaster->GetGuildIdx(), GUILD_MEMBER, pGuild->GetGuildName(), pGuild->GetMarkName());
	MarkChange(pTarget, pMaster->GetGuildIdx(), pGuild->GetMarkName());

	GUILDMEMBERINFO member;
	member.InitInfo(pTarget->GetID(), pTarget->GetObjectName(), pTarget->GetLevel(), GUILD_MEMBER, TRUE);
	pGuild->AddMember(&member);
	pGuild->SetLogInfo(pTarget->GetID(), TRUE);

	SEND_GUILD_TOTALINFO tmsg;
	memset(&tmsg, 0, sizeof(SEND_GUILD_TOTALINFO));
	tmsg.Category = MP_GUILD;
	tmsg.Protocol = MP_GUILD_ACCEPT_ACK;
	tmsg.InitGuildInfo(pGuild->GetGuildInfo());
	tmsg.InitPointInfo(pGuild->GetGuildPointInfo());
	pGuild->GetTotalMember(tmsg.MemberInfo);
	tmsg.membernum = pGuild->GetMemberNum();
	pTarget->SendMsg(&tmsg, tmsg.GetMsgLength());

	SEND_GUILD_MEMBER_INFO notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
	notify.GuildIdx = pMaster->GetGuildIdx();
	SafeStrCpy(notify.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	notify.MemberInfo.MemberIdx = pTarget->GetID();
	notify.MemberInfo.Memberlvl = pTarget->GetLevel();
	SafeStrCpy(notify.MemberInfo.MemberName, pTarget->GetObjectName(), MAX_NAME_LENGTH+1);
	notify.MemberInfo.Rank = GUILD_MEMBER;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));

	GUILDFIELDWARMGR->AddPlayer( pTarget );

	if( pGuild->GetGuildUnionIdx() )
		GUILDUNIONMGR->AddPlayer( pTarget, pGuild->GetGuildUnionIdx() );		
	
	LogGuild(pTarget->GetID(), pGuild->GetIdx(), eGuildLog_MemberAdd);

	DeleteMunhaJoinInfo(pTarget->GetID());
}

void CGuildManager::DeleteMunhaJoinInfo(DWORD TargetIdx)
{
	DWORD dwGuildIdx = 0;
	BOOL bFlag = FALSE;
	PTRLISTPOS pos = GetGuildTraineeList()->GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee;
	while(pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)GetGuildTraineeList()->GetNext(pos);
		if( pTrainee )
		{
			if( pTrainee->dwUserIdx == TargetIdx)
			{
				bFlag = TRUE;
				GuildTraineeDelete(pTrainee->dwUserIdx);
				dwGuildIdx = pTrainee->dwGuildIdx;
				GetGuildTraineeList()->Remove(pTrainee);
				break;
			}
		}
	}

	if(!bFlag)
		return;

	MSG_DWORD msg;
	SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_BROD );
	msg.dwData = TargetIdx;
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

	CGuild* pGuild = (CGuild*)GUILDMGR->GetGuild( dwGuildIdx );
	if( !pGuild )
		return;

	MSG_NAME_DWORD Sendmsg;
	SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_OTHERJOIN_SYN );
	Sendmsg.dwObjectID = pGuild->GetGuildInfo()->MasterIdx;
	Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
	SafeStrCpy( Sendmsg.Name, pTrainee->strName, MAX_NAME_LENGTH+1 );
	g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));

	pos = pGuild->GetMemberList()->GetHeadPosition();
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)pGuild->GetMemberList()->GetNext(pos);

		if( pInfo )
		{
			if(pInfo->Rank == GUILD_SENIOR || pInfo->Rank == GUILD_VICEMASTER)
			{
				MSG_NAME_DWORD Sendmsg;
				SetProtocol( &Sendmsg, MP_GUILD, MP_GUILD_MUNHA_OTHERJOIN_SYN );
				Sendmsg.dwObjectID = pInfo->MemberIdx;
				Sendmsg.dwData = pGuild->GetGuildInfo()->GuildIdx;
				SafeStrCpy( Sendmsg.Name, pTrainee->strName, MAX_NAME_LENGTH+1 );
				g_Network.Send2AgentServer((char*)&Sendmsg, sizeof(Sendmsg));
			}
		}
	}
}

void CGuildManager::UserLogIn(CPlayer* pPlayer)
{
	if(pPlayer->GetGuildIdx() == 0)
		return;
	
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
	{
		ASSERTMSG(0, "UserLogIn - No Guild Info! ");
		return;
	}
	
	SetLogInfo(pGuild, pPlayer->GetID(), TRUE);

	pGuild->AddConnectingMapMemberCount(+1);
	
	SEND_GUILD_TOTALINFO msg;
	memset(&msg, 0, sizeof(SEND_GUILD_TOTALINFO));
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_INFO;
	msg.InitGuildInfo(pGuild->GetGuildInfo());
	msg.InitPointInfo(pGuild->GetGuildPointInfo());
	pGuild->GetTotalMember(msg.MemberInfo);
	
	msg.membernum = pGuild->GetMemberNum();
	
	pPlayer->SendMsg(&msg, msg.GetMsgLength());

	if( pGuild->GetGuildUnionIdx() )
		GUILDUNIONMGR->LoginPlayer( pPlayer, pGuild->GetGuildUnionIdx() );

	if( pGuild->IsMaster(pPlayer->GetID()) )
	{
		pGuild->SendMsgForMasterChecking();
	}
}

void CGuildManager::UserLogOut(CPlayer* pPlayer)
{
	if(pPlayer->GetGuildIdx() == 0)
		return;
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
		return;
	SetLogInfo(pGuild, pPlayer->GetID(), FALSE);

	pGuild->AddConnectingMapMemberCount(-1);
}

void CGuildManager::SecedeSyn(CPlayer* pPlayer)
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
	{
		OnErr(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_NoGuild);
	}
	if(pPlayer->GetGuildMemberRank() >= GUILD_MASTER)
	{
		OnErr(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
	}
	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		OnErr(pPlayer, MP_GUILD_SECEDE_NACK, eGuildErr_Err);
	}
	GuildSecedeMember(GuildIdx, pPlayer->GetID());
}

void CGuildManager::SecedeResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(MemberIDX);
	
	if(pPlayer)
	{		
		pPlayer->SetGuildInfo(0, GUILD_NOTMEMBER, "", 0);
		pPlayer->SetNickName("");
		SendGuildEntryDate(pPlayer, EntryDay);
		MarkChange(pPlayer, 0, 0);
		pPlayer->InitGuildUnionInfo(0,"",0);//[ÍË°ïºó¿ªºì²»ÄÜ¹¥»÷ÐÞ¸´][by:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019/1/10]
		pPlayer->ClearKillTimes();			//[ÍË°ïºóÇå¿Õ»÷É±¼ÆÊý][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-9-4][17:14:15]
		MSGBASE msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_SECEDE_ACK;
		pPlayer->SendMsg(&msg, sizeof(msg));
		
	}

	CGuild* pGuild = GetGuild(GuildIdx);

	if( pGuild == NULL )
	{
		return;
	}
	
	BOOL rt = pGuild->DeleteMember(MemberIDX, eGuild_Secede);
	if(rt == FALSE)
		return;	

	pGuild->SendDeleteMsg(MemberIDX, eGuildLog_MemberSecede);
	
	MSG_DWORD notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_SECEDE_NOTIFY_TO_MAP;
	notify.dwObjectID = GuildIdx;

	notify.dwData = MemberIDX;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));
	
	LogGuild(MemberIDX, GuildIdx, eGuildLog_MemberSecede);
}

void CGuildManager::SendChatMsg(DWORD GuildIdx, DWORD PlayerIdx, char* pMsg)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;

	TESTMSGID msg;
	msg.Category = MP_CHAT;
	msg.Protocol = MP_CHAT_GUILD;
	msg.dwSenderID = PlayerIdx;
	SafeStrCpy(msg.Msg, pMsg, MAX_CHAT_LENGTH+1);

	
	pGuild->SendMsgToAll(&msg, msg.GetMsgLength());
	
}

void CGuildManager::RegistMarkSyn(DWORD GuildIdx, char* ImgData, DWORD PlayerIdx)
{	
	GuildMarkRegist(GuildIdx, ImgData, PlayerIdx);
}

void CGuildManager::RegistMarkResult(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName, char * pImgData)
{	
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;
	pPlayer->SetGuildMarkName(MarkName);

	MSG_GUILDMARK_IMG msg;
	char *pStr = (char*)pImgData;
	char tempBuf[3] = {0,};
	int curpos = 0;
	for(int n=0;n<GUILDMARK_BUFSIZE;++n)
	{
		strncpy(tempBuf,&pStr[curpos],2);
		msg.imgData[n] = HexToByte(tempBuf);
		curpos += 2;
	}
	
	MarkChange(pPlayer, GuildIdx, MarkName);

	DoRegistMark(pGuild, MarkName, msg.imgData);
	msg.Category = MP_GUILD;
	msg.GuildIdx = GuildIdx;
	msg.MarkName = MarkName;
	msg.Protocol = MP_GUILDMARK_NOTIFY_TO_MAP;
	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
}

void CGuildManager::DoRegistMark(CGuild* pGuild, MARKNAMETYPE MarkName, char* imgData)
{
	pGuild->SetMarkName(MarkName);
	CGuildMark * pMark = new CGuildMark;
	pMark->Init(MarkName, imgData);
	m_GuildMarkTable.Add(pMark, MarkName);
}

int CGuildManager::convertCharToInt(char c)
{
	if('0' <= c && c <= '9')
		return c - '0';
	if('A' <= c && c <= 'F')
		return c - 'A' +10;
	if('a' <= c && c <= 'f')
		return c - 'a' +10;
	return 0;
}

BYTE CGuildManager::HexToByte(char* pStr)
{
	int n1 = convertCharToInt(pStr[0]);
	int n2 = convertCharToInt(pStr[1]);

	return n1 * 16 + n2;
}

void CGuildManager::RequestMark(CPlayer* pPlayer, MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = GetMark(MarkName);
	if(pMark == 0)
	{
		return;
	}
	MSG_GUILDMARK_IMG msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILDMARK_REQUEST_ACK;
	msg.MarkName = MarkName;
	memcpy(msg.imgData, pMark->GetImgData(), GUILDMARK_BUFSIZE);
	pPlayer->SendMsg(&msg, sizeof(msg));
}

void CGuildManager::DeleteMark(MARKNAMETYPE MarkName)
{
	CGuildMark* pMark = m_GuildMarkTable.GetData(MarkName);
	if(pMark == 0)
		return;
	
	m_GuildMarkTable.Remove(MarkName);
	delete pMark;
}

void CGuildManager::MarkChange(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName)
{
	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILDMARK_CHANGE;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = GuildIdx;
	msg.dwData3 = MarkName;
	
	PACKEDDATA_OBJ->QuickSend(pPlayer, &msg, sizeof(msg));
}

void CGuildManager::SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, char * GuildName)
{
	SEND_GUILD_CREATE_ACK msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILDNAME_CHANGE;
	msg.dwObjectID = pPlayer->GetID();
	msg.GuildIdx = dwGuildIdx;
	SafeStrCpy(msg.GuildName, GuildName, MAX_GUILD_NAME+1);
	
	PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
}

void CGuildManager::LevelUpSyn(CPlayer* pPlayer)
{
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
	if(pGuild->GetLevel() == MAX_GUILD_LEVEL)
		OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);

	if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, pPlayer->GetAbilityGroup()) <= pGuild->GetLevel())
		OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotAbil);
	if( pPlayer->GetMoney() < GetLevelUpMoney(pGuild->GetLevel()) )
		OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_NotMoney);
	if(pGuild->GetLevel() >= MAX_GUILD_LEVEL)
		OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_LevelUp_Complete);
	if(pGuild->GetLevel() == GUILD_4LEVEL)
	{
		if( pPlayer->GetStage() == eStage_Normal )
			OnErr(pPlayer, MP_GUILD_LEVELUP_NACK, eGuildErr_Err);
	}
	pPlayer->SetMoney(GetLevelUpMoney(pGuild->GetLevel()), MONEY_SUBTRACTION, MF_LOST, eItemTable_Inventory, eMoneyLog_LoseGuildLevelUp);
	pGuild->LevelUp();

	GuildLevelUp(pPlayer->GetGuildIdx(), pGuild->GetLevel());

	MSG_BYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LEVELUP_NOTIFY_TO_MAP;
	msg.dwObjectID = pPlayer->GetGuildIdx();
	msg.bData = pGuild->GetLevel();
	g_Network.Send2AgentServer((char*)&msg, sizeof(msg));
}

void CGuildManager::LoadLevelUpInfo()
{
	CMHFile file;
	file.Init("Resource/GuildLevel.bin","rb");

	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0, "Fail To Load a File : GuildLevel");
		return;
	}
	
	for(int n=0;n<MAX_GUILD_STEP;++n)
	{
		m_LevelUpInfo[n] = file.GetDword();
	}
	file.Release();
}


MONEYTYPE CGuildManager::GetLevelUpMoney(BYTE GuildLevel)
{
	if((GuildLevel == 0) || (GuildLevel > MAX_GUILD_LEVEL))
	{
		return 0;
	}
	return m_LevelUpInfo[GuildLevel-1];
}

BOOL CGuildManager::CanEntryGuild(char* date)
{
	SYSTEMTIME ti;
	GetLocalTime(&ti);
	DWORD year;
	DWORD month;
	DWORD day;

	char buf[5]={ 0, };
	strncpy(buf, &date[0],4);
	year = atoi(buf);
	if( ti.wYear > year)
		return TRUE;
	else if(ti.wYear < year)
		return FALSE;

	char dbuf[3]={ 0, };
	strncpy(dbuf, &date[5], 2);
	month = atoi(dbuf);
	
	if(ti.wMonth > month)
		return TRUE;
	else if(ti.wMonth < month)
		return FALSE;
	
	strncpy(dbuf, &date[8], 2);
	dbuf[2] = 0;
	day = atoi(dbuf);
	if(ti.wDay < day)
		return FALSE;
	
	return TRUE;
}

void CGuildManager::SendGuildEntryDate(CPlayer* pPlayer, char* day)
{
	pPlayer->SetGuildCanEntryDate(day);

	MSG_NAME dmsg;
	dmsg.Category = MP_GUILD;
	dmsg.Protocol = MP_GUILD_SETENTRYDATE;
	SafeStrCpy(dmsg.Name, day, 11);
	pPlayer->SendMsg(&dmsg, sizeof(dmsg));
}

void CGuildManager::ChangeMemberRank(CPlayer* pMaster, DWORD MemberIdx, BYTE Rank)
{
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pMaster, MP_GUILD_CHANGERANK_NACK, eGuildErr_ChangeRank_NoGuild);
	if(pMaster->GetGuildMemberRank() != GUILD_MASTER)
		OnErr(pMaster, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err);
	if(pGuild->IsMember(MemberIdx) == FALSE)
		OnErr(pMaster, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err);

	GUILDMEMBERINFO* pMemberInfo = pGuild->GetMemberInfo(MemberIdx);
	if(NULL == pMemberInfo) return;

	BYTE tRank = pMemberInfo->Rank;
	WORD tLevel = pMemberInfo->Memberlvl;

	if( tRank == Rank ) return;

	if( tRank < GUILD_MEMBER && Rank >= GUILD_MEMBER )	
	{
		if( FALSE == pGuild->CanAddMember() )
		{
			OnErr(pMaster, MP_GUILD_ADDMEMBER_NACK, eGuildErr_AddMember_FullMember);
		}
	}
	else if( tRank >= GUILD_MEMBER && Rank < GUILD_MEMBER )
	{
		if( FALSE == pGuild->CanAddStudent() )
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_FullMember);
		}
		if( tLevel >= GUILD_STUDENT_MAX_LEVEL)
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddStudent_TooHighLevelAsStudent);
		}
	}

	if(pGuild->ChangeMemberRank(MemberIdx, Rank) == FALSE)
		OnErr(pMaster, MP_GUILD_CHANGERANK_NACK, eGuildErr_Err);
	
	GuildChangeMemberRank(pMaster->GetGuildIdx(), MemberIdx, Rank);

	CPlayer* pMember = (CPlayer*)g_pUserTable->FindUser(MemberIdx);
	if(pMember)
	{
		pMember->SetGuildMemberRank(Rank);
	}
	
	MSG_DWORDBYTE notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_CHANGERANK_NOTIFY_TO_MAP;
	notify.dwObjectID = pMaster->GetGuildIdx();
	notify.dwData = MemberIdx;
	notify.bData = Rank;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));

	LogGuild(pMaster->GetID(), pMaster->GetGuildIdx(), eGuildLog_MasterChangeRank, MemberIdx, Rank);
}

void CGuildManager::GiveNickNameSyn(CPlayer* pMaster, DWORD TargetId, char* NickName)
{
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_NoGuild);
	if(pMaster->GetGuildMemberRank() != GUILD_MASTER)
		OnErr(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Err);
	if(pGuild->IsMember(TargetId) == FALSE)
		OnErr(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Nick_NotMember);
	int len = strlen(NickName);
	if((len == 0) || (len > MAX_GUILD_NICKNAME))
		OnErr(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Nick_NotAvailableName);
	if(pGuild->GetLevel() < GUILD_2LEVEL)
		OnErr(pMaster, MP_GUILD_GIVENICKNAME_NACK, eGuildErr_Err);
	
	GuildGiveMemberNickName(TargetId, NickName);
	
	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(TargetId);
	if(pTarget)
	{
		pTarget->SetNickName(NickName);

		SEND_GUILD_NICKNAME msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_GIVENICKNAME_ACK;
		msg.TargetIdx = TargetId;
		SafeStrCpy(msg.NickName, NickName, MAX_GUILD_NICKNAME+1);
		PACKEDDATA_OBJ->QuickSend(pTarget, &msg, sizeof(msg));	
	}
	else
	{
		SEND_GUILD_NICKNAME nmsg;
		nmsg.Category = MP_GUILD;
		nmsg.Protocol = MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP;
		nmsg.TargetIdx = TargetId;
		SafeStrCpy(nmsg.NickName, NickName, MAX_GUILD_NICKNAME+1);
		g_Network.Broadcast2AgentServer((char*)&nmsg,sizeof(nmsg));
	}
}

void CGuildManager::RegistGuildItem(DWORD GuildIdx, ITEMBASE* Guilditem)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
	{
		char buf[64];
		sprintf(buf, "RegistGuildItem - NO %d Guild Info", GuildIdx);
		ASSERTMSG(0, buf);
		return;
	}

	if( pGuild )
		pGuild->InitGuildItem(Guilditem);
}

CItemSlot* CGuildManager::GetSlot(DWORD GuildIdx)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild)
	{
		return pGuild->GetSlot();
	}
	return NULL;
}

void CGuildManager::GuildWarehouseInfo(CPlayer* pPlayer, BYTE TabNum)
{
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(!pGuild)
	{
		SendNackMsg(pPlayer, MP_ITEM_GUILD_WAREHOUSE_INFO_NACK, eGuildErr_NoGuild);
		return;
	}
	if(pGuild->GetLocation() !=  GAMERESRCMNGR->GetLoadMapNum())
	{
		OnErr(pPlayer, MP_ITEM_GUILD_WAREHOUSE_INFO_NACK, eGuildErr_Err);
	}
		
	if((TabNum < 0) || (TabNum >= TAB_GUILDWAREHOUSE_NUM))
	{
		ASSERTMSG(0, "GuildWarehouseInfo - TabNum Error");
		SendNackMsg(pPlayer, MP_ITEM_GUILD_WAREHOUSE_INFO_NACK, eGuildErr_Err);
		return;
	}

	if( FALSE == pGuild->IsItemInfoInited() )
	{
		CGuildWarehouse* pWarehouse = pGuild->GetWarehouse();
		pWarehouse->AddItemInfoWaitingMember(pPlayer, TabNum);
		if( FALSE == pGuild->IsWaitngItemInfoFromDB() )
		{
			GuildItemLoadInNeed( GAMERESRCMNGR->GetLoadMapNum(), pPlayer->GetGuildIdx() );
			pGuild->SetWaitingItemInfoFromDB(TRUE);
		}
	}
	else
	{
		pGuild->EnterWareHouse(pPlayer, TabNum);
	}
}

void CGuildManager::LeaveWareHouse(CPlayer* pPlayer, BYTE TabNum)
{
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(pGuild == 0)
	{
		return;
	}
	if((TabNum < 0) || (TabNum >= TAB_GUILDWAREHOUSE_NUM))
	{
		ASSERTMSG(0, "GuildWarehouseInfo - TabNum Error");
		return;
	}
	pGuild->LeaveWareHouse(pPlayer->GetID(), TabNum);
}

void CGuildManager::AddGuildItemRareOption(ITEM_RARE_OPTION_INFO* pRareInfo)
{
	if(pRareInfo->dwRareOptionIdx == 0)
		return;
	if(m_ItemRareOptionList.GetData( pRareInfo->dwRareOptionIdx ))
		return;
	ITEM_RARE_OPTION_INFO* pNewInfo = new ITEM_RARE_OPTION_INFO;
	memcpy(pNewInfo, pRareInfo, sizeof(ITEM_RARE_OPTION_INFO));
	m_ItemRareOptionList.Add(pNewInfo, pNewInfo->dwRareOptionIdx );
}

void CGuildManager::RemoveGuildItemRareOption(DWORD dwRareIdx)
{
	ASSERT(m_ItemRareOptionList.GetData(dwRareIdx));
	ITEM_RARE_OPTION_INFO* pNewInfo = m_ItemRareOptionList.GetData(dwRareIdx);
	SAFE_DELETE( pNewInfo );
	m_ItemRareOptionList.Remove(dwRareIdx);
}

ITEM_RARE_OPTION_INFO* CGuildManager::GetItemRareOption(DWORD dwRareIdx)
{//ÐèÒªÐÞ¸´
	ASSERT(m_ItemRareOptionList.GetData(dwRareIdx));
	return m_ItemRareOptionList.GetData(dwRareIdx);
}

void CGuildManager::AddGuildItemOption(ITEM_OPTION_INFO * pInfo)
{
	if(pInfo->dwOptionIdx == 0)
		return;
	if(m_ItemOptionList.GetData(pInfo->dwOptionIdx))
	{
		return;
	}
	ITEM_OPTION_INFO * pNewInfo = new ITEM_OPTION_INFO;
	memcpy(pNewInfo, pInfo, sizeof(ITEM_OPTION_INFO));
	m_ItemOptionList.Add(pNewInfo, pNewInfo->dwOptionIdx);
}

void CGuildManager::RemoveGuildItemOption(DWORD dwOptionIdx)
{
	ITEM_OPTION_INFO * pNewInfo = m_ItemOptionList.GetData(dwOptionIdx);
	ASSERT(m_ItemOptionList.GetData(dwOptionIdx));
	SAFE_DELETE( pNewInfo );
	m_ItemOptionList.Remove(dwOptionIdx);
}

ITEM_OPTION_INFO * CGuildManager::GetItemOption(DWORD dwOptionIdx)
{
	return m_ItemOptionList.GetData(dwOptionIdx);
}

void CGuildManager::PutInMoneyGuild(CPlayer* pPlayer, DWORD Money)
{
	if(pPlayer->GetGuildMemberRank() < GUILD_MEMBER)
	{
		ASSERT(0);
		return;
	}
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	if(!pGuild)
	{
		return;
	}

	DWORD playermon = pPlayer->GetMoney();
	DWORD guildmon = pGuild->GetMoney();
	MONEYTYPE maxmon = pGuild->GetMaxPurseMoney();
	
	if( playermon < Money )
	{
		Money = playermon;
	}	
	if( maxmon - guildmon < Money)
	{
		Money = maxmon - guildmon;
	}	
	if(Money == 0)
		return;
	if( pGuild->GetLocation() != g_pServerSystem->GetMapNum() )
		return;

	Money = pGuild->SetMoney(Money, MONEY_ADDITION, MF_PUTINGUILD);
	pPlayer->SetMoney(Money, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseGuild, pPlayer->GetGuildIdx());
	
	GuildMoneyUpdate(pPlayer->GetGuildIdx(), pGuild->GetMoney());
	
	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PUTIN_MONEY_ACK;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = pPlayer->GetMoney();
	msg.dwData3 = pGuild->GetMoney();
	pGuild->SendtoWarehousePlayerAll(&msg, sizeof(msg));

	InsertLogMoney( eMoneyLog_LoseGuild, pPlayer->GetID(), pPlayer->GetMoney(), Money, pPlayer->GetGuildIdx(), pGuild->GetMoney());
}

void CGuildManager::PutOutMoneyGuild(CPlayer* pPlayer, DWORD Money)
{
	CGuild* pGuild = GetGuild(pPlayer->GetGuildIdx());
	MONEYTYPE maxmon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory);
	
	if(!pGuild)
	{
		ASSERT(0);
		return;
	}
	if(Money > pGuild->GetMoney())
	{
		Money = pGuild->GetMoney();
	}
	if(maxmon - pPlayer->GetMoney() < Money)
	{
		Money = maxmon - pPlayer->GetMoney();
	}
	if(Money == 0)
		return;
	if( pGuild->GetLocation() != g_pServerSystem->GetMapNum() )
		return;

	Money = pPlayer->SetMoney(Money, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetGuild, pPlayer->GetGuildIdx());
	pGuild->SetMoney(Money, MONEY_SUBTRACTION, MF_PUTOUTGUILD);
	GuildMoneyUpdate(pPlayer->GetGuildIdx(), pGuild->GetMoney());
	
	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PUTOUT_MONEY_ACK;
	msg.dwData1 = pPlayer->GetID();
	msg.dwData2 = pPlayer->GetMoney();
	msg.dwData3 = pGuild->GetMoney();
	pGuild->SendtoWarehousePlayerAll(&msg, sizeof(msg));

	InsertLogMoney( eMoneyLog_GetGuild, pPlayer->GetID(), pPlayer->GetMoney(), Money, pPlayer->GetGuildIdx(), pGuild->GetMoney());
}

void CGuildManager::MemberLevelUp(DWORD GuildIdx, DWORD PlayerIdx, LEVELTYPE PlayerLvl)
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(pGuild == 0)
		return;
	if(pGuild->IsMember(PlayerIdx) == FALSE)
		return;
	pGuild->SetMemberLevel(PlayerIdx, PlayerLvl);

	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP;
	msg.dwData1 = GuildIdx;
	msg.dwData2 = PlayerIdx;
	msg.dwData3 = PlayerLvl;
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));	
}

void CGuildManager::SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals)
{
	if(pGuild->IsMember(PlayerIdx) == FALSE)
		return;

	pGuild->SetLogInfo(PlayerIdx, vals);
	
	SEND_GUILDMEMBER_LOGININFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LOGINFO_NOTIFY_TO_MAP;
	msg.dwObjectID = pGuild->GetIdx();
	msg.MemberIdx = PlayerIdx;
	msg.bLogIn = vals;
	g_Network.SendAll2AgentServer((char*)&msg,sizeof(msg));	
}

BOOL CGuildManager::CheckHack( CPlayer* pPlayer )
{
	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;

	return TRUE;
}

void CGuildManager::MsgChangeLocationSyn( void* pMsg )
{
	MSG_WORD* pmsg = (MSG_WORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	DWORD dwGuildIdx = pPlayer->GetGuildIdx();
	if( !dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_NoGuild );
		return;
	}
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_InviteApt_NoMaster );
		return;
	}
	
	CGuild* pGuild = m_GuildRoomHashTable.GetData(dwGuildIdx);
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_NoGuild );
		return;
	}
	if( pGuild->GetLocation() != GAMERESRCMNGR->GetLoadMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_Filter );
		return;
	}
	if( pGuild->GetLocation() == pmsg->wData )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotMember );
		return;
	}
	if( !pGuild->IsEmptyGuildWare() )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotAvailableName );
		return;
	}
	if( pGuild->GetMoney() )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotAvailableName );
		return;
	}
//#ifdef _HK_LOCAL_
//	MONEYTYPE money = pGuild->GetLevel() * 100000;
//#else
	MONEYTYPE money = 500000;
//#endif

	if( pPlayer->GetMoney() < money )
	{
		SendNackMsg( pPlayer, MP_GUILD_CHANGE_LOCATION_NACK, eGuildErr_Nick_NotLogIn );
		return;
	}

	pPlayer->SetMoney( money, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseChangeLocation, 0 );
	pGuild->SetLocation( pmsg->wData );
	pGuild->SetZeroMoney();

	MSG_DWORD Money;
	Money.Category = MP_GUILD;
	Money.Protocol = MP_GUILD_CHANGE_LOCATION_MONEY;
	Money.dwData = money;
	pPlayer->SendMsg( &Money, sizeof(Money) );

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD;
	Msg.Protocol = MP_GUILD_CHANGE_LOCATION_ACK;
	Msg.dwData1 = dwGuildIdx;
	Msg.dwData2 = pmsg->wData;
	pGuild->SendMsgToAll( &Msg, sizeof(Msg) );

	MSG_DWORD2 Msg2;
	Msg2.Category = MP_GUILD;
	Msg2.Protocol = MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP;
	Msg2.dwData1 = dwGuildIdx;
	Msg2.dwData2 = pmsg->wData;

	PACKEDDATA_OBJ->SendToBroadCastMapServer( &Msg2, sizeof(Msg2) );

	ChangeGuildLocation( dwGuildIdx, pmsg->wData );
}

void CGuildManager::MsgChangeLocationNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CGuild* pGuild = m_GuildRoomHashTable.GetData(pmsg->dwData1);
	if( !pGuild )	return;

	pGuild->SetLocation( (MAPTYPE)pmsg->dwData2 );
	pGuild->SetZeroMoney();

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD;
	Msg.Protocol = MP_GUILD_CHANGE_LOCATION_ACK;
	Msg.dwData1 = pmsg->dwData1;
	Msg.dwData2 = pmsg->dwData2;
	pGuild->SendMsgToAll( &Msg, sizeof(Msg) );
}

void CGuildManager::MsgGuildNoteSyn( void* pMsg )
{
	MSG_GUILD_SEND_NOTE* msg = (MSG_GUILD_SEND_NOTE*) pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
	if( !pPlayer )	return;

	DWORD dwGuildIdx = pPlayer->GetGuildIdx();
	if( !dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTE_NACK, eGuildErr_NoGuild );
		return;
	}
	CGuild* pGuild = m_GuildRoomHashTable.GetData(dwGuildIdx);
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTE_NACK, eGuildErr_NoGuild );
		return;
	}

	pGuild->SendAllNote(pPlayer, msg->Note);

	MSGBASE ack;
	ack.Category = MP_GUILD;
	ack.Protocol = MP_GUILD_NOTE_ACK;
	pPlayer->SendMsg(&ack, sizeof(ack));
}

void CGuildManager::MsgGuildNoticeSyn( void* pMsg )
{
	MSG_GUILDNOTICE* pmsg = (MSG_GUILDNOTICE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( !pmsg->dwGuildId )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTICE_NACK, eGuildErr_NoGuild );
		return;
	}
	CGuild* pGuild = m_GuildRoomHashTable.GetData(pmsg->dwGuildId);
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_NOTICE_NACK, eGuildErr_NoGuild );
		return;
	}

	pGuild->SetGuildNotice(pmsg->Msg);
	GuildUpdateNotice(pmsg->dwGuildId, pmsg->Msg);

	MSG_GUILDNOTICE ack;

	ack.Category = MP_GUILD;
	ack.Protocol = MP_GUILD_NOTICE_ACK;
	ack.dwGuildId = pmsg->dwGuildId;
	strcpy(ack.Msg, pmsg->Msg);

	pPlayer->SendMsg(&ack, ack.GetMsgLength());

	MSG_GUILDNOTICE msg;

	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_NOTICE_NOTIFY;
	msg.dwGuildId = pmsg->dwGuildId;
	strcpy(msg.Msg, pmsg->Msg);

	g_Network.Send2AgentServer((char*)&msg, msg.GetMsgLength());
}

void CGuildManager::MsgGuildNoticeNotify( void* pMsg )
{
	MSG_GUILDNOTICE* pmsg = (MSG_GUILDNOTICE*)pMsg;
	
	CGuild* pGuild = m_GuildRoomHashTable.GetData(pmsg->dwGuildId);
	if( !pGuild )
	{
		return;
	}

	pGuild->SetGuildNotice(pmsg->Msg);
}

void CGuildManager::RegistGuildNotice(DWORD GuildIdx, char* Notice)
{
	CGuild* pGuild  = m_GuildRoomHashTable.GetData(GuildIdx);
	if( !pGuild )
	{
		return;
	}

	pGuild->SetGuildNotice(Notice);
}

void CGuildManager::SetGuildItemInfoInited( DWORD dwGuildIdx, BOOL bInit )
{
	CGuild* pGuild  = m_GuildRoomHashTable.GetData(dwGuildIdx);
	if( !pGuild )
	{
		return;
	}

	pGuild->SetItemInfoInited(bInit);
}

void CGuildManager::SendGuildItemInfoToAllWarehouseMember( DWORD dwGuildIdx )
{
	CGuild* pGuild = GetGuild(dwGuildIdx);

	if(!pGuild)
	{
		char buf[128];
		sprintf(buf, "<<NonGuildInfo>>GuildIdx : %d", dwGuildIdx);
		ASSERTMSG(0, buf);
		return;
	}
	if(pGuild->GetLocation() != GAMERESRCMNGR->GetLoadMapNum())
	{
		char buf[128];
		sprintf(buf, "<<NotMatchGuildPos>>GuildIdx : %d", dwGuildIdx);
		ASSERTMSG(0, buf);
		return;
	}

	CGuildWarehouse* pWarehouse = pGuild->GetWarehouse();
	if(!pWarehouse)
	{
		ASSERT(0);
		return;
	}

	pWarehouse->SendItemInfoToAllWaitingMembers();
}

void CGuildManager::AddStudentSyn( CPlayer* pMaster, DWORD TargetPlayerID )
{
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(NULL == pGuild)
	{
		OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_Err);
	}

	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_Err);
	}

	if(pMaster->GetGuildMemberRank() < GUILD_SENIOR)
	{
		OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_NothaveAuthority);
	}

	CPlayer* pTarget = (CPlayer*)g_pUserTable->FindUser(TargetPlayerID);
	if(NULL==pTarget)
	{
		OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_NotPlayer);
		
		ASSERT(0);
		return;
	}
	else
	{
		if(pTarget->GetGuildIdx())
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_OtherGuild);
		}

		if(pTarget->GetLevel() >= GUILD_STUDENT_MAX_LEVEL)
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddStudent_TooHighLevelAsStudent);
		}
	}

	if( pGuild )
	{
		if( TRUE == pGuild->IsMember(TargetPlayerID) )	
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_AlreadyMember);
		}

		if( FALSE == pGuild->CanAddStudent() )
		{
			OnErr(pMaster, MP_GUILD_ADDSTUDENT_NACK, eGuildErr_AddMember_FullMember);
		}
	}
	else
	{
		ASSERT(0);
		return;
	}

	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_ADDSTUDENT_INVITE;
	pMaster->SendMsg(&msg, sizeof(msg));

	SEND_GUILD_INVITE invite;
	invite.Category = MP_GUILD;
	invite.Protocol = MP_GUILD_INVITE_AS_STUDENT;
	invite.MasterIdx = pMaster->GetID();
	SafeStrCpy(invite.MasterName, pMaster->GetObjectName(), MAX_NAME_LENGTH+1);
	SafeStrCpy(invite.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	pTarget->SendMsg(&invite, sizeof(invite));

}

BOOL CGuildManager::CanEntryGuildForStudent( char* date )
{
	return CanEntryGuild(date);
}

void CGuildManager::AddStudentResult( CPlayer* pTarget, DWORD MasterID )
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterID);
	if(pMaster == 0)
		OnErr(pTarget, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_InviteApt_NoMaster);
	
	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pTarget, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_Err);
	
	if(pGuild)
	{
		if(pGuild->IsMember(pTarget->GetID()) == TRUE)
			OnErr(pTarget, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_Err);

		if(pGuild->CanAddStudent() == FALSE)
			OnErr(pMaster, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_AddMember_FullMember);
	}
	GuildAddMember(pMaster->GetGuildIdx(), pTarget->GetID(), pGuild->GetGuildName(), GUILD_STUDENT);

	pTarget->SetGuildInfo(pMaster->GetGuildIdx(), GUILD_STUDENT, pGuild->GetGuildName(), pGuild->GetMarkName());
	MarkChange(pTarget, pMaster->GetGuildIdx(), pGuild->GetMarkName());

	GUILDMEMBERINFO member;
	member.InitInfo(pTarget->GetID(), pTarget->GetObjectName(), pTarget->GetLevel(), GUILD_STUDENT, TRUE);
	pGuild->AddMember(&member);
	pGuild->SetLogInfo(pTarget->GetID(), TRUE);

	SEND_GUILD_TOTALINFO tmsg;
	memset(&tmsg, 0, sizeof(SEND_GUILD_TOTALINFO));
	tmsg.Category = MP_GUILD;
	tmsg.Protocol = MP_GUILD_ACCEPT_STUDENT_ACK;
	tmsg.InitGuildInfo(pGuild->GetGuildInfo());
	tmsg.InitPointInfo(pGuild->GetGuildPointInfo());
	pGuild->GetTotalMember(tmsg.MemberInfo);
	tmsg.membernum = pGuild->GetMemberNum();
	pTarget->SendMsg(&tmsg, tmsg.GetMsgLength());

	SEND_GUILD_MEMBER_INFO notify;
	notify.Category = MP_GUILD;
	notify.Protocol = MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP;
	notify.GuildIdx = pMaster->GetGuildIdx();
	SafeStrCpy(notify.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1);
	notify.MemberInfo.MemberIdx = pTarget->GetID();
	notify.MemberInfo.Memberlvl = pTarget->GetLevel();
	SafeStrCpy(notify.MemberInfo.MemberName, pTarget->GetObjectName(), MAX_NAME_LENGTH+1);
	notify.MemberInfo.Rank = GUILD_STUDENT;
	g_Network.Send2AgentServer((char*)&notify, sizeof(notify));

	GUILDFIELDWARMGR->AddPlayer( pTarget );

	if( pGuild->GetGuildUnionIdx() )
		GUILDUNIONMGR->AddPlayer( pTarget, pGuild->GetGuildUnionIdx() );		
	
	LogGuild(pTarget->GetID(), pGuild->GetIdx(), eGuildLog_MemberAdd);

	DeleteMunhaJoinInfo(pTarget->GetID());
}

void CGuildManager::AddStudentResult( DWORD TargetID, DWORD MasterID )
{
	CPlayer* pMaster = (CPlayer*)g_pUserTable->FindUser(MasterID);
	if(pMaster == 0)
		OnErr(pMaster, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_InviteApt_NoMaster);

	CGuild* pGuild = GetGuild(pMaster->GetGuildIdx());
	if(pGuild == 0)
		OnErr(pMaster, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_Err);

	if(pGuild)
	{
		if(pGuild->IsMember(TargetID) == TRUE)
			OnErr(pMaster, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_Err);

		if(pGuild->CanAddStudent() == FALSE)
			OnErr(pMaster, MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK, eGuildErr_AddMember_FullMember);
	}

	GuildAddStudent( pMaster->GetGuildIdx(), TargetID, pGuild->GetGuildName(), GUILD_STUDENT );
}

BOOL CGuildManager::LoadGuildPointRule()
{
	CMHFile file;
	char szBuf[256] = {0,};

	if(!file.Init("./Resource/Server/GuildPointRule.bin", "rb"))
		return FALSE;

	while(!file.IsEOF())
	{
		strcpy(szBuf,strupr(file.GetString()));

		if( szBuf[0] == ';' ) continue;

		if(strcmp(szBuf,"#TIME_CHECK_GUILDHUNTEDMONSTER_COUNT") == 0)
		{
			m_CHECKTIMELEN_GuildHuntedMonsterCounter = file.GetDword();

		}

		else if(strcmp(szBuf,"#NOMAL_MONSTER_HUNTCOUNT") == 0)
		{
			m_GuildPointRule.NOMAL_MONSTER_HUNTCOUNT = file.GetInt();
		}

		else if(strcmp(szBuf,"#FIELDBOSS_MONSTER_HUNTCOUNT") == 0)
		{
			m_GuildPointRule.FIELDBOSS_MONSTER_HUNTCOUNT = file.GetInt();
		}

		else if(strcmp(szBuf,"#BOSS_MONSTER_HUNTCOUNT") == 0)
		{
			m_GuildPointRule.BOSS_MONSTER_HUNTCOUNT = file.GetInt();
		}

		else if(strcmp(szBuf,"#QUEST_MONSTER_HUNTCOUNT") == 0)
		{
			m_GuildPointRule.QUEST_MONSTER_HUNTCOUNT = file.GetInt();
		}

		else if(strcmp(szBuf,"#0000_0999") == 0)
		{
			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount0_999] = file.GetInt();
		}

		else if(strcmp(szBuf,"#1000_1999") == 0)
		{
			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount1000_1999] = file.GetInt();
		}

		else if(strcmp(szBuf,"#2000_2999") == 0)
		{
			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount2000_2999] = file.GetInt();
		}

		else if(strcmp(szBuf,"#3000_3999") == 0)
		{
			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount3000_3999] = file.GetInt();
		}

		else if(strcmp(szBuf,"#4000_4999") == 0)
		{
			m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount4000_4999] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_32") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_32] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_16") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_16] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_8") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_8] = file.GetInt();

		}

		else if(strcmp(szBuf,"#GDT_4") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_4] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_3") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_3] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_2") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_2] = file.GetInt();
		}

		else if(strcmp(szBuf,"#GDT_1") == 0)
		{
			m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_1] = file.GetInt();
		}

		else if(strcmp(szBuf,"#1_19") == 0)
		{
			m_GuildPointRule.HUNTCOUNT_LV_1_19 = file.GetInt();
		}

		else if(strcmp(szBuf,"#20_29") == 0)
		{
			m_GuildPointRule.HUNTCOUNT_LV_20_29 = file.GetInt();
		}

		else if(strcmp(szBuf,"#30_39") == 0)
		{
			m_GuildPointRule.HUNTCOUNT_LV_30_39 = file.GetInt();
		}

		else if(strcmp(szBuf,"#LEVELUPTIME") == 0)
		{
			m_GuildPointRule.LEVELUPTIME_FOR_GIFTMONEY = file.GetInt();
		}

		else if(strcmp(szBuf,"#GIFTMONEY") == 0)
		{
			m_GuildPointRule.GUILD_GIFTMONEY = file.GetInt();
		}
	}
	return TRUE;
}

void CGuildManager::AddGuildHuntedMonsterCount( CPlayer* pPlayer, CObject* pHuntedMonster )
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	if( 0 == GuildIdx )	return;

	CGuild* pGuild = GetGuild(GuildIdx);

	if(NULL == pGuild)
	{
		ASSERTMSG(0, "No GuildInfo at AddGuildHuntedMonsterCount(){..}");
		return;
	}

	int	HuntedCount = 0;

	switch(pHuntedMonster->GetObjectKind())
	{
	case eObjectKind_Monster:
	case eObjectKind_TitanMonster:
		{
			if( pPlayer->GetMaxLevel() - pHuntedMonster->GetLevel() < 4 )
			{
				if( 1 <= pPlayer->GetMaxLevel() && pPlayer->GetMaxLevel() < 20 )
				{
					HuntedCount = m_GuildPointRule.HUNTCOUNT_LV_1_19;
				}
				else if( 20 <= pPlayer->GetMaxLevel() && pPlayer->GetMaxLevel() < 29 )
				{
					HuntedCount = m_GuildPointRule.HUNTCOUNT_LV_20_29;
				}
				else if( 30 <= pPlayer->GetMaxLevel() && pPlayer->GetMaxLevel() < 39 )
				{
					HuntedCount = m_GuildPointRule.HUNTCOUNT_LV_30_39;
				}
				else	// 40 <= x
				{
					HuntedCount = m_GuildPointRule.NOMAL_MONSTER_HUNTCOUNT;
				}
			}
		}
		break;
	case eObjectKind_FieldBossMonster:
		{
			if( pPlayer->GetMaxLevel() - pHuntedMonster->GetLevel() < 9 )
			{
				HuntedCount = m_GuildPointRule.FIELDBOSS_MONSTER_HUNTCOUNT;
			}
		}
		break;
	case eObjectKind_BossMonster:
		{
			HuntedCount = m_GuildPointRule.BOSS_MONSTER_HUNTCOUNT;
		}
		break;
	case eObjectKind_SpecialMonster:
		{
			HuntedCount = m_GuildPointRule.QUEST_MONSTER_HUNTCOUNT;
		}
		break;
	}

	if(HuntedCount)
	{
		pGuild->AddHuntedMonsterCount( HuntedCount );
	}
}

void CGuildManager::SendGuildHuntedMonsterTotalCountToMapServerExceptOneself( DWORD GuildIdx, int nTotalCount )
{
	QSTATETYPE dwCurtime = GetCurTime();
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! SendGuildHuntedMonsterTotalCountToMapServerExceptOneself: pGuild is NULL GuildIdx: %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	SEND_GUILD_HUNTEDMOB_INFO msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_HUNTEDMONSTER_TOTALCOUNT_TO_MAP;
	msg.dwObjectID = GuildIdx;
	msg.dwData1 = nTotalCount;
	msg.dwData2 = dwCurtime;
	g_Network.Send2AgentServer((char*)&msg,sizeof(msg));

	SetGuildHuntedMonsterTotalCountInfo(GuildIdx, nTotalCount, dwCurtime);
}

void CGuildManager::SetGuildHuntedMonsterTotalCountInfo( DWORD GuildIdx, int nTotalCount, DWORD DBProcessTime )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild ) return;

	pGuild->SetHuntedMonsterTotalCountInfo(nTotalCount, DBProcessTime);
}

void CGuildManager::GuildConvertHuntedMonsterTotalCountToGuildPoint( DWORD GuildIdx, int nTotalCount )
{
	QSTATETYPE dwCurtime = GetCurTime();

	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild ) return;

	int pt = GuildPointFromHuntedMonsterTotalCount(nTotalCount);

	pGuild->SetHuntedMonsterTotalCountInfo( 0, dwCurtime );	

	LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildHuntedMonsterTotalCountConverting, pt, nTotalCount, 0 );

	GuildAddGuildPoint(GuildIdx, pt, eGuildHuntedMonsterTotalCountConverting, nTotalCount);
}

int CGuildManager::GuildPointFromHuntedMonsterTotalCount( int nTotalCount )
{
	if( 0 == nTotalCount )
	{
		ASSERT(0);
		return 0;
	}

	if( 0 <=  nTotalCount && nTotalCount <= 999 )
	{
		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount0_999];
	}
	else if( 1000 <=  nTotalCount && nTotalCount <= 1999 )
	{
		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount1000_1999];
	}
	else if( 2000 <=  nTotalCount && nTotalCount <= 2999 )
	{
		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount2000_2999];
	}
	else if( 3000 <=  nTotalCount && nTotalCount <= 3999 )
	{
		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount3000_3999];
	}
	else if( 4000 <=  nTotalCount && nTotalCount <= 4999 )
	{
		return m_GuildPointRule.GUILDPOINT_SECTION[eHuntedMonsterCount4000_4999];
	}
	else if( nTotalCount >= 5000  )
	{
		int pt = ((nTotalCount-5000)/50) + 150;
		return pt;
	}
	else
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildPointFromHuntedMonsterTotalCount: TotalCount is Out %d", nTotalCount );
		ASSERTMSG(0, buf);

		return 0;
	}
}

void CGuildManager::GuildMemberLevelUpToGuildPoint( CPlayer* pPlayer )
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	if( 0 == GuildIdx ) return;

	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildMemberLevelUpToGuildPoint: pGuild is NULL. PlayerID %d", pPlayer->GetID() );
		ASSERTMSG(0, buf);
		return;
	}

	int pt = GuildPointFromGuildMemberLevelUp(pPlayer);

	LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildMemberMaxLevelUp, pt, pPlayer->GetMaxLevel(), pPlayer->GetID());

	GuildAddGuildPoint(GuildIdx, pt, eGuildMemberMaxLevelUp, pPlayer->GetID());
}

int CGuildManager::GuildPointFromGuildMemberLevelUp( CPlayer* pPlayer )
{
	int AddPoint = 0;
	int stage = pPlayer->GetStage();

	switch(stage)
	{
	case eStage_Normal:
		{
			if( pPlayer->GetMaxLevel() >= 10 )	
			{ 
				AddPoint = pPlayer->GetMaxLevel();
			}
		}
		break;
	case eStage_Hwa:
	case eStage_Geuk:
		{
			AddPoint = pPlayer->GetMaxLevel() * 3;
		}
		break;
	case eStage_Hyun:
	case eStage_Tal:
		{
			AddPoint = pPlayer->GetMaxLevel() * 5;
		}
		break;
	default:
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! GuildPointFromGuildMemberLevelUp: Out of Stage. PlayerID %d", pPlayer->GetID() );
			ASSERTMSG(0, buf);
		}
	}

	return AddPoint;
}

void CGuildManager::GuildTournamentResultToGuildPoint( DWORD GuildIdx, WORD wRank )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildTournamentResultToGuildPoint: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	int pt = GuildPointFromGuildTournamentRank(wRank);

	LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildTournamentMarks, pt, wRank, 0);

	GuildAddGuildPoint(GuildIdx, pt, eGuildTournamentMarks, wRank);
}

void CGuildManager::GuildPointFromFortWar( DWORD dwGuildIdx, DWORD dwGuildPoint )
{
	CGuild* pGuild = GetGuild(dwGuildIdx);
	if( NULL == pGuild )
        return;

	LogGuildPoint( dwGuildIdx, pGuild->GetGuildPoint(), TRUE, eGuildFortWar, dwGuildPoint, dwGuildPoint, 0);

	GuildAddGuildPoint( dwGuildIdx, dwGuildPoint, eGuildFortWar, pGuild->GetGuildPoint() );
}

int	CGuildManager::GuildPointFromGuildTournamentRank( WORD wRank )
{
	int AddPoint = 0;

	switch(wRank)
	{
	case eGTStanding_32TH:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_32];
		}
		break;
	case eGTStanding_16TH:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_16];
		}
		break;
	case eGTStanding_8TH:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_8];
		}
		break;
	case eGTStanding_4TH:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_4];
		}
		break;
	case eGTStanding_3RD:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_3];
		}
		break;
	case eGTStanding_2ND:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_2];
		}
		break;
	case eGTStanding_1ST:
		{
			AddPoint = m_GuildPointRule.GUILDPOINT_GT_RESULT[eGDT_1];
		}
		break;
	}

	return AddPoint;
}

BOOL CGuildManager::LoadGuildPointPlustimeList()
{
	CMHFile file;
	char szBuf[256] = {0,};

	if(!file.Init("./Resource/GuildPointPlustimeList.bin", "rb"))
		return FALSE;

	m_gPlustimeKindNum = file.GetInt();

	if(m_gPlustimeKindNum)
	{
		m_pGuildPlustimeInfo = new GUILDPLUSTIME_INFO[m_gPlustimeKindNum];
	}
	else
	{
		g_Console.LOG( 4, "°ïË«ÐÅÏ¢¼ÓÔØÊ§°Ü");
		return FALSE;
	}

	int count = 0;

	while(!file.IsEOF())
	{
		m_pGuildPlustimeInfo[count].PlusTimeIdx = file.GetInt();
		m_pGuildPlustimeInfo[count].PlusTimeKind = file.GetDword();
		SafeStrCpy( m_pGuildPlustimeInfo[count].PlustimeName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
		m_pGuildPlustimeInfo[count].UsableTimeLen =	file.GetDword();
		m_pGuildPlustimeInfo[count].NeedPoint =		file.GetDword();
		m_pGuildPlustimeInfo[count].AddData =		file.GetDword();

		++count;

		if(count == m_gPlustimeKindNum)
			break;
	}
	file.Release();
	return TRUE;
}

void CGuildManager::InitGuildPointInfo( DWORD GuildIdx, GUILDPOINT_INFO* pGuildPointInfo )
{
	CGuild* pGuild = GetGuild(GuildIdx);

	if(NULL == pGuild)
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! InitGuildPointInfo: pGuild is NULL GuildIdx: %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	pGuild->InitGuildPointInfo(pGuildPointInfo);

}

void CGuildManager::InitGuildPlustimeInfo( DWORD GuildIdx, DWORD GuildUsePlustimeIdx, DWORD GuildIUsePlustimeEndtime )
{
	CGuild* pGuild = GetGuild(GuildIdx);

	if(NULL == pGuild)
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! InitGuildPlustimeInfo: pGuild is NULL GuildIdx: %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	AddGuildPlustimeUsingGuildList(pGuild);

	pGuild->AddGuildPlustime(GuildUsePlustimeIdx, GuildIUsePlustimeEndtime);
}

void CGuildManager::UseGuildPoint( CPlayer* pPlayer, DWORD UseKind, DWORD ListIdx )
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	DWORD NeedPoint = 0;

	if(ForGuildPlusTime == UseKind)
	{
		NeedPoint = m_pGuildPlustimeInfo[ListIdx-1].NeedPoint;
		CGuild* pGuild = GetGuild(GuildIdx);
		if(NULL == pGuild)
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! UseGuildPoint: pGuild is NULL. PlayerID %d", pPlayer->GetID() );
			ASSERTMSG(0, buf);
			return;
		}
		if( ListIdx > eGuildPlusTime_Max )
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! UseGuildPoint: Out of ListIdx. PlayerID %d", pPlayer->GetID() );
			ASSERTMSG(0, buf);
			return;
		}

		if( pPlayer->GetGuildMemberRank() < GUILD_SENIOR )
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! UseGuildPoint: Under Usable Rank. Rank %d, PlayerID %d", pPlayer->GetGuildMemberRank(), pPlayer->GetID() );
			ASSERTMSG(0, buf);
			return;
		}

		if( pGuild->IsUsingPlustimeIdx(ListIdx) )
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! UseGuildPoint: Dupl_UsingPlustimeIdx. ListIdx %d, PlayerID %d", ListIdx, pPlayer->GetID() );
			ASSERTMSG(0, buf);
			return;
		}

		if(pGuild->GetGuildPoint() < (int)NeedPoint)
		{
			char buf[128] = {0,};
			sprintf( buf, "Err! UseGuildPoint: LessNeedPoint. Point %d, PlayerID %d", pGuild->GetGuildPoint(), pPlayer->GetID() );
			ASSERTMSG(0, buf);
			return;
		}

		SYSTEMTIME systime;
		stTIME startime, usetime, endtime;
		GetLocalTime(&systime);
		startime.SetTime(systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);

		DWORD UsableTimeLen = m_pGuildPlustimeInfo[ListIdx-1].UsableTimeLen;
		endtime = startime;
		DWORD day = UsableTimeLen/(24*60);
		DWORD hour = (UsableTimeLen%(24*60))/60;
		DWORD minute = (UsableTimeLen%(24*60))%60;
		usetime.SetTime(0, 0, day, hour, minute, 0);
		endtime += usetime;

		LogGuildPoint( GuildIdx, pGuild->GetGuildPoint(), FALSE, eGuildPlusTimeUse, NeedPoint, ListIdx, pPlayer->GetID());

		GuildUseGuildPoint(GuildIdx, NeedPoint, ForGuildPlusTime, ListIdx, endtime.value);
	}
	else if(ForGuildMugong == UseKind)
	{

	}
}

void CGuildManager::AddGuildPointFromDBReturn( DWORD GuildIdx, DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AdditionalData )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild ) return;

	MSG_DWORD4 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINT_ADD_TO_MAP;
	msg.dwObjectID = GuildIdx;
	msg.dwData1 = AddPoint;
	msg.dwData2 = TotalPoint;
	msg.dwData3 = eAddKind;
	msg.dwData4 = AdditionalData;

	g_Network.Send2AgentServer((char*)&msg, sizeof(MSG_DWORD4));

	pGuild->SendGuildPointAddInfoToAll(AddPoint, TotalPoint, eAddKind, AdditionalData);

	SetGuildPoint(GuildIdx, TotalPoint);
}

void CGuildManager::UseGuildPointFromDBReturn( DWORD GuildIdx, DWORD UsePoint, DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD Endtime )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild ) return;

	if(eUseKind == ForGuildPlusTime)
	{
		MSG_DWORD3 msgPlus;
		msgPlus.Category = MP_GUILD;
		msgPlus.Protocol = MP_GUILD_PLUSTIME_SET_TO_MAP;
		msgPlus.dwData1 = GuildIdx;
		msgPlus.dwData2 = KindIdx;
		msgPlus.dwData3 = Endtime;

		g_Network.Send2AgentServer((char*)&msgPlus, sizeof(MSG_DWORD3));

		AddGuildPlustimeUsingGuildList(pGuild);
		pGuild->AddGuildPlustime(KindIdx, Endtime);
	}
	else if(eUseKind == ForGuildMugong)
	{

	}

	MSG_DWORD4 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINT_USEINFO_TO_MAP;
	msg.dwObjectID = GuildIdx;
	msg.dwData1 = TotalPoint;
	msg.dwData2 = eUseKind;
	msg.dwData3 = KindIdx;
	msg.dwData4 = Endtime;

	g_Network.Send2AgentServer((char*)&msg, sizeof(MSG_DWORD4));

	pGuild->SendGuildPointUseInfoToAll(TotalPoint, eUseKind, KindIdx, Endtime);

	SetGuildPoint(GuildIdx, TotalPoint);
}

void CGuildManager::SetGuildPoint( DWORD GuildIdx, DWORD TotalPoint )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild ) return;

	MSG_DWORD msgMap;
	msgMap.Category = MP_GUILD;
	msgMap.Protocol = MP_GUILD_POINT_TO_MAP;
	msgMap.dwObjectID = GuildIdx;
	msgMap.dwData = TotalPoint;

	g_Network.Send2AgentServer((char*)&msgMap, sizeof(MSG_DWORD));

	pGuild->SetGuildPoint(TotalPoint);
}

void CGuildManager::AddGuildPlustimeUsingGuildList( CGuild* pGuild )
{
	if(0 != pGuild->GetGuildPlustimeFlg())	return;

	m_GuildPlustimeUsingGuildList.AddTail(pGuild);
}

void CGuildManager::RemoveGuildPlustimeUsingGuildList( CGuild* pGuild )
{
	if(pGuild->GetGuildPlustimeFlg())	return;

	m_GuildPlustimeUsingGuildList.Remove(pGuild);
}

void CGuildManager::EndGuildPlustimeDBReturn( DWORD GuildIdx, DWORD PlustimeIdx )
{
	CGuild* pGuild = GetGuild(GuildIdx);

	if(NULL == pGuild)
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! EndGuildPlustimeDBReturn: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}
	MSG_DWORD msgMap;
	msgMap.Category = MP_GUILD;
	msgMap.Protocol = MP_GUILD_PLUSTIME_END_TO_MAP;
	msgMap.dwObjectID = GuildIdx;
	msgMap.dwData = PlustimeIdx;

	g_Network.Send2AgentServer((char*)&msgMap,sizeof(MSG_DWORD));

	EndGuildPlustime(GuildIdx, PlustimeIdx);
}

void CGuildManager::EndGuildPlustime( DWORD GuildIdx, DWORD PlustimeIdx )
{
	CGuild* pGuild = GetGuild(GuildIdx);

	if(NULL == pGuild)
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! EndGuildPlustime: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}
	pGuild->RemoveGuildPlustime(PlustimeIdx);
	RemoveGuildPlustimeUsingGuildList(pGuild);
}

DWORD CGuildManager::GetGuildPlustimeKindFromIdx( DWORD GuildPlustimeIdx )
{
	GUILDPLUSTIME_INFO* pGPInfo = GetGuildPointPlustimeList();

	if(GuildPlustimeIdx>4)
	{
		g_Console.LOG(4,"°ïË«ÐÅÏ¢´íÎó£¬±àºÅ=%d",GuildPlustimeIdx);
		return 0;
	}
	DWORD Kind = pGPInfo[GuildPlustimeIdx-1].PlusTimeKind;

	return Kind;
}

BOOL CGuildManager::GetGuildPlustimeRt( DWORD GuildIdx, int PlustimeKind, void* pData )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if(NULL == pGuild)
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GetGuildPlustimeRt: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return FALSE;
	}

	int GuildPlustime = pGuild->GetGuildPlustimeFlg();
	if(0 == GuildPlustime)
	{
		return FALSE;
	}

	if( FALSE == pGuild->IsUsingPlustimeKind(PlustimeKind) )
	{
		return FALSE;
	}

	GUILDPOINT_INFO* pPointInfo = pGuild->GetGuildPointInfo();

	if( FALSE == CheckValidPlustimeKind(pPointInfo, GuildIdx, PlustimeKind) )
		return FALSE;

	DWORD UsingIdxFromKind = 0;

	switch( PlustimeKind )
	{
	case eGPT_SuRyun:
		{
			DWORD* pAddAbil = (DWORD*)pData;
			
			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_SuRyun].PlusTimeIdx;

			*pAddAbil = DWORD((*pAddAbil * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
		}
		break;
	case eGPT_MuGong:
		{
			DWORD* pAddMugong = (DWORD*)pData;

			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_MuGong].PlusTimeIdx;

			*pAddMugong = DWORD((*pAddMugong * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
		}
		break;
	case eGPT_Exp:
		{
			DWORD* pAddExp = (DWORD*)pData;

			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_Exp].PlusTimeIdx;

			*pAddExp = DWORD((*pAddExp * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData)/100.f);
		}
		break;
	case eGPT_DamageUp:
		{
			double* pAttDmg = (double*)pData;
			double AddDmg = *pAttDmg;

			UsingIdxFromKind = pPointInfo->GuildUsingPlusTimeInfo[eGPT_DamageUp].PlusTimeIdx;

			AddDmg = double(AddDmg + (AddDmg * m_pGuildPlustimeInfo[UsingIdxFromKind-1].AddData/100.f));
			*pAttDmg = AddDmg;
		}
		break;
	default:
		{
			return FALSE;
		}
		break;
	}
	return TRUE;
}

void CGuildManager::GUILDMGR_Process()
{
	if(gCurTime - m_CheckTimeForGuildPlustimeEnd > 30000)
	{
		CheckGuildPlustimeEnd();
		m_CheckTimeForGuildPlustimeEnd = gCurTime;
	}
}

void CGuildManager::GUILDMGR_5MinProcess()
{
	GuildHuntedMonsterCountDBUpdate();

	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();

	stTIME curTime, OneDayTime, comparetime;
 	curTime.value = GetCurTime();
	OneDayTime.SetTime(0,0,1,0,0,0);

	PTRLISTPOS pos = m_GuildTraineeList.GetHeadPosition();
	GUILD_TRAINEE_LIST* pTrainee = NULL;
	while(pos)
	{
		pTrainee = (GUILD_TRAINEE_LIST*)m_GuildTraineeList.GetNext(pos);
		if( pTrainee )
		{
			comparetime.SetTime( pTrainee->dwTime );
			comparetime += OneDayTime;

			if( curTime > comparetime )
			{
				m_GuildTraineeList.Remove(pTrainee);
				GuildTraineeDelete(pTrainee->dwUserIdx);
			}
		}
	}
}

void CGuildManager::GuildHuntedMonsterCountDBUpdate()
{
	if(gCurTime - m_dwLastDBUpdateTime < m_CHECKTIMELEN_GuildHuntedMonsterCounter)
	{
		return;
	}

	int nHuntedMonsterCount = 0;
	CGuild* pGuild = NULL;
	m_GuildRoomHashTable.SetPositionHead();
	while( pGuild = m_GuildRoomHashTable.GetData() )
	{
		nHuntedMonsterCount = pGuild->GetHuntedMonsterCount();
		if( pGuild->GetConnectingMapMemberCount() || nHuntedMonsterCount )
		{
			GuildAddHuntedMonsterCount(pGuild->GetIdx(), nHuntedMonsterCount);
			pGuild->SetHuntedMonsterCount(0);
		}
	}

	m_dwLastDBUpdateTime = gCurTime;
}

void CGuildManager::CheckDateForGuildHuntedMonsterCountConvertToGuildPoint()
{
	SYSTEMTIME st;
	GetLocalTime(&st);
	CGuild* pGuild = NULL;
	if( m_wCurDateCheckForConverCount != st.wDay )
	{
		m_GuildRoomHashTable.SetPositionHead();
		while( pGuild = m_GuildRoomHashTable.GetData() )
		{
			if( pGuild->GetLocation() == g_pServerSystem->GetMapNum() )
			{
				if( pGuild->GetHuntedMonsterTotalCount() )
				{
					GuildGetHuntedMonsterTotalCountWithInit(pGuild->GetIdx());
				}
			}
		}

		m_wCurDateCheckForConverCount = st.wDay;
	}
}

void CGuildManager::CheckGuildPlustimeEnd()
{
	static stTIME curtime;
	curtime.value = GetCurTime();

	CGuild* pGuild = NULL;
	PTRLISTPOS pos = m_GuildPlustimeUsingGuildList.GetHeadPosition();
	while( pos )
	{
		pGuild = (CGuild*)m_GuildPlustimeUsingGuildList.GetNext( pos );
		
		if( pGuild )
		if( pGuild->GetLocation() == g_pServerSystem->GetMapNum() )
		{
			if( pGuild->GetGuildPlustimeFlg() )	
			{
				pGuild->CheckPlustimeEnd(curtime.value);
			}
		}
	}
}

BOOL CGuildManager::CheckValidPlustimeKind( GUILDPOINT_INFO* pGPInfo, DWORD GuildIdx, DWORD plusTimeKind )
{
	int usingKind = pGPInfo->GuildUsingPlusTimeInfo[plusTimeKind].PlusTimeIdx;

	if( 0 == usingKind  || usingKind > m_gPlustimeKindNum )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! CheckValidPlustimeKind: PlusTimeUsingKind Num is Not Valiable(: %d). GuildIdx %d", usingKind, GuildIdx );
		ASSERTMSG(0, buf);

		pGPInfo->GuildPlusTimeflg &= ~(1<<plusTimeKind);

		pGPInfo->GuildUsingPlusTimeInfo[plusTimeKind].PlusTimeIdx = 0;

		return FALSE;
	}
	return TRUE;
}

void CGuildManager::AddGuildTraineeInfo(GUILD_TRAINEE_LIST* temp)
{
	m_GuildTraineeList.AddTail(temp);
}

void CGuildManager::AddGuildTraineeGuildInfo(GUILD_TRAINEE_GUILDLIST* temp)
{
	m_GuildTraineeGuildList.AddTail(temp);
}

void CGuildManager::AddGuildTraineeGuildIntroInfo(GUILD_INTRO_INFO* temp)
{
	m_GuildTraineeGuildIntroInfoList.AddTail(temp);
}

void CGuildManager::GuildStudentLvUpPointDBUpdate( CPlayer* pPlayer )
{
	DWORD GuildIdx = pPlayer->GetGuildIdx();
	if( 0 == GuildIdx ) return;

	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GuildStudentLvUpPointDBUpdate: pGuild is NULL. PlayerID %d", pPlayer->GetID() );
		ASSERTMSG(0, buf);
		return;
	}

	if(pPlayer->GetGuildMemberRank() != GUILD_STUDENT)
		return;

#define GLD_STDT_LVUP_ADDPOINT	1

	GuildStudentLvUpCtUpdate( pPlayer->GetGuildIdx(), GLD_STDT_LVUP_ADDPOINT
		, m_GuildPointRule.LEVELUPTIME_FOR_GIFTMONEY );
}

void CGuildManager::SetGuildStudentLvUpCount( DWORD GuildIdx, DWORD GuildStudentLvUpCount, BOOL bMasterChecking )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! SetGuildStudentLvUpCount: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	MSG_DWORD2BYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_STUDENT_LVUP_COUNTINFO;
	msg.dwData1 = GuildIdx;
	msg.dwData2 = GuildStudentLvUpCount;

	msg.bData = bMasterChecking;

	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

	pGuild->SetStudentLvUpCountInfo(GuildStudentLvUpCount, bMasterChecking);
}

void CGuildManager::GiveGuildCerebrationMoney( DWORD GuildIdx )
{
	CGuild* pGuild = GetGuild(GuildIdx);
	if( NULL == pGuild )
	{
		char buf[128] = {0,};
		sprintf( buf, "Err! GiveGuildCerebrationMoney: pGuild is NULL. GuildIdx %d", GuildIdx );
		ASSERTMSG(0, buf);
		return;
	}

	if( pGuild->GetLocation() != g_pServerSystem->GetMapNum() )
	{
		g_pServerTable->SetPositionHead();
		SERVERINFO* pAgentInfo = g_pServerTable->GetNextAgentServer();
		if(pAgentInfo == NULL)
		{
			ASSERT(0);
			return;
		}

		MSG_DWORD msg;
		msg.Category = MP_GUILD;
		msg.Protocol = MP_GUILD_CELEBRATIONMONEY_TO_GUILDWEAR;
		msg.dwData = pGuild->GetIdx();
		PACKEDDATA_OBJ->SendToMapServer( /*pAgentInfo->dwConnectionIndex,*/ (MAPTYPE)pGuild->GetLocation(), &msg, sizeof(msg) );
	}
	else
	{
		PutInGuildCerabrationMoney( pGuild, m_GuildPointRule.GUILD_GIFTMONEY );
	}
}

void CGuildManager::PutInGuildCerabrationMoney( CGuild* pGuild, DWORD Money )
{
	if(!pGuild) return;

	DWORD guildMoney = pGuild->GetMoney();
	MONEYTYPE maxMoney = pGuild->GetMaxPurseMoney();

	if( maxMoney - guildMoney < Money )
	{
		Money = maxMoney - guildMoney;
	}

	if( 0 == Money )
		return;

	if( pGuild->GetLocation() != g_pServerSystem->GetMapNum() )
		return;

	Money = pGuild->SetMoney(Money, MONEY_ADDITION, MF_PUTINGUILD);
	GuildMoneyUpdate(pGuild->GetIdx(), pGuild->GetMoney());	

	MSG_DWORD3 msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_PUTIN_MONEY_ACK;
	msg.dwData1 = 0;	
	msg.dwData2 = 0;
	msg.dwData3 = pGuild->GetMoney();
	pGuild->SendtoWarehousePlayerAll(&msg, sizeof(msg));

	MSG_DWORD msg2;
	msg2.Category = MP_GUILD;
	msg2.Protocol = MP_GUILD_CELEBRATIONMONEY_NOTIFY;
	msg2.dwData = pGuild->GetIdx();

	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg2, sizeof(msg2) );

	pGuild->SendMsgToAll(&msg2, sizeof(msg2));
	

	InsertLogMoney( eMoneyLog_GuildCerebration, pGuild->GetMasterIdx(), 0, Money, pGuild->GetIdx(), pGuild->GetMoney());
}

BOOL CGuildManager::IsSameGuild( CPlayer* pPlayerA, CPlayer* pPlayerB )
{
	if( !(pPlayerA && pPlayerB) )
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD GuildIdx = pPlayerA->GetGuildIdx();
	if(GuildIdx)
	if(GuildIdx == pPlayerB->GetGuildIdx())
		return TRUE;

	return FALSE;
}




