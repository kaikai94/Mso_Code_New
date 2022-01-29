







// cWindow.cpp: implementation of the cWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cWindow.h"
#include "cFont.h"
#include "cWindowManager.h"
#include "../Input/Mouse.h"
#include "MouseCursor.h"
#include "GameIn.h"
#include "cSendItemInfoDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void defaultWindowFunc(LONG lId, void * p, DWORD we){}

cWindow::cWindow()
{
	m_ID = 0;
	m_wFontIdx=0;
	m_type=WT_WINDOW;
	m_bActive=TRUE;
	m_bDisable = FALSE;
	m_bFocus=TRUE;
	m_bMovable=FALSE;
	m_width=0;
	m_height=0;
	m_pCurImage=NULL;
	m_pToolTipImage=NULL;
	m_bDepend=FALSE;
	m_pParent=NULL;
	m_dwViewCurTime=m_dwViewLastTime=0;
//	memset(m_strToolTipMsg, 0, MAX_TOOLTIPSTR_SIZE);
	m_dwImageRGB = 0xffffff;

	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_validPos.x = 0;
	m_validPos.y = 0;
	cbWindowFunc=defaultWindowFunc;
	m_scale.x = 1.f;
	m_scale.y = 1.f;

	m_bAlwaysTop = FALSE;	
	m_bDestroy = FALSE;
	m_bSetTopOnActive = TRUE;
	//[装备对比][BY:十里坡剑神][QQ:112582793][2019-4-12][15:41:46]
	m_pRelationTip=NULL;
	m_pRelationTip1=NULL;

}


cWindow::~cWindow()
{
	toolTip.Release();
	//[装备对比][BY:十里坡剑神][QQ:112582793][2019-4-12][15:41:46]
	m_pRelationTip=NULL;
	m_pRelationTip1=NULL;
}


void cWindow::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * pBasicImage, LONG ID)
{
	SetAbsXY(x,y);
	SetRelXY(x,y);
	SetWH(wid,hei);
	m_ID=ID;
	if(pBasicImage)
	{
		m_BasicImage = *pBasicImage;
		m_pCurImage = &m_BasicImage;
	}
}

BOOL cWindow::PtInWindow(LONG x, LONG y)
{
	if(m_absPos.x+m_validPos.x > x || m_absPos.y+m_validPos.y > y || m_absPos.x+m_validPos.x+m_width < x || m_absPos.y+m_validPos.y+m_height < y)
		return FALSE;
	else
		return TRUE;
}



DWORD cWindow::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;

	if( !WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
	{
		if( !(WINDOWMGR->IsDragWindow() && (cWindow*)WINDOWMGR->GetDragDlg() == this ) )
		{
			we |= WE_MOUSEOVER;
					
			m_dwViewCurTime = GetTickCount();
			if(m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
			{

				m_ttX = mouseInfo->GetMouseX();

				m_ttY = mouseInfo->GetMouseY();
			}
		}

		
		if( !WINDOWMGR->IsDragWindow() )
		{
			 if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
			 {
				 if( m_type & WT_ICON )

					 CURSOR->SetCursor( eCURSOR_ITEMSELECT );
				 else
					 CURSOR->SetCursor( eCURSOR_DEFAULT );
			 }
		}
	}
	else
	{
		m_dwViewLastTime = m_dwViewCurTime = 0;
	}



	if( !WINDOWMGR->IsMouseDownUsed() )
	if( PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
	{
		if( mouseInfo->LButtonDown() )
			we |= WE_LBTNCLICK;
		if( mouseInfo->RButtonDown() )

			we |= WE_RBTNCLICK;
		if( mouseInfo->LButtonDoubleClick() )

			we |= WE_LBTNDBLCLICK;

		if( mouseInfo->RButtonDoubleClick() )
			we |= WE_RBTNDBLCLICK;
	}

	return we;

}

void cWindow::ExcuteCBFunc( DWORD we )
{
	if( cbWindowFunc )
		(*cbWindowFunc)(m_ID, m_pParent, we);

}

void cWindow::Render()
{
	if( m_bActive )
	if( m_pCurImage )
	{
		DWORD alpha = m_alpha * m_dwOptionAlpha / 100;
		DWORD dwRGBA = RGBA_MERGE( m_dwImageRGB, alpha );
		m_pCurImage->RenderSprite( &m_scale, NULL, 0, &m_absPos, dwRGBA );
		ToolTipRender();
	}
}

void cWindow::ToolTipRender()
{
	if(toolTip.IsValid() && m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
	{
		if(toolTip.GetItemDBIdx()!=0)
		{
			if(GAMEIN->GetSendItemInfoDlg())
			{//[物品信息发送][BY:十里坡剑神][QQ:112582793][2019-4-12][15:41:15]
				if(GAMEIN->GetSendItemInfoDlg()->IsActive())
					GAMEIN->GetSendItemInfoDlg()->SetActive(FALSE);
			}
		}
		WINDOWMGR->SetToolTipWindow( &toolTip );
		toolTip.SetXY(m_ttX+TOOLTIP_MOUSESIZE_X, m_ttY+TOOLTIP_MOUSESIZE_Y);
	}
	if(m_pRelationTip)
	{//[装备对比][BY:十里坡剑神][QQ:112582793][2019-4-12][15:41:02]
		if(m_pRelationTip->IsValid()&&m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
		{
			WINDOWMGR->SetRelationTipWindow(m_pRelationTip);
			m_pRelationTip->SetXY(m_ttX-301, m_ttY+TOOLTIP_MOUSESIZE_Y);
			m_pRelationTip->SetShowCurItem(TRUE);
		}
	}
	if(m_pRelationTip1)
	{//[装备对比][BY:十里坡剑神][QQ:112582793][2019-4-12][15:41:02]
		if(m_pRelationTip1->IsValid()&&m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
		{
			WINDOWMGR->SetRelationTipWindow1(m_pRelationTip1);
			m_pRelationTip1->SetXY(m_ttX-301, m_ttY-m_pRelationTip1->GetHight());
			m_pRelationTip1->SetShowCurItem(TRUE);
		}
	}
}

void cWindow::SetToolTip( char* msg, DWORD color, cImage * image, DWORD imgColor )
{ 
	if( msg )
	{
		toolTip.Init( 0, color, image, imgColor );
		toolTip.SetImageAlpha( TOOLTIP_ALPHA );	

		toolTip = msg;
	}
}

void cWindow::AddImageTop(cImage * image)
{
	toolTip.AddImageTop(image);
}
void cWindow::AddImagTopLine(cImage* image,WORD Index)
{
	toolTip.AddImageTopLine(image,Index);
}
WORD cWindow::GetTipLineCount()
{
	return toolTip.GetLineCount();
}
void cWindow::IniTipLineCount()
{
	toolTip.ClearLineCount();
}
void cWindow::AddToolTipLine( char* msg, DWORD color )
{
	toolTip.AddLine( msg, color );
}

void cWindow::AddToolTipImageEquip(cImage * image)
{
	toolTip.AddImageEquip(image);
}
 void cWindow::SetEquipItem()
 {
	toolTip.SetEquipItem(TRUE);
 }



