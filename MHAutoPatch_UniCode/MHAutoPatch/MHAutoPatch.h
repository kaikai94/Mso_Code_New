
// MHAutoPatch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMHAutoPatchApp: 
// �йش����ʵ�֣������ MHAutoPatch.cpp
//

class CMHAutoPatchApp : public CWinApp
{
public:
	CMHAutoPatchApp();
	~CMHAutoPatchApp();
	
// ��д
public:
	virtual BOOL InitInstance();
	

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMHAutoPatchApp theApp;