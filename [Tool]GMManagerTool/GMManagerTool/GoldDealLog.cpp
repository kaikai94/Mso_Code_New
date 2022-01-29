// GoldDealLog.cpp : 实现文件
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "GoldDealLog.h"
#include "afxdialogex.h"


// GoldDealLog 对话框

IMPLEMENT_DYNAMIC(GoldDealLog, CDialogEx)

GoldDealLog::GoldDealLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DEALLOG, pParent)
{

}

GoldDealLog::~GoldDealLog()
{
}



void GoldDealLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//全局变量初始化	by:OpU_Eye	QQ:418914674.
	m_pGoldDeal_App = (CGMManagerToolApp*)AfxGetApp();
	//界面初始化背景颜色	by:OpU_Eye	QQ:418914674
	m_GoldDealLogbrush.CreateSolidBrush(RGB(255, 255, 255));
	//提示文字初始化颜色	by:OpU_Eye	QQ:418914674
	m_pTipStatic = (CStatic*)GetDlgItem(IDC_STATIC_GOLDDEALTIP);
	m_TipColor = RGB(255,0,0);
	m_pTipStatic->RedrawWindow();
	//列表控件初始化	by:OpU_Eye	QQ:418914674
	m_pGoldDealList = (CListCtrl*)GetDlgItem(IDC_LIST_GOLDDEAL);
	CRect cGoldDealRect;
	m_pGoldDealList->GetClientRect(&cGoldDealRect);
	m_pGoldDealList->InsertColumn(0, _T("交易类型"), LVCFMT_CENTER, cGoldDealRect.Width() / 10, 0);
	m_pGoldDealList->InsertColumn(1, _T("交易方账号"), LVCFMT_CENTER, cGoldDealRect.Width() / 9,1);
	m_pGoldDealList->InsertColumn(2, _T("交易方角色ID"), LVCFMT_CENTER, cGoldDealRect.Width() / 7, 2);
	m_pGoldDealList->InsertColumn(3, _T("交易方角色名"), LVCFMT_CENTER, cGoldDealRect.Width() / 7, 3);
	m_pGoldDealList->InsertColumn(4, _T("交易方元宝数量"), LVCFMT_CENTER, cGoldDealRect.Width() / 6, 4);
	m_pGoldDealList->InsertColumn(5, _T("对方账号"), LVCFMT_CENTER, cGoldDealRect.Width() / 10, 5);
	m_pGoldDealList->InsertColumn(6, _T("对方角色ID"), LVCFMT_CENTER, cGoldDealRect.Width() / 9, 6);
	m_pGoldDealList->InsertColumn(7, _T("对方角色名"), LVCFMT_CENTER, cGoldDealRect.Width() / 9, 7);
	m_pGoldDealList->InsertColumn(8, _T("对方元宝数量"), LVCFMT_CENTER, cGoldDealRect.Width() / 7, 8);
	m_pGoldDealList->InsertColumn(9, _T("交易数额"), LVCFMT_CENTER, cGoldDealRect.Width() / 10, 9);
	m_pGoldDealList->InsertColumn(10, _T("交易的物品"), LVCFMT_CENTER, cGoldDealRect.Width() / 8, 10);
	m_pGoldDealList->InsertColumn(11, _T("交易物品数量"), LVCFMT_CENTER, cGoldDealRect.Width() / 8, 11);
	m_pGoldDealList->InsertColumn(12, _T("交易日期"), LVCFMT_CENTER, cGoldDealRect.Width() / 10, 12);
}


BEGIN_MESSAGE_MAP(GoldDealLog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_GOLDDEAL_QUE, &GoldDealLog::OnBnClickedButtonGolddealQue)
END_MESSAGE_MAP()


HBRUSH GoldDealLog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_GOLDDEALTIP:
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_TipColor);
				break;
		default:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0, 0, 0));
			break;
		}
	}
	return (HBRUSH)m_GoldDealLogbrush.GetSafeHandle();
}
void GoldDealLog::GetItemNumToName()
{
	if (m_DealLog.BuyItemIdx != 0)
	{
		std::map<int, ITEMINFO>::iterator iter;
		iter = m_pGoldDeal_App->m_ChangeUseConfig.find(m_DealLog.BuyItemIdx);
		if (iter!= m_pGoldDeal_App->m_ChangeUseConfig.end())
		{
			m_ItemName = iter->second.Name.c_str();
		}
	}
}

void GoldDealLog::GetLogFromDBForDeal()
{
	int count = 0;
	while (!m_pGoldDeal_App->m_LogDB.IsRecordEOF())
	{
		m_pGoldDeal_App->m_LogDB.CollectMsg("LogType", m_DealLog.LogType);
		m_pGoldDeal_App->m_LogDB.CollectMsg("FromUserID", m_DealLog.FromUserID);
		m_pGoldDeal_App->m_LogDB.CollectMsg("FromChrID", m_DealLog.FromChrID);
		m_pGoldDeal_App->m_LogDB.CollectMsg("FromChrName", m_DealLog.FromChrName);
		m_pGoldDeal_App->m_LogDB.CollectMsg("BeforeChangeGold", m_DealLog.BeforeChangeGold);
		m_pGoldDeal_App->m_LogDB.CollectMsg("ToUserID", m_DealLog.ToUserID);
		m_pGoldDeal_App->m_LogDB.CollectMsg("ToChrID", m_DealLog.ToChrID);
		m_pGoldDeal_App->m_LogDB.CollectMsg("ToChrName", m_DealLog.ToChrName);
		m_pGoldDeal_App->m_LogDB.CollectMsg("DoChangedGold", m_DealLog.DoChangedGold);
		m_pGoldDeal_App->m_LogDB.CollectMsg("ChangeGold", m_DealLog.ChangeGold);
		m_pGoldDeal_App->m_LogDB.CollectMsg("BuyItemIdx", m_DealLog.BuyItemIdx);
		m_pGoldDeal_App->m_LogDB.CollectMsg("ItemDurability", m_DealLog.ItemDurability);
		m_pGoldDeal_App->m_LogDB.CollectMsg("RegDate",1, m_DealLog.RegDate);
		GetItemNumToName();
		SetLogToListForDeal(count);
		count++;
		m_pGoldDeal_App->m_LogDB.RecordMoveNext();
	}
	m_pGoldDeal_App->m_LogDB.CloseRecord();
}

void GoldDealLog::SetLogToListForDeal(int i)
{
	m_pGoldDealList->InsertItem(i, m_DealLog.LogType);
	m_pGoldDealList->SetItemText(i, 1, m_DealLog.FromUserID);
	m_TempStr.Format(_T("%d"), m_DealLog.FromChrID);
	m_pGoldDealList->SetItemText(i, 2, m_TempStr);

	m_pGoldDealList->SetItemText(i, 3, m_DealLog.FromChrName);

	m_TempStr.Format(_T("%d"), m_DealLog.BeforeChangeGold);
	m_pGoldDealList->SetItemText(i, 4, m_TempStr);

	m_pGoldDealList->SetItemText(i, 5, m_DealLog.ToUserID);

	m_TempStr.Format(_T("%d"), m_DealLog.ToChrID);
	m_pGoldDealList->SetItemText(i, 6, m_TempStr);

	m_pGoldDealList->SetItemText(i, 7, m_DealLog.ToChrName);

	m_TempStr.Format(_T("%d"), m_DealLog.DoChangedGold);
	m_pGoldDealList->SetItemText(i, 8, m_TempStr);

	m_TempStr.Format(_T("%d"), m_DealLog.ChangeGold);
	m_pGoldDealList->SetItemText(i, 9, m_TempStr);

	m_pGoldDealList->SetItemText(i, 10, m_ItemName);

	m_TempStr.Format(_T("%d"), m_DealLog.ItemDurability);
	m_pGoldDealList->SetItemText(i, 11, m_TempStr);

	m_pGoldDealList->SetItemText(i, 12, m_DealLog.RegDate);
}

void GoldDealLog::DisplayInfoForDeal(int count)
{
	int UseGoldMoney = 0;
	m_TipColor = RGB(0, 255, 0);
	if (!m_csQueID.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE (FromUserID='%s'OR ToUserID='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueID, m_csQueID);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("账号【%s】共查询到交易记录【%d条】,一共交易【%d】元宝"), m_csQueID, count, UseGoldMoney);
		m_pTipStatic->SetWindowTextW(m_TempStr);
		m_pTipStatic->RedrawWindow();
	}
	if (!m_csQueChrIDX.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE (FromChrID=%s OR ToChrID=%s) AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrIDX, m_csQueChrIDX);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("共查询到交易记录【%d条】,一共交易【%d】元宝"), count, UseGoldMoney);
		m_pTipStatic->SetWindowTextW(m_TempStr);
		m_pTipStatic->RedrawWindow();
	}
	if (!m_csQueChrName.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE (FromChrName='%s'OR ToChrName='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrName, m_csQueChrName);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("角色【%s】共查询到交易记录【%d条】,一共交易【%d】元宝"), m_csQueChrName, count, UseGoldMoney);
		m_pTipStatic->SetWindowTextW(m_TempStr);
		m_pTipStatic->RedrawWindow();
	}
}

void GoldDealLog::OnBnClickedButtonGolddealQue()
{
	int count = 0;
	//获取查询条件	by:OpU_Eye	QQ:418914674
	GetDlgItemText(IDC_EDIT_GOLDDEAL_USERID, m_csQueID);
	GetDlgItemText(IDC_EDIT_GOLDDEAL_CHRNAMEFORDEL, m_csQueChrName);
	GetDlgItemText(IDC_EDIT_GOLDDEAL_CHRIDXFORDEL, m_csQueChrIDX);
	if (!m_csQueID.IsEmpty() && m_csQueChrName.IsEmpty() && m_csQueChrIDX.IsEmpty())  //账号查询判断	by:OpU_Eye	QQ:418914674
	{
		m_pGoldDealList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE (FromUserID='%s' OR ToUserID='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueID, m_csQueID);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("COUNT", count);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("【查 询 条 件】账号【%s】不存在元宝交易记录！"), m_csQueID);
			m_pTipStatic->SetWindowTextW(m_TempStr);
			m_pTipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE (FromUserID='%s' OR ToUserID='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueID, m_csQueID);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDBForDeal();
		DisplayInfoForDeal(count);
		count = 0;
		return;
	}
	if (m_csQueID.IsEmpty() && !m_csQueChrName.IsEmpty() && m_csQueChrIDX.IsEmpty())  //角色名查询判断	by:OpU_Eye	QQ:418914674
	{
		m_pGoldDealList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE (FromChrName='%s' OR ToChrName='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrName, m_csQueChrName);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("COUNT", count);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("【查 询 条 件】角色【%s】不存在元宝交易记录！"), m_csQueChrName);
			m_pTipStatic->SetWindowTextW(m_TempStr);
			m_pTipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE (FromChrName='%s' OR ToChrName='%s') AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrName, m_csQueChrName);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDBForDeal();
		DisplayInfoForDeal(count);
		count = 0;
		return;
	}
	if (m_csQueID.IsEmpty() && m_csQueChrName.IsEmpty() && !m_csQueChrIDX.IsEmpty())	//角色ID查询判断	by:OpU_Eye	QQ:418914674
	{
		m_pGoldDealList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE (FromChrID=%s OR ToChrID=%s) AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrIDX, m_csQueChrIDX);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr))return;
		m_pGoldDeal_App->m_LogDB.CollectMsg("COUNT", count);
		m_pGoldDeal_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("【查 询 条 件】角色ID为【%s】的角色不存在元宝交易记录！"), m_csQueChrIDX);
			m_pTipStatic->SetWindowTextW(m_TempStr);
			m_pTipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE (FromChrID=%s OR ToChrID=%s) AND (LogType = '摆摊交易' OR LogType ='直接交易')"), m_csQueChrIDX, m_csQueChrIDX);
		if (!m_pGoldDeal_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDBForDeal();
		DisplayInfoForDeal(count);
		count = 0;
		return;
	}
	m_TipColor = RGB(255, 0, 0);
	m_TempStr.Format(_T("【查 询 条 件】每次只能输入一个查询条件！"));
	m_pTipStatic->SetWindowTextW(m_TempStr);
	m_pTipStatic->RedrawWindow();
	m_TempStr.IsEmpty();

}
