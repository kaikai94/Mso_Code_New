// GoldLogManager.cpp : 实现文件
//

#include "stdafx.h"
#include "GMManagerTool.h"
#include "GoldLogManager.h"
#include "afxdialogex.h"


// GoldLogManager 对话框

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
	/*对话框背景颜色设置  by:OpU_Eye QQ:418914674*/
	m_GoldLog_brush.CreateSolidBrush(RGB(255, 255, 255));
	/*Tip字体默认红色设置  by:OpU_Eye QQ:418914674*/

	DDX_Control(pDX, IDC_TAB_GOLDLOG,m_GoldLogCtrl);

	m_GoldLogCtrl.InsertItem(0, _T("*             元宝【充值】记录查询             *"));
	m_GoldLogCtrl.InsertItem(1, _T("*             元宝【交易】记录查询             *"));
	m_GoldLogCtrl.InsertItem(2, _T("*             元宝【消费】记录查询             *"));

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


// GoldLogManager 消息处理程序


BOOL GoldLogManager::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
	{
		// 如果消息是键盘按下事件，且是Esc键，执行以下代码（什么都不做，你可以自己添加想要的代码）
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		// 如果消息是键盘按下事件，且是Entert键，执行以下代码（什么都不做，你可以自己添加想要的代码）
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
