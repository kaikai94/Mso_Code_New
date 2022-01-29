// CharMake.cpp: implementation of the CCharMake class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharMake.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cResourceManager.h"
#include "ObjectManager.h"
#include "GameResourceManager.h"
#include "MHCamera.h"
#include "MHTimeManager.h"
#include "MAINGAME.h"
#include "cWindowSystemFunc.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "HelperManager.h"
#include "CharMakeDialog.h"
#include "mhMap.h"
#include "./Input/UserInput.h"
#include "GMNotifyManager.h"

//VECTOR3				g_v3CreatePos = { 3600.0f, 0.0f, 1850.0f };
//VECTOR3				g_v3CreatePos = { 16021.421875f, 0.0f, 12884.078125f };

#define DEFAULT_JOB 0
GLOBALTON(CCharMake)
CCharMake::CCharMake()
{
	m_pCharMakeDlg = NULL;
	m_pCharMakeCamera = NULL;
}

CCharMake::~CCharMake()
{

}

BOOL CCharMake::Init(void* pInitParam)
{	
	m_imageLoginBarTop.LoadSprite("image/2D/login_bar00.TIF");
	m_imageLoginBarBottom.LoadSprite("image/2D/login_bar02.TIF");

	if(MAP->IsInited() == FALSE)
	{
		MAP->InitMap(CHARMAKEMGR->GetMakeCfg().wMapNum);
	}
	
	NETWORK->SetCurState(this);

	CreateCharMakeDlg();
	

	CHARMAKEMGR->Init();
	CHARMAKEMGR->CreateNewCharacter(&CHARMAKEMGR->GetMakeCfg().vCreatePos);	

	m_pCharMakeDlg->ChangeComboStatus( 0 );

	HELPERMGR->Init();
	HELPERMGR->ShowRespect( 1, FALSE );

	WINDOWMGR->AfterInit();
	

/*	vPivot.x = 23058 - 600; 

	vPivot.y = 530;
	vPivot.z = 31780 + 420;
	CAMERA->InitCamera(0, -4.0f, 90.0f, 4215, &vPivot); 
*/	

/*	
	VECTOR3 vPivot;
	vPivot.x = 24480;
	vPivot.y = 6020;
	vPivot.z = 9884;

	CAMERA->InitCamera(0, -6.0f, 0.0f, 2000, &vPivot);*/

	//VECTOR3 v3Eye		= { 3427.488525f, 410.0f, 1218.216919f };	
	//VECTOR3 v3Eye		= { 3427.488525f + 71.680961f, 410.0f -118.823507f, 1218.216919f + 237.419061f};	
	//VECTOR3 v3Rotate	= { -0.446804f, 0.293215f, 0.0f };

	VECTOR3 vPivot;
	vPivot.x = CHARMAKEMGR->GetMakeCfg().vCameraPos.x;
	vPivot.y = CHARMAKEMGR->GetMakeCfg().vCameraPos.y;
	vPivot.z = CHARMAKEMGR->GetMakeCfg().vCameraPos.z;
	CAMERA->InitCamera( CHARMAKEMGR->GetMakeCfg().wCameraNum, CHARMAKEMGR->GetMakeCfg().vAnglePos.x,CHARMAKEMGR->GetMakeCfg().vAnglePos.y, CHARMAKEMGR->GetMakeCfg().vAnglePos.z, &vPivot);


	//VECTOR3 v3Eye		= { 16307.261719f, 30.643303f, 12567.238281f };	
	//VECTOR3 v3Rotate	= { -6.136824f, -6.966158f, 0.0f };

	CAMERA->SetCurCamera(CHARMAKEMGR->GetMakeCfg().wCurCamera);
	CAMERA->SetCameraMode(CHARMAKEMGR->GetMakeCfg().wCameraMode);	

	if (NULL == m_pCharMakeCamera)
	{
		m_pCharMakeCamera = new CCharMakeCamera;
		m_pCharMakeCamera->InitCamera(g_pExecutive, &CHARMAKEMGR->GetMakeCfg().vEyePos, &CHARMAKEMGR->GetMakeCfg().vRotate, 0);
	}

	return TRUE;
}

void CCharMake::Release(CGameState* pNextGameState)
{
	m_imageLoginBarTop.Release();
	m_imageLoginBarBottom.Release();	

	if (NULL != m_pCharMakeCamera)
	{
		delete m_pCharMakeCamera;
		m_pCharMakeCamera = NULL;
	}

	OBJECTMGR->RemoveAllObject();
	
	WINDOWMGR->DestroyWindowAll();
	RESRCMGR->ReleaseResource(12);
	CHARMAKEMGR->Release();
	HELPERMGR->Release();
		
	if(pNextGameState == NULL)
		MAP->Release();
			
}

void CCharMake::Process()
{	
	if(IsBlock()) return;
	
	if(	!CHARMAKEMGR->GetNewPlayer() ) 
	{
		CHARMAKEMGR->CreateNewCharacter(&CHARMAKEMGR->GetMakeCfg().vCreatePos);	
	}
	
	CHARMAKEMGR->ApplyScale();

	static int s_iLastY = MOUSE->GetMouseY();
	static int s_iLastX = MOUSE->GetMouseX();	
	

	if( MOUSE->RButtonDrag() )
	{
		int iCurrentY		= MOUSE->GetMouseY();
		int iCurrentX		= MOUSE->GetMouseX();

		if (abs(iCurrentY - s_iLastY) > abs(iCurrentX - s_iLastX) )
		{

			GXOBJECT_HANDLE gxh = CHARMAKEMGR->GetNewPlayer()->GetEngineObject()->GetGXOHandle();		
			m_pCharMakeCamera->RotateCamera(gxh, 0, s_iLastY, 0, iCurrentY);		
		}
		else
		{
			CHARMAKEMGR->NewCharacterRotate( MOUSE->GetMouseAxisX() );
		}	

		s_iLastY = iCurrentY;
		s_iLastX = iCurrentX;		
	}
	
	int iWheel	= MOUSE->GetWheel();
	if (0 < iWheel)
	{
		m_pCharMakeCamera->ZoomInCamera();			
	}
	else if (0 > iWheel)
	{		
		m_pCharMakeCamera->ZoomOutCamera();		
	}
}

void CCharMake::BeforeRender()
{
	g_pExecutive->GetRenderer()->BeginRender(0,0,0);
	g_pExecutive->GetRenderer()->EndRender();
	MAP->Process( gCurTime );
}

void CCharMake::AfterRender()
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

void CCharMake::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_CHARACTER_NAMECHECK_NACK:
				{
					MSG_WORD* pmsg = (MSG_WORD*)pMsg;

					CHARMAKEMGR->GetCharacterMakeInfo()->bDuplCheck = FALSE;
					cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
					editboxName->SetEditText("");
				
					switch(pmsg->wData)
					{
					case 2:
						CHARMAKE->DisplayNotice( 14 );
						break;
					case 3:
						CHARMAKE->DisplayNotice( 14 );
						break;
					case 4:
						CHARMAKE->DisplayNotice( 14 );
						break;
					default:
						CHARMAKE->DisplayNotice( 14 );
						break;
					}
					CHARMAKE->SetDisableDlg( FALSE );
				}
				return;
			case MP_USERCONN_CHARACTER_NAMECHECK_ACK:
				{
					CHARMAKEMGR->GetCharacterMakeInfo()->bDuplCheck =TRUE;
					CHARMAKE->DisplayNotice( 13 );
					CHARMAKE->SetDisableDlg( FALSE );
				}
				return;
			case MP_USERCONN_CHARACTER_MAKE_NACK:
				{
					CHARMAKE->DisplayNotice( 16 );
					SetDisableDlg( FALSE );
				}
				return;
			case MP_USERCONN_CHARACTERLIST_ACK:
				{		
					SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
					SetDisableDlg( FALSE );
					MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));									
					CAMERA->MoveStart(eCP_CHARSEL);				
				}
				return;
			}
		}
		return;
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
		break;

	}
}


void CCharMake::DisplayNotice( int MsgNum )
{
	WINDOWMGR->MsgBox( MBI_CHARMAKEERROR, MBT_OK, CHATMGR->GetChatMsg(MsgNum) );
	SetDisableDlg( TRUE );
}

void CCharMake::SetDisableDlg( BOOL val )
{
	if( m_pCharMakeDlg )
		m_pCharMakeDlg->SetDisable( val );
}






