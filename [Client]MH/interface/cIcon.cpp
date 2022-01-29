









// cIcon.cpp: implementation of the cIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIcon.h"

#include "cWindowManager.h"
#include "cScriptManager.h"
#include "cButton.h"
#include "./Audio/MHAudioManager.h"

#include "ObjectManager.h"


#include "../Input/Mouse.h"
#include "cIconDialog.h"

#ifdef _GMTOOL_
#include "ChatManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cIcon::cIcon()
{
	m_IconType		= 1;
	m_dwData		= 0;
	m_type			= WT_ICON;

	m_bLock			= FALSE;


//	m_pLockImage	= NULL;
	m_TitanEnduranceFlag = FALSE;	// magi82 - Titan(070424)
	InitShowImage();

}


cIcon::~cIcon()
{
	PTRLISTSEARCHSTART(m_LinkDialogList,cIconDialog*,pDlg)
		pDlg->NotifyLinkIconDeleted(this);
	PTRLISTSEARCHEND

}


void cIcon::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	RECT capRect={0,0,wid,hei};
	SetCaptionRect(&capRect);
	
	SCRIPTMGR->GetImage( 65, &m_LockImage );

	SCRIPTMGR->GetImage( 65, &m_TitanEnduranceImage);
//#ifdef _GMTOOL_
//	CHATMGR->AddMsg(CTC_SYSMSG,"资源ID[%d]",ID);
//#endif
}


void cIcon::Render()
{
	if( !m_bActive )		return;
	
	if( m_bLock )
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
								RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
	cDialog::Render();
	if( m_TitanEnduranceFlag )
		m_TitanEnduranceImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
}

void cIcon::Render(VECTOR2* pPos)
{
	VECTOR2 backup = m_absPos;
	m_absPos = *pPos;
	Render();
	m_absPos = backup;
}


DWORD cIcon::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);
	if(m_bDisable) return we;
	we |= cDialog::ActionEventComponent(mouseInfo);

	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();


	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftPress = mouseInfo->LButtonPressed();
	BOOL click = mouseInfo->LButtonDown();


	if( m_pCloseBtn )
	{
		we = m_pCloseBtn->ActionEvent(mouseInfo);

		if(we & WE_BTNCLICK)
		{
			cbWindowFunc(m_ID, this, WE_CLOSEWINDOW);
			cWindowManager::OnWindowEvent(m_ID, m_pParent, WE_CLOSEWINDOW);

			return (we |= WE_CLOSEWINDOW);
		}

		if( m_pCloseBtn->IsClickInside() )
			return we;
	}

	if( mouseInfo->LButtonDown() && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{
			if( m_bMovable && PtInCaption( x2, y2 ) )
				m_bCaptionHit = TRUE;
			
			we |= WE_TOPWINDOW;

		}

	}

	if( mouseInfo->LButtonUp() )
		m_bCaptionHit = FALSE;

	if( m_bCaptionHit )
	{
		if( mouseInfo->LButtonDrag() )
		{
			if(WINDOWMGR->m_OldDragFlag == FALSE)
			{					
				SetDepend(FALSE);
				
				float X = (float)x2;
				float Y = (float)y2;

				LONG OrigX = (LONG)m_absPos.x;
				LONG OrigY = (LONG)m_absPos.y;
				
				WINDOWMGR->SetDragStart(OrigX, OrigY, DEFAULT_ICONSIZE/2, DEFAULT_ICONSIZE/2, m_ID);
				

				m_bCaptionHit = FALSE;
				
				AUDIOMGR->Play(57, HERO);

			}
		}
	}
	return we;
}


void cIcon::SetLock( BOOL bLock )
{
	m_bLock = bLock;


	SetMovable( !bLock );
}

void cIcon::AddLinkIconDialog(cIconDialog* pDialog)
{
	m_LinkDialogList.AddTail(pDialog);
}
void cIcon::RemoveLinkIconDialog(cIconDialog* pDialog)

{
	m_LinkDialogList.Remove(pDialog);
}




void cIcon::InitShowImage()
{
	WORD t_wImageIdx = 222;
	for(int i = 0;i<eShowKind_Max;i++)
	{
		SCRIPTMGR->GetImage(t_wImageIdx+i,&m_ItemShowKind[i]);
	}
}
BOOL cIcon::IsMixItem(std::string ItemName)
{
	int t_iFlag = ItemName.find("+");
	return t_iFlag!=-1;
}
BOOL cIcon::GetShowImage(const char* ItemName)
{
	std::string t_strName = ItemName;
	int t_iFlag = t_strName.find("真|");
	int t_iFlag1 = t_strName.find("魂|");
	int t_iFlag2 = t_strName.find("强|");
	int t_iFlag3 = t_strName.find("魔|");
	int t_iFlag4 = t_strName.find("高阶原石");
	int t_iFlag5 = t_strName.find("高阶圣石");
	if(t_iFlag!=-1||t_iFlag1!=-1||t_iFlag2!=-1||t_iFlag3!=-1||t_iFlag4!=-1||t_iFlag5!=-1)
	{
		return TRUE;
	}
	else
		return FALSE;
}
eItemShowKind cIcon::GetShowImagAddNum(const char * ItemName)
{
	std::string t_strTemp = ItemName;
	t_strTemp = t_strTemp.substr(t_strTemp.length()-1,1);
	switch(*t_strTemp.c_str())
	{
	case '1':	return eShowKind_Add1;
	case '2':	return eShowKind_Add2;
	case '3':	return eShowKind_Add3;
	case '4':	return eShowKind_Add4;
	case '5':	return eShowKind_Add5;
	case '6':	return eShowKind_Add6;
	case '7':	return eShowKind_Add7;
	case '8':	return eShowKind_Add8;
	case '9':	return eShowKind_Add9;
	case '0':	return eShowKind_Add10;
	default:	return eShowKind_Max;
	}
}


