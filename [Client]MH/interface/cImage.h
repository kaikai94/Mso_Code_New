




// cImage.h: interface for the cImage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _cIMAGE_H_
#define _cIMAGE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindowDef.h"
#include "../Engine/GraphicEngine.h"
#include "../4DyuchiGRX_common/IGeometry.h"


class cImageBase 
{
	friend class cImage;
public:
	cImageBase();

	virtual ~cImageBase();

	BOOL IsNull() const { return (m_pSurface==NULL); }
	void SetNull() {m_pSurface=NULL;}
	void SetSpriteObject(IDISpriteObject * obj)	{	m_pSurface = obj;	}
protected:
	inline void RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, RECT* pRect, DWORD dwColor )
	{		
		if(m_pSurface)
		{
			g_pExecutive->GetGeometry()->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans,pRect,dwColor,0,RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
	IDISpriteObject	*		m_pSurface;
};

class cImage : public cImageBase
{
public:	
	cImage();

	virtual ~cImage();
	void RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, DWORD dwColor );

	void SetImageSrcSize(const cImageSize * sz)	{	memcpy( &m_srcImageSize, sz, sizeof(cImageSize) );	}
	void SetImageSrcRect(const cImageRect * rt)	{	memcpy( &m_srcImageRect, rt, sizeof(cImageRect) );	}
	
	cImageRect* GetImageRect()	{	return &m_srcImageRect;	}
	cImageSize* GetImageSrcSize() { return &m_srcImageSize; }
protected:
	cImageRect m_srcImageRect;
	cImageSize m_srcImageSize;
};

#endif // _cIMAGE_H_