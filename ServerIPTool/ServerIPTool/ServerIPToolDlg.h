
// ServerIPToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <vector>
#include <fstream>
// CServerIPToolDlg 对话框
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
	CServerIPToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVERIPTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
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
