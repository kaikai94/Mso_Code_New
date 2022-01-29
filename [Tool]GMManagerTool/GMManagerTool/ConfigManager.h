/***配置设置类声明	by:OpU_Eye QQ:418914674***/
#pragma once
#include "afxwin.h"
class ConfigManager : public CDialogEx
{
	DECLARE_DYNAMIC(ConfigManager)
	/**对话框变量定义	by:OpU_Eye QQ:418914674**/
	CGMManagerToolApp*	m_pConfig_App;
	CBrush m_brush;
	COLORREF m_color;
	WORD m_Hour;
	WORD m_Minute;
	WORD First;
	WORD second;
	WORD third;
	WORD fourth;
	WORD fifth;
	WORD sixth;
	WORD seventh;
	WORD eighth;
	WORD ninth;
	WORD tenth;
	WORD eleventh;
	WORD twelfth;
	WORD thirteenth;
	WORD fourteenth;
	WORD fifteenth;
	WORD sixteenth;
	WORD seventeenth;
	WORD eighteenth;
	WORD nineteenth;
	WORD twentieth;
	WORD m_Type;
public:
	ConfigManager(CWnd* pParent = NULL);   // 默认构造函数	by:OpU_Eye QQ:418914674
	virtual ~ConfigManager();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	// 状态提示信息
	CStatic m_TipMsg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonStarttoplist();
	afx_msg void OnBnClickedButtonStoptoplist();
	afx_msg void OnBnClickedRadioGoldmoney();
	afx_msg void OnBnClickedRadioMallmoney();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
