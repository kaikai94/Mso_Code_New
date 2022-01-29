#pragma once
#include <stdio.h>
#include <vector>

using namespace std;

typedef struct _RESIZEINFO
{
	UINT	nID;
	CWnd*	pParent;
	float	fLeft;
	float	fRight;
	float	fTop;
	float	fBottom;
}RESIZEINFO;

class ItemResize 
{
	CRect m_rectDlg;
	vector<RESIZEINFO>m_vResizeID;
	public:
		ItemResize();
		~ItemResize();
	void AddItemRect(UINT nID,CWnd* pParent);

	void ResizeItem();

};


