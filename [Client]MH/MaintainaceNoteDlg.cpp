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
	//sprintf(buf,"服务器维护中，预计开放时间2018年6月13日。维护完毕后将会自动更新后进行重连操作，请耐心等待！");
	m_Note->SetScriptText("");
}
void MaintainaceNoteDlg::SetNoteMsg(char * msg)
{
	m_Note->SetScriptText(msg);
}
