#include "stdafx.h"
#include "cTopRankDlg.h"
#include "./interface/cWindowManager.h"
#include "./interface/cButton.h"
#include "./interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "../[CC]Header/CommonDefine.h"
#include "BillBoardManager.h"
cTopRankDlg::cTopRankDlg(void)
{
	m_pCharTop = NULL;
	m_pGuildTop = NULL;
	m_pUniTop = NULL;
}

cTopRankDlg::~cTopRankDlg(void)
{
}
void cTopRankDlg::Linking()
{
	m_pCharTop	 = (cStatic *)GetWindowForID(BILL_TOP_NO1);
	m_pGuildTop  = (cStatic *)GetWindowForID(BILL_GUILD_TOP_NO1);
	m_pUniTop	 = (cStatic *)GetWindowForID(BILL_UNI_TOP_NO1);

	TOPLISTINFOHERO * HeroTop = NULL;
	HeroTop = sBOARDMGR->GetHeroTop();
	if(HeroTop)
		SetCharTopText(HeroTop->Name);

	TOPLISTINFOGUILD * GuildList = NULL;
	GuildList = sBOARDMGR->GetGuildTop();
	if (GuildList)
	{
		SetGuildTopText(GuildList->GuildName);
		SetUniTopText(GuildList->MasterName);
	}
}

void cTopRankDlg::SetCharTopText(char * buf)
{
	m_pCharTop->SetStaticText(buf);
}
void cTopRankDlg::SetGuildTopText(char * buf)
{
	m_pGuildTop->SetStaticText(buf);
}
void cTopRankDlg::SetUniTopText(char * buf)
{
	m_pUniTop->SetStaticText(buf);
}