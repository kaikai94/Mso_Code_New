/***配置设置类实现	by:OpU_Eye QQ:418914674***/
#include "stdafx.h"
#include "GMManagerTool.h"
#include "ConfigManager.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(ConfigManager, CDialogEx)

ConfigManager::ConfigManager(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

ConfigManager::~ConfigManager()
{
}

void ConfigManager::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/**背景颜色设置	by:OpU_Eye QQ:418914674*/
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	m_pConfig_App = (CGMManagerToolApp*)AfxGetApp();
	m_Type = 1;
	DDX_Control(pDX, IDC_STATIC_TOPLISTTIP, m_TipMsg);
	((CButton*)GetDlgItem(IDC_RADIO_GOLDMONEY))->SetCheck(TRUE);
}

BEGIN_MESSAGE_MAP(ConfigManager, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STARTTOPLIST, &ConfigManager::OnBnClickedButtonStarttoplist)
	ON_BN_CLICKED(IDC_BUTTON_STOPTOPLIST, &ConfigManager::OnBnClickedButtonStoptoplist)
	ON_BN_CLICKED(IDC_RADIO_GOLDMONEY, &ConfigManager::OnBnClickedRadioGoldmoney)
	ON_BN_CLICKED(IDC_RADIO_MALLMONEY, &ConfigManager::OnBnClickedRadioMallmoney)
	ON_BN_CLICKED(IDC_BUTTON1, &ConfigManager::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ConfigManager::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &ConfigManager::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &ConfigManager::OnBnClickedButton5)
END_MESSAGE_MAP()
/**字体颜色改变消息响应函数	by:OpU_Eye QQ:418914674**/
HBRUSH ConfigManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == IDC_STATIC_TOPLISTTIP)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_color);
	}
	return (HBRUSH)m_brush.GetSafeHandle();
}

BOOL ConfigManager::PreTranslateMessage(MSG* pMsg)
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



void ConfigManager::OnTimer(UINT_PTR nIDEvent)
{
	CString str;
	SYSTEMTIME LocalTime;
	GetLocalTime(&LocalTime);
	if (LocalTime.wHour == m_Hour&&LocalTime.wMinute == m_Minute)
	{
		str.Format(_T("EXEC GMTool_GiveGoldMoney_ForTopList %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d"),
			1,First, second, third, fourth, fifth, sixth, seventh, eighth, ninth, tenth, eleventh, twelfth, thirteenth,
			fourteenth, fifteenth, sixteenth, seventeenth, eighteenth, nineteenth, twentieth);
		long count;
		m_pConfig_App->m_IDDB.ExecuteSQL(str, count);
		KillTimer(1);
		str.Format(_T("奖励发放完成"));
		m_color = RGB(0, 255, 0);
		m_TipMsg.SetWindowText(str);
		m_TipMsg.RedrawWindow();
	}
	else
	{
		str.Format(_T("奖励发放剩余时间:%d小时%d分%d秒"), m_Hour-LocalTime.wHour , m_Minute-LocalTime.wMinute,60- LocalTime.wSecond);
		m_color = RGB(0, 255, 0);
		m_TipMsg.SetWindowText(str);
		m_TipMsg.RedrawWindow();
	}
	CDialogEx::OnTimer(nIDEvent);
}


void ConfigManager::OnBnClickedButtonStarttoplist()
{
	CString str;
	if (!m_pConfig_App->m_IDDB.IsConnect())
	{
		str.Format(_T("数据库未连接，请先连接数据库"));
		m_color = RGB(255, 0, 0);
		m_TipMsg.SetWindowText(str);
		m_TipMsg.RedrawWindow();
		return;
	}
	First		= GetDlgItemInt(IDC_EDIT_FIRST);
	second		= GetDlgItemInt(IDC_EDIT_SECOND);
	third		= GetDlgItemInt(IDC_EDIT_THIRD);
	fourth		= GetDlgItemInt(IDC_EDIT_FOURTH);
	fifth		= GetDlgItemInt(IDC_EDIT_FIFTH);
	sixth		= GetDlgItemInt(IDC_EDIT_SIXTH);
	seventh		= GetDlgItemInt(IDC_EDIT_SEVENTH);
	eighth		= GetDlgItemInt(IDC_EDIT_EIGHTH);
	ninth		= GetDlgItemInt(IDC_EDIT_NINTH);
	tenth		= GetDlgItemInt(IDC_EDIT_TENTH);
	eleventh	= GetDlgItemInt(IDC_EDIT_ELEVENTH);
	twelfth		= GetDlgItemInt(IDC_EDIT_TWELFTH);
	thirteenth	= GetDlgItemInt(IDC_EDIT_THIRTEENTH);
	fourteenth	= GetDlgItemInt(IDC_EDIT_FOURTEENTH);
	fifteenth	= GetDlgItemInt(IDC_EDIT_FIFTEENTH);
	sixteenth	= GetDlgItemInt(IDC_EDIT_SIXTEENTH);
	seventeenth = GetDlgItemInt(IDC_EDIT_SEVENTEENTH);
	eighteenth	= GetDlgItemInt(IDC_EDIT_EIGHTEENTH);
	nineteenth	= GetDlgItemInt(IDC_EDIT_NINETEENTH);
	twentieth	= GetDlgItemInt(IDC_EDIT_TWENTIETH);
	m_Hour		= GetDlgItemInt(IDC_EDIT_HOUR);
	m_Minute	= GetDlgItemInt(IDC_EDIT_MINUT);

	SetTimer(1, 1000, NULL);
	m_color = RGB(0, 255, 0);
	str.Format(_T("开始计时"));
	m_TipMsg.SetWindowText(str);
	m_TipMsg.RedrawWindow();
}


void ConfigManager::OnBnClickedButtonStoptoplist()
{
	CString str;
	KillTimer(1);
	m_color = RGB(255, 0, 0);
	str.Format(_T("已停止奖励发放"));
	m_TipMsg.SetWindowText(str);
	m_TipMsg.RedrawWindow();
}


void ConfigManager::OnBnClickedRadioGoldmoney()
{
	m_Type = 1;
	((CButton*)GetDlgItem(IDC_RADIO_GOLDMONEY))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_RADIO_MALLMONEY))->SetCheck(FALSE);
}


void ConfigManager::OnBnClickedRadioMallmoney()
{
	m_Type = 2;
	((CButton*)GetDlgItem(IDC_RADIO_GOLDMONEY))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_RADIO_MALLMONEY))->SetCheck(TRUE);
}


void ConfigManager::OnBnClickedButton1()
{
	CString str;
	long temp = GetDlgItemInt(IDC_EDIT_ALL);
	if (m_Type == 1)
		str.Format(_T("EXEC GMTool_Give_GoldMoney %d"), temp);
	if (m_Type == 2)
		str.Format(_T("EXEC GMTool_Give_MallMoney %d"), temp);
	if (m_pConfig_App->m_IDDB.ExecuteSQL(str, temp))
		str.Format(_T("发放成功"));
	else
		str.Format(_T("发放失败"));
	::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
}


void ConfigManager::OnBnClickedButton2()
{
	CString str;
	long temp = GetDlgItemInt(IDC_EDIT_ONLINE);
	if (m_Type == 1)
		str.Format(_T("EXEC GMTool_Give_GoldMoneyForOnline %d"), temp);
	if (m_Type == 2)
		str.Format(_T("EXEC GMTool_Give_MallMoneyForOnline %d"), temp);
	if (m_pConfig_App->m_IDDB.ExecuteSQL(str, temp))
		str.Format(_T("发放成功"));
	else
		str.Format(_T("发放失败"));
	::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
}


void ConfigManager::OnBnClickedButton4()
{
	CString str,ID;
	long temp= GetDlgItemInt(IDC_EDIT_GIFCOUNT);
	GetDlgItemTextW(IDC_EDIT_GIFUSERID, ID);
	if (ID.IsEmpty())
	{
		str.Format(_T("请输入账号"));
		::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
		return;
	}
	if (m_Type == 1)
		str.Format(_T("EXEC GMTool_GoldMoneyGiveForPlayer %s,%d"),ID,temp);
	if (m_Type == 2)
		str.Format(_T("EXEC GMTool_MallMoneyGiveForPlayer %s,%d"), ID,temp);
	if (m_pConfig_App->m_IDDB.ExecuteSQL(str, temp))
		str.Format(_T("发放成功"));
	else
		str.Format(_T("发放失败"));
	::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
}


void ConfigManager::OnBnClickedButton5()
{
	CString str, CharName;
	CString id_loginid;
	int CharId = 0;
	long temp = GetDlgItemInt(IDC_EDIT_CHARCOUNT);
	CharId = GetDlgItemInt(IDC_EDIT_CHARID);
	GetDlgItemTextW(IDC_EDIT_CHARNAME, CharName);
	if (CharId != 0 && !CharName.IsEmpty())
	{
		str.Format(_T("只能根据一个条件进行奖励发放"));
		::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
		return;
	}
	if (CharId != 0 && CharName.IsEmpty())
	{
		str.Format(_T("exec GMTool_GiveGoldMoneyForCharID %d,%d,%d"), CharId,temp, m_Type);
		if (m_pConfig_App->m_IDDB.ExecuteSQL(str, temp))
			str.Format(_T("发放成功"));
		else
			str.Format(_T("发放失败"));
		::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
	}
	if (CharId == 0 && !CharName.IsEmpty())
	{
		str.Format(_T("exec GMTool_GiveGoldMoneyForCharName %s,%d,%d"), CharName, temp, m_Type);
		if (m_pConfig_App->m_IDDB.ExecuteSQL(str, temp))
			str.Format(_T("发放成功"));
		else
			str.Format(_T("发放失败"));
		::MessageBox(this->m_hWnd, str, _T("提示"), MB_OK);
	}
}
