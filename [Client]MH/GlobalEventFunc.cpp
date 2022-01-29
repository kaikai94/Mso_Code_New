


// GlobalEventFunc.cpp: implementation of the Global Function
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalEventFunc.h"

#include "MainGame.h"
#include "CharMake.h"
#include "CharSelect.h"
#include "GameIn.h"
#include "MainTitle.h"
#include "cComboBoxEx.h"
#include "cComboBox.h"

#include "ObjectManager.h"
#include "MoveManager.h"
#include "AppearanceManager.h"

#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ExchangeManager.h"
#include "StreetStallManager.h"
#include "VimuManager.h"
#include "FilteringTable.h"
#include "CharMakeManager.h"
#include "WantedManager.h"
#include "PetManager.h"
#include "../[CC]Suryun/SuryunManager_client.h"

#include "ChannelDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StreetStall.h"
#include "BuyItem.h"

#include "cListDialogEx.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "HelperManager.h"
#include "HelperSpeechDlg.h"
#include "MoneyDlg.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "MiniFriendDialog.h"
#include "FriendDialog.h"
#include "MiniNoteDialog.h"
#include "ChatDialog.h"
#include "NoteDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "OptionDialog.h"
#include "ExchangeDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "PKLootingDialog.h"
#include "PyogukDialog.h"
#include "MiniMapDlg.h"
#include "CharMakeDialog.h"
#include "MugongSuryunDialog.h"
#include "SuryunDialog.h"
#include "ExitDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "QuestManager.h"
#include "MPRegistDialog.h"
#include "GuildDialog.h"
#include "GuildCreateDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildRankDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildWarehouseDialog.h"
#include "TitanPartsMakeDlg.h"
#include "TitanMixDlg.h"		
#include "TitanRepairDlg.h"		
#include "TitanRecallDlg.h"			
#include "TitanInventoryDlg.h"		
#include "TitanGuageDlg.h"			
#include "TitanRegisterDlg.h"		
#include "TitanDissolutionDlg.h"	
#include "TitanUpgradeDlg.h"
#include "TitanBreakDlg.h"
#include "TitanManager.h"
#include "MHMap.h"
#include "MHCamera.h"

#include "cMsgBox.h"
#include "cDivideBox.h"
#include "./input/UserInput.h"
#include "./Audio/MHAudioManager.h"
#include "cImeEx.h"
#include "PKManager.h"
#include "ObjectStateManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "UserInfoManager.h"
#include "JournalManager.h"
#include "ExitManager.h"
#include "BailDialog.h"
#include "InventoryExDialog.h"
#include "PartyIconManager.h"
#include "ExchangeItem.h"
#include "DissolutionDialog.h"
#include "GuildMarkManager.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "GuildFieldWarDialog.h"
#include "GuildFieldWar.h"
#include "Shellapi.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "BarIcon.h"
#include "MainBarDialog.h"
#include "GameEventManager.h"
#include "PartyWar.h"
#include "ShoutDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"

// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"

// guildunion
#include "GuildUnion.h"
#include "GuildUnionMarkMgr.h"

#include "SeigeWarDialog.h"

#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "SiegeWarMgr.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"

// Pet
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"

/////////////
//---MurimNet
#include "MNStgPlayRoom.h"
#include "MNPlayRoomDialog.h"

#include "MNStgChannel.h"
#include "MNChannelDialog.h"

#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"

#include "ServerListDialog.h"
#include "AbilityManager.h"

#include "../Interface/cResourceManager.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "GuildNoticeDlg.h"

#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"

#include "guildjoindialog.h"
#include "GuildTraineeDialog.h"

#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"

#include "UniqueItemCurseCancellationDlg.h"	
#include "UniqueItemMixDlg.h"				

#include "UniqueItemMixProgressBarDlg.h"	
#include "TitanMixProgressBarDlg.h"	
#include "TitanPartsProgressBarDlg.h"	

#include "SkinSelectDialog.h"	
#include "CostumeSkinSelectDialog.h"	

#include "SOSDialog.h"

#include "SkillPointResetDlg.h"	

#include "StallFindDlg.h"	
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"	
#include "AutoAnswerDlg.h"
#include "FortWarDialog.h"

#include "PartyMatchingDlg.h"

#include "MiniMarryDialog.h"

#include "ControlDialog.h"

#include "ItemLockManager.h"

#include "ItemUnLockManager.h"

#include "MHTimeManager.h"

#include "Intro.h"
#include <wininet.h>
#include "AIManager.h"				//[内挂管理][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "StatusIconDlg.h"			//[右键删除buff引用][By:十里坡剑神][QQ:112582793][2017/11/22]
#include "BillboardDialog.h"		//[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "GuildTopList.h"			//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "RegID.h"					//[注册账号	][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "RestPwd.h"				//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "CharacterRBTN.h"			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "OpenPyogukDlg.h"			//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
#include "NpcScriptManager.h"		//[开仓卷选择引用][By:十里坡剑神][QQ:112582793][2017/12/9]
#include "cSingedDialog.h"			//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
#include "cRechargeManager.h"		//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
#include "cSetingInfoDlg.h"			//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
#include "cStatusIconBtnDlg.h"		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
#include "cTopRankDlg.h"
#include "cInventoryRBTDlg.h"		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "cSendItemInfoDlg.h"	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "cKillTopList.h"		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
#include "cSafeLockDialog.h"	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
#include "cMainDlgHideBtn.h"	//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
#include "cSafeLockMainDlg.h"	//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
#include "cMainDlgSafeBtn.h"	//[安全锁界面按钮][By:十里坡剑神][QQ:112582793][2018/6/18]
#include "OnlineFame.h"
#include "MarryManager.h"
#include "BigMapDlg.h"
#include "VideoCaptureDlg.h"
#include "VideoCaptureManager.h"
#include "GoldShopMainDailog.h"   //[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
#include "OtherCharacterEquip.h"
#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
#include "ReinforceDlg.h"
extern HWND _g_hWnd;
#include "cChangeCharCode.h"
#include "FortWarManager.h"
#include "QuickManager.h"
#include "cOnlineVoiceDlg.h"
#include "FastConfigPointDlg.h" //[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
#include "LotSplitItemDlg.h"	//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:13:13]
#include "ReConnect.h"			//[断线重连][BY:十里坡剑神][QQ:112582793][2019-4-14][1:18:53]
#include "VimuWithGoldDlg.h"	//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:47:13]
#include "cEquipItemInherit.h"	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:36:43]
#include "BillBoardManager.h"
#include "FBTipDlg.h"
extern int	g_nServerSetNum; 
extern BOOL   g_IsExit;
extern DWORD gUserID;
extern BOOL g_SafeLockIsOpen;
char RobotName[17];

FUNCEX g_mt_funcEx[]=
{//[窗口过程函数扩展][By:十里坡剑神][QQ:112582793][2019/3/20][18:38:57]
#ifdef _XDYY_
	{OnlineVoiceGuageFunc,"OnlineVoiceGuageFunc"},		//[小队语音音量调节重载函数][By:十里坡剑神][QQ:112582793][2019/3/20][18:31:24]
#endif
	{NULL, ""},
};

FUNC g_mt_func[] =
{
	{MT_LogInOkBtnFunc,"MT_LogInOkBtnFunc"},
	{MT_EditReturnFunc, "MT_EditReturnFunc"},
	{MT_ExitBtnFunc,"MT_ExitBtnFunc"},
	{MT_DynamicBtnFunc,"MT_DynamicBtnFunc"},
	{MT_DynamicEditReturnFunc, "MT_DynamicEditReturnFunc"},

	{CS_BtnFuncCreateChar,"CS_BtnFuncCreateChar"}, 
	{CS_BtnFuncDeleteChar,"CS_BtnFuncDeleteChar"},	 
	{CS_BtnFuncFirstChar,"CS_BtnFuncFirstChar"},	 
	{CS_BtnFuncSecondChar, "CS_BtnFuncSecondChar"},
	{CS_BtnFuncThirdChar,"CS_BtnFuncThirdChar"},
	{CS_BtnFuncFourthchar,"CS_BtnFuncFourthchar"},
	{CS_BtnFuncFifthchar,"CS_BtnFuncFifthchar"},
	{CS_BtnFuncEnter,"CS_BtnFuncEnter"},
	{CS_FUNC_OkISee,"CS_FUNC_OkISee"},
	{CS_BtnFuncLogOut,"CS_BtnFuncLogOut"},

	{MI_CharBtnFunc, "MI_CharBtnFunc"},
	{MI_ExchangeBtnFunc, "MI_ExchangeBtnFunc"},
	{MI_InventoryBtnFunc, "MI_InventoryBtnFunc"},
	{MI_MugongBtnFunc, "MI_MugongBtnFunc"},
	{MI_ChatEditBoxFunc, "MI_ChatEditBoxFunc"},

	{IN_CreateStreetStallFunc, "IN_CreateStreetStallFunc"},
	{IN_DlgFunc, "IN_DlgFunc"}, 
	{MGI_DlgFunc, "MGI_DlgFunc"},
	{MGI_SuryunDlgFunc, "MGI_SuryunDlgFunc"},	
	{MGI_MugongDlgFunc, "MGI_MugongDlgFunc"},	
	{CI_DlgFunc, "CI_DlgFunc"},
	{CI_AddExpPoint, "CI_AddExpPoint"},
	{PA_DlgFunc, "PA_DlgFunc"},

	{SO_DlgFunc, "SO_DlgFunc"},

	{BRS_DlgFunc, "BRS_DlgFunc"},
	{BRS_CloseFunc, "BRS_CloseFunc"},
	{BRS_DeleteFunc, "BRS_DeleteFunc"},
	{BRS_TypeListFunc, "BRS_TypeListFunc"},
	{BRS_ItemListFunc, "BRS_ItemListFunc"},
	{BRS_ClassListFunc, "BRS_ClassListFunc"},
	{BRS_VolumeEditFunc, "BRS_VolumeEditFunc"},
	{BRS_MoneyEditFunc, "BRS_MoneyEditFunc"},
	{BRS_REGBtnFunc, "BRS_REGBtnFunc"},

	{BS_DlgFunc, "BS_DlgFunc"},
	{BS_TitleEditBoxFunc, "BS_TitleEditBoxFunc"},
	{BS_MoneyEditBoxFunc, "BS_MoneyEditBoxFunc"},
	{BS_SellBtnFunc, "BS_SellBtnFunc"}, 
	{BS_REGBtnFunc, "BS_REGBtnFunc"}, 
	{BS_SPFunc, "BS_SPFunc"},	

	{SSI_DlgFunc, "SSI_DlgFunc"},
	{SSI_TitleEditBoxFunc, "SSI_TitleEditBoxFunc"},
	{SSI_MoneyEditBoxFunc, "SSI_MoneyEditBoxFunc"},
	{SSI_BuyBtnFunc, "SSI_BuyBtnFunc"}, 
	{SSI_RegistBtnFunc, "SSI_RegistBtnFunc"}, 
	{SSI_EditBtnFunc, "SSI_EditBtnFunc"}, 
	
	{NSI_DlgFunc, "NSI_DlgFunc"}, 
	{NSI_HyperLinkFunc, "NSI_HyperLinkFunc"},
	

	{HSI_ExitBtnFunc, "HSI_ExitBtnFunc"},

	{CMI_MoneyOkFunc, "CMI_MoneyOkFunc"},
	{CMI_MoneySpinFunc, "CMI_MoneySpinFunc"}, 
	
	{DE_BuyItemFunc, "DE_BuyItemFunc"},
	{DE_SellItemFunc, "DE_SellItemFunc"},
	{DE_DlgFunc, "DE_DlgFunc"},
	{DE_CloseHideBtn, "DE_CloseHideBtn"},

	{PA_BtnFunc, "PA_BtnFunc"},
	
	{QI_QuickDlgFunc, "QI_QuickDlgFunc"},

	{CM_OverlapCheckBtnFunc, "CM_OverlapCheckBtnFunc"},
	{CM_CharMakeBtnFunc, "CM_CharMakeBtnFunc"},
	{CM_CharCancelBtnFunc, "CM_CharCancelBtnFunc"}, 
	{CM_ComboBoxCheckFunc,"CM_ComboBoxCheckFunc"}, 
	{CMFUNC_OkISee,"CMFUNC_OkISee"},
	{CM_OptionSelectBtnFunc, "CM_OptionSelectBtnFunc"},

	{PYO_DlgBtnFunc, "PYO_DlgBtnFunc"},
	{Note_DlgBtnFunc, "Note_DlgBtnFunc"},
	{Friend_DlgBtnFunc, "Friend_DlgBtnFunc"},
	{CR_DlgBtnFunc, "CR_DlgBtnFunc"},
	
	{MNM_DlgFunc,"MNM_DlgFunc"},
	{ITD_DlgFunc,"ITD_DlgFunc"},
	{ITMD_DlgFunc,"ITMD_DlgFunc"}, 
	{ITR_DlgFunc, "ITR_DlgFunc"},

	{CHA_DlgBtnFunc, "CHA_DlgBtnFunc"},
	
	{MAC_DlgFunc, "MAC_DlgFunc"},
	{CTI_DlgFunc, "CTI_DlgFunc"},	
	{COI_DlgFunc, "COI_DlgFunc"},	
	{OTI_DlgFunc, "OTI_DlgFunc"},	
	{EXT_DlgFunc, "EXT_DlgFunc"},	
	{XCI_DlgFunc, "XCI_DlgFunc"},	
	{CSS_DlgFunc, "CSS_DlgFunc"},	
	{MSI_MenuSlotDlgFunc, "MSI_MenuSlotDlgFunc"},
	{PLI_DlgFunc, "PLI_DlgFunc"},
	{CNA_BtnOkFunc, "CNA_BtnOkFunc"},
	{DIS_DlgFunc, "DIS_DlgFunc"},
	{MI_DlgFunc, "MI_DlgFunc"},
	{DIVIDEBOX_Func, "DIVIDEBOX_Func"},

	{MNPRI_DlgFunc, "MNPRI_DlgFunc"},
	{MNCNL_DlgFunc, "MNCNL_DlgFunc"}, 
	{SL_DlgBtnFunc, "SL_DlgBtnFunc" },
	
	{WANTNPC_DlgFunc, "WANTNPC_DlgFunc"},
	{WANTREG_DlgFunc, "WANTREG_DlgFunc"},

	{QUE_QuestDlgFunc, "QUE_QuestDlgFunc"},
	{QUE_JournalDlgFunc, "QUE_JournalDlgFunc"},
	{MP_RegistDlgFunc, "MP_RegistDlgFunc"},

	{BAIL_BailDlgFunc, "BAIL_BailDlgFunc"},
	{ITMALL_DlgBtnFunc, "ITMALL_DlgBtnFunc"},
	{SA_DlgBtnFunc, "SA_DlgBtnFunc"},
	{CHA_DlgFunc, "CHA_DlgFunc"},

	{GD_DlgFunc, "GD_DlgFunc"},

	{ITEM_GOLDDlgFunc, "ITEM_GOLDDlgFunc" }, 

	{GFW_DlgFunc, "GFW_DlgFunc" },

	{SK_DlgBtnFunc, "SK_DlgBtnFunc"},

	{PW_DlgFunc, "PW_DlgFunc"},

	{CHS_DlgFunc, "CHS_DlgFunc"},

	{CHAN_DlgFunc, "CHAN_DlgFunc"},
	
	{GDT_DlgFunc, "GDT_DlgFunc"},
	
	{SW_DlgFunc, "SW_DlgFunc" },

	{CHA_ChangeDlgFunc, "CHA_ChangeDlgFunc" },

	{IT_DlgFunc, "IT_DlgFunc"},

	{CJOB_DlgFunc, "CJOB_DlgFunc"},
	
	{CG_DlgFunc, "CG_DlgFunc"},

	{RFDefault_DlgFunc, "RFDefault_DlgFunc"},

	{RareCreate_DlgFunc, "RareCreate_DlgFunc"},
	
	{PET_DlgFunc, "PET_DlgFunc"},

	{PET_MiniDlgFunc, "PET_MiniDlgFunc"},

	{PET_InvenDlgFunc, "PET_InvenDlgFunc"},

	{PET_UpgradeDlgFunc, "PET_UpgradeDlgFunc"},

	{PET_RevivalDlgFunc, "PET_RevivalDlgFunc"},

	{RFDGUIDE_DlgFunc, "RFDGUIDE_DlgFunc"},

	{TB_DlgFunc, "TB_DlgFunc"},
	{TB_STATE_DlgFunc, "TB_STATE_DlgFunc"},

	{GN_DlgBtnFunc, "GN_DlgBtnFunc"},
	{AN_DlgBtnFunc, "AN_DlgBtnFunc"},

	{GNotice_DlgBtnFunc, "GNotice_DlgBtnFunc"},
	
	{GuildPlusTime_DlgFunc, "GuildPlusTime_DlgFunc"},

	{SkillTrans_DlgFunc, "SkillTrans_DlgFunc"},
	{TDefault_DlgFunc, "TDefault_DlgFunc"},

	{JO_DlgFunc, "JO_DlgFunc"},
	{GU_DlgFunc, "GU_DlgFunc"},
	{GT_DlgFunc, "GT_DlgFunc"},
	{TPMD_DlgFunc, "TPMD_DlgFunc"},						
	{TitanMix_DlgFunc, "TitanMix_DlgFunc"},				
	{Titan_Repair_DlgFunc, "Titan_Repair_DlgFunc"},		
	{Titan_Recall_DlgFunc, "Titan_Recall_DlgFunc"},		
	{TitanUpgrade_DlgFunc, "TitanUpgrade_DlgFunc"},			
	{TitanBreak_DlgFunc, "TitanBreak_DlgFunc"},			
	{titan_inventory_DlgFunc, "titan_inventory_DlgFunc"},	
	{Titan_guage_DlgFunc, "Titan_guage_DlgFunc"},			
	{Titan_Use_DlgFunc, "titan_Use_DlgFunc"},			
	{Titan_Bongin_DlgFunc, "Titan_Bongin_DlgFunc"},		
	{UniqueItemCurseCancellationDlgFunc, "UniqueItemCurseCancellationDlgFunc"	},
	{UniqueItemMixDlgFunc, "UniqueItemMixDlgFunc"	},
	{SOSDlgFunc, "SOS_DlgFunc"},	
	{UniqueItemMix_ProgressBar_DlgFunc, "UniqueItemMix_ProgressBar_DlgFunc"},
	{TitanMix_ProgressBar_DlgFunc, "TitanMix_ProgressBar_DlgFunc"},

	{TitanParts_ProgressBar_DlgFunc, "TitanParts_ProgressBar_DlgFunc"},

	{SkinSelect_DlgFunc, "SkinSelect_DlgFunc"},	
    {CostumeSkinSelect_DlgFunc, "CostumeSkinSelect_DlgFunc"},	
	{SK_Reset_DlgFunc, "SK_Reset_DlgFunc"},	
	{SVD_DlgFunc, "SVD_DlgFunc"},	

	{SFR_DlgFunc, "SFR_DlgFunc"},

	{AutoNoteDlg_Func, "AutoNoteDlg_Func"},
	{AutoAnswerDlg_Func, "AutoAnswerDlg_Func"},

	{PA_CreateBtnFunc, "PA_CreateBtnFunc"},
	{Party_MatchingList_DlgFunc, "Party_MatchingList_DlgFunc"},

	{FW_DlgFunc, "FW_DlgFunc" },

	{FD_WareHouseDlgFunc, "FD_WareHouseDlgFunc"},

	{Marry_DlgBtnFunc,"Marry_DlgBtnFunc"},  

	{BARCONTROL_DIALOGFunc,"BARCONTROL_DIALOGFunc"}, 

	{IT_LOCKDlgFunc,"IT_LOCKDlgFunc"},      

	{IT_UNLOCKDlgFunc,"IT_UNLOCKDlgFunc"},       

	{CG_TargetObjectBtnFunc,"CG_TargetObjectBtnFunc"},
	{CI_RegIDDlgFunc,"CI_RegIDDlgFunc"},			//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	{CI_KeepIDFunc,"CI_KeepIDFunc"	},				//[记录帐号密码][By:十里坡剑神][QQ:112582793][2017/12/6]
	{CI_OpenPyogukFunc,"CI_OpenPyogukFunc"},		//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	{CI_SingedDlgFunc,"CI_SingedDlgFunc"},			//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	{CI_RechargeDlgFunc,"CI_RechargeDlgFunc"},		//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	{CI_InventoryRBTDlgFunc,"CI_InventoryRBTDlgFunc"},//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	{TB_CharacterDialogFunc,"TB_CharacterDialogFunc"},//[自动加点][By:十里坡剑神][QQ:112582793][2018/4/17]
	{TB_SafeLockDialogFunc,"TB_SafeLockDialogFunc"},  //[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	{TB_Dlg_Hide_BtnFunc,"TB_Dlg_Hide_BtnFunc"},		//[界面功能隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	{TB_Main_Dlg_BtnFunc,"TB_Main_Dlg_BtnFunc"},
	{BigMapDlgFunc, "BigMapDlgFunc"},					//[大地图自动寻路][By:慌张的油条][QQ:112582793][2018/9/17]
	{VideoCaptureDlgFunc, "VideoCaptureDlgFunc"},		//[录像功能界面][By:慌张的油条][QQ:112582793][2018/9/17]
#ifdef _XDYY_
	{OnlineVoiceDlgFunc,"OnlineVoiceDlgFunc"},			//[小队语音界面消息响应][By:十里坡剑神][QQ:112582793][2019/3/20][15:28:33]
	{OnlineVoiceMemberFunc,"OnlineVoiceMemberFunc"},	//[小队语音成员按钮][By:十里坡剑神][QQ:112582793][2019/3/20][22:17:21]
#endif
	{FastConfiPointDlgFunc,"FastConfiPointDlgFunc"},	//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
	{LotSplitItemDlgFunc,"LotSplitItemDlgFunc"},//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:11:24]
	{VimuWithGoldDlgFunc,"VimuWithGoldDlgFunc"},//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:48:12]
	{EquipItemInherit, "EquipItemInherit" },	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:36:23]
	{ TopListUpdate ,"TopListUpdate"},			//[排行榜刷新][BY:十里坡剑神][QQ:112582793][2019-9-5][0:58:11]
	{ FBItemTimeAdd ,"FBItemTimeAdd"},			//[副本卷时间增加][BY:十里坡剑神][QQ:112582793][2019-9-21][13:50:07]
	{NULL, ""},
};

int FUNCSEARCHEX(char * funcName)
{
	for(int i = 0 ;; i++)
	{
		if(g_mt_funcEx[i]._func == NULL)
			break;
		if(strcmp(funcName, g_mt_funcEx[i]._funcName)==0)
			return i;
	}

	return -1; 
}
int FUNCSEARCH(char * funcName)
{
	for(int i = 0 ;; i++)
	{
		if(g_mt_func[i]._func == NULL)
			break;
		if(strcmp(funcName, g_mt_func[i]._funcName)==0)
				return i;
	}

	return -1; 
}

extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we)
{
	cStatic * CharacterMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CMID_MessageBox);
	CharacterMessageDlg->SetActive(FALSE);
		
}

void CS_BtnFuncLogOut(LONG lId, void* p, DWORD we)
{
	CHARSELECT->BackToMainTitle();
	TITLE->SetServerList();
	CAMERA->ResetInterPolation();
}
void CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we)
{
	if(CHARSELECT->IsFull() == FALSE)
	{
		MAINGAME->SetGameState(eGAMESTATE_CHARMAKE);
		CAMERA->MoveStart(eCP_CHARMAKE);
	}
	else
	{		
		CHARSELECT->DisplayNotice(17);		
	}
}

void	CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(0);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void	CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(1);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we)
{

//#ifdef _HK_LOCAL_
//	if( !CHARSELECT->GetExtraCharacterSlot() )
//	{
//		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif
//
//#ifdef _TL_LOCAL_
//	if( !CHARSELECT->GetExtraCharacterSlot() )
//	{
//		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif

	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(2);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we)
{
//#ifdef _JAPAN_LOCAL_
//	if( !CHARSELECT->GetExtraCharacterSlot() )
//	{
//		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif
//
//#ifdef _HK_LOCAL_
//	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
//	{
//		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif
//
//#ifdef _TL_LOCAL_
//	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
//	{
//		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif


	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(3);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we)
{
//#ifdef _JAPAN_LOCAL_
//	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
//	{
//        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif
//
//#ifdef _HK_LOCAL_
//	if( CHARSELECT->GetExtraCharacterSlot() < 3 )
//	{
//        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif
//#ifdef _TL_LOCAL_
//	if( CHARSELECT->GetExtraCharacterSlot() < 3 )
//	{
//        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
//		return;
//	}
//#endif

	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(4);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncEnter(LONG lId, void* p, DWORD we)
{
	if(	CHARSELECT->GetCurSelectedPlayer() == NULL)
		CHARSELECT->DisplayNotice(18);
	else
	{
		CHARSELECT->SendMsgGetChannelInfo();
		CHARSELECT->SetDisablePick(TRUE);
	} 
}

void CS_FUNC_OkISee(LONG lId, void* p, DWORD we)	 
{
	cStatic * CharacterSelectMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CSID_MessageBox);
	CharacterSelectMessageDlg->SetActive(FALSE);
}

void CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we)
{
	int nChar = CHARSELECT->GetCurSelectedPlayerNum();
	int MaxChar = MAX_CHARACTER_NUM-1;
//#ifdef _JAPAN_LOCAL_
//	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
//	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
//	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };
//
//#elif defined _HK_LOCAL_
//	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
//	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 235 };
//	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 200 };
//
//#elif defined _TL_LOCAL_
//	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
//	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
//	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };
//
//#else
//#endif

	if( nChar >= 0 && nChar <= MaxChar )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, CHATMGR->GetChatMsg(282) );
		if( pMsgBox )
		{
            VECTOR2 Pos1;
			Pos1.x = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / 2)-(pMsgBox->GetWidth()/2);
			Pos1.y = ((float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight/ 2)-(pMsgBox->GetHeight()/2);
			pMsgBox->SetAbsXY(Pos1.x,Pos1.y );
		}
		CHARSELECT->SetDisablePick( TRUE );
	}
	else
	{
		CHARSELECT->DisplayNotice( 20 );
	}
}
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we)
{
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText();
	int nLen	= strlen(pName);

	if( nLen == 0 )
	{
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );		
		return;
	}

	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	MSG_NAME msg;
	
	msg.Category = MP_USERCONN;
	msg.Protocol = 	MP_USERCONN_CHARACTER_NAMECHECK_SYN;
	strcpy(msg.Name,pName);
	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
}

extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we)
{
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText(); 
	int nLen	= strlen(pName);
	if( nLen == 0 )
	{
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );
		return;
	}

	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	
	CHARACTERMAKEINFO msg;
	memcpy(&msg, CHARMAKEMGR->GetCharacterMakeInfo(), sizeof(msg) );

	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_MAKE_SYN;
	msg.StandingArrayNum = -1;
	strcpy(msg.Name, editboxName->GetEditText());
	
	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
}

extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DIRECTCHARACTERLIST_SYN;
	NETWORK->Send(&msg,sizeof(msg));	
	CAMERA->MoveStart(eCP_CHARSEL);
}

extern void CM_ComboBoxCheckFunc( LONG lId, void* p, DWORD we )
{
	if( we != WE_COMBOBOXSELECT ) return;

	cComboBoxEx* pBox = (cComboBoxEx*)((cDialog*)p)->GetWindowForID( lId );
	
	if( CHARMAKEMGR->ReplaceCharMakeInfo( (CHAR_M)pBox->GetOptionValue(), pBox->GetCurSelectedIdx() ) )
	{
		APPEARANCEMGR->AddCharacterPartChange(1);
		APPEARANCEMGR->SetScalebyGuageBar(1);
	}
	WINDOWMGR->SetMouseDownUsed();
}

extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we)
{
	if( we != WE_BTNCLICK ) return;

	CHARMAKE->GetCharMakeDialog()->OnActionEvent(lId, p, we);
}


extern char g_szHeroIDName[];
extern char g_CLIENTVERSION[];
////#include "ReDisConnManager.h"
void MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we)
{
	cDialog * dlg = (cDialog *)p;
	cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	
	char * userID = editboxId->GetEditText();
	char * userPWD = editboxPwd->GetEditText();	

	if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0))
	{
		MSG_LOGIN_SYN msg;
		/*if(GAMEIN->GetMacPlus(msg.Mac)!=1)
		{
		cMsgBox * MsgBox = WINDOWMGR->MsgBox(MBI_OK,MBT_OK,CHATMGR->GetChatMsg(2261));
		return;
		}*/
		GAMEIN->GetHWID(msg.Mac);
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_LOGIN_SYN;
//#ifdef _HK_LOCAL_
//		msg.dwObjectID = TITLE->m_dwOld;
//#endif
		SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pn, "1", MAX_NAME_LENGTH + 1 );	
		SafeStrCpy(msg.Version,g_CLIENTVERSION, 33 );
		SafeStrCpy(msg.IP,GAMEIN->GetInternetIP().c_str(),18);
		//[断线重连，保存账号密码][By:十里坡剑神][QQ:112582793][2017/11/23]
		REDISCONNECT->OnLoginAccount(msg.id, msg.pw);
		//[保存帐号密码判断][By:十里坡剑神][QQ:112582793][2017/12/6]
		if(((cCheckBox*)dlg->GetWindowForID( MT_KEEPID ))->IsChecked())
			WINDOWMGR->SetUserCfg(TRUE,userID,userPWD);
		else
			WINDOWMGR->SetUserCfg(FALSE,"","");

		OPTIONMGR->SetGameOption(OPTIONMGR->GetGameOption());
		msg.gate = (BYTE)OPTIONMGR->GetGameOption()->nLoginCombo;


		if( msg.gate == 3 )
		{
			HINTERNET hInternet, hHttp, hReq;
			DWORD dwRead = 0;
			char strResult[1024] = { 0, };
			char strRequest[256] = { 0, };
			DWORD Size = 0;

			hInternet = InternetOpen("BUDDY", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
			if( !hInternet )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (004)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			hHttp = InternetConnect( hInternet, "www.google.com", 0, "", "", INTERNET_SERVICE_HTTP, 0, 0 );
			if( !hHttp )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (005)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			sprintf( strRequest, "Auth/AuthUsrInfoChk.aspx?USRID=%s&USRPWD=%s", msg.id, msg.pw );
			hReq = HttpOpenRequest( hHttp, "POST", strRequest, NULL, NULL, NULL, 0, 0 );
			HttpSendRequest( hReq, NULL, 0, NULL, 0 );
			InternetQueryDataAvailable( hReq, &Size, 0, 0 );
			InternetReadFile( hReq, strResult, Size, &dwRead );
			InternetCloseHandle( hHttp );
			InternetCloseHandle( hInternet );
			hHttp = NULL;			
			hInternet = NULL;

			if( strResult[0] != '0' )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (001)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			else
			{
				MSG_LOGIN_SYN_BUDDY msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_LOGIN_SYN_BUDDY;
				SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.Version,g_CLIENTVERSION, 33 );
				SafeStrCpy(msg.AuthCode, strResult, strlen(strResult)+1 );
				msg.gate = (BYTE)OPTIONMGR->GetGameOption()->nLoginCombo;
				msg.AuthKey = TITLE->GetDistAuthKey();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		else
		{
		msg.AuthKey = TITLE->GetDistAuthKey();
		NETWORK->Send(&msg,sizeof(msg));
		}
		
		SafeStrCpy(g_szHeroIDName, userID, MAX_NAME_LENGTH + 1 );

		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_WAIT_LOGINCHECK, MBT_CANCEL, CHATMGR->GetChatMsg( 291 ) );
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}


		TITLE->StartWaitConnectToAgent( TRUE );
		
		USERINFOMGR->SetUserID( userID );
	}
	else
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINEDITCHECK, MBT_OK, CHATMGR->GetChatMsg(362) ); 
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}
	}

	editboxId->SetFocusEdit(FALSE);

	editboxPwd->SetFocusEdit(FALSE);
	dlg->SetDisable( TRUE );
}

void MT_DynamicBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog*)p;

	if( lId == MT_DYNAMICOKBTN )
	{
		cEditBox* editboxPwd = (cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX);
		char* userPWD = editboxPwd->GetEditText();
		if( strcmp(userPWD, "") != 0 )
		{
			MSG_LOGIN_DYNAMIC_SYN msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGIN_DYNAMIC_SYN;
			msg.AuthKey = TITLE->GetDistAuthKey();
			SafeStrCpy( msg.id, g_szHeroIDName, MAX_NAME_LENGTH + 1 );
			SafeStrCpy( msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
			NETWORK->Send( &msg, sizeof(msg) );

			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
			if( pMsgBox )
			{
				pMsgBox->SetDisable( FALSE );
				pMsgBox->SetActive( TRUE );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}

			TITLE->StartWaitConnectToAgent( TRUE );
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg(362) ); 
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		}
		
		editboxPwd->SetFocusEdit(FALSE);
		dlg->SetDisable( TRUE );
	}
	else if( lId == MT_DYNAMICEXITBTN )
	{
		((cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );

		dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		((cEditBox*)dlg->GetWindowForID(MT_IDEDITBOX))->SetEditText( "" );
		((cEditBox*)dlg->GetWindowForID(MT_PWDEDITBOX))->SetEditText( "" );

		TITLE->NoDisconMsg();
		TITLE->ShowServerList();
	}
}

void MT_DynamicEditReturnFunc( LONG lId, void* p, DWORD we )
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;		
		if( lId == MT_DYNAMICPWDEDITBOX )
		{
			 MT_DynamicBtnFunc( MT_DYNAMICOKBTN, p, we );
		}
	}
}

void MT_EditReturnFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;
		cEditBox* pEdit;
		if( lId == MT_IDEDITBOX )
		{
			 pEdit = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
			 if( *pEdit->GetEditText() != 0 )
			 {
				 WINDOWMGR->SetNextEditFocus();
			 }
		}
		else if( lId == MT_PWDEDITBOX )
		{
			pEdit = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
			if( *pEdit->GetEditText() != 0 )
			{
				MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
			}
		}
	}
}

void MT_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog *)p;
	cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	

	editboxId->SetEditText("");
	editboxPwd->SetEditText("");	
	editboxId->SetFocusEdit(TRUE);
	editboxPwd->SetFocusEdit(FALSE);

	TITLE->NoDisconMsg();
	TITLE->ShowServerList();
}

void MI_CharBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetCharacterDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetCharacterDialog()->UpdateData();
		GAMEIN->GetCharacterDialog()->SetActive(TRUE);
	}
}
void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we)
{
	static BOOL run = 0;
	if(run)
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_WALKMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 0;
		MOVEMGR->SetWalkMode(HERO);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_RUNMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 1;
		MOVEMGR->SetRunMode(HERO);
	}
}

void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	}
}

void MI_MugongBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
	}
		
}

void IN_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK || we == WE_RBTNCLICK || we == WE_LBTNDBLCLICK || we == WE_RBTNDBLCLICK)
	{
		GAMEIN->GetInventoryDialog()->OnActionEvnet(lId, p, we);
	}
	if(lId == IN_SHOPITEMBTN)
		GAMEIN->GetInventoryDialog()->OnShopItemBtn();
	else if(lId == IN_TABBTN1 || lId == IN_TABBTN2 || lId == IN_TABBTN3 || lId == IN_TABBTN4)
		GAMEIN->GetInventoryDialog()->OffShopItemBtn(lId);
	else if(lId == IN_MONEYEDIT)
	{
		WORD wTime;
		if(STREETSTALLMGR->IsOpenState()||EXCHANGEMGR->IsExchanging()||sCPlayerAI->IsOpen())
		{//[交易状态、摆摊状态、挂机状态 禁止整理][BY:十里坡剑神][QQ:112582793][2019-9-16][23:15:11]
			CHATMGR->AddMsg(CTC_AI_TIP, CHATMGR->GetChatMsg(2291));
			return;
		}
		if(GAMEIN->GetInventoryDialog()->IsCanMovePack(wTime))
		{
			if (GAMEIN->GetMixDialog()->IsActive())//[组合窗口是否开启][By:十里坡剑神][QQ:112582793][2019/3/18][1:34:12]
				GAMEIN->GetMixDialog()->Release();;
			if(GAMEIN->GetReinforceDialog()->IsActive())//[强化窗口是否开启][By:十里坡剑神][QQ:112582793][2019/3/18][1:34:24]
				GAMEIN->GetReinforceDialog()->Release();
			if (GAMEIN->GetDissolutionDialog()->IsActive())//[分解窗口是否开启][By:十里坡剑神][QQ:112582793][2019/3/18][1:34:33]
				GAMEIN->GetDissolutionDialog()->SetActive(FALSE);

			//[上述窗口开启的情况下会导致渲染失败报错][By:十里坡剑神][QQ:112582793][2019/3/18][1:34:42]
		
			//[整理过程禁止操作各个物品相关窗口][By:十里坡剑神][QQ:112582793][2019/3/18][1:36:13]		
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_PetInven);
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_Deal);
			ITEMMGR->SetDisableDialog(TRUE,eItemTable_Shop);
			MSGBASE msg;
			msg.Category =	MP_ITEMEXT;
			msg.Protocol = 	MP_ITEMEX_PAKMOVE_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg,sizeof(msg));
			GAMEIN->GetInventoryDialog()->SetMovePackTime();
		}
		else
		{
			CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2292),10-wTime);
			return;
		}
	}
}
void MGI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_PUSHDOWN || we == WE_PUSHUP)
	{
		GAMEIN->GetMugongDialog()->ActiveMogongGrid( lId );
	}
}
void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK || we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetMugongDialog()->OnActionEvnet(lId, p, we);
	}
}
void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_BTNCLICK && lId == MGI_SURYUN_BTN)
	{
		GAMEIN->GetSuryunDialog()->LearnAbilitySyn();
	}

	if(we == WE_BTNCLICK && lId == MGI_SKPOINTREDIST_BTN)
	{
		if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(734) );
			return;
		}
		
		GAMEIN->GetMugongSuryunDialog()->SetActive( FALSE );
		GAMEIN->GetSkPointDlg()->SetActive( TRUE );
		OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
	}

	if(we == WE_LBTNCLICK ||  we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetSuryunDialog()->OnActionEvent(lId, p, we);
	}
}
void CI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK)
	{
		if(lId == CI_BESTTIP)
			GAMEIN->GetTipBrowserDlg()->Show();
	}
}
void CI_AddExpPoint(LONG lId, void * p, DWORD we)
{
	BYTE whats=GENGOL_POINT;
	switch(lId)
	{
	case CI_GENGOL_POINT:
	case CI_GENGOL_POINT2:
		whats=GENGOL_POINT;
		break;
	case CI_SIMMEK_POINT:
	case CI_SIMMEK_POINT2:
		whats=SIMMEK_POINT;
		break;
	case CI_MINCHUB_POINT:
	case CI_MINCHUB_POINT2:
		whats=MINCHUB_POINT;
		break;
	case CI_CHERYUK_POINT:
	case CI_CHERYUK_POINT2:
		whats=CHERYUK_POINT;
		break;
	}
	
	if( lId==CI_GENGOL_POINT || lId==CI_SIMMEK_POINT || lId==CI_MINCHUB_POINT || lId==CI_CHERYUK_POINT )
		GAMEIN->GetCharacterDialog()->OnAddPoint(whats);
	else
		GAMEIN->GetCharacterDialog()->OnMinusPoint(whats);
}
void PA_DlgFunc(LONG lId, void * p, DWORD we)
{
}

void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we)	
{

}



void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we)
{
}

void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetStallDialog()->OnTitleEditClick();
	}
}

void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{

}

void SSI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallDialog()->OnActionEvnet(lId, p, we);
}

void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetStall* pStallDlg = GAMEIN->GetStreetStallDialog();

	if( pStallDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallDlg->SelectedItemCheck() ) return;

		if( ITEMMGR->IsDupItem( pStallDlg->GetCurSelectedItemIdx() ) )
		{
			if( pStallDlg->GetCurSelectedItemDur() == 1 )
				STREETSTALLMGR->BuyItem();
			else
				GAMEIN->GetStreetStallDialog()->ShowDivideBox();
		}
		else
		{
			STREETSTALLMGR->BuyItem();
		}
	}
}

void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we)
{

}

void SSI_EditBtnFunc(LONG lId, void * p, DWORD we)
{
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;

	CStreetStall* pStall = (CStreetStall*)p;
	if(we == WE_BTNCLICK)
	{
		CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
		if( pExItem == NULL ) return;

		if( pExItem->IsLocked() == TRUE ) return;

		STREETSTALLMGR->ItemStatus( pExItem, FALSE );
	}
}

void SO_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallKindSelectDialog()->OnActionEvnet(lId, p, we);
}

void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetBuyStallDialog()->OnTitleEditClick();
	}
}

void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
}

void BS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetBuyStallDialog()->OnActionEvnet(lId, p, we);
}

void BS_SellBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetBuyStall* pStallBuyDlg = GAMEIN->GetStreetBuyStallDialog();

	if( pStallBuyDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallBuyDlg->SelectedItemCheck() ) return;

		STREETSTALLMGR->BuyItem();
	}
}

void BS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
}

void BS_SPFunc(LONG lId, void * p, DWORD we)
{
}


void BRS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->OnActionEvnet(lId, p, we);
}

void BRS_CloseFunc(LONG lId, void * p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;
	msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
	
	NETWORK->Send( &msg, sizeof(MSGBASE) );
	
	GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_DeleteFunc(LONG lId, void * p, DWORD we)
{
	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
	
	if( pos != -1 && pItem )
		GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);
	
	GAMEIN->GetBuyRegDialog()->Close();
}


void BRS_TypeListFunc(LONG lId, void * p, DWORD we)

{
	GAMEIN->GetBuyRegDialog()->UpdateType();
}

void BRS_ItemListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateItem();
}

void BRS_ClassListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateClass();
}

void BRS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
	ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();

	if(!pItem)
		return;

	BUY_REG_INFO RegInfo;
	
	if( !GAMEIN->GetBuyRegDialog()->GetBuyRegInfo(RegInfo) )
		return;
	
	GAMEIN->GetStreetBuyStallDialog()->FakeRegistItem(RegInfo, pItem);
	GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we)
{

}

void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we)
{

}

void SFR_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallFindDlg()->OnActionEvent(lId, p, we);
}
void NSI_DlgFunc(LONG lId, void * p, DWORD we)
{

}

void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;

	GAMEIN->GetNpcScriptDialog()->HyperLinkParser( nIdx );
	WINDOWMGR->SetMouseInputProcessed();	
}


void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	HELPERMGR->GetHelperDlg()->CloseDialog();
}


void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMoneyDialog()->OkPushed();
}
void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we)
{
}
void CMI_AlertFunc(LONG lId, void * p, DWORD we)
{

}

void DE_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_CLOSEWINDOW)
	{
		CDealDialog* pDlg = GAMEIN->GetDealDialog();

		if( pDlg && pDlg->IsShow() )
		{
			pDlg->SetActive(FALSE);
		}
	}
	
	if(we == WE_LBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnSelectedItem();
	}
	else if(/*we == WE_LBTNCLICK || we == WE_RBTNCLICK || we == WE_RBTNDBLCLICK ||*/ we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetDealDialog()->OnActionEvnet(lId, p, we);
	}
}
void DE_BuyItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnBuyPushed();
}
void DE_SellItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnSellPushed();
}
void DE_CloseHideBtn(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->HideDealer();
}

void PA_BtnFunc(LONG lId, void * p, DWORD we) 
{
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
		return;

	switch(lId) 
	{		
	case PA_SECEDEBTN:
		{

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));		
		}	
		break;
		
	case PA_FORCEDSECEDEBTN:
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(DeleteID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->BanPartyMemberSyn(DeleteID);
		}
		break;
		
	case PA_TRANSFERBTN: 
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			DWORD NewMasterID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(NewMasterID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->ChangeMasterPartySyn(HEROID, NewMasterID);
		}
		break;
		
	case PA_BREAKUPBTN:
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			if(PARTYMGR->IsProcessing() == TRUE)
				return;

			WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(301));
		}

		break;
		
	case PA_ADDPARTYMEMBERBTN: 
		{
			if(HERO->GetPartyIdx() == 0)	
			{//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
				WINDOWMGR->MsgBox(MSBOX_ADDPARTY,MBT_YESNO,CHATMGR->GetChatMsg(2224));
				if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
					GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
				return;
			}
			else if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			CObject* pObject = OBJECTMGR->GetSelectedObject();

			if( pObject )
			if( pObject->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			if(!pObject)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				return;
			}
			else 
			{
				if(pObject->GetObjectKind() == eObjectKind_Player)
					PARTYMGR->AddPartyMemberSyn(OBJECTMGR->GetSelectedObject()->GetID());
				else

					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
			}
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())	//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;		
	case PA_INVITEOK:
		{
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
			PARTYMGR->PartyInviteAccept();
		}
		break;
	case PA_INVITECANCEL:
		{
			PARTYMGR->PartyInviteDeny();
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
		}
		break;
	case PA_WARSUGGESTBTN:
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			PARTYWAR->SendPartyWarSuggest();
		}
		break;
	case PA_OPTIONBTN:
		{
			GAMEIN->GetPartyDialog()->ShowOption();
		}
		break;
	case PA_MEMBERBTN:
		{
			GAMEIN->GetPartyDialog()->ShowMember();
		}
		break;
	case APPLYPK:   
		{ //[角色右键菜单比武申请][By:十里坡剑神][QQ:112582793][2017/11/23]
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(402 );
			if( pAbility == NULL )
				return ;
			pAbility->Use();
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;
	case APPLYDEAL:	
		{//[角色右键菜单交易申请][By:十里坡剑神][QQ:112582793][2017/11/23]
			CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(405 );
			if( pAbility == NULL )
				return ;
			pAbility->Use();
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;
	case MININOTE:	
		{//[角色右键菜单传书][By:十里坡剑神][QQ:112582793][2017/11/23]
			CObject* object = OBJECTMGR->GetSelectedObject();
			if(!object||object->GetObjectKind()!=eObjectKind_Player)
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2250));
				return;
			}
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(object->GetObjectName(), "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;
#ifdef _XDYY_
	case OV_TEAMVOICE_SET_BTN:
		{//[小队语音_设置开关按钮][By:十里坡剑神][QQ:112582793][2019/3/20][21:59:44]
			if(GAMEIN->GetOnlineVoiceDlg())
				GAMEIN->GetOnlineVoiceDlg()->SetActive(!GAMEIN->GetOnlineVoiceDlg()->IsActive());
		}
		break;
#endif
	}
}
void QI_QuickDlgFunc(LONG lId, void * p, DWORD we)
{
	if(lId == QI_PREVPAGEBTN || lId == QI_NEXTPAGEBTN)
		GAMEIN->GetQuickDialog()->OnActionEvent(lId, p, we);
}

void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case PYO_PYOGUKLISTBTN:
		{
			GAMEIN->GetPyogukDialog()->SetPyogukListInfo();
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
			if(GAMEIN->GetGuildDlg()->IsActive() == TRUE)
				GAMEIN->GetGuildDlg()->SetActive(FALSE);
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);
		}
		break;
	case PYO_BUYPYOGUKBTN:
		{
			if(HERO->GetPyogukNum() == MAX_PYOGUKLIST_NUM)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				return;
			}
			
//#ifdef _JAPAN_LOCAL_
//
//			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();
//
//			if( rt != 0 )
//			{
//				if( rt == 1 )	
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
//				}
//				else if( rt == 2 )
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
//				}
//				return;
//			}
//#endif
//#ifdef _HK_LOCAL_
//			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();
//			if( rt != 0 )
//			{
//				if( rt == 1 )	
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
//				}
//				else if( rt == 2 ) 
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
//				}
//				return;
//			}
//#endif
//#ifdef _TL_LOCAL_
//			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();
//
//			if( rt != 0 )
//			{
//				if( rt == 1 )	
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
//				}
//				else if( rt == 2 )
//				{
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
//				}
//				return;
//			}
//#endif

			WINDOWMGR->MsgBox( MBI_PYOGUKBUY, MBT_YESNO, CHATMGR->GetChatMsg( 196 ), 
				HERO->GetPyogukNum() + 1, AddComma(MONEYTYPE(GAMEIN->GetPyogukDialog()->GetPrice(HERO->GetPyogukNum()))));
		}
		break;

	case PYO_PYOGUK1BTN: case PYO_PYOGUK2BTN: case PYO_PYOGUK3BTN:
	case PYO_PYOGUK4BTN: case PYO_PYOGUK5BTN:
		{
			int num = lId-PYO_PYOGUK1BTN;
			CPyogukDialog* pDlg = (CPyogukDialog*)GAMEIN->GetPyogukDialog();
			if( !pDlg )
			{
				ASSERT(0);
				return;
			}

			cPushupButton* ppyogukbtn[TAB_PYOGUK_NUM];

//#ifdef _JAPAN_LOCAL_
//			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
//#elif defined _HK_LOCAL_
//			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
//#elif defined _TL_LOCAL_
//			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
//#else
			if( HERO->GetPyogukNum() == 0 )
//#endif
			{
				for(int i=0; i<TAB_PYOGUK_NUM; ++i)
				{
					ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
					ppyogukbtn[i]->SetPush(FALSE);
				}


				return;
			}
			for(int i=0; i<TAB_PYOGUK_NUM; ++i)
			{
				ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
				if(i==num)
				{
					ppyogukbtn[i]->SetPush(TRUE);
				}
				else
				{
					ppyogukbtn[i]->SetPush(FALSE);
				}
			}
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1+num);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1+num);
		}
		break;

	case PYO_PUTINMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(1);
		}
		break;
	case PYO_PUTOUTMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(0);
		}
		break;
	case PYO_NOBUYBTN: case PYO_NOBUYPYOGUKBTN:
		{
			GAMEIN->GetPyogukDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
		}
		break;
	}
	if(GAMEIN->GetPyogukDialog()->IsActive())//[右键仓储][BY:十里坡剑神][QQ:112582793][2019-4-16][21:32:18]
		GAMEIN->GetPyogukDialog()->OnActionEvnet(lId,p,we);
}


void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case FRI_PAGE1BTN: case FRI_PAGE2BTN: case FRI_PAGE3BTN: 
		{
			GAMEIN->GetFriendDialog()->SetFriendList(lId - FRI_PAGE1BTN + 1);
		}
		break;
	case FRI_ADDFRIENDBTN:
		{
			if(GAMEIN->GetMiniFriendDialog()->IsActive())
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniFriendDialog()->SetName(Name);
				}
			}
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())	//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;
	case FRI_DELFRIENDBTN:
		{
			GAMEIN->GetFriendDialog()->CheckedDelFriend();
		}
		break;
	case FRI_ADDOKBTN:
		{		
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(FRI_NAMEEDIT);
			char code[MAX_NAME_LENGTH+1] = {0,};			
			sscanf(pName->GetEditText(),"%s",code);
			strupr(code);
			if(strcmp("",code) == 0)
				return;
			char heroname[MAX_NAME_LENGTH+1] = {0,};
			sscanf(HERO->GetObjectName(), "%s", heroname);
			strupr(heroname);
			if(strcmp(heroname, code) == 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
				return;
			}				
			FRIENDMGR->AddDelFriendSyn(pName->GetEditText(), MP_FRIEND_ADD_SYN);
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_ADDCANCELBTN:
		{
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_SENDNOTEBTN:
		{
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(GAMEIN->GetFriendDialog()->GetChkedName(), "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
			
		}
		break;
	case FRI_SENDWHISPERBTN:
		{
			char temp[MAX_NAME_LENGTH+5];
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			sprintf(temp,"/%s ",GAMEIN->GetFriendDialog()->GetChkedName());
#ifdef _BIG5SYSTEM_
			if (strlen(temp)>=2)
			{
				char* buf = CODEMGR->ChangeCharCode(temp);
			}
#endif  //_BIG5SYSTEM_
			cEditBox* pChatBox = GAMEIN->GetChatDialog()->GetChatEditBox();
			pChatBox->SetFocusEdit(TRUE);
			pChatBox->SetEditText(temp);
		}
		break;
	}
}


void Marry_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
    switch(lId) 
	{
	    case MAR_ADDOKBTN:
		{
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(MAR_NAMEEDIT);

			char* maryName = pName->GetEditText();

			HERO_TOTALINFO* pHeroInfo = HERO->GetHeroTotalInfo();

			if (HERO->GetGender() != 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1849) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}
			if (strcmp(HERO->GetMarryName(), "0") != 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1848) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}

			if (strlen(maryName) > MAX_NAME_LENGTH)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1799) );
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
				return;
			}

			MSG_NAME msg;
			msg.Category = MP_MARRY;
			msg.Protocol = MP_MARRY_ADD_SYN;
			SafeStrCpy(msg.Name, maryName, MAX_NAME_LENGTH+1);
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));
			GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
		}
		break;
	case MAR_ADDCANCELBTN:
		{
			GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
		}
		break;
	case CG_MARRYME:	
		{//[角色右键菜单响应][By:十里坡剑神][QQ:112582793][2017/11/23]
			if(GAMEIN->GetMiniMaryDialog()->IsActive())
			{
				GAMEIN->GetMiniMaryDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniMaryDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniMaryDialog()->SetName(Name);
				}
			}
		}
		break;
	case CG_DELMARRY:	
		{//[角色右键菜单响应][By:十里坡剑神][QQ:112582793][2017/11/23]
			if (!HERO->IsMarry())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1826 ) );
				return;
			}
			MSG_NAME msg;
			SetProtocol(&msg, MP_MARRY, MP_MARRY_DEL_SYN);
			msg.dwObjectID = HEROID;
			SafeStrCpy(msg.Name, HERO->GetMarryName(), MAX_NAME_LENGTH + 1);
			NETWORK->Send(&msg, sizeof(msg));
		}
		break;
	}
}

void Note_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case NOTE_WRITENOTEBTN:
		{
			if(GAMEIN->GetMiniNoteDialog()->IsActive())
			{
				if(GAMEIN->GetMiniNoteDialog()->GetCurMode() == eMinNoteMode_Read)
					GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
				else
					GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);			
			}
		}
		break;
	case NOTE_DELNOTEBTN:
		{
			GAMEIN->GetNoteDialog()->CheckedNoteID();
		}
		break;

	case NOTE_PAGE1BTN:
	case NOTE_PAGE2BTN:
	case NOTE_PAGE3BTN:
	case NOTE_PAGE4BTN:
	case NOTE_PAGE5BTN:
		{
			NOTEMGR->NoteListSyn( lId - NOTE_PAGE1BTN +1, GAMEIN->GetNoteDialog()->GetMode() );
		}
		break;
	case NOTE_MSENDOKBTN:
		{
			cEditBox * pReceiver = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NOTE_MRECEIVEREDIT);
			char pname[MAX_NAME_LENGTH+1] = {0};
			SafeStrCpy(pname, pReceiver->GetEditText(), MAX_NAME_LENGTH+1);
			if(strlen(pname) == 0)
				return;

			cTextArea * pNote = (cTextArea *)WINDOWMGR->GetWindowForIDEx(NOTE_MWNOTETEXTREA);
			char FullText[200];
			pNote->GetScriptText(FullText);
			int len = strlen(FullText);
			if(len >= MAX_NOTE_LENGTH)
			{
				len = MAX_NOTE_LENGTH -1;
			}
			char Note[MAX_NOTE_LENGTH];
			strncpy(Note, FullText, len);
			Note[len] = 0;
			NOTEMGR->WriteNote(Note,  pname);
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);			
		}
		break;
	case NOTE_MSENDCANCELBTN:
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
		}
		break;
	case NOTE_MREPLYBTN:
		{
			char ToName[MAX_NAME_LENGTH+1];
			strcpy(ToName, GAMEIN->GetMiniNoteDialog()->GetSenderName());
			if(strlen(ToName) == 0 )
				return;
			
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(ToName, "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);

		}
		break;
	case NOTE_MDELETEBTN:
		{
			NOTEMGR->DelNoteSyn(GAMEIN->GetMiniNoteDialog()->GetNoteID(),TRUE);
		}
		break;
	case NOTE_TABBTN1:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_NormalNote );
			NOTEMGR->NoteListSyn( 1, eMode_NormalNote );
		}
		break;
	case NOTE_TABBTN2:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
		}
		break;
	case NOTE_NOTELISTCHK11:
		{
			GAMEIN->GetNoteDialog()->SetChkAll();
		}
		break;
	}
}

void CR_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case CR_PRESENTSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			if(IsBattleChannel[gChannelNum]&&FORTWARMGR->IsWarStart())
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2323));
				return;
			}
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_LOGINSPOT:
		{
			if( EXITMGR->IsExitWaiting() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
			msg.dwObjectID = HEROID;			
			NETWORK->Send(&msg, sizeof(msg));	

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_TOWNSPOT:
		{
			if( EXITMGR->IsExitWaiting() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}

			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_VILLAGESPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	}
	GAMEIN->GetReviveDialog()->SetDisable(TRUE);
}

void MAC_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMacroDialog()->OnActionEvent(lId, p, we);
}

void CTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if (lId == CHA_CREATEOKBTN || lId == CHA_CREATEOKBTNSUPER)
		GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
	//else if( lId == CHA_CREATEOKBTNSUPER )
	//	GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
	else if(lId==CHA_BTN_IMAGE || lId==CHA_ICONGRID_IMAGE)
	{
		GAMEIN->GetShoutDlg()->OnActionEvent(lId, p, we);
	}
	else
		GAMEIN->GetChatDialog()->OnActionEvent(lId, p, we);
}


void COI_DlgFunc(LONG lId, void * p, DWORD we)
{
}

void OTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetOptionDialog()->OnActionEvent(lId, p, we);
}

void EXT_DlgFunc(LONG lId, void * p, DWORD we)
{

	if( we == WE_BTNCLICK )
	{
		if( lId == EXT_BTN_EXITGAME )
		{
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
		}		
		else if( lId == EXT_BTN_SELECTCHAR )
		{
			////老商城	by:十里坡剑神	QQ:112582793
			/*if(GAMEIN->GetGoldShopDialog()->IsActive())
				GAMEIN->GetGoldShopDialog()->HideDealer();*/
			//[删除发送物品信息][By:十里坡剑神][QQ:112582793][2018/4/3]
			GAMEIN->GetSendItemInfoDlg()->ClearUseItem();

			//内挂配置保存 by:十里坡剑神	QQ:112582793
			//sCPlayerAI->SaveConfig();
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			int nErrorCode;
			if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
			{
				EXITMGR->SendExitMsg( eEK_CHARSEL );
				g_IsExit= TRUE;
			}
			else
			{
				switch( nErrorCode )
				{
				case eEXITCODE_PKMODE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );
					break;
				case eEXITCODE_LOOTING:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );
					break;
				default:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );
					break;
				}
			}
		}
		else if(lId == EXT_BTN_SELECTCHAR_ROBOT)
		{
			//老商城	by:十里坡剑神	QQ:112582793
			/*if(GAMEIN->GetGoldShopDialog()->IsActive())
				GAMEIN->GetGoldShopDialog()->HideDealer();*/
			//[删除发送物品信息][By:十里坡剑神][QQ:112582793][2018/4/3]
			GAMEIN->GetSendItemInfoDlg()->ClearUseItem();

			//内挂配置保存 by:十里坡剑神	QQ:112582793
			//sCPlayerAI->SaveConfig();
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			EXITMGR->SendExitMsg( eEK_CHARSEL,TRUE );
			g_IsExit= TRUE;

		}
	}

}

void XCI_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetExchangeDialog()->OnActionEvent(lId, p, we);
}

void CSS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetCharStateDialog()->OnActionEvent(lId, p, we);
}

void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetMenuSlotDialog()->OnActionEvent(lId, p, we);
}

void PLI_DlgFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		if( ((cDialog*)p)->GetType() == WT_PKLOOTINGDLG )
		{
			((CPKLootingDialog*)p)->OnActionEvent( lId, p, we );

		}
		else
		{
			int a = 0;
		}
	}
}

void CNA_BtnOkFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		((cDialog*)p)->SetActive( FALSE );
	}
	TITLE->SetServerList();
}

void DIS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetDissolutionDialog()->OnActionEvent( lId, p, we );
}

void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we)
{
	if( we == eIMEE_SETFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
	}
	else if( we == eIMEE_KILLFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( TRUE );
	}
}

void MNPRI_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgPlayRoom::GetInterface()->OnActionEvent( lId, p, we );
}

void MNPRI_ChatFunc( LONG lId, void * p, DWORD we )
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

	NETWORK->Send( &msg, msg.GetMsgLength() );
	
	pEdit->SetEditText("");
}

void MNCNL_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgChannel::GetInterface()->OnActionEvent( lId, p, we );
}


void MNCNL_ChatFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

	NETWORK->Send( &msg, msg.GetMsgLength() );
	
	pEdit->SetEditText("");
}


void DIVIDEBOX_Func(LONG lId, void * p, DWORD we)
{
	if( we == WE_RETURN )
	{
		((cDivideBox*)p)->ExcuteDBFunc( WE_RETURN );		
	}
}
#include "MapChange.h"
#include "UnionWarInfo.h"
void MessageBox_Func( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{
	case MBI_EXIT:
		{
			if( we == MBI_YES )
			{
				//老商城	by:十里坡剑神	QQ:112582793
				/*if(GAMEIN->GetGoldShopDialog()->IsActive())
					GAMEIN->GetGoldShopDialog()->HideDealer();*/
				//[删除发送物品信息][By:十里坡剑神][QQ:112582793][2018/4/3]
				GAMEIN->GetSendItemInfoDlg()->ClearUseItem();
				//内挂配置保存	by:十里坡剑神  QQ:112582793
				//sCPlayerAI->SaveConfig();
				if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
				{
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
					break;
				}

				int nErrorCode;
				if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
				{
					EXITMGR->SendExitMsg( eEK_GAMEEXIT );
					g_IsExit= TRUE;
				}
				else
				{
					switch( nErrorCode )
					{
					case eEXITCODE_PKMODE:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	
						break;
					case eEXITCODE_LOOTING:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );
						break;
					default:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	
						break;
					}
				}
			}
		}
		break;

	case MBI_PASSERROR:
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
 			cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_IDEDITBOX );
			cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
			if( pEditId )
				pEditId->SetFocusEdit( FALSE );
			if( pEditPwd )
			{
				pEditPwd->SetEditText( "" );
				pEditPwd->SetFocusEdit( TRUE );
			}
			cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg1->SetDisable( FALSE );

			cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );

			if( pDlg2 )
			if( pDlg2->IsActive() )				
			{
				pDlg1->SetActive( TRUE );
				pDlg2->SetDisable( FALSE );
				pDlg2->SetActive( FALSE	);
				pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_DYNAMICPWDEDITBOX );
				pEditPwd->SetEditText( "" );
			}
		}
		break;
	case MBI_OTHERUSER_CONNECTTRY:
		{
			
		}
		break;

	case MBI_DISCONNECTED_OVERLAPPEDLOGIN:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_PARTYBREAKUP:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->BreakUpSyn();
			}
		}
		break;
	case MBI_PYOGUKBUY:
		{
			if(we == MBI_YES)
			{
				PYOGUKMGR->BuyPyogukSyn();
			}
		}
		break;
	case MBI_MUGONGDELETE:
	case MBI_MUGONGDELETE2:
	case MBI_MUGONGDELETE3:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetMugongDialog()->SendDeleteMugong();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_BUYREG:
		{

		}
		break;
	case MBI_MAKESTREETSTALL:
		{
			if(we == MBI_YES)
			{
				STREETSTALLMGR->OpenStreetStall();
			}
			else if( we == MBI_NO )
			{
				STREETSTALLMGR->SetOpenMsgBox(TRUE);
			}
		}
		break;

	case MBI_DELETECHAR:
		{
			if( we == MBI_YES )
			{
				CHARSELECT->DeleteCharacter();
			}
			else
			{
				CHARSELECT->SetDisablePick( FALSE );
			}
		}
		break;
		
	case MBI_EXCHANGEAPPLY:
		{
			if(we == MBI_YES)
			{
				EXCHANGEMGR->AcceptExchange( TRUE );
			}
			else if( we == MBI_NO )
			{
				EXCHANGEMGR->AcceptExchange( FALSE );
			}
		}
		break;
		
	case MBI_EXCHANGEWAITING:
		{
			if(we == MBI_CANCEL)
			{
				EXCHANGEMGR->CancelApply();				
			}
		}
		break;
		
	case MBI_SELECTLOGINPOINT:
		{
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_SELECTLOGINPOINT );				
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					g_UserInput.GetHeroMove()->SelectLoginPoint( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
				}

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
		
	case MBI_LOGINEDITCHECK:
		{

			if( we == MBI_OK )
			{
				cEditBox * pIdEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_IDEDITBOX); 
				cEditBox * pPwEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX); 

				if( *pIdEdit->GetEditText() == 0 )
					pIdEdit->SetFocusEdit(TRUE);
				else
					pPwEdit->SetFocusEdit(TRUE);
			}
			cDialog* pDlg= (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg->SetDisable( FALSE );
		}
		break;
	case MBI_DELETEICONCHECK:
		{
			if( we == MBI_YES )
			{
				ITEMMGR->SendDeleteItem();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
				GAMEIN->GetPyogukDialog()->SetDisable( FALSE );
				GAMEIN->GetGuildDlg()->SetDisable( FALSE );
			}
		}
		break;

	case MBI_FRIEND_ADD:
		{
			if(we == MBI_YES)
			{
				FRIENDMGR->FriendInviteAccept();
				
			}
			else if( we == MBI_NO )
			{
				FRIENDMGR->FriendInviteDeny();
			}
		}
		break;
	case MBI_VIMUAPPLY:
		{
			if(we == MBI_YES)
			{
				VIMUMGR->AcceptVimu( TRUE );
			}
			else if( we == MBI_NO )
			{
				VIMUMGR->AcceptVimu( FALSE );
			}
		}
		break;
	case MBI_VIMUAPPLYWITHGOLD:
		{//[押注比武接受邀请][BY:十里坡剑神][QQ:112582793][2019-4-27][18:21:40]
			if(we==MBI_YES)
			{
				VIMUMGR->AcceptVimuWithGold(TRUE);
			}
			else if(we ==MBI_NO)
			{
				VIMUMGR->AcceptVimuWithGold(FALSE);
			}
		}
		break;
	case MSBOX_UNIONRETURN:
		{//[盟战人数不足踢出提示][BY:十里坡剑神][QQ:112582793][2019-10-26][12:41:05]
			DWORD  m_MoveMap = 17;
			MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
			MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE, &m_MoveMap, 4);
			CHATMGR->SaveChatList();
		}
		break;
	case MSBOX_UNIONENTER:
		{//[盟战地图传送][BY:十里坡剑神][QQ:112582793][2019-10-29][16:11:01]
			if (we == MBI_YES)
			{
				if (GAMEIN->GetUnionWarInfoDlg())
				{
					DWORD dwMapNum = 0;
					dwMapNum = cUNIONWARMGR->GetUnionMapNum();
					if (dwMapNum != 0)
					{
						g_UserInput.GetHeroMove()->MapChange(dwMapNum, 0, 0, eMapChange_SOS);
					}
				}
			}
		}
		break;
	case MBI_VIMUWAITING:
		{
			if(we == MBI_CANCEL)
			{
				VIMUMGR->CancelApply();
			}
		}
		break;
	case MBI_CHARSELERROR:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_CHARMAKEERROR:
		{
			CHARMAKE->SetDisableDlg( FALSE );
		}
		break;
	case MBI_PARTY_SECEDE:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->DelPartyMemberSyn();
			}
		}
		break;
	case MBI_MAPCHANGE:
		{
			GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
			
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_MAPCHANGE );
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					if( !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied() )	
						g_UserInput.GetHeroMove()->MapChange( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MBI_OVERLAPPEDLOGININOTHERSERVER:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
		}
		break;
	case MBI_OVERLAPPEDLOGIN:
		{			
			if(we == MBI_YES)
			{				
				MSG_LOGIN_SYN msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
				SafeStrCpy(msg.id, REDISCONNECT->GetUserAccount(), MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.pw, REDISCONNECT->GetUserPassWord(), MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.pn, "1111", MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.Version,g_CLIENTVERSION, 33 );
				SafeStrCpy(msg.IP,GAMEIN->GetInternetIP().c_str(),18);
				msg.dwObjectID = 0;
				NETWORK->Send(&msg,sizeof(msg));

				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(4) );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}
			else if( we == MBI_NO )
			{
				cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( dlg )
				{
					dlg->SetDisable( FALSE );
					cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
					cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
					if( editboxId )
					{
						editboxId->SetEditText("");
						editboxId->SetFocusEdit(TRUE);
					}
					if( editboxPwd )
					{
						editboxPwd->SetEditText("");

						editboxPwd->SetFocusEdit(FALSE);
					}
				}				
			}
		}
		break;
	case MBI_SEND_DISCONNECT_OVERLAPPEDLOGIN:
		{

		}
		break;
	case MBI_LOGINERRORCLOSE:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
			
			TITLE->NoDisconMsg();	
			TITLE->ShowServerList();
		}
		break;
	case MBI_CANTUSEDYNAMIC:
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );
			if( pDlg )
			{
				pDlg->SetDisable( FALSE );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetEditText( "" );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetFocusEdit( TRUE );
			}			
		}
		break;
	case MBI_SERVERDISCONNECT:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_DISTCONNECT_ERROR:
		{
			WINDOWMGR->CloseAllMsgBoxNoFunc();
			TITLE->ShowServerList();
		}
		break;
	case MBI_MAPSERVER_CLOSE:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;

	case MBI_SELLITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendSellItemMsg();
			}
			else
			{
				CDealDialog::OnCancelSellItem(0,NULL,0,GAMEIN->GetDealDialog(),NULL);
			}
		}
		break;
	case MBI_BUYITEM:
		{

			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendBuyItemMsg();
			}
			else
			{
				GAMEIN->GetDealDialog()->CancelBuyItem();
			}
		}
		break;
	case MBI_GOLDSHOPBUYITEM://商城购买消息发送  by:十里坡剑神  QQ:112582793
		{
			if(we == MBI_YES)
			{
				GAMEIN->GetGoldShopMainDlg()->SendBuyMsg();
			}
			else
			{
				GAMEIN->GetGoldShopMainDlg()->ClearBuyMsgInfo();
				//GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
			}
		}
		break;
	case MSBOX_GOLDMONEPAY:
		{//[商城充值提示][By:十里坡剑神][QQ:112582793][2018/10/1]
			if(we==MBI_YES)
			{
				ShellExecute( NULL, "open", CHATMGR->GetChatMsg(2161),NULL, NULL, SW_SHOWNORMAL );
			}
			GAMEIN->GetGoldShopMainDlg()->ClearBuyMsgInfo();
			//GAMEIN->GetInventoryDialog()->SetDisable(FALSE);
		}
		break;
	case MSBOX_FBTIME:
		{//[副本时间增加][BY:十里坡剑神][QQ:112582793][2019-9-21][14:40:31]
			if (we == MBI_YES)
			{
				GAMEIN->GetFBTipDlg()->CalcAddTime();
			}
		}
		break;
	case MBI_PKMODE:
		{
			if( we == MBI_YES )
			{
				PKMGR->SendMsgPKOn();
			}
			else
			{

			}
		}
		break;
	case MBI_NOT_GM:
		{
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;

	case MBI_WAIT_LOGINCHECK:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}

			TITLE->NoDisconMsg();		
			TITLE->ShowServerList();
			TITLE->StartWaitConnectToAgent( FALSE );
		}
		break;
	case MBI_TIMEOVER_TOCONNECT:
		{
			TITLE->GetServerListDialog()->SetActive( TRUE );
		}
		break;


	case MBI_STREETBUYMSG:
		{
			if( we == MBI_YES )
			{
				STREETSTALLMGR->SendBuyItemMsg();
			}
			else
			{
			}
		}
		break;

	case MBI_REGEDITMSG:
		{
			if( we == MBI_YES )
			{
				if(!GAMEIN->GetStreetBuyStallDialog()->IsActive())

					return;

				POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
				BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);
				GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
				GAMEIN->GetBuyRegDialog()->Show();	
				
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_STREETSTALL;
				msg.dwObjectID = HEROID;
				msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
				
				NETWORK->Send( &msg, sizeof(MSGBASE) );
			}
		}
		break;
	case MBI_STREETSELLMSG:
		{
			CStreetBuyStall* pStall = GAMEIN->GetStreetBuyStallDialog();
			
			if( we == MBI_YES )
				if(pStall && pStall->IsActive() && STREETSTALLMGR->SendSellItemMsg())
					break;
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );
	
			if(pStall) GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
			
			CItem* pItem = pStall->GetSellItem();

			if(pItem)	pItem->SetLock(FALSE);

			pStall->DelSellItem();
		}
		break;

	case MBI_QUESTGIVEUP:
		{
			if( we == MBI_YES )
				QUESTMGR->QuestAbandon();
			else
				QUESTMGR->QuestAbandonClose();
		}
		break;
	case MBI_BAILNOTICEMSG:
		{
			if( we == MBI_YES )
				GAMEIN->GetBailDialog()->SetBadFrameSync();
			else
			{
				GAMEIN->GetBailDialog()->SetDisable( FALSE );
				GAMEIN->GetBailDialog()->Close();
			}

		}
		break;

	case MBI_QUESTSTART:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUseForQuestStart();
			
		}
		break;
	case MBI_BAILNOTICEERR:
		{
			GAMEIN->GetBailDialog()->SetDisable( FALSE );
		}
		break;
	case MBI_BAILLOWBADFAME:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_GUILD_SECEDE:
		{
			if( we == MBI_YES )
				GUILDMGR->SecedeSyn();
		}
		break;
	case MBI_GUILD_BREAKUP:
		{
			if( we == MBI_YES )
			{
				if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 3 ) )
					GUILDMGR->BreakUpGuildSyn();
			}
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MBI_GUILD_BAN:
		{
			if(we == MBI_YES)
				GUILDMGR->DeleteMemberSyn();
		}
		break;
	case MBI_SHOPITEM_LOCK:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUnSealingSync();
		}
		break;

	case MBI_SAVEDPOINTMOVE:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->MapMoveOK();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_SAVEDPOINTDEL:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->DelMoveInfoSync();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_OTHERREVIVE:
		{
			if( we == MBI_YES )			
				ITEMMGR->ReviveOtherSync();			
			else if( we == MBI_NO )			
				ITEMMGR->ReviveOtherCancel();			
		}
		break;
	case MBI_REVIVECONFIRM:
		{
			if( we == MBI_YES )
				ITEMMGR->ReviveOtherConfirm( TRUE );
			else if( we == MBI_NO )
				ITEMMGR->ReviveOtherConfirm( FALSE );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_ACCEPT:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_ACCEPT );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_DENY:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_DENY );
		}
		break;	
	case MBI_GUILDFIELDWAR_SUGGEST:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggest();
		}
		break;
	case MBI_GUILDFIELDWAR_SURREND:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSurrend();
		}
		break;
	case MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY:
		{
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT );
			else if( we == MBI_NO )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_DENY );
		}
		break;
//--------------------------------------------------------------------------------
	case MBI_GUILD_UNION_INVITE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_ACCEPT );
			else if( we == MBI_NO )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_DENY );
		}
		break;
	case MBI_GUILD_UNION_REMOVE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendRemoveGuildUnion();			
		}
		break;
	case MBI_GUILD_UNION_DESTROY:
		{
			if( we == MBI_YES )
				GUILDUNION->SendDestroyGuildUnion();
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
//----------------------------------------------------------------------------------
	case MBI_SKPOINTREDIST_USE:
		{
			if( we == MBI_YES )
			{
				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.TargetItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				msg.UseBaseInfo.ShopItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.ShopItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				
				NETWORK->Send(&msg,sizeof(msg));

				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
		}
		break;
	case MBI_SKPOINTREDIST_UP:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( FALSE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_SKPOINTREDIST_DOWN:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( TRUE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_PRESENT_NOTIFY:
		{
			GAMEIN->GetNoteDialog()->SetActive( TRUE );
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
			GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );

			CBarIcon* pIcon = GAMEIN->GetMainInterfaceDialog()->GetBarIcon( OPT_NOTEDLGICON );
			if( pIcon )
				pIcon->SetPush( TRUE );
		}
		break;
	case MBI_PARTYWAR_SUGGEST:
		{
			if( we == MBI_YES )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_ACCEPT );
			else if( we == MBI_NO )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_DENY );
		}
		break;
	case MBI_CHANGENAME_ACK:
	case MBI_JOBCHANGE_ACK:
		{
			//[删除发送物品信息][By:十里坡剑神][QQ:112582793][2018/4/3]
			GAMEIN->GetSendItemInfoDlg()->ClearUseItem();
			EXITMGR->SendExitMsg( eEK_CHARSEL );
		}
		break;
	case MBI_WANTBUY:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->BuyWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_WANTGIVEUP:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->GiveUpWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_GTREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistDlg() )
				{
					DWORD res;
					if( (res = GAMEIN->GetGTRegistDlg()->TournamentRegistSyn()) != eGTError_NOERROR )
					{
						char buf[256];
						if( res == eGTError_NOGUILDMASTER )
							sprintf( buf, CHATMGR->GetChatMsg( 950 ) );
						else if( res == eGTError_UNDERLEVEL )
							sprintf( buf, CHATMGR->GetChatMsg( 951 ), LIMITLEVEL_GUILDTOURNAMENT );
						else if( res == eGTError_UNDERLIMITEMEMBER )
							sprintf( buf, CHATMGR->GetChatMsg( 952 ), LIMITMEMBER_GUILDTOURNAMENT );
						else if( res == eGTError_NOTREGISTDAY )
							sprintf( buf, CHATMGR->GetChatMsg( 962 ), LIMITMEMBER_GUILDTOURNAMENT );

						CHATMGR->AddMsg( CTC_SYSMSG, buf );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistDlg() )
				{
					if( GAMEIN->GetGTRegistDlg()->IsActive() )
						GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTREGISTCANCEL_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					GAMEIN->GetGTRegistcancelDlg()->TournamentRegistCancelSyn();
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
						GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTBATTLELIST_OBSERVER:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->EnterBattleonObserverSyn() == FALSE )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(958) );
						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						if( GAMEIN->GetGTBattleListDlg()->IsActive() )
							GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->IsActive() )
						GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
				}
			}

		}
		break;
	case MBI_GTSCORE_EXIT:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GTOURNAMENT;
				msg.Protocol = MP_GTOURNAMENT_OBSERVER_EXIT;
				msg.dwData = HERO->GetBattleID();
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_SW_PROTECTREG_OK:
		{
			if( we == MBI_YES )
			{
				DWORD dwGuildIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildIdx();
				if( !dwGuildIdx )	return;

				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ACCEPTGUILD_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwGuildIdx;
				NETWORK->Send( &Msg, sizeof(Msg) );

				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
				GAMEIN->GetSWProtectRegDlg()->SetActive( FALSE );
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_TAXCHANGE_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
				if( pTax )
				{
					if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
					{
						SWPROFIT->SendChangTexrateSyn( atoi(pTax->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_PROFIT_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
				if( pMoney )
				{
					if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
					{
						SWPROFIT->SendGetMoneySyn( atoi(pMoney->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_GUILD_CHANGE_LOCATION:
		{
			if( we == MBI_YES )
			{
				GUILDMGR->SendChangeLocation();
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_ENGRAVE_SYN:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_PROPOSAL_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_DEFENCE_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_ATTACKREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ATTACK_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_OBSERVER_SYN:
		{
			if( we == MBI_YES )
			{
				if(PETMGR->GetCurSummonPet())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
					return;
				}

				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 2;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SKILLOPTIONCLEAR_ACK:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillOptionClearDlg()->OptionClearSyn();
			}
		}
		break;
	case MBI_SKILLOPTIONCLEAR_NACK:
		{
		}
		break;
	case MBI_GUILD_STUDENTLVUP_CERABRATIONMONEY_MASTERCHECK:
		{
			if( we == MBI_OK )
			{
				MSG_DWORD2 msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CELEBRATIONMONEY_MASTER_CHECKED;
				msg.dwData1 = HERO->GetGuildIdx();
				msg.dwData2 = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_MUNPADELETESEL:
		{
            if( we == MBI_YES )
			{
				MSG_MUNHANOTEINFO msg;
				ZeroMemory( &msg, sizeof(MSG_MUNHANOTEINFO) );
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_DELETE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
				SafeStrCpy(msg.Intro, CHATMGR->GetChatMsg(1494), MAX_CHAT_LENGTH+1 );
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_TITAN_REPAIR:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = TITANMGR->GetTitanRepairItem();
				if(!pItem)
					break;

				TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
				if( !pEnduranceInfo )
					break;

				float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
				if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
				{
					fEnduranceRate = 1.0f;
				}

				DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);

				if( HERO->GetMoney() < dwRepairPrice )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pItem->GetItemIdx();
				msg.dwData2 = pItem->GetPosition();
												
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			}
		}
		break;
	case MBI_TITAN_TOTAL_REPAIR:
		{
			if( we == MBI_YES )
			{
				MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN msg;
				msg.Init();
				DWORD dwMoney = TITANMGR->GetTitanEnduranceTotalInfo(&msg, TRUE);
				if( HERO->GetMoney() < dwMoney )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}

				msg.Init();
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_SYN );
				msg.dwObjectID = HEROID;
				
				TITANMGR->GetTitanEnduranceTotalInfo(&msg);

				NETWORK->Send(&msg, msg.GetSize());
			}
		}
		break;
	case MBI_SOS:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSOSDlg()->SetActive(TRUE);
			}
			else
			{
                MSGBASE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL );
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(MSGBASE));
			}
		}
		break;
	case MSBOX_MARRY_ACCEPT:
		{//[夫妻传送卷][By:十里坡剑神][QQ:112582793][2018/9/10]
			if(we == MBI_YES)
			{
				g_UserInput.GetHeroMove()->MapChange( MARRYMGR->GetTargerMapNum(), 0, 0, eMapChange_SOS );
				MARRYMGR->SetTargetMapNum(0);
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_RECKON_NO;
				msg.dwObjectID= MARRYMGR->GetMarryIDX();
				NETWORK->Send(&msg,sizeof(msg));

				MARRYMGR->SetTargetMapNum(0);
				MARRYMGR->SetTargerPos(0);
				MARRYMGR->SetChannelIdx(-1);
			}
		}
		break;
	case MBI_SOS_YES:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_DIE_CHECK;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_NO;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));

				GUILDMGR->SetSOSDieMemberIdx(0);
				GUILDMGR->SetSOSTargetMapNum(0);
				GUILDMGR->SetSOSTargetPos(0);
				GUILDMGR->SetSOSChannelIdx(-1);
			}
		}
		break;
	case MBI_BUDDY_INFOAGREE:
		{
			ShellExecute( NULL, NULL, "Iexplore.exe", 
				"http://user.buddybuddy.co.kr/login/loginForm.asp?url=http://dark.buddybuddy.co.kr/channeling/buddybuddy/buddybuddy_member.asp", 
				NULL, SW_SHOWNORMAL );
		}
		break;
	case MBI_BUDDY_NAMECER:
		{
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://game.buddybuddy.co.kr/Customer2/Member/UserCertify.aspx", 
				NULL, SW_SHOWNORMAL );				
		}
		break;
	case MBI_STATUS_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();

				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
				msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
				msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
				msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();

				NETWORK->Send(&msg,sizeof(msg));

				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
				GAMEIN->GetCharacterDialog()->InitAutoPoint();
			}
			else
			{
				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
			}
		}
		break;
	case MBI_SKILL_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillPointResetDlg()->SetAbilityResetSyn();
			}
		}
		break;
	case MBI_SKILL_POINT_RESET_FAIL:
		{
		}
		break;
	case MBI_PARTYDECISION:
		{
			if( we == MBI_YES )
			{
				PARTYMGR->PartyRequestResult(TRUE);
			}
			else if( we == MBI_NO)
			{
				PARTYMGR->PartyRequestResult(FALSE);
			}			
		}
		break;		
	case MBI_FT_ENGRAVE_START_SYN:
		{
			if( we == MBI_YES )

			{
				MSGBASE msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_ENGRAVE_START_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_FT_ITEM_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->SendItemMoveSyn();
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_FT_MONEY_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->PutOutMoney();	
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MARY_FT_ACCEPT_BT:  
		{
			if (we == MBI_YES)
			{
				MSG_DWORD msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_ADD_ACCEPT; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;

				NETWORK->Send(&msg, sizeof(msg));	
			}
			else if (we == MBI_NO)
			{
				MSG_DWORD msg;
				msg.Category = MP_MARRY;
				msg.Protocol = MP_MARRY_ADD_DENY; 
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->m_mary_msg.PlayerID;

				NETWORK->Send(&msg, sizeof(msg));
			}
			

		}
		break;
		case MSBOX_DELBUF:   //[右键删除buff处理][By:十里坡剑神][QQ:112582793][2017/11/23]
			{
				if(we == MBI_YES)
				{	
					MSG_WORD2 msg;
					msg.Category = MP_ITEMEXT;
					msg.Protocol = MP_ITEM_BUF_DEL_SYN;
					msg.dwObjectID = HERO->GetID();
					msg.wData1 = STATUSICONDLG->GetDelItemIdx();
					NETWORK->Send(&msg, sizeof(msg));
					STATUSICONDLG->SetCanDelBuf(FALSE);
				}
				if(we ==MBI_NO)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2194));
					STATUSICONDLG->SetCanDelBuf(FALSE);
				}
				STATUSICONDLG->SetDelItemIdx(0);
			}
			break;
		case MSBOX_DELBUFTIP:
			{
				if(we==MBI_YES)
				{
					//[删除buff扣费检查][By:十里坡剑神][QQ:112582793][2017/12/5]
					//if(HERO->GetGoldMoney()<50)
					//{
					//	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2195));
					//	return;
					//}
					//else
					//{
						STATUSICONDLG->SetCanDelBuf(TRUE);
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2196));
					//}
				}
			}
			break;
		//case MSBOX_ZHTIP:
		//	{
		//		if(we==MBI_YES)
		//			GAMEIN->GetMixDialog()->SetActiveRecursive( TRUE );
		//		else
		//			GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
		//	}
		//	break;
		//case MSBOX_QHTIP:
		//	{
		//		if(we==MBI_YES)
		//			GAMEIN->GetReinforceDialog()->SetActiveRecursive( TRUE );
		//		else
		//			GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
		//	}
		//	break;
		//case MSBOX_LHTIP:
		//	{
		//		if(we==MBI_YES)
		//			GAMEIN->GetRareCreateDlg()->SetActive( TRUE );
		//		else
		//			if(HERO->GetState()==eObjectState_Deal)
		//				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		//	}
		//	break;
		//case MSBOX_DFTIP:
		//	{
		//		if (we == MBI_YES)
		//			GAMEIN->GetInventoryDialog()->SendUseDF();
		//		else
		//			GAMEIN->GetInventoryDialog()->ClearDFMsg();
		//	}	
		//	break;
		case MSBOX_SINGED:
			{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
				if (we == MBI_YES)
					GAMEIN->GetSingedDlg()->SendLaterMsg();
				else
					GAMEIN->GetSingedDlg()->ClearLaterMsg();
			}	
			break;
		case MSBOX_RECHARGE:
			{
				GAMEIN->UpdataGoldMoney();
			}
			break;
		case MSBOX_INVENTORY_RBT_DEL:
			{
				if(we == MBI_YES)
					GAMEIN->GetInventoryRBTDlg()->DeleteItem();
				else
					GAMEIN->GetInventoryRBTDlg()->ClearSelectItemInfo();
			}
			break;
	}
}

void MNM_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case MNM_PARTYDISPLAY_ONOFF:
		break;
	case MNM_TOGGLEFULL:
		{
			GAMEIN->GetMiniMapDialog()->ToggleMinimapMode();
		}
		break;
	case MNM_PATHFIND_BTN:
		{
			CMiniMapDlg* pMiniMapDlg = (CMiniMapDlg*)p;
				if( pMiniMapDlg && we == WE_BTNCLICK )
					pMiniMapDlg->Move_UsePath();
			return;//[自动寻路小地图屏蔽][By:十里坡剑神][QQ:112582793][2019/3/20][23:58:15]
		}
		break;
	case MNM_VIDEO_REC_START_BTN:
		{
			CVideoCaptureDlg* pVideoCaptureDlg = GAMEIN->GetVideoCaptureDlg();
			if( !pVideoCaptureDlg )
				break;
			pVideoCaptureDlg->SetActive( !pVideoCaptureDlg->IsActive() );
		}
		break;
	case MNM_VIDEO_REC_STOP_BTN:
		{
			cStatusIconBtnDlg* pMiniMapDlg = (cStatusIconBtnDlg*)p;
			if( !pMiniMapDlg )
				break;
			pMiniMapDlg->ShowVideoCaptureStartBtn( TRUE );
			VIDEOCAPTUREMGR->CaptureStop();
		}
		break;
	}
}

void ITD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CUpgradeDlg::OnActionEvent(lId, p, we);
}

void ITMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMixDialog::OnActionEvent(lId, p, we);
	
}

void ITR_DlgFunc(LONG lId, void * p, DWORD we)
{
	CReinforceDlg::OnActionEvent(lId, p, we);
}
void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_CONNNECTOK:
		{
//#ifdef _KOR_LOCAL_
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->OnConnect();	
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
				{

					pChannelDlg->MapChange();
				}
			}
//#else
//			CHARSELECT->GetChannelDialog()->OnConnect();	
//#endif
		}
		break;
	case CHA_CONNNECTCANCEL:
		{
//#ifdef _KOR_LOCAL_
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->SetActive(FALSE);
				CHARSELECT->SetDisablePick(FALSE);
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
					pChannelDlg->SetActive(FALSE);
				CMoveDialog* pMoveDlg = (CMoveDialog*)WINDOWMGR->GetWindowForID( SA_MOVEDLG );
				if(pMoveDlg)
				{
					pMoveDlg->SetDisable(FALSE);
				}
				//当使用回城或者遁甲,取消操作后,回到moveDlg,并且限制玩家移动 //此处限制去 过地图或者回城取消后卡住!
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
//#else
//			CHARSELECT->GetChannelDialog()->SetActive(FALSE);
//			CHARSELECT->SetDisablePick(FALSE);
//#endif
		}
		break;
	}
}

void SL_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	CServerListDialog* pDlg = (CServerListDialog*)p;

	if(lId==SL_BTN_CONNECT)
		TITLE->ConnectToServer(pDlg->GetType(),pDlg->GetSelectedIndex() );
	else if(lId==SL_BTN_EXIT)
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	else if(lId==INTRO_BTN_REPLAY)
	{
			AUDIOMGR->StopBGM();
			MAINGAME->SetGameState(eGAMESTATE_INTRO);
			INTRO->SetReplayIntroFlag();			
	}
	else 
		pDlg->ActionEvent(lId,p,we);
}

//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
void CI_OpenPyogukFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case OPEN_PYOGUK:
		NPCSCRIPTMGR->StartNpcBusiness( 0, eNpcParam_ShowpPyoguk );
		break;
	case OPEN_ETC_PYOGUK:
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
		NPCSCRIPTMGR->StartNpcBusiness( 3, eNpcParam_ShowpPyoguk );
		break;
	}
	GAMEIN->GetOpenPyogukDlg()->SetActive(FALSE);
}
void MI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(lId == MI_BTN_SIZE)
		GAMEIN->GetMainInterfaceDialog()->OnActionEvent(lId,p,we);
	//else if (lId == BAIL_BAILBTONLINE)
	//{
	//	//[在线洗恶窗口打开][By:十里坡剑神][QQ:112582793][2017/11/22]
	//	if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
	//	{
	//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
	//		return;
	//	}
	//	OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
	//	GAMEIN->GetBailDialog()->SetUseGoldClear(TRUE);
	//	GAMEIN->GetBailDialog()->Open();
	//}

}

void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTNPC_LISTORDERCOMBOBOX:
		{
			if(we != WE_COMBOBOXSELECT) return;	
			
			BYTE OrderType = eWan_Date;
			cComboBox * btnOrder = (cComboBox *)WINDOWMGR->GetWindowForIDEx(WANTNPC_LISTORDERCOMBOBOX);
		
			OrderType = btnOrder->GetCurSelectedIdx();

			GAMEIN->GetWantNpcDialog()->SetOrderType(OrderType);
			WANTEDMGR->SortWantedListSyn(1);
		}
		break;
	case WANTNPC_SEARCHMODEBTN:
		{
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Search);
			SEND_WANTNPCLIST Info;
			memset(&Info, 0, sizeof(SEND_WANTNPCLIST));
			GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);

		}
		break;
	case WANTNPC_BUYNPCBTN: 	case WANTNPC_BUYSEARCHBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}
				char buf[64];
				sprintf(buf, CHATMGR->GetChatMsg( 886 ), name );

				WINDOWMGR->MsgBox( MBI_WANTBUY, MBT_YESNO, buf);
			}
		}
		break;
	case WANTNPC_FRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_1LISTBTN: case WANTNPC_2LISTBTN: case WANTNPC_3LISTBTN: case WANTNPC_4LISTBTN: 
	case WANTNPC_5LISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD CurUnit = (CurPage-1) / MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->SortWantedListSyn((lId-WANTNPC_1LISTBTN+1) + (CurUnit)*MAX_WANTNPC_PAGEUNIT);
		}
		break;
	case WANTNPC_BACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MY1LISTBTN: case WANTNPC_MY2LISTBTN: case WANTNPC_MY3LISTBTN: case WANTNPC_MY4LISTBTN: 
	case WANTNPC_MY5LISTBTN:
		{
			WANTEDMGR->MyWantedListSyn(lId-WANTNPC_MY1LISTBTN+1);
		}
		break;
	case WANTNPC_MYBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYWANTBTN: case WANTNPC_MYWANTSEARCHBTN:
		{
			WANTEDMGR->MyWantedListSyn(1);
		}
		break;
	case WANTNPC_SEARCHFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCH1LISTBTN: case WANTNPC_SEARCH2LISTBTN: case WANTNPC_SEARCH3LISTBTN: case WANTNPC_SEARCH4LISTBTN: 
	case WANTNPC_SEARCH5LISTBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), lId-WANTNPC_SEARCH1LISTBTN+1);
		}
		break;
	case WANTNPC_SEARCHBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCHBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), 1);
		}
		break;
	case WANTNPC_SORTBTN:
		{
			WANTEDMGR->SortWantedListSyn(1);
		}
		break;
	case WANTNPC_GIVEUPBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}

				char buf[64];
				sprintf(buf, CHATMGR->GetChatMsg( 887 ), name );
				WINDOWMGR->MsgBox( MBI_WANTGIVEUP, MBT_YESNO, buf);			
			}
		}
		break;
	}
}

void WANTREG_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTREG_OK:
		{
			CWantRegistDialog* pDlg = (CWantRegistDialog*)p;
			cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID(WANTREG_PRIZEEDIT);
			char* temp = RemoveComma(pEdit->GetEditText());
			DWORD prize = atoi(temp);
			if((prize < MIN_PRIZE) || (prize > MAX_PRIZE))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(559) );
				return;
			}			
			WANTEDMGR->RegistKillerSyn(prize);
		}
		break;
	case WANTREG_CANCEL:
		{
			GAMEIN->GetWantRegistDialog()->SetActive(FALSE);
			WANTEDMGR->SetWaiting(eNone);
			WANTEDMGR->SetMurdererID(0);
		}
		break;
	}
}


void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we )
{
	CQuestDialog* pDlg = (CQuestDialog*)p;

	switch(lId)
	{
	case QUE_JOURNALBTN:
		JOURNALMGR->GetListSyn();
		break;
	case QUE_ITEM_GRID:
//		pDlg->GetPositionForXYRef();
		break;
	case QUE_PAGE1BTN:
		pDlg->RefreshQuestItem(0);
		break;
	case QUE_PAGE2BTN:
		pDlg->RefreshQuestItem(1);
		break;
	case QUE_PAGE3BTN:
		pDlg->RefreshQuestItem(2);
		break;
	case QUE_PAGE4BTN:
		pDlg->RefreshQuestItem(3);
		break;
	case QUE_PAGE5BTN:
		pDlg->RefreshQuestItem(4);
		break;
	}
}

void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we )
{
	CJournalDialog* pDlg = (CJournalDialog*)p;

	switch(lId)
	{
	case QUE_JOURNALFRONTBTN:
		pDlg->SetBasePage(FALSE);
		break;
	case QUE_JOURNALBACKBTN:
		pDlg->SetBasePage(TRUE);
		break;		
	case QUE_JOURNALPAGE1BTN:
		pDlg->SetPage(0);		
		break;
	case QUE_JOURNALPAGE2BTN:
		pDlg->SetPage(1);		
		break;
	case QUE_JOURNALPAGE3BTN:
		pDlg->SetPage(2);		
		break;
	case QUE_JOURNALPAGE4BTN:
		pDlg->SetPage(3);		
		break;
	case QUE_JOURNALPAGE5BTN:
		pDlg->SetPage(4);		
		break;
	case QUE_JOURNALCHECKBOX1BTN:
		pDlg->SetItemCheck(0);
		break;
	case QUE_JOURNALCHECKBOX2BTN:
		pDlg->SetItemCheck(1);
		break;
	case QUE_JOURNALCHECKBOX3BTN:
		pDlg->SetItemCheck(2);
		break;
	case QUE_JOURNALCHECKBOX4BTN:
		pDlg->SetItemCheck(3);
		break;
	case QUE_JOURNALCHECKBOX5BTN:
		pDlg->SetItemCheck(4);
		break;
	case QUE_JOURNALSAVE:
		if(pDlg->IsSavedJournal())
			pDlg->SelectedJournalDelete();
		else
			pDlg->SelectedJournalSave();
		break;
	case QUE_JOURNALSAVEDLIST:
		pDlg->ViewJournalListToggle();
		break;
	}
}


void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we )
{
	CBailDialog* pDlg = (CBailDialog*)p;

	switch(lId)
	{
	case BAIL_OK:
			pDlg->SetFame();
		break;
	case BAIL_CANCEL:
			pDlg->SetActive(FALSE);
		break;
	}
}

//荐访厘 mugong practice
void MP_RegistDlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case MP_ROKBTN:
		{
			if(PETMGR->GetCurSummonPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1277));
				return;
			}
			GAMEIN->GetMPRegistDialog()->SetDisable(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
			GAMEIN->GetMugongSuryunDialog()->SetDisable(TRUE);
			SURYUNMGR->GoSuryunMapSyn();
		}
		break;
	case MP_RCANCELBTN:
		{
			GAMEIN->GetMPRegistDialog()->SetActive(FALSE);
		}
	}	
}

void GD_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	///////////////////////////////////////////////////
	//create
	case GD_CCREATEOKBTN:
		{	//magi82 - 巩窍积 啊涝祈府矫胶袍
			if(HERO->GetGuildIdx())	// 家俺臂 荐沥且锭
			{
				MSG_MUNHAINFOUPDATE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_INFOUPDATE_SYN );
				msg.dwObjectID = HEROID;
				msg.Index = GUILDMGR->GetGuild()->GetGuildIdx();
				
				char strInfo[MAX_GUILD_INTRO+1] = { 0, };
				cTextArea* pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				pIntro->GetScriptText(strInfo);
				SafeStrCpy(msg.Intro, strInfo, MAX_GUILD_INTRO+1);
				
				int len = strlen(msg.Intro);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}

				NETWORK->Send(&msg, sizeof(msg));
				GAMEIN->GetGuildCreateDlg()->SetActive( FALSE );
			}
			else	// 巩颇 父甸锭
			{
				cEditBox * pGuildName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(GD_CNAME);
				if(strcmp(pGuildName->GetEditText(), "") == 0)
					return;
				cTextArea * pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				char FullText[100];
				pIntro->GetScriptText(FullText);
				int len = strlen(FullText);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}
				GUILDMGR->CreateGuildSyn(pGuildName->GetEditText(), FullText);
			}
		}
		break;
	///////////////////////////////////////////////////
	//invite
	case GD_IOKBTN:
		{
			if( GUILDMGR->GetInvitedKind() == AsMember )
			{
				GUILDMGR->AddMemberAccept();
			}
			else if( GUILDMGR->GetInvitedKind() == AsStudent )
			{
				GUILDMGR->AddStudentAccept();
			}
		}
		break;
	case GD_ICANCELBTN:
		{
			GUILDMGR->AddMemberDeny();
		}
		break;	
	///////////////////////////////////////////////////
	//guild info / func 
	case GD_MEMBERINFOPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_GuildInfo);
		}
		break;
	case GD_GUILFUNCPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_Func);
		}
		break;
	case GD_BANMEMBER:
		{
			if(HERO->GetGuildMemberRank() < GUILD_MASTER)
				return;
			if((GUILDMGR->GetSelectedMemberID() == 0) || (GUILDMGR->GetSelectedMemberID() == HEROID))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
			}
			else
			{
				char msg[128];
				sprintf(msg, CHATMGR->GetChatMsg( 46 ), GUILDMGR->GetSelectedMemberName());
				WINDOWMGR->MsgBox( MBI_GUILD_BAN, MBT_YESNO, msg);
				GUILDMGR->SetActiveFunc(TRUE);
			}				
		}
		break;
	case GD_INVITEMEMBER:
		{
//#ifdef _HK_LOCAL_	//hk block
//			CObject * targetObj = OBJECTMGR->GetSelectedObject();
//			if(targetObj)
//			{
//				if(targetObj->GetObjectKind()  == eObjectKind_Player)
//				{
//					if(((CPlayer*)targetObj)->GetGuildIdx())
//					{
//						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
//						return;
//					}
//					else
//						GUILDMGR->AddMemberSyn(targetObj->GetID());
//				}
//			}
//#else
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GAMEIN->GetGuildJoinDialog()->SetActive(TRUE);
				}
			}
//#endif
		}
		if(GAMEIN->GetCharacterRBTNDlg()->IsActive())	//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
			GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
		break;
	case GD_GIVEMEMBERNICK:
		{	
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			GAMEIN->GetGuildNickNameDlg()->SetActive(TRUE);
		}
		break;
	case GD_GIVEMEMBERRANK:
		{
			if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
				return;
			GAMEIN->GetGuildRankDlg()->SetActive(TRUE);
		}
		break;
	case GD_SECEDE:
		{
			if(HERO->GetGuildIdx() == 0)
				return;
			if(( GUILD_STUDENT <= HERO->GetGuildMemberRank() ) && (HERO->GetGuildMemberRank() <= GUILD_VICEMASTER) )
				WINDOWMGR->MsgBox( MBI_GUILD_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg( 347 ) );
		}
		break;
	// 06. 03. 巩颇傍瘤 - 捞康霖
	case GD_NOTICE:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(710));
			GAMEIN->GetGuildNoticeDlg()->SetActive(TRUE);			
		}
		break;
	case GD_INVITEUNION:
		{
			CObject* targetObj = OBJECTMGR->GetSelectedObject();
			if( targetObj )
			{
				if( targetObj->GetObjectKind() == eObjectKind_Player )
				{
					GUILDUNION->SendInviteGuildUnion( (CPlayer*)targetObj );
				}
			}
		}
		break;			
	case GD_SECEDEUNION:
		{
			GUILDUNION->SendSecedeGuildUnion();
		}
		break;
	case GD_REMOVEUNION:
		{
			GAMEIN->GetGFWarInfoDlg()->ShowGuildUnion();
		}
		break;
	case GD_DECLARE:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 0 ) )
				GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );
		}
		break;
	case GD_SUGGEST:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 1 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
		}
		break;
	case GD_SURRENDER:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 2 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
		}
		break;

	case GD_POSITION://流困
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyPosition();//Add 060803 by wonju
		}
		break;
	case GD_RANK:    //饭骇
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyLevel();//Add 060803 by wonju
		}
		break;
	case GD_PLUSTIME: //敲矾胶 鸥烙
		{
			GAMEIN->GetGuildPlusTimeDlg()->SetActive( TRUE ); //Add 060803 by wonju
		}
		break;

	case GD_RELATIONBTN: //悼竿包拌
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( TRUE );
		}
		break;

	case GD_MUNHA:
		{
			GAMEIN->GetGuildMunhaDialog()->SetActive( TRUE );
		}	
		break;
	

	///////////////////////////////////////////////////
	//mark
	case GDM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDM_NAMEEDIT);
			if(strcmp(pMarkName->GetEditText(), "") == 0)
				return;
			if( GUILDMARKMGR->ChangeGuildMarkSyn(HERO->GetGuildIdx(), pMarkName->GetEditText()) == FALSE )
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
			GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
		}
		break;

	case GUM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GDM_NAMEEDIT );
			if( strcmp(pMarkName->GetEditText(), "") == 0 )	return;
			if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
			{
				if( !GUILDUNIONMARKMGR->RegistGuildUnionMark( g_nServerSetNum, HERO->GetGuildUnionIdx(), 
					HERO->GetGuildUnionMarkIdx(), pMarkName->GetEditText() ) )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
				GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
			}
		}
		break;
		
	///////////////////////////////////////////////////
	//levelup
	case GD_LUOKBTN:
		{
			GUILDMGR->LevelUpSyn();
		}
		break;
	
	///////////////////////////////////////////////////
	//nick name 
	case GD_NREGISTOKBTN:
		{
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			if(GUILDMGR->GetSelectedMemberID())
			{
				cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GD_NICKNAMEEDIT);
				if(strcmp(pName->GetEditText(), "") == 0)
					return;
				if(strlen(pName->GetEditText()) > MAX_GUILD_NICKNAME)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(98));
					return;
				}
				if((  FILTERTABLE->IsUsableName(pName->GetEditText(), FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pName->GetEditText(), " '") == TRUE))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(96));
					return;
				}
				GUILDMGR->GiveNickNameSyn(pName->GetEditText());
				GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);
			}
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
		}
		break;

	///////////////////////////////////////////////////
	//rank
	case GDR_OKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE Rank=0;
			cComboBox* pRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_RANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pRankCombo->GetComboText());
			
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(155) )
			Rank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(156) )
			Rank = GUILD_SENIOR;
			CMD_CS( CHATMGR->GetChatMsg(157) )
			Rank = GUILD_VICEMASTER;
			CMD_EN
			
			GUILDMGR->ChangeMemberRank(Rank);
		}
		break;
	case GDR_DOKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE DRank =0;
			cComboBox* pDRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_DRANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pDRankCombo->GetComboText());
			
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(713) )
			DRank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(712) )
			DRank = GUILD_VICEMASTER;
			CMD_EN			
			GUILDMGR->ChangeMemberRank(DRank);
		}
		break;
	case GDW_WAREHOUSE1BTN: case GDW_WAREHOUSE2BTN:case GDW_TABDLG1:case GDW_TABDLG2:
		{//[右键仓储][BY:十里坡剑神][QQ:112582793][2019-8-24][19:09:06]
			GAMEIN->GetGuildWarehouseDlg()->OnActionEvent(lId, p, we);
		}
		break;
	case GDW_PUTINMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(1);
		}
		break;
	case GDW_PUTOUTMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(0);
		}
		break;

	// guild union create
	case GDU_OKBTN:
		{
			cEditBox* pGuildUnionName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDU_NAME);
			if( strcmp(pGuildUnionName->GetEditText(), "") == 0 )	return;
			if( strlen(pGuildUnionName->GetEditText()) > MAX_GUILD_NAME )	return;
			GUILDUNION->SendCreateGuildUnion( pGuildUnionName->GetEditText() );
		}
		break;

	default:
		{
			int a = 0;
		}
		break;
	}
}


void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if(lId == ITMALL_BTN1 || lId == ITMALL_BTN2 || lId == ITMALL_BTN3)
		GAMEIN->GetItemShopDialog()->TabChange(lId-ITMALL_BTN1);
	if(GAMEIN->GetItemShopDialog()->IsActive())//[右键仓储][BY:十里坡剑神][QQ:112582793][2019-4-16][21:31:30]
		GAMEIN->GetItemShopDialog()->OnActionEvnet(lId,p,we);
}


// 遁甲/回城 弹出"移动场所目录" SaveMove.bin
void SA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if( lId == SA_MOVETABBTN1 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 0 );
		return;
	}
	if( lId == SA_MOVETABBTN2 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 1 );
		return;
	}
		
	if( !GAMEIN->GetMoveDialog()->IsTownMove() && !GAMEIN->GetMoveDialog()->GetSelectedDBIdx() )
		return;

	switch(lId)
	{
	case SA_CHANGNAME:
		{
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( FALSE );			
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );
			GAMEIN->GetMoveDialog()->SetDisable(TRUE);
		}		
		break;
	case SA_MOVEOK:		
		{
//#ifdef _KOR_LOCAL_
			GAMEIN->GetMoveDialog()->MapMoveOK();		
/*#else
			if( GAMEIN->GetMoveDialog()->IsTownMove() )
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(781) );
			else
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(759) );		
#endif	*/	
		}
		break;
	case SA_MOVEDEL:
		WINDOWMGR->MsgBox( MBI_SAVEDPOINTDEL, MBT_YESNO, CHATMGR->GetChatMsg(760) );
		break;
	case CMI_CLOSEBTN:
		GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
		break;
	}
}


// 遁甲 名称变更 "相应场所名称" ChangeName.bin
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_NAME_OKBTN:
		GAMEIN->GetPointSaveDialog()->ChangePointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	case CHA_NAME_CANCELBTN:
		GAMEIN->GetPointSaveDialog()->CancelPointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	}
}

// guildfieldwar
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	// declare
	case GFW_DECLARE_OKBTN:
		{
			cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_DECLARE_EDITBOX );
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_MONEY_EDITBOX );
			GUILDFIELDWAR->SendGuildFieldWarDeclare( pName->GetEditText(), RemoveComma(pMoney->GetEditText()) );
		}
		break;
	case GFW_DECLARE_CANCELBTN:
		{
			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
		}
		break;
	case GFW_MONEY_CHK:
		{
			GAMEIN->GetGFWarDeclareDlg()->ShowMoneyEdit();
		}
		break;

	// result
	case GFW_RESULT_OKBTN:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 4 ) )
			{
				GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_ACCEPT, MBT_YESNO, CHATMGR->GetChatMsg( 813 ) );
			}
		}
		break;
	case GFW_RESULT_CANCELBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_DENY, MBT_YESNO, CHATMGR->GetChatMsg( 814 ) );
		}
		break;
	case GFW_RESULT_CONFIRMBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetActive( FALSE );
		}
		break;

	// info
	case GFW_INFO_SUGGEST_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST, MBT_YESNO, CHATMGR->GetChatMsg( 831 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_INFO_SURREND_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SURREND, MBT_YESNO, CHATMGR->GetChatMsg( 825 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_GDU_REMOVE_OKBTN:	// guildunion remove
		{
			char* pName = GUILDUNION->GetGuildUnionNameFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pName )
				WINDOWMGR->MsgBox( MBI_GUILD_UNION_REMOVE, MBT_YESNO, CHATMGR->GetChatMsg(1116), pName );
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
		}
		break;
	case GFW_INFO_CANCELBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
		}
		break;

	// warinfo
	case GW_INFOCLOSEBTN:
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( FALSE );
		}
		break;
	}
}


// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case SK_UPBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();
				if( Level == -1 )					return;
				CAbilityInfo* pInfo = GAMEIN->GetSkPointDlg()->GetCurAbilityInfo();
				if( !pInfo )	return;
				if(ABILITYMGR->CanUpgradeSkPoint(pInfo,HERO->GetAbilityGroup()) == FALSE)		return;
//#ifdef _TL_LOCAL_
//				sprintf( buf, CHATMGR->GetChatMsg(739), Level+1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
//#else
				sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level+1 );
//#endif

				WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_UP, MBT_YESNO, buf );
				GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
			}			
		}
		break;
	case SK_DOWNBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();				
				if( Level > 0 )
				{
//#ifdef _TL_LOCAL_
//					sprintf( buf, CHATMGR->GetChatMsg(739), Level-1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
//#else
					sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level-1 );
//#endif
					WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_DOWN, MBT_YESNO, buf );
					GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SK_OKBTN:
		{			
			GAMEIN->GetSkPointDlg()->SetActive( FALSE );
		}
		break;
	case SK_STARTBTN:
		{
			MOVEMGR->HeroMoveStop();

			GAMEIN->GetSkPointNotifyDlg()->SetActive( FALSE );
			GAMEIN->GetSkPointDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		break;
	case SK_POINTAGAIN1BTN:
	case SK_POINTAGAIN2BTN:
	case SK_POINTAGAIN3BTN:
		{
			if( (lId-SK_POINTAGAIN1BTN) == GAMEIN->GetSkPointDlg()->GetTabNumber() )
				return;

			GAMEIN->GetSkPointDlg()->SetTabNumber( lId-SK_POINTAGAIN1BTN );
		}
		break;
	}
}

void PW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case PW_CB_MEMBER0:	case PW_CB_MEMBER1:	case PW_CB_MEMBER2:	case PW_CB_MEMBER3:	case PW_CB_MEMBER4:
	case PW_CB_MEMBER5:	case PW_CB_MEMBER6:	case PW_CB_MEMBER7:	case PW_CB_MEMBER8:	case PW_CB_MEMBER9:
	case PW_CB_MEMBER10:	case PW_CB_MEMBER11:	case PW_CB_MEMBER12:	case PW_CB_MEMBER13:
		{
			PARTYWAR->SendPartyWarMember( lId - PW_CB_MEMBER0 );
		}
		break;
	case PW_BTN_LOCK:	PARTYWAR->SendPartyWarLock( TRUE );		break;
	case PW_BTN_UNLOCK:	PARTYWAR->SendPartyWarLock( FALSE );	break;
	case PW_BTN_START:	PARTYWAR->SendPartyWarStart();			break;
	case PW_BTN_CANCEL:	PARTYWAR->SendPartyWarCancel();			break;
	}

}


void CHS_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case CHASE_SEEOKBTN:
		GAMEIN->GetChaseDlg()->SetActive( FALSE );
		break;
	case CHASE_OKBTN:
		GAMEIN->GetChaseinputDlg()->WantedChaseSyn();
		break;
	case CHASE_CANCELBTN:
		GAMEIN->GetChaseinputDlg()->SetActive( FALSE );
		break;
	}
}


void CHAN_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case CH_NAME_CHANGE_OKBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->NameChangeSyn();
		}		
		break;
	case CH_NAME_CHANGE_CANCELBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->SetItemDBIdx( 0 );
			GAMEIN->GetNameChangeDlg()->SetActive( FALSE );
		}
		break;
	case CH_NAME_CHANGE_STARTBTN:
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
			}
			else
			{
				pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVNPLUS);
				if(pItem)
				{
					GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
					GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
					GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
				}
			}

		}
		break;

	}
}


void GDT_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case GDT_ENTRYBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(960) );
		}
		break;
	case GDT_CANCELBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGISTCANCEL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(964) );
		}
		break;
	case GDT_WARVIEWBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->GetSelectedBattle() > -1 )
					WINDOWMGR->MsgBox( MBI_GTBATTLELIST_OBSERVER, MBT_YESNO, CHATMGR->GetChatMsg(956) );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(957) );
			}
		}
		break;
	case GDT_VIEWCANCELBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->IsActive() )
					GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
			}
		}
		break;
	case GDT_SEMIFINAL_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 0 );
		}
		break;
	case GDT_AGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 1 );
		}
		break;
	case GDT_BGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 2 );
		}
		break;
	case GDT_CGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 3 );
		}
		break;
	case GDT_DGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 4 );
		}
		break;
	case GDT_OUTBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTSCORE_EXIT, MBT_YESNO, CHATMGR->GetChatMsg(969) );
		}
		break;
	}
}

void SW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case SW_TIMEREG_OKBTN:
		{
			cEditBox* pYear = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_YEAR);
			cEditBox* pMon = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_MON);
			cEditBox* pDay = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_DAY);
			cEditBox* pHour = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_HOUR);
			if( !pYear || !pMon || !pDay || !pHour )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( (strcmp(pYear->GetEditText(), "") == 0) || (strcmp(pMon->GetEditText(), "") == 0) ||
				(strcmp(pDay->GetEditText(), "") == 0) || (strcmp(pHour->GetEditText(), "") == 0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}

			DWORD dwYear = (DWORD)atoi( pYear->GetEditText() );
			DWORD dwMon = (DWORD)atoi( pMon->GetEditText() );
			DWORD dwDay = (DWORD)atoi( pDay->GetEditText() );
			DWORD dwHour = (DWORD)atoi( pHour->GetEditText() );

			if( (dwYear<2005) || (dwMon>12||dwMon<=0) || (dwDay>31||dwDay<=0) || (dwHour>23||dwHour<0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( dwYear%4 == 0 )
				if( dwDay > DayOfMonth_Yundal[dwMon] )	return;
			else
				if( dwDay > DayOfMonth[dwMon] )			return;

			stTIME time;
			time.SetTime( dwYear, dwMon, dwDay, dwHour, 0, 0 );	//符咒

			MSG_DWORD msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_REGISTTIME_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = time.value;
			NETWORK->Send( &msg, sizeof(msg) );

			GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
		}
		break;
	case SW_PROTECTREG_OKBTN:
		{
			int nIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedListIdx();
			if( nIdx > -1 )
			{
				WINDOWMGR->MsgBox( MBI_SW_PROTECTREG_OK, MBT_YESNO, CHATMGR->GetChatMsg(1121), GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildName( nIdx ) );
				GAMEIN->GetSWProtectRegDlg()->SetDisable( TRUE );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1118) );				
		}
		break;
	case SW_INFOCLOSEBTN:
		{
			GAMEIN->GetSWInfoDlg()->SetActive( FALSE );
		}
		break;
	case SW_TAXCHANGE_OKBTN:
		{
			cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
			if( pTax )
			{
				if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_TAXCHANGE_OK, MBT_YESNO, CHATMGR->GetChatMsg(1029), atoi(pTax->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_OKBTN:
		{
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
			if( pMoney )
			{
				if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_PROFIT_OK, MBT_YESNO, CHATMGR->GetChatMsg(1030), atoi(pMoney->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_CANCELBTN:
		{
			GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
		}
		break;
	case SW_ENGRAVECANCEL:
		{
			if( GAMEIN->GetSWEngraveDlg() )
				GAMEIN->GetSWEngraveDlg()->CancelEngraveSyn();
		}
		break;
	case SW_STARTTIME_OKBTN:
		{
			GAMEIN->GetSWStartTimeDlg()->SetActive( FALSE );
		}
		break;
	}
}


void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we)
{
	CCharChangeDlg* pDlg = (CCharChangeDlg*)p;
	
	switch( lId )
	{
	case CHA_CharMake:
		pDlg->CharacterChangeSyn();
		break;
	case CHA_CharCancel:
		pDlg->Reset( FALSE );
		pDlg->SetActive( FALSE );
		break;
	case CHA_SexType1:
		pDlg->ChangeSexType( TRUE );
		break;
	case CHA_SexType2:
		pDlg->ChangeSexType( FALSE );
		break;
	case CHA_HairType1:
		pDlg->ChangeHairType( TRUE );
		break;
	case CHA_HairType2:
		pDlg->ChangeHairType( FALSE );
		break;
	case CHA_FaceType1:
		pDlg->ChangeFaceType( TRUE );
		break;
	case CHA_FaceType2:
		pDlg->ChangeFaceType( FALSE );
		break;
	}
}


void IT_DlgFunc(LONG lId, void* p, DWORD we)
{
	CSealDialog* pDlg = (CSealDialog*)p;
	
	switch( lId )
	{
	case IT_LOCKOKBTN:
		pDlg->ItemSealSyn();
		break;
	case IT_LOCKCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}


void CJOB_DlgFunc(LONG lId, void* p, DWORD we)
{
	CChangeJobDialog* pDlg = (CChangeJobDialog*)p;

	switch( lId )
	{
	case CJOB_OKBTN:
		pDlg->ChangeJobSyn();
		break;
	case CJOB_CANCELBTN:
		pDlg->CancelChangeJob();
		break;		
	}
}

void CG_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuageDlg()->OnActionEvent(lId, p, we);
}


void RFDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	CReinforceResetDlg* pDlg = (CReinforceResetDlg*)p;

	switch( lId )
	{
	case RF_DefaultOKBTN:
		pDlg->ItemResetSyn();
		break;
	case RF_DefaultCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}

void RareCreate_DlgFunc(LONG lId, void *p, DWORD we)
{
    CRareCreateDialog* pDlg = (CRareCreateDialog*)p;

	switch( lId )
	{
	case RareCreate_OKBTN:
		pDlg->ItemRareCreateSyn();
		break;
	case RareCreate_CANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}

void PET_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateDialog()->OnActionEvent(lId, p, we);
}

void PET_MiniDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateMiniDialog()->OnActionEvent(lId, p, we);
}

void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetReinforceDataGuideDlg()->OnActionEvent(lId, p, we);
}

void TB_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void PET_InvenDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetInventoryDialog()->OnActionEvent(lId, p, we);
}

void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetUpgradeDialog* pDlg = (CPetUpgradeDialog*)p;
	switch( lId )
	{
	case PetUpgrade_OKBTN:
		{
			pDlg->PetUpgradeSyn();
		}
		break;
	case PetUpgrade_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}

void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetRevivalDialog* pDlg = (CPetRevivalDialog*)p;
	switch( lId )
	{
	case PetRevival_OKBTN:
		{
			pDlg->PetRevivalSyn();	
		}
		break;
	case PetRevival_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}

void GN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoteDlg()->OnActionEvnet(lId, p, we);
}

void AN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetUnionNoteDlg()->OnActionEvnet(lId, p, we);
}

void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoticeDlg()->OnActionEvnet(lId, p, we);
}

void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we)//Add 060803 by wonju
{
	GAMEIN->GetGuildPlusTimeDlg()->OnActionEvnet(IId,p,we);
}

void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionChangeDlg()->OnActionEvnet(lId, p, we);
}

void TDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionClearDlg()->OnActionEvnet(lId, p, we);
}
//////////////////////////////////////////////////////////////////

void JO_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildJoinDialog()->OnActionEvent(lId, p, we);
}

void GU_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildTraineeDialog()->OnActionEvent(lId, p, we);
}
///////////////////////////////////////////////////////////////////////////////////

void GT_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildMunhaDialog()->OnActionEvent(lId, p, we);
}
///////////////////////////////////////////////////////////////////////////////////

void TPMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsMakeDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void TitanMix_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRepairDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////
void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRecallDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanUpgradeDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanBreakDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanInventoryDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanGuageDlg()->OnActionEvent(lId, p, we);
}

void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRegisterDlg()->OnActionEvent(lId, p, we);
}

void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanDissolutionDlg()->OnActionEvent(lId, p, we);
}

void UniqueItemCurseCancellationDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemCurseCancellationDlg()->OnActionEvent(lId, p, we);
}

void UniqueItemMixDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixDlg()->OnActionEvent(lId, p, we);
}

void SOSDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSOSDlg()->OnActionEvent(lId, p, we);
}

void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////
void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////
void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSkinSelectDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetCostumeSkinSelectDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void SK_Reset_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetSkillPointResetDlg()->OnActionEvent(lId, p, we);
}


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////

void SVD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallItemViewDlg()->OnActionEvent(lId, p, we);
}

void AutoNoteDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoNoteDlg()->OnActionEvent(lId, p, we);
}

void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoAnswerDlg()->OnActionEvent(lId, p, we);
}

void PA_CreateBtnFunc(LONG lId, void * p, DWORD we) 
{
	GAMEIN->GetPartyCreateDialog()->OnActionEvent(lId, p, we);
}

void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetPartyMatchingDlg()->OnActionEvent(lId, p, we);
}

void FW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case FW_ENGRAVECANCEL:	GAMEIN->GetFWEngraveDlg()->OnActionEvent(lId, p, we);	break;
	}
}

void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case FW_PUTOUTMONEYBTN:	GAMEIN->GetFWWareHouseDlg()->PutOutMoneyMsgBox();	break;
	}
}
void ITEM_GOLDDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetGoldShopMainDlg()->OnActionEvent(lId,p,we);
}
void  BARCONTROL_DIALOGFunc(LONG lId, void * p, DWORD we)
{
	CControlDialog* pDlg=(CControlDialog*)p;

	switch(lId)
	{
   case BAR_PH_BTN_OPEN:	//[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	   {
		   cBillBoardDialog* pHandler = GAMEIN->GetBillboardDialog();  

		   if( !pHandler ) return;

		   if( pHandler->IsActive() )
		   {
			   pHandler->SetActive( FALSE );
		   }
		   else
		   {
			   pHandler->OpenBillBoard();
		   }			
	   }
	   break;
   case BAR_PH_GUILD_OPEN:	//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	   {
		   GuildTopList * pHandler2 = GAMEIN->GetGuildTopListDlg();
		   if(!pHandler2) return;
		   if (pHandler2->IsActive())
			   pHandler2->SetActive(FALSE);
		   else
			   pHandler2->OpenGuildTopList();
	   }
	   break;
   case BAR_PH_KILL_OPEN:
	   {//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
			cKillTopList* pHandler3 = GAMEIN->GetKillTopListDlg();
			if(!pHandler3) return;
			if(pHandler3->IsActive())
				pHandler3->SetActive(FALSE);
			else
				pHandler3->OpenKillBoard();
	   }
	   break;
   case SAFELOCK_ULOCK_BTN:
	   {//[安全锁解锁][By:十里坡剑神][QQ:112582793][2018/4/30]
		   cSafeLockDialog * pLockDlg = GAMEIN->GetSafeLockDlg();
		   if(!pLockDlg) return;
		   if(pLockDlg->IsActive())
			   pLockDlg->Close();
		   else
			   pLockDlg->Show(FALSE);
	   }
	   break;
   case SAFELOCK_RESTPWD_BTN:
	   {//[安全锁修改密码][By:十里坡剑神][QQ:112582793][2018/4/30]
		   cSafeLockDialog * pLockDlg = GAMEIN->GetSafeLockDlg();
		   if(!pLockDlg) return;
		   if(pLockDlg->IsActive())
			   pLockDlg->Close();
		   else
			   pLockDlg->Show(TRUE);
	   }
	   break;
   case SAFELOCK_RELOCK_BTN:
	   {// [安全锁加判断] [by:十里坡剑神][QQ:112582793][2018/11/20]
			MSG_SAFELOCK msg;
			msg.Category = MP_SAFELOCK;
			msg.Protocol = MP_SAFELOCK_RELOCK_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg,sizeof(msg));
			GAMEIN->GetMainDlgSafeBtn()->Show(FALSE);
			CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2300));
			g_SafeLockIsOpen = FALSE;
	   }
	   break;
   case SYS_INFO_BOBUSANGINFO:
	   {//[包袱商人信息获取][BY:十里坡剑神][QQ:112582793][2019-6-12][13:37:35]
	       DWORD dwTime = 0;
		   if (HERO->CanSendBobuSangInfoMSG(dwTime))
		   {
			   MSGBASE msg;
			   msg.Category = MP_BOBUSANG;
			   msg.Protocol = MP_CHEAT_BOBUSANGINFO_GET_SYN;
			   msg.dwObjectID = HEROID;
			   NETWORK->Send(&msg, sizeof(msg));
			   HERO->SetBobuSangInfoMsgTime();
		   }
		   else
		   {
			   CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2374), 60-(dwTime/1000));
		   }
	   }
	   break;
	}
}

void IT_LOCKDlgFunc(LONG lId,void * p,DWORD we)
{
    CItemLockDialog* pHandler = (CItemLockDialog*)GAMEIN->GetItemLockDialog();

	if(!pHandler) return;

	switch(lId)
	{
	case IT_LOCKOKBTNEX:
		{
			if(!pHandler->VirtualItemIsNull())
			{
				pHandler->SendSynMsg();
			}
			pHandler->SetActive(FALSE);
		}
		break;
	case IT_LOCKCANCERBTNEX:
		{
			if(pHandler->IsActive())
			{
				pHandler->Release();
			}
		}
		break;
	}
}

void IT_UNLOCKDlgFunc(LONG lId,void * p,DWORD we)
{
    CItemUnLockDialog* pHandler = (CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();

	if(!pHandler) return;

	switch(lId)
	{
	case IT_UNLOCKOKBTNEX:
		{
			if(!pHandler->VirtualItemIsNull())
			{
				pHandler->SendSynMsg();
			}
			pHandler->SetActive(FALSE);
		}
		break;
	case IT_UNLOCKCANCERBTNEX:
		{
			if(pHandler->IsActive())
			{
				pHandler->Release();
			}
		}
		break;
	}
}

void CG_TargetObjectBtnFunc( LONG lId, void* p, DWORD we )
{
	CObject* obj =OBJECTMGR->GetSelectedObject();
	if(!obj||obj->GetObjectKind()!=eObjectKind_Player)return;
	switch( lId )
	{
	case CG_VIEWTARGETEQ:
		{
			CQuipInfoDlg *pDlg = (CQuipInfoDlg*)GAMEIN->GetCharacterEquipInfoDlg();

			if (!pDlg)
				return;
			
			if (pDlg->IsActive())
			{
				pDlg->SetActive(FALSE);
			}
			else
			{
				pDlg->SendGetInfoMsg();
			}
		}
		break;
	case CG_GETCHARNAME:
		{
			if(GAMEIN->CopyToClipboard(obj->GetObjectName(),MAX_NAME_LENGTH))
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2096) );
		}
		break;
	case CG_WHISPERSEND:
		{
			CHATMGR->SetWhisperName(obj->GetObjectName());
			CHATMGR->AutoWhisperForRbt();
		}
		break;
	case CG_REPORT:
		{
			GAMEIN->GetCharacterRBTNDlg()->SendReportMsg();
		}
		break;
	}
	if(GAMEIN->GetCharacterRBTNDlg()->IsActive())
		GAMEIN->GetCharacterRBTNDlg()->SetActive(FALSE);
}
void TB_Main_Dlg_BtnFunc(LONG lId,void *p,DWORD we)
{//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	if(GAMEIN->GetSafeLockMainDlg()->IsActive())
	{
		GAMEIN->GetSafeLockMainDlg()->SetActive(FALSE);
	}
	else
	{
		GAMEIN->GetSafeLockMainDlg()->SetActive(TRUE);
	}
}
void TB_Dlg_Hide_BtnFunc(LONG lId,void *p,DWORD we)
{//[界面信息隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
	BOOL bIsHide =GAMEIN->GetOnlineFameDlg()->IsActive(); 
	GAMEIN->GetOnlineFameDlg()->SetActive(!bIsHide);
	bIsHide = GAMEIN->GetTopRankDlg()->IsActive();
	GAMEIN->GetTopRankDlg()->SetActive(!bIsHide);
}

void CI_RechargeDlgFunc(LONG lId, void * p, DWORD we)
{//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	if ( we & WE_BTNCLICK )
	{
		if(lId==RECHARGE_BTN_DLG)
		{
			if (GAMEIN->GetRechargeDlg()->IsActive())
				GAMEIN->GetRechargeDlg()->SetActive(FALSE);
			else
				GAMEIN->GetRechargeDlg()->SetActive(TRUE);
		}
		else if(lId==RE_BTN_RECHARGE)
		{
			ShellExecute( NULL, "open", CHATMGR->GetChatMsg(2178),NULL, NULL, SW_SHOWNORMAL );
			cMsgBox * pMsgBox = WINDOWMGR->MsgBox(MSBOX_RECHARGE,MBT_OK,CHATMGR->GetChatMsg(2160));
		}
		else if(lId==SYS_INFO_BTN_OPEN)
		{//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
			if(!GAMEIN->GetSetingInfoDlg()->IsActive())
				GAMEIN->GetSetingInfoDlg()->Show(TRUE);
		}
		else if(lId == SYS_INFO_BTN_CLOSE)
		{
			if(GAMEIN->GetSetingInfoDlg()->IsActive())
				GAMEIN->GetSetingInfoDlg()->Show(FALSE);
		}
		else
			GAMEIN->GetRechargeDlg()->OnActionEvent(lId, p, we);
	}
	if ( we & WE_PUSHDOWN )
	{
		if(lId == STATU_ICON_BTN_OPEN)
		{//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
			STATUSICONDLG->SetShow(FALSE);
		}
	}
	else if(we & WE_PUSHUP)
	{
		if(lId == STATU_ICON_BTN_OPEN)
		{
			STATUSICONDLG->SetShow(TRUE);
		}
	}
}
void CI_InventoryRBTDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetInventoryRBTDlg()->OnActionEvent(lId,p,we);
}
void CI_SingedDlgFunc(LONG lId, void * p, DWORD we)
{//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	if ( we & WE_BTNCLICK )
	{
		if (lId==SINGED_MAINBTN)
		{
			if (GAMEIN->GetSingedDlg()->IsActive())
				GAMEIN->GetSingedDlg()->SetActive(FALSE);
			else
				GAMEIN->GetSingedDlg()->SetActive(TRUE);
		}
		else
			GAMEIN->GetSingedDlg()->OnActionEvent(lId, p, we);
	}
	if(we & WE_LBTNCLICK)
	{//[击杀榜奖励领取][By:十里坡剑神][QQ:112582793][2018/4/27]
		if(lId==BILL_KILL_ICON_ITEM)
			GAMEIN->GetKillTopListDlg()->OnActionEvent(lId, p, we);
	}
}

void CI_RegIDDlgFunc(LONG lId,void *p,DWORD we)		
{//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	if(we & WE_BTNCLICK && lId == MT_REGBTN)
		GAMEIN->GetRegID()->Show();
	if(we & WE_BTNCLICK && lId == REGNO)
		GAMEIN->GetRegID()->Close();
	if(we & WE_BTNCLICK && lId == REGOK)
		GAMEIN->GetRegID()->OnActionEvent(lId,p,we);
	if(we & WE_BTNCLICK && lId == MT_RESTPWD)
		GAMEIN->GetRestPwdDlg()->Show();
	if(we & WE_BTNCLICK && lId == RESTPWD_NO)
		GAMEIN->GetRestPwdDlg()->Close();
	if(we & WE_BTNCLICK && lId == RESTPWD_OK)
		GAMEIN->GetRestPwdDlg()->OnActionEvent(lId,p,we);
}
void CI_KeepIDFunc(LONG lId,void *p,DWORD we)
{//[记录帐号密码][By:十里坡剑神][QQ:112582793][2017/12/6]
	cDialog* pDlg = (cDialog*)WINDOWMGR->GetWindowForID(MT_LOGINDLG);
	if(pDlg)
	{
		if(((cCheckBox*)pDlg->GetWindowForID(MT_KEEPID))->IsChecked())
		{
			char * userID = ((cEditBox *)pDlg->GetWindowForID(MT_IDEDITBOX))->GetEditText();
			char * userPWD = ((cEditBox *)pDlg->GetWindowForID(MT_PWDEDITBOX))->GetEditText();
			if(strlen(userID)==0||strlen(userID)==0)
			{
				::MessageBox(_g_hWnd,"请先填写账号密码","提示",MB_OK);
				((cCheckBox*)pDlg->GetWindowForID(MT_KEEPID))->SetChecked(FALSE);
				return;
			}
			WINDOWMGR->SetUserCfg(TRUE,userID,userPWD);
		}
		else
		{
			((cEditBox *)pDlg->GetWindowForID(MT_IDEDITBOX))->SetEditText("");
			((cEditBox *)pDlg->GetWindowForID(MT_PWDEDITBOX))->SetEditText("");
			WINDOWMGR->SetUserCfg(FALSE,"","");
		}
	}
}
void TB_CharacterDialogFunc(LONG lId,void *p,DWORD we)
{//[自动加点][By:十里坡剑神][QQ:112582793][2018/4/17]
	GAMEIN->GetCharacterDialog()->OnActionEvent(lId, p, we);
}
void TB_SafeLockDialogFunc(LONG lId,void *p,DWORD we)
{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	GAMEIN->GetSafeLockDlg()->OnActionEvent(lId, p, we);
}
void BigMapDlgFunc(LONG lId, void * p, DWORD we)
{//[大地图自动寻路][By:慌张的油条][QQ:112582793][2018/9/17]
	if( p == NULL )
		return;
	((CBigMapDlg*)p)->OnActionEvent( lId, p, we );
}
void VideoCaptureDlgFunc(LONG lId, void* p, DWORD we )
{//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	cDialog* pVideoCaptureDlg = (cDialog*)p;
	if( !pVideoCaptureDlg )
		return;

	switch( lId )
	{
	case VIDEOCAPTURE_START_BTN:
		{
			pVideoCaptureDlg->SetActive( FALSE );

			cStatusIconBtnDlg* pMiniMapDlg = GAMEIN->GetStatusIconBtnDlg();
			if( !pMiniMapDlg )
				break;

			pMiniMapDlg->ShowVideoCaptureStartBtn( FALSE );

			VIDEOCAPTUREMGR->CaptureStart();
		}
		break;
	case VIDEOCAPTURE_CANCEL_BTN:
		{
			pVideoCaptureDlg->SetActive( FALSE );
		}
		break;
	}
}
#ifdef _XDYY_
void OnlineVoiceDlgFunc(LONG lId, void* p, DWORD we )
{//[小队语音界面消息响应][By:十里坡剑神][QQ:112582793][2019/3/20][15:29:03]
	if(GAMEIN->GetOnlineVoiceDlg())
		GAMEIN->GetOnlineVoiceDlg()->OnActionEvent(lId,p,we);
}
void OnlineVoiceGuageFunc(LONG lId, void* p, DWORD we,long Value)
{
	if(GAMEIN->GetOnlineVoiceDlg())
		GAMEIN->GetOnlineVoiceDlg()->OnVoiceSetEvent(lId,p,we,Value);
}
void OnlineVoiceMemberFunc(LONG lId, void* p, DWORD we )
{
	if(GAMEIN->GetPartyDialog())
		GAMEIN->GetPartyDialog()->OnActionEvent(lId,p,we);
}
#endif
void FastConfiPointDlgFunc(LONG lId, void* p, DWORD we )
{//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
	FastConfigPointDlg * pDlg = (FastConfigPointDlg *)p;
	if(pDlg)
	{
		pDlg->OnActionEvent(lId,p,we);
	}
}

void LotSplitItemDlgFunc(LONG lId, void* p, DWORD we )
{//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:11:24]
	LotSplitItemDlg* pDlg = (LotSplitItemDlg*)p;
	if(pDlg)
	{
		pDlg->OnActionEvent(lId,p,we);
	}
}

void VimuWithGoldDlgFunc(LONG lId, void* p, DWORD we )
{//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:47:33]
	VimuWithGoldDlg * pDlg = (VimuWithGoldDlg*)p;
	if(pDlg)
	{
		pDlg->OnActionEvent(lId,p,we);
	}
}
void EquipItemInherit(LONG lId, void* p, DWORD we)
{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:35:03]
	switch (lId)
	{
	case INHERIT_OKBTN:
		{
			((cEquipItemInherit*)p)->ItemInheritCreateSyn();
		}
		break;
	case INHERIT_CANCERBTN:
		{
			((cEquipItemInherit*)p)->ReleaseItem();
		}
		break;
	case INHERIT_GROWCHECK:
		{
			if (we & WE_CHECKED)
			{
				WINDOWMGR->MsgBox(MBI_NOBTNMSGBOX, 1, CHATMGR->GetChatMsg(2386));
			}
		}
		break;
	default:
		break;
	}
}
void TopListUpdate(LONG lId, void* p, DWORD we)
{//[排行榜刷新][BY:十里坡剑神][QQ:112582793][2019-9-5][0:57:02]

	switch (lId)
	{
	case TOPLIST_UPDATE_BTN:
		{
			if (sBOARDMGR->IsCanUpdate())
			{
				MSGBASE msg;
				msg.Category = MP_TOPLIST;
				msg.Protocol = MP_TOPLIST_TO_GET_SYN;
				NETWORK->Send(&msg, sizeof(msg));
			}
		}
		break;
	}
}

void FBItemTimeAdd(LONG lId, void* p, DWORD we)
{//[副本卷时间添加][BY:十里坡剑神][QQ:112582793][2019-9-21][13:49:07]
	if (lId == FB_TIME_SET_BTN)
	{
		WORD wTime = ((FBTipDlg*)p)->GetInputTime();
		if (wTime == 0)
			return;
		if (HERO->GetGoldMoney() < wTime * 1000)
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(2105));
			return;
		}
		cMsgBox* msgBox = WINDOWMGR->MsgBox(MSBOX_FBTIME, MBT_YESNO, CHATMGR->GetChatMsg(2420), wTime, wTime * 1000);
	}
}