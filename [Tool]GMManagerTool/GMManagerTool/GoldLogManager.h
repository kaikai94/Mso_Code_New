#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "GoldDealLog.h"
#include "PayForItem.h"
#include "PayForGold.h"

class GoldLogManager : public CDialogEx
{
	DECLARE_DYNAMIC(GoldLogManager)
	CBrush		m_GoldLog_brush;
	CTabCtrl	m_GoldLogCtrl;

	GoldDealLog	m_GoldDealLog;
	PayForItem	m_PayForItem;
	PayForGold	m_PayForGold;
public:
	GoldLogManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GoldLogManager();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTcnSelchangeTabGoldlog(NMHDR *pNMHDR, LRESULT *pResult);
};
