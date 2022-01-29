








// DissolutionDialog.cpp: implementation of the CDissolutionDialog class.
//

//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cScriptManager.h"
#include "Item.h"
#include "ItemManager.h"
#include "DissolutionManager.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDissolutionDialog::CDissolutionDialog()
{
	m_pDissolutionItemDlg	= NULL;
	m_pUnitItemDlg[0]		= NULL;
	m_pUnitItemDlg[1]		= NULL;
	m_pUnitItemDlg[2]		= NULL;
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	m_VirtualDissolutionItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualDissolutionItem.SetMovable(FALSE);

	for(int i = 0 ; i < MAX_DISSOLUTION_UNITNUM ; ++i)
	{
		m_ShowUnitItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_ShowUnitItem[i].SetMovable(FALSE);
	}


	m_nTotalUnitOutNum	= 0;
	ZeroMemory( &m_CheckItemInfo, sizeof(m_CheckItemInfo) );
}


CDissolutionDialog::~CDissolutionDialog()
{

}

void CDissolutionDialog::Add( cWindow* window )
{
	if( window->GetType() == WT_PUSHUPBUTTON )
		AddTabBtn( curIdx1++, (cPushupButton * )window );
	else if( window->GetType() == WT_ICONGRIDDIALOG )
	{
		AddTabSheet( curIdx2++, window );
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}

void CDissolutionDialog::Linking()
{
	m_pDissolutionItemDlg	= (cIconDialog*)GetWindowForID(DIS_BASEICONDLG);
	m_pUnitItemDlg[0]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG1);
	m_pUnitItemDlg[1]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG2);
	m_pUnitItemDlg[2]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG3);
}

void CDissolutionDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();	
	cTabDialog::RenderTabComponent();

}

DWORD CDissolutionDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL;

	we |= cTabDialog::ActionEvent( mouseInfo );

	return we;

}

void CDissolutionDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;



	if( val == FALSE )

	{
		RemoveDissolutionItem();
		RemoveAllUnitItems();
	}

	cTabDialog::SetActive(val);
}

void CDissolutionDialog::OnActionEvent( LONG lId, void * p, DWORD we )

{
	if( lId == DIS_OKBTN )
	{
		SendMsg_Dissolution();
	}
	else if( lId == DIS_CANCELBTN )
	{
		SetActive( FALSE );
	}

	return;
}

BOOL CDissolutionDialog::FakeMoveIcon( LONG x, LONG y, cIcon * icon )
{
	if( m_bDisable )					return FALSE;
	if( icon == NULL )					return FALSE;
	if( icon->GetType() != WT_ITEM )	return FALSE;


	CItem* pOrigItem = (CItem*)icon;
	if( pOrigItem->IsLocked() )			return FALSE;

	// �κ��丮�̿ܿ� ������ FALSE
	if( !ITEMMGR->IsEqualTableIdxForPos( eItemTable_Inventory, pOrigItem->GetPosition() ) )
		return FALSE;

	//��ȭ �������̸� �ȵ�

	if( ITEMMGR->IsOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetDurability() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(689) );	//"��ȭ�� �������� ���� �� �� �����ϴ�."
		return FALSE;
	}
	if( ITEMMGR->IsRareOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetRareness() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );
		return FALSE;
	}
	
	//����Ʈ�� �ִ��� Ȯ��
	if( !DISSOLUTIONMGR->CanDissolutionItem( pOrigItem->GetItemIdx() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );	//"������ �� ���� ������ �Դϴ�."
		return FALSE;
	}
	
	if( pOrigItem->GetDurability() > 1 ) //������ �������� �ȵȴ�.
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1099) );	//"������ �������� ������ �� �����ϴ�."
		return FALSE;		
	}

	//������ �ö�� �ִ� �������� �����.
	RemoveDissolutionItem();
	RemoveAllUnitItems();

	//�������� �ø���.
	AddDissolutionItem( pOrigItem );

	return FALSE;
}

CVirtualItem* CDissolutionDialog::GetDissolutionVirtualItem()
{
	 return (CVirtualItem*)m_pDissolutionItemDlg->GetIconForIdx(0);
}

CItem* CDissolutionDialog::GetDissolutionInventoryItem()
{
	 CVirtualItem* pVItem = (CVirtualItem*)m_pDissolutionItemDlg->GetIconForIdx(0);
	 if( pVItem == NULL ) return NULL;


	 return (CItem*)pVItem->GetLinkItem();
}

void CDissolutionDialog::AddDissolutionItem( CItem* pOrigItem )

{
	if( m_pDissolutionItemDlg->AddIcon( 0, &m_VirtualDissolutionItem ) )
	{
		m_nTotalUnitOutNum = 0;

		if( ShowUnitItems( pOrigItem->GetItemIdx() ) )
		{
			m_VirtualDissolutionItem.SetData( pOrigItem->GetItemIdx() );
			m_VirtualDissolutionItem.SetLinkItem( pOrigItem );
			//!!!�⺻ NULL ���� Ȯ�� SW050920 Rare
			if( ITEMMGR->IsOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetDurability() ) )
			{
				ITEMMGR->SetToolTipIcon( &m_VirtualDissolutionItem, ITEMMGR->GetItemOption( pOrigItem->GetDurability() ),
					ITEMMGR->GetItemRareOption( pOrigItem->GetRareness() ),0);
			}
			else
				ITEMMGR->SetToolTipIcon( &m_VirtualDissolutionItem, NULL, ITEMMGR->GetItemRareOption( pOrigItem->GetRareness() ),0); 

			pOrigItem->SetLock( TRUE );

			m_CheckItemInfo = *pOrigItem->GetItemBaseInfo();
		}
		else
		{
			//error
			RemoveDissolutionItem();
			RemoveAllUnitItems();
		}
	}
}


void CDissolutionDialog::RemoveDissolutionItem()
{
	CVirtualItem* pVItem = NULL;
	m_pDissolutionItemDlg->DeleteIcon( 0, (cIcon**)&pVItem );
	if( pVItem == NULL )
		return;
	pVItem->SetLinkItem(NULL);

	CItem* pOrigItem = ITEMMGR->GetItem( m_CheckItemInfo.dwDBIdx );
	ZeroMemory( &m_CheckItemInfo, sizeof(m_CheckItemInfo) );
	

	m_nTotalUnitOutNum = 0;
	if( pOrigItem == NULL ) return;
	pOrigItem->SetLock( FALSE );
}

BOOL CDissolutionDialog::ShowUnitItems( WORD wDissolutionItemIdx )
{
	WORD wUnitKindNum = 0;
	sITEM_DISSOLUTIONITEM* pUnitInfo = DISSOLUTIONMGR->GetDissolutionUnitItemInfo( wDissolutionItemIdx, &wUnitKindNum );
	if( pUnitInfo == NULL )
		return FALSE;
	
	if( wUnitKindNum > MAX_DISSOLUTION_UNITNUM ) return FALSE;		//������ ĭ���� ���� ���� ����.

	for( int i = 0 ; i < wUnitKindNum ; ++i )
	{
		WORD tempUnitItemIdx = pUnitInfo[i].wUnitItemIdx;
		WORD tempUnitItemNum = pUnitInfo[i].wUnitItemNum;


		if( tempUnitItemIdx == 0 || tempUnitItemNum == 0 ) return FALSE;

		//���⼭ ���� �������� ������ �� �ִ� �������̸� ���ѹ� �����Ѵ�.
		eITEM_KINDBIT eItemKind = GetItemKind( tempUnitItemIdx );
		if( !(eItemKind & eEXTRA_ITEM) )
		{
			if( ShowUnitItems( tempUnitItemIdx ) == TRUE )	//���صǸ� �Ʒ��� �ʿ����.
				continue;			
		}
		
		while( tempUnitItemNum > 0 )
		{
			if( m_nTotalUnitOutNum >= MAX_DISSOLUTION_UNITNUM ) return FALSE;	//=�� �ȴ�.

			if( m_pUnitItemDlg[m_nTotalUnitOutNum/MAX_UNIT_PERTAB]->AddIcon( m_nTotalUnitOutNum%MAX_UNIT_PERTAB, (cIcon*)&m_ShowUnitItem[m_nTotalUnitOutNum] ) )

			{
				m_ShowUnitItem[m_nTotalUnitOutNum].SetData( tempUnitItemIdx );

				if( tempUnitItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					m_ShowUnitItem[m_nTotalUnitOutNum].Init( tempUnitItemIdx, MAX_YOUNGYAKITEM_DUPNUM );
					tempUnitItemNum -= MAX_YOUNGYAKITEM_DUPNUM;	//while(tempUnitItemNum>5) �̹����� 0���� �۾��� ���� ����.
				}
				else
				{

					m_ShowUnitItem[m_nTotalUnitOutNum].Init( tempUnitItemIdx, tempUnitItemNum );
					tempUnitItemNum = 0;
				}

				ITEMMGR->SetToolTipIcon( &m_ShowUnitItem[m_nTotalUnitOutNum] );		
				++m_nTotalUnitOutNum;
			}
			else
			{
				//error
				//�ö� showitem�� �����
				return FALSE;
			}
		}
	}

	if( m_nTotalUnitOutNum > MAX_DISSOLUTION_UNITNUM ) return FALSE;	//=�� �ƴϴ�.

	return TRUE;
}


void CDissolutionDialog::RemoveAllUnitItems()
{
	CItemShow* pSItem = NULL;

	for( int i = 0 ; i < MAX_DISSOLUTION_UNITNUM ; ++i )
	{
		m_pUnitItemDlg[i/MAX_UNIT_PERTAB]->DeleteIcon( i%MAX_UNIT_PERTAB, (cIcon**)&pSItem );
		if( pSItem != NULL )
		{
			m_ShowUnitItem[i].SetData( 0 );
			m_ShowUnitItem[i].Init( 0, 0 );
		}
	}
}

BOOL CDissolutionDialog::SendMsg_Dissolution()
{
	const ITEMBASE* pInventoryItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, m_CheckItemInfo.Position );
	CItem* pItem = GetDissolutionInventoryItem();
	if( pInventoryItemInfo == NULL || pItem == NULL )
	{
		//������ ������ �ٸ��ϴ�.
		return FALSE;
	}

	const ITEMBASE* pDissolutionItemInfo = pItem->GetItemBaseInfo();

	if( pDissolutionItemInfo->dwDBIdx != pInventoryItemInfo->dwDBIdx ||

		pDissolutionItemInfo->wIconIdx != pInventoryItemInfo->wIconIdx ||
		pDissolutionItemInfo->Position != pInventoryItemInfo->Position ||
		pDissolutionItemInfo->Durability != pInventoryItemInfo->Durability ||
		pDissolutionItemInfo->QuickPosition != pInventoryItemInfo->QuickPosition )
	{
		//������ ������ �ٸ��ϴ�.
		return FALSE;
	}

	//������ �� ������ Ȯ������
	int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
	if( m_nTotalUnitOutNum > nBlankNum )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(691) );//�κ��丮 ������ �����Ͽ� ������ �� �����ϴ�.
		return FALSE;
	}
	
	SetDisable( TRUE );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );

	MSG_ITEM msg;
	msg.Category	= MP_ITEM;
	msg.Protocol	= MP_ITEM_DISSOLUTION_SYN;
	msg.dwObjectID	= HEROID;
	msg.ItemInfo	= *pInventoryItemInfo;
	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}


void CDissolutionDialog::EndDissolution( BOOL bSuccess )
{
	SetDisable( FALSE );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

	if( bSuccess )
	{
		//���� : �Ʒ��� ���������ÿ�. m_CheckItemInfo �ʱ�ȭ�Ǹ� �ȵ�.
		ITEMMGR->DeleteItemofTable( eItemTable_Inventory, m_CheckItemInfo.Position );
		//������ �ö�� �ִ� �������� �����.
		RemoveDissolutionItem();
		RemoveAllUnitItems();
	}
	else
	{
		
	}
}




