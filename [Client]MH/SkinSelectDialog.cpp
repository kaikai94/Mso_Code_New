








// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "WindowIDEnum.h"

#include "cWindowManager.h"
#include "GameIn.h"
#include "SkinSelectDialog.h"
#include "GameResourceManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "ChatManager.h"

CSkinSelectDialog::CSkinSelectDialog()
{
	m_pNomalSkinListDlg = NULL;
	m_pNomalSkinIconDlg = NULL;

	m_dwSelectIdx = 0;	
	//InitSkinDelayTime();
}

CSkinSelectDialog::~CSkinSelectDialog()
{
	m_pNomalSkinListDlg->RemoveAll();


	m_pNomalSkinListDlg = NULL;
	m_pNomalSkinIconDlg = NULL;
}

void CSkinSelectDialog::Linking()
{
	m_pNomalSkinListDlg = (cListDialog*)GetWindowForID(SKIN_SELECTLIST);
	m_pNomalSkinListDlg->SetShowSelect(TRUE);

	m_pNomalSkinIconDlg = (cIconDialog*)GetWindowForID(SKIN_SELECT_ITEMVIEW);		

	m_NomalSkinView[0].SetMovable(FALSE);
	m_NomalSkinView[1].SetMovable(FALSE);
	m_NomalSkinView[2].SetMovable(FALSE);
}

void CSkinSelectDialog::SetActive( BOOL val )
{	
	cDialog::SetActive( val );
	if( val == FALSE )	
	{
		m_pNomalSkinListDlg->RemoveAll();
		m_pNomalSkinListDlg->SetCurSelectedRowIdx(-1);		
		m_pNomalSkinIconDlg->DeleteIconAll();
		m_dwSelectIdx = 0;
	}
	else
		SkinItemListInfo();
}

BOOL CSkinSelectDialog::OnActionEvent(LONG lId, void * p, DWORD we)


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
	case SKIN_SELECT_OK:	//Ȯ�� ��ư

		{
			if(m_dwSelectIdx > 0)
			{
				SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GAMERESRCMNGR->GetNomalClothesSkinList(m_dwSelectIdx);
				if(NULL == pSkinItemInfo)
					break;

				//���� üũ
				if(HERO->GetLevel() < pSkinItemInfo->dwLimitLevel)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(238), pSkinItemInfo->dwLimitLevel);
					break;
				}

				//���� üũ
				if( FALSE == HERO->CheckSkinDelay() )
					break;

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );

				msg.dwObjectID = HEROID;
				msg.dwData1 = m_dwSelectIdx;
				msg.dwData2 = eSHOP_ITEM_NOMALCLOTHES_SKIN;

				NETWORK->Send(&msg, sizeof(MSG_DWORD2));
			}			
		}
		break;
	case SKIN_SELECT_CANCEL:	//��� ��ư
		{
			SetActive( FALSE );
		}
		break;
	case SKIN_SELECT_RECOVERY:	//������� �ǵ����� ��ư
		{

			//���� üũ
			if( FALSE == HERO->CheckSkinDelay() )
				break;		

			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_SYN );

			msg.dwObjectID = HEROID;
			msg.dwData1 = 0;	//0������ �����ϸ� ������� �ǵ�����
			msg.dwData2 = eSHOP_ITEM_NOMALCLOTHES_SKIN;


			NETWORK->Send(&msg, sizeof(MSG_DWORD2));
		}
		break;
	}


	return TRUE;
}

DWORD CSkinSelectDialog::ActionEvent(CMouse* mouseInfo)

{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pNomalSkinListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{

		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pNomalSkinListDlg->GetCurSelectedRowIdx();
				m_dwSelectIdx = Idx + 1; //��Ų �ε����� 1���� �����ϱ� ������ +1�� ���ش�

				// �ش� �ε����� ���� �̹����� ����
				if(m_dwSelectIdx > 0)
				{
					SKIN_SELECT_ITEM_INFO* pSkinItemInfo = GAMERESRCMNGR->GetNomalClothesSkinList(m_dwSelectIdx);
					if(pSkinItemInfo != NULL)
					{
						m_pNomalSkinIconDlg->DeleteIconAll();

						for(int i = 0 ; i < SKINITEM_LIST_MAX ; i++)
						{
							m_NomalSkinView[i].SetData(pSkinItemInfo->wEquipItem[i]);
							m_NomalSkinView[i].Init(pSkinItemInfo->wEquipItem[i], NULL);
							
							m_pNomalSkinIconDlg->AddIcon(i, (cIcon*)&m_NomalSkinView[i]);
						}
					}					
				}
			}
		}
	}	

	return we;

}

void CSkinSelectDialog::SkinItemListInfo()
{
	DWORD dwTotalNum = GAMERESRCMNGR->GetNomalClothesSkinListCountNum();
	for (DWORD i = 0 ; i < dwTotalNum ; i++)
	{
		DWORD dwColor;

		char buf[64] = { 0, };

		char szSkinItemName[MAX_NAME_LENGTH+1];

		
		int nLen = 0;


		SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(i+1);	//��Ų �ε����� 1���� �����ϱ� ������ +1 ���ش�.
		if(pSkinInfo == NULL)
			continue;

		//�������ѿ� ���� ���� �� ���� ( ���밡�� : ���, ����Ұ��� : ������ )
		if(HERO->GetLevel() < pSkinInfo->dwLimitLevel)
			dwColor = RGBA_MAKE(255,50,50,255);
		else
			dwColor = RGBA_MAKE(255,255,255,255);			

		//��Ų ��Ʈ �̸� �ڿ� ������ �־ ���� ���ڿ� ������ �Ѵ�.
		SafeStrCpy(szSkinItemName, pSkinInfo->szSkinName, MAX_NAME_LENGTH+1);
		nLen = strlen(pSkinInfo->szSkinName);
		for( int i = nLen; i < MAX_NAME_LENGTH; i++ )
		{
			szSkinItemName[i] = 0x20;
		}

		sprintf(buf, "%s %15d", szSkinItemName, pSkinInfo->dwLimitLevel);
		m_pNomalSkinListDlg->AddItem(buf, dwColor);
	}	
}
/*
void CSkinSelectDialog::Render()
{
	cDialog::Render();

	//���� ó�� Process
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
void CSkinSelectDialog::InitSkinDelayTime()
{
	m_dwSkinDelayTime = 0;	//���� �� �ð� ����

	m_bSkinDelayResult = FALSE;	//���������� üũ�ϴ� ����
}


void CSkinSelectDialog::StartSkinDelayTime()
{
	m_dwSkinDelayTime = gCurTime + eSkinDelayTime;
	m_bSkinDelayResult = TRUE;
}

BOOL CSkinSelectDialog::CheckDelay()
{
	//���� ó��
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




