#pragma once
#include "cDialog.h"
class cTextArea;

class MaintainaceNoteDlg:public cDialog
{
	cTextArea * m_Note;
public:
	MaintainaceNoteDlg(void);
	~MaintainaceNoteDlg(void);
	void Linking();
	void SetNoteMsg(char * msg);
};
