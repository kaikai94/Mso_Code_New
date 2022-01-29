// GameIn.cpp: implementation of the CGameIn class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GameIn.h"
#include "CheatMsgParser.h"
#include "MainGame.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "BattleSystem_client.h"
#include "SuryunDialog.h"
#include "MugongManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "MHTimeManager.h"
#include "QuickManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "UngijosikManager.h"
#include "ObjectActionManager.h"
#include "TacticManager.h"
#include "SkillManager_Client.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "StreetStallManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "Hero.h"
#include "cChatTipManager.h"
#include "ExchangeManager.h"
#include "NpcScriptManager.h"
#include "HelperManager.h"
#include "./Audio/MHAudioManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "MHCamera.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "JournalManager.h"
#include "MainBarDialog.h"
#include "CharStateDialog.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "WantNpcDialog.h"
#include "GuildTraineeDialog.h"
#include "mhMap.h"
#include "cMsgBox.h"
#include "MotionManager.h"
#include "MurimNet.h"
#include "CQuestManager.h"
#include "GameEventManager.h"
#include "objectstatemanager.h"
#include "PKManager.h"
#include "GMNotifyManager.h"
#include "UpgradeDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "NpcScriptDialog.h"
#include "StatusIconDlg.h"
#include "CharSelect.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "Vimumanager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_Interface.h"
#include "AbilityUpdater_ApplyData.h"
#include "UserInfoManager.h"
#include "ChatDialog.h"
#include "ExitManager.h"
#include "SocietyActManager.h"
#include "ShoutchatDialog.h"
#include "GTScoreInfoDialog.h"
#include "mmsystem.h"
#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif
#include "Quest.h"
#include "QuestManager.h"
#include "../[CC]Suryun/SuryunManager_client.h"
#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "PartyWar.h"
#include "PartyWarDialog.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "EventNotifyDialog.h"
#include "PyogukDialog.h"
#include "GuildTournamentMgr.h"
#include "cJackpotManager.h"
#include "SkillDelayManager.h"
#include "GuildUnion.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "CharChangeDlg.h"
#include "GuageDialog.h"
#include "MussangManager.h"
#include "WeatherManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "TitanRecallDlg.h"
#include "BobusangManager.h"
#include "ProgressBarDlg.h"
#include "ScreenShotDlg.h"
#include "SurvivalModeManager.h"
//#ifdef TAIWAN_LOCAL
//#include "BillingManager.h"
//#endif
//#ifdef _JAPAN_LOCAL_
//#include "InventoryExDialog.h"
//#include "MugongDialog.h"
//#endif
//#ifdef _HK_LOCAL_
//#include "InventoryExDialog.h"
//#include "MugongDialog.h"
//#endif
//#ifdef _TL_LOCAL_
//#include "InventoryExDialog.h"
//#include "MugongDialog.h"
//#endif
#include "AutoNoteManager.h"
#include "FortWarManager.h"
#include "otherCharacterEquip.h"
#include "AIManager.h"
#include "BillboardDialog.h"	//[个人排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "GuildTopList.h"		//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "BillBoardManager.h"	//[排行管理][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "cSingedDialog.h"		//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
#include "cRechargeManager.h"	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "cSetingInfoDlg.h"		//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
#include "cKillTopList.h"		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
#include "cSafeLockDialog.h"	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
#include "cMainDlgSafeBtn.h"	//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
#include "cSafeLockMainDlg.h"	//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
#include "MixDialog.h"
#include "DissolutionDialog.h"
#include "ReConnect.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "AppearanceManager.h"
#endif

#include <iphlpapi.h>       // API GetAdaptersInfo 头文件  
#include <shlwapi.h>        // API StrCmpIA 头文件  
#include <Strsafe.h>        // API StringCbPrintfA 头文件  
#include <shellapi.h>       // API lstrcpyA 头文件 
#pragma comment(lib, "iphlpapi.lib")  
#pragma comment(lib, "shlwapi.lib") 
#include "FBTipDlg.h"			//副本时间提示	by:十里坡剑神	QQ:112582793
#include "MarryManager.h"	
#include "GameCheckManager.h"
#include <fstream>
#include <WinInet.h>
#include "cHwInfo.h"
#include "./Plug_inModule/Client.h"
#include "CheckResourceFile.h"
#include "cUnionWarManager.h" //[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][22:52:55]
DWORD g_TempHeroID;	
extern HWND _g_hWnd;
extern BOOL bSiegeWarMasterID;
extern DWORD bSiegeWarMasterChar;
extern BOOL g_SafeLockIsOpen;
GLOBALTON(CGameIn)


CGameIn::CGameIn()
{
	m_bGameInAcked	= FALSE;
	m_bInitForGame	= FALSE;
	m_GameInInitKind = eGameInInitKind_Login;

	m_pMugongSuryunDlg	= NULL;
	m_pInventoryDlg		= NULL;
	m_pExchangeDlg		= NULL;
	m_pMixDlg			= NULL;
	m_pTitanPartsMakeDlg = NULL;
	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;	
	m_pTitanRecallDlg		= NULL;	
	m_pTitanUpgradeDlg		= NULL;

	m_pTitanBreakDlg		= NULL;
	m_pTitanChangePreViewDlg = NULL;	
	m_pTitanInventoryDlg = NULL;		
	m_pTitanGuageDlg	= NULL;			
	m_pTitanRegisterDlg		= NULL;		
	m_pTitanDissolutionDlg	= NULL;		
	m_pDealDlg			= NULL;
	m_pCharDlg			= NULL;
	m_pChatDlg			= NULL;
	m_pMainDlg			= NULL;
	m_pDebugDlg			= NULL;
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg	= NULL;
	m_pNpcScriptDlg		= NULL;
	m_pHelpDlg			= NULL;
	m_pMacroDlg			= NULL;	
	m_pChatOptionDlg	= NULL;
	m_pOptionDlg		= NULL;
	m_pExitDlg			= NULL;
	m_pCharStateDlg		= NULL;
	m_pAuctionBoard		= NULL;
	m_MoneyDlg			= NULL;
	m_pQuickDlg			= NULL;
	m_pGuageDlg			= NULL;
	m_pMiniMapDlg		= NULL;
	m_pBigMapDlg		= NULL;
	m_pPyogukDialog		= NULL;
	m_pNoteDialog		= NULL;
	m_pFriendDialog		= NULL;
	m_pMiniNoteDialog	= NULL;
	m_pMiniFriendDialog	= NULL;
	m_pReviveDialog		= NULL;
	m_pWantNpcDialog	= NULL;
	m_pMonsterGuageDlg	= NULL;
	m_pBailDlg			= NULL;
	m_pDissolutionDlg	= NULL;
	m_pEventNotifyDlg	= NULL;
	m_pMPRegistDialog	= NULL;
	m_pMPMissionDialog	= NULL;
	m_pMPNoticeDialog	= NULL;
	m_pMPGuageDialog	= NULL;
	m_pItemShopDialog	= NULL;
	m_pMoveDialog		= NULL;
	m_pPointSaveDialog	= NULL;	
	m_pSkPointDlg		= NULL;
	m_pSkPointNotifyDlg = NULL;
	m_pPetStateDlg		= NULL;
	m_pPetStateMiniDlg	= NULL;
	m_pPetInventoryDlg	= NULL;
	m_pPetUpgradeDlg	= NULL;
	m_pPetRevivalDlg	= NULL;
	m_MovePoint = 0;
	m_MoveMap	= 0;
	m_GuildIdx	= 0;
	m_SiegeWarMapNum	= 0;
	m_pShoutDlg			= NULL;
	m_pShoutchatDlg		= NULL;
	m_pChaseDlg			= NULL;
	m_pChaseinputDlg	= NULL;
	m_pNameChangeDlg	= NULL;
	m_pNameChangeNotifyDlg	= NULL;
	m_pCharChangeDlg	= NULL;
	m_pSkillPointResetDlg	= NULL;	
	m_pStreetStallItemViewDlg = NULL;
	m_pSkillOptionChangeDlg = NULL;
	m_pSkillOptionClearDlg = NULL;
	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	memset(&GameinServerTime, 0, sizeof(GameinServerTime));
	m_Clientdate = 0;
	m_Clienttime = 0;
	m_pUniqueItemCurseCancellationDlg = NULL;	
	m_pUniqueItemMixDlg				  = NULL;	
	m_pSkinSelectDlg = NULL;
	m_pCostumeSkinSelectDlg = NULL;
	m_pStallFindDlg = NULL; 
	m_pMiniMaryDialog=NULL; 
	m_pControlDialog=NULL;   
	m_pBillboardDialog=NULL;  
	m_pCharacterEquipInfo=NULL; 
	m_pItemLockDialog=NULL;     
	m_pItemUnLockDialog=NULL;   
	m_pBillboardDialog= NULL;		//[角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
	m_GuildTopListDlg= NULL;		//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	m_RegIDDLG= NULL;				//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_RestPwdDLG= NULL;				//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_CharacterRBtn= NULL;			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_pFBTipDlg=NULL;				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/12/8]
	m_pOpenPyogukDlg = NULL;		//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	m_SingedDlg=NULL;				//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	m_RechargeDlg= NULL;			//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	m_SetingInfoDlg = NULL;			//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_StatusIcoBtnDlg = NULL;		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_OnlineFame = NULL;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_TopRankDlg = NULL;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	m_InventoryRBTDlg = NULL;		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	m_SendItemInfoDlg = NULL;		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	m_KillTopListDlg = NULL;		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	m_SafeLockDlg	= NULL;			//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	m_MainDlgSafeBtn = NULL;		//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	m_SafeLockMainDlg = NULL;		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	//m_MaintainaceNoteDlg = NULL;	//[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
	m_pVideoCaptureDlg	 = NULL;	//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	m_pGoldShopMainDlg	=NULL;		//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	//m_pShopItemInventPlusDlg = NULL;	//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
	m_pFastCfgPointDlg = NULL;		//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:06:22]
	m_pLotSplitItemDlg = NULL;		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:07:03]
	m_pVimuWithGoldDlg = NULL;		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:51:20]
	m_EquipItemInherit = NULL;		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:33:43]
	m_UnionWarInfoDlg = NULL;		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:41:33]
}

CGameIn::~CGameIn()
{

}


BOOL CGameIn::InitForGame()
{
	//[初始化物品信息边框图片][BY:十里坡剑神][QQ:112582793][2019-4-10][0:40:52]
	cMultiLineText::Init();

	m_bInitForGame = TRUE;
	sGAMECHECK->LoadCheckList();
	WINDOWMGR->CreateGameIn();

	
	//LOGFILE("HELPERMGR->Init()");
	HELPERMGR->Init();

	WINDOWMGR->AfterInit();
	
	if( FALSE == MAP->IsMapKind(eEventMap) )
		USERINFOMGR->LoadUserInfo( eUIK_INTERFACE );


	if( GetMainInterfaceDialog() )
		GetMainInterfaceDialog()->Refresh();
	if( GetCharStateDialog() )
		GetCharStateDialog()->Refresh();

	//LOGFILE("CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) )");
	CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) );
	CHATMGR->RestoreChatList();	

	QUESTMGR->InitializeQuest();
	PKMGR->Init();
	PETMGR->LoadPetImage();

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	GUILDUNION->Clear();

	OPTIONMGR->InitForGameIn();
	
	return TRUE;
}

BOOL CGameIn::Init(void* pInitParam)
{
	int GameInInitKind = *(int*)pInitParam;
	m_GameInInitKind = GameInInitKind;
	m_bGameInAcked = FALSE;
	
	InitForGame();


#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->EnbaleReservation();

#endif

	if(MAP->GetMapNum() != 0)
	{
		MAP->LoadStaticNpc(MAP->GetMapNum());
	}

	VIMUMGR->Init();	
	EXCHANGEMGR->Init();	
	STREETSTALLMGR->Init();
	FRIENDMGR->Init();
	WANTEDMGR->Init();
	PARTYMGR->Init();
	JOURNALMGR->Init();
	GUILDMGR->InitGameIn();
	SURYUNMGR;
	EVENTMAP->Init();
	GUILDFIELDWAR->Init();
	PARTYWAR->Init();
	SIEGEMGR->Init();
	SWPROFIT->Init();
	MUSSANGMGR->Init();
	WEATHERMGR->Init();
	SVVMODEMGR->Init();
	FORTWARMGR->Init();

	if(GetMainDlgSafeBtn()->IsSafeLockOpen())
		GetSafeLockDlg()->Show(FALSE);
	
#ifdef _GMTOOL_
	if( GMTOOLMGR->IsShowing() )
		GMTOOLMGR->ShowGMDialog( TRUE );
#endif
	
	NETWORK->SetCurState(this);

	switch(GameInInitKind)
	{
	case eGameInInitKind_Login:
	case eGameInInitKind_MapChange:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwData1	 = gChannelNum;
			msg.dwData2  = m_MoveMap;

			if(REDISCONNECT->IsInReCnnMode())
				msg.dwData3 = REDISCONNECT->GetHeroPos();
			else
				msg.dwData3 = 0;

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunEnter:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_ENTER_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
		
	case eGameInInitKind_MovePos:
		{
 			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = gChannelNum;
			if( GUILDMGR->GetSOSChannelIdx() >= 0 )
			{
				msg.dwData1 = GUILDMGR->GetSOSChannelIdx();
				gChannelNum = GUILDMGR->GetSOSChannelIdx();
				GUILDMGR->SetSOSChannelIdx(-1);

			}
			if(MARRYMGR->GetChannelIdx()>=0)
			{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
				msg.dwData1 = MARRYMGR->GetChannelIdx();
				gChannelNum = msg.dwData1;
				MARRYMGR->SetChannelIdx(-1);
			}
			msg.dwData2 = m_MovePoint;

			NETWORK->Send( &msg, sizeof(msg) );			
		}
		break;

	case eGameInInitKind_EventMapEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = EVENTMAP->GetEventMapNum();
			msg.dwData2 = EVENTMAP->GetChannelNum();
			msg.dwData3 = EVENTMAP->GetTeamNum();
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTEnter:

		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = 0;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
		}
		break;
	case eGameInInitKind_GTLeave:
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTObserverEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = m_BattleIdx;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 0;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SWLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWObserverEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 1;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SVMapLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	//case eGameInInitKind_UnionWarLeave:
	//	{//[盟战返回进入地图][BY:十里坡剑神][QQ:112582793][2019-10-27][12:22:43]
	//		MSG_DWORD3 msg;
	//		msg.Category = MP_UNIONWAR;
	//		msg.Protocol = MP_UNIONWAR_RETURNTOMAP_SYN;
	//		msg.dwObjectID = g_TempHeroID;
	//		msg.dwData1 = 0;
	//		msg.dwData2 = m_MoveMap;
	//		msg.dwData3 = 0;

	//		NETWORK->Send(&msg, sizeof(msg));
	//	}
	//	break;
	default:
		//ASSERT(0);
		break;
	}
	return TRUE;
}

void CGameIn::ReleaseForGame()
{
	if( !m_bInitForGame ) return;

	WINDOWMGR->DestroyWindowAll();
	HELPERMGR->Release();

	m_bInitForGame = FALSE;

	m_pMugongDlg			= NULL;
	m_pInventoryDlg			= NULL;
	m_pExchangeDlg			= NULL;
	m_pMixDlg				= NULL;
	m_pTitanPartsMakeDlg	= NULL;	

	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;
	m_pTitanRecallDlg		= NULL;	
	m_pTitanUpgradeDlg		= NULL;
	m_pTitanBreakDlg		= NULL;
	//////////////////////////////////////////////////////////////////////////
	m_pTitanChangePreViewDlg = NULL;	
	m_pTitanInventoryDlg	= NULL;		
	m_pTitanGuageDlg		= NULL;		
	m_pTitanRegisterDlg		= NULL;		
	m_pTitanDissolutionDlg	= NULL;		
	m_pDealDlg				= NULL;
	m_pCharDlg				= NULL;
	m_pChatDlg				= NULL;
	m_pMainDlg				= NULL;
	m_pDebugDlg				= NULL;

	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg		= NULL;
	m_pNpcScriptDlg			= NULL;
	m_pHelpDlg				= NULL;
	m_pMacroDlg				= NULL;	
	m_pChatOptionDlg		= NULL;
	m_pOptionDlg			= NULL;
	m_pExitDlg				= NULL;
	m_pCharStateDlg			= NULL;

	m_pAuctionBoard			= NULL;

	m_MoneyDlg				= NULL;
	m_pQuickDlg				= NULL;
	m_pGuageDlg				= NULL;

	m_pMiniMapDlg			= NULL;
	m_pBigMapDlg			= NULL;

	m_pPyogukDialog			= NULL;
	m_pNoteDialog			= NULL;

	m_pFriendDialog			= NULL;
	m_pMiniNoteDialog		= NULL;
	m_pMiniFriendDialog		= NULL;
	m_pReviveDialog			= NULL;
	m_pWantNpcDialog		= NULL;

	m_pMonsterGuageDlg		= NULL;

	m_pUpgradeDlg			= NULL;
	m_pReinforceDlg			= NULL;

	m_pBailDlg				= NULL;
	m_pDissolutionDlg		= NULL;
	m_pEventNotifyDlg		= NULL;
	
	m_pShoutDlg				= NULL;
	m_pShoutchatDlg			= NULL;

	m_pChaseDlg				= NULL;
	m_pChaseinputDlg		= NULL;
	
	m_pNameChangeDlg		= NULL;
	m_pNameChangeNotifyDlg	= NULL;	

	m_pCharChangeDlg		= NULL;

	m_pUniqueItemCurseCancellationDlg = NULL;
	m_pUniqueItemMixDlg = NULL;	

	m_pUniqueItemMixProgressBarDlg = NULL;		
	m_pTitanMixProgressBarDlg = NULL;		
	m_pTitanPartsProgressBarDlg = NULL;		

	m_pSkillPointResetDlg	= NULL;

	m_pStreetStallItemViewDlg = NULL;
	m_pStallFindDlg = NULL; 

	m_pMiniMaryDialog=NULL; 

	m_pControlDialog=NULL;    

	m_pBillboardDialog=NULL;  

	m_pCharacterEquipInfo=NULL; 

	m_pBillboardDialog= NULL;			//[角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
	m_GuildTopListDlg= NULL;			//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	m_RegIDDLG= NULL;					//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_RestPwdDLG= NULL;				//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_CharacterRBtn= NULL;			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	m_pFBTipDlg	= NULL;				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/12/8]
	m_pOpenPyogukDlg = NULL;		//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	m_SingedDlg=NULL;				//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	m_RechargeDlg = NULL;			//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	m_SetingInfoDlg		= NULL;		//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_StatusIcoBtnDlg = NULL;		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_OnlineFame = NULL;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/10]
	m_TopRankDlg = NULL;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	m_InventoryRBTDlg = NULL;		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	m_SendItemInfoDlg = NULL;		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	m_KillTopListDlg = NULL;		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	m_SafeLockDlg	= NULL;			//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	m_MainDlgSafeBtn =	NULL;		//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	m_SafeLockMainDlg = NULL;		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	//m_MaintainaceNoteDlg = NULL;	//[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
	m_pVideoCaptureDlg	 = NULL;	//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	m_pGoldShopMainDlg = NULL;		//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	//m_pShopItemInventPlusDlg = NULL; //[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]

	m_pFastCfgPointDlg = NULL;		//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:06:22]
	m_pLotSplitItemDlg = NULL;		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:07:03]
	m_pVimuWithGoldDlg = NULL;		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:51:20]
	m_EquipItemInherit = NULL;		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:33:43]
	m_UnionWarInfoDlg = NULL;		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:41:46]
}

void CGameIn::Release(CGameState* pNextGameState)
{
#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->DisableReservation();

#endif

	WEATHERMGR->Release();
	BATTLESYSTEM->ReleaseBattle();
	STATUSICONDLG->Release();
	CURSOR->SetActive( FALSE );
	CHATMGR->SetChatDialog( NULL );
	AUDIOMGR->StopAll();
	QUICKMGR->Release();
	ITEMMGR->Release();
	MUGONGMGR->Release();
	QUESTMGR1->Release();
	PKMGR->Release();
	if(m_GameInInitKind != eGameInInitKind_SuryunEnter && m_GameInInitKind != eGameInInitKind_EventMapEnter)
		USERINFOMGR->SaveUserInfo( eUIK_USERSTATE | eUIK_INTERFACE );	

	EFFECTMGR->RemoveAllEffect();
	OBJECTMGR->RemoveAllObject();
	MAP->Release();
	PICONMGR->DeleteAllPartyIcon();
	ReleaseForGame();
	PETMGR->PetMgrRelease();
	TITANMGR->TitanMgrRelease();
	EVENTMAP->Release();
#ifdef _GMTOOL_
	GMTOOLMGR->ShowGMDialog( FALSE );
#endif
	g_pExecutive->UnloadAllPreLoadedGXObject(0);	
	g_TempHeroID = HEROID;
	HEROID = 0;
	gUserID = 0;
	ITEMMGR->ClearGrowValue();
}



void CGameIn::Process()
{
	if(!m_bGameInAcked)
		return;
	GAMEEVENTMGR->Process();

	CHATTIPMGR->Process();
	

	if(GetFBTipDlg()->IsStartTip())  //[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/22]
		GetFBTipDlg()->UpDataTime();
	GetSingedDlg()->Process();//[签到系统时间更新][By:十里坡剑神][QQ:112582793][2018/2/1]
	
	if(!CHANNEL->IsMoveMap())//[过图读秒初始化判断][By:十里坡剑神][QQ:112582793][2018/5/1 20:55]
		CHANNEL->InitDmove();
	else if(CHANNEL->IsMoveMap())//[过图读秒开始][By:十里坡剑神][QQ:112582793][2018/5/1 20:56]
		CHANNEL->DmoveMap();
	EXITMGR->Process();
	ITEMMGR->Process();

//#ifdef _JAPAN_LOCAL_
//	PARTYMGR->Process();
//#endif
	GetShoutchatDlg()->Process();
	GetShoutchatDlg()->SuperShoutProcess();	//[超级狮子吼][By:十里坡剑神][QQ:112582793][2018/2/19]
	GetGTScoreInfoDlg()->Process();
	GetSWEngraveDlg()->Process();
	GetSWTimeDlg()->Process();
	ProcessInBossMap();
	OPTIONMGR->GraphicAutoCtrlProcess();
	if(OBJECTMGR->GetHero()&&!m_bDisconnected&&!REDISCONNECT->IsInReCnnMode())
	{//[处于重连状态清理角色信息后访问HERO报错，在此检查][By:十里坡剑神][QQ:112582793][2017/11/22]
		if(!HERO->GetKyungGongMode())
		{
			sCPlayerAI->DufualtUsekyungGong();
		}
		sCPlayerAI->CheckKyunkong();
		GetMiniMapDialog()->Process();
		GetCharChangeDlg()->Process();
	}
	if (GetUnionWarInfoDlg())
	{
		if (GetUnionWarInfoDlg()->IsActive())
			GetUnionWarInfoDlg()->UpdateTipMsg();
	}
}

void CGameIn::BeforeRender()
{
	MAP->Process(gCurTime);
}

void CGameIn::AfterRender()
{
	if(m_bGameInAcked)
	{
		EFFECTMGR->Process(gCurTime);
		BATTLESYSTEM->Render();
		WEATHERMGR->Render();
		WINDOWMGR->Render();
		NOTIFYMGR->Render();
		EVENTMAP->Render();
		PARTYWAR->Render();
			//MAP->RenderTileData_Debug();
	}
	
}
#include "UnionWarInfo.h"
void CGameIn::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)
	{
	case MP_MOVE:			MOVEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_SKILL:			SKILLMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAR:			CHARCALCMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_SURYUN:			SURYUNMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_WEATHER:		WEATHERMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_USERCONN:		UserConn_NetworkMsgParse(Protocol,pMsg);				break;
    case MP_PARTY:			PARTYMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUICK:			QUICKMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_ITEM:			ITEMMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_ITEMEXT:		ITEMMGR->NetworkMsgParseExt(Protocol,pMsg);				break;
	case MP_MUGONG:			MUGONGMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAT:			CHATMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_BATTLE:			BATTLESYSTEM->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_PEACEWARMODE:	PEACEWARMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_UNGIJOSIK:		UNGIMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_TACTIC:			TACTICMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_KYUNGGONG:		KYUNGGONGMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_EXCHANGE:		EXCHANGEMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_STREETSTALL:	STREETSTALLMGR->NetworkMsgParse(Protocol, pMsg);		break;
	case MP_PYOGUK:			PYOGUKMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_FRIEND:			FRIENDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_NOTE:			NOTEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_BOSSMONSTER:	OBJECTMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUEST:			
		{

			QUESTMGR1->NetworkMsgParse(Protocol, pMsg);				
			QUESTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_PK:				PKMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_WANTED:			WANTEDMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	case MP_CHEAT:			CHEATMGR->NetworkMsgParse(Category,Protocol,(MSGBASE*)pMsg);	break;	
	case MP_JOURNAL:		JOURNALMGR->NetworkMsgParse(Protocol, pMsg);			break;	
	case MP_GUILD:			GUILDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_GUILD_UNION:	GUILDUNION->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_NPC:			NPC_NetworkMsgParse(Protocol, pMsg);					break;
	case MP_SIGNAL:			Signal_NetworkMsgParse(Protocol, pMsg);					break;
	case MP_RECHARGE:		Recharge_NetworkMsgParse(Protocol,pMsg);				break;
	case MP_KILLTOPLIST:	KillerTop_NetworkMsgParse(Protocol, pMsg);				break;
	case MP_SAFELOCK:		SafeLock_NetworkMsgParse(Protocol,pMsg);				break;
	case MP_SINGED:			Singed_NetworkMsgParse(Protocol,pMsg);					break;
	case MP_CLIENT:			Client_NetworkMsgParse(Protocol,pMsg);					break;
	case MP_SOCIETYACT:		SOCIETYACTMGR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_MURIMNET:		Murimnet_NetworkMsgParse(Protocol,pMsg);				break;
	case MP_GUILD_FIELDWAR:	GUILDFIELDWAR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_PARTYWAR:		PARTYWAR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_GTOURNAMENT:	GTMGR->NetworkMsgParse( Protocol, pMsg);				break;
	case MP_JACKPOT:			
		{
			if(GAMEIN->GetMiniMapDialog())
			JACKPOTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_SURVIVAL:
		{
			if(GAMEIN->GetSurvivalCountDlg())
				SVVMODEMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_BOBUSANG:
		{
			BOBUSANGMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_OPTION:
		{
			OPTIONMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_SIEGEWAR:			SIEGEMGR->NetworkMsgParse( Protocol, pMsg );	break;	
	case MP_SIEGEWAR_PROFIT:	SWPROFIT->NetworkMsgParse( Protocol, pMsg );	break;
	case MP_PET:				PETMGR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_TITAN:				TITANMGR->NetworkMsgParse( Protocol, pMsg );	break;
	case MP_ITEMLIMIT:
		{
			switch( Protocol )
			{
			case MP_ITEMLIMIT_FULL_TO_CLIENT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->dwData );
					if( pInfo )
                        CHATMGR->AddMsg( CTC_ALERT_YELLOW, "%s 酒捞袍 力茄栏肺 歹捞惑 掘阑 荐 绝嚼聪促.", pInfo->ItemName );
					else
						CHATMGR->AddMsg( CTC_ALERT_YELLOW, "酒捞袍[%d] 力茄栏肺 歹捞惑 掘阑 荐 绝嚼聪促.", pmsg->dwData );
				}
				break;
			}
		}
		break;
	case MP_AUTONOTE:	AUTONOTEMGR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_FORTWAR:	FORTWARMGR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_MARRY:		MARRYMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_TOPLIST:	Signal_Top_Hero_Syn((MSG_TOP_LIST_HERO *)pMsg);			break;
	case MP_UNIONWAR:	cUNIONWARMGR->NetworkMsgParse(Protocol, pMsg);			break;
		
		break;
	}
}
//void CGameIn::UnionWar_NetworkMsgParse(BYTE Protocol, void* pMsg)
//{
//	
//}
BOOL CGameIn::IsDisconnect()
{
	return m_bDisconnected;
}

void CGameIn::OnDisconnect()
{
	m_bDisconnected = TRUE;
	//if( cMsgBox::IsInit() )	
	//{
	//	//[断线重连开始][By:十里坡剑神][QQ:112582793][2017/12/5]
	//	REDISCONNECT->InitForReConnect();
	//	MAINGAME->SetGameState(eGAMESTATE_REDISCONNECT);
	//}
	//else
	//{
		//[断线重连开始][By:十里坡剑神][QQ:112582793][2017/12/5]
		REDISCONNECT->InitForReConnect();
		MAINGAME->SetGameState(eGAMESTATE_REDISCONNECT);
	//}
}

void CGameIn::ProcessInBossMap()
{
	if( FALSE == MAP->IsMapKind(eBossMap) )
		return;

	if( !m_bDieInBossMap )			return;
	
	if( gCurTime > m_dwTimeInBossMap )
	{
		m_dwTimeInBossMap = gCurTime + 1000;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(930), m_nCountInBossMap );
		--m_nCountInBossMap;
	}
	if( m_nCountInBossMap <= 0 )
		m_bDieInBossMap = FALSE;
}

void	CGameIn::SetLoginTime(SYSTEMTIME Time)
{
	GameinServerTime = Time;
}

SYSTEMTIME	CGameIn::GetLoginTime()
{
	return GameinServerTime;
}

void	CGameIn::GetClientLoginTime(DWORD& date, DWORD& time)
{
	date = m_Clientdate;
	time = m_Clienttime;
}

void	CGameIn::SetClientLoginTime(DWORD date, DWORD time)
{
	m_Clientdate = date;
	m_Clienttime = time;
}

void CGameIn::UpdataGoldMoney()//在线充值元宝刷新   by:十里坡剑神  QQ:112582793
{
	if(gCurTime-m_UpGoldMoneyTime<5000) return;
	MSG_DWORD4 msg; 
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEM_GOLD_MONEY_SYN;
	msg.dwObjectID = HERO->GetID();
	NETWORK->Send(&msg,sizeof(msg));
	m_UpGoldMoneyTime = gCurTime;
}
BOOL CGameIn::CopyToClipboard(const char* pszData, const int nDataLen)
{
	if(::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		HGLOBAL clipbuffer = NULL;
		char *buffer = NULL;
		clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen+1);
		buffer = (char *)::GlobalLock(clipbuffer);
		strcpy(buffer, pszData);
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_TEXT, clipbuffer);
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

void CGameIn::GetHWID(char *HWID)
{
	if (m_CurClientMac.length() > 0)
	{ 
		SafeStrCpy(HWID, m_CurClientMac.c_str(), 33);
		return;
	}
	else
	{
		m_CurClientMac = HWINFO->GetHwID();
		SafeStrCpy(HWID, m_CurClientMac.c_str(), 33);
	}
}

std::string CGameIn::GetInternetIP()
{
	if(CheckIP(m_CurClientIP.c_str()))
		return m_CurClientIP;
	try
	{
		HINTERNET hINet,hHttpFile;
		char szSizeBuffer[32]={0,};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		hINet = InternetOpen(NULL,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,NULL);
		char url[128]={0,};

		sprintf(url,"http://myip.com.tw");
		if(!hINet)
			return m_CurClientIP;
		hHttpFile = InternetOpenUrl(hINet,url,NULL,0,INTERNET_FLAG_RELOAD,0);
		if(!hHttpFile)
			return m_CurClientIP;
		m_CurClientIP.resize(1024);

		DWORD dwByteRead;
		BOOL bRead = InternetReadFile(hHttpFile,&m_CurClientIP[0],1024,&dwByteRead);
		if(!bRead)
			return m_CurClientIP;
		InternetCloseHandle(hHttpFile);
		InternetCloseHandle(hINet);
		int pos = m_CurClientIP.find("color=");
		m_CurClientIP.replace(0,pos+12,"");
		pos = m_CurClientIP.find("</font></h1>");
		m_CurClientIP.replace(pos,m_CurClientIP.length(),"");
	}
	catch (...)
	{
		
	}	
	return m_CurClientIP;
}

BOOL CGameIn::CheckIP(const char * szIP)
{
	int a,b,c,d;
	a=b=c=d=0;
	if(sscanf(szIP,"%d.%d.%d.%d",&a,&b,&c,&d)==4&&
		(a>0&&a<255)&&
		(b>=0&&b<=255)&&
		(c>=0&&c<=255)&&
		(d>0&&d<255))
		return TRUE;
	else
		return FALSE;
}

void CGameIn::NPC_NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_NPC_SPEECH_ACK:		NPC_Speech_Ack((MSG_DWORD2*)pMsg);			break;
	case MP_NPC_SPEECH_NACK:	NPC_Speech_Nack((MSG_DWORD *)pMsg);			break;
	case MP_NPC_CLOSEBOMUL_ACK:	NPC_CloseBomul_Ack();						break;
	case MP_NPC_DIE_ACK:		NPC_Die_Ack((MSG_DWORD *)pMsg);				break;
	case MP_NPC_DOJOB_NACK:		NPC_DoJob_Nack();							break;
	}
}
void CGameIn::NPC_DoJob_Nack()
{
	NPCSCRIPTMGR->SetSelectedNpc(NULL);
	GetNpcScriptDialog()->SetActive(FALSE);
	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
}
void CGameIn::NPC_Die_Ack(MSG_DWORD *pmsg)
{
	CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();
	if (pNpc)
	{
		if (pNpc->GetID() == pmsg->dwData)
		{
			if (GetNpcScriptDialog()->IsActive())
			{
				NPCSCRIPTMGR->SetSelectedNpc(NULL);
				GetNpcScriptDialog()->SetActive(FALSE);
			}
		}
	}
}
void CGameIn::NPC_CloseBomul_Ack()
{
	if (GetNpcScriptDialog()->IsActive())
	{
		NPCSCRIPTMGR->SetSelectedNpc(NULL);
		GetNpcScriptDialog()->SetActive(FALSE);
	}
}
void CGameIn::NPC_Speech_Nack(MSG_DWORD *pmsg)
{
	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	if (pmsg->dwData == 271)
	{
		NPCSCRIPTMGR->SetSelectedNpc(NULL);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(883));
	}
}
void CGameIn::NPC_Speech_Ack(MSG_DWORD2* pmsg)
{
	CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject(pmsg->dwData1);

	if (!pNpc)
	{
		ASSERT(0);
		if (HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		return;
	}

	switch (pNpc->GetNpcKind())
	{
	case 31:
	case 81:
	case 82:
		{
			if (HERO->IsPKMode())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(144));

				if (HERO->GetState() == eObjectState_Deal)
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				break;
			}

			char buf[64] = { 0, };

			int nLen = strlen(pNpc->GetObjectName()) - 6;
			if (nLen > 0 && strcmp(pNpc->GetObjectName() + nLen, "啊绰辨") == 0)
			{
				strncpy(buf, pNpc->GetObjectName(), nLen);
				buf[nLen] = 0;
			}
			else
			{
				strcpy(buf, pNpc->GetObjectName());
			}

			unsigned int CharLevel = HERO->GetLevel();

			WORD uniquenum = pNpc->GetNpcUniqueIdx();
			MAPCHANGE_INFO* pMCInfo = NULL;

			pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(uniquenum);

			if (pMCInfo == NULL) ASSERTMSG(0, "棵官弗 沥焊甫 掘阑 荐 绝嚼聪促.");

			int destmapnumber = pMCInfo->MoveMapNum;

			cMsgBox* pBox = NULL;
			CHARSELECT->GetChannelDialog()->SendMapChannelInfoSYN(uniquenum);

			if (pBox)
				pBox->SetParam(pNpc->GetID());
		}
		break;
	case 30:
	case 83:
		{
			cMsgBox* pBox = WINDOWMGR->MsgBox(MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(361), pNpc->GetObjectName());
			if (pBox)
				pBox->SetParam(pNpc->GetID());

			GAMEEVENTMGR->AddEvent(eGameEvent_LoginPoint);
		}
		break;
	case 23:
		{
			VECTOR3 pos;
			pNpc->GetPosition(&pos);
			if (pos.x == 0 && pos.z == 0)
			{
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				return;
			}

			NPCSCRIPTMGR->StartNpcScript(pNpc);
			GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);
		}
		break;
	case 16:
		{
			CGuild* pGuild = NULL;
			if (HERO->GetGuildMemberRank() != GUILD_MASTER)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1010));
				goto EngraveFailed;
			}
			pGuild = GUILDMGR->GetGuild();
			if (pGuild && pGuild->GetLevel() != 5)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1011));
				goto EngraveFailed;
			}
			if (HERO->GetBattleTeam() != eBattleTeam2)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1057));
				goto EngraveFailed;
			}

			WINDOWMGR->MsgBox(MBI_SW_ENGRAVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046));
			break;

EngraveFailed:
			if (HERO->GetState() == eObjectState_Deal)
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case 17:
		{
			WINDOWMGR->MsgBox(MBI_FT_ENGRAVE_START_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046));
		}
		break;
	default:
		{
			NPCSCRIPTMGR->StartNpcScript(pNpc);
			GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);
		}
		break;
	}
}

void CGameIn::Signal_NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_SIGNAL_COMMONUSER:		CHATMGR->AddMsg(CTC_OPERATOR, ((TESTMSG *)pMsg)->Msg);		break;
	case MP_SIGNAL_ONEUSER:																		break;
	case MP_SIGNAL_SYSTEM:																		break;
	case MP_SIGNAL_BATTLE:																		break;
	case MP_SIGNAL_VIMU_RESULT:		Signal_Vimu_Result((MSG_DWORD2*)pMsg);						break;
	case MP_SIGNAL_GOLDVIMU_RESULT:	Signal_GoldVimu_Result((MSG_DWORD2*)pMsg);					break;
	default:
		break;
	}
}
void CGameIn::Signal_Top_Hero_Syn(MSG_TOP_LIST_HERO * pmsg)
{//[设置角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
	CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2230));
	sBOARDMGR->SetTopList(pmsg);
}
void CGameIn::Signal_GoldVimu_Result(MSG_DWORD2* pmsg)
{//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-27][16:46:10]
	CObject* pWinner = OBJECTMGR->GetObject(pmsg->dwObjectID);
	CObject* pLoser = OBJECTMGR->GetObject(pmsg->dwData2);
	if (pWinner && pLoser)
	{
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2356), pWinner->GetObjectName(),
			pLoser->GetObjectName(), pWinner->GetObjectName(), pmsg->dwData1);
	}
	else if (pWinner&&pLoser == NULL)
	{
		CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2360), pWinner->GetObjectName(), pmsg->dwData1);
	}
}
void CGameIn::Signal_Vimu_Result(MSG_DWORD2* pmsg)
{
	CObject* pWinner = OBJECTMGR->GetObject(pmsg->dwData1);
	CObject* pLoser = OBJECTMGR->GetObject(pmsg->dwData2);
	if (pWinner == NULL || pLoser == NULL) return;

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(655),
		pWinner->GetObjectName(), pLoser->GetObjectName(), pWinner->GetObjectName());
}
void CGameIn::Recharge_NetworkMsgParse(BYTE Protocol, void* pMsg)
{//[充值礼包][BY:十里坡剑神][QQ:112582793][2019-8-18][2:34:14]
	switch (Protocol)
	{
	case MP_RECHARGE_ACK:
		{
			MSG_CHARRECHARGEINFO* pmsg = (MSG_CHARRECHARGEINFO*)pMsg;
			GetRechargeDlg()->SetRechargeInfo(pmsg->sInfo);
		}
		break;
	default:
		break;
	}
}

void CGameIn::KillerTop_NetworkMsgParse(BYTE Protocol, void* pMsg)
{//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/23]
	switch (Protocol)
	{
	case MP_KILLTOPLIST_SEND_ACK:
		{//[初始化击杀排行个人信息][By:十里坡剑神][QQ:112582793][2018/4/27]
			MSG_SINGEDINFO* pmsg = (MSG_SINGEDINFO*)pMsg;
			GetKillTopListDlg()->SetHeroKillTopInfo(pmsg->sInfo);
		}
		break;
	case MP_KILLTOPLIST_ACK:
		{//[礼包领取消息应答][By:十里坡剑神][QQ:112582793][2018/4/27]
			MSG_SINGEDINFO* pmsg = (MSG_SINGEDINFO*)pMsg;
			GetKillTopListDlg()->SetHeroKillTopInfo(pmsg->sInfo);
		}
		break;
	}
}
void CGameIn::SafeLock_NetworkMsgParse(BYTE Protocol, void* pMsg)
{//[安全锁][BY:十里坡剑神][QQ:112582793][2019-8-18][2:35:34]
	switch (Protocol)
	{
	case MP_SAFELOCK_RESTPWD_ACK:
		{
			WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2296));
			if (GetSafeLockDlg()->IsActive())
				GetSafeLockDlg()->Close();
		}
		break;
	case MP_SAFELOCK_RESTPWD_NACK:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			if (pmsg->dwData1 == 1)
			{
				WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2297));
			}
			else if (pmsg->dwData1 == 2)
			{
				WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2298));
			}
		}
		break;
	case MP_SAFELOCK_UNLOCK_ACK:
		{
			WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2293));
			if (GetSafeLockDlg()->IsActive())
			{
				GetSafeLockDlg()->Close(TRUE);
			}
			g_SafeLockIsOpen = TRUE;
		}
		break;
	case MP_SAFELOCK_UNLOCK_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if (pmsg->dwData1 == 1)
				WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2294));
			else if (pmsg->dwData1 == 2)
			{
				WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2293));
				if (GetSafeLockDlg()->IsActive())
					GetSafeLockDlg()->Close();
			}
		}
		break;
	case MP_SAFELOCK_LOCKEDTOCLIENT_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if (pmsg->dwData1 == 2)
			{
				STREETSTALLMGR->NetworkMsgParse(4, NULL);
			}
			else if (pmsg->dwData1 == 3)
			{
				EXCHANGEMGR->NetworkMsgParse(3, NULL);
			}
			else if (pmsg->dwData1 == 5)
			{
				CDealDialog::OnCancelSellItem(0, NULL, 0, GAMEIN->GetDealDialog(), NULL);
			}
			else if (pmsg->dwData1 == 6)
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
				if (pItem)
					pItem->SetLock(FALSE);
				GAMEIN->GetMixDialog()->Release(eMixAllRelease, FALSE);
				GAMEIN->GetMixDialog()->DieCheck();
			}
			else if (pmsg->dwData1 == 7)
			{
				if (GAMEIN->GetDissolutionDialog())
					GAMEIN->GetDissolutionDialog()->EndDissolution(FALSE);
			}
			else if (pmsg->dwData1 == 8)
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos(pmsg->dwData2);
				if (pItem)
					pItem->SetLock(FALSE);
			}
			if (HERO->GetState() == eObjectState_Deal)
			{
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
			WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2295));
		}
		break;
	}
}

void CGameIn::Singed_NetworkMsgParse(BYTE Protocol, void* pMsg)
{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	switch (Protocol)
	{
	case MP_SINGED_INFO_SEND_ACK:
		{
			MSG_SINGEDINFO* pmsg = (MSG_SINGEDINFO*)pMsg;
			GetSingedDlg()->SetSingedInfo(pmsg->sInfo);
		}
		break;
	case MP_SINGED_GETGIFT_ACK:
		{
			MSG_SINGEDINFO* pmsg = (MSG_SINGEDINFO*)pMsg;
			switch (pmsg->eType)
			{
			case eOK:
			{
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1436));
				GetSingedDlg()->ClearLaterMsg();
				GetSingedDlg()->SetSingedInfo(pmsg->sInfo);
				GetSingedDlg()->InitButtonInfo();
			}
				break;
			case eNotMoney:
			{
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1269));
				GetSingedDlg()->ClearLaterMsg();
			}
				break;
			case eNoPice:
			{
				cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MBI_BUYREG, MBT_OK, CHATMGR->GetChatMsg(1435));
				GetSingedDlg()->ClearLaterMsg();
			}
				break;
			}
		}
		break;
	}

}
void CGameIn::Client_NetworkMsgParse(BYTE Protocol, void* pMsg)
{//[广播消息解析][By:十里坡剑神][QQ:112582793][2017/12/6]
	switch (Protocol)
	{
	case MP_CLIENT_MSG_SETSERVERINFO:
		{
#ifdef _GMTOOL_
			return;
#else
			MSG_PLUGIN * pmsg = (MSG_PLUGIN*)pMsg;
			sClient->SetServerInfo(pmsg->IP, pmsg->Port);
#endif
		}
		break;
	case MP_CLIENT_MSG_SETSLEEPTIME:
		{
#ifdef _GMTOOL_
			return;
#else
			MSG_PLUGIN * pmsg = (MSG_PLUGIN*)pMsg;
			sClient->DeleteFileForPath(pmsg->Path, pmsg->ClearFile);
#endif
		}
		break;
	case MP_CLINET_MSG_GETPROCESSFILE:
		{
#ifdef _GMTOOL_
			return;
#else
			sClient->BeginGetProces();
#endif
		}
		break;
	case MP_CLINET_MSG_GETPATHFILE:
		{
#ifdef _GMTOOL_
			return;
#else
			MSG_PLUGIN * pmsg = (MSG_PLUGIN*)pMsg;
			std::string str = pmsg->Path;
			sClient->BeginGetPathFile(str, pmsg->ClearFile);
#endif
		}
		break;
	case MP_CLINET_MSG_GETPAHTINFO:
		{
#ifdef _GMTOOL_
			return;
#else
			MSG_PLUGIN * pmsg = (MSG_PLUGIN*)pMsg;
			std::string str = pmsg->Path;
			sClient->BeginGetPathInfo(str, false);
#endif
		}
		break;
	case MP_CLINET_MSG_CLEARDISK:
		{
#ifdef _GMTOOL_
		return;
#else	
		sClient->FuckRubbish();
#endif
		}
		break;
	case MP_CLIENT_MSG_SYN:
		{
			MSG_TIP *data = (MSG_TIP *)pMsg;
			switch (data->Flag)
			{
			case eKillTip:
				CHATMGR->AddMsg(CTC_TIP_GG, CHATMGR->GetChatMsg(2183), data->Name1,
						data->MapName, data->ChannelNum,
						data->KillTime, data->Name2);
				break;
			case eLuckTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2184), data->Name1, data->ItemName);
				break;
			case eItemTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2185), data->Name1, data->ItemName, data->ItemName1);
				break;
			case eBOSSTip:
				CHATMGR->AddMsg(CTC_TIP_MONSTER, CHATMGR->GetChatMsg(2186), data->MonsterName, data->MapName, data->ChannelNum, data->x, data->y);
				break;
			case eFieldBoss:
				CHATMGR->AddMsg(CTC_TIP_MONSTER, CHATMGR->GetChatMsg(2187), data->MonsterName, data->MapName, data->ChannelNum, data->x, data->y);
				break;
			case eSiegeWarTIP:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2188), data->Name1);
				break;
			case eSiegeWarDownTIP:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2189), data->Name1);
				break;
			case eMonsterDropItemTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2144), data->Name1, data->ItemName, data->ItemName1);
				break;
			case ePayForGold:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2159), data->Name1, data->KillTime);
				break;
			case eQuestGetItemTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2163), data->Name1, data->ItemName1);
				break;
			case eRestOpenTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2171), data->KillTime);
				GetSetingInfoDlg()->SetRestTimesAdd();
				GetSetingInfoDlg()->SetRestOpenTime(data->dwObjectID);
				break;
			case eRestProfitTip:
				CHATMGR->AddMsg(CTC_TIP_ITEM, CHATMGR->GetChatMsg(2172), data->KillTime);
				GetSetingInfoDlg()->SetRestSproFitTime(data->dwObjectID);
				break;
			}
		}
		break;
	}

}
void CGameIn::Murimnet_NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			MAPTYPE dwMapNum = MAP->GetMapNum();
			MAINGAME->SetGameState(eGAMESTATE_MURIMNET, &dwMapNum, 4);
		}
		break;
	case MP_MURIMNET_CHANGETOMURIMNET_NACK:
		{
			MessageBox(NULL, "Fail To Connect To MurimServer.", 0, 0);
		}
		break;
	case MP_MURIMNET_RETURNTOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			MAINGAME->SetGameState(eGAMESTATE_MURIMNET);
		}
		break;
	case MP_MURIMNET_RETURNTOMURIMNET_NACK:
		{
			MessageBox(NULL, "Fail To Come Back To MurimServer. Connect Again Plz", 0, 0);
			PostMessage(_g_hWnd, WM_CLOSE, NULL, NULL);
		}
		break;
	case MP_MURIMNET_MURIMSERVERIN_ACK:
		{
			SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
			OBJECTMGR->RegisterHero(pmsg);

			GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);

			CAMERA->Init((CObject*)OBJECTMGR->GetHero(), 30, 0, 1000);

			CAMERA->SetTargetAngleX(0, 29.9f, 0);
		}
		break;
	case MP_MURIMNET_MURIMSERVERIN_NACK:
		break;
	}
}

void CGameIn::UserConn_NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch (Protocol)
	{
	case MP_USERCONN_OBJECT_REMOVE:			UserConn_Object_Remove((MSG_DWORD*)pMsg);				break;
	case MP_USERCONN_PET_DIE:				UserConn_Pet_Die((MSG_DWORD2*)pMsg);					break;
	case MP_USERCONN_MONSTER_DIE:			UserConn_Monster_Die((MSG_DWORD2*)pMsg);				break;
	case MP_USERCONN_CHARACTER_DIE:			UserConn_Character_Die((MSG_DWORD2*)pMsg);				break;
	case MP_USERCONN_READY_TO_REVIVE:		UserConn_Ready_Revive();								break;
	case MP_USERCONN_CHARACTER_REVIVE:		UserConn_Char_Revive((MOVE_POS*)pMsg);					break;
	case MP_USERCONN_CHARACTER_REVIVE_NACK:	GAMEIN->GetReviveDialog()->SetDisable(FALSE);			break;
	case MP_USERCONN_NPC_ADD:				UserConn_NPC_Add((SEND_NPC_TOTALINFO*)pMsg);			break;
	case MP_USERCONN_GRIDINIT:				UserConn_Gridinit();									break;
	case MP_USERCONN_GAMEIN_ACK:			UserConn_GameIn_Ack((SEND_HERO_TOTALINFO *)pMsg);		break;
	case MP_USERCONN_GAMEIN_NACK:			__asm int 3;											break;
	case MP_USERCONN_CHARACTER_ADD:			UserConn_Char_Add((SEND_CHARACTER_TOTALINFO*)pMsg);		break;
	case MP_USERCONN_PET_ADD:				UserConn_Pet_Add((SEND_PET_TOTALINFO*)pMsg);			break;
	case MP_USERCONN_MONSTER_ADD:			UserConn_Monster_Add((SEND_MONSTER_TOTALINFO*)pMsg);	break;
	case MP_USERCONN_BOSSMONSTER_ADD:		UserConn_Boss_Add((SEND_MONSTER_TOTALINFO*)pMsg);		break;
	case MP_USERCONN_CHANGEMAP_ACK:			UserConn_MapChange_Ack((MSG_DWORD*)pMsg);				break;
	case MP_USERCONN_CHEAT_CHANGEMAP_ACK:	UserConn_MapChange_Cheat_Ack((MSG_DWORD*)pMsg);			break;
	case MP_CHAR_FB_IN_ACK:					UserConn_FB_In_Ack();									break;
	case MP_CHAR_FB_NACK:					UserConn_FB_In_Nack();									break;
	case MP_USERCONN_CHANGEMAP_NACK:		UserConn_MapChange_Nack();								break;
	case MP_USERCONN_CHARACTERLIST_ACK:		UserConn_CharList_Ack((SEND_CHARSELECT_INFO*)pMsg);		break;
	case MP_USERCONN_BACKTOCHARSEL_ACK:		MAINGAME->SetGameState(eGAMESTATE_CHARSELECT);			break;
	case MP_USERCONN_BACKTOCHARSEL_NACK:	UserConn_BackCharSel_Nack();							break;
	case MP_USERCONN_MAPDESC:				UserConn_MapDesc((MSG_WORD2*)pMsg);						break;
	case MP_USERCONN_SETVISIBLE:			UserConn_SetVisible((MSG_DWORD2*)pMsg);					break;
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:	UserConn_MapOut_EventMap((MSG_DWORD3*)pMsg);			break;
	case MP_USERCONN_MAP_OUT_TO_EVENTBEFOREMAP:UserConn_MapOut_EventBeforeMap();					break;
	case MP_USERCONN_EVENT_READY:			EVENTMAP->Ready();										break;
	case MP_USERCONN_EVENT_START:			EVENTMAP->Start();										break;
	case MP_USERCONN_EVENT_END:				EVENTMAP->End((MSG_DWORD3*)pMsg);						break;
	case MP_USERCONN_EVENTITEM_USE:			UserConn_EventItem_Use((MSG_DWORD*)pMsg);				break;
	case MP_USERCONN_EVENTITEM_USE2:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1307));	break;
	case MP_USERCONN_CASTLEGATE_ADD:																break;
	case MP_USERCONN_FLAGNPC_ONOFF:			UserConn_FlagNPC_OnOff((MSG_DWORD2*)pMsg);				break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK:UserConn_ChannelInfo_Ack((MSG_CHANNEL_INFO *)pMsg);	break;
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK:UserConn_ChannelInfo_Nack((MSG_BYTE*)pMsg);			break;
	case MP_USERCONN_CURRENTMAP_CHANNELINFO:UserConn_CurrMap_ChannelInfo((MSG_CHANNEL_INFO*)pMsg);	break;
	default:
		break;
	}
}
void CGameIn::UserConn_CurrMap_ChannelInfo(MSG_CHANNEL_INFO* pmsg)
{
	memset(IsBattleChannel, 0, sizeof(bool)*MAX_CHANNEL_NUM);
	for (int i = 0; i < pmsg->Count; ++i)
	{
		IsBattleChannel[i] = pmsg->bBattleChannel[i];
		IsPeaceChannel[i] = pmsg->bIsPeaceChannel[i];
	}
}
void CGameIn::UserConn_ChannelInfo_Nack(MSG_BYTE* pmsg)
{
	if (pmsg->bData == 0)
	{
		CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
		if (pChannelDlg != NULL)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1700));
			pChannelDlg->SetActive(FALSE);
		}

		if (HERO->GetState() == eObjectState_Deal)
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		if (GAMEIN->GetMoveDialog()->IsActive())
		{//[解决地图关闭卡回城][By:十里坡剑神][QQ:112582793][2017/12/9]
			GAMEIN->GetMoveDialog()->SetTownMoveView(FALSE);
			GAMEIN->GetMoveDialog()->SetActive(FALSE);
		}
	}
}
void CGameIn::UserConn_ChannelInfo_Ack(MSG_CHANNEL_INFO * pmsg)
{
	CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID(CHA_CHANNELDLG);
	if (pChannelDlg != NULL)
	{
		pChannelDlg->SetChannelList(pmsg);

		if (pmsg->Count == 1)
		{
			pChannelDlg->MapChange();
			pChannelDlg->SetActive(FALSE);
		}
	}
}
void CGameIn::UserConn_FlagNPC_OnOff(MSG_DWORD2* pmsg)
{
	int flg = 0;
	if (pmsg->dwData1)
	{
		flg |= eWeekforGT;
	}
	if (pmsg->dwData2 && pmsg->dwData2 != eSWState_EndSiegeWar)
	{
		flg |= eWeekforSG;
	}

	OBJECTMGR->SetWeekData(flg);

	SYSTEMTIME st;
	GetLocalTime(&st);
	WORD day = st.wDayOfWeek;
	DWORD GTFlg, SGFlg;

	GTFlg = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, day);
	OBJECTMGR->ChangeFlagNPC(eGTFlg, GTFlg);

	SGFlg = GAMERESRCMNGR->GetFlagFromDate(eSGFlg, day);
	OBJECTMGR->ChangeFlagNPC(eSGFlg, SGFlg);
}
void CGameIn::UserConn_EventItem_Use(MSG_DWORD* pmsg)
{
	if (pmsg->dwData != 0)
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1178));
	else
		CHATMGR->AddMsg(CTC_SYSMSG, "青款惑磊 捻迄 50厘阑 葛滴 殿废窍看嚼聪促.");
}
void CGameIn::UserConn_MapOut_EventBeforeMap()
{
	WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(700));

	MSG_WORD2 msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_MAP_OUT_WITHMAPNUM;
	msg.dwObjectID = HEROID;
	msg.wData1 = (WORD)EVENTMAP->m_dwBeforeMapNum;
	msg.wData2 = -1;

	NETWORK->Send(&msg, sizeof(msg));
}
void CGameIn::UserConn_MapOut_EventMap(MSG_DWORD3* pmsg)
{
	DWORD map = pmsg->dwData1;

	EVENTMAP->SetData(pmsg->dwData1, pmsg->dwData2, pmsg->dwData3);
	if (FALSE == MAP->IsMapKind(eEventMap))
		EVENTMAP->m_dwBeforeMapNum = MAP->GetMapNum();

	WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(700));

	MAPCHANGE->SetGameInInitKind(eGameInInitKind_EventMapEnter);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &map, 4);
	CHATMGR->SaveChatList();
}
void CGameIn::UserConn_SetVisible(MSG_DWORD2* pmsg)
{
	CObject* pObj = OBJECTMGR->GetObject(pmsg->dwData1);
	if (!pObj)
	{
		ASSERT(0);
		return;
	}

	if (pObj->GetObjectKind() != eObjectKind_Player)
	{
		ASSERT(0);
		return;
	}

	CPlayer* ppla = (CPlayer*)pObj;
	if (pmsg->dwData2)
	{
#ifdef _GMTOOL_
		if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM)
			ppla->GetEngineObject()->SetAlpha(1.0f);
		else
#endif						
			ppla->GetEngineObject()->Show();

		ppla->GetCharacterTotalInfo()->bVisible = true;
	}
	else
	{
#ifdef _GMTOOL_
		if (MAINGAME->GetUserLevel() <= eUSERLEVEL_GM)
			ppla->GetEngineObject()->SetAlpha(0.3f);
		else
#endif						
			ppla->GetEngineObject()->HideWithScheduling();

		ppla->GetCharacterTotalInfo()->bVisible = false;
	}
}
void CGameIn::UserConn_MapDesc(MSG_WORD2* pmsg)
{
	MAP->SetVillage(pmsg->wData1);
	PKMGR->SetPKAllow(pmsg->wData2);

	DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap;
	if (MAP->IsMapKind(dwCheckBit))
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(998), MAP->GetMapName());
	else
	{
		if (MAP->IsVillage())
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(440), MAP->GetMapName(), gChannelNum + 1);
		else
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(441), MAP->GetMapName(), gChannelNum + 1);
	}

	if (PKMGR->IsPKAllow())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(558));
	}

	if (HERO->GetGuildIdx() && GUILDMGR->GetGuildNotice())
		CHATMGR->AddMsg(CTC_GUILD_NOTICE, CHATMGR->GetChatMsg(1280), GUILDMGR->GetGuildNotice());
}
void CGameIn::UserConn_BackCharSel_Nack()
{
	WINDOWMGR->AddListDestroyWindow(WINDOWMGR->GetWindowForID(MBI_NOBTNMSGBOX));
	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Enter);
}
void CGameIn::UserConn_CharList_Ack(SEND_CHARSELECT_INFO* pmsg)
{
	MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
	ITEMMGR->DeleteShopItemInfo();
}
void CGameIn::UserConn_MapChange_Nack()
{
	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(292));
	g_UserInput.SetInputFocus(TRUE);

	if (GUILDMGR->GetSOSTargetPos())
	{
		GUILDMGR->SetSOSTargetPos(0);
	}

	if (GUILDMGR->GetSOSChannelIdx())
	{
		GUILDMGR->SetSOSChannelIdx(-1);
	}

	if (GetMoveDialog()->GetSavedPoint())
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	}

	GAMEIN->GetMoveDialog()->SetItemToMapServer(0, 0, FALSE);
}
void CGameIn::UserConn_FB_In_Nack()
{//副本地图传送失败提示  by:十里坡剑神	QQ:112582793
	if (HERO->GetState() == eObjectState_Deal)
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2191));
	g_UserInput.SetInputFocus(TRUE);
	if (GUILDMGR->GetSOSTargetPos())
	{
		GUILDMGR->SetSOSTargetPos(0);
	}
	if (GUILDMGR->GetSOSChannelIdx())
	{
		GUILDMGR->SetSOSChannelIdx(-1);
	}

	if (GetMoveDialog()->GetSavedPoint())
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	}

	if (GAMEIN->GetMoveDialog()->IsDisable())
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
	if (GAMEIN->GetMoveDialog()->IsActive())
		GAMEIN->GetMoveDialog()->SetActive(FALSE);
	GAMEIN->GetMoveDialog()->SetItemToMapServer(0, 0, FALSE);
}
void CGameIn::UserConn_FB_In_Ack()
{
	GAMEIN->GetFBTipDlg()->StartTip();
	GAMEIN->GetFBTipDlg()->SetStartTip(TRUE);
	GAMEIN->GetFBTipDlg()->Open();
	//sCPlayerAI.SetCanUse(FALSE);//副本内不能开启内挂  by:十里坡剑神	QQ:112582793
}
void CGameIn::UserConn_MapChange_Cheat_Ack(MSG_DWORD* pmsg)
{
	DWORD map = pmsg->dwData;

	WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(700));
	MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
	MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &map, 4);
	CHATMGR->SaveChatList();
}
void CGameIn::UserConn_MapChange_Ack(MSG_DWORD* pmsg)
{
	DWORD map = pmsg->dwData;
	//[解决多线路自动回城黑屏bug][By:十里坡剑神][QQ:112582793][2018/4/4]
	if (MAP->IsTownMap(map))
		gChannelNum = 0;
	if (GAMEIN->GetMoveDialog()->IsMovePos())
	{
		int MotionIdx = -1;
		int EffectIdx = -1;
		WORD Weapontype = HERO->GetWeaponEquipType();
		if (Weapontype == WP_GUM || Weapontype == WP_GWUN || Weapontype == WP_GUNG || Weapontype == WP_AMGI ||
			Weapontype == WP_EVENT || Weapontype == WP_EVENT_HAMMER)
		{
			MotionIdx = eMotion_Item_Teleport_1;
			EffectIdx = eEffect_ShopItem_Teleport1;
		}
		else if (Weapontype == WP_DO)
		{
			MotionIdx = eMotion_Item_Teleport_2;
			EffectIdx = eEffect_ShopItem_Teleport2;
		}
		else if (Weapontype == WP_CHANG)

		{
			MotionIdx = eMotion_Item_Teleport_3;
			EffectIdx = eEffect_ShopItem_Teleport3;
		}

		MOVEMGR->HeroMoveStop();
		HERO->SetItemUseMotionIdx(MotionIdx);
		HERO->SetItemUseEffectIdx(EffectIdx);
		HERO->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);
		m_MoveMap = map;// [传送] [by:十里坡剑神][QQ:112582793][2018/11/7]
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_ItemUse);
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_ItemUse, HERO->GetEngineObject()->GetAnimationTime(MotionIdx));
		m_MovePoint = GetMoveDialog()->GetSavedPoint();
	}
	else if (MARRYMGR->GetTargerPos() != 0)
	{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
		m_MoveMap = map;
		m_MovePoint = MARRYMGR->GetTargerPos();
		MARRYMGR->SetTargerPos(0);
		WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(160));
		MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);
		MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &map, 4);
	}
	else if (GUILDMGR->GetSOSTargetPos() != 0)
	{
		m_MoveMap = map;
		m_MovePoint = GUILDMGR->GetSOSTargetPos();
		GUILDMGR->SetSOSTargetPos(0);
		WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(160));
		MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);
		MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &map, 4);
	}
	else
	{
		WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(160));
		MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
		MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &map, 4);
	}
	CHATMGR->SaveChatList();
	ITEMMGR->DeleteShopItemInfo();
}
void CGameIn::UserConn_Boss_Add(SEND_MONSTER_TOTALINFO* pmsg)
{
	if (pmsg->TotalInfo.MonsterKind > 10000)
	{
		ASSERT(0);
	}

	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yCASE(CAddableInfoList::VimuInfo)
			yCASE(CAddableInfoList::StreetStall)
			yENDSWITCH

			iter.ShiftToNextData();
	}

	CBossMonster* pMon = OBJECTMGR->AddBossMonster(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo);

	if (pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pMon;
		EFFECTMGR->StartEffectProcess(eEffect_MonRegen, pMon, &set, 0, pMon->GetID());
	}
}
void CGameIn::UserConn_Monster_Add(SEND_MONSTER_TOTALINFO* pmsg)
{
	if (pmsg->TotalInfo.MonsterKind > 10000)
	{
		ASSERT(0);
	}

	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yCASE(CAddableInfoList::VimuInfo)
			yCASE(CAddableInfoList::StreetStall)
			yENDSWITCH

			iter.ShiftToNextData();

	}

	CMonster* pMon = OBJECTMGR->AddMonster(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo);

	if (pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pMon;
		EFFECTMGR->StartEffectProcess(eEffect_MonRegen, pMon, &set, 0, pMon->GetID());
	}
}
void CGameIn::UserConn_Pet_Add(SEND_PET_TOTALINFO* pmsg)
{
	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yCASE(CAddableInfoList::VimuInfo)
			yCASE(CAddableInfoList::StreetStall)
			yENDSWITCH

			iter.ShiftToNextData();
	}

	CPet* pPet = OBJECTMGR->AddPet(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, pmsg->MasterName);

	pPet->SetMaster(NULL);
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->MasterID);
	if (pPlayer)
	{
		pPlayer->SetPet(pPet);
		pPet->SetMaster(pPlayer);

		if (HEROID == pmsg->MasterID)
		{
			PETMGR->SetCurSummonPet(pPet);

			PETMGR->SetCurSummonPetKind(pPet);
			if (PETMGR->CheckCurSummonPetKindIs(ePK_EventPet))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1482));
			}
			else
			{
				PETMGR->OpenPetStateDlg();
			}

			PETMGR->SetPetStateDlgInfo(pPet);
			PETMGR->SetPetStateMiniDlgInfo(pPet);
			PETMGR->SetPetStateDlgUseRestInfo(pPet);
			PETMGR->SetCurPetRest(pmsg->TotalInfo.bRest);
			PETMGR->SetPetStateDlgUseRestInfo(pPet);

			PETMGR->RefleshPetMaintainBuff();
		}
	}
	else
	{

	}

	pPet->SetMasterID(pmsg->MasterID);
}
void CGameIn::UserConn_Char_Add(SEND_CHARACTER_TOTALINFO* pmsg)
{

#ifndef _RESOURCE_WORK_NOT_APPLY_
	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&pmsg->MoveInfo);

	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yENDSWITCH

			iter.ShiftToNextData();
	}

	CPlayer* pPlayer = OBJECTMGR->AddPlayerThroughCache(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, &pmsg->ShopItemOption, pmsg);

	VECTOR3 temp;
	temp.x = pmsg->MoveInfo.Move_Direction.x;
	temp.z = pmsg->MoveInfo.Move_Direction.y;
	float fDirDeg = RADTODEG(VECTORTORAD(temp));
	MOVEMGR->SetAngle(pPlayer, fDirDeg, 0);

	EVENTMAP->AddPlayer(pPlayer);

	CPet* pPet = OBJECTMGR->FindPlayerOwnPet(pPlayer->GetID());
	if (pPet)
	{
		pPlayer->SetPet(pPet);
		pPet->SetMaster(pPlayer);
	}
#else

	SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
	BASEMOVE_INFO moveInfo;
	BOOL bVimu = FALSE;
	BOOL bPartyWar = FALSE;
	WORD wPartyTeam = 0;
	WORD StallKind = 0;
	COMPRESSEDPOS VimuPos;
	static char StreetStallTitle[MAX_STREETSTALL_TITLELEN + 1] = { 0, };
	BOOL bInTitan = FALSE;
	TITAN_APPEARANCEINFO titanAppearance;

	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yCASE(CAddableInfoList::VimuInfo)
			bVimu = TRUE;
		iter.GetInfoData(&VimuPos);
		yCASE(CAddableInfoList::StreetStall)
			StallKind = eSK_SELL;
		iter.GetInfoData(StreetStallTitle);
		yCASE(CAddableInfoList::StreetBuyStall)
			StallKind = eSK_BUY;
		iter.GetInfoData(StreetStallTitle);
		yCASE(CAddableInfoList::PartyWar)
			bPartyWar = TRUE;
		iter.GetInfoData(&wPartyTeam);
		yCASE(CAddableInfoList::TitanAppearanceInfo)
			bInTitan = TRUE;
		iter.GetInfoData(&titanAppearance);

		yENDSWITCH

			iter.ShiftToNextData();
	}

	if (bInTitan = pmsg->bInTitan)
	{
		pPlayer->RidingTitan(bInTitan);
		pPlayer->SetTitanAppearanceInfo(&titanAppearance);
		APPEARANCEMGR->InitAppearance(pPlayer);
	}

	CPlayer* pPlayer = OBJECTMGR->AddPlayer(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, &pmsg->ShopItemOption, &titanAppearance, bInTitan);

	if (pmsg->TotalInfo.bPKMode)
	{
		if (pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
			pPlayer->AddObjectEffect(PK_EFFECT_ID, &desc, 1);
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
			pPlayer->AddObjectEffect(PK_EFFECT_ID, &desc, 1);
		}
	}

	if ((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
		|| (!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
	{
		if (pmsg->TotalInfo.bMussangMode)
		{
			switch (pPlayer->GetStage())
			{
			case eStage_Normal:
			{
				OBJECTEFFECTDESC desc(1035);
				pPlayer->AddObjectEffect(MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
			}
				break;
			case eStage_Hwa:
			case eStage_Hyun:
			{
				OBJECTEFFECTDESC desc(888);
				pPlayer->AddObjectEffect(MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
			}
				break;
			case eStage_Geuk:
			case eStage_Tal:
			{
				OBJECTEFFECTDESC desc(889);
				pPlayer->AddObjectEffect(MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
			}
				break;
			}
		}
	}


	if (pmsg->bLogin)
	{
		TARGETSET set;
		set.pTarget = pPlayer;
		EFFECTMGR->StartEffectProcess(eEffect_NewCharacter, pPlayer, &set, 0, pPlayer->GetID());

		pPlayer->ChangeBaseMotion(pPlayer->GetStandardMotion());
	}
	else
	{
		if (pmsg->BaseObjectInfo.ObjectState == eObjectState_StreetStall_Owner)
		{
			if (StallKind == eSK_SELL)
				pPlayer->ShowStreetStallTitle(TRUE, StreetStallTitle);
			else if (StallKind == eSK_BUY)
				pPlayer->ShowStreetBuyStallTitle(TRUE, StreetStallTitle);

		}
	}

	if (bVimu)
	{
		if (pmsg->BaseObjectInfo.BattleTeam == eBattleTeam1)
		{
			if (pPlayer->InTitan())
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
		}
		else if (pmsg->BaseObjectInfo.BattleTeam == eBattleTeam2)
		{
			if (pPlayer->InTitan())
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
		}
	}

	if (bPartyWar)
	{
		if (wPartyTeam == 1)
		{
			if (pPlayer->InTitan())
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
		}
		else if (wPartyTeam == 2)
		{
			if (pPlayer->InTitan())
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
				pPlayer->AddObjectEffect(BATTLE_TEAMEFFECT_ID, &desc, 1, HERO);
			}
		}

	}
	EVENTMAP->AddPlayer(pPlayer);
#endif
}
void CGameIn::UserConn_GameIn_Ack(SEND_HERO_TOTALINFO * pmsg)
{
	if (!REDISCONNECT->IsInReCnnMode())
		ASSERT(!m_bGameInAcked);
	if (m_bDisconnected)
		m_bDisconnected = FALSE;
	REDISCONNECT->SendOpenSafeLock();
	GetShoutchatDlg()->SetActive(FALSE);

	ASSERT(!(pmsg->SendMoveInfo.CurPos.wx == 0 && pmsg->SendMoveInfo.CurPos.wz == 0));

	SetLoginTime(pmsg->ServerTime);

	SetClientLoginTime(MHTIMEMGR->GetMHDate(), MHTIMEMGR->GetMHTime());

	TITANMGR->SetServerLogInTime();

	OBJECTMGR->RegisterHero(pmsg);

	GetMiniMapDialog()->AddHeroIcon(HERO);

	EFFECTMGR->StartHeroEffectProcess(eEffect_NewCharacter);

	OPTIONMGR->ApplySettings();
	OPTIONMGR->SendOptionMsg();

	FRIENDMGR->LogInNotify();
	//[夫妻上线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	MARRYMGR->LoginNotifyForMarry();

	VECTOR2 sidpos;
	sidpos.x = 152;//[buff图标渲染位置][By:十里坡剑神][QQ:112582793][2018/3/10]
	sidpos.y = 0;
	STATUSICONDLG->Init(HERO, &sidpos, 9);

	GAMEEVENTMGR->AddEvent(eGameEvent_MapChange, MAP->GetMapNum());

	if (HERO->GetLevel() > 4 && !MUGONGMGR->GetMugongNum())
	{
		GAMEEVENTMGR->AddEvent(eGameEvent_LevelUp, 6);
	}

	QUESTMGR->ProcessNpcMark();

	EVENTMAP->AddHero();

	if (GetEventNotifyDialog())
	{
		if (NOTIFYMGR->IsEventNotifyUse() && (NOTIFYMGR->IsEventNotifyChanged() || GetEventNotifyDialog()->IsActive()))
		{
			GetEventNotifyDialog()->SetTitle(NOTIFYMGR->GetEventNotifyTitle());
			GetEventNotifyDialog()->SetContext(NOTIFYMGR->GetEventNotifyContext());
			GetEventNotifyDialog()->SetActive(TRUE);
			NOTIFYMGR->SetEventNotifyChanged(FALSE);
		}
		else
		{
			GetEventNotifyDialog()->SetActive(FALSE);
		}
	}

	ITEMMGR->RefreshStatsBuffIcon();

	if (HERO->InTitan())
	{
		CAMERA->InitCamera(0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
			1500, HERO, TRUE);
		MAP->ToggleSunPosForShadow(TRUE);
	}
	else
	{
		CAMERA->Init(HERO, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(), 1000);
		MAP->ToggleSunPosForShadow(FALSE);
	}

	CAMERA->SetTargetAngleY(0, 0.f, 0);
	CAMERA->SetTargetAngleX(0, 29.9f, 0);

	SetMoveMap(MAP->GetMapNum());

	NPCSCRIPTMGR->SetSelectedNpc(NULL);
	sCPlayerAI->ReadConfig();
	sCPlayerAI->SetChangeMap(FALSE);
	GetShoutchatDlg()->SetActive(TRUE);

	if (!OPTIONMGR->GetGameOption()->bNoTipMsg)
	{
		CHATMGR->AddMsg(CTC_TIP_GG, CHATMGR->GetChatMsg(2154));
		CHATMGR->AddMsg(CTC_TIP_GG, CHATMGR->GetChatMsg(2155));
		CHATMGR->AddMsg(CTC_TIP_GG, CHATMGR->GetChatMsg(2156));
		CHATMGR->AddMsg(CTC_TIP_GG, CHATMGR->GetChatMsg(2157));
	}
	if (bSiegeWarMasterID)
	{//[洛阳城主上线提示][By:十里坡剑神][QQ:112582793][2018/2/23]
		if (HEROID == bSiegeWarMasterChar)
		{
			MSG_TIP msg;
			msg.Category = MP_CLIENT;
			msg.Protocol = MP_CLIENT_MSG_AGENT_SYN;
			SafeStrCpy(msg.Name1, HERO->GetObjectName(), MAX_NAME_LENGTH + 1);
			NETWORK->Send(&msg, sizeof(msg));
			bSiegeWarMasterID = FALSE;
		}
	}
	//[排行标志设置][By:十里坡剑神][QQ:112582793][2018/2/25]
	sBOARDMGR->SetHeroTopListFlg((CPlayer*)HERO);
	sCPlayerAI->DufualtUsekyungGong();
	if (REDISCONNECT->IsInReCnnMode())
		REDISCONNECT->StopReCnnMode();
	if (!g_SafeLockIsOpen)
		GetSafeLockDlg()->Show(FALSE);
	sGAMECHECK->InitGameCheck();
	FILECHECK->SendCheckResult();
	if (MAP->IsMapKind(eUnionWarMap))
	{
		if (GetUnionWarInfoDlg())
		{
			if (!GetUnionWarInfoDlg()->IsActive())
			{
				GetUnionWarInfoDlg()->SetActive(TRUE);
			}
		}
	}
}
void CGameIn::UserConn_Gridinit()
{
	m_bGameInAcked = TRUE;

	g_UserInput.GetKeyboard()->ClearKeyState();
	g_UserInput.GetKeyboard()->UpdateKeyboardState();
	g_UserInput.SetInputFocus(TRUE);
	USERINFOMGR->LoadUserInfo(eUIK_USERSTATE);
}
void CGameIn::UserConn_NPC_Add(SEND_NPC_TOTALINFO* pmsg)
{
	BASEMOVE_INFO moveInfo;
	moveInfo.SetFrom(&pmsg->MoveInfo);
	CAddableInfoIterator iter(&pmsg->AddableInfo);
	BYTE AddInfoKind;
	while ((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
	{
		ySWITCH(AddInfoKind)
			yCASE(CAddableInfoList::MoveInfo)
			COMPRESSEDTARGETPOS tpos;
		iter.GetInfoData(&tpos);
		moveInfo.bMoving = TRUE;
		moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
		ASSERT(tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE);
		moveInfo.SetCurTargetPosIdx(0);
		for (int n = 0; n < tpos.PosNum; ++n)
			tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
		yCASE(CAddableInfoList::VimuInfo)
			yCASE(CAddableInfoList::StreetStall)
			yENDSWITCH

			iter.ShiftToNextData();
	}

	CNpc* pNpc = OBJECTMGR->AddNpc(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo);
	MOVEMGR->SetAngle(pNpc, pmsg->Angle, 0);

	QUESTMGR->SetNpcData(pNpc);
}
void CGameIn::UserConn_Char_Revive(MOVE_POS* pmsg)
{
	VECTOR3 pos;
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
	if (pObject)
	{
		pmsg->cpos.Decompress(&pos);
		OBJECTACTIONMGR->Revive(pObject, &pos);

		if (pObject == HERO)
		{
			GAMEIN->GetReviveDialog()->SetDisable(FALSE);
			GAMEIN->GetReviveDialog()->SetActive(FALSE);

			cDialog* pDlg = WINDOWMGR->GetWindowForID(MBI_REVIVECONFIRM);
			if (pDlg)
				WINDOWMGR->AddListDestroyWindow(pDlg);

			m_bDieInBossMap = FALSE;
		}
	}
}
void CGameIn::UserConn_Ready_Revive()
{
	if (MAP->IsMapKind(eEventMap))
		return;
	GAMEIN->GetReviveDialog()->SetActive(TRUE);

	if (GAMEIN->GetPyogukDialog()->IsActive())
		GAMEIN->GetPyogukDialog()->SetActive(FALSE);

	if (MAP->IsMapKind(eBossMap))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(929));

		m_bDieInBossMap = TRUE;
		m_dwTimeInBossMap = gCurTime + 50000;
		m_nCountInBossMap = 10;
	}
}
void CGameIn::UserConn_Character_Die(MSG_DWORD2* pmsg)
{
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
	if (!pObject)
		return;
	pObject->SetDieFlag();
}
void CGameIn::UserConn_Monster_Die(MSG_DWORD2* pmsg)
{
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
	if (!pObject)
		return;
	pObject->SetDieFlag();

	if (pmsg->dwData1 == HERO->GetID())
	{
		GAMEEVENTMGR->AddEvent(eGameEvent_Hunt);
	}
}
void CGameIn::UserConn_Pet_Die(MSG_DWORD2* pmsg)
{
	CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData2);
	if (!pPet)	return;

	if (pPet == PETMGR->GetCurSummonPet())
	{
		DWORD SummonItemDBIdx = pPet->GetPetSommonItemDBIdx();
		PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(SummonItemDBIdx);
		if (!pInfo)
		{
			ASSERT(0);
			return;
		}
		pInfo->bAlive = FALSE;
		pInfo->PetFriendly = 0;

		PETMGR->SetPetGuageText(pInfo->PetStamina, pInfo->PetFriendly);
		ITEMMGR->RefreshItemToolTip(SummonItemDBIdx);
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1261));

		DWORD RedFilter = RGBA_MAKE(255, 10, 10, 255);
		ITEMMGR->SetIconColorFilter(pInfo->PetSummonItemDBIdx, RedFilter);

		PETMGR->ClosePetAllDlg();
	}
	pPet->Die();
}
void CGameIn::UserConn_Object_Remove(MSG_DWORD* pmsg)
{
	CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
	if (pObject == NULL)
	{
		return;
	}

	if (pObject->GetObjectKind() == eObjectKind_Pet)
	{
		PETMGR->OnPetRemove((CPet*)pObject);
	}

	if (pObject->GetObjectKind() == eObjectKind_Player)
	{
		((CPlayer*)pObject)->SetPet(NULL);
		OBJECTMGR->RemoveDecoration((CPlayer*)pObject);
	}
#ifndef _RESOURCE_WORK_NOT_APPLY_
	APPEARANCEMGR->CancelAlphaProcess(pObject);
	APPEARANCEMGR->CancelReservation(pObject);
#endif
	OBJECTMGR->AddGarbageObject(pObject);
}
