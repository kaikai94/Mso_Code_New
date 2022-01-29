#include "stdafx.h"
#include "GoldShopSubDialog.h"
#include "cPushupButton.h"
CGoldShopSubDialog::CGoldShopSubDialog(void)
{
	m_type = WT_GOLDSHOPSUB_DLG;
}

CGoldShopSubDialog::~CGoldShopSubDialog(void)
{
}

void CGoldShopSubDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG)
		AddTabSheet(curIdx2++, window);
	else 
		cTabDialog::Add(window);
}
