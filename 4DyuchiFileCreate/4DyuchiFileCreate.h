// 4DyuchiFileCreate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CDyuchiFileCreateApp:
// �йش����ʵ�֣������ 4DyuchiFileCreate.cpp
//

class CDyuchiFileCreateApp : public CWinApp
{
public:
	CDyuchiFileCreateApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDyuchiFileCreateApp theApp;
