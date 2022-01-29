// cScriptManager.cpp: implementation of the cScriptManager class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "cScriptManager.h"
#include "../GlobalEventFunc.h"
#include "../WindowIDEnum.h"
#include "cResourceManager.h"
#include "../CharacterDialog.h"
#include "../MugongSuryunDialog.h"
#include "../MugongDialog.h"
#include "../SuryunDialog.h"
#include "../InventoryExDialog.h"
#include "../ChatDialog.h"
#include "../MoneyDlg.h"
#include "../DealDialog.h"
#include "../QuickDialog.h"
#include "../PartyMemberDlg.h"
#include "../PartyBtnDlg.h"
#include "../PartyCreateDlg.h"
#include "../PartyInviteDlg.h"
#include "../MousePointer.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../PyogukDialog.h"
#include "../FriendDialog.h"
#include "../NoteDialog.h"
#include "../MiniNoteDialog.h"
#include "../MiniFriendDialog.h"
#include "../ReviveDialog.h"
#include "../MacroDialog.h"
#include "../ChatOptionDialog.h"
#include "../OptionDialog.h"
#include "../ExitDialog.h"
#include "../ExchangeDialog.h"
#include "../CharStateDialog.h"
#include "../MenuSlotDialog.h"
#include "../PKLootingDialog.h"
#include "../StreetStall.h"
#include "../StallKindSelectDlg.h"
#include "../StreetBuyStall.h"
#include "../BuyRegDialog.h"
#include "../NpcScriptDialog.h"
#include "../HelpDialog.h"
#include "../HelperSpeechDlg.h"
#include "../MainBarDialog.h"
#include "../ObjectGuagen.h"
#include "../cComboBoxEx.h"
#include "../CharMakeDialog.h"
#include "../MonsterGuageDlg.h"
#include "../PrivateWarehouseDialog.h"
#include "../MunpaMarkDialog.h"
#include "../cDivideBox.h"
#include "../LoadingDlg.h"
#include "../UpgradeDlg.h"
#include "../MixDialog.h"
#include "../TitanPartsMakeDlg.h"	
#include "../TitanMixDlg.h"			
#include "../TitanRepairDlg.h"		
#include "../TitanRecallDlg.h"	
#include "../TitanUpgradeDlg.h"	
#include "../TitanBreakDlg.h"		
#include "../TitanGuageDlg.h"		
#include "../TitanRegisterDlg.h"	
#include "../TitanDissolutionDlg.h"
#include "../TitanChangePreView.h"
#include "../TitanInventoryDlg.h"
#include "../ReinforceDlg.h"
#include "../ChannelDialog.h"
#include "../ServerListDialog.h"
#include "../WantNpcDialog.h"
#include "../WantRegistDialog.h"
#include "../QuestTotalDialog.h"
#include "../QuestDialog.h"
#include "../WantedDialog.h"
#include "../JournalDialog.h"
#include "../MPRegistDialog.h"
#include "../MPMissionDialog.h"
#include "../MPNoticeDialog.h"
#include "../MPGuageDialog.h"
#include "../ChinaAdviceDlg.h"
#include "../BailDialog.h"
#include "../EventNotifyDialog.h"
#include "../DissolutionDialog.h"
#include "../GuildCreateDialog.h"
#include "../GuildDialog.h"
#include "../GuildInviteDialog.h"
#include "../GuildMarkDialog.h"
#include "../GuildLevelupDialog.h"
#include "../GuildNickNameDialog.h"
#include "../GuildRankDialog.h"
#include "../GuildWarehouseDialog.h"
#include "../ItemShopDialog.h"
#include "../ItemShopGridDialog.h"
#include "../ItemShopInven.h"
#include "../MoveDialog.h"
#include "../PointSaveDialog.h"
#include "../SkillPointRedist.h"
#include "../SkillPointNotify.h"
#include "../ShoutDialog.h"
#include "../ShoutchatDialog.h"
#include "../ChaseDialog.h"
#include "../ChaseinputDialog.h"
#include "../NameChangeDialog.h"
#include "../NameChangeNotifyDlg.h"
#include "../GTRegistDialog.h"
#include "../GTRegistcancelDialog.h"
#include "../GTStandingDialog.h"
#include "../GTBattleListDialog.h"
#include "../GTScoreInfoDialog.h"
#include "../ItemManager.h"
#include "../MNPlayRoomDialog.h"
#include "../MNChannelDialog.h"
#include "../GuildFieldWarDialog.h"
#include "../PartyWarDialog.h"
#include "../SeigeWarDialog.h"
#include "../CharChangeDlg.h"
#include "../SealDialog.h"
#include "../ChangeJobDialog.h"
#include "../ReinforceResetDlg.h"
#include "../GuageDialog.h"
#include "../RareCreateDialog.h"
#include "../ReinforceDataGuideDlg.h"
#include "../TipBrowserDlg.h"
#include "../KeySettingTipDlg.h"
#include "../PetStateDlg.h"
#include "../PetStateMiniDlg.h"
#include "../PetInventoryDlg.h"
#include "../PetUpgradeDialog.h"
#include "../PetRevivalDialog.h"
#include "../GuildNoteDlg.h"
#include "../UnionNoteDlg.h"
#include "../GuildNoticeDlg.h"
#include "../SkillOptionChangeDlg.h"
#include "../SkillOptionClearDlg.h"
#include "../GuildJoinDialog.h"
#include "../ScreenShotDlg.h"
#include "../GuildPlusTimeDialog.h"
#include "../GuildMunhaDialog.h"
#include "../GuildTraineeDialog.h"
#include "../SurvivalCountDialog.h"
#include "../UniqueItemCurseCancellationDlg.h"
#include "../UniqueItemMixDlg.h"
#include "../SOSDialog.h"
#include "../IntroReplayDlg.h"
#include "../UniqueItemMixProgressBardlg.h"		
#include "../TitanMixProgressBardlg.h"		
#include "../TitanPartsProgressBardlg.h"		
#include "../SkinSelectDialog.h"	
#include "../CostumeSkinSelectDialog.h"	
#include "../SkillPointResetDlg.h"
#include "../StreetStallItemViewDlg.h"
#include "../StallFindDlg.h"	
#include "../AutoNoteDlg.h"	
#include "../AutoAnswerDlg.h"
#include "FortWarDialog.h"
#include "../PartyMatchingDlg.h"	
#include "MiniMarryDialog.h"      
#include "ControlDialog.h"         
#include "OtherCharacterEquip.h"    
#include "cDivideBoxEx.h"         
#include "ItemLockManager.h"        
#include "ItemUnLockManager.h"    
#include "OnlineFame.h"				//[在线洗恶引用][By:十里坡剑神][QQ:112582793][2017/11/22]
#include "BillboardDialog.h"		//[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "GuildTopList.h"			//[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
#include "RegID.h"					//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "RestPwd.h"				//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "CharacterRBTN.H"			//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "FBTipDlg.h"				//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/11/23]
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
#include "VideoCaptureDlg.h"	//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
#include "GoldShopSubDialog.h"
#include "GoldShopMainDailog.h"
#include "cShopItemInventPlusDlg.h"	//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
#include "FastConfigPointDlg.h"		//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][16:59:34]
#include "LotSplitItemDlg.h"		//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:05:44]
#include "VimuWithGoldDlg.h"		//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:51:53]
#include "cEquipItemInherit.h"		//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:29:29]
#include "cUnionWarManager.h"		//[盟战][BY:十里坡剑神][QQ:112582793][2019-10-28][23:37:05]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(cScriptManager);
cScriptManager::cScriptManager()
{	
}

cScriptManager::~cScriptManager()
{
	ReleaseParseTypeData();
	ReleaseImagePathTable();
}

void cScriptManager::ReleaseParseTypeData()
{
	m_ParseTypeTable.SetPositionHead();
	PARSE_TYPE_INFO* pParseTypeInfo = NULL;
	while(pParseTypeInfo = m_ParseTypeTable.GetData())
	{
		m_ParseTypeTable.StartGetMultiData(pParseTypeInfo->nKeyValue);
		PARSE_TYPE_INFO* pParseTypeSubInfo = NULL;
		while( pParseTypeSubInfo = m_ParseTypeTable.GetMultiData() )
		{
			m_ParseTypeTable.Remove(pParseTypeSubInfo->nKeyValue);
			SAFE_DELETE(pParseTypeSubInfo);
		}		
	}
	m_ParseTypeTable.RemoveAll();
}

void cScriptManager::ReleaseImagePathTable()
{
	m_ImageHardPath.SetPositionHead();
	sIMAGHARDPATH* pImageHardPath = NULL;
	while(pImageHardPath = m_ImageHardPath.GetData())
	{		
		SAFE_DELETE(pImageHardPath);
	}	
	m_ImageHardPath.RemoveAll();

	m_ItemHardPath.SetPositionHead();
	sIMAGHARDPATH* pItemHardPath = NULL;
	while(pItemHardPath = m_ItemHardPath.GetData())
	{		
		SAFE_DELETE(pItemHardPath);
	}	
	m_ItemHardPath.RemoveAll();

	m_MugongHardPath.SetPositionHead();
	sIMAGHARDPATH* pMugongHardPath = NULL;
	while(pMugongHardPath = m_MugongHardPath.GetData())
	{		
		SAFE_DELETE(pMugongHardPath);
	}	
	m_MugongHardPath.RemoveAll();

	m_AbilityHardPath.SetPositionHead();
	sIMAGHARDPATH* pAbilityHardPath = NULL;

	while(pAbilityHardPath = m_AbilityHardPath.GetData())
	{		
		SAFE_DELETE(pAbilityHardPath);
	}	
	m_AbilityHardPath.RemoveAll();

	m_BuffHardPath.SetPositionHead();	
	sIMAGHARDPATH* pBuffHardPath = NULL;
	while(pBuffHardPath = m_BuffHardPath.GetData())
	{		
		SAFE_DELETE(pBuffHardPath);
	}	
	m_BuffHardPath.RemoveAll();

	m_MiniMapHardPath.SetPositionHead();
	sIMAGHARDPATH* pMiniMapHardPath = NULL;
	while(pMiniMapHardPath = m_MiniMapHardPath.GetData())
	{		
		SAFE_DELETE(pMiniMapHardPath);
	}	
	m_MiniMapHardPath.RemoveAll();

	m_JackPotHardPath.SetPositionHead();
	sIMAGHARDPATH* pJackPotHardPath = NULL;
	while(pJackPotHardPath = m_JackPotHardPath.GetData())
	{		
		SAFE_DELETE(pJackPotHardPath);
	}	
	m_JackPotHardPath.RemoveAll();
	//[图片称号资源清理][By:十里坡剑神][QQ:112582793][2017/11/29]
	sIMAGHARDPATH* pImageNamePath = NULL;
	while(pImageNamePath = m_ImageNamePath.GetData())
	{
		SAFE_DELETE(pImageNamePath);
	}
	m_ImageNamePath.RemoveAll();

	IMAGENAMEINFO* pImageNameInfo = NULL;
	while (pImageNameInfo = m_ImageNameInfo.GetData())
	{
		SAFE_DELETE(pImageNameInfo);
	}
	m_ImageNameInfo.RemoveAll();
}

char * cScriptManager::GetMsg( int idx )
{
	return RESRCMGR->GetMsg(idx);
}
void cScriptManager::GetImage( CMHFile * fp, cImage * pImage )
{
	ASSERT(pImage);
	char buff[255];

	int	idx = 0;
	cImageRect rect = {0,0,0,0};

	fp->GetString(buff);
	if(buff[0] != '(')		return ;
	idx						= fp->GetInt();
	rect.left				= fp->GetLong();
	rect.top				= fp->GetLong();
	rect.right				= fp->GetLong();
	rect.bottom				= fp->GetLong();
	fp->GetString(buff);
	if(buff[0] != ')')		return ;

	GetImage( idx, pImage, &rect );	
}

void cScriptManager::GetImage( int idx, cImage * pImage, cImageRect * rect )
{
	ASSERT(pImage);

	pImage->SetSpriteObject(RESRCMGR->GetImageInfo(idx));
	IMAGE_NODE* pNode = RESRCMGR->GetInfo(idx);
	if(pNode)
		pImage->SetImageSrcSize(&pNode->size);
	pImage->SetImageSrcRect(rect);
}


void cScriptManager::InitScriptManager()
{
	char buf[64] = {0,};
	m_ImageNamePath.Initialize(1000);
	m_ImageNameInfo.Initialize(1000); //[图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]
	m_ImageHardPath.Initialize(300);
	m_ItemHardPath.Initialize(1200);
	m_MugongHardPath.Initialize(100);
	m_AbilityHardPath.Initialize(100);
    m_BuffHardPath.Initialize(200);
	m_MiniMapHardPath.Initialize(40);
    m_JackPotHardPath.Initialize(10);

	CMHFile file;
	sIMAGHARDPATH* pPath = NULL;
	int index = 0;
	if( file.Init( FILE_IMAGE_HARD_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			file.GetLineX(buf,MAX_ITEMNAME_LENGTH);// [注释] [by:十里坡剑神][QQ:112582793][2018/11/22]
			if( m_ImageHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}
			m_ImageHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ITEM_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			file.GetLineX(buf,MAX_ITEMNAME_LENGTH);// [注释] [by:十里坡剑神][QQ:112582793][2018/11/22]
			if( m_ItemHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ItemHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MUGONG_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			if( m_MugongHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MugongHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_ABILITY_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			if( m_AbilityHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;				
			}

			m_AbilityHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_BUFF_PATH, "rb" ) == TRUE )
	{		
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			file.GetLineX(buf,MAX_ITEMNAME_LENGTH);// [注释] [by:十里坡剑神][QQ:112582793][2018/11/22]
			if( m_BuffHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_BuffHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_MINIMAP_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			if( m_MiniMapHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_MiniMapHardPath.Add( pPath, index );
		}

		file.Release();
	}
	if( file.Init( FILE_IMAGE_JACKPOT_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			if( m_JackPotHardPath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_JackPotHardPath.Add( pPath, index );
		}

		file.Release();
	}
	//[图片称号资源加载][By:十里坡剑神][QQ:112582793][2017/11/29]
	IMAGENAMEINFO* pImageInfo = NULL;
	if(file.Init(FILE_IMAGE_NAME_INFO,"rb") == TRUE)
	{
		while(!file.IsEOF())
		{
			pImageInfo = new IMAGENAMEINFO;
			pImageInfo->idx = file.GetWord();
			pImageInfo->IsTrends = file.GetBool();
			pImageInfo->ImageCount = file.GetWord();
			pImageInfo->Hight = file.GetWord();
			pImageInfo->Scaling.x = file.GetFloat();
			pImageInfo->Scaling.y = file.GetFloat();
			pImageInfo->LeftPosition = file.GetInt();
			file.GetLineX(buf,MAX_ITEMNAME_LENGTH);// [注释] [by:十里坡剑神][QQ:112582793][2018/11/22]
			if(m_ImageNameInfo.GetData(pImageInfo->idx))
			{
				SAFE_DELETE(pImageInfo);
				continue;
			}
			m_ImageNameInfo.Add(pImageInfo,pImageInfo->idx);
		}
		file.Release();
	}
	if( file.Init( FILE_IMAGE_NAME_PATH, "rb" ) == TRUE )
	{
		while( !file.IsEOF() )
		{
			pPath = new sIMAGHARDPATH;
			index = file.GetInt();
			pPath->idx = file.GetInt();
			pPath->left = file.GetLong();
			pPath->top = file.GetLong();
			pPath->right = file.GetLong();
			pPath->bottom = file.GetLong();
			file.GetLineX(buf,MAX_ITEMNAME_LENGTH);// [注释] [by:十里坡剑神][QQ:112582793][2018/11/22]
			if( m_ImageNamePath.GetData( index ) )
			{
				SAFE_DELETE(pPath);
				continue;
			}

			m_ImageNamePath.Add( pPath, index );
		}

		file.Release();
	}
}


void cScriptManager::GetImage( int hard_idx, cImage * pImage , ePATH_FILE_TYPE type )
{
	sIMAGHARDPATH* pData = NULL;
	switch( type )

	{
	case PFT_HARDPATH:	pData = m_ImageHardPath.GetData( hard_idx );	break;
	case PFT_ITEMPATH:	pData = m_ItemHardPath.GetData( hard_idx );	break;
	case PFT_MUGONGPATH:	pData = m_MugongHardPath.GetData( hard_idx );	break;
	case PFT_ABILITYPATH:	pData = m_AbilityHardPath.GetData( hard_idx );	break;
	case PFT_BUFFPATH:	pData = m_BuffHardPath.GetData( hard_idx );	break;
	case PFT_MINIMAPPATH:	pData = m_MiniMapHardPath.GetData( hard_idx );	break;
	case PFT_JACKPOTPATH:	pData = m_JackPotHardPath.GetData( hard_idx );	break;
	case PFT_IMAGENAME:	pData = m_ImageNamePath.GetData(hard_idx);break;  //[图片称号][By:十里坡剑神][QQ:112582793][2017/11/29]
	}

	if( pData == NULL )
	{
		pImage = NULL;
		return;
	}

	cImageRect rect = {0,0,0,0};
	rect.right = pData->right;
	rect.top = pData->top;
	rect.left = pData->left;
	rect.bottom = pData->bottom;

	GetImage( pData->idx, pImage, &rect );
}



cWindow * cScriptManager::GetDlgInfoFromFile(char * filePath, char* mode)
{
	CMHFile fp;
	if(!fp.Init(filePath, mode))
		return NULL;
	char buff[256]={0,};
	cWindow * dlg = NULL;


	while(1)
	{
		fp.GetString(buff);
		if(fp.IsEOF())
			break;
		if(buff[0] == '@')
		{
			fp.GetLineX(buff, 256);
			continue;
		}
				
		PARSE_TYPE eParseType = GetParseType(buff);		
		{			
			switch(eParseType)
			{
			case eDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			case eCHARGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eLISTDLGEX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cListDialogEx), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharacterDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMUGONGSURYUNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMugongSuryunDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMainBarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CInventoryExDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChatDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			case eSTALLOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStallKindSelectDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetBuyStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eBUYREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBuyRegDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSTREETSTALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CStreetStall), &fp);
					else
						__asm int 3;
					break;
				}
			case eNPCSCRIPTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cNpcScriptDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eHELPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cHelpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cCharMakeDlg *)GetInfoFromFile((cWindow*)(new cCharMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}				
			case eHELPERSPEECHDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cHelperSpeechDlg *)GetInfoFromFile((cWindow*)(new cHelperSpeechDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONEYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoneyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDEALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CDealDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eQUICKDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuickDialog), &fp);
					else
						__asm int 3;	
					break;
				}

			case ePA_MEMBER1DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER2DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER3DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER4DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER5DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case ePA_MEMBER6DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyMemberDlg *)GetInfoFromFile((cWindow*)(new CPartyMemberDlg), &fp);
					else
						__asm int 3;
					break;
				}

			case ePA_BTNDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyBtnDlg *)GetInfoFromFile((cWindow*)(new CPartyBtnDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case ePA_CREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyCreateDlg *)GetInfoFromFile((cWindow*)(new CPartyCreateDlg), &fp);
					else
						__asm int 3;			
					break;
				}
			case ePA_INVITEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPartyInviteDlg *)GetInfoFromFile((cWindow*)(new CPartyInviteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eANI:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cAni *)GetInfoFromFile((cWindow*)(new cAni), &fp);
					else
						__asm int 3;	
					break;
				}

			case eMOUSEPOINTER:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMousePointer *)GetInfoFromFile((cWindow*)(new CMousePointer), &fp);
					else
						__asm int 3;
					break;
				}
			case ePYOGUKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CPyogukDialog *)GetInfoFromFile((cWindow*)(new CPyogukDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CFriendDialog *)GetInfoFromFile((cWindow*)(new CFriendDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CNoteDialog *)GetInfoFromFile((cWindow*)(new CNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniNoteDialog *)GetInfoFromFile((cWindow*)(new CMiniNoteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMINFRIENDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CMiniFriendDialog *)GetInfoFromFile((cWindow*)(new CMiniFriendDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eREVIVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CReviveDialog*)GetInfoFromFile((cWindow*)(new CReviveDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CChannelDialog*)GetInfoFromFile((cWindow*)(new CChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eWANTNPCDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantNpcDialog*)GetInfoFromFile((cWindow*)(new CWantNpcDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eWANTREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CWantRegistDialog*)GetInfoFromFile((cWindow*)(new CWantRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}				
			case eEXITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExitDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMACRODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMacroDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new COptionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEXCHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CExchangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CCharStateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMENUSLOTDIALOG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMenuSlotDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePKLOOTINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CPKLootingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCNADVICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CChinaAdviceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDISSOLUTIONDLG:
				{

					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CDissolutionDialog), &fp);
					else
						__asm int 3;
					break;
				}					
			case eMNPLAYROOMDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNPlayRoomDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMNCHANNELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = GetInfoFromFile((cWindow*)(new CMNChannelDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eMINIMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eBIGMAPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBigMapDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMONSTERGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMonsterGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eDIVIDEBOX:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBox), &fp);
					else
						__asm int 3;
					break;
				}
			case eDIVIDEBOXEX: 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cDivideBoxEx), &fp);
					else
						__asm int 3;
					break;
				}
			case eLOADINGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CLoadingDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMixDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eREINFORCEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSERVERLISTDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CServerListDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eQUESTTOTALDLG:
				{
					if((fp.GetString())[0] == '{')

						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuestTotalDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPREGISTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPMISSIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPMissionDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPNoticeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eMPGUAGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMPGuageDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eBAILDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CBailDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGMOPENTALKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CEventNotifyDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eGUILDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildCreateDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDUNIONCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildUnionCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildDialog), &fp);
					else
						__asm int 3;	
					break;
				}
			case eGUILDINVITEDlG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildInviteDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDMARKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildMarkDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDLEVELUPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildLevelUpDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNICKNAMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildNickNameDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDRANKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildRankDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWAREHOUSEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarehouseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eITEMALLBASE:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemShopDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSAVEMOVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMoveDialog), &fp);

					else
						__asm int 3;
					break;
				}
			case eCHANGSAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPointSaveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARDECLAREDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarDeclareDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARRESULTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarResultDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGFWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGFWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildWarInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSKILLPOINTAGAINDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointRedist), &fp);
					else
						__asm int 3;
					break;
				}
			case eSKILLPOINTOPDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSkillPointNotify), &fp);
					else
						__asm int 3;
					break;
				}
			case ePARTYWARDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CPartyWarDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHALLWORLDCREATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHATALLDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CShoutchatDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSEECHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHASEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChaseinputDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eOPCHNAMECHANGEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CNameChangeNotifyDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTENTRYCANCELDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTRegistcancelDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTOURNAMENTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTStandingDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eGDTVIEWDLG:
				{

					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTBattleListDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eEVENTMAPCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGTScoreInfoDialog), &fp);
					else
						__asm int 3;			
					break;
				}
			case eSEIGEWARTIMEREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeRegDlg), &fp);
					else
						__asm int 3;		
					break;
				}
			case eSEIGEWARPROTECTREGDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProtectRegDlg), &fp);
					else
						__asm int 3;	
					break;
				}
			case eSEIGEWARINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWSTARTTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWStartTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARCHANGEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CCharChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eITEMLOCKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSealDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWPROFITDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWProfitDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eENGRAVEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWEngraveDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eSWCOUNTDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSWTimeDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHANGEJOBDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CChangeJobDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eRFDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceResetDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eRareCreateDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CRareCreateDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case ePETSTATEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETSTATEMINIDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetStateMiniDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETINVENTORYDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetInventoryDlg), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetUpgradeDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case ePETREVIVALDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CPetRevivalDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eRFDATAGUIDEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CReinforceDataGuideDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTIPBROWSERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTipBrowserDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eKSDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CKeySettingTipDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoteDlg *)GetInfoFromFile((cWindow*)(new CGuildNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eOnlineFame://[在线洗恶][By:十里坡剑神][QQ:112582793][2017/11/22]
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new OnlineFame), &fp);
					else
						__asm int 3;
					break;	
				}
			case eMainDlgHideBtn:
				{//[主界面隐藏按钮][By:十里坡剑神][QQ:112582793][2018/6/7]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cMainDlgHideBtn), &fp);
					else
						__asm int 3;
					break;	
				}
			case eSafeLockMainDlg:
				{//[安全锁主界面][By:十里坡剑神][QQ:112582793][2018/6/10]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cSafeLockMainDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eMainDlgSafeBtn:
				{//[安全锁按钮][By:十里坡剑神][QQ:112582793][2018/6/9]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cMainDlgSafeBtn), &fp);
					else
						__asm int 3;
					break;	
				}
			case eREGDLG:		//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new RegID), &fp);
					else
						__asm int 3;
					break;
				}
#ifdef _XDYY_
			case eOnlineVoiceDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cOnlineVoiceDlg), &fp);
					else
						__asm int 3;
					break;
				}
#endif	
			case eVideo_Capture_Dldg:
				{//[录像功能界面][By:十里坡剑神][QQ:112582793][2018/9/17]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CVideoCaptureDlg), &fp);
					else
						__asm int 3;
				}
				break;
			case eGoldShopMainDlg:
				{//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGoldShopMainDailog), &fp);
					else
						__asm int 3;
				}
				break;
			case eRESTPWDDLG:	
				{//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new RestPwd), &fp);
					else
						__asm int 3;
					break;
				}
			case eCHARACTERRBTNDLG:	
				{//[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CharacterRBTN), &fp);
					else
						__asm int 3;
					break;
				}	
			case eFBMAPTIPDLG:	
				{//[副本剩余时间提示][By:十里坡剑神][QQ:112582793][2017/12/8]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new FBTipDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eOpenPyogukDlg:	//[开仓卷选择界面][By:十里坡剑神][QQ:112582793][2017/12/9]
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new OpenPyoguk), &fp);
					else
						__asm int 3;
					break;
				}
			case eSingedDlg://[签到系统][By:十里坡剑神][QQ:112582793][2018/1/4]
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cSingedDialog), &fp);
					else
						__asm int 3;
					break;
				}
			case eRechargeDlg:
				{//[充值礼包][By:十里坡剑神][QQ:112582793][2018/1/31]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cRechargeManager), &fp);
					else
						__asm int 3;
					break;
				}
			case eSetingInfoDlg:
				{//[设置信息查看界面][By:十里坡剑神][QQ:112582793][2018/3/10]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cSetingInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTopRankDlg:
				{//[排名显示][By:十里坡剑神][QQ:112582793][2018/3/23]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cTopRankDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eInventoryRBTDlg:
				{//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cInventoryRBTDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSendItemInfoDlg:
				{//[发送物品信息窗口][By:十里坡剑神][QQ:112582793][2018/4/3]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cSendItemInfoDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eFastConfigPointDlg:
				{//[快速加点卷][BY:十里坡剑神][QQ:112582793][2019-4-5][16:58:58]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new FastConfigPointDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eLotSplitItemDlg:
				{//[批量拆分][BY:十里坡剑神][QQ:112582793][2019-4-8][10:06:04]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new LotSplitItemDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eVimuWithGoldDlg:
				{//[押注比武][BY:十里坡剑神][QQ:112582793][2019-4-20][22:52:12]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new VimuWithGoldDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eEquipItemInherit:
				{//[装备传承][BY:十里坡剑神][QQ:112582793][2019-8-22][13:29:06]
					if ((fp.GetString())[0] == '{')
					{
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new cEquipItemInherit), &fp);
					}
					else
						__asm int 3;
					break;
				}
			case eUnionWarInfoDlg:
				{//[盟战][BY:十里坡剑神][QQ:112582793][2019-8-22][13:29:06]
					if ((fp.GetString())[0] == '{')
					{
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new cUnionWarManager), &fp);
					}
					else
						__asm int 3;
					break;
				}
			}
		}
		{
			switch(eParseType)
			{
			case eALLYNOTEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CUnionNoteDlg *)GetInfoFromFile((cWindow*)(new CUnionNoteDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eGUILDNOTICEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CGuildNoticeDlg *)GetInfoFromFile((cWindow*)(new CGuildNoticeDlg), &fp);
					else
						__asm int 3;
					break;
				}	
			case eGUILDPLUSTIMEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CGuildPlusTimeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSkillTransDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionChangeDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionChangeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTDefaultDlg:
				{
					if((fp.GetString())[0] == '{')
						dlg = (CSkillOptionClearDlg *)GetInfoFromFile((cWindow*)(new CSkillOptionClearDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSCREENSHOTDLG: 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CScreenShotDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eJOINOPTIONDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildJoinDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eSURVCOUNTERDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CSurvivalCountDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEINFODLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildMunhaDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eGUILDTRAINEEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog*)GetInfoFromFile((cWindow*)(new CGuildTraineeDialog), &fp);
					else
						_asm int 3;
					break;
				}
			case eTITANPARTSMAKEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanPartsMakeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITANMIXDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanMixDlg), &fp);
					else
						__asm int 3;
					break;
				}			
			case eTITANUPGRADEDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanUpgradeDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITANBREAKDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanBreakDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITANPARTSCHANGEPREVIEWDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanChangePreViewDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITAN_INVENTORY_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanInventoryDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITAN_GUAGE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanGuageDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITAN_USE_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanRegisterDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eTITAN_BONGIN_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CTitanDissolutionDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eUNIQUEITEM_CURSECANCELLATION_DLG:

				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemCurseCancellationDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eUNIQUEITEM_MIX_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CUniqueItemMixDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eSOS_DLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CSOSDlg), &fp);
					else
						__asm int 3;
					break;
				}
			case eINTROREPLAY_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CIntroReplayDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eTITANREPAIR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRepairDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eTITANRECALL_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanRecallDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eUNIQUEITEMMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CUniqueItemMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eTITANMIX_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanMixProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eTITANPARTS_PROGRESSBAR_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CTitanPartsProgressBarDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eSKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eCOSTUMESKIN_SELECT_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CCostumeSkinSelectDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eSKILL_POINT_RESET_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CSkillPointResetDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eSTALLVIEWDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStreetStallItemViewDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eSTALLFINDDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CStallFindDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eAUTONOTEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoNoteDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eAUTOANSWERDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CAutoAnswerDlg), &fp );
					else
						__asm int 3;
					break;
				}

			case ePARTYMATCHING_DLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CPartyMatchingDlg), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWENGRAVEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWEngraveDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWTIMEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWTimeDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eFWWAREHOUSEDLG:
				{
					if( (fp.GetString())[0] == '{' )
						dlg = (cDialog*)GetInfoFromFile( (cWindow*)(new CFWWareHouseDialog), &fp );
					else
						__asm int 3;
					break;
				}
			case eMiniMarryDialog:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CMiniMaryDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eControlDialog: 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CControlDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eOtherCharacterEquip: 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CQuipInfoDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemLockDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemLockDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eItemUnLockDLG:
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new CItemUnLockDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eGUILDBOARDDLG: //[帮会排行][By:十里坡剑神][QQ:112582793][2017/12/5]
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new GuildTopList), &fp);
					else				
						__asm int 3;
					break;				
				}
			case eBillboardDialog:  //[角色排行][By:十里坡剑神][QQ:112582793][2017/12/5] 
				{
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cBillBoardDialog), &fp);
					else				
						__asm int 3;
					break;
				}
			case eStatuIcoBtnDlg:
				{//[状态图标隐藏按钮][By:十里坡剑神][QQ:112582793][2018/3/10]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cStatusIconBtnDlg), &fp);
					else				
						__asm int 3;
					break;
				}
			case eKillBoardDlg:
				{//[击杀排行][By:十里坡剑神][QQ:112582793][2018/4/22]
					if((fp.GetString())[0] == '{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cKillTopList), &fp);
					else				
						__asm int 3;
					break;
				}
			case eSafeLockDlg:
				{//[安全锁][By:十里坡剑神][QQ:112582793][2018/4/30]
					if((fp.GetString())[0]=='{')
						dlg = (cDialog *)GetInfoFromFile((cWindow*)(new cSafeLockDialog), &fp);
					else
						__asm int 3;
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
			
	fp.Release();
	
	return dlg;
}
cWindow * cScriptManager::GetCtrlInfo(cWindow * wnd, char * buff, CMHFile * fp)
{	
	PARSE_TYPE eParseType = GetParseType(buff);
	switch(eParseType)
	{
	case eBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cButton), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}	
	case eEDITBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cEditBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSTATIC:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cStatic), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case ePUSHUPBTN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cPushupButton), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCHECKBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cCheckBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCOMBOBOX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBox), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCOMBOBOXEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cComboBoxEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSPIN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cSpin), fp);
			ASSERT(wnd);
			wnd->Add(window);

			break;
		}
	case eLISTCTRL:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrl), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTCTRLEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListCtrlEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eICONDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eICONGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cIconGridDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTDLGEX:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialogEx), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLISTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cListDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eWEAREDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWearedExDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eLIST:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cList), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGEBAR:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuageBar), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eANI:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cAni), fp);
			ASSERT(wnd);
			wnd->Add(window);
		}
	case eGUAGE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuage), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eTEXTAREA:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cTextArea), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGEN:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cGuagen), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGUAGENE:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CObjectGuagen), fp);
 			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new cDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eCHARGUAGEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CGuageDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eMUNPAMARKDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMunpaMarkDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case ePRIVATEWAREHOUSEDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CPrivateWarehouseDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eMUGONGDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CMugongDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eSURYUNDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CSuryunDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eQUESTDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CQuestDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eWANTEDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CWantedDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eJOURNALDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CJournalDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eITEMSHOPGRIDDLG:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopGridDialog), fp);
			ASSERT(wnd);
			wnd->Add(window); 
			break;
		}
	case eSHOPITEMINVENGRID:
		{
			cWindow * window = GetInfoFromFile((cWindow*)(new CItemShopInven), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}	
	case eShopItemInventPlusDlg:
		{//[ShopInventoryPlus][By:十里坡剑神][QQ:112582793][2018/10/9]
			cWindow * window = GetInfoFromFile((cWindow*)(new cShopItemInventPlusDlg), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	case eGoldShopSubDlg:
		{//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
			cWindow * window = GetInfoFromFile((cWindow*)(new CGoldShopSubDialog), fp);
			ASSERT(wnd);
			wnd->Add(window);
			break;
		}
	default:
		{	
			break;
		}
	}	
	return wnd;
}
cWindow * cScriptManager::GetInfoFromFile(cWindow * wnd, CMHFile * fp)
{
	cPoint point={0,0,0,0};
	cPoint listBarPoint={0,0,0,0};
	char toolTipMsg[256]={0,};
	char buff[256]={0,};
	char funcName[256]={0,};
	char IDName[256]={0,};
	char staticText[256]={0,};
	char imagePath[256]={0,};

	RGBCOLOR color={0,0,0};
	BOOL fMovable = TRUE;
	BOOL fActive = TRUE;
	BOOL fPush = FALSE;
	cImage BasicImage;
	cImage OverImage;
	cImage ListOverImage;
	cImage PressImage	;
	cImage ToolTipImage	;
	cImage TopImage		;
	cImage MiddleImage	;
	cImage DownImage	;
	cImage HeadImage	;
	cImage BodyImage	;
	cImage ListBarImage	;
	cImage FocusImage	;
	cImage SelectedImage;

	cImage AniSpriteImage	;
	cImage GuagePieceImage	;
	cImage CheckImage		;
	cImage CheckBoxImage	;
	cImage selectedBGImage	;
	cImage dragoverBGImage	;
	cImage guageEffectPieceImage	;

	cImage BalloonImage[10];
	FLOAT  BalloonBorder = 8;

	BOOL	bChecked = FALSE;
	WORD	HeadHei = 0;
	WORD	BodyHei = 0;
	int		nLeftMargin = 0;
	int		nTopMargin = 0;

	WORD EditBoxSize = 0;
	WORD EditBoxEableTypingSize = 0;
	BOOL fSecret = FALSE;
	RGBCOLOR activeTextColor={0,0,0};
	RGBCOLOR nonactiveTextColor={0,0,0};
	RGBCOLOR fgColor={0,0,0};
	RGBCOLOR comboTextColor={0,0,0};
	char szBtnText[256]={0,};
	RGBCOLOR btnTextBasicCol={0,0,0};
	RGBCOLOR btnTextOverCol={0,0,0};
	RGBCOLOR btnTextPressCol={0,0,0};
	BYTE alphaDepth = 255;
	cCoord coord;
	RECT captionRect={0,0,0,0};
	RECT textRect={0,0,0,0};
	RECT imgSrcRect={0,0,0,0};
	BYTE cols=0;
	BYTE rows=0;
	BYTE tabNum=0;
	BYTE middleNum=0;
	WORD topHei = 0;
	WORD middleHei = 0;
	WORD downHei = 0;
	LONG minValue = 0;
	LONG maxValue = 100;
	LONG spinUnit = 10;
	BOOL lineUpward = TRUE;
	int	 interval = 0;
	WORD maxLine = 0;
	LONG listMaxHeight = 15;
	BYTE minMiddleNum=0;
	BYTE maxMiddleNum=0;
	WORD gridBorderX = 0;
	WORD gridBorderY = 0;
	cPoint gridPos ={0,0,0,0};
	cCoord validXY={0,0};
	BOOL vertical = FALSE;
	BOOL fSpriteloop = TRUE;
	WORD travelingType = 0;

	float guageMinVal=0;
	float guageMaxVal=0;
	float guageCurVal=0;
	cCoord guagePos = {0,0};
	WORD selectComboIdx = 0;

	BOOL bAutoClose = FALSE;
	cCoord textXY = {5,6};
	WORD	fontIdx = 0;
	BOOL bReadOnly = FALSE;
	LONG limitLineNum = 20;
	int	soundIdx = -1;
	WORD	wOption = 0;
	int	nLimitBytes = 256;
	int nTextAlign = TXT_LEFT | TXT_TOP;
	BYTE btnTextAniX = 1;
	BYTE btnTextAniY = 1;
	BOOL bAutoScroll = TRUE;
	BOOL bPassive = FALSE;
	int tt_idx = -1;
	cImageScale scale={1,1};
	cImageScale ListScale={1,1};
	float guageWidth = 0.f;
	float guagePieceWidth = 0.f;
	float guageEffectPieceWidth = 0.f;
	float guagePieceHeightScaleY = 1.f;
	float guageEffectPieceHeightScaleY = 1.f;
	DWORD	guageEstTime = 0;
	LONG		shadowTextX=0;
	LONG		shadowTextY=0;
	RGBCOLOR	dwShadowColor={10,10,10};
	BOOL		bShadow = FALSE;


	while(1)
	{
		fp->GetString(buff);
		if(buff[0] == '}'|| fp->IsEOF())
			break;
		if(buff[0] == '{')
			continue;
		if(buff[0] == '$')	
		{
			GetCtrlInfo(wnd, buff, fp);
			continue;
		}
		if(buff[0] == '@')
		{
			fp->GetLineX(buff, 256);
			continue;
		}
		PARSE_TYPE eParseType = GetParseType(buff);
	
		switch(eParseType)
		{
		case ePOINT:
			{
				point.x = fp->GetDword();
				point.y = fp->GetDword();
				point.w = fp->GetDword();
				point.h = fp->GetDword();

				wnd->cWindow::SetAbsXY(point.x, point.y);
				wnd->cWindow::SetRelXY(point.x, point.y);
				break;
			}
		case ePOINT_:
			{
				break;
			}
		case eFUNC:
			{
				fp->GetString(funcName);
				break;
			}
		case eID:
			{
				fp->GetString(IDName);
				break;
			}
		case eFONTIDX:
			{
				fontIdx = fp->GetWord();
				break;
			}
		case eAUTOCLOSE:
			{
				bAutoClose = fp->GetBool();
				break;
			}
		case eTOOLTIPIMAGE:
			{
				GetImage(fp, &ToolTipImage);
				break;
			}
		case eTOOLTIPMSG:
			{
				strcpy( toolTipMsg, GetMsg(fp->GetInt()));
				break;
			}
		case eTOOLTIPCOL:
			{
				color.r = fp->GetByte();
				color.g = fp->GetByte();
				color.b = fp->GetByte();
				break;
			}
		case eTEXTXY:
			{
				textXY.x = fp->GetDword();
				textXY.y = fp->GetDword();
				break;
			}
		case eMOVEABLE:
			{
				fMovable = fp->GetBool();
				break;
			}
		case eACTIVE:
			{
				fActive = fp->GetBool();
				break;
			}
		case eBASICIMAGE:
			{
				GetImage(fp, &BasicImage);
				break;
			}
		case eBASICIMAGE_:
			{
				break;
			}
		case eOVERIMAGE:
			{
				GetImage(fp, &OverImage);
				break;
			}
		case eLISTOVERIMAGE:
			{
				GetImage(fp, &ListOverImage);
				break;
			}
		case eSELECTIMAGE:
			{
				GetImage(fp, &SelectedImage);
				break;
			}
		case ePRESSIMAGE:
			{
				GetImage(fp, &PressImage);
				break;
			}
		case eFOCUSIMAGE:
			{
				GetImage(fp, &FocusImage);
				break;
			}
		case eBTNTEXT:
			{
				strcpy( szBtnText, GetMsg(fp->GetInt()) );
				btnTextBasicCol.r = fp->GetByte();
				btnTextBasicCol.g = fp->GetByte();
				btnTextBasicCol.b = fp->GetByte();

				btnTextOverCol.r = fp->GetByte();
				btnTextOverCol.g = fp->GetByte();
				btnTextOverCol.b = fp->GetByte();

				btnTextPressCol.r = fp->GetByte();
				btnTextPressCol.g = fp->GetByte();
				btnTextPressCol.b = fp->GetByte();
				break;
			}
		case eBTNTEXTANI:
			{
				btnTextAniX	= fp->GetByte();
				btnTextAniY	= fp->GetByte();
				break;
			}
		case eIMAGESRCRECT:
			{
				imgSrcRect.left =  fp->GetLong();
				imgSrcRect.top = fp->GetLong();
				imgSrcRect.right = fp->GetLong();
				imgSrcRect.bottom = fp->GetLong();
				break;
			}
		case eALPHA:
			{
				alphaDepth = fp->GetByte();
				break;
			}
		case eEDITSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSPINSIZE:
			{
				EditBoxSize = fp->GetWord();
				EditBoxEableTypingSize = fp->GetWord();
				break;
			}
		case eSECRET:
			{
				fSecret = fp->GetBool();
				break;
			}
		case eTEXTCOLOR:	
			{
				activeTextColor.r = fp->GetByte();
				activeTextColor.g = fp->GetByte();
				activeTextColor.b = fp->GetByte();
				nonactiveTextColor.r = fp->GetByte();
				nonactiveTextColor.g = fp->GetByte();
				nonactiveTextColor.b = fp->GetByte();
				break;
			}
		case eCOORD:
			{
				coord.x = fp->GetDword();
				coord.y = fp->GetDword();
				break;
			}
		case eFGCOLOR:
			{
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXT:
			{
				int msg_idx = fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				break;
			}
		case eITEMTOOLTIP:
			{
				tt_idx = fp->GetInt();
				break;
			}
		case eSHADOW:
			{
				bShadow = fp->GetBool();
				break;
			}
		case eSHADOWCOLOR:
			{
				dwShadowColor.r = fp->GetByte();
				dwShadowColor.g = fp->GetByte();
				dwShadowColor.b = fp->GetByte();
				break;
			}
		case eSHADOWTEXTXY:
			{
				shadowTextX = fp->GetLong();
				shadowTextY = fp->GetLong();
				break;
			}
		case eMESSAGE:
			{
				int msg_idx =  fp->GetInt();
				strcpy( staticText, GetMsg(msg_idx) );
				fgColor.r = fp->GetByte();
				fgColor.g = fp->GetByte();
				fgColor.b = fp->GetByte();
				break;
			}
		case eTEXTRECT:
			{
				textRect.left =  fp->GetLong();
				textRect.top = fp->GetLong();
				textRect.right = fp->GetLong();
				textRect.bottom = fp->GetLong();
				break;
			}
		case ePUSHUP:
			{
				fPush = fp->GetBool();
				break;
			}
		case eVALIDXY:
			{
				validXY.x = fp->GetDword();
				validXY.y = fp->GetDword();
				break;
			}
		case eCAPTIONRECT:
			{
				captionRect.left = fp->GetLong();
				captionRect.top = fp->GetLong();
				captionRect.right = fp->GetLong();
				captionRect.bottom = fp->GetLong();
				break;
			}
		case eICONCELLNUM:
			{
				cIconDialog * win = (cIconDialog *)wnd;
				win->SetCellNum(fp->GetByte());				
				break;
			}
		case eICONCELL:
			{
				RECT iconCellRect={0,0,0,0};
				iconCellRect.left = fp->GetLong();
				iconCellRect.top = fp->GetLong();
				iconCellRect.right = fp->GetLong();
				iconCellRect.bottom = fp->GetLong();
				cIconDialog * win = (cIconDialog *)wnd;
				win->AddIconCell(&iconCellRect);
				break;
			}
		case eSELECTOPTION:
			{
				wOption = fp->GetWord();
				break;
			}
		case eCELLRECT:
			{
				RECT CellRect={0,0,0,0};
				CellRect.left = fp->GetLong();
				CellRect.top = fp->GetLong();
				CellRect.right = fp->GetLong();
				CellRect.bottom = fp->GetLong();
				cIconGridDialog * win = (cIconGridDialog *)wnd;
				win->SetCellRect(&CellRect);
				break;
			}
		case eLIMITLINENUM:
			{
				limitLineNum = fp->GetLong();
				break;
			}
		case eLIMITBYTES:
			{
				nLimitBytes = fp->GetInt();
				break;
			}
		case eTEXTALIGN:
			{
				nTextAlign = fp->GetInt();
				if( nTextAlign == 0 )	nTextAlign = TXT_LEFT;
				else if( nTextAlign == 1 )	nTextAlign = TXT_RIGHT;
				else if( nTextAlign == 2 )	nTextAlign = TXT_CENTER;
				break;
			}
		case eAUTOSCROLL:
			{
				bAutoScroll = fp->GetByte();
				break;
			}
		case ePASSIVE:
			{
				bPassive = fp->GetByte();
				break;
			}
		case eTABNUM:
			{
				cTabDialog * win = (cTabDialog *)wnd;
				tabNum = fp->GetByte();
				win->InitTab(tabNum);
				break;
			}
		case eICONCELLBGIMAGE:
			{
				GetImage(fp, &selectedBGImage);
				break;
			}
		case eDRAGOVERBGIMAGE:
			{
				GetImage(fp, &dragoverBGImage);
				break;
			}
		case eCOLS:
			{
				cols = fp->GetByte();
				break;
			}
		case eROWS:
			{
				rows = fp->GetByte();
				break;
			}
		case eMIDDLENUM:
			{
				middleNum = fp->GetByte();
				break;
			}
		case eTOPIMAGE:
			{
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				break;
			}
		case eTOPHEIGHT:
			{
				topHei = fp->GetWord();
				break;
			}
		case eDOWNHEIGHT:
			{
				downHei = fp->GetWord();
				break;
			}
		case eMIDDELIMAGE:
			{
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				break;
			}
		case eMIDDLEHEIGHT:
			{
				middleHei = fp->GetWord();
				break;
			}
		case eDOWNIMAGE:
			{
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				break;
			}
		case eCOMBOTEXTCOLOR:
			{
				comboTextColor.r = fp->GetByte();
				comboTextColor.g = fp->GetByte();
				comboTextColor.b = fp->GetByte();
				break;
			}
		case eSELECTCOMBOINDEX:
			{
				selectComboIdx = fp->GetWord();
				break;
			}
		case eREADONLY:
			{
				bReadOnly = fp->GetBool();
				break;
			}
		case eADDSTRING:
			{
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				RGBCOLOR comboListColor={0,0,0};
				comboListColor.r = fp->GetByte();
				comboListColor.g = fp->GetByte();
				comboListColor.b = fp->GetByte();

				switch(wnd->GetType())
				{
				case WT_LIST:
					{
						cList * win = (cList *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				case WT_LISTDIALOG:
					{
						cListDialog * win = (cListDialog *)wnd;
						win->AddItem(msg2, RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b));
					}
					break;
				case WT_COMBOBOX:
					{
						cComboBox * win = (cComboBox *)wnd;
						ITEM* pItem = new ITEM;
						strcpy(pItem->string, msg2);
						pItem->rgb =  RGB_HALF(comboListColor.r, comboListColor.g, comboListColor.b);
						win->AddItem(pItem);
					}
					break;
				}
				break;
			}
		case eINITCOMBOLIST:
			{
				cComboBox * win = (cComboBox *)wnd;
				WORD listWidth = fp->GetWord();
				win->SetMaxLine(fp->GetWord());
				GetImage(fp, &TopImage);
				topHei = fp->GetWord();
				GetImage(fp, &MiddleImage);
				middleHei = fp->GetWord();
				GetImage(fp, &DownImage);
				downHei = fp->GetWord();
				GetImage(fp, &ListOverImage);
				win->InitComboList(listWidth, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage);
				break;
			}
		case eCREATE:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				win->Create(fp->GetInt());
				break;
			}
		case eHEADIMAGE:
			{
				GetImage(fp, &HeadImage);
				break;
			}
		case eBODYIMAGE:
			{
				GetImage(fp, &BodyImage);
				break;
			}
		case eHEADHEIGHT:
			{
				HeadHei = fp->GetWord();
				break;
			}
		case eBODYHEIGHT:
			{
				BodyHei = fp->GetWord();
				break;
			}
		case eINSERTCOLUMN:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				int colSize = fp->GetInt();
				win->InsertColoumn(col_idx, msg2, colSize);
				break;
			}
		case eSETITEMTEXT:
			{
				cListCtrl * win = (cListCtrl *)wnd;
				int col_idx =  fp->GetInt();
				int row_idx =  fp->GetInt();
				int msg_idx =  fp->GetInt();
				char msg2[255]={0,};
				strcpy( msg2, GetMsg(msg_idx) );
				win->SetItemText(col_idx, row_idx, msg2);
				break;
			}
		case eSPINMINMAX:
			{
				minValue = fp->GetLong();
				maxValue = fp->GetLong();
				break;
			}
		case eSPINUNIT:
			{
				spinUnit = fp->GetLong();
				break;
			}
		case eLINEUPWARD:
			{
				lineUpward = fp->GetBool();
				break;
			}
		case eLISTBARINTERVAL:
			{
				interval = fp->GetInt();
				break;
			}
		case eLISTBARPOINT:
			{
				listBarPoint.x = fp->GetDword();
				listBarPoint.y = fp->GetDword();
				listBarPoint.w = fp->GetDword();
				listBarPoint.h = fp->GetDword();
				break;
			}
		case eLISTBARIMAGE:
			{
				GetImage(fp, &ListBarImage);
				break;
			}
		case eLISTMAXLINE:
			{
				maxLine = fp->GetWord();
				break;
			}
		case eLINEHEIGHT:
			{
				listMaxHeight = fp->GetLong();
				break;
			}
		case eBTNCLICKSOUND:
			{
				soundIdx = fp->GetInt();
				break;
			}
		case eMINMAXMIDDLENUM:
			{
				minMiddleNum = fp->GetByte();

				maxMiddleNum = fp->GetByte();
				break;
			}
		case eINITGRID:
			{
				gridPos.x = fp->GetDword();
				gridPos.y = fp->GetDword();
				gridPos.w = fp->GetDword();
				gridPos.h = fp->GetDword();
				gridBorderX = fp->GetWord();
				gridBorderY = fp->GetWord();
				break;
			}
		case eINITGUAGEBAR:
			{
				cGuageBar * win = (cGuageBar *)wnd;
				interval = fp->GetLong();
				vertical = fp->GetBool();
				win->InitGuageBar(interval, vertical);
				break;
			}
		case eMAXSPRITE:
			{
				cAni * win = (cAni *)wnd;
				win->SetMaxSprite(fp->GetWord());
				break;
			}
		case eSPRITELOOP:
			{
				fSpriteloop = fp->GetBool();
				break;
			}
		case eTRAVELINGTYPE:
			{
				travelingType = fp->GetWord();
				break;
			}
		case eADDSPRITE:
			{
				cAni * win = (cAni *)wnd;
				GetImage(fp, &AniSpriteImage);
				WORD delayTime = fp->GetWord();
				win->AddSprite(&AniSpriteImage, delayTime);
				break;
			}
		case eGUAGEPIECEIMAGE:
			{
				GetImage(fp, &GuagePieceImage);
				break;
			}
		case eGUAGEMINVALUE:
			{
				guageMinVal = fp->GetFloat();
				break;
			}
		case eGUAGEMAXVALUE:
			{
				guageMaxVal = fp->GetFloat();
				break;
			}
		case eGUAGECURVALUE:
			{
				guageCurVal = fp->GetFloat();
				break;
			}
		case eGUAGENEVALUE:
			{
				guageCurVal		= fp->GetFloat();
				guageEstTime	= fp->GetDword();
				break;
			}
		case eGUAGEPOS:
			{
				guagePos.x = fp->GetLong();
				guagePos.y = fp->GetLong();
				break;
			}
		case eCHECKBOXIMAGE:
			{
				GetImage(fp, &CheckBoxImage);
				break;
			}
		case eCHECKIMAGE:
			{
				GetImage(fp, &CheckImage);
				break;
			}
		case eSETCHECK:
			{
				bChecked = fp->GetBool();
				break;
			}
		case eSCALE:
			{
				scale.x = fp->GetFloat();
				scale.y = fp->GetFloat();
				break;
			}
		case eLISTSCALE:
			{
				ListScale.x = fp->GetFloat();
				ListScale.y = fp->GetFloat();
				break;
			}
		case eGUAGEWIDTH:
			{
				guageWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEWIDTH:
			{
				guagePieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEIMAGE:
			{
				GetImage(fp, &guageEffectPieceImage);
				break;
			}
		case eGUAGEEFFECTPIECEWIDTH:
			{
				guageEffectPieceWidth = fp->GetFloat();
				break;
			}
		case eGUAGEPIECEHEIGHTSCALE:
			{
				guagePieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eGUAGEEFFECTPIECEHEIGHTSCALE:
			{
				guageEffectPieceHeightScaleY = fp->GetFloat();
				break;
			}
		case eBALLOONIMAGETOP:
			{
				GetImage(fp, &BalloonImage[0]);
				GetImage(fp, &BalloonImage[1]);
				GetImage(fp, &BalloonImage[2]);
				break;
			}
		case eBALLOONIMAGEMIDDLE:
			{
				GetImage(fp, &BalloonImage[3]);
				GetImage(fp, &BalloonImage[4]);
				GetImage(fp, &BalloonImage[5]);
				break;
			}
		case eBALLOONIMAGEBOTTOM:
			{
				GetImage(fp, &BalloonImage[6]);
				GetImage(fp, &BalloonImage[7]);
				GetImage(fp, &BalloonImage[8]);
				break;
			}
		case eBALLOONIMAGETAIL:
			{
				GetImage(fp, &BalloonImage[9]);
				break;
			}
		case eBALLOONIMAGEBORDER:
			{
				BalloonBorder = fp->GetFloat();
				break;
			}
		default:
			{				
				break;
			}
		}
	}

	wnd->SetAutoClose(bAutoClose);
	wnd->SetFontIdx(fontIdx);
	wnd->SetImageSrcRect(&imgSrcRect);
	wnd->SetScale(&scale);
	wnd->OnCreate(fActive);

	SWINDOWTYPE(wnd->GetType())
	CASE(WT_GUAGE)
		cGuage * win = (cGuage *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitGuage(guagePos.x,guagePos.y);
		win->SetMinValue(guageMinVal);
		win->SetMaxValue(guageMaxVal);
		win->SetCurValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		
	BREAK
	CASE(WT_GUAGEN)
		cGuagen * win = (cGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
	BREAK
	CASE(WT_GUAGENE)
		CObjectGuagen * win = (CObjectGuagen *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetGuageImagePos(guagePos.x,guagePos.y);
		win->SetValue(guageCurVal, guageEstTime);
		win->SetPieceImage(&GuagePieceImage);
		win->SetGuageWidth(guageWidth);
		win->SetGuagePieceWidth(guagePieceWidth);
		win->SetGuagePieceHeightScale(guagePieceHeightScaleY);


		win->SetGuageEffectPieceWidth(guageEffectPieceWidth);
		win->SetEffectPieceImage(&guageEffectPieceImage);
		win->SetGuageEffectPieceHeightScale(guageEffectPieceHeightScaleY);
	BREAK
	CASE(WT_ANI)
		cAni * win = (cAni *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetLoop(fSpriteloop);
		win->SetTravelingType(travelingType);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_SPIN)
		cSpin * win = (cSpin *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->InitSpin(EditBoxSize, EditBoxEableTypingSize);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetMinMax(minValue, maxValue);
		win->SetUnit(spinUnit);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_STATIC)
		cStatic * win = (cStatic *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetValidXY(validXY.x,validXY.y);
		win->SetTextXY(textXY.x, textXY.y);
		win->SetStaticText(staticText);
		win->SetAlign( nTextAlign );
		win->SetFGColor(RGB_HALF(fgColor.r, fgColor.g, fgColor.b));

		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);

		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);

		if( tt_idx >= 0 )
		{
			cImage imgToolTip;
			SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
			win->SetToolTip( "", RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_DEFAULT );
			ITEMMGR->AddItemDescriptionToolTip( win, tt_idx, TTTC_DEFAULT, 0 );
		}

	BREAK
	CASE(WT_PUSHUPBUTTON)
		cPushupButton * win = (cPushupButton *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else	
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);		
		win->SetPush(fPush);
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );

		win->SetAlign( nTextAlign );
		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);
		win->SetPassive(bPassive);
	BREAK
	CASE(WT_COMBOBOX)
		cComboBox * win = (cComboBox *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);

		win->SetOverImageScale(ListScale.x, ListScale.y); 
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_COMBOBOXEX)
		cComboBoxEx * win = (cComboBoxEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, NULL, IDSEARCH(IDName));
		win->SetComboTextColor(RGB_HALF(comboTextColor.r, comboTextColor.g, comboTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SelectComboText(selectComboIdx);
		win->SetOverImageScale(ListScale.x, ListScale.y); 
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_BUTTON)
		cButton * win = (cButton *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, NULL, IDSEARCH(IDName));
		else 
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &OverImage, &PressImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));

		if(FUNCSEARCH(funcName) == -1)
		{
			int a = 0;
		}

		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		if(*szBtnText != 0)
		win->SetText(szBtnText, RGB_HALF(btnTextBasicCol.r,btnTextBasicCol.g,btnTextBasicCol.b), RGB_HALF(btnTextOverCol.r,btnTextOverCol.g,btnTextOverCol.b), RGB_HALF(btnTextPressCol.r,btnTextPressCol.g,btnTextPressCol.b)); 
		win->SetTextXY(textXY.x,textXY.y);
		win->SetAlpha(alphaDepth);
		win->SetTextAni(btnTextAniX, btnTextAniY );
		win->SetAlign( nTextAlign );

		win->SetShadowTextXY(shadowTextX, shadowTextY);
		win->SetShadowColor(RGB_HALF(dwShadowColor.r, dwShadowColor.g, dwShadowColor.b));
		win->SetShadow(bShadow);

		if( soundIdx != -1 )
			win->SetClickSound( soundIdx );
	BREAK
	CASE(WT_CHECKBOX)	
		cCheckBox * win = (cCheckBox *)wnd;
		if(FUNCSEARCH(funcName) == -1)
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, NULL, IDSEARCH(IDName));
		else
			win->Init(point.x, point.y, point.w, point.h, &BasicImage, &CheckBoxImage, &CheckImage, g_mt_func[FUNCSEARCH(funcName)]._func, IDSEARCH(IDName));
		win->SetChecked(bChecked);
		win->SetCheckBoxMsg(staticText, RGB_HALF(fgColor.r,fgColor.g,fgColor.b));
	BREAK
	CASE(WT_EDITBOX)
		cEditBox * win = (cEditBox *)wnd;
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, &FocusImage, IDSEARCH(IDName));
		win->InitEditbox(EditBoxSize, EditBoxEableTypingSize);
		win->SetAlign( nTextAlign );
		win->SetToolTip(toolTipMsg, RGBA_MAKE(color.r,color.g,color.b, 255), &ToolTipImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
		win->SetSecret(fSecret);
		win->SetActiveTextColor(RGB_HALF(activeTextColor.r,activeTextColor.g,activeTextColor.b));
		win->SetNonactiveTextColor(RGB_HALF(nonactiveTextColor.r,nonactiveTextColor.g,nonactiveTextColor.b));
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
	BREAK
	CASE(WT_GUAGEBAR)
		cGuageBar * win = (cGuageBar *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		if(-1 != FUNCSEARCHEX(funcName))
			win->SetcbFunc(g_mt_funcEx[FUNCSEARCHEX(funcName)]._func);
		win->InitValue(guageMinVal, guageMaxVal, guageCurVal);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_TEXTAREA)
		cTextArea * win = (cTextArea *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitTextArea(&textRect, nLimitBytes, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei );
		win->SetLimitLine( limitLineNum );
		win->SetAlpha(alphaDepth);
		win->SetReadOnly(bReadOnly);
		win->SetMiddleScale( ListScale );
		win->SetScriptText(staticText);
	BREAK
	CASE(WT_LIST)
		cList * win = (cList *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &TopImage, IDSEARCH(IDName));
		win->InitList(maxLine, &textRect);
		win->SetLineUpward(lineUpward);
		win->SetListBarImage(&ListBarImage);
		win->SetListBar(listBarPoint.x, listBarPoint.y, listBarPoint.w, listBarPoint.h, interval);
		win->SetMaxLine(maxLine);
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_LISTCTRL)
		cListCtrl * win = (cListCtrl *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrl(cols, rows);
		win->InitListCtrlImage(&HeadImage, HeadHei, &BodyImage, BodyHei, &ListOverImage );
		win->SetOverImageScale(ListScale.x, ListScale.y); 
		win->SetAlpha(alphaDepth);
		win->SetSelectOption( wOption );
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_LISTCTRLEX)
		cListCtrlEx * win = (cListCtrlEx *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->InitListCtrlEx( cols, middleNum, middleHei, nLeftMargin, nTopMargin );
		win->InitListCtrlExImage( &HeadImage, &BodyImage, &DownImage, &SelectedImage, &ListOverImage );
		win->SetAlpha(alphaDepth);
	BREAK
	CASE(WT_LISTDIALOG)
		cListDialog * win = (cListDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->SetMinMaxMiddleNum(minMiddleNum, maxMiddleNum);
		win->Init(point.x, point.y, point.w, &TopImage, topHei, &MiddleImage, middleHei, &DownImage, downHei, &ListOverImage, middleNum, IDSEARCH(IDName));
		win->SetLineHeight(listMaxHeight);
		win->InitList(maxLine, &textRect);
		win->SetMaxLine(maxLine);
		win->SetOverImageScale(ListScale.x, ListScale.y); 
		win->SetAlpha(alphaDepth);
		win->SetAutoScroll( bAutoScroll );

		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARMAKEDIALOG)
		cCharMakeDlg * win = (cCharMakeDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DIALOG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_TABDIALOG)
		cTabDialog * win = (cTabDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHATDIALOG)
		CChatDialog * win = (CChatDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_ICONDIALOG)
		cIconDialog * win = (cIconDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WEAREDDIALOG)
		CWearedExDialog * win = (CWearedExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ICONGRIDDIALOG)
		cIconGridDialog * win = (cIconGridDialog *)wnd;

		win->SetValidXY(validXY.x,validXY.y);	
		win->InitDialog(point.x, point.y, point.w, point.h, &BasicImage, cols, rows, IDSEARCH(IDName));
		win->SetCaptionRect(&captionRect);
		win->InitGrid(gridPos.x,gridPos.y,gridPos.w,gridPos.h,gridBorderX,gridBorderY);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONSTERGUAGEDLG)
		cDialog * win = (cDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUICKDIALOG)
		CQuickDialog * win = (CQuickDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);		
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARINFODIALOG)
		CCharacterDialog * win = (CCharacterDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUGONGSURYUNDIALOG)
		CMugongSuryunDialog * win = (CMugongSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUGONGDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SURYUNDIALOG)
		CSuryunDialog * win = (CSuryunDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MAINDIALOG)
		CMugongDialog * win = (CMugongDialog *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_INVENTORYDIALOG)
		CInventoryExDialog * win = (CInventoryExDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPERSPEECHDIALOG)
		cHelperSpeechDlg* win = (cHelperSpeechDlg*)wnd;
		win->SetValidXY(validXY.x, validXY.y);
		win->Init(point.x, point.y, point.w, point.h, listMaxHeight, IDSEARCH(IDName) );
		win->Linking();
		win->InitBalloon( &BalloonImage[0], &BalloonImage[2], &BalloonImage[6], &BalloonImage[8], &BalloonImage[3],
			   &BalloonImage[5], &BalloonImage[1], &BalloonImage[7], &BalloonImage[4], &BalloonImage[9], BalloonBorder);
		win->SetTextRect(&textRect);
		win->SetCaptionRect(&captionRect);
		win->SetMovable(fMovable);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETSTALLDIALOG)
		CStreetStall * win = (CStreetStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_STREETBUYSTALLDIALOG)
		CStreetBuyStall * win = (CStreetBuyStall *)wnd;	
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_NPCSCRIPDIALOG)
		cNpcScriptDialog * win = (cNpcScriptDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);

		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_HELPDIALOG)
		cHelpDialog * win = (cHelpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MONEYDIALOG)
		CMoneyDlg * win = (CMoneyDlg *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_DEALDIALOG)
		CDealDialog * win = (CDealDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MUNPAMARKDLG)
		CMunpaMarkDialog * win = (CMunpaMarkDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PYOGUKDLG)
		CPyogukDialog * win = (CPyogukDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NOTEDLG)
		CNoteDialog * win = (CNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MININOTEDLG)
		CMiniNoteDialog * win = (CMiniNoteDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_FRIENDDLG)
		CFriendDialog * win = (CFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_MINIFRIENDDLG)
		CMiniFriendDialog * win = (CMiniFriendDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_WANTNPCDIALOG)
		CWantNpcDialog * win = (CWantNpcDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_WANTREGISTDIALOG)
		CWantRegistDialog * win = (CWantRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_QUESTTOTALDIALOG)
		CQuestTotalDialog * win = (CQuestTotalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);

		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_QUESTDIALOG)
		CQuestDialog * win = (CQuestDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetIconCellBGImage(&selectedBGImage);
		win->SetDragOverBGImage(&dragoverBGImage);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_WANTEDDIALOG)
		CWantedDialog * win = (CWantedDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_JOURNALDIALOG)
		CJournalDialog * win = (CJournalDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);

	BREAK
	CASE(WT_PKLOOTINGDLG)
		CPKLootingDialog* win = (CPKLootingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, -1);
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPREGISTDIALOG)
		CMPRegistDialog* win = (CMPRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPMISSIONDLG)
		CMPMissionDialog* win = (CMPMissionDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPNOTICEDIALOG)
		CMPNoticeDialog* win = (CMPNoticeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MPGUAGEDLG)
		CMPGuageDialog * win = (CMPGuageDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYCREATEDLG)
		CPartyCreateDlg * win = (CPartyCreateDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYINVITEDLG)
		CPartyInviteDlg * win = (CPartyInviteDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDCREATEDLG)
		CGuildCreateDialog * win = (CGuildCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDDLG)
		CGuildDialog * win = (CGuildDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDINVITEDLG)
		CGuildInviteDialog * win = (CGuildInviteDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDMARKDLG)
		CGuildMarkDialog * win = (CGuildMarkDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDLEVELUPDLG)
		CGuildLevelUpDialog * win = (CGuildLevelUpDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDNICKNAMEDLG)
		CGuildNickNameDialog * win = (CGuildNickNameDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDRANKDLG)
		CGuildRankDialog * win = (CGuildRankDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);		
	BREAK
	CASE(WT_GUILDWAREHOUSEDLG)
		CGuildWarehouseDialog * win = (CGuildWarehouseDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDUNIONCREATEDLG)
		CGuildUnionCreateDialog * win = (CGuildUnionCreateDialog *)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		win->SetActive(fActive);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEMSHOPDIALOG)
		CItemShopDialog * win = (CItemShopDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_MOVEDIALOG)
		CMoveDialog * win = (CMoveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_DECLARE_DLG)
		CGFWarDeclareDlg* win = (CGFWarDeclareDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_RESULT_DLG)
		CGFWarResultDlg* win = (CGFWarResultDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDFIELDWAR_INFO_DLG)
		CGFWarInfoDlg* win = (CGFWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDWAR_INFO_DLG)
		CGuildWarInfoDlg* win = (CGuildWarInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SKPOINTDIALOG)
		CSkillPointRedist * win = (CSkillPointRedist*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SKPOINTNOTIFYDIALOG)
		CSkillPointNotify * win = (CSkillPointNotify*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_PARTYWAR_DLG)
		CPartyWarDialog* win = (CPartyWarDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUT_DLG)
		CShoutDialog* win = (CShoutDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_SHOUTCHAT_DLG)
		CShoutchatDialog* win = (CShoutchatDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASE_DLG)
		CChaseDialog* win = (CChaseDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_CHASEINPUT_DLG)
		CChaseinputDialog* win = (CChaseinputDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGE_DLG)
		CNameChangeDialog* win = (CNameChangeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_NAMECHANGENOTIFY_DLG)
		CNameChangeNotifyDlg* win = (CNameChangeNotifyDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGIST_DLG)
		CGTRegistDialog* win = (CGTRegistDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTREGISTCANCEL_DLG)
		CGTRegistcancelDialog* win = (CGTRegistcancelDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSTANDING_DLG)
		CGTStandingDialog* win = (CGTStandingDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTBATTLELIST_DLG)
		CGTBattleListDialog* win = (CGTBattleListDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GTSCOREINFO_DLG)
		CGTScoreInfoDialog* win = (CGTScoreInfoDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_TIMEREG_DLG)
		CSWTimeRegDlg* win = (CSWTimeRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_PROTECTREG_DLG)
		CSWProtectRegDlg* win = (CSWProtectRegDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SEIGEWAR_INFO_DLG)
		CSWInfoDlg* win = (CSWInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_CHARCHANGE_DLG)
		CCharChangeDlg* win = (CCharChangeDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_SEAL_DLG)
		CSealDialog* win = (CSealDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SEIGEWAR_PROFIT_DLG)
		CSWProfitDlg* win = (CSWProfitDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_ENGRAVE_DLG)
		CSWEngraveDialog* win = (CSWEngraveDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SIEGEWAR_TIME_DLG)
		CSWTimeDialog* win = (CSWTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK		
	CASE(WT_ITEM_CHANGEJOB_DLG)
		CChangeJobDialog* win = (CChangeJobDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK	
	CASE(WT_SW_STARTTIME_DLG)
		CSWStartTimeDialog* win = (CSWStartTimeDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_REINFORCERESET_DLG)
		CReinforceResetDlg* win = (CReinforceResetDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_ITEM_RARECREATE_DLG)
		CRareCreateDialog* win = (CRareCreateDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SCREENSHOT_DLG)
		CScreenShotDlg* win = (CScreenShotDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_GUILDMUNHADLG)
		CGuildMunhaDialog* win = (CGuildMunhaDialog*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
			win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
	CASE(WT_SHOWEQUIPDLG)    
		CQuipInfoDlg * win = (CQuipInfoDlg*)wnd;
		win->SetValidXY(validXY.x,validXY.y);
		win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));
		win->SetMovable(fMovable);
		win->SetCaptionRect(&captionRect);
		win->SetAlpha(alphaDepth);
		if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK
		//[注册账号][By:十里坡剑神][QQ:112582793][2017/11/23]
		CASE(WT_REG_DLG)	
		RegID* win = (RegID*)wnd;
	win->SetValidXY(validXY.x,validXY.y);
	win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
	win->SetMovable(fMovable);
	win->SetCaptionRect(&captionRect);
	win->SetAlpha(alphaDepth);
	if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	//[密码修改][By:十里坡剑神][QQ:112582793][2017/11/23]
	CASE(WT_REPWD_DLG)	
		RestPwd* win = (RestPwd*)wnd;
	win->SetValidXY(validXY.x,validXY.y);
	win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
	win->SetMovable(fMovable);
	win->SetCaptionRect(&captionRect);
	win->SetAlpha(alphaDepth);
	if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_CHARRBTN_DLG) //角色右键菜单	by:十里坡剑神	QQ:112582793
		CharacterRBTN* win = (CharacterRBTN*)wnd;
	win->SetValidXY(validXY.x,validXY.y);
	win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
	win->SetMovable(fMovable);
	win->SetCaptionRect(&captionRect);
	win->SetAlpha(alphaDepth);
	if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	CASE(WT_GOLDSHOPSUB_DLG)//[新商城][By:十里坡剑神][QQ:112582793][2018/9/29]
		CGoldShopSubDialog* win = (CGoldShopSubDialog*)wnd;
	win->SetValidXY(validXY.x,validXY.y);
	win->Init(point.x, point.y, point.w, point.h, &BasicImage, IDSEARCH(IDName));		
	win->SetMovable(fMovable);
	win->SetCaptionRect(&captionRect);
	win->SetAlpha(alphaDepth);
	if(-1 != FUNCSEARCH(funcName))
		win->SetcbFunc(g_mt_func[FUNCSEARCH(funcName)]._func);
	BREAK;
	EWINDOWTYPE	
	return wnd;
}

void cScriptManager::CheckPosForResolution( cPoint* pPos )
{
	int right = pPos->x + pPos->w;
	int bottom = pPos->y + pPos->h;

	int Wgap = right - LOWRSLTN_W;
	int Hgap = bottom - LOWRSLTN_H;

	if( Wgap > 0 )
	{
		pPos->x -= Wgap;
	}
	if( Hgap > 0 )
	{
		pPos->y -= Hgap;
	}

	if( pPos->x < 0 || Wgap > LOWRSLTN_W )
	{
		pPos->x = 0;
	}
	if( pPos->y < 0 || Hgap > LOWRSLTN_H )
	{
		pPos->y = 0;
	}
}

BOOL cScriptManager::InitParseTypeData()
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	m_ParseTypeTable.Initialize(nMaxNum); 
	
	CMHFile fp;
	PARSE_TYPE_INFO* pParseTypeInfo;

	if(!fp.Init("./image/ParseTypeList.bin", "rb"))
		return FALSE;
	
	while(!fp.IsEOF())
	{
		pParseTypeInfo = NULL;
		pParseTypeInfo = new PARSE_TYPE_INFO;

		pParseTypeInfo->nType = fp.GetInt();
		SafeStrCpy(pParseTypeInfo->cpTypeName, fp.GetString(), MAX_PARSE_TYPE_NAME_LENGTH+1 );		
		int nKeyValue = ChangeHashKey(pParseTypeInfo->cpTypeName, nMaxNum);
		pParseTypeInfo->nKeyValue = nKeyValue;
		if(nKeyValue < 0)
		{
			return FALSE;
		}
		m_ParseTypeTable.Add(pParseTypeInfo, nKeyValue);
	}
	
	return TRUE;
}

int cScriptManager::ChangeHashKey(const char* cpTypeName, int nMaxNum)
{
	int nKeyValue = -1;
	int nValue = 0;

	if(nMaxNum < 1)
	{
		return -1;
	}
	
	int nStrSize = strlen(cpTypeName);
	nKeyValue = ((cpTypeName[0] + cpTypeName[1] + nStrSize) * (cpTypeName[nStrSize] + cpTypeName[nStrSize-1])) % nMaxNum;

	if(nKeyValue > nMaxNum)
	{
		return -1;
	}

	return nKeyValue;
}

PARSE_TYPE cScriptManager::GetParseType(const char* str)
{
	int nMaxNum = eMAX_PARSETYPE_NUM;

	int nKeyValue = ChangeHashKey(str, nMaxNum);

	if(nKeyValue < 0)
	{
		return (PARSE_TYPE)-1;
	}		
	
	PARSE_TYPE_INFO* pParseTypeList = m_ParseTypeTable.GetData(nKeyValue);
	
	if(pParseTypeList == NULL)
	{
		return (PARSE_TYPE)-1;
	}	

	if(strcmp(pParseTypeList->cpTypeName, str) != 0)
	{
		PARSE_TYPE_INFO* pListTemp = NULL;
		pListTemp = pParseTypeList;
		m_ParseTypeTable.SetPositionHead();
		m_ParseTypeTable.StartGetMultiData(nKeyValue);

		while(strcmp(pListTemp->cpTypeName, str) != 0)
		{				
			pListTemp = m_ParseTypeTable.GetMultiData();

			if(pListTemp == NULL)
			{
				return (PARSE_TYPE)-1;
			}
		}
		pParseTypeList = pListTemp;		
	}

	return (PARSE_TYPE)pParseTypeList->nType;
}
///////////////////////////////////////////////////////////////////////////////////////





