// PatchMaker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPatchMakerApp:
// �йش����ʵ�֣������ PatchMaker.cpp
//

class CPatchMakerApp : public CWinApp
{
public:
	CPatchMakerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPatchMakerApp theApp;