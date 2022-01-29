// ShopItemTimeToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ShopItemTimeTool.h"
#include "ShopItemTimeToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
DWORD DayOfMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
DWORD DayOfMonth_Yundal[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
char Days[7][16] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };
char WeatherState[][16] = { "CLEAR", "SNOW" };
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CShopItemTimeToolDlg 对话框




CShopItemTimeToolDlg::CShopItemTimeToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShopItemTimeToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShopItemTimeToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShopItemTimeToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CShopItemTimeToolDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CShopItemTimeToolDlg 消息处理程序

BOOL CShopItemTimeToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CShopItemTimeToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CShopItemTimeToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CShopItemTimeToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CShopItemTimeToolDlg::OnBnClickedButton1()
{
	
	WORD wYear,wMonth,wDay,wHour,wMinute,wSeconds;
	wYear = 0;
	wMonth = 0;
	wDay = 0;
	wHour = 0;
	wMinute = 0;
	wSeconds = 0;

	wYear		= GetDlgItemInt(IDC_EDIT1);
	wMonth		= GetDlgItemInt(IDC_EDIT2);
	wDay		= GetDlgItemInt(IDC_EDIT3);
	wHour		= GetDlgItemInt(IDC_EDIT4);
	wMinute		= GetDlgItemInt(IDC_EDIT5);
	wSeconds	= GetDlgItemInt(IDC_EDIT6);
	stTIME Time;
	Time.SetTime(wYear,wMonth,wDay,wHour,wMinute,wSeconds);
	SetDlgItemInt(IDC_EDIT7,Time.value);

}
