#include "stdafx.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "GameResourceManager.h"
#include "../GlobalEventFunc.h"
#include "..\CharMake.h"
#include "..\MainDialog.h"
#include "..\GameIn.h"
#include "..\WindowIDEnum.h"
#include "..\ObjectManager.h"
#include "..\CharacterDialog.h"
#include "..\MugongManager.h"
#include "..\ItemManager.h"
#include "..\InventoryExDialog.h"
#include "..\MoneyDlg.h"
#include "cTabDialog.h"
#include "..\AlertDlg.h"
#include "cWindowFunc.h"
#include "../MousePointer.h"
#include "../input/cIMEWnd.h"
#include "../input/Mouse.h"
#include "cAnimationManager.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../MHMap.h"
#include "../cMsgBox.h"
#include "../cDivideBox.h"
#include "cWindowSystemFunc.h"
#include "mhFile.h"
#include "SuryunDialog.h"
#include "../MugongSuryunDialog.h"
#include "MainBarDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "PyogukDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "ChatDialog.h"
#include "OptionDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "NoteDialog.h"
#include "FriendDialog.h"
#include "MiniFriendDialog.h"
#include "MiniNoteDialog.h"
#include "ReviveDialog.h"
#include "ExchangeDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StallFindDlg.h"
#include "StreetStall.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "InventoryExDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "TitanPartsMakeDlg.h"	
#include "TitanMixDlg.h"		
#include "TitanRepairDlg.h"
#include "TitanRecallDlg.h"
#include "TitanUpgradeDlg.h"	
#include "TitanBreakDlg.h"	
#include "TitanGuageDlg.h"		
#include "TitanRegisterDlg.h"	
#include "TitanDissolutionDlg.h"
#include "TitanChangePreView.h"
#include "TitanInventoryDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestTotalDialog.h"
#include "QuestDialog.h"
#include "WantedDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "BailDialog.h"
#include "MPRegistDialog.h"
#include "MPMissionDialog.h"
#include "MPNoticeDialog.h"
#include "MPGuageDialog.h"
#include "DissolutionDialog.h"
#include "EventNotifyDialog.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "StatusIconDlg.h"
#include "GuildFieldWarDialog.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "PartyWarDialog.h"
#include "ShoutDialog.h"
#include "ShoutchatDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "SeigeWarDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"
#include "KeySettingTipDlg.h"
#include "cIMEEx.h"
#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "GuildNoticeDlg.h"
#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"
#include "guildjoindialog.h"
#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif
#include "ScreenShotDlg.h"
#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildTraineeDialog.h"
#include "SurvivalModeManager.h"
#include "SurvivalCountDialog.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "UniqueItemMixDlg.h"
#include "MHCamera.h"
#include "SOSDialog.h"
#include "UniqueItemMixProgressBarDlg.h"
#include "TitanMixProgressBarDlg.h"		
#include "TitanPartsProgressBarDlg.h"	
#include "SkinSelectDialog.h"	
#include "CostumeSkinSelectDialog.h"	
#include "SkillPointResetDlg.h"
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"
#include "PartyMatchingDlg.h"
#include "FortWarDialog.h"
#include "MiniMarryDialog.h"  
#include "ControlDialog.h"    
#include "OtherCharacterEquip.h"  
#include "../cDivideBoxEx.h"      
#include "ItemLockManager.h"         
#include "ItemUnLockManager.h"       
#include "OnlineFame.h"				//[在线洗恶][By:十里坡剑神][QQ:112582793][2017/11/22]
#include "BillboardDialog.h"		//[个人排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "GuildTopList.h"			//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "CharacterRBTN.h"			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "FBTipDlg.h"				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/22]
#include "OpenPyogukDlg.h"			//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
#include "cSingedDialog.h"			//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
#include "cRechargeManager.h"		//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "cSetingInfoDlg.h"			//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
#include "cStatusIconBtnDlg.h"		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
#include "cTopRankDlg.h"			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
#include "cInventoryRBTDlg.h"		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "cSendItemInfoDlg.h"	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "cKillTopList.h"		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
#include "cSafeLockDialog.h"	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
#include "cMainDlgHideBtn.h"	//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
#include "cMainDlgSafeBtn.h"	//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
#include "cSafeLockMainDlg.h"	//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
#include "VideoCaptureDlg.h"	//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/6/10]
#ifdef _GMTOOL_
#include "ExitDialog.h"
#endif
#include "GoldShopMainDailog.h"	//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
#ifndef _XDYY_
#include "cOnlineVoiceDlg.h"	//[小队语音设置界面][By:十里坡剑神][QQ:112582793][2019/3/20][14:56:18]
#endif
#include "FastConfigPointDlg.h"//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
#include "LotSplitItemDlg.h"   //[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:09:26]
#include "VimuWithGoldDlg.h"	//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:52:41]
#include "cEquipItemInherit.h"  //[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:28:24]
#include "cUnionWarManager.h"	//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:49:35]
extern HWND _g_hWnd;

GLOBALTON(cWindowManager);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cWindowManager::cWindowManager()
{
	m_pScriptManager	= NULL;
	m_pWindowList		= NULL;
	m_pIconDlgListRef	= NULL;
	m_pEditBoxListRef	= NULL;
	m_pDestroyWindowRef = NULL;
	m_pScreenTextList	= NULL;
	m_OldDragFlag		= FALSE;
	m_pMousePointWindow = NULL;
	m_pGuageWindow		= NULL;
	m_pMonsterGuageWindow = NULL;
	m_fComposition		= FALSE;
	cbDragnDropProcess	= NULL;
	
	m_pFocusedEdit		= NULL;
	
	m_id			= -1;
	m_pDragDialog	= NULL;
	m_bHideInterface = FALSE;
	m_pRelationTip = NULL;
	m_pRelationTip1 = NULL;
}

cWindowManager::~cWindowManager()
{
}
void cWindowManager::Release()
{
	DestroyWindowAll();
	SAFE_DELETE(m_pScriptManager);
	SAFE_DELETE(m_pIconDlgListRef);
	SAFE_DELETE(m_pEditBoxListRef);
	SAFE_DELETE(m_pScreenTextList);
	SAFE_DELETE(m_pPlayerChatTooltip);
	SAFE_DELETE(m_pWindowList);
	SAFE_DELETE(m_pDestroyWindowRef);
	SAFE_DELETE(m_pMousePointWindow);
	SAFE_DELETE(m_pGuageWindow);
	SAFE_DELETE(m_pMonsterGuageWindow);
	
	CFONT_OBJ->Release();
}
void cWindowManager::Init()
{
	m_pScriptManager = new cScriptManager;
	m_pWindowList = new cPtrList;
	m_pEditBoxListRef = new cPtrList;
	m_pIconDlgListRef = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScreenTextList = new cPtrList;
	m_pPlayerChatTooltip = new cPtrList;

	m_pScriptManager->InitParseTypeData(); 

	LOGFONT	font;

//#ifdef TAIWAN_LOCAL
//	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	//	font.lfHeight = -12;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfEscapement = 0;
//	font.lfOrientation = 0;
//	font.lfWeight = FW_NORMAL;
//	font.lfItalic = 0;
//	font.lfUnderline = 0;
//	font.lfStrikeOut = 0;
//	font.lfCharSet = GB2312_CHARSET;	//埃眉
//	font.lfOutPrecision = 0;
//	font.lfClipPrecision = 0;
//	font.lfQuality =  PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
//	font.lfPitchAndFamily = 0;
//	//	lstrcpy(font.lfFaceName,"灿砰");
//	lstrcpy(font.lfFaceName,"宋体"); 
//
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);
//
//	font.lfUnderline = 1;
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"System");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"宋体");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
//
//	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfWidth = -font.lfHeight / 2;
//	//	lstrcpy(font.lfFaceName,"灿砰");
//	lstrcpy(font.lfFaceName,"宋体");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
//#elif defined _JAPAN_LOCAL_
//	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	//	font.lfHeight = -12;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfEscapement = 0;
//	font.lfOrientation = 0;
//	font.lfWeight = 0;//FW_NORMAL;
//	font.lfItalic = 0;
//	font.lfUnderline = 0;
//	font.lfStrikeOut = 0;
//	font.lfCharSet = SHIFTJIS_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
//	font.lfOutPrecision = 0;
//	font.lfClipPrecision = 0;
//	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
//	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
//	//	lstrcpy(font.lfFaceName,"MS PGothic"); 
//	//	lstrcpy(font.lfFaceName,"MS UI Gothic"); 
//	//	lstrcpy(font.lfFaceName,"MS UI Gothic"); 
//	lstrcpy(font.lfFaceName,"MS Gothic");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);
//
//	font.lfUnderline = 1;
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = FW_BOLD;
//	lstrcpy(font.lfFaceName,"MS Gothic");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = 0;
//	lstrcpy(font.lfFaceName,"MS Gothic");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
//
//	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"MS Gothic");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
//
//#elif defined _HK_LOCAL_
//	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	//	font.lfHeight = -12;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfEscapement = 0;
//	font.lfOrientation = 0;
//	font.lfWeight = 0;//FW_NORMAL;
//	font.lfItalic = 0;
//	font.lfUnderline = 0;
//	font.lfStrikeOut = 0;
//	font.lfCharSet = CHINESEBIG5_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
//	font.lfOutPrecision = 0;
//	font.lfClipPrecision = 0;
//	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
//	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
//	lstrcpy(font.lfFaceName,"MingLiU");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);
//
//	font.lfUnderline = 1;
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = FW_BOLD;
//	lstrcpy(font.lfFaceName,"MingLiU");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = 0;
//	lstrcpy(font.lfFaceName,"MingLiU");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
//
//	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"MingLiU");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
//#elif defined _TL_LOCAL_
//	font.lfHeight = -MulDiv(8, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	//	font.lfHeight = -12;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfEscapement = 0;
//	font.lfOrientation = 0;
//	font.lfWeight = 0;//FW_NORMAL;
//	font.lfItalic = 0;
//	font.lfUnderline = 0;
//	font.lfStrikeOut = 0;
//	font.lfCharSet = THAI_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
//	font.lfOutPrecision = 0;
//	font.lfClipPrecision = 0;
//	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
//	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
//	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);
//
//	font.lfUnderline = 1;
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = FW_BOLD;
//	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);
//
//	font.lfUnderline = 0;
//	font.lfHeight = -14;
//	font.lfWidth = -font.lfHeight / 2;
//	font.lfWeight = 0;
//	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
//
//	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
//	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
//
//#else
	extern HWND _g_hWnd;
	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_NORMAL;	
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;

	// auto check the charset 2014-05-10!  CHINESEBIG5_CHARSET  GB2312_CHARSET 
#ifdef _USEBIG5_
	font.lfCharSet = CHINESEBIG5_CHARSET;
#else
	font.lfCharSet = GB2312_CHARSET;
#endif
	font.lfOutPrecision = 0;
	font.lfQuality = PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName,"宋体");	
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);	
	font.lfWidth = -font.lfHeight / 2;		
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT5);

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);	
	font.lfWidth = -font.lfHeight / 2;		
	font.lfUnderline = 1;	
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;	
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);

	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);

	font.lfHeight = -12;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT6);

	font.lfHeight = -11;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT7);

	font.lfHeight = -14;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT8);

	font.lfHeight = -16;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT9);
	
	font.lfHeight = -14;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT10);

	font.lfHeight = -26;
	font.lfWeight = FW_HEAVY;
	font.lfWidth = -font.lfHeight / 2;
	font.lfQuality = PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_SWISS;
	lstrcpy(font.lfFaceName,"宋体");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT11);
//#endif
	cMsgBox::InitMsgBox();
}


void cWindowManager::CreatMousePoint()
{

}


void cWindowManager::CreateGameIn()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/15.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth();
	Pos1.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - window->GetHeight();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	
	GAMEIN->SetMainInterfaceDialog((CMainBarDialog * )window);
	
	GAMEIN->GetMainInterfaceDialog()->InitBar();	
	CreateMiniMapDlg();
	CreateMonsterGuageDlg();
	CreateCharDlg();
	CreateMugongSuryunDlg();
	CreateInventoryDlg();
	CreateRecvExchangeDlg();
	CreateHeroGuage();
	CreateDealDlg();
	CreateChatDlg();
	CreateStallKindSelectDlg();
	CreateStreetBuyStallDlg();
	CreateBuyRegDlg();
	CreateStallFindDlg();
	CreateStreetStallDlg();
	CreateNpcScriptDlg();
	CreateHelpDlg();
	CreateQuickDlg();
	CreatePartyDlg();
	CreatePartyCreateDlg();
	CreatePartyInviteDlg();
	CreateGuildCreateDlg();
	CreateGuildNickNameDlg();
	CreateGuildRankDlg();
	CreateGuildDlg();
	CreateGuildInviteDlg();
	CreateGuildMarkDlg();
	CreateGuildLevelupDlg();
	CreateGuildWarehouseDlg();
	CreatePyogukDlg();
	CreateFriendDlg();
	CreateMiniFriendDlg();
	CreateNoteDlg();
	CreateMiniNoteDlg();
	CreateReviveDlg();
	CreateWantNpcDlg();
	CreateWantRegistDlg();
	CreateQuestTotalDlg();
	CreateMPRegistDlg();
	CreateOptionDlg();
	CreateEventNotifyDlg();
	CreateMarryMiniDialog(); 
	CreateControlDialog();   
    CreateOtherCharacterEquip(); 
	CreateItemLockDlg();         
	CreateItemUnLockDlg();       
	CreateExitDlg();
	CreateExchangeDlg();
	CreateUpgradeDlg();
	CreateMixDlg();
	CreateTitanPartsProgressBarDlg();	
	CreateTitanPartsMakeDlg();		
	CreateTitanMixProgressBarDlg();	
	CreateTitanMixDlg();				
	CreateTitanRepairDlg();			
	CreateTitanRecallDlg();			
	CreateTitanUpgradeDlg();		
	CreateTitanBreakDlg();			
	CreateTitanChangePreViewDlg();	
	CreateTitanInventoryDlg();		
	CreateTitanGuageDlg();			
	CreateTitanRegisterDlg();		
	CreateTitanDissolution();		
	CreateReinforceDlg();
	CreateReinforceDataGuideDlg();
	CreateMoveDlg();
	CreatePointSaveDlg();
	CreateBailDlg();
	CreateDissolutionDlg();
	CreateGuildFieldWarDlg();
	CreateSkPointDlg();
	CreateSkPointNotifyDlg();
	CreatePartyWarDlg();
	CreateShoutDlg();
	CreateShoutchatDlg();
	CreateChaseDlg();
	CreateChaseinputDlg();
	CreateNameChangeDlg();
	CreateNameChangeNotifyDlg();
	CreateGTRegistDlg();
	CreateGTRegistcancelDlg();
	CreateGTStandingDlg();
	CreateGTBattleListDlg();
	CreateGTScoreInfoDlg();
	CreateSeigeWarDlg();
	CreateCharChangeDlg();
	CreateItemSealDlg();
	CreateChangeJobDlg();
	CreateReinforceResetDlg();
    CreateRareCreateDlg();
	CreatePetStateDlg();
	CreatePetStateMiniDlg();
	CreatePetInvenDlg();
	CreatePetUpgradeDlg();
	CreatePetRevivalDlg();
	CreateTipBrowserDlgDlg();
	CreateKeySettingTipDlg();
	CreateGuildNoteDlg();
	CreateUnionNoteDlg();
	CreateGuildNoticeDlg();
	CreateSkillOptionChangeDlg();
	CreateSkillOptionClearDlg();
	CreateGuildInvitationKindSelectionDlg();
	CreateGuildMunhaDlg();
	CreateGuildTraineeDlg();
	ScreenShotDlg();
	CreateSurvivalCountDlg();
	CreateUniqueItemMixProgressBarDlg();	
	CreateUniqueItemCurseCancellationDlg();
	CreateUniqueItemMixDlg();
	CreateSOSDlg();		
	CreateSkinSelectDlg();
	CreateCostumeSkinSelectDlg();
	CreateSkillPointResetDlg();	
	CreateStreetStallItemViewDlg();
	CreateChannelDlg();
	CreatePartyMatchingDlg(); 
	CreateOnlineFame();				//[在线洗恶][By:十里坡剑神][QQ:112582793][2017/11/22]
	CreateGuildTopListDlg();		//[帮会排行][By:十里坡剑神][QQ:112582793][2017/11/24]
	CreateBillboardDialog();		//[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	CreateCharacterRBTN();			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	CreateFBTipDlg();				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/22]
	CreateOpenPyogukDlg();			//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	CreateSingedDlg();				//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	CreateRechargeDlg();			//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	CreateSetingInfoDlg();			//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	CreateStatusIconBtnDlg();		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	CreateTopRankDlg();			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	CreateInventoryRBTDlg();		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	CreateSendItemInfoDlg();		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	CreateKillTopListDlg();			//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	CreateSafeLockDlg();			//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	CreateMainDlgHideBtnDlg();		//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
	CreateMainDlgSafeBtn();			//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	CreateSafeLockMainDlg();		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	CreateVideoCaptureDlg();		//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	CreateGoldShopMainDlg();		//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	CreateItemShopDlg();
	CreateFastCfgPointDlg();		//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
	CreateLotSplitItemDlg();		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:10:00]
	CreateVimuWithGoldDlg();		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:53:06]
	CreateEquipItemInherit();		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:27:18]
	CreateUnionWarInfoDlg();		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:42:34]
}

void cWindowManager::CreatePartyWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile( "./image/InterfaceScript/PartyWarDlg.bin", "rb" );
	AddWindow( window );
	GAMEIN->SetPartyWarDlg( (CPartyWarDialog*)window );
	GAMEIN->GetPartyWarDlg()->Linking();
}

void cWindowManager::CreateGuildFieldWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarDeclare.bin", "rb" );

	AddWindow( window );
	GAMEIN->SetGFWarDeclareDlg( (CGFWarDeclareDlg*)window );
	GAMEIN->GetGFWarDeclareDlg()->Linking();

	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarResult.bin", "rb" );

	AddWindow( window );
	GAMEIN->SetGFWarResultDlg( (CGFWarResultDlg*)window );
	GAMEIN->GetGFWarResultDlg()->Linking();

	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarInfo.bin", "rb" );

	AddWindow( window );
	GAMEIN->SetGFWarInfoDlg( (CGFWarInfoDlg*)window );
	GAMEIN->GetGFWarInfoDlg()->Linking();
	
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GuildWarInfo.bin", "rb" );

	AddWindow( window );
	GAMEIN->SetGuildWarInfoDlg( (CGuildWarInfoDlg*)window );
	GAMEIN->GetGuildWarInfoDlg()->Linking();

	window = GetDlgInfoFromFile( "./image/InterfaceScript/GuildPlusTime.bin", "rb" );
	AddWindow( window );
	GAMEIN->SetGuildPlusTimeDlg( (CGuildPlusTimeDlg*)window );
	GAMEIN->GetGuildPlusTimeDlg()->Linking();
}

void cWindowManager::CreateDissolveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/58.bin", "rb");

	AddWindow(window);
	GAMEIN->SetDissolveDialog((CDissloveDlg * )window);
	GAMEIN->GetDissolveDialog()->Linking();
}
void cWindowManager::CreateUpgradeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/55.bin", "rb");
	AddWindow(window);
	GAMEIN->SetUpgradeDialog((CUpgradeDlg * )window);
	GAMEIN->GetUpgradeDialog()->Linking();
}
void cWindowManager::CreateMixDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/56.bin", "rb");
	AddWindow(window);
	GAMEIN->SetMixDialog((CMixDialog * )window);
	GAMEIN->GetMixDialog()->Linking();
}

void cWindowManager::CreateTitanPartsMakeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/60.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanPartsMakeDlg((CTitanPartsMakeDlg * )window);
	GAMEIN->GetTitanPartsMakeDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

void cWindowManager::CreateTitanMixDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titanmix.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanMixDlg((CTitanMixDlg * )window);
	GAMEIN->GetTitanMixDlg()->Linking();
}

void cWindowManager::CreateTitanRepairDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Repair.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRepairDlg((CTitanRepairDlg * )window);
	GAMEIN->GetTitanRepairDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanRecallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_recall.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRecallDlg((CTitanRecallDlg * )window);
	GAMEIN->GetTitanRecallDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanUpgradeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanUpgrade.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanUpgradeDlg((CTitanUpgradeDlg * )window);
	GAMEIN->GetTitanUpgradeDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

void cWindowManager::CreateTitanBreakDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanBreak.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanBreakDlg((CTitanBreakDlg * )window);
	GAMEIN->GetTitanBreakDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanChangePreViewDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsChangePreview.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanChangePreViewDlg((CTitanChangePreViewDlg * )window);
	GAMEIN->GetTitanChangePreViewDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanInventoryDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_inventory.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanInventoryDlg((CTitanInventoryDlg * )window);
	GAMEIN->GetTitanInventoryDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanGuageDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_guage.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanGuageDlg((CTitanGuageDlg * )window);
	GAMEIN->GetTitanGuageDlg()->Linking();
}

void cWindowManager::CreateTitanRegisterDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_use.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanRegisterDlg((CTitanRegisterDlg * )window);
	GAMEIN->GetTitanRegisterDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanDissolution()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Bongin.bin", "rb");
	AddWindow(window);
	GAMEIN->SetTitanDissolutionDlg((CTitanDissolutionDlg * )window);
	GAMEIN->GetTitanDissolutionDlg()->Linking();
}

void cWindowManager::CreateReinforceDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/57.bin", "rb");

	AddWindow(window);
	GAMEIN->SetReinforceDialog((CReinforceDlg * )window);
	GAMEIN->GetReinforceDialog()->Linking();
}

void cWindowManager::CreateHeroGuage()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/23.bin", "rb");
	AddWindow(window);
	GAMEIN->SetGuageDlg((CGuageDialog * )window);
	GAMEIN->GetGuageDlg()->Linking();
}

void cWindowManager::CreateQuickDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/14.bin", "rb");
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x = (float)(GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	Pos1.y =GAMEIN->GetMiniMapDialog()->GetHeight();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetQuickDialog((CQuickDialog * )window);
}

void cWindowManager::CreateMenuSlotDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/MenuSlot.bin", "rb");
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetMenuSlotDialog((CMenuSlotDialog * )window);
	GAMEIN->GetMenuSlotDialog()->Linking();
}

void cWindowManager::CreatePartyDlg() 
{
	cWindow * member1window = GetDlgInfoFromFile("./image/InterfaceScript/partymember1dlg.bin", "rb");
	cWindow * member2window = GetDlgInfoFromFile("./image/InterfaceScript/partymember2dlg.bin", "rb");
	cWindow * member3window = GetDlgInfoFromFile("./image/InterfaceScript/partymember3dlg.bin", "rb");
	cWindow * member4window = GetDlgInfoFromFile("./image/InterfaceScript/partymember4dlg.bin", "rb");
	cWindow * member5window = GetDlgInfoFromFile("./image/InterfaceScript/partymember5dlg.bin", "rb");
	cWindow * member6window = GetDlgInfoFromFile("./image/InterfaceScript/partymember6dlg.bin", "rb");
	cWindow * partybtnwindow = GetDlgInfoFromFile("./image/InterfaceScript/PartyBtnDialog.bin", "rb");
	
	ASSERT(member1window);	ASSERT(member2window);	ASSERT(member3window);
	ASSERT(member4window);	ASSERT(member5window);	ASSERT(member6window);
	ASSERT(partybtnwindow);
	
	AddWindow(member1window);	AddWindow(member2window);	AddWindow(member3window);
	AddWindow(member4window);	AddWindow(member5window);	AddWindow(member6window);
	AddWindow(partybtnwindow);
	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(0, (CPartyMemberDlg*)member1window);	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(1, (CPartyMemberDlg*)member2window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(2, (CPartyMemberDlg*)member3window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(3, (CPartyMemberDlg*)member4window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(4, (CPartyMemberDlg*)member5window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(5, (CPartyMemberDlg*)member6window);

	GAMEIN->GetPartyDialog()->RegistBtnDlg((CPartyBtnDlg*)partybtnwindow);

} 

void cWindowManager::CreatePartyCreateDlg()
{

	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyCreate.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyCreateDialog((CPartyCreateDlg * )window);
	GAMEIN->GetPartyCreateDialog()->Linking();

}

void cWindowManager::CreatePartyInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyInvite.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyInviteDialog((CPartyInviteDlg * )window);
	GAMEIN->GetPartyInviteDialog()->Linking();

}

void cWindowManager::CreateGuildCreateDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildCreate.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildCreateDlg((CGuildCreateDialog * )window);
	GAMEIN->GetGuildCreateDlg()->Linking();
	
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildUnionCreate.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildUnionCreateDlg((CGuildUnionCreateDialog * )window);
	GAMEIN->GetGuildUnionCreateDlg()->Linking();
}

void cWindowManager::CreateGuildDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Guild.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildDlg((CGuildDialog * )window);
	GAMEIN->GetGuildDlg()->Linking();
}

void cWindowManager::CreateGuildInviteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildInvite.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildInviteDlg((CGuildInviteDialog * )window);
	GAMEIN->GetGuildInviteDlg()->Linking();
}

void cWindowManager::CreateGuildMarkDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildMark.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildMarkDlg((CGuildMarkDialog * )window);
	GAMEIN->GetGuildMarkDlg()->Linking();
}

void cWindowManager::CreateGuildLevelupDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildLevelUp.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildLevelUpDlg((CGuildLevelUpDialog * )window);
	GAMEIN->GetGuildLevelUpDlg()->Linking();
}

void cWindowManager::CreateGuildNickNameDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNickName.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildNickNameDlg((CGuildNickNameDialog * )window);
	GAMEIN->GetGuildNickNameDlg()->Linking();
}

void cWindowManager::CreateGuildRankDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildRank.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildRankDlg((CGuildRankDialog * )window);
	GAMEIN->GetGuildRankDlg()->Linking();
}

void cWindowManager::CreateGuildWarehouseDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildWarehouse.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildWarehouseDlg((CGuildWarehouseDialog * )window);
	GAMEIN->GetGuildWarehouseDlg()->Linking();
}

void cWindowManager::CreatePyogukDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Pyoguk.bin", "rb");

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPyogukDialog((CPyogukDialog * )window);
	GAMEIN->GetPyogukDialog()->Linking();
}

void cWindowManager::CreateWantNpcDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantNpc.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetWantNpcDialog((CWantNpcDialog * )window);
	GAMEIN->GetWantNpcDialog()->Linking();
}

void cWindowManager::CreateWantRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantRegist.bin", "rb");
	ASSERT(window);
	AddWindow(window);

	GAMEIN->SetWantRegistDialog((CWantRegistDialog * )window);
	GAMEIN->GetWantRegistDialog()->Linking();

}

void cWindowManager::CreateMiniMapDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/17.bin", "rb");
	ASSERT(window);

	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth());
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	GAMEIN->SetMiniMapDialog((CMiniMapDlg * )window);
	GAMEIN->GetMiniMapDialog()->Linking();
	GAMEIN->GetMiniMapDialog()->InitMiniMap(MAP->GetMapNum());
	
	cWindow * window2 = GetDlgInfoFromFile("./image/InterfaceScript/bigmap.bin", "rb");

	ASSERT(window2);
	VECTOR2 Pos2;
	Pos2.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2)-278; 
	Pos2.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2)-278;
	window2->SetAbsXY(Pos2.x,Pos2.y);
	AddWindow(window2);

	GAMEIN->SetBigMapDialog((CBigMapDlg * )window2);
	if( GAMEIN->GetBigMapDialog() )
	{
		GAMEIN->GetBigMapDialog()->Linking();
		GAMEIN->GetBigMapDialog()->InitBigMap(MAP->GetMapNum());
	}
}
//KES
void cWindowManager::CreateMacroDlg()
{
	cWindow* window = GetDlgInfoFromFile( "./image/16.bin", "rb" );
	AddWindow( window );

	GAMEIN->SetMacroDialog( (CMacroDialog*)window );
	GAMEIN->GetMacroDialog()->Linking();

}

void cWindowManager::CreateChatDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/22.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX();
	Pos1.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - window->GetHeight()+6;
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetChatDialog((CChatDialog * )window);
	GAMEIN->GetChatDialog()->Linking();
}

void cWindowManager::CreateOptionDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/21.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-2;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetOptionDialog((COptionDialog * )window);
	GAMEIN->GetOptionDialog()->Linking();
}

void cWindowManager::CreateExitDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/25.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight());
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	GAMEIN->SetExitDialog((CExitDialog * )window);

}

void cWindowManager::CreateCharStateDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/29.bin", "rb");
	AddWindow(window);
	GAMEIN->SetCharStateDialog((CCharStateDialog*)window);
	GAMEIN->GetCharStateDialog()->Linking();
}

void cWindowManager::CreateNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Note.bin", "rb");
	
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	
	GAMEIN->SetNoteDialog((CNoteDialog * )window);
	GAMEIN->GetNoteDialog()->Linking();
}

void cWindowManager::CreateFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Friend.bin", "rb");
	
	ASSERT(window);
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	
	GAMEIN->SetFriendDialog((CFriendDialog * )window);
	GAMEIN->GetFriendDialog()->Linking();
}


void cWindowManager::CreateMiniFriendDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniFriend.bin","rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniFriendDialog((CMiniFriendDialog * )window);
	GAMEIN->GetMiniFriendDialog()->Linking();
}

void cWindowManager::CreateMiniNoteDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniNote.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniNoteDialog((CMiniNoteDialog * )window);
	GAMEIN->GetMiniNoteDialog()->Linking();
}

void cWindowManager::CreateMarryMiniDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniMarry.bin","rb");

	ASSERT(window);
	AddWindow(window);

	CMiniMaryDialog* pDlg = (CMiniMaryDialog*)window;
	GAMEIN->SetMiniMaryDialog(pDlg);
	GAMEIN->GetMiniMaryDialog()->Linking();
}



void cWindowManager::CreateControlDialog()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Control.bin","rb");

	ASSERT(window);

	// 功菜单 2016-01-07
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W)/2;
	Pos1.y =window->GetAbsY()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)/2; 
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

    CControlDialog* pDlg = (CControlDialog*)window;

	GAMEIN->SetControlDialog(pDlg);

	GAMEIN->GetControlDialog()->Linking();
}

void cWindowManager::CreateReviveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Revive.bin", "rb");
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetReviveDialog((CReviveDialog * )window);
	GAMEIN->GetReviveDialog()->Linking();
}

void cWindowManager::CreateCharDlg()
{
//#ifdef _JAPAN_LOCAL_
//	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/26.bin", "rb");
//#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/24.bin", "rb");
//#endif
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX();
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetCharacterDialog((CCharacterDialog * )window);
	GAMEIN->GetCharacterDialog()->Linking();
}
void cWindowManager::CreateMugongSuryunDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/10.bin", "rb");
	AddWindow(window);

	CMugongSuryunDialog * pDlg = (CMugongSuryunDialog * )window;
	GAMEIN->SetMugongSuryunDialog(pDlg);

	CMugongDialog* pMugongDlg = (CMugongDialog*)pDlg->GetTabSheet(0);
//#ifdef _JAPAN_LOCAL_
//	pMugongDlg->Linking_JP();
//#elif defined _HK_LOCAL_
//	pMugongDlg->Linking_JP();
//#elif defined _TL_LOCAL_
//	pMugongDlg->Linking_JP();
//#else
	pMugongDlg->Linking();	
//#endif //_JAPAN_LOCAL_
	
	GAMEIN->SetMugongDialog(pMugongDlg);
	MUGONGMGR->SetMugongDialog(pMugongDlg);

	CSuryunDialog* pSuryunDlg = (CSuryunDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetSuryunDialog(pSuryunDlg);
	pSuryunDlg->Linking();
}

void cWindowManager::CreateMPRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPRegist.bin", "rb");
	AddWindow(window);

	GAMEIN->SetMPRegistDialog((CMPRegistDialog * )window);
	GAMEIN->GetMPRegistDialog()->Linking();
}

void cWindowManager::CreateMPMissionDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPMission.bin", "rb");
	AddWindow(window);

	GAMEIN->SetMPMissionDialog((CMPMissionDialog * )window);
	GAMEIN->GetMPMissionDialog()->Linking();
}

void cWindowManager::CreateMPNoticeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPNotice.bin", "rb");
	AddWindow(window);
	GAMEIN->SetMPNoticeDialog((CMPNoticeDialog * )window);
	GAMEIN->GetMPNoticeDialog()->Linking();
}

void cWindowManager::CreateMPGuageDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPGuage.bin", "rb");
	AddWindow(window);
	GAMEIN->SetMPGuageDialog((CMPGuageDialog * )window);
	GAMEIN->GetMPGuageDialog()->Linking();
}

void cWindowManager::CreatePetStateDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetState.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-300; 
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetStateDialog((CPetStateDlg* )window);
	GAMEIN->GetPetStateDialog()->Linking();
	PETMGR->SetPetStateDlg((CPetStateDlg*)window);
}

void cWindowManager::CreatePetStateMiniDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetStateMini.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-300;
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	GAMEIN->SetPetStateMiniDialog((CPetStateMiniDlg*)window);
	GAMEIN->GetPetStateMiniDialog()->Linking();
	PETMGR->SetPetStateMiniDlg((CPetStateMiniDlg*)window);
}

void cWindowManager::CreatePetInvenDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetInven.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-300; 
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	GAMEIN->SetPetInventoryDialog((CPetInventoryDlg*)window);
	GAMEIN->GetPetInventoryDialog()->Linking();
	PETMGR->SetPetInventoryDlg((CPetInventoryDlg*)window);
}

void cWindowManager::CreatePetUpgradeDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetUpgrade.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-300;  
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetUpgradeDialog((CPetUpgradeDialog*)window);
	GAMEIN->GetPetUpgradeDialog()->Linking();
}

void cWindowManager::CreatePetRevivalDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetRevival.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-window->GetWidth())-300;  
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GAMEIN->SetPetRevivalDialog((CPetRevivalDialog*)window);
	GAMEIN->GetPetRevivalDialog()->Linking();
}

void cWindowManager::CreateInventoryDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/11.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-MIDRSLTN_W);
	Pos1.y =window->GetAbsY();; // 
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	GAMEIN->SetInventoryDialog((CInventoryExDialog * )window);
	GAMEIN->GetInventoryDialog()->Linking();
}

void cWindowManager::CreateOtherCharacterEquip()
{
   	cWindow * pShowEquipInfo = WINDOWMGR->GetDlgInfoFromFile("./image/InterfaceScript/OtherCharacterEquip.bin","rb");

	WINDOWMGR->AddWindow(pShowEquipInfo);

	GAMEIN->SetShowEquipInfoDlg((CQuipInfoDlg * )pShowEquipInfo);
	GAMEIN->GetCharacterEquipInfoDlg()->Linking();

}

void cWindowManager::CreateExchangeDlg()
{
	cWindow* window = GetDlgInfoFromFile( "./image/InterfaceScript/28.bin", "rb" );
	AddWindow(window);
	GAMEIN->SetExchangeDialog( (CExchangeDialog*)window );
	GAMEIN->GetExchangeDialog()->Linking();
}


void cWindowManager::CreateRecvExchangeDlg()
{
	
}


void cWindowManager::CreateStallKindSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/StallOption.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStallKindSelectDialog((CStallKindSelectDlg * )window);
	GAMEIN->GetStallKindSelectDialog()->Linking();
}

void cWindowManager::CreateStreetBuyStallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/30.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStreetBuyStallDialog((CStreetBuyStall * )window);
	GAMEIN->GetStreetBuyStallDialog()->Linking();
}

void cWindowManager::CreateBuyRegDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/32.bin", "rb");
	AddWindow(window);
	GAMEIN->SetBuyRegDialog((CBuyRegDialog * )window);
	GAMEIN->GetBuyRegDialog()->Linking();
}

void cWindowManager::CreateStallFindDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemSearch.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStallFindDlg((CStallFindDlg * )window);
	GAMEIN->GetStallFindDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateStreetStallDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/27.bin", "rb");
	AddWindow(window);
	GAMEIN->SetStreetStallDialog((CStreetStall * )window);
	GAMEIN->GetStreetStallDialog()->Linking();
}

void cWindowManager::CreateNpcScriptDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/31.bin", "rb");
	AddWindow(window);
	GAMEIN->SetNpcScriptDialog((cNpcScriptDialog * )window);
	GAMEIN->GetNpcScriptDialog()->Linking();
}

void cWindowManager::CreateHelpDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/33.bin", "rb");
	AddWindow(window);
	GAMEIN->SetHelpDialog( (cHelpDialog*)window );
}

void cWindowManager::CreateDealDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/19.bin", "rb");
	AddWindow(window);
	GAMEIN->SetDealDialog((CDealDialog * )window);
	GAMEIN->GetDealDialog()->Linking();

	CMHFile fp;
	fp.Init("./Resource/DealItem.bin","rb");
	GAMEIN->GetDealDialog()->LoadDealerItem(&fp);
	fp.Release();

}

void cWindowManager::CreateQuestTotalDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/QuestTotal.bin", "rb");
	AddWindow(window);

	CQuestTotalDialog * pDlg = (CQuestTotalDialog * )window;
	GAMEIN->SetQuestTotalDialog(pDlg);

	CQuestDialog* pQuestDlg = (CQuestDialog*)pDlg->GetTabSheet(0);
	GAMEIN->SetQuestDialog(pQuestDlg);
	pQuestDlg->Linking();

	CWantedDialog* pWantedDlg = (CWantedDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetWantedDialog(pWantedDlg);
	pWantedDlg->Linking();
	
	CJournalDialog* pJournalDlg = (CJournalDialog*)pDlg->GetTabSheet(2);
	GAMEIN->SetJournalDialog(pJournalDlg);
	pJournalDlg->Linking();
}

void cWindowManager::CreateBailDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/bail.bin", "rb");
	AddWindow(window);

	CBailDialog* pDlg = (CBailDialog*)window;
	GAMEIN->SetBailDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateEventNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GMOpenTalk.bin", "rb");

	AddWindow(window);

	CEventNotifyDialog* pDlg = (CEventNotifyDialog*)window;
	GAMEIN->SetEventNotifyDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateDissolutionDlg()
{
	cWindow * window = GetDlgInfoFromFile( "./image/InterfaceScript/58.bin", "rb" );

	AddWindow(window);

	CDissolutionDialog* pDlg = (CDissolutionDialog*)window;
	GAMEIN->SetDissolutionDialog( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateMoveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SaveMove.bin", "rb");

	AddWindow(window);

	CMoveDialog* pDlg = (CMoveDialog*)window;
	GAMEIN->SetMoveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}



void cWindowManager::CreatePointSaveDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ChangeName.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	CPointSaveDialog* pDlg = (CPointSaveDialog*)window;
	GAMEIN->SetPointSaveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}

void cWindowManager::CreateSkPointDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointagain.bin", "rb");

	AddWindow(window);

	CSkillPointRedist* pDlg = (CSkillPointRedist*)window;
	GAMEIN->SetSkPointDlg(pDlg);
	if( pDlg )
		pDlg->Linking();
}

void cWindowManager::CreateSkPointNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointop.bin", "rb");

	AddWindow(window);

	CSkillPointNotify* pDlg = (CSkillPointNotify*)window;
	GAMEIN->SetSkPointNotifydlg(pDlg);
	if( pDlg )
	{
		pDlg->Linking();
		pDlg->InitTextArea();
	}
}


void cWindowManager::CreateShoutDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworldcreate.bin", "rb");

	AddWindow(window);

	CShoutDialog* pDlg = (CShoutDialog*)window;
	GAMEIN->SetShoutDlg(pDlg);
	pDlg->Linking();	
}


void cWindowManager::CreateShoutchatDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworld.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =window->GetAbsX();
	Pos1.y =window->GetAbsY()+((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-MIDRSLTN_H)-2;
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	CShoutchatDialog* pDlg = (CShoutchatDialog*)window;
	GAMEIN->SetShoutchatDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateChaseDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/seechase.bin", "rb");
	AddWindow(window);

	CChaseDialog* pDlg = (CChaseDialog*)window;
	GAMEIN->SetChaseDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateChaseinputDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chase.bin", "rb");

	AddWindow(window);

	CChaseinputDialog* pDlg = (CChaseinputDialog*)window;
	GAMEIN->SetChaseinputDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateNameChangeDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chnamechange.bin", "rb");

	AddWindow(window);

	CNameChangeDialog* pDlg = (CNameChangeDialog*)window;
	GAMEIN->SetNameChangeDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateNameChangeNotifyDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/opchnamechange.bin", "rb");

	AddWindow(window);

	CNameChangeNotifyDlg* pDlg = (CNameChangeNotifyDlg*)window;
	GAMEIN->SetNameChangeNotifyDlg(pDlg);
}


void cWindowManager::CreateGTRegistDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTENTRY.bin", "rb");

	AddWindow(window);

	CGTRegistDialog* pDlg = (CGTRegistDialog*)window;
	GAMEIN->SetGTRegistDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTRegistcancelDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTCANCEL.bin", "rb");

	AddWindow(window);

	CGTRegistcancelDialog* pDlg = (CGTRegistcancelDialog*)window;
	GAMEIN->SetGTRegistcancelDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTStandingDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTournament.bin", "rb");
	AddWindow(window);

	CGTStandingDialog* pDlg = (CGTStandingDialog*)window;
	GAMEIN->SetGTStandingDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTBattleListDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDViewTournament.bin", "rb");
	AddWindow(window);

	CGTBattleListDialog* pDlg = (CGTBattleListDialog*)window;
	GAMEIN->SetGTBattleListDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTScoreInfoDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/EventMapCount.bin", "rb");
	AddWindow(window);
	CGTScoreInfoDialog* pDlg = (CGTScoreInfoDialog*)window;
	GAMEIN->SetGTScoreInfoDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateSeigeWarDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWTimeReg.bin", "rb");

	AddWindow(window);


	CSWTimeRegDlg* pTimeRegDlg = (CSWTimeRegDlg*)window;
	GAMEIN->SetSWTimeRegDlg(pTimeRegDlg);
	pTimeRegDlg->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProtectReg.bin", "rb");
	AddWindow(window);

	CSWProtectRegDlg* pProRegDlg = (CSWProtectRegDlg*)window;
	GAMEIN->SetSWProtectRegDlg(pProRegDlg);
	pProRegDlg->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/SWInfo.bin", "rb");

	AddWindow(window);

	CSWInfoDlg* pInfoDlg = (CSWInfoDlg*)window;
	GAMEIN->SetSWInfoDlg(pInfoDlg);
	pInfoDlg->Linking();
	
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProfit.bin", "rb");

	AddWindow(window);

	CSWProfitDlg* pProfitDlg = (CSWProfitDlg*)window;
	GAMEIN->SetSWProfitDlg(pProfitDlg);
	pProfitDlg->Linking();	

	window = GetDlgInfoFromFile("./image/InterfaceScript/Engrave.bin", "rb");

	AddWindow(window);

	CSWEngraveDialog* pEngraveDlg = (CSWEngraveDialog*)window;
	GAMEIN->SetSWEngraveDlg(pEngraveDlg);
	pEngraveDlg->Linking();	


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWCount.bin", "rb");

	AddWindow(window);

	CSWTimeDialog* pSWTimeDlg = (CSWTimeDialog*)window;
	GAMEIN->SetSWTimeDlg(pSWTimeDlg);
	pSWTimeDlg->Linking();	


	window = GetDlgInfoFromFile("./image/InterfaceScript/SWStartTime.bin", "rb");

	AddWindow(window);

	CSWStartTimeDialog* pSTDlg = (CSWStartTimeDialog*)window;
	GAMEIN->SetSWStartTimeDlg(pSTDlg);
	pSTDlg->Linking();	

	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarEngraveDlg.bin", "rb");

	AddWindow(window);

	CFWEngraveDialog* pFWEngraveDlg = (CFWEngraveDialog*)window;
	GAMEIN->SetFWEngraveDlg(pFWEngraveDlg);
	pFWEngraveDlg->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarTimeDlg.bin", "rb");

	AddWindow(window);

	CFWTimeDialog* pFWTimeDlg = (CFWTimeDialog*)window;
	GAMEIN->SetFWTimeDlg(pFWTimeDlg);
	pFWTimeDlg->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarWareHouseDlg.bin", "rb");

	AddWindow(window);

	CFWWareHouseDialog* pFWWareHouseDlg = (CFWWareHouseDialog*)window;
	GAMEIN->SetFWWareHouseDlg(pFWWareHouseDlg);
	pFWWareHouseDlg->Linking();
}

void cWindowManager::CreateCharChangeDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/CharChange.bin", "rb");

	AddWindow(window);

	CCharChangeDlg* pDlg = (CCharChangeDlg*)window;
	GAMEIN->SetCharChangeDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateItemSealDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLock.bin", "rb");

	AddWindow(window);

	CSealDialog* pDlg = (CSealDialog*)window;
	GAMEIN->SetSealDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateItemLockDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLockEx.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	CItemLockDialog* pDlg = (CItemLockDialog*)window;

	GAMEIN->SetItemLockDialog( pDlg );

	pDlg->Linking();

}

void cWindowManager::CreateItemUnLockDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/ItemUnLockEx.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	CItemUnLockDialog* pDlg = (CItemUnLockDialog*)window;

	GAMEIN->SetItemUnLockDialog( pDlg );

	pDlg->Linking();

}

void cWindowManager::CreateChangeJobDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/Changejob.bin", "rb");

	AddWindow(window);

	CChangeJobDialog* pDlg = (CChangeJobDialog*)window;
	GAMEIN->SetChangeJobDlg( pDlg );
}

void cWindowManager::CreateReinforceResetDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/ReinforceDefault.bin", "rb");

	AddWindow(window);

	if( window )
	{
		CReinforceResetDlg* pDlg = (CReinforceResetDlg*)window;
		pDlg->Linking();
		GAMEIN->SetReinforceResetDlg( pDlg );
	}
}

void cWindowManager::CreateRareCreateDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/RareCreate.bin", "rb");

	AddWindow(window);

	CRareCreateDialog* pDlg = (CRareCreateDialog*)window;
	pDlg->Linking();
	GAMEIN->SetRareCreateDialog( pDlg );
}

void cWindowManager::CreateReinforceDataGuideDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/RFDataGuide.bin", "rb");

	AddWindow(window);

	CReinforceDataGuideDlg* pDlg = (CReinforceDataGuideDlg*)window;
	pDlg->Linking();
	GAMEIN->SetReinforceDataGuideDlg( pDlg );
}

void cWindowManager::CreateTipBrowserDlgDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/TipBrowser.bin", "rb");

	AddWindow(window);

	CTipBrowserDlg* pDlg = (CTipBrowserDlg*)window;
	pDlg->Linking();
	GAMEIN->SetTipBrowserDlg( pDlg );
}

void cWindowManager::CreateKeySettingTipDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/KeySetting.bin", "rb");

	AddWindow(window);

	CKeySettingTipDlg* pDlg = (CKeySettingTipDlg*)window;
	pDlg->Linking();
	GAMEIN->SetKeySettingTipDlg( pDlg );
}

void cWindowManager::CreateGuildNoteDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNote.bin", "rb");

	AddWindow(window);

	CGuildNoteDlg* pDlg = (CGuildNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoteDlg( pDlg );
}

void cWindowManager::CreateUnionNoteDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/AllyNote.bin", "rb");

	AddWindow(window);

	CUnionNoteDlg* pDlg = (CUnionNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetUnionNoteDlg( pDlg );
}

void cWindowManager::CreateGuildNoticeDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNotice.bin", "rb");

	AddWindow(window);

	CGuildNoticeDlg* pDlg = (CGuildNoticeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoticeDlg( pDlg );
}
/////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateSkillOptionChangeDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/SkillTrans.bin", "rb");

	AddWindow(window);

	CSkillOptionChangeDlg* pDlg = (CSkillOptionChangeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionChangeDlg( pDlg );
}

void cWindowManager::CreateSkillOptionClearDlg()
{
	cWindow* window = NULL;

	window = GetDlgInfoFromFile("./image/InterfaceScript/TransDefault.bin", "rb");

	AddWindow(window);

	CSkillOptionClearDlg* pDlg = (CSkillOptionClearDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionClearDlg( pDlg );
}

void cWindowManager::ScreenShotDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/screenshotdlg.bin", "rb");

	AddWindow(window);

	CScreenShotDlg* pDlg = (CScreenShotDlg*)window;
	GAMEIN->SetScreenShotDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateUniqueItemMixProgressBarDlg()
{	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemProgressBarDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetUniqueItemMixProgressBarDlg((CUniqueItemMixProgressBarDlg * )window);
	GAMEIN->GetUniqueItemMixProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanMixProgressBarDlg()
{	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanmixProgressBarDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetTitanMixProgressBarDlg((CTitanMixProgressBarDlg * )window);
	GAMEIN->GetTitanMixProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateTitanPartsProgressBarDlg()
{	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsProgressBarDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetTitanPartsProgressBarDlg((CTitanPartsProgressBarDlg * )window);
	GAMEIN->GetTitanPartsProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateSkinSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SkinSelectDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetSkinSelectDlg((CSkinSelectDialog * )window);
	GAMEIN->GetSkinSelectDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateCostumeSkinSelectDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/CostumeSkinSelectDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetCostumeSkinSelectDlg((CCostumeSkinSelectDialog * )window);
	GAMEIN->GetCostumeSkinSelectDlg()->Linking();
}
/////////////////////////////////////////////////////////////////////////
void cWindowManager::CreateSkillPointResetDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SkillPointReset.bin", "rb");

	AddWindow(window);
	GAMEIN->SetSkillPointResetDlg( (CSkillPointResetDlg*)window );
	GAMEIN->GetSkillPointResetDlg()->Linking();
}


void cWindowManager::CreateStreetStallItemViewDlg()
{
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SearchChr.bin", "rb");

	AddWindow(window);
	GAMEIN->SetStreetStallItemViewDlg( (CStreetStallItemViewDlg*)window );
	GAMEIN->GetStreetStallItemViewDlg()->Linking();
}

void cWindowManager::CreatePartyMatchingDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyMatchingDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetPartyMatchingDlg((CPartyMatchingDlg * )window);
	GAMEIN->GetPartyMatchingDlg()->Linking();
}

void cWindowManager::AfterInit()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetType() & WT_ICONDIALOG || win->GetType() & WT_ICONGRIDDIALOG)
			m_pIconDlgListRef->AddTail(win);
		else if(win->GetType() & WT_TABDIALOG)
		{
			cTabDialog * tabDlg = (cTabDialog *)win;

			int i = 0;
			while(i < tabDlg->GetTabNum())
			{
				if(tabDlg->GetTabSheet(i)->GetType() & WT_ICONDIALOG || tabDlg->GetTabSheet(i)->GetType() & WT_ICONGRIDDIALOG)
					m_pIconDlgListRef->AddTail(tabDlg->GetTabSheet(i));
				i++;
			}
		}
		PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
		while(pos2)
		{
			cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
			if(win2->GetType() == WT_EDITBOX)
			{
				m_pEditBoxListRef->AddTail((void *)win2);
			}
			else if(win2->GetType() & WT_ICONDIALOG || win2->GetType() & WT_ICONGRIDDIALOG)
			{
				m_pIconDlgListRef->AddTail(win2);
			}
		}
	}
}
//#include "ReDisConnManager.h"
void cWindowManager::KeyboardInput( CKeyboard* keyInfo )
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	DWORD Action = WE_NULL;

	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);
		//if(RECONNMGR->IsInReCnnMode()&&window->GetType() != WT_MSGBOX){m_pWindowList->GetPrev(pos);continue;}
		if(window->IsActive() && !window->IsDepend() )
		{
			Action = window->ActionKeyboardEvent( keyInfo );

			if( Action & WE_CLOSEWINDOW )
				window->SetActive(FALSE);

			if( Action != WE_NULL ) break;	
		}

		m_pWindowList->GetPrev(pos);
	}
}

void cWindowManager::SetNextEditFocus()
{
	if( m_pFocusedEdit )
	{
		cEditBox * win;
		PTRLISTPOS posCur = m_pEditBoxListRef->Find( m_pFocusedEdit );
		PTRLISTPOS pos = posCur;

		m_pEditBoxListRef->GetNext(pos);
		
		while( pos != posCur )
		{
			if( pos == NULL )
				pos = m_pEditBoxListRef->GetHeadPosition();

			win = (cEditBox *)m_pEditBoxListRef->GetNext(pos);
			
			if( win->IsActive() && win->GetParent() == m_pFocusedEdit->GetParent() )
			{
				m_pFocusedEdit->SetFocus( FALSE );
				win->SetFocusEdit( TRUE );
				break;
			}
		}
	}
}
void cWindowManager::BackDragWindow(LONG id)
{
	cDialog * srcDlg = GetWindowForID(id);
	if (!srcDlg) return;
	srcDlg->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);
	DragWindowNull();
}
void cWindowManager::BackDragWindow()
{
	cDialog * srcDlg = GetWindowForID(m_id);
	if(!srcDlg) return;
	srcDlg->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);
	DragWindowNull();
}

void cWindowManager::Preprocess()
{
	if( m_OldDragFlag )
	{
		BOOL LeftUp = MOUSE->LButtonUp();
		
		if( LeftUp )
		{
			LONG ClickUpX = MOUSE->GetMouseEventX();
			LONG ClickUpY = MOUSE->GetMouseEventY();

			if( cbDragnDropProcess )
				cbDragnDropProcess( ClickUpX, ClickUpY, m_id, (LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);

			SetDragEnd();
		}
		else if( m_pDragDialog )
		{
			LONG x = MOUSE->GetMouseX();
			LONG y = MOUSE->GetMouseY();

			m_pDragDialog->SetAbsXY( x-m_OldX, y-m_OldY );
			RECT rect = { 0, 0, GET_MAINWIN_W,GET_MAINWIN_H}; 
			rect.right = GET_MAINWIN_W;
			rect.bottom = GET_MAINWIN_H;
			VECTOR2 magPos;
			magPos.x = m_pDragDialog->GetAbsX();
			magPos.y = m_pDragDialog->GetAbsY();
			if( cAnimationManager::MagProcess( &rect, (cWindow*)m_pDragDialog, &magPos ) )
				m_pDragDialog->SetAbsXY( magPos.x, magPos.y );
		}
	}
}

void cWindowManager::AddListDestroyWindow(cWindow * window)

{
	if( window == NULL ) return;

	PTRLISTSEARCHSTART((*m_pDestroyWindowRef),cWindow*,pWindow)
		if(pWindow == window)
			return;
	PTRLISTSEARCHEND

	m_pDestroyWindowRef->AddTail(window);
	window->SetDestroy();
}

void cWindowManager::DestroyWindowProcess()
{
	PTRLISTPOS pos = m_pDestroyWindowRef->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pDestroyWindowRef->GetNext(pos);
		DeleteWindow( window );
	}
	m_pDestroyWindowRef->RemoveAll();
}

void cWindowManager::OnWindowEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_CLOSEWINDOW )
	{
		GAMEIN->GetMainInterfaceDialog()->PushMenuID( lId, FALSE );
	}
}

void cWindowManager::Process()
{
	ANIMGR->FadeProcess();
 
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		window->ActionEvent( MOUSE );
		
		if( !window->IsActive() )
			m_pPlayerChatTooltip->RemoveAt(pos);

		m_pPlayerChatTooltip->GetNext(pos);
	}
}

void cWindowManager::MouseInput( CMouse* pMouse )
{
	m_bMouseOverUsed	= FALSE;
	m_bMouseDownUsed	= FALSE;
	m_bMouseUpUsed		= FALSE;
	cWindow* pMouseOverWindow = NULL;
	m_MouseInputProcessed = FALSE;
	DestroyWindowProcess();
	Preprocess();
	
	cWindow * TopWindow = NULL;
	PTRLISTPOS TopPos = NULL;
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);
		LONG Action=0;
		if( window->IsActive() )
		{
			if( !window->IsDepend() )
				Action = window->ActionEvent( pMouse );			
		}
		if((Action & WE_CLOSEWINDOW))
			window->SetActive(FALSE);
		else if(Action & WE_TOPWINDOW)
		{
			TopPos = pos;
			TopWindow = window;
		}
		if( Action & WE_LBTNCLICK || Action & WE_RBTNCLICK || 
			Action & WE_LBTNDBLCLICK || Action & WE_RBTNDBLCLICK)
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseDownUsed		= TRUE;
		}
		if( Action & WE_MOUSEOVER )
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseOverUsed		= TRUE;
			pMouseOverWindow		= window;
		}
		m_pWindowList->GetPrev(pos);
	}

	if(TopWindow && TopPos)
	{
		m_pWindowList->RemoveAt(TopPos);		
		
		if( TopWindow->IsAlwaysTop() )
			m_pWindowList->AddTail(TopWindow);	
		else
		{
			pos = m_pWindowList->GetTailPosition();	
			
			while( pos )
			{
				cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
				if( pWindow )
				if( !pWindow->IsAlwaysTop() )	break;
				m_pWindowList->GetPrev( pos );
			}

			if( pos )
				m_pWindowList->InsertAfter( pos, TopWindow );
			else 
				m_pWindowList->AddHead( TopWindow );
		}
	}
	
	if(m_pGuageWindow)
	{
		m_pGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMonsterGuageWindow)
	{
		m_pMonsterGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMousePointWindow)
	{
		m_pMousePointWindow->SetAbsXY(pMouse->GetMouseX(), pMouse->GetMouseY());
	}
}


void cWindowManager::Render()
{
	PTRLISTPOS pos = NULL;
	if (m_pScreenTextList != NULL)
	{
		pos = m_pScreenTextList->GetHeadPosition();
		while (pos)
		{
			cMultiLineText* mlwindow = (cMultiLineText*)m_pScreenTextList->GetNext(pos);
			mlwindow->Render();
		}
	}
	
	if (m_pPlayerChatTooltip != NULL)
	{
		pos = m_pPlayerChatTooltip->GetHeadPosition();

		while (pos)
		{
			cWindow* window = (cWindow*)m_pPlayerChatTooltip->GetNext(pos);
			window->Render();
		}
	}
	

	SetToolTipWindow( NULL );
	SetRelationTipWindow(NULL);
	SetRelationTipWindow1(NULL);
	if (m_pWindowList != NULL)
	{
		pos = m_pWindowList->GetHeadPosition();
		if (m_OldDragFlag)
		{
			BOOL bRenderDragDlg = FALSE;

			while (pos)
			{
				cWindow* window = (cWindow *)m_pWindowList->GetNext(pos);

				if (window->IsAlwaysTop())
					if (window->IsActive())
						if (!bRenderDragDlg)
							if (!m_pDragDialog->IsAlwaysTop())
							{
								m_pDragDialog->Render();
								bRenderDragDlg = TRUE;
							}

				if (!window->IsDepend() && window->GetID() != m_id)
					window->Render();
			}

			if (!bRenderDragDlg && m_pDragDialog)
				m_pDragDialog->Render();
		}
		else
		{
			while (pos)
			{
				cWindow* window = (cWindow *)m_pWindowList->GetNext(pos);
				if (!window->IsDepend())
					window->Render();
			}
		}
	}
	
	STATUSICONDLG->Render();


	if( !m_OldDragFlag )	
	if( m_pToolTipWindow )
	{
		m_pToolTipWindow->SetShowCurItem(FALSE);
		m_pToolTipWindow->Render();
	}
	if(m_pRelationTip)
	{
		m_pRelationTip->Render();
	}
	if(m_pRelationTip1)
	{
		m_pRelationTip1->Render();
	}

	if(m_pGuageWindow)
		m_pGuageWindow->Render();
	if(m_pMonsterGuageWindow)
		m_pMonsterGuageWindow->Render();
	if(m_pMousePointWindow)
		m_pMousePointWindow->Render();

	if( MAP->IsMapKind(eSurvivalMap) )
	{
		SVVMODEMGR->Render();
	}
	if(m_pSendItemInfo)
	{
		m_pSendItemInfo->Render();
	}
	
}

void cWindowManager::DestroyWindowAll()
{
	if(!m_pWindowList)return;

	DestroyWindowProcess();
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);

		SAFE_DELETE(win);
	}
	m_pWindowList->RemoveAll();

	if(m_pGuageWindow)
	{
		SAFE_DELETE(m_pGuageWindow);
	}
	if(m_pMonsterGuageWindow)
	{
		SAFE_DELETE(m_pMonsterGuageWindow);
	}
	
	DeleteEditBoxListAll();
	DeleteIconDlgListAll();
	DeleteChatTooltipListAll();

	m_pDragDialog = NULL;
}
void cWindowManager::CloseWindowForID(LONG id)
{
	if(!m_pWindowList) return;
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
		{
			win->SetActive(FALSE);
			break;
		}
	}
}
void cWindowManager::DeleteIconDlgListAll()
{
	if(!m_pIconDlgListRef)return;
	m_pIconDlgListRef->RemoveAll();
}
void cWindowManager::DeleteEditBoxListAll()
{
	if(!m_pEditBoxListRef)return;
	m_pEditBoxListRef->RemoveAll();
}
void cWindowManager::DeleteChatTooltipListAll()
{
	if(!m_pPlayerChatTooltip)return;
	m_pPlayerChatTooltip->RemoveAll();
}

void cWindowManager::DeleteWindowForID(LONG id)
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win->GetID() == id )
		{
			m_pWindowList->RemoveAt(pos);
			SAFE_DELETE(win);
			if( m_id == id ) SetDragEnd();
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}


void cWindowManager::DeleteWindow( cWindow* pWindow )
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow* win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win == pWindow )
		{
			m_pWindowList->RemoveAt(pos);
			if( m_id == win->GetID() ) SetDragEnd();
			SAFE_DELETE(win);
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}

void cWindowManager::RemoveChatTooltipWindow(LONG playerID)
{
	if(!m_pPlayerChatTooltip) return;
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		if(win->GetID() == playerID)
		{
			m_pPlayerChatTooltip->RemoveAt(pos);
			break;
		}
		m_pPlayerChatTooltip->GetNext(pos);
	}
	return ;
}

void cWindowManager::RemoveChatTooltipWindow( cWindow* pWindow )
{
	if(m_pPlayerChatTooltip)
		m_pPlayerChatTooltip->Remove( pWindow );
}

cDialog * cWindowManager::GetWindowForID(LONG id)
{

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
			return win;
	}
	return NULL;
}


cWindow * cWindowManager::GetWindowForIDEx(LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
			return (cWindow *)win;
		else
		{
			PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
			while(pos2)
			{
				cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
				if(win2->GetID() == id)
				{
					return win2;
				}
			}
		}
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXYExceptIcon(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetPrev(pos);
		if(win->IsActive() == FALSE || win->GetType() & WT_ICON)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXYExceptID(LONG x, LONG y, LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)

	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id || win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cIconDialog * cWindowManager::GetIconDialogForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pIconDlgListRef->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pIconDlgListRef->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return (cIconDialog *)win;
	}
	return NULL;
}


void cWindowManager::SetOptionAlpha( DWORD dwAlpha )
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		win->SetOptionAlpha( dwAlpha );
	}
}
cMsgBox* cWindowManager::MsgBoxTip(LONG lMBId, int nMBType, char* pStrMsg, ...)
{
	if (!cMsgBox::IsInit()) return NULL;

	if (WINDOWMGR->GetWindowForID(lMBId) != NULL)
		return NULL;

	cMsgBox* pMsgBox = new cMsgBox;

	char msg[256] = { 0, };

	if (pStrMsg != NULL)
	{
		va_list argList;

		va_start(argList, pStrMsg);
		vsprintf(msg, pStrMsg, argList);
		va_end(argList);
	}

	pMsgBox->MsgBox(lMBId, nMBType, msg, MessageBox_Func, 1, true);
	pMsgBox->SetAlwaysTop(TRUE);
	AddWindow(pMsgBox);

	LONG lX = (LONG)pMsgBox->GetAbsX();
	LONG lY = (LONG)pMsgBox->GetAbsY();
	PositioningDlg(lX, lY);
	pMsgBox->SetAbsXY(lX, lY);
	pMsgBox->SetActive(TRUE);

	return pMsgBox;
}
cMsgBox* cWindowManager::MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... )
{
	if( !cMsgBox::IsInit() ) return NULL;

	if( WINDOWMGR->GetWindowForID( lMBId ) != NULL )
		return NULL;
	
	cMsgBox* pMsgBox = new cMsgBox;

	char msg[256] = {0,};

	if( pStrMsg != NULL ) 
	{
		va_list argList;

		va_start(argList, pStrMsg);
		vsprintf(msg, pStrMsg, argList);
		va_end(argList);
	}

	pMsgBox->MsgBox( lMBId, nMBType, msg, MessageBox_Func );
	pMsgBox->SetAlwaysTop( TRUE );	
	AddWindow( pMsgBox );

	LONG lX = (LONG)pMsgBox->GetAbsX();
	LONG lY = (LONG)pMsgBox->GetAbsY();
	PositioningDlg( lX, lY );
	pMsgBox->SetAbsXY( lX, lY );
	pMsgBox->SetActive( TRUE );

	return pMsgBox;
}

void cWindowManager::PositioningDlg( LONG& rX, LONG& rY )
{
	int nCount = 0;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while( pos )
	{
		cWindow* pWindow = (cWindow*)m_pWindowList->GetNext( pos );
		if( (LONG)pWindow->GetAbsX() == rX )
		if( (LONG)pWindow->GetAbsY() == rY )
		{
			rX += 20;
			rY += 20;
			if( ++nCount >= 10 )
			{
				rX -= 200;
				rY -= 200;
				nCount = 0;
			}
		}
	}
}

cDivideBox * cWindowManager::DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char * strTitle )
{
	if( WINDOWMGR->GetWindowForID( lId ) != NULL )
		return NULL;

	cDivideBox * window = (cDivideBox * )GetDlgInfoFromFile("./image/InterfaceScript/DivideBox.bin", "rb" );
	
	window->CreateDivideBox( x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle );
	window->SetActive( TRUE );
	window->SetAlwaysTop( TRUE );

	AddWindow( window );

	return window;
}


cDivideBoxEx * cWindowManager::DivideBoxEx( LONG lId, LONG x, LONG y, cbDivideExFUNC cbFc1, cbDivideExFUNC cbFc2, void * vData1, void* vData2, char * strTitle )
{
	if( WINDOWMGR->GetWindowForID( lId ) != NULL )
		return NULL;

	cDivideBoxEx * window = (cDivideBoxEx * )GetDlgInfoFromFile("./image/InterfaceScript/DivideBoxEx.bin", "rb" );
	
	window->CreateDivideBox( x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle );
	window->SetActive( TRUE );
	window->SetAlwaysTop( TRUE );

	AddWindow( window );

	return window;
}


void cWindowManager::SetWindowTop( cWindow* win )
{
	PTRLISTPOS pos = m_pWindowList->Find( win );

	if( pos == NULL ) return;

	m_pWindowList->RemoveAt( pos );
	
	if( win->IsAlwaysTop() )
		m_pWindowList->AddTail(win);					
	else
	{
		pos = m_pWindowList->GetTailPosition();	
		
		while( pos )
		{
			cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
			if( pWindow )
			if( !pWindow->IsAlwaysTop() )	break;

			m_pWindowList->GetPrev(pos);
		}
		
		if( pos )
			m_pWindowList->InsertAfter( pos, win );
		else 
			m_pWindowList->AddHead( win );
	}
}

void cWindowManager::SetFocusedEdit( cWindow* pEdit, BOOL bFocus )
{
	if( bFocus )
	{
		if( m_pFocusedEdit == pEdit ) return;
//
//#ifdef _HACK_SHIELD_
//		HS_PauseService();
//#endif
//
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#elif defined _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#elif defined _JAPAN_LOCAL_	
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//
//		if( m_pFocusedEdit == NULL && m_bOpenStatus )
//		{
//			ImmSetOpenStatus( hIMC, TRUE );
//		}
//
//		if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
//		{
//			if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
//			{
//				m_bOpenStatus = ImmGetOpenStatus( hIMC );
//				ImmSetOpenStatus( hIMC, FALSE );
//			}
//		}
//
//		ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//		ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//
//		ImmReleaseContext( _g_hWnd, hIMC );
//#endif

		m_pFocusedEdit = pEdit;
	}
	else
	{

//#ifndef _JAPAN_LOCAL_
		if( m_pFocusedEdit == pEdit )
		{
			m_pFocusedEdit = NULL;

//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
//#else
//		
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//
//		if( m_pFocusedEdit == pEdit )
//		{
//			m_pFocusedEdit = NULL;
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//
//			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
//			{
//				if( ((cEditBox*)pEdit)->GetValidCheckMethod() != VCM_NUMBER )
//				{
//					m_bOpenStatus = ImmGetOpenStatus( hIMC );
//				}
//
//				ImmSetOpenStatus( hIMC, FALSE );
//			}
//			else
//			{
//				m_bOpenStatus = ImmGetOpenStatus( hIMC );
//				ImmSetOpenStatus( hIMC, FALSE );
//			}
//		}
//		else
//		{
//			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
//			{
//				if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
//				{
//					ImmSetOpenStatus( hIMC, m_bOpenStatus );
//				}
//			}
//		}
//
//		ImmReleaseContext( _g_hWnd, hIMC );
//#endif

	}
}


BOOL cWindowManager::CloseAllAutoCloseWindows()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	cWindow* pWin;
	BOOL bClose = FALSE;

	while(pos)
	{
		pWin = (cWindow*)m_pWindowList->GetNext(pos);

		if( pWin->IsAutoClose() && pWin->IsActive() )
		{
			pWin->SetActive( FALSE );
			bClose = TRUE;
		}
	}

	return bClose;
}


cWindow* cWindowManager::GetDlgInfoFromFile (char * filePath, char* mode )
{
	return m_pScriptManager->GetDlgInfoFromFile(filePath, mode);
}


cMsgBox* cWindowManager::GetFirstMsgBox()
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetPrev(pos);
		if( !window ) continue;
		
		if( window->GetType()  == WT_MSGBOX )
			return (cMsgBox*)window;
	}

	return NULL;
}

void cWindowManager::CloseAllMsgBox()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			((cMsgBox*)pWindow)->ForceClose();
		}
	PTRLISTSEARCHEND	
}

void cWindowManager::CloseAllMsgBoxNoFunc()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			AddListDestroyWindow( pWindow );			
		}
	PTRLISTSEARCHEND	
}



BOOL cWindowManager::CloseAllWindow()
{
	GAMEIN->GetGuageDlg()->SetActive(FALSE);
	BOOL rt = FALSE;
	PTRLISTSEARCHSTART((*m_pWindowList),cWindow*,pWindow)
		if(pWindow->IsActive() == TRUE)
		{
			rt = TRUE;
			pWindow->SetActive(FALSE);
			m_bHideInterface = TRUE;
		}
	PTRLISTSEARCHEND

	return rt;
}
void cWindowManager::ShowBaseWindow()
{
	GAMEIN->GetChatDialog()->SetActive(TRUE);
	GAMEIN->GetGuageDlg()->SetActive(TRUE);
	GAMEIN->GetMainInterfaceDialog()->SetActive(TRUE);
	GAMEIN->GetQuickDialog()->SetActive(TRUE);
	m_bHideInterface = FALSE;
}
void cWindowManager::ToggleShowInterface()
{
	if(CloseAllWindow() == FALSE)
		ShowBaseWindow();
}

void cWindowManager::CreateGuildInvitationKindSelectionDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/JoinOption.bin", "rb");

	AddWindow(window);

	CGuildJoinDialog* pDlg = (CGuildJoinDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildJoinDialog( pDlg );
}


void cWindowManager::CreateGuildMunhaDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTraineeInfo.bin", "rb");

	AddWindow(window);

	CGuildMunhaDialog* pDlg = (CGuildMunhaDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildMunhaDialog( pDlg );
}


void cWindowManager::CreateGuildTraineeDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTrainee.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	CGuildTraineeDialog* pDlg = (CGuildTraineeDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildTraineeDialgo( pDlg );
}


void cWindowManager::CreateSurvivalCountDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SurvivalDlg.bin", "rb");

	AddWindow(window);

	CSurvivalCountDialog* pDlg = (CSurvivalCountDialog*)window;
	GAMEIN->SetSurvivalCountDialog( pDlg );
	pDlg->Linking();
	GAMEIN->GetSurvivalCountDlg()->InitSurvivalCountDlg(MAP->GetMapNum());
}

void cWindowManager::CreateUniqueItemCurseCancellationDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemCurseCancellation.bin", "rb");

	AddWindow(window);
	GAMEIN->SetUniqueItemCurseCancellationDlg( (CUniqueItemCurseCancellationDlg*)window );
	GAMEIN->GetUniqueItemCurseCancellationDlg()->Linking();
}

void cWindowManager::CreateUniqueItemMixDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemMixDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetUniqueItemMixDlg( (CUniqueItemMixDlg*)window );
	GAMEIN->GetUniqueItemMixDlg()->Linking();
}

void cWindowManager::CreateSOSDlg()
{
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SOScall.bin", "rb");

	AddWindow(window);
	GAMEIN->SetSOSDlg( (CSOSDlg*)window );
	GAMEIN->GetSOSDlg()->Linking();


	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoNoteDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetAutoNoteDlg( (CAutoNoteDlg*)window );
	GAMEIN->GetAutoNoteDlg()->Linking();

	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoAnswerDlg.bin", "rb");

	AddWindow(window);
	GAMEIN->SetAutoAnswerDlg( (CAutoAnswerDlg*)window );
	GAMEIN->GetAutoAnswerDlg()->Linking();

}



void cWindowManager::CreateRechargeDlg()
{//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/RechargeDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	cRechargeManager* pDlg = (cRechargeManager*)window;
	GAMEIN->SetRechargeDlg(pDlg);
	GAMEIN->GetRechargeDlg()->Linking();
}
void cWindowManager::CreateSingedDlg()				
{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SingedDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	cSingedDialog* pDlg = (cSingedDialog*)window;
	GAMEIN->SetSingedDlg(pDlg);
	GAMEIN->GetSingedDlg()->Linking();
}
void cWindowManager::CreateOpenPyogukDlg()
{//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2018/6/8]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/OpenPyogukDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	OpenPyoguk* pDlg = (OpenPyoguk*)window;
	GAMEIN->SetOpenPyogukDlg(pDlg);
}

void cWindowManager::CreateSetingInfoDlg()
{//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SetingInfoDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth()) - 150;
	Pos1.y =-1;
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	cSetingInfoDlg* pDlg = (cSetingInfoDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSetingInfoDlg(pDlg);
}
void cWindowManager::CreateStatusIconBtnDlg()
{//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/StatusIconBtnDlgDlg.bin", "rb");

	AddWindow(window);
	cStatusIconBtnDlg* pDlg = (cStatusIconBtnDlg*)window;
	pDlg->Linking();
	GAMEIN->SetStatusIconBtnDlg(pDlg);
}
void cWindowManager::CreateTopRankDlg()
{//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/TopRankDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth()) - 110;
	Pos1.y =220;
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	cTopRankDlg * pDlg = (cTopRankDlg *)window;
	pDlg->Linking();
	GAMEIN->SetTopRankDlg(pDlg);
}
void cWindowManager::CreateInventoryRBTDlg()
{//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/InventoryRBTDlg.bin", "rb");

	AddWindow(window);
	cInventoryRBTDlg* pDlg = (cInventoryRBTDlg*)window;
	pDlg->Linking();
	GAMEIN->SetInventoryRBTDlg(pDlg);
}
void cWindowManager::CreateSendItemInfoDlg()		
{//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SendItemInfoDlg.bin", "rb");
	AddWindow(window);
	cSendItemInfoDlg* pDlg = (cSendItemInfoDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSendItemInfoDlg(pDlg);
}
void cWindowManager::CreateSafeLockDlg()
{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SafeLockDlg.bin", "rb");

	AddWindow(window);
	cSafeLockDialog* pDlg = (cSafeLockDialog*)window;
	pDlg->Linking();
	GAMEIN->SetSafeLockDlg(pDlg);
}
void cWindowManager::CreateMainDlgSafeBtn()
{//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/MianDlgSafeBtn.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth()) - 52;
	Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - window->GetWidth()) - 57;
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	cMainDlgSafeBtn* pDlg = (cMainDlgSafeBtn*)window;
	pDlg->Linking();
	GAMEIN->SetMianDlgSafeBtn(pDlg);
}
void cWindowManager::CreateSafeLockMainDlg()
{//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/SafeLockMainDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	cSafeLockMainDlg* pDlg = (cSafeLockMainDlg*)window;
	GAMEIN->SetSafeLockMainDlg(pDlg);
}
void cWindowManager::CreateMainDlgHideBtnDlg()		
{//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/MainDlgHideBtnDlg.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - 67);
	Pos1.y =210;
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
}
#ifdef _XDYY_
void cWindowManager::CreateOnlineVoiceDlg()
{//[小队语音设置界面][By:十里坡剑神][QQ:112582793][2019/3/20][14:57:12]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/OnlineVoice.bin", "rb");
	AddWindow(window);
	cOnlineVoiceDlg* pDlg = (cOnlineVoiceDlg*)window;
	pDlg->Linking();
	GAMEIN->SetOnlineVoiceDlg(pDlg);
}
#endif
void cWindowManager::CreateOnlineFame()
{//[在线洗恶][By:十里坡剑神][QQ:112582793][2017/11/22]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/OnlineFame.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - window->GetWidth()) - 100;
	Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - window->GetHeight()) - 48;
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	OnlineFame* pDlg = (OnlineFame*)window;
	pDlg->Linking();
	GAMEIN->SetOnlineFameDlg(pDlg);
}
void cWindowManager::CreateBillboardDialog()
{//[角色排行窗口创建][By:十里坡剑神][QQ:112582793][2017/11/26]

	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Billboard.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);

	cBillBoardDialog* pDlg = (cBillBoardDialog*)window;

	GAMEIN->SetBillboardDialog(pDlg);

	GAMEIN->GetBillboardDialog()->Linking();
}
void cWindowManager::CreateGuildTopListDlg()
{//[帮会排行][By:十里坡剑神][QQ:112582793][2017/11/24]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/BillboardGuild.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	GuildTopList* pDlg = (GuildTopList*)window;
	pDlg->Linking();
	GAMEIN->SetGuildTopListDlg(pDlg);
}
void cWindowManager::CreateKillTopListDlg()	
{//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/BillboardKill.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	cKillTopList* pDlg = (cKillTopList*)window;
	pDlg->Linking();
	GAMEIN->SetKillTopListDlg(pDlg);
}
void cWindowManager::LoadUserIDInfo()
{//[账号密码记录读取][By:十里坡剑神][QQ:112582793][2017/11/23]
	FILE* fp;
	fp = fopen("./Data/UserCfgInfo.dat","rb");
	if(fp)
	{
		fread(&sKeepLoginInfo,sizeof(KEEPLOGIN),1,fp);
		fclose(fp);
		int i=0;
		for(;i<strlen(sKeepLoginInfo.ID);i++)
			sKeepLoginInfo.ID[i] -= 'c';
		int j = 0;
		for (;j<strlen(sKeepLoginInfo.Pwd);j++)
			sKeepLoginInfo.Pwd[j]  -= 'c';
	}
}
void cWindowManager::SaveUserIDInfo()
{//[账号密码记录保存][By:十里坡剑神][QQ:112582793][2017/11/23]
	FILE* fp;
	fp = fopen("./Data/UserCfgInfo.dat","w");
	KEEPLOGIN tmp;
	memcpy(&tmp,&sKeepLoginInfo,sizeof(KEEPLOGIN));
	if(fp)
	{
		int i=0;
		for(;i<strlen(tmp.ID);i++)
			tmp.ID[i] += 'c';
		int j = 0;
		for (;j<strlen(tmp.Pwd);j++)
			tmp.Pwd[j]  += 'c';
		fwrite(&tmp,sizeof(KEEPLOGIN),1,fp);
		fclose(fp);
	}
}
void cWindowManager::SetUserCfg(BOOL IsKeep,char * id,char * pwd)
{//[设置记住账号密码][By:十里坡剑神][QQ:112582793][2017/11/23]
	sKeepLoginInfo.IsKeep = IsKeep;
	sprintf(sKeepLoginInfo.ID,id);
	sprintf(sKeepLoginInfo.Pwd,pwd);
	if (IsKeep)
		SaveUserIDInfo();
	else
		DeleteFile("./Data/UserCfgInfo.dat");
}
void cWindowManager::CreateCharacterRBTN()
{//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/CharacterRBTN.bin", "rb");

	AddWindow(window);

	CharacterRBTN* pDlg = (CharacterRBTN*)window;
	pDlg->Linking();
	GAMEIN->SetCharacterRBTN(pDlg);
}
void cWindowManager::CreateFBTipDlg()
{//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2018/6/8]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/FBTip.bin", "rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-(150+window->GetWidth()));
	Pos1.y =window->GetAbsY();
	window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	FBTipDlg* pDlg = (FBTipDlg*)window;
	pDlg->Linking();
	GAMEIN->SetFBTipDlg( pDlg );
}

void cWindowManager::CreateVideoCaptureDlg()
{//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	CVideoCaptureDlg* dialog = ( CVideoCaptureDlg* )GetDlgInfoFromFile("./image/InterfaceScript/VideoCaptureDlg.bin", "rb" );
	if( ! dialog )
	{
		return;
	}

	AddWindow( dialog );
	GAMEIN->SetVideoCaptureDlg( dialog );
	dialog->Linking();
}

void cWindowManager::CreateGoldShopMainDlg()
{//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	cWindow* window = NULL;
	window = GetDlgInfoFromFile("./image/InterfaceScript/GoldShopDlg.bin", "rb");

	//VECTOR2 Pos1;
	//Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-(150+window->GetWidth()));
	//Pos1.y =window->GetAbsY();
	//window->SetAbsXY(Pos1.x,Pos1.y);

	AddWindow(window);
	CGoldShopMainDailog* pDlg = (CGoldShopMainDailog*)window;
	GAMEIN->SetGoldShopMainDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateItemShopDlg()
{
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemShop.bin", "rb");

	AddWindow(window);

	CItemShopDialog* pDlg = (CItemShopDialog*)window;
	GAMEIN->SetItemShopDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}
void cWindowManager::CreateFastCfgPointDlg()
{//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
	cWindow * window =GetDlgInfoFromFile("./image/InterfaceScript/FastCfgPoint.bin","rb");
	
	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	FastConfigPointDlg* pDlg = (FastConfigPointDlg*)window;
	GAMEIN->SetFastCfgPointDlg(pDlg);
}

void cWindowManager::CreateLotSplitItemDlg()
{//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:10:00]
	cWindow * window =GetDlgInfoFromFile("./image/InterfaceScript/LotSplitItem.bin","rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	LotSplitItemDlg* pDlg = (LotSplitItemDlg*)window;
	pDlg->Linking();
	GAMEIN->SetLotSplitItemDlg(pDlg);
}

void cWindowManager::CreateVimuWithGoldDlg()
{//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:53:06]
	cWindow * window =GetDlgInfoFromFile("./image/InterfaceScript/VimuWithGoldDlg.bin","rb");

	VECTOR2 Pos1;
	Pos1.x =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth/2-window->GetWidth()/2);
	Pos1.y =((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/2-window->GetHeight()/2);
	window->SetAbsXY(Pos1.x,Pos1.y);
	AddWindow(window);
	VimuWithGoldDlg* pDlg = (VimuWithGoldDlg*)window;
	pDlg->Linking();
	GAMEIN->SetVimuWithGoldDlg(pDlg);
}
void cWindowManager::CreateEquipItemInherit()
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:27:18]
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/EquipItemInheritDlg.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / 2 - window->GetWidth() / 2);
	Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight / 2 - window->GetHeight() / 2);
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	cEquipItemInherit* pDlg = (cEquipItemInherit*)window;
	GAMEIN->SetEquipItemInherit(pDlg);
	pDlg->Linking();
}
void cWindowManager::CreateUnionWarInfoDlg()
{//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:42:34]
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/UnionWarInfo.bin", "rb");
	VECTOR2 Pos1;
	Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth  - 540);
	Pos1.y = 0;
	window->SetAbsXY(Pos1.x, Pos1.y);
	AddWindow(window);
	cUnionWarManager* pDlg = (cUnionWarManager*)window;
	GAMEIN->SetUnionWarInfoDlg(pDlg);
	pDlg->Linking();
}