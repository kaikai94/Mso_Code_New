// PayForGold.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "PayForGold.h"


// PayForGold �Ի���

IMPLEMENT_DYNAMIC(PayForGold, CDialogEx)

PayForGold::PayForGold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PAYGOLD, pParent)
{

}

PayForGold::~PayForGold()
{
}

void PayForGold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_PayMoney = 0;
	PayForGold_App = (CGMManagerToolApp *)AfxGetApp();
	m_PayForGoldbrush.CreateSolidBrush(RGB(255, 255, 255));
	m_PayForGold_color = RGB(255, 0, 0);
	m_PayForGold_pStaticTip = (CStatic*)GetDlgItem(IDC_STATIC_PAYGOLDTIP);
	m_PayForGold_pStaticTip->RedrawWindow();

	DDX_Control(pDX, IDC_COMBO_PAYMONEYOP, m_PayMoneyOP);
	m_PayMoneyOP.InsertString(0, _T("����"));
	m_PayMoneyOP.InsertString(1, _T("����"));
	m_PayMoneyOP.InsertString(2, _T("С��"));
	m_PayMoneyOP.SetCurSel(0);
	DDX_Control(pDX, IDC_LIST_PAYGOLD, m_PayLogList);
	CRect cRect;
	m_PayLogList.GetClientRect(cRect);
	m_PayLogList.InsertColumn(0, _T("��ֵ�˺�"), LVCFMT_CENTER, cRect.Width() / 6, 0);
	m_PayLogList.InsertColumn(1, _T("��ֵ����"), LVCFMT_CENTER, cRect.Width() / 6, 1);
	m_PayLogList.InsertColumn(2, _T("��ֵ���"), LVCFMT_CENTER, cRect.Width() / 6, 2);
	m_PayLogList.InsertColumn(3, _T("��ֵǰԪ������"), LVCFMT_CENTER, cRect.Width() / 6, 3);
	m_PayLogList.InsertColumn(4, _T("��ֵ��Ԫ������"), LVCFMT_CENTER, cRect.Width() / 6, 4);
	m_PayLogList.InsertColumn(5, _T("��ֵ����"), LVCFMT_CENTER, cRect.Width() / 6, 5);
}


BEGIN_MESSAGE_MAP(PayForGold, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_PAYGOLD, &PayForGold::OnBnClickedButtonPaygold)
END_MESSAGE_MAP()


HBRUSH PayForGold::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_PAYGOLDTIP:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_PayForGold_color);
				break;
			}
			default:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(0, 0, 0));
				break;
			}
		}
	}
		return (HBRUSH)m_PayForGoldbrush.GetSafeHandle();
}
void PayForGold::OnBnClickedButtonPaygold()
{
	int count = 0;
	GetDlgItemTextW(IDC_EDIT_GOLDLOGBYID, m_QueID);
	m_PayMoney = GetDlgItemInt(IDC_EDIT_PAYMONEY);
	m_MoneyOP = m_PayMoneyOP.GetCurSel();
	if (!m_QueID.IsEmpty() && m_PayMoney != 0)
	{
		m_PayForGold_color = RGB(255, 0, 0);
		Temp.Format(_T("���� ѯ �� ������ �� �� �� ֵ �� �� ÿ �� ֻ �� �� �� һ �� �� ѯ �� ��!"));
		m_PayForGold_pStaticTip->SetWindowTextW(Temp);
		m_PayForGold_pStaticTip->RedrawWindow();
		Temp.Empty();
		return;
	}
	if (!m_QueID.IsEmpty())
	{
		m_PayLogList.DeleteAllItems();
		Temp.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE FromUserID='%s'"), m_QueID);
		if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
		PayForGold_App->m_LogDB.CollectMsg("COUNT", count);
		PayForGold_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_PayForGold_color = RGB(255, 0, 0);
			Temp.Format(_T("���� ѯ �� ������ �š�%s���� �� �� �� ֵ �� ¼��"), m_QueID);
			m_PayForGold_pStaticTip->SetWindowTextW(Temp);
			m_PayForGold_pStaticTip->RedrawWindow();
			Temp.Empty();
			return;
		}
		Temp.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE FromUserID='%s'AND LogType='Ԫ����ֵ'"), m_QueID);
		if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
		GetItemByID();
	}
	if (m_PayMoney != 0)
	{
		switch (m_MoneyOP)
		{
			case 0:
				{
					m_PayLogList.DeleteAllItems();
					Temp.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE PayMoney='%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					PayForGold_App->m_LogDB.CollectMsg("COUNT", count);
					PayForGold_App->m_LogDB.CloseRecord();
					if (count == 0)
					{
						m_PayForGold_color = RGB(255, 0, 0);
						Temp.Format(_T("���� ѯ �� ������ �� �� �� ֵ �� �� �� �ڡ�%d���� �� ¼��"), m_PayMoney);
						m_PayForGold_pStaticTip->SetWindowTextW(Temp);
						m_PayForGold_pStaticTip->RedrawWindow();
						Temp.Empty();
						return;
					}
					Temp.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE PayMoney='%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					GetItemByID();
					m_PayMoney = 0;
					break;
				}
			case 1:
				{
					m_PayLogList.DeleteAllItems();
					Temp.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE PayMoney>'%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					PayForGold_App->m_LogDB.CollectMsg("COUNT", count);
					PayForGold_App->m_LogDB.CloseRecord();
					if (count == 0)
					{
						m_PayForGold_color = RGB(255, 0, 0);
						Temp.Format(_T("���� ѯ �� ������ �� �� �� ֵ �� �� �� �ڡ�%d���� �� ¼��"), m_PayMoney);
						m_PayForGold_pStaticTip->SetWindowTextW(Temp);
						m_PayForGold_pStaticTip->RedrawWindow();
						Temp.Empty();
						return;
					}
					Temp.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE PayMoney>'%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					GetItemByID();
					m_PayMoney = 0;
					break;
				}
			case 2:
				{
					m_PayLogList.DeleteAllItems();
					Temp.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE PayMoney<'%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					PayForGold_App->m_LogDB.CollectMsg("COUNT", count);
					PayForGold_App->m_LogDB.CloseRecord();
					if (count == 0)
					{
						m_PayForGold_color = RGB(255, 0, 0);
						Temp.Format(_T("���� ѯ �� ������ �� �� �� ֵ �� �� С �ڡ�%d���� �� ¼��"), m_PayMoney);
						m_PayForGold_pStaticTip->SetWindowTextW(Temp);
						m_PayForGold_pStaticTip->RedrawWindow();
						Temp.Empty();
						return;
					}
					Temp.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE PayMoney<'%d'"), m_PayMoney);
					if (!PayForGold_App->m_LogDB.SelectSQL(Temp))return;
					GetItemByID();
					m_PayMoney = 0;
					break;
				}
			default:
			break;
		}
	}
}
void PayForGold::GetItemByID()
{
	int deal = 0, payitem = 0, paygold = 0;
	while (!PayForGold_App->m_LogDB.IsRecordEOF())
	{
		PayForGold_App->m_LogDB.CollectMsg("LogType", m_Log.m_LogType);
		PayForGold_App->m_LogDB.CollectMsg("FromUserID", m_Log.FromUserID);
		PayForGold_App->m_LogDB.CollectMsg("BeforeChangeGold", m_Log.BeforeChangeGold);
		PayForGold_App->m_LogDB.CollectMsg("DoChangedGold", m_Log.DoChangedGold);
		PayForGold_App->m_LogDB.CollectMsg("ChangeGold", m_Log.ChangeGold);
		PayForGold_App->m_LogDB.CollectMsg("RegDate", 1, m_Log.RegDate);
		PayForGold_App->m_LogDB.CollectMsg("PayMoney", m_Log.PayMoney);
		SetPayLogList(paygold);
		PayForGold_App->m_LogDB.RecordMoveNext();
		paygold++;
	}
	m_PayForGold_color = RGB(0, 255, 0);
	Temp.Format(_T("�� �š�%s���� �� ѯ �� �� ֵ �� ¼��%d������"), m_QueID,paygold);
	m_PayForGold_pStaticTip->SetWindowTextW(Temp);
	m_PayForGold_pStaticTip->RedrawWindow();
	Temp.Empty();
	PayForGold_App->m_LogDB.CloseRecord();
}
void PayForGold::SetPayLogList(int i)
{
	m_PayLogList.InsertItem(i, m_Log.FromUserID);
	Temp.Format(_T("%d"), m_Log.ChangeGold);
	m_PayLogList.SetItemText(i, 1, Temp);
	Temp.Format(_T("%d"), m_Log.PayMoney);
	m_PayLogList.SetItemText(i, 2, Temp);
	m_PayLogList.SetItemText(i, 5, m_Log.RegDate);
	Temp.Format(_T("%d"), m_Log.BeforeChangeGold);
	m_PayLogList.SetItemText(i, 3, Temp);
	Temp.Format(_T("%d"), m_Log.DoChangedGold);
	m_PayLogList.SetItemText(i, 4, Temp);
}