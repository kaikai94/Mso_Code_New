#include "stdafx.h"
#include "MaintainaceNoteDlg.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "cTextArea.h"
MaintainaceNoteDlg::MaintainaceNoteDlg(void)
{
	m_Note = NULL;
}

MaintainaceNoteDlg::~MaintainaceNoteDlg(void)
{
}
void MaintainaceNoteDlg::Linking()
{
	m_Note = (cTextArea*)GetWindowForID(MAINTAINACE_STATIC);
	m_Note->SetLineHight(25);
	m_Note->SetEnterAllow(FALSE);
	//char buf[256]={0,};
	//sprintf(buf,"������ά���У�Ԥ�ƿ���ʱ��2018��6��13�ա�ά����Ϻ󽫻��Զ����º�������������������ĵȴ���");
	m_Note->SetScriptText("");
}
void MaintainaceNoteDlg::SetNoteMsg(char * msg)
{
	m_Note->SetScriptText(msg);
}
