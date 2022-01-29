// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "GameIn.h"
#include "CostumeSkinSelectDialog.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "AppearanceManager.h"

CCostumeSkinSelectDialog::CCostumeSkinSelectDialog()
{
	m_pCostumeSkinListDlg = NULL;
	m_pCostumeSkinIconDlg = NULL;

	m_dwSelectIdx = 0;	

	m_CostumeSkinHat.Initialize(50);
	m_CostumeSkinDress.Initialize(50);	
	m_CostumeSkinAccessory.Initialize(50);	
}

CCostumeSkinSelectDialog::~CCostumeSkinSelectDialog()
{
	m_pCostumeSkinListDlg->RemoveAll();

	m_pCostumeSkinListDlg = NULL;
	m_pCostumeSkinIconDlg = NULL;

	m_CostumeSkinHat.RemoveAll();	
	m_CostumeSkinDress.RemoveAll();
	m_CostumeSkinAccessory.RemoveAll();
}

void CCostumeSkinSelectDialog::Linking()
{
	m_pCostumeSkinListDlg = (cListDialog*)GetWindowForID(COSTUME_SKIN_SELECTLIST);
	m_pCostumeSkinListDlg->SetShowSelect(TRUE);
	
	m_pCostumeSkinIconDlg = (cIconDialog*)GetWindowForID(COSTUME_SKIN_SELECT_ITEMVIEW);			

	m_CostumeSkinView.SetMovable(FALSE);	

	m_pCostumTabBtn[eTabBtn_Hat] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_HAT);

	m_pCostumTabBtn[eTabBtn_Dress] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_DRESS);
	m_pCostumTabBtn[eTabBtn_Accessory] = (cPushupButton*)GetWindowForID(COSTUME_TABBTN_ACCESSORY);

	CostumeSkinKindData();	
}

void CCostumeSkinSelectDialog::SetActive( BOOL val )
{	
	cDialog::SetActive( val );
	if( val == FALSE )	
	{
		m_pCostumeSkinListDlg->RemoveAll();
		m_pCostumeSkinListDlg->SetCurSelectedRowIdx(-1);

		m_pCostumeSkinIconDlg->DeleteIconAll();
		m_dwSelectIdx = 0;
	}
	else
	{
		SetCostumTabBtnFocus(eTabBtn_Hat);
		CostumeSkinListInfo(eTabBtn_Hat);
	}
}

BOOL CCostumeSkinSelectDialog::OnActionEvent(LONG lId, void * p, DWORD we)

{

	switch(we)
	{			
	case WE_CLOSEWINDOW:

		{	
			return TRUE;
		}
		break;
	}

	switch( lId )
	{		
	case COSTUME_SKIN_SELECT_OK:

		{
			if(m_dwSelectIdx > 0)
			{
				SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GetCurrentSkinInfo(m_dwSelectIdx);
				if(NULL == pSkinItemInfo)
					break;

				AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pSkinItemInfo->wEquipItem[0] );
				if(!pAvatarItem)
					break;

				if( (pAvatarItem->Gender < GENDER_MAX) && (pAvatarItem->Gender != APPEARANCEMGR->GetGenderFromMap(HERO)) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(778) );
					break;
				}

				if( FALSE == HERO->CheckSkinDelay() )
					break;

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pSkinItemInfo->dwIndex;
				msg.dwData2 = eSHOP_ITEM_COSTUME_SKIN;

				NETWORK->Send(&msg, sizeof(MSG_DWORD2));
			}			
		}
		break;
	case COSTUME_SKIN_SELECT_CANCEL:
		{
			SetActive( FALSE );
		}
		break;
	case COSTUME_SKIN_SELECT_RECOVERY:	
		{
			if( FALSE == HERO->CheckSkinDelay() )
				break;		


			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );
			msg.dwObjectID = HEROID;

			msg.dwData1 = 0;

			msg.dwData2 = eSHOP_ITEM_COSTUME_SKIN;

			NETWORK->Send(&msg, sizeof(MSG_DWORD2));
		}
		break;
	case COSTUME_TABBTN_HAT:
		{
			SetCostumTabBtnFocus(eTabBtn_Hat);			
			CostumeSkinListInfo(eTabBtn_Hat);
		}
		break;
	case COSTUME_TABBTN_DRESS:
		{			

			SetCostumTabBtnFocus(eTabBtn_Dress);			

			CostumeSkinListInfo(eTabBtn_Dress);
		}
		break;
	case COSTUME_TABBTN_ACCESSORY:
		{
			SetCostumTabBtnFocus(eTabBtn_Accessory);
			CostumeSkinListInfo(eTabBtn_Accessory);
		}
		break;
	}

	return TRUE;

}

DWORD CCostumeSkinSelectDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pCostumeSkinListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{		
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pCostumeSkinListDlg->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx + 1;
				if(m_dwSelectIdx > 0)
				{					
					SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GetCurrentSkinInfo(m_dwSelectIdx);
					if(pSkinItemInfo != NULL)
					{
						m_pCostumeSkinIconDlg->DeleteIconAll();

						m_CostumeSkinView.SetData(pSkinItemInfo->wEquipItem[0]);
						m_CostumeSkinView.Init(pSkinItemInfo->wEquipItem[0], NULL);
							
						m_pCostumeSkinIconDlg->AddIcon(0, (cIcon*)&m_CostumeSkinView);
					}					
				}
			}
		}
	}	
	return we;
}

void CCostumeSkinSelectDialog::CostumeSkinListInfo(TAB_BTN eCostumeKind)
{	
	m_pCostumeSkinListDlg->RemoveAll();

	switch(eCostumeKind)
	{
	case eTabBtn_Hat:
		m_pCurrentSkinTable = &m_CostumeSkinHat;
		break;
	case eTabBtn_Dress:
		m_pCurrentSkinTable = &m_CostumeSkinDress;
		break;
	case eTabBtn_Accessory:
		m_pCurrentSkinTable = &m_CostumeSkinAccessory;
		break;
	}
	
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
	m_pCurrentSkinTable->SetPositionHead();
	while(pSkinInfo = m_pCurrentSkinTable->GetData())
	{
		if(pSkinInfo == NULL)
			continue;
		m_pCostumeSkinListDlg->AddItem(pSkinInfo->szSkinName, RGBA_MAKE(255,255,255,255));
	}
}
/*
void CCostumeSkinSelectDialog::Render()
{
	cDialog::Render();
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwCurrentTime = gCurTime;
		if(dwCurrentTime > m_dwSkinDelayTime)
		{
			InitSkinDelayTime();
		}
	}
}
*/
/*
void CCostumeSkinSelectDialog::InitSkinDelayTime()
{
	m_dwSkinDelayTime = 0;
	m_bSkinDelayResult = FALSE;	
}


void CCostumeSkinSelectDialog::StartSkinDelayTime()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}


BOOL CCostumeSkinSelectDialog::CheckDelay()
{
	if(m_bSkinDelayResult == TRUE)
	{
		DWORD dwTime = m_dwSkinDelayTime - gCurTime;
		if(gCurTime > m_dwSkinDelayTime)
		{

			dwTime = 0;
		}		
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1355), dwTime/1000+1);

		return FALSE;
	}

	return TRUE;
}
*/
void CCostumeSkinSelectDialog::CostumeSkinKindData()
{
	DWORD dwTotalNum = GAMERESRCMNGR->GetCostumeSkinListCountNum();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{
		SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetCostumeSkinList(i+1);
		if(pSkinInfo == NULL)
			continue;

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pSkinInfo->wEquipItem[0]);
		if(pInfo == NULL)
			continue;

		WORD wType = pInfo->Part3DType;
		
		if(wType == 0)
			m_CostumeSkinHat.Add(pSkinInfo, i);
		else if(wType == 2)	
			m_CostumeSkinDress.Add(pSkinInfo, i);
		else
			m_CostumeSkinAccessory.Add(pSkinInfo, i);
	}
}

SKIN_SELECT_ITEM_INFO* CCostumeSkinSelectDialog::GetCurrentSkinInfo(DWORD dwSelectIdx)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;

	DWORD dwTotalNum = m_pCurrentSkinTable->GetDataNum();
	m_pCurrentSkinTable->SetPositionHead();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{		
		pSkinInfo = m_pCurrentSkinTable->GetData();

		if(pSkinInfo == NULL)
			continue;

		if(i+1 == dwSelectIdx)
			return pSkinInfo;

		pSkinInfo = NULL;		
	}
	return pSkinInfo;
}

void CCostumeSkinSelectDialog::SetCostumTabBtnFocus(TAB_BTN eCostumeKind)
{
	for(int i = 0 ; i < eTabBtn_Max ; i++)
	{
		BOOL bFlag = FALSE;

		if(i == eCostumeKind)
			bFlag = TRUE;
		else
			bFlag = FALSE;

		m_pCostumTabBtn[i]->SetPush(bFlag);

	}
}