// UpDataLuch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUpDataLuchApp:
// �йش����ʵ�֣������ UpDataLuch.cpp
//

class CUpDataLuchApp : public CWinApp
{
public:
	CUpDataLuchApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUpDataLuchApp theApp;