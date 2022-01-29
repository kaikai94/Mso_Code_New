








// BuyItem.cpp: implementation of the CBuyItem class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BuyItem.h"
#include "ItemManager.h"
#include "./Interface/cFont.h"

#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////

// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuyItem::CBuyItem()
{
	m_ItemIdx = 0;
	m_nVolume = 0;
	m_nMoney = 0;
	m_dwParam = 0;
	m_RareIdx=0;
	m_OptionIdx=0;
	//[�;ã������߸���][By:ʮ���½���][QQ:112582793][2018/1/2]
	m_Abrasion=0;
	m_MaxItemAbrasion = 0;
	memset(&m_Maker,0,sizeof(m_Maker));
	memset(&m_Maker1,0,sizeof(m_Maker1));
}

CBuyItem::~CBuyItem()
{

}


void CBuyItem::InitItem(ITEMBASE ItemInfo, WORD Volume, MONEYTYPE Money)

{
	m_ItemIdx = ItemInfo.wIconIdx;
	m_nVolume = Volume;
	m_nMoney = Money;
	m_dwParam = ItemInfo.ItemParam;

	WORD ItemKind = GetItemKind(m_ItemIdx);

	m_bVolume = !(( ItemKind > eEQUIP_ITEM) && ( ItemKind < eEXTRA_ITEM) && !ITEMMGR->IsDupItem(m_ItemIdx));


			
	cImage tmpImage;

	Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE,
		ITEMMGR->GetIconImage( ItemInfo.wIconIdx, &tmpImage ) );
	
	SetIconType( eIconType_LinkedItem );
	SetData( ItemInfo.wIconIdx );
	SetGrow( ItemInfo.ItemGrow ); 
	//[�;ã������߸���][By:ʮ���½���][QQ:112582793][2018/1/2]
	SetAbrasion(ItemInfo.ItemAbrasion);
	SetMaxItemAbrasion(ItemInfo.MaxItemAbrasion);
	SetMaker(ItemInfo.ItemMaker);
	SetMaker1(ItemInfo.ItemMaker1);
	ITEMMGR->SetToolTipIcon(this);
}

void CBuyItem::Render()

{
	if( m_bActive == FALSE ) return;

	if( m_dwParam & ITEM_PARAM_SEAL )
	{
		cDialog::Render();
		
		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}


	else
		cIcon::Render();

	if(m_bVolume && m_nVolume > 0)
	{
		static char nums[3];
		wsprintf(nums,"%2d", m_nVolume);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));	
	}
}




