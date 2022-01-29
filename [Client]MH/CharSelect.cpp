// CharSelect.cpp: implementation of the CCharSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharSelect.h"
#include "MainGame.h"
#include "MainTitle.h"
#include "cWindowSystemFunc.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cResourceManager.h"
#include "ObjectManager.h"
#include "MHTimeManager.h"
#include "ChatManager.h"
#include "OptionManager.h"
#include "MHMap.h"
#include "MHCamera.h"
#include "./Interface/cScriptManager.h"
#include "GameResourceManager.h"
#include "cMsgBox.h"
#include "./Input/UserInput.h"
#include "GMNotifyManager.h"
//#ifdef TAIWAN_LOCAL
//#include "BillingManager.h"
//#endif


extern char RobotName[];


GLOBALTON(CCharSelect)
CCharSelect::CCharSelect()
{
	m_CurSelectedPlayer = -1;
	m_pDoublcClickedPlayer = NULL;

	ZeroMemory( m_pPlayer, sizeof(CPlayer*)*MAX_CHARACTER_NUM );

	m_pSelectDlg = NULL;
	m_pChannelDlg = NULL;

	m_ExtraCharacterCount = 0;

	m_bEnterGame = FALSE;
	LoadCharSelectInfo();
}

CCharSelect::~CCharSelect()
{
	
}
void CCharSelect::LoadCharSelectInfo()
{
	char buff[64]={0,};
	CMHFile file;
	if(file.Init("./Resource/Client/CharSelect.bin","rb")==FALSE)
		return;
	while(1)
	{
		if(file.IsEOF())
			break;
		file.GetString(buff);
		START_STRING_COMPARE(buff)
			COMPARE("#MAPNUM")
			sCharSelect.wMapNum = file.GetWord();
			COMPARE("#STANDPOS")
				sCharSelect.vStandPos.x = file.GetFloat();
				sCharSelect.vStandPos.y = file.GetFloat();
				sCharSelect.vStandPos.z = file.GetFloat();
			COMPARE("#STANDHEIGHT")
				sCharSelect.wStandHeight = file.GetWord();
			COMPARE("#MANSTANDPOS")
				for(int i= 0;i<4;i++)
				{
					sCharSelect.vManStandPos[i].x = file.GetFloat();
					sCharSelect.vManStandPos[i].y = file.GetFloat();
					sCharSelect.vManStandPos[i].z = file.GetFloat();
				}
			COMPARE("#WOMANSTANDPOS")
				for(int i= 0;i<4;i++)
				{
					sCharSelect.vWomanStandPos[i].x = file.GetFloat();
					sCharSelect.vWomanStandPos[i].y = file.GetFloat();
					sCharSelect.vWomanStandPos[i].z = file.GetFloat();
				}
			COMPARE("#COUNT")
				sCharSelect.wCount[0] = file.GetWord();
				sCharSelect.wCount[1] = file.GetWord();
			COMPARE("#SHADOWPOS")
				sCharSelect.vShadowPos.x = file.GetFloat();
				sCharSelect.vShadowPos.y = file.GetFloat();
			COMPARE("#HEIGHTPOS1")
				sCharSelect.vHeightPos1.x = file.GetFloat();
				sCharSelect.vHeightPos1.y = file.GetFloat();
			COMPARE("#HEIGHTPOS2")
				sCharSelect.vHeightPos2.x = file.GetFloat();
				sCharSelect.vHeightPos2.y = file.GetFloat();
			COMPARE("#ANGLE")
				sCharSelect.vAngle[0]	  = file.GetFloat();
				sCharSelect.vAngle[1]	  = file.GetFloat();
				sCharSelect.vAngle[2]	  = file.GetFloat();
				sCharSelect.vAngle[3]	  = file.GetFloat();
			COMPARE("#MONTION")
				sCharSelect.wMontion = file.GetWord();
			COMPARE("#BASEMOTION")
				sCharSelect.wBaseMotion[0] = file.GetWord();
				sCharSelect.wBaseMotion[1] = file.GetWord();
			COMPARE("#CAMERAPOINT")
				sCharSelect.vCameraPoint.x = file.GetFloat();
				sCharSelect.vCameraPoint.y = file.GetFloat();
				sCharSelect.vCameraPoint.z = file.GetFloat();
			COMPARE("#CAMERANUM")
				sCharSelect.wCameraNum = file.GetWord();
			COMPARE("#ANGLEPOS")
				sCharSelect.fAngle.x = file.GetFloat();
				sCharSelect.fAngle.y = file.GetFloat();
				sCharSelect.fAngle.z = file.GetFloat();
			COMPARE("#CAMERAMODE")
				sCharSelect.wCameraMode = file.GetWord();
			COMPARE("#CURCAMERA")
				sCharSelect.wCurCamera = file.GetWord();
		END_COMPARE
	}
	file.Release();
}
BOOL CCharSelect::Init(void* pInitParam)
{	
	m_imageLoginBarTop.LoadSprite("image/2D/login_bar00.TIF");
	m_imageLoginBarBottom.LoadSprite("image/2D/login_bar02.TIF");	

	if(MAP->IsInited() == FALSE)
	{
		MAP->InitMap(sCharSelect.wMapNum);
	}
	else
	{
		MAP->Release();
		MAP->InitMap(sCharSelect.wMapNum);
	}

	ZeroMemory( m_pPlayer, MAX_CHARACTER_NUM * sizeof(sizeof(CPlayer*)) );
	
	NETWORK->SetCurState(this);


	NewCreateCharSelect_m();
	m_pSelectDlg = WINDOWMGR->GetWindowForID( CS_CHARSELECTDLG );
	CreateChannelDlg();
	m_pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
//#ifndef _KOR_LOCAL_
//	if(m_pChannelDlg)
//	{
//		m_pChannelDlg->Linking();
//	}
//#endif

	WINDOWMGR->AfterInit();
	ASSERT(pInitParam);
	SEND_CHARSELECT_INFO * pmsg = (SEND_CHARSELECT_INFO *)pInitParam;
	NetworkMsgParse(pmsg->Category, pmsg->Protocol, pInitParam);

	VECTOR3 pos = {0,0,0};
	float xpos = sCharSelect.vStandPos.x;
	float zpos = sCharSelect.vStandPos.y;
	float gap = sCharSelect.vStandPos.z;
	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{
		pos.x = xpos + (n *sCharSelect.wCount[0]);
		pos.y = sCharSelect.wStandHeight;
		pos.z = zpos + sCharSelect.wCount[1] * n;
	}

	pos.x = sCharSelect.vShadowPos.x;
	pos.z = sCharSelect.vShadowPos.y;

	MAP->SetShadowPivotPos(&pos);
	InitCameraInCharSelect();
	
	m_CurSelectedPlayer = -1;
	SelectPlayer( -1 );
	m_pDoublcClickedPlayer = NULL;
	SetDisablePick( FALSE );

	m_bBackToMainTitle = FALSE;
	m_bEnterGame = FALSE;

	return TRUE;
}

void CCharSelect::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	

	OBJECTMGR->RemoveAllObject();
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(11);
		
	if(pNextGameState == NULL || m_bEnterGame )
		MAP->Release();
}

void CCharSelect::Process()
{
	if( m_pDoublcClickedPlayer )
	{
		int iMotionNum = m_pDoublcClickedPlayer->GetEngineObject()->GetCurMotion();

		if( iMotionNum == eMotion_Gum_AfterStop ||
			iMotionNum == eMotion_Kwon_AfterStop ||
			iMotionNum == eMotion_Do_AfterStop ||
			iMotionNum == eMotion_Chang_AfterStop ||
			iMotionNum == eMotion_Gung_AfterStop ||

			iMotionNum == eMotion_Amki_AfterStop)		

		{
			SendMsgGetChannelInfo();
			m_pDoublcClickedPlayer = NULL;

		}
	}	

	if( WINDOWMGR->IsMouseInputProcessed() ) return;
	if( m_bDiablePick ) return;	

	if( MOUSE->LButtonDoubleClick() )
	{
		CPlayer* pPlayer = (CPlayer*)GetSelectedObjectBoneCheck( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());

		if( pPlayer && m_pDoublcClickedPlayer == NULL )
		if( pPlayer == GetCurSelectedPlayer() )
		{
			m_pDoublcClickedPlayer = pPlayer;
			SetDisablePick( TRUE );
		}
	}
	else if( MOUSE->LButtonDown()  )
	{		
		if( GetCurSelectedPlayer() )		
		{
			int iMotionNumber = GetCurSelectedPlayer()->GetEngineObject()->GetCurMotion();

			if( iMotionNumber != eMotion_Gum_AfterStop &&
				iMotionNumber != eMotion_Kwon_AfterStop &&
				iMotionNumber != eMotion_Do_AfterStop &&
				iMotionNumber != eMotion_Chang_AfterStop &&
				iMotionNumber != eMotion_Gung_AfterStop &&
				iMotionNumber != eMotion_Amki_AfterStop)		
		
			{
				return;				
			}

		}
		CPlayer* pPlayer = (CPlayer*)GetSelectedObjectBoneCheck( MOUSE->GetMouseEventX(), MOUSE->GetMouseEventY());

		if( pPlayer && m_pDoublcClickedPlayer == NULL )
		{
			for( int i = 0 ; i < MAX_CHARACTER_NUM ; ++i )
			{
				if( m_pPlayer[i] == pPlayer )
				{
 					SelectPlayer( i );
					break;
				}
			}
		}
	}
}

void CCharSelect::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();

	MAP->Process( gCurTime );
}

void CCharSelect::AfterRender()
{
	VECTOR2 v2TopPos = { 0, 0 };
	VECTOR2 v2BottomPos = { 0, 640 	};

	VECTOR2 PosScale = { 1, 1 };


	float scaleRate = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth / MIDRSLTN_W;
	v2BottomPos.y = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight-128;
	PosScale.x =scaleRate;

	m_imageLoginBarTop.RenderSprite(&PosScale,0,0,&v2TopPos,0xffffffff);
	m_imageLoginBarBottom.RenderSprite(&PosScale,0,0,&v2BottomPos,0xffffffff);

	WINDOWMGR->Render();
}

CPlayer* CCharSelect::GetCurSelectedPlayer()
{
	if(m_CurSelectedPlayer == -1)
		return NULL;
	CPlayer* pPlayer = m_pPlayer[m_CurSelectedPlayer];
		return pPlayer;
}

void CCharSelect::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
//#ifdef TAIWAN_LOCAL
//			case MP_USERCONN_REMAINTIME_NOTIFY:
//				{
//					MSG_INT* pmsg = (MSG_INT*)pMsg;
//					BILLINGMGR->SetUseRemainTime(TRUE);
//					BILLINGMGR->SetRemainTime( pmsg->nData );	
//				}
//				break;
//#endif
				
			case MP_USERCONN_CHARACTERLIST_ACK:
				{	
					
					SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;

					memset(m_pPlayer,0,sizeof(CPlayer*)*MAX_CHARACTER_NUM);

					VECTOR3 pos;

					DWORD basiccolor = RGBA_MAKE(255,255,255,255);
					DWORD overcolor = RGBA_MAKE(255,255,255,255);
					DWORD presscolor = RGBA_MAKE(255,255,0,255);

					int t = -1;
					for(int i = 0; i < pmsg->CharNum ; i++)
					{
						WORD standpoint = pmsg->StandingArrayNum[i];
						if(sCharSelect.wMontion==1)
							pmsg->ChrTotalInfo[i].WearedItemIdx[eWearedItem_Weapon] = 0; //登录角色取消武器
						pmsg->ChrTotalInfo[i].MunpaID = 0;
						pmsg->ChrTotalInfo[i].MarkName = 0;
						
						m_pPlayer[standpoint] = OBJECTMGR->AddPlayer(&pmsg->BaseObjectInfo[i],NULL,&pmsg->ChrTotalInfo[i],NULL,NULL,FALSE);
						CPlayer* pPlayer = m_pPlayer[standpoint];						

						pPlayer->GetEngineObject()->ApplyHeightField(FALSE);
						pPlayer->SetOverInfoOption(0);

						if (1 == pPlayer->GetGender())
						{//[角色站立位置调整][By:十里坡剑神][QQ:112582793][2018/10/2]
							pos = sCharSelect.vWomanStandPos[standpoint];
						}
						else
						{
							pos = sCharSelect.vManStandPos[standpoint];
						}		
											
						char LevelName[255];

						cPushupButton * btn = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+standpoint);
						wsprintf(LevelName,"[Lv%2d] %-16s",pmsg->ChrTotalInfo[i].Level,pmsg->BaseObjectInfo[i].ObjectName );
						btn->SetText(LevelName, basiccolor, overcolor, presscolor);
						cImage ToolTipImg;
						SCRIPTMGR->GetImage( 63, &ToolTipImg, PFT_HARDPATH );
						btn->SetToolTip( MAP->GetMapName(pmsg->ChrTotalInfo[i].LoginMapNum), RGB_HALF(255,255,255), &ToolTipImg, TTCLR_ITEM_CANEQUIP );


						pPlayer->SetPosition(&pos);						
						pPlayer->SetAngle(sCharSelect.vAngle[standpoint]);
						//[角色动作设置][By:十里坡剑神][QQ:112582793][2018/10/2]
						if(sCharSelect.wMontion==0)
						{
							if( standpoint == 4 )	
							{//[角色场景位置调整][By:十里坡剑神][QQ:112582793][2018/10/2]
								pos.x -= ( pmsg->ChrTotalInfo[standpoint].Height - sCharSelect.vHeightPos1.x ) * sCharSelect.vHeightPos1.y;
								pos.z += ( pmsg->ChrTotalInfo[standpoint].Height - sCharSelect.vHeightPos2.x ) * sCharSelect.vHeightPos2.y;
							}
							
							DWORD dwWeaponType = pPlayer->GetWeaponEquipType();
							dwWeaponType = CheckWeaponType(dwWeaponType);	
							pPlayer->ChangeMotion(eMotion_Gum_Before + 4*(dwWeaponType-1),0);
							pPlayer->ChangeBaseMotion(eMotion_Gum_Before + 4*(dwWeaponType-1));
						}
						else if(sCharSelect.wMontion==1)
						{
							if(standpoint<4)  // 第一个角色设置动作和面向 
							{
								pPlayer->ChangeBaseMotion(eMotion_Sel1Before+4*standpoint);
								pPlayer->ChangeMotion(eMotion_Sel1Before+4*standpoint, 0);
							}
						}

						pPlayer->ShowObjectName( FALSE );
						pPlayer->GetEngineObject()->EnableShadow(TRUE);
						
					}

					InitCameraInCharSelect();
				}				
				return;
			case MP_USERCONN_CHARACTERLIST_NACK:
				{
					LOG(EC_CHARACTERLIST_FAILED);
				}				
				return;
			case MP_USERCONN_CHARACTER_REMOVE_ACK:
				{
					CPlayer* pPlayer = GetCurSelectedPlayer();
					if(!pPlayer)
					{
						ASSERTMSG(0, "某腐磐 昏力-敲饭捞绢啊 绝嚼聪促.No Character For Delete!");
						return;
					}				
					
					cPushupButton * editboxName = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN + m_CurSelectedPlayer);
					editboxName->SetText("",RGBA_MAKE(255,255,255,255),NULL,NULL);
					editboxName->SetPush(FALSE);
					
					m_pPlayer[m_CurSelectedPlayer] = NULL;
					m_CurSelectedPlayer = -1;
					OBJECTMGR->AddGarbageObject(pPlayer);
				
					SelectPlayer(-1);
					SetDisablePick( FALSE );
				}
				return;
			case MP_USERCONN_CHARACTER_REMOVE_NACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					if(pmsg->dwData == 4)
						WINDOWMGR->MsgBox( MBI_DELETECHARFAIL, MBT_OK, CHATMGR->GetChatMsg(2284) );
					else if( pmsg->dwData == 3 )
						WINDOWMGR->MsgBox( MBI_DELETECHARFAIL, MBT_OK, CHATMGR->GetChatMsg(995) );
					else
						WINDOWMGR->MsgBox( MBI_DELETECHARFAIL, MBT_OK, CHATMGR->GetChatMsg(25) );

					SetDisablePick( FALSE );
				}
				return;
			
			case MP_USERCONN_DISCONNECT_ACK:
				{
					SetDisablePick( TRUE );
					m_bBackToMainTitle = TRUE;
				}
				return;
			case MP_USERCONN_CHANNELINFO_ACK:
				{
					MSG_CHANNEL_INFO * pmsg = (MSG_CHANNEL_INFO*)pMsg;
					GetChannelDialog()->SetChannelList(pmsg);
					SetDisablePick(TRUE);
					
					if(pmsg->Count == 1)
					{
						GetChannelDialog()->OnConnect();	
						GetChannelDialog()->SetActive(FALSE);
					}
				}
				return;
			case MP_USERCONN_CHANNELINFO_NACK:
				{
					MSG_BYTE* msg = (MSG_BYTE*)pMsg;
					if( msg->bData == 0 )
					{
						WINDOWMGR->MsgBox( MBI_MAPSERVER_CLOSE, MBT_OK, CHATMGR->GetChatMsg(449) );
					}
					else
					{						
						WINDOWMGR->MsgBox( MBI_MAPSERVER_CLOSE, MBT_OK, CHATMGR->GetChatMsg(450) );
					}
				}
				return;
			}
		}
		break; 
	case MP_CHEAT:
		{
			switch(Protocol) 
			{
				case MP_CHEAT_EVENTNOTIFY_ON:
				{
					MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

					NOTIFYMGR->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
					NOTIFYMGR->SetEventNotify( TRUE );
					NOTIFYMGR->SetEventNotifyChanged( TRUE );
					
					NOTIFYMGR->ResetEventApply();
					for(int i=0; i<eEvent_Max; ++i)
					{
						if( pmsg->EventList[i] )
							NOTIFYMGR->SetEventApply( i );
					}						
				}
				break;
		
				case MP_CHEAT_EVENTNOTIFY_OFF:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					NOTIFYMGR->SetEventNotify( FALSE );
					NOTIFYMGR->SetEventNotifyChanged( FALSE );
				}	
				break;

			}
		}		
	}
}

void CCharSelect::InitCameraInCharSelect()
{
	VECTOR3 vPivot;
	
	vPivot.x = sCharSelect.vCameraPoint.x;
	vPivot.y = sCharSelect.vCameraPoint.y;
	vPivot.z = sCharSelect.vCameraPoint.z;
	CAMERA->InitCamera( sCharSelect.wCameraNum, sCharSelect.fAngle.x, sCharSelect.fAngle.y, sCharSelect.fAngle.z, &vPivot);

	CAMERA->SetCameraMode(sCharSelect.wCameraMode);
	CAMERA->SetCurCamera(sCharSelect.wCurCamera);
}

void CCharSelect::SetCurSelctedPlayer(int num)
{	
	cPushupButton * charBtn[MAX_CHARACTER_NUM];

	if(num == -1)
	{
		for(int n=0;n<MAX_CHARACTER_NUM;++n)
		{
			charBtn[n] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+n);
			charBtn[n]->SetPush(FALSE);
		}
		m_CurSelectedPlayer = -1;
		return;
	}
	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{

		charBtn[n] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+n);
		if(n == num)
			charBtn[n]->SetPush(TRUE);
		else
			charBtn[n]->SetPush(FALSE);
	}
	m_CurSelectedPlayer = num;
}

void CCharSelect::SelectPlayer(int num)
{
	if( GetCurSelectedPlayer() )
	{
		int iMotionNum = GetCurSelectedPlayer()->GetEngineObject()->GetCurMotion();

		if( iMotionNum != eMotion_Gum_AfterStop &&
			iMotionNum != eMotion_Kwon_AfterStop &&
			iMotionNum != eMotion_Do_AfterStop &&
			iMotionNum != eMotion_Chang_AfterStop &&
			iMotionNum != eMotion_Gung_AfterStop &&
			iMotionNum != eMotion_Amki_AfterStop)		

		{

			cPushupButton * charBtn = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN + num);
			charBtn->SetPush( FALSE );
			return;
		}
	}
	
	if( num == m_CurSelectedPlayer) 
	{
		return;
	}

	if(num != -1)
	if(m_pPlayer[num] == NULL)
	{
			cPushupButton * charBtn[MAX_CHARACTER_NUM];
			charBtn[num] = (cPushupButton *)WINDOWMGR->GetWindowForIDEx(MT_FIRSTCHOSEBTN+num);
			charBtn[num]->SetPush(FALSE);
			return;
	}
	
	PlayDeselectedMotion(m_CurSelectedPlayer);
	PlaySelectedMotion(num);
	SetCurSelctedPlayer(num);
}

BOOL CCharSelect::IsFull()
{
//#ifdef _JAPAN_LOCAL_
//
//	for(DWORD n=0; n<GIVEN_CHARACTER_SLOT+m_ExtraCharacterCount; ++n)
//	{
//		if(m_pPlayer[n] == NULL)
//			return FALSE;
//	}
//	
//#elif defined _HK_LOCAL_
//
//	for(int n=0; n<GIVEN_CHARACTER_SLOT+m_ExtraCharacterCount; ++n)
//	{
//		if(m_pPlayer[n] == NULL)
//			return FALSE;
//	}
//#elif defined _TL_LOCAL_
//
//	for(int n=0; n<GIVEN_CHARACTER_SLOT+m_ExtraCharacterCount; ++n)
//	{
//		if(m_pPlayer[n] == NULL)
//			return FALSE;
//	}
//	
//#else

	for(int n=0;n<MAX_CHARACTER_NUM;++n)
	{
		if(m_pPlayer[n] == NULL)
			return FALSE;
	}

//#endif

	return TRUE;
}


BOOL CCharSelect::EnterGame()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(pPlayer == NULL)
		return FALSE;

//#ifdef _JAPAN_LOCAL_
//	if( gChannelNum == 1 )
//	if( pPlayer->GetBadFame() > 0 )
//	{
//		DisplayNotice( 1490 );
//		return TRUE;
//	}
//#endif

	GameLoadingParam loadingParam;
	loadingParam.selectedCharNum = pPlayer->GetID();
	loadingParam.m_MapNum = pPlayer->GetCharacterTotalInfo()->LoginMapNum;

	MAINGAME->SetGameState(eGAMESTATE_GAMELOADING, (void *)&loadingParam, sizeof(GameLoadingParam));

	m_bEnterGame = TRUE;

	CAMERA->ResetInterPolation();

	return TRUE;
}

void CCharSelect::DeleteCharacter()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();
	if(pPlayer == NULL)
		return;
		
	MSG_DWORD  msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REMOVE_SYN;
	msg.dwData = pPlayer->GetID();
	NETWORK->Send(&msg,sizeof(msg));
}

void CCharSelect::PlaySelectedMotion(int num)
{
	if(num == -1)
		return;
	if(m_pPlayer[num] == NULL)
		return;

 //[角色选中后动作设置][By:十里坡剑神][QQ:112582793][2018/10/2]
	if(sCharSelect.wMontion==0)
	{
		DWORD dwWeaponType = m_pPlayer[num]->GetWeaponEquipType();
		dwWeaponType = CheckWeaponType(dwWeaponType);	
		m_pPlayer[num]->ChangeMotion(eMotion_Gum_After + 4*(dwWeaponType-1),0);
		m_pPlayer[num]->ChangeBaseMotion(eMotion_Gum_AfterStop + 4*(dwWeaponType-1));
	}
	else if(sCharSelect.wMontion==1)
	{
		m_pPlayer[num]->ChangeMotion(eMotion_sel1After+4*num,0);
		m_pPlayer[num]->ChangeBaseMotion(eMotion_Sel1Afterstop+4*num);
	}
}
void CCharSelect::PlayDeselectedMotion(int num)
{
	if(num == -1)
		return;
	if(m_pPlayer[num] == NULL)
		return;
			
 //[角色放弃选择动作][By:十里坡剑神][QQ:112582793][2018/10/2]
	if(sCharSelect.wMontion==0)
	{
		DWORD dwWeaponType = m_pPlayer[num]->GetWeaponEquipType();
		dwWeaponType = CheckWeaponType(dwWeaponType);	
		m_pPlayer[num]->ChangeMotion(eMotion_Gum_Back + 4*(dwWeaponType-1),0);
		m_pPlayer[num]->ChangeBaseMotion(eMotion_Gum_Before + 4*(dwWeaponType-1));
	}
	else if(sCharSelect.wMontion==1)
	{
		m_pPlayer[num]->ChangeMotion(eMotion_Sel1Back+4*num,0);
		m_pPlayer[num]->ChangeBaseMotion(eMotion_Sel1Before+4*num);
	}
}

void CCharSelect::DisplayNotice(int MsgNum)
{
	WINDOWMGR->MsgBox( MBI_CHARSELERROR, MBT_OK, CHATMGR->GetChatMsg(MsgNum) );
	SetDisablePick( TRUE );
}


void CCharSelect::SetDisablePick( BOOL val )
{
	m_bDiablePick = val;
	if( m_pSelectDlg )
		m_pSelectDlg->SetDisable( val );
}

void CCharSelect::BackToMainTitle()
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DISCONNECT_SYN;
	NETWORK->Send(&msg,sizeof(msg));
	SetBlock(TRUE);
	gUserID = 0;
	SetDisablePick(TRUE);
	m_bBackToMainTitle = TRUE;
}

void CCharSelect::OnDisconnect()
{
	if( m_bBackToMainTitle )
	{
		SetBlock(FALSE);
		OBJECTMGR->RemoveAllObject();
		MAINGAME->SetGameState(eGAMESTATE_TITLE);
		m_bBackToMainTitle = FALSE;
		NETWORK->ReleaseKey();
	}
	else
	{
		CGameState::OnDisconnect();
	}
}
#include "ReConnect.h"
void CCharSelect::SendMsgGetChannelInfo()
{
	CPlayer* pPlayer = GetCurSelectedPlayer();

	if(pPlayer == NULL)
	{
		DisplayNotice(18);
		return;
	}

	MSGSHANNELINFOSYN msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
	msg.dwObjectID = pPlayer->GetID();
	msg.bIsReConn = FALSE;
	msg.wMapNum = 0;
	REDISCONNECT->OnSelectPlayer(msg.dwObjectID);
	NETWORK->Send( &msg, sizeof(msg) );
}

DWORD CCharSelect::CheckWeaponType(DWORD dwWeaponType)
{
	if(dwWeaponType == WP_EVENT)
	{
		dwWeaponType = WP_GWUN;
	}
	else if(dwWeaponType == WP_EVENT_HAMMER)
	{
		dwWeaponType = WP_GUM;
	}
	return dwWeaponType;
}




