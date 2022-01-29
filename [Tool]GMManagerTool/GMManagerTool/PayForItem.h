#pragma once


// PayForItem 对话框
struct GoldPayForItemLog
{
	CString		m_LogType;
	CString		FromUserID;
	long		FromChrID;
	CString		FromChrName;
	long		BuyItemIdx;
	int			ItemDurability;
	long		BeforeChangeGold;
	long		DoChangedGold;
	long		ChangeGold;
	CString		RegDate;
};
class PayForItem : public CDialogEx
{
	DECLARE_DYNAMIC(PayForItem)
	//界面相关变量 by:OpU_Eye	QQ:418914674
	CBrush				m_PayForItembrush;
	COLORREF			m_TipColor;
	CStatic*			m_TipStatic;
	CGMManagerToolApp	*PayForItem_App;
	CListCtrl			*m_GoldPayForItemList;
	CString				m_TempStr;

	//查询相关变量	by:OpU_Eye	QQ:418914674
	CString		m_QueID;
	CString		m_QueChrName;
	CString		m_QueChrIDX;
	CString		m_ItemName;
	GoldPayForItemLog m_Log;
public:
	PayForItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PayForItem();
	void GetItemName();
	void GetLogFromDB();
	void SetLogToList(int i);
	void DisplayInfo(int count);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PAYFORITEM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGoldpayitemok();
};
