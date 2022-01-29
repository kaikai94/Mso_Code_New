#include "stdafx.h"
#include "VideoCaptureDlg.h"
#include "WindowIDEnum.h"
#include "cComboBox.h"
#include "cCheckBox.h"
#include "./Interface/cResourceManager.h"

CVideoCaptureDlg::CVideoCaptureDlg()
{
}

CVideoCaptureDlg::~CVideoCaptureDlg()
{
}

void CVideoCaptureDlg::Linking()
{
	m_pSizeCombo			= (cComboBox*)GetWindowForID( VIDEOCAPTURE_SIZE_COMBO );
	m_pOptCombo				= (cComboBox*)GetWindowForID( VIDEOCAPTURE_OPT_COMBO );
	m_pNoCursor				= (cCheckBox*)GetWindowForID( VIDEOCAPTURE_NO_CURSOR_CHECKBOX );

	m_pSizeCombo->RemoveAll();
	m_pOptCombo->RemoveAll();

	for( int i=0 ; i<eVideoCaptureSize_Max ; ++i )
	{
		ITEM* pItem = new ITEM;
		ZeroMemory( pItem, sizeof(ITEM) );
		SafeStrCpy( pItem->string, GetTextVideoCaptureSize( (eVideoCaptureSize)i ), MAX_LISTITEM_SIZE );
		pItem->rgb = RGB(255, 255, 255);

		m_pSizeCombo->AddItem( pItem );
	}

	for( int i=0 ; i<eVideoCaptureOpt_Max ; ++i )
	{
		ITEM* pItem = new ITEM;
		ZeroMemory( pItem, sizeof(ITEM) );
		SafeStrCpy( pItem->string, GetTextVideoCaptureOpt( (eVideoCaptureOpt)i ), MAX_LISTITEM_SIZE );
		pItem->rgb = RGB(255, 255, 255);

		m_pOptCombo->AddItem( pItem );
	}

	m_pSizeCombo->SelectComboText( 0 );
	m_pOptCombo->SelectComboText( 0 );
}

const char* CVideoCaptureDlg::GetTextVideoCaptureSize( eVideoCaptureSize eSize )
{
	switch( eSize )
	{
	case eVideoCaptureSize_Default:
		return RESRCMGR->GetMsg( 1115 );	
	case eVideoCaptureSize_500x375:
		return RESRCMGR->GetMsg( 1116 );
	}

	return "";
}

const char* CVideoCaptureDlg::GetTextVideoCaptureOpt( eVideoCaptureOpt eOpt )
{
	switch( eOpt )
	{
	case eVideoCaptureOpt_High:
		return RESRCMGR->GetMsg( 1117 );	
	case eVideoCaptureOpt_Low:
		return RESRCMGR->GetMsg( 1118 );	
	}

	return "";
}

int CVideoCaptureDlg::GetSelecedSize()
{
	if(m_pSizeCombo->GetCurSelectedIdx()==-1)
		return 0;
	else
		return m_pSizeCombo->GetCurSelectedIdx();
}

int CVideoCaptureDlg::GetSelecedOpt()
{
	if(m_pOptCombo->GetCurSelectedIdx()==-1)
		return 0;
	else
		return m_pOptCombo->GetCurSelectedIdx();
}

BOOL CVideoCaptureDlg::IsIncludeCursor()
{
	return !m_pNoCursor->IsChecked();
}