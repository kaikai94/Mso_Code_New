// GoldLogManager.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "GoldLogManager.h"
#include "afxdialogex.h"


// GoldLogManager �Ի���

IMPLEMENT_DYNAMIC(GoldLogManager, CDialogEx)

GoldLogManager::GoldLogManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

GoldLogManager::~GoldLogManager()
{

}

void GoldLogManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*�Ի��򱳾���ɫ����  by:OpU_Eye QQ:418914674*/
	m_GoldLog_brush.CreateSolidBrush(RGB(255, 255, 255));
	/*Tip����Ĭ�Ϻ�ɫ����  by:OpU_Eye QQ:418914674*/

	DDX_Control(pDX, IDC_TAB_GOLDLOG,m_GoldLogCtrl);

	m_GoldLogCtrl.InsertItem(0, _T("*             Ԫ������ֵ����¼��ѯ             *"));
	m_GoldLogCtrl.InsertItem(1, _T("*             Ԫ�������ס���¼��ѯ             *"));
	m_GoldLogCtrl.InsertItem(2, _T("*             Ԫ�������ѡ���¼��ѯ             *"));

	m_PayForGold.Create(IDD_DIALOG_PAYGOLD,&m_GoldLogCtrl);
	m_GoldDealLog.Create(IDD_DIALOG_DEALLOG, &m_GoldLogCtrl);
	m_PayForItem.Create(IDD_DIALOG_GOLDPAYITEM,&m_GoldLogCtrl);

	CRect GoldTabRect;
	m_GoldLogCtrl.GetClientRect(&GoldTabRect);
	GoldTabRect.left += 1;
	GoldTabRect.top += 22;
	GoldTabRect.right -= 3;
	GoldTabRect.bottom -= 2;

	m_PayForGold.MoveWindow(&GoldTabRect);
	m_GoldDealLog.MoveWindow(&GoldTabRect);
	m_PayForItem.MoveWindow(&GoldTabRect);

	m_PayForGold.ShowWindow(true);
	m_GoldDealLog.ShowWindow(false);
	m_PayForItem.ShowWindow(false);
	m_GoldLogCtrl.SetCurSel(0);

}


BEGIN_MESSAGE_MAP(GoldLogManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_GOLDLOG, &GoldLogManager::OnTcnSelchangeTabGoldlog)
END_MESSAGE_MAP()


// GoldLogManager ��Ϣ�������


BOOL GoldLogManager::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		// �����Ϣ�Ǽ��̰����¼�������Esc����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// �����Ϣ�Ǽ��̰����¼�������Entert����ִ�����´��루ʲô��������������Լ������Ҫ�Ĵ��룩
		return TRUE;
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH GoldLogManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
	}
	return (HBRUSH)m_GoldLog_brush.GetSafeHandle();
}


void GoldLogManager::OnTcnSelchangeTabGoldlog(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_GoldLogCtrl.GetCurSel())
	{
		case 0:
			{
				m_PayForGold.ShowWindow(true);
				m_GoldDealLog.ShowWindow(false);
				m_PayForItem.ShowWindow(false);
				break;
			}
		case 1:
			{
				m_PayForGold.ShowWindow(false);
				m_GoldDealLog.ShowWindow(true);
				m_PayForItem.ShowWindow(false);
				break;
			}
		case 2:
			{
				m_PayForGold.ShowWindow(false);
				m_GoldDealLog.ShowWindow(false);
				m_PayForItem.ShowWindow(true);
				break;
			}
	}
	*pResult = 0;
}
