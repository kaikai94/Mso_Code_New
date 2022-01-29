#pragma once
#include <map>
#include<string>
#include "afxcmn.h"
#include "afxdtctl.h"
// cMapManager 对话框

class cMapManager : public CDialogEx
{
	DECLARE_DYNAMIC(cMapManager)
	CGMManagerToolApp *App;
	CBrush m_brush;
	COLORREF m_color;
	DWORD count;
public:
	cMapManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~cMapManager();
	void LoadMapInfo();
	void ShowSiegeWarTime(DWORD dwTime0, DWORD dwTime1);
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_MapList;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonMapstart();
	afx_msg void OnEnChangeEditMapStarttime();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonCloseallmap();
	afx_msg void OnBnClickedButtonCloseserver();
	void KillProcess(int port);
	int execmd(char * cmd, int & result);
	int pskill(int id);
	void EnableDebugPriv();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	
	afx_msg void OnBnClickedButtonAg0();
	afx_msg void OnBnClickedButtonAg1();
	afx_msg void OnBnClickedButtonAg2();
	afx_msg void OnBnClickedButtonAg3();
	afx_msg void OnBnClickedButtonAg4();
	afx_msg void OnBnClickedButtonAg5();
	afx_msg void OnBnClickedButtonDis0();
	afx_msg void OnBnClickedButtonDis1();
	afx_msg void OnBnClickedButtonDis2();
	afx_msg void OnBnClickedButtonDis3();
	afx_msg void OnBnClickedButtonDis4();
	afx_msg void OnBnClickedButtonDis5();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGetsiegewartime();
	afx_msg void OnBnClickedButtonSetsiegewartime();
};
