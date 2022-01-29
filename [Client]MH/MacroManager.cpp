// MacroManager.cpp: implementation of the CMacroManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#include "CommonGameFunc.h"
#endif	//_CHEATENABLE_
#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "EventMapInfo.h"
#endif
#include <direct.h>
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "../GlobalEventFunc.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "MurimNet.h"
#include "QuickManager.h"
#include "MoveManager.h"
#include "PeaceWarModeManager.h"
#include "UngijosikManager.h"
#include "AbilityManager.h"
#include "ObjectManager.h"
#include "WantedManager.h"
#include "MainGame.h"
#include "MHCamera.h"
#include "ChatManager.h"
#include "cChatTipManager.h"
#include "./input/cIMEWnd.h"
#include "./input/UserInput.h"
#include "HelpDicManager.h"
#include "VimuManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "MouseCursor.h"
#include "HelpDialog.h"
#include "QuickDialog.h"
#include "MainBarDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "OptionDialog.h"
#include "ReviveDialog.h"
#include "DissolutionDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildManager.h"
#include "GuildMarkManager.h"
#include "GuildTraineeDialog.h"
#include "TitanPartsMakeDlg.h"		
#include "TitanMixDlg.h"			
#include "TitanRepairDlg.h"			
#include "TitanUpgradeDlg.h"		
#include "TitanBreakDlg.h"			
#include "TitanInventoryDlg.h"		
#include "TitanRegisterDlg.h"		
#include "TitanDissolutionDlg.h"
#include "BattleSystem_Client.h"
#include "PKManager.h"
#include "SkillManager_Client.h"
#include "QuestTotaldialog.h"
#include "QuestManager.h"
#include "MiniMapDlg.h"
#include "BigmapDlg.h"
#include "BailDialog.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "PartyIconManager.h"
#include "WantNpcDialog.h"
#include "FreeImage/FreeImage.h"
#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"
#include "NoteDialog.h"
#include "SkillPointRedist.h"
#include "ShoutchatDialog.h"
#include "AppearanceManager.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "NameChangeNotifyDlg.h"
#include "EventNotifyDialog.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "ReinforceResetDlg.h"
#include "SealDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "MugongSuryunDialog.h"
#include "MussangManager.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "KeySettingTipDlg.h"
#include "UniqueItemCurseCancellationDlg.h"	
#include "UniqueItemMixDlg.h"
#include "SOSDialog.h"
#include "SkillPointResetDlg.h"
#include "ScreenShotDlg.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
#include "AIManager.h"
#include "MHMap.h"
#include "BailDialog.h"
#include "cSafeLockDialog.h"
#include "FBTipDlg.h"
#ifdef _GMTOOL_
#include "PartyManager.h"
#include "GuildUnion.h"
#endif 
#include "GoldShopMainDailog.h"
#include <fstream>
#include "FortWarManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL g_bActiveApp;
extern HINSTANCE g_hInst;

extern BOOL g_bDisplayFPS;
extern int g_nServerSetNum;

extern HWND _g_hWnd;  

GLOBALTON(CMacroManager)

CMacroManager::CMacroManager()
{
	ZeroMemory( m_MacroKey, sizeof( m_MacroKey ) );
	ZeroMemory( m_DefaultKey, sizeof( m_DefaultKey ) );

	ZeroMemory( m_MacroPressKey, sizeof( m_MacroPressKey ) );
	ZeroMemory( m_DefaultPressKey, sizeof( m_DefaultPressKey ) );

	m_nMacroMode	= MM_CHAT;
	m_bChatMode		= TRUE;
	m_bPause		= FALSE;
	m_bInVisibleCursor = FALSE;
#ifdef _GMTOOL_
	m_idx = 0;
#endif
}

CMacroManager::~CMacroManager()
{
}


void CMacroManager::KeyboardInput( CKeyboard* keyInfo )
{
	if (keyInfo->GetKeyDown(KEY_W) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{//[Òþ²Ø´°¿ÚÈÈ¼ü][BY:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019-10-6][12:52:52]
		SendMessage(_g_hWnd,WM_HOTKEY,0x7777,0);
		return;
	}
	if( !g_bActiveApp || m_bPause )	return;
	if(GAMEIN->GetChatDialog())
	{
		if(GAMEIN->GetChatDialog()->IsChatFocus())
			return;
	}
		
	if(HEROID == 0)
		return;

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;	

	if (keyInfo->GetKeyPressed(88) == 1) 
	{
        GAMEIN->GetScreenShotDlg()->SetActive( TRUE, gHeroID ); 
	}

	BOOL bDock = FALSE;
	if( CIMEWND->IsDocking() )
	{
		if( m_nMacroMode == MM_MACRO )
			bDock = TRUE;
	}
	
	int nSysKey = MSK_NONE;

	if( keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		nSysKey = 0;	
		nSysKey |= MSK_ALT;
	}
	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )
	{
		nSysKey &= ~MSK_NONE;
		nSysKey |= MSK_CTRL;
	}
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )
	{
		nSysKey &= ~MSK_NONE;
		nSysKey |= MSK_SHIFT;
	}
#ifdef _GMTOOL_
	if(keyInfo->GetKeyDown(KEY_F5))
	{
		if (sCPlayerAI->SetOpen(!sCPlayerAI->sAiConfig.AutoSwitch[eOpen]))
		{
			HERO->GetPosition(&sCPlayerAI->Heropos);
			sCPlayerAI->sAiConfig.SetValue[ePointX] = sCPlayerAI->Heropos.x / 100;
			sCPlayerAI->sAiConfig.SetValue[ePointY] = sCPlayerAI->Heropos.z / 100;
			sCPlayerAI->sAiConfig.SetValue[eLastMap] = MAP->GetMapNum();
			sCPlayerAI->SaveConfig();
		}
	}
	if(keyInfo->GetKeyDown(KEY_HOME))
	{
		sCPlayerAI->SetUseHunXue();
	}
#else
	if(keyInfo->GetKeyDown(KEY_F5)||keyInfo->GetKeyDown(KEY_HOME))
	{
		//if(GAMEIN->GetFBTipDlg())
		//{//[¸±±¾½ûÓÃÄÚ¹Ò][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/5/2 17:08]
		//	if (GAMEIN->GetFBTipDlg()->IsActive())
		//	{
		//		CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2174));
		//		return;
		//	}
		//}
		//if(IsBattleChannel[gChannelNum]&&FORTWARMGR->IsWarStart())
		//{//[ÒªÈûÕ½½ûÖ¹Ê¹ÓÃÄÚ¹Ò][by:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2019/1/13]
		//	CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2324));
		//	return;
		//}
		//[ÄÚ¹Ò¿ì½Ý¼ü][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/12/4]
		//if(MAP->GetMapNum()==5) return;//[¹¥³ÇÕ½²»ÄÜÊ¹ÓÃÄÚ¹Ò][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/5/2 17:11]

		if (sCPlayerAI->SetOpen(!sCPlayerAI->sAiConfig.AutoSwitch[eOpen]))
		{
			HERO->GetPosition(&sCPlayerAI->Heropos);
			sCPlayerAI->sAiConfig.SetValue[ePointX] = sCPlayerAI->Heropos.x / 100;
			sCPlayerAI->sAiConfig.SetValue[ePointY] = sCPlayerAI->Heropos.z / 100;
			sCPlayerAI->sAiConfig.SetValue[eLastMap] = MAP->GetMapNum();
			sCPlayerAI->SaveConfig();
		}
		
		
	}
#endif
	if(keyInfo->GetKeyDown(KEY_F6))
	{//[ÓÒ¼üÉ¾³ýbuff¿ª¹Ø][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/22]
		WINDOWMGR->MsgBox( MSBOX_DELBUFTIP, MBT_YESNO, CHATMGR->GetChatMsg(2197));
	}
	if(keyInfo->GetKeyDown(KEY_E))
	{
		if(GAMEIN->GetChatDialog()->IsChatFocus())
			return;
		GAMEIN->GetBailDialog()->SetFame(HERO->GetBadFame());
	}

	if(keyInfo->GetKeyDown(KEY_F7))
	{
#ifdef _GMTOOL_
		CItem* Item = NULL;
		for(int i=390;i<=409;i++)
		{
			Item = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(Item)
			{
				MSG_ITEM_DISCARD_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DISCARD_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos =Item->GetPosition();
				msg.wItemIdx = Item->GetItemIdx();
				msg.ItemNum = Item->GetDurability();
				msg.dwItemDBIdx = Item->GetDBIdx();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}

#else
		if(!HERO->IsDied()) return;
		if(!GAMEIN->GetReviveDialog()->IsActive())
			GAMEIN->GetReviveDialog()->SetActive(TRUE);
#endif  //_GMTOOL_
	}
#ifdef _GMTOOL_
	if(keyInfo->GetKeyDown(KEY_TAB))
	{
		VECTOR3 Heropos;
		HERO->GetPosition(&Heropos);
		CObject * pObj = OBJECTMGR->GetTargetInNear(&Heropos, Heropos.z * 100);
		if (pObj)
		{
			CBattle* pBattle = BATTLESYSTEM->GetBattle(HERO);
			if (pBattle->IsEnemy(HERO, pObj))
			{
				//if (!OBJECTMGR->GetSelectedObject())
				OBJECTMGR->SetSelectedObject(pObj);
				//QUICKMGR->UseQuickItem(0, 0);
			}
		}
	}
	if(keyInfo->GetKeyDown(KEY_F8))
	{
		CItem* Item = NULL;
		for(int i=0;i<=79;i++)
		{
			Item = GAMEIN->GetInventoryDialog()->GetItemForPos(i);
			if(Item)
			{
				MSG_ITEM_DISCARD_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DISCARD_SYN;
				msg.dwObjectID = HEROID;
				msg.TargetPos =Item->GetPosition();
				msg.wItemIdx = Item->GetItemIdx();
				msg.ItemNum = Item->GetDurability();
				msg.dwItemDBIdx = Item->GetDBIdx();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
	}
#endif //_GMTOOL_
	for( int i = 0 ; i < ME_COUNT ; ++i )
	{
		if( bDock && !m_MacroKey[m_nMacroMode][i].bAllMode )
			continue;		

		if( m_MacroKey[m_nMacroMode][i].nSysKey & nSysKey )
		{
			if( m_MacroKey[m_nMacroMode][i].bUp )
			{
				if( keyInfo->GetKeyUp( m_MacroKey[m_nMacroMode][i].wKey ) )
					PlayMacro( i );
			}
			else
			{
				if( keyInfo->GetKeyDown( m_MacroKey[m_nMacroMode][i].wKey ) )
					PlayMacro( i );
			}
		}
	}

	//	// 2014-02-16 ÏÂÒþ²Ø Alt+ Q

 //   if(keyInfo->GetKeyDown(KEY_Q) && keyInfo->GetKeyPressed(KEY_MENU))
	//{
	//	if(IsWindowVisible(_g_hWnd)!=0)
	//    {
	//	   ShowWindow(_g_hWnd,SW_HIDE);
	//	   ::InstallHookEv(g_hInst);
	//    }
	//}


#ifdef _GMTOOL_
	if( ( keyInfo->GetKeyDown(KEY_RETURN) || keyInfo->GetKeyDown(KEY_PADENTER) ) && keyInfo->GetKeyPressed(KEY_MENU) )
	{
		if( GMTOOLMGR->IsShowing() )
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );
		else
			GMTOOLMGR->ShowGMDialog( TRUE, TRUE );
	}
	else if( keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU) )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_AGENTCHECK_SYN;
		msg.dwObjectID	= HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	
#endif


#ifdef _CHEATENABLE_

#ifndef _GMTOOL_
	if( !CHEATMGR->IsCheatEnable() ) return;
#endif

	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		CPetUpgradeDialog* pdlg = GAMEIN->GetPetUpgradeDialog();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}

	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		CPetRevivalDialog* pdlg = GAMEIN->GetPetRevivalDialog();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}

	if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		if(GAMEIN->GetSOSDlg()->IsActive() == TRUE)
		{
			GAMEIN->GetSOSDlg()->SetActive(FALSE);
		}
		else
		{
			GAMEIN->GetSOSDlg()->SetActive(TRUE);
		}
	}
	
	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		g_bDisplayFPS = !g_bDisplayFPS;
	}
	if(keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		WINDOWMGR->ToggleShowInterface();
	}

	if(keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		m_bInVisibleCursor ^= TRUE;
		CURSOR->SetCheatMode( m_bInVisibleCursor );
	}

	if( keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( CAMERA->GetCameraMode() == eCM_EyeView )
		{
			CAMERA->SetCameraMode( eCM_Free );
		}
		else
		{
			CAMERA->SetCameraMode( eCM_EyeView );
		}
	}

	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CSWTimeDialog* pdlg = GAMEIN->GetSWTimeDlg();
		if( pdlg )
		{
			pdlg->SetActive( !pdlg->IsActive() );
			pdlg->SetMunpaName( "±×·¹ÀÌÆ®Çü´ÔÀÌ¼Å" );
		}
	}
	if(keyInfo->GetKeyDown(KEY_S) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTRegistcancelDialog* pdlg = GAMEIN->GetGTRegistcancelDlg();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}
	if(keyInfo->GetKeyDown(KEY_L) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTStandingDialog* pdlg = GAMEIN->GetGTStandingDlg();
		if( pdlg )
		{
			REGISTEDGUILDINFO gi;
			gi.Ranking = eGTStanding_8TH;
			gi.Position = 0;
			gi.GuildIdx = 2;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶ÀÏ" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 1;
			gi.GuildIdx = 4;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶ÀÌ" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_16TH;
			gi.Position = 2;
			gi.GuildIdx = 6;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶»ï" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 3;
			gi.GuildIdx = 8;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶»ç" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 4;			
			gi.GuildIdx = 10;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶¿À" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_4TH;
			gi.Position = 5;			
			gi.GuildIdx = 12;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶À°" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_16TH;
			gi.Position = 6;			
			gi.GuildIdx = 14;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶Ä¥" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 7;			
			gi.GuildIdx = 16;
			strcpy( gi.GuildName, "¶ó¸¶»ç¸¶ÆÈ" );
			pdlg->AddGuildInfo( &gi );
			pdlg->SetActive( !pdlg->IsActive() );
			pdlg->RefreshGuildInfo();
		}
	}
	if(keyInfo->GetKeyDown(KEY_J) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		GTBATTLE_INFO bt1, bt2;
		bt1.Group = 0;
		sprintf( bt1.GuildName1, "¶ó¸¶´Ô¸¸¼¼" );
		sprintf( bt1.GuildName2, "¶ó¸¶´ÔÂ¯" );
		bt2.Group = 1;
		sprintf( bt2.GuildName1, "Å©ÇìÇì" );
		sprintf( bt2.GuildName2, "¾Æ¿À" );

		CGTBattleListDialog* pdlg = GAMEIN->GetGTBattleListDlg();
		if( pdlg )
		{
			pdlg->SetPlayOff( TRUE );
			pdlg->AddBattleInfo( &bt1 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->RefreshBattleList();
			pdlg->SetActive( !pdlg->IsActive() );
		}
	}
	if(keyInfo->GetKeyDown(KEY_H) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTScoreInfoDialog* pdlg = GAMEIN->GetGTScoreInfoDlg();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}
	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( GAMEIN->GetTitanRegisterDlg()->IsActive() == FALSE )
		{
			GAMEIN->GetTitanRegisterDlg()->SetActive( TRUE );
		}
		else
			GAMEIN->GetTitanRegisterDlg()->SetActive( FALSE );
	}	
	if(keyInfo->GetKeyDown(KEY_K) && keyInfo->GetKeyPressed(KEY_MENU))
	{	
		/*  ÐÞ¸´Alt + Q »Ö¸´¼¼µãbug ´Ë¹¦½ûÖ¹Ê¹ÓÃ! 2014-08-14

		if(GAMEIN->GetSkillPointResetDlg()->IsActive() == FALSE)
		{

			GAMEIN->GetSkillPointResetDlg()->SetActive(TRUE);
		}
		else
		{
			GAMEIN->GetSkillPointResetDlg()->SetActive(FALSE);
		}	

		*/
	}	
	if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( GAMEIN->GetUniqueItemMixDlg()->IsActive() == FALSE )
		{

            GAMEIN->GetUniqueItemMixDlg()->SetActive(TRUE);
		}
		else
		{
			GAMEIN->GetUniqueItemMixDlg()->SetActive(FALSE);
		}
	}

	

#ifdef _GMTOOL_
	if( !CHEATMGR->IsCheatEnable() ) return;
#endif

	if( bDock == TRUE) return;

	if(keyInfo->GetKeyDown(KEY_I) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
		msg.dwObjectID = HERO->GetID();
		NETWORK->Send(&msg, sizeof(msg));
	}
	if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		if( GAMEIN->GetReinforceResetDlg() )
		{
			GAMEIN->GetReinforceResetDlg()->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );
		}
	}
	if(keyInfo->GetKeyDown(KEY_Y) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = 2;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	if(keyInfo->GetKeyDown(KEY_Q) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		MSGBASE msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_REGIST_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = 1;
		NETWORK->Send( &msg, sizeof(msg) );
	}
#endif	//_CHEATENABLE_

}

void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )	return;

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;
	if( CIMEWND->IsDocking() )
	{
			return;
	}

	int nSysKey = MSK_NONE;

	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )		nSysKey |= MSK_CTRL;
	if( keyInfo->GetKeyPressed( KEY_MENU ) )		nSysKey |= MSK_ALT;
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )		nSysKey |= MSK_SHIFT;



	for( int i = 0 ; i < MPE_COUNT ; ++i )
	{
		if( m_MacroPressKey[m_nMacroMode][i].nSysKey & nSysKey
			&& keyInfo->GetKeyPressed( m_MacroPressKey[m_nMacroMode][i].wKey ) )
		{
			PlayMacroPress( i );
		}
	}		
}

void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}


void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );
}


void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}

void CMacroManager::SetMacroMode( int nMacroMode )
{
	m_nMacroMode = nMacroMode;

	if( m_nMacroMode == MM_CHAT )
		m_bChatMode = TRUE;
	else
		m_bChatMode = FALSE;
}

void CMacroManager::LoadUserMacro()
{
	SetDefaultMacro( MM_MACRO );
	SetDefaultMacro( MM_CHAT );
	SetMacroMode( MM_MACRO );
}


BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
{
	HANDLE	hFile;

	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL, 
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwRead;

	if( !ReadFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	if( !ReadFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	
	CloseHandle( hFile );
	return TRUE;
}

void CMacroManager::SaveUserMacro()
{
	SaveMacro( "./INI/MacroUserSet.mcr" );
}

BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
{
	HANDLE	hFile;

	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL, 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	
	DWORD dwWritten;
	
	if( !WriteFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	if( !WriteFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	CloseHandle( hFile );
	return TRUE;	
}

void CMacroManager::PlayMacro( int nMacroEvent )
{
	switch( nMacroEvent )
	{

		case ME_USE_QUICKITEM01:
			QUICKMGR->UseQuickItem(0);
			break;

		case ME_USE_QUICKITEM02:
			QUICKMGR->UseQuickItem(1);
			break;

		case ME_USE_QUICKITEM03:
			QUICKMGR->UseQuickItem(2);
			break;

		case ME_USE_QUICKITEM04:
			QUICKMGR->UseQuickItem(3);
			break;

		case ME_USE_QUICKITEM05:
			QUICKMGR->UseQuickItem(4);
			break;

		case ME_USE_QUICKITEM06:
			QUICKMGR->UseQuickItem(5);
			break;

		case ME_USE_QUICKITEM07:
			QUICKMGR->UseQuickItem(6);
			break;

		case ME_USE_QUICKITEM08:
			QUICKMGR->UseQuickItem(7);
			break;

		case ME_USE_QUICKITEM09:
			QUICKMGR->UseQuickItem(8);
			break;

		case ME_USE_QUICKITEM10:
			QUICKMGR->UseQuickItem(9);
			break;

		case ME_SELECT_QUICKSLOT1:
			GAMEIN->GetQuickDialog()->SelectPageDirect(0);
			break;

		case ME_SELECT_QUICKSLOT2:
			GAMEIN->GetQuickDialog()->SelectPageDirect(1);
			break;

		case ME_SELECT_QUICKSLOT3:
			GAMEIN->GetQuickDialog()->SelectPageDirect(2);
			break;

		case ME_SELECT_QUICKSLOT4:
			GAMEIN->GetQuickDialog()->SelectPageDirect(3);
			break;

		case ME_TOGGLE_INVENTORYDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_INVENTORYDLGICON);
			break;

		case ME_TOGGLE_CHARACTERDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_CHARACTERDLGICON);
			break;

		case ME_TOGGLE_MUGONGDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MUGONGDLGICON);
			break;

		case ME_TOGGLE_MUNPADLG:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_GUILDDLGICON);
			break;

		case ME_TOGGLE_QUESTDLG:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_QUESTDLGICON);			
			break;
		case ME_TOGGLE_EXITDLG:

			if( !WINDOWMGR->CloseAllAutoCloseWindows() )
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_EXITDLGICON);
			}			

			break;

		case ME_TOGGLE_OPTIONDLG:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_OPTIONDLGICON );
			}
			break;

		case ME_TOGGLE_FRIENDLIST:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_FRIENDDLGICON );
			}
			break;

		case ME_SEND_MEMO:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_NOTEDLGICON );
			}
			break;
			
		case ME_TOGGLE_MOVEMODE:

			// ·âËøR¼üÁ¬¼¼ 2014-08-30

			//MOVEMGR->ToggleHeroMoveMode();

			break;

		case ME_TOGGLE_PEACEWARMODE:
			PEACEWARMGR->ToggleHeroPeace_WarMode();
			break;

		case ME_TOGGLE_AUTOATTACK:
		case ME_TOGGLE_AUTOATTACK2:
			g_UserInput.GetHeroMove()->AttackSelectedObject();
			break;

		case ME_TOGGLE_HELP:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_HELPDLGICON);
			break;
		case ME_TOGGLE_GOLDSHOP:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_GOLDSHOPDLGICON);
			break;
		case ME_TOGGLE_MINIMAP:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MINIMAPDLGICON);
			break;

		case ME_TOGGLE_CAMERAVIEWMODE:
			CAMERA->ToggleCameraViewMode();
			break;

		case ME_SCREENCAPTURE:
			{
				DIRECTORYMGR->SetLoadMode(eLM_Root);
				CreateDirectory( "ScreenShot", NULL );


				
				WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;
				WORD wHH = 0, wMM = 0, wSS = 0;	
				char fname[256];
				char fname2[256];

				SYSTEMTIME ti;

				GetLocalTime( &ti );

				HANDLE hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_READ, 0, NULL, 
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

				DWORD dwSize = 0;

				if( hFile != INVALID_HANDLE_VALUE )
				{
					ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
					ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
					ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
					ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
					ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
					ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
					ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
					ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );
					
					CloseHandle( hFile );

					if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD 
						&& ti.wHour == wHH && ti.wMinute == wMM && ti.wSecond == wSS) )
					{
						wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
						wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
					}					
				}
				else

				{
					wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
					wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
				}

//#ifndef _HK_LOCAL_
				sprintf(fname,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
				sprintf(fname2,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.txt", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
//#else
//				sprintf(fname,"ScreenShot/Mosiang_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
//#endif
				BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);

				if( rt )
				{
					hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_WRITE, 0, NULL, 
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

					if( hFile != INVALID_HANDLE_VALUE )
					{
						++wN;
						WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
						WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
						WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
						WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
						WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
						WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
						WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
						WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							

						CloseHandle( hFile );						
					}

					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(201), fname );
				}

				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(200) );
				}

				FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);
				
				if(pBitmap)
				{
					DeleteFile(fname);
					int len = strlen(fname);
					fname[len-3] = 'j';
					fname[len-2] = 'p';
					fname[len-1] = 'g';
					FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);
					FreeImage_Unload(pBitmap);
				}
#ifdef _GMTOOL_
				CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
				if( pTargetObj )
				if( pTargetObj->GetObjectKind() == eObjectKind_Player )
				{
                    FILE* fp = fopen(fname2,"a+");
                    if(fp)
					{
                        fprintf(fp,"%s\n",pTargetObj->GetObjectName());
                        fclose(fp);
					}
				}
#endif
			}
			break;
		case ME_SHOWALLNAME_ON:
			{
				OBJECTMGR->SetAllNameShow( TRUE );
			}
			break;
		case ME_SHOWALLNAME_OFF:
			{

				OBJECTMGR->SetAllNameShow( FALSE );
			}
			break;

		case ME_CHANGE_WHISPERLIST_UP:
			{
				if( GAMEIN->GetChatDialog() )
				if( GAMEIN->GetChatDialog()->GetChatEditBox() )
				{
					if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
					{
						CHATMGR->ChangeWhisperUser( TRUE );
					}
				}
			}
			break;
		case ME_CHANGE_WHISPERLIST_DOWN:
			{
				if( GAMEIN->GetChatDialog() )
				if( GAMEIN->GetChatDialog()->GetChatEditBox() )
				{
					if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
					{
						CHATMGR->ChangeWhisperUser( FALSE );
					}
				}
			}
			break;

		case ME_TOGGLE_BIGMAP:
			{
#ifdef _GMTOOL_
				if( CAMERA->GetCameraMode() != eCM_EyeView )
#endif
				if( GAMEIN->GetBigMapDialog() )
					GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
			}
			break;
		case ME_MUSSANG_ON:
			{
				if(!HERO->IsDied() && !HERO->InTitan()) 
					MUSSANGMGR->SendMsgMussangOn();
			}
			break;
		case ME_TOGGLE_KEY_SETTING_TIP_HIDE:
			{
				if( CAMERA->GetCameraMode() != eCM_EyeView )
					GAMEIN->GetKeySettingTipDlg()->SetActive(FALSE);
			}
			break;
		case ME_TOGGLE_KEY_SETTING_TIP_SHOW:
			{
				if( CAMERA->GetCameraMode() != eCM_EyeView )
				{
					GAMEIN->GetKeySettingTipDlg()->SetMode( m_nMacroMode );
					GAMEIN->GetKeySettingTipDlg()->SetActive(TRUE);
				}
			}
			break;
		case ME_TOGGLE_TITANINVENTORYDLG:	
			{
				if(HERO->InTitan() == TRUE)
				{
					CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();
					if(pTitanInven->IsActive() == TRUE)
					{
						pTitanInven->SetActive(FALSE);
					}
					else
					{
						pTitanInven->SetActive(TRUE);
					}					
				}
			}			
			break;
	}
}

void CMacroManager::PlayMacroPress( int nMacroEvent )
{
	float Rate = 1.f;

	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )
		Rate = 0.3f;

	switch( nMacroEvent )
	{
		case MPE_CAMERAMOVE_ZOOMIN:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( 0.8 * gTickTime*Rate*5 );
			else
#endif
				CAMERA->ZoomDistance( 0, -0.8 * gTickTime*Rate, 0 );

			break;

		case MPE_CAMERAMOVE_ZOOMOUT:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( -0.8 * gTickTime*Rate*5 );
			else
#endif
				CAMERA->ZoomDistance( 0, 0.8 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_UP:
			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_DOWN:
			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_RIGHT:
			CAMERA->MouseRotate( -0.3 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_LEFT:
			CAMERA->MouseRotate( 0.3 * gTickTime*Rate, 0 );
			break;

	}
}

void CMacroManager::Init()
{

	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
		{
			{ MSK_NONE,		KEY_ESCAPE,		TRUE , FALSE },			
			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM01
			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM02
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM03
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM04
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM05
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM06
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM07
			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM08
			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },				//	ME_USE_QUICKITEM09
			{ MSK_NONE,		KEY_F10,FALSE, FALSE },				//	ME_USE_QUICKITEM10

			{ MSK_ALT,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_MUGONGDLG
			{ MSK_ALT,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
			{ MSK_ALT,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
			{ MSK_ALT,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG

			{ MSK_ALT,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP

			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG

			{ MSK_ALT,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
			{ MSK_ALT,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
			{ MSK_ALT,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO

			{ MSK_ALT,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE	

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT1
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT2
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT3
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT4

			{ MSK_ALT,		KEY_Z,	FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE

			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
			{ MSK_ALT,		KEY_A,	FALSE, FALSE },					//	ME_TOGGLE_AUTOATTACK2

			{ MSK_ALT,		KEY_H,	FALSE, FALSE },				//	ME_TOGGLE_HELP

			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			//	ME_CAMERAMODE_FREEVIEW
			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },				//	ME_SCREENCAPTURE

			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
			{ MSK_ALL,		KEY_SHIFT,  TRUE,	TRUE },				//ME_SHOWALLNAME_OFF
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
			{ MSK_ALT,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
			{ MSK_ALT,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
			{ MSK_ALT,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
			{ MSK_ALT,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE
			{ MSK_ALT,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_GOLDSHOP
		},

		{
			{ MSK_NONE,		KEY_ESCAPE,		TRUE, FALSE },			//	ME_TOGGLE_EXITDLG
			{ MSK_NONE,		KEY_1,	FALSE, FALSE },				//	ME_USE_QUICKITEM01
			{ MSK_NONE,		KEY_2,	FALSE, FALSE },				//	ME_USE_QUICKITEM02
			{ MSK_NONE,		KEY_3,	FALSE, FALSE },				//	ME_USE_QUICKITEM03
			{ MSK_NONE,		KEY_4,	FALSE, FALSE },				//	ME_USE_QUICKITEM04
			{ MSK_NONE,		KEY_5,	FALSE, FALSE },				//	ME_USE_QUICKITEM05
			{ MSK_NONE,		KEY_6,	FALSE, FALSE },				//	ME_USE_QUICKITEM06
			{ MSK_NONE,		KEY_7,	FALSE, FALSE },				//	ME_USE_QUICKITEM07
			{ MSK_NONE,		KEY_8,	FALSE, FALSE },				//	ME_USE_QUICKITEM08
			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				//	ME_USE_QUICKITEM09
			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				//	ME_USE_QUICKITEM10
			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_MUGONGDLG
			{ MSK_NONE,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
			{ MSK_NONE,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
			{ MSK_NONE,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG
			{ MSK_NONE,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP
			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG
			{ MSK_NONE,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
			{ MSK_NONE,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
			{ MSK_NONE,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO
			{ MSK_NONE,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE
			{ MSK_NONE,		KEY_F1,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT1
			{ MSK_NONE,		KEY_F2,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT2
			{ MSK_NONE,		KEY_F3,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT3
			{ MSK_NONE,		KEY_F4,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT4
			{ MSK_NONE,		KEY_Z,		FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE
			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
			{ MSK_NONE,		KEY_A,		FALSE, FALSE },				//	ME_TOGGLE_AUTOATTACK2
			{ MSK_NONE,		KEY_H,		FALSE, FALSE },				//	ME_TOGGLE_HELP
			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },			//	ME_CAMERAMODE_FREEVIEW/BACKVIRE TOGGLE
			{ MSK_NONE,		KEY_F12,	TRUE, TRUE },				//	ME_SCREENCAPTURE
			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
			{ MSK_ALL,		KEY_SHIFT,  TRUE, TRUE },				//ME_SHOWALLNAME_OFF
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
			{ MSK_NONE,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
			{ MSK_NONE,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
			{ MSK_NONE,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
			{ MSK_NONE,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_GOLDSHOP
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_GOLDSHOP
		}
	};

	sMACRO DefaultPressMacro[MM_COUNT][MPE_COUNT] = {
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
		},
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },			//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
		}
	};

////////////////////////////////////////////////


	memcpy( m_DefaultKey, DefaultMacro, sizeof( DefaultMacro ) );
	memcpy( m_DefaultPressKey, DefaultPressMacro, sizeof( DefaultPressMacro ) );

	LoadUserMacro();
}


void CMacroManager::SetDefaultMacro( int nMacroMode )
{
	memcpy( m_MacroKey[nMacroMode], m_DefaultKey[nMacroMode], sizeof( sMACRO ) * ME_COUNT );
	memcpy( m_MacroPressKey[nMacroMode], m_DefaultPressKey[nMacroMode], sizeof( sMACRO ) * MPE_COUNT );
}


int CMacroManager::IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode )
{
	static WORD wUsableSysKey[] =
	{
		KEY_CONTROL,
		KEY_MENU,
		KEY_SHIFT
	};

	static WORD wUsableCombineKey[] =
	{
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
		KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_EQUALS, KEY_SEMICOLON, KEY_OEM_3, KEY_SLASH, KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH,
		KEY_APOSTROPHE, KEY_MINUS, KEY_EQUALS, KEY_COMMA, KEY_PERIOD, 

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

		KEY_PAD0, KEY_PAD1, KEY_PAD2, KEY_PAD3, KEY_PAD4, KEY_PAD5, KEY_PAD6, KEY_PAD7, KEY_PAD8, KEY_PAD9,
		KEY_MULTIPLY, KEY_ADD, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE
	};
	
	static WORD wUsableKey[] =
	{
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	};

	static BOOL bSysKeyDown = FALSE;
	int i;

	if( ( keyInfo->GetKeyDown( KEY_TAB ) || keyInfo->GetKeyDown( KEY_F4 ) )
		  && keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		return 0;
	}


	if( bSysKeyDown || bUserMode == 1 )
	{
		for( i = 0 ; i < sizeof( wUsableCombineKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableCombineKey[i] ) )
			{
				*wKey = wUsableCombineKey[i];
				return 1;
			}

		}
	}
	else

	{
		for( i = 0 ; i < sizeof( wUsableKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableKey[i] ) )
			{
				*wKey = wUsableKey[i];
				return 1;
			}
		}
	}

	for( i = 0 ; i < sizeof( wUsableSysKey ) / sizeof( WORD ) ; ++i )
	{
		if( keyInfo->GetKeyPressed( wUsableSysKey[i] ) )
		{
			*wKey = wUsableSysKey[i];
			bSysKeyDown = TRUE;
			return 2;
		}
	}

	*wKey = 0x0000;
	bSysKeyDown = FALSE;
	return 0;
}






