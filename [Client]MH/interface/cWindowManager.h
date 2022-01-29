// cWindowManager.h: interface for the cWindowManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_)
#define AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindowDef.h"

#include "IndexGenerator.h"
#include "PtrList.h"
#include "cDialog.h"


class cWindow;
class cImage;
class cDialog;
class cIconDialog;
class cMultiLineText;
class CMousePointer;
class cScriptManager;
class cAnimationManager;
class cMsgBox;
class cDivideBox;
class cDivideBoxEx; 
class CKeyboard;
class CMouse;


#define WINDOWMGR	USINGTON(cWindowManager)
typedef void (*cbDROPPROCESSFUNC)(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY);
class cDialog;
class cWindowManager  
{
public:
	cWindowManager();
	virtual ~cWindowManager();

	void Init();
	void AfterInit();
	void Preprocess();
	void DestroyWindowProcess();
	void AddListDestroyWindow(cWindow * window);
	void Process();
	void Render();
	cbDROPPROCESSFUNC cbDragnDropProcess;
	void SetcbDropProcess(cbDROPPROCESSFUNC func) { cbDragnDropProcess = func; }
	cDialog * GetWindowForID(LONG id);
	cWindow * GetWindowForIDEx(LONG id);
	cDialog * GetWindowForXY(LONG x, LONG y);
	cDialog * GetWindowForXYExceptIcon(LONG x, LONG y);
	cDialog * GetWindowForXYExceptID(LONG x, LONG y, LONG id);
	cIconDialog * GetIconDialogForXY(LONG x, LONG y);
	void CloseWindowForID(LONG id);
	void DestroyWindowAll();
	void DeleteWindowForID(LONG id);
	void DeleteWindow( cWindow* pWindow );
	void DeleteEditBoxListAll();
	void DeleteIconDlgListAll();
	void DeleteChatTooltipListAll();
	void DeleteSSTitletipListAll();
	void Release();
	cbWindowProcess cbProcess;
	cScriptManager * m_pScriptManager;
	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");

	cImage * GetImagePath(int idx, char * path, int size)
	{
		return NULL;
	}

	cImage * GetImageOfFile(char * filePath, int layer)
	{
		return NULL;
	}

	void CreateGameIn();
	void CreateCharDlg();
	void CreateMugongSuryunDlg();
	void CreateInventoryDlg();
	void CreateExchangeDlg();
	void CreateRecvExchangeDlg();
	void CreateMixDlg();
	void CreateTitanPartsMakeDlg();	
	void CreateTitanMixDlg();			
	void CreateTitanRepairDlg();		
	void CreateTitanRecallDlg();		
	void CreateTitanUpgradeDlg();		
	void CreateTitanBreakDlg();			
	void CreateTitanChangePreViewDlg();
	void CreateTitanInventoryDlg();		
	void CreateTitanGuageDlg();			
	void CreateTitanRegisterDlg();		
	void CreateTitanDissolution();		
	void CreateDealDlg();
	void CreateMarryMiniDialog();  
	void CreateControlDialog();  
 
	void CreateOtherCharacterEquip();
	void CreateItemLockDlg();         
	void CreateItemUnLockDlg();     
	void CreateChatDlg();
	void CreateHeroGuage();
	void CreateQuickDlg();
	void CreatePartyDlg();
	void CreatePartyCreateDlg();
	void CreatePartyInviteDlg();
	void CreateGuildCreateDlg();
	void CreateGuildDlg();
	void CreateGuildInviteDlg();
	void CreateGuildMarkDlg();
	void CreateGuildLevelupDlg();
	void CreateGuildNickNameDlg();
	void CreateGuildRankDlg();
	void CreateGuildWarehouseDlg();
	void CreatePyogukDlg();
	void CreateNoteDlg();
	void CreateFriendDlg();
	void CreateMiniFriendDlg();
	void CreateMiniMapDlg();
	void CreateMiniNoteDlg();
	void CreateReviveDlg();
	void CreateWantNpcDlg();
	void CreateWantRegistDlg();
	void CreateQuestTotalDlg();
	void CreateMPRegistDlg();
	void CreateMPMissionDlg();
	void CreateMPNoticeDlg();
	void CreateMPGuageDlg();
	void CreatePetStateDlg();
	void CreatePetStateMiniDlg();
	void CreatePetInvenDlg();
	void CreatePetUpgradeDlg();
	void CreatePetRevivalDlg();
	void CreateStallKindSelectDlg();
	void CreateStreetBuyStallDlg();
	void CreateBuyRegDlg();
	void CreateStallFindDlg();
	void CreateStreetStallDlg();
	void CreateNpcScriptDlg();
	void CreateHelpDlg();
	void CreateMacroDlg();
	void CreateChatOptionDlg();
	void CreateOptionDlg();
	void CreateExitDlg();
	void CreateCharStateDlg();
	void CreateMenuSlotDlg();
	void CreateUpgradeDlg();
	void CreateReinforceDlg();
	void CreateDissolveDlg();
	void CreateBailDlg();
	void CreateDissolutionDlg();
	void CreateEventNotifyDlg();
	void CreateMoveDlg();
	void CreatePointSaveDlg();
	void CreateGuildFieldWarDlg();
	void CreateSkPointDlg();
	void CreateSkPointNotifyDlg();
	void CreatePartyWarDlg();
	void CreateShoutDlg();
	void CreateShoutchatDlg();
	void CreateChaseDlg();
	void CreateChaseinputDlg();
	void CreateNameChangeDlg();
	void CreateNameChangeNotifyDlg();
	void CreateGTRegistDlg();
	void CreateGTRegistcancelDlg();
	void CreateGTStandingDlg();
	void CreateGTBattleListDlg();
	void CreateGTScoreInfoDlg();
	void CreateSeigeWarDlg();
	void CreateCharChangeDlg();
	void CreateItemSealDlg();
	void CreateChangeJobDlg();
	void CreateReinforceResetDlg();
	void CreateRareCreateDlg();
	void CreateReinforceDataGuideDlg();
	void CreateTipBrowserDlgDlg();
	void CreateKeySettingTipDlg();
	void CreateGuildNoteDlg();
	void CreateUnionNoteDlg();
	void CreateGuildNoticeDlg();
	void CreateSkillOptionChangeDlg();
	void CreateSkillOptionClearDlg();
	void CreateGuildInvitationKindSelectionDlg();
	void CreateGuildMunhaDlg();
	void CreateGuildTraineeDlg();
	void CreateSurvivalCountDlg();
	void CreateUniqueItemCurseCancellationDlg();
	void CreateUniqueItemMixDlg();
	void CreateSOSDlg();	
	void CreateUniqueItemMixProgressBarDlg();
	void CreateTitanMixProgressBarDlg();
	void CreateTitanPartsProgressBarDlg();
	void CreateSkinSelectDlg();
	void CreateCostumeSkinSelectDlg();
	void CreateSkillPointResetDlg();
	void CreateStreetStallItemViewDlg();
	void CreatePartyMatchingDlg();
	void ScreenShotDlg();
	void CreateItemShopDlg();
	void CreateOnlineFame();			//[在线洗恶][By:十里坡剑神][QQ:112582793][2017/11/22]
	void CreateGuildTopListDlg();		//[帮会排行][By:十里坡剑神][QQ:112582793][2017/11/24]
	void CreateBillboardDialog();		//[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	void CreateCharacterRBTN();			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	void CreateFBTipDlg();				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
	void CreateOpenPyogukDlg();			//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	void CreateSingedDlg();				//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	void CreateRechargeDlg();			//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	void CreateSetingInfoDlg();			//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	void CreateStatusIconBtnDlg();		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	void CreateTopRankDlg();			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	void CreateInventoryRBTDlg();		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	void CreateSendItemInfoDlg();		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	void CreateKillTopListDlg();		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	void CreateSafeLockDlg();			//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	void CreateMainDlgHideBtnDlg();		//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
	void CreateMainDlgSafeBtn();		//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	void CreateSafeLockMainDlg();		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	void CreateVideoCaptureDlg();		//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	void CreateGoldShopMainDlg();		//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
#ifdef _XDYY_
	void CreateOnlineVoiceDlg();		//[小队语音设置界面][By:十里坡剑神][QQ:112582793][2019/3/20][14:57:32]	
#endif

	void CreateFastCfgPointDlg();		//[快速加点][BY:十里坡剑神][QQ:112582793][2019-4-5][17:02:55]
	void CreateLotSplitItemDlg();		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:10:39]
	void CreateVimuWithGoldDlg();		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:53:06]
	void CreateEquipItemInherit();		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:27:18]
	void CreateUnionWarInfoDlg();		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:42:34]
	//[账号密码记录读取][By:十里坡剑神][QQ:112582793][2017/11/23]
	KEEPLOGIN	sKeepLoginInfo;
	void LoadUserIDInfo();
	void SaveUserIDInfo();
	KEEPLOGIN * GetUserCfg(){return &sKeepLoginInfo;}
	void SetUserCfg(BOOL IsKeep,char * id,char * pwd);

	cMsgBox* MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... );	
	cMsgBox* MsgBoxTip(LONG lMBId, int nMBType, char* pStrMsg, ...);
	cDivideBox * DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void * vData2, char* strTitle );

	cDivideBoxEx * DivideBoxEx( LONG lId, LONG x, LONG y, cbDivideExFUNC cbFc1, cbDivideExFUNC cbFc2, void * vData1, void * vData2, char* strTitle );//  2014-10-30  cDivideBoxEx声明

	void CreatMousePoint();

	cPtrList * m_pDestroyWindowRef;				
	LONG ID_SEED;
	inline void AddWindow(cWindow * window)
	{ 
#ifdef _DEBUG
		if(!window)
		{
			char buff[256];
			sprintf(buff, "window有NULL。脚本的脚本错误或路径错误。");
			LOGEX(buff, PT_MESSAGEBOX);
			return;
		}
		PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
		while(pos)
		{
			
			cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
			if(win == window)
			{
				char buff[256];
				sprintf(buff, "[AddWindow()] : 已存在的窗口。");
				LOGEX(buff, PT_MESSAGEBOX);
			}
			m_pWindowList->GetNext(pos);
		}
#endif 
		m_pWindowList->AddTail((void *)window); 
	}


	void AddChatTooltip(cWindow * window)
	{
		m_pPlayerChatTooltip->AddTail(window);
	}
	void RemoveChatTooltipWindow(LONG playerID);	
	void RemoveChatTooltipWindow( cWindow* pWindow );


	cPtrList * m_pWindowList;					
	cPtrList * m_pIconDlgListRef;					
	cPtrList * m_pEditBoxListRef;					
	cPtrList * m_pScreenTextList;					
	cPtrList * m_pPlayerChatTooltip;

	void SetDragStart(LONG sX, LONG sY, LONG x, LONG y, LONG id)
	{
		m_id = id;
		m_pDragDialog = GetWindowForID( id );
		m_OldX = x; m_OldY = y;
		m_posBeforeDrag.x=(float)sX;
		m_posBeforeDrag.y=(float)sY;
		m_OldDragFlag = TRUE;
	}
	void SetDragEnd()
	{
		m_OldX = -1; m_OldY = -1;
		m_OldDragFlag = FALSE;
	}

	void BackDragWindow();
	void BackDragWindow(LONG id);
	void DragWindowNull()
	{
		m_pDragDialog = NULL;
	}

	BOOL IsDragWindow() { return m_OldDragFlag;	}
	
	BOOL CloseAllWindow();
	void ShowBaseWindow();


public:
	void ToggleShowInterface();
	
	BOOL m_OldDragFlag;
	void SetComposition(BOOL val){ m_fComposition = val; }
	BOOL IsComposition(){ return m_fComposition; }
	CMousePointer * GetMouseWindow() { return m_pMousePointWindow;	}

	cWindow* GetFocusedEdit() { return m_pFocusedEdit; }
	void SetFocusedEdit( cWindow* pEdit, BOOL bFocus );
	void SetNextEditFocus();
	void SetOptionAlpha( DWORD dwAlpha );
	void SetWindowTop( cWindow* win );
	void SetToolTipWindow( cMultiLineText* toolTip ) { m_pToolTipWindow = toolTip; }
	void SetRelationTipWindow( cMultiLineText* toolTip ){m_pRelationTip = toolTip;}
	void SetRelationTipWindow1( cMultiLineText* toolTip ){m_pRelationTip1 = toolTip;}
	void SetSendItemInfoTip(cMultiLineText* toolTip ){m_pSendItemInfo = toolTip;}
	BOOL IsMouseInputProcessed()	{ return m_MouseInputProcessed; }

	BOOL IsMouseOverUsed()			{ return m_bMouseOverUsed; }
	BOOL IsMouseDownUsed()			{ return m_bMouseDownUsed; }
	BOOL IsMouseUpUsed()			{ return m_bMouseUpUsed; }
	
	void SetMouseInputProcessed()	{ m_MouseInputProcessed = TRUE; }
	void SetMouseOverUsed()			{ m_bMouseOverUsed = TRUE; }
	void SetMouseDownUsed()			{ m_bMouseDownUsed = TRUE; }
	void SetMouseUpUsed()			{ m_bMouseUpUsed = TRUE; }

	void KeyboardInput( CKeyboard* keyInfo );
	void MouseInput( CMouse* pMouse );
	
	cMsgBox* GetFirstMsgBox();
	void CloseAllMsgBox();
	void CloseAllMsgBoxNoFunc();
	void PositioningDlg( LONG& rX, LONG& rY );

	BOOL CloseAllAutoCloseWindows();
	cDialog* GetDragDlg() { return m_pDragDialog; }


	static void OnWindowEvent(LONG lId, void * p, DWORD we);
protected:

	BOOL			m_MouseInputProcessed;
	BOOL			m_fComposition;

	cDialog*		m_pGuageWindow;
	cWindow*		m_pMonsterGuageWindow;
	CMousePointer*	m_pMousePointWindow;

	cWindow*		m_pFocusedEdit;
	cMultiLineText* m_pToolTipWindow;
	cMultiLineText* m_pRelationTip;
	cMultiLineText* m_pRelationTip1;
	cMultiLineText* m_pSendItemInfo;
	BOOL			m_bMouseOverUsed;
	BOOL			m_bMouseDownUsed;
	BOOL			m_bMouseUpUsed;
	
	LONG		m_id;
	cDialog*	m_pDragDialog;	

	LONG		m_OldX;
	LONG		m_OldY;
	VECTOR2		m_posBeforeDrag;

	BOOL		m_bHideInterface;
	//WORD		m_WindowPosX;
	//WORD		m_WindowPosY;
public:
	BOOL		IsInterfaceHided() { return m_bHideInterface; }
	
//#ifdef _JAPAN_LOCAL_
//protected:
//
//	BOOL	m_bOpenStatus;
//
//public:
//	void SetOpenStatus( BOOL bOpen ) { m_bOpenStatus = bOpen; }
//#endif

};
EXTERNGLOBALTON(cWindowManager);
#endif // !defined(AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_)


