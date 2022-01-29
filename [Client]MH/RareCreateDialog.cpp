








#include "stdafx.h"
#include ".\rarecreatedialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cIcon.h"
#include "./Interface/cIconDialog.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "MHFile.h"
#include "GameIn.h"



CRareCreateDialog::CRareCreateDialog(void)
{
	m_type = WT_ITEM_RARECREATE_DLG;

	cImage imgToolTip;


	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	m_VirtualItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualItem.SetMovable(FALSE);

	m_ItemIdx = 0;
	m_ItemPos = 0;
	//Rare_Info->SetStaticText(" ");

	LoadRARE_Max_INFO();
}

CRareCreateDialog::~CRareCreateDialog(void)
{
	Rare_Info->SetStaticText(" ");
	RareMax_Info->SetStaticText(" ");
}

void CRareCreateDialog::Linking()
{
	m_pIconDlg = (cIconDialog*)GetWindowForID(RareCreate_ICON);

	Rare_Info = (cStatic *)GetWindowForID(RareCreate_Info);
	Rare_Info->SetMultiLine(TRUE);
	Rare_Info->InitMultiLine();

	RareMax_Info = (cStatic *)GetWindowForID(RareCreate_MaxInfo);
	RareMax_Info->SetMultiLine(TRUE);
	RareMax_Info->InitMultiLine();
}
BOOL CRareCreateDialog::FakeMoveIcon(LONG x, LONG y, cIcon * pOrigIcon)
{
	Rare_Info->SetStaticText(" ");
	if (pOrigIcon->GetType() != WT_ITEM) return FALSE;
	if (m_bDisable) return FALSE;

	CItem * pOrigItem = (CItem *)pOrigIcon;

	if (pOrigItem->IsLocked()) return FALSE;

	ITEM_INFO* pBaseItemInfo = ITEMMGR->GetItemInfo(pOrigItem->GetItemIdx());
	if (!pBaseItemInfo)
		return FALSE;


	eITEM_KINDBIT bits = pOrigItem->GetItemKind();
	if (!(bits & eEQUIP_ITEM))
		return FALSE;

	if (pBaseItemInfo->WeaponType > WP_KEY)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1455));
		return FALSE;
	}

	int bit1 = bits;

	DWORD Itemidx = pOrigItem->GetItemIdx();


	//[  取消祝福整10處理  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	if (bits < eEQUIP_ITEM_WEAPON /*&& pOrigItem->GetItemIdx() % 10*/)
	{

		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));
		return FALSE;
	}

	//[  是否強化  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	ITEM_OPTION_INFO* pOptionInfo = ITEMMGR->GetItemOption(pOrigItem->GetDurability());
	if (pOrigItem->GetDurability() != 0 || pOptionInfo->dwOptionIdx != 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));

		return FALSE;
	}

	//[  是否可以祝福  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	if (!ITEMMGR->IsRareItemAble(pOrigItem->GetItemIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1234));
		return FALSE;
	}

	//獲取祝福值
	ITEM_RARE_OPTION_INFO* RareInfo = ITEMMGR->GetItemRareOption(pOrigItem->GetRareness());
	if (RareInfo)
	{
		SetRareInfo(RareInfo);
	}



	if (!ITEMMGR->IsEqualTableIdxForPos(eItemTable_Inventory, pOrigItem->GetPosition()))
		return FALSE;

	ITEM_INFO* pShopItem = ITEMMGR->GetItemInfo((WORD)m_ItemIdx);
	if (!pShopItem)
		return FALSE;

	//[  祝福石是否與武器等級匹配  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	if (pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1082));
		return FALSE;
	}

	//設置最大值
	if (bits >= eEQUIP_ITEM_WEAPON&&bits <= eEQUIP_ITEM_BELT)
	{
		int Index = 0;
		switch (bits)
		{
		case eEQUIP_ITEM_WEAPON:
			Index = 0;
			break;
		case eEQUIP_ITEM_DRESS:
		case eEQUIP_ITEM_HAT:
		case eEQUIP_ITEM_SHOES:
			Index = 1;
			break;
		case eEQUIP_ITEM_RING:
		case eEQUIP_ITEM_CAPE:
		case eEQUIP_ITEM_NECKLACE:
		case eEQUIP_ITEM_ARMLET:
		case eEQUIP_ITEM_BELT:
			Index = 2;
			break;
		}
		SetRARE_Max_INFO(Index);
	}

	if (m_VirtualItem.GetSrcItemKind() != eKIND_ITEM_MAX)
	{
		CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
		if (pItem)
		{
			if (m_pIconDlg)
			{
				CVirtualItem* pVItem = NULL;
				m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);
				if (pVItem)
					pVItem->GetLinkItem()->SetLock(FALSE);
			}
		}
	}



	//[  鎖定物品  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	pOrigItem->SetLock(TRUE);


	//[  添加圖標  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
	AddVirtualItem(pOrigItem);

	/*ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);*/


	return FALSE;
}

void CRareCreateDialog::Clear_RARE_INFO()
{
	Rare_Info->SetStaticText(" ");
	RareMax_Info->SetStaticText(" ");
}

void CRareCreateDialog::ReleaseItem()
{
	if( m_pIconDlg )

	{
		CVirtualItem* pVItem = NULL;

		m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
		if( pVItem )			
			pVItem->GetLinkItem()->SetLock( FALSE );

		m_VirtualItem.SetLinkItem( NULL );
	}

	CItem* pOriItem = ITEMMGR->GetItemofTable( eItemTable_ShopInven, (POSTYPE)m_ItemPos );
	if( pOriItem )
		pOriItem->SetLock( FALSE );

	m_ItemIdx = 0;
	m_ItemPos = 0;


	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	Rare_Info->SetStaticText(" ");
	RareMax_Info->SetStaticText(" ");

	SetActive( FALSE );
}

void CRareCreateDialog::AddVirtualItem( CItem* pItem )
{
	m_VirtualItem.SetData( pItem->GetItemIdx() );
	m_VirtualItem.SetLinkItem( pItem );

	SHOPITEMBASE* pShopItemInfo = NULL;
	pShopItemInfo = ITEMMGR->GetUsedItemInfo( pItem->GetItemBaseInfo()->wIconIdx );
	if( pShopItemInfo )
		ITEMMGR->AddUsedAvatarItemToolTip( pShopItemInfo );
	m_pIconDlg->AddIcon( 0, (cIcon*)&m_VirtualItem );
}

void CRareCreateDialog::ItemRareCreateSyn()
{
	if( !m_VirtualItem.GetLinkItem() )		return;

	CItem* pItem = ITEMMGR->GetItem( m_VirtualItem.GetLinkItem()->GetDBIdx() );
	if( !pItem )
	{
		if( m_pIconDlg )
		{
			CVirtualItem* pVItem = NULL;
			m_pIconDlg->DeleteIcon( 0, (cIcon**)&pVItem );
			if( pVItem )
				pVItem->GetLinkItem()->SetLock( FALSE );
		}

		m_ItemIdx = 0;
		m_ItemPos = 0;


		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);


		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		return;
	}

	MSG_DWORD4	msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_SYN );
	msg.dwObjectID	= HEROID;
	msg.dwData1 = m_ItemIdx;

	msg.dwData2 = m_ItemPos;
	msg.dwData3 = pItem->GetItemBaseInfo()->wIconIdx;
	msg.dwData4 = pItem->GetItemBaseInfo()->Position;
	NETWORK->Send( &msg, sizeof(msg) );

	//SetActive( FALSE );
}

//[  祝福成功回調  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
void CRareCreateDialog::ItemRareCreateAck()
{
	CVirtualItem* pVItem = NULL;
	//重置祝福窗口數字
	/*m_ItemIdx = 0;
	m_ItemPos = 0;*/

	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		//[  獲取當前祝福值  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
		ITEM_RARE_OPTION_INFO* RARE_OPTION_ITEM = ITEMMGR->GetItemRareOption(pItem->GetRareness());
		if (RARE_OPTION_ITEM)
		{
			//[  更新當前祝福值  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
			ITEMMGR->SetToolTipIcon((cIcon*)pItem, NULL, RARE_OPTION_ITEM, 0);

			SetRareInfo(RARE_OPTION_ITEM);
		}

		if (!GAMEIN->GetRareCreateDlg()->IsActive())
		{
			pItem->SetLock(FALSE);

			if (m_pIconDlg)

				m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);

			m_VirtualItem.SetLinkItem(NULL);
		}

	}
	if (!GAMEIN->GetRareCreateDlg()->IsActive())
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}

BOOL CRareCreateDialog::Ishint()
{
	if (!m_VirtualItem.GetLinkItem())		return FALSE;
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		//[  獲取當前祝福值  ] [By:周Vs周] [QQ:137037798] [2021-05-26]	
		ITEM_RARE_OPTION_INFO* RareInfo = ITEMMGR->GetItemRareOption(pItem->GetRareness());
		if (RareInfo)
		{

			int Index = 0;
			switch (pItem->GetItemKind())
			{
			case eEQUIP_ITEM_WEAPON:
				Index = 0;
				break;
			case eEQUIP_ITEM_DRESS:
			case eEQUIP_ITEM_HAT:
			case eEQUIP_ITEM_SHOES:
				Index = 1;
				break;
			case eEQUIP_ITEM_RING:
			case eEQUIP_ITEM_CAPE:
			case eEQUIP_ITEM_NECKLACE:
			case eEQUIP_ITEM_ARMLET:
			case eEQUIP_ITEM_BELT:
				Index = 2;
				break;
			}
			ITEM_RARE_Max_INFO oITEM_RARE_Max_INFO = GetRARE_Max_INFO(Index);
			if (oITEM_RARE_Max_INFO.ThreeMax>0)
			{
				if (RareInfo->GenGol >= oITEM_RARE_Max_INFO.ThreeMax ||
					RareInfo->MinChub >= oITEM_RARE_Max_INFO.ThreeMax ||
					RareInfo->SimMek >= oITEM_RARE_Max_INFO.ThreeMax)
				{
					return TRUE;
				}
			}
			if (oITEM_RARE_Max_INFO.CheRyukMax>0 && RareInfo->CheRyuk >= oITEM_RARE_Max_INFO.CheRyukMax)
			{
				return TRUE;
			}
			if (oITEM_RARE_Max_INFO.LifeMax>0)
			{
				if (RareInfo->Life >= oITEM_RARE_Max_INFO.LifeMax ||
					RareInfo->NaeRyuk >= oITEM_RARE_Max_INFO.LifeMax ||
					RareInfo->Shield >= oITEM_RARE_Max_INFO.LifeMax)
				{
					return TRUE;
				}
			}

			if (oITEM_RARE_Max_INFO.PhyAttackMax>0 && RareInfo->PhyAttack >= oITEM_RARE_Max_INFO.PhyAttackMax)
			{
				return TRUE;
			}

			if (oITEM_RARE_Max_INFO.PhyDefenseMax> 0 && RareInfo->PhyDefense >= oITEM_RARE_Max_INFO.PhyDefenseMax)
			{
				return TRUE;
			}

			if (oITEM_RARE_Max_INFO.AttrAttackMax>0)
			{
				for (size_t i = ATTR_FIRE; i <= ATTR_MAX; i++)
				{
					if ((RareInfo->AttrAttack.GetElement_Val(i) + 0.0005) * 100 >= oITEM_RARE_Max_INFO.AttrAttackMax)
					{
						return TRUE;
					}
				}
			}
			if (oITEM_RARE_Max_INFO.AttrRegistMax>0)
			{
				for (size_t i = ATTR_FIRE; i <= ATTR_MAX; i++)
				{
					if ((RareInfo->AttrRegist.GetElement_Val(i) + 0.0005) * 100 >= oITEM_RARE_Max_INFO.AttrRegistMax)
					{
						return TRUE;
					}
				}
			}

		}
	}
	return FALSE;
}
//祝福創建--取消
void CRareCreateDialog::ItemRareCreateNAck()
{
	CVirtualItem* pVItem = NULL;

	/*m_ItemIdx = 0;
	m_ItemPos = 0;*/
	CItem* pItem = ITEMMGR->GetItem(m_VirtualItem.GetLinkItem()->GetDBIdx());
	if (pItem)
	{
		pItem->SetLock(FALSE);

		if (m_pIconDlg)
			m_pIconDlg->DeleteIcon(0, (cIcon**)&pVItem);

		m_VirtualItem.SetLinkItem(NULL);
	}

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
}

void CRareCreateDialog::SetRareInfo(ITEM_RARE_OPTION_INFO* RareInfo)
{
	if (RareInfo)
	{
		char line[128] = { 0, };

		char szDescText[384] = { 0, };

		if (RareInfo->GenGol != 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(261), RareInfo->GenGol);
			strcat(szDescText, line);
		}
		if (RareInfo->MinChub != 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(262), RareInfo->MinChub);
			strcat(szDescText, line);
		}
		if (RareInfo->CheRyuk != 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(263), RareInfo->CheRyuk);
			strcat(szDescText, line);
		}
		if (RareInfo->SimMek != 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(264), RareInfo->SimMek);
			strcat(szDescText, line);
		}

		if (RareInfo->Life > 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(386), RareInfo->Life);
			strcat(szDescText, line);
		}
		if (RareInfo->NaeRyuk > 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(387), RareInfo->NaeRyuk);
			strcat(szDescText, line);
		}
		if (RareInfo->Shield > 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(388), RareInfo->Shield);
			strcat(szDescText, line);
		}

		if (RareInfo->PhyAttack > 0)
		{
			wsprintf(line, "%s  	[+%d] ^n", CHATMGR->GetChatMsg(681), RareInfo->PhyAttack);
			strcat(szDescText, line);
		}

		if (RareInfo->PhyDefense != 0)
		{
			wsprintf(line, "%s	[+%d] ^n", CHATMGR->GetChatMsg(397), RareInfo->PhyDefense);
			strcat(szDescText, line);
		}

		for (size_t i = ATTR_FIRE; i <= ATTR_MAX; i++)
		{
			if ((RareInfo->AttrRegist.GetElement_Val(i)) != 0)
			{
				wsprintf(line, "%s  	[+%d%%] ^n", CHATMGR->GetChatMsg(264 + i), (int)(RareInfo->AttrRegist.GetElement_Val(i) * 100 + 0.005));
				strcat(szDescText, line);
			}

			if (RareInfo->AttrAttack.GetElement_Val(i) != 0)
			{
				wsprintf(line, "%s	[+%d%%] ^n", CHATMGR->GetChatMsg(391 + i), (int)(RareInfo->AttrAttack.GetElement_Val(i) * 100 + 0.005));
				strcat(szDescText, line);
			}
		}
		Rare_Info->SetStaticText(szDescText);
	}
	else
		Rare_Info->SetStaticText("");
}



void CRareCreateDialog::LoadRARE_Max_INFO()
{
	CMHFile fp;

	char szBuf[256] = { 0, };
	char line[512];

	char FileName[256];

	sprintf(FileName, "Resource/Item_RareOptionMaxInfo.bin");
	if (!fp.Init(FileName, "rb"))
		return;

	DWORD dwItemKindIdx = 0;

	while (!fp.IsEOF())
	{
		if (dwItemKindIdx == 3)
			break;

		fp.GetString(szBuf);

		if (szBuf[0] == '@')
		{
			fp.GetLineX(line, 512);
			continue;
		}
		SafeStrCpy(RARE_Max_INFO[dwItemKindIdx].TypeName, szBuf, MAX_ITEMNAME_LENGTH + 1);
		RARE_Max_INFO[dwItemKindIdx].ThreeMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].CheRyukMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].LifeMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].PhyAttackMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].PhyDefenseMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].AttrRegistMax = fp.GetWord();
		RARE_Max_INFO[dwItemKindIdx].AttrAttackMax = fp.GetWord();
		/*
		char TypeName[MAX_ITEMNAME_LENGTH + 1];
		WORD	ThreeMax;//三維
		WORD	CheRyukMax;//體質
		DWORD	LifeMax;//生命
		WORD	PhyAttackMax;//攻擊
		WORD	PhyDefenseMax;//防御
		WORD AttrRegistMax;//屬性防御
		WORD AttrRegistMax;//屬性防御
		WORD AttrAttackMax;//屬性攻擊

		*/
		dwItemKindIdx++;
	}
}

ITEM_RARE_Max_INFO CRareCreateDialog::GetRARE_Max_INFO(int Index)
{
	if (Index >= 0 && Index < 3)
	{
		return RARE_Max_INFO[Index];
	}
	else
	{
		return RARE_Max_INFO[0];
	}
}
void CRareCreateDialog::SetRARE_Max_INFO(int Index)
{
	ITEM_RARE_Max_INFO Max_INFO = GetRARE_Max_INFO(Index);

	char line[128] = { 0, };

	char szDescText[384] = { 0, };

	/*if (Max_INFO.TypeName)
	{
	wsprintf(line, "%s Maxㄩ^n", Max_INFO.TypeName);

	strcat(szDescText, line);
	}*/

	if (Max_INFO.CheRyukMax>0)
	{
		wsprintf(line, "[极窐扽俶]ㄩ%d ^n", Max_INFO.CheRyukMax);

		strcat(szDescText, line);
	}

	if (Max_INFO.ThreeMax > 0)
	{
		wsprintf(line, "[薯|鏗|陑]ㄩ%d ^n", Max_INFO.ThreeMax);

		strcat(szDescText, line);
	}

	if (Max_INFO.LifeMax > 0)
	{
		wsprintf(line, "[悛|誘|囀]ㄩ%d ^n", Max_INFO.LifeMax);

		strcat(szDescText, line);
	}

	if (Max_INFO.PhyAttackMax > 0)
	{
		wsprintf(line, "[挕ん馴僻]ㄩ%d ^n", Max_INFO.PhyAttackMax);

		strcat(szDescText, line);
	}

	if (Max_INFO.PhyDefenseMax > 0)
	{
		wsprintf(line, "[昜燴滅郘]ㄩ%d ^n", Max_INFO.PhyDefenseMax);

		strcat(szDescText, line);
	}

	if (Max_INFO.AttrAttackMax > 0)
	{
		wsprintf(line, "[拻俴馴僻]ㄩ%d ^n", Max_INFO.AttrAttackMax);

		strcat(szDescText, line);
	}
	if (Max_INFO.AttrRegistMax > 0)
	{
		wsprintf(line, "[拻俴滅郘]ㄩ%d ^n", Max_INFO.AttrRegistMax);

		strcat(szDescText, line);
	}

	RareMax_Info->SetStaticText(szDescText);
}

