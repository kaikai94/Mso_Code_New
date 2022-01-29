#pragma once
#include "afxcmn.h"
struct StuGoldDealLog
{
	CString LogType;			//��־����			by:OpU_Eye	QQ;418914674
	CString FromUserID;			//���׷��˺�		by:OpU_Eye	QQ;418914674
	long	FromChrID;			//���׷���ɫID		by:OpU_Eye	QQ;418914674
	CString FromChrName;		//���׷���ɫ��		by:OpU_Eye	QQ;418914674
	long	BeforeChangeGold;	//���׷�Ԫ������	by:OpU_Eye	QQ;418914674
	CString ToUserID;			//�Է��˺�			by:OpU_Eye	QQ;418914674
	long	ToChrID;			//�Է���ɫID		by:OpU_Eye	QQ;418914674
	CString ToChrName;			//�Է���ɫ��		by:OpU_Eye	QQ;418914674
	long	DoChangedGold;		//�Է�Ԫ������		by:OpU_Eye	QQ;418914674
	long	ChangeGold;			//���׵�Ԫ������	by:OpU_Eye	QQ;418914674
	long	BuyItemIdx;			//���׵���Ʒ����	by:OpU_Eye	QQ;418914674
	int		ItemDurability;		//������Ʒ����		by:OpU_Eye	QQ;418914674
	CString RegDate;			//��������			by:OpU_Eye	QQ;418914674
};
class GoldDealLog : public CDialogEx
{
	DECLARE_DYNAMIC(GoldDealLog)
	//������ر��� by:OpU_Eye	QQ:418914674
	CBrush m_GoldDealLogbrush;
	COLORREF			m_TipColor;
	CStatic*			m_pTipStatic;
	CGMManagerToolApp*	m_pGoldDeal_App;
	CListCtrl*			m_pGoldDealList;
	CString				m_TempStr;
	//��ѯ��ر���	by:OpU_Eye	QQ:418914674
	CString			m_csQueChrName;
	CString			m_csQueChrIDX;
	CString			m_csQueID;
	StuGoldDealLog	m_DealLog;
	CString			m_ItemName;
public:
	GoldDealLog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GoldDealLog();
	void GetItemNumToName();
	void GetLogFromDBForDeal();
	void SetLogToListForDeal(int i);
	void DisplayInfoForDeal(int count);
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DEALLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGolddealQue();
};
