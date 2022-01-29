#include "stdafx.h"
#include "cStatusIconBtnDlg.h"
#include "./interface/cWindowManager.h"
#include "./interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "../[CC]Header/CommonDefine.h"
#include "ChatManager.h"
#include "cPushupButton.h"
#include "StatusIconDlg.h"
#include "VideoCaptureManager.h"
#include "GameResourceManager.h"
cStatusIconBtnDlg::cStatusIconBtnDlg(void)
{
	pShowStatuIcoBtn = NULL;
	//[录像功能按钮][By:十里坡剑神][QQ:112582793][2019/3/16][21:09:14]
	m_pVideoCaptureStartBtn = NULL;
	m_pVideoCaptureStopBtn = NULL;
}

cStatusIconBtnDlg::~cStatusIconBtnDlg(void)
{
}
void cStatusIconBtnDlg::Linking()
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );
	pShowStatuIcoBtn = (cPushupButton *)GetWindowForID(STATU_ICON_BTN_OPEN);
	pShowStatuIcoBtn->SetPush(!STATUSICONDLG->IsShow());
	//[录像功能添加][By:十里坡剑神][QQ:112582793][2019/3/16][21:08:33]
	m_pVideoCaptureStartBtn = (cButton*)GetWindowForID( MNM_VIDEO_REC_START_BTN );
	m_pVideoCaptureStopBtn  = (cButton*)GetWindowForID( MNM_VIDEO_REC_STOP_BTN );
	float x = (float)GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth-210;
	m_pVideoCaptureStartBtn->SetAbsX(x);
	m_pVideoCaptureStopBtn->SetAbsX(x);
	ShowVideoCaptureStartBtn(!VIDEOCAPTUREMGR->IsCapturing());
}
void cStatusIconBtnDlg::ShowVideoCaptureStartBtn( BOOL bShow )
{//[录像按钮状态切换][By:十里坡剑神][QQ:112582793][2019/3/16][21:09:01]
	m_pVideoCaptureStartBtn->SetActive( bShow );
	m_pVideoCaptureStopBtn->SetActive( !bShow );
}



