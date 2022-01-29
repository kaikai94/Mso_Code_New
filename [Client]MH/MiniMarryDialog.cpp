#include "stdafx.h"
#include "MiniMarryDialog.h"
#include "WindowIDEnum.h"
#include "cImeEx.h"
#include "cChangeCharCode.h"
CMiniMaryDialog::CMiniMaryDialog()
{
}
CMiniMaryDialog::~CMiniMaryDialog()
{
}
void CMiniMaryDialog::Linking()
{
	m_pName = (cStatic*)GetWindowForID(MAR_NAME);
	m_pNameEdit = (cEditBox*)GetWindowForID(MAR_NAMEEDIT);
	m_pNameEdit->SetValidCheck(VCM_CHARNAME);
	m_pNameEdit->SetEditText("");
	m_pAddOkBtn = (cButton*)GetWindowForID(MAR_ADDOKBTN);
	m_pAddCancelBtn = (cButton*)GetWindowForID(MAR_ADDCANCELBTN);
}
void CMiniMaryDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	if(val)
		m_pNameEdit->SetEditText("");
	cDialog::SetActiveRecursive(val);
}
void CMiniMaryDialog::SetName(char* Name)
{
#ifdef _BIG5SYSTEM_
	if (strlen(Name)>=2)
	{
		CODEMGR->ChangeCharCode(Name);
	}
#endif
	m_pNameEdit->SetEditText(Name);
}