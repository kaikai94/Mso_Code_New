







// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Item.h"
#include "ItemManager.h"

#include "ObjectManager.h"
#include "QuickItem.h"
#include "GameIn.h"
#include "./Interface/cFont.h"

#include "QuickDialog.h"
#include "ChatManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "interface/cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItem::CItem(ITEMBASE* pInfo)

{
	m_type = WT_ITEM;
	m_bAdded = FALSE;

	memcpy(&m_ItemBaseInfo,pInfo,sizeof(ITEMBASE));

	m_pLinkItem	= NULL;
	m_LinkPos	= 0;
	m_Param = 0;	
}

CItem::~CItem()
{
}
eITEM_KINDBIT CItem::GetItemKind()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);	//KES 040512
	if(pInfo)
		return (eITEM_KINDBIT)pInfo->ItemKind;


	return (eITEM_KINDBIT)0;
}


const ITEM_INFO* CItem::GetItemInfo()
{
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	ASSERT(pInfo);
	if(pInfo)
	ASSERT(pInfo->ItemIdx == m_ItemBaseInfo.wIconIdx);
	return pInfo;
}

void CItem::SetItemBaseInfo( ITEMBASE * itemInfo )
{
	memcpy( &m_ItemBaseInfo, itemInfo, sizeof( ITEMBASE ) );
}


void CItem::Render()
{
	if( !m_bActive )		return;	
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_ItemBaseInfo.wIconIdx);
	cDialog::Render();
	if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_SEAL )
	{
		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(773),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	//else if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_PD)  // 如果是泡点 2014-11-20
	//{
	//	RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
	//	CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1515),strlen(CHATMGR->GetChatMsg(1515)),&rect,RGBA_MAKE(250,250,128,255));
	//}
	else if( m_ItemBaseInfo.ItemParam & ITEM_PARAM_TITAN_REGIST )
	{
		DWORD dwRemainTime = 0;

		float delayRate = HERO->GetDelayGroup()->CheckDelay(CDelayGroup::eDK_Item,m_ItemBaseInfo.wIconIdx, &dwRemainTime );
		if( delayRate > 0.0f )

		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}

		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1515),4,&rect,RGBA_MAKE(250,250,128,255));	// magi82 - Titan(070427)
	}
	else if( PETMGR->IsCurPetSummonItem(m_ItemBaseInfo.dwDBIdx) )
	{
		RECT rect={(LONG)m_absPos.x+9, (LONG)m_absPos.y+15, 1,1};
		CFONT_OBJ->RenderFont(0,CHATMGR->GetChatMsg(1246),strlen(CHATMGR->GetChatMsg(773)),&rect,RGBA_MAKE(250,250,128,255));
	}
	else
		cIcon::Render();

	if( m_Param == 1 )
	{
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
			RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
	}

	if( ITEMMGR->IsDupItem(m_ItemBaseInfo.wIconIdx) )// && m_ItemBaseInfo.Durability >= 1 )
	{
		static char nums[3];

		wsprintf(nums,"%2d", m_ItemBaseInfo.Durability);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};

		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));		// color hard coding : taiyo 
	}

	if( GetItemKind() == eYOUNGYAK_ITEM_TITAN )
	{
		DWORD dwRemainTime = 0;
		float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime );
		if( delayRate > 0.0f )
		{
			VECTOR2 vScale = { 1.0f, delayRate };
			VECTOR2 pos = { m_absPos.x, m_absPos.y + m_height*(1 - delayRate) };
			m_DelayImage.RenderSprite( &vScale, NULL, 0.0f, &pos, 0xff000000  );
		}
	}
	if(m_ItemBaseInfo.ItemStatic == ITEM_STATIC_LUCK) 
	{
		SCRIPTMGR->GetImage( 204, &m_LockImageEx, PFT_HARDPATH );
		VECTOR2 vScale = { 0.3f, 0.3f };
		VECTOR2 pos = { m_absPos.x, m_absPos.y+28};
		m_LockImageEx.RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		
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
		if(m_ItemBaseInfo.RareIdx!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE)||
			m_ItemBaseInfo.RareIdx != 0 && (pInfo->ItemKind == eEQUIP_ITEM_INHERIT))
		{//[绿装标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x, m_absPos.y+28};
			m_ItemShowKind[eShowKind_Rare].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
		if(m_ItemBaseInfo.Durability!=0&&(pInfo->ItemKind>=eEQUIP_ITEM&&pInfo->ItemKind<=eEQUIP_ITEM_UNIQUE))
		{//[强化标志][By:十里坡剑神][QQ:112582793][2018/9/21]
			VECTOR2 vScale = { 1.0f, 1.0f };
			VECTOR2 pos = { m_absPos.x+29, m_absPos.y+26};
			m_ItemShowKind[eShowKind_Option].RenderSprite( &vScale, NULL, 0.0f, &pos,0xffffffff);
		}
	}
}

void CItem::SetLock( BOOL bLock,BOOL bSend )
{
	cIcon::SetLock( bLock );	
	SetMovable( !bLock );
	if(bSend)
	{//[服务端锁定物品消息][By:十里坡剑神][QQ:112582793][2018/6/17]
		MSG_ITEM_LOCK msg;
		msg.Category = MP_ITEMEXT;
		msg.Protocol = MP_ITEMEX_LOCK_SYN;
		msg.FromPos  = this->GetPosition();
		msg.bIsLock = bLock;
		msg.dwItemDBIdx = this->GetDBIdx();
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
	}
}

BOOL CItem::CanMoveToDialog( cDialog* ptargetdlg )
{

	if( GetItemKind() == eQUEST_ITEM_EQUIP )
	{
		int type = ptargetdlg->GetType();
		switch( type )
		{
		case WT_STREETSTALLDIALOG:
		case WT_STREETBUYSTALLDIALOG:
		case WT_DEALDIALOG:
		case WT_MUGONGSURYUNDIALOG:

		case WT_GUILDWAREHOUSEDLG:
		case WT_QUESTTOTALDIALOG:
		case WT_ITEMSHOPDIALOG:
			return FALSE;

		}
	}
	return TRUE;
}

BOOL CItem::CanDelete()
{
	if( GetItemKind() == eQUEST_ITEM_EQUIP )
		return FALSE;
	return TRUE;

}


