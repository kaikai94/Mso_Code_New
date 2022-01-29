// VirtualItem.cpp: implementation of the CVirtualItem class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include "VirtualItem.h"
#include "ItemManager.h"
#include "./interface/cWindowManager.h"
#include "./interface/cFont.h"
#include "cScriptManager.h"

#include "./Interface/cFont.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction

//////////////////////////////////////////////////////////////////////


CVirtualItem::CVirtualItem()
{
	m_pItemLink = NULL;
	m_type = WT_VIRTUALITEM;
	cIcon::Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL, -1 );
	//InitShowImage();
}

CVirtualItem::~CVirtualItem()
{
	m_pItemLink = NULL;
}
void CVirtualItem::SetLinkItem(CBaseItem * pItem)
{
	m_pItemLink = pItem;
	if(pItem)
	{
		SetBasicImage(m_pItemLink->GetBasicImage());
	}
	else
	{
		SetCurImage(NULL);
	}
}
eITEM_KINDBIT CVirtualItem::GetSrcItemKind()
{
	if(m_pItemLink)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_pItemLink->GetItemIdx());
		return (eITEM_KINDBIT)pInfo->ItemKind;
	}
	return eKIND_ITEM_MAX;
}
void CVirtualItem::Render()

{
	cIcon::Render();

	if( m_pItemLink && ITEMMGR->IsDupItem(m_pItemLink->GetItemIdx()) )//m_pItemLink->GetDurability() >= 1 )
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_pItemLink->GetDurability());
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_pItemLink->GetItemIdx());
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
		if(m_pItemLink->GetRareness()!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE))
		{//[绿装标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y+28};
			m_ItemShowKind[eShowKind_Rare].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_pItemLink->GetDurability()!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE))
		{//[强化标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+29, m_absPos.y+26};
			m_ItemShowKind[eShowKind_Option].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
}