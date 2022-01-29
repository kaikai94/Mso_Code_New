#pragma once
#include "afxcmn.h"
struct StuGoldDealLog
{
	CString LogType;			//日志类型			by:OpU_Eye	QQ;418914674
	CString FromUserID;			//交易方账号		by:OpU_Eye	QQ;418914674
	long	FromChrID;			//交易方角色ID		by:OpU_Eye	QQ;418914674
	CString FromChrName;		//交易方角色名		by:OpU_Eye	QQ;418914674
	long	BeforeChangeGold;	//交易方元宝数量	by:OpU_Eye	QQ;418914674
	CString ToUserID;			//对方账号			by:OpU_Eye	QQ;418914674
	long	ToChrID;			//对方角色ID		by:OpU_Eye	QQ;418914674
	CString ToChrName;			//对方角色名		by:OpU_Eye	QQ;418914674
	long	DoChangedGold;		//对方元宝数量		by:OpU_Eye	QQ;418914674
	long	ChangeGold;			//交易的元宝数量	by:OpU_Eye	QQ;418914674
	long	BuyItemIdx;			//交易的物品名称	by:OpU_Eye	QQ;418914674
	int		ItemDurability;		//交易物品数量		by:OpU_Eye	QQ;418914674
	CString RegDate;			//交易日期			by:OpU_Eye	QQ;418914674
};
class GoldDealLog : public CDialogEx
{
	DECLARE_DYNAMIC(GoldDealLog)
	//界面相关变量 by:OpU_Eye	QQ:418914674
	CBrush m_GoldDealLogbrush;
	COLORREF			m_TipColor;
	CStatic*			m_pTipStatic;
	CGMManagerToolApp*	m_pGoldDeal_App;
	CListCtrl*			m_pGoldDealList;
	CString				m_TempStr;
	//查询相关变量	by:OpU_Eye	QQ:418914674
	CString			m_csQueChrName;
	CString			m_csQueChrIDX;
	CString			m_csQueID;
	StuGoldDealLog	m_DealLog;
	CString			m_ItemName;
public:
	GoldDealLog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GoldDealLog();
	void GetItemNumToName();
	void GetLogFromDBForDeal();
	void SetLogToListForDeal(int i);
	void DisplayInfoForDeal(int count);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEALLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGolddealQue();
};
