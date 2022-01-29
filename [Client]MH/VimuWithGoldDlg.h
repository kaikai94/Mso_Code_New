#pragma once
#include "./interface/cDialog.h"
class VimuWithGoldDlg: public cDialog
{
	cStatic* m_pTipStatic;
public:
	VimuWithGoldDlg(void);
	~VimuWithGoldDlg(void);
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
};
