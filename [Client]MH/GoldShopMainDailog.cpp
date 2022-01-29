#include "stdafx.h"
#include "GoldShopMainDailog.h"
#include "WindowIDEnum.h"
#include "GoldShopSubDialog.h"
#include "cPushupButton.h"
#include "ItemManager.h"
#include "cScriptManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "cWindowManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "cDivideBox.h"
#include "ObjectManager.h"

CGoldShopMainDailog::CGoldShopMainDailog(void)
{
	for(int i = 0;i<15;i++)
	{
		m_pItemPrice[i]	= NULL;
		m_pItemName[i]	= NULL;
		m_pItemTip[i]	= NULL;
		m_pBuyBtton[i]	= NULL;
	}
	for(int i = 0;i<11;i++)
		m_SubDlg[i] = NULL;
	m_GoldMoney	= NULL;
	m_MallMoney	= NULL;
	m_bShow = FALSE;
	InitImage();
	m_nCurMainIndex	= 0;
	m_nCurSubIndex	= 0;
	m_nCurItemIndex	= 0;
	m_DealIdx = 0;
	m_BuyItem = NULL;
	m_dwBuyPrice =0;
	m_IconIndexCreator.Init(5000,90000);
}
void CGoldShopMainDailog::InitImage()
{
	WORD t_wImageIdx = 235;
	for(int i = 0;i<eMaxType;i++)
		SCRIPTMGR->GetImage(t_wImageIdx+i,&m_MoneyImage[i]);
}
CGoldShopMainDailog::~CGoldShopMainDailog(void)
{
	m_BuyItem = NULL;
	m_IconIndexCreator.Release();
}
void CGoldShopMainDailog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_GOLDSHOPSUB_DLG||
		window->GetType() == WT_DIALOG)
	{
		AddTabSheet(curIdx2++, window);
	}
	else 
		cDialog::Add(window);
}

void CGoldShopMainDailog::Linking()
{
	for(int i = 0;i<15;i++)
	{
		m_pItemPrice[i]	= (cStatic*)GetWindowForID(ITEM_PRICE_01+i);
		m_pItemName[i]	= (cStatic*)GetWindowForID(ITEM_NAME_01+i);
		m_pItemTip[i]	= (cStatic*)GetWindowForID(ITEM_TIP_01+i);
		m_pBuyBtton[i]	= (cButton*)GetWindowForID(BUY_BTN_1+i);
		m_pItemPrice[i]->SetRender(FALSE);
		m_pItemName[i]->SetRender(FALSE);
		m_pItemTip[i]->SetRender(FALSE);
		m_pBuyBtton[i]->SetRender(FALSE);
	}
	for(int i = 0;i<11;i++)
	{
		m_SubDlg[i] = (CGoldShopSubDialog*)GetTabSheet( i );
		for(int j = 1;j<10;j++)
		{
			((cPushupButton*)m_SubDlg[i]->GetTabBtn(j))->SetRender(FALSE);
		}
	}
	m_GoldMoney=(cStatic *)GetWindowForID(SHOP_SHOW_GOLDMONEY);

	m_MallMoney=(cStatic *)GetWindowForID(SHOP_SHOW_MALLMONEY);
}
void CGoldShopMainDailog::AddGoldShopItem(GOLD_SHOP_ITEM_INFO& pInfo)
{
	GoldDealItem* pItem = NULL;
	cImage tmpImage;
	pItem = new GoldDealItem;
	pItem->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE, ITEMMGR->GetIconImage(pInfo.wItemIndex, &tmpImage),m_IconIndexCreator.GenerateIndex()/*ITEMMGR->GetItemGenerateIndex()*/);
	pItem->SetData(pInfo.wItemIndex);
	pItem->SetItemInfo(pInfo);
	ITEMMGR->SetGoldShopToolTipIcon(pItem);
	if( !ITEMMGR->CanEquip(pInfo.wItemIndex) )
		pItem->SetToolTipImageRGB( TTCLR_ITEM_CANNOTEQUIP );
	if(pInfo.dwCanBuyTime!=0)
	{
		stTIME stTime;
		stTime.SetTime(pInfo.dwCanBuyTime);
		pItem->AddToolTipLine("");
		char buf[64] = {0,};
		sprintf(buf,CHATMGR->GetChatMsg(2361),stTime.GetYear(),stTime.GetMonth(),stTime.GetDay(),stTime.GetHour(),stTime.GetMinute());
		pItem->AddToolTipLine(buf,RGB(0,255,0));
	}
	WINDOWMGR->AddWindow(pItem);
	cIconGridDialog * dlg = (cIconGridDialog *)m_SubDlg[pInfo.wMainIndex]->GetTabSheet(pInfo.wSubIndex);
	dlg->AddIcon(pInfo.wGridIndex, pItem);
	((cPushupButton*)m_SubDlg[pInfo.wMainIndex]->GetTabBtn(pInfo.wSubIndex))->SetRender(TRUE);
}

void CGoldShopMainDailog::SetActive(BOOL val)
{
	m_BuyItem = NULL;
	if(val)
	{
		GAMEIN->UpdataGoldMoney();
		ITEMMGR->AddGoldShopItem();
		SetStatic();
	}
	else
		ClearShopItem();
	cDialog::SetActive(val);
	m_bShow = val;

	m_DealIdx = 0;
}
void CGoldShopMainDailog::SetStatic()
{
	for(int i = 0;i<15;i++)
	{
		m_pItemPrice[i]->SetRender(FALSE);
		m_pItemName[i]->SetRender(FALSE);
		m_pItemTip[i]->SetRender(FALSE);
		m_pItemTip[i]->SetBasicImage(NULL);
		m_pBuyBtton[i]->SetRender(FALSE);
	}
	cIconGridDialog * dlg = (cIconGridDialog *)m_SubDlg[m_nCurMainIndex]->GetTabSheet(m_nCurSubIndex);
	GoldDealItem* pItem = NULL;

	for(int i = 0;i<15;i++)
	{
		pItem = (GoldDealItem* )dlg->GetIconForIdx(i);
		if(pItem)
		{
			if(pItem->GetInfo().wItemIndex!=0)
			{
				m_pItemPrice[i]->SetStaticText(AddComma(MONEYTYPE(pItem->GetInfo().dwPrice)));
				if(pItem->GetInfo().wCurrency==3)
					m_pItemPrice[i]->SetBasicImage(&m_MoneyImage[0]);
				else
					m_pItemPrice[i]->SetBasicImage(&m_MoneyImage[pItem->GetInfo().wCurrency]);
				m_pItemPrice[i]->SetRender(TRUE);
				m_pItemName[i]->SetStaticText(pItem->GetInfo().strItemName);
				m_pItemName[i]->SetRender(TRUE);
				m_pItemTip[i]->SetStaticText(pItem->GetInfo().strItemTip);
				m_pItemTip[i]->SetRender(TRUE);
				m_pBuyBtton[i]->SetRender(TRUE);
			}
		}
	}
}
void CGoldShopMainDailog::ClearShopItem()
{
	cIcon* pIcon;
	for(int tab=0;tab<m_bTabNum;++tab)
	{
		for(int i = 0;i<10;i++)
		{
			cIconGridDialog * dlg = (cIconGridDialog *)m_SubDlg[tab]->GetTabSheet(i);
			{
				for(int n=0;n<dlg->m_nCol*dlg->m_nRow;++n)
				{
					pIcon = dlg->GetIconForIdx(n);
					if(pIcon != NULL)
					{
						m_IconIndexCreator.ReleaseIndex(pIcon->GetID());
						dlg->DeleteIcon(pIcon);
						pIcon->SetActive( FALSE );
						WINDOWMGR->AddListDestroyWindow( pIcon );
						pIcon = NULL;
					}

				}
			}
		}
	}
}
void CGoldShopMainDailog::OnSelectItem()
{
	m_dwBuyPrice = 0;
	cIconGridDialog * gridDlg = (cIconGridDialog *)m_SubDlg[GetCurTabNum()]->GetTabSheet(m_SubDlg[GetCurTabNum()]->GetCurTabNum());
	m_BuyItem = (GoldDealItem*)gridDlg->GetIconForIdx(m_nCurItemIndex);
	if(m_BuyItem)
	{
		FakeBuyItem((LONG)(this->GetWidth()/2),(LONG)(this->GetAbsY()+100));
	}
}
void CGoldShopMainDailog::FakeBuyItem(LONG x, LONG y)
{
	if(m_BuyItem==NULL) return;
	memset(&m_buyMsg, 0, sizeof(m_buyMsg));
	m_buyMsg.Category		= MP_ITEM;
	m_buyMsg.Protocol		= MP_ITEM_BUY_SYN;
	m_buyMsg.dwObjectID		= HEROID;
	m_buyMsg.wBuyItemIdx	= m_BuyItem->GetInfo().wItemIndex;
	m_buyMsg.wDealerIdx		= 10000;
	m_buyMsg.wIdxPos        = m_nCurItemIndex;
	m_buyMsg.wMainIndex		= GetCurTabNum();
	m_buyMsg.wSunIndex		= m_SubDlg[GetCurTabNum()]->GetCurTabNum();
	m_buyMsg.wBuyType		= m_BuyItem->GetInfo().wCurrency;

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_BUY, x, y, OnFakeBuyItem, OnCancelBuyItem,
		this, m_BuyItem, CHATMGR->GetChatMsg(187) );
	if( !pDivideBox )
		return;

	pDivideBox->SetMaxValue( MAX_ITEMBUY_NUM );
	pDivideBox->SetValue(1);
}
void CGoldShopMainDailog::OnFakeBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	if( param1 == 0 )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Deal);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		return;
	}

	CGoldShopMainDailog * tDlg = ( CGoldShopMainDailog * )vData1;
	GoldDealItem * pItem = (GoldDealItem *)vData2;
	if(pItem==NULL) return;

	tDlg->m_buyMsg.BuyItemNum	= (WORD)param1;

	MONEYTYPE BuyPrice,ItemBuyPrice;

	if(pItem->GetInfo().wItemIndex==0) return;

	BuyPrice = pItem->GetInfo().dwPrice;
	ItemBuyPrice = pItem->GetInfo().dwPrice;

	if(pItem->GetInfo().wCurrency==2)
	{//[元宝购买提示][By:十里坡剑神][QQ:112582793][2018/9/30]
		if(HERO->GetGoldMoney()<BuyPrice)
		{//[元宝不够是否充值提示][By:十里坡剑神][QQ:112582793][2018/9/30]
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MSBOX_GOLDMONEPAY, MBT_YESNO, CHATMGR->GetChatMsg( 2307 ));
			//GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_GOLDSHOPBUYITEM, MBT_YESNO, CHATMGR->GetChatMsg( 2142 ), 
				pItem->GetItemName(), tDlg->m_buyMsg.BuyItemNum, 
				BuyPrice, BuyPrice*tDlg->m_buyMsg.BuyItemNum );
		}
	}
	else if(pItem->GetInfo().wCurrency==1)
	{//[泡点购买提示][By:十里坡剑神][QQ:112582793][2018/9/30]
		if(HERO->GetMallMoney()<BuyPrice)
		{//[泡点不够][By:十里坡剑神][QQ:112582793][2018/9/30]
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2181));
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_GOLDSHOPBUYITEM, MBT_YESNO, CHATMGR->GetChatMsg( 2143 ), 
				pItem->GetItemName(), tDlg->m_buyMsg.BuyItemNum, 
				BuyPrice, BuyPrice*tDlg->m_buyMsg.BuyItemNum );
		}
	}
	else if(pItem->GetInfo().wCurrency==3)
	{//[游戏币购买提示][By:十里坡剑神][QQ:112582793][2018/9/30]
		if(HERO->GetMoney()<BuyPrice)
		{//[游戏币不够][By:十里坡剑神][QQ:112582793][2018/9/30]
			CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(117));
		}
		else
		{
			tDlg->m_dwBuyPrice =BuyPrice; 
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_GOLDSHOPBUYITEM, MBT_YESNO, CHATMGR->GetChatMsg( 2306 ), 
				pItem->GetItemName(), tDlg->m_buyMsg.BuyItemNum, 
				BuyPrice, BuyPrice*tDlg->m_buyMsg.BuyItemNum );
		}
	}
}
void CGoldShopMainDailog::ClearBuyMsgInfo()
{
	memset(&m_buyMsg,0,sizeof(m_buyMsg));
	m_nCurItemIndex = 0;
	m_nCurMainIndex = GetCurTabNum();
	m_nCurSubIndex = m_SubDlg[m_nCurMainIndex]->GetCurTabNum();
	m_BuyItem = NULL;
}
void CGoldShopMainDailog::OnCancelBuyItem( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	GAMEIN->GetGoldShopMainDlg()->ClearBuyMsgInfo();
}
void CGoldShopMainDailog::SendBuyMsg()
{
	NETWORK->Send( &this->m_buyMsg, this->m_buyMsg.GetSize() );
	ClearBuyMsgInfo();
}
void CGoldShopMainDailog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we&WE_BTNCLICK)
	{
		switch(lId)
		{
			case BUY_BTN_1:m_nCurItemIndex = 0;OnSelectItem();break;
			case BUY_BTN_2:m_nCurItemIndex = 1;OnSelectItem();break;
			case BUY_BTN_3:m_nCurItemIndex = 2;OnSelectItem();break;
			case BUY_BTN_4:m_nCurItemIndex = 3;OnSelectItem();break;
			case BUY_BTN_5:m_nCurItemIndex = 4;OnSelectItem();break;
			case BUY_BTN_6:m_nCurItemIndex = 5;OnSelectItem();break;
			case BUY_BTN_7:m_nCurItemIndex = 6;OnSelectItem();break;
			case BUY_BTN_8:m_nCurItemIndex = 7;OnSelectItem();break;
			case BUY_BTN_9:m_nCurItemIndex = 8;OnSelectItem();break;
			case BUY_BTN_10:m_nCurItemIndex = 9;OnSelectItem();break;
			case BUY_BTN_11:m_nCurItemIndex = 10;OnSelectItem();break;
			case BUY_BTN_12:m_nCurItemIndex = 11;OnSelectItem();break;
			case BUY_BTN_13:m_nCurItemIndex = 12;OnSelectItem();break;
			case BUY_BTN_14:m_nCurItemIndex = 13;OnSelectItem();break;
			case BUY_BTN_15:m_nCurItemIndex = 14;OnSelectItem();break;
		}
	}
	if(we&WE_PUSHDOWN)
	{
		switch(lId)
		{
		case GOLDSHOPMAINTABBTN1:m_nCurMainIndex = 0;((cTabDialog*)m_SubDlg[0])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN2:m_nCurMainIndex = 1;((cTabDialog*)m_SubDlg[1])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN3:m_nCurMainIndex = 2;((cTabDialog*)m_SubDlg[2])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN4:m_nCurMainIndex = 3;((cTabDialog*)m_SubDlg[3])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN5:m_nCurMainIndex = 4;((cTabDialog*)m_SubDlg[4])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN6:m_nCurMainIndex = 5;((cTabDialog*)m_SubDlg[5])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN7:m_nCurMainIndex = 6;((cTabDialog*)m_SubDlg[6])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN8:m_nCurMainIndex = 7;((cTabDialog*)m_SubDlg[7])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN9:m_nCurMainIndex = 8;((cTabDialog*)m_SubDlg[8])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN10:m_nCurMainIndex = 9;((cTabDialog*)m_SubDlg[9])->SelectTab(0);m_nCurSubIndex=0;break;
		case GOLDSHOPMAINTABBTN11:m_nCurMainIndex = 10;((cTabDialog*)m_SubDlg[10])->SelectTab(0);m_nCurSubIndex=0;break;

		case GOLDSHOP_SUB1_TABBTN1_1:
		case GOLDSHOP_SUB2_TABBTN1_1:
		case GOLDSHOP_SUB3_TABBTN1_1:
		case GOLDSHOP_SUB4_TABBTN1_1:
		case GOLDSHOP_SUB5_TABBTN1_1:
		case GOLDSHOP_SUB6_TABBTN1_1:
		case GOLDSHOP_SUB7_TABBTN1_1:
		case GOLDSHOP_SUB8_TABBTN1_1:
		case GOLDSHOP_SUB9_TABBTN1_1:
		case GOLDSHOP_SUB10_TABBTN1_1:
		case GOLDSHOP_SUB11_TABBTN1_1:m_nCurSubIndex=0;	break;

		case GOLDSHOP_SUB1_TABBTN1_2:
		case GOLDSHOP_SUB2_TABBTN1_2:
		case GOLDSHOP_SUB3_TABBTN1_2:
		case GOLDSHOP_SUB4_TABBTN1_2:
		case GOLDSHOP_SUB5_TABBTN1_2:
		case GOLDSHOP_SUB6_TABBTN1_2:
		case GOLDSHOP_SUB7_TABBTN1_2:
		case GOLDSHOP_SUB8_TABBTN1_2:
		case GOLDSHOP_SUB9_TABBTN1_2:
		case GOLDSHOP_SUB10_TABBTN1_2:
		case GOLDSHOP_SUB11_TABBTN1_2:m_nCurSubIndex=1;	break;

		case GOLDSHOP_SUB1_TABBTN1_3:
		case GOLDSHOP_SUB2_TABBTN1_3:
		case GOLDSHOP_SUB3_TABBTN1_3:
		case GOLDSHOP_SUB4_TABBTN1_3:
		case GOLDSHOP_SUB5_TABBTN1_3:
		case GOLDSHOP_SUB6_TABBTN1_3:
		case GOLDSHOP_SUB7_TABBTN1_3:
		case GOLDSHOP_SUB8_TABBTN1_3:
		case GOLDSHOP_SUB9_TABBTN1_3:
		case GOLDSHOP_SUB10_TABBTN1_3:
		case GOLDSHOP_SUB11_TABBTN1_3:m_nCurSubIndex=2;	break;

		case GOLDSHOP_SUB1_TABBTN1_4:
		case GOLDSHOP_SUB2_TABBTN1_4:
		case GOLDSHOP_SUB3_TABBTN1_4:
		case GOLDSHOP_SUB4_TABBTN1_4:
		case GOLDSHOP_SUB5_TABBTN1_4:
		case GOLDSHOP_SUB6_TABBTN1_4:
		case GOLDSHOP_SUB7_TABBTN1_4:
		case GOLDSHOP_SUB8_TABBTN1_4:
		case GOLDSHOP_SUB9_TABBTN1_4:
		case GOLDSHOP_SUB10_TABBTN1_4:
		case GOLDSHOP_SUB11_TABBTN1_4:m_nCurSubIndex=3;	break;

		case GOLDSHOP_SUB1_TABBTN1_5:
		case GOLDSHOP_SUB2_TABBTN1_5:
		case GOLDSHOP_SUB3_TABBTN1_5:
		case GOLDSHOP_SUB4_TABBTN1_5:
		case GOLDSHOP_SUB5_TABBTN1_5:
		case GOLDSHOP_SUB6_TABBTN1_5:
		case GOLDSHOP_SUB7_TABBTN1_5:
		case GOLDSHOP_SUB8_TABBTN1_5:
		case GOLDSHOP_SUB9_TABBTN1_5:
		case GOLDSHOP_SUB10_TABBTN1_5:
		case GOLDSHOP_SUB11_TABBTN1_5:m_nCurSubIndex=4;	break;

		case GOLDSHOP_SUB1_TABBTN1_6:
		case GOLDSHOP_SUB2_TABBTN1_6:
		case GOLDSHOP_SUB3_TABBTN1_6:
		case GOLDSHOP_SUB4_TABBTN1_6:
		case GOLDSHOP_SUB5_TABBTN1_6:
		case GOLDSHOP_SUB6_TABBTN1_6:
		case GOLDSHOP_SUB7_TABBTN1_6:
		case GOLDSHOP_SUB8_TABBTN1_6:
		case GOLDSHOP_SUB9_TABBTN1_6:
		case GOLDSHOP_SUB10_TABBTN1_6:
		case GOLDSHOP_SUB11_TABBTN1_6:m_nCurSubIndex=5;	break;

		case GOLDSHOP_SUB1_TABBTN1_7:
		case GOLDSHOP_SUB2_TABBTN1_7:
		case GOLDSHOP_SUB3_TABBTN1_7:
		case GOLDSHOP_SUB4_TABBTN1_7:
		case GOLDSHOP_SUB5_TABBTN1_7:
		case GOLDSHOP_SUB6_TABBTN1_7:
		case GOLDSHOP_SUB7_TABBTN1_7:
		case GOLDSHOP_SUB8_TABBTN1_7:
		case GOLDSHOP_SUB9_TABBTN1_7:
		case GOLDSHOP_SUB10_TABBTN1_7:
		case GOLDSHOP_SUB11_TABBTN1_7:m_nCurSubIndex=6;	break;

		case GOLDSHOP_SUB1_TABBTN1_8:
		case GOLDSHOP_SUB2_TABBTN1_8:
		case GOLDSHOP_SUB3_TABBTN1_8:
		case GOLDSHOP_SUB4_TABBTN1_8:
		case GOLDSHOP_SUB5_TABBTN1_8:
		case GOLDSHOP_SUB6_TABBTN1_8:
		case GOLDSHOP_SUB7_TABBTN1_8:
		case GOLDSHOP_SUB8_TABBTN1_8:
		case GOLDSHOP_SUB9_TABBTN1_8:
		case GOLDSHOP_SUB10_TABBTN1_8:
		case GOLDSHOP_SUB11_TABBTN1_8:m_nCurSubIndex=7;	break;

		case GOLDSHOP_SUB1_TABBTN1_9:
		case GOLDSHOP_SUB2_TABBTN1_9:
		case GOLDSHOP_SUB3_TABBTN1_9:
		case GOLDSHOP_SUB4_TABBTN1_9:
		case GOLDSHOP_SUB5_TABBTN1_9:
		case GOLDSHOP_SUB6_TABBTN1_9:
		case GOLDSHOP_SUB7_TABBTN1_9:
		case GOLDSHOP_SUB8_TABBTN1_9:
		case GOLDSHOP_SUB9_TABBTN1_9:
		case GOLDSHOP_SUB10_TABBTN1_9:
		case GOLDSHOP_SUB11_TABBTN1_9:m_nCurSubIndex=8;	break;

		case GOLDSHOP_SUB1_TABBTN1_10:
		case GOLDSHOP_SUB2_TABBTN1_10:
		case GOLDSHOP_SUB3_TABBTN1_10:
		case GOLDSHOP_SUB4_TABBTN1_10:
		case GOLDSHOP_SUB5_TABBTN1_10:
		case GOLDSHOP_SUB6_TABBTN1_10:
		case GOLDSHOP_SUB7_TABBTN1_10:
		case GOLDSHOP_SUB8_TABBTN1_10:
		case GOLDSHOP_SUB9_TABBTN1_10:
		case GOLDSHOP_SUB10_TABBTN1_10:
		case GOLDSHOP_SUB11_TABBTN1_10:m_nCurSubIndex=9;	break;
		}
		SetStatic();
	}
}
void CGoldShopMainDailog::SetGoldMoney(MONEYTYPE val)
{
	m_GoldMoney->SetStaticText(AddComma(val));
	m_GoldMoney->SetAlign(TXT_LEFT);
}
void CGoldShopMainDailog::SetMallMoney(MONEYTYPE val)
{
	m_MallMoney->SetStaticText(AddComma(val));
	m_MallMoney->SetAlign(TXT_LEFT);
}
