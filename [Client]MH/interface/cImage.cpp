// cImage.cpp: implementation of the cImage class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "cImage.h"
#include "cResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------------------------
// cImageBase class member

// ------------------------------------------------------------------------------------------
cImageBase::cImageBase()
{
	m_pSurface = NULL;
}
cImageBase::~cImageBase()
{
	//SAFE_RELEASE(m_pSurface);// ¸®¼Ò½º ¸Å´ÏÀú ¿¡¼­ 
}
cImage::cImage()
{
}

cImage::~cImage()
{
}

void cImage::RenderSprite(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,
						  VECTOR2* pv2Trans, DWORD dwColor)
{

	VECTOR2 sc;
	if(pv2Scaling)
	{

		sc.x = (float)(m_srcImageRect.right-m_srcImageRect.left)*pv2Scaling->x/m_srcImageSize.x;
		sc.y = (float)(m_srcImageRect.bottom-m_srcImageRect.top)*pv2Scaling->y/m_srcImageSize.y;
	}
	else
	{
		sc.x = (float)(m_srcImageRect.right-m_srcImageRect.left)/m_srcImageSize.x;
		sc.y = (float)(m_srcImageRect.bottom-m_srcImageRect.top)/m_srcImageSize.y;
	}

	
	cImageBase::RenderSprite( &sc, pv2RotCenter, fRot, pv2Trans, &m_srcImageRect, dwColor );
}




