#include "stdafx.h"
#include "cFindPatnIcon.h"
#include "./Interface/cScriptManager.h"
CFindPatnIcon::CFindPatnIcon(void)
{
	m_bShowFindPathMark = FALSE;
	m_Image = NULL;
	m_Image = new cImage;
	SCRIPTMGR->GetImage(40,m_Image,PFT_MINIMAPPATH);
	m_ImageWH.x = 13;
	m_ImageWH.y = 18;
	nMouseX = -1;
	nMouseY = -1;
}

CFindPatnIcon::~CFindPatnIcon(void)
{
	if(m_Image)
	{
		delete m_Image;
		m_Image = NULL;
	}
}

void CFindPatnIcon::Render()
{
	if(!m_bShowFindPathMark) return;

	VECTOR2 trans;



	static DWORD dwTime = gCurTime;
	static DWORD dwSpecialTime = gCurTime;

	static int n = 0;
	//	static int nSpecial = 0;
	VECTOR2 pv2Scaling;
	pv2Scaling.x = 1.0f;
	pv2Scaling.y = 1.0f;
	
	if( m_Image )
	{
		trans.x = nMouseX-m_ImageWH.x*0.5;
		trans.y = nMouseY - m_ImageWH.y;
		m_Image->RenderSprite( &pv2Scaling, NULL, 0.0f, &trans, 0xeeffffff );	
	}


	if( gCurTime - dwTime > 220 )
	{
		n = (n+1)%2;
		if(n==0)
			m_ImageWH.y-=3;
		else if(n==1)
			m_ImageWH.y+=3;
		dwTime = gCurTime;
	}
}

