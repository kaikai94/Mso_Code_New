// InventoryExDialog.cpp: implementation of the CInventoryExDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InventoryExDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "ObjectStateManager.h"

#include "GameIn.h"

#include "ItemManager.h"
#include "ObjectManager.h"
#include "KyungGongManager.h"
#include "MugongManager.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "MussangManager.h"

#include "DealItem.h"
#include "cDivideBox.h"
#include "./Audio/MHAudioManager.h"

#include "MugongDialog.h"
#include "DealDialog.h"

#include "./Input/UserInput.h"

#include "PyogukDialog.h"
#include "GuildWarehouseDialog.h"

#include "MainBarDialog.h"
#include "PKManager.h"
#include "cMsgBox.h"
#include "QuestManager.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "SkillPointRedist.h"
#include "ChaseinputDialog.h"
#include "ReinforceDlg.h"
#include "MixDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "ReinforceResetDlg.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "RareCreateDialog.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "PetWearedExDialog.h"
#include "TitanInventoryDlg.h"
#include "PetInventoryDlg.h"
#include "StreetStallManager.h"
#include "ShoutDialog.h"
#include "ItemLockManager.h" // 引用装备 2014-12-03
#include "ItemUnLockManager.h" // 引用装备解锁 2014-12-07 

#include "StreetStallManager.h"
#include "ShoutDialog.h"

#include "VimuManager.h"
#include "MoveManager.h"
#include "./Interface/cScriptManager.h"

#include "NpcScriptManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "PetManager.h"
#include "TitanManager.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"

#include "SkillOptionCleardlg.h"
#include "SurvivalModeManager.h"

//#ifdef _TL_LOCAL_
//#include "BattleSystem_Client.h"
//#include "PartyWar.h"
//#endif

#include "SkinSelectDialog.h"
#include "CostumeSkinSelectDialog.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "SkillPointResetDlg.h"
//#include "TitanMugongMixDlg.h"	// magi82 - Titan(070611) 鸥捞藕 公傍函券 林籍贸府
#include "MouseCursor.h"
#include "FortWarDialog.h"
#include "FortWarManager.h"
#include "AIManager.h"			//[内挂管理][By:TT天语][QQ:2013734367][2017/12/9]
#include "OpenPyogukDlg.h"		//[开仓卷选择界面][By:TT天语][QQ:2013734367][2017/12/9]
#include "cInventoryRBTDlg.h"		//[背包右键菜单][By:TT天语][QQ:2013734367][2018/4/3]
#include "MarryManager.h"
#include "CharacterDialog.h"
//#include "ReDisConnManager.h"
#include "FastConfigPointDlg.h"
#include "cEquipItemInherit.h"		//[装备传承][BY:TT天语][QQ:2013734367][2019-8-22][14:48:09]
#include "PetInventoryDlg.h"		//[右键仓储][BY:TT天语][QQ:2013734367][2019-4-16][18:18:03]
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInventoryExDialog::CInventoryExDialog()
{
	m_type				= WT_INVENTORYDIALOG;
	m_pWearedDlg		= NULL;
	//m_pMoneyEdit		= NULL;
	m_fSpread			= FALSE;
	m_iInventoryState	= eITEMDEFAULT_STATE;
	m_pItemShopInven	= NULL;
	m_pItemShopInvenPlus = NULL;	//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
	m_pItemShopBtn		= NULL;
	m_bShopInven		= FALSE;

	m_BtnPushstartTime		= 0;
	m_BtnPushDelayTime		= 500;
	m_bPushTabWithMouseOver = FALSE;
	
	m_pSelectedShopItem = NULL;
	m_CanMoveTimeCount = 0;
	memset(&m_UseMsg,0,sizeof(MSG_SHOPITEM_USE_SYN));
	 p_Money=NULL;
}

CInventoryExDialog::~CInventoryExDialog()
{
	m_pWearedDlg		= NULL;
	m_pItemShopInven	= NULL;
	m_pItemShopBtn		= NULL;
	m_pItemShopInvenPlus= NULL;
}

void CInventoryExDialog::ReleaseInventory()
{

}

void CInventoryExDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}
void CInventoryExDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON && window->GetID() != IN_SHOPITEMBTN)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG && window->GetID() != IN_SHOPITEMGRID
		&&window->GetID()!=IN_SHOPITEMGRID_PLUS)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}


DWORD CInventoryExDialog::ActionEvent(CMouse * mouseInfo)
{
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( !m_bShopInven && WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( int i = 0 ; i < m_bTabNum ; i++ )
			{
				m_ppPushupTabBtn[i]->ActionEvent( mouseInfo );
				
				if( we & WE_MOUSEOVER )
				if( m_ppPushupTabBtn[i]->IsActive() && m_ppPushupTabBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
				{
					bMouseOver = TRUE;
					if( m_bPushTabWithMouseOver )
					{
						if( i != m_bSelTabNum )
						{
							SelectTab(i);
							m_bSelTabNum = i;							
						}
					}
					else
					{
						if( m_BtnPushstartTime == 0 )
							m_BtnPushstartTime = gCurTime;
						else if( gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime )
							m_bPushTabWithMouseOver = TRUE;
					}				
				}
			}
			
			if( !bMouseOver )
				m_BtnPushstartTime = 0;
			
			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );
			return we;
		}
		if(m_bShopInven&& WINDOWMGR->GetDragDlg()->GetType() != WT_ITEM)
		{
			m_pItemShopInvenPlus->SetAbsXY(GetAbsX()+251,GetAbsY()+25);
		}
	}
	m_BtnPushstartTime		= 0;
	m_bPushTabWithMouseOver = FALSE;
	return cTabDialog::ActionEvent( mouseInfo );	
}


WORD CInventoryExDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_INVENTORY_START ) / TABCELL_INVENTORY_NUM;
}

BOOL CInventoryExDialog::AddItem(ITEMBASE * itemBase)
{
	if( itemBase->dwDBIdx == 0 )
	{
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( itemBase, "AddItem" );
	if(newItem == NULL)
		return FALSE;
	
	return AddItem( newItem );
}

BOOL CInventoryExDialog::AddItem(CItem* pItem)
{
	if(pItem == NULL)
	{
		ASSERT(pItem);
		return FALSE;
	}

	ITEMMGR->RefreshItem( pItem );

	BOOL rt = FALSE;

	if(TP_INVENTORY_START <= pItem->GetPosition() && pItem->GetPosition() < TP_INVENTORY_END)
	{

		int tabIndex = GetTabIndex( pItem->GetPosition() );
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);

		WORD relPos = pItem->GetPosition()-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*tabIndex;
		CItem * item = (CItem * )dlg->GetIconForIdx(relPos);
		if(item)
		{
			//if(!RECONNMGR->IsInReCnnMode())
			//	ASSERT(0);
			return FALSE;
		}
		else
		{
			rt = dlg->AddIcon(relPos, pItem);
			ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_Inven);
		}
	}
	else if(TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		return m_pWearedDlg->AddItem(pItem->GetPosition()-TP_WEAR_START, pItem);
	}
	else if(m_pItemShopInven->InPt(pItem->GetPosition())/*TP_SHOPINVEN_START <= pItem->GetPosition() && pItem->GetPosition() < TP_SHOPINVEN_END-TABCELL_SHOPINVEN_NUM-TABCELL_SHOPINVEN_PLUS_NUM*/)
	{
		rt = m_pItemShopInven->AddItem( pItem );
		ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInven);
	}
	else if(m_pItemShopInvenPlus->InPt(pItem->GetPosition())/*TP_SHOPINVEN_START+TABCELL_SHOPINVEN_NUM<=pItem->GetPosition()&&pItem->GetPosition()<TP_SHOPINVEN_END*/)
	{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
		rt = m_pItemShopInvenPlus->AddItem(pItem);
		ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInvenPlus);
	}
	else
		return FALSE;
	//if(rt&&sCPlayerAI->sAiConfig.AutoSwitch[eAutoOpenBox])
	//{//自动开箱子  by:TT天语	QQ:2013734367
	//	if(pItem->GetItemKind()==eCHANGE_ITEM)
	//	{
	//		MSG_ITEM_USE_SYN msg;
	//		msg.Category = MP_ITEM;
	//		msg.Protocol = MP_ITEM_USE_CHANGEITEM_SYN;
	//		msg.dwObjectID = HEROID;
	//		msg.TargetPos = pItem->GetPosition();
	//		msg.wItemIdx = pItem->GetItemIdx();
	//		NETWORK->Send(&msg,sizeof(msg));
	//	}
	//}
	if (rt&&sCPlayerAI->sAiConfig.AutoSwitch[eAutoUseShopItem])
		ItemUnSealingSync(pItem->GetPosition());
	return rt;
}

BOOL CInventoryExDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
	if(TP_INVENTORY_START <= Pos && Pos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( Pos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(!Dlg->IsAddable(Pos - TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex))
			return Dlg->DeleteIcon(Pos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex, (cIcon **)ppItem);
		else
			return FALSE;
	}
	else if(TP_WEAR_START <= Pos && Pos < TP_WEAR_END)
	{
		if(!m_pWearedDlg->IsAddable(Pos - TP_WEAR_START))
			return m_pWearedDlg->DeleteItem(Pos-TP_WEAR_START, (cIcon **)ppItem);
		else
		{
			return FALSE;
		}
	}
	else if(m_pItemShopInven->InPt(Pos))
	{
		if(!m_pItemShopInven->IsAddable(Pos-TP_SHOPINVEN_START))
			return m_pItemShopInven->DeleteItem(Pos, ppItem);
		else

		{
			return FALSE;
		}
	}
	else if(m_pItemShopInvenPlus->InPt(Pos))
	{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
		if(!m_pItemShopInvenPlus->IsAddable(Pos-TP_SHOPINVEN_START-20))
			return m_pItemShopInvenPlus->DeleteItem(Pos,ppItem);
		else 
			return FALSE;
	}
	else
		return FALSE;

}
BOOL CInventoryExDialog::UpdateItemDurabilityAdd(POSTYPE absPos, int dur)
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		WORD relPos = absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex;
		CItem * item = (CItem * )Dlg->GetIconForIdx(relPos);
		item->SetDurability(item->GetDurability()+dur);
		return TRUE;
	}
	else if(m_pItemShopInven->InPt(absPos)/*TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END*/)
	{
		CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
		if(pItem)
		{
			pItem->SetDurability(pItem->GetDurability()+dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else if(m_pItemShopInvenPlus->InPt(absPos))
	{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
		CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(absPos-TP_SHOPINVEN_START-20);
		if(pItem)
		{
			pItem->SetDurability(pItem->GetDurability()+dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}
CItem * CInventoryExDialog::GetItemForPos(POSTYPE absPos)
{
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex);
	}
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos-TP_WEAR_START);
	}
	else if(m_pItemShopInven->InPt(absPos)/*TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END*/)
	{
		return (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
	}
	else if(m_pItemShopInvenPlus->InPt(absPos))
	{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
		return (CItem*)m_pItemShopInvenPlus->GetIconForIdx(absPos-TP_SHOPINVEN_START-20);
	}
	else
		return NULL;
}
#include "Quest.h"
void CInventoryExDialog::UseItem_InheritItemUse(CItem* pItem)
{
	if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
		return;
	ITEM_INFO * pInfo = NULL;
	pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());

	if (NULL == pInfo) return;

	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
	GAMEIN->GetEquipItemInherit()->SetShopItemInfo(pItem);
	if (pInfo->wPulsItemEffect == eEQUIP_ITEM_WEAPON)
		GAMEIN->GetEquipItemInherit()->Show(TRUE);
	else
		GAMEIN->GetEquipItemInherit()->Show();
	return;
}
void CInventoryExDialog::UseItem(CItem* pItem)
{
	CObject* pObjectTemp = OBJECTMGR->GetObject(HERO->GetID());
	if(NULL == pObjectTemp)
	{
#ifdef _GMTOOL_
		printf("[ItemUse Object Error] : [%d]\n", pObjectTemp->GetID());
#endif	
		return;
	}

	ASSERT(pItem);

	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )

	if( WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM)
		return;
	
	if( m_bDisable )	return;	

 	if( pItem->IsLocked() ) return;

	if( HERO->GetState() == eObjectState_Die )	
		return;

	if(HERO->IsHide())
	if(ITEMMGR->IsUnfitItemInfoListForHide(pItem->GetItemIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}

	eITEM_KINDBIT bits = pItem->GetItemKind();

	if( MAP->IsMapKind(eSurvivalMap) )
	{
		if( !(bits == eYOUNGYAK_ITEM)
			&& !(bits & eEQUIP_ITEM)
			&& !(bits & eSHOP_ITEM_MAKEUP)
			&& !(bits & eSHOP_ITEM_DECORATION)
			&& !(bits & eSHOP_ITEM_EQUIP)
			)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );
			return;
		}

		if( 8000 <= pItem->GetItemIdx() && pItem->GetItemIdx() <= 8013 )
		{
			if( SVVMODEMGR->GetCurModeState() == eSVVMode_Fight )
			if( FALSE == SVVMODEMGR->AddItemUsingCount() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
				return;
			}
		}
	}
	if (bits == eEQUIP_ITEM_INHERIT)
	{//[装备传承][BY:TT天语][QQ:2013734367][2019-8-22][14:46:22]
		UseItem_InheritItemUse(pItem);
		return;
	}
	if (bits & eYOUNGYAK_ITEM)
	{
		//[对药品增加回气时间判断][By:TT天语][QQ:2013734367][2017/11/27]
		YOUNGYAK_ITEM_DELAY_TYPE * Tmp = NULL;
		Tmp = GAMERESRCMNGR->GetYoungYakDelay()->GetData(pItem->GetItemIdx());
		if (Tmp)
		{
			DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckYoungYakPotionDelay(Tmp->wtype, &dwRemainTime);
			if (delayRate > 0.0f)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(909));
				return;
			}
		}

		if (GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
			return;

		if (bits == eYOUNGYAK_ITEM_PET)
		{
			if (PETMGR->GetCurSummonPet() == NULL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}
		}
		else if (bits == eYOUNGYAK_ITEM_UPGRADE_PET)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(), pItem->GetItemIdx());

			BOOL Grade = FALSE;

			if (pItem->GetItemIdx() == 36001)	Grade = ePUDG_Grade2;
			else if (pItem->GetItemIdx() == 36002)	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}

		else if (bits == eYOUNGYAK_ITEM_REVIVAL_PET)
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;

			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());

			BOOL Grade = FALSE;

			if (pItem->GetItemIdx() == 36003)	Grade = ePRDG_Grade1;
			else if (pItem->GetItemIdx() == 36004)	Grade = ePRDG_Grade2;
			else if (pItem->GetItemIdx() == 36005)	Grade = ePRDG_Grade3;

			GAMEIN->GetPetRevivalDialog()->SetActiveWithGradeTxt(Grade);
			return;
		}

		else if (bits == eYOUNGYAK_ITEM_TITAN)
		{
			if (FALSE == HERO->InTitan())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1655));
				return;
			}

			DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime);
			if (delayRate > 0.0f)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(909));
				return;
			}
		}

		if (pItem->GetItemIdx() == 10700)
		{
			if (HERO->GetSingleSpecialState(eSingleSpecialState_Detect))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1361));
				return;
			}
		}
		//#ifdef _TL_LOCAL_
		//		if( MAP->IsVillage() )
		//		if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_VIMUSTREET 
		//			|| PARTYWAR->IsPartyWar() )
		//		{
		//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1289) );
		//			return;
		//		}
		//
		//#endif	
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:TT天语][QQ:2013734367][2018/4/13]
		NETWORK->Send(&msg, sizeof(msg));

		//#ifdef TAIWAN_LOCAL
		//		if( pItem->GetItemIdx() == 53074 )
		//		{
		//			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
		//			CHATMGR->AddMsg( CTC_CHEAT_1, "8 \" 墨香元宵节特别奉献，                                 8" );
		//			CHATMGR->AddMsg( CTC_CHEAT_1, "8 东方墨香祝您家人朋友永远团团圆圆合合美美! \"          8" );
		//			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
		//		}
		//#endif
	}
	else if(bits & eCHANGE_ITEM)	
	{
		if (bits == eCHANGE_ITEM_LOCK)
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if (!pInfo)	return;
			if (HERO->GetLevel() < pInfo->ItemType)	return;
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_CHANGEITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();

		NETWORK->Send(&msg, sizeof(msg));
	}
	else if(bits & eEQUIP_ITEM)
	{
		if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
		{
			CAction act;
			act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
			HERO->SetNextAction(&act);
			return;
		}

		switch( GetTableIdxPosition( pItem->GetPosition() ) )
		{
		case eItemTable_Inventory:
			{
				if(ITEMMGR->CanEquip(pItem->GetItemIdx()))
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					if(!pInfo) return ;
					
					WORD EquipToPos = pInfo->EquipKind;
					if(pInfo->EquipKind == eWearedItem_Ring1)
					{
						if(IsExist(EquipToPos+TP_WEAR_START))
						{
							if(!IsExist(EquipToPos+1+TP_WEAR_START))
								EquipToPos++;
						}
					}
					CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
					FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
				}
			}
			break;
		case eItemTable_Weared:
			{
				POSTYPE emptyPos;
				if( GetBlankPositionRestrictRef( emptyPos ) )
				{
					CItem * pToItem = GetItemForPos( emptyPos );
					FakeGeneralItemMove(emptyPos, pItem, pToItem);					
				}
			}
			break;
		}
		
	}
	else if(bits & eMUGONG_ITEM)
	{
		if(bits == eMUGONG_ITEM_JOB)
		{
			if(!MUGONGMGR->SearchJobSkill())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "一个只使用一种生活技请先已学的生活技再学习");
				return;
			}
		}

		if(pItem->GetItemIdx() == 10750)
		{
			GAMEIN->GetSkillOptionClearDlg()->SetItem(pItem);
			GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);
			return;
		}
		if(MUGONGMGR->GetMugongByMugongIdx(pItem->GetItemInfo()->MugongNum) != NULL)
				return ;

		SKILL_CHANGE_INFO * pSkillChangeInfo = SKILLMGR->GetSkillChangeInfo(pItem->GetItemInfo()->MugongNum);
		if(pSkillChangeInfo)
		{
			if(MUGONGMGR->GetMugongByMugongIdx(pSkillChangeInfo->wTargetMugongIdx))
				return;
		}

		if( bits == eMUGONG_ITEM_TITAN )
		{
			if( HERO->InTitan() == FALSE )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1653));
				return;
			}
		}

		if(bits == eMUGONG_ITEM_JINBUB)
		{
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),MUGONGTYPE_JINBUB))
				return;
		}
		else if(bits == eMUGONG_ITEM_KYUNGGONG)	
		{
			if(KYUNGGONGMGR->IsSetableKyungGong(HERO,pItem) == FALSE)
				return;
		}
		else if(bits == eMUGONG_ITEM_JOB)
		{
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(), MUGONGTYPE_JOB))
				return;
		}
		else
		{
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),MUGONGTYPE_NORMAL))
				return;				
		}

		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();
		msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:TT天语][QQ:2013734367][2018/4/13]
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if(bits & eEXTRA_ITEM)
	{

		if(pItem->GetItemIdx() == 50510)
		{
			GAMEIN->GetGuildNoteDlg()->Show(pItem);
		}

		if(pItem->GetItemIdx() == 50511)
		{
			GAMEIN->GetUnionNoteDlg()->Show(pItem);
		}
		if(bits & eEXTRA_ITEM_MATERIAL)
	    {
			if(pItem->GetItemIdx()==eItemLockStone)
			{
				CItemLockDialog * pDlg=(CItemLockDialog*)GAMEIN->GetItemLockDialog();

				ITEM_INFO * pInfo=ITEMMGR->GetItemInfo(pItem->GetItemIdx());

				pDlg->SetLockItem(pItem);

				if(!pInfo)
				{
					return;
				}
				if(pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);

					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
#ifdef	_ABRASION_
			if (pItem->GetItemIdx()==eItemMixEquiLow||
				pItem->GetItemIdx()==eItemMixEquiMid||
				pItem->GetItemIdx()==eItemMixEquiMax)
			{//[装备修理][By:TT天语][QQ:2013734367][2018/2/4]
				CItemUnLockDialog * pDlg=(CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();

				ITEM_INFO * pInfo=ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				pDlg->SetLockType(eUnMixEqui);
				pDlg->SetLockItem(pItem);
				if(!pInfo)
				{
					return;
				}
				if(pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
#endif
			if (pItem->GetItemIdx()==eItemUnLockStoneForPD)
			{//[泡点解锁][By:TT天语][QQ:2013734367][2018/1/1]
				CItemUnLockDialog * pDlg=(CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();

				ITEM_INFO * pInfo=ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				pDlg->SetLockType(eUnLockForPD);
				pDlg->SetLockItem(pItem);
				if(!pInfo)
				{
					return;
				}
				if(pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
			if(pItem->GetItemIdx()==eItemUnLockStone)
			{//[锁定解锁][By:TT天语][QQ:2013734367][2018/1/1]
				CItemUnLockDialog * pDlg=(CItemUnLockDialog*)GAMEIN->GetItemUnLockDialog();

				ITEM_INFO * pInfo=ITEMMGR->GetItemInfo(pItem->GetItemIdx());
				pDlg->SetLockType(eUnLockForLock);
				pDlg->SetLockItem(pItem);
				if(!pInfo)
				{
					return;
				}
				if(pDlg->IsActive())
				{
					pDlg->SetActive(FALSE);
					pItem->SetLock(FALSE);
				}
				else
				{
					pDlg->SetActive(TRUE);
				}
			}
		}
	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{

	}
	else if(bits & eQUEST_ITEM)
	{
		if( bits == eQUEST_ITEM_START )	
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;
			if( !QUESTMGR->IsQuestStarted( pInfo->ItemType ) )
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(651));
			else
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUESTSTART, MBT_YESNO, CHATMGR->GetChatMsg(650));

			m_pTempQuestItem = pItem;
		}
		else if( bits == eQUEST_ITEM_PET )
		{
			if(2 == HERO->GetBattleTeam())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
				return;
			}

			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;

			if( FALSE == PETMGR->CheckResummonAvailable() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
				return;
			}

			if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
				return;
			}

			if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
				return;
			}
			if(pInfo->LimitLevel > HERO->GetMaxLevel())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1242));
				return;
			}

			MOVEMGR->HeroMoveStop();

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
			msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:TT天语][QQ:2013734367][2018/4/13]
			NETWORK->Send(&msg,sizeof(msg));
		}
		else if( bits == eQUEST_ITEM_EQUIP )	
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return;
			}
			
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )

					{
						ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if( !pInfo ) return;

						WORD EquipToPos = pInfo->EquipKind;
						CItem* pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
					}
				}	
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
	}
	else if(bits & eSHOP_ITEM)
	{
		if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(751) );
			return;
		}

		if( GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
			return;

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo)		return;
		if(pInfo->ItemKind == eSHOP_ITEM_GOLDITEM )
		{//[元宝物品使用][By:TT天语][QQ:2013734367][2018/2/2]
			MSGGOLDITEM msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEM_GOLDITEMUSE_SYN;
			msg.dwObjectID = HERO->GetID();
			msg.dwItemIdx = pInfo->ItemIdx;
			msg.dwItemDBidx = pItem->GetDBIdx();
			msg.dwGoldMoney =  pInfo->SellPrice;
			msg.dwItemPos = pItem->GetPosition();
			NETWORK->Send( &msg, sizeof(msg) );
			return;
		}
		if(pInfo->NaeRyukRecoverRate > HERO->GetLevel())
		{
			char temp[256] = {0, };
			sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate );
			CHATMGR->AddMsg( CTC_SYSMSG, temp );
			return;
		}


		if( bits == eSHOP_ITEM_EQUIP )
		{
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_ShopInven:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )
					{
						ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if(!pInfo) return ;

						WORD EquipToPos = pInfo->EquipKind;
						if(pInfo->EquipKind == eWearedItem_Ring1)
						{
							if(IsExist(EquipToPos+TP_WEAR_START))
							{
								if(!IsExist(EquipToPos+1+TP_WEAR_START))
									EquipToPos++;
							}
						}
						CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
						return;
					}
				}
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( m_pItemShopInven->GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
						return;
					}
					else if(m_pItemShopInvenPlus->GetBlankPositionRestrictRef(emptyPos))
					{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
						return;
					}
				}
				break;
			}
		}


		if( pInfo->ItemIdx == eIncantation_MemoryStone )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;			

			DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
			if( ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMove2 ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend7 ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend30 ) )
				Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

			if( GAMEIN->GetMoveDialog()->GetSavedCount() >= Validsavenum )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(758) );
				return;
			}

			DWORD dwCheckBit = eBossMap | eQuestRoom | eTournament;
			if( MAP->IsMapKind(dwCheckBit) || MAP->IsMapSame(eRunningMap) )
				return;

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetPointSaveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( TRUE );
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );


			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove || pInfo->ItemIdx == eIncantation_MemoryMove )
		{
			if(HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitLevel )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}


			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
				return;
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			char buf[128] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(761), pObject->GetObjectName() );
			m_pSelectedShopItem = pItem;

			WINDOWMGR->MsgBox( MBI_OTHERREVIVE, MBT_YESNO, buf );
		}
		else if( pInfo->ItemIdx == eIncantation_CharChange || pInfo->ItemIdx == eIncantation_ShapeChange )
		{			
			if(MAP->IsTitanMap())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1580) );				
				return;
			}
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			for(int i=0; i<eAvatar_Effect; ++i)
			{
				if( HERO->GetShopItemStats()->Avatar[i] )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1184) );
					return;
				}
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetCharChangeDlg()->SetItemInfo( pItem->GetPosition() );
			GAMEIN->GetCharChangeDlg()->SetCharacterInfo( HERO->GetCharacterTotalInfo() );
			if( pInfo->ItemIdx == eIncantation_CharChange )
				GAMEIN->GetCharChangeDlg()->SetShapeChange( FALSE );
			else
				GAMEIN->GetCharChangeDlg()->SetShapeChange( TRUE );
			GAMEIN->GetCharChangeDlg()->SetActive( TRUE );

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			
		}
		else if( pInfo->ItemIdx == eIncantation_ItemSeal )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetSealDlg() )
			{
				GAMEIN->GetSealDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetSealDlg()->SetActive( TRUE );
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ReinforceReset )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetReinforceResetDlg() )
			{
				GAMEIN->GetReinforceResetDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetReinforceResetDlg()->SetActive( TRUE );
			}
		}	
		else if(pInfo->ItemIdx == eMarryReckon_15||
				pInfo->ItemIdx == eMarryReckon_7||
				pInfo->ItemIdx == eMarryReckon_3)
		{//[夫妻传送卷][By:TT天语][QQ:2013734367][2018/9/11]
			if(!HERO->IsMarry())
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2164));
				return ;
			}
			MARRYMGR->SendBeckonMsg(pItem);
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION ||
			pInfo->ItemKind == eSHOP_ITEM_IMAGENAME)//[图片称号物品使用设置][By:TT天语][QQ:2013734367][2017/11/29]
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pInfo->ItemIdx );
			if(!pAvatarItem)
				return;

			if( pAvatarItem->Gender < 2 && pAvatarItem->Gender != HERO->GetGender() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(778) );
				return;
			}			
			else
			{
				if( HERO->GetState() == eObjectState_Move )
					MOVEMGR->HeroMoveStop();

				else if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
					return;
				}

				MSG_WORD2 msg;
				msg.Category = MP_ITEM;
				if( pItem->GetUseParam() )
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
				else
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pItem->GetItemIdx();
				msg.wData2 = pItem->GetPosition();				
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->Life )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
				return;
			}
			
			GAMEIN->GetSkPointDlg()->SetCurItem( pItem->GetItemIdx(), pItem->GetPosition() );
			
			char buf[128];
			sprintf( buf, CHATMGR->GetChatMsg(737), pInfo->Life );
			WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_USE, MBT_YESNO, buf );			
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove15 ||
			pInfo->ItemIdx == eIncantation_MemoryMove15 ||
			pInfo->ItemIdx == eIncantation_TownMove7 ||
			pInfo->ItemIdx == eIncantation_TownMove7_NoTrade ||
			pInfo->ItemIdx == eIncantation_MemoryMove7 ||
			pInfo->ItemIdx == eIncantation_MemoryMove7_NoTrade ||
			pInfo->ItemIdx == 55357 ||
			pInfo->ItemIdx == 55362 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend ||
			pInfo->ItemIdx == eIncantation_MemoryMove2 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend7 || 
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend30 )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove15 ||
				pInfo->ItemIdx == eIncantation_TownMove7 || 
				pInfo->ItemIdx == eIncantation_TownMove7_NoTrade )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_ShowPyoguk
				|| pInfo->ItemIdx == eIncantation_ShowPyoguk7
				|| pInfo->ItemIdx == eIncantation_ShowPyoguk7_NoTrade )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			if (!GAMEIN->GetOpenPyogukDlg()->IsActive())
			{
				GAMEIN->GetOpenPyogukDlg()->SetActive(TRUE);
			}
		}
		else if( pInfo->ItemIdx == eSundries_RareItemCreate50 ||
			pInfo->ItemIdx == eSundries_RareItemCreate70 ||
			pInfo->ItemIdx == eSundries_RareItemCreate90 )
		{
			if (HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal)
				return;
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Deal);
			/*if (pItem->GetStatic()==1)
			{
			WINDOWMGR->MsgBox(MSBOX_LHTIP,MBT_YESNO,CHATMGR->GetChatMsg(2260));
			GAMEIN->GetRareCreateDlg()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );

			}
			else
			{*///[獁翴厚杆称玛][2018/5/1 20:40]
			GAMEIN->GetRareCreateDlg()->SetShopItemInfo(pItem->GetItemIdx(), pItem->GetPosition());
			GAMEIN->GetRareCreateDlg()->Clear_RARE_INFO();
			GAMEIN->GetRareCreateDlg()->SetActive(TRUE);
			/*}*/
		}
		else if( pInfo->ItemIdx == eIncantation_PetRevival )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPetRevivalDialog()->SetActive( TRUE );
		}
		else if( pInfo->ItemIdx == eIncantation_PetGrade2
			|| pInfo->ItemIdx == eIncantation_PetGrade3 )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(),pItem->GetItemIdx());

			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == eIncantation_PetGrade2 )	Grade = ePUDG_Grade2;
			else if( pItem->GetItemIdx() == eIncantation_PetGrade3 )	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->GenGol )
		{
			if( pItem->GetItemIdx() >= 55633 && pItem->GetItemIdx() <= 55635 )
			{
				GAMEIN->GetReinforceDialog()->Release();
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetReinforceDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				pItem->SetLock( TRUE );
				/*if(pItem->GetStatic()==1)
					WINDOWMGR->MsgBox(MSBOX_QHTIP,MBT_YESNO,CHATMGR->GetChatMsg(2260));
				else*///[泡点强化加锁取消][By:TT天语][QQ:2013734367][2018/5/1 20:40]
					GAMEIN->GetReinforceDialog()->SetActiveRecursive( TRUE );
			}
			if( pItem->GetItemIdx() >= 55636 && pItem->GetItemIdx() <= 55638 )
			{
				GAMEIN->GetMixDialog()->Release();
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetMixDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				pItem->SetLock( TRUE );

				/*if(pItem->GetStatic()==1)
					WINDOWMGR->MsgBox(MSBOX_ZHTIP,MBT_YESNO,CHATMGR->GetChatMsg(2260));
				else*///[泡点组合加锁取消][By:TT天语][QQ:2013734367][2018/5/1 20:39]
					GAMEIN->GetMixDialog()->SetActiveRecursive( TRUE );
			}
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->CheRyuk )
		{
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;

			if( ( HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal )
					&& !VIMUMGR->IsVimuing() )
			{
				STREETSTALLMGR->ChangeDialogState();
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeJob )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			if( HERO->GetStage() != eStage_Hwa && HERO->GetStage() != eStage_Geuk )
				return;
		
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetChangeJobDlg()->SetActive( TRUE );
			GAMEIN->GetChangeJobDlg()->SetItemInfo( pItem->GetPosition(), pItem->GetDBIdx() );
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeName || pInfo->ItemIdx == eIncantation_ChangeName_Dntrade )
		{				
			if( HERO->IsPKMode() || VIMUMGR->IsVimuing() || PKMGR->IsPKLooted() || ( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal ) )
				return;

			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		}
//#ifdef _JAPAN_LOCAL_
//		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
//			return;
//		}
//#endif
//#ifdef _HK_LOCAL_
//		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
//			return;
//		}
//#endif
//#ifdef _TL_LOCAL_
//		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
//		{
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
//			return;
//		}
//		else if( pInfo->ItemIdx == eIncantation_MugongExtend && HERO->GetExtraMugongSlot() >= 2 )
//		{
//			CHATMGR->AddMsg( CTC_ALERT_YELLOW, CHATMGR->GetChatMsg(1435) );
//			return;
//		}
//#endif
		else if( pInfo->ItemIdx == eSundries_Shout || pInfo->ItemIdx == eSundries_Shout_Once 
			|| pInfo->ItemIdx == eSundries_Shout_Once_NoTrade )
		{
			GAMEIN->GetShoutDlg()->Show();
			GAMEIN->GetShoutDlg()->SetItemInfo( pInfo->ItemIdx, pItem->GetPosition() );
			return;
		}
		else if( pInfo->ItemIdx == eSundries_SuperShout || pInfo->ItemIdx == eSundries_SuperShout_Once 
			|| pInfo->ItemIdx == eSundries_SuperShout_Once_NoTrade )
		{//[超级狮子吼][By:TT天语][QQ:2013734367][2018/2/19]
			GAMEIN->GetShoutDlg()->Show( TRUE );
			GAMEIN->GetShoutDlg()->SetItemInfo( pInfo->ItemIdx, pItem->GetPosition() );
			return;
		}
		else if( pInfo->ItemIdx == eIncantation_MugongOptionReset )
		{
			if( GAMEIN->GetSkillOptionClearDlg()->IsActive() )
				return;

			pItem->SetLock( TRUE );
			GAMEIN->GetSkillOptionClearDlg()->SetItem( pItem );
			GAMEIN->GetSkillOptionClearDlg()->SetActive( TRUE );
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
		{
			if( PETMGR->GetCurSummonPet() == NULL )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}

			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();

			POSTYPE	EmptyPos = 0;
			if(pPetEquipDlg->GetBlankPositionRestrictRef(EmptyPos))
			{
				CItem* pToItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos(EmptyPos);
				FakeGeneralItemMove(EmptyPos, pItem, pToItem);
				return;
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1287) );
				return;
			}
		}
		else if(pInfo->ItemIdx == eIncantation_Curse_Cancellation)
		{
			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetShopItemPosion(pItem->GetPosition());

			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
		{
			if( HERO->InTitan() == FALSE )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1679) );
				return;

			}

			switch( GetTableIdxPosition(pItem->GetPosition()) )
			{
			case eItemTable_ShopInven:
				{
					POSTYPE	EmptyPos = 0;
					if(GAMEIN->GetTitanInventoryDlg()->GetBlankSlotPosition(EmptyPos))
					{
						CItem* pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos(EmptyPos);
						FakeGeneralItemMove(EmptyPos, pItem, pToItem);
						return;
					}
					else
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1680) );
						return;
					}
				}
				break;
			case eItemTable_TitanShopItem:
				{
					POSTYPE EmptyPos = 0;
					if( m_pItemShopInven->GetBlankPositionRestrictRef(EmptyPos) == TRUE )
					{
						CItem* pToItem = GetItemForPos(EmptyPos);
						FakeGeneralItemMove(EmptyPos, pItem, pToItem);
					}
					else if(m_pItemShopInvenPlus->GetBlankPositionRestrictRef(EmptyPos)==TRUE)
					{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
						CItem* pToItem = GetItemForPos(EmptyPos);
						FakeGeneralItemMove(EmptyPos,pItem,pToItem);
					}
				}
				break;
			}
		}		
		else if(pInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN)	
		{
			if( GAMEIN->GetSkinSelectDlg()->IsActive() == FALSE )
				GAMEIN->GetSkinSelectDlg()->SetActive(TRUE);

			return;
		}
		else if(pInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN)	
		{
			if( GAMEIN->GetCostumeSkinSelectDlg()->IsActive() == FALSE )
				GAMEIN->GetCostumeSkinSelectDlg()->SetActive(TRUE);

			return;
		}
		else if(pInfo->ItemIdx == eIncantation_StatusPointReset_71 ||
				pInfo->ItemIdx == eIncantation_StatusPointReset_1_50 ||
				pInfo->ItemIdx == eIncantation_StatusPointReset_1_99)
		{
			if( HERO->GetShopItemStats()->UseStatePoint > 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1688) );
				return;
			}

			int nMaxLevel = HERO->GetMaxLevel();
			if( nMaxLevel < pInfo->RangeType || nMaxLevel > pInfo->EquipKind )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
				return;
			}

			//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			//ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			//ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			/*m_pSelectedShopItem = pItem;*/
			/*WINDOWMGR->MsgBox(MBI_STATUS_POINT_RESET, MBT_YESNO, CHATMGR->GetChatMsg(1683));*/
			//[洗点卷不需点击确定修改][By:TT天语][QQ:2013734367][2018/5/1 20:44]
			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
			msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
			msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
			msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
			GAMEIN->GetCharacterDialog()->InitAutoPoint();
			NETWORK->Send(&msg,sizeof(msg));

			return;
		}
		else if(pInfo->ItemIdx == eFastCofigPoint)
		{//[快速加点卷][BY:TT天语][QQ:2013734367][2019-4-5][19:32:33]
			int nMaxLevel = HERO->GetMaxLevel();
			if( nMaxLevel < pInfo->RangeType || nMaxLevel > pInfo->EquipKind )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
				return;
			}
			FastConfigPointDlg* pDlg = GAMEIN->GetFastConfigPointDlg();
			if(pDlg)
			{
				if(!pDlg->IsActive())
				{
					pDlg->SetItemInfo(pItem);
					pDlg->SetActive(TRUE);
				}
			}
			
		}
		else if(pInfo->ItemIdx == eIncantation_SkillPoint_Reset)
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(734) );
				return;
			}

			GAMEIN->GetSkillPointResetDlg()->SetShopItemPos(pItem->GetPosition());
			GAMEIN->GetSkillPointResetDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if(pInfo->ItemIdx == eLazyItem_N||pInfo->ItemIdx == eLazyItem_J)
		{//[牛巨任务物品使用][By:TT天语][QQ:2013734367][2018/4/20]
			if(MAP->GetMapNum()==75||MAP->GetMapNum()==72)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2346) );
				return;
			}
			BOOL IsGetQuest = FALSE;
			if(pInfo->ItemIdx == eLazyItem_N)
				IsGetQuest = QUESTMGR->IsStartQuest(286);
			else if(pInfo->ItemIdx == eLazyItem_J)
				IsGetQuest = QUESTMGR->IsStartQuest(283);
			if(IsGetQuest)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2277) );
				return;
			}
			MSGNJQUESTITEMUSE msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEX_NJQUEST_SYN;
			msg.dwObjectID = HEROID;
			msg.dwItemDBidx = pItem->GetDBIdx();
			msg.dwItemIdx = pItem->GetItemIdx();
			msg.wPos = pItem->GetPosition();

			if(pInfo->ItemIdx == eLazyItem_N)
			{//[牛任务是否是重复任务判断][BY:TT天语][QQ:2013734367][2019-4-14][22:54:58]
				CQuest* pQuest = QUESTMGR->GetQuest( 153 );
				if( pQuest->IsQuestState( 45 ) != 1 )
					msg.bRepeat = FALSE;
			}
			else if(pInfo->ItemIdx == eLazyItem_J)
			{//[巨任务是否是重复任务判断][BY:TT天语][QQ:2013734367][2019-4-14][22:54:58]
				CQuest* pQuest = QUESTMGR->GetQuest( 23 );
				if( pQuest->IsQuestState( 69 ) != 1 )
					msg.bRepeat = FALSE;
			}
			NETWORK->Send(&msg,sizeof(msg));
			return;
		}
		else
		{
			if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->EquipKind )
			{
				if( HERO->GetMaxLevel() < pInfo->EquipKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(931) );
					return;
				}
			}
			else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
			{
				if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
				else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
				else if( pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90 ) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
			}

			if( pInfo->ItemKind == eSHOP_ITEM_PET )
			{
				if(2 == HERO->GetBattleTeam())
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
					return;
				}

				if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
				////////////////////////////////////////////////////////////////////////////////
				if( FALSE == PETMGR->CheckResummonAvailable() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
					return;
				}

				if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) 
					&& !PETMGR->CheckCurSummonPetKindIs(ePK_EventPet) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
					return;
				}

				if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
				{

					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
					return;
				}
				if(pInfo->EquipKind > HERO->GetMaxLevel())
				{
					return;
				}
				
				MOVEMGR->HeroMoveStop();
			}
			if(pInfo->ItemKind==eSHOP_ITEM_MARRYCHARM )//同心符咒使用判断	by:TT天语	QQ:2013734367
			{
				if(!HERO->IsMarry())
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(2164));
					return;	
				}
			}
			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
			msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
			msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
			msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
			/*if (pItem->GetItemIdx()==eIncantation_MixUp&&pItem->GetStatic()==1)
			{
				m_UseMsg = msg;
				WINDOWMGR->MsgBox(MSBOX_DFTIP,MBT_YESNO,CHATMGR->GetChatMsg(2260));
				return;
			}*///[泡点多福加锁取消][By:TT天语][QQ:2013734367][2018/5/1 20:41]
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else if(bits & eTITAN_EQUIPITEM)
	{		
		if( HERO->InTitan() )
		{
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					if(!pInfo) return ;					

					DWORD EquipToPos = pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET;

					CItem * pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos( (POSTYPE)(EquipToPos + TP_TITANWEAR_START) );
					FakeGeneralItemMove((POSTYPE)(EquipToPos + TP_TITANWEAR_START), pItem, pToItem);
				}
				break;
			case eItemTable_Titan:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
		else
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1668));
		}
	}
	else if( bits & eTITAN_ITEM )
	{
		if( eTITAN_ITEM_PAPER == bits )
		{
			if(HERO->GetState() != eObjectState_None)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1548));
				return;
			}

			if( FALSE == TITANMGR->IsRegistedTitan(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1510) );
				return;
			}

			if( TITANMGR->GetCurRidingTitan() && TITANMGR->GetCallItemDBIdx() != pItem->GetDBIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1549) );
				return;
			}
			
			if( NULL == TITANMGR->GetCurRidingTitan() )
			{
				if( FALSE == TITANMGR->CheckRecallAvailable() )
				{
					stTIME stCurTime = TITANMGR->GetTitanResummonTime();
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1545), GetCurTimeToString(&stCurTime) );
					return;
				}

				if( FALSE == TITANMGR->CheckMasterLifeAmountForRiding() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(403) );
					return;
				}
			}

			if(MUSSANGMGR->IsMussangMode() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
				return;
			}

			MOVEMGR->HeroMoveStop();

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
			msg.dwItemDBIdx = pItem->GetDBIdx();//[增加物品使用数据库编号判断][By:TT天语][QQ:2013734367][2018/4/13]
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CInventoryExDialog::SendUseDF()
{
	if (m_UseMsg.UseBaseInfo.ShopItemIdx!=0)
	{
		NETWORK->Send(&m_UseMsg,sizeof(m_UseMsg));
	}
}
void CInventoryExDialog::ClearDFMsg()
{
	memset(&m_UseMsg,0,sizeof(m_UseMsg));
}
void CInventoryExDialog::SetActive(BOOL val)
{
	if( !m_bDisable )
	{
		if(val == FALSE)
		{
			CDealDialog * dDlg = GAMEIN->GetDealDialog();
			if(dDlg)
				if(dDlg->IsShow())
					dDlg->SetActive(FALSE);

			CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();

			if(pPyogukDlg->IsActive())
				pPyogukDlg->SetActive(FALSE);

			if(GAMEIN->GetGuildWarehouseDlg()->IsActive())
				GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);
		}
		else
		{
		}

		cTabDialog::SetActive(val);



//#ifdef _JAPAN_LOCAL_
//
//		if( !val )			return;
//
//		if( HERO == NULL )	return;
//
//		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
//		for(int i=0; i<4; ++i)
//		{
//			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
//			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );
//
//			if( i < nActive )
//			{
//				if( pBtn )		pBtn->SetActive( TRUE );
//				if( pGrid)		pGrid->SetActive( TRUE );
//			}
//			else
//			{
//				if( pBtn )		pBtn->SetActive( FALSE );
//				if( pGrid)		pGrid->SetActive( FALSE );
//			}	
//		}
//
//		if( m_pItemShopBtn )
//		{
//			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
//			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
//			if( pBtn1 && pBtn2 )
//			{
//				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
//					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );
//
//				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
//					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
//			}
//		}
//
//#endif	// _JAPAN_LOCAL_
//
//#ifdef _HK_LOCAL_
//
//		if( !val )			return;
//
//		if( HERO == NULL )	return;
//
//		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
//		for(int i=0; i<4; ++i)
//		{
//			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
//			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );
//
//			if( i < nActive )
//			{
//				if( pBtn )		pBtn->SetActive( TRUE );
//				if( pGrid)		pGrid->SetActive( TRUE );
//			}
//			else
//			{
//				if( pBtn )		pBtn->SetActive( FALSE );
//				if( pGrid)		pGrid->SetActive( FALSE );
//			}	
//		}
//
//		if( m_pItemShopBtn )
//		{
//			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
//			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
//			if( pBtn1 && pBtn2 )
//			{
//				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
//					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );
//
//				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
//					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
//			}
//		}
//
//#endif	// _HK_LOCAL_
//
//#ifdef _TL_LOCAL_
//
//		if( !val )			return;
//
//		if( HERO == NULL )	return;
//
//		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
//		for(int i=0; i<4; ++i)
//		{
//			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
//			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );
//
//			if( i < nActive )
//			{
//				if( pBtn )		pBtn->SetActive( TRUE );
//				if( pGrid)		pGrid->SetActive( TRUE );
//			}
//			else
//			{
//
//				if( pBtn )		pBtn->SetActive( FALSE );
//				if( pGrid)		pGrid->SetActive( FALSE );
//			}	
//		}
//
//		if( m_pItemShopBtn )
//		{
//			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
//			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
//			if( pBtn1 && pBtn2 )
//			{
//				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
//					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );
//
//				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
//					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
//			}
//		}
//
//#endif	// _TL_LOCAL_

		if( m_bShopInven )
		{
			OnShopItemBtn();
		}
		else
		{
			m_pItemShopBtn->SetPush(FALSE);
			m_pItemShopInven->SetActive(FALSE);
			m_pItemShopInvenPlus->SetActive(FALSE);
		}
	}
}


void CInventoryExDialog::OnShopItemBtn()
{	
	for(int i=0; i<m_bTabNum; i++)
	{
		m_ppPushupTabBtn[i]->SetPush(FALSE);
		m_ppWindowTabSheet[i]->SetActive(FALSE);				
	}
	
	m_pItemShopBtn->SetPush(TRUE);
	m_pItemShopInven->SetActive(TRUE);
	m_pItemShopInvenPlus->SetAbsXY(GetAbsX()+251,GetAbsY()+25);
	m_pItemShopInvenPlus->SetActive(TRUE);
	m_bShopInven = TRUE;
	this->SetWH(468,GetHeight());
}

void CInventoryExDialog::OffShopItemBtn(LONG lId)
{
	if(m_pItemShopBtn->IsPushed() == FALSE)
		return;

	m_pItemShopBtn->SetPush(FALSE);
	m_pItemShopInven->SetActive(FALSE);
	m_pItemShopInvenPlus->SetActive(FALSE);
	for(int i=0; i<m_bTabNum; i++)
	{
		if(lId == m_ppPushupTabBtn[i]->GetID())
			SelectTab(i);
	}

	m_bShopInven = FALSE;
	this->SetWH(m_dWidth,GetHeight());
}

#include "ItemShopDialog.h"
void CInventoryExDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
 	if(we == WE_LBTNDBLCLICK)
	{
		if(GAMEIN->GetInventoryRBTDlg()->IsActive())
			GAMEIN->GetInventoryRBTDlg()->SetActive(FALSE);
		CItem * pItem = NULL;
		if(lId == IN_WEAREDDLG)
		{
			pItem = (CItem*)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
			if( !pItem ) return;
		}
		else if(lId == IN_SHOPITEMGRID)
		{
			pItem = (CItem*)m_pItemShopInven->GetIconForIdx((WORD)m_pItemShopInven->GetCurSelCellPos());

			if( !pItem )			return;			

			if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
			{
				if( pItem->IsLocked() ) return;
				WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg(774));
				return;
			}
		}
		else if(lId == IN_SHOPITEMGRID_PLUS)
		{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
			pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx((WORD)m_pItemShopInvenPlus->GetCurSelCellPos());

			if( !pItem )			return;			

			if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
			{
				if( pItem->IsLocked() ) return;
				WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg(774));
				return;
			}
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if( !gridDlg ) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if( !pItem ) return;
		}
		
		if(CURSOR->GetCursor() != eCURSOR_TITANREPAIR)
		{
			UseItem(pItem);
		}

	}
	else if(we == WE_LBTNCLICK)
	{
		if(lId == IN_SHOPITEMGRID)
			m_pItemShopInvenPlus->SetCurSelCellPos(-1);
		else if(lId == IN_SHOPITEMGRID_PLUS)
			m_pItemShopInven->SetCurSelCellPos(-1);
		if(GAMEIN->GetInventoryRBTDlg()->IsActive())
			GAMEIN->GetInventoryRBTDlg()->SetActive(FALSE);
		if( CURSOR->GetCursor() == eCURSOR_TITANREPAIR && lId != IN_WEAREDDLG && lId != IN_SHOPITEMGRID )
		{
			CItem * pItem = NULL;
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if( !gridDlg ) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if( !pItem ) return;


			if(pItem->GetItemKind() & eTITAN_EQUIPITEM)

			{
                TITANMGR->SetTitanRepairItem(pItem);

				TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
				if( pInfo )
				{
					float fEnduranceRate = (float)pInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
					if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
					{
						fEnduranceRate = 1.0f;
					}
					DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

					WINDOWMGR->MsgBox( MBI_TITAN_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1538), dwRepairPrice );
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Titan);
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1539) );
			}
		}

		if(m_iInventoryState == eITEMUPGRADE_STATE)
		{

		}
		else if(m_iInventoryState == eITEMDEAL_STATE)
		{		
			if(!GAMEIN->GetDealDialog()->IsShow()) return;
			CItem * pItemTT = GetCurSelectedItem(INV);
			if(!pItemTT)
			{
				GAMEIN->GetDealDialog()->SetMoney(0, 2);
				return;
			}
			
			int itemIDX = pItemTT->GetItemIdx();
			ITEM_INFO * pItem = ITEMMGR->GetItemInfo(itemIDX);
			ASSERT(pItem);
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->SellPrice );
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->SellPrice );
			GAMEIN->GetDealDialog()->SetMoney(SellPrice, 0);
		}
	}
	else if(we == WE_RBTNCLICK)
	{

		CItem * pItem = NULL;
		if(lId == IN_WEAREDDLG)
			return;
		else if(lId == IN_SHOPITEMGRID)
		{
			pItem = (CItem*)m_pItemShopInven->GetIconForIdx((WORD)m_pItemShopInven->GetCurSelCellPos());
		}
		else if(lId == IN_SHOPITEMGRID_PLUS)
		{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
			pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx((WORD)m_pItemShopInvenPlus->GetCurSelCellPos());
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if( !gridDlg ) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
		}
		if( !pItem ) return;

		GAMEIN->GetInventoryRBTDlg()->SetSelectItem(pItem);
		CDealDialog * pDealDlg =  GAMEIN->GetDealDialog();

		if(pDealDlg)
		{//[右键出售][BY:TT天语][QQ:2013734367][2019-4-16][21:29:42]
			if(pDealDlg->IsActive())
			{//[NPC商店打开，出售物品][BY:TT天语][QQ:2013734367][2019-4-16][17:39:16]
				pDealDlg->FakeMoveIcon(0,0,pItem);
				return;
			}
		}
		if(lId==IN_SHOPITEMGRID||lId == IN_SHOPITEMGRID_PLUS)
		{//[百宝右键仓储][BY:TT天语][QQ:2013734367][2019-4-16][16:56:21]
			CItemShopDialog* pItemShopDlg = GAMEIN->GetItemShopDialog();
			if(pItemShopDlg)
			{
				if(pItemShopDlg->IsActive())
				{//[百宝仓库打开，判断选中物品类型是否可存入][BY:TT天语][QQ:2013734367][2019-4-16][17:40:38]
						pItemShopDlg->MoveItemToItemShop(pItem);
						return;
				}
			}
		}
		else if(lId != IN_WEAREDDLG)
		{//[普通物品右键仓储][BY:TT天语][QQ:2013734367][2019-4-16][16:56:21]
			CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
			if(pPyogukDlg)
			{
				if(pPyogukDlg->IsActive())
				{//[普通仓库打开，判断选中物品类型是否可存入][BY:TT天语][QQ:2013734367][2019-4-16][17:41:58]
					pPyogukDlg->MoveItemToPyoguk(pItem);
					return;
				}
			}
			CPetInventoryDlg* pPetInven = GAMEIN->GetPetInventoryDialog();
			if (pPetInven)
			{
				if (pPetInven->IsActive())
				{
					pPetInven->MoveItemToPetInventory(pItem);
					return;
				}
			}
			CGuildWarehouseDialog* pGuildPyogukDlg = GAMEIN->GetGuildWarehouseDlg();
			if (pGuildPyogukDlg)
			{
				if (pGuildPyogukDlg->IsActive())
				{
					pGuildPyogukDlg->MoveItemToGuildPyoguk(pItem);
					return;
				}
			}
	
		}
		else
		{//[否则如果背包右键菜单为激活状态则关闭退出][BY:TT天语][QQ:2013734367][2019-4-16][16:59:04]
			if(GAMEIN->GetInventoryRBTDlg()->IsActive())
			{
				GAMEIN->GetInventoryRBTDlg()->SetActive(FALSE);
				return;
			}
		}
		CMouse* mouse = g_UserInput.GetMouse();
		GAMEIN->GetInventoryRBTDlg()->SetAbsXY(mouse->GetMouseX(),mouse->GetMouseY());
		GAMEIN->GetInventoryRBTDlg()->SetActive(TRUE);
	}
}

CItem * CInventoryExDialog::GetCurSelectedItem(BYTE type)
{
	switch(type)
	{
	case INV:
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
		}
		break;
	case WEAR:
		{
			if(m_pWearedDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
		}
		break;
	case SHOPIVN:
		{
			return m_pItemShopInven->GetItemForPos((POSTYPE)(m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START));
		}
		break;
	case SHOPIVNPLUS:
		{
			return m_pItemShopInvenPlus->GetItemForPos((POSTYPE)(m_pItemShopInvenPlus->GetCurSelCellPos()+TP_SHOPINVEN_START+20));
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	return NULL;
}

BOOL CInventoryExDialog::GetBlankPositionRestrictRef(WORD & absPos)
{
	for(int i = 0 ; i < GetTabNum() ; ++i)
	{
		cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(i);
		for(int j = 0 ; j < gridDlg->GetCellNum() ; ++j)
		{
			if(gridDlg->IsAddable(j))
			{
				absPos = TP_INVENTORY_START+i*TABCELL_INVENTORY_NUM+j;
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

int CInventoryExDialog::GetBlankNum()
{
	int nBlankNum = 0;
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if( gridDlg->IsAddable(j) )
				++nBlankNum;
		}
	}

	return nBlankNum;
}

BOOL CInventoryExDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )	return FALSE;

	if( PKMGR->IsPKLooted() )	return FALSE;

	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM ) return FALSE;
	
	if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}
	else if(icon->GetType() == WT_DEALITEM ||icon->GetType()==WT_SHOPITEM )
	{
		FakeBuyItem(x, y, (CDealItem *)icon);
		AUDIOMGR->Play(58, HERO);
	}
	return FALSE;
}

void CInventoryExDialog::FakeBuyItem( LONG mouseX, LONG mouseY, CDealItem * pFromItem )
{
	if( pFromItem )
	{
		GAMEIN->GetDealDialog()->FakeBuyItem( (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), pFromItem );
	}
}

void CInventoryExDialog::MoveItem(POSTYPE FromPos, POSTYPE ToPos )
{
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	DeleteItem(FromPos, &FromItem);
	DeleteItem(ToPos, &ToItem);

	if(FromItem)
	{
		FromItem->SetPosition(ToPos);
		if(!AddItem(FromItem))
			assert(0);
	}

	if(ToItem)
	{
		ToItem->SetPosition(FromPos);
		if(!AddItem(ToItem))
			assert(0);
	}
}

BOOL CInventoryExDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	
		return FALSE;

	CItem * pToItem = GetItemForPos( ToPos );
	
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

	if( pToItem )
	if( pToItem->IsLocked() ) return FALSE;
	
	if( pFromItem->IsLocked() ) return FALSE;
	
	if(TP_PYOGUK_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_PYOGUK_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
	{
		if(m_pItemShopInven->InPt(pFromItem->GetPosition()))
			return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		else if(m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
			m_pItemShopInvenPlus->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}
	else if(m_pItemShopInven->InPt(pFromItem->GetPosition())/*TP_SHOPINVEN_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPINVEN_END*/)
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
			{	
				m_pItemShopInven->SetCurSelCellPos(-1);
				return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
			}
		}
	}
	else if(m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
			{
				m_pItemShopInvenPlus->SetCurSelCellPos(-1);
				return m_pItemShopInvenPlus->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
			}
		}
	}
	else if( (TP_SIEGEFORTWARE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SIEGEFORTWARE_END) )
	{
        return GAMEIN->GetFWWareHouseDlg()->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}


	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( IsDivideFunctionKey() && !( pToItem ) )
		{
			return FakeItemDivide( ToPos, pFromItem, pToItem );
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			return FakeItemCombine( ToPos, pFromItem, pToItem );
		}
	}
	return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

BOOL CInventoryExDialog::IsDivideFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_MENU);
}

BOOL CInventoryExDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}
	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
		return FALSE;
	}

	if(pToItem)
	{
		if(FromIdx == eItemTable_Pyoguk && pToItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
			return FALSE;
		}
	}


	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}

BOOL CInventoryExDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return FALSE;
	if( pFromItem->GetPosition() == ToPos )
		return FALSE;
	if( pFromItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;
	
	if(pFromItem->GetDurability() == 1)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	m_divideMsg.Category			= MP_ITEM;
	m_divideMsg.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID			= HEROID;

	m_divideMsg.wItemIdx = pFromItem->GetItemIdx();
	m_divideMsg.FromPos = pFromItem->GetPosition();
	m_divideMsg.FromDur = pFromItem->GetDurability();
	m_divideMsg.ToPos = ToPos;
	m_divideMsg.ToDur = 0;
	m_divideMsg.dwItemDBIdx = pFromItem->GetDBIdx();

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_ShopInven);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
	pDivideBox->SetValue( 1 );	
	
	return FALSE;
		
}

void CInventoryExDialog::OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
}

void CInventoryExDialog::OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CInventoryExDialog * tDlg = ( CInventoryExDialog * )vData1;
	if( param1 == 0 )
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return;
	}

	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.FromPos);
	CItem * pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, tDlg->m_divideMsg.FromPos);
	if(!pTargetItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(pTargetItem->IsLocked())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if(tDlg->m_divideMsg.wItemIdx != pTargetItem->GetItemIdx())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(tDlg->m_divideMsg.FromDur != pTargetItem->GetDurability())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	CItem * pToItem = ITEMMGR->GetItemofTable(ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.ToPos), tDlg->m_divideMsg.ToPos);
	if(pToItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if( tDlg->m_divideMsg.FromDur > param1 )
	{
		tDlg->m_divideMsg.ToDur			= param1;
		tDlg->m_divideMsg.FromDur		= tDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send( &tDlg->m_divideMsg, sizeof(tDlg->m_divideMsg) );
	}
	else
	{
		tDlg->FakeGeneralItemMove(tDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	}
}

BOOL CInventoryExDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return FALSE;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return FALSE;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return FALSE;

	if( pFromItem->IsLocked() || pToItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;
	if (pFromItem->GetDurability() >= MAX_YOUNGYAKITEM_DUPNUM || pToItem->GetDurability() >= MAX_YOUNGYAKITEM_DUPNUM)
	{
		MSG_ITEM_MOVE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_MOVE_SYN;
		msg.dwObjectID = HEROID;
		msg.FromPos = pFromItem->GetPosition();
		msg.wFromItemIdx = pFromItem->GetItemIdx();
		msg.ToPos = pToItem->GetPosition();
		msg.wToItemIdx = pToItem->GetItemIdx();
		NETWORK->Send(&msg, sizeof(msg));
	}
	else
	{
		MSG_ITEM_COMBINE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_COMBINE_SYN;
		msg.dwObjectID = HEROID;

		msg.FromPos = pFromItem->GetPosition();
		msg.ToPos = pToItem->GetPosition();
		msg.wItemIdx = pFromItem->GetItemIdx();
		msg.FromDur = pFromItem->GetDurability();
		msg.ToDur = pToItem->GetDurability();
		NETWORK->Send(&msg, sizeof(msg));
	}


	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);

	return TRUE;
}	

void CInventoryExDialog::Linking()
{
	if(!m_fSpread)
	{
		m_pWearedDlg = (CWearedExDialog *)GetWindowForID(IN_WEAREDDLG);
		m_pItemShopInven = (CItemShopInven*)GetWindowForID(IN_SHOPITEMGRID);
		m_pItemShopInvenPlus = (cShopItemInventPlusDlg*)GetWindowForID(IN_SHOPITEMGRID_PLUS);
		m_pItemShopBtn = (cPushupButton*)GetWindowForID(IN_SHOPITEMBTN);
		m_pItemShopBtn->SetActive(FALSE);
		cImage Image;
		SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
		m_pItemShopBtn->SetToolTip( CHATMGR->GetChatMsg(731), RGB_HALF(255,255,255), &Image, TTCLR_DEFAULT );
		m_pItemShopInven->SetActive(FALSE);
		m_fSpread = TRUE;
	}
	p_Money=(cStatic*)GetWindowForID(IN_MONEYPACKET);
	p_Money->SetAlign(TXT_CENTER);
	m_dWidth = GetWidth();
}
CItem * CInventoryExDialog::GetItemLike(WORD wItemIdx,WORD &OutPos)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<(SLOT_SHOPINVEN_NUM/*/2*/+TABCELL_SHOPINVEN_PLUS_NUM)/*m_pItemShopInven->GetCellNum()*/; i++)
		{
			if(m_pItemShopInven->InPt(i+TP_SHOPINVEN_START))
			{
				if( !m_pItemShopInven->IsAddable(i) )
				{
					CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
					if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						OutPos=TP_SHOPINVEN_START+i;
						return pItem;
					}

				}
			}
			else if(m_pItemShopInvenPlus->InPt(i+TP_SHOPINVEN_START))
			{
				if( !m_pItemShopInvenPlus->IsAddable(i-20) )
				{
					CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(i-20);
					if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						OutPos=TP_SHOPINVEN_START+i;
						return pItem;
					}

				}
			}
		}
	}

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
				if(pItem->GetItemIdx() == wItemIdx)
				{
					OutPos=TP_INVENTORY_START+i;
					return pItem;
				}
			}
		}
	}
	OutPos=0;
	return NULL;
}
void CInventoryExDialog::GetItemForCallPet(DWORD dwDBIdx)
{
	for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
	{
		if( !m_pItemShopInven->IsAddable(i) )
		{
			CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
			if(pItem->GetDBIdx() == dwDBIdx)
				UseItem(pItem);
		}
	}
}
CItem * CInventoryExDialog::GetItemLike(WORD wItemIdx)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<(SLOT_SHOPINVEN_NUM/*/2*/+TABCELL_SHOPINVEN_PLUS_NUM)/*m_pItemShopInven->GetCellNum()*/; i++)
		{
			if(m_pItemShopInven->InPt(i+TP_SHOPINVEN_START))
			{
				if( !m_pItemShopInven->IsAddable(i) )
				{
					CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
					if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						return pItem;
					}

				}
			}
			else if(m_pItemShopInvenPlus->InPt(i+TP_SHOPINVEN_START))
			{
				if( !m_pItemShopInvenPlus->IsAddable(i-20) )
				{
					CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(i-20);
					if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					{
						return pItem;
					}

				}
			}
		}
	}

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
				if(pItem->GetItemIdx() == wItemIdx)
					return pItem;
			}
		}
	}
	return NULL;
}
DURTYPE	CInventoryExDialog::GetTotalItemDurability(WORD wItemIdx)
{
	DURTYPE DurSum = 0;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if(pItemInfo)
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if( !m_pItemShopInven->IsAddable(i) )
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}
		for(int i = 0;i<m_pItemShopInvenPlus->GetCellNum();i++)
		{// [修复物品放到快捷栏以后数量显示不正确] [by:TT天语][QQ:2013734367][2018/12/4]
			if(!m_pItemShopInvenPlus->IsAddable(i))
			{
				CItem* pItem = (CItem*)m_pItemShopInvenPlus->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}
		return DurSum;
	}

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);

				if(pItem->GetItemIdx() == wItemIdx)
					DurSum += pItem->GetDurability();
			}
		}
	}

	return DurSum;
}

BOOL CInventoryExDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;									

	if(param != 2)

	{
		if(m_bShopInven)
		{
			if( m_pItemShopInven->GetPositionForXYRef(x, y, position) )
			{
				pos = position+TP_SHOPINVEN_START;
				return TRUE;
			}
			else if(m_pItemShopInvenPlus->GetPositionForXYRef(x,y,position))
			{//[ShopInventoryPlus][By:TT天语][QQ:2013734367][2018/10/9]
				pos = position+TP_SHOPINVEN_START+20;
				return TRUE;
			}
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetPositionForXYRef(x, y, position))
			{
				pos = position+TP_INVENTORY_START+TABCELL_INVENTORY_NUM*GetCurTabNum();		
				return TRUE;
			}
		}
	}

	if(param != 1)
	if(m_pWearedDlg->GetPositionForXYRef(x, y, position))
	{
		pos = position+TP_WEAR_START;			
		return TRUE;
	}
	
	return FALSE;
}

BOOL CInventoryExDialog::CheckItemLockForItemIdx(WORD wItemIdx)
{
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )

	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
				
				if( pItem->GetItemIdx() == wItemIdx && pItem->IsLocked() )
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CInventoryExDialog::IsExist(POSTYPE abs_pos)
{
	BOOL bExist = FALSE;
	if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	{
		int tabIndex = GetTabIndex( abs_pos );		
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
		if(dlg->GetIconForIdx(abs_pos-TABCELL_INVENTORY_NUM*tabIndex-TP_INVENTORY_START))
			bExist = TRUE;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		if( m_pWearedDlg->GetIconForIdx(abs_pos - TP_WEAR_START) )
			bExist = TRUE;
	}

	return bExist;
}

BOOL CInventoryExDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	if(pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;
		if( (TP_WEAR_START <= fromPos && fromPos < TP_WEAR_END) ||
			(TP_WEAR_START <= toPos && toPos < TP_WEAR_END) )
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return FALSE;
			}
		}
	}
	if(TP_INVENTORY_START<= pos && pos < TP_INVENTORY_END)
	{
 		POSTYPE frompos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END )
			return FALSE;
		else if( TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END )
			return FALSE;
		else if( TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END )
			return FALSE;
		else if( TP_TITANWEAR_START <= frompos && frompos < TP_TITANWEAR_END )
			return TRUE;
		else
			return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		if( !( pItem->GetItemKind() & eEQUIP_ITEM ) )
		{
			if( pItem->GetItemKind() == eQUEST_ITEM_EQUIP )
			{
			}
			else if( pItem->GetItemKind() == eSHOP_ITEM_EQUIP )
			{
				if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
					return FALSE;
			}
			else
				return FALSE;
		}
		else
		{
			CItem* ptarget = (CItem*)m_pWearedDlg->GetIconForIdx( pos - TP_WEAR_START ); 
			if( ptarget )
			{
				POSTYPE frompos = pItem->GetPosition();
				if( (TP_PYOGUK_START <= frompos && frompos < TP_PYOGUK_END) &&
					ptarget->GetItemKind() & eSHOP_ITEM_EQUIP )
					return FALSE;
			}
		}

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo) return FALSE;
		if(pInfo->EquipKind == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		
		return ITEMMGR->CanEquip(pItem->GetItemIdx());
	}
	else if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <=pos && pos <TP_SHOPINVEN_END)
	{
		POSTYPE frompos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END )
			return TRUE;
		else if( TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END )
			return TRUE;
		else if(TP_WEAR_START <= frompos && frompos < TP_WEAR_END )
		{
			return TRUE;
		}
		else if( TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END )
			return FALSE;
		else if( TP_TITANSHOPITEM_START <= frompos && frompos < TP_TITANSHOPITEM_END )
			return TRUE;
		else
			return FALSE;
	}
	else if(TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{
		POSTYPE fromPos = pItem->GetPosition();

		if( TP_SHOPINVEN_START <= fromPos && fromPos < TP_SHOPINVEN_END )
		{
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();
			if( FALSE == pPetEquipDlg->CheckDuplication( pItem->GetItemIdx() ) )
			{
				return TRUE;
			}
			return FALSE;
		}
		else
			return FALSE;
	}
	else if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		if( ( pItem->GetItemKind() & eTITAN_EQUIPITEM ) )
		{
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			if(pos-TP_TITANWEAR_START != pInfo->ItemKind-eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1505) );
				return FALSE;
			}

			if(pInfo->LimitLevel > HERO->GetLevel())
			{
				char temp[256] = {0, };
				sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->LimitLevel );
				CHATMGR->AddMsg( CTC_SYSMSG, temp );
				return FALSE;
			}

			return TRUE;
		}
	}
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		if( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP )
		{
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

void CInventoryExDialog::ItemUseForQuestStart()
{
	if( m_pTempQuestItem )
	{
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_FOR_QUESTSTART_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_pTempQuestItem->GetPosition();
		msg.wItemIdx = m_pTempQuestItem->GetItemIdx();

		NETWORK->Send(&msg,sizeof(msg));
	}
}


void CInventoryExDialog::ItemUnSealingSync(DWORD absPos)
{
	CItem* pItem = GetItemForPos(absPos);
	if( !pItem || !(pItem->GetItemParam()&ITEM_PARAM_SEAL) ) return;	

	if( pItem->IsLocked() ) return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
	if(!pItemInfo)	return;

	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || 
		pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
		pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME ||
		pItemInfo->ItemKind == eSHOP_ITEM_PET)
	{
		if( ITEMMGR->GetUsedItemInfo( pItemInfo->ItemIdx ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
			return;
		}
	}


	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData = absPos;

	NETWORK->Send(&msg, sizeof(msg));
}
void CInventoryExDialog::ItemUnSealingSync()
{
	BOOL bIsPlus = FALSE;
	CItem* pItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)(m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START));
	if(!pItem)
	{
		bIsPlus = TRUE;
		pItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)(m_pItemShopInvenPlus->GetCurSelCellPos()+TP_SHOPINVEN_START+20));
	}
	if( !pItem || !(pItem->GetItemParam()&ITEM_PARAM_SEAL) )				return;	
	
	if( pItem->IsLocked() ) return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
	if(!pItemInfo)	return;

	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || 
		pItemInfo->ItemKind == eSHOP_ITEM_DECORATION||
		pItemInfo->ItemKind == eSHOP_ITEM_IMAGENAME||
		pItemInfo->ItemKind == eSHOP_ITEM_PET)	//[图片称号物品解锁设置][By:TT天语][QQ:2013734367][2017/11/29]
	{
		if( ITEMMGR->GetUsedItemInfo( pItemInfo->ItemIdx ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
			return;
		}
	}

	
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	if(bIsPlus)
		msg.dwData = m_pItemShopInvenPlus->GetCurSelCellPos()+TP_SHOPINVEN_START+20;
	else
		msg.dwData = m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START;

	NETWORK->Send(&msg, sizeof(msg));
}


void CInventoryExDialog::ItemUnSealing(DWORD absPos)
{
	CItem* pItem = ITEMMGR->GetItemofTable((WORD)eItemTable_ShopInven, (POSTYPE)absPos);

	if( !pItem )		return;

	ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
	Param ^= ITEM_PARAM_SEAL;
	pItem->SetItemParam( Param );

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
	if( pItemInfo && 
		pItemInfo->ItemKind != eSHOP_ITEM_MAKEUP && 
		pItemInfo->ItemKind != eSHOP_ITEM_DECORATION &&
		pItemInfo->ItemKind != eSHOP_ITEM_EQUIP )
		ITEMMGR->SetToolTipIcon( pItem );

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(776) );
}


void CInventoryExDialog::SetShopItemInvenBtn( BOOL val )
{
	m_pItemShopBtn->SetActive( val );
}

CItem* CInventoryExDialog::GetPriorityItemForCristmasEvent()
{
	CItem* pItem = NULL;

	pItem = GetItemLike( EVENT_ITEM_HARDSNOW );
	if( pItem )
		return pItem;
	else
	{
		pItem = GetItemLike( EVENT_ITEM_SNOW );
	}

	return pItem;
}

DWORD CInventoryExDialog::GetPriorityItemIdx()
{
	CItem* pItem = NULL;
	DWORD Idx = 0;
	pItem = GetPriorityItemForCristmasEvent();

	if( pItem )
	{
		Idx = pItem->GetItemIdx();
	}

	return Idx;
}

//[背包整理函数重载][By:TT天语][QQ:2013734367][2018/2/2]
BOOL CInventoryExDialog::FakeMoveItem(CItem * pFromItem,WORD ToPos)
{

	CItem * pToItem = GetItemForPos( ToPos );
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

	if( pToItem )
		if( pToItem->IsLocked() ) return FALSE;

	if( pFromItem->IsLocked() ) return FALSE;
	if(TP_PYOGUK_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_PYOGUK_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
	{
		if(m_pItemShopInven->InPt(pFromItem->GetPosition()))
			return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		else if(m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
			return m_pItemShopInvenPlus->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}
	else if(m_pItemShopInven->InPt(pFromItem->GetPosition())/*TP_SHOPINVEN_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPINVEN_END*/)
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
				return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		}
	}
	else if(m_pItemShopInvenPlus->InPt(pFromItem->GetPosition()))
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
				return m_pItemShopInvenPlus->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		}
	}
	else if( (TP_SIEGEFORTWARE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SIEGEFORTWARE_END) )
	{
		return GAMEIN->GetFWWareHouseDlg()->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}


	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( IsDivideFunctionKey() && !( pToItem ) )
		{
			return FakeItemDivide( ToPos, pFromItem, pToItem );
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			return FakeItemCombine( ToPos, pFromItem, pToItem );
		}
	}
	return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}
BOOL CInventoryExDialog::IsCanMovePack(WORD & Time)
{//[客户端计时必须比服务端大否则会出现细微差别造成客户端可以整理服务端拒绝][By:TT天语][QQ:2013734367][2019/3/18][1:33:25]
	if(gCurTime-m_CanMoveTimeCount>=10000)
	{
		Time = 0;
		return TRUE;
	}
	else
	{
		Time = (gCurTime-m_CanMoveTimeCount)/1000;
		return FALSE;
	}
}

void CInventoryExDialog::SetMoney(MONEYTYPE Val)
{
	p_Money->SetStaticText(AddComma(Val));

	p_Money->SetAlign(TXT_RIGHT);
}
void CInventoryExDialog::GetEmptyNum(BYTE type,std::vector<BYTE>& v)
{
	switch(type)
	{
	case INV:
		{//[普通背包栏空闲格子获取][BY:TT天语][QQ:2013734367][2019-4-8][9:18:30]
		for (int i = 79; i >= 0; i--)
			{
				CItem * pItem=NULL;
				pItem = GetItemForPos(i);
				if (pItem == NULL)
					v.push_back(i);
			}
		}
		break;
	case SHOPIVN:
	case SHOPIVNPLUS:
		{//[百宝背包栏空闲格子获取][BY:TT天语][QQ:2013734367][2019-4-8][9:18:45]
			for(int i = 449,j=0;i>=390;j++,i--)
			{
				CItem * pItem=NULL;
				pItem = GetItemForPos(i);
				if (pItem == NULL)
					v.push_back(i);
			}	
		}
		break;
	}
}
void CInventoryExDialog::GetItemFromPyoguk(CItem* pItem)
{//[右键仓储][BY:TT天语][QQ:2013734367][2019-4-16][20:50:32]
	CItem * pPosItem = NULL;
	for (int i = 0;i<80;i++)
	{
		if(!GetItemForPos(i))
		{
			FakeGeneralItemMove(i,pItem,NULL);
			return;
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2347));
}
void CInventoryExDialog::GetItemFromItemShop(CItem* pItem)
{//[右键仓储][BY:TT天语][QQ:2013734367][2019-4-16][20:50:32]
	CItem * pPosItem = NULL;
	for (int i = TP_SHOPINVEN_START;i<TP_SHOPINVEN_END;i++)
	{
		if(!GetItemForPos(i))
		{
			FakeGeneralItemMove(i,pItem,NULL);
			return;
		}
	}
	CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2347));
}