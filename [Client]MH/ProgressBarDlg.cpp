
// TitanRecallDlg.cpp: implementation of the CTitanPartsChangeDlg class.

//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ProgressBarDlg.h"

#include "Hero.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "./ObjectGuagen.h"

CProgressBarDlg::CProgressBarDlg()
{
	m_pProgressGuagen = NULL;

	m_pRemaintimeStatic = NULL;

	m_bProgressStart = FALSE;
	m_bSuccessProgress = FALSE;
	m_dwProcessTime = 0;
	m_dwCurrentTime = 0;
	m_dwSuccessTime = 0;
}

CProgressBarDlg::~CProgressBarDlg()

{
	m_pProgressGuagen = NULL;
	m_pRemaintimeStatic = NULL;
}

void CProgressBarDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );



	if( val == FALSE )

	{
		m_bProgressStart = FALSE;
		m_dwProcessTime = 0;
		m_dwCurrentTime = 0;		
		m_pProgressGuagen->SetValue( 0, 0 ); //������ �ʱ�ȭ		
	}
}

void CProgressBarDlg::Process()
{
	if( !m_bProgressStart )
	{
		return;
	}

	m_dwCurrentTime = gCurTime; //����ð� ����


	float fGageValue = 1.0f - ( ((float)m_dwProcessTime-(float)m_dwCurrentTime) /(float)m_dwSuccessTime);
	m_pProgressGuagen->SetValue( (GUAGEVAL)fGageValue, m_dwCurrentTime );

	if(m_dwProcessTime < m_dwCurrentTime)
	{
		m_bSuccessProgress = TRUE;		
	}	


	char buf[128];
	sprintf( buf, CHATMGR->GetChatMsg(1043), (m_dwProcessTime-m_dwCurrentTime+1000)/1000 ); //���� �ð����� ī��Ʈ �ҷ��� 1000�� �������.
	m_pRemaintimeStatic->SetStaticText( buf );
}

void CProgressBarDlg::StartProgress()
{
	InitProgress();
	m_bProgressStart= TRUE;		
	m_dwCurrentTime = gCurTime;	//����ð� ����

	m_dwProcessTime = m_dwCurrentTime+m_dwSuccessTime;	//����ð� + ī���� �ð�




	SetActive( TRUE );
}

void CProgressBarDlg::InitProgress()
{
	m_bProgressStart = FALSE;

	m_bSuccessProgress = FALSE;
	m_dwProcessTime = 0;


	m_dwCurrentTime = 0;

	SetActive( FALSE );

}

BOOL CProgressBarDlg::GetSuccessProgress()

{
	return m_bSuccessProgress;
}

void CProgressBarDlg::SetSuccessProgress(BOOL bVal)
{
	m_bSuccessProgress = bVal;
}

void CProgressBarDlg::SetSuccessTime(DWORD dwTime)
{
	m_dwSuccessTime = dwTime;
}

void CProgressBarDlg::Render()
{	
	Process();
	cDialog::Render();	
}




