// PayForItem.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "PayForItem.h"
#include "afxdialogex.h"


// PayForItem �Ի���

IMPLEMENT_DYNAMIC(PayForItem, CDialogEx)

PayForItem::PayForItem(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PAYFORITEM, pParent)
{

}

PayForItem::~PayForItem()
{
}

void PayForItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//��ȡ���ݿ����ȫ�ֱ���	by:OpU_Eye	QQ:418914674
	PayForItem_App = (CGMManagerToolApp *)AfxGetApp();
	//���汳������ʾ������ɫ����  by:OpU_Eye	QQ:418914674
	m_PayForItembrush.CreateSolidBrush(RGB(255, 255, 255));
	m_TipColor = RGB(255, 0, 0);
	m_TipStatic = (CStatic*)GetDlgItem(IDC_STATIC_GOLDPAYITEM_TIP);
	m_TipStatic->RedrawWindow();
	//�б�ؼ���ʼ��	by:OpU_Eye	QQ:418914674
	m_GoldPayForItemList = (CListCtrl*)GetDlgItem(IDC_LIST1_GOLDPAYITEM);
	CRect cRect;
	m_GoldPayForItemList->GetClientRect(&cRect);
	m_GoldPayForItemList->InsertColumn(0, _T("��������"), LVCFMT_CENTER, cRect.Width() / 10, 0);
	m_GoldPayForItemList->InsertColumn(1, _T("�˺�"), LVCFMT_CENTER, cRect.Width() / 14, 1);
	m_GoldPayForItemList->InsertColumn(2, _T("��ɫ��"), LVCFMT_CENTER, cRect.Width() / 11, 2);
	m_GoldPayForItemList->InsertColumn(3, _T("��ɫID"), LVCFMT_CENTER, cRect.Width() / 12, 3);
	m_GoldPayForItemList->InsertColumn(4, _T("������Ʒ��"), LVCFMT_CENTER, cRect.Width() / 9, 4);
	m_GoldPayForItemList->InsertColumn(5, _T("��������"), LVCFMT_CENTER, cRect.Width() / 9, 5);
	m_GoldPayForItemList->InsertColumn(6, _T("����ǰԪ��"), LVCFMT_CENTER, cRect.Width() / 9, 6);
	m_GoldPayForItemList->InsertColumn(7, _T("�����Ԫ��"), LVCFMT_CENTER, cRect.Width() / 9, 7);
	m_GoldPayForItemList->InsertColumn(8, _T("����Ԫ������"), LVCFMT_CENTER, cRect.Width() / 7, 8);
	m_GoldPayForItemList->InsertColumn(9, _T("����"), LVCFMT_CENTER, cRect.Width() / 13, 9);

}
//��Ϣӳ��	by:OpU_Eye	QQ:418914674
BEGIN_MESSAGE_MAP(PayForItem, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_GOLDPAYITEMOK, &PayForItem::OnBnClickedButtonGoldpayitemok)
END_MESSAGE_MAP()
//����������ɫ����	by:OpU_Eye	QQ:418914674
HBRUSH PayForItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_GOLDPAYITEM_TIP:
			{
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(m_TipColor);
				break;
			}
		default:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0, 0, 0));
			break;
		}
	}
	return (HBRUSH)m_PayForItembrush.GetSafeHandle();
}
//��ƷIDת����Ʒ����   by:OpU_Eye  QQ:418914674
void PayForItem::GetItemName()
{
	if (m_Log.BuyItemIdx != 0)
	{
		std::map<int, ITEMINFO>::iterator iter;
		iter = PayForItem_App->m_ChangeUseConfig.find(m_Log.BuyItemIdx);
		if (iter != PayForItem_App->m_ChangeUseConfig.end())
		{
			m_ItemName = iter->second.Name.c_str();
		}
	}
}
//��ѯ��ť���������	by:OpU_Eye	QQ:418914674
void PayForItem::OnBnClickedButtonGoldpayitemok()
{
	int count = 0;
	//��ȡ��ѯ����	by:OpU_Eye	QQ:418914674
	GetDlgItemText(IDC_EDIT_GOLDDEAL_ID, m_QueID);
	GetDlgItemText(IDC_EDIT_GOLDDEAL_CHRNAME, m_QueChrName);
	GetDlgItemText(IDC_EDIT_GOLDDEAL_CHRIDX, m_QueChrIDX);
	if (!m_QueID.IsEmpty() && m_QueChrName.IsEmpty() && m_QueChrIDX.IsEmpty())  //�˺Ų�ѯ�ж�	by:OpU_Eye	QQ:418914674
	{
		m_GoldPayForItemList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE FromUserID='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueID);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("COUNT", count);
		PayForItem_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("���� ѯ �� �����˺š�%s��������Ԫ�����Ѽ�¼��"), m_QueID);
			m_TipStatic->SetWindowTextW(m_TempStr);
			m_TipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE FromUserID='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueID);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDB();
		DisplayInfo(count);
		count = 0;
		return;
	}
	if (m_QueID.IsEmpty() && !m_QueChrName.IsEmpty() && m_QueChrIDX.IsEmpty())  //��ɫ����ѯ�ж�	by:OpU_Eye	QQ:418914674
	{
		m_GoldPayForItemList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE FromChrName='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrName);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("COUNT", count);
		PayForItem_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("���� ѯ �� ������ɫ��%s��������Ԫ�����Ѽ�¼��"), m_QueChrName);
			m_TipStatic->SetWindowTextW(m_TempStr);
			m_TipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE FromChrName='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrName);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDB();
		DisplayInfo(count);
		count = 0;
		return;
	}
	if (m_QueID.IsEmpty() && m_QueChrName.IsEmpty() && !m_QueChrIDX.IsEmpty())	//��ɫID��ѯ�ж�	by:OpU_Eye	QQ:418914674
	{
		m_GoldPayForItemList->DeleteAllItems();
		m_TempStr.Format(_T("SELECT COUNT(LogType) AS COUNT FROM GoldMoneyChangeLog WHERE FromChrID=%s AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrIDX);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("COUNT", count);
		PayForItem_App->m_LogDB.CloseRecord();
		if (count == 0)
		{
			m_TipColor = RGB(255, 0, 0);
			m_TempStr.Format(_T("���� ѯ �� ������ɫIDΪ��%s���Ľ�ɫ������Ԫ�����Ѽ�¼��"), m_QueChrIDX);
			m_TipStatic->SetWindowTextW(m_TempStr);
			m_TipStatic->RedrawWindow();
			m_TempStr.IsEmpty();
			return;
		}
		m_TempStr.Format(_T("SELECT * FROM GoldMoneyChangeLog WHERE FromChrID=%s AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrIDX);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr)) return;
		GetLogFromDB();
		DisplayInfo(count);
		count = 0;
		return;
	}
	m_TipColor = RGB(255, 0, 0);
	m_TempStr.Format(_T("���� ѯ �� ����ÿ��ֻ������һ����ѯ������"));
	m_TipStatic->SetWindowTextW(m_TempStr);
	m_TipStatic->RedrawWindow();
	m_TempStr.IsEmpty();
}
//��ѯ���ȡ������	by:OpU_Eye	QQ:418914674
void PayForItem::GetLogFromDB()
{
	int count = 0;
	while (!PayForItem_App->m_LogDB.IsRecordEOF())
	{
		PayForItem_App->m_LogDB.CollectMsg("LogType", m_Log.m_LogType);
		PayForItem_App->m_LogDB.CollectMsg("FromUserID", m_Log.FromUserID);
		PayForItem_App->m_LogDB.CollectMsg("FromChrID", m_Log.FromChrID);
		PayForItem_App->m_LogDB.CollectMsg("FromChrName", m_Log.FromChrName);
		if(m_Log.m_LogType=="����ٱ�")
		{
			PayForItem_App->m_LogDB.CollectMsg("BuyItemIdx", m_Log.BuyItemIdx);
			PayForItem_App->m_LogDB.CollectMsg("ItemDurability", m_Log.ItemDurability);
			GetItemName();
		}
		PayForItem_App->m_LogDB.CollectMsg("BeforeChangeGold", m_Log.BeforeChangeGold);
		PayForItem_App->m_LogDB.CollectMsg("DoChangedGold", m_Log.DoChangedGold);
		PayForItem_App->m_LogDB.CollectMsg("ChangeGold", m_Log.ChangeGold);
		PayForItem_App->m_LogDB.CollectMsg("RegDate",1, m_Log.RegDate);
		SetLogToList(count);
		count++;
		PayForItem_App->m_LogDB.RecordMoveNext();
	}
	PayForItem_App->m_LogDB.CloseRecord();
}
//��ʾ��ѯ������Ӧ��Ϣ	by:OpU_Eye	QQ:418914674
void PayForItem::DisplayInfo(int count)
{
	int UseGoldMoney = 0;
	m_TipColor = RGB(0,255, 0);
	if (!m_QueID.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE FromUserID='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueID);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		PayForItem_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("�˺š�%s������ѯ�����Ѽ�¼��%d����,һ�����ѡ�%d��Ԫ��"), m_QueID, count, UseGoldMoney);
		m_TipStatic->SetWindowTextW(m_TempStr);
		m_TipStatic->RedrawWindow();
	}
	if (!m_QueChrIDX.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE FromChrID=%s AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrIDX);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		PayForItem_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("����ѯ�����Ѽ�¼��%d����,һ�����ѡ�%d��Ԫ��"), count, UseGoldMoney);
		m_TipStatic->SetWindowTextW(m_TempStr);
		m_TipStatic->RedrawWindow();
	}
	if (!m_QueChrName.IsEmpty())
	{
		m_TempStr.Format(_T("SELECT SUM(ChangeGold) AS Change FROM GoldMoneyChangeLog WHERE FromChrName='%s' AND LogType != '��̯����' AND LogType !='Ԫ����ֵ' AND LogType !='ֱ�ӽ���'"), m_QueChrName);
		if (!PayForItem_App->m_LogDB.SelectSQL(m_TempStr))return;
		PayForItem_App->m_LogDB.CollectMsg("Change", UseGoldMoney);
		PayForItem_App->m_LogDB.CloseRecord();
		m_TempStr.Format(_T("��ɫ��%s������ѯ�����Ѽ�¼��%d����,һ�����ѡ�%d��Ԫ��"), m_QueChrName,count, UseGoldMoney);
		m_TipStatic->SetWindowTextW(m_TempStr);
		m_TipStatic->RedrawWindow();
	}
}
//����ѯ���Ľ����ӵ��б�	by:OpU_Eye	QQ:418914674
void PayForItem::SetLogToList(int i)
{
	m_GoldPayForItemList->InsertItem(i, m_Log.m_LogType);
	m_GoldPayForItemList->SetItemText(i, 1, m_Log.FromUserID);
	m_GoldPayForItemList->SetItemText(i, 2, m_Log.FromChrName);
	m_TempStr.Format(_T("%d"), m_Log.FromChrID);
	m_GoldPayForItemList->SetItemText(i, 3, m_TempStr);
	if (m_Log.m_LogType != "����ٱ�")
	{
		m_ItemName.Format(_T("��"));
		m_Log.ItemDurability = 0;
	}
	m_GoldPayForItemList->SetItemText(i, 4, m_ItemName);
	m_TempStr.Format(_T("%d"), m_Log.ItemDurability);
	m_GoldPayForItemList->SetItemText(i, 5, m_TempStr);
	m_TempStr.Format(_T("%d"), m_Log.BeforeChangeGold);
	m_GoldPayForItemList->SetItemText(i, 6, m_TempStr);
	m_TempStr.Format(_T("%d"), m_Log.DoChangedGold);
	m_GoldPayForItemList->SetItemText(i, 7, m_TempStr);
	m_TempStr.Format(_T("%d"), m_Log.ChangeGold);
	m_GoldPayForItemList->SetItemText(i, 8, m_TempStr);
	m_GoldPayForItemList->SetItemText(i, 9, m_Log.RegDate);
}