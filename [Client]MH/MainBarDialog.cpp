// MainBarDialog.cpp: implementation of the CMainBarDialog class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainBarDialog.h"
#include "GameIn.h"
#include "../GlobalEventFunc.h"
#include "./BarIcon.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cResourceManager.h"
#include "ObjectManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "VimuManager.h"
#include "./Interface/cAnimationManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "MacroDialog.h"
#include "./Input/Mouse.h"
#include "MapChange.h"

#include "QuestDialog.h"
#include "CharacterDialog.h"
#include "PartyManager.h"
#include "Interface/cFont.h"
#include "AbilityManager.h"
#include "ChatManager.h"
#include "./Interface/cStatic.h"

#include "PKManager.h"
#include "ExitManager.h"

#include "GameEventManager.h"
#include "HelpDialog.h"
#include "EventMapInfo.h"
#include "BattleSystem_Client.h"
#include "GameResourceManager.h"
#include "GuageDialog.h"
#include "GoldShopMainDailog.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainBarDialog::CMainBarDialog()
{
	m_IconBarWidth = 42;				
	m_IconBarHeight = 42;				
	m_IconBarSpace = 0;
	m_IconIndexCreator.Init( 100, IG_MAINBARICON_START );
	m_OldDragFlag = FALSE;
	m_crossStep = -1;
	m_crossBarHalfX = -6;
	m_crossBarHalfY	= -1;
	m_bFold			= FALSE;
	m_nSlidingDir	= 0;
	m_pBtnSize		= NULL;
}

CMainBarDialog::~CMainBarDialog()
{
	ReleaseBar();
}

void CMainBarDialog::InitBar()
{
	m_pBtnSize = (cButton*)GetWindowForID( MI_BTN_SIZE );

	m_lOriX = (LONG)GetAbsX();
	m_lOriY = (LONG)GetAbsY();
	m_lSlidingX = 0;
	m_lOffSetX = GetCaptionRect()->right;
	SCRIPTMGR->GetImage( 48, &m_MoveCrossImage );
	LoadInfo();
	//[Ä¬ÈÏÕ¹¿ª²Ëµ¥À¸,ÊÕËõÉèÖÃtrue][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2018/3/13]
	Fold(FALSE);
}

void CMainBarDialog::ReleaseBar()
{
	SaveInfo();

	int n = 0;
	CBarIcon * pIcon = NULL;
	while( pIcon = (CBarIcon *)m_MainList.RemoveTail() )
	{
		m_IconIndexCreator.ReleaseIndex( pIcon->GetID() );
		delete pIcon;
	}
}

void CMainBarDialog::LoadInfo()
{
	CMHFile file;
	if( !file.Init("./Image/InterfaceScript/barInfo.bin", "rb") )
		return;
	m_nIconNum = file.GetInt();
	for( int i = 0 ; i < m_nIconNum ; ++i )
	{
		BARICON_INFO info;
		info.type			= file.GetByte();
		info.kind			= file.GetWord();
		info.barPos			= file.GetWord();
		info.nToolTipMsg	= file.GetWord();

		CBarIcon * pIcon = GetIcon(info.kind);
		pIcon->InitInfo(&info);

		AddIcon(info.barPos, pIcon);
	}
	file.Release();
}

void CMainBarDialog::Refresh()
{

	int s_nMacro[] = {
		ME_TOGGLE_GOLDSHOP,	
		ME_TOGGLE_HELP,
		ME_TOGGLE_CHARACTERDLG,
		ME_TOGGLE_INVENTORYDLG,
		ME_TOGGLE_MUGONGDLG,
		ME_TOGGLE_QUESTDLG,
		ME_TOGGLE_MINIMAP,
		ME_TOGGLE_MUNPADLG,
		ME_TOGGLE_FRIENDLIST,
		ME_SEND_MEMO,		
		ME_TOGGLE_OPTIONDLG,
		ME_TOGGLE_EXITDLG,
		ME_COUNT,	
	};

	cImage ToolTipImg;
	SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );	

	char imagePath[128];
	char strMacro[32];
	
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
		BARICON_INFO* pInfo = pIcon->GetInfo();

		strcpy( imagePath, RESRCMGR->GetMsg(pInfo->nToolTipMsg) );

		sMACRO* pMacro = MACROMGR->GetCurMacroKey( s_nMacro[pInfo->kind] );
		if( pMacro )
		{
			GAMEIN->GetMacroDialog()->ConvertMacroToText( strMacro, pMacro );
			wsprintf( imagePath, "%s(%s)", imagePath, strMacro );
		}

		pIcon->SetToolTip( imagePath, RGB_HALF( 255, 255, 255 ), &ToolTipImg );
	}
}

void CMainBarDialog::SaveInfo()
{
//	CMHFile file;
//	if( !file.Init("./ini/barInfo.txt", "w+") )
//		return;

}


CBarIcon * CMainBarDialog::GetIcon(WORD idx)
{
	int ImgIdxForIdx [][3]=
	{
		{116,116,116},
		{98,98,98},
		{0,1,2},
		{3,4,5},
		{6,7,8},
		{9,10,11},
		{12,13,14},
		{15,16,17},
		{18,19,20},
		{21,22,23},
		{24,25,26},
		{27,28,29},
		{77,78,79},
	};

	cImage		ImageBasic;
	cImage		ImageOver;
	cImage		ImagePress;
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][0], &ImageOver  );
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][1], &ImagePress );
	SCRIPTMGR->GetImage( ImgIdxForIdx[idx][2], &ImageBasic );
		
	CBarIcon * pIcon = new CBarIcon;
	pIcon->Init( 0, 0, m_IconBarWidth, m_IconBarHeight, &ImageBasic, &ImageOver, &ImagePress, NULL, m_IconIndexCreator.GenerateIndex() );
	pIcon->SetMovable( TRUE );
	return pIcon;
}

DWORD CMainBarDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	
	if( m_bFold == FALSE )
	{
		if( m_nSlidingDir == eSLIDING_NONE )	
		{
			PTRLISTPOS pos = m_MainList.GetHeadPosition();
			while(pos)
			{
				CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
				pIcon->ActionEvent(mouseInfo);
			}
		}
		else if( m_nSlidingDir == eSLIDING_RIGHT )
		{
			m_lSlidingX += gTickTime;

			if( m_lSlidingX >= m_IconBarWidth*m_nIconNum - 1 )
			{
				m_lSlidingX = m_IconBarWidth*m_nIconNum;

				m_pBtnSize->SetRelXY( m_lOffSetX, 0 );
				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth(), GetHeight() );
				m_nSlidingDir = eSLIDING_NONE;
				
				m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
				m_lOriY = (LONG)GetAbsY();

				m_bFold = TRUE;
			}
			else
			{
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum - m_lSlidingX, 0 );
				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum - m_lSlidingX, GetHeight() );
			}
		}
		else
		{
			m_lSlidingX -= gTickTime;

			if( m_lSlidingX <= 1 ) 	//1 : buffer
			{
				m_lSlidingX = 0;
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum, 0 );
				SetAbsXY( m_lOriX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum, GetHeight() );

				m_nSlidingDir = eSLIDING_NONE;

				m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
				m_lOriY = (LONG)GetAbsY();
			}
			else
			{
				m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum - m_lSlidingX, 0 );

				SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
				SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum - m_lSlidingX, GetHeight() );
			}
		}
	}
	

	we |= cWindow::ActionEvent(mouseInfo);

	if( !m_bDisable && m_nSlidingDir == eSLIDING_NONE )		
		we |= ActionEventWindow(mouseInfo);

	we |= ActionEventComponent(mouseInfo);

	return we;
}

void CMainBarDialog::SetAbsXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
	
		pIcon->SetAbsXY( x + m_lOffSetX + pIcon->GetData()*m_IconBarWidth, y );
	}

	cDialog::SetAbsXY(x,y);
}

void CMainBarDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == MI_BTN_SIZE )
		{
			if( m_nSlidingDir == eSLIDING_NONE )
			{
				if( !m_bFold )

				{
					if( m_nSlidingDir == eSLIDING_NONE )
					{
						m_lOriX = (LONG)GetAbsX();
						m_lOriY = (LONG)GetAbsY();
						m_lSlidingX = 0;	
					}

					m_nSlidingDir = eSLIDING_RIGHT;
				}
				else
				{
					if( m_nSlidingDir == eSLIDING_NONE )
					{
						m_lOriX = (LONG)GetAbsX() - m_IconBarWidth*m_nIconNum;
						m_lOriY = (LONG)GetAbsY();
						m_lSlidingX = m_IconBarWidth*m_nIconNum;
					}
					m_nSlidingDir = eSLIDING_LEFT;
					m_bFold = FALSE;
				}
				
			}
			else if( m_nSlidingDir == eSLIDING_LEFT )
			{
				if( m_nSlidingDir == eSLIDING_NONE )
				{
					m_lOriX = (LONG)GetAbsX();
					m_lOriY = (LONG)GetAbsY();
					m_lSlidingX = 0;	
				}
				m_nSlidingDir = eSLIDING_RIGHT;
			}
			else
			{
				if( m_nSlidingDir == eSLIDING_NONE )
				{
					m_lOriX = (LONG)GetAbsX() - m_IconBarWidth*m_nIconNum;
					m_lOriY = (LONG)GetAbsY();
					m_lSlidingX = m_IconBarWidth*m_nIconNum;
				}
				m_nSlidingDir = eSLIDING_LEFT;
			}
		}
	}
}

void CMainBarDialog::RepositionIconAbs(POSTYPE AtPos, CBarIcon * pIcon)
{
	pIcon->SetData(AtPos);
	WORD real_x = (WORD)(GetAbsX() + m_lOffSetX + (m_IconBarWidth*AtPos)+(m_IconBarSpace*(AtPos+1)));

	WORD real_y = (WORD)GetAbsY();
	pIcon->SetAbsXY(real_x, real_y);
}

void CMainBarDialog::RepositionIcon(int offset, CBarIcon * pIcon)
{
	POSTYPE AtPos = pIcon->GetData()+offset;
	RepositionIconAbs(AtPos, pIcon);
}

BOOL CMainBarDialog::AddIcon(POSTYPE AtPos, CBarIcon * pIcon)
{
	RepositionIconAbs(AtPos, pIcon);
	m_MainList.AddTail(pIcon);

	// ³ª¸ÓÁö°¡ ÀÖ´Ù¸é µÚ·Î ¹°¸®±â
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if(pIcon->GetData() == AtPos)
		{
		//	ASSERT(0);
		}
		else if(pIcon->GetData() > AtPos)
		{
			RepositionIcon(1, pIcon);
		}
		m_MainList.GetNext(pos);
	}

	return TRUE;
}

BOOL CMainBarDialog::RemoveIcon(POSTYPE AtPos, CBarIcon ** ppIcon)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if(pIcon->GetData() == AtPos)
		{
			m_MainList.RemoveAt(pos);
			*ppIcon = pIcon;
		}
		else if(pIcon->GetData() > AtPos)
		{
			RepositionIcon(-1, pIcon);
		}
		m_MainList.GetNext(pos);
	}

	return TRUE;
}

BOOL CMainBarDialog::MoveIcon(POSTYPE FromPos, POSTYPE ToPos)
{
	if( FromPos == ToPos)
	{
		ASSERT(0);
		return TRUE;
	}
	int offset = -1;
	POSTYPE lowPos = FromPos;
	POSTYPE HighPos = ToPos;
	if(FromPos > ToPos)
	{
		lowPos = ToPos;
		HighPos = FromPos;
		offset = 1;
	}

	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetAt(pos);
		if( pIcon->GetData() == FromPos )
		{
			//m_MainList.RemoveAt(pos);
			if(FromPos < ToPos)

				RepositionIconAbs(ToPos-1, pIcon);
			else
				RepositionIconAbs(ToPos, pIcon);
		}
		else
		{		
			if(FromPos < ToPos)
			{
				if( lowPos < pIcon->GetData() && pIcon->GetData() < HighPos )
				{
					RepositionIcon(offset, pIcon);
				}				
			}
			else
			{
				if( lowPos <= pIcon->GetData() && pIcon->GetData() < HighPos )
				{
					RepositionIcon(offset, pIcon);
				}				
			}

		}
		m_MainList.GetNext(pos);
		
	}

	return TRUE;
}

void CMainBarDialog::Render()
{
	if(!m_bActive) return;

	if( !m_bFold )
	{
		PTRLISTPOS pos = m_MainList.GetHeadPosition();

		while(pos)
		{
			CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);

			if( pIcon )
			if( pIcon->GetAbsX() + pIcon->GetWidth() < GetAbsX() + GetWidth() )
				pIcon->Render();			
		}
	}
	
	cDialog::Render();
}

void CMainBarDialog::PushMenuID( DWORD windowID, BOOL bPushed )
{
	int idx = -1;
	switch(windowID)
	{
	case GOLDSHOPMAINDLG:
		{
			idx = OPT_GOLDSHOPDLGICON;
		}
		break;
	case HI_DLG:
		{
			idx = OPT_HELPDLGICON;
		}
		break;
	case CI_CHARDLG:
		{
			idx = OPT_CHARACTERDLGICON;
		}
		break;
	case IN_INVENTORYDLG:
		{
			idx = OPT_INVENTORYDLGICON;
		}
		break;
	case MGI_MUGONGSURYUNDLG:
		{
			idx = OPT_MUGONGDLGICON;
		}
		break;
//	case QUESTDLG:
//		{
//		}
//		break;
	case MNM_DIALOG:
		{
			idx = OPT_MINIMAPDLGICON;
		}
		break;
//	case MUNPADLG:
//		{
//		}
//		break;
	case FRI_FRIENDDLG:
		{
			idx = OPT_FRIENDDLGICON;
		}
		break;
	case NOTE_NOTEDLG:
		{
			idx = OPT_NOTEDLGICON;
		}
		break;
	/*
		case MGI_MUGONGDLG:
				{
					idx = OPT_CLOSEALLDLGICON;
				}
				break;
				*/
	case OTI_TABDLG:
		{
			idx = OPT_OPTIONDLGICON;
		}
		break;	
	case EXT_DIALOG:
		{
			idx = OPT_EXITDLGICON;
		}
		break;
//	case XCI_DIALOG:
//		{
//			idx = OPT_EXCHANGEDLGICON;
//		}

//		break;
	/*
		case MGI_MUGONGDLG:
				{
					idx = OPT_PARTYDLGICON;
				}
				break;*/
	}


	if( idx != -1 )
	{
		CBarIcon * bar = GetBarIcon(idx);
		if( bar )
		{
//			bar->SetPush(bar->IsPushed() ^ TRUE);
			bar->SetPush( bPushed );
		}
	}
}

#include "MHCamera.h"

void CMainBarDialog::PushDirectMenu( WORD idx )
{
	// FreeCamera¸ðµå(¿Ö ÀÌ¸§À» eCM_EyeView¶ó°í ÇÑ°Ü..) ÀÏ¶§´Â Å°°¡ ¾È ¸Ôµµ·Ï ÇÑ´Ù.  by RaMa
	if( idx != -1 && CAMERA->GetCameraMode() != eCM_EyeView)
	{
		PushMenu( idx );
	}
}

CBarIcon * CMainBarDialog::GetBarIcon(WORD kind)
{
	PTRLISTPOS pos = m_MainList.GetHeadPosition();
	while(pos)
	{
		CBarIcon * pIcon = (CBarIcon * )m_MainList.GetNext(pos);
		if( pIcon->GetInfo()->kind == kind)

			return pIcon;
	}
	return NULL;
}

void CMainBarDialog::PushMenu( WORD idx )
{
	cDialog * pHandler = NULL;
	CBarIcon* pBarIcon = GetBarIcon( idx );
	if( !pBarIcon ) return;
	
	if( GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
		GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )

		return;
	switch(idx)
	{
	case OPT_GOLDSHOPDLGICON:
		{
			CGoldShopMainDailog * pDlg = GAMEIN->GetGoldShopMainDlg();
			if(pDlg)
				pDlg->SetActive(!pDlg->IsActive());
		}
		break;
	case OPT_HELPDLGICON:
		{
			cHelpDialog * pHandler = (cHelpDialog*)GAMEIN->GetHelpDialog();
			if(!pHandler) return;
			pHandler->SetActive(!pHandler->IsActive());
		}
		break;
	case OPT_CHARACTERDLGICON:
		{
			pHandler = GAMEIN->GetCharacterDialog();
			if( !pHandler )	return;

			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				((CCharacterDialog*)pHandler)->UpdateData();
				pHandler->SetActive( TRUE );	
				GAMEEVENTMGR->AddEvent( eGameEvent_CharInterface );
			}

			pBarIcon->SetPush( pHandler->IsActive() );
			GAMEIN->GetGuageDlg()->SetTopOnActive(TRUE);
			GAMEIN->GetGuageDlg()->SetFocus(TRUE);
			GAMEIN->GetGuageDlg()->SetActive(TRUE);
		}
		break;

	case OPT_INVENTORYDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetInventoryDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				pHandler->SetActive( TRUE );
				GAMEIN->UpdataGoldMoney();   //[ÔÚÏß³äÖµË¢ÐÂÔª±¦][By:Ê®ÀïÆÂ½£Éñ][QQ:112582793][2017/11/29]
				GAMEEVENTMGR->AddEvent( eGameEvent_InvenInterface );
			}

			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
		
	case OPT_MUGONGDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetMugongSuryunDialog();
			if( !pHandler ) return ;

			if( pHandler->IsActive() )
			{
				pHandler->SetActive( FALSE );
			}
			else 
			{
				pHandler->SetActive( TRUE );
				GAMEEVENTMGR->AddEvent( eGameEvent_MugongInterface );
			}

			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;

	case OPT_QUESTDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetQuestTotalDialog();
			if( !pHandler ) return ;

			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetQuestDialog()->CheckQuestDate();
				pHandler->SetActive(TRUE);
				GAMEEVENTMGR->AddEvent( eGameEvent_QuestInterface );
			}

			pBarIcon->SetPush( pBarIcon->IsActive() );	
		}
		break;
	case OPT_MINIMAPDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetMiniMapDialog();
			if( !pHandler ) return ;

			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				pHandler->SetActive(TRUE);

				GAMEEVENTMGR->AddEvent( eGameEvent_MapInterface );
			}
		}
		break;
	case OPT_GUILDDLGICON:
		{
			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
				return;

			pHandler =  (cDialog*)GAMEIN->GetGuildDlg();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				if(HERO->GetGuildIdx() != 0)
					pHandler->SetActive(TRUE);
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 35 ));

				GAMEEVENTMGR->AddEvent( eGameEvent_MunpaInterface );
			}
			pBarIcon->SetPush( !pBarIcon->IsPushed() );
		}
		break;
	case OPT_FRIENDDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetFriendDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				FRIENDMGR->FriendListSyn(1);

				GAMEEVENTMGR->AddEvent( eGameEvent_FriendInterface );
			}

			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;
		
	case OPT_NOTEDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetNoteDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				NOTEMGR->NoteListSyn(1);

				GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );
			}


			pBarIcon->SetPush( pHandler->IsActive() );
		}
		break;

	case OPT_OPTIONDLGICON:
		{
			pHandler = (cDialog*)GAMEIN->GetOptionDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				pHandler->SetActive(TRUE);

				GAMEEVENTMGR->AddEvent( eGameEvent_OptionInterface );
			}
		}
		break;

	case OPT_EXITDLGICON:
		{
			if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
				return;

			pHandler = (cDialog*)GAMEIN->GetExitDialog();
			if( !pHandler ) return ;
			if( pHandler->IsActive() )
			{
				pHandler->SetActive(FALSE);
			}
			else
			{
				if( !WINDOWMGR->GetWindowForID( MBI_EXIT ) )
				if( !EXITMGR->IsExitWaiting() )
					pHandler->SetActive(TRUE);

				GAMEEVENTMGR->AddEvent( eGameEvent_ExitInterface );
			}
		}
		break;
	case OPT_PKMODEICON:
		{
			if( !PKMGR->ToggleHeroPKMode() )
				pBarIcon->SetPush( !pBarIcon->IsPushed() );	
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
}


void CMainBarDialog::SetPushBarIcon( WORD idx, BOOL bPushed )
{
	CBarIcon* pBarIcon = GetBarIcon( idx );
	if( pBarIcon )
		pBarIcon->SetPush( bPushed );
}

void CMainBarDialog::Fold( BOOL bFold )
{
	if( bFold == m_bFold ) return;

	m_bFold = bFold;

	if( bFold )
	{
		m_lSlidingX = m_IconBarWidth*m_nIconNum;

		m_pBtnSize->SetRelXY( m_lOffSetX, 0 );
		SetAbsXY( m_lOriX + m_lSlidingX, m_lOriY );
		SetWH( m_lOffSetX + m_pBtnSize->GetWidth(), GetHeight() );
		m_nSlidingDir = eSLIDING_NONE;
				
		m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
		m_lOriY = (LONG)GetAbsY();
	}
	else
	{
		m_lSlidingX = 0;
		m_pBtnSize->SetRelXY( m_lOffSetX + m_IconBarWidth*m_nIconNum, 0 );
		SetAbsXY( m_lOriX, m_lOriY );
		SetWH( m_lOffSetX + m_pBtnSize->GetWidth() + m_IconBarWidth*m_nIconNum, GetHeight() );
		
		m_nSlidingDir = eSLIDING_NONE;
		
		m_lOriX = (LONG)GetAbsX() - m_lSlidingX;
		m_lOriY = (LONG)GetAbsY();
		
	}
}

void CMainBarDialog::SetAlram( int idx, BOOL bAlram )
{
	CBarIcon* pBarIcon = GetBarIcon( idx );
	pBarIcon->SetAlram( bAlram );
}




