








// ExitManager.cpp: implementation of the CExitManager class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExitManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "MoveManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "cMsgBox.h"
#include "MainGame.h"
#include "PKManager.h"
#include "MHTimeManager.h"
#include "GameIn.h"
#include "MarryManager.h"
#include "AIManager.h"
#ifdef _XDYY_
#include "OnlineVoiceManager.h" //[小队语音][By:十里坡剑神][QQ:112582793][2019/3/27][10:33:36]
#endif
#include "GameCheckManager.h"
extern HWND _g_hWnd;
GLOBALTON(CExitManager);

CExitManager::CExitManager()
{
	m_bExitProcessing	= FALSE;
	m_dwStartTime		= 0;
	m_nExitCount		= EXIT_COUNT;
	m_nCount			= 0;
	m_bExitSync			= FALSE;
}

CExitManager::~CExitManager()
{

}


int CExitManager::CanExit()
{
	if( HERO->IsPKMode() )	
		return eEXITCODE_PKMODE;

	if( PKMGR->IsPKLooted() )	
		return eEXITCODE_LOOTING;

	if( HERO->GetState() == eObjectState_Exchange )	
		return eEXITCODE_NOT_ALLOW_STATE;

	if( HERO->GetState() == eObjectState_StreetStall_Owner ||
		HERO->GetState() == eObjectState_StreetStall_Guest )	
		return eEXITCODE_NOT_ALLOW_STATE;

	if( HERO->GetState() == eObjectState_Deal )	
		return eEXITCODE_NOT_ALLOW_STATE;

	return eEXITCODE_OK;
}


BOOL CExitManager::SendExitMsg( int nExitKind,BOOL bIsRobot )
{
	if( m_bExitSync || m_bExitProcessing ) return FALSE;

	m_nExitKind		= nExitKind;
	m_bExitSync		= TRUE;
	m_bIsRobot = bIsRobot;

	if(!bIsRobot)
	{
		HERO->DisableAutoAttack();					
		HERO->SetNextAction(NULL);				
		MOVEMGR->HeroMoveStop();
		OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Exit );
	}
	EXITMSG msg;
	msg.Category	= MP_CHAR;
	msg.Protocol	= MP_CHAR_EXITSTART_SYN;
	msg.dwObjectID	= HEROID;
	msg.bIsRobot = m_bIsRobot;//[假人添加测试][By:十里坡剑神][QQ:112582793][2018/4/22]
	NETWORK->Send( &msg, sizeof(msg) );
	
	return TRUE;
}

void CExitManager::StartExit()
{
	//sCPlayerAI->SetOpen(FALSE);
	sCPlayerAI->SaveConfig();
	GAMEIN->UpdataGoldMoney();  //[在线充值刷新元宝][By:十里坡剑神][QQ:112582793][2017/12/5]
	m_bExitProcessing	= TRUE;
	DWORD dwCurTime = MHTIMEMGR->GetNewCalcCurTime();
	m_dwStartTime		= dwCurTime - 1000;
	m_nCount			= m_nExitCount;
	//[夫妻下线提示][By:十里坡剑神][QQ:112582793][2018/9/10]
	HERO->ClearWayPoint();
	MARRYMGR->SendLoginOutNotice();
	sGAMECHECK->SetExit(TRUE);
	sGAMECHECK->SetEventHandle();
#ifdef _XDYY_
	//[小队语音退出房间][By:十里坡剑神][QQ:112582793][2019/3/20][23:44:34]
	VOICEMGR->QuiteRoom();
	VOICEMGR->ClearVoice();
#endif
}


void CExitManager::RejectExit( int nExitCode )

{

	m_bExitSync			= FALSE;
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Exit );

	switch( nExitCode )

	{
	case eEXITCODE_PKMODE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	//[暴露杀气状态下，不能结束游戏][By:十里坡剑神][QQ:112582793][2018/5/13]
		break;
	case eEXITCODE_LOOTING:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	//[被抢夺过程中不能结束游戏][By:十里坡剑神][QQ:112582793][2018/5/13]
		break;
	case eEXITCODE_NOT_IN_LOGIN_MAP:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2300) );	//[离线不在登陆点地图][By:十里坡剑神][QQ:112582793][2018/5/13]
		break;
	default:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	//[目前不能结束游戏][By:十里坡剑神][QQ:112582793][2018/5/13]
		break;
	}
	
}

void CExitManager::Exit()
{
	if( m_nExitKind == eEK_GAMEEXIT )
	{
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 307 ) );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
	else if( m_nExitKind == eEK_CHARSEL )
	{
		MSGBASE data;
		data.Category	= MP_USERCONN;
		data.Protocol	= MP_USERCONN_BACKTOCHARSEL_SYN;
		data.dwObjectID	= HEROID;
		NETWORK->Send(&data,sizeof(data));
		WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 290 ) );
		
	}
	
	m_bExitSync = FALSE;
}


void CExitManager::CancelExit( int nErrorCode )	
{
	m_bExitProcessing	= FALSE;

	m_bExitSync			= FALSE;
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Exit );

	switch( nErrorCode )
	{

	case eEXITCODE_PKMODE:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );
		break;
	case eEXITCODE_LOOTING:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );
		break;
	case eEXITCODE_SPEEDHACK:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(556) );
		break;
	case eEXITCODE_DIEONEXIT:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(555) );
		break;
	default:
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );
		break;

	}	
}

void CExitManager::Process()
{
	if( !m_bExitProcessing || !m_bExitSync ) return;

	if( gCurTime - m_dwStartTime >= 1000 )
	{
		m_dwStartTime += 1000;
		
		if( m_nCount <= 0  )
		{
			m_bExitProcessing = FALSE;
			
			EXITMSG msg;

			msg.Category	= MP_CHAR;

			msg.Protocol	= MP_CHAR_EXIT_SYN;
			msg.dwObjectID	= HEROID;
			msg.bIsRobot	= m_bIsRobot;
			NETWORK->Send( &msg, sizeof(msg) );

		}
		else
		{
			if( m_nExitKind == eEK_GAMEEXIT )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 540 ), m_nCount );
			else if( m_nExitKind == eEK_CHARSEL )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 541 ), m_nCount );
		}

		--m_nCount;	
	}
}




