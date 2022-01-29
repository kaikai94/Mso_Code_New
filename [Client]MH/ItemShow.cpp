









// ItemShow.cpp: implementation of the CItemShow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "ItemShow.h"


#include "ItemManager.h"
#include "./Interface/cFont.h"
#include "./interface/cWindowManager.h"
#include "cScriptManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemShow::CItemShow()

{
	m_type = WT_ITEMSHOW;
	cIcon::Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, NULL, -1 );
	

	m_wItemIdx	= 0;
	m_Dur		= 0;
	InitShowImage();
}


CItemShow::~CItemShow()
{


}

void CItemShow::Init(WORD wItemIdx, DURTYPE dur)
{
	m_wItemIdx = wItemIdx;
	m_Dur = dur;
	if(m_wItemIdx)
	{
		cImage tmpImage;

		ITEMMGR->GetIconImage( wItemIdx, &tmpImage );
		SetBasicImage(&tmpImage);

	}
	else
	{
		SetCurImage(NULL);
	}

}
void CItemShow::Render()
{
	cIcon::Render();

	if( ITEMMGR->IsDupItem(m_wItemIdx) )
	{
		static char nums[3];

		wsprintf(nums,"%2d", m_Dur);
		RECT rect={(LONG)m_absPos.x+27, (LONG)m_absPos.y+29, 1,1};
		CFONT_OBJ->RenderFont(0,nums,strlen(nums),&rect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100 ));
	}
	ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(m_wItemIdx);
	if(pInfo)
	{	
				//[¡Á¡ã¡À?¡Á¨¦o???¨º?¦Ì?¨ª?¡À¨º][By:o¨²oo¨¨y][QQ:112582793][2018/1/23]
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
	}
}