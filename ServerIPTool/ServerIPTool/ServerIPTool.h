
// ServerIPTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServerIPToolApp: 
// �йش����ʵ�֣������ ServerIPTool.cpp
//

class CServerIPToolApp : public CWinApp
{
public:
	CServerIPToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerIPToolApp theApp;