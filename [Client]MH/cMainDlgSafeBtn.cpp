#include "stdafx.h"
#include "cMainDlgSafeBtn.h"
#include "WindowIDEnum.h"
#include "./interface/cWindowManager.h"
#include "cPushupButton.h"
cMainDlgSafeBtn::cMainDlgSafeBtn(void)
{
	m_pBtnClose = NULL;
	m_pBtnOpen = NULL;
	m_bIsSafeLockOpen = FALSE;
}

cMainDlgSafeBtn::~cMainDlgSafeBtn(void)
{
}

void cMainDlgSafeBtn::Linking()
{
	m_pBtnClose = (cPushupButton*)GetWindowForID(MAINDLGSAFE_BTN);
	m_pBtnOpen  = (cPushupButton*)GetWindowForID(MAINDLGSAFE_BTN2);
	if(m_bIsSafeLockOpen)
		Show(TRUE);
	else
		Show(FALSE);
}

void cMainDlgSafeBtn::Show(BOOL val)
{
	if(val)
	{
		m_pBtnOpen->SetActive(TRUE);
		m_pBtnClose->SetActive(FALSE);
	}
	else
	{
		m_pBtnOpen->SetActive(FALSE);
		m_pBtnClose->SetActive(TRUE);
	}
}