#pragma once
#include "./interface/cDialog.h"

class cComboBox;
class cCheckBox;

class CVideoCaptureDlg : public cDialog
{
	cComboBox* m_pSizeCombo;	
	cComboBox* m_pOptCombo;		
	cCheckBox* m_pNoCursor;		
public:
	CVideoCaptureDlg();
	virtual ~CVideoCaptureDlg();

	enum eVideoCaptureSize
	{
		eVideoCaptureSize_Default,
		eVideoCaptureSize_500x375,
		eVideoCaptureSize_Max,
	};

	enum eVideoCaptureOpt
	{
		eVideoCaptureOpt_High,
		eVideoCaptureOpt_Low,
		eVideoCaptureOpt_Max,
	};

	void Linking();

	const char* GetTextVideoCaptureSize( eVideoCaptureSize eSize );			
	const char* GetTextVideoCaptureOpt( eVideoCaptureOpt eOpt );		

	int GetSelecedSize();												
	int GetSelecedOpt();													

	BOOL IsIncludeCursor();													
};