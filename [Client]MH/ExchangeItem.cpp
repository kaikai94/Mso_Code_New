









#include "stdafx.h"

#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "./interface/cFont.h"
#include "cScriptManager.h"
#include "ItemManager.h"
#include "ChatManager.h"



CExchangeItem::CExchangeItem()
{
	m_pLinkItem		= NULL;
	m_pLinkImage	= NULL;
	m_LinkPos		= -1;

	m_Durability	= 1;
	m_ItemIdx		=	0;
	m_type			= WT_EXCHANGEITEM;

	m_dwParam		= 0;

	m_Grow			= 0; 
	//[耐久，制造者附加][By:十里坡剑神][QQ:112582793][2018/1/2]
	m_Abrasion		= 0;
	m_MaxItemAbrasion = 0;
	memset(&m_Maker,0,sizeof(m_Maker));
	memset(&m_Maker1,0,sizeof(m_Maker1));
}

CExchangeItem::~CExchangeItem()
{

}


void CExchangeItem::InitItem( CItem* pItem, POSTYPE Pos, LONG id )
{
	cImage tmpImage;
	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE,
		  ITEMMGR->GetIconImage( pItem->GetItemIdx(), &tmpImage ), id );

	m_pLinkItem = pItem;
	m_Position	= Pos;
	m_LinkPos	= pItem->GetPosition();
	//[保存物品编号，获取物品信息。强化绿化图标添加][By:十里坡剑神][QQ:112582793][2018/2/18]
	m_ItemIdx = pItem->GetItemIdx();

	m_pLinkImage = WINDOWMGR->GetImageOfFile("./image/GameIn/link.tif", 14);

	m_dwParam = pItem->GetItemParam();

	SetIconType( eIconType_LinkedItem );
	SetData( pItem->GetItemIdx() );

	if( ITEMMGR->IsPetSummonItem( pItem->GetItemIdx() ) )
	{
		ITEMMGR->SetToolTipIcon( this, NULL, NULL, pItem->GetDBIdx() );
	}
	else if( ITEMMGR->IsOptionItem( pItem->GetItemIdx(), pItem->GetDurability() ) )
	{
		ITEMMGR->SetToolTipIcon( this, ITEMMGR->GetItemOption( pItem->GetDurability() ),
		ITEMMGR->GetItemRareOption( pItem->GetRareness() ),0); 
	}
	else
		ITEMMGR->SetToolTipIcon( this, NULL, ITEMMGR->GetItemRareOption( pItem->GetRareness() ),0);
}


void CExchangeItem::SetLock( BOOL bLock )
{

	cIcon::SetLock( bLock );
	SetMovable( !bLock );
}
void CExchangeItem::Render()
{
	if( m_bActive == FALSE ) return;
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemIdx);
	cDialog::Render();
	if( m_dwParam & ITEM_PARAM_SEAL )
	{
		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	else
		cIcon::Render();

	if( ITEMMGR->IsDupItem( (WORD)m_dwData ) )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_Durability);

		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));		// color hard coding : taiyo 
	}
	if(pInfo)
	{	
			//[装备组合显示到图标][By:十里坡剑神][QQ:112582793][2018/1/23]
		if( IsMixItem(pInfo->ItemName) ||GetShowImage(pInfo->ItemName))
		{
			eItemShowKind t_ShowKind = eShowKind_Max;
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+28, m_absPos.y};

			t_ShowKind = GetShowImagAddNum(pInfo->ItemName);
			if(t_ShowKind!=eShowKind_Max)
			{
				pos.x = m_absPos.x+1;
				pos.y = m_absPos.y+1;
				m_ItemShowKind[t_ShowKind].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
				t_ShowKind = eShowKind_Max;
			}
			if(t_ShowKind==eShowKind_Add10)
			{
				m_ItemShowKind[eShowKind_Zhen].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			}
		}
		/*if(*(pInfo->ItemName + nLen - 3) == '+')
		{
			if(GetShowImage(pInfo->ItemName))
			{
				VECTOR2 vScale = { 1.0f, 1.0f };
				VECTOR2 pos = { m_absPos.x+28, m_absPos.y};
				m_ItemShowKind[eShowKind_Zhen].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
					pos.x = m_absPos.x+1;
				pos.y = m_absPos.y+1;
				m_ItemShowKind[eShowKind_Add10].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			}
			else
			{
				char buf1[3];
				sprintf(buf1,"+%c",pInfo->ItemName[(nLen - 1)]);
				RECT rect={(LONG)m_absPos.x, (LONG)m_absPos.y-2, 1,1};
				CFONT_OBJ->RenderFont(6,buf1,strlen(buf1),&rect,RGBA_MAKE(252,255,110,255));
			}
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+28, m_absPos.y};
			m_ItemShowKind[eShowKind_Zhen].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
			pos.x = m_absPos.x+1;
			pos.y = m_absPos.y+1;
			m_ItemShowKind[eShowKind_Add10].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}*/
		if(m_dwRareIdx!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE))
		{//[绿装标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y+28};
			m_ItemShowKind[eShowKind_Rare].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_Durability!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE))
		{//[强化标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+29, m_absPos.y+26};
			m_ItemShowKind[eShowKind_Option].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
}


char* CExchangeItem::GetItemName()
{
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo((WORD)GetData());
	if(pItemInfo)
		return pItemInfo->ItemName;
	return NULL;

}
