// 4DyuchiFileCreateDlg.h : ͷ�ļ�
//

#pragma once

#include "../4DyuchiGRX_Common/IFileStorage.h"


#define MAX_FILE_ATOM_NUM	25000


// CDyuchiFileCreateDlg �Ի���
class CDyuchiFileCreateDlg : public CDialog
{

	char pPakFileName[256];

	CEdit  *  m_EditPath;

	CStatic * m_Msg;

	CProgressCtrl * m_Progress;

	I4DyuchiFileStorage*	m_pStorage;

	CListBox  * m_ListFile;

	FSFILE_ATOM_INFO*		m_pFileInfoList;

	void*	m_pPackFileHandle;   //liuwei Pak�ļ����


// ����
public:
	CDyuchiFileCreateDlg(CWnd* pParent = NULL);	// ��׼���캯��

	DWORD	UnlockProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);

// �Ի�������
	enum { IDD = IDD_DYUCHIFILECREATE_DIALOG };

	protected:

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	HANDLE   CreatePakFile(char * szPackFileName);
 
	void     BrowseCurrentAllFile(CString strDir);


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
