// GuildManager.h: interface for the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_)
#define AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GUILDMGR CGuildManager::GetInstance()
#define OnErr(pPlayer, Protocol, errstate) \
if(errstate) { SendNackMsg(pPlayer, Protocol, errstate); return;}

#define GUILD_STUDENT_MAX_LEVEL	40

#include <map>
#include <vector>

class CGuild;
class CGuildMark;
class CItemSlot;

enum GuildPointAddKind
{
	eGM_CheatGuildPoint,
	eGuildHuntedMonsterTotalCountConverting,
	eGuildMemberMaxLevelUp,
	eGuildTournamentMarks,
	eGuildPlusTimeUse,
	eGuildSkillUse,
	eGuildFortWar,
	eGuildUseChangeItem,
};

enum eSectionForGuildHuntedMonsterCount
{
	eHuntedMonsterCount0_999,
	eHuntedMonsterCount1000_1999,
	eHuntedMonsterCount2000_2999,
	eHuntedMonsterCount3000_3999,
	eHuntedMonsterCount4000_4999,
	eSectionForGuildHuntedMonsterCount_MAX,
};

enum eGuildPointPerGuildTournamentResult
{
	eGDT_32,
	eGDT_16,
	eGDT_8,
	eGDT_4,
	eGDT_3,
	eGDT_2,
	eGDT_1,
	eGDT_MAX,
};

struct GUILDPOINT_RULE
{
	int NOMAL_MONSTER_HUNTCOUNT;
	int FIELDBOSS_MONSTER_HUNTCOUNT;
	int BOSS_MONSTER_HUNTCOUNT;
	int QUEST_MONSTER_HUNTCOUNT;

	int GUILDPOINT_SECTION[eSectionForGuildHuntedMonsterCount_MAX];
	int GUILDPOINT_GT_RESULT[eGDT_MAX];

	int HUNTCOUNT_LV_1_19;
	int HUNTCOUNT_LV_20_29;
	int HUNTCOUNT_LV_30_39;

	int LEVELUPTIME_FOR_GIFTMONEY;
	int GUILD_GIFTMONEY;
};

struct GUILDPLUSTIME_INFO
{
	DWORD PlusTimeIdx;
	DWORD PlusTimeKind;
	char PlustimeName[MAX_ITEMNAME_LENGTH+1];
	DWORD UsableTimeLen;
	DWORD NeedPoint;
	DWORD AddData;

};

//===============================================


class CGuildManager  
{
	static int	m_gPlustimeKindNum;
	GUILDPOINT_RULE	m_GuildPointRule;
	DWORD	m_CHECKTIMELEN_GuildHuntedMonsterCounter;	
	DWORD	m_dwLastDBUpdateTime;	
	WORD	m_wCurDateCheckForConverCount;	

	DWORD	m_CheckTimeForGuildPlustimeEnd;

	GUILDPLUSTIME_INFO* m_pGuildPlustimeInfo;
	cPtrList	m_GuildPlustimeUsingGuildList;

	cPtrList m_GuildTraineeList;
	cPtrList m_GuildTraineeGuildList;
	cPtrList m_GuildTraineeGuildIntroInfoList;

	CYHHashTable<CGuild> m_GuildRoomHashTable;
	CYHHashTable<CGuildMark> m_GuildMarkTable;
	CYHHashTable<ITEM_OPTION_INFO> m_ItemOptionList;
	CYHHashTable<ITEM_RARE_OPTION_INFO> m_ItemRareOptionList;

	MONEYTYPE m_LevelUpInfo[MAX_GUILD_STEP];
	CGuildManager();
	BOOL IsVaildGuildName(char* GuildName);

	BYTE DoBreakUp(DWORD GuildIdx);
	void RegistMarkSyn(DWORD GuildIdx, char* ImgData, DWORD PlayerIdx);
	void RequestMark(CPlayer* pPlayer, MARKNAMETYPE MarkName);
	int convertCharToInt(char c);
	BYTE HexToByte(char* pStr);
	void LoadLevelUpInfo();
	MONEYTYPE GetLevelUpMoney(BYTE GuildLevel);
	BOOL CanEntryGuild(char* date);
public:
	GETINSTANCE(CGuildManager);	
	virtual ~CGuildManager();

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	CGuild* RegistGuild(GUILDINFO* pGuildInfo, MONEYTYPE GuildMoney);
	void RegistGuildItem(DWORD GuildIdx, ITEMBASE* Guilditem);
	void LoadMembers(GUILDMEMBERINFO_ID* pInfo);
	void LoadMark(MARKNAMETYPE MarkName, char* pImgData);
	
	void UserLogIn(CPlayer* pPlayer);
	void UserLogOut(CPlayer* pPlayer);

	void CreateGuildSyn(CPlayer* pPlayer, char* GuildName, char* Intro);
	void CreateGuildResult(LEVELTYPE MasterLvl, GUILDINFO* pGuildInfo);
	
	void BreakUpGuildSyn(CPlayer* pMaster);
	void BreakUpGuildResult(CPlayer* pMaster, DWORD GuildIdx, char* EntryDate);

	void DeleteMemberSyn(CPlayer* pMaster, DWORD MemberIDX);
	BYTE DoDeleteMember(DWORD GuildIDX, DWORD MemberIDX, BYTE bType, char* EntryDay);
	void DeleteMemberResult(DWORD GuildIDX, DWORD MemberIDX, char* EntryDay);
	
	void AddMemberSyn(CPlayer* pMaster, DWORD TargetIdx);
	void AddMemberResult(CPlayer* pTarget, DWORD MasterIdx);
	void DeleteMunhaJoinInfo(DWORD TargetIdx);
	
	CGuild* GetGuild(DWORD GuildIdx);
	CGuild* GetGuildFromMasterIdx( DWORD dwMasterIdx );
	CGuildMark* GetMark(MARKNAMETYPE MarkName);
	CGuild* GetGuildIndex( char* GuildName );

	void SendNackMsg(CPlayer* pPlayer, BYTE Protocol, BYTE errstate);
	
	void SecedeSyn(CPlayer* pPlayer);
	void SecedeResult(DWORD GuildIdx, DWORD MemberIDX, char* EntryDay);
	
	void SendChatMsg(DWORD GuildIdx, DWORD PlayerIdx, char* pMsg);
	
	void RegistMarkResult(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName, char * pImgData);
	void DoRegistMark(CGuild* pGuild, MARKNAMETYPE MarkName, char* imgData);


	void DeleteMark(MARKNAMETYPE MarkName);
	void MarkChange(CPlayer* pPlayer, DWORD GuildIdx, MARKNAMETYPE MarkName);

	void LevelUpSyn(CPlayer* pPlayer);
	
	void SendGuildEntryDate(CPlayer* pPlayer, char* day);

	
	void ChangeMemberRank(CPlayer* pMaster, DWORD MemberIdx, BYTE Rank);

	void GiveNickNameSyn(CPlayer* pMaster, DWORD TargetId, char* NickName);
	
	CItemSlot* GetSlot(DWORD GuildIdx);

	void GuildWarehouseInfo(CPlayer* pPlayer, BYTE TabNum);
	void LeaveWareHouse(CPlayer* pPlayer, BYTE TabNum);

	void AddGuildItemRareOption(ITEM_RARE_OPTION_INFO* pRareInfo);
	void RemoveGuildItemRareOption(DWORD dwRareIdx);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareIdx);

	void AddGuildItemOption(ITEM_OPTION_INFO * pInfo);
	void RemoveGuildItemOption(DWORD dwOptionIdx);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);
	
	void PutInMoneyGuild(CPlayer* pPlayer, DWORD Money);
	void PutOutMoneyGuild(CPlayer* pPlayer, DWORD Money);
	
	void MemberLevelUp(DWORD GuildIdx, DWORD PlayerIdx, LEVELTYPE PlayerLvl);
	
	void SendGuildName(CPlayer* pPlayer, DWORD dwGuildIdx, char * GuildName); 
	
	void SetLogInfo(CGuild* pGuild, DWORD PlayerIdx, BOOL vals);
	
	BOOL	CheckHack( CPlayer* pPlayer ); 
	
	void	MsgChangeLocationSyn( void* pMsg );

	void	MsgChangeLocationNotifyToMap( void* pMsg );
	void	MsgGuildNoteSyn( void* pMsg );

	void	MsgGuildNoticeSyn( void* pMsg );
	void	MsgGuildNoticeNotify( void* pMsg );
	void	RegistGuildNotice(DWORD GuildIdx, char* Notice);

	void	SetGuildItemInfoInited(DWORD dwGuildIdx, BOOL bInit);	
	void	SendGuildItemInfoToAllWarehouseMember(DWORD dwGuildIdx);	

	void	AddStudentSyn(CPlayer* pMaster, DWORD TargetPlayerID);
	BOOL	CanEntryGuildForStudent(char* date);
	void	AddStudentResult(CPlayer* pTarget, DWORD MasterID);

	void	AddStudentResult(DWORD TargetID, DWORD MasterID);
	//////////////////////////////////////////////////////////////////////////
	BOOL	LoadGuildPointRule();
	GUILDPLUSTIME_INFO* GetGuildPointPlustimeList()	{	return m_pGuildPlustimeInfo;	}

	void	AddGuildHuntedMonsterCount(CPlayer* pPlayer, CObject* pHuntedMonster);
	void	SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(DWORD GuildIdx, int nTotalCount);
	void	SetGuildHuntedMonsterTotalCountInfo(DWORD GuildIdx, int nTotalCount, DWORD DBProcessTime);
	void	GuildConvertHuntedMonsterTotalCountToGuildPoint(DWORD GuildIdx, int nTotalCount);
	int		GuildPointFromHuntedMonsterTotalCount(int nTotalCount);
	void	GuildMemberLevelUpToGuildPoint(CPlayer* pPlayer);
	int		GuildPointFromGuildMemberLevelUp(CPlayer* pPlayer);
	void	GuildTournamentResultToGuildPoint(DWORD GuildIdx, WORD wRank);
	int		GuildPointFromGuildTournamentRank(WORD wRank);
	void	GuildPointFromFortWar( DWORD dwGuildIdx, DWORD dwGuildPoint );

	BOOL	LoadGuildPointPlustimeList();

	void	InitGuildPointInfo(DWORD GuildIdx, GUILDPOINT_INFO* pGuildPointInfo);
	void	InitGuildPlustimeInfo(DWORD GuildIdx, DWORD GuildUsePlustimeIdx, DWORD GuildIUsePlustimeEndtime);

	void	UseGuildPoint(CPlayer* pPlayer, DWORD UseKind, DWORD ListIdx);

	void	AddGuildPointFromDBReturn(DWORD GuildIdx, DWORD AddPoint, DWORD TotalPoint, int eAddKind, DWORD AdditionalData);

	void	UseGuildPointFromDBReturn(DWORD GuildIdx, DWORD UsePoint, DWORD TotalPoint, int eUseKind, DWORD KindIdx, DWORD Endtime);
	void	SetGuildPoint(DWORD GuildIdx, DWORD TotalPoint);

	void	AddGuildPlustimeUsingGuildList(CGuild* pGuild);
	void	RemoveGuildPlustimeUsingGuildList(CGuild* pGuild);
	void	EndGuildPlustimeDBReturn(DWORD GuildIdx, DWORD PlustimeIdx);
	void	EndGuildPlustime(DWORD GuildIdx, DWORD PlustimeIdx);

	DWORD	GetGuildPlustimeKindFromIdx(DWORD GuildPlustimeIdx);

	BOOL	GetGuildPlustimeRt(DWORD GuildIdx, int PlustimeKind, void* pData);

	void	GUILDMGR_Process();
	void	GUILDMGR_5MinProcess();

	void	GuildHuntedMonsterCountDBUpdate();
	void	CheckDateForGuildHuntedMonsterCountConvertToGuildPoint();
	void	CheckGuildPlustimeEnd();

	BOOL	CheckValidPlustimeKind(GUILDPOINT_INFO* pGPInfo, DWORD GuildIdx, DWORD plusTimeKind);

	void AddGuildTraineeInfo(GUILD_TRAINEE_LIST* temp);
	void AddGuildTraineeGuildInfo(GUILD_TRAINEE_GUILDLIST* temp);
	void AddGuildTraineeGuildIntroInfo(GUILD_INTRO_INFO* temp);
	//////////////////////////////////////////////////////////////////////////
	void GuildStudentLvUpPointDBUpdate(CPlayer* pPlayer);

	void SetGuildStudentLvUpCount(DWORD GuildIdx, DWORD GuildStudentLvUpCount, BOOL bMasterChecking);

	void GiveGuildCerebrationMoney(DWORD GuildIdx);
	void PutInGuildCerabrationMoney(CGuild* pGuild, DWORD Money);

	cPtrList * GetGuildTraineeList()	{ return &m_GuildTraineeList; };

	BOOL	IsSameGuild(CPlayer* pPlayerA, CPlayer* pPlayerB);
public:
	void Guild_Create_Syn(void* pMsg);
	void Guild_BreakUp_Syn(void* pMsg);
	void Guild_DeleteMember_Syn(void* pMsg);
	void Guild_AddMember_Syn(void* pMsg);
	void Guild_Invite_Accept(void* pMsg);
	void Guild_Invite_Deny(void* pMsg);
	void Guild_Secede_Syn(void* pMsg);
	void Guild_LevelUp_Syn(void* pMsg);
	void Guild_ChangeRank_Syn(void* pMsg);
	void Guild_GiveNickName_Syn(void* pMsg);
	void Guild_PutIn_Money_Syn(void* pMsg);
	void Guild_PutOut_Money_Syn(void* pMsg);
	void Guild_Create_Notify_To_Map(void* pMsg);
	void Guild_BreakUp_Notify_To_Map(void* pMsg);
	void Guild_DeleteMember_Notify_To_Map(void* pMsg);
	void Guild_AddMember_Notify_To_Map(void* pMsg);
	void Guild_Secede_Notify_To_Map(void* pMsg);
	void Guild_LevelUp_Notify_To_Map(void* pMsg);
	void Guild_ChangeRank_Notify_To_Map(void* pMsg);
	void Guild_MemberLevel_Notify_To_Map(void* pMsg);
	void Guild_GiveNickName_Notify_To_Map(void* pMsg);
	void Guild_LogInfo_Notify_To_Map(void* pMsg);
	void Guild_Mark_UpDate_Syn(void* pMsg);
	void Guild_Mark_Request_Syn(void* pMsg);
	void Guild_Mark_Notify_To_Map(void* pMsg);
	void Guild_AddStudent_Syn(void* pMsg);
	void Guild_Invite_As_Student_Accept(void* pMsg);
	void Guild_HuntedMonster_TotalCount_To_Map(void* pMsg);
	void Guild_Point_UseInfo_To_Map(void* pMsg);
	void Guild_Point_To_Map(void* pMsg);
	void Guild_Point_Add_To_Map(void* pMsg);
	void Guild_PlusTime_Set_To_Map(void* pMsg);
	void Guild_PlusTime_End_To_Map(void* pMsg);
	void Guild_PointUse_Syn(void* pMsg);
	void Guild_Get_HuntedMonster_TotalCount_Syn(void* pMsg);
	void Guild_Munpa_Regist_Syn(void* pMsg);
	void Guild_Munpa_Regist_Brod(void* pMsg);
	void Guild_Munpa_Regist_Intro_Brod(void* pMsg);
	void Guild_Munpa_Delete_Syn(void* pMsg);
	void Guild_Munpa_Delete_Brod(void* pMsg);
	void Guild_Munpa_Catalog_Syn(void* pMsg);
	void Guild_InfoUpdate_Syn(void* pMsg);
	void Guild_InfoUpdate_Brod(void* pMsg);
	void Guild_Munpa_Join_Syn(void* pMsg);
	void Guild_Munpa_Join_Brod(void* pMsg);
	void Guild_Munha_Catalog_Syn(void* pMsg);
	void Guild_Munha_Delete_Syn(void* pMsg);
	void Guild_Munha_Delete_Brod(void* pMsg);
	void Guild_Munha_UserDelete_Syb(void* pMsg);
	void Guild_Munha_Accept_Syn(void* pMsg);
	void Guild_Student_LvUp_CountInfo(void* pMsg);
	void Guild_CelebrationMoney_Master_Checked(void* pMsg);
	void Guild_CelebrationMoney_Notify(void* pMsg);
	void Guild_CelebrationMoney_To_Guildwear(void* pMsg);
	void Guild_SOS_Send_Syn(void* pMsg);
	void Guild_SOS_Send_Cancel(void* pMsg);
	void Guild_SOS_Send_Die_Check(void* pMsg);
	void Guild_SOS_Send_Die_Check_ToMap(void* pMsg);
	void Guild_SOS_Send_No(void* pMsg);
};
#endif // !defined(AFX_GUILDMANAGER_H__FE162F77_2547_40A8_8C25_54C49BC0ED05__INCLUDED_)


