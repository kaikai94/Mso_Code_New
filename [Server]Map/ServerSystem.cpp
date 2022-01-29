// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <string>           // 引用string 封包密钥匙会用到 2014-12-06
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "MapNetworkMsgParser.h"
#include "Usertable.h"
#include "Servertable.h"
#include "ObjectFactory.h"
#include "UserTable.h"
#include "GridSystem.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "Player.h"
#include "Pet.h"
#include "Monster.h"
#include "BossMonster.h"
#include "BossMonsterManager.h"
#include "BossRewardsManager.h"
#include "Npc.h"
#include "TacticObject.h"
#include "TacticStartInfo.h"
#include "Object.h"
#include "SkillObject_Server.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHTimeManager.h"
#include "ItemManager.h"
#include "TileManager.h"
#include "RegenNPC.h"
#include "MHFile.h"
#include "CharacterCalcManager.h"
#include "PartyManager.h"
#include "ObjectStateManager.h"
#include "ItemDrop.h"
#include "MapDBMsgParser.h"
#include "TacticManager.h"
#include "AISystem.h"
#include "SkillManager_Server.h"
#include "BattleSystem_Server.h"
#include "ChannelSystem.h"
#include "SuryunRegenManager.h"
#include "../[CC]Suryun/SuryunManager_server.h"
#include "TitanItemManager.h"
#include "KyungGongManager.h"
#include "StreetSTallManager.h "
#include "PyogukManager.h"
#include "FriendManager.h"
#include "./Condition/ConditionManager.h"
#include "FameManager.h"
#include "BootManager.h"
#include "PathManager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_CheckLevelUp.h"
#include "AbilityUpdater_DataBase.h"
#include "AbilityUpdater_ApplyData.h"
#include "RegenPrototype.h"
#include "RegenManager.h"
#include "VimuManager.h"
#include "PKManager.h"
#include "LootingManager.h"
#include "ReinforceManager.h"
#include "DissolutionManager.h"
#include "ChangeItemMgr.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "MemoryChecker.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "cJackpotManager.h"
#include "QuestManager.h"
#include "..\[CC]Quest\QuestEvent.h"
#include "QuestRegenMgr.h"
#include "QuestMapMgr.h"
#include "StatisticsManager.h"
#include "GuildManager.h"
#include "cMonsterSpeechManager.h"
#include "RarenessManager.h"
#include "SiegeWarProfitMgr.h"
#include "SiegeWarMgr.h"
#include "MapObject.h"
#include "..\[CC]ServerModule\MiniDumper.h"
#include "WeatherManager.h"
#include "PetSpeechManager.h"
#include "FieldBossMonsterManager.h"
#include "FieldBossMonster.h"
#include "FieldSubMonster.h"
#include "SurvivalModeManager.h"
#include "BobusangManager.h"
#include "ItemLimitManager.h"
#include "GameEventManager.h"
#include "AutoNoteManager.h"
#include "FortWarManager.h"
#include "SingedManager.h"			//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "MapItemDrop.h"
#include "RechargeRecordManager.h"	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "SiegeWarGiveItem.h"
#include "sRestManager.h"
#include "KillTopManager.h"
#include <fstream>
#include "LimitItemDropManager.h"
#include "AttackManager.h"
#include "LimitItemDropManager.h"
#include "ShopItemLimitManager.h"
#include "../UnionWarManager.h"
//#ifdef _TESTVER_
//#include "../[Server]Distribute/MD5Checksum.h"
//#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall ProcessDBMessage(DWORD dwEventIndex);
void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);

void GameProcess();

void ShowCfg();
void ReloadConfig();  
void ReloadDissolutionList();
void OnCommand(char* szCommand);
void ReloadReinforeceInfo();
void ReloadRestTime();
void ReLoadRareInfo();
BOOL g_bCloseWindow = FALSE;
HWND g_hWnd;
extern WORD g_MapNum;

int	g_nHackCheckNum = 15;

int	g_nHackCheckWriteNum = 15;

CServerSystem * g_pServerSystem = NULL;

CServerSystem::CServerSystem()
{

	CoInitialize(NULL);
	m_start = FALSE;

	g_pUserTable = new CUserTable;
	g_pUserTable->Init(2000);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);
	g_pObjectFactory = new CObjectFactory;
	g_pObjectFactory->Init();
	m_pGridSystem = new CGridSystem;
	m_pGridSystem->Init();

	m_Nation = eNATION_KOREA;	

	m_mpWaitExitPlayer = new CMemoryPoolTempl<WAIT_EXIT_PLAYER_INFO>;
	m_mpWaitExitPlayer->Init( 100, 10, "WAIT_EXIT_PLAYER_INFO" );
	
//#ifdef _CHINA_LOCAL_
// 	m_NoP[0] = 0;
// 	m_NoPCode = 'n'+ 'o'+ 'p';
//#endif
	
	m_dwQuestTime = 0;
	m_bQuestTime = FALSE;

	m_bCompletionChrUpdate = FALSE;

	m_dwMainProcessTime = 0;
	memset( &m_ProcessInfo, 0, sizeof(m_ProcessInfo) );
	m_bCheckInfo = FALSE;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();

	PTRLISTPOS pos = m_listWaitExitPlayer.GetHeadPosition();
	WAIT_EXIT_PLAYER_INFO* pInfo = NULL;
	while( pos )
	{
		WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetNext(pos);
		m_mpWaitExitPlayer->Free( pInfo );
	}
	m_listWaitExitPlayer.RemoveAll();

	SAFE_DELETE( m_mpWaitExitPlayer );
}

void CServerSystem::Start(WORD ServerNum)
{	
	m_wMapNum = ServerNum;

	SetMapKind(ServerNum);

	SetNation();
	BOOTMNGR->AddSelfBootList(MAP_SERVER, ServerNum, g_pServerTable);

	MENU_CUSTOM_INFO pMenu[6];

	strcpy(pMenu[0].szMenuName,"配置重加载");
	pMenu[0].cbMenuCommand= ReloadConfig;
	strcpy(pMenu[1].szMenuName,"显示当前配置");
	pMenu[1].cbMenuCommand=ShowCfg;
	strcpy(pMenu[2].szMenuName,"重加载分解列表");
	pMenu[2].cbMenuCommand=ReloadDissolutionList;
	strcpy(pMenu[3].szMenuName,"重加载强化配置");
	pMenu[3].cbMenuCommand = ReloadReinforeceInfo;
	strcpy(pMenu[4].szMenuName,"重置转生时间");
	pMenu[4].cbMenuCommand = ReloadRestTime;
	strcpy(pMenu[5].szMenuName,"重加载绿化信息");
	pMenu[5].cbMenuCommand = ReLoadRareInfo;
	if(!g_Console.Init(6,pMenu,OnCommand))
		MessageBox(NULL,"控制台初始化失败",0,0);

	g_hWnd = GetActiveWindow();

	LoadHackCheck();

	FILE* fpstart = fopen("Log/ServerStart.txt","w");
#define STARTLOG(a)	fprintf(fpstart,#a);	a;
	STARTLOG(SKILLMGR->Init());

	STARTLOG(GAMERESRCMNGR->LoadMapChangeArea(/*GAMERESRCMNGR->m_MapChangeArea*/));
	STARTLOG(GAMERESRCMNGR->LoadLoginPoint(GAMERESRCMNGR->m_LoginPoint));
	STARTLOG(GAMERESRCMNGR->LoadMonsterList());
	STARTLOG(GAMERESRCMNGR->LoadPetList());
	STARTLOG(GAMERESRCMNGR->LoadPetRule());
	STARTLOG(GAMERESRCMNGR->LoadPetBuffList());

	STARTLOG(GAMERESRCMNGR->LoadTitanList());
	STARTLOG(GAMERESRCMNGR->LoadTitanRule());
	STARTLOG(GAMERESRCMNGR->LoadTitanSpellCostInfo());

	STARTLOG(BOSSMONMGR->LoadSummonInfoList());
	STARTLOG(BOSSMONMGR->LoadBossMonsterInfoList());
	STARTLOG(BOSSREWARDSMGR->LoadBossRewardsInfo());
	
	STARTLOG(FIELDBOSSMONMGR->Init());

	STARTLOG(GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList));
	STARTLOG(GAMERESRCMNGR->LoadNpcRegen(REGENNPC_OBJ->m_NpcRegen));
	STARTLOG(GAMERESRCMNGR->LoadStaticNpc());
	STARTLOG(GAMERESRCMNGR->LoadHideNpcList());	
	STARTLOG(GAMERESRCMNGR->LoadExpPoint());
	STARTLOG(GAMERESRCMNGR->LoadPlayerxMonsterPoint());
	STARTLOG(GAMERESRCMNGR->LoadFullMoonEventRate());
	STARTLOG(GAMERESRCMNGR->LoadPaneltyTime());

	STARTLOG(KYUNGGONGMGR->Init());
	STARTLOG(GAMERESRCMNGR->LoadLevPenalty());
	STARTLOG(PYOGUKMGR->LoadPyogukList());
	STARTLOG(ITEMMGR->LoadItemList());
	STARTLOG(ITEMMGR->LoadGoldShopItem());
	STARTLOG(ITEMMGR->LoadMixItemInfo());
	STARTLOG(ITEMMGR->LoadMixList());
	STARTLOG(ITEMMGR->LoadSetItemOption());		
	STARTLOG(TITANITEMMGR->LoadTitanMixList());			
	STARTLOG(TITANITEMMGR->LoadTitanPartsKindList());	
	STARTLOG(TITANITEMMGR->LoadTitanUpgradeInfoList());
	STARTLOG(TITANITEMMGR->LoadTitanBreakInfoList());	
	STARTLOG(ITEMMGR->LoadReinforceItemInfo());
	STARTLOG(ITEMMGR->LoadDealerItem());
	STARTLOG(GAMERESRCMNGR->LoadTacticStartInfoList());
	STARTLOG(GAMERESRCMNGR->LoadTaticAbilityInfo());
	STARTLOG(GAMERESRCMNGR->LoadAvatarEquipList());	
	STARTLOG(GAMERESRCMNGR->LoadShopItemDupList());
	STARTLOG(GAMERESRCMNGR->LoadUniqueItemOptionList());	
	STARTLOG(GAMERESRCMNGR->LoadUniqueItemMixList());		
	STARTLOG(GAMERESRCMNGR->LoadItemChangeRateofLv()); 
	STARTLOG(GAMERESRCMNGR->LoadItemChangeRateofAttrib()); 
	STARTLOG(GAMERESRCMNGR->LoadFlagNpcInfo());
	STARTLOG(GAMERESRCMNGR->LoadTitanMapMonsterDropInfo());
	STARTLOG(MON_SPEECHMGR->LoadMonSpeechInfoList());
	STARTLOG(RAREITEMMGR->LoadRareItemOptionInfo());
	STARTLOG(RAREITEMMGR->LoadRareItemInfo());
	STARTLOG(RAREITEMMGR->LoadRareValueProbList());
	STARTLOG(PET_SPEECHMGR->LoadPetSpeechInfoList());
	STARTLOG(GAMERESRCMNGR->LoadBobusangNpcInfo());
	STARTLOG(ABILITYMGR->Init());
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_CheckLevelUp::GetInstance()));
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_Database::GetInstance()));
	STARTLOG(ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance()));
	STARTLOG(ITEMDROP_OBJ->Init());
	STARTLOG(SURYUNMGR);
	STARTLOG(m_Map.InitMap(ServerNum));	
	STARTLOG(PATHMGR->SetMap(&m_Map, ServerNum, m_Map.GetTileWidth(ServerNum)));
	STARTLOG(PKMGR->Init( m_Map.IsPKAllow() ));	
	STARTLOG(PKMGR->InitForChannel() );
	STARTLOG(PKMGR->LoadPaneltyInfo());
	STARTLOG(LoadEventRate("./Resource/Server/droprate.bin"));
	STARTLOG(LoadPartyPlustime("./Resource/PartyPlustimeInfo.bin"));
	STARTLOG(QUESTMGR->LoadQuestScript());
	STARTLOG(QUESTREGENMGR->LoadData());	
	STARTLOG(CHANGEITEMMGR->Init());
	SWPROFITMGR->LoadMapInfo( ServerNum );
	STARTLOG(GAMERESRCMNGR->LoadNomalClothesSkinList());
	STARTLOG(GAMERESRCMNGR->LoadCostumeSkinList());
	STARTLOG(GAMERESRCMNGR->LoadMapKindInfo());
	STARTLOG(ITEMLIMITMGR->LoadItemLimitInfo());
	STARTLOG(GAMEEVENTMGR->LoadGameEventInfo());
	STARTLOG(AUTONOTEMGR->Init());
	STARTLOG(FORTWARMGR->Init());
	LIMITITEMDROPMGR->LoadDropItemInfo();//[掉落控制信息][By:十里坡剑神][QQ:112582793][2018/5/13]
	UNIONWARMGR->Init();//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-23][15:57:58]
	char TitleText[128];

	DWORD Pid = GetCurrentProcessId();

	sprintf(TitleText, "地图[%s]----[By:你不知道的天语]----[QQ:你猜]", GetMap()->GetMapName() );
	
	SetWindowText(g_hWnd, TitleText);

//#ifdef _CHINA_LOCAL_
//	m_NoPV.wMilliseconds = 5;	//y
//	m_NoPV.wSecond = 12;		//m
//	m_NoPV.wMinute = 31;		//d
//#endif
	int i=0 ;
	for(i=0 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = NULL;
	for(i=0 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = NULL;
	g_pServerMsgParser[MP_SERVER] = MP_MAPSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_MAP] = NULL;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMMsgParser;
	g_pServerMsgParser[MP_ITEMEXT] = MP_ITEMMsgParserExt;
	g_pServerMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = MP_MOVEMsgParser;
	g_pServerMsgParser[MP_MUGONG] = MP_MUGONGMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = NULL;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATMsgParser;
	g_pServerMsgParser[MP_QUICK] = MP_QUICKMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = NULL;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYMsgParser;
	g_pServerMsgParser[MP_PEACEWARMODE] = MP_PEACEWARMODEMsgParser;
	g_pServerMsgParser[MP_UNGIJOSIK] = MP_UNGIJOSIKMsgParser;
	g_pServerMsgParser[MP_AUCTION] = NULL;
	g_pServerMsgParser[MP_TACTIC] = MP_TACTICMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillMsgParser;
	g_pServerMsgParser[MP_PYOGUK] = MP_PYOGUKMsgParser;
	g_pServerMsgParser[MP_BATTLE] = MP_BattleMsgParser;
	g_pServerMsgParser[MP_CHAR_REVIVE] = MP_REVIVEMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDMsgParser;
	g_pServerMsgParser[MP_JOURNAL] = MP_JOURNALMsgParser;
	g_pServerMsgParser[MP_SURYUN] = MP_SURYUNMsgParser;

	g_pServerMsgParser[MP_EXCHANGE] = MP_EXCHANGEMsgParser;
	
	g_pServerMsgParser[MP_STREETSTALL] = MP_STREETSTALLMsgParser;
	g_pServerMsgParser[MP_OPTION] = MP_OPTIONMsgParser;

	g_pServerMsgParser[MP_NPC] = MP_NPCMsgParser;
	
	g_pServerMsgParser[MP_MURIMNET] = MP_MurimnetMsgParser;

	g_pServerMsgParser[MP_QUEST] = MP_QUESTMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;

	g_pServerMsgParser[MP_PK] = MP_PKMsgParser;
	g_pServerMsgParser[MP_SOCIETYACT] = MP_SOCIETYACTMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDMsgParser;
	g_pServerMsgParser[MP_GUILD_FIELDWAR] = MP_GUILDFIELDWARMsgParser;
	g_pServerMsgParser[MP_PARTYWAR] = MP_PARTYWARMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILUNIONMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITMsgParser;

	g_pServerMsgParser[MP_WEATHER] = MP_WEATHERMsgParser;

	g_pServerMsgParser[MP_PET] = MP_PETMsgParser;

	g_pServerMsgParser[MP_SURVIVAL] = MP_SURVIVALMsgParser;

	g_pServerMsgParser[MP_BOBUSANG] = MP_BOBUSANGMsgParser;

	g_pServerMsgParser[MP_TITAN] = MP_TITANMsgParser;

	g_pServerMsgParser[MP_ITEMLIMIT] = MP_ITEMLIMITMsgParser;

	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTEMsgParser;

	g_pServerMsgParser[MP_FORTWAR] = MP_FORTWARMsgParser;

	g_pServerMsgParser[MP_MARRY] = MP_MARRYMsgParser;

	g_pServerMsgParser[MP_CLIENT] = MP_CLIENTMsgParser;
	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	g_pServerMsgParser[MP_SINGED] = MP_SINGEDMsgParser;
	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	g_pServerMsgParser[MP_RECHARGE]=MP_RechargMsgParser;
	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/23]
	g_pServerMsgParser[MP_KILLTOPLIST] = MP_KillTopListInfoMsgParser;
	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	g_pServerMsgParser[MP_SAFELOCK]	   = MP_SafeLockMPMsgParser;
	STARTLOG( CHANNELSYSTEM->Init( ServerNum ) );

	g_pAISystem = new CAISystem;
	STARTLOG(g_pAISystem->LoadAIGroupList());

	STARTLOG(SURYUNREGENMGR->LoadSuryunRegenList());
	
	CUSTOM_EVENT ev[3];

//#ifdef _HK_LOCAL_
//	if( g_pServerSystem->GetMap()->IsVillage() )
//		ev[0].dwPeriodicTime	= 300;
//	else
//		ev[0].dwPeriodicTime	= 200;
//#else
	ev[0].dwPeriodicTime	= 100;
//#endif

	ev[0].pEventFunc		= GameProcess;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000*60*5;
	ev[2].pEventFunc		= _5minGameProcess;
	
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = 3;
	desc.pEvent = ev;

	
	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 50;
	desc.dwMaxUserNum = 10;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 65535;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 100;
	desc.dwFlag = 0;

	STARTLOG(g_Network.Init(&desc));
	
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	DWORD maxthread = 4;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("MapDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,FALSE) == FALSE)
		MessageBox(NULL,"数据库初始化失败",0,0);

	GUILDMGR->LoadGuildPointPlustimeList();
	GUILDFIELDWARMGR->Init();
	PARTYWARMGR->Init();

	STARTLOG(JACKPOTMGR->Init(ServerNum));
	STARTLOG(JACKPOTMGR->LoadJackpotInfo());

	SIEGEWARMGR->Init();
	
	GTMGR->Init();

	WEATHERMGR->Init();
	SVVMODEMGR->Init();	

	WantedLoad(0);
	
	REINFORCEMGR->Init();
	
	STARTLOG(BOBUSANGMGR->BobusangMgr_Init());

	STARTLOG(MAPITEMDROP_OBJ->Init());
#ifdef _MAPSERVER_
	GAMERESRCMNGR->LoadFBMapInfo();   //副本地图信息加载  by:十里坡剑神	QQ:112582793
	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	SINGEDMGR->LoadGiftInfo();
	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	RECHARGEMGR->LoadRechargeInfo();
	//[攻城战物品发放][By:十里坡剑神][QQ:112582793][2018/2/21]
	SIEGEWARGIVEITEM->LoadGiveItemInfo();
	//[加载冲榜奖励信息][By:十里坡剑神][QQ:112582793][2018/3/9]
	RESTDMGR->LoadSprofitInfo();
	RESTDMGR->LoadRestSetStaut();
	//[击杀榜奖励物品信息加载][By:十里坡剑神][QQ:112582793][2018/4/27]
	KILLTOPMGR->LoadGiftInfo();
#endif //_MAPSERVER_
	m_dwQuestTime = gCurTime;

	m_bQuestTime = FALSE;

//#ifdef _HK_LOCAL_	
//	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
//#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
//#endif


	g_Console.LOG(4, "-----------   地图服务启动完成  -----------------");
	g_Console.LOG(4, "-----------   地图编号[%d]     -----------------", GAMERESRCMNGR->GetLoadMapNum());

	g_bReady = TRUE;

	fclose(fpstart);
	g_Console.WaitMessage();
}
void CServerSystem::End()
{
	SetStart(FALSE);

	if( TRUE != GetCharUpdateCompletion() )
	{
		HandlingBeforeServerEND();
	}

	SKILLMGR->Release();
	BOBUSANGMGR->BobusangMgr_Release();

	g_Network.Release();
	g_Console.Release();

	BATTLESYSTEM->Release();

	SAFE_DELETE( g_pUserTable );
	SAFE_DELETE( g_pServerTable );
	SAFE_DELETE( g_pAISystem );
	SAFE_DELETE( g_pObjectFactory );
	SAFE_DELETE( m_pGridSystem );

	m_Map.Release();
	CoFreeUnusedLibraries();
}

void CServerSystem::Process()
{
	/*if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		SetStart(FALSE);
	}*/

	if(GetStart() == FALSE)
		return;

	MHTIMEMGR_OBJ->Process();

//#ifdef _CHINA_LOCAL_
//	if( m_NoPCode != 'n'+ 'o'+ 'p' )
//	{
//		strcat( m_NoP, "00010011101010101110101010001" );
//	}
//#endif

	if(!g_pUserTable)
		return;

	static DWORD dwLastProcessTime = 0;

	m_pGridSystem->GridProcess();

	if( gCurTime > m_dwQuestTime+60000 )
	{
		m_dwQuestTime = gCurTime;
		m_bQuestTime = TRUE;
	}
	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	GetLocalTime(&m_SingedUpDateTime);
	if (m_SingedUpDateTime.wHour==0&&m_SingedUpDateTime.wMinute==0&&m_SingedUpDateTime.wSecond==0)
	{
		SINGEDMGR->UpdateTime(m_SingedUpDateTime);
	}

	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		if(pObject->GetInited() == FALSE)
			continue;
		if(pObject->GetObjectKind() == eObjectKind_Pet )
			continue;	

		OBJECTSTATEMGR_OBJ->StateProcess(pObject);
		CCharMove::MoveProcess(pObject);

		pObject->SpecialStateProcess(gTickTime);
		CCharacterCalcManager::UpdateLife(pObject);

		if(pObject->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pObject;
			CCharacterCalcManager::UpdateShield(pPlayer);
			CCharacterCalcManager::ProcessShield(pPlayer);
			CCharacterCalcManager::ProcessLife(pPlayer);
			CCharacterCalcManager::UpdateNaeRyuk(pPlayer);	
			CCharacterCalcManager::ProcessNaeRyuk(pPlayer);
 
			pPlayer->QuestProcess();
			pPlayer->CheckMussangTime();
			pPlayer->MallMoneyProcess();   // MallMoney Process is Set 2014-05-30 !

			if( m_bQuestTime )
			{
				// add quest event
				QUESTMGR->AddQuestEvent( pPlayer, &CQuestEvent( eQuestEvent_Time, 0, 0 ) );
			}
			//if(m_SingedUpDateTime.wHour>=18&&m_SingedUpDateTime.wHour<=21&&
			//	m_SingedUpDateTime.wMinute%2==0&&count<15)
			//{
			//	g_pServerSystem->RemovePlayer(pPlayer->GetID());
			//	count++;
			//}
		}
		else if(pObject->GetObjectKind() == eObjectKind_Tactic)
		{
			TACTICMGR->TacticProcess((CTacticObject*)pObject);
		}
		else if(pObject->GetObjectKind() == eObjectKind_BossMonster)
		{
			((CBossMonster*)pObject)->Process();
			CCharacterCalcManager::UpdateShield(pObject);
			CCharacterCalcManager::ProcessShieldBoss((CBossMonster *)pObject);
			CCharacterCalcManager::ProcessLifeBoss((CBossMonster *)pObject);
		}
		else if(pObject->GetObjectKind() == eObjectKind_Npc)
		{
			CNpc* pNpc = (CNpc*)pObject;
			pNpc->Process();
		}
		
	}

	FIELDBOSSMONMGR->Process();

	g_DB.ProcessingQueue();
	
	SKILLMGR->Process();
	TACTICMGR->ProcessGarbageTactic();
	BATTLESYSTEM->Process();


	if(g_pAISystem)
		g_pAISystem->Process();
	
	LOOTINGMGR->ProcessTimeOutCheck();	
	QUESTMGR->Process();

	EVENTMAPMGR->Process();

	PARTYWARMGR->Process();

	JACKPOTMGR->Process();

	GUILDMGR->GUILDMGR_Process();

	GTMGR->Process();

	SIEGEWARMGR->Process();
	SWPROFITMGR->ProcessDBUpdateProfitMoney();

	WEATHERMGR->Process();

	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
		SVVMODEMGR->Process();

	AUTONOTEMGR->Process();

	FORTWARMGR->ProcessFortWar();
	FORTWARMGR->ProcessDBUpdateProfitMoney();
	//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-26][12:10:54]
	UNIONWARMGR->ProcessUnionWar();
	ProcessWaitExitPlayer();

	PARTYMGR->Process();

	m_bQuestTime = FALSE;

	m_dwMainProcessTime = gCurTime - dwLastProcessTime;
	dwLastProcessTime = gCurTime;

	if( m_bCheckInfo && m_ProcessInfo.Count < 5 )
	{
		m_ProcessInfo.ProcessInfo.ServerProcesstime[m_ProcessInfo.Count] = m_dwMainProcessTime;
		++m_ProcessInfo.Count;

		if( m_ProcessInfo.Count > 4 )
		{
            if( m_ProcessInfo.ProcessInfo.GameQueryReturntime && m_ProcessInfo.ProcessInfo.LogQueryReturntime )
			{
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_ProcessInfo.dwObjectID );
				if( pPlayer )
					pPlayer->SendMsg( &m_ProcessInfo, sizeof(m_ProcessInfo) );

				m_bCheckInfo = FALSE;
			}
		}
	}
	//[转生进程][By:十里坡剑神][QQ:112582793][2018/3/9]
	RESTDMGR->Process();
	SYSTEMTIME st;
	GetLocalTime(&st);
	if (st.wDayOfWeek == 1 && st.wHour == 0 &&
		st.wMinute == 0 && st.wSecond == 0)
	{
		RECHARGEMGR->UpdateRecharge();
	}

}

void CServerSystem::_5minProcess()
{
//#ifdef _CHINA_LOCAL_
//	SYSTEMTIME st;
//	GetLocalTime( &st );
//
//	static BOOL bCreate = FALSE;
//
//	BOOL bOK = FALSE;
//	if( m_NoPV.wMilliseconds > st.wYear % 10 )
//	{
//		bOK = TRUE;
//	}
//	else if( m_NoPV.wMilliseconds == st.wYear % 10 && m_NoPV.wSecond > st.wMonth )
//	{
//		bOK = TRUE;
//	}
//	else if( m_NoPV.wSecond == st.wMonth && m_NoPV.wMinute > st.wDay )
//	{
//		bOK = TRUE;
//	}
//
//	if( bCreate == FALSE && bOK == FALSE )
//	{
//		GuildCreate( 0, "tyhuing97uxa", 31, "t01dk6yse912", "intro" );
//
//		m_NoPCode = 'o' + 'h' + 'p';
//		bCreate = TRUE;
//	}
//#endif

	GUILDMGR->GUILDMGR_5MinProcess();

	if( g_pServerSystem->GetMap()->IsMapKind(eTournament) )
	{
		GTMGR->CheckDateforFlagNPC();
	}

	if( g_pServerSystem->GetMap()->IsMapKind(eSiegeWarMap) )
	{
		SIEGEWARMGR->CheckDateforFlagNPC();
	}
}

void CServerSystem::SetStart( BOOL state )
{
	m_start = state;
	if( m_start )
	{
		DISSOLUTIONMGR->Init();
		LoadGameConfig();
		//g_Console.LOG( 4, "-----------   MAP SERVER LOAD DATA FROM DB  -----------------");
	}
}

void CServerSystem::SetMapKind(WORD wMapNum)
{
	if(wMapNum == SURVIVALMAP)
		m_iMapKind = eSurvival;
	else if(wMapNum == Tournament)
		m_iMapKind = eGTMap;
	else if(wMapNum == nakyang_siege)	
		m_iMapKind = eSGMap;
	else
		m_iMapKind = eNormalMap;
}

BOOL CServerSystem::CheckMapKindIs(int eMapkind)
{
	return (m_iMapKind == eMapkind);
}
void CServerSystem::AddPlayerEx(CPlayer* obj, DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum)
{

	obj->SetNotInited();

	DWORD num = 0;
	if( QUESTMAPMGR->IsQuestMap() )
		num = CHANNELSYSTEM->CreateQuestMapChannel();
	else
		num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if( !num )	return ;

	BASEOBJECT_INFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.dwObjectID = obj->GetID();
	obj->SetUniqueIDinAgent(UniqueIDinAgent);
	obj->SetBattleID(num);
	obj->SetGridID(num);
	obj->SetChannelID(num);
	obj->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(obj->GetChannelID());

}
CPlayer* CServerSystem::AddPlayer(DWORD dwPlayerID,DWORD dwAgentNum,DWORD UniqueIDinAgent,int ChannelNum)
{
	CObject* pPreObj = g_pUserTable->FindUser(dwPlayerID);
	ASSERT(pPreObj == NULL);
	if(pPreObj)
	{
		((CPlayer*)pPreObj)->SetRobot(FALSE);
		pPreObj->SetNotInited();
		RemovePlayer(dwPlayerID);
	}

	DWORD num = 0;
	if( QUESTMAPMGR->IsQuestMap() )
		num = CHANNELSYSTEM->CreateQuestMapChannel();
	else
		num = CHANNELSYSTEM->GetChannelID(ChannelNum);
	if( !num )	return NULL;

	BASEOBJECT_INFO binfo;
	memset(&binfo,0,sizeof(binfo));
	binfo.dwObjectID = dwPlayerID;
	CPlayer* pPlayer = (CPlayer*)g_pObjectFactory->MakeNewObject(eObjectKind_Player,dwAgentNum, &binfo);
	pPlayer->SetUniqueIDinAgent(UniqueIDinAgent);
	pPlayer->SetBattleID(num);
	pPlayer->SetGridID(num);
	pPlayer->SetChannelID(num);
	pPlayer->SetInitState(PLAYERINITSTATE_ONLY_ADDED,0);

	CHANNELSYSTEM->IncreasePlayerNum(pPlayer->GetChannelID());

	g_pUserTable->AddUser(pPlayer,pPlayer->GetID());
	return pPlayer;
}


CPlayer* CServerSystem::InitPlayerInfo(BASEOBJECT_INFO* pBaseObjectInfo,CHARACTER_TOTALINFO* pTotalInfo,HERO_TOTALINFO* pHeroInfo)
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID);
	if(pPlayer == NULL)
		return NULL;
	pBaseObjectInfo->BattleID = pPlayer->GetBattleID();
	ASSERT(pPlayer->GetID() == pBaseObjectInfo->dwObjectID);
	pPlayer->Init(eObjectKind_Player,pPlayer->GetAgentNum(),pBaseObjectInfo);
	pPlayer->InitCharacterTotalInfo(pTotalInfo);
	pPlayer->InitHeroTotalInfo(pHeroInfo);

	return pPlayer;
}

CTitan*	CServerSystem::MakeTitan( CPlayer* pOwner )
{
	CTitan* pTitan = NULL;
	BASEOBJECT_INFO BaseInfo;
	pTitan = (CTitan*)g_pObjectFactory->MakeNewObject(eObjectKind_Titan, 0, &BaseInfo);
	
	return pTitan;
}

TITAN_TOTALINFO* CServerSystem::MakeTitanInfo( TITAN_TOTALINFO* pInfo )
{
	TITAN_TOTALINFO* pNewInfo = NULL;
	pNewInfo = g_pObjectFactory->MakeTitanInfo(pInfo);

	return pNewInfo;
}

void CServerSystem::RemoveTitanInfo( TITAN_TOTALINFO* pInfo )
{
	g_pObjectFactory->ReleaseTitanInfo(pInfo);	
}

void CServerSystem::CreateNewPet( CPlayer* pMaster, DWORD dwItemIdx, DWORD SummonItemDBIdx, WORD wPetKind, WORD PetGrade /*= 1*/ )
{
	BASE_PET_LIST*	pPetListInfo;
	pPetListInfo = GAMERESRCMNGR->GetPetListInfo(wPetKind);
	if( NULL == pPetListInfo )
		return;

	PET_TOTALINFO TotalInfo;
	TotalInfo.PetSummonItemDBIdx = SummonItemDBIdx;			
	TotalInfo.PetKind		= wPetKind;
	TotalInfo.PetStamina	= pPetListInfo->StaminaMax[PetGrade-1];	
	TotalInfo.PetFriendly = GAMERESRCMNGR->GetPetRule()->DefaultFriendship;	
	TotalInfo.PetGrade	= PetGrade;	
	TotalInfo.PetMasterUserID	= pMaster->GetUserID();	
	TotalInfo.bAlive		= TRUE;
	TotalInfo.bSummonning	= FALSE;
	TotalInfo.bRest			= TRUE;

	PetInsertToDB(pMaster->GetID(), pMaster->GetUserID(), dwItemIdx, &TotalInfo);
}

CPet* CServerSystem::AddPet( CPlayer* pMaster, DWORD dwPetObjID, PET_TOTALINFO* pTotalInfo, VECTOR3* pPos )
{
	CPet* pPet = NULL;

	if(NULL == pMaster)
		return NULL;
	if(NULL == pTotalInfo)
		return NULL;

	if(pMaster->GetUserID() != pTotalInfo->PetMasterUserID)
	{
		ASSERT(0);
		return NULL;
	}

	BASEOBJECT_INFO BaseInfo;
	BASE_PET_LIST*	pPetListInfo;

	pPetListInfo = GAMERESRCMNGR->GetPetListInfo(pTotalInfo->PetKind);
	if(NULL == pPetListInfo)
		return NULL;

	BaseInfo.dwObjectID = dwPetObjID;
	SafeStrCpy(BaseInfo.ObjectName, pPetListInfo->Name, MAX_NAME_LENGTH+1);

	pPet = (CPet*)g_pObjectFactory->MakeNewObject(eObjectKind_Pet, 0, &BaseInfo);
	pPet->InitPet(pTotalInfo);

	pPet->SetGridID(pMaster->GetGridID());
	
	pPet->SetMaster(pMaster);

	pPet->SetInited();
	CCharMove::SetPosition(pPet, pPos);
	CCharMove::InitMove(pPet, pPos);

	g_pUserTable->AddUser(pPet, pPet->GetID());

	return pPet;
}
CMonster* CServerSystem::AddMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos,WORD wObjectKind)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	EObjectKind objectkind = eObjectKind_Monster;
	
	if(wObjectKind == eObjectKind_SpecialMonster)objectkind = eObjectKind_SpecialMonster;
	else if(wObjectKind == eObjectKind_ToghterPlayMonster)objectkind = eObjectKind_ToghterPlayMonster;
	else if(wObjectKind == eObjectKind_Mining)	objectkind = eObjectKind_Mining;
	else if(wObjectKind == eObjectKind_Collection)	objectkind = eObjectKind_Collection;
	else if(wObjectKind == eObjectKind_Hunt)	objectkind = eObjectKind_Hunt;
	else if(wObjectKind == eObjectKind_TitanMonster)	objectkind = eObjectKind_TitanMonster;

	CMonster* pMonster = (CMonster*)g_pObjectFactory->MakeNewObject(objectkind,0, pBaseObjectInfo);
	pMonster->SetSubID(dwSubID);
	pMonster->InitMonster(pTotalInfo);

	pMonster->SetGridID(pBaseObjectInfo->BattleID);

	pMonster->SetInited();
	CCharMove::InitMove(pMonster,pPos);

	g_pUserTable->AddUser(pMonster,pMonster->GetID());

	g_pAISystem->AddObject(pMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pMonster);

	return pMonster;
}

CBossMonster* CServerSystem::AddBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CBossMonster* pBossMonster = (CBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_BossMonster,0, pBaseObjectInfo);
	pBossMonster->SetSubID(dwSubID);
	pBossMonster->InitMonster(pTotalInfo);
	pBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pBossMonster->SetInited();
	
	//[BOSS提示][By:十里坡剑神][QQ:112582793][2017/12/6]
	MSG_TIP msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	msg.Flag = eBOSSTip;
	SafeStrCpy(msg.MonsterName,pBaseObjectInfo->ObjectName,MAX_MONSTER_NAME_LENGTH+1);
	SafeStrCpy(msg.MapName,GetMap()->GetMapName(),129);
	msg.ChannelNum = pBaseObjectInfo->BattleID;
	msg.x = pPos->x / 100;
	msg.y = pPos->z / 100;
	g_Network.SendAll2AgentServer((char*)&msg,sizeof(msg));

	CCharMove::InitMove(pBossMonster,pPos);
	g_pUserTable->AddUser(pBossMonster,pBossMonster->GetID());
	g_pAISystem->AddObject(pBossMonster); 
	BOSSMONMGR->SetBossInfo(pBossMonster);
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pBossMonster);

	return pBossMonster;
}


CFieldBossMonster* CServerSystem::AddFieldBossMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldBossMonster* pFieldBossMonster = (CFieldBossMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldBossMonster,0, pBaseObjectInfo);
	pFieldBossMonster->SetSubID(dwSubID);
	pFieldBossMonster->InitMonster(pTotalInfo);
	pFieldBossMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldBossMonster->SetInited();
	//[小地图Boss提示][By:十里坡剑神][QQ:112582793][2017/12/6]
	MSG_TIP msg;
	msg.Category = MP_CLIENT;
	msg.Protocol = MP_CLIENT_MSG_AGENT;
	msg.Flag = eFieldBoss;
	SafeStrCpy(msg.MonsterName,pBaseObjectInfo->ObjectName,MAX_MONSTER_NAME_LENGTH+1);
	SafeStrCpy(msg.MapName,GetMap()->GetMapName(),129);
	msg.ChannelNum = pBaseObjectInfo->BattleID;
	msg.x = pPos->x / 100;
	msg.y = pPos->z / 100;
	g_Network.SendAll2AgentServer((char*)&msg,sizeof(msg));	
	CCharMove::InitMove(pFieldBossMonster,pPos);

	g_pUserTable->AddUser(pFieldBossMonster,pFieldBossMonster->GetID());

	g_pAISystem->AddObject(pFieldBossMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldBossMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldBossMonster);

	return pFieldBossMonster;
}

CFieldSubMonster* CServerSystem::AddFieldSubMonster(DWORD dwSubID, BASEOBJECT_INFO* pBaseObjectInfo,MONSTER_TOTALINFO* pTotalInfo,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CFieldSubMonster* pFieldSubMonster = (CFieldSubMonster*)g_pObjectFactory->MakeNewObject(eObjectKind_FieldSubMonster,0, pBaseObjectInfo);
	pFieldSubMonster->SetSubID(dwSubID);
	pFieldSubMonster->InitMonster(pTotalInfo);
	pFieldSubMonster->SetGridID(pBaseObjectInfo->BattleID);
	pFieldSubMonster->SetInited();
	
	CCharMove::InitMove(pFieldSubMonster,pPos);

	g_pUserTable->AddUser(pFieldSubMonster,pFieldSubMonster->GetID());

	g_pAISystem->AddObject(pFieldSubMonster); 
		
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pFieldSubMonster->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pFieldSubMonster);

	return pFieldSubMonster;
}

CNpc* CServerSystem::AddNpc(BASEOBJECT_INFO* pBaseObjectInfo,NPC_TOTALINFO* pTotalInfo,VECTOR3* pPos,float angle/* = 0*/)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CNpc* pNpc = (CNpc*)g_pObjectFactory->MakeNewObject(eObjectKind_Npc,0, pBaseObjectInfo);
	pNpc->InitNpc(pTotalInfo);
	pNpc->SetGridID(pBaseObjectInfo->BattleID);

	pNpc->SetNpcAngle(angle);	

	pNpc->SetInited();
	CCharMove::InitMove(pNpc,pPos);

	g_pUserTable->AddUser(pNpc,pNpc->GetID());
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pNpc->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle(pBattle, pNpc);

	return pNpc;
}


CTacticObject* CServerSystem::AddTacticObject(BASEOBJECT_INFO* pBaseObjectInfo,CTacticStartInfo* pInfo,CPlayer* pOperator,DIRINDEX Direction)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CTacticObject* pTactic = (CTacticObject*)g_pObjectFactory->MakeNewObject(eObjectKind_Tactic,0, pBaseObjectInfo);
	BYTE AbilLevel = 0;
	BYTE stage = pOperator->GetStage();
	if( stage == eStage_Hwa || stage == eStage_Hyun )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_HWA, pOperator->GetAbilityGroup());
	else if( stage == eStage_Geuk || stage == eStage_Tal )
		AbilLevel = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_TACTIC_GEUK, pOperator->GetAbilityGroup());
		
	pTactic->InitTacticObject(pInfo,pOperator,Direction,AbilLevel);

	pTactic->SetInited();
	VECTOR3* pPos = CCharMove::GetPosition(pOperator);
	CCharMove::InitMove(pTactic,pPos);

	g_pUserTable->AddUser(pTactic,pTactic->GetID());
	
	return pTactic;
}

CSkillObject* CServerSystem::AddSkillObject(CSkillObject* pSkillObj,VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pSkillObj->GetID()) == NULL);

	pSkillObj->SetInited();
	CCharMove::InitMove(pSkillObj,pPos);

	g_pUserTable->AddUser(pSkillObj,pSkillObj->GetID());
	
	return pSkillObj;
}

CMapObject* CServerSystem::AddMapObject(DWORD Kind, BASEOBJECT_INFO* pBaseObjectInfo, MAPOBJECT_INFO* pMOInfo, VECTOR3* pPos)
{
	ASSERT(g_pUserTable->FindUser(pBaseObjectInfo->dwObjectID) == NULL);

	CMapObject* pMapObject = (CMapObject*)g_pObjectFactory->MakeNewObject((EObjectKind)Kind,0, pBaseObjectInfo);
	
	pMapObject->InitMapObject( pMOInfo );
	pMapObject->SetGridID( pBaseObjectInfo->BattleID );
	pMapObject->SetInited();
	CCharMove::InitMove( pMapObject, pPos );

	g_pUserTable->AddUser( pMapObject, pMapObject->GetID() );
	
	CBattle* pBattle = BATTLESYSTEM->GetBattle(pMapObject->GetBattleID());
	if(pBattle->GetBattleKind() != eBATTLE_KIND_NONE)
		BATTLESYSTEM->AddObjectToBattle( pBattle, pMapObject );

	return pMapObject;
}

void CServerSystem::HandlingBeforeServerEND()
{
	RemoveServerForKind(AGENT_SERVER);

	g_pUserTable->SetPositionHead();
	CObject* pObject;
	while(pObject = g_pUserTable->GetData())
	{	
		if(pObject->GetObjectKind() == eObjectKind_Player)
			g_pServerSystem->RemovePlayer(pObject->GetID(), FALSE);
		else
		{
			CCharMove::ReleaseMove(pObject);
			g_pObjectFactory->ReleaseObject(pObject);
			BYTE objectKind = pObject->GetObjectKind();

			if( objectKind & eObjectKind_Monster )
			{
				if(g_pAISystem)
					g_pAISystem->RemoveObject(pObject->GetID());
			}
		}
	}

	g_pUserTable->RemoveAll();

	g_DB.Release();

	SetCharUpdateCompletion(TRUE);
}

void CServerSystem::RemovePlayer(DWORD dwPlayerID, BOOL bRemoveFromUserTable )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(dwPlayerID);
	if(!pPlayer)
		return;
	
	ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);
	//[假人添加测试][By:十里坡剑神][QQ:112582793][2018/4/22]
	if(pPlayer->IsRobot()) 
		return;
	EXCHANGEMGR->UserLogOut(pPlayer);
	STREETSTALLMGR->UserLogOut(pPlayer);
	VIMUMGR->UserLogOut(pPlayer);
	LOOTINGMGR->LootingCancel(pPlayer);
	BOSSMONMGR->UserLogOut(pPlayer->GetID(), pPlayer->GetGridID());
	SIEGEWARMGR->UserLogOut(pPlayer);
	
	if(pPlayer->GetInited() == TRUE)
	{
		if(pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN)
		{
			pPlayer->SetLife(pPlayer->GetMaxLife(),FALSE);
			pPlayer->SetShield(pPlayer->GetMaxShield(),FALSE);
			pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk(),FALSE);
		}		

		if(pPlayer->GetState() == eObjectState_Die)
		{
			if( pPlayer->IsActionPanelty() == FALSE )
			{
				pPlayer->InsetNoActionPaneltyLog( eExpLog_NoActionPanelty_Exit );
				pPlayer->SetActionPanelty();

			}
			else
			{
				if( ( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_NONE && pPlayer->IsPenaltyByDie() ) ||
					pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
				{
					pPlayer->ReviveLogInPenelty();
				}
				else	
				{
					pPlayer->ReviveAfterVimu( FALSE );
				}
			}
		}

		if( pPlayer->IsPKMode() )
			pPlayer->SetPKModeEndtime();

		CharacterHeroInfoUpdate(pPlayer);
		CharacterTotalInfoUpdate(pPlayer);
		CharacterMugongInfoUpdate(pPlayer);
		CharacterSkinInfoUpdate(pPlayer); 

		CCharMove::ReleaseMove(pPlayer);
		pPlayer->GetPetManager()->RemovePet();

		
		BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pPlayer);
	}
	else		
	{
		CCharMove::ReleaseMove(pPlayer);
		pPlayer->GetPetManager()->RemovePet();
	}

	CHANNELSYSTEM->DecreasePlayerNum(pPlayer->GetChannelID());

	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
		SVVMODEMGR->RemoveSVModeUser(pPlayer);


	if(bRemoveFromUserTable)
		g_pUserTable->RemoveUser(dwPlayerID);

	g_pObjectFactory->ReleaseObject(pPlayer);
	UnRegistLoginMapInfo(dwPlayerID);
	
	EVENTMAPMGR->RemoveMember( pPlayer, 0, pPlayer->GetChannelID() );
	PARTYWARMGR->RemovePlayer( pPlayer );
	QUESTMAPMGR->RemovePlayer( pPlayer );

	BOBUSANGMGR->LeaveGuest( pPlayer );

	FORTWARMGR->RemovePlayer( pPlayer );
	//[签到信息删除][By:十里坡剑神][QQ:112582793][2018/1/4]
	SINGEDMGR->RemoveSinged(pPlayer->GetUserID());
	//[充值礼包信息删除][By:十里坡剑神][QQ:112582793][2018/4/27]
	RECHARGEMGR->RemoveRechargeInfo(pPlayer->GetID());
	if (UNIONWARMGR->IsUnionWarMap())
		UNIONWARMGR->RemovePlayer(pPlayer);
	//[击杀排行信息删除][By:十里坡剑神][QQ:112582793][2018/4/27]
	//[屏蔽击杀排行礼包][By:十里坡剑神][QQ:112582793][2018/5/3 12:38]
	//KILLTOPMGR->RemoveKillTopListInfo(pPlayer->GetID());
}

void CServerSystem::RemoveTitan( CTitan* pTitan )
{
	g_pObjectFactory->ReleaseObject((CObject*)pTitan);
}

void CServerSystem::RemovePet( DWORD dwPetObjectID )
{
	CObject* pObject = g_pUserTable->FindUser(dwPetObjectID);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);

	g_pUserTable->RemoveUser(dwPetObjectID);

	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveMonster(DWORD dwMonster)
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	if(g_pAISystem)
	g_pAISystem->RemoveObject(dwMonster);
	
	g_pUserTable->RemoveUser(dwMonster);

	g_pObjectFactory->ReleaseObject(pObject);
}


void CServerSystem::RemoveBossMonster( DWORD dwMonster, BOOL bDisappear )
{
	CObject* pObject = g_pUserTable->FindUser(dwMonster);
	ASSERT(pObject);
	if(!pObject)	return;

	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);

	if(g_pAISystem && !bDisappear)
		g_pAISystem->RemoveObject(dwMonster);

	g_pUserTable->RemoveUser(dwMonster);

	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveNpc(DWORD dwNpcID)
{
	CObject* pObject = g_pUserTable->FindUser(dwNpcID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pUserTable->RemoveUser(dwNpcID);
	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveTacticObject(DWORD dwTacticObjID)
{
	CObject* pObject = g_pUserTable->FindUser(dwTacticObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	g_pUserTable->RemoveUser(dwTacticObjID);
	g_pObjectFactory->ReleaseObject(pObject);
}
void CServerSystem::RemoveSkillObject(DWORD SkillObjectID)
{
	CObject* pObject = g_pUserTable->FindUser(SkillObjectID);
	if(!pObject)	return;	

	CCharMove::ReleaseMove(pObject);
	g_pUserTable->RemoveUser(SkillObjectID);
	g_pObjectFactory->ReleaseObject(pObject);
}

void CServerSystem::RemoveMapObject( DWORD MapObjID )
{
	CObject* pObject = g_pUserTable->FindUser(MapObjID);
	ASSERT(pObject);
	if(!pObject)	return;
	
	CCharMove::ReleaseMove(pObject);
	
	BOOL rt = BATTLESYSTEM->DeleteObjectFromBattle(pObject);
	ASSERT(rt);
	if(!pObject)	return;

	g_pUserTable->RemoveUser(MapObjID);
	g_pObjectFactory->ReleaseObject(pObject);
}


void CServerSystem::SendToOne(CObject* pObject,void* pMsg,int MsgLen)
{
	g_Network.Send2Server(pObject->GetAgentNum(),(char*)pMsg,MsgLen);
}

void CServerSystem::ReloadResourceData()	
{
	SKILLMGR->Release();
	SKILLMGR->Init();
	
	GAMERESRCMNGR->LoadPlayerxMonsterPoint();

	ITEMMGR->ReloadItemList();

	ITEMMGR->LoadReinforceItemInfo();

	ITEMMGR->LoadGoldShopItem();



	g_pUserTable->SetPositionUserHead();
	CObject* pObject;
	while(pObject = (CObject*)g_pUserTable->GetUserData())
	{
		BYTE objectKind = pObject->GetObjectKind();

		switch(objectKind)
		{
		case eObjectKind_Monster:
		case eObjectKind_Mining:
		case eObjectKind_Collection:
		case eObjectKind_Hunt:
		case eObjectKind_TitanMonster:
			{
				RemoveMonster(pObject->GetID());
				break;
			}
		case eObjectKind_BossMonster:
			{
				RemoveBossMonster(pObject->GetID());
				break;
			}
		case eObjectKind_Npc:
			{
				RemoveNpc(pObject->GetID());
				break;
			}		
		}
	}
	g_pAISystem->RemoveAllList();

	REGENMGR->Release();
	GROUPMGR->Release();

	GAMERESRCMNGR->ResetPetList();
	GAMERESRCMNGR->LoadPetList();
	GAMERESRCMNGR->LoadPetRule();
	GAMERESRCMNGR->ResetPetBuffList();
	GAMERESRCMNGR->LoadPetBuffList();

	GAMERESRCMNGR->RemoveTitanList();
	GAMERESRCMNGR->LoadTitanList();
	GAMERESRCMNGR->LoadTitanRule();
	GAMERESRCMNGR->LoadTitanSpellCostInfo();
	GAMERESRCMNGR->ResetMonsterList();
	GAMERESRCMNGR->LoadMonsterList();
	GAMERESRCMNGR->LoadSMonsterList();

	GAMERESRCMNGR->LoadPaneltyTime();

	MON_SPEECHMGR->LoadMonSpeechInfoList();

	RAREITEMMGR->LoadRareItemOptionInfo();
	RAREITEMMGR->LoadRareItemInfo();
	RAREITEMMGR->LoadRareValueProbList();

	GAMERESRCMNGR->LoadNomalClothesSkinList();

	GAMERESRCMNGR->LoadCostumeSkinList();

	ITEMDROP_OBJ->Init();

	MAPITEMDROP_OBJ->Init();
	
	g_pAISystem->LoadAIGroupList();
}


void CServerSystem::AddWaitExitPlayer( DWORD dwAgentConnetionIndex, DWORD dwPlayerID )
{
	WAIT_EXIT_PLAYER_INFO* pInfo = m_mpWaitExitPlayer->Alloc();
	pInfo->dwAgentConnectionIndex = dwAgentConnetionIndex;
	pInfo->dwPlayerID = dwPlayerID;
	pInfo->dwStartTime = gCurTime;
	m_listWaitExitPlayer.AddTail( pInfo );
}

void CServerSystem::ProcessWaitExitPlayer()
{
	if( m_listWaitExitPlayer.GetCount() == 0 ) return;

	WAIT_EXIT_PLAYER_INFO* pInfo = (WAIT_EXIT_PLAYER_INFO*)m_listWaitExitPlayer.GetHead();
	if( pInfo )
	{
		if( gCurTime - pInfo->dwStartTime >= 5*1000 )	
		{
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pInfo->dwPlayerID );
			if( pPlayer )
			{
				DWORD dwObjectID = pPlayer->GetID();
				DWORD dwUserID = pPlayer->GetUserID();

				pPlayer->UpdateLogoutToDB();

				PARTYMGR->UserLogOut( pPlayer );
				FRIENDMGR->UserLogOut( dwObjectID );
				GUILDMGR->UserLogOut( pPlayer );


				g_pServerSystem->RemovePlayer( dwObjectID );
				ConfirmUserOut( pInfo->dwAgentConnectionIndex, dwUserID, dwObjectID, 2 );
			}

			m_mpWaitExitPlayer->Free( pInfo );
			m_listWaitExitPlayer.RemoveHead();
		}
	}
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( !file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	if( strcmp( file.GetString(), "*NATION" ) == 0 )
	{
		if( strcmp( file.GetString(), "CHINA" ) == 0 )
		{
			m_Nation = eNATION_CHINA;
		}
	}

	file.Release();
}

void CServerSystem::RemoveServerForKind( WORD wSrvKind )
{
	g_pServerTable->RemoveServerForKind(wSrvKind);
}


void CServerSystem::LoadHackCheck()
{
	CMHFile file;
	if( !file.Init( "ServerSet/HackCheck.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		return;
	}

	char temp[256] = {0, };
	while( 1 )
	{
		if( file.IsEOF() )	break;

		file.GetString( temp );
		if( strcmp( temp, "*HACKCHECKNUM" ) == 0 )
		{
			g_nHackCheckNum = file.GetInt();
		}
		else if( strcmp( temp, "*HACKCHECKWRITENUM" ) == 0 )
		{
			g_nHackCheckWriteNum = file.GetInt();
		}
	}
	file.Release();
}


void CServerSystem::CheckServerInfo( DWORD ObjectID )
{
	memset( &m_ProcessInfo, 0, sizeof(m_ProcessInfo) );
	SetProtocol( &m_ProcessInfo,MP_CHEAT, MP_CHEAT_MAP_CONDITION );
	m_ProcessInfo.dwObjectID = ObjectID;
	m_bCheckInfo = TRUE;

	TestGameQuery( ObjectID );
	TestLogQuery( ObjectID );

	m_ProcessInfo.ProcessInfo.DBQueryCount = g_DB.GetQueryQueueCount();
	m_ProcessInfo.StarTime = gCurTime;
}
void CServerSystem::SetGameConfig(GAMECONFIG &sGame)
{
	memcpy(&sGameSet,&sGame,sizeof(GAMECONFIG));
	RESTDMGR->InitRestConfig(m_wMapNum,sGameSet.ReSetPower,sGameSet.MaxReSetTime,sGameSet.MinReSetLevel,sGameSet.ReSetMoney);
}
void CServerSystem::SetRestConfig(WORD RestCfg,BOOL val)
{
	sGameSet.MaxReSetTime = RestCfg;
	if (val)
		UpdateGameConfig(sGameSet);
}
void CServerSystem::UpdateGameCfg(MSG_GAMECFG* msg)
{
	SetGameConfig(msg->sGamecfg);
	ShowConfig();
	UpdateGameConfig(sGameSet);
}
void CServerSystem::SetGameCfgMsg(MSG_GAMECFG& msg)
{
	msg.Category = MP_CHEAT;
	msg.Protocol = MP_CHEAT_GAMECFG_GET_ACK;
	memcpy(&msg.sGamecfg,&sGameSet,sizeof(GAMECONFIG));
}
void CServerSystem::ShowConfig()
{
	g_Console.LOGEX(4, RGB(0,255,0),"--------------[游戏配置信息]--------------");
	if(sGameSet.ReSetPower==1)
		g_Console.LOGEX(4, RGB(0,255,0), "在线转生系统开放");
	else
		g_Console.LOGEX(4, RGB(255,0,0),"在线转生系统关闭");
	if(sGameSet.ReStagePower==1)
		g_Console.LOGEX(4, RGB(0,255,0), "在线转职系统开放");
	else
		g_Console.LOGEX(4, RGB(255,0,0),"在线转职系统关闭");
	g_Console.LOGEX(4, RGB(0,255,0), "最大转生数:[%d]",sGameSet.MaxReSetTime);

	g_Console.LOGEX(4, RGB(0,255,0), "最小转生等级:[%d]",sGameSet.MinReSetLevel);

	g_Console.LOGEX(4, RGB(0,255,0), "转生所需游戏币:[%d]",sGameSet.ReSetMoney);

	g_Console.LOGEX(4, RGB(0,255,0), "一次转职所需游戏币:[%d]",sGameSet.FirstStageMoney);

	g_Console.LOGEX(4, RGB(0,255,0), "二次转职所需游戏币:[%d]",sGameSet.SecondStageMoney);

	g_Console.LOGEX(4, RGB(0,255,0), "结婚所需等级:[%d]",sGameSet.MarryLvSet);

	g_Console.LOGEX(4, RGB(0,255,0), "结婚所需游戏币:[%d]",sGameSet.MarryMoney);

	g_Console.LOGEX(4, RGB(0,255,0), "结婚所需元宝:[%d]",sGameSet.MarryGold);


	g_Console.LOGEX(4, RGB(0,255,0), "泡点所需等级:[%d]",sGameSet.PaoDianLevel);

	g_Console.LOGEX(4, RGB(0,255,0), "泡点所需转数:[%d]",sGameSet.PaoDianReSet);

	g_Console.LOGEX(4, RGB(0,255,0), "泡点时间:[%d分钟]",sGameSet.PaoDianTime);
	if(g_MapNum==17)
		g_Console.LOGEX(4, RGB(0,255,0), "当前地图所得泡点数量:[%d]",sGameSet.PaoDianMap_LZ);
	else
		g_Console.LOGEX(4, RGB(0,255,0), "当前地图所得泡点数量:[%d]",sGameSet.PaoDianMap);
	g_Console.LOGEX(4, RGB(0,255,0), "--------------[游戏配置信息结束]--------------");
}

void __stdcall OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "连接到监控服务进程: IP=%s	端口=%d	编号=%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "连接到服务端进程: IP=%s	端口=%d	编号=%d", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
			g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
}

void __stdcall OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(MAP_SERVER, GAMERESRCMNGR->GetLoadMapNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
	}
	else
	{
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}

void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "服务端进程连接:编号[%d]", dwConnectionIndex);
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if( g_pServerSystem->GetStart() == FALSE ) return;

	g_Console.LOG(4, "服务端进程链接断开:编号[%d]", dwConnectionIndex);

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;


	g_pUserTable->SetPositionUserHead();
	CObject * info = NULL;
	cPtrList list;

	while(info = g_pUserTable->GetUserData())
	{
		if(info->GetObjectKind() == eObjectKind_Player)
		{
			CPlayer * ply = (CPlayer *)info;
			if(ply->GetAgentNum() == dwConnectionIndex)
			{
				SaveMapChangePointUpdate(ply->GetID(), 0);
				list.AddTail( ply );
			}
		}
	}
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPlayer* p = (CPlayer*)list.GetNext(pos);
		g_pServerSystem->RemovePlayer(p->GetID());
	}
	list.RemoveAll();
}

void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{

}

void __stdcall ProcessDBMessage(DWORD dwEventIndex)
{
	/*if( g_bCloseWindow == TRUE )
		return;*/

	g_DB.ProcessingDBMessage();
}

void __stdcall ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(pTempMsg->Category < MP_MAX);
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;
	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void __stdcall ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(g_pUserMsgParser[pTempMsg->Category]);
	
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void __stdcall GameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->Process();
}
void __stdcall _5minGameProcess(DWORD dwEventIndex)
{
	g_pServerSystem->_5minProcess();

	MAPITEMDROP_OBJ->Process();
}
void ReLoadRareInfo()
{//[重加载绿化参数信息][BY:十里坡剑神][QQ:112582793][2019-4-4][21:22:12]
	RAREITEMMGR->LoadRareItemOptionInfo();
}
void ReloadRestTime()
{
	RESTDMGR->ReLoadRestTime();
}
void ReloadDissolutionList()
{
	DISSOLUTIONMGR->ReloadDissolutionInfo();
}

void ShowCfg()
{
	g_pServerSystem->ShowConfig();
}


extern BOOL g_bWriteQuery;

void ReloadConfig()
{
	//g_pServerSystem->ReadServerConfig(g_pServerSystem->GetMapNum());  // Reload The Game Config 2014-05-06
}

void OnCommand(char* szCommand)
{
	DWORD val = 0;
	DWORD val1 = 0;
	float fVal = 0.0f;
	strupr(szCommand);
	char Header[256] = {0,};	
	char Command[64] = {0,};
	sscanf(szCommand,"%s",Header);
	START_STRING_COMPARE(Header)

	COMPARE("DROPITEMEX")//[添加随机掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:47:32]
		sscanf(szCommand,"%s %d",Header,&val);
		LIMITITEMDROPMGR->AddCtrlRandDropItem(val);
	COMPARE("DROPITEM")//[添加指定掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:47:38]
		sscanf(szCommand,"%s %d %d ",Header,&val,&val1);
		LIMITITEMDROPMGR->AddCtrlDropItem(val,val1);
	COMPARE("DELDROPITEM1")//[删除指定掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:47:47]
		sscanf(szCommand,"%s %d",Header,&val);
		LIMITITEMDROPMGR->DelCtrlDropItem(val);
	COMPARE("DELDROPITEM2")//[删除随机掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:47:59]
		sscanf(szCommand,"%s %s",Header,&Command);
		LIMITITEMDROPMGR->DelCtrlDropItem(Command);
	COMPARE("CLEARDROPITEM")//[清空添加的掉落规则][By:十里坡剑神][QQ:112582793][2019/3/27][17:48:11]
		sscanf(szCommand,"%s %d",Header,&val);
		LIMITITEMDROPMGR->ClearCtrlDropItem(val);
	COMPARE("OUTPUTTIMEINFO")//[输出地图掉落控制信息][By:十里坡剑神][QQ:112582793][2019/3/27][17:48:26]
		LIMITITEMDROPMGR->OutPutLimitItemDropTimeInfo();
	COMPARE("RELOADLIMITITEMDROP")//[重加载地图掉落控制信息][By:十里坡剑神][QQ:112582793][2019/3/27][17:48:46]
		LIMITITEMDROPMGR->ReLoadDropItemInfo();
	COMPARE("SHOWDROPLIST")//[显示掉落规则信息][By:十里坡剑神][QQ:112582793][2019/3/27][17:49:22]
		LIMITITEMDROPMGR->ShowDropItemList();
	COMPARE("SHOWTIME")
		RESTDMGR->ShowConfigInformation();
	END_COMPARE
}

BOOL LoadEventRate(char* strFileName)
{
	int i=0;
	for(i=0; i<eEvent_Max; ++i)	
	{
		gEventRate[i] = 1.f;
		gEventRateFile[i] = 1.f;
	}

	CMHFile file;
	if( !file.Init( strFileName, "rb") )
		return FALSE;


	char Token[256];
	
	while( !file.IsEOF() )
	{
		file.GetString(Token);

		if(strcmp(Token,"#EXP") == 0)	
			gEventRateFile[eEvent_ExpRate] = file.GetFloat();
		else if(strcmp(Token, "#ABIL") == 0)
			gEventRateFile[eEvent_AbilRate] = file.GetFloat();
		else if(strcmp(Token,"#ITEM") == 0)
			gEventRateFile[eEvent_ItemRate] = file.GetFloat();
		else if(strcmp(Token,"#MONEY") == 0)
			gEventRateFile[eEvent_MoneyRate] = file.GetFloat();
		else if(strcmp(Token,"#GETMONEY") == 0)
			gEventRateFile[eEvent_GetMoney] = file.GetFloat();
		else if(strcmp(Token,"#DAMAGERECIVE") == 0)
			gEventRateFile[eEvent_DamageReciveRate] = file.GetFloat();
		else if(strcmp(Token,"#DAMAGERATE") == 0)
			gEventRateFile[eEvent_DamageRate] = file.GetFloat();
		else if(strcmp(Token,"#NAERYUKSPEND") == 0)
			gEventRateFile[eEvent_NaeRuykRate] = file.GetFloat();
		else if(strcmp(Token,"#UNGISPEED") == 0)
			gEventRateFile[eEvent_UngiSpeed] = file.GetFloat();
		else if(strcmp(Token,"#PARTYEXP") == 0)
			gEventRateFile[eEvent_PartyExpRate] = file.GetFloat();
		else if(strcmp(Token,"#MUGONGEXPRATE") == 0)
			gEventRateFile[eEvent_MugongExp] = file.GetFloat();
		else if(strcmp(Token,"#PVPD")==0)
			gEventRateFile[eEvent_PVPD] = file.GetFloat();
		else if(strcmp(Token,"#PVPA")==0)
			gEventRateFile[eEvent_PVPA] = file.GetFloat();
		else if(strcmp(Token,"#ATTRIB")==0)
			gEventRateFile[eEvent_Attrib] = file.GetFloat();
		else if(strcmp(Token,"#PHYAYYACK")==0)
			gEventRateFile[eEvent_PhyAttack] = file.GetFloat();
		else if(strcmp(Token,"#MAXPHYDEFENCERATE")==0)
			gEventRateFile[eEvent_PhyDefence] = file.GetFloat();
		else if(strcmp(Token,"#MAXREGDEFENCERATE")==0)
			gEventRateFile[eEvent_RegDefence] = file.GetFloat();
	}


	file.Release();

	for(i=0; i<eEvent_Max; ++i)	
		gEventRate[i] = gEventRateFile[i];

	return TRUE;
}



BOOL LoadPartyPlustime(char* strFileName)
{
	memset( &gPartyEvent, 0, sizeof(PARTYEVENT)*ePartyEvent_Max );

	CMHFile file;
	if( !file.Init( strFileName, "rb") )
		return FALSE;

	int count = file.GetInt();	

	for( int i=0; i<count; ++i )
	{//[组队加成信息读取][By:十里坡剑神][QQ:112582793][2018/4/22]
		int idx = file.GetInt();
		gPartyEvent[idx].Rate[0] = file.GetFloat();
		gPartyEvent[idx].Rate[1] = file.GetFloat();
		gPartyEvent[idx].Rate[2] = file.GetFloat();
		gPartyEvent[idx].Rate[3] = file.GetFloat();
		gPartyEvent[idx].Rate[4] = file.GetFloat();
		gPartyEvent[idx].Rate[5] = file.GetFloat();
		gPartyEvent[idx].Rate[6] = file.GetFloat();
	}

	return TRUE;
}


void ReloadReinforeceInfo()
{
	//REINFORCEMGR->RLoadItemLevelLimitInfo();
}

