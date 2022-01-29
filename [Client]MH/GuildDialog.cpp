







// GuildDialog.cpp: implementation of the CGuildDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildDialog.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cComboBox.h"
#include "./interface/cListDialog.h"
#include "./interface/cPushupButton.h"
#include "WindowIDEnum.h"

#include "ObjectManager.h"
#include "GuildManager.h"
#include "ChatManager.h"
#include "../Interface/cResourceManager.h"
#include "cMsgBox.h"
#include "Guild.h"
#include "../Input/Mouse.h"
#include "GameIn.h"
#include "GuildRankDialog.h"

#include "GuildNickNameDialog.h"
#include "GuildUnion.h"
#include <list>
#include "MHMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildDialog::CGuildDialog()
{
	m_type = WT_GUILDDLG;
	m_CurGuildMode = -1;
	m_nPositionFlag = 1;
	m_nLevelFlag = 1;
	
}

CGuildDialog::~CGuildDialog()
{
/*	for(int n=0;n<eMaxFunc;++n)
	{
		m_GuildFuncCtrlArray[n].RemoveAll();

	}

	for(n=0; n<eGuildMode_Max; ++n)
	{
		m_GuildModeArray[n].RemoveAll();
	}
	*/
	
}

void CGuildDialog::Linking()
{
	m_pGuildName = (cStatic*)GetWindowForID(GD_NAME);
	m_pGuildLvl = (cStatic*)GetWindowForID(GD_LEVEL);
	m_pMasterName = (cStatic*)GetWindowForID(GD_MASTER);
	m_pMemberNum = (cStatic*)GetWindowForID(GD_MEMBERNUM);
	m_pLocation = (cStatic*)GetWindowForID(GD_LOCATION);
	m_pUnionName = (cStatic*)GetWindowForID(GD_UNIONNAME);
	m_CurLoginNum = (cStatic*)GetWindowForID(GD_MEMBERLOGINCOUNT);
	m_pListDlg = (cListDialog*)GetWindowForID(GD_MEMBERLIST);	
	m_pListDlg->SetShowSelect(TRUE);	
	m_pListDlg->SetHeight(158);

	for( int i = 0; i < MAX_GUILDDIALOG_STATIC; ++i )
		m_pGuildStatic[i] = (cStatic*)GetWindowForID(GD_NAME+i);
	
	for(int i = 0; i < 2; ++i )
		m_pGuildPushupBtn[i] = (cPushupButton*)GetWindowForID(GD_MEMBERINFOPUSHUPBTN+i);
	m_pRelationBtn = (cButton*)GetWindowForID( GD_RELATIONBTN );
	
	for(int i = 0; i < MAX_GUILDDIALOG_BUTTON; ++i )
		m_pGuildBtn[i] = (cButton*)GetWindowForID(GD_BANMEMBER+i);


/*	for(i=0; i<10; ++i)
	{
		m_GuildModeArray[eGuildMode_GuildInfo].AddTail((cStatic*)GetWindowForID(GD_NAME+i));
	}
	for(i=0; i<12; ++i)
	{
		m_GuildModeArray[eGuildMode_Func].AddTail((cButton*)GetWindowForID(GD_BANMEMBER+i));
	}
	for(i=0; i<4; ++i)
		m_GuildFuncCtrlArray[eInvailedFunc].AddTail((cButton*)GetWindowForID(GD_INVITEUNION+i));
*/

/*	m_pBanMemberBtn = (cButton*)GetWindowForID(GD_BANMEMBER);
	m_pGiveMemberNickBtn = (cButton*)GetWindowForID(GD_GIVEMEMBERNICK);
	m_pGiveMemberRankBtn = (cButton*)GetWindowForID(GD_GIVEMEMBERRANK);
	m_pInviteMemberBtn = (cButton*)GetWindowForID(GD_INVITEMEMBER);
	m_pInviteUnionBtn = (cButton*)GetWindowForID(GD_INVITEUNION);
	m_pSecedeBtn = (cButton*)GetWindowForID(GD_SECEDE);
	m_pDeclareBtn = (cButton*)GetWindowForID(GD_DECLARE);
	m_pSuggestBtn = (cButton*)GetWindowForID(GD_SUGGEST);
	m_pSurrenderBtn = (cButton*)GetWindowForID(GD_SURRENDER);


	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pInviteUnionBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pGiveMemberRankBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pGiveMemberNickBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pBanMemberBtn);
	m_GuildFuncCtrlArray[eViceMasterFunc].AddTail(m_pInviteMemberBtn);
	m_GuildFuncCtrlArray[eMemberFunc].AddTail(m_pSecedeBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pDeclareBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pSuggestBtn);
	m_GuildFuncCtrlArray[eMasterFunc].AddTail(m_pSurrenderBtn);	
*/	
}

void CGuildDialog::SetInfo(char* guildname, BYTE guildlvl, char* mastername, BYTE membernum, BYTE location)
{
	m_pGuildName->SetStaticText(guildname);
	m_pGuildLvl->SetStaticValue(guildlvl);
}

void CGuildDialog::DeleteMemberAll()
{	
	m_pListDlg->RemoveAll();
	m_MemberList.clear();
	std::vector<GUILDMEMBERINFO>().swap(m_MemberList);
}

void CGuildDialog::ResetMemberInfo(GUILDMEMBERINFO * pInfo)
{
	char buf[64] = {" ",};
	char rank[8] = {0,};
	int namelen;
	int ranklen;
	char membername[MAX_NAME_LENGTH+1];
	char memberrank[7];
	m_MemberList.push_back(*pInfo);
	SafeStrCpy(membername, pInfo->MemberName, MAX_NAME_LENGTH+1);
	namelen = strlen(pInfo->MemberName);
	for(int i=namelen; i<MAX_NAME_LENGTH; ++i)
		membername[i] = 0x20;
	
	SafeStrCpy(memberrank, GUILDMGR->GetRankName(pInfo->Rank), 7);
	ranklen = strlen(memberrank);
	for(int i=ranklen; i<6; ++i)
		memberrank[i] = 0x20;
	sprintf(buf, "%s %10s %4d", membername, memberrank, pInfo->Memberlvl);
	if(pInfo->bLogged == TRUE)
		m_pListDlg->AddItem(buf, 0xffffffff);
	else
		m_pListDlg->AddItem(buf, RGBA_MAKE(172,182,199,255));
}
void CGuildDialog::SetGuildInfo( char* GuildName, char* MasterName, char* MapName, BYTE GuildLevel, BYTE MemberNum, char* pUnionName)
{
	m_pGuildName->SetStaticText(GuildName);		
	m_pGuildLvl->SetStaticValue(GuildLevel);
	m_pMasterName->SetStaticText(MasterName);
	m_pMemberNum->SetStaticValue(MemberNum);
	m_pLocation->SetStaticText(MapName);
	m_pUnionName->SetStaticText(pUnionName);
}

void CGuildDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		ShowGuildMode(eGuildMode_GuildInfo);
	}
	if(val == FALSE)
	{
		cMsgBox* pSece =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_GUILD_SECEDE);
		if(pSece)
			WINDOWMGR->AddListDestroyWindow( pSece );
		cMsgBox* pBan =  (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_GUILD_BAN);
		if(pBan)
			WINDOWMGR->AddListDestroyWindow( pBan );
		if(GAMEIN->GetGuildRankDlg()->IsActive() == TRUE)
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
	}
	cDialog::SetActive(val);
}

void CGuildDialog::RefreshGuildInfo(CGuild* pGuild)
{
	m_pGuildName->SetStaticText(pGuild->GetGuildName());		
	m_pGuildLvl->SetStaticValue(pGuild->GetLevel());
	m_pMasterName->SetStaticText(pGuild->GetMasterName());
	m_pMemberNum->SetStaticValue(pGuild->GetMemberNum());

	m_pLocation->SetStaticText(MAP->GetMapName(pGuild->GetLocation()));
	m_pUnionName->SetStaticText(GUILDUNION->GetGuildUnionName());
}

void CGuildDialog::SetGuildUnionName()
{
	m_pUnionName->SetStaticText(GUILDUNION->GetGuildUnionName());
}

DWORD CGuildDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pListDlg->GetCurSelectedRowIdx();
				if(Idx != -1)
				{
					GUILDMGR->SetSelectedMemberNum(Idx);
					if(GAMEIN->GetGuildRankDlg()->IsActive() == TRUE)
						GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
					if(GAMEIN->GetGuildNickNameDlg()->IsActive() == TRUE)
						GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);
					cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID(MBI_GUILD_BAN);
					if(pMsgBox)
						WINDOWMGR->AddListDestroyWindow( pMsgBox );
				}
			}
		}
	}
	return we;
}

void CGuildDialog::ShowGuildMode(BYTE ShowMode)
{
	SetGuildPushupBtn(ShowMode);
	if(m_CurGuildMode == ShowMode)	return;

	SetActiveGuildMode(ShowMode,TRUE);
	m_CurGuildMode = ShowMode;
	m_CurLoginNum->SetActive(TRUE);
//	if(m_CurGuildMode  != -1)
//	{
//		ClearDisableBtn();
//		SetActiveGuildMode(m_CurGuildMode ,FALSE);
//	}
//
//	SetActiveGuildMode(ShowMode,TRUE);
//	m_CurGuildMode = ShowMode;

//	SetDisableFuncBtn(HERO->GetGuildMemberRank());
}

void CGuildDialog::SetActiveGuildMode(int showmode,BOOL bActive)
{
	if( showmode == eGuildMode_GuildInfo )
	{
		for( int i = 0; i < MAX_GUILDDIALOG_STATIC; ++i )
			m_pGuildStatic[i]->SetActive( TRUE );
		for(int i = 0; i < MAX_GUILDDIALOG_BUTTON; ++i )
		{
			if( m_pGuildBtn[i] )
			{
				m_pGuildBtn[i]->SetDisable( FALSE );
				m_pGuildBtn[i]->SetActive( FALSE );
			}
		}		
	}
	else if( showmode == eGuildMode_Func )
	{
		for( int i = 0; i < MAX_GUILDDIALOG_STATIC; ++i )
			m_pGuildStatic[i]->SetActive( FALSE );

		for( int i = 0; i < MAX_GUILDDIALOG_BUTTON; ++i )
		{
			if( m_pGuildBtn[i] )
			{
				m_pGuildBtn[i]->SetActive( TRUE );
				m_pGuildBtn[i]->SetDisable( TRUE );									
			}
		}
		BYTE bRank = HERO->GetGuildMemberRank();
		switch( bRank )
		{
		case GUILD_MASTER:
			{
				m_pGuildBtn[0]->SetDisable( FALSE );
				m_pGuildBtn[1]->SetDisable( FALSE );
				m_pGuildBtn[2]->SetDisable( FALSE );
				m_pGuildBtn[3]->SetDisable( FALSE );
				if( m_pGuildBtn[5] )
					m_pGuildBtn[5]->SetDisable( FALSE );
				
				if( GUILDUNION->GetGuildUnionIdx() )
				{
					if( GUILDUNION->IsGuildUnionMaster( HERO ) )
					{
						m_pGuildBtn[6]->SetDisable( FALSE );
						m_pGuildBtn[8]->SetDisable( FALSE );
					}
					else
						m_pGuildBtn[7]->SetDisable( FALSE );
				}					
				m_pGuildBtn[9]->SetDisable( FALSE );
				m_pGuildBtn[10]->SetDisable( FALSE );
				m_pGuildBtn[11]->SetDisable( FALSE );
				if( m_pGuildBtn[12] )	
					m_pGuildBtn[12]->SetDisable( FALSE );
				if( m_pGuildBtn[13] )	//hk block
					m_pGuildBtn[13]->SetDisable( FALSE );
			}
			break;
		case GUILD_VICEMASTER:
			{
				m_pGuildBtn[3]->SetDisable( FALSE );
				m_pGuildBtn[4]->SetDisable( FALSE );
				if( m_pGuildBtn[5] )	
					m_pGuildBtn[5]->SetDisable( FALSE );
				if( m_pGuildBtn[12] )
					m_pGuildBtn[12]->SetDisable( FALSE );
				if( m_pGuildBtn[13] )	//hk block
					m_pGuildBtn[13]->SetDisable( FALSE );//magi82 - 巩窍积 啊涝祈府矫胶袍

			}
			break;
		case GUILD_SENIOR:
			{
				m_pGuildBtn[3]->SetDisable( FALSE );
				m_pGuildBtn[4]->SetDisable( FALSE );
				if( m_pGuildBtn[12] )	
					m_pGuildBtn[12]->SetDisable( FALSE );
				if( m_pGuildBtn[13] )	//hk block
					m_pGuildBtn[13]->SetDisable( FALSE );//magi82 - 巩窍积 啊涝祈府矫胶袍
			}
			break;
		case GUILD_MEMBER:
		case GUILD_STUDENT:
			{
				m_pGuildBtn[4]->SetDisable( FALSE );
			}

			break;
		}
	}
//	PTRLISTSEARCHSTART(m_GuildModeArray[showmode],cWindow*,pWin)
//		pWin->SetActive(bActive);

//	PTRLISTSEARCHEND
}

void CGuildDialog::SetGuildPushupBtn(BYTE ShowMode)

{
	if((ShowMode < 0) || (ShowMode > 2))
	{

		ASSERT(0);
		return;
	}
	for(int i=0; i<2; ++i)
		m_pGuildPushupBtn[i]->SetPush(FALSE);
	m_pGuildPushupBtn[ShowMode]->SetPush(TRUE);
}

void CGuildDialog::ClearDisableBtn()
{
//	PTRLISTSEARCHSTART(m_GuildModeArray[eGuildMode_Func],cWindow*,pWin)
//		pWin->SetDisable(FALSE);
//	PTRLISTSEARCHEND
}

void CGuildDialog::SetDisableFuncBtn(BYTE Rank)
{
/*	int i = 0;
	switch(Rank)

	{
	case GUILD_MASTER:	i = eInvailedFunc;	break;

	case GUILD_VICEMASTER:	i = eMasterFunc;	break;
	case GUILD_SENIOR:	i = eViceMasterFunc;	break;
	case GUILD_MEMBER:	i = eSeniorFunc;	break;
	}

	for(int j=0; j<i; ++j)
	{		
		PTRLISTSEARCHSTART(m_GuildFuncCtrlArray[j],cWindow*,pWin)
			pWin->SetDisable(FALSE);
		PTRLISTSEARCHEND
	}
	for(j=i; j<eMaxFunc; ++j)
	{		
		PTRLISTSEARCHSTART(m_GuildFuncCtrlArray[j],cWindow*,pWin)
			pWin->SetDisable(TRUE);
		PTRLISTSEARCHEND
	}
	if(Rank == GUILD_MASTER)
	{
		m_pSecedeBtn->SetDisable(TRUE);
	}
	*/
}

void CGuildDialog::SetGuildPosition( char* MapName )
{
	m_pLocation->SetStaticText( MapName );
}

void CGuildDialog::SortMemberListbyPosition()
{
	if(m_nPositionFlag)
	{
		std::list<GUILDMEMBERINFO> ChangeList;
		std::list<GUILDMEMBERINFO>::iterator iter;
		std::vector<GUILDMEMBERINFO>* prevList = &m_MemberList;

		for(unsigned int i = 0; i < prevList->size(); ++i)
		{
			GUILDMEMBERINFO curmemberinfo = (*prevList)[i]; 

			if(ChangeList.size() > 0)
			{
				bool bEnd = TRUE;

				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
				{
					BYTE Rank = iter->Rank;

				    if(curmemberinfo.Rank > Rank) 
					{
                        ChangeList.insert(iter,curmemberinfo);
						bEnd = FALSE;
						break;
					}
				}

				if(TRUE == bEnd)
                ChangeList.push_back(curmemberinfo);

			}
			else
			{
				ChangeList.push_back(curmemberinfo);
			}
		}

		m_MemberList.clear();
		m_pListDlg->RemoveAll();

		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
		{

			GUILDMEMBERINFO info = *iter;
			ResetMemberInfo(&info);
			
		}


		
	}
	else
	{
		std::list<GUILDMEMBERINFO> ChangeList;
		std::list<GUILDMEMBERINFO>::iterator iter;

		std::vector<GUILDMEMBERINFO>* prevList = &m_MemberList;

		unsigned int size = prevList->size();

		for(unsigned int i = 0; i < prevList->size(); ++i)
		{
			GUILDMEMBERINFO curmemberinfo = (*prevList)[i];

			if(ChangeList.size() > 0)
			{
				bool bEnd = TRUE;

				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
				{
					BYTE Rank = iter->Rank;

					if(curmemberinfo.Rank < Rank) 
					{
						ChangeList.insert(iter,curmemberinfo);
						bEnd = FALSE;
						break;
					}
				}

				if(TRUE == bEnd)
					ChangeList.push_back(curmemberinfo);

			}
			else
			{
				ChangeList.push_back(curmemberinfo);
			}
		}

		m_MemberList.clear();
		m_pListDlg->RemoveAll();

		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
		{
			GUILDMEMBERINFO info = *iter;
			ResetMemberInfo(&info);
			
		}
	}

	m_nPositionFlag ^= 1;
}

void CGuildDialog::SortMemberListbyLevel()
{
	if(m_nLevelFlag)
	{
		std::list<GUILDMEMBERINFO> ChangeList;
		std::list<GUILDMEMBERINFO>::iterator iter;
		std::vector<GUILDMEMBERINFO>* prevList = &m_MemberList;

		for(unsigned int i = 0; i < prevList->size(); ++i)
		{
			GUILDMEMBERINFO curmemberinfo = (*prevList)[i];

			if(ChangeList.size() > 0)
			{
				bool bEnd = TRUE;

				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
				{
					WORD lvl = iter->Memberlvl;


					if(curmemberinfo.Memberlvl > lvl)
					{
						ChangeList.insert(iter,curmemberinfo);
						bEnd = FALSE;
						break;
					}
				}

				if(TRUE == bEnd)
					ChangeList.push_back(curmemberinfo);

			}
			else
			{
				ChangeList.push_back(curmemberinfo);
			}
		}

		m_MemberList.clear();
		m_pListDlg->RemoveAll();

		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
		{
			GUILDMEMBERINFO info = *iter;
			ResetMemberInfo(&info);

		}
	}
	else
	{
		std::list<GUILDMEMBERINFO> ChangeList;
		std::list<GUILDMEMBERINFO>::iterator iter;

		std::vector<GUILDMEMBERINFO>* prevList = &m_MemberList;

		for(unsigned int i = 0; i < prevList->size(); ++i)
		{
			GUILDMEMBERINFO curmemberinfo = (*prevList)[i];

			if(ChangeList.size() > 0)
			{

				bool bEnd = TRUE;

				for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
				{
					WORD lvl = iter->Memberlvl;


					if(curmemberinfo.Memberlvl < lvl)
					{
						ChangeList.insert(iter,curmemberinfo);
						bEnd = FALSE;
						break;
					}
				}

				if(TRUE == bEnd)
					ChangeList.push_back(curmemberinfo);


			}
			else
			{
				ChangeList.push_back(curmemberinfo);
			}
		}

		m_MemberList.clear();
		m_pListDlg->RemoveAll();

		for(iter = ChangeList.begin(); iter != ChangeList.end(); ++iter)
		{
			GUILDMEMBERINFO info = *iter;
			ResetMemberInfo(&info);

		}
	}

	m_nLevelFlag ^= 1;

}

void CGuildDialog::SetListDlgMaxLine(WORD val)
{
	m_pListDlg->SetMaxLine(val);
}
void CGuildDialog::SetLoginNum(WORD val)
{
	char buf[64]={0,};
	sprintf(buf,"当前成员在线[%d]人",val);
	m_CurLoginNum->SetStaticText(buf);
}

