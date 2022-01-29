#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// PayForGold 对话框
struct Log
{
	CString m_LogType;
	CString	FromUserID;
	long	BeforeChangeGold;
	long	DoChangedGold;
	long	ChangeGold;
	CString	RegDate;
	int		PayMoney;
};
class PayForGold : public CDialogEx
{
	DECLARE_DYNAMIC(PayForGold)
	CGMManagerToolApp *PayForGold_App;
	CBrush		m_PayForGoldbrush;
	COLORREF	m_PayForGold_color;
	CStatic*	m_PayForGold_pStaticTip;
	CString		Temp;
	CListCtrl	m_PayLogList;
	CComboBox	m_PayMoneyOP;
	Log			m_Log;
	CString		m_QueID;
	long		m_PayMoney;
	int			m_MoneyOP;
public:
	PayForGold(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PayForGold();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAYGOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnBnClickedButtonPaygold();
	void GetItemByID();
	void SetPayLogList(int i);
};
