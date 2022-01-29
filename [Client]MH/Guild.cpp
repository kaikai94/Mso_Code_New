







// Guild.cpp: implementation of the CGuild class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Guild.h"
#include "GameIn.h"
#include "GuildDialog.h"
#include "GuildPlusTimeDialog.h"
#include "ObjectManager.h"
#include "GuildManager.h"
#include "GuildMunhaDialog.h"
#include "MHMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuild::CGuild()
{
	m_GuildIdx = 0;
	m_SortType = eRank;

	memset(m_GuildNotice, 0, MAX_GUILD_NOTICE+1);

	m_nStudentCount = 0;
}

CGuild::~CGuild()
{
	Release();
}

void CGuild::Init(GUILDINFO* pInfo)
{
	Release();
	if(pInfo)
	{
		m_GuildIdx = pInfo->GuildIdx;
		m_GuildLvl = pInfo->GuildLevel;
		m_GuildLocation = (BYTE)pInfo->MapNum;
		SafeStrCpy(m_MasterName, pInfo->MasterName, MAX_NAME_LENGTH+1);
		SafeStrCpy(m_GuildName, pInfo->GuildName, MAX_GUILD_NAME+1);
		strcpy(m_GuildNotice, pInfo->GuildNotice);
		m_UnionIdx = pInfo->UnionIdx;
		for(int i=0; i<eRankPos_Max; ++i)
			m_RankMemberIdx[i] = 0;
		
		m_SortType = eRank;
	}
	m_nStudentCount = 0;
}

void CGuild::Release()

{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while( pos )
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		SAFE_DELETE(pInfo);
	}
	m_MemberList.RemoveAll();

	m_UnionIdx = 0;
	for(int i=0; i<eRankPos_Max; ++i)
		m_RankMemberIdx[i] = 0;
	
	m_MemberMultiMap.clear();
	m_MemberMultiMap_LoginOut.clear();
}

BOOL CGuild::DeleteMember(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetAt(pos);
		if(pInfo)
		{
			if(pInfo->Rank == GUILD_STUDENT)
			{
				if(pInfo->MemberIdx == PlayerIDX)
				{
					--m_nStudentCount;
					char buf[64] = { 0, };
					sprintf(buf, "%s", pInfo->MemberName);
					GAMEIN->GetGuildMunhaDialog()->DeleteMunHaInfo(buf);
				}
			}

			if(pInfo->MemberIdx == PlayerIDX)
			{
				ResetRankMemberInfo(PlayerIDX, pInfo->Rank);
				SAFE_DELETE(pInfo);
				m_MemberList.RemoveAt(pos);
				RefreshMember();
				return TRUE;
			}
		}
		m_MemberList.GetNext(pos);
	}

	return FALSE;
}

BOOL CGuild::AddMember(GUILDMEMBERINFO* pMemberInfo)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)

	{
		pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

		if(pInfo)
		if(pInfo->MemberIdx == pMemberInfo->MemberIdx)

		{
			return FALSE;

		}
	}
	
	pInfo = new GUILDMEMBERINFO;
	pInfo->InitInfo(pMemberInfo->MemberIdx, pMemberInfo->MemberName, pMemberInfo->Memberlvl, pMemberInfo->Rank, pMemberInfo->bLogged);

	m_MemberList.AddTail(pInfo);
	if (pInfo->Rank == GUILD_VICEMASTER)
	{
		int pos = IsVacancy(GUILD_VICEMASTER);
		if ((pos <0) || (pos > eRankPos_Max))
		{
			ASSERTMSG(0, "Load GuildMemberInfo - Set Ranked Member");
			return TRUE;
		}
		m_RankMemberIdx[pos] = pInfo->MemberIdx;
	}
	else if(pInfo->Rank == GUILD_SENIOR)
	{
		int pos = IsVacancy(GUILD_SENIOR);
		if((pos <0) || (pos > eRankPos_Max))

		{
			ASSERTMSG(0, "Load GuildMemberInfo - Set Ranked Member");
			return TRUE;
		}
		m_RankMemberIdx[pos] = pInfo->MemberIdx;
	}

	if(pInfo->Rank == GUILD_STUDENT)
	{
		++m_nStudentCount;
		char buf[64] = { 0, };
		sprintf(buf, "%s", pInfo->MemberName);

		GAMEIN->GetGuildMunhaDialog()->SetMunHaInfo(buf);
	}

	return TRUE;
}

BOOL CGuild::IsMember(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();

	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
	
		if(pInfo)
		if(pInfo->MemberIdx == PlayerIDX)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CGuild::SetTotalInfo(GUILDINFO* pInfo, GUILDMEMBERINFO* pMemberInfo, int MemberNum)
{
	m_GuildIdx = pInfo->GuildIdx;
	m_GuildLvl = pInfo->GuildLevel;

	if( MemberNum > 100 )
		MemberNum = 100;
	GAMEIN->GetGuildDlg()->SetListDlgMaxLine(MemberNum+1);
	for(int i=0; i<MemberNum; ++i)
	{
		if(HEROID == pMemberInfo[i].MemberIdx)
			pMemberInfo[i].bLogged = TRUE;
		AddMember(&pMemberInfo[i]);
	}
	m_GuildLocation = (BYTE)pInfo->MapNum;
	SafeStrCpy(m_MasterName, pInfo->MasterName, MAX_NAME_LENGTH+1);
    SafeStrCpy(m_GuildName, pInfo->GuildName , MAX_GUILD_NAME+1);
	strcpy(m_GuildNotice, pInfo->GuildNotice);
	RefreshMember();	
}

void CGuild::RefreshMember()
{
	SortbyType();
	GAMEIN->GetGuildDlg()->DeleteMemberAll();
	GAMEIN->GetGuildDlg()->SetLoginNum(m_MemberMultiMap.size());
	multimap< RANK, GUILDMEMBERINFO* >::reverse_iterator rit;
	for(rit= m_MemberMultiMap.rbegin(); rit != m_MemberMultiMap.rend(); ++rit)
	{
		GAMEIN->GetGuildDlg()->ResetMemberInfo(rit->second);
	}
	for(rit = m_MemberMultiMap_LoginOut.rbegin();rit!=m_MemberMultiMap_LoginOut.rend();++rit)
	{
		GAMEIN->GetGuildDlg()->ResetMemberInfo(rit->second);
	}
}

GUILDMEMBERINFO* CGuild::GetMemberInfo(DWORD PlayerIDX)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if(pInfo)
		if(pInfo->MemberIdx == PlayerIDX)
		{
			return pInfo;
		}
	}
	return NULL;
}

char* CGuild::GetMemberName(DWORD MemberID)
{
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();
	
	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if(pInfo)
		if(pInfo->MemberIdx == MemberID)
		{
			return pInfo->MemberName;
		}
	}
	return "";
}

DWORD CGuild::GetMemberIdx(int num)
{
	int i=0;
	if(GetSortType() == eRank)
	{
		multimap< RANK, GUILDMEMBERINFO* >::reverse_iterator rit;
		
		for(rit = m_MemberMultiMap.rbegin(); rit != m_MemberMultiMap.rend(); rit++)
		{
			if(i == num)
			{	
				GUILDMEMBERINFO* pInfo = (*rit).second;
				return pInfo->MemberIdx;
			}
			i++;
		}
		for(rit = m_MemberMultiMap_LoginOut.rbegin();rit!=m_MemberMultiMap_LoginOut.rend();rit++)
		{
			if(i==num)
			{
				GUILDMEMBERINFO* pInfo = (*rit).second;
				return pInfo->MemberIdx;
			}
			i++;
		}

	}		
	return 0;
}

void CGuild::SetLevel(BYTE Level)
{
	m_GuildLvl = Level;
}

BYTE CGuild::GetMaxMemberNum()
{//[����Ա��������	][By:ʮ���½���][QQ:112582793][2017/12/7]
	if(( m_GuildLvl <= 0) || ( m_GuildLvl > GUILD_5LEVEL))
	{
		char buf[64];
		sprintf(buf, "Wrong Member Num, GuildIdx : %u", m_GuildIdx);
		ASSERTMSG(0, buf);
		return 0;
	}
#ifdef _CHINA_LOCAL_

	switch( m_GuildLvl )
	{
	case 1:	return 10;
	case 2:	return 15;
	case 3:	return 20;
	case 4:	return 30;
	case 5:	return 50;
	}
	return 0;
#else
	return 10 + ( (m_GuildLvl-1) * 10 ); 
#endif
}

BOOL CGuild::CanAddMember()
{
	if( GetMemberNum() - GetStudentNum() < GetMaxMemberNum() )
		return TRUE;
	else
		return FALSE;
}

int CGuild::IsVacancy(BYTE ToRank)
{
	int end = 0;
	int start = 0;
	switch(ToRank)
	{
	case GUILD_VICEMASTER:
		{
			start = 0; end = 2;
			//start = 0; end = 1;
		}

		break;
	case GUILD_SENIOR:
		{
			//start = 1; end = 3;
			start = 2; end = 7;
		}
		break;
	case GUILD_MEMBER:
		return eRankPos_Max;
		
	default:
		ASSERTMSG(0, "IsVancancy Err.");
		return eRankPos_Err;
	}

	int rt = eRankPos_Err;
	for(int i=start; i<end; ++i)
	{
		if( m_RankMemberIdx[i] == 0 )
		{
			return i;
		}
	}
	return rt;
}


BOOL CGuild::ChangeRank(DWORD MemberID, BYTE Rank)

{
	int Pos = IsVacancy(Rank);
	if(Pos == eRankPos_Err)
	{
		ASSERTMSG(0, "Client Change Rank IsVacancy Error");
		return FALSE;
	}
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberID);
	if(pMemberInfo == 0)
	{
		ASSERTMSG(0, "Change Rank - No Member Data");
		return FALSE;
	}
	if((Pos <0) || (Pos >eRankPos_Max))
	{
		ASSERTMSG(0, "change rank pos boundary error");
		return FALSE;
	}
	ResetRankMemberInfo(MemberID, pMemberInfo->Rank);

	if(pMemberInfo->Rank == GUILD_STUDENT && Rank != GUILD_STUDENT)
	{
		--m_nStudentCount;
		char buf[64] = { 0, };
		sprintf(buf, "%s", pMemberInfo->MemberName);
		GAMEIN->GetGuildMunhaDialog()->DeleteMunHaInfo(buf);
	}

	pMemberInfo->Rank = Rank;
	if(Pos == eRankPos_Max)
		return TRUE;
	m_RankMemberIdx[Pos] = MemberID;
	return TRUE;
}
RTChangeRank CGuild::GetRankPos(DWORD MemberIdx)
{
	for (int i = eRankPos_1; i < eRankPos_Max; i++)
	{
		if (m_RankMemberIdx[i] == MemberIdx)
			return (RTChangeRank)i;
	}
	return eRankPos_Err;
}
void CGuild::ResetRankMemberInfo(DWORD MemberIdx, BYTE Rank)
{
	RTChangeRank pos = GetRankPos(MemberIdx);
	if (pos == eRankPos_Err)
	{
		ASSERTMSG(0, "ResetRankMemberInfo() - No Rank");
		return;
	}
	else
		m_RankMemberIdx[pos] = 0;
	//switch(Rank)
	//{
	//case GUILD_VICEMASTER:
	//	m_RankMemberIdx[0] = 0; break;
	//case GUILD_SENIOR:
	//	{
	//		if(m_RankMemberIdx[1] == MemberIdx)
	//			m_RankMemberIdx[1] = 0;
	//		else if(m_RankMemberIdx[2] == MemberIdx)
	//			m_RankMemberIdx[2] = 0;
	//		else 
	//			ASSERTMSG(0, "ResetRankMemberInfo() - No Rank");
	//	}
	//	break;
	//}
}

void CGuild::SetMemberLevel(DWORD MemberIdx, DWORD MemberLvl)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
	if(pMemberInfo == 0)
		return;
	pMemberInfo->Memberlvl = (LEVELTYPE)MemberLvl;

	SortbyType();
	GAMEIN->GetGuildDlg()->DeleteMemberAll();

	multimap< RANK, GUILDMEMBERINFO* >::reverse_iterator rit;
	for(rit= m_MemberMultiMap.rbegin(); rit != m_MemberMultiMap.rend(); ++rit)
	{
		GAMEIN->GetGuildDlg()->ResetMemberInfo(rit->second);
	}
	for(rit = m_MemberMultiMap_LoginOut.rbegin();rit!=m_MemberMultiMap_LoginOut.rend();++rit)
	{
		GAMEIN->GetGuildDlg()->ResetMemberInfo(rit->second);
	}
	/*GAMEIN->GetGuildDlg()->DeleteMemberAll();
	PTRLISTPOS pos = m_MemberList.GetHeadPosition();


	while(pos)
	{
		GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);
		
		if(pInfo)
			GAMEIN->GetGuildDlg()->ResetMemberInfo(pInfo);	
	}*/
}

void CGuild::SetLogInfo(DWORD MemberIdx, BOOL vals)
{
	GUILDMEMBERINFO* pMemberInfo = GetMemberInfo(MemberIdx);
	if(pMemberInfo == 0)
		return;
	pMemberInfo->bLogged = vals;

	//dlg refresh
}

void CGuild::SetSortType(BYTE type)
{
	m_SortType = type;	
}

BYTE CGuild::GetSortType()
{
	return m_SortType;
}

void CGuild::SortbyType()
{
	if(m_SortType == eRank)
	{
		m_MemberMultiMap.clear();
		m_MemberMultiMap_LoginOut.clear();
		PTRLISTPOS pos = m_MemberList.GetHeadPosition();

		while(pos)
		{
			GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)m_MemberList.GetNext(pos);

			if(pInfo)
			{
				if( pInfo->bLogged )
					m_MemberMultiMap.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(10+pInfo->Rank, pInfo));
				else
					m_MemberMultiMap_LoginOut.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(pInfo->Rank, pInfo));
			}

//			m_MemberMultiMap.insert(multimap< RANK, GUILDMEMBERINFO* >::value_type(pInfo->Rank, pInfo));
		}		
	}
}

void CGuild::SetGuildPosition( DWORD dwMapnum )
{
	m_GuildLocation = (BYTE)dwMapnum;
	// magi82(37) �� �� ������
	GAMEIN->GetGuildDlg()->SetGuildPosition( MAP->GetMapName( (MAPTYPE)dwMapnum ) );	
}

BOOL CGuild::CanAddStudent()
{
//#define GUILD_STUDENT_NUM_MAX	25
	return ( GetStudentNum() < GUILD_STUDENT_NUM_MAX );
}

void CGuild::AddGuildPlustime( DWORD plustimeKind, DWORD plustimeIdx, DWORD endTime )
{
	m_GuildPoint.GuildPlusTimeflg |= (1<<plustimeKind);

	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeIdx = plustimeIdx;
	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = endTime;
}

void CGuild::RemoveGuildPlustime( DWORD plustimeKind )
{
	m_GuildPoint.GuildPlusTimeflg ^= (1<<plustimeKind);

	m_GuildPoint.GuildUsingPlusTimeInfo[plustimeKind].PlusTimeEndTime = 0;
}

BOOL CGuild::IsUsingPlustime( DWORD plustimeKind )
{
	return ( m_GuildPoint.GuildPlusTimeflg & (1<<plustimeKind) );
}

void CGuild::SetGuildPointInfo( GUILDPOINT_INFO* pGuildPointInfo )
{

	//memcpy(&m_GuildPoint, pGuildPointInfo, sizeof(GUILDPOINT_INFO));

	SetGuildPoint(pGuildPointInfo->GuildPoint);
	//m_GuildPoint.GuildHuntedMonsterCount = pGuildPointInfo->GuildHuntedMonsterCount;
	m_GuildPoint.GuildHuntedMonsterTotalCount = pGuildPointInfo->GuildHuntedMonsterTotalCount;
	m_GuildPoint.DBProcessTime = pGuildPointInfo->DBProcessTime;

	m_GuildPoint.GuildPlusTimeflg = 0;
	for( int n = 0; n < eGPT_Kind_Max; ++n )
	{
		int PlustimeIdx = pGuildPointInfo->GuildUsingPlusTimeInfo[n].PlusTimeIdx;
		//AddGuildPlustime(m_GuildPoint.GuildUsingPlusTimeInfo[n].PlusTimeKind, m_GuildPoint.GuildUsingPlusTimeInfo[n].PlusTimeEndTime);
		if(PlustimeIdx)
		{
			AddGuildPlustime(n, PlustimeIdx, pGuildPointInfo->GuildUsingPlusTimeInfo[n].PlusTimeEndTime);
			GUILDMGR->AddGuildPlustimeStatusIcon(PlustimeIdx);
		}
	}
}

void CGuild::SetGuildPoint( int TotalPoint )
{
	m_GuildPoint.GuildPoint = TotalPoint;

	CGuildPlusTimeDlg* pDlg = GAMEIN->GetGuildPlusTimeDlg();
	if(pDlg)
	{
		pDlg->SetGuildPointText(TotalPoint);
	}

}





