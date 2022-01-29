#include "stdafx.h"
#include "ExitDialog.h"

#include "GameIn.h"

#include "MainBarDialog.h"

void CExitDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_EXITDLGICON, m_bActive );	

}





