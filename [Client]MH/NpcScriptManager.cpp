#include "stdafx.h"
#include "NpcScriptManager.h"
#include "cPage.h"
#include "cDialogueList.h"
#include "cHyperTextList.h"
#include "GameIn.h"
#include "Npc.h"
#include "ObjectManager.h"
#include "PyogukDialog.h"
#include "MugongSuryunDialog.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "NpcScriptDialog.h"
#include "InventoryExDialog.h"
#include "MPRegistDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildMarkDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildCreateDialog.h"
#include "objectstatemanager.h"
#include "WantedManager.h"
#include "GuildManager.h"
#include "BailDialog.h"
#include "WantedDialog.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "AbilityManager.h"
#include "ItemShopDialog.h"
#include "MHMap.h"
#include "MoveManager.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTBattleListDialog.h"
#include "GuildUnion.h"
#include "GuildTraineeDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "SkillOptionChangeDlg.h"
#include "TitanPartsMakeDlg.h"
#include "TitanMixDlg.h"
#include "TitanUpgradeDlg.h"
#include "TitanInventoryDlg.h"
#include "TitanRepairDlg.h"
#include "TitanBreakDlg.h"
#include "TitanRegisterDlg.h"
#include "TitanDissolutionDlg.h"
#include "UniqueItemMixDlg.h"
#include "TitanManager.h"
#include "bobusangmanager.h"
#include "item.h"


GLOBALTON(cNpcScriptManager)
cNpcScriptManager::cNpcScriptManager()
{
	m_nRegistNpcCount = 0;
	m_pDialogue = NULL;
	m_pHyperText = NULL;

	SetSelectedNpc(NULL);
}

cNpcScriptManager::~cNpcScriptManager()
{
	DeleteAllScriptInfo();
	SAFE_DELETE( m_pDialogue );
	SAFE_DELETE( m_pHyperText );
}


void cNpcScriptManager::Init()
{
	LoadNpcScriptInfo();
	m_pDialogue = new cDialogueList;
	m_pDialogue->LoadDialogueListFile(NPC_DIALOGUE_PATH, "rb");
	m_pHyperText = new cHyperTextList;
	m_pHyperText->LoadHyperTextFormFile(NPC_HYPERTEXT_PATH, "rb");
}


void cNpcScriptManager::AddNpcScriptInfo(DWORD dwNpcId, cPage* pPage)
{
	m_NpcScriptList[dwNpcId].AddTail(pPage);
}

void cNpcScriptManager::DeleteAllScriptInfo()
{
	for(int i=0;i<MAX_NPC_COUNT;++i)
	{
		PTRLISTSEARCHSTART(m_NpcScriptList[i],cPage*,pPage)
			delete pPage;
		PTRLISTSEARCHEND
			m_NpcScriptList[i].RemoveAll();
	}
}

cPage* cNpcScriptManager::GetMainPage( DWORD dwNpcId )
{
	if( m_NpcScriptList[dwNpcId].IsEmpty() ) return NULL;
	return (cPage*)m_NpcScriptList[dwNpcId].GetHead();
}

cPage* cNpcScriptManager::GetPage( DWORD dwNpcId, DWORD dwPageId )
{
	PTRLISTSEARCHSTART(m_NpcScriptList[dwNpcId],cPage*,pPage)
		if( pPage->GetPageId() == dwPageId )
			return pPage;
	PTRLISTSEARCHEND
		return NULL;
}


void cNpcScriptManager::LoadNpcScriptInfo()
{
	CMHFile fp;
	if(!fp.Init(NPC_SCRIPT_PATH, "rb"))
		return;

	char buff[256]={0,};

	while(1)
	{
		fp.GetString(buff);
		if( fp.IsEOF() )
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}

		CMD_ST(buff)
			CMD_CS("$NPC")
			if((fp.GetString())[0] == '{')
			{
				LoadPageInfo(NULL, &fp);
				++m_nRegistNpcCount;
			}
			else
				__asm int 3;
		CMD_EN
	}
}

void cNpcScriptManager::LoadPageInfo(cPage* pPage, CMHFile* fp)
{
	char buff[256]={0,};

	static DWORD dwNpcId;
	DWORD dwPageId;
	int nDialogueCount = 0;
	int nHyperLinkCount = 0;
	DWORD dwDialogueId;
	HYPERLINK sHyper;

	while(1)
	{
		fp->GetString(buff);
		if(buff[0] == '}'|| fp->IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		CMD_ST(buff)
			CMD_CS("$PAGE")
			if((fp->GetString())[0] == '{')
			{
				cPage* pNewPage = new cPage;
				AddNpcScriptInfo( dwNpcId, pNewPage );
				LoadPageInfo(pNewPage, fp);
			}
			CMD_CS("#NPCID")
				dwNpcId = fp->GetDword();
			CMD_CS("#PAGEINFO")
				dwPageId = fp->GetDword();
			nDialogueCount = fp->GetInt();
			nHyperLinkCount = fp->GetInt();
			pPage->Init( dwPageId );
			CMD_CS("#DIALOGUE")
				for(int i =0; i<nDialogueCount;++i)
				{
					dwDialogueId = fp->GetDword();
					pPage->AddDialogue( dwDialogueId );
				}
				CMD_CS("#HYPERLINK")
					sHyper.wLinkId = fp->GetWord();
				sHyper.wLinkType = fp->GetWord()+1;
				sHyper.dwData = fp->GetDword();
				pPage->AddHyperLink( &sHyper );
				CMD_EN
	}
}

void cNpcScriptManager::StartNpcScript( CNpc* pNpc )
{
	if( m_pCurSelectedNpc )
	{
		GAMEIN->GetNpcScriptDialog()->EndDialog();
		SetSelectedNpc(NULL);
	}

	SetSelectedNpc(pNpc);
	GAMEIN->GetNpcScriptDialog()->OpenDialog( pNpc->GetNpcUniqueIdx() );
}

void cNpcScriptManager::StartNpcBusiness(DWORD dwData, DWORD dwParam)
{
	if( m_pCurSelectedNpc == NULL && dwParam == 0 ) 
	{
		ASSERT(0);

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		return;
	}

	if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == DEALER_ROLE)
	{
		switch( dwData )
		{
		case 0:		
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DEALER_SYN;
				msg.dwObjectID = HEROID;
				msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			break;
		case 1:
			{
				GAMEIN->GetUniqueItemMixDlg()->SetActive( TRUE );
			}
			break;
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == BOBUSANG_ROLE )
	{
		CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemLike( BOBUSANG_ITEM_TICKET );

		if( !pItem )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1639) );
			GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			return;
		}
		else
		{
			MSG_WORD msg;
			msg.Category = MP_BOBUSANG;
			msg.Protocol = MP_BOBUSANG_ADD_GUEST_SYN;
			msg.dwObjectID = HEROID;
			msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
			NETWORK->Send(&msg, sizeof(msg));
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CASTLE_ROLE )
	{
		switch( dwData )

		{
		case 0:	
			{
				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );
					return;
				}
				SWPROFIT->SendProfitInfoSyn();
			}
			break;
		case 1:
		case 2:
		case 3:	
		case 4:
		case 5:
		case 6:	
			{	
				if( HERO->GetGuildIdx() == 0 || HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );
					return;
				}
				DWORD dwMoney = 0;
				switch( dwData )
				{
				case 1:		dwMoney = 1000000;		break;
				case 2:		dwMoney = 2000000;		break;
				case 3:		dwMoney = 3000000;		break;
				case 4:		dwMoney = 1500000;		break;
				case 5:		dwMoney = 2500000;		break;				
				case 6:		dwMoney = 4000000;		break;
				}
				if( HERO->GetMoney() < dwMoney )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
					return;
				}
				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_UPGRADE_GATE_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwData;
				NETWORK->Send( &Msg, sizeof(Msg) );
				return;
			}
			break;
		}		
	}
	else if( (m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == CHANGGO_ROLE) || dwParam == eNpcParam_ShowpPyoguk )
	{
		if(dwData == 0) 
		{	
			if(GAMEIN->GetPyogukDialog()->IsItemInit())
			{
				if( !GAMEIN->GetInventoryDialog()->IsActive() )
					GAMEIN->GetInventoryDialog()->SetActive(TRUE);
				GAMEIN->GetPyogukDialog()->SetActive(TRUE);
				if(HERO->GetPyogukNum() == 0)
				{
//#ifdef _JAPAN_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )					
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
//					else
//					{
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
//						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
//					}
//#elif defined _HK_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )					
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
//					else
//					{
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
//						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
//					}
//#elif defined _TL_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )					
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
//					else
//					{
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare4);
//						GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare4);
//					}
//#else
					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
//#endif	// _JAPAN_LOCAL_
				}
				else
				{
					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1);
					GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1);
				}
			}
			else
			{
				if(HERO->GetPyogukNum() == 0)
				{
//#ifdef _JAPAN_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )
//					{
//						if( !GAMEIN->GetInventoryDialog()->IsActive() )
//							GAMEIN->GetInventoryDialog()->SetActive(TRUE);
//						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
//					}
//					else
//					{
//						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
//						{						
//							MSG_WORD2 msg;
//							msg.Category = MP_ITEM;
//							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
//							msg.dwObjectID = HEROID;						
//							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
//							msg.wData2 = eNpcParam_ShowpPyoguk;
//							NETWORK->Send(&msg, sizeof(msg));
//						}
//						else
//							return;
//					}
//#elif defined _HK_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )
//					{
//						if( !GAMEIN->GetInventoryDialog()->IsActive() )
//							GAMEIN->GetInventoryDialog()->SetActive(TRUE);
//						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
//					}
//					else
//					{
//						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
//						{						
//							MSG_WORD2 msg;
//							msg.Category = MP_ITEM;
//							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
//							msg.dwObjectID = HEROID;						
//							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
//							msg.wData2 = eNpcParam_ShowpPyoguk;
//							NETWORK->Send(&msg, sizeof(msg));
//						}
//						else
//							return;
//					}
//#elif defined _TL_LOCAL_
//					if( HERO->GetExtraPyogukSlot() == 0 )
//					{
//						if( !GAMEIN->GetInventoryDialog()->IsActive() )
//							GAMEIN->GetInventoryDialog()->SetActive(TRUE);
//
//						GAMEIN->GetPyogukDialog()->SetActive(TRUE);
//						GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);						
//					}
//					else
//					{
//						if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
//						{						
//							MSG_WORD2 msg;
//							msg.Category = MP_ITEM;
//							msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
//							msg.dwObjectID = HEROID;						
//							dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
//							msg.wData2 = eNpcParam_ShowpPyoguk;
//							NETWORK->Send(&msg, sizeof(msg));
//						}
//						else
//							return;
//					}
//#else
					if( !GAMEIN->GetInventoryDialog()->IsActive() )
						GAMEIN->GetInventoryDialog()->SetActive(TRUE);
					GAMEIN->GetPyogukDialog()->SetActive(TRUE);
					GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_NoWare);
//#endif	// _JAPAN_LOCAL_

				}
				else
				{
					if( dwParam == eNpcParam_ShowpPyoguk || MAP->IsVillage() == TRUE )
					{						
						MSG_WORD2 msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_PYOGUK_ITEM_INFO_SYN;
						msg.dwObjectID = HEROID;						
						dwParam==eNpcParam_ShowpPyoguk ? msg.wData1=79 : msg.wData1=m_pCurSelectedNpc->GetNpcUniqueIdx();
						msg.wData2 = eNpcParam_ShowpPyoguk;
						NETWORK->Send(&msg, sizeof(msg));
					}
					else
						return;
				}
			}

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		}	
		else if(dwData == 1)
		{
			GAMEIN->GetPyogukDialog()->SetPyogukListInfo();
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);			
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
		}
		else if(dwData == 2)
		{
			if(HERO->GetGuildIdx() == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
				return;
			}
			if(GUILDMGR->GetGuildLevel() < GUILD_3LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			if(GUILDMGR->CanUseWarehouse() == FALSE)
				return;
			GAMEIN->GetGuildWarehouseDlg()->SetActive(TRUE);	
		}
		else if(dwData == 3)
		{
			MSGBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
			msg.dwObjectID = HERO->GetID();
			NETWORK->Send(&msg, sizeof(msg));
		}
		else if(dwData == 4)
		{
			MSGBASE msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 5)
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_1;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 6)
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_2;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		else if(dwData == 7)
		{
			MSG_INT msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_SYN;
			msg.dwObjectID = HEROID;
			msg.nData = FORTWARWAREIDX_3;
			NETWORK->Send(&msg, sizeof(msg));

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MUNPA_ROLE)
	{
		switch( dwData )
		{
		case 0:
			{
				if(HERO->GetGuildIdx())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(28));
					return;
				}
				if((ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, HERO->GetAbilityGroup()) < 1) || (HERO->GetLevel() < GUILD_CREATELEVEL))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(41));
					return;
				}
				if(GUILDMGR->CanEntryGuild(HERO->GetGuildEntryDate()) == FALSE)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(59), HERO->GetGuildEntryDate());
					return;
				}
				cDialog* pDlg = (cDialog*)GAMEIN->GetGuildCreateDlg();
				pDlg->SetActive(TRUE);
			}
			break;
		case 1:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() == GUILD_MASTER)
				{
					WINDOWMGR->MsgBox( MBI_GUILD_BREAKUP, MBT_YESNO, CHATMGR->GetChatMsg( 302 ) );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
			}
			break;
		case 2:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
				else
				{
					GAMEIN->GetGuildLevelUpDlg()->SetActive(TRUE);
				}	
			}
			break;
		case 3:
			{
				if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
					return;
				}
				if(HERO->GetGuildIdx() && (HERO->GetGuildMemberRank() == GUILD_MASTER))
				{
					GAMEIN->GetGuildMarkDlg()->ShowGuildMark();
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(113));
					return;
				}	
			}
			break;
		case 4:	
			{
				if( GUILDUNION->CheckGuildUnionCondition( 0 ) )
					GAMEIN->GetGuildUnionCreateDlg()->SetActive( TRUE );
				else
					return;
			}
			break;
		case 5:	
			{
				if( GUILDUNION->CheckGuildUnionCondition( 1 ) )
					WINDOWMGR->MsgBox( MBI_GUILD_UNION_DESTROY, MBT_YESNO, CHATMGR->GetChatMsg( 1126 ) );
				else
					return;
			}
			break;
		case 6:	
			{
				if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
					GAMEIN->GetGuildMarkDlg()->ShowGuildUnionMark();
				else
					return;
			}
			break;
		case 7:	
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}
				GAMEIN->GetSWTimeRegDlg()->SetActive( TRUE );
			}
			break;
		case 8:	
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_PROPOSALLIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
			break;
		case 9:	
			{	
				WINDOWMGR->MsgBox( MBI_SW_OBSERVER_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1146) );
				return;
			}
			break;
		case 10:
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_GUILDLIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
			break;
		case 11:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}


				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 0;
				NETWORK->Send( &Msg, sizeof(Msg) );		

				return;
			}
			break;
		case 12:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}

				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 1;
				NETWORK->Send( &Msg, sizeof(Msg) );	

				return;
			}
			break;
		case 13:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;

				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				WINDOWMGR->MsgBox( MBI_SW_PROPOSAL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1058) );
				return;

			}
			break;
		case 14:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				else if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));

					return;
				}

				WINDOWMGR->MsgBox( MBI_SW_ATTACKREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1059) );		
				return;
			}
			break;
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
			{
				if(HERO->GetGuildIdx() == 0)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35));
					return;
				}
				if(HERO->GetGuildMemberRank() != GUILD_MASTER)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				if( GUILDMGR->CheckChangeLocation( dwData ) )
				{
					// magi82(37) �� �� ������
					WINDOWMGR->MsgBox( MBI_GUILD_CHANGE_LOCATION, MBT_YESNO, CHATMGR->GetChatMsg(1200), MAP->GetMapName( GUILDMGR->GetTempLocation() ) );
					//					return;
				}
				else
					return;
			}
			break;
		case 20:

			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_TIMEINFO_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );				
			}
			break;
		case 21:			// ���ϻ� -> ���ļҰ��� ���
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				if( GUILDMGR->GetGuild()->GetLevel() < MAX_GUILD_LEVEL )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
					return;
				}

				MSG_DWORD msg;
				ZeroMemory( &msg, sizeof(MSG_DWORD) );
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_REGIST_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
				NETWORK->Send( &msg, sizeof(msg) );

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO,eObjectState_Deal );
			}
			break;
		case 22:			// ���ϻ� -> ���ļҰ��� ����
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				//GAMEIN->GetGuildCreateDlg()->SetGuildName();
				//GAMEIN->GetGuildCreateDlg()->GetGuildIntro();
				GAMEIN->GetGuildCreateDlg()->SetActive( TRUE );
			}
			break;
		case 23:		// ���ϻ� -> ���ļҰ�â
			{
				// magi82 - ���ϻ� ����(�������� �޼��� ���������� ���̾�α�â ��Ƽ�� 070119
				GAMEIN->GetGuildTraineeDialog()->SendPage(1, TRUE);
			}
			break;
		case 24:		// ���ϻ� -> ���ļҰ��� ���
			{
				if( HERO->GetGuildMemberRank() != GUILD_MASTER )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726));
					return;
				}

				WINDOWMGR->MsgBox( MBI_MUNPADELETESEL, MBT_YESNO, CHATMGR->GetChatMsg(1493) );	// magi82 - ���ϻ�����(070124)

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO,eObjectState_Deal );
			}
			break;
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == WANTED_ROLE)
	{
		//���� Ŭ���Ѱ� �ʰԿ� ���� �ֳ�
		if(GAMEIN->GetWantedDialog()->IsActive() == TRUE)
			return;
		if(dwData == 0) //â�� ����
			WANTEDMGR->SortWantedListSyn(1);
		else if(dwData == 1)
			GAMEIN->GetBailDialog()->Open();
		//
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SURYUN_ROLE)
	{
		if( dwData == eSuryunNpc_Suryun )		// �����ϱ�
		{
			// magi82 - Titan(070612)
			if( TITANMGR->GetCurRidingTitan() == NULL )
			{
				GAMEIN->GetMPRegistDialog()->SetActive(TRUE);
				if(GAMEIN->GetMugongSuryunDialog()->IsActive() == FALSE)
					GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		else if( dwData == eSuryunNpc_About_GuildTournament )		// ������ʸ�Ʈ �˾ƺ���
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		}
		else if( dwData == eSuryunNpc_Regist_GuildTournament )		// ������ʸ�Ʈ ������û�ϱ�
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;

			msg.Protocol = MP_GTOURNAMENT_REGISTCOUNT;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else if( dwData == eSuryunNpc_Cancel_GuildTournament )		// ������ʸ�Ʈ ��û ����ϱ�
		{
			if( GAMEIN->GetGTRegistcancelDlg() )
				GAMEIN->GetGTRegistcancelDlg()->SetActive( TRUE );
		}
		else if( dwData == eSuryunNpc_Show_GuildStand )				// ������ʸ�Ʈ ����ǥ ����
		{
			MSG_WORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
			msg.wData = 1;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		else if( dwData == eSuryunNpc_Enter_GuildTournament )		// ������ʸ�Ʈ �����ϱ�
		{

			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = 0;
			NETWORK->Send( &msg, sizeof(msg) );
		}

		else if( dwData == eSuryunNpc_EnterObserver_GuildTournament )	// �������� ������ʸ�Ʈ �����ϱ�
		{
			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );

			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEINFO_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );

			//if( GAMEIN->GetGTBattleListDlg() )
			//	GAMEIN->GetGTBattleListDlg()->SetActive( TRUE );
		}
		/////////////////////////////////////////////////////////////////////
		// 06. 06. 2�� ���� - �̿���
		// ���� ��ȯ

		else if( dwData == eSuryunNpc_SkillOption )
		{
			GAMEIN->GetSkillOptionChangeDlg()->SetActive(TRUE);

			if(GAMEIN->GetMugongSuryunDialog()->IsActive() == FALSE)

				GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
			if(GAMEIN->GetInventoryDialog()->IsActive() == FALSE)
				GAMEIN->GetInventoryDialog()->SetActive(TRUE);
		}
		/////////////////////////////////////////////////////////////////////
	}
	else if(m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == MAPCHANGE_ROLE )
	{
		if(dwData == 0)
		{
			MSG_DWORD msg;
			msg.Category = MP_NPC;

			msg.Protocol = MP_NPC_DOJOB_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = m_pCurSelectedNpc->GetID();
			NETWORK->Send(&msg, sizeof(msg));
		}
		else
		{
			if(GAMEIN->GetNpcScriptDialog()->IsActive())
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);						
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == SYMBOL_ROLE )
	{
		// ���ν���		
		if( HERO->GetGuildMemberRank() == GUILD_MASTER )
		{
			CGuild* pGuild = GUILDMGR->GetGuild();
			if( pGuild && pGuild->GetLevel() == 5 )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );
		}
		else
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );
	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == TALKER_ROLE )
	{

	}
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == GUIDE_ROLE )
	{
		//magi82 - ���ϻ� �������ý���		
		switch( dwData )
		{
		case 0:	// �ʺ��� �������� ���ϻ� ����â
			{
				// magi82 - ���ϻ� ����(�������� �޼��� ���������� ���̾�α�â ��Ƽ�� 070119
				GAMEIN->GetGuildTraineeDialog()->SendPage(1, TRUE);
			}
			break;
		default:
			break;
		}
	}
	// magi82 - Titan(070323)
	else if( m_pCurSelectedNpc && m_pCurSelectedNpc->GetNpcJob() == TITAN_ROLE )
	{
		switch( dwData )
		{
		case eTITAN_NPC_BuyItem:	// Ÿ��ź ������ ����
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DEALER_SYN;
				msg.dwObjectID = HEROID;
				msg.wData = m_pCurSelectedNpc->GetNpcUniqueIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			break;
		case eTITAN_NPC_TitanMix:	// Ÿ��ź ����
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanMixDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanUpgrade:	// Ÿ��ź ��޾�
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanUpgradeDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanPartsMake:	// Ÿ��ź ���� ����(��ǰ����)
			{
				GAMEIN->GetTitanPartsMakeDlg()->SetActive( TRUE );
			}
			break;
		case eTITAN_NPC_TitanPartsChange:	// Ÿ��ź ������ ü����
			{
				if(TITANMGR->GetRegistedTitanItemDBIdx())
				{
					if( TITANMGR->GetCurRidingTitan() == NULL )
					{						
						GAMEIN->GetTitanRepairDlg()->SetActive( TRUE );
						GAMEIN->GetInventoryDialog()->SetActive( TRUE );
						GAMEIN->GetTitanInventoryDlg()->SetActive( TRUE );
					}
					else
					{

						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					}
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1510) );

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanBreak:	// Ÿ��ź ������ ����
			{
				GAMEIN->GetTitanBreakDlg()->SetActive( TRUE );
			}
			break;
		case eTITAN_NPC_TitanRegister:	// Ÿ��ź ���
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanRegisterDlg()->SetActive( TRUE );
				}
				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
		case eTITAN_NPC_TitanDissolution:	// Ÿ��ź ����
			{
				if( TITANMGR->GetCurRidingTitan() == NULL )
				{
					GAMEIN->GetTitanDissolutionDlg()->SetActive( TRUE );
				}

				else
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1557));

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				}
			}
			break;
			// magi82 - Titan(070611) Ÿ��ź ������ȯ �ּ�ó��
			/*
		case eTITAN_NPC_TitanMugongMix:	// Ÿ��ź ���� ����
			{
				GAMEIN->GetTitanMugongMixDlg()->SetActive( TRUE );
			}
			break;
			*/
		default:
			break;
		}
	}
	else
	{
		ASSERT(0);
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	GAMEIN->GetNpcScriptDialog()->DisActiveEx();
	//	GAMEIN->GetNpcScriptDialog()->EndDialog();
}




