



// GameIn.h: interface for the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)
#define AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "PartyDialog.h"
class CMugongSuryunDialog;
class CMugongDialog;
class CSuryunDialog;
class CInventoryExDialog;
class CExchangeDialog;
class CMixDialog;
class CTitanPartsMakeDlg;		
class CTitanMixDlg;				
class CTitanRepairDlg;			
class CTitanRecallDlg;			
class CTitanUpgradeDlg;			
class CTitanBreakDlg;			
class CTitanChangePreViewDlg;	
class CTitanInventoryDlg;		
class CTitanGuageDlg;			
class CTitanRegisterDlg;		
class CTitanDissolutionDlg;	
class CDealDialog;
class CCharacterDialog;
class CChatDialog;

class cJackpotDialog;

class CMainBarDialog;
class CDebugDlg;

class CStallKindSelectDlg;
class CStreetBuyStall;
class CBuyRegDialog;

class CStallFindDlg;

class CStreetStall;
class cNpcScriptDialog;
class cHelpDialog;

class CMacroDialog;
class CChatOptionDialog;
class COptionDialog;
class cDialog;
class CCharStateDialog;
class CMenuSlotDialog;

class cAuctionBoard;
class CMoneyDlg;
class CQuickDialog;
class CExitDialog;

class CGuildCreateDialog;
class CGuildDialog;
class CGuildInviteDialog;
class CGuildMarkDialog;
class CGuildLevelUpDialog;
class CGuildNickNameDialog;

class CGuildRankDialog;
class CGuildWarehouseDialog;
class CGuildUnionCreateDialog;
class CMiniMapDlg;
class CPyogukDialog;
class CNoteDialog;
class CFriendDialog;
class CMiniNoteDialog;
class CMiniFriendDialog;
class CReviveDialog;
class CMonsterGuageDlg;
class CUpgradeDlg;
class CReinforceDlg;
class CDissloveDlg;
class CWantNpcDialog;
class CWantRegistDialog;
class CQuestTotalDialog;
class CQuestDialog;
class CWantedDialog;
class CJournalDialog;
class CPartyCreateDlg;
class CPartyInviteDlg;
class CBailDialog;
class CDissolutionDialog;
class CEventNotifyDialog;
class CItemShopDialog;
class CMoveDialog;
class CPointSaveDialog;

class CMPRegistDialog;
class CMPMissionDialog;
class CMPNoticeDialog;
class CMPGuageDialog;
class CMHMap;
class CMHCamera;

class CGFWarDeclareDlg;
class CGFWarResultDlg;
class CGFWarInfoDlg;
class CGuildWarInfoDlg;
class CGuildPlusTimeDlg;

class CSkillPointRedist;

class CSkillPointNotify;

class CMallShopDialog;  


class CMiniMaryDialog; 

class CControlDialog;  

class CQuipInfoDlg;     

class CItemLockDialog;  

class CItemUnLockDialog;

class CPartyWarDialog;

class CShoutDialog;
class CShoutchatDialog;
class CChaseDialog;
class CChaseinputDialog;
class CNameChangeDialog;
class CNameChangeNotifyDlg;

class CBigMapDlg;

class CGTRegistDialog;
class CGTRegistcancelDialog;
class CGTStandingDialog;
class CGTBattleListDialog;
class CGTScoreInfoDialog;

class CSWTimeRegDlg;
class CSWProtectRegDlg;
class CSWInfoDlg;

class CCharChangeDlg;
class CSealDialog;
class CSWProfitDlg;
class CSWEngraveDialog;
class CSWTimeDialog;

class CChangeJobDialog;

class CSWStartTimeDialog;
class CGuageDialog;
class CReinforceResetDlg;
class CRareCreateDialog;

class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;
class CPetUpgradeDialog;
class CPetRevivalDialog;

class CReinforceDataGuideDlg;

class CTipBrowserDlg;
class CKeySettingTipDlg;

class CGuildNoteDlg;
class CUnionNoteDlg;

class CGuildNoticeDlg;
class CSkillOptionChangeDlg;
class CSkillOptionClearDlg;

class CGuildJoinDialog;

class CScreenShotDlg;
class CGuildMunhaDialog;
class CGuildTraineeDialog;
class CSurvivalCountDialog;

class CUniqueItemCurseCancellationDlg;	
class CUniqueItemMixDlg;				

class CSOSDlg;

class CUniqueItemMixProgressBarDlg;		
class CTitanMixProgressBarDlg;		
class CTitanPartsProgressBarDlg;	

class CSkinSelectDialog;		
class CCostumeSkinSelectDialog;		

class CSkillPointResetDlg;

class CStreetStallItemViewDlg;

class CAutoNoteDlg;
class CAutoAnswerDlg;

class CPartyMatchingDlg;	

class CFWEngraveDialog;
class CFWTimeDialog;
class CFWWareHouseDialog;

class cBillBoardDialog;		 //[角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
class GuildTopList;			 //[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
class RegID;				//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
class RestPwd;				//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
class CharacterRBTN;		//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
class FBTipDlg;			//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
class OpenPyoguk;		//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
class cSingedDialog;	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
class cRechargeManager;	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
class cSetingInfoDlg;		//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
class cStatusIconBtnDlg;	//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
class OnlineFame;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/10]
class cTopRankDlg;		//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
class cInventoryRBTDlg;		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
class cSendItemInfoDlg;		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
class cKillTopList;			//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
class cSafeLockDialog;		//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
class cMainDlgSafeBtn;		//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
class cSafeLockMainDlg;		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
//class MaintainaceNoteDlg;	//[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
class CVideoCaptureDlg;		//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
class CGoldShopMainDailog;	//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
//class cShopItemInventPlusDlg;  //[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
class FastConfigPointDlg;	//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:04:28]
class LotSplitItemDlg;		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:06:33]
class VimuWithGoldDlg;		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:49:33]
class cEquipItemInherit;	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:32:00]
class cUnionWarManager;		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:39:39]
#define GAMEIN USINGTON(CGameIn)
class CGameIn : public CGameState  
{
	///////////////////// Dialog Pointer //////////////////////////////////
	CMugongSuryunDialog	* m_pMugongSuryunDlg;
public :
	CMugongDialog		* m_pMugongDlg;
private:
	CSuryunDialog		* m_pSuryunDlg;

	CInventoryExDialog	* m_pInventoryDlg;
	CExchangeDialog		* m_pExchangeDlg;
	CMixDialog			* m_pMixDlg;
	CTitanPartsMakeDlg	* m_pTitanPartsMakeDlg;		
	CTitanMixDlg		* m_pTitanMixDlg;				
	CTitanRepairDlg		*m_pTitanRepairDlg;				
	CTitanRecallDlg		*m_pTitanRecallDlg;				
	CTitanUpgradeDlg	* m_pTitanUpgradeDlg;			
	CTitanBreakDlg		* m_pTitanBreakDlg;				
	CTitanChangePreViewDlg	* m_pTitanChangePreViewDlg;
	CTitanInventoryDlg	* m_pTitanInventoryDlg;			
	CTitanGuageDlg		* m_pTitanGuageDlg;				
	CTitanRegisterDlg	* m_pTitanRegisterDlg;			
	CTitanDissolutionDlg* m_pTitanDissolutionDlg;	

	CDealDialog			* m_pDealDlg;

	CCharacterDialog	* m_pCharDlg;
	CChatDialog			* m_pChatDlg;
	
	CMainBarDialog			* m_pMainDlg;
	CDebugDlg			* m_pDebugDlg;			//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]

	CStallKindSelectDlg * m_pStallKindSelectDlg;
	CStreetBuyStall		* m_pStreetBuyStallDlg;
	CBuyRegDialog		* m_pBuyRegDlg;

	CStallFindDlg		* m_pStallFindDlg;

	CStreetStall		* m_pStreetStallDlg;
	cNpcScriptDialog	* m_pNpcScriptDlg;
	cHelpDialog			* m_pHelpDlg;

	CMacroDialog		* m_pMacroDlg;			//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CChatOptionDialog	* m_pChatOptionDlg;		//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	COptionDialog		* m_pOptionDlg;
	CExitDialog			* m_pExitDlg;
	CCharStateDialog	* m_pCharStateDlg;		//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CMenuSlotDialog		* m_pMenuSlotDlg;		//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CDissolutionDialog	* m_pDissolutionDlg;
	CEventNotifyDialog	* m_pEventNotifyDlg;

	cAuctionBoard		* m_pAuctionBoard;		//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]

	CMoneyDlg			* m_MoneyDlg;			//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CQuickDialog		* m_pQuickDlg;
	CGuageDialog		* m_pGuageDlg;

	CPartyDialog		m_PartyDlg;
	CPartyCreateDlg		* m_pPartyCreateDlg;
	CPartyInviteDlg		* m_pPartyInviteDlg;

	CGuildCreateDialog	* m_pGuildCreateDlg;
	CGuildDialog		* m_pGuildDlg;
	CGuildInviteDialog	* m_pGuildInviteDlg;

	CGuildMarkDialog	* m_pGuildMarkDlg;
	CGuildLevelUpDialog	* m_pGuildLevelUpDlg;
	CGuildNickNameDialog* m_pGuildNickNameDlg;
	CGuildRankDialog	* m_pGuildRankDlg;
	CGuildWarehouseDialog * m_pGuildWarehouseDlg;
	CGuildUnionCreateDialog * m_pGuildUnionCreateDlg;

	CMiniMapDlg			* m_pMiniMapDlg;
	CBigMapDlg			* m_pBigMapDlg;

	CPyogukDialog		* m_pPyogukDialog;
	CNoteDialog			* m_pNoteDialog;
	CFriendDialog		* m_pFriendDialog;
	CMiniNoteDialog		* m_pMiniNoteDialog;
	CMiniFriendDialog	* m_pMiniFriendDialog;


	CMiniMaryDialog     *  m_pMiniMaryDialog;  

	CControlDialog      * m_pControlDialog;    

	CQuipInfoDlg		* m_pCharacterEquipInfo;

	CItemLockDialog     * m_pItemLockDialog;   

	CItemUnLockDialog   * m_pItemUnLockDialog;

	CReviveDialog		* m_pReviveDialog;
	CWantNpcDialog		* m_pWantNpcDialog;
	CWantRegistDialog	* m_pWantRegistDialog;
	CMonsterGuageDlg	* m_pMonsterGuageDlg;
	CUpgradeDlg			* m_pUpgradeDlg;
	CReinforceDlg		* m_pReinforceDlg;
	CDissloveDlg		* m_pDissolveDlg;

	CQuestTotalDialog			* m_pQuestTotalDlg;
	CQuestDialog				* m_pQuestDlg;
	CWantedDialog				* m_pWantedDlg;
	CJournalDialog				* m_pJournalDlg;

	CBailDialog					* m_pBailDlg;

	CMPRegistDialog		* m_pMPRegistDialog;
	CMPMissionDialog	* m_pMPMissionDialog;	//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CMPNoticeDialog		* m_pMPNoticeDialog;	//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	CMPGuageDialog		* m_pMPGuageDialog;		//[标记][BY:十里坡剑神][QQ:112582793][2019-9-2][20:43:29]
	
	CItemShopDialog		* m_pItemShopDialog;
	CMoveDialog			* m_pMoveDialog;
	CPointSaveDialog	* m_pPointSaveDialog;

	CGFWarDeclareDlg*	m_pGFWarDeclareDlg;
	CGFWarResultDlg*	m_pGFWarResultDlg;
	CGFWarInfoDlg*		m_pGFWarInfoDlg;
	CGuildWarInfoDlg*	m_pGuildWarInfoDlg;
	CGuildPlusTimeDlg*  m_pGuildPlusTimeDlg;

	CGuildMunhaDialog*	m_pGuildMunhaDlg;
	CGuildTraineeDialog*	m_pGuildTraineeDlg;

	CSkillPointRedist*	m_pSkPointDlg;
	CSkillPointNotify*	m_pSkPointNotifyDlg;

	CPartyWarDialog*	m_pPartyWarDlg;

	CShoutDialog*		m_pShoutDlg;
	CShoutchatDialog*	m_pShoutchatDlg;

	CChaseDialog*		m_pChaseDlg;
	CChaseinputDialog*	m_pChaseinputDlg;

	CNameChangeDialog*	m_pNameChangeDlg;
	CNameChangeNotifyDlg*	m_pNameChangeNotifyDlg;

	CGTRegistDialog*			m_pGTRegistDlg;
	CGTRegistcancelDialog*		m_pGTRegistcancelDlg;
	CGTStandingDialog*			m_pGTStandingDlg;
	CGTBattleListDialog*		m_pGTBattleListDlg;
	CGTScoreInfoDialog*			m_pGTScoreInfoDlg;

	CSWTimeRegDlg*				m_pSWTimeRegDlg;
	CSWProtectRegDlg*			m_pSWProtectRegDlg;
	CSWInfoDlg*					m_pSWInfoDlg;
	CSWStartTimeDialog*			m_pSWStartTimeDlg;

	CCharChangeDlg*			m_pCharChangeDlg;
	
	CSealDialog*			m_pSealDlg;

	CSWProfitDlg*				m_pSWProfitDlg;
	CSWEngraveDialog*			m_pEngraveDlg;
	CSWTimeDialog*				m_pSWTimeDlg;

	CChangeJobDialog*			m_pChangeJobDlg;

	CReinforceResetDlg*			m_pReinforceResetDlg;
	CRareCreateDialog*			m_pRareCreateDlg;

	CPetStateDlg*				m_pPetStateDlg;
	CPetStateMiniDlg*			m_pPetStateMiniDlg;
	CPetInventoryDlg*			m_pPetInventoryDlg;
	CPetUpgradeDialog*			m_pPetUpgradeDlg;
	CPetRevivalDialog*			m_pPetRevivalDlg;

	CReinforceDataGuideDlg*		m_pReinforceDataGuideDlg;

	CTipBrowserDlg*				m_pTipBrowserDlg;
	CKeySettingTipDlg*			m_pKeySettingTipDlg;

	CGuildNoteDlg*				m_pGuildNoteDlg;
	CUnionNoteDlg*				m_pUnionNoteDlg;

	CGuildNoticeDlg*			m_pGuildNoticeDlg;

	CSkillOptionChangeDlg*		m_pSkillOptionChangeDlg;
	CSkillOptionClearDlg*		m_pSkillOptionClearDlg;
	/////////////////////////////////////////////////////////////
	CGuildJoinDialog* m_pGuildInvitationKindSelectDlg;

	CScreenShotDlg*			m_pScreenShotDlg;

	CSurvivalCountDialog*	m_pSurvivalCountDlg;
	
	CUniqueItemCurseCancellationDlg* m_pUniqueItemCurseCancellationDlg;

	CUniqueItemMixDlg* m_pUniqueItemMixDlg;

	CSOSDlg* m_pSOSDlg;

	CAutoNoteDlg*	m_pAutoNoteDlg;
	CAutoAnswerDlg*	m_pAutoAnswerDlg;

	CFWEngraveDialog*	m_pFWEngraveDlg;
	CFWTimeDialog*		m_pFWTimeDlg;
	CFWWareHouseDialog*	m_pFWWareHouseDlg;

	CUniqueItemMixProgressBarDlg*	m_pUniqueItemMixProgressBarDlg;		
	CTitanMixProgressBarDlg*		m_pTitanMixProgressBarDlg;	
	CTitanPartsProgressBarDlg*		m_pTitanPartsProgressBarDlg;	

	CSkinSelectDialog* m_pSkinSelectDlg;
	CCostumeSkinSelectDialog* m_pCostumeSkinSelectDlg;	

	CSkillPointResetDlg* m_pSkillPointResetDlg;	

	CStreetStallItemViewDlg* m_pStreetStallItemViewDlg;

	CPartyMatchingDlg* m_pPartyMatchingDlg;	

	cBillBoardDialog    * m_pBillboardDialog;		 //[角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
	GuildTopList*				m_GuildTopListDlg;	 //[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
	RegID*						m_RegIDDLG;			//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	RestPwd*					m_RestPwdDLG;		//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	CharacterRBTN*				m_CharacterRBtn;	//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	FBTipDlg*					m_pFBTipDlg;		//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
	OpenPyoguk*					m_pOpenPyogukDlg;	//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	cSingedDialog*				m_SingedDlg;		//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	cRechargeManager*			m_RechargeDlg;		//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	cSetingInfoDlg*				m_SetingInfoDlg;	//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	cStatusIconBtnDlg*			m_StatusIcoBtnDlg;		//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	OnlineFame *				m_OnlineFame;			//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/10]
	cTopRankDlg *				m_TopRankDlg;		//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	cInventoryRBTDlg*			m_InventoryRBTDlg;		//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	cSendItemInfoDlg*			m_SendItemInfoDlg;		//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	cKillTopList*				m_KillTopListDlg;		//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	cSafeLockDialog*			m_SafeLockDlg;			//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	cMainDlgSafeBtn*			m_MainDlgSafeBtn;		//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	cSafeLockMainDlg*			m_SafeLockMainDlg;		//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	//MaintainaceNoteDlg*			m_MaintainaceNoteDlg;	//[维护公告][By:十里坡剑神][QQ:112582793][2018/6/13]
	CVideoCaptureDlg*			m_pVideoCaptureDlg;		//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	CGoldShopMainDailog*		m_pGoldShopMainDlg;		//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	//cShopItemInventPlusDlg*		m_pShopItemInventPlusDlg;//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
	FastConfigPointDlg*			m_pFastCfgPointDlg;		//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:05:02]
	LotSplitItemDlg*			m_pLotSplitItemDlg;		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:07:03]
	VimuWithGoldDlg*			m_pVimuWithGoldDlg;		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:50:02]
	cEquipItemInherit*			m_EquipItemInherit;		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:32:28]
	cUnionWarManager*			m_UnionWarInfoDlg;		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:40:04]
	BOOL	m_bInitForGame;
	int		m_GameInInitKind;	
	DWORD	m_MovePoint;
	DWORD	m_MoveMap;
	DWORD	m_GuildIdx;		
	DWORD	m_BattleIdx;	
	DWORD	m_SiegeWarMapNum;	
public:
	
	void UserConn_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void Recharge_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void KillerTop_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void SafeLock_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void Singed_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void Client_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void Murimnet_NetworkMsgParse(BYTE Protocol, void* pMsg);
	/***************************************************************/
	void NPC_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void UserConn_Object_Remove(MSG_DWORD* pmsg);
	void UserConn_Pet_Die(MSG_DWORD2* pmsg);
	void UserConn_Monster_Die(MSG_DWORD2* pmsg);
	void UserConn_Character_Die(MSG_DWORD2* pmsg);
	void UserConn_Ready_Revive();
	void UserConn_Char_Revive(MOVE_POS* pmsg);
	void UserConn_NPC_Add(SEND_NPC_TOTALINFO* pmsg);
	void UserConn_Gridinit();
	void UserConn_GameIn_Ack(SEND_HERO_TOTALINFO * pmsg);
	void UserConn_Char_Add(SEND_CHARACTER_TOTALINFO* pmsg);
	void UserConn_Pet_Add(SEND_PET_TOTALINFO* pmsg);
	void UserConn_Monster_Add(SEND_MONSTER_TOTALINFO* pmsg);
	void UserConn_Boss_Add(SEND_MONSTER_TOTALINFO* pmsg);
	void UserConn_MapChange_Ack(MSG_DWORD* pmsg);
	void UserConn_MapChange_Cheat_Ack(MSG_DWORD* pmsg);
	void UserConn_FB_In_Ack();
	void UserConn_FB_In_Nack();
	void UserConn_MapChange_Nack();
	void UserConn_CharList_Ack(SEND_CHARSELECT_INFO* pmsg);
	void UserConn_BackCharSel_Nack();
	void UserConn_MapDesc(MSG_WORD2* pmsg);
	void UserConn_SetVisible(MSG_DWORD2* pmsg);
	void UserConn_MapOut_EventMap(MSG_DWORD3* pmsg);
	void UserConn_MapOut_EventBeforeMap();
	void UserConn_EventItem_Use(MSG_DWORD* pmsg);
	void UserConn_FlagNPC_OnOff(MSG_DWORD2* pmsg);
	void UserConn_ChannelInfo_Ack(MSG_CHANNEL_INFO * pmsg);
	void UserConn_ChannelInfo_Nack(MSG_BYTE* pmsg);
	void UserConn_CurrMap_ChannelInfo(MSG_CHANNEL_INFO* pmsg);
	/***************************************************************/
	void Signal_NetworkMsgParse(BYTE Protocol, void* pMsg);
	void Signal_Vimu_Result(MSG_DWORD2* pmsg);
	void Signal_GoldVimu_Result(MSG_DWORD2* pmsg);
	void Signal_Top_Hero_Syn(MSG_TOP_LIST_HERO * pmsg);
	/***************************************************************/
	void NPC_Speech_Ack(MSG_DWORD2* pmsg);
	void NPC_Speech_Nack(MSG_DWORD *pmsg);
	void NPC_CloseBomul_Ack();
	void NPC_Die_Ack(MSG_DWORD *pmsg);
	void NPC_DoJob_Nack();
	///***************************************************************/
	//void UnionWar_NetworkMsgParse(BYTE Protocol, void* pMsg);
public:	

	BOOL m_bGameInAcked;
	CGameIn();
	virtual ~CGameIn();

	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

	BOOL InitForGame();		
	BOOL GetInitForGame()	{	return m_bInitForGame;	}	
	void ReleaseForGame();	

	void Process();
	void BeforeRender();
	void AfterRender();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	
	BOOL IsGameInAcked()		{	return m_bGameInAcked;	}
	int GetGameInInitKind()	{ return m_GameInInitKind; 	}
	BOOL IsDisconnect();
	virtual void OnDisconnect();

	DWORD GetMoveMap()			{	 return m_MoveMap;	}
	void SetGuildIdx( DWORD GuildIdx )		{	m_GuildIdx = GuildIdx;	}
	void SetMoveMap( DWORD MapNum )			{	m_MoveMap = MapNum;		}
	void SetBattleIdx( DWORD BattleIdx )	{	m_BattleIdx = BattleIdx;	}
	void SetSiegeWarMapNum( DWORD MapNum )	{	m_SiegeWarMapNum = MapNum;	}
	
	CMonsterGuageDlg	*	GetMonsterGuageDlg()	{ return m_pMonsterGuageDlg;	}
	void	SetMonsterGuageDlg(CMonsterGuageDlg	* dlg)	{ m_pMonsterGuageDlg = dlg;	}

	CCharacterDialog * GetCharacterDialog(){ return m_pCharDlg; }
	void SetCharacterDialog(CCharacterDialog * dlg){ m_pCharDlg=dlg; }
	CMainBarDialog	* GetMainInterfaceDialog(){ return m_pMainDlg; }
	void SetMainInterfaceDialog(CMainBarDialog	* Dlg){ m_pMainDlg=Dlg; }

	void SetChatDialog(CChatDialog	* Dlg){ m_pChatDlg=Dlg; }
	CChatDialog * GetChatDialog(){ return m_pChatDlg; }

	void SetDebugDlg(CDebugDlg* Dlg){m_pDebugDlg = Dlg;}
	CDebugDlg* GetDebugDlg(){return m_pDebugDlg;}

	CMugongSuryunDialog * GetMugongSuryunDialog(){ return m_pMugongSuryunDlg; }
	void SetMugongSuryunDialog(CMugongSuryunDialog * dlg){ m_pMugongSuryunDlg=dlg; }

	CMugongDialog * GetMugongDialog(){ return m_pMugongDlg; }
	void SetMugongDialog(CMugongDialog * dlg){ m_pMugongDlg=dlg; }
	CSuryunDialog * GetSuryunDialog(){ return m_pSuryunDlg; }
	void SetSuryunDialog(CSuryunDialog * dlg){ m_pSuryunDlg=dlg; }

    CUpgradeDlg * GetUpgradeDialog(){ return m_pUpgradeDlg; }
	void SetUpgradeDialog(CUpgradeDlg * dlg){ m_pUpgradeDlg=dlg; }
	CInventoryExDialog * GetInventoryDialog(){ return m_pInventoryDlg; }
	void SetInventoryDialog(CInventoryExDialog * dlg){ m_pInventoryDlg=dlg; }
	CExchangeDialog * GetExchangeDialog(){ return m_pExchangeDlg; }
	void SetExchangeDialog(CExchangeDialog * dlg){ m_pExchangeDlg=dlg; }

	CMixDialog * GetMixDialog(){ return m_pMixDlg; }
	void SetMixDialog(CMixDialog * dlg){ m_pMixDlg=dlg; }

	CTitanPartsMakeDlg * GetTitanPartsMakeDlg(){ return m_pTitanPartsMakeDlg; }
	void SetTitanPartsMakeDlg(CTitanPartsMakeDlg * dlg){ m_pTitanPartsMakeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanMixDlg * GetTitanMixDlg(){ return m_pTitanMixDlg; }
	void SetTitanMixDlg(CTitanMixDlg * dlg){ m_pTitanMixDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanRepairDlg* GetTitanRepairDlg(){ return m_pTitanRepairDlg; }
	void SetTitanRepairDlg(CTitanRepairDlg * dlg){ m_pTitanRepairDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanRecallDlg* GetTitanRecallDlg(){ return m_pTitanRecallDlg; }
	void SetTitanRecallDlg(CTitanRecallDlg * dlg){ m_pTitanRecallDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanUpgradeDlg* GetTitanUpgradeDlg(){ return m_pTitanUpgradeDlg; }
	void SetTitanUpgradeDlg(CTitanUpgradeDlg * dlg){ m_pTitanUpgradeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanBreakDlg* GetTitanBreakDlg(){ return m_pTitanBreakDlg; }
	void SetTitanBreakDlg(CTitanBreakDlg * dlg){ m_pTitanBreakDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanChangePreViewDlg* GetTitanChangePreViewDlg(){ return m_pTitanChangePreViewDlg; }
	void SetTitanChangePreViewDlg(CTitanChangePreViewDlg * dlg){ m_pTitanChangePreViewDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanInventoryDlg* GetTitanInventoryDlg(){ return m_pTitanInventoryDlg; }
	void SetTitanInventoryDlg(CTitanInventoryDlg * dlg){ m_pTitanInventoryDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanGuageDlg* GetTitanGuageDlg(){ return m_pTitanGuageDlg; }
	void SetTitanGuageDlg(CTitanGuageDlg * dlg){ m_pTitanGuageDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////
	CTitanRegisterDlg* GetTitanRegisterDlg(){ return m_pTitanRegisterDlg; }
	void SetTitanRegisterDlg(CTitanRegisterDlg * dlg){ m_pTitanRegisterDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CTitanDissolutionDlg* GetTitanDissolutionDlg(){ return m_pTitanDissolutionDlg; }
	void SetTitanDissolutionDlg(CTitanDissolutionDlg * dlg){ m_pTitanDissolutionDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CReinforceDlg * GetReinforceDialog(){ return m_pReinforceDlg; }
	void SetReinforceDialog(CReinforceDlg * dlg){ m_pReinforceDlg=dlg; }
	CDissloveDlg * GetDissolveDialog(){ return m_pDissolveDlg; }
	void SetDissolveDialog(CDissloveDlg * dlg){ m_pDissolveDlg=dlg; }

	
	CDealDialog * GetDealDialog(){ return m_pDealDlg; }
	void SetDealDialog(CDealDialog * dlg){ m_pDealDlg=dlg; }

	CStallKindSelectDlg * GetStallKindSelectDialog(){ return m_pStallKindSelectDlg; }
	void SetStallKindSelectDialog(CStallKindSelectDlg * dlg){ m_pStallKindSelectDlg=dlg; }	
	CStreetBuyStall * GetStreetBuyStallDialog(){ return m_pStreetBuyStallDlg; }
	void SetStreetBuyStallDialog(CStreetBuyStall * dlg){ m_pStreetBuyStallDlg=dlg; }
	CBuyRegDialog * GetBuyRegDialog(){ return m_pBuyRegDlg; }
	void SetBuyRegDialog(CBuyRegDialog * dlg){ m_pBuyRegDlg=dlg; }
	
	//////////////////////////////////////////////////////////////////////////
	CStallFindDlg * GetStallFindDlg(){ return m_pStallFindDlg; };
	void SetStallFindDlg(CStallFindDlg * dlg){ m_pStallFindDlg = dlg; };

	CStreetStall * GetStreetStallDialog(){ return m_pStreetStallDlg; }
	void SetStreetStallDialog(CStreetStall * dlg){ m_pStreetStallDlg=dlg; }
	cNpcScriptDialog* GetNpcScriptDialog() { return m_pNpcScriptDlg; }
	void SetNpcScriptDialog(cNpcScriptDialog* dlg) { m_pNpcScriptDlg = dlg; }
	cHelpDialog* GetHelpDialog() { return m_pHelpDlg; }
	void SetHelpDialog(cHelpDialog* dlg) { m_pHelpDlg = dlg; }

	CMoneyDlg* GetMoneyDialog()	{	return m_MoneyDlg;	}
	void SetMoneyDialog(CMoneyDlg* dlg)	{	m_MoneyDlg = dlg;	}
	CQuickDialog* GetQuickDialog()	{	return m_pQuickDlg;	}
	void SetQuickDialog(CQuickDialog* dlg)	{	m_pQuickDlg = dlg;	}

	CMacroDialog * GetMacroDialog(){ return m_pMacroDlg; }
	void SetMacroDialog(CMacroDialog * dlg){ m_pMacroDlg=dlg; }

	CChatOptionDialog * GetChatOptionDialog() { return m_pChatOptionDlg; }
	void SetChatOptionDialog(CChatOptionDialog * dlg) { m_pChatOptionDlg = dlg; }	
	COptionDialog * GetOptionDialog() { return m_pOptionDlg; }
	void SetOptionDialog(COptionDialog * dlg) { m_pOptionDlg = dlg; }
	CExitDialog * GetExitDialog() { return m_pExitDlg; }
	void SetExitDialog(CExitDialog * dlg) { m_pExitDlg = dlg; }
	CCharStateDialog* GetCharStateDialog() { return m_pCharStateDlg; }
	void SetCharStateDialog(CCharStateDialog* dlg) { m_pCharStateDlg = dlg; }

	void SetMenuSlotDialog( CMenuSlotDialog* pDlg ) { m_pMenuSlotDlg = pDlg; }
	CMenuSlotDialog* GetMenuSlotDialog() { return m_pMenuSlotDlg; }

	void SetDissolutionDialog( CDissolutionDialog* pDlg ) { m_pDissolutionDlg = pDlg; }
	CDissolutionDialog* GetDissolutionDialog() { return m_pDissolutionDlg; }

	void SetEventNotifyDialog( CEventNotifyDialog* pDlg ) { m_pEventNotifyDlg = pDlg; }
	CEventNotifyDialog* GetEventNotifyDialog() { return m_pEventNotifyDlg; }

	cAuctionBoard * GetAuctionBoard(){ return m_pAuctionBoard; }
	void SetAuctionBoard(cAuctionBoard * dlg){ m_pAuctionBoard=dlg; }

	CGuageDialog * GetGuageDlg(){ return m_pGuageDlg; }
	void SetGuageDlg(CGuageDialog * dlg){ m_pGuageDlg=dlg; }

	CPartyDialog* GetPartyDialog() { return &m_PartyDlg; }
	
	CPartyCreateDlg* GetPartyCreateDialog() { return m_pPartyCreateDlg;	}
	void SetPartyCreateDialog(CPartyCreateDlg* dlg)	{ m_pPartyCreateDlg = dlg;	}

	CPartyInviteDlg* GetPartyInviteDialog() { return m_pPartyInviteDlg;	}
	void SetPartyInviteDialog(CPartyInviteDlg* dlg)	{ m_pPartyInviteDlg = dlg;	}
	
	CGuildCreateDialog* GetGuildCreateDlg() { return m_pGuildCreateDlg; }
	void SetGuildCreateDlg(CGuildCreateDialog* dlg) { m_pGuildCreateDlg = dlg;	}

	CGuildUnionCreateDialog* GetGuildUnionCreateDlg() { return m_pGuildUnionCreateDlg; }
	void SetGuildUnionCreateDlg(CGuildUnionCreateDialog* dlg) { m_pGuildUnionCreateDlg = dlg;	}	
	
	CGuildDialog* GetGuildDlg() { return m_pGuildDlg; }
	void SetGuildDlg(CGuildDialog* dlg) { m_pGuildDlg = dlg;	}

	CGuildInviteDialog* GetGuildInviteDlg() { return m_pGuildInviteDlg; }
	void SetGuildInviteDlg(CGuildInviteDialog* dlg) { m_pGuildInviteDlg = dlg;	}
	
	CGuildMarkDialog* GetGuildMarkDlg() { return m_pGuildMarkDlg; }
	void SetGuildMarkDlg(CGuildMarkDialog* dlg) { m_pGuildMarkDlg = dlg;	}
	
	CGuildLevelUpDialog* GetGuildLevelUpDlg() { return m_pGuildLevelUpDlg; }
	void SetGuildLevelUpDlg(CGuildLevelUpDialog* dlg) { m_pGuildLevelUpDlg = dlg;	}

	CGuildNickNameDialog* GetGuildNickNameDlg() { return m_pGuildNickNameDlg; }
	void SetGuildNickNameDlg(CGuildNickNameDialog* dlg) { m_pGuildNickNameDlg = dlg;	}

	CGuildRankDialog* GetGuildRankDlg() { return m_pGuildRankDlg; }
	void SetGuildRankDlg(CGuildRankDialog* dlg) { m_pGuildRankDlg = dlg;	}
	
	CGuildWarehouseDialog* GetGuildWarehouseDlg() { return m_pGuildWarehouseDlg; }
	void SetGuildWarehouseDlg(CGuildWarehouseDialog* dlg) { m_pGuildWarehouseDlg = dlg;	}

	CMiniMapDlg* GetMiniMapDialog() { return m_pMiniMapDlg; 	}
	void SetMiniMapDialog(CMiniMapDlg* dlg) { m_pMiniMapDlg = dlg; 	}

	CBigMapDlg* GetBigMapDialog() { return m_pBigMapDlg; 	}
	void SetBigMapDialog(CBigMapDlg* dlg) { m_pBigMapDlg = dlg; 	}

	CPyogukDialog* GetPyogukDialog() { return m_pPyogukDialog;	}
	void SetPyogukDialog(CPyogukDialog* dlg) { m_pPyogukDialog = dlg; 	}
	
	CFriendDialog* GetFriendDialog() { return m_pFriendDialog; }
	void SetFriendDialog(CFriendDialog* dlg) { m_pFriendDialog = dlg; }

	CNoteDialog* GetNoteDialog() { return m_pNoteDialog; }
	void SetNoteDialog(CNoteDialog* dlg) { m_pNoteDialog = dlg;	}
	
	CMiniNoteDialog* GetMiniNoteDialog() { return m_pMiniNoteDialog;	}
	void SetMiniNoteDialog(CMiniNoteDialog* dlg) { m_pMiniNoteDialog = dlg;	}
	
	CMiniFriendDialog * GetMiniFriendDialog() { return m_pMiniFriendDialog; 	}
	void SetMiniFriendDialog(CMiniFriendDialog* dlg) { m_pMiniFriendDialog = dlg;	}

	CMiniMaryDialog * GetMiniMaryDialog() { return m_pMiniMaryDialog; 	}
	void SetMiniMaryDialog(CMiniMaryDialog* dlg) { m_pMiniMaryDialog = dlg;	}

	CControlDialog* GetControlDialog() {return m_pControlDialog;}
	void SetControlDialog(CControlDialog* dlg){m_pControlDialog=dlg;}

	CQuipInfoDlg*	GetCharacterEquipInfoDlg(){return m_pCharacterEquipInfo;}
	void SetShowEquipInfoDlg(CQuipInfoDlg *dlg) { m_pCharacterEquipInfo = dlg;};


	CItemLockDialog * GetItemLockDialog(){return m_pItemLockDialog;}
	void SetItemLockDialog(CItemLockDialog *dlg){m_pItemLockDialog=dlg;}


	CItemUnLockDialog * GetItemUnLockDialog(){return m_pItemUnLockDialog;}
	void SetItemUnLockDialog(CItemUnLockDialog *dlg){m_pItemUnLockDialog=dlg;}

	
	CReviveDialog * GetReviveDialog() { return m_pReviveDialog; 	}
	void SetReviveDialog(CReviveDialog* dlg) { m_pReviveDialog = dlg;	}

	CWantNpcDialog * GetWantNpcDialog() { return m_pWantNpcDialog; 	}
	void SetWantNpcDialog(CWantNpcDialog* dlg) { m_pWantNpcDialog = dlg; }
	

	CWantRegistDialog * GetWantRegistDialog() { return m_pWantRegistDialog; }
	void SetWantRegistDialog(CWantRegistDialog * dlg) { m_pWantRegistDialog = dlg;	}

	CQuestTotalDialog * GetQuestTotalDialog() { return m_pQuestTotalDlg;	}
	void SetQuestTotalDialog(CQuestTotalDialog * dlg) { m_pQuestTotalDlg = dlg;	}
	CQuestDialog* GetQuestDialog()			{ return m_pQuestDlg;	}
	void SetQuestDialog(CQuestDialog* dlg)	{ m_pQuestDlg = dlg;	}
	CWantedDialog* GetWantedDialog() { return m_pWantedDlg; }
	void SetWantedDialog(CWantedDialog* dlg) { m_pWantedDlg = dlg;	}
	CJournalDialog* GetJournalDialog() { return m_pJournalDlg;	}
	void SetJournalDialog(CJournalDialog* dlg) { m_pJournalDlg = dlg; }

	void SetMPRegistDialog(CMPRegistDialog * dlg) { m_pMPRegistDialog = dlg; }
	CMPRegistDialog* GetMPRegistDialog() { return m_pMPRegistDialog;	}
	void SetMPMissionDialog(CMPMissionDialog* dlg) { m_pMPMissionDialog = dlg; }
	CMPMissionDialog* GetMPMissionDialog() { return m_pMPMissionDialog;	}
	void SetMPNoticeDialog(CMPNoticeDialog* dlg) { m_pMPNoticeDialog = dlg; }
	CMPNoticeDialog* GetMPNoticeDialog() { return m_pMPNoticeDialog;	}
	void SetMPGuageDialog(CMPGuageDialog* dlg) { m_pMPGuageDialog = dlg; }
	CMPGuageDialog* GetMPGuageDialog() { return m_pMPGuageDialog;	}

	void SetPetStateDialog(CPetStateDlg* dlg) { m_pPetStateDlg = dlg; }
	CPetStateDlg* GetPetStateDialog() { return m_pPetStateDlg; }
	void SetPetStateMiniDialog(CPetStateMiniDlg* dlg) { m_pPetStateMiniDlg = dlg; }
	CPetStateMiniDlg* GetPetStateMiniDialog() { return m_pPetStateMiniDlg; }
	void SetPetInventoryDialog(CPetInventoryDlg* dlg) { m_pPetInventoryDlg = dlg; }
	CPetInventoryDlg* GetPetInventoryDialog() { return m_pPetInventoryDlg; }
	void SetPetUpgradeDialog(CPetUpgradeDialog* dlg) {	m_pPetUpgradeDlg = dlg;	}
	CPetUpgradeDialog* GetPetUpgradeDialog() {	return m_pPetUpgradeDlg;	}
	void SetPetRevivalDialog(CPetRevivalDialog* dlg) {	m_pPetRevivalDlg = dlg;	}
	CPetRevivalDialog* GetPetRevivalDialog() {	return m_pPetRevivalDlg;	}

	CBailDialog* GetBailDialog()			{	return m_pBailDlg;	}
	void SetBailDialog(CBailDialog* dlg)	{	m_pBailDlg = dlg; }

	CItemShopDialog* GetItemShopDialog()			{	return m_pItemShopDialog;	}
	void SetItemShopDialog(CItemShopDialog* dlg)	{	m_pItemShopDialog = dlg; 	}
	
	CMoveDialog* GetMoveDialog()					{	return m_pMoveDialog;	}
	void SetMoveDialog(CMoveDialog* dlg)			{	m_pMoveDialog = dlg;	}
	CPointSaveDialog* GetPointSaveDialog()			{	return m_pPointSaveDialog;	}
	void SetPointSaveDialog(CPointSaveDialog* dlg)	{	m_pPointSaveDialog = dlg;	}

	CGFWarDeclareDlg* GetGFWarDeclareDlg()			{	return m_pGFWarDeclareDlg;	}
	void SetGFWarDeclareDlg(CGFWarDeclareDlg* dlg)	{	m_pGFWarDeclareDlg = dlg;	}
	CGFWarResultDlg* GetGFWarResultDlg()			{	return m_pGFWarResultDlg;	}
	void SetGFWarResultDlg(CGFWarResultDlg* dlg)	{	m_pGFWarResultDlg = dlg;	}
	CGFWarInfoDlg* GetGFWarInfoDlg()				{	return m_pGFWarInfoDlg;	}
	void SetGFWarInfoDlg(CGFWarInfoDlg* dlg)		{	m_pGFWarInfoDlg = dlg;	}
	CGuildWarInfoDlg* GetGuildWarInfoDlg()			{	return m_pGuildWarInfoDlg;	}
	void SetGuildWarInfoDlg(CGuildWarInfoDlg* dlg)	{	m_pGuildWarInfoDlg = dlg;	}
	CGuildPlusTimeDlg* GetGuildPlusTimeDlg()			{	return m_pGuildPlusTimeDlg;	}
	void SetGuildPlusTimeDlg(CGuildPlusTimeDlg* dlg)	{	m_pGuildPlusTimeDlg = dlg;	}
	
	CSkillPointRedist*	GetSkPointDlg()					{	return m_pSkPointDlg;	}
	void SetSkPointDlg(CSkillPointRedist* dlg)			{	m_pSkPointDlg = dlg;	}
	CSkillPointNotify*	GetSkPointNotifyDlg()			{	return m_pSkPointNotifyDlg;	}
	void SetSkPointNotifydlg(CSkillPointNotify* dlg)	{	m_pSkPointNotifyDlg = dlg;	}

	CPartyWarDialog*	GetPartyWarDlg()				{	return m_pPartyWarDlg;	}
	void SetPartyWarDlg(CPartyWarDialog* dlg)			{	m_pPartyWarDlg = dlg;	}

	CShoutDialog*	GetShoutDlg()						{	return m_pShoutDlg;		}
	void SetShoutDlg(CShoutDialog* dlg)					{	m_pShoutDlg = dlg;		}
	CShoutchatDialog*	GetShoutchatDlg()				{	return m_pShoutchatDlg;		}
	void SetShoutchatDlg(CShoutchatDialog* dlg)			{	m_pShoutchatDlg = dlg;		}

	CChaseDialog*	GetChaseDlg()						{	return m_pChaseDlg;		}
	void SetChaseDlg(CChaseDialog* dlg)					{	m_pChaseDlg = dlg;		}
	CChaseinputDialog*	GetChaseinputDlg()					{	return m_pChaseinputDlg;	}
	void SetChaseinputDlg(CChaseinputDialog* dlg)			{	m_pChaseinputDlg = dlg;		}

	CNameChangeDialog*	GetNameChangeDlg()				{	return m_pNameChangeDlg;	}
	void SetNameChangeDlg(CNameChangeDialog* dlg)		{	m_pNameChangeDlg = dlg;		}
	CNameChangeNotifyDlg*	GetNameChangeNotifyDlg()		{	return m_pNameChangeNotifyDlg;	}
	void SetNameChangeNotifyDlg(CNameChangeNotifyDlg* dlg)	{	m_pNameChangeNotifyDlg = dlg;		}

	CRareCreateDialog*	GetRareCreateDlg()				{	return m_pRareCreateDlg;	}
	void SetRareCreateDialog( CRareCreateDialog* dlg )	{	m_pRareCreateDlg = dlg;		}

	CScreenShotDlg*	GetScreenShotDlg()				{	return m_pScreenShotDlg;	}
	void SetScreenShotDlg( CScreenShotDlg* dlg )	{	m_pScreenShotDlg = dlg;		}

	CSurvivalCountDialog* GetSurvivalCountDlg()		{	return m_pSurvivalCountDlg;	}
	void SetSurvivalCountDialog(CSurvivalCountDialog* dlg)	{ m_pSurvivalCountDlg = dlg;	}

	CGTRegistDialog*	GetGTRegistDlg()					{	return m_pGTRegistDlg;			}
	void SetGTRegistDlg(CGTRegistDialog* dlg)				{	m_pGTRegistDlg = dlg;			}
	CGTRegistcancelDialog*	GetGTRegistcancelDlg()			{	return m_pGTRegistcancelDlg;	}
	void SetGTRegistcancelDlg(CGTRegistcancelDialog* dlg)	{	m_pGTRegistcancelDlg = dlg;		}
	CGTStandingDialog*	GetGTStandingDlg()					{	return m_pGTStandingDlg;		}
	void SetGTStandingDlg(CGTStandingDialog* dlg)			{	m_pGTStandingDlg = dlg;			}
	CGTBattleListDialog*	GetGTBattleListDlg()			{	return m_pGTBattleListDlg;		}
	void SetGTBattleListDlg(CGTBattleListDialog* dlg)		{	m_pGTBattleListDlg = dlg;		}
	CGTScoreInfoDialog*	GetGTScoreInfoDlg()					{	return m_pGTScoreInfoDlg;		}
	void SetGTScoreInfoDlg(CGTScoreInfoDialog* dlg)			{	m_pGTScoreInfoDlg = dlg;		}

	CSWTimeRegDlg*		GetSWTimeRegDlg()					{ return m_pSWTimeRegDlg; }
	void				SetSWTimeRegDlg( CSWTimeRegDlg* dlg )	{ m_pSWTimeRegDlg = dlg; }
	CSWProtectRegDlg*	GetSWProtectRegDlg()				{ return m_pSWProtectRegDlg; }
	void				SetSWProtectRegDlg( CSWProtectRegDlg* dlg )	{ m_pSWProtectRegDlg = dlg; }
	CSWInfoDlg*			GetSWInfoDlg()						{ return m_pSWInfoDlg; }
	void				SetSWInfoDlg( CSWInfoDlg* dlg )		{ m_pSWInfoDlg = dlg; }
	CSWProfitDlg*		GetSWProfitDlg()					{ return m_pSWProfitDlg; }
	void				SetSWProfitDlg( CSWProfitDlg* dlg )	{ m_pSWProfitDlg = dlg; }
	CSWEngraveDialog*	GetSWEngraveDlg()					{ return m_pEngraveDlg;	}
	void				SetSWEngraveDlg( CSWEngraveDialog* dlg)	{	m_pEngraveDlg = dlg;	}
	CSWTimeDialog*		GetSWTimeDlg()						{ return m_pSWTimeDlg;	}
	void				SetSWTimeDlg( CSWTimeDialog* dlg)	{ m_pSWTimeDlg = dlg;	}
	CSWStartTimeDialog*	GetSWStartTimeDlg()					{ return m_pSWStartTimeDlg;	}
	void				SetSWStartTimeDlg( CSWStartTimeDialog* dlg )	{ m_pSWStartTimeDlg = dlg; }
	
	CCharChangeDlg* GetCharChangeDlg()						{	return m_pCharChangeDlg;		}
	void SetCharChangeDlg( CCharChangeDlg* dlg )			{	m_pCharChangeDlg = dlg;			}

	CSealDialog* GetSealDlg()								{	return m_pSealDlg;			}
	void SetSealDlg( CSealDialog* dlg )						{	m_pSealDlg = dlg;			}

	CChangeJobDialog*	GetChangeJobDlg()					{	return m_pChangeJobDlg;		}
	void				SetChangeJobDlg( CChangeJobDialog* dlg )		{	m_pChangeJobDlg = dlg;		}

	CReinforceResetDlg*	GetReinforceResetDlg()				{	return m_pReinforceResetDlg;	}
	void				SetReinforceResetDlg( CReinforceResetDlg* dlg )		{	m_pReinforceResetDlg = dlg;		}

	CReinforceDataGuideDlg* GetReinforceDataGuideDlg()	{ return m_pReinforceDataGuideDlg; }
	void SetReinforceDataGuideDlg(CReinforceDataGuideDlg* dlg) { m_pReinforceDataGuideDlg = dlg; }

	CTipBrowserDlg* GetTipBrowserDlg() { return m_pTipBrowserDlg; }
	void SetTipBrowserDlg(CTipBrowserDlg* dlg) { m_pTipBrowserDlg = dlg; }

	CKeySettingTipDlg* GetKeySettingTipDlg() { return m_pKeySettingTipDlg; }
	void SetKeySettingTipDlg(CKeySettingTipDlg* dlg) { m_pKeySettingTipDlg = dlg; }

	CGuildNoteDlg*	GetGuildNoteDlg() { return	m_pGuildNoteDlg; }
	void SetGuildNoteDlg(CGuildNoteDlg* dlg) { m_pGuildNoteDlg = dlg; }
	CUnionNoteDlg*	GetUnionNoteDlg() { return	m_pUnionNoteDlg; }
	void SetUnionNoteDlg(CUnionNoteDlg* dlg) { m_pUnionNoteDlg = dlg; }

	CGuildNoticeDlg* GetGuildNoticeDlg() { return m_pGuildNoticeDlg; }
	void SetGuildNoticeDlg(CGuildNoticeDlg *dlg) { m_pGuildNoticeDlg = dlg; }

	CSkillOptionChangeDlg* GetSkillOptionChangeDlg() { return m_pSkillOptionChangeDlg; }
	void SetSkillOptionChangeDlg(CSkillOptionChangeDlg *dlg) { m_pSkillOptionChangeDlg = dlg; }
	
	CSkillOptionClearDlg* GetSkillOptionClearDlg() { return m_pSkillOptionClearDlg; }
	void SetSkillOptionClearDlg(CSkillOptionClearDlg *dlg) { m_pSkillOptionClearDlg = dlg; }
	////////////////////////////////////////////////////////////////////////
	CGuildJoinDialog* GetGuildJoinDialog() {	return m_pGuildInvitationKindSelectDlg;	}
	void SetGuildJoinDialog(CGuildJoinDialog* dlg)	{	m_pGuildInvitationKindSelectDlg = dlg;	}
	CGuildMunhaDialog* GetGuildMunhaDialog()				{	return m_pGuildMunhaDlg;	}
	void SetGuildMunhaDialog(CGuildMunhaDialog* dlg)		{	m_pGuildMunhaDlg = dlg;		}
	CGuildTraineeDialog* GetGuildTraineeDialog()			{	return m_pGuildTraineeDlg;	}
	void SetGuildTraineeDialgo(CGuildTraineeDialog* dlg)	{	m_pGuildTraineeDlg = dlg;	}

	CUniqueItemCurseCancellationDlg* GetUniqueItemCurseCancellationDlg() {	return m_pUniqueItemCurseCancellationDlg;	}
	void SetUniqueItemCurseCancellationDlg(CUniqueItemCurseCancellationDlg* dlg) {	m_pUniqueItemCurseCancellationDlg = dlg;	}

	CUniqueItemMixDlg* GetUniqueItemMixDlg() {	return m_pUniqueItemMixDlg;	}
	void SetUniqueItemMixDlg(CUniqueItemMixDlg* dlg) {	m_pUniqueItemMixDlg = dlg;	}

	CSOSDlg* GetSOSDlg()	{	return m_pSOSDlg;	}
	void SetSOSDlg(CSOSDlg* dlg)	{	m_pSOSDlg = dlg;	}	

	CUniqueItemMixProgressBarDlg* GetUniqueItemMixProgressBarDlg(){ return m_pUniqueItemMixProgressBarDlg; }
	void SetUniqueItemMixProgressBarDlg(CUniqueItemMixProgressBarDlg * dlg){ m_pUniqueItemMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CTitanMixProgressBarDlg* GetTitanMixProgressBarDlg(){ return m_pTitanMixProgressBarDlg; }
	void SetTitanMixProgressBarDlg(CTitanMixProgressBarDlg * dlg){ m_pTitanMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CTitanPartsProgressBarDlg* GetTitanPartsProgressBarDlg(){ return m_pTitanPartsProgressBarDlg; }
	void SetTitanPartsProgressBarDlg(CTitanPartsProgressBarDlg * dlg){ m_pTitanPartsProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CSkinSelectDialog* GetSkinSelectDlg(){ return m_pSkinSelectDlg; }
	void SetSkinSelectDlg(CSkinSelectDialog * dlg){ m_pSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CCostumeSkinSelectDialog* GetCostumeSkinSelectDlg(){ return m_pCostumeSkinSelectDlg; }
	void SetCostumeSkinSelectDlg(CCostumeSkinSelectDialog * dlg){ m_pCostumeSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CSkillPointResetDlg* GetSkillPointResetDlg()	{	return m_pSkillPointResetDlg;	}
	void SetSkillPointResetDlg(CSkillPointResetDlg* dlg)	{	m_pSkillPointResetDlg = dlg;	}

	CStreetStallItemViewDlg* GetStreetStallItemViewDlg()	{	return m_pStreetStallItemViewDlg;	}
	void SetStreetStallItemViewDlg(CStreetStallItemViewDlg* dlg)	{	m_pStreetStallItemViewDlg = dlg;	}

	CAutoNoteDlg* GetAutoNoteDlg()				{ return m_pAutoNoteDlg; }
	void SetAutoNoteDlg( CAutoNoteDlg* dlg )	{ m_pAutoNoteDlg = dlg; }
	CAutoAnswerDlg* GetAutoAnswerDlg()				{ return m_pAutoAnswerDlg; }
	void SetAutoAnswerDlg( CAutoAnswerDlg* dlg )	{ m_pAutoAnswerDlg = dlg; }

	CFWEngraveDialog* GetFWEngraveDlg()				{ return m_pFWEngraveDlg; }
	void SetFWEngraveDlg( CFWEngraveDialog* dlg )	{ m_pFWEngraveDlg = dlg; }
	CFWTimeDialog* GetFWTimeDlg()					{ return m_pFWTimeDlg; }
	void SetFWTimeDlg( CFWTimeDialog* dlg )			{ m_pFWTimeDlg = dlg; }
	CFWWareHouseDialog* GetFWWareHouseDlg()				{ return m_pFWWareHouseDlg; }
	void SetFWWareHouseDlg( CFWWareHouseDialog* dlg )	{ m_pFWWareHouseDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	
	CPartyMatchingDlg* GetPartyMatchingDlg(){ return m_pPartyMatchingDlg; }
	void SetPartyMatchingDlg(CPartyMatchingDlg * dlg){ m_pPartyMatchingDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	//[角色排行][By:十里坡剑神][QQ:112582793][2017/11/26]
	cBillBoardDialog* GetBillboardDialog(){return m_pBillboardDialog;}
	void SetBillboardDialog(cBillBoardDialog* dlg){m_pBillboardDialog=dlg;}
	//[帮会排行][By:十里坡剑神][QQ:112582793][2017/11/24]
	GuildTopList* GetGuildTopListDlg(){return m_GuildTopListDlg;}
	void SetGuildTopListDlg(GuildTopList* dlg){m_GuildTopListDlg=dlg;}
	//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
	RegID* GetRegID(){return m_RegIDDLG;}	
	void SetRegID(RegID* dlg){m_RegIDDLG=dlg;}
	//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	RestPwd* GetRestPwdDlg(){return m_RestPwdDLG;}
	void SetRestPwdDlg(RestPwd* dlg){m_RestPwdDLG=dlg;}
	//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
	CharacterRBTN*	GetCharacterRBTNDlg(){return m_CharacterRBtn;}
	void SetCharacterRBTN (CharacterRBTN* dlg){m_CharacterRBtn = dlg;}	
	//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
	FBTipDlg* GetFBTipDlg()	{return m_pFBTipDlg;}
	void SetFBTipDlg(FBTipDlg* dlg) { m_pFBTipDlg = dlg; } 
	//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
	OpenPyoguk* GetOpenPyogukDlg(){return m_pOpenPyogukDlg;}
	void SetOpenPyogukDlg(OpenPyoguk* dlg){m_pOpenPyogukDlg=dlg;}
	//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
	cSingedDialog * GetSingedDlg(){return m_SingedDlg;}
	void SetSingedDlg(cSingedDialog* dlg){m_SingedDlg = dlg;}
	//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
	cRechargeManager* GetRechargeDlg(){return m_RechargeDlg;}
	void SetRechargeDlg(cRechargeManager* dlg){m_RechargeDlg=dlg;}
	//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	cSetingInfoDlg * GetSetingInfoDlg(){return m_SetingInfoDlg;}
	void SetSetingInfoDlg(cSetingInfoDlg* dlg){m_SetingInfoDlg = dlg;}
	//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
	cStatusIconBtnDlg* GetStatusIconBtnDlg(){return m_StatusIcoBtnDlg;}
	void SetStatusIconBtnDlg(cStatusIconBtnDlg* dlg){m_StatusIcoBtnDlg = dlg;}
	//[功能按钮界面][By:十里坡剑神][QQ:112582793][2018/3/10]
	OnlineFame * GetOnlineFameDlg(){return m_OnlineFame;}
	void SetOnlineFameDlg(OnlineFame* dlg){m_OnlineFame = dlg;}
	//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
	cTopRankDlg * GetTopRankDlg(){return m_TopRankDlg;}
	void SetTopRankDlg(cTopRankDlg * dlg){m_TopRankDlg = dlg;}
	//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
	cInventoryRBTDlg* GetInventoryRBTDlg(){return m_InventoryRBTDlg;}
	void SetInventoryRBTDlg(cInventoryRBTDlg* dlg){m_InventoryRBTDlg = dlg;}
	//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
	cSendItemInfoDlg* GetSendItemInfoDlg(){return m_SendItemInfoDlg;}
	void SetSendItemInfoDlg(cSendItemInfoDlg* dlg){m_SendItemInfoDlg = dlg;}
	//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
	cKillTopList* GetKillTopListDlg(){return m_KillTopListDlg;}
	void SetKillTopListDlg(cKillTopList* dlg){m_KillTopListDlg = dlg;}
	//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
	cSafeLockDialog* GetSafeLockDlg(){return m_SafeLockDlg;}
	void SetSafeLockDlg(cSafeLockDialog* dlg){m_SafeLockDlg = dlg;}
	//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
	cMainDlgSafeBtn* GetMainDlgSafeBtn(){return m_MainDlgSafeBtn;}
	void SetMianDlgSafeBtn(cMainDlgSafeBtn* dlg){m_MainDlgSafeBtn = dlg;}
	//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
	cSafeLockMainDlg* GetSafeLockMainDlg(){return m_SafeLockMainDlg;}
	void SetSafeLockMainDlg(cSafeLockMainDlg* dlg){m_SafeLockMainDlg = dlg;}
	//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
	void SetVideoCaptureDlg( CVideoCaptureDlg* pDlg )	{ m_pVideoCaptureDlg = pDlg; }
	CVideoCaptureDlg* GetVideoCaptureDlg()				{ return m_pVideoCaptureDlg; }
	//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
	void SetGoldShopMainDlg(CGoldShopMainDailog* pDlg){m_pGoldShopMainDlg = pDlg;}
	CGoldShopMainDailog* GetGoldShopMainDlg(){return m_pGoldShopMainDlg;}
	//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
	//void SetShopItemInventPlusDlg(cShopItemInventPlusDlg* pDlg){m_pShopItemInventPlusDlg = pDlg;}
//	cShopItemInventPlusDlg* GetShopItemInventPlusDlg(){return m_pShopItemInventPlusDlg;}

	//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:06:00]
	void SetFastCfgPointDlg(FastConfigPointDlg* dlg){m_pFastCfgPointDlg = dlg;}
	FastConfigPointDlg* GetFastConfigPointDlg(){return m_pFastCfgPointDlg;}
	//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:07:13]
	void SetLotSplitItemDlg(LotSplitItemDlg* dlg){m_pLotSplitItemDlg = dlg;}
	LotSplitItemDlg* GetLotSplitItemDlg(){return m_pLotSplitItemDlg;}
	//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:50:11]
	void SetVimuWithGoldDlg(VimuWithGoldDlg* dlg){m_pVimuWithGoldDlg = dlg;}
	VimuWithGoldDlg* GetVimuWithGoldDlg(){return m_pVimuWithGoldDlg;}
	//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:32:40]
	void SetEquipItemInherit(cEquipItemInherit* dlg){ m_EquipItemInherit = dlg; }
	cEquipItemInherit* GetEquipItemInherit(){ return m_EquipItemInherit; }
	//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:40:19]
	void SetUnionWarInfoDlg(cUnionWarManager* dlg){ m_UnionWarInfoDlg = dlg; }
	cUnionWarManager* GetUnionWarInfoDlg(){ return m_UnionWarInfoDlg; }
	SYSTEMTIME	GameinServerTime;		
	DWORD				m_Clientdate;		
	DWORD				m_Clienttime;		

	void	SetLoginTime(SYSTEMTIME Time);
	SYSTEMTIME	GetLoginTime();

	void	GetClientLoginTime(DWORD& date, DWORD& time);
	void	SetClientLoginTime(DWORD date, DWORD time);
	void UpdataGoldMoney();
	BOOL CopyToClipboard(const char* pszData, const int nDataLen);

	std::string GetInternetIP();
	void  InitInternetIP(){ m_CurClientIP.clear(); m_CurClientMac.clear(); }
	BOOL CheckIP(const char * szIP);
	//[获取硬件ID][BY:十里坡剑神][QQ:112582793][2019-5-21][23:13:20]
	void GetHWID(char* HWID);
	void	ProcessInBossMap();
	//////////////////////////////////////////////////////////////////////////
	BOOL	m_bDieInBossMap;
	int		m_nCountInBossMap;
	DWORD	m_dwTimeInBossMap;
	DWORD					m_UpGoldMoneyTime;
	unsigned				m_uiAccTick;
	int						m_iMilliSecend;
	unsigned				m_uiCurrentTime;
	unsigned				m_uiPreTime;
	float					m_fFramePerSec;
	int						m_iFPSCnt;
	std::string				m_CurClientIP;
	std::string				m_CurClientMac;
};


EXTERNGLOBALTON(CGameIn)


#endif // !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)


