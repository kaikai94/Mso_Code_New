#ifndef _GLOBALEVENTFUNC_H_
#define _GLOBALEVENTFUNC_H_
struct FUNC
{
	void (*_func)(LONG lId, void * p, DWORD we);
	const char * _funcName;
};

struct FUNCEX
{//[窗口过程函数扩展][By:十里坡剑神][QQ:112582793][2019/3/20][18:38:57]
	void (*_func)(LONG lId, void * p, DWORD we,long Value);
	const char * _funcName;
};
extern FUNCEX g_mt_funcEx[];//[窗口过程函数扩展][By:十里坡剑神][QQ:112582793][2019/3/20][18:38:57]
extern int FUNCSEARCHEX(char * funcName);//[窗口过程函数扩展][By:十里坡剑神][QQ:112582793][2019/3/20][18:38:57]

extern FUNC g_mt_func[];
extern int FUNCSEARCH(char * funcName);
extern void	MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we);	 
extern void	MT_ExitBtnFunc(LONG lId, void* p, DWORD we);
extern void	MT_EditReturnFunc(LONG lId, void* p, DWORD we);
extern void	MT_DynamicBtnFunc(LONG lId, void* p, DWORD we);
extern void	MT_DynamicEditReturnFunc(LONG lId, void* p, DWORD we);
extern void	CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we);	 
extern void	CS_BtnFuncEnter(LONG lId, void* p, DWORD we);
extern void	CS_FUNC_OkISee(LONG lId, void* p, DWORD we);
extern void	CS_BtnFuncLogOut(LONG lId, void* p, DWORD we);
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we); 
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_CharBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_MugongBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we);
extern void MI_DlgFunc(LONG lId, void * p, DWORD we);
extern void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we);
extern void IN_DlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we);
extern void CI_DlgFunc(LONG lId, void * p, DWORD we);
extern void CI_AddExpPoint(LONG lId, void * p, DWORD we);
extern void PA_DlgFunc(LONG lId, void * p, DWORD we);
extern void QI_QuickDlgFunc(LONG lId, void * p, DWORD we);
extern void SO_DlgFunc(LONG lId, void * p, DWORD we);
extern void BRS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BRS_CloseFunc(LONG lId, void * p, DWORD we);
extern void BRS_DeleteFunc(LONG lId, void * p, DWORD we);
extern void BRS_TypeListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ItemListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ClassListFunc(LONG lId, void * p, DWORD we);
extern void BRS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we);
extern void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we);
extern void BS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_SellBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_SPFunc(LONG lId, void * p, DWORD we);
extern void SSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_EditBtnFunc(LONG lId, void * p, DWORD we);
extern void NSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we);
extern void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we);
extern void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we);
extern void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we);
extern void CMI_AlertFunc(LONG lId, void * p, DWORD we);
extern void DE_DlgFunc(LONG lId, void * p, DWORD we);
extern void DE_BuyItemFunc(LONG lId, void * p, DWORD we);
extern void DE_SellItemFunc(LONG lId, void * p, DWORD we);
extern void DE_CloseHideBtn(LONG lId, void * p, DWORD we); 
extern void PA_BtnFunc(LONG lId, void * p, DWORD we);
extern void GD_DlgFunc( LONG lId, void* p, DWORD we );
extern void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Note_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Marry_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void CR_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we );
extern void WANTREG_DlgFunc( LONG lId, void* p, DWORD we );
extern void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we );
extern void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we );
extern void MP_RegistDlgFunc( LONG lId, void* p, DWORD we );
extern void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we );
extern void MAC_DlgFunc(LONG lId, void * p, DWORD we);
extern void CTI_DlgFunc(LONG lId, void * p, DWORD we);
extern void COI_DlgFunc(LONG lId, void * p, DWORD we);
extern void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we);
extern void OTI_DlgFunc(LONG lId, void * p, DWORD we);
extern void EXT_DlgFunc(LONG lId, void * p, DWORD we);
extern void XCI_DlgFunc(LONG lId, void * p, DWORD we);
extern void CSS_DlgFunc(LONG lId, void * p, DWORD we);
extern void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we );
extern void PLI_DlgFunc( LONG lId, void* p, DWORD we );
extern void CNA_BtnOkFunc( LONG lId, void* p, DWORD we );
extern void DIS_DlgFunc( LONG lId, void* p, DWORD we );
extern void ITEM_GOLDDlgFunc( LONG lId, void* p, DWORD we );
extern void MNPRI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNPRI_ChatFunc( LONG lId, void * p, DWORD we );
extern void MNCNL_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNCNL_ChatFunc(LONG lId, void * p, DWORD we);
extern void MessageBox_Func(LONG lId, void * p, DWORD we);
extern void DIVIDEBOX_Func(LONG lId, void * p, DWORD we);
extern void MNM_DlgFunc(LONG lId, void * p, DWORD we);
extern void ITD_DlgFunc(LONG lId, void * p, DWORD we);
extern void ITMD_DlgFunc(LONG lId, void * p, DWORD we);
extern void TPMD_DlgFunc(LONG lId, void * p, DWORD we);	
extern void TitanMix_DlgFunc(LONG lId, void * p, DWORD we);	
extern void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we);	
extern void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we); 
extern void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we);	
extern void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we);	
extern void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we);	
extern void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we);		
extern void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we);		
extern void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we);		
extern void ITR_DlgFunc(LONG lId, void * p, DWORD we);
extern void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void SA_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we);
extern void SL_DlgBtnFunc( LONG lId, void* p, DWORD we );
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we );
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we );
extern void PW_DlgFunc( LONG lId, void* p, DWORD we );
extern void CHS_DlgFunc( LONG lId, void* p, DWORD we );
extern void CHAN_DlgFunc(LONG lId, void * p, DWORD we);
extern void GDT_DlgFunc(LONG lId, void * p, DWORD we);
extern void SW_DlgFunc(LONG lId, void* p, DWORD we);
extern void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we);
extern void IT_DlgFunc(LONG lId, void* p, DWORD we);
extern void CJOB_DlgFunc(LONG lId, void* p, DWORD we);
extern void CG_DlgFunc(LONG lId, void *p, DWORD we);
extern void RFDefault_DlgFunc(LONG lId, void *p, DWORD we);
extern void RareCreate_DlgFunc(LONG lId, void *p, DWORD we);
extern void PET_DlgFunc(LONG lId, void *p, DWORD we);
extern void PET_MiniDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_InvenDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we);
extern void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we);
extern void TB_DlgFunc(LONG lId, void *p, DWORD we);
extern void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we);
extern void GN_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void AN_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we);
extern void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we);
extern void TDefault_DlgFunc(LONG lId, void *p, DWORD we);
extern void JO_DlgFunc(LONG lId, void* p, DWORD we);
extern void GU_DlgFunc(LONG lId, void* p, DWORD we);	
extern void GT_DlgFunc(LONG lId, void* p, DWORD we);	
extern void UniqueItemCurseCancellationDlgFunc(LONG lId, void* p, DWORD we);	
extern void UniqueItemMixDlgFunc(LONG lId, void* p, DWORD we);	
extern void SOSDlgFunc(LONG lId, void* p, DWORD we);	
extern void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
extern void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
extern void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
extern void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
extern void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
extern void SK_Reset_DlgFunc(LONG lId, void * p, DWORD we);
extern void SFR_DlgFunc(LONG lId, void * p, DWORD we);
extern void SVD_DlgFunc(LONG lId, void * p, DWORD we);
extern void AutoNoteDlg_Func(LONG lId, void * p, DWORD we);
extern void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we);
extern void PA_CreateBtnFunc(LONG lId, void * p, DWORD we);
extern void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we);
extern void FW_DlgFunc(LONG lId, void* p, DWORD we);
extern void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we );
extern void BARCONTROL_DIALOGFunc(LONG lId, void * p, DWORD we);
extern void IT_LOCKDlgFunc(LONG lId,void * p,DWORD we);
extern void IT_UNLOCKDlgFunc(LONG lId,void * p,DWORD we);
extern void CG_TargetObjectBtnFunc( LONG lId, void* p, DWORD we );
//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
extern void CI_RegIDDlgFunc(LONG lId,void *p,DWORD we); 
//[记录帐号密码][By:十里坡剑神][QQ:112582793][2017/12/6]
extern void CI_KeepIDFunc(LONG lId,void *p,DWORD we);
//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
extern void CI_OpenPyogukFunc(LONG lId, void * p, DWORD we);
//[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
extern void CI_SingedDlgFunc(LONG lId, void * p, DWORD we);
//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
extern void CI_RechargeDlgFunc(LONG lId, void * p, DWORD we);
//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
extern void CI_InventoryRBTDlgFunc(LONG lId, void * p, DWORD we);
//[自动加点][By:十里坡剑神][QQ:112582793][2018/4/17]
extern void TB_CharacterDialogFunc(LONG lId,void *p,DWORD we);
//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
extern void TB_SafeLockDialogFunc(LONG lId,void *p,DWORD we);
//[界面信息隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
extern void TB_Dlg_Hide_BtnFunc(LONG lId,void *p,DWORD we);
//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
extern void TB_Main_Dlg_BtnFunc(LONG lId,void *p,DWORD we);
//[大地图自动寻路][By:慌张的油条][QQ:112582793][2018/9/17]
extern void BigMapDlgFunc(LONG lId, void * p, DWORD we);
//[录像功能界面][By:慌张的油条][QQ:112582793][2018/9/17]
extern void VideoCaptureDlgFunc(LONG lId, void* p, DWORD we );
#ifdef _XDYY_
//[小队语音界面消息响应][By:十里坡剑神][QQ:112582793][2019/3/20][15:29:03]
extern void OnlineVoiceDlgFunc(LONG lId, void* p, DWORD we );
//[小队语音音量调节重载函数][By:十里坡剑神][QQ:112582793][2019/3/20][18:31:24]
extern void OnlineVoiceGuageFunc(LONG lId, void* p, DWORD we,long Value);
//[小队语音成员按钮][By:十里坡剑神][QQ:112582793][2019/3/20][22:16:48]
extern void OnlineVoiceMemberFunc(LONG lId, void* p, DWORD we );
#endif  //_XDYY_
//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][17:00:58]
extern void FastConfiPointDlgFunc(LONG lId, void* p, DWORD we );
//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:11:24]
extern void LotSplitItemDlgFunc(LONG lId, void* p, DWORD we );
//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:47:33]
extern void VimuWithGoldDlgFunc(LONG lId, void* p, DWORD we );
//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:35:03]
extern void EquipItemInherit(LONG lId, void* p, DWORD we);
//[排行榜刷新][BY:十里坡剑神][QQ:112582793][2019-9-5][0:57:02]
extern void TopListUpdate(LONG lId, void* p, DWORD we);
//[副本卷时间添加][BY:十里坡剑神][QQ:112582793][2019-9-21][13:49:07]
extern void FBItemTimeAdd(LONG lId, void* p, DWORD we);
#endif //_GLOBALEVENTFUNC_H_


