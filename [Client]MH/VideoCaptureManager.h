#pragma once

#include "bandicap.h"
#define VIDEOCAPTUREMGR		CVideoCaptureManager::GetInstance()

class cDialog;
class CVideoCaptureManager
{
	CBandiCaptureLibrary	 m_bandiCaptureLibrary;
public:
	CVideoCaptureManager();
	~CVideoCaptureManager();

	GETINSTANCE( CVideoCaptureManager );

	BOOL IsCapturing();

	BOOL CaptureStart();
	void CaptureStop();

	void Process();

	void SetCaptureOpt();
};