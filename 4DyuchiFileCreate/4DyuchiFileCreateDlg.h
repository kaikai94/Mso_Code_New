// 4DyuchiFileCreateDlg.h : 头文件
//

#pragma once

#include "../4DyuchiGRX_Common/IFileStorage.h"


#define MAX_FILE_ATOM_NUM	25000


// CDyuchiFileCreateDlg 对话框
class CDyuchiFileCreateDlg : public CDialog
{

	char pPakFileName[256];

	CEdit  *  m_EditPath;

	CStatic * m_Msg;

	CProgressCtrl * m_Progress;

	I4DyuchiFileStorage*	m_pStorage;

	CListBox  * m_ListFile;

	FSFILE_ATOM_INFO*		m_pFileInfoList;

	void*	m_pPackFileHandle;   //liuwei Pak文件句柄


// 构造
public:
	CDyuchiFileCreateDlg(CWnd* pParent = NULL);	// 标准构造函数

	DWORD	UnlockProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);

// 对话框数据
	enum { IDD = IDD_DYUCHIFILECREATE_DIALOG };

	protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	HANDLE   CreatePakFile(char * szPackFileName);
 
	void     BrowseCurrentAllFile(CString strDir);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrown();
	afx_msg void OnBnClickedLook();
	afx_msg void OnBnClickedExtra();
	afx_msg void OnBnClickedZip();
};
