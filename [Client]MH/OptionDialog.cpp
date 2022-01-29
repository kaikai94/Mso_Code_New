#include "stdafx.h"
#include "OptionDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h" 
#include "./Interface/cPushupButton.h"
#include "./Interface/cGuageBar.h"
#include "./Interface/cCheckBox.h"
#include "./interface/cStatic.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "cMsgBox.h"
#include "ObjectManager.h"
#include "./interface/cScriptManager.h"
#include "ChatManager.h"
#include "MacroDialog.h"
#include "cSpin.h"
#include "MainBarDialog.h"
#include "MHMap.h"  
#include  "ObjectManager.h"
#include "cSpin.h"
#include "cCheckBox.h"
#include "cEditBox.h"
#include "MHMap.h"
#include "MugongBase.h"
#include "MugongDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListDialog.h" 
#include "ChatManager.h"
#include "ItemManager.h"
#include "./input/Keyboard.h"
#include "cWindowManager.h"
#include "cMsgBox.h"
#include "GameIn.h"
#include "AIManager.h"
#include "SkillManager_client.h"
#include <ShellAPI.h>
COptionDialog::COptionDialog()
{
}

COptionDialog::~COptionDialog()
{

}

void COptionDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_DIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}

void COptionDialog::Linking()
{
	cWindow* pWnd = ((cWindow*)((cDialog*)GetTabSheet(2))->GetWindowForID( OTI_CB_AUTOCONTROL ));
	cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	pWnd->SetToolTip( CHATMGR->GetChatMsg(1098), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );
	cDialog* pDlg = (cDialog*)GetTabSheet( 7 );
	cListDialog * Packet_List =((cListDialog*)pDlg->GetWindowForID(AI_LIST_PACKET_SELECT));
	if(Packet_List)
	{
		ITEMMGR->AddClearPacketItem(Packet_List);
	}
}

void COptionDialog::SetActive(BOOL val)
{
	if( !m_bDisable )
	{
		if( val )
		{
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			UpdateData( FALSE );
			UpdataSreenAI(FALSE);
		}
		else
		{
			((cListDialog*)(((cDialog*)GetTabSheet( 6 ))->GetWindowForID(AI_LIST_SKILL)))->RemoveAll();
			((cListDialog*)(((cDialog*)GetTabSheet( 5 ))->GetWindowForID(AI_PORTECT_LIFE_LIST)))->RemoveAll();
			((cListDialog*)(((cDialog*)GetTabSheet( 5 ))->GetWindowForID(AI_PORTECT_NAERYUK_LIST)))->RemoveAll();
			((cListDialog*)(((cDialog*)GetTabSheet( 5 ))->GetWindowForID(AI_PORTECT_SHIELD_LIST)))->RemoveAll();
			((cListDialog*)(((cDialog*)GetTabSheet( 5 ))->GetWindowForID(AI_PORTECT_ETC_LIST)))->RemoveAll();
		}
		cTabDialog::SetActive(val);
	}
	if(!val)
		sCPlayerAI->SaveConfig();
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_OPTIONDLGICON, m_bActive );
}

void COptionDialog::UpdateData( BOOL bSave )
{
	cDialog* pDlg;

	if( bSave )	
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	
		m_GameOption.bShowPet		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEPET ))->IsChecked();
		m_GameOption.bShowNpc		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDENPC ))->IsChecked();
		m_GameOption.bShowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEMONSTER ))->IsChecked();
		m_GameOption.bNoDeal		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->IsChecked();
		m_GameOption.bNoParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->IsChecked();
		m_GameOption.bNoFriend		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->IsChecked();
		m_GameOption.bNoVimu		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->IsChecked();
		m_GameOption.bNameMunpa		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->IsChecked();
		m_GameOption.bNameParty		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->IsChecked();
		m_GameOption.bNameOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->IsChecked();
		m_GameOption.bNoMemberDamage	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->IsChecked();
		m_GameOption.bNoGameTip		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->IsChecked();
		m_GameOption.bMunpaIntro	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->IsChecked();
		m_GameOption.nMacroMode		= ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->IsPushed();
		m_GameOption.bSoundBGM			= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->IsChecked();
		m_GameOption.bSoundEnvironment	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->IsChecked();
		m_GameOption.nVolumnBGM			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->GetCurValue();
		m_GameOption.nVolumnEnvironment = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->GetCurValue();

		pDlg = (cDialog*)GetTabSheet( 1 );
		m_GameOption.bNoWhisper		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->IsChecked();
		m_GameOption.bNoChatting	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->IsChecked();
		m_GameOption.bNoBalloon		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->IsChecked();
		m_GameOption.bAutoHide		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->IsChecked();
		m_GameOption.bNoShoutChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->IsChecked();
		m_GameOption.bNoGuildChat	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->IsChecked();
		m_GameOption.bNoAllianceChat= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->IsChecked();
		m_GameOption.bNoSystemMsg	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->IsChecked();
		m_GameOption.bNoExpMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->IsChecked();
		m_GameOption.bNoItemMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->IsChecked();
		m_GameOption.bNoTipMsg		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOTIPMSG ))->IsChecked();


		pDlg = (cDialog*)GetTabSheet( 2 );
		m_GameOption.nGamma			= ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->GetCurValue();
		m_GameOption.nSightDistance = ((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->GetCurValue();
		m_GameOption.bShadowHero	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->IsChecked(  );
		m_GameOption.bShadowMonster = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->IsChecked(  );
		m_GameOption.bShadowOthers	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->IsChecked( );
		m_GameOption.bNoAvatarView = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->IsChecked();
	

		m_GameOption.nEffectSnow    = ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->IsChecked( ); 

		m_GameOption.bAutoCtrl		= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->IsChecked();
		m_GameOption.bAmbientMax	= ((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->IsChecked();
		m_GameOption.nLODMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->IsPushed(); 
		m_GameOption.nEffectMode = ((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->IsPushed();

		//pDlg = (cDialog*)GetTabSheet( 3 );

	}
	else	
	{
		pDlg = (cDialog*)GetTabSheet( 0 );	

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEPET ))->SetChecked(m_GameOption.bShowPet );         
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDENPC ))->SetChecked( m_GameOption.bShowNpc );        
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HIDEMONSTER ))->SetChecked( m_GameOption.bShowMonster ); 

		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NODEAL ))->SetChecked( m_GameOption.bNoDeal );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOPARTY ))->SetChecked( m_GameOption.bNoParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFRIEND ))->SetChecked( m_GameOption.bNoFriend );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOFIGHT ))->SetChecked( m_GameOption.bNoVimu );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPANAME ))->SetChecked( m_GameOption.bNameMunpa );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_PARTYNAME ))->SetChecked( m_GameOption.bNameParty );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSNAME ))->SetChecked( m_GameOption.bNameOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MEMBERDAMAGE ))->SetChecked( m_GameOption.bNoMemberDamage );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_GAMETIP ))->SetChecked( m_GameOption.bNoGameTip );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAPR ))->SetChecked( m_GameOption.bMunpaIntro );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( !m_GameOption.nMacroMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( m_GameOption.nMacroMode );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_BGMSOUND ))->SetChecked( m_GameOption.bSoundBGM );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ENVSOUND ))->SetChecked( m_GameOption.bSoundEnvironment );

		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_BGMSOUND ))->SetCurValue( m_GameOption.nVolumnBGM );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_ENVSOUND ))->SetCurValue( m_GameOption.nVolumnEnvironment );
		pDlg = (cDialog*)GetTabSheet( 1 );	
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOWHISPER ))->SetChecked( m_GameOption.bNoWhisper );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOCHATTING ))->SetChecked( m_GameOption.bNoChatting );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOBALLOON ))->SetChecked( m_GameOption.bNoBalloon );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOHIDE ))->SetChecked( m_GameOption.bAutoHide );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_WORLDHIDE ))->SetChecked( m_GameOption.bNoShoutChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MUNPAHIDE ))->SetChecked( m_GameOption.bNoGuildChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_ALLMUNPAHIDE ))->SetChecked( m_GameOption.bNoAllianceChat );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOSYSTEMMSG ))->SetChecked( m_GameOption.bNoSystemMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOACQUIREMSG ))->SetChecked( m_GameOption.bNoExpMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOITEMMSG ))->SetChecked( m_GameOption.bNoItemMsg );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_NOTIPMSG ))->SetChecked( m_GameOption.bNoTipMsg );

		pDlg = (cDialog*)GetTabSheet( 2 );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_GAMMA ))->SetCurValue( m_GameOption.nGamma );
		((cGuageBar*)pDlg->GetWindowForID( OTI_GB_SIGHT ))->SetCurValue( m_GameOption.nSightDistance );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_HEROSHADOW ))->SetChecked( m_GameOption.bShadowHero );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_MONSTERSHADOW ))->SetChecked( m_GameOption.bShadowMonster );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_OTHERSSHADOW ))->SetChecked( m_GameOption.bShadowOthers );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AVATA ))->SetChecked( m_GameOption.bNoAvatarView );



		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_SNOWEFFECT ))->SetChecked( m_GameOption.nEffectSnow );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AUTOCONTROL ))->SetChecked( m_GameOption.bAutoCtrl );
		((cCheckBox*)pDlg->GetWindowForID( OTI_CB_AMBIENT ))->SetChecked( m_GameOption.bAmbientMax );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( !m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( m_GameOption.nLODMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( !m_GameOption.nEffectMode );
		((cPushupButton*)pDlg->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( m_GameOption.nEffectMode );

		DisableGraphicTab(m_GameOption.bAutoCtrl);

		//pDlg = (cDialog*)GetTabSheet( 3 );
	}
}
void COptionDialog::CheckHideSet()
{
	m_GameOption.bShowPet? OBJECTMGR->HideAllPet(FALSE):OBJECTMGR->HideAllPet(TRUE);		
	m_GameOption.bShowNpc? OBJECTMGR->HideAllNpc(FALSE):OBJECTMGR->HideAllNpc(TRUE);		
	m_GameOption.bShowMonster? OBJECTMGR->HideAllMonster(FALSE):OBJECTMGR->HideAllMonster(TRUE);		
}
void COptionDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	ControlId = lId;
	if( we & WE_BTNCLICK )
	{
		if(lId == AI_BTN_GETPOSITION )
		{
			cDialog* pDlg = (cDialog*)GetTabSheet( 4 );
			if (pDlg)
			{			
				HERO->GetPosition(&sCPlayerAI->Heropos);	 
				sCPlayerAI->sAiConfig.SetValue[ePointX]=(int)(sCPlayerAI->Heropos.x/100);
				sCPlayerAI->sAiConfig.SetValue[ePointY]=(int)(sCPlayerAI->Heropos.z/100);
				char TempPosX[10];
				char TempPosY[10];
				itoa(sCPlayerAI->sAiConfig.SetValue[ePointX],TempPosX,10);
				itoa(sCPlayerAI->sAiConfig.SetValue[ePointY],TempPosY,10);
				((cSpin*)pDlg->GetWindowForID(  AI_SPIN_POSITIONX ))->SetEditText(TempPosX) ;
				((cSpin*)pDlg->GetWindowForID(  AI_SPIN_POSITIONZ ))->SetEditText(TempPosY) ;	
				//sCPlayerAI->SaveConfig();
			}
		}
		else if(lId == AI_BTN_SETREST)
		{
			RestSet();
		}
		if( lId == OTI_BTN_OK ) //确定
		{
			UpdateData( TRUE );
			CheckHideSet();//[对象隐藏修正][By:十里坡剑神][QQ:112582793][2018/1/10]
			if( m_GameOption.bMunpaIntro )
			{
				m_GameOption.bMunpaIntro = FALSE;
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
				m_GameOption.bMunpaIntro = TRUE;
				OPTIONMGR->SetGameOption( &m_GameOption );
			}
			else
			{
				OPTIONMGR->SetGameOption( &m_GameOption );
				OPTIONMGR->ApplySettings();
			}
			OPTIONMGR->SendOptionMsg();		
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_CANCEL )
		{
			OPTIONMGR->CancelSettings();
			SetActive( FALSE );
		}
		else if( lId == OTI_BTN_RESET )
		{
			OPTIONMGR->SetDefaultOption();
			m_GameOption = *( OPTIONMGR->GetGameOption() );
			UpdateData( FALSE );
		}
		else if(lId == AI_INSTRUCT_BTN)
		{//[内挂说明][BY:十里坡剑神][QQ:112582793][2019-4-28][21:18:24]
			ShellExecute( NULL, "open", CHATMGR->GetChatMsg(2362),NULL, NULL, SW_SHOWNORMAL );
		}
	}

	if( we & WE_PUSHUP )
	{
		if( lId == OTI_PB_CHATMODE || lId == OTI_PB_MACROMODE )
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( lId ))->SetPush( TRUE );
		if( lId == OTI_PB_BASICGRAPHIC || lId == OTI_PB_DOWNGRAPHIC )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );
		if( lId == OTI_PB_BASICEFFECT || lId == OTI_PB_ONEEFFECT )
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( lId ))->SetPush( TRUE );

		//sCPlayerAI->SaveConfig();
	}

	if( we & WE_PUSHDOWN )
	{
		if( lId == OTI_PB_CHATMODE )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_MACROMODE ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_MACROMODE )
		{

			((cPushupButton*)((cDialog*)GetTabSheet( 0 ))->GetWindowForID( OTI_PB_CHATMODE ))->SetPush( FALSE );
		}
		if( lId == OTI_PB_BASICGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_DOWNGRAPHIC )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ))->SetPush( FALSE );
		}
		if( lId == OTI_PB_BASICEFFECT )

		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ))->SetPush( FALSE );
		}
		else if( lId == OTI_PB_ONEEFFECT )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ))->SetPush( FALSE );
		}
		if (lId == AI_PB_REBUILDLOCAL )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOCAL ))->SetPush( TRUE );
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOGIN ))->SetPush( FALSE );
			sCPlayerAI->sAiConfig.AutoSwitch[eReBuildType] = TRUE;
		}
		else if (lId == AI_PB_REBUILDLOGIN )
		{
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOGIN ))->SetPush( TRUE );
			((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOCAL ))->SetPush( FALSE );
			sCPlayerAI->sAiConfig.AutoSwitch[eReBuildType] = FALSE;
		}
	}
	if( we & WE_CHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(TRUE);
		}
		if(lId == AI_CB_TIMEROPEN)
		{
			cDialog* pDlg = (cDialog*)GetTabSheet( 3 );
			sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer] = ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMEROPEN ))->IsChecked();
			if(sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer])
				CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2327));
			else
				CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2328));

		}

	}
	else if( we & WE_NOTCHECKED )
	{
		if( lId == OTI_CB_AUTOCONTROL )
		{
			DisableGraphicTab(FALSE);
		}
		if(lId == AI_CB_TIMEROPEN)
		{
			cDialog* pDlg = (cDialog*)GetTabSheet( 3 );
			sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer] = ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMEROPEN ))->IsChecked();
			if(sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer])
				CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2327));
			else
				CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2328));
		}
	}
	if(we & WE_ROWCLICK)
	{
		if( lId == AI_LIST_SKILL)
			DeleteSkillItem();
		if(lId == AI_PORTECT_LIFE_LIST)
			DeleteLifeItem();
		if(lId == AI_PORTECT_NAERYUK_LIST)
			DeleteNaeRyukItem();
		if(lId == AI_PORTECT_SHIELD_LIST)
			DeleteShieldItem();
		if(lId ==AI_PORTECT_ETC_LIST)
			DeleteEtcItem();
		if( lId == AI_LIST_PACKET_SELECT)
			DeleteClearItem();
	}
	UpdataSreenAI(TRUE);
}
void COptionDialog::AddSkillListItem(CSkillInfo* SkillInfo)
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 6 );
	cListDialog * m_Skill_List =(cListDialog*)(pDlg->GetWindowForID(AI_LIST_SKILL));
	SKILLSET * pSkillSetInfo = NULL;
	pSkillSetInfo = sCPlayerAI->GetSkillSetInfo()->GetData(SkillInfo->GetSkillIndex());

	if(pSkillSetInfo)
	{
		if(pSkillSetInfo->StateIcon==19||pSkillSetInfo->StateIcon==20||
			pSkillSetInfo->StateIcon==21||pSkillSetInfo->StateIcon==22||
			pSkillSetInfo->StateIcon==23)
		{
			for(int i = 19;i<24;i++)
			{
				if (sCPlayerAI->sAiConfig.SkillInfo[i]!=0)
				{
					SKILLSET * pInfo= sCPlayerAI->GetSkillSetInfo()->GetData(sCPlayerAI->sAiConfig.SkillInfo[i]);
					sCPlayerAI->sAiConfig.SkillInfo[i]=0;
					m_Skill_List->RemoveItem(pInfo->SkillName);
				}
			}
		}	
		if(sCPlayerAI->sAiConfig.SkillInfo[pSkillSetInfo->StateIcon]!=0)return;
		sCPlayerAI->sAiConfig.SkillInfo[pSkillSetInfo->StateIcon] = pSkillSetInfo->SkillIndex;
		m_Skill_List->AddItem(pSkillSetInfo->SkillName,0xffffffff);
	}
	else if (SkillInfo->GetSkillIndex()==1202)
	{
		if(sCPlayerAI->sAiConfig.AutoSwitch[eUsePoHun]) return;
		sCPlayerAI->sAiConfig.AutoSwitch[eUsePoHun] = TRUE;
		m_Skill_List->AddItem(SkillInfo->GetSkillName(),0xffffffff);
	}
	else if(SkillInfo->GetSkillIndex()==1203)
	{
		if(sCPlayerAI->sAiConfig.AutoSwitch[eUseSheXin]) return;
		sCPlayerAI->sAiConfig.AutoSwitch[eUseSheXin] = TRUE;
		m_Skill_List->AddItem(SkillInfo->GetSkillName(),0xffffffff);
	}
	else if(SkillInfo->GetSkillIndex()==900||
		SkillInfo->GetSkillIndex()==910)
	{
		if(sCPlayerAI->sAiConfig.AutoSwitch[eAutoToNa]) return;
		sCPlayerAI->sAiConfig.AutoSwitch[eAutoToNa] = TRUE;
		m_Skill_List->AddItem(SkillInfo->GetSkillName(),0xffffffff);
	}
	else
	{
		CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2254));
	}
	sCPlayerAI->SaveConfig();
}
void COptionDialog::AddClearItemList(cIcon * icon)
{
	cDialog * pDlg = (cDialog*)GetTabSheet( 7 );
	cListDialog * Packet_List =((cListDialog*)pDlg->GetWindowForID(AI_LIST_PACKET_SELECT));
	//CYHHashTable<ITEM_PACKET_CLEAR> * pClearItemInfo=ITEMMGR->GetClearPacketItem();

	//ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(icon->GetData());
	//ITEM_PACKET_CLEAR * pItemClear=NULL;
	//if(pItemInfo)
	//{
		//if(!pClearItemInfo->GetData(pItemInfo->ItemIdx))
		//{

			//pItemClear=new  ITEM_PACKET_CLEAR;

			//pItemClear->ItemIdx=pItemInfo->ItemIdx;

			ITEMMGR->AddClearPacketItem(Packet_List,icon->GetData());

			//Packet_List->SetShowSelect(TRUE);

			//Packet_List->AddItem(pItemInfo->ItemName,0xffffffff);
		//}
	//}
}
void COptionDialog::AddPortectItem(cIcon * icon)
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(icon->GetData());
	if(pItemInfo)
	{
		sUseItem * pUseItem = NULL;
		pUseItem = sCPlayerAI->GetUseItemInfo()->GetData(pItemInfo->ItemIdx);
		if(!pUseItem)
		{
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2255));
			return;
		}
		cDialog* pDlg = (cDialog*)GetTabSheet(5);

		switch(pUseItem->ItemType)
		{
		case 1:
			{
				cListDialog * Life_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_LIFE_LIST));
				for(int i = 0;i<eMaxLife;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectLife[i]==pUseItem->ItemIdx)
						return;
				}
				for(int i = 0;i<eMaxLife;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectLife[i]==0)
					{
						sCPlayerAI->sAiConfig.ProtectLife[i] = pUseItem->ItemIdx;
						Life_List->AddItem(pUseItem->ItemName,0xffffffff);
						return;
					}
				}
			}
			break;
		case 2:
			{
				cListDialog * NaeRyuk_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_NAERYUK_LIST));
				for (int i = 0;i<eMaxNaeRyuk;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectNaeRyuk[i]==pUseItem->ItemIdx)
						return;
				}
				for(int i = 0;i<eMaxNaeRyuk;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectNaeRyuk[i]==0)
					{
						sCPlayerAI->sAiConfig.ProtectNaeRyuk[i] = pUseItem->ItemIdx;
						NaeRyuk_List->AddItem(pUseItem->ItemName,0xffffffff);
						return;
					}
				}
			}
			break;
		case 3:
			{
				cListDialog * Shield_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_SHIELD_LIST));
				for (int i = 0;i<eMaxShield;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectShield[i]==pUseItem->ItemIdx)
						return;
				}
				for(int i = 0;i<eMaxShield;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectShield[i]==0)
					{
						sCPlayerAI->sAiConfig.ProtectShield[i] = pUseItem->ItemIdx;
						Shield_List->AddItem(pUseItem->ItemName,0xffffffff);
						return;
					}
				}
			}
			break;
		case 4:
			{
				cListDialog * EtcItem_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_ETC_LIST));
				for (int i = 0;i<eMaxEtcItem;i++)
				{
					if(sCPlayerAI->sAiConfig.EtcItem[i]==pUseItem->ItemIdx)
						return;
				}
				for(int i = 0;i<eMaxEtcItem;i++)
				{
					if(sCPlayerAI->sAiConfig.EtcItem[i]==0)
					{
						sCPlayerAI->sAiConfig.EtcItem[i] = pUseItem->ItemIdx;
						EtcItem_List->AddItem(pUseItem->ItemName,0xffffffff);
						return;
					}
				}
			}
			break;
		}
	}
}
BOOL COptionDialog::checkNotMainSkill(DWORD idx)
{
	if(idx == 1202||idx == 1203||idx == 910||idx ==900)
		return TRUE;
	else
		return FALSE;
}

BOOL COptionDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{

	if((cDialog*)GetTabSheet( 5 )->IsActive())
	{
		AddPortectItem(icon);
		return FALSE;
	}
	else if((cDialog*)GetTabSheet( 6 )->IsActive())
	{//[此处有个小bug，需自定义检查图标类型否则非技能图标，获取技能信息会报错][By:十里坡剑神][QQ:112582793][2017/12/7]
		SKILLSET * pInfo = NULL;
		pInfo = sCPlayerAI->GetSkillSetInfo()->GetData(icon->GetData());
		if(!pInfo&&!checkNotMainSkill(icon->GetData())) return FALSE;
		CSkillInfo* SkillInfo = ((CMugongBase*)icon)->GetSkillInfo();
		if(SkillInfo)
		{	
			AddSkillListItem(SkillInfo);
			return FALSE;
		}
	}
	else if((cDialog*)GetTabSheet( 7 )->IsActive()&&icon->GetType()==WT_ITEM)
	{
		AddClearItemList(icon);
		return FALSE;
	}
	return FALSE;
}

void COptionDialog::UpdataSreenAI(BOOL Val)
{
	if(Val)
	{
		GetSet();
		GetSetProtect();
		GetClear();
		GetSkill();
		if(GetCurTabNum()==3)
			GetTimerSet();
		sCPlayerAI->sAiConfig.SetValue[eLastMap]			= MAP->GetMapNum();
	}
	else
	{
		UpdataSreenSet();
		UpdataSreenPortect();
		UpdataSreenClear();
		UpdataSreenSkill();
		UpdataTimerSet();
	}

}
void COptionDialog::RestSet()
{
	memset(&sCPlayerAI->sAiConfig,0,sizeof(sCPlayerAI->sAiConfig));
	sCPlayerAI->SaveConfig();
	UpdataSreenAI(FALSE);
}

void COptionDialog::DeleteEtcItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 5 );
	cListDialog * EtcItem_List =((cListDialog*)pDlg->GetWindowForID(AI_PORTECT_ETC_LIST));
	ITEM_INFO * ItemInfo = NULL;
	if(EtcItem_List)
	{
		int row = EtcItem_List->GetSelectRowIdx();
		ITEM* pItem = EtcItem_List->GetSelectedITEM(row);
		if(pItem)
		{
			ItemInfo = ITEMMGR->FindItemInfoForName(pItem->string);
			if (ItemInfo)
			{
				for (int i = 0;i<eMaxEtcItem;i++)
				{
					if(sCPlayerAI->sAiConfig.EtcItem[i]==ItemInfo->ItemIdx)
					{
						sCPlayerAI->sAiConfig.EtcItem[i] = 0;
						EtcItem_List->RemoveItem(ItemInfo->ItemName);
					}
				}
			}
		}	
	}
}
void COptionDialog::DeleteShieldItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 5 );
	cListDialog * Shield_List =((cListDialog*)pDlg->GetWindowForID(AI_PORTECT_SHIELD_LIST));
	ITEM_INFO * ItemInfo = NULL;
	if(Shield_List)
	{
		int row = Shield_List->GetSelectRowIdx();
		ITEM* pItem = Shield_List->GetSelectedITEM(row);
		if(pItem)
		{
			ItemInfo = ITEMMGR->FindItemInfoForName(pItem->string);
			if (ItemInfo)
			{
				for (int i = 0;i<eMaxShield;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectShield[i]==ItemInfo->ItemIdx)
					{
						sCPlayerAI->sAiConfig.ProtectShield[i] = 0;
						Shield_List->RemoveItem(ItemInfo->ItemName);
					}
				}
			}
		}	
	}
}
void COptionDialog::DeleteNaeRyukItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 5 );
	cListDialog * NaeRyuk_List =((cListDialog*)pDlg->GetWindowForID(AI_PORTECT_NAERYUK_LIST));
	ITEM_INFO * ItemInfo = NULL;
	if(NaeRyuk_List)
	{
		int row = NaeRyuk_List->GetSelectRowIdx();
		ITEM* pItem = NaeRyuk_List->GetSelectedITEM(row);
		if(pItem)
		{
			ItemInfo = ITEMMGR->FindItemInfoForName(pItem->string);
			if (ItemInfo)
			{
				for (int i = 0;i<eMaxNaeRyuk;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectNaeRyuk[i]==ItemInfo->ItemIdx)
					{
						sCPlayerAI->sAiConfig.ProtectNaeRyuk[i] = 0;
						NaeRyuk_List->RemoveItem(ItemInfo->ItemName);
					}
				}
			}
		}	
	}
}
void COptionDialog::DeleteLifeItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 5 );
	cListDialog * Life_List =((cListDialog*)pDlg->GetWindowForID(AI_PORTECT_LIFE_LIST));
	ITEM_INFO * ItemInfo = NULL;
	if(Life_List)
	{
		int row = Life_List->GetSelectRowIdx();
		ITEM* pItem = Life_List->GetSelectedITEM(row);
		if(pItem)
		{
			ItemInfo = ITEMMGR->FindItemInfoForName(pItem->string);
			if (ItemInfo)
			{
				for (int i = 0;i<eMaxLife;i++)
				{
					if(sCPlayerAI->sAiConfig.ProtectLife[i]==ItemInfo->ItemIdx)
					{
						sCPlayerAI->sAiConfig.ProtectLife[i] = 0;
						Life_List->RemoveItem(ItemInfo->ItemName);
					}
				}
			}
		}	
	}
}
void COptionDialog::DeleteSkillItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 6 );
	cListDialog * m_Skill_List =((cListDialog*)pDlg->GetWindowForID(AI_LIST_SKILL));
	int Row = m_Skill_List->GetSelectRowIdx();
	ITEM* pItem =m_Skill_List->GetSelectedITEM(Row);
	if(strcmp(pItem->string,"吐纳法")==0||strcmp(pItem->string,"真吐纳法")==0)
	{
		m_Skill_List->RemoveItem(pItem->string);
		sCPlayerAI->sAiConfig.AutoSwitch[eAutoToNa] = FALSE;
	}
	else if (strcmp(pItem->string,"破魂击")==0)
	{
		m_Skill_List->RemoveItem(pItem->string);
		sCPlayerAI->sAiConfig.AutoSwitch[eUsePoHun] = FALSE;
	}
	else if (strcmp(pItem->string,"慑心击")==0)
	{
		m_Skill_List->RemoveItem(pItem->string);
		sCPlayerAI->sAiConfig.AutoSwitch[eUseSheXin] = FALSE;
	}
	else
	{
		WORD StateIcon = sCPlayerAI->GetSkillStateIconForName(pItem->string);
		m_Skill_List->RemoveItem(pItem->string);
		sCPlayerAI->sAiConfig.SkillInfo[StateIcon]=0;
	}
}
void COptionDialog::DeleteClearItem()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 7 );

	cListDialog * m_Packet_Real_List =((cListDialog*)pDlg->GetWindowForID(AI_LIST_PACKET_SELECT));

	if(m_Packet_Real_List)
	{
		int row=m_Packet_Real_List->GetSelectRowIdx();

		ITEM* pItem =m_Packet_Real_List->GetSelectedITEM(row);

		if(pItem)
		{

			ITEM_INFO * pItemInfo=ITEMMGR->FindItemInfoForName(pItem->string);

			if(pItemInfo)
			{
				m_Packet_Real_List->RemoveItem(pItem->string);

				ITEMMGR->DeleteClearPacketItem(pItemInfo->ItemIdx);
			}
		}
	}
}
void COptionDialog::UpdataSreenPortect()
{
	cDialog* pDlg = (cDialog*)GetTabSheet(5);
	((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_LIFE ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eProtectLife]);
	((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_NAEYUK ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eProtectNaeRyuk]);
	((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_SHIELD ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eProtectShield]);
	((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_ITEM ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eProtectItem]);
	char buf[10];
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eProtectLifeVaule]	,buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_LIFE_VAUEL))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_LIFE_VAUEL))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eProtectNaeRyukVaule]	,buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_NAEYUK_VAUEL))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_NAEYUK_VAUEL))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eProtectShieldVaule],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_SHIELD_VAUEL))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_SHIELD_VAUEL))->SetReadOnly(TRUE);

	cListDialog * Life_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_LIFE_LIST));
	for(int i = 0;i<eMaxLife;i++)
	{
		if(sCPlayerAI->sAiConfig.ProtectLife[i]!=0)
		{
			DWORD a = sCPlayerAI->sAiConfig.ProtectLife[i];
			Life_List->AddItem(ITEMMGR->GetItemInfo(sCPlayerAI->sAiConfig.ProtectLife[i])->ItemName,0xffffffff);
		}
	}

	cListDialog * NaeRyuk_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_NAERYUK_LIST));
	for(int i = 0;i<eMaxNaeRyuk;i++)
	{
		if(sCPlayerAI->sAiConfig.ProtectNaeRyuk[i]!=0)
			NaeRyuk_List->AddItem(ITEMMGR->GetItemInfo(sCPlayerAI->sAiConfig.ProtectNaeRyuk[i])->ItemName,0xffffffff);
	}

	cListDialog * Shield_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_SHIELD_LIST));
	for(int i = 0;i<eMaxShield;i++)
	{
		if(sCPlayerAI->sAiConfig.ProtectShield[i]!=0)
			Shield_List->AddItem(ITEMMGR->GetItemInfo(sCPlayerAI->sAiConfig.ProtectShield[i])->ItemName,0xffffffff);
	}

	cListDialog * EtcItem_List =(cListDialog*)(pDlg->GetWindowForID(AI_PORTECT_ETC_LIST));
	for(int i = 0;i<eMaxEtcItem;i++)
	{
		if(sCPlayerAI->sAiConfig.EtcItem[i]!=0)
			EtcItem_List->AddItem(ITEMMGR->GetItemInfo(sCPlayerAI->sAiConfig.EtcItem[i])->ItemName,0xffffffff);
	}
}

void COptionDialog::UpdataSreenSet()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 4 );
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_LIFE ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoH]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_NAERYUK ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoL]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_SHIELD ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoT]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOSKILL1 ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoSkill1]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOSKILL2 ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoSkill2]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOREBUILD ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoReBuild]);
	char buf[10];
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eDaZuoLife],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZLIFE))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZLIFE))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eDaZuoNaeRyuk],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZNAERYUK))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZNAERYUK))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eDaZuoShield],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZSHIELD))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_CB_DZSHIELD))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePressKey1],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE1))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE1))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePressPage1],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB1))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB1))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePressKey2],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE2))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE2))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePressPage2],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB2))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB2))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePointX],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONX))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONX))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[ePointY],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONZ))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONZ))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eDistance],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_HUNTDISTANCE))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_HUNTDISTANCE))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eReBuildTime],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_REBUILDCOUNTTIME))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_REBUILDCOUNTTIME))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eDaZuoMonsterLife],buf,10);
	((cEditBox*)pDlg->GetWindowForID(AI_SPI_MONSTERLIFE))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPI_MONSTERLIFE))->SetReadOnly(TRUE);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eBuildTime],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_REBUILDTIME))->SetEditText(buf);

	if (sCPlayerAI->sAiConfig.AutoSwitch[eReBuildType])
	{
		((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOCAL ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOGIN ))->SetPush( FALSE );
	}
	else
	{
		((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOCAL ))->SetPush( FALSE );
		((cPushupButton*)((cDialog*)GetTabSheet( 4 ))->GetWindowForID( AI_PB_REBUILDLOGIN ))->SetPush( TRUE );
	}
}
void COptionDialog::UpdataSreenClear()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 7 );
	((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_MUSUANG ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoUseWuShuang]);
	((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_PARTY ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoJoinGroup]);
	((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_WANT ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoWant]);
	((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_CLOSEWND ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoCloseWin]);
	((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_ATTARPLAYER ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoAttrPlay]);
	//((cCheckBox*)pDlg->GetWindowForID( AI_CHECK_AUTOOPENBOX ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eAutoOpenBox]);
	char buf[10];
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eWantMoney],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_WANT))->SetEditText(buf);
}
void COptionDialog::UpdataSreenSkill()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 6 );
	char buf[10];

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.SetValue[eAutoTuNaLife],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_TUNALIFE))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_TUNALIFE))->SetReadOnly(TRUE);

	cListDialog * m_Skill_List =(cListDialog*)(pDlg->GetWindowForID(AI_LIST_SKILL));
	for (int i = 0;i<eMaxSkill;i++)
	{
		if(sCPlayerAI->sAiConfig.SkillInfo[i]!=0)
		{
			m_Skill_List->AddItem(SKILLMGR->GetSkillInfo(sCPlayerAI->sAiConfig.SkillInfo[i])->GetSkillName(),0xffffffff);
		}
	}
	if (sCPlayerAI->sAiConfig.AutoSwitch[eUsePoHun])
		m_Skill_List->AddItem(SKILLMGR->GetSkillInfo(1202)->GetSkillName(),0xffffffff);
	if (sCPlayerAI->sAiConfig.AutoSwitch[eUseSheXin])
		m_Skill_List->AddItem(SKILLMGR->GetSkillInfo(1203)->GetSkillName(),0xffffffff);
	if(sCPlayerAI->sAiConfig.AutoSwitch[eAutoToNa])
		m_Skill_List->AddItem(SKILLMGR->GetSkillInfo(900)->GetSkillName(),0xffffffff);
}
void COptionDialog::GetSetProtect()
{
	cDialog* pDlg = (cDialog*)GetTabSheet(5);
	sCPlayerAI->sAiConfig.AutoSwitch[eProtectLife]	= ((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_LIFE ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eProtectNaeRyuk]= ((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_NAEYUK ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eProtectShield]	= ((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_SHIELD ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eProtectItem]	= ((cCheckBox*)pDlg->GetWindowForID( AI_PORTECT_ITEM ))->IsChecked();

	sCPlayerAI->sAiConfig.SetValue[eProtectLifeVaule]	= atoi(((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_LIFE_VAUEL))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eProtectNaeRyukVaule]	= atoi(((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_NAEYUK_VAUEL))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eProtectShieldVaule]	= atoi(((cEditBox*)pDlg->GetWindowForID(AI_PORTECT_SHIELD_VAUEL))->GetEditText());
}
void COptionDialog::UpdataTimerSet()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 3 );
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMEROPEN ))->SetChecked(sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER1 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch1]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER2 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch2]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER3 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch3]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER4 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch4]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER5 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch5]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER6 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch6]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER7 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch7]);
	((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER8 ))->SetChecked(sCPlayerAI->sAiConfig.TimeSwitch[eSwitch8]);

	char buf[10];
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch1]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch1],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET1))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch2]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch2],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET2))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch3]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch3],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET3))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch4]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch4],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET4))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch5]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch5],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET5))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch6]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch6],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET6))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch7]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch7],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET7))->SetEditText(buf);
	memset(buf,0,10);
	if(sCPlayerAI->sAiConfig.TimeValue[eSwitch8]>=0)
		itoa(sCPlayerAI->sAiConfig.TimeValue[eSwitch8],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET8))->SetEditText(buf);

	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage1],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB1))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB1))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage2],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB2))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB2))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage3],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB3))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB3))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage4],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB4))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB4))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage5],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB5))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB5))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage6],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB6))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB6))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage7],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB7))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB7))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimePage[ePage8],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB8))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB8))->SetReadOnly(TRUE);
	
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey1],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE1))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE1))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey2],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE2))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE2))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey3],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE3))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE3))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey4],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE4))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE4))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey5],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE5))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE5))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey6],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE6))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE6))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey7],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE7))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE7))->SetReadOnly(TRUE);
	memset(buf,0,10);
	itoa(sCPlayerAI->sAiConfig.TimeKey[eKey8],buf,10);	
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE8))->SetEditText(buf);
	((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE8))->SetReadOnly(TRUE);
}
void COptionDialog::GetTimerSet()
{
	if(sCPlayerAI->sAiConfig.AutoSwitch[eUseTimer]&&ControlId!=AI_CB_TIMEROPEN)
	{
		CHATMGR->AddMsg(CTC_AI_TIP,CHATMGR->GetChatMsg(2326));
		UpdataTimerSet();
		return;
	}
	cDialog* pDlg = (cDialog*)GetTabSheet( 3 );
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch1]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER1 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch2]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER2 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch3]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER3 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch4]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER4 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch5]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER5 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch6]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER6 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch7]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER7 ))->IsChecked();
	sCPlayerAI->sAiConfig.TimeSwitch[eSwitch8]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_TIMER8 ))->IsChecked();

	sCPlayerAI->sAiConfig.TimeValue[eSwitch1]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET1))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch2]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET2))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch3]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET3))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch4]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET4))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch5]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET5))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch6]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET6))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch7]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET7))->GetEditText());
	sCPlayerAI->sAiConfig.TimeValue[eSwitch8]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_TIMERSET8))->GetEditText());
	
	sCPlayerAI->sAiConfig.TimePage[ePage1]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB1))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage2]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB2))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage3]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB3))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage4]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB4))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage5]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB5))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage6]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB6))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage7]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB7))->GetEditText());
	sCPlayerAI->sAiConfig.TimePage[ePage8]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERTAB8))->GetEditText());
	BYTE key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE1))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey1]		= key;
	key	= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE2))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey2]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE3))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey3]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE4))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey4]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE5))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey5]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE6))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey6]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE7))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey7]		= key;
	key = atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_TIMERKEYCODE8))->GetEditText());
	if(key==0)
		key=10;
	sCPlayerAI->sAiConfig.TimeKey[eKey8]		= key;
}
void COptionDialog::GetSet()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 4 );
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoH]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_LIFE ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoL]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_NAERYUK ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoDazuoT]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_DAZUO_SHIELD ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoSkill1]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOSKILL1 ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoSkill2]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOSKILL2 ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoReBuild]	= ((cCheckBox*)pDlg->GetWindowForID( AI_CB_AUTOREBUILD ))->IsChecked();

	sCPlayerAI->sAiConfig.SetValue[eDaZuoLife]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_CB_DZLIFE))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eDaZuoNaeRyuk]	= atoi(((cEditBox*)pDlg->GetWindowForID(AI_CB_DZNAERYUK))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eDaZuoShield]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_CB_DZSHIELD))->GetEditText());

	sCPlayerAI->sAiConfig.SetValue[ePressKey1]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE1))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[ePressPage1]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB1))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[ePressKey2]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLKEYCODE2))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[ePressPage2]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_SKILLTAB2))->GetEditText());

	sCPlayerAI->sAiConfig.SetValue[ePointX]			= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONX))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[ePointY]			= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_POSITIONZ))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eDistance]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_HUNTDISTANCE))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eReBuildTime]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPIN_REBUILDCOUNTTIME))->GetEditText());
	if(sCPlayerAI->sAiConfig.SetValue[eReBuildTime]==0)
		sCPlayerAI->sAiConfig.SetValue[eReBuildTime] = 5;
	sCPlayerAI->sAiConfig.SetValue[eDaZuoMonsterLife]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_SPI_MONSTERLIFE))->GetEditText());
	sCPlayerAI->sAiConfig.SetValue[eBuildTime]		= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_REBUILDTIME))->GetEditText());

}
void COptionDialog::GetSkill()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 6 );
	sCPlayerAI->sAiConfig.SetValue[eAutoTuNaLife]			= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_TUNALIFE))->GetEditText());

}
void COptionDialog::GetClear()
{
	cDialog* pDlg = (cDialog*)GetTabSheet( 7 );
	sCPlayerAI->sAiConfig.SetValue[eWantMoney]				= atoi(((cEditBox*)pDlg->GetWindowForID(AI_EDIT_AUTO_WANT))->GetEditText());

	sCPlayerAI->sAiConfig.AutoSwitch[eAutoUseWuShuang]		= ((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_MUSUANG ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoJoinGroup]			= ((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_PARTY ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoWant]				= ((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_WANT ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoCloseWin]			= ((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_CLOSEWND ))->IsChecked();
	sCPlayerAI->sAiConfig.AutoSwitch[eAutoAttrPlay]			= ((cCheckBox*)pDlg->GetWindowForID( AI_AUTO_ATTARPLAYER ))->IsChecked();
	//sCPlayerAI->sAiConfig.AutoSwitch[eAutoOpenBox]			= ((cCheckBox*)pDlg->GetWindowForID( AI_CHECK_AUTOOPENBOX ))->IsChecked();
}
void COptionDialog::DisableGraphicTab(BOOL bDisable)
{
			DWORD FullColor = 0xffffffff;
			DWORD HalfColor = RGBA_MAKE(200,200,200,255);

			if( bDisable )
			{
				FullColor = HalfColor;
			}

			cWindow* pWnd = NULL;
			cGuageBar* pGuag = NULL;
			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_GAMMA ));
			pGuag->SetDisable(bDisable);
			pGuag->SetImageRGB(FullColor);
			pGuag->SetGuageLock(bDisable, FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_GAMMA ))->SetFGColor(FullColor);
			pGuag = ((cGuageBar*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_GB_SIGHT ));
			pGuag->SetDisable(bDisable);
			pGuag->SetImageRGB(FullColor);
			pGuag->SetGuageLock(bDisable, FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_SIGHT ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_HEROSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);

			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_HEROSHADOW ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_MONSTERSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_MONSTERSHADOW ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_CB_OTHERSSHADOW ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_OTHERSSHADOW ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICGRAPHIC ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_DOWNGRAPHIC ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_DOWNGRAPHIC ))->SetFGColor(FullColor);
			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_BASICEFFECT ));
			pWnd->SetDisable(bDisable);

			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_BASICEFFECT ))->SetFGColor(FullColor);

			pWnd = ((cWindow*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_PB_ONEEFFECT ));
			pWnd->SetDisable(bDisable);
			pWnd->SetImageRGB(FullColor);
			((cStatic*)((cDialog*)GetTabSheet( 2 ))->GetWindowForID( OTI_TEXT_ONEEFFECT ))->SetFGColor(FullColor);	
}




