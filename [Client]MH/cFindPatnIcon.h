#pragma once

class CFindPatnIcon
{
	BOOL m_bShowFindPathMark;
	cImage * m_Image;
	VECTOR2 m_ImageWH;
	VECTOR2 m_ScreenPos;
	int nMouseX;
	int nMouseY;
public:
	CFindPatnIcon(void);
	~CFindPatnIcon(void);

	virtual void Render();

	void SetMousePoint(int x,int y)
	{
		nMouseX = x;
		nMouseY = y;
		m_bShowFindPathMark = TRUE;
	}
	void ClearPoint()
	{
		nMouseX=-1;
		nMouseY=-1;
		m_bShowFindPathMark = FALSE;
	}
};
