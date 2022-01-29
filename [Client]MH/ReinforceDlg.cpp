







// ReinforceDlg.cpp: implementation of the CReinforceDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReinforceDlg.h"
#include "Item.h"
#include "INTERFACE\cIconDialog.h"
#include "INTERFACE\cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "Gamein.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ReinforceManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
#include "./Interface/cStatic.h"
#include "ReinforceDataGuideDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#ifdef _GMTOOL_
#include "CheatMsgParser.h"
#endif

CReinforceDlg::CReinforceDlg()

{
	m_pReinforceDlg = NULL;
	m_pReinforceGridDlg = NULL;
	m_pReinforceGravityDlg = NULL;
	m_ScrItem = NULL;
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	for(int i = 0 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_VirtualItem[i].SetMovable(FALSE);
	}
	m_bShopItemUse	= FALSE;
	m_ShopItemIdx	= 0;
	m_ShopItemPos	= 0;
	m_pTitle		= NULL;
	memset( m_Title, 0, sizeof(char)*128 );
}

CReinforceDlg::~CReinforceDlg()
{
	// don't call Release() 
	// pre delete item 
}
void CReinforceDlg::Release(eReinforceReleaseOpt op)
{
	CVirtualItem * pVItem = NULL;
	
	if((op & eReinforceBasicRelease) && !IsEmpty(0))
	{
		m_pReinforceDlg->DeleteIcon(0, (cIcon **)&pVItem);
		pVItem->GetLinkItem()->SetLock(FALSE);
		pVItem->SetLinkItem(NULL);
	}

	if(m_ScrItem)
	{
		m_ScrItem->SetLock(FALSE,TRUE);
		m_ScrItem = NULL;
	}
	if( (op & eReinforceViewRelease) )
	{
		for(int i = 1 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
		{
			if(!IsEmpty(i))
			{
				m_pReinforceGridDlg->DeleteIcon(i-1, (cIcon **)&pVItem);
				pVItem->GetLinkItem()->SetLock(FALSE);
				pVItem->SetLinkItem(NULL);
			}

		}		
	}
	REINFORCEMGR->SetRareMaterialTotalGravity(0);
}
void CReinforceDlg::Linking()
{
	m_pReinforceDlg		= (cIconDialog *)GetWindowForID(ITR_REINFORCEICONDLG);
	m_pReinforceGridDlg = (cIconGridDialog *)GetWindowForID(ITR_REINFORCEICONGRIDDLG);
	m_pTitle = (cStatic *)GetWindowForID( ITR_REINFORCETITLE );

	if( m_pTitle )
		strcpy( m_Title, m_pTitle->GetStaticText() );


	m_pReinforceGravityDlg = (cStatic*)GetWindowForID(ITR_REINFORCEGRAVITY);
	cImage ToolTipImage;
	SCRIPTMGR->GetImage( 63, &ToolTipImage, PFT_HARDPATH );
	m_pReinforceGravityDlg->SetToolTip( CHATMGR->GetChatMsg(1204), RGBA_MAKE(255, 255, 255, 255), &ToolTipImage, TTCLR_DEFAULT );
}

void CReinforceDlg::SetActiveRecursive(BOOL val)
{
	cDialog::SetActiveRecursive( val );

	if( val )
	{
		if( m_bShopItemUse )
		{
			char buf [256];
			sprintf( buf, "%s - (%s)", m_Title, CHATMGR->GetChatMsg( 1091) );
			m_pTitle->SetStaticText( buf );
		}
		else
			m_pTitle->SetStaticText( m_Title );

		SetGravityText();
	}

	if( !val && m_bShopItemUse )
	{
		CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ShopItemPos );
		if( pItem )
			pItem->SetLock( FALSE );
		m_bShopItemUse	= FALSE;
		m_ShopItemIdx	= 0;
		m_ShopItemPos	= 0;
	}
}

BOOL CReinforceDlg::IsEmpty(POSTYPE pos)
{
	if(pos == 0)
	{
		if(m_pReinforceDlg->IsAddable(0))
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		if(m_pReinforceGridDlg->IsAddable(pos-1))
			return TRUE;
		else
			return FALSE;
	}
}
BOOL CReinforceDlg::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	if(pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if( m_bDisable ) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;
	if( pOrigItem->IsLocked() ) return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo( pOrigItem->GetItemIdx() );
	if( !pBaseItemInfo )			return FALSE;

	if( pBaseItemInfo->WeaponType > WP_KEY )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1455) );
		return FALSE;
	}

	if(!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	WORD OptKind = REINFORCEMGR->GetTargetOptionKind();	

	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if(bits & eEQUIP_ITEM)
	{
		if( REINFORCEMGR->GetRareMaterialTotalGravity() )	
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1205) );
			return FALSE;
		}
		else
		{
			if( ITEMMGR->IsRareOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetRareness() ) )
			{
				REINFORCEMGR->SetTatgetOptionKind(eIOK_Rare);
			}
			else
			{
				REINFORCEMGR->SetTatgetOptionKind(eIOK_Normal);	
			}
		}

#ifdef _GMTOOL_
		if(CHEATMGR->m_bReadyToOptionCheat)
		{
			CHEATMGR->m_wTargetItemIdx = pOrigItem->GetItemIdx();
			CHEATMGR->m_ItemAbsPos	= pOrigItem->GetPosition();
			CHATMGR->AddMsg( CTC_CHEAT_2, "OK... TargetItem for ReinforceOption is Registered");
		}
#endif

		if( m_bShopItemUse )
		{
			ITEM_OPTION_INFO * pInfo = ITEMMGR->GetItemOption( pOrigItem->GetDurability() );
			if( pInfo->GetOptionType() == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1081) );
				return FALSE;
			}

			ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo( (WORD)m_ShopItemIdx );
			if( !pShopItem )				return FALSE;
			if( !(pShopItem->ItemKind & eSHOP_ITEM) )		return FALSE;
			if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
				return FALSE;
			}
		}

		if(!IsEmpty(0))
		{
			Release(eReinforceBasicRelease);
		}
		AddVirtualItemWrap(0, pOrigItem);

		switch(bits)
		{
		case eEQUIP_ITEM_WEAPON :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(0);
			}
			break;
		case eEQUIP_ITEM_DRESS :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(2);
			}
			break;
		case eEQUIP_ITEM_HAT :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(1);
			}
			break;
		case eEQUIP_ITEM_SHOES :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(3);
			}
			break;
		case eEQUIP_ITEM_RING :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(8);
			}
			break;
		case eEQUIP_ITEM_CAPE :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(4);
			}
			break;
		case eEQUIP_ITEM_NECKLACE :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(7);
			}
			break;
		case eEQUIP_ITEM_ARMLET	:
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(5);
			}
			break;
		case eEQUIP_ITEM_BELT :
			{
				GAMEIN->GetReinforceDataGuideDlg()->SelectData(6);
			}
			break;
		}
	}
	else if( bits == eEXTRA_ITEM_JEWEL )
	{
		CVirtualItem* pVItem = GetVirtualItem(0);
		if( pVItem )
		{
			if( m_bShopItemUse )
			{
				for(int i=1; i<MAX_REINFORCEGRID_NUM+1; ++i)
				{

					if( !IsEmpty(i) )
					if( m_VirtualItem[i].GetSrcItemIdx() != pOrigItem->GetItemIdx() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1080) );							
						return FALSE;
					}
				}
				if( !REINFORCEMGR->CheckReinforceItemWithMeterial( pVItem->GetSrcPosition(), pOrigItem->GetItemIdx(), OptKind ) )

				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(680) );

					return FALSE;
				}
			}

			if( !REINFORCEMGR->CheckValidMaterial( pVItem->GetSrcItemIdx(), pOrigItem->GetItemIdx(), OptKind ) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(680) );
				return FALSE;
			}

			int OverGravity = REINFORCEMGR->CheckMaterialTotalGravity( pOrigItem->GetItemIdx(), (WORD)pOrigItem->GetDurability(), OptKind );
			if( OverGravity > 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1203), OverGravity );
				return FALSE;
			}

			SetGravityText();

			BOOL bEmpty = FALSE;
			int i = 1;
			for(; i < MAX_REINFORCEGRID_NUM +1 ; ++i)
			{
				if(IsEmpty(i))
				{
					bEmpty = TRUE;
					break;
				}
			}
			if(!bEmpty)	return FALSE;

			if(!AddVirtualItemWrap(i, pOrigItem))
				return FALSE;
		}
		else
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(222) );
			return FALSE;
		}
	}
	else
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(220) );
		return FALSE;
	}


	pOrigItem->SetLock(TRUE,TRUE);

	return FALSE;
}
CVirtualItem * CReinforceDlg::GetVirtualItem(POSTYPE relPos)
{
	if(relPos == 0)
	{
		return (CVirtualItem *)m_pReinforceDlg->GetIconForIdx(0);
	}
	else
	{
		return (CVirtualItem *)m_pReinforceGridDlg->GetIconForIdx(relPos-1);
	}
}
BOOL CReinforceDlg::AddVirtualItemWrap(POSTYPE relPos, CItem * pItem)
{
	m_ScrItem = pItem;
	if (relPos>1)
	{//[化多个材料判断是否为同一种材料][By:十里坡剑神][QQ:112582793][2017/12/16]
		if(m_VirtualItem[1].GetData() !=pItem->GetItemIdx())
		{
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2257));
			return FALSE;
		}
	}
	m_VirtualItem[relPos].SetData(pItem->GetItemIdx());
	m_VirtualItem[relPos].SetLinkItem(pItem);
	if( pItem->GetDurability() != 0 && !ITEMMGR->IsDupItem(pItem->GetItemIdx()) )
	{

		ITEMMGR->SetToolTipIcon( (cIcon*)&m_VirtualItem[relPos], ITEMMGR->GetItemOption(pItem->GetDurability()),
			ITEMMGR->GetItemRareOption(pItem->GetRareness()),0); 
	}
	else
		ITEMMGR->SetToolTipIcon((cIcon*)&m_VirtualItem[relPos], NULL, ITEMMGR->GetItemRareOption(pItem->GetRareness()),0);
	return AddVirtualItem(relPos, &m_VirtualItem[relPos]);
}

BOOL CReinforceDlg::AddVirtualItem(POSTYPE relPos, CVirtualItem * pItem)
{
	if(relPos == 0)
	{
		if(IsEmpty(0))
		{
			m_pReinforceDlg->AddIcon(0, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		if(IsEmpty(relPos))
		{
			m_pReinforceGridDlg->AddIcon(relPos-1, (cIcon *)pItem);
			return TRUE;
		}
		return FALSE;
	}

}

DWORD CReinforceDlg::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cDialog::ActionEvent(mouseInfo);

	for(int i = 0 ; i < MAX_REINFORCEGRID_NUM+1 ; ++i)
	{
		m_VirtualItem[i].ActionEvent(mouseInfo);
	}
	return we;
}

BOOL CReinforceDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{
	case WE_CLOSEWINDOW:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg*)WINDOWMGR->GetWindowForID(lId);
			if( pReinforceDlg->m_bShopItemUse )
			{
				CItem* pItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)pReinforceDlg->m_ShopItemPos );
				if( pItem )
					pItem->SetLock( FALSE );

				pReinforceDlg->m_bShopItemUse	= FALSE;
				pReinforceDlg->m_ShopItemIdx	= 0;
				pReinforceDlg->m_ShopItemPos	= 0;
			}
			pReinforceDlg->Release();
			GAMEIN->GetReinforceDataGuideDlg()->Close();
		}
		break;
	}

	switch( lId )
	{
	case ITR_REINFORCEOKBTN:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg *)p;
			CReinforceDlg::ReinforceOkBtn(pReinforceDlg);
		}
		break;
	case ITR_REINFORCECANCELBTN:
		{
			CReinforceDlg * pReinforceDlg = (CReinforceDlg *)p;
			CReinforceDlg::ReinforceCancelBtn(pReinforceDlg);
		}
		break;

	case ITR_RFGUIDEBTN:
		{
			GAMEIN->GetReinforceDataGuideDlg()->Show();
		}
		break;
	}
	return TRUE;
}


void CReinforceDlg::ReinforceOkBtn(CReinforceDlg * pThis)
{
	if(pThis->IsEmpty(0))
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(222) );
		return;
	}
	if( pThis->m_bShopItemUse )
	{
		pThis->ReinforceOkBtnWithShopItem( pThis );
		return;
	}

	MSG_ITEM_REINFORCE_SYN msg;
	msg.Init(MP_ITEM, MP_ITEM_REINFORCE_SYN, HEROID);

	CVirtualItem * pTargetItem = (CVirtualItem *)pThis->m_pReinforceDlg->GetIconForIdx(0);
	msg.wTargetItemIdx	= pTargetItem->GetSrcItemIdx();
	msg.TargetPos		= pTargetItem->GetSrcPosition();

	for(int i = 1 ; i < MAX_REINFORCEGRID_NUM +1 ; ++i)
	{

		if(!pThis->IsEmpty(i))
		{
			CVirtualItem * pJewelItem = (CVirtualItem *)pThis->m_pReinforceGridDlg->GetIconForIdx(i-1);
			msg.AddJewelInfo(pJewelItem->GetSrcItemIdx(), pJewelItem->GetSrcPosition(), pJewelItem->GetSrcDurability() );
		}
	}	
	if(msg.wJewelUnit == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(224) );
		return ;
	}
	NETWORK->Send(&msg, msg.GetSize());
	pThis->SetDisable(TRUE);
}

void CReinforceDlg::ReinforceOkBtnWithShopItem(CReinforceDlg * pThis)
{
	MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN msg;
	msg.Init( MP_ITEM, MP_ITEM_REINFORCE_WITHSHOPITEM_SYN, HEROID );
	CVirtualItem * pTargetItem = (CVirtualItem *)pThis->m_pReinforceDlg->GetIconForIdx(0);
	msg.wTargetItemIdx	= pTargetItem->GetSrcItemIdx();
	msg.TargetPos		= pTargetItem->GetSrcPosition();
	msg.wShopItemIdx	= (WORD)m_ShopItemIdx;
	msg.ShopItemPos		= (POSTYPE)m_ShopItemPos;
	for(int i=1; i<MAX_REINFORCEGRID_NUM+1; ++i)
	{
		if(!pThis->IsEmpty(i))
		{
			CVirtualItem * pJewelItem = (CVirtualItem *)pThis->m_pReinforceGridDlg->GetIconForIdx(i-1);
			msg.AddJewelInfo(pJewelItem->GetSrcItemIdx(), pJewelItem->GetSrcPosition(), pJewelItem->GetSrcDurability() );
		}
	}	
	if(msg.wJewelUnit == 0)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(224) );
		return ;
	}
	NETWORK->Send(&msg, msg.GetSize());
	
	pThis->SetDisable(TRUE);
}

void CReinforceDlg::ReinforceCancelBtn(CReinforceDlg * pThis)
{
	pThis->Release();
	pThis->SetActiveRecursive(FALSE);
	GAMEIN->GetReinforceDataGuideDlg()->Close();
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(223) );
}



void CReinforceDlg::DieCheck()
{
	if( HERO->GetState() == eObjectState_Die )
	{
		Release();
		SetActiveRecursive( FALSE );
	}

}


void CReinforceDlg::SetShopItemInfo( DWORD ShopItemIdx, DWORD ShopItemPos )
{
	m_ShopItemIdx = ShopItemIdx;
	m_ShopItemPos = ShopItemPos;
	m_bShopItemUse = TRUE;
}

void CReinforceDlg::SetGravityText()
{
	char cTemp[64] = {0,};
	sprintf(cTemp, "%4d/100", REINFORCEMGR->GetRareMaterialTotalGravity());
	m_pReinforceGravityDlg->SetStaticText(cTemp);	
}




