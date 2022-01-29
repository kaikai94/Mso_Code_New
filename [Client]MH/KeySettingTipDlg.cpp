









#include "stdafx.h"
#include "KeySettingTipDlg.h"
#include "GameResourceManager.h"

CKeySettingTipDlg::CKeySettingTipDlg(void)
{
	m_wMode = 2;

}

CKeySettingTipDlg::~CKeySettingTipDlg(void)
{


}



void CKeySettingTipDlg::Linking()


{
	RECT rt;

	rt.top = rt.left = 0;
	rt.right = GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth;  

	rt.bottom = GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight; 

//	m_KeyImage[0].LoadSprite("Image/KeySetting1.tga");
	m_KeyImage[0].LoadSprite("Image/2D/KeySetting1.tga");
	m_KeyImage[0].SetImageSrcRect(&rt);
//	m_KeyImage[1].LoadSprite("Image/KeySetting2.tga");


	m_KeyImage[1].LoadSprite("Image/2D/KeySetting2.tga");
	m_KeyImage[1].SetImageSrcRect(&rt);

}


void CKeySettingTipDlg::Render()
{
	if( !m_bActive ) return;
	if(m_wMode > 1) return;

	cDialog::RenderWindow();


	DWORD color = RGBA_MAKE(255,255,255,255);


	VECTOR2 vScale = { 1.f, (float)(GET_MAINWIN_H/GET_MAINWIN_W)}; 

	m_KeyImage[m_wMode].RenderSprite( &vScale, NULL, 0, &m_absPos, color );


	cDialog::RenderComponent();
}





