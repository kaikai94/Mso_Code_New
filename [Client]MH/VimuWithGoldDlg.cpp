#include "stdafx.h"
#include "VimuWithGoldDlg.h"
#include "GameIn.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "VimuManager.h"
#include "./interface/cEditBox.h"
VimuWithGoldDlg::VimuWithGoldDlg(void)
{
	m_pTipStatic = NULL;
}

VimuWithGoldDlg::~VimuWithGoldDlg(void)
{
}
void VimuWithGoldDlg::Linking()
{
	m_pTipStatic = (cStatic*)GetWindowForID(VIMU_WITHGOLD_STATIC);
	m_pTipStatic->SetMultiLine(TRUE);
	m_pTipStatic->InitMultiLine();
	char line[512] = {0,};
	char szDescText[512] = {0,};
	wsprintf(line, CHATMGR->GetChatMsg(2349));
	strcat(szDescText, line);
	wsprintf(line, CHATMGR->GetChatMsg(2350));
	strcat(szDescText, line);
	wsprintf(line, CHATMGR->GetChatMsg(2351));
	strcat(szDescText, line);
	wsprintf(line, CHATMGR->GetChatMsg(2352));
	strcat(szDescText, line);
	m_pTipStatic->SetStaticText(szDescText);
}
void VimuWithGoldDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case VIMU_WITHGOLD_BTN_OK:
		{//[发送比武请求消息][BY:十里坡剑神][QQ:112582793][2019-4-21][0:32:12]
			DWORD dwGold = atoi(((cEditBox*)GetWindowForID(VIMU_WITHGOLD_EDIT))->GetEditText());
			if(dwGold>200000000||dwGold<10)
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2353));
				return;
			}
			if(HERO->GetGoldMoney()<dwGold)
			{
				CHATMGR->AddMsg(CTC_SYSMSG,CHATMGR->GetChatMsg(2354));
				return;
			}
			VIMUMGR->ApplyVimuWithGold(dwGold);
			cDialog::SetActive(FALSE);
		}
		break;
	//[取消][BY:十里坡剑神][QQ:112582793][2019-4-21][0:32:37]
	case VIMU_WITHGOLD_BTN_CANCAL:SetActive(FALSE);break;
	}
}
