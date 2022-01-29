







// MouseEventThrower.cpp: implementation of the CMouseEventThrower class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MouseEventThrower.h"
#include "MouseEventReceiver.h"
#include "Object.h"
#include "interface\cWindowManager.h"


#include "MousePointer.h"

#include "ObjectManager.h"
#include "gamein.h"
#include "MouseCursor.h"
#include "MHMap.h"

#include "MonsterGuageDlg.h"
#include "CharacterRBTN.h"   //[角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
#include "cInventoryRBTDlg.h"//[背包右键菜单][By:十里坡剑神][QQ:112582793][2018/4/3]
#include "cSendItemInfoDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMouseEventThrower::CMouseEventThrower()
{
	m_pCurrentReceiver	= NULL;
	m_pDefaultReceiver	= NULL;

	m_bCancelSelect		= FALSE;
}

CMouseEventThrower::~CMouseEventThrower()
{

}

void CMouseEventThrower::SetDefaultReceiver(CMouseEventReceiver* pDefaultReceiver)
{
	m_pDefaultReceiver = pDefaultReceiver;
}


void CMouseEventThrower::SetCurrentReceiver(CMouseEventReceiver* pCurrentReceiver)
{
	m_pCurrentReceiver = pCurrentReceiver;
}

void CMouseEventThrower::SetReceiverToDefault()
{
	ASSERT(m_pDefaultReceiver);
	m_pCurrentReceiver = m_pDefaultReceiver;
}


#define MOUSE_MOVE_TICK		400
#define MOUSE_PRESS_TICK	400
#define MOUSE_ON_TICK		200

void CMouseEventThrower::Process(CMouse* Mouse)
{
	static CObject* pOldMouseOverObject		= NULL;
	static DWORD dwMouseMoveTick			= 0;
	static DWORD dwMouseOnTick				= 0;
	static DWORD dwMousePressTick			= 0;

	if( WINDOWMGR->IsMouseInputProcessed() || WINDOWMGR->IsDragWindow() )
	{
		if( pOldMouseOverObject )
		{
			m_pCurrentReceiver->OnMouseLeave( pOldMouseOverObject );
			pOldMouseOverObject = NULL;
		}
		
		dwMousePressTick = gCurTime;
		return;
	}

	DWORD MouseEvent = MOUSEEVENT_NONE;

	if( Mouse->LButtonDoubleClick() || Mouse->RButtonDoubleClick() )
		MouseEvent = MOUSEEVENT_DBLCLICK;
	else
	{
		if( Mouse->LButtonDown() )
		{
			MouseEvent = MOUSEEVENT_LCLICK;
			if (GAMEIN->GetCharacterRBTNDlg())  //[目标改变关闭角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
			{
				if (GAMEIN->GetCharacterRBTNDlg()->IsActive())
					GAMEIN->GetCharacterRBTNDlg()->Close();
			}
			if (GAMEIN->GetInventoryRBTDlg())
			{
				if (GAMEIN->GetInventoryRBTDlg()->IsActive())
					GAMEIN->GetInventoryRBTDlg()->SetActive(FALSE);
			}
			if (GAMEIN->GetSendItemInfoDlg())
			{
				if (GAMEIN->GetSendItemInfoDlg()->IsActive())
					GAMEIN->GetSendItemInfoDlg()->SetActive(FALSE);
			}
			
		}
		else if( Mouse->RButtonDown() )
		{
			MouseEvent = MOUSEEVENT_RCLICK;
			if (GAMEIN->GetCharacterRBTNDlg())  //[目标改变关闭角色右键菜单][By:十里坡剑神][QQ:112582793][2017/11/23]
			{
				if (GAMEIN->GetCharacterRBTNDlg()->IsActive())
					GAMEIN->GetCharacterRBTNDlg()->Close();
			}
			if (GAMEIN->GetInventoryRBTDlg())
			{
				if (GAMEIN->GetInventoryRBTDlg()->IsActive())
					GAMEIN->GetInventoryRBTDlg()->SetActive(FALSE);
			}
			if (GAMEIN->GetSendItemInfoDlg())
			{
				if (GAMEIN->GetSendItemInfoDlg()->IsActive())
					GAMEIN->GetSendItemInfoDlg()->SetActive(FALSE);
			}
		}


		if( Mouse->LButtonPressed() )
			MouseEvent |= MOUSEEVENT_LPRESS;
	}


//---某腐磐 急琶 秦力包访
	if( Mouse->RButtonDown() )	m_bCancelSelect = TRUE;

	if( Mouse->RButtonDrag() )	m_bCancelSelect = FALSE;

	if( OBJECTMGR->GetSelectedObjectID() && Mouse->RButtonUp() && m_bCancelSelect )
	{
		OBJECTMGR->SetSelectedObject( NULL );
//		if( GAMEIN->GetMonsterGuageDlg())
//			GAMEIN->GetMonsterGuageDlg()->SetActive(FALSE);
//		if(GAMEIN->GetPlayerGuageDlg())
//			GAMEIN->GetPlayerGuageDlg()->SetActive(FALSE);

		m_bCancelSelect = FALSE;
	}
	
	if( MouseEvent == MOUSEEVENT_NONE )
	{
		if( gCurTime - dwMouseOnTick > MOUSE_ON_TICK )
		{

			dwMouseOnTick = gCurTime;

			CObject* pOverObject = (CObject*)GetSelectedObject( Mouse->GetMouseX(), Mouse->GetMouseY() );
			
			if( pOldMouseOverObject != pOverObject )
			{
				m_pCurrentReceiver->OnMouseLeave( pOldMouseOverObject );
				m_pCurrentReceiver->OnMouseOver( pOverObject );
				
				pOldMouseOverObject = pOverObject;
			}
			if( pOverObject == NULL )
			{
				if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
                    CURSOR->SetCursor( eCURSOR_DEFAULT );
			}
		}
	}
	else
	{
		CObject* pObject = (CObject*)GetSelectedObject( Mouse->GetMouseEventX(), Mouse->GetMouseEventY() );

		if( pObject )
		{
			if( pObject->GetEngineObjectType() == eEngineObjectType_Character ||
				pObject->GetEngineObjectType() == eEngineObjectType_Titan )
			{
				//if(pObject->IsDied())return;//角色死亡后不可选中	By:十里坡剑神	QQ:112582793
				m_pCurrentReceiver->OnClickPlayer(MouseEvent,(CPlayer*)pObject);					

			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_Monster)
			{

				m_pCurrentReceiver->OnClickMonster(MouseEvent,(CMonster*)pObject);
			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_Npc)
			{

				m_pCurrentReceiver->OnClickNpc(MouseEvent,(CNpc*)pObject);
			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_SkillObject)
			{
				m_pCurrentReceiver->OnClickSkillObject(MouseEvent,(CSkillObject*)pObject);
			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_Effect)
			{
				ASSERT(0);
			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_Pet)
			{

				m_pCurrentReceiver->OnClickPet(MouseEvent,(CPetBase*)pObject);
			}
			else if( pObject->GetEngineObjectType() == eEngineObjectType_MapObject )
			{
				m_pCurrentReceiver->OnClickMapObject(MouseEvent,(CPetBase*)pObject);

			}
			else
			{
				ASSERT(0);
			}
			
			if( MouseEvent & MOUSEEVENT_DBLCLICK )
			{
				if( pObject->GetEngineObjectType() == eEngineObjectType_Character ||
					pObject->GetEngineObjectType() == eEngineObjectType_Titan )
				{
					//if(pObject->IsDied())return;//角色死亡后不可选中	By:十里坡剑神
					m_pCurrentReceiver->OnDbClickPlayer( MouseEvent,(CPlayer*)pObject );
				}
				else if( pObject->GetEngineObjectType() == eEngineObjectType_Monster )
				{
					m_pCurrentReceiver->OnDbClickMonster( MOUSEEVENT_LCLICK, (CMonster*)pObject );
				}
			}

			if( MouseEvent & MOUSEEVENT_LCLICK && pObject != HERO )
			{
				if( !HERO->GetCharacterTotalInfo()->bPeace && pObject->GetEngineObjectType() == eEngineObjectType_Pet)
					OBJECTMGR->SetSelectedObject( NULL );
				else
					OBJECTMGR->SetSelectedObject( pObject );
			}
		}
		else
		{
			if( MouseEvent & MOUSEEVENT_LCLICK )
			{
				if( gCurTime - dwMouseMoveTick >= MOUSE_MOVE_TICK )
				{
					dwMouseMoveTick = gCurTime;
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseEventX(),Mouse->GetMouseEventY());
					if(!TargetPos)
						return;

					m_pCurrentReceiver->OnClickGround(MouseEvent,TargetPos);				
				}
			}
			else if( MouseEvent & MOUSEEVENT_LPRESS )
			{
				if( gCurTime - dwMousePressTick >= MOUSE_PRESS_TICK )
				{
					dwMousePressTick = gCurTime;
					VECTOR3* TargetPos = GetPickedPosition(Mouse->GetMouseX(), Mouse->GetMouseY());
					if(!TargetPos)
						return;
					m_pCurrentReceiver->OnClickGround(MouseEvent,TargetPos);

				}				
			}
		}
	}
}




