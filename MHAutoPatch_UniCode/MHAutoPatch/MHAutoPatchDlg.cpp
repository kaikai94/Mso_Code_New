
// MHAutoPatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MHAutoPatch.h"
#include "MHAutoPatchDlg.h"
#include "afxdialogex.h"
#include "MonitorAdapter.h"
#include <afxinet.h>
#include <thread>
#include "./lib/ClinkAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//[登录器版本号][BY:十里坡剑神][QQ:112582793][2019-5-2][20:34:20]
//char AutoPatchVer[] = "20190429001";
TCHAR AutoPatchVer[] = L"20190801001";

// CMHAutoPatchDlg 对话框



CMHAutoPatchDlg::CMHAutoPatchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MHAUTOPATCH_DIALOG, pParent),
	mHtmlViewNotice(new CHtmlViewNotice)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//_CrtSetBreakAlloc(14694);
}

CMHAutoPatchDlg::~CMHAutoPatchDlg()
{
	//[GDI+资源清理][by:十里坡剑神][QQ:112582793][2019-5-2][20:00]
	GdiplusShutdown(m_pGdiToken);
	m_pGdiToken = NULL;
}

void CMHAutoPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_WDNMODE, m_CheckWndMode);
	DDX_Control(pDX, IDC_COMBO_SCREEN, m_ComboBox);
	DDX_Control(pDX, IDC_STATIC_Tip, m_StTip);
	DDX_Control(pDX, IDC_STATIC_FILESIZETIP, m_StDwnlTip);
	DDX_Control(pDX, IDC_PROGRESS2, m_Progress);
	DDX_Control(pDX, IDC_PROGRESS_DONE, m_ProgressDone);
	StopTheard = FALSE;
	DDX_Control(pDX, IDC_STATIC_VER, m_AutoPatchVer);
}
void CMHAutoPatchDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);

	PostMessage(WM_NCLBUTTONDOWN,
		HTCAPTION,
		MAKELPARAM(point.x, point.y));
}
HBRUSH CMHAutoPatchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SelectObject(&m_font);       //设置字体 
	pDC->SetTextColor(RGB(sTextColor[eFont].Red, sTextColor[eFont].Green, sTextColor[eFont].Blue));  //设置字体颜色
	pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_Tip:
		return hbr;
	case IDC_STATIC_VER:
		return hbr2;
	case IDC_STATIC_FILESIZETIP:
		return hbr1;
	}
	return hbr;
}
BEGIN_MESSAGE_MAP(CMHAutoPatchDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_SCREEN, &CMHAutoPatchDlg::OnCbnSelchangeComboScreen)	//[分辨率选择事件][BY:十里坡剑神][QQ:112582793][2019-4-30][23:24:04]
	ON_BN_CLICKED(IDC_BUTTON_START, &CMHAutoPatchDlg::OnBnClickedButtonStart)			//[开始按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:08]
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CMHAutoPatchDlg::OnBnClickedButtonClose)			//[关闭按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:41]
	ON_BN_CLICKED(IDC_BUTTON_HOMEPAGE, &CMHAutoPatchDlg::OnBnClickedButtonHomepage)	//[主页按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:47]
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CMHAutoPatchDlg::OnBnClickedButtonClose()
{//[关闭按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:41]
	StopTheard = TRUE;
	Sleep(1000);
	if (lpBase)
		UnmapViewOfFile(lpBase);
	if (hMapFile)
		CloseHandle(hMapFile);
	KillTimer(1);
	clinkStop();
	AfxGetMainWnd()->SendMessage(WM_CLOSE);

	//if (::MessageBox(this->m_hWnd, "是否结束游戏?", "提示", MB_YESNO) == MB_OK)
	//{
	//StopTheard = TRUE;
	//Sleep(500);
	//	//Shell_NotifyIcon(NIM_DELETE, &NotifyIcon);
	//	AfxGetMainWnd()->SendMessage(WM_CLOSE);
	//}
	//else
	//{
	//	StopTheard = TRUE;
	//	Sleep(1000);
	//	NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	//	NotifyIcon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//	NotifyIcon.hWnd = m_hWnd;
	//	lstrcpy(NotifyIcon.szTip, "缘起墨香");
	//	NotifyIcon.uCallbackMessage = WM_NC;
	//	NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	//	Shell_NotifyIcon(NIM_ADD, &NotifyIcon);
	//	ShowWindow(SW_HIDE);
	//}
}
void CMHAutoPatchDlg::OnCbnSelchangeComboScreen()
{//[分辨率选择事件][BY:十里坡剑神][QQ:112582793][2019-4-30][23:24:04]
	std::ofstream outFile;
	CString str;
	std::string str1;
	int col;
	int WndMode;
	outFile.open("./AutopatchOption.opt");
	col = m_ComboBox.GetCurSel();
	WndMode = m_CheckWndMode.GetCheck();
	str.Format(L"%d\n%d", WndMode, col + 1);
	str1 = m_FileCheck.m_CodePage.UnicodeToAscii(str.GetBuffer());
	outFile << str1;
	outFile.close();
}

void CMHAutoPatchDlg::RunGame(std::wstring path)
{
	clinkStop();
	hMapFile = CreateFileMappingA(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		64,
		"YuanQiMso");

	lpBase = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 64);

	strcpy_s((char*)lpBase, 64, m_FileCheck.m_CodePage.UnicodeToAscii(m_ServerInfo.CommandLine).c_str());

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	BOOL bStatus = CreateProcess((LPCWSTR)path.c_str(),NULL , NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (bStatus)
	{
		SetTimer(1, 1000,NULL);
		/*if (!DebugActiveProcess(pi.dwProcessId))
		{
			TerminateProcess(pi.hProcess, 0);
			int err = GetLastError();
			CString str;
			str.Format("运行失败!错误代码%d", err);
			AfxMessageBox(str);
			return;
		}
		else
		{*/
			//StopTheard = TRUE;
			//Sleep(500);
			//NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
			//NotifyIcon.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
			//NotifyIcon.hWnd = m_hWnd;
			//lstrcpy(NotifyIcon.szTip, "缘起墨香");
			//NotifyIcon.uCallbackMessage = WM_NC;
			//NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			//Shell_NotifyIcon(NIM_ADD, &NotifyIcon);
			//ShowWindow(SW_HIDE);
			
			/*DEBUG_EVENT DbgEvent = { 0 };
			DWORD dwState = DBG_EXCEPTION_NOT_HANDLED;
			BOOL bExit = FALSE;
			while (!bExit)
			{
				WaitForDebugEvent(&DbgEvent, INFINITE);
				if (DbgEvent.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
				{
					bExit = TRUE;
					StopTheard = TRUE;
					ShowWindow(SW_SHOW);
				}
				ContinueDebugEvent(DbgEvent.dwProcessId, DbgEvent.dwThreadId, dwState);
			}*/
			

		//}

	}
	else
	{
		int err = GetLastError();
		CString str;
		str.Format(L"运行失败!错误代码%d", err);
		AfxMessageBox(str);
		HANDLE hProcess = GetCurrentProcess();//get current process
		TerminateProcess(hProcess, 0);         //close process
	}
}
void CMHAutoPatchDlg::OnBnClickedButtonStart()
{//[开始按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:08]
	OnCbnSelchangeComboScreen();

	std::wstring Patch;
	Patch = m_CurPath;
	Patch += L"\\";
	Patch += m_ServerInfo.ExeName;
	std::thread R(&CMHAutoPatchDlg::RunGame, this, Patch);
	R.join();
	//OnBnClickedButtonClose();
}
void CMHAutoPatchDlg::OnBnClickedButtonHomepage()
{//[主页按钮点击][BY:十里坡剑神][QQ:112582793][2019-4-30][23:27:47]
	ShellExecute(NULL, L"open", m_ServerInfo.HomeUrl.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

// CMHAutoPatchDlg 消息处理程序

BOOL CMHAutoPatchDlg::OnInitDialog()
{
	HANDLE hGameHandle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, NULL, "YQMsoSecond");
	if (hGameHandle != NULL)
	{
		CloseHandle(hGameHandle);
		hGameHandle = NULL;
		MessageBox(L"游戏已运行!", L"提示", MB_OK);
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
		return FALSE;
	}
	//[GDI+初始化][by:十里坡剑神][QQ:112582793][2019-5-2][20:00]
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	CDialogEx::OnInitDialog();
	hMapFile = NULL;
	lpBase = NULL;
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);
	if (!Init())
	{
		MessageBox(L"登录器初始化失败!", L"错误！", MB_OK);
		return FALSE;
	}
	StartWork();
	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMHAutoPatchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		//[绘制背景图片][BY:十里坡剑神][QQ:112582793][2019-4-30][22:31:08]
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		m_bgImag = m_bgImag->FromFile(L".\\Image\\Autopatch\\BackGround.bmp", FALSE);
		CRect rect;
		GetClientRect(&rect);
		Graphics g(dc);
		g.DrawImage(m_bgImag, rect.left, rect.top, rect.right, rect.bottom);
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMHAutoPatchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CMHAutoPatchDlg::LoadDlgInfo()
{//[界面配置加载][BY:十里坡剑神][QQ:112582793][2019-5-2][21:11:17]
	std::string buf;
	CMHFile file;
	if (!file.Init(".\\Image\\Autopatch\\AutoPatchDlg.bin"))
		return false;
	while (1)
	{
		if (file.IsEOF())
			break;
		buf = file.GetString();
		if (buf.compare("#主界面") == 0)
		{
			sDlgRect[eMain].x = file.GetWord();
			sDlgRect[eMain].y = file.GetWord();
			sDlgRect[eMain].cx = file.GetWord();
			sDlgRect[eMain].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#开始按钮") == 0)
		{
			sDlgRect[eStart].x = file.GetWord();
			sDlgRect[eStart].y = file.GetWord();
			sDlgRect[eStart].cx = file.GetWord();
			sDlgRect[eStart].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#注册按钮") == 0)
		{
			sDlgRect[ePay].x = file.GetWord();
			sDlgRect[ePay].y = file.GetWord();
			sDlgRect[ePay].cx = file.GetWord();
			sDlgRect[ePay].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#官网按钮") == 0)
		{
			sDlgRect[eHome].x = file.GetWord();
			sDlgRect[eHome].y = file.GetWord();
			sDlgRect[eHome].cx = file.GetWord();
			sDlgRect[eHome].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#关闭按钮") == 0)
		{
			sDlgRect[eClose].x = file.GetWord();
			sDlgRect[eClose].y = file.GetWord();
			sDlgRect[eClose].cx = file.GetWord();
			sDlgRect[eClose].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#窗口模式") == 0)
		{
			sDlgRect[eWindow].x = file.GetWord();
			sDlgRect[eWindow].y = file.GetWord();
			sDlgRect[eWindow].cx = file.GetWord();
			sDlgRect[eWindow].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#当前进度条") == 0)
		{
			sDlgRect[ePro].x = file.GetWord();
			sDlgRect[ePro].y = file.GetWord();
			sDlgRect[ePro].cx = file.GetWord();
			sDlgRect[ePro].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#当前进度条背景色") == 0)
		{
			sTextColor[eProb].Red = file.GetWord();
			sTextColor[eProb].Green = file.GetWord();
			sTextColor[eProb].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#当前进度条前景色") == 0)
		{
			sTextColor[eProf].Red = file.GetWord();
			sTextColor[eProf].Green = file.GetWord();
			sTextColor[eProf].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#完成进度条") == 0)
		{
			sDlgRect[eProd].x = file.GetWord();
			sDlgRect[eProd].y = file.GetWord();
			sDlgRect[eProd].cx = file.GetWord();
			sDlgRect[eProd].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#完成进度条背景色") == 0)
		{
			sTextColor[eProdb].Red = file.GetWord();
			sTextColor[eProdb].Green = file.GetWord();
			sTextColor[eProdb].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#完成进度条前景色") == 0)
		{
			sTextColor[eProdf].Red = file.GetWord();
			sTextColor[eProdf].Green = file.GetWord();
			sTextColor[eProdf].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#分辨率选择") == 0)
		{
			sDlgRect[eFenbianlv].x = file.GetWord();
			sDlgRect[eFenbianlv].y = file.GetWord();
			sDlgRect[eFenbianlv].cx = file.GetWord();
			sDlgRect[eFenbianlv].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#提示消息") == 0)
		{
			sDlgRect[eTipMsg].x = file.GetWord();
			sDlgRect[eTipMsg].y = file.GetWord();
			sDlgRect[eTipMsg].cx = file.GetWord();
			sDlgRect[eTipMsg].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#下载提示") == 0)
		{
			sDlgRect[eFileMsg].x = file.GetWord();
			sDlgRect[eFileMsg].y = file.GetWord();
			sDlgRect[eFileMsg].cx = file.GetWord();
			sDlgRect[eFileMsg].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#版本号") == 0)
		{
			sDlgRect[eVerMsg].x = file.GetWord();
			sDlgRect[eVerMsg].y = file.GetWord();
			sDlgRect[eVerMsg].cx = file.GetWord();
			sDlgRect[eVerMsg].cy = file.GetWord();
			continue;
		}
		if (buf.compare("#公告") == 0)
		{
			m_HtmlRect.left = file.GetWord();
			m_HtmlRect.top = file.GetWord();
			m_HtmlRect.right = m_HtmlRect.left + file.GetWord();
			m_HtmlRect.bottom = m_HtmlRect.top + file.GetWord();
			continue;
		}
		if (buf.compare("#公告背景色") == 0)
		{
			sTextColor[eWeb].Red = file.GetWord();
			sTextColor[eWeb].Green = file.GetWord();
			sTextColor[eWeb].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#字体颜色") == 0)
		{
			sTextColor[eFont].Red = file.GetWord();
			sTextColor[eFont].Green = file.GetWord();
			sTextColor[eFont].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#版本号颜色") == 0)
		{
			sTextColor[eVer].Red = file.GetWord();
			sTextColor[eVer].Green = file.GetWord();
			sTextColor[eVer].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#提示消息颜色") == 0)
		{
			sTextColor[eTip].Red = file.GetWord();
			sTextColor[eTip].Green = file.GetWord();
			sTextColor[eTip].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#下载提示颜色") == 0)
		{
			sTextColor[eFile].Red = file.GetWord();
			sTextColor[eFile].Green = file.GetWord();
			sTextColor[eFile].Blue = file.GetWord();
			continue;
		}
		if (buf.compare("#字体") == 0)
		{
			sFontSet.Size = file.GetWord();
			_tcscpy_s(sFontSet.FontName,64, file.GetStringW().c_str());
			continue;
		}
	}
	file.Release();
	return true;
}
bool CMHAutoPatchDlg::LoadServerInfo()
{//[加载更新配置文件][BY:十里坡剑神][QQ:112582793][2019-4-30][21:53:29]
	std::string  buf;
	CMHFile file;
	if (!file.Init(UPDATAURL))
		return false;

	while (!file.IsEOF())
	{
		buf = file.GetString();
		if (buf.compare("*UpUrl") == 0)
		{

			m_ServerInfo.UpdataUrl = file.GetStringW();
			continue;
		}
		if (buf.compare("*HomePage") == 0)
		{
			m_ServerInfo.HomeUrl = file.GetStringW();
			continue;
		}
		if (buf.compare("*PayUrl") == 0)
		{
			m_ServerInfo.PayUrl = file.GetStringW();
			continue;
		}
		if (buf.compare("*QQ") == 0)
		{
			m_ServerInfo.QQ = file.GetStringW();
			continue;
		}
		if (buf.compare("*Line") == 0)
		{
			m_ServerInfo.CommandLine = file.GetStringW();
			continue;
		}
		if (buf.compare("*ExeName") == 0)
		{
			m_ServerInfo.ExeName = file.GetStringW();
			continue;
		}
		if (buf.compare("*LunchVer") == 0)
		{
			m_ServerInfo.LunchVer = file.GetStringW();
			continue;
		}
		if (buf.compare("*GGUrl") == 0)
		{
			m_ServerInfo.GGUrl = file.GetStringW();
			continue;
		}
		if (buf.compare("*UpLuchUrl") == 0)
		{
			m_ServerInfo.UpLuchUrl = file.GetStringW();
			continue;
		}
		if (buf.compare("*LunchName") == 0)
		{
			m_ServerInfo.LunchName = file.GetStringW();
			continue;
		}
	}
	file.Release();
	return true;
}
void CMHAutoPatchDlg::DrawDialog()
{
	//开始按钮
	m_BtnStart.LoadBitmaps(IDB_BITMAP_START_U, IDB_BITMAP_START_D, IDB_BITMAP_START_U, IDB_BITMAP_START_X);
	m_BtnStart.SubclassDlgItem(IDC_BUTTON_START, this);
	m_BtnStart.SizeToContent();
	m_BtnStart.SetWindowPos(NULL, sDlgRect[eStart].x, sDlgRect[eStart].y, sDlgRect[eStart].cx, sDlgRect[eStart].cy, SWP_SHOWWINDOW);

	//官网按钮
	m_BtnHomePage.LoadBitmaps(IDB_BITMAP_HOME_U, IDB_BITMAP_HOME_D, IDB_BITMAP_HOME_U);
	m_BtnHomePage.SubclassDlgItem(IDC_BUTTON_HOMEPAGE, this);
	m_BtnHomePage.SizeToContent();
	m_BtnHomePage.SetWindowPos(NULL, sDlgRect[eHome].x, sDlgRect[eHome].y, sDlgRect[eHome].cx, sDlgRect[eHome].cy, SWP_SHOWWINDOW);

	//关闭按钮
	m_BtnClose.LoadBitmaps(IDB_BITMAP_CLOSE_U, IDB_BITMAP_CLOSE_D);
	m_BtnClose.SubclassDlgItem(IDC_BUTTON_CLOSE, this);
	m_BtnClose.SizeToContent();
	m_BtnClose.SetWindowPos(NULL, sDlgRect[eClose].x, sDlgRect[eClose].y, sDlgRect[eClose].cx, sDlgRect[eClose].cy, SWP_SHOWWINDOW);

	//窗口模式
	m_CheckWndMode.SetWindowPos(NULL, sDlgRect[eWindow].x, sDlgRect[eWindow].y, sDlgRect[eWindow].cx, sDlgRect[eWindow].cy, SWP_SHOWWINDOW);
	m_CheckWndMode.SetCheck(TRUE);
	//当前进度条
	m_Progress.SetWindowPos(NULL, sDlgRect[ePro].x, sDlgRect[ePro].y, sDlgRect[ePro].cx, sDlgRect[ePro].cy, SWP_SHOWWINDOW);
	m_Progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(sTextColor[eProb].Red, sTextColor[eProb].Green, sTextColor[eProb].Blue));
	m_Progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(sTextColor[eProf].Red, sTextColor[eProf].Green, sTextColor[eProf].Blue));
	m_Progress.ShowWindow(SW_SHOW);
	//完成进度条
	m_ProgressDone.SetWindowPos(NULL, sDlgRect[eProd].x, sDlgRect[eProd].y, sDlgRect[eProd].cx, sDlgRect[eProd].cy, SWP_SHOWWINDOW);
	m_ProgressDone.SendMessage(PBM_SETBKCOLOR, 0, RGB(sTextColor[eProdb].Red, sTextColor[eProdb].Green, sTextColor[eProdb].Blue));
	m_ProgressDone.SendMessage(PBM_SETBARCOLOR, 0, RGB(sTextColor[eProdf].Red, sTextColor[eProdf].Green, sTextColor[eProdf].Blue));
	m_ProgressDone.ShowWindow(SW_SHOW);
	//分辨率选择
	m_ComboBox.SetWindowPos(NULL, sDlgRect[eFenbianlv].x, sDlgRect[eFenbianlv].y, sDlgRect[eFenbianlv].cx, sDlgRect[eFenbianlv].cy, SWP_SHOWWINDOW);
	//提示消息
	m_StTip.SetWindowPos(NULL, sDlgRect[eTipMsg].x, sDlgRect[eTipMsg].y, sDlgRect[eTipMsg].cx, sDlgRect[eTipMsg].cy, SWP_SHOWWINDOW);
	m_StTip.RedrawWindow();
	//下载提示
	m_StDwnlTip.SetWindowPos(NULL, sDlgRect[eFileMsg].x, sDlgRect[eFileMsg].y, sDlgRect[eFileMsg].cx, sDlgRect[eFileMsg].cy, SWP_SHOWWINDOW);
	m_StDwnlTip.RedrawWindow();
	//[版本号][BY:十里坡剑神][QQ:112582793][2019-5-3][1:18:56]
	m_AutoPatchVer.SetWindowPos(NULL, sDlgRect[eVerMsg].x, sDlgRect[eVerMsg].y, sDlgRect[eVerMsg].cx, sDlgRect[eVerMsg].cy, SWP_SHOWWINDOW);
	m_AutoPatchVer.RedrawWindow();
	m_StTip.SetWindowText(L"");
	m_StDwnlTip.SetWindowText(L"");
	m_AutoPatchVer.SetWindowText(AutoPatchVer);
	//[绘制公告窗口][BY:十里坡剑神][QQ:112582793][2019-4-30][22:34:35]
	mHtmlViewNotice->Create(
		0,
		0,
		WS_CHILD,
		m_HtmlRect,
		this,
		IDC_HTML_VIEW
	);
	mHtmlViewNotice->Navigate2(m_ServerInfo.GGUrl.c_str());
	const int htmlViewBorder = 2;
	mHtmlViewNotice->SetLeft(
		-htmlViewBorder);
	mHtmlViewNotice->SetTop(
		-htmlViewBorder);
	mHtmlViewNotice->SetWidth(
		mHtmlViewNotice->GetWidth() + htmlViewBorder);
	mHtmlViewNotice->SetHeight(
		mHtmlViewNotice->GetHeight() + htmlViewBorder);
	//[处理背景图片透明色][BY:十里坡剑神][QQ:112582793][2019-4-30][22:34:44]
	COLORREF TransColor = RGB(255, 255, 255);//白色为不显示的部分
	CBitmap cBitmap;
	cBitmap.LoadBitmap(IDB_BITMAP7);//加载描述窗口形状的BMP图片
	CDC* pDC = this->GetDC();
	CDC memDC;//为什么要创建，是不是可以直接设置当前控件的CRgn
	memDC.CreateCompatibleDC(pDC);//创建与传入DC兼容的临时DC
	CBitmap *pOldMemBmp = NULL;
	pOldMemBmp = memDC.SelectObject(&cBitmap);//将位图选入临时DC，方便对图片在控件环境中进行操作
	static CRgn wndRgn;
	wndRgn.CreateRectRgn(0, 0, 0, 0);//创建总的窗体区域，初始region为0
	BITMAP bit;
	cBitmap.GetBitmap(&bit);//取得位图参数，这里要用到位图的长和宽
	int y;
	CRgn rgnTemp; //保存临时region
	for (y = 0; y <= bit.bmHeight; y++)
	{
		int iX = 0;
		do
		{
			//跳过透明色找到下一个非透明色的点.
			while (iX <= bit.bmWidth && memDC.GetPixel(iX, y) != TransColor)
				iX++;
			//记住这个起始点
			int iLeftX = iX;
			//寻找下个透明色的点
			while (iX <= bit.bmWidth && memDC.GetPixel(iX, y) == TransColor)
				++iX;
			//创建一个包含起点与重点间高为1像素的临时“region”
			rgnTemp.CreateRectRgn(iLeftX, y, iX, y + 1);
			//合并到主"region".
			wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR);
			//删除临时"region",否则下次创建时和出错
			rgnTemp.DeleteObject();
		} while (iX < bit.bmWidth);
	}
	this->SetWindowRgn(wndRgn, TRUE);
	memDC.DeleteDC();
	//[创建字体][BY:十里坡剑神][QQ:112582793][2019-4-30][22:53:34]
	m_font.CreatePointFont(sFontSet.Size, sFontSet.FontName);
	//[设置字体颜色][BY:十里坡剑神][QQ:112582793][2019-4-30][22:53:42]
	hbr = CreateSolidBrush(RGB(sTextColor[eTip].Red, sTextColor[eTip].Green, sTextColor[eTip].Blue));
	hbr2 = CreateSolidBrush(RGB(sTextColor[eVer].Red, sTextColor[eVer].Green, sTextColor[eVer].Blue));
	hbr1 = CreateSolidBrush(RGB(sTextColor[eFile].Red, sTextColor[eFile].Green, sTextColor[eFile].Blue));
	//[设置初试窗口大小][BY:十里坡剑神][QQ:112582793][2019-4-30][22:54:16]
	CWnd::SetWindowPos(NULL, sDlgRect[eMain].x, sDlgRect[eMain].y, sDlgRect[eMain].cx, sDlgRect[eMain].cy, 0);

	//[启动后开始游戏按钮默认激活][BY:十里坡剑神][QQ:112582793][2019-4-30][22:54:32]
	m_BtnStart.EnableWindow(FALSE);
	GetCurrentDirectory(MAX_PATH, m_CurPath);

	InitComBox();
	//[获取启动参数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:49:40]
	std::wstring szCommd = GetCommandLine();
	size_t i = -1;
	i = szCommd.find(L"RunClient");
	if (i != -1)
		m_bIsRunClient = true;
}

void CMHAutoPatchDlg::InitComBox()
{//[初始化分辨率选择][BY:十里坡剑神][QQ:112582793][2019-4-30][23:04:11]
	char buf[64], buf2[64];
	std::wstring str;
	std::ofstream ofn("./PrintScreenList.txt");
	MonitorAdapter          m_monitorAdapter;
	VEC_MONITORMODE_INFO vecMointorListInfo;
	std::vector<info>* a;
	m_monitorAdapter.GetAllDisplayMode(vecMointorListInfo);
	int nWidth = 0, nHeight = 0, nFreq = 0, nBits = 0;
	VEC_MONITORMODE_INFO::iterator itBeg = vecMointorListInfo.begin();
	a = m_monitorAdapter.GetInfo();
	info n;
	int i = 0;
	for (vector<info>::iterator iter1 = a->begin(); iter1 != a->end(); iter1++)
	{
		n = *iter1;
		sprintf_s(buf, "%d*%d", n.m_nWidth, n.m_nHeight);
		str = m_FileCheck.m_CodePage.AsciiToUnicode(buf);
		m_ComboBox.InsertString(i, str.c_str());
		sprintf_s(buf2, "%d\t%d\t%d", i + 1, n.m_nWidth, n.m_nHeight);
		ofn << buf2 << std::endl;
		i++;
	}
	ofn.close();
	std::ifstream Infile;
	Infile.open("AutopatchOption.opt");
	int index = 0;
	Infile >> index;
	Infile >> index;
	Infile.close();
	m_ComboBox.SetCurSel(index - 1);
}
bool CMHAutoPatchDlg::Init()
{//[初始化登录器][BY:十里坡剑神][QQ:112582793][2019-4-30][22:22:40]
	bool bIsInit = false;

	//[初始化安卫士防护][BY:十里坡剑神][QQ:112582793][2019-4-30][23:47:00]
	int ret = clinkStart(ClinkKey.c_str());
	if (ret != 150)
	{
		MessageBox(L"验证服务器连接失败!", L"错误!", MB_OK);
		bIsInit = false;
	}

	bIsInit = LoadServerInfo();
	if (!bIsInit)
		MessageBox(L"配置列表加载失败!", L"错误", MB_OK);
	bIsInit = LoadDlgInfo();
	if (!bIsInit)
		MessageBox(L"界面配置加载失败!", L"错误", MB_OK);
	if (bIsInit)
		DrawDialog();
	return bIsInit;
}
void CMHAutoPatchDlg::StartWork()
{
	char buf[512] = { 0, };
	std::string str;
	if (m_ServerInfo.LunchVer.compare(AutoPatchVer) != 0)
	{
		MessageBox(L"登录器需要更新，请稍等...", L"提示", MB_OK);
		UpDataLunch();
		return;
	}

	if (!m_FileCheck.GetMyUpData(m_ServerInfo.UpdataUrl + L"/MyUpdata.txt"))
		return;
	else
	{
		m_StTip.SetWindowText(L"文件校验中...");
		std::thread CheckFileThread(&CMHAutoPatchDlg::StartCheckFile, this);
		CheckFileThread.detach();
		return;
	}
}
//[下载业务相关函数][BY:十里坡剑神][QQ:112582793][2019-5-2][21:27:30]
std::wstring CMHAutoPatchDlg::MakePath(std::wstring& str)
{
	std::wstring tmp = m_CurPath;
	tmp += str;
	return tmp;
}
std::wstring  CMHAutoPatchDlg::MakeUrl(std::wstring& str)
{
	std::wstring tmp = m_ServerInfo.UpdataUrl;
	TCHAR buf[32];
	time_t nRand = time(NULL);

	swprintf_s(buf, L"?abc=%I64d", nRand);//生成随机参数，防止读取cookie
	m_TextPars.replace_all(str, L"\\", L"/");
	tmp += (str + buf);
	return tmp;
}
std::wstring  CMHAutoPatchDlg::GetFileName(std::wstring& str)
{
	std::wstring result = str;
	size_t Pos = result.find_last_of(L"\\");
	result.replace(0, Pos + 1, L"");
	return result;
}
void CMHAutoPatchDlg::MakeDownloadInfo(std::wstring &str)
{
	std::wstring DwonLoadFileInfo;
	DwonLoadFileInfo = L"KB";
	str += L"文件:%s";
	str += L" ";


	str += L"大小:%d";
	str += L" ";
	str += DwonLoadFileInfo;
	str += L" ";

	str += L"剩余:%d";
	str += L" ";
	str += DwonLoadFileInfo;
	str += L" ";
}
std::wstring CMHAutoPatchDlg::Ansi2WChar(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[nSize + 1];
	if (NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF)
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	wstring wcharString(pwszDst);
	delete pwszDst;
	return wcharString;
}
void CMHAutoPatchDlg::StartDownload()
{
	CString str;
	m_ProgressDone.SetRange32(0, int(m_FileCount));
	int FileNum = int(m_FileCount);
	while (m_WaitDownlaodFileList.size())
	{
		if (StopTheard)return;
		str.Format(L"需下载数[%d]", m_FileCount);
		m_StTip.SetWindowText(str);
		//[下载信息生成][BY:十里坡剑神][QQ:112582793][2019-5-2][22:18:41]
		DownLoadInfo info;
		info.Path = MakePath(m_WaitDownlaodFileList.back());
		info.Url = MakeUrl(m_WaitDownlaodFileList.back());
		//[去掉已下载元素][BY:十里坡剑神][QQ:112582793][2019-5-2][22:19:00]
		if (m_WaitDownlaodFileList.size())
			m_WaitDownlaodFileList.pop_back();
		//[总进度条设置][BY:十里坡剑神][QQ:112582793][2019-5-2][22:19:16]
		m_FileCount--;
		m_ProgressDone.SetPos(FileNum - int(m_FileCount));
		//[下载文件][BY:十里坡剑神][QQ:112582793][2019-5-2][22:19:37]
		std::thread DownloadFileThread(&CMHAutoPatchDlg::DownloadFile, this, info);
		DownloadFileThread.join();
	}
	m_StTip.SetWindowText(L"完成");
	m_StDwnlTip.SetWindowText(L"完成");
	m_Progress.SetRange32(0, 1);
	m_Progress.SetPos(1);
	if (!m_bIsRunClient)
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	else
		OnBnClickedButtonStart();
	return;
}
void CMHAutoPatchDlg::StartCheckFile()
{

	m_FileCheck.CheckFile(m_WaitDownlaodFileList);//文件校验
	if (m_WaitDownlaodFileList.size() > 0)
	{
		m_FileCount = m_WaitDownlaodFileList.size();
		m_StTip.SetWindowText(L"开始下载");
		std::thread DownLoadThread(&CMHAutoPatchDlg::StartDownload, this);
		DownLoadThread.join();
	}
	else
	{
		m_ProgressDone.SetRange32(0, 1);
		m_ProgressDone.SetPos(1);

		m_Progress.SetRange32(0, 1);
		m_Progress.SetPos(1);

		m_StTip.SetWindowText(L"完成");
		if (!m_bIsRunClient)
			m_BtnStart.EnableWindow(TRUE);
		else
			OnBnClickedButtonStart();
	}
}
void CMHAutoPatchDlg::DownloadFile(DownLoadInfo info)
{
	std::wstring DownLoadFileInfo;
	CString str;
	MakeDownloadInfo(DownLoadFileInfo);
	std::wstring FileName = GetFileName(info.Path);

	CInternetSession Sess(L"lpload");
	Sess.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, 20000); //20秒的连接超时  
	Sess.SetOption(INTERNET_OPTION_SEND_TIMEOUT, 20000); //20秒的发送超时  
	Sess.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, 20000); //20秒的接收超时  
	Sess.SetOption(INTERNET_OPTION_DATA_SEND_TIMEOUT, 20000); //20秒的发送超时  
	Sess.SetOption(INTERNET_OPTION_DATA_RECEIVE_TIMEOUT, 20000); //20秒的接收超时  
	DWORD dwFlag = INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD;
	CHttpFile* cFile = NULL;
	char      *pBuf = NULL;
	int        nBufLen = 0;
	do {
		try {
			cFile = (CHttpFile*)Sess.OpenURL(info.Url.c_str(), 1, dwFlag);
			DWORD dwStatusCode;
			cFile->QueryInfoStatusCode(dwStatusCode);
			if (dwStatusCode == HTTP_STATUS_OK) {
				//[查询文件长度][BY:十里坡剑神][QQ:112582793][2019-5-2][21:40:34]
				DWORD nLen = 0;
				cFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, nLen);

				m_Progress.SetRange32(0, nLen);
				nBufLen = nLen;
				
				str.Format(DownLoadFileInfo.c_str(), FileName.c_str(), nBufLen / 1024, nLen / 1024);
				m_StDwnlTip.SetWindowText(str);
				if (nLen <= 0) break;

				//[分配接收数据缓存][BY:十里坡剑神][QQ:112582793][2019-5-2][21:40:26] 
				pBuf = new char[nLen + 8];
				ZeroMemory(pBuf, nLen + 8);

				char *p = pBuf;
				while (nLen > 0) {
					if (StopTheard)
					{
						goto lReturn;
					}
					//[每次下载8K ][BY:十里坡剑神][QQ:112582793][2019-5-2][21:38:54]
					int n = cFile->Read(p, (nLen < RECVPACK_SIZE) ? nLen : RECVPACK_SIZE);
					//[接收完成退出循环 ][BY:十里坡剑神][QQ:112582793][2019-5-2][21:38:49]
					if (n <= 0) break;
					//[接收缓存后移][BY:十里坡剑神][QQ:112582793][2019-5-2][21:38:40]				
					p += n;
					//[剩余长度递减  ][BY:十里坡剑神][QQ:112582793][2019-5-2][21:38:29]
					nLen -= n;
					m_Progress.SetPos(nBufLen - nLen);
					str.Format(DownLoadFileInfo.c_str(), FileName.c_str(), nBufLen / 1024, nLen / 1024);
					m_StDwnlTip.SetWindowText(str);
				}
				//[如果未接收完中断退出][BY:十里坡剑神][QQ:112582793][2019-5-2][21:39:12]
				if (nLen != 0) break;

				//[接收成功保存到文件][BY:十里坡剑神][QQ:112582793][2019-5-2][21:39:20]
				CFile file(info.Path.c_str(), CFile::modeCreate | CFile::modeWrite);
				file.Write(pBuf, nBufLen);
				file.Close();
			}
		}
		catch (...) {
			str.Format(L"文件[%s]下载出错！！！",FileName);
			m_StTip.SetWindowText(str);
			break;//  
		}
	} while (0);
lReturn:
	//[释放缓存][BY:十里坡剑神][QQ:112582793][2019-5-2][21:39:29]
	if (pBuf) {
		delete[]pBuf;
		pBuf = NULL;
		nBufLen = 0;
	}

	//[关闭下载连接][BY:十里坡剑神][QQ:112582793][2019-5-2][21:39:35]
	if (cFile) {
		cFile->Close();
		Sess.Close();
		delete cFile;
	}
}
void CMHAutoPatchDlg::UpDataLunch()
{
	std::wstring Patch, Commandl;

	Patch = m_CurPath;
	Patch += L"\\Image\\Autopatch\\UpDataLuch.exe";

	Commandl = m_ServerInfo.UpLuchUrl;
	Commandl += L"/";
	Commandl += m_ServerInfo.LunchName;
	Commandl += L"|";
	Commandl += m_CurPath;
	Commandl += L"\\";
	Commandl += m_ServerInfo.LunchName;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	if (CreateProcess((LPCWSTR)Patch.c_str(), (LPWSTR)Commandl.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		AfxMessageBox(L"登录器更新进程启动失败!");
		HANDLE hProcess = GetCurrentProcess();//get current process
		TerminateProcess(hProcess, 0);         //close process
	}
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
void CMHAutoPatchDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		char szBuffer[64] = { 0 };
		strcpy_s(szBuffer, 64, (char*)lpBase);
		if (strcmp(szBuffer, "KWAsl0") == 0)
		{
			StopTheard = TRUE;
			Sleep(1000);
			UnmapViewOfFile(lpBase);
			lpBase = NULL;
			CloseHandle(hMapFile);
			hMapFile = NULL;
			KillTimer(1);
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
		}
		
	}

	CDialogEx::OnTimer(nIDEvent);
}
