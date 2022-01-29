








// cIconGridDialog.cpp: implementation of the cIconGridDialog class.

//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIconGridDialog.h"
#include "cWindowManager.h"
#include "cIcon.h"
#include "../Input/Mouse.h"
#include "cScriptManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cIconGridDialog::cIconGridDialog()
{
	m_nRow=m_nCol=0;

	m_IconDlgID=0;
	m_wCellBorderX=0;
	m_wCellBorderY=0;
	m_wCellWidth=0;
	m_wCellHeight=0;
	m_acceptableIconType=0xffffffff;			//all accept
	m_type = WT_ICONGRIDDIALOG;
	SetRect(&m_cellRect,0,0,0,0);
	m_lCurSelCellPos = -1;
	m_lCurDragOverPos = -1;
	m_nIconType	= WT_ICON;
	m_bItemDraged = FALSE;

	m_pIconGridCell = NULL;
	m_bShowGrid = FALSE;

//#ifdef _JAPAN_LOCAL_
//	m_DisableFromPos	= 0;
//	m_DisableToPos		= 0;
//#endif	// _JAPAN_LOCAL
//
//#ifdef _HK_LOCAL_
//	m_DisableFromPos	= 0;
//	m_DisableToPos		= 0;
//#endif	// _HK_LOCAL
//
//#ifdef _TL_LOCAL_
//	m_DisableFromPos	= 0;
//	m_DisableToPos		= 0;
//#endif	// _HK_LOCAL
}

cIconGridDialog::~cIconGridDialog()
{
	SAFE_DELETE_ARRAY(m_pIconGridCell);
}

void cIconGridDialog::InitDialog(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, WORD col, WORD row, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_nRow = row;
	m_nCol = col;

	m_pIconGridCell = new cIconGridCell[m_nRow*m_nCol];
	for(int i = 0 ; i < m_nRow*m_nCol ; i++)
	{
		m_pIconGridCell[i].icon = NULL;
		m_pIconGridCell[i].use = NOTUSE;
	}
	

	
	m_wCellBorderX = DEFAULT_CELLBORDER;
	m_wCellBorderY = DEFAULT_CELLBORDER;
	m_wCellWidth =   DEFAULT_CELLSIZE;
	m_wCellHeight =  DEFAULT_CELLSIZE;
	
	SCRIPTMGR->GetImage( 137, &m_GridLockImage, PFT_HARDPATH );
}
void cIconGridDialog::InitGrid(LONG grid_x, LONG grid_y, WORD cellWid, WORD cellHei, WORD borderX, WORD borderY)
{
	m_gridPos.x = (float)grid_x;
	m_gridPos.y = (float)grid_y;
	m_wCellWidth = cellWid;

	m_wCellHeight = cellHei;
	m_wCellBorderX = borderX;
	m_wCellBorderY = borderY;

	m_cellRect.left = (LONG)m_gridPos.x;
	m_cellRect.right = m_cellRect.left + m_nCol*m_wCellWidth+m_wCellBorderX*(m_nCol+1);
	m_cellRect.top = (LONG)m_gridPos.y;
	m_cellRect.bottom = m_cellRect.top + m_nRow*m_wCellHeight+m_wCellBorderY*(m_nRow+1);
}

BOOL cIconGridDialog::PtInCell(LONG x, LONG y)
{
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
		{
			if(m_pIconGridCell[i].icon->PtInWindow(x,y))
				return TRUE;
		}
	return FALSE;
}

void cIconGridDialog::SetCellRect(RECT * rect)

{
	CopyRect(&m_cellRect, rect); 
}

void cIconGridDialog::SetAbsXY(LONG x, LONG y)
{
	LONG tmpX = x - (LONG)m_absPos.x;
	LONG tmpY = y - (LONG)m_absPos.y;

	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
		{
			m_pIconGridCell[i].icon->SetAbsXY(tmpX+(LONG)m_pIconGridCell[i].icon->GetAbsX(), 
											  tmpY+(LONG)m_pIconGridCell[i].icon->GetAbsY());
		}
	cDialog::SetAbsXY(x,y);

}

DWORD cIconGridDialog::ActionEvent(CMouse * mouseInfo)
{
	if(!m_bActive) return WE_NULL;
	DWORD we = WE_NULL;
	DWORD we2 = WE_NULL;
	BOOL selCell = FALSE;
	POSTYPE pos;

	we = cDialog::ActionEvent(mouseInfo);
	
	if( !m_bDisable )
	{
		if(we&WE_RBTNCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_RBTNCLICK);
			}
			return we;
		}
		if(we & WE_LBTNCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNCLICK);	//PPP
			}
		}
		else if(we & WE_LBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, m_pParent, WE_LBTNDBLCLICK);	//PPP
			}
		}
		else if(we & WE_RBTNDBLCLICK)
		{
			if(GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos))
			{
				m_lCurSelCellPos = pos;
				cbWindowFunc(m_ID, this, WE_RBTNDBLCLICK);			
			}
		}
		else
		{
			if( mouseInfo->LButtonDrag() )
			{
				if( ( we & WE_MOUSEOVER ) && IsDragOverDraw()
					&& GetPositionForXYRef(mouseInfo->GetMouseX(),mouseInfo->GetMouseY(),pos) )
				{
					m_lCurDragOverPos = pos;
					m_bItemDraged = TRUE;
				}
				else
				{
					m_lCurDragOverPos = -1;
					m_bItemDraged = FALSE;
				}
			}
			else
			{
				if( m_bItemDraged )	
				{
					if( GetPositionForXYRef(mouseInfo->GetMouseEventX(),mouseInfo->GetMouseEventY(),pos) )
						m_lCurSelCellPos = pos;
					m_bItemDraged = FALSE;
				}
				m_lCurDragOverPos = -1;				
			}
		}
	}

	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
	{
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon->IsActive() && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->ActionEvent(mouseInfo);
	}

	return we;
}


void cIconGridDialog::Render()
{
	cDialog::Render();

	VECTOR2 start_pos;
	if(m_lCurSelCellPos != -1 && !m_SelectedBGImage.IsNull())
	{
		start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(m_lCurSelCellPos%m_nCol+1))+(m_lCurSelCellPos%m_nCol*m_wCellWidth)-2;		// 窍靛 内爹 -2(taiyo temp)
		start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(m_lCurSelCellPos/m_nCol+1))+(m_lCurSelCellPos/m_nCol*m_wCellHeight)-2;		// 窍靛 内爹 -2(taiyo temp)
		m_SelectedBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));

	}

	if(m_lCurDragOverPos != -1 && !m_DragOverBGImage.IsNull())
//	if(IsDragOverDraw())
	{
		start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(m_lCurDragOverPos%m_nCol+1))+(m_lCurDragOverPos%m_nCol*m_wCellWidth)-2;		// 窍靛 内爹 -2(taiyo temp)

		start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(m_lCurDragOverPos/m_nCol+1))+(m_lCurDragOverPos/m_nCol*m_wCellHeight)-2;		// 窍靛 内爹 -2(taiyo temp)
		m_DragOverBGImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}

	// inverse order for tooltip
	for(int i = m_nCol*m_nRow-1 ; i >= 0  ; --i)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->Render();


//#ifdef _JAPAN_LOCAL_
//
//	for(i=m_DisableFromPos; i<m_DisableToPos; ++i)
//	{
//		if( !m_GridLockImage.IsNull() )
//		{
//			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
//			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;
//			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
//		}
//	}
//
//#endif	// _JAPAN_LOCAL_
//
//#ifdef _HK_LOCAL_
//
//	for(i=m_DisableFromPos; i<m_DisableToPos; ++i)
//	{
//		if( !m_GridLockImage.IsNull() )
//		{
//			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
//			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;
//			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
//		}
//	}
//
//#endif	// _HK_LOCAL_
//
//
//#ifdef _TL_LOCAL_
//
//	for(i=m_DisableFromPos; i<m_DisableToPos; ++i)
//	{
//		if( !m_GridLockImage.IsNull() )
//		{
//			start_pos.x = m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i%m_nCol+1))+(i%m_nCol*m_wCellWidth)-2;
//			start_pos.y = m_absPos.y+m_cellRect.top+(m_wCellBorderY*(i/m_nCol+1))+(i/m_nCol*m_wCellHeight)-2;
//
//			m_GridLockImage.RenderSprite(NULL,NULL,0,&start_pos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
//		}
//	}
//
//#endif	// _HK_LOCAL_
}

void cIconGridDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	

	cDialog::SetActiveRecursive(val);

/// need confirm
	m_lCurSelCellPos = -1;
///
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon && m_pIconGridCell[i].icon->IsDepend())
			m_pIconGridCell[i].icon->SetActive(val);
}


void cIconGridDialog::SetAlpha(BYTE al)
{ 
	cDialog::SetAlpha(al);
	for(int i = 0 ; i < m_nCol*m_nRow ; i++)
	{
		if(m_pIconGridCell[i].use == USE && m_pIconGridCell[i].icon)
			m_pIconGridCell[i].icon->SetAlpha(al);
	}
}
/**/
/**/
/**/
/**/
// 1-dimension
BOOL cIconGridDialog::IsAddable(WORD pos)
{
	ASSERT(pos < m_nCol*m_nRow);
	if(pos>m_nCol*m_nRow)
		return FALSE;
	if(m_pIconGridCell[pos].use == USE)
		return FALSE;
	return TRUE;
}
BOOL cIconGridDialog::AddIcon(WORD pos, cIcon * icon)
{
	ASSERT(pos < m_nCol*m_nRow);
	WORD cellX = pos%m_nCol;
	WORD cellY = pos/m_nCol;

	if(m_pIconGridCell[pos].use == USE) return FALSE;
	icon->SetAbsXY((LONG)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(cellX+1))+((cellX)*m_wCellWidth)), (LONG)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(cellY+1))+((cellY)*m_wCellHeight)));
	icon->SetDepend(TRUE);
	icon->SetActive(m_bActive);
	m_pIconGridCell[pos].icon = icon;
	

	m_pIconGridCell[pos].use = USE;
	

	icon->SetParent(this);

	if( m_bDisable )
		icon->SetDisable( TRUE );

	icon->SetCellPosition(cellX,cellY);
	return TRUE;
}

BOOL cIconGridDialog::GetCellAbsPos(WORD pos, int& absX, int& absY)
{
	ASSERT(pos < m_nCol*m_nRow);
	WORD cellX = pos%m_nCol;

	WORD cellY = pos/m_nCol;

	if(m_pIconGridCell[pos].use == NOTUSE) return FALSE;

	absX = (int)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(cellX+1))+((cellX)*m_wCellWidth));
	absY = (int)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(cellY+1))+((cellY)*m_wCellHeight));

	return TRUE;
}

BOOL cIconGridDialog::GetCellPosition(LONG mouseX, LONG mouseY, WORD& cellX,WORD& cellY)
{

	for(int i = 0 ; i < m_nCol ; i++)
	{
		for(int j = 0 ; j < m_nRow ; j++)
		{
			int cellpX = (int)(m_absPos.x+m_cellRect.left+(m_wCellBorderX*(i+1))+(i*m_wCellWidth));
			int cellpY = (int)(m_absPos.y+m_cellRect.top+(m_wCellBorderY*(j+1))+(j*m_wCellHeight));

			//此处代码有bug. 编写表情系统时发现,若 iconGridDialog  #INITGRID 尺寸是默认的40*40 ,则获取到的Pos位置有偏差! DEFAULT_CELLSIZE 替换为 m_wCellWidth ,m_wCellHeight
			if(cellpX < mouseX && mouseX < cellpX + m_wCellWidth && cellpY < mouseY && mouseY < cellpY + m_wCellHeight)
			{
				cellX = i;
				cellY = j;
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

BOOL cIconGridDialog::GetPositionForXYRef(LONG mouseX, LONG mouseY, WORD& pos)
{
	

	WORD x,y;
	BOOL rt = GetCellPosition(mouseX, mouseY, x, y);
	pos = y*m_nCol+x;	//tmp) pos = GetPositionForCell(x,y);
	return rt;
}

WORD cIconGridDialog::GetPositionForCell(WORD cellX, WORD cellY)
{
	return (cellY)*m_nCol+(cellX);
}
BOOL cIconGridDialog::IsAddable(WORD cellX,WORD cellY,cIcon* pIcon)
{
	if(m_pIconGridCell[cellY*m_nCol+cellX].use == USE)
		return FALSE;
	if(!(pIcon->GetIconType() & m_acceptableIconType))

		return FALSE;

	return TRUE;
}

BOOL cIconGridDialog::AddIcon(WORD cellX, WORD cellY, cIcon * icon)
{
	return AddIcon(cellX+cellY*m_nCol, icon);
}

BOOL cIconGridDialog::MoveIcon(WORD cellX, WORD cellY, cIcon * icon)
{
//KES 040511 俊矾府畔 荐沥
	if( !IsAddable( GetPositionForCell( cellX, cellY ) ) )
		return FALSE;

	cIconGridDialog* pBeforeGrid = (cIconGridDialog*)icon->GetParent();

	WORD wOldCellX = icon->GetCellX();
	WORD wOldCellY = icon->GetCellY();

	if( pBeforeGrid->DeleteIcon(icon) == FALSE )
		return FALSE;

	if( AddIcon( cellX, cellY, icon ) )
	{
		return TRUE;
	}
	else
	{
		pBeforeGrid->AddIcon( wOldCellX, wOldCellY, icon );

		return FALSE;
	}
}

BOOL cIconGridDialog::DeleteIcon(cIcon * icon)
{
	WORD x = icon->GetCellX();
	WORD y = icon->GetCellY();
	
	cIcon * dummy;
	
	return DeleteIcon(x, y, &dummy);
}
BOOL cIconGridDialog::DeleteIcon(WORD pos, cIcon ** icon)
{
	ASSERT(pos < m_nCol*m_nRow);

	if(m_pIconGridCell[pos].use == NOTUSE)
	{
		if( icon ) 
		*icon = NULL;
		//ASSERT(0);
		return FALSE;
	}

	if( icon ) 
	*icon = m_pIconGridCell[pos].icon;
	m_pIconGridCell[pos].icon->SetCellPosition(0,0);

	m_pIconGridCell[pos].icon->SetRelationToolTip(NULL);
	m_pIconGridCell[pos].icon->SetRelationToolTip1(NULL);

	m_pIconGridCell[pos].icon->SetDepend(FALSE);
	m_pIconGridCell[pos].icon = NULL;
	m_pIconGridCell[pos].use = NOTUSE;


	return TRUE;
}


BOOL cIconGridDialog::DeleteIcon(WORD cellX, WORD cellY, cIcon ** iconOut)
{
	return DeleteIcon(cellY*m_nCol+cellX, iconOut);
}


void cIconGridDialog::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );

	for(int i = 0 ; i < m_nRow*m_nCol ; i++)
	{
		if( m_pIconGridCell[i].icon )
			m_pIconGridCell[i].icon->SetDisable( val );
	}	
}

BOOL cIconGridDialog::IsDragOverDraw()
{
	if( !WINDOWMGR->IsDragWindow() ) return FALSE;

	cDialog* pDlg = WINDOWMGR->GetDragDlg();

	if( pDlg )
	if( pDlg->GetType() == m_nIconType )
		return TRUE;

	return FALSE;
}




