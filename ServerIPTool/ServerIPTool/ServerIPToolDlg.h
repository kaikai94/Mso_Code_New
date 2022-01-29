
// ServerIPToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <fstream>
// CServerIPToolDlg �Ի���
class CServerIPToolDlg : public CDialogEx
{
	std::vector<SERVERINFO*> m_ServerList;
	int m_ServerSetNum;
	BOOL ReadText(char* pFileName);
	BOOL SaveText(char* pFileName);
	void clear();
	void Add(SERVERINFO* pInfo);
	BOOL Save(char* pFileName);
	BOOL Load(char* pFileName);
	void Show();
public:
	CServerIPToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERIPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CListCtrl*	m_SelChrList;
};
