







// GMToolManager.cpp: implementation of the CGMToolManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _GMTOOL_

#include "GMToolManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "CheatMsgParser.h"
#include "FilteringTable.h"
#include "ChatManager.h"

#include "MHMap.h"
#include "Commdlg.h"
#include "CommCtrl.h"
#include "MHFile.h"
#include "MugongManager.h"
#include "MainGame.h"
#include "WeatherManager.h"

extern HWND _g_hWnd;
extern HINSTANCE g_hInst;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CGMToolManager)

//extern HWND ghWnd;

CGMToolManager::CGMToolManager()
{
	m_hWndDlg = NULL;
	ZeroMemory( m_hWndSub, sizeof( m_hWndSub ) );

	m_nSelectMenu	= eMenu_Count;
	m_bShow			= FALSE;
	m_bCanUse		= FALSE;
	m_bSubShow		= FALSE;
	m_cbChannelCount	= 0;

	m_lBossStartIndex	= 0;
	m_hWndChatList		= NULL;

	m_nNumNow			= 0;
	m_nNumWait			= 0;
	m_nBufLen			= 0;

	m_bLogin			= FALSE;
	m_nPower			= eGM_POWER_MAX;	//3
	m_hWndLoginDlg		= NULL;
	m_nNullMove			= 0;
	m_CanSendCfg		= FALSE;
}

CGMToolManager::~CGMToolManager()
{

}

BOOL CGMToolManager::CreateGMDialog()
{
	if( m_hWndDlg ) return FALSE;

	m_hWndDlg = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GMDIALOG), _g_hWnd, GMDlgProc );	
	m_hWndLoginDlg = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GMLOGIN), _g_hWnd, GMLoginDlgProc );

	CreateGMSubDialog();

	return TRUE;
}

BOOL CGMToolManager::CreateGMSubDialog()
{
	m_hWndSub[eMenu_Move]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_MOVE_DIALOG), m_hWndDlg, GMSubMoveDlgProc );
	m_hWndSub[eMenu_Where]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_WHERE_DIALOG), m_hWndDlg, GMSubWhereDlgProc );
	m_hWndSub[eMenu_Item]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_ITEM_DIALOG), m_hWndDlg, GMSubItemDlgProc );
	m_hWndSub[eMenu_Hide]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_HIDE_DIALOG), m_hWndDlg, GMSubHideDlgProc );

	m_hWndSub[eMenu_Chat]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_CHAT_DIALOG), m_hWndDlg, GMSubChatDlgProc );
	m_hWndSub[eMenu_PK]		= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_PK_DIALOG), m_hWndDlg, GMSubPKDlgProc );
	m_hWndSub[eMenu_Discon] = CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_DISCON_DIALOG), m_hWndDlg, GMSubDisconDlgProc );
	m_hWndSub[eMenu_Block]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_BLOCK_DIALOG), m_hWndDlg, GMSubBlockDlgProc );
	
	m_hWndSub[eMenu_Mob]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_REGEN_DIALOG), m_hWndDlg, GMSubRegenDlgProc );
	m_hWndSub[eMenu_Counsel]= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_COUNSEL_DIALOG), m_hWndDlg, GMSubCounselDlgProc );
	m_hWndSub[eMenu_Notice]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_NOTICE_DIALOG), m_hWndDlg, GMSubNoticeDlgProc );

	m_hWndSub[eMenu_Event]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENT_DIALOG), m_hWndDlg, GMSubEventDlgProc );
	m_hWndSub[eMenu_EventMap]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENTMAP_DIALOG), m_hWndDlg, GMSubEventMapDlgProc );
	m_hWndSub[eMenu_EventNotify]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_EVENTNOTIFY_DIALOG), m_hWndDlg, GMSubEventNotifyDlgProc );
	
	m_hWndSub[eMenu_Weather]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_WEATHER_DIALOG), m_hWndDlg, GMSubWeatherDlgProc );
	m_hWndSub[eMenu_GameCfg]	= CreateDialog( g_hInst, MAKEINTRESOURCE(IDD_GM_GAMECFG), m_hWndDlg, GMSubGameCfgDlgProc );

	return TRUE;
}

BOOL CGMToolManager::DestroyGMDialog()
{
	if( !m_hWndDlg ) return FALSE;


	SaveChatList();

	m_hWndDlg = NULL;
	m_hWndLoginDlg = NULL;

	DestroyGMSubDialog();

	m_nSelectMenu = eMenu_Count;

	return TRUE;
}

BOOL CGMToolManager::DestroyGMSubDialog()
{
	for( int i = 0 ; i < eMenu_Count ; ++i )
	{
		if( m_hWndSub[i] )
		{
			DestroyWindow( m_hWndSub[i] );			
			m_hWndSub[i] = NULL;
		}
	}

	return TRUE;
}

void CGMToolManager::ShowGMDialog( BOOL bShow, BOOL bUpdate )
{
	if( !m_hWndDlg )		return;
	if( !m_bCanUse )		return;


	if( bShow && !m_bLogin && MAINGAME->GetUserLevel() == eUSERLEVEL_GM )
	{
		ShowWindow( m_hWndLoginDlg, SW_SHOW );

		return;
	}
	else if( MAINGAME->GetUserLevel() > eUSERLEVEL_GM )
	{
		return;
	}

	if( bShow )
	{
		ShowWindow( m_hWndDlg, SW_SHOWNA );
		SetPositionByMainWindow();
	}
	else
	{
		GMTOOLMGR->OnClickMenu( eMenu_Count );
		ShowWindow( m_hWndDlg, SW_HIDE );		
	}

	if( bUpdate )
		m_bShow = bShow;

	static int bRegist = FALSE;
	int i;
	char bufNum[20];
	if( !bRegist )
	{
		{
			ITEMMGR->SetItemIfoPositionHead();
			ITEM_INFO* pInfo = NULL;
			while( pInfo = ITEMMGR->GetItemInfoData() )
			{
				if( pInfo->ItemKind & eEQUIP_ITEM )
				{
					switch( pInfo->ItemKind )
					{
					case eEQUIP_ITEM_WEAPON:
					case eEQUIP_ITEM_UNIQUE:
						{
							if( pInfo->ItemGrade == 0 )
								SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					case eEQUIP_ITEM_DRESS:
					case eEQUIP_ITEM_HAT:
					case eEQUIP_ITEM_SHOES:
						{
							if( pInfo->ItemGrade == 0 )
								SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					case eEQUIP_ITEM_RING:
					case eEQUIP_ITEM_CAPE:
					case eEQUIP_ITEM_NECKLACE:
					case eEQUIP_ITEM_ARMLET:
					case eEQUIP_ITEM_BELT:
						{
							SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
						}
						break;
					}
				}
				else if( pInfo->ItemKind & eMUGONG_ITEM )
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLBOOK, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILL, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
				}
				else if( pInfo->ItemKind & eYOUNGYAK_ITEM )
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
				}
				else
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
				}
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLBOOK, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILL, CB_SETCURSEL, 0, 0 );

			for( i = 0 ; i <= 9 ; ++i )
			{
				wsprintf( bufNum, "+%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			for( i = 0 ; i <= 12 ; ++i )
			{
				wsprintf( bufNum, "%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_SETCURSEL, 0, 0 );
			SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_POTION, 1, TRUE );
			SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ETC, 1, TRUE );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_SPN_POTION, UDM_SETRANGE, 0, (LPARAM)MAKELONG(50, 1) );
			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_SPN_ETC, UDM_SETRANGE, 0, (LPARAM)MAKELONG(50, 1) );

			SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
		}

		{
			BASE_MONSTER_LIST* pList = NULL;
			for( i = 1 ; pList = GAMERESRCMNGR->GetMonsterListInfo( i ) ; ++i )
			{
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_ADDSTRING, 0, (LPARAM)pList->Name );
			}
			m_lBossStartIndex = i - 1;


			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_SETCURSEL, 0, 0 );
			ITEMMGR->SetItemIfoPositionHead();
			ITEM_INFO* pInfo = NULL;
			while( pInfo = ITEMMGR->GetItemInfoData() )
			{
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, CB_ADDSTRING, 0, (LPARAM)pInfo->ItemName );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, CB_SETCURSEL, 0, 0 );

			char* pStrMap = NULL;			
			for( i = 1 ; i < MAX_MAP_ENUMCOUNT ; ++i )
			{
				pStrMap = MAP->GetMapName(i);
				if( strlen(pStrMap)!=0 )
				{
					SendDlgItemMessage( m_hWndSub[eMenu_Mob],		IDC_GMREGEN_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Move],		IDC_GMMOVE_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Discon],	IDC_GMDISCON_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Chat],		IDC_GMCHAT_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_PK],		IDC_GMPK_CMB_MAP,		CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Notice],	IDC_GMNOTICE_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
					SendDlgItemMessage( m_hWndSub[eMenu_Weather],	IDC_GMWEATHER_CMB_MAP,	CB_ADDSTRING, 0, (LPARAM)pStrMap );
				}
			}

			SendDlgItemMessage( m_hWndSub[eMenu_Mob],		IDC_GMREGEN_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Move],		IDC_GMMOVE_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Discon],	IDC_GMDISCON_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat],		IDC_GMCHAT_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_PK],		IDC_GMPK_CMB_MAP,		CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Notice],	IDC_GMNOTICE_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			
			SendDlgItemMessage( m_hWndSub[eMenu_Weather],	IDC_GMWEATHER_CMB_MAP,	CB_SETCURSEL, 0, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Mob],	IDC_GMREGEN_CMB_CHANNEL,	CB_ADDSTRING, 0, (LPARAM)"全部" );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat],	IDC_GMCHAT_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"全部" );
			SendDlgItemMessage( m_hWndSub[eMenu_PK],	IDC_GMPK_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"全部" );
//			SendDlgItemMessage( m_hWndSub[eMenu_Weather],	IDC_GMWEATHER_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"全部" );
			SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,		CB_ADDSTRING, 0, (LPARAM)"全部" );

			for( i = 1 ; i <= m_cbChannelCount ; ++i )
			{
				wsprintf( bufNum, "%d", i );
				SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Chat], IDC_GMCHAT_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				
//				SendDlgItemMessage( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_CHANNEL, CB_ADDSTRING, 0, (LPARAM)bufNum );
				SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,	CB_ADDSTRING, 0, (LPARAM)bufNum );
			}
			SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Chat], IDC_GMCHAT_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
			SendDlgItemMessage( m_hWndSub[eMenu_Hide],	IDC_GMHIDE_CMB_CHANNEL,	CB_SETCURSEL, gChannelNum, 0 );
			//SendDlgItemMessage( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_CHANNEL, CB_SETCURSEL, gChannelNum, 0 );
		}

		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"经验倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"掉宝倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"掉钱倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"受到的伤害值" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"造成的伤害值" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"内力消耗" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"打坐回复速度" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"组队经验倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"技能点倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"掉钱数额倍率 " );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_ADDSTRING, 0, (LPARAM)"武功经验倍率" );
		SendDlgItemMessage( m_hWndSub[eMenu_Event],		IDC_GMEVENT_CMB_EVENT,	CB_SETCURSEL, 0, 0 );
		bRegist = TRUE;
	}
}

BOOL CGMToolManager::IsGMDialogMessage( LPMSG pMessage)
{
	if( IsWindow( m_hWndDlg ) && IsDialogMessage( m_hWndDlg, pMessage ) )
		return TRUE;

	if( m_nSelectMenu < eMenu_Count )
	if( m_hWndSub[m_nSelectMenu] )
	{
		if( IsWindow( m_hWndSub[m_nSelectMenu] ) && IsDialogMessage( m_hWndSub[m_nSelectMenu], pMessage ) )
			return TRUE;
	}

	return FALSE;
}

void CGMToolManager::SetPositionByMainWindow()
{
	RECT rcMain, rcDlg;
	GetWindowRect( _g_hWnd, &rcMain );
	GetWindowRect( m_hWndDlg, &rcDlg );
	int nDlgWidth = rcDlg.right - rcDlg.left;
	int X = rcMain.left - nDlgWidth;
	if( X < 0 ) X = rcMain.left;
	int Y = rcMain.top+GetSystemMetrics(SM_CYSIZEFRAME)+GetSystemMetrics(SM_CYCAPTION);
	
	SetWindowPos( m_hWndDlg, NULL, X, Y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetPositionSubDlg();
}

void CGMToolManager::SetPositionSubDlg()
{
	RECT rcDlg;
	GetWindowRect( m_hWndDlg, &rcDlg );

	if( m_nSelectMenu < eMenu_Count )
	{
		if( m_hWndSub[m_nSelectMenu] )
		{
			int subX = rcDlg.right;
			int subY = rcDlg.top + 36 + m_nSelectMenu * 28;

			SetWindowPos( m_hWndSub[m_nSelectMenu], NULL, subX, subY, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
		}
	}
}

void CGMToolManager::OnClickMenu( int nMenu )
{
	if( nMenu >= eMenu_Count )
	{
		if( m_nSelectMenu < eMenu_Count )
		{
			if( m_hWndSub[m_nSelectMenu] )
				ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );

			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );			
		}

		m_nSelectMenu = eMenu_Count;
//		CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_NONE, BST_CHECKED );

		m_bSubShow = FALSE;
		SetFocus( _g_hWnd );
	}
	else
	{
		if( m_nSelectMenu == nMenu )
		{
			if( m_hWndSub[m_nSelectMenu] )
				ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );

			m_nSelectMenu = eMenu_Count;
//			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_NONE, BST_CHECKED );

			m_bSubShow = FALSE;
			SetFocus( _g_hWnd );
		}
		else
		{
#ifdef _GMTOOL_
#ifdef _GMTOOL_SIMPLE_
			if(nMenu == 2 || nMenu == 5 || nMenu == 8 || nMenu == 11 || nMenu == 12 || nMenu == 13 || nMenu == 14)
			{
				return;
			}
#endif
#endif
////////////////////////////////////////////////////////////////////////////////////////
			ShowWindow( m_hWndSub[m_nSelectMenu], SW_HIDE );
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_UNCHECKED );
			
			m_nSelectMenu = nMenu;
			SetPositionSubDlg();
			CheckDlgButton( m_hWndDlg, IDC_GM_MNBTN_MOVE + m_nSelectMenu, BST_CHECKED );

			if( m_hWndSub[m_nSelectMenu] )
				AnimateWindow( m_hWndSub[m_nSelectMenu], 200, AW_SLIDE | AW_HOR_POSITIVE );

			m_bSubShow = TRUE;
		}
	}	
}

BOOL CGMToolManager::OnGMMoveCommand( int nMethod )
{
	if( !HERO ) return FALSE;
	if( !m_hWndSub[eMenu_Move] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nMethod == 0 )
	{
		BOOL	bRt;
		VECTOR3 pos;
		pos.x = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_X, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;
		pos.y = 0;
		pos.z = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_Y, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;

		MOVE_POS msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVE_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwMoverID	= HEROID;
		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( nMethod == 1 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_NAME, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH+1 ) == 0  )	
			return FALSE;

		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 0;		
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));
	}
	else if( nMethod == 2)
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH ) return FALSE;
		WORD wMapNum = MAP->GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;

		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;

		if( m_cbChannelCount > 0 && nChannel > 0 && nChannel <= m_cbChannelCount )
		{
			gChannelNum = nChannel - 1; 
		}

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)wMapNum;
		msg.dwData2		= (DWORD)gChannelNum;
		msg.Name[0]		= 0;	
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnUserMoveCommand( int nMethod )
{
	if( !m_hWndSub[eMenu_Move] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nMethod == 0 )
	{
		BOOL	bRt;
		VECTOR3 pos;
		pos.x = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_X, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;
		pos.y = 0;
		pos.z = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_Y, &bRt, TRUE ) * 100.0f;
		if( !bRt ) return FALSE;

		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4  ) return FALSE;

		if( strncmp( buf, "[炔锋]", 6 ) == 0 ||
			strncmp( buf, "[没锋]", 6 ) == 0 )
		{
		}
		else
			if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
				return FALSE;

		MOVE_POS_USER msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVEUSER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		msg.cpos.Compress(&pos);
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( nMethod == 1 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;		

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )	
			return FALSE;

		if( strncmp( buf, "[炔锋]", 6 ) == 0 ||
			strncmp( buf, "[没锋]", 6 ) == 0 )
		{
		}
		else
			if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
				return FALSE;
		
		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MOVETOCHAR_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= 1;	
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg, sizeof(msg));

	}
	else if( nMethod == 2 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen;
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH ) return FALSE;
		WORD wMapNum = MAP->GetMapNumForName( buf );
		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Move], IDC_GMMOVE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;
		if( m_cbChannelCount > 0 && nChannel > 0 && nChannel <= m_cbChannelCount )
		{
			gChannelNum = nChannel - 1;
		}
		
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMMOVE_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		MSG_NAME_DWORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_CHANGEMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= (DWORD)wMapNum;
		msg.dwData2		= (DWORD)gChannelNum;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnItemCommand()
{
	if( !m_hWndSub[eMenu_Item] ) return FALSE;
	SetFocus( _g_hWnd );

	int nResult = 0;
	WORD wCount = 1;
	char buf[64];
	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_WEAPON ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPON, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_WEAPONGRADE, CB_GETCURSEL, 0, 0 );
		if( nSel > 0 && nSel <= 9 )
			wsprintf( buf, "%s+%d", buf, nSel );

		nResult = 1;
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_CLOTHES ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHES, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_CLOTHESGRADE, CB_GETCURSEL, 0, 0 );
		if( nSel > 0 && nSel <= 9 )
			wsprintf( buf, "%s+%d", buf, nSel );
		nResult = 1;
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_ACCESSORY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ACCESSORY, buf, MAX_ITEMNAME_LENGTH+1 );
		nResult = 1;
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_SKILLBOOK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLBOOK, buf, MAX_ITEMNAME_LENGTH+1 );
		nResult = 1;
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_POTION ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_POTION, buf, MAX_ITEMNAME_LENGTH+1 );
		BOOL rt;
		int nCount = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_POTION, &rt, TRUE );
		if( rt )
		{
			if( nCount < 0 )			nCount = 0;
			//else if( nCount > 50 )		nCount = 50;

			wCount = (WORD)nCount;
			nResult = 1;
		}
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_ETC ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_ETC, buf, MAX_ITEMNAME_LENGTH+1 );
		BOOL rt;
		int nCount = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ETC, &rt, TRUE );
		if( rt )
		{
			if( nCount < 0 )			nCount = 0;
			//else if( nCount > 50 )		nCount = 50;

			wCount = (WORD)nCount;
			nResult = 1;
		}
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_MONEY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		DWORD dwMoney = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_MONEY, &rt, TRUE );
		if( !rt )	return FALSE;

		SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_MONEY, 0, TRUE );

		MSG_NAME_DWORD4 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_MONEY_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= dwMoney;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;
		NETWORK->Send( &msg, sizeof(msg) );		
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_SKILL ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		GetDlgItemText( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILL, buf, MAX_ITEMNAME_LENGTH+1 );
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG, CB_GETCURSEL, 0, 0 );

		if( nSel < 0 )			nSel = 0;
		else if( nSel > 12 )	nSel = 12;

		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;

		CMugongBase* pBase = MUGONGMGR->GetMugongByMugongIdx( pInfo->MugongNum );
		if( pBase == NULL )
		{
			MSG_NAME_DWORD4 msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_ADDMUGONG_SYN;
			msg.dwObjectID	= HEROID;
			msg.dwData1		= pInfo->MugongNum;
			msg.dwData2		= pInfo->ItemKind;
			msg.dwData3		= nSel;
			if(m_GetMode==eGetToCharacter)
				sprintf(msg.Name,m_SelectName);
			msg.dwData4 = m_GetMode;

			NETWORK->Send(&msg,sizeof(msg));
			EnableWindow( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_CMB_SKILLSUNG ), TRUE );
		}
		else
		{
			if( pBase->GetSung() != nSel )
			{
				MSG_WORD2 msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_MUGONGSUNG_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pInfo->MugongNum;
				msg.wData2 = nSel;
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_ABILITY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		int nAbil = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ABILITY, &rt, TRUE );

		if( nAbil <= 0 ) return FALSE;
		if( nAbil > 1000000000 ) nAbil = 1000000000;	

		SetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_ABILITY, 0, TRUE );

		MSG_NAME_DWORD4 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_ABILITYEXP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = (DWORD)nAbil;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_GOLD ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		int nGoldMoney = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_GOLD, &rt, TRUE );
		if(nGoldMoney<=0) return FALSE;
		if(nGoldMoney>=MAX_INVENTORY_GOLD) nGoldMoney = MAX_INVENTORY_GOLD;

		MSG_NAME_DWORD4 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GMGETGOLD_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = (DWORD)nGoldMoney;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_PD ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		int nMallMoney = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_PD, &rt, TRUE );
		if(nMallMoney<=0) return FALSE;
		if(nMallMoney>=MAX_INVENTORY_GOLD) nMallMoney = MAX_INVENTORY_GOLD;

		MSG_NAME_DWORD4 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GMGETMALL_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = (DWORD)nMallMoney;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_SW ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		BOOL rt;
		int nFame = GetDlgItemInt( m_hWndSub[eMenu_Item], IDC_GMITEM_EDT_SW, &rt, TRUE );
		if(nFame<=0) return FALSE;
		if(nFame>=50000) nFame = 50000;

		MSG_NAME_DWORD4 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_GMGETFAME_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = (DWORD)nFame;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;
		NETWORK->Send(&msg,sizeof(msg));
	}

	if( nResult == 1 )
	{
		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;

		MSG_NAME_DWORD4 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_ITEM_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= pInfo->ItemIdx;
		msg.dwData2		= wCount;
		if(m_GetMode==eGetToCharacter)
			sprintf(msg.Name,m_SelectName);
		msg.dwData3 = m_GetMode;

		NETWORK->Send(&msg,sizeof(msg));
	}
	return TRUE;
}

BOOL CGMToolManager::OnDisconCommand()
{
	if( !m_hWndSub[eMenu_Block] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = { 0, };

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Discon], IDC_GMDISCON_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Discon], IDC_GMDISCON_EDT_CHAR, buf, MAX_NAME_LENGTH+1 );
		if( nLen < 4 || nLen > MAX_NAME_LENGTH )
			return FALSE;

		if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )
			return FALSE;
		
		if( strncmp( buf, "[炔锋]", 6 ) == 0 ||
			strncmp( buf, "[没锋]", 6 ) == 0 )
		{
		}
		else
			if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
				return FALSE;

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send( &msg,sizeof(msg) );
	}
	else	//map傈眉
	{
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Discon], IDC_GMDISCON_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH )
			return FALSE;
		
		WORD wMapNum = MAP->GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;

		WORD wExceptSelf = 0;
		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Discon], IDC_GMDISCON_BTN_EXCEPTSELF ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
			wExceptSelf = 1;
		
			MSG_WORD2 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANMAP_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1		= wMapNum;
		msg.wData2		= wExceptSelf;
		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

BOOL CGMToolManager::OnBlockCommand()
{
	if( !HERO ) return FALSE;
	if( !m_hWndSub[eMenu_Block] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = { 0, };
	int nLen = GetDlgItemText( m_hWndSub[eMenu_Block], IDC_GMBLOCK_EDT_CHAR, buf, MAX_NAME_LENGTH+1 );
	if( strncmp( HERO->GetObjectName(), buf, MAX_NAME_LENGTH ) == 0  )
		return FALSE;
	if( nLen < 4 || nLen > MAX_NAME_LENGTH ) return FALSE;

	if( strncmp( buf, "[炔锋]", 6 ) == 0 ||
		strncmp( buf, "[没锋]", 6 ) == 0 )
	{
	}
	else
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			return FALSE;

	BOOL bBlock;


	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Block], IDC_GMBLOCK_BTN_BLOCK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		bBlock = TRUE;
	else
		bBlock = FALSE;

	MSG_NAME_DWORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_BLOCKCHARACTER_SYN;
	msg.dwObjectID	= HEROID;
	msg.dwData		= (DWORD)bBlock;
	SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
	NETWORK->Send(&msg,sizeof(msg));

	if( bBlock )
	{	//disconnect

		MSG_NAME msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_BANCHARACTER_SYN;
		msg.dwObjectID	= HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH + 1 );
		NETWORK->Send(&msg,sizeof(msg));
		
		return TRUE;
	}

	return TRUE;
}
BOOL CGMToolManager::OnWhereSelectObj()
{
	CObject* pObject = OBJECTMGR->GetSelectedObject();
	if (pObject)
	{
		SetDlgItemText(m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_USER, pObject->GetObjectName());
		return TRUE;
	}
	return FALSE;
}
BOOL CGMToolManager::OnWhereCommand()
{
	if( !m_hWndSub[eMenu_Where] ) return FALSE;
	SetFocus( _g_hWnd );

	MSG_NAME msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
	msg.dwObjectID	= HEROID;

	int nLen = GetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_USER, msg.Name, MAX_NAME_LENGTH+1 );

	if( nLen < 4 || nLen > MAX_NAME_LENGTH ) return FALSE;

	msg.Name[MAX_NAME_LENGTH] = 0;
	
	if( strncmp( msg.Name, "[炔锋]", 6 ) == 0 ||
		strncmp( msg.Name, "[没锋]", 6 ) == 0 )
	{
	}
	else
		if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)msg.Name)) == TRUE )
		{
			return TRUE;
		}
	NETWORK->Send( &msg, sizeof(msg) );
	
	SetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_MAP, "" );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_CHANNEL, 0, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_X, 0, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_Y, 0, TRUE );
	SetDlgItemText(m_hWndSub[eMenu_Where], IDC_EDIT_LOCALINFO, "");
	return TRUE;
}

void CGMToolManager::DisplayWhereInfo( char* MapName, int nChannel, int nX, int nY )
{
	if( !m_hWndSub[eMenu_Where] ) return;
	SetFocus( _g_hWnd );

	SetDlgItemText( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_MAP, MapName );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_CHANNEL, nChannel, FALSE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_X, nX, FALSE );
	SetDlgItemInt( m_hWndSub[eMenu_Where], IDC_GMWHERE_EDT_Y, nY, FALSE );
}
void CGMToolManager::DisplayLocalInfo(char* LocalInfo)
{
	if (!m_hWndSub[eMenu_Where]) return;
	SetFocus(_g_hWnd);
	SetDlgItemText(m_hWndSub[eMenu_Where], IDC_EDIT_LOCALINFO, LocalInfo);
}
BOOL CGMToolManager::OnHideCommand()
{
	if( !m_hWndSub[eMenu_Hide] ) return FALSE;

	SetFocus( _g_hWnd );

	int  nLen	= 0;
	BOOL bHide = TRUE;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_HIDE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		bHide = TRUE;
	else
		bHide = FALSE;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_NPC ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		CObject* pObject = OBJECTMGR->GetSelectedObject();
		if( !pObject )	return FALSE;
		if( pObject->GetObjectKind() != eObjectKind_Npc )	return FALSE;

		MSG_WORD3 msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_NPCHIDE_SYN;
		msg.dwObjectID	= HEROID;
		msg.wData1 = ((CNpc*)pObject)->GetNpcUniqueIdx();
		msg.wData2 = (WORD)( SendDlgItemMessage( m_hWndSub[eMenu_Hide], IDC_GMHIDE_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) );
		msg.wData3 = !bHide;

		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;
	}
	else
	{
		MSG_NAME_DWORD msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_HIDE_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData		= (DWORD)bHide;

		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Hide], IDC_GMHIDE_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )	
		{
			nLen = GetDlgItemText( m_hWndSub[eMenu_Hide], IDC_GMHIDE_EDT_USER, msg.Name, MAX_NAME_LENGTH+1 );
			if( nLen < 4 || nLen > MAX_NAME_LENGTH )
				return FALSE;
		}
		else
		{
			msg.Name[0] = 0;
		}

		msg.Name[MAX_NAME_LENGTH] = 0;
		NETWORK->Send( &msg, sizeof(msg) );

		return TRUE;
	}
}

BOOL CGMToolManager::OnChatCommand()
{
	if( !m_hWndSub[eMenu_Chat] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = {0,};
	int nLen	= 0;
	BOOL bAllow = TRUE;
	int nTime	= 0;
	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Move], IDC_GMCHAT_BTN_USER ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		nLen = GetDlgItemText( m_hWndSub[eMenu_Move], IDC_GMCHAT_EDT_USER, buf, MAX_NAME_LENGTH+1 );
		if( nLen > MAX_NAME_LENGTH || nLen < 4 ) return FALSE;

		if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Move], IDC_GMCHAT_BTN_NOCHAT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		{
			bAllow = FALSE;
			BOOL bRt = FALSE;
//			nTime = GetDlgItemInt( m_hWndSub[eMenu_Move], IDC_GMCHAT_EDT_TIME, &bRt, TRUE );
			if( !bRt ) return FALSE;
		}
		else
		{
			bAllow = TRUE;
		}
	}
	else
	{

	}

	return TRUE;
}


BOOL CGMToolManager::OnPKCommand()
{
	if( !m_hWndSub[eMenu_PK] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_NAME_LENGTH+1] = {0,};
	int nLen = GetDlgItemText( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
	if( nLen > MAX_NAME_LENGTH ) return FALSE;
	// magi82(37) 甘 己 单捞磐
	WORD wMapNum = MAP->GetMapNumForName( buf );
	if( wMapNum == 0 ) return FALSE;

	int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_PK], IDC_GMPK_CMB_CHANNEL, CB_GETCURSEL, 0, 0 ) + 1;

	MSG_WORD2 msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_PKALLOW_SYN;
	msg.dwObjectID	= HEROID;
	msg.wData1		= wMapNum;

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_PK], IDC_GMPK_BTN_NOPK ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		msg.wData2	= 0;		//not allow
	}
	else
	{
		msg.wData2	= 1;		//allow		
	}

	NETWORK->Send(&msg,sizeof(msg));	

	return TRUE;
}

BOOL CGMToolManager::OnRegenCommand()
{
	if( !m_hWndSub[eMenu_Mob] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_ITEMNAME_LENGTH+1] = {0,};

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_MODEDIRECT ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
//mob
		int nSel = SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MOBNAME, CB_GETCURSEL, 0, 0 );
//		BYTE bBoss = 0;
		BASE_MONSTER_LIST* pList = NULL;
		if( nSel < m_lBossStartIndex )
		{
			pList = GAMERESRCMNGR->GetMonsterListInfo( nSel + 1 );
		}
		else
		{
			pList = GAMERESRCMNGR->GetMonsterListInfo( 1000 + nSel - m_lBossStartIndex );
//			bBoss = 1;
		}

		if( pList == NULL ) return FALSE;
//mob count
		BOOL rt;
		int nMobCount = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_MOBNUM, &rt, TRUE );
		if( !rt ) return FALSE;
		if( nMobCount <= 0 ) return FALSE;
		if( nMobCount > 100 ) return FALSE;
//map
		int nLen = GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_MAP, buf, MAX_NAME_LENGTH+1 );
		if( nLen == 0 ) return FALSE;
		buf[MAX_NAME_LENGTH] = 0;
		WORD wMapNum = MAP->GetMapNumForName( buf );
		if( wMapNum == 0 ) return FALSE;
//channel
		int nChannel = SendDlgItemMessage( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_CHANNEL, CB_GETCURSEL, 0, 0 );
		if( nChannel < 0 ) return FALSE;
		if( nChannel > m_cbChannelCount ) return FALSE;		
//pos & radius
		int nX = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_X, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;
		int nY = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_Y, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;
		int nRad = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_RAD, &rt, TRUE ) * 100;
		if( !rt ) return FALSE;

		if( nX < 0 ) nX = 0;	
		if( nY < 0 ) nY = 0;	
		if( nRad < 0 ) nRad = 0;
		else if( nRad > 10000 ) nRad = 10000;	

		VECTOR3 vPos = { (float)nX, 0.0f, (float)nY };
//Item
		nLen = GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_CMB_ITEM, buf, MAX_ITEMNAME_LENGTH+1 );
		if( nLen == 0 ) return FALSE;
		buf[MAX_ITEMNAME_LENGTH] = 0;
		ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
		if( pInfo == NULL ) return FALSE;
//Drop Ratio
		int nDropRatio = GetDlgItemInt( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_ITEM, &rt, TRUE );
		if( !rt ) return FALSE;
		if( nDropRatio < 0 )	return FALSE;
		if( nDropRatio > 100 )	return FALSE;

		MSG_EVENT_MONSTERREGEN msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
		msg.dwObjectID	= HEROID;
		msg.MonsterKind = pList->MonsterKind;
		msg.cbMobCount	= nMobCount;
		msg.wMap		= wMapNum;
		msg.cbChannel	= (BYTE)nChannel;
		msg.Pos			= vPos;
		msg.wRadius		= (WORD)nRad;
		msg.ItemID		= pInfo->ItemIdx;
		msg.dwDropRatio	= (DWORD)nDropRatio;
//		msg.bBoss		= bBoss;

		NETWORK->Send( &msg,sizeof(msg) );
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_MODEFILE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		char lpstrFile[MAX_PATH] = {0,};
		GetDlgItemText( m_hWndSub[eMenu_Mob], IDC_GMREGEN_EDT_FILE, lpstrFile, MAX_PATH );
	
		if( lpstrFile[0] == 0 )
			return FALSE;
		
		CMHFile fp;		
		if( !fp.Init( lpstrFile, "rt" ) )
			return FALSE;
		char buff[256]={0,};

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
			if( strcmp( buff, "$REGEN" ) == 0 )
			{
				DWORD dwGroup = fp.GetDword();
				if( dwGroup == 0 )
				{
					fp.Release();
					return FALSE;
				}
				if((fp.GetString())[0] == '{')
				{
					MSG_EVENT_MONSTERREGEN msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_EVENT_MONSTER_REGEN;
					msg.dwObjectID	= HEROID;
					msg.MonsterKind = GAMERESRCMNGR->GetMonsterIndexForName( fp.GetString() );
					msg.cbMobCount	= fp.GetByte();
					// magi82(37) 甘 己 单捞磐
					msg.wMap		= MAP->GetMapNumForName( fp.GetString() );
					msg.cbChannel	= fp.GetByte();
					msg.Pos.x		= (float)fp.GetWord() * 100.0f;
					msg.Pos.z		= (float)fp.GetWord() * 100.0f;
					msg.wRadius		= fp.GetWord()*100;
					ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( fp.GetString() );
					if( pInfo )
						msg.ItemID		= pInfo->ItemIdx;
					else
						msg.ItemID		= 0;
					msg.dwDropRatio	= fp.GetDword();

					if( msg.MonsterKind == 0 || msg.wMap == 0 || msg.cbChannel > m_cbChannelCount
						|| ( msg.ItemID == 0 && msg.dwDropRatio != 0 ) )
					{

						ASSERT(0);
					}
					else
					{
						NETWORK->Send( &msg,sizeof(msg) );
					}					
				}
				else
				{
					fp.Release();
					return FALSE;
				}

				if((fp.GetString())[0] != '}')
				{
					fp.Release();
					return FALSE;
				}
			}
		}
		fp.Release();		
	}
	else if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Mob], IDC_GMREGEN_BTN_DELETE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENT_MONSTER_DELETE;
		msg.dwObjectID	= HEROID;

		NETWORK->Send( &msg, sizeof(msg) );
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

#define MAX_PREFACE_LENGTH	30

BOOL CGMToolManager::OnNoticeCommand()
{
	if( !m_hWndSub[eMenu_Notice] ) return FALSE;
	SetFocus( _g_hWnd );

	char buf[MAX_CHAT_LENGTH+1] = {0,};
	char bufMap[MAX_NAME_LENGTH+1] = {0,};

	int nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_PREFACE ), buf, MAX_PREFACE_LENGTH+1 );
	int nLen2 = GetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ), buf+nLen, MAX_CHAT_LENGTH-MAX_PREFACE_LENGTH );
	
	if( nLen2 == 0 ) return FALSE;

	MSG_CHAT_WORD msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_NOTICE_SYN;
	msg.dwObjectID	= HEROID;
	SafeStrCpy( msg.Msg, buf, MAX_CHAT_LENGTH+1 );

	if( SendMessage( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_BTN_ALLMAP ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		msg.wData = 0;
	}
	else
	{
		GetDlgItemText( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_CMB_MAP, bufMap, MAX_NAME_LENGTH+1 );
		msg.wData = MAP->GetMapNumForName(bufMap);
	}

	NETWORK->Send( &msg, msg.GetMsgLength() );

	SetWindowText( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ), "" );
	SetFocus( GetDlgItem( m_hWndSub[eMenu_Notice], IDC_GMNOTICE_EDT_NOTICE ) );

	return TRUE;
}

BOOL CGMToolManager::OnEventCommand()
{
	if( !m_hWndSub[eMenu_Notice] ) return FALSE;
	SetFocus( _g_hWnd );


	//沥焊 佬绢 坷扁
	int nEvent = SendDlgItemMessage( m_hWndSub[eMenu_Event], IDC_GMEVENT_CMB_EVENT, CB_GETCURSEL, 0, 0 )+1;
	BOOL rt;
	int nRatio = GetDlgItemInt( m_hWndSub[eMenu_Event], IDC_GMEVENT_EDT_RATE, &rt, TRUE );

	MSG_WORD2 msg;
	msg.Category	= MP_CHEAT;
	msg.Protocol	= MP_CHEAT_EVENT_SYN;
	msg.dwObjectID	= HEROID;
	msg.wData1		= nEvent;
	msg.wData2		= nRatio;
	NETWORK->Send( &msg, sizeof(msg) );
	CHATMGR->AddMsg( CTC_TOGM, "事件开启" );

	return TRUE;
}


BOOL CGMToolManager::OnEventMapCommand( int nKind )
{
	if( !m_hWndSub[eMenu_EventMap] ) return FALSE;
	SetFocus( _g_hWnd );

	if( nKind == 1 )
	{
		char buf[MAX_NAME_LENGTH+1] = {0,};
		int nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHAR ), buf, MAX_NAME_LENGTH+1 );

		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "请输入角色名" );
			return FALSE;
		}
		BOOL rt;
		int nChannel	= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHANNEL, &rt, TRUE );
		if( nChannel <= 0 || nChannel > 10 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "频道号无效" );
			return FALSE;
		}
		int nTeam		= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_TEAM, &rt, TRUE );
		if( nTeam != 1 && nTeam != 2 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "队伍编号无效" );
			return FALSE;
		}
		
		MSG_NAME_DWORD3 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_CHANGE_EVENTMAP_SYN;
		msg.dwObjectID = HEROID;
		SafeStrCpy( msg.Name, buf, MAX_NAME_LENGTH+1 );
		msg.dwData1 = 58;
		msg.dwData2 = nChannel;
		msg.dwData3 = nTeam;

		NETWORK->Send( &msg, sizeof(msg) );
		
		return TRUE;		
	}
	else if( nKind == 2 )
	{
		BOOL rt;
		int nChannel	= GetDlgItemInt( m_hWndSub[eMenu_EventMap], IDC_GMEVENTMAP_EDT_CHANNEL2, &rt, TRUE );
		if( nChannel <= 0 || nChannel > 10 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "无效的频道" );
			return FALSE;
		}

		MSG_DWORD2 msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENT_READY_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = 58;
		msg.dwData2 = nChannel;
		
		NETWORK->Send( &msg, sizeof(msg) );
		return TRUE;		
	}

	return FALSE;
}

BOOL CGMToolManager::OnEventNotifyCommand( BOOL bOn )
{
	if( !m_hWndSub[eMenu_EventNotify] ) return FALSE;
	SetFocus( _g_hWnd );

	if( bOn )
	{
		char bufTitle[32] = {0,};
		char bufContext[128] = {0,};
		int nLen = 0;

		nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventNotify], IDC_GMEVENTNOTIFY_EDT_TITLE ), bufTitle, 32 );
		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "请输入标题" );
			return FALSE;
		}
		nLen = GetWindowText( GetDlgItem( m_hWndSub[eMenu_EventNotify], IDC_GMEVENTNOTIFY_EDT_CONTEXT ), bufContext, 128 );
		if( nLen <=0 )
		{
			CHATMGR->AddMsg( CTC_TOGM, "请输入公告内容" );
			return FALSE;
		}

		if( !m_hWndSub[eMenu_Notice] ) return FALSE;
		SetFocus( _g_hWnd );
		//沥焊 佬绢 坷扁
		int nEvent = SendDlgItemMessage( m_hWndSub[eMenu_Event], IDC_GMEVENT_CMB_EVENT, CB_GETCURSEL, 0, 0 )+1;
	
		bufTitle[31] = 0;
		bufContext[127] = 0;

		MSG_EVENTNOTIFY_ON msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFY_ON;
		msg.dwObjectID = HEROID;
		memset( msg.EventList, 0, sizeof(BYTE)*eEvent_Max );
		// magi82(43)
		// msg.EventList[nEvent] = 1;
		SafeStrCpy( msg.strTitle, bufTitle, 32 );
		SafeStrCpy( msg.strContext, bufContext, 128 );

		NETWORK->Send( &msg, sizeof(msg) );
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_CHEAT;
		msg.Protocol = MP_CHEAT_EVENTNOTIFY_OFF;
		msg.dwObjectID = HEROID;

		NETWORK->Send( &msg, sizeof(msg) );
	}

	return TRUE;
}

void CGMToolManager::Login( BOOL bLogin, int nGMPower )
{
	static char power_str[5][16] = { "Master", "Monitor", "Patroller", "Auditor", "Eventer" };

	char buf[128];
	if( bLogin )
	{
		wsprintf( buf, "< GM Power : %s >", power_str[nGMPower] );
		MessageBox( m_hWndLoginDlg, buf, "登陆成功", MB_OK );
		m_bLogin = TRUE;
		m_nPower = nGMPower;
		ShowWindow( m_hWndLoginDlg, SW_HIDE );
	}
	else
	{
		if( nGMPower == 1)		//nGMPower = error code : if bLogin == FALSE
		{
			MessageBox( m_hWndLoginDlg, "无效的帐号密码", "登陆失败", MB_OK );

		}
		else
		{
			MessageBox( m_hWndLoginDlg, "错误", "登陆失败", MB_OK );
		}		
	}
}

INT_PTR CALLBACK GMLoginDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
			RECT rcGame;
			RECT rcDlg;
			POINT pt;
			GetClientRect( hWnd, &rcDlg );
			GetClientRect( _g_hWnd, &rcGame );
			pt.x = rcGame.left;
			pt.y = rcGame.top;
			ClientToScreen( _g_hWnd, &pt );

			int x = ( ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth - ( rcDlg.right - rcDlg.left ) ) / 2 ) + pt.x;
			int y = ( ( (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight - ( rcDlg.bottom - rcDlg.top ) ) / 2 ) + pt.y;

			SetWindowPos( hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );

			SendMessage( GetDlgItem( hWnd, IDC_GMLOGIN_EDT_ID ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMLOGIN_EDT_PW ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
		}
		return TRUE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				if( LOWORD( wParam ) == IDC_GMLOGIN_BTN_CONNECT )
				{
					char strID[MAX_NAME_LENGTH+1];
					char strPW[MAX_NAME_LENGTH+1];
					GetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), strID, MAX_NAME_LENGTH+1 );
					GetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), strPW, MAX_NAME_LENGTH+1 );
					
					if( strID[0] == 0 || strPW[0] == 0 )
					{
						MessageBox( hWnd, "Input ID/PW", "Error", MB_OK );
						return TRUE;
					}
				
					MSG_NAME2 msg;
					msg.Category = MP_CHEAT;
					msg.Protocol = MP_CHEAT_GM_LOGIN_SYN;
					msg.dwObjectID = HEROID;
					SafeStrCpy( msg.str1, strID, MAX_NAME_LENGTH+1 );
					SafeStrCpy( msg.str2, strPW, MAX_NAME_LENGTH+1 );

					NETWORK->Send( &msg, sizeof(msg) );

					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), "" );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), "" );
				}
				else if( LOWORD( wParam ) == IDC_GMLOGIN_BTN_CANCEL )
				{
					ShowWindow( hWnd, SW_HIDE );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_ID), "" );
					SetWindowText( GetDlgItem(hWnd, IDC_GMLOGIN_EDT_PW), "" );
				}

			}
		}
		return TRUE;
	}
    return FALSE;
}


INT_PTR CALLBACK GMDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_INITDIALOG:
		{
//			CheckDlgButton( hWnd, IDC_GM_MNBTN_NONE, BST_CHECKED );
		}
		return FALSE;	

	case WM_NCHITTEST:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:

		{
			SetFocus( _g_hWnd );
		}
		break;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				GMTOOLMGR->OnClickMenu( LOWORD( wParam ) - IDC_GM_MNBTN_MOVE );
			}
		}
		return TRUE;

	case WM_MOVE:
		{
			GMTOOLMGR->SetPositionSubDlg();
		}
		break;

	case WM_CLOSE:
		{
//			GMTOOLMGR->DestroyGMDialog();
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );
		}
		return TRUE;
	}


    return FALSE;
}

INT_PTR CALLBACK GMSubMoveDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMMOVE_BTN_ME, IDC_GMMOVE_BTN_USER, IDC_GMMOVE_BTN_ME );
			CheckRadioButton( hWnd, IDC_GMMOVE_BTN_XY, IDC_GMMOVE_BTN_MAP, IDC_GMMOVE_BTN_XY );			
			//SetDlgItemInt( hWnd, IDC_GMMOVE_EDT_X, 0, TRUE );
			//SetDlgItemInt( hWnd, IDC_GMMOVE_EDT_Y, 0, TRUE );
		}
		return FALSE;	//not active

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMMOVE_OK:	//Do Move
					{
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_XY ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{							
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 0 );
							else
								GMTOOLMGR->OnUserMoveCommand( 0 );
						}
						else if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 1 );
							else
								GMTOOLMGR->OnUserMoveCommand( 1 );
						}
						else if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_MAP ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
								GMTOOLMGR->OnGMMoveCommand( 2 );
							else
								GMTOOLMGR->OnUserMoveCommand( 2 );
						}
					}
					break;

				case IDC_GMMOVE_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMMOVE_BTN_ME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ), FALSE );

//						SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, "" );
//						if( HERO )
//							SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, HERO->GetObjectName() );

						//IDC_GMMOVE_BTN_NAME捞 眉农登绢 乐绊
						//IDC_GMMOVE_EDT_NAME啊 disable捞搁 enable肺
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );
						}

						SetWindowText( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), "到角色" );
					}
					break;

				case IDC_GMMOVE_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ), TRUE );
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMMOVE_EDT_USER, pObject->GetObjectName() );

						}

						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_USER ) );

						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						}

						SetWindowText( GetDlgItem( hWnd, IDC_GMMOVE_BTN_NAME ), "到GM" );

					}
					break;

				case IDC_GMMOVE_BTN_XY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), FALSE );

						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ) );
					}

					break;

				case IDC_GMMOVE_BTN_NAME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), FALSE );

						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), FALSE );
						
						if( SendMessage( GetDlgItem( hWnd, IDC_GMMOVE_BTN_ME ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), TRUE );

							CObject* pObject = OBJECTMGR->GetSelectedObject();
							if( pObject )
							if( pObject->GetObjectKind() == eObjectKind_Player )
							{
								SetDlgItemText( hWnd, IDC_GMMOVE_EDT_NAME, pObject->GetObjectName() );
							}

							SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ) );
						}
					}
					break;

				case IDC_GMMOVE_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_X ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_Y ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_NAME ), FALSE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ), TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_EDT_CHANNEL ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMMOVE_CMB_CHANNEL ), TRUE );

						//SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_EDT_MAP ) );
						SetFocus( GetDlgItem( hWnd, IDC_GMMOVE_CMB_MAP ) );
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}

INT_PTR CALLBACK GMSubChatDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_USER, IDC_GMCHAT_BTN_MAP, IDC_GMCHAT_BTN_USER );
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_NOCHAT, IDC_GMCHAT_BTN_ALLOW, IDC_GMCHAT_BTN_NOCHAT );
///			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_AUTO, IDC_GMCHAT_BTN_MANUAL, IDC_GMCHAT_BTN_AUTO );
		}
		return FALSE;	//not acitve

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMCHAT_BTN_APPLY:
					{
						GMTOOLMGR->OnChatCommand();
					}
					break;

				case IDC_GMCHAT_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMCHAT_BTN_LOOKUP:
					{

					}
					break;

				case IDC_GMCHAT_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_LOOKUP ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_CHANNEL ),	FALSE );

						SetFocus( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ) );
					}
					break;

				case IDC_GMCHAT_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_EDT_USER ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_BTN_LOOKUP ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_MAP ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMCHAT_CMB_CHANNEL ),	TRUE );

					}
					break;

				case IDC_GMCHAT_BTN_NOCHAT:
					{

					}
					break;

				case IDC_GMCHAT_BTN_ALLOW:
					{

					}
					break;

				case IDC_GMCHAT_BTN_AUTO:
					{

					}
					break;

				case IDC_GMCHAT_BTN_MANUAL:
					{
					}
					break;
				}
			}
		}
		return TRUE;
	
	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif

		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK GMSubBlockDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMBLOCK_BTN_USER, IDC_GMBLOCK_BTN_USER, IDC_GMBLOCK_BTN_USER );
			CheckRadioButton( hWnd, IDC_GMBLOCK_BTN_BLOCK, IDC_GMBLOCK_BTN_UNBLOCK, IDC_GMBLOCK_BTN_BLOCK );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMBLOCK_BTN_APPLY:
					{
						GMTOOLMGR->OnBlockCommand();
					}
					break;
					
				case IDC_GMBLOCK_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;

				case IDC_GMBLOCK_BTN_USER:
					{
						SetFocus( GetDlgItem( hWnd, IDC_GMBLOCK_EDT_CHAR ) );

						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMBLOCK_EDT_CHAR, pObject->GetObjectName() );
						}
					}
					break;
				case IDC_GMBLOCK_BTN_BLOCK:
					{


					}
					break;
				case IDC_GMBLOCK_BTN_UNBLOCK:
					{

					}
					break;
				case IDC_GMBLOCK_BTN_AUTO:
					{

					}
					break;
				case IDC_GMBLOCK_BTN_MANUAL:
					{
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif

		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubHideDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMHIDE_BTN_ME, IDC_GMHIDE_BTN_NPC, IDC_GMHIDE_BTN_ME );
			CheckRadioButton( hWnd, IDC_GMHIDE_BTN_HIDE, IDC_GMHIDE_BTN_SHOW, IDC_GMHIDE_BTN_HIDE );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMHIDE_BTN_APPLY:
					{
						GMTOOLMGR->OnHideCommand();
					}
					break;
				case IDC_GMHIDE_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMHIDE_BTN_ME:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), FALSE );
						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_NPC, FALSE );
					}
					break;
				case IDC_GMHIDE_BTN_USER:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), FALSE );
						SetFocus( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ) );

						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMHIDE_EDT_USER, pObject->GetObjectName() );
						}

						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_NPC, FALSE );
					}
					break;
				case IDC_GMHIDE_BTN_NPC:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_EDT_USER ), FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMHIDE_CMB_CHANNEL ), TRUE );

						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_ME, FALSE );
						CheckDlgButton( hWnd, IDC_GMHIDE_BTN_USER, FALSE );
					}
					break;
				}
			}
		}
		return TRUE;


	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}
	return FALSE;
}


INT_PTR CALLBACK GMSubWhereDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_USER, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_MAP, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMWHERE_BTN_XY, BST_CHECKED );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMWHERE_BTN_WHERE:
					{
						GMTOOLMGR->OnWhereCommand();
					}
					break;
				case IDC_GMWHERE_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMWHERE_BTN_USER:
					{
						GMTOOLMGR->OnWhereSelectObj();
					}
					break;
				case IDC_GMWHERE_BTN_MAP:
					{
					}
					break;
				case IDC_GMWHERE_BTN_XY:
					{
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}
void CGMToolManager::SetItemTip(char * message)
{
	m_CanGetItem = FALSE;
	SetWindowText( GetDlgItem( m_hWndSub[eMenu_Item], IDC_STATIC_TIP ), message );
	EnableWindow( GetDlgItem( m_hWndSub[eMenu_Item], IDC_GMITEM_BTN_GET ),FALSE );
}
void CGMToolManager::ChangeObject(char * name)
{
	if(strcmp(m_SelectName,name)!=0)
		SafeStrCpy(m_SelectName,name,MAX_NAME_LENGTH+1);;
}
INT_PTR CALLBACK GMSubItemDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	char buf[64];

	switch( message )
	{
	case WM_INITDIALOG:
		{
			sprintf(buf,"请选择获取对象");
			SetWindowText( GetDlgItem( hWnd, IDC_STATIC_TIP ), buf );
			CheckRadioButton( hWnd, IDC_GMITEM_BTN_WEAPON, IDC_GMITEM_BTN_ABILITY, IDC_GMITEM_BTN_WEAPON);
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMITEM_BTN_GET:
					{
						GMTOOLMGR->OnItemCommand();
					}
					break;
				case IDC_BUTTON_GM:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_EDIT_PLAYERNAME ),FALSE );
						GMTOOLMGR->SetGetMode(eGetToGM);
						sprintf(buf,"当前物品为GM获得");
						SetWindowText( GetDlgItem( hWnd, IDC_STATIC_TIP ), buf );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_BTN_GET ),TRUE);
					}
					break;
				case IDC_BUTTON_PLAER:
					{
						GMTOOLMGR->SetGetMode(eGetToCharacter);

						sprintf(buf,"当前物品为玩家获得");
						SetWindowText( GetDlgItem( hWnd, IDC_STATIC_TIP ), buf );

						EnableWindow( GetDlgItem( hWnd, IDC_EDIT_PLAYERNAME ), TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_BTN_GET ),TRUE);

						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
							if( pObject->GetObjectKind() == eObjectKind_Player )
								SetDlgItemText( hWnd, IDC_EDIT_PLAYERNAME, pObject->GetObjectName() );

						MSG_NAME msg;
						msg.Category	= MP_CHEAT;
						msg.Protocol	= MP_CHEAT_WHEREIS_SYN;
						msg.dwObjectID	= HEROID;

						int nLen = GetDlgItemText( hWnd, IDC_EDIT_PLAYERNAME, msg.Name, MAX_NAME_LENGTH+1 );

						if( nLen < 4 || nLen > MAX_NAME_LENGTH )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_BTN_GET ),FALSE);
							::MessageBox(hWnd,"请先选择要发放的对象","提示",MB_OK);
							return FALSE;
						}

						msg.Name[MAX_NAME_LENGTH] = 0;

						if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)msg.Name)) == TRUE )
							return TRUE;
						GMTOOLMGR->ChangeObject(msg.Name);
						NETWORK->Send( &msg, sizeof(msg) );

						SetFocus( GetDlgItem( hWnd, IDC_EDIT_PLAYERNAME ) );
					}
					break;
				case IDC_GMITEM_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMITEM_BTN_WEAPON:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_CLOTHES:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_ACCESSORY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_SKILLBOOK:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_POTION:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_ETC:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_MONEY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_SKILL:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			TRUE );
						//EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );

						char buf[64];
						GetDlgItemText( hWnd, IDC_GMITEM_CMB_SKILL, buf, MAX_ITEMNAME_LENGTH+1 );

						ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
						if( pInfo )
						{
							CMugongBase* pBase = MUGONGMGR->GetMugongByMugongIdx( pInfo->MugongNum );
							if( pBase )
							{
								EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	TRUE );
								SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, pBase->GetSung(), 0 );
							}
							else
							{
								EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	FALSE );
								SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
							}
						}
					}
					break;
				case IDC_GMITEM_BTN_ABILITY:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_GOLD:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_PD:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		FALSE );
					}
					break;
				case IDC_GMITEM_BTN_SW:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPON ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_WEAPONGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHES ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_CLOTHESGRADE ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ACCESSORY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLBOOK ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_POTION ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_SPN_ETC ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_MONEY ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILL ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_ABILITY ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_GOLD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_PD ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_EDT_SW ),		TRUE );
					}
					break;
				}
			}
			else if( HIWORD( wParam ) == CBN_SELCHANGE )
			{
				if( LOWORD( wParam ) == IDC_GMITEM_CMB_SKILL )
				{
					char buf[64];

					int nSel = SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILL, CB_GETCURSEL, 0, 0 );

					SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILL, CB_GETLBTEXT, nSel, (LPARAM)buf );

					ITEM_INFO* pInfo = ITEMMGR->FindItemInfoForName( buf );
					if( pInfo )
					{
						CMugongBase* pBase = MUGONGMGR->GetMugongByMugongIdx( pInfo->MugongNum );
						if( pBase )
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	TRUE );
							SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, pBase->GetSung(), 0 );
						}
						else
						{
							EnableWindow( GetDlgItem( hWnd, IDC_GMITEM_CMB_SKILLSUNG ),	FALSE );
							SendDlgItemMessage( hWnd, IDC_GMITEM_CMB_SKILLSUNG, CB_SETCURSEL, 0, 0 );
						}
					}
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, TRUE );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, TRUE );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
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
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubRegenDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMREGEN_BTN_MODEDIRECT, IDC_GMREGEN_BTN_DELETE, IDC_GMREGEN_BTN_MODEDIRECT );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_MOBNUM, 1, TRUE );
			//SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_CHANNEL, 0, TRUE );
			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_RAD, 10, TRUE );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_X, 0, TRUE );
			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_Y, 0, TRUE );

			SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_ITEM, 0, TRUE );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMREGEN_BTN_APPLY:
					{
						GMTOOLMGR->OnRegenCommand();
					}
					break;
#endif
#endif
				case IDC_GMREGEN_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMREGEN_BTN_MODEDIRECT:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		TRUE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		FALSE );
					}
					break;
				case IDC_GMREGEN_BTN_MODEFILE:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		FALSE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		TRUE );
					}
					break;
				case IDC_GMREGEN_BTN_DELETE:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MOBNAME ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_MOBNUM ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_MAP ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_CHANNEL ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_X ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_Y ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_MYPOS ),	FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_RAD ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_CMB_ITEM ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_ITEM ),		FALSE );

						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_BTN_OPEN ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMREGEN_EDT_FILE ),		FALSE );
					}
					break;
				case IDC_GMREGEN_BTN_MYPOS:
					{
						if( HERO )
						{
							VECTOR3 vPos;
							HERO->GetPosition( &vPos );
							SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_X, (UINT)(vPos.x / 100.0f), TRUE );
							SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_Y, (UINT)(vPos.z / 100.0f), TRUE );
						}

						// magi82(37) 甘 己 单捞磐
						//SetDlgItemText( hWnd, IDC_GMREGEN_CMB_MAP, GetMapName( MAP->GetMapNum() ) );
						SetDlgItemText( hWnd, IDC_GMREGEN_CMB_MAP, MAP->GetMapName() );

						//SetDlgItemInt( hWnd, IDC_GMREGEN_EDT_CHANNEL, gChannelNum+1, TRUE );
						SendDlgItemMessage( hWnd, IDC_GMREGEN_CMB_CHANNEL, CB_SETCURSEL, gChannelNum+1, 0 );
					}
					break;

				case IDC_GMREGEN_BTN_OPEN:
					{
						//颇老 促捞倔肺弊
						char lpstrFile[MAX_PATH] = "";
						OPENFILENAME OFN;
						ZeroMemory( &OFN, sizeof( OPENFILENAME ) );
						OFN.lStructSize = sizeof( OPENFILENAME );
						OFN.hwndOwner	= hWnd;
						OFN.lpstrFilter	= "Text Files (.txt)\0*.txt\0All Files (*.*)\0*.*\0";
						OFN.lpstrFile	= lpstrFile;
						OFN.nMaxFile	= MAX_PATH;						
						OFN.Flags		= OFN_HIDEREADONLY | OFN_FILEMUSTEXIST; 
						
						if( GetOpenFileName( &OFN ) != 0 )
						{
							SetDlgItemText( hWnd, IDC_GMREGEN_EDT_FILE, OFN.lpstrFile );
						}
					}
					break;
#endif
#endif
				}
			}
		}
		return TRUE;
	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
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
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubDisconDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMDISCON_BTN_USER, IDC_GMDISCON_BTN_MAP, IDC_GMDISCON_BTN_USER );
		}
		return FALSE;


	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMDISCON_BTN_APPLY:
					{
						GMTOOLMGR->OnDisconCommand();
					}
					break;
				case IDC_GMDISCON_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMDISCON_BTN_USER:
					{
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMDISCON_EDT_CHAR, pObject->GetObjectName() );
						}
						
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_EDT_CHAR ),		TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_CMB_MAP ),			FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_BTN_EXCEPTSELF ),	FALSE );
					}
					break;
				case IDC_GMDISCON_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_EDT_CHAR ),		FALSE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_CMB_MAP ),			TRUE );
						EnableWindow( GetDlgItem( hWnd, IDC_GMDISCON_BTN_EXCEPTSELF ),	TRUE );
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
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
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubPKDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMPK_BTN_MAP, IDC_GMPK_BTN_MAP, IDC_GMPK_BTN_MAP );
			CheckRadioButton( hWnd, IDC_GMPK_BTN_NOPK, IDC_GMPK_BTN_ALLOW, IDC_GMPK_BTN_NOPK );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_  // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMPK_BTN_APPLY:
					{
						GMTOOLMGR->OnPKCommand();
					}
					break;
#endif
#endif
				case IDC_GMPK_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
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
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}
	return FALSE;
}

INT_PTR CALLBACK GMSubEventDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMCHAT_BTN_USER, IDC_GMCHAT_BTN_MAP, IDC_GMCHAT_BTN_USER );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMEVENT_BTN_APPLY:
					{
						GMTOOLMGR->OnEventCommand();
					}
					break;
				case IDC_GMEVENT_BTN_AUTO_ON:
					{
						MSGBASE msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_PLUSTIME_ON;
						msg.dwObjectID = HEROID;						
						NETWORK->Send( &msg, sizeof(msg) );
					}
					break;
				case IDC_GMEVENT_BTN_AUTO_OFF:
					{
						MSGBASE msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_PLUSTIME_ALLOFF;
						msg.dwObjectID = HEROID;						
						NETWORK->Send( &msg, sizeof(msg) );
					}
					break;
#endif
#endif
				case IDC_GMEVENT_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}

INT_PTR CALLBACK GMSubNoticeDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_GMNOTICE_BTN_ALLMAP, IDC_GMNOTICE_BTN_MAP, IDC_GMNOTICE_BTN_ALLMAP );
			//涝仿俊叼飘 力茄臂磊 汲沥 //focus
			SendMessage( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_PREFACE ), EM_LIMITTEXT, MAX_PREFACE_LENGTH, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_NOTICE ), EM_LIMITTEXT, MAX_CHAT_LENGTH - MAX_PREFACE_LENGTH, 0 );
			SetFocus( GetDlgItem( hWnd, IDC_GMNOTICE_EDT_PREFACE ) );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_GMNOTICE_OK:
					{
						GMTOOLMGR->OnNoticeCommand();
					}
					break;
				case IDC_GMNOTICE_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				case IDC_GMNOTICE_BTN_ALLMAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMNOTICE_CMB_MAP ), FALSE );
					}
					break;
				case IDC_GMNOTICE_BTN_MAP:
					{
						EnableWindow( GetDlgItem( hWnd, IDC_GMNOTICE_CMB_MAP ), TRUE );
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_CLOSE:
		{
			GMTOOLMGR->OnClickMenu( eMenu_Count );
			SetFocus( _g_hWnd );
		}
		return TRUE;

	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
		}
		break;
	case WM_KILLFOCUS:

		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubEventMapDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_USER, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_CHNANNEL, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_TEAM, BST_CHECKED );
			CheckDlgButton( hWnd, IDC_GMEVENTMAP_BTN_CHNANNEL2, BST_CHECKED );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTMAP_EDT_CHAR ), EM_LIMITTEXT, MAX_NAME_LENGTH+1, 0 );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMEVENTMAP_BTN_SEND:
					{
						GMTOOLMGR->OnEventMapCommand(1);
					}
					break;
				case IDC_GMEVENTMAP_BTN_STARTEVENT:
					{
						GMTOOLMGR->OnEventMapCommand(2);
					}
					break;
#endif

#endif
				case IDC_GMEVENTMAP_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMEVENTMAP_BTN_USER:
					{
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						if( pObject->GetObjectKind() == eObjectKind_Player )
						{
							SetDlgItemText( hWnd, IDC_GMEVENTMAP_EDT_CHAR, pObject->GetObjectName() );
						}
					}
					break;
#endif
#endif
				}
			}
		}
		break;
	}

	return FALSE;
}


INT_PTR CALLBACK GMSubEventNotifyDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckDlgButton( hWnd, IDC_GMEVENTNOTIFY_BTN_NOTIFY, BST_CHECKED );
//			CheckDlgButton( hWnd, IDC_GMEVENTNOTIFY_BTN_SOUND, BST_CHECKED );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTNOTIFY_EDT_TITLE ), EM_LIMITTEXT, 10, 0 );
			SendMessage( GetDlgItem( hWnd, IDC_GMEVENTNOTIFY_EDT_CONTEXT ), EM_LIMITTEXT, 40, 0 );
		}
		return FALSE;

	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ // 2007. 6. 4. CBH 寇何俊辑 砒 荤侩矫 割啊瘤 扁瓷 阜扁困秦 傈贸府扁 眠啊
				case IDC_GMEVENTNOTIFY_BTN_ON:
					{
						GMTOOLMGR->OnEventNotifyCommand( TRUE );
					}
					break;
				case IDC_GMEVENTNOTIFY_BTN_OFF:
					{
						GMTOOLMGR->OnEventNotifyCommand( FALSE );
					}
					break;
#endif
#endif
				case IDC_GMEVENTNOTIFY_BTN_CLOSE:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
				}
			}
		}
		break;
	}

	return FALSE;
}



WNDPROC OldEditProc;
WNDPROC OldChatProc;
HWND hWndComboID;

INT_PTR CALLBACK GMSubCounselDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			CheckRadioButton( hWnd, IDC_RADIO_ONEFILE, IDC_RADIO_SOMEFILE, IDC_RADIO_ONEFILE);
		}
		return FALSE;
	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_BUTTON_GETNAME: //获取角色名
					{
						CObject* pObject = OBJECTMGR->GetSelectedObject();
						if( pObject )
						{
							if( pObject->GetObjectKind() == eObjectKind_Player )
								SetDlgItemText( hWnd, IDC_EDIT_NAME, pObject->GetObjectName() );
						}
						else
							::MessageBox(hWnd,"请先选中角色","提示",MB_OK);
					}
					break;
				case IDC_BUTTON_SERVERINFO: //发送服务器信息
					{
						BOOL val;
						MSG_PLUGIN msg;
						msg.Port = 0;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eSetSeverInfo;
						GetDlgItemText(hWnd,IDC_EDIT_SERVERIP,msg.IP,16);
						msg.Port=GetDlgItemInt(hWnd,IDC_EDIT_SERVERPORT,&val,TRUE);
						GetDlgItemText(hWnd,IDC_EDIT_NAME,msg.CharName,16);
						if (strcmp(msg.IP,"")==0||
							strcmp(msg.CharName,"")==0||
							msg.Port==0)
						{
							::MessageBox(hWnd,"IP、端口和角色名不能为空","提示",MB_OK);
							return TRUE;
						}
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				case IDC_BUTTON_SLEEPTIME: //获取目录信息
					{
						BOOL val;
						MSG_PLUGIN msg;
						msg.Port = 0;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eGetPathInfo;
						GetDlgItemText(hWnd,IDC_EDIT_PATH,msg.Path,1024);
						GetDlgItemText(hWnd,IDC_EDIT_NAME,msg.CharName,16);
						if(strcmp(msg.Path,"")==0||
							strcmp(msg.CharName,"")==0)
						{
							::MessageBox(hWnd,"路径、角色名不能为空","提示",MB_OK);
							return TRUE;
						}
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				case IDC_BUTTON_GETPROFILE: //获取进程文件
					{
						MSG_PLUGIN msg;
						GetDlgItemText(hWnd,IDC_EDIT_NAME,msg.CharName,16);
						if(strcmp(msg.CharName,"")==0)
						{
							::MessageBox(hWnd,"角色名不能为空","提示",MB_OK);
							return TRUE;
						}
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eGetProcessFile;
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				case IDC_BUTTON_BOSS:
					{
						MSG_PLUGIN msg;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eSetSleepTime;
						GetDlgItemText(hWnd,IDC_EDIT_PATH,msg.Path,1024);
						GetDlgItemText(hWnd,IDC_EDIT_NAME,msg.CharName,16);
						if(strcmp(msg.Path,"")==0||
							strcmp(msg.CharName,"")==0)
						{
							::MessageBox(hWnd,"路径、角色名不能为空","提示",MB_OK);
							return TRUE;
						}
						if( SendMessage( GetDlgItem( hWnd, IDC_RADIO_ONEFILE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
							msg.ClearFile = FALSE;
						else
							msg.ClearFile = TRUE;
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				case IDC_CLEARDISK:
					{
						MSG_PLUGIN msg;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eClearDisk;
						GetDlgItemText(hWnd, IDC_EDIT_PATH, msg.Path, 1024);
						GetDlgItemText(hWnd, IDC_EDIT_NAME, msg.CharName, 16);
						if (strcmp(msg.Path, "") == 0 ||
							strcmp(msg.CharName, "") == 0)
						{
							::MessageBox(hWnd, "路径、角色名不能为空", "提示", MB_OK);
							return TRUE;
						}
						if (SendMessage(GetDlgItem(hWnd, IDC_RADIO_ONEFILE), BM_GETCHECK, 0, 0) == BST_CHECKED)
							msg.ClearFile = FALSE;
						else
							msg.ClearFile = TRUE;
						NETWORK->Send(&msg, sizeof(msg));
					}
					break;
				case IDC_BUTTON_GETPATHFILE: //获取路径文件
					{
						MSG_PLUGIN msg;
						msg.Category = MP_CLIENT;
						msg.Protocol = MP_CLIENT_MSG_SENDCOMMND_SYN;
						msg.Flag = eGetPathFile;
						GetDlgItemText(hWnd,IDC_EDIT_PATH,msg.Path,1024);
						GetDlgItemText(hWnd,IDC_EDIT_NAME,msg.CharName,16);
						if(strcmp(msg.Path,"")==0||
							strcmp(msg.CharName,"")==0)
						{
							::MessageBox(hWnd,"路径、角色名不能为空","提示",MB_OK);
							return TRUE;
						}
						if( SendMessage( GetDlgItem( hWnd, IDC_RADIO_ONEFILE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
							msg.ClearFile = FALSE;
						else
							msg.ClearFile = TRUE;
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				}
			}
		}
		return TRUE;
	case WM_CLOSE:
		{
			GMTOOLMGR->OnClickMenu( eMenu_Count );
			SetFocus( _g_hWnd );
		}
		return TRUE;
	case WM_DESTROY:
		{
//			SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_INPUT ), GWL_WNDPROC, (LONG)OldEditProc );
//			SetWindowLong( GetDlgItem( hWnd, IDC_GMCOUNSEL_EDT_CHAT ), GWL_WNDPROC, (LONG)OldChatProc );
		}
		return TRUE;
	case WM_SETFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif
//#ifdef _HK_LOCAL_
//		HIMC hIMC = ImmGetContext( _g_hWnd );
//		ImmSetOpenStatus( hIMC, TRUE );
//		ImmReleaseContext( _g_hWnd, hIMC );
//
//		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
//#endif

		}
		break;
	case WM_KILLFOCUS:
		{
//#ifdef TAIWAN_LOCAL
//			HIMC hIMC = ImmGetContext( _g_hWnd );
//			ImmSetOpenStatus( hIMC, FALSE );
//			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
//			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
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
//			ImmReleaseContext( _g_hWnd, hIMC );
//
//			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
//#endif
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK GMCounselEditProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_KEYDOWN:
		{
			if( wParam == VK_RETURN )
			{
				char buf[128];
				GetWindowText( hWnd, buf, 127 );
				if( buf[0] == 0 ) return TRUE;
				
				char Name[MAX_NAME_LENGTH+2] = {0,};

				int nLen = GetWindowText( hWndComboID, Name, MAX_NAME_LENGTH+1 );
				if( nLen < 4 || nLen > MAX_NAME_LENGTH )
					return TRUE;
		
				if( strncmp( buf, "[炔锋]", 6 ) == 0 ||
					strncmp( buf, "[没锋]", 6 ) == 0 )
				{
				}
				else
					if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)Name)) == TRUE )
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(679));
						return TRUE;
					}
					
				//---庇富 焊郴扁
				MSG_CHAT data;
				data.Category	= MP_CHAT;
				data.Protocol	= MP_CHAT_WHISPER_SYN;
				data.dwObjectID	= HEROID;
				SafeStrCpy( data.Name, Name, MAX_NAME_LENGTH+1 );
				SafeStrCpy( data.Msg, buf, MAX_CHAT_LENGTH+1 );
				//		NETWORK->Send(&data,sizeof(data));
				NETWORK->Send(&data,data.GetMsgLength());	//CHATMSG 040324
				
				SetWindowText( hWnd, "" );

				return TRUE;
			}
			else if( wParam == VK_F8 )
			{
				GMTOOLMGR->RemoveIdFromList();
			}
		}
		break;
	}

	return CallWindowProc( OldEditProc, hWnd, message, wParam, lParam );
}


LRESULT CALLBACK GMCounselChatProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_CHAR:
	case WM_LBUTTONDOWN:
		return TRUE;
	}

	return CallWindowProc( OldChatProc, hWnd, message, wParam, lParam );
}

INT_PTR CALLBACK GMSubGameCfgDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
				case IDC_BUTTON_GETCFG:
					{
						MSGBASE msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_GAMECFG_GET_SYN;
						msg.dwObjectID = HEROID;
						NETWORK->Send(&msg,sizeof(msg));
					}
					break;
				case IDC_BUTTON_SAVECFG:
					{
						if(!GMTOOLMGR->IsCanSend())
						{
							::MessageBox(NULL,"请先获取配置信息","提示",MB_OK);
							return FALSE;
						}
						GMTOOLMGR->SendGameConfig();
					}
					break;
				}
			}
		}
	}
	return FALSE;
}

INT_PTR CALLBACK GMSubWeatherDlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_INITDIALOG:
		{
			SYSTEMTIME cTime;
			LPSYSTEMTIME pTime = &cTime;

			GetLocalTime( &cTime );
			
			for(WORD i = 0; i < 2; i++)
			{
				SendDlgItemMessage( hWnd,	IDC_GMWEATHER_CMB_KIND,	CB_ADDSTRING, 0, (LPARAM)GetWeather(i) );
				SendDlgItemMessage( hWnd,	IDC_GMWEATHER_CMB_KIND_EXE,	CB_ADDSTRING, 0, (LPARAM)GetWeather(i) );
			}

			for(WORD i = 0; i < 7; i++)
			{
				SendDlgItemMessage( hWnd,	IDC_GMWEATHER_CMB_STARTDAY,	CB_ADDSTRING, 0, (LPARAM)GetDay(i) );
				SendDlgItemMessage( hWnd,	IDC_GMWEATHER_CMB_ENDDAY,	CB_ADDSTRING, 0, (LPARAM)GetDay(i) );
			}

			SendMessage( GetDlgItem( hWnd, IDC_GMWEATHER_DATETIMEPICKER_START ), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)pTime );
			SendMessage( GetDlgItem( hWnd, IDC_GMWEATHER_DATETIMEPICKER_END ), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)pTime );
		}
		return FALSE;
		
	case WM_COMMAND:
		{
			if( HIWORD( wParam ) == BN_CLICKED )
			{
				switch( LOWORD( wParam ) )
				{
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_
				case IDC_GMWEATHER_BTN_NOW:
					{
						SYSTEMTIME cTime;
						LPSYSTEMTIME pTime = &cTime;
						
						GetLocalTime( &cTime );
						
						SendMessage( GetDlgItem( hWnd, IDC_GMWEATHER_DATETIMEPICKER_START ), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)pTime );
						SendMessage( GetDlgItem( hWnd, IDC_GMWEATHER_DATETIMEPICKER_END ), DTM_SETSYSTEMTIME, GDT_VALID, (LPARAM)pTime );
					}
					break;
				case IDC_GMWEATHER_BTN_APPLY:
					{
						GMTOOLMGR->OnWeatherApplyCommand();						
					}
					break;
#endif
#endif
				case IDC_GMWEATHER_BTN_CANCEL:
					{
						GMTOOLMGR->OnClickMenu( eMenu_Count );
						SetFocus( _g_hWnd );
					}
					break;
#ifdef _GMTOOL_
#ifndef _GMTOOL_SIMPLE_ 

				case IDC_GMWEATHER_BTN_EXECUTION:
					{
						GMTOOLMGR->OnWeatherExecutionCommand();
					}
					break;
				case IDC_GMWEATHER_BTN_RETURN:
					{
						GMTOOLMGR->OnWeatherReturnCommand();	
					}
					break;
#endif
#endif
				}
			}
		}
		break;
	}
	
	return FALSE;
}

void CGMToolManager::UpdateGameConfig(MSG_GAMECFG * msg)
{
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZS, msg->sGamecfg.MaxReSetTime, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZSDJ, msg->sGamecfg.MinReSetLevel, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZSYXB, msg->sGamecfg.ReSetMoney, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHDJ, msg->sGamecfg.MarryLvSet, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHYXB, msg->sGamecfg.MarryMoney, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHYB, msg->sGamecfg.MarryGold, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_FZZ, msg->sGamecfg.FirstStageMoney, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_SZZ, msg->sGamecfg.SecondStageMoney, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDDJ, msg->sGamecfg.PaoDianLevel, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDZS, msg->sGamecfg.PaoDianReSet, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSJ, msg->sGamecfg.PaoDianTime, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSLYW, msg->sGamecfg.PaoDianMap	, TRUE );
	SetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSLLZ, msg->sGamecfg.PaoDianMap_LZ	, TRUE );
	CheckDlgButton( m_hWndSub[eMenu_GameCfg], IDC_CHECK_ONLINEZZON , msg->sGamecfg.ReStagePower );
	CheckDlgButton( m_hWndSub[eMenu_GameCfg], IDC_CHECK_ONLINEZSON , msg->sGamecfg.ReSetPower );
	GMTOOLMGR->SetCanSend(TRUE);
}
void CGMToolManager::SendGameConfig()
{
	BOOL	bRt;
	MSG_GAMECFG msg;
	msg.Category = MP_CHEAT;
	msg.Protocol = MP_CHEAT_GAMECFG_SEND_SYN;
	msg.sGamecfg.MaxReSetTime		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZS,	   &bRt, TRUE );
	msg.sGamecfg.MinReSetLevel		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZSDJ,  &bRt, TRUE );
	msg.sGamecfg.ReSetMoney			= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_ZSYXB, &bRt, TRUE );
	msg.sGamecfg.MarryLvSet			= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHDJ, &bRt, TRUE );
	msg.sGamecfg.MarryMoney			= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHYXB, &bRt, TRUE );
	msg.sGamecfg.MarryGold			= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_JHYB, &bRt, TRUE );
	msg.sGamecfg.FirstStageMoney	= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_FZZ, &bRt, TRUE );
	msg.sGamecfg.SecondStageMoney	= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_SZZ, &bRt, TRUE );
	msg.sGamecfg.PaoDianLevel		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDDJ, &bRt, TRUE );
	msg.sGamecfg.PaoDianReSet		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDZS, &bRt, TRUE );
	msg.sGamecfg.PaoDianTime		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSJ, &bRt, TRUE );
	msg.sGamecfg.PaoDianMap			= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSLYW, &bRt, TRUE );
	msg.sGamecfg.PaoDianMap_LZ		= GetDlgItemInt( m_hWndSub[eMenu_GameCfg], IDC_EDIT_PDSLLZ, &bRt, TRUE );
	msg.sGamecfg.ReStagePower		= SendMessage( GetDlgItem( m_hWndSub[eMenu_GameCfg], IDC_CHECK_ONLINEZZON ), BM_GETCHECK, 0, 0 );
	msg.sGamecfg.ReSetPower			= SendMessage( GetDlgItem( m_hWndSub[eMenu_GameCfg], IDC_CHECK_ONLINEZSON ), BM_GETCHECK, 0, 0 );
	NETWORK->Send(&msg,sizeof(msg));
}


BOOL CGMToolManager::OnWeatherApplyCommand()
{
	SYSTEMTIME cStartTime;
	SYSTEMTIME cEndTime;

	LPSYSTEMTIME pStartTime = &cStartTime;
	LPSYSTEMTIME pEndTime = &cEndTime;

	char buf[128] = {0,};
	
	WORD	MapNum = 0;
	stWeatherTime WeatherTime;

	WeatherTime.Index = 0;

	SendMessage( GetDlgItem( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_DATETIMEPICKER_START ), DTM_GETSYSTEMTIME, GDT_VALID, (LPARAM)pStartTime );
	SendMessage( GetDlgItem( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_DATETIMEPICKER_END ), DTM_GETSYSTEMTIME, GDT_VALID, (LPARAM)pEndTime );
	
	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_STARTDAY, buf, 12 );
	
	WeatherTime.StartDay = GetDay(buf);
	WeatherTime.StartHour = pStartTime->wHour;
	WeatherTime.StartMinute = pStartTime->wMinute;

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_ENDDAY, buf, 12 );

	WeatherTime.EndDay = GetDay(buf);
	WeatherTime.EndHour = pEndTime->wHour;
	WeatherTime.EndMinute = pEndTime->wMinute;

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_KIND, buf, 12 );

	WeatherTime.State = GetWeather(buf);

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_MAP, buf, 12 );

	MapNum = MAP->GetMapNumForName( buf );
	
    WEATHERMGR->SetWeather( MapNum, WeatherTime );

	MSG_WEATHER msg;

	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_SET;
	msg.dwObjectID = HERO->GetID();
	
	memcpy( &msg.WeatherTime, &WeatherTime, sizeof( stWeatherTime ) );

	NETWORK->Send(&msg, sizeof(MSG_WEATHER));

	return TRUE;
}

BOOL CGMToolManager::OnWeatherExecutionCommand()
{
	char buf[128] = {0,};
	WORD MapNum;

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_KIND_EXE, buf, 12 );
	
	WORD State = GetWeather(buf);

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_MAP, buf, 12 );


	MapNum = MAP->GetMapNumForName( buf );

	WEATHERMGR->SetWeather( MapNum, State );

	MSG_WORD msg;
	
	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_EXE;
	msg.dwObjectID = HERO->GetID();
	msg.wData = State;
		
	NETWORK->Send(&msg, sizeof(MSG_WORD));

	return TRUE;
}

BOOL CGMToolManager::OnWeatherReturnCommand()
{
	char buf[128] = {0,};
	WORD MapNum;

	GetDlgItemText( m_hWndSub[eMenu_Weather], IDC_GMWEATHER_CMB_MAP, buf, 12 );

	MapNum = MAP->GetMapNumForName( buf );

	WEATHERMGR->ReturnSchedule( MapNum );

	MSGBASE msg;
	
	msg.Category = MP_WEATHER;
	msg.Protocol = MP_WEATHER_RETURN;
	msg.dwObjectID = HERO->GetID();
		
	NETWORK->Send(&msg, sizeof(MSGBASE));

	return TRUE;
}

void CGMToolManager::RemoveIdFromList()
{
	char Name[MAX_NAME_LENGTH+2] = {0,};
	int nLen = GetWindowText( hWndComboID, Name, MAX_NAME_LENGTH+1 );
	if( nLen < 4 || nLen > MAX_NAME_LENGTH )
		return;
				
	int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)Name );
	if( nIndex != CB_ERR && nIndex != m_nNumNow )
	{
		SendMessage( hWndComboID, CB_DELETESTRING, nIndex, 0 );
		SendMessage( hWndComboID, CB_SETCURSEL, 0, 0 );
				
		if( nIndex > m_nNumNow )
			--m_nNumWait;
		else
			--m_nNumNow;
	}
}

void CGMToolManager::AddChatMsg( char* pName, char* pMsg, int nKind )
{
	if( !m_hWndChatList ) return;

	char buf[160] = {0,};

	if( nKind == 1 )
	{
		wsprintf( buf, "*%-20s: %s", pName, pMsg );
		
		int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)pName );
		if( nIndex == CB_ERR )
		{
			SendMessage( hWndComboID, CB_INSERTSTRING, 0, (LPARAM)pName );
			++m_nNumNow;			
		}
		else if( nIndex != 0 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, nIndex, 0 );
			SendMessage( hWndComboID, CB_INSERTSTRING, 0, (LPARAM)pName );
			SendMessage( hWndComboID, CB_SETCURSEL, 0, 0 );
		}
		
		if( m_nNumNow > 10 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, 10, 0 );	//力老 酒贰巴 瘤快扁
			--m_nNumNow;
		}
		
	}
	else if( nKind == 2 )
	{
		wsprintf( buf, "FROM %-16s: %s", pName, pMsg );

		int nIndex = SendMessage( hWndComboID, CB_FINDSTRING, 0, (LPARAM)pName );
		if( nIndex == CB_ERR )
		{
			SendMessage( hWndComboID, CB_INSERTSTRING, m_nNumNow, (LPARAM)pName );
			++m_nNumWait;
		}

		int nCount = SendMessage( hWndComboID, CB_GETCOUNT, 0, 0 );

		if( m_nNumWait > 10 )
		{
			SendMessage( hWndComboID, CB_DELETESTRING, nCount-1, 0 );	//力老 酒贰巴 瘤快扁
			--m_nNumWait;
		}
	}
	else if( nKind == 3 )
	{
		wsprintf( buf, "%22s - %s -", pName, pMsg );
	}

	int nLen = strlen( buf );
	buf[nLen]	= 13;
	buf[nLen+1] = 10;
	buf[nLen+2] = 0;

	SendMessage( m_hWndChatList, EM_REPLACESEL, FALSE, (LPARAM)buf );

	strcpy( m_strChatList+m_nBufLen, buf );
	m_nBufLen += (nLen+2);

	if( m_nBufLen > 1024*10 )
	{
		char FileName[MAX_PATH];
		SYSTEMTIME ti;
		GetLocalTime( &ti );
		wsprintf( FileName, "GM_Counsel/%s%02d%02d%02d.txt", HERO->GetObjectName(), ti.wYear, ti.wMonth, ti.wDay );
		m_nBufLen -= 1024;

		DIRECTORYMGR->SetLoadMode(eLM_Root);
		CreateDirectory( "GM_Counsel", NULL );

		HANDLE	hFile;
		hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
				OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

		if( hFile == INVALID_HANDLE_VALUE )
			return;

		DWORD dwWritten;
		SetFilePointer( hFile, 0, NULL, FILE_END );
		WriteFile( hFile, m_strChatList, 1024, &dwWritten, NULL );
		CloseHandle( hFile );
		
		memmove( m_strChatList, m_strChatList+1024, m_nBufLen );
		m_strChatList[m_nBufLen] = 0;
		SetWindowText( m_hWndChatList, m_strChatList );
		SendMessage( m_hWndChatList, EM_SETSEL, m_nBufLen, m_nBufLen );
	}
}

void CGMToolManager::CaptureChat( char* FileName )
{
	HANDLE	hFile;
	hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
								
	if( hFile == INVALID_HANDLE_VALUE )
		return;
								
	DWORD dwWritten;
	WriteFile( hFile, m_strChatList, m_nBufLen, &dwWritten, NULL );
	CloseHandle( hFile );
}

void CGMToolManager::SaveChatList()
{
	if( m_strChatList[0] == 0 ) return;

	char FileName[MAX_PATH];
	SYSTEMTIME ti;
	GetLocalTime( &ti );
	wsprintf( FileName, "GM_Counsel/%s%02d%02d%02d.txt", HERO->GetObjectName(), ti.wYear, ti.wMonth, ti.wDay );
	
	DIRECTORYMGR->SetLoadMode(eLM_Root);
	CreateDirectory( "GM_Counsel", NULL );
	
	HANDLE	hFile;
	hFile = CreateFile( FileName, GENERIC_WRITE, 0, NULL, 
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	
	if( hFile == INVALID_HANDLE_VALUE )
		return;
	
	DWORD dwWritten;
	SetFilePointer( hFile, 0, NULL, FILE_END );
	WriteFile( hFile, m_strChatList, m_nBufLen, &dwWritten, NULL );
	CloseHandle( hFile );

	m_strChatList[0] = 0;
	m_nBufLen = 0;
	
	SetWindowText( m_hWndChatList, "" );
}

#endif //_GMTOOL_




