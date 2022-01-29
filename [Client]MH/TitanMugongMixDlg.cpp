








// TitanMugongMixDlg.cpp: implementation of the CTitanMugongMixDlg class.

//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanMugongMixDlg.h"

#include "WindowIDEnum.h"
#include "Item.h"
#include "MugongBase.h"
#include "ChatManager.h"
#include "SkillManager_Client.h"
#include "ObjectManager.h"
#include "MugongManager.h"
#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitanMugongMixDlg::CTitanMugongMixDlg()
{

}

CTitanMugongMixDlg::~CTitanMugongMixDlg()
{

}

void CTitanMugongMixDlg::Linking()
{
	m_pTitanMugongIconDlg = (cIconDialog*)GetWindowForID(TITAN_MUGONGMIX_SLOT_MUGONG_DLG);
	m_pTitanItemIconDlg = (cIconDialog*)GetWindowForID(TITAN_MUGONGMIX_SLOT_MUNSEO_DLG);
}

void CTitanMugongMixDlg::SetActive( BOOL val )
{
	if( val == FALSE )

	{

		cIcon* temp;

		m_pTitanMugongIconDlg->DeleteIcon(0, &temp);
		m_pTitanItemIconDlg->DeleteIcon(0, &temp);
		if(temp)
			((CItem*)temp)->SetLock(FALSE);


		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	cDialog::SetActive( val );
}

BOOL CTitanMugongMixDlg::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{

	ASSERT(icon);

	CItem* pItem = NULL;

	if( icon->GetType() == WT_MUGONG )
	{
		// �巡���� ������ �����϶�
		CMugongBase* pMugong = (CMugongBase *)icon;

		CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pMugong->GetMugongBase()->wIconIdx);
		if(!pSInfo)	return FALSE;

		// ��ȯ �������� üũ
		//if(!pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN)
		//	return FALSE;

		// �̹� ��ȯ�� �������� üũ
		//if(MUGONGMGR->GetMugongByMugongIdx(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN))
		//	return FALSE;

		// 12�� ���ϸ� �ȵ�
		if(pMugong->GetSung() < 12)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
			return FALSE;
		}

		// ���� �ö� �ִ� ������ �����
		cIcon* pOldMugong = NULL;
		m_pTitanMugongIconDlg->DeleteIcon(0, &pOldMugong);



		// ������ ����Ѵ�
		m_pTitanMugongIconDlg->AddIcon(0, icon, TRUE);

		return FALSE;
	}
	else if(icon->GetType() == WT_ITEM)
	{
		// �巡���� ������ �������϶�
		pItem = (CItem*)icon;


		// ����ĭ�� ������ �����´�.
		//CMugongBase* pOldMugong = (CMugongBase*)(m_pTitanMugongIconDlg->GetIconForIdx(0));

		//if(!pOldMugong)	// ������ ���ٸ�

		//	return FALSE;		

		// Ÿ��ź ������ȯ �������� �ƴϸ� ����
		//if(pItem->GetItemKind() != eMUGONG_ITEM_TITAN)
		//	return FALSE;

		// ���� �ö� �ִ� �������� �����
		cIcon* pOldItem = NULL;
		m_pTitanItemIconDlg->DeleteIcon(0, &pOldItem);


		if(pOldItem)
			((CItem*)pOldItem)->SetLock(FALSE);

		// �������� ����Ѵ�

		m_pTitanItemIconDlg->AddIcon(0, icon, TRUE);
		((CItem*)icon)->SetLock(TRUE);

		return FALSE;
	}
	else
		return FALSE;


}

void CTitanMugongMixDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case TITAN_MUGONGMIX_OKBTN:
			{
				CMugongBase* pMugong = (CMugongBase*)(m_pTitanMugongIconDlg->GetIconForIdx(0));
				CItem* pItem = (CItem*)(m_pTitanItemIconDlg->GetIconForIdx(0));

				if(!pMugong || !pItem)
				{
					return;
				}
				CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pMugong->GetMugongBase()->wIconIdx);
				if(!pSInfo)	return;

				// ��ȯ �������� üũ


				//if(!pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN)
				//	return;

	
				// �̹� ��ȯ�� �������� üũ
				//if(MUGONGMGR->GetMugongByMugongIdx(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN))
				//	return;

				// 12�� ���ϸ� �ȵ�
				if(pMugong->GetSung() < 12)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
					return;
				}

				// Ÿ��ź ������ȯ �������� �ƴϸ� ����
				//if(pItem->GetItemKind() != eMUGONG_ITEM_TITAN)
				//	return;


//				MSG_WORD3 msg;
//				msg.Category = MP_ITEM;
//				msg.Protocol = MP_ITEM_TITAN_MUGONG_CHANGE_SYN;
//				msg.dwObjectID = HEROID;
//				msg.wData1 = pMugong->GetItemIdx();
//				msg.wData2 = pItem->GetItemIdx();

//				msg.wData3 = pItem->GetPosition();

//				NETWORK->Send( &msg, sizeof(msg) );

				SetActive(FALSE);
			}
			break;
		case TITAN_MUGONGMIX_CANCELBTN:
			SetActive(FALSE);
			break;
		}
	}
}

void CTitanMugongMixDlg::DeleteMugongIcon()
{
	cIcon* temp;

	m_pTitanMugongIconDlg->DeleteIcon(0, &temp);
}

void CTitanMugongMixDlg::DeleteItemIcon()
{
	cIcon* temp;

	m_pTitanItemIconDlg->DeleteIcon(0, &temp);
	if(temp)
		((CItem*)temp)->SetLock(FALSE);
}





