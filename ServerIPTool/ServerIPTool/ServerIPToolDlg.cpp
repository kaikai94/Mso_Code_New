
// ServerIPToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerIPTool.h"
#include "ServerIPToolDlg.h"
#include "afxdialogex.h"
#include "SimpleCodec.h"
#include <string>
#include <fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerIPToolDlg 对话框



CServerIPToolDlg::CServerIPToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerIPToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerIPToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	m_SelChrList = (CListCtrl*)GetDlgItem(IDC_LIST1);
	CRect rect1;
	m_SelChrList->GetClientRect(&rect1);
	m_SelChrList->SetExtendedStyle(m_SelChrList->GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_SelChrList->InsertColumn(0, _T("类型"), LVCFMT_CENTER, rect1.Width() / 14, 0);
	m_SelChrList->InsertColumn(1, _T("编号"), LVCFMT_CENTER, rect1.Width() / 11, 1);
	m_SelChrList->InsertColumn(2, _T("服务IP"), LVCFMT_CENTER, rect1.Width() / 5, 2);
	m_SelChrList->InsertColumn(3, _T("服务端口"), LVCFMT_CENTER, rect1.Width() / 9, 3);
	m_SelChrList->InsertColumn(4, _T("用户IP"), LVCFMT_CENTER, rect1.Width() / 5, 4);
	m_SelChrList->InsertColumn(5, _T("用户端口"), LVCFMT_CENTER, rect1.Width() / 9, 5);
	m_SelChrList->InsertColumn(6, _T("连接限制"), LVCFMT_CENTER, rect1.Width() / 9, 6);
	m_SelChrList->InsertColumn(7, _T("连接编号"), LVCFMT_CENTER, rect1.Width() / 9, 7);
}

BEGIN_MESSAGE_MAP(CServerIPToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CServerIPToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CServerIPToolDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CServerIPToolDlg 消息处理程序

BOOL CServerIPToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerIPToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerIPToolDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerIPToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerIPToolDlg::OnBnClickedButton1()
{
	CFileDialog dlg(true, NULL, "*.list", NULL,
		"列表文件|*.list||", this);
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();
		if (str != "")
		{
			Load(str.GetBuffer());
			Show();
			SaveText(str.GetBuffer());
		}
		else
		{
			::MessageBox(NULL, "请选择要导出的文件", "提示", MB_OK);
		}
	}
}

void CServerIPToolDlg::OnBnClickedButton2()
{
	CFileDialog dlg(true, NULL, "*.txt", NULL,
		"文本文档|*.txt||", this);
	if (dlg.DoModal() == IDOK)
	{
		CString str = dlg.GetPathName();
		if (str != "")
		{
			ReadText(str.GetBuffer());
			Show();
			Save(str.GetBuffer());
		}
		else
		{
			::MessageBox(NULL, "请选择要导出的文件", "提示", MB_OK);
		}
	}
}
void CServerIPToolDlg::Show()
{
	m_SelChrList->DeleteAllItems();
	CString str;
	char buf[64] = { 0 }; 
	std::vector<SERVERINFO*>::iterator iter;
	iter = m_ServerList.begin();
	int i = 0;
	while (true)
	{
		if (iter == m_ServerList.end())
			break;
		else
		{
			str.Format("%d", (*iter)->wServerKind);
			m_SelChrList->InsertItem(i, str);
			str.Format("%d", (*iter)->wServerNum);
			m_SelChrList->SetItemText(i,1, str);

			m_SelChrList->SetItemText(i, 2, (*iter)->szIPForServer);

			str.Format("%d", (*iter)->wPortForServer);
			m_SelChrList->SetItemText(i, 3, str);

			m_SelChrList->SetItemText(i, 4, (*iter)->szIPForUser);

			str.Format("%d", (*iter)->wPortForUser);
			m_SelChrList->SetItemText(i, 5, str);

			str.Format("%d", (*iter)->wAgentUserCnt);
			m_SelChrList->SetItemText(i, 6, str);

			str.Format("%d", (*iter)->dwConnectionIndex);
			m_SelChrList->SetItemText(i, 7, str);
		}
		iter++;
		i++;
	}
}
void CServerIPToolDlg::clear()
{
	std::vector<SERVERINFO*>::iterator iter;
	iter = m_ServerList.begin();
	while (1)
	{
		if (iter == m_ServerList.end())
			break;
		else
		{
			delete *iter;
			*iter = NULL;
		}
		iter++;
	}
	m_ServerList.clear();
}
void CServerIPToolDlg::Add(SERVERINFO* pInfo)
{
	SERVERINFO* pNewServerInfo = new SERVERINFO;
	memcpy(pNewServerInfo, pInfo, sizeof(SERVERINFO));
	m_ServerList.push_back(pNewServerInfo);
}
BOOL CServerIPToolDlg::Save(char* pFileName)
{
	std::string str(pFileName);
	str.replace(str.length() - 4, str.length(), "");
	str += ".list";
	BOOL rt;
	std::vector<SERVERINFO*>::iterator iter;
	iter = m_ServerList.begin();
	CSimpleCodec codec((char*)str.c_str(), TRUE, "djjGWskl6crA0FKa");
	rt = codec.Encode(&m_ServerSetNum, sizeof(m_ServerSetNum));
	while (true)
	{
		if (iter == m_ServerList.end()||rt==FALSE)
			break;
		else
		{
			rt = codec.Encode(*iter, sizeof(SERVERINFO));
		}
		iter++;
	}
	return TRUE;
}

BOOL CServerIPToolDlg::Load(char* pFileName)
{
	clear();
	CSimpleCodec codec(pFileName, FALSE, "djjGWskl6crA0FKa");
	codec.Decode(&m_ServerSetNum, sizeof(m_ServerSetNum));
	SERVERINFO info;
	while (codec.Decode(&info, sizeof(SERVERINFO)))
	{
		if (info.wServerKind >= 10)
		{
			::MessageBox(NULL, "不存在的服务器类型", "提示", MB_OK);
		}
		Add(&info);
	}
	return TRUE;
}
BOOL CServerIPToolDlg::ReadText(char* pFileName)
{
	std::ifstream Infile(pFileName);
	clear();
	
	Infile>>m_ServerSetNum;
	SERVERINFO info;
	while (Infile>>info.wServerKind)
	{
		Infile >> info.wServerNum;
		Infile >> info.szIPForServer;
		Infile >> info.wPortForServer;
		Infile >> info.szIPForUser;
		Infile >> info.wPortForUser;
		Infile >> info.dwConnectionIndex;
		Infile >> info.wAgentUserCnt;
		Add(&info);
	}

	return TRUE;
}
BOOL CServerIPToolDlg::SaveText(char* pFileName)
{
	std::string str(pFileName);
	str.replace(str.length() - 5, str.length(), "");
	str += ".txt";
	std::ofstream OutFile(str.c_str());
	OutFile << m_ServerSetNum << std::endl;
	
	std::vector<SERVERINFO*>::iterator iter;
	SERVERINFO info;
	iter = m_ServerList.begin();
		while (1)
		{
			if (iter == m_ServerList.end())
				break;
			else
			{
				OutFile << (*iter)->wServerKind << "\t" << (*iter)->wServerNum << "\t" << (*iter)->szIPForServer
					<< "\t" << (*iter)->wPortForServer << "\t" << (*iter)->szIPForUser << "\t" << (*iter)->wPortForUser
					<< "\t" << (*iter)->dwConnectionIndex<< "\t" << (*iter)->wAgentUserCnt << std::endl;
			}
			iter++;
		}
		OutFile.close();

	return TRUE;
}