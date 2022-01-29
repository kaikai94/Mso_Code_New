// PatchMakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PatchMaker.h"
#include "PatchMakerDlg.h"
#include "cTextParsing.h"
#include "MD5Checksum.h"
#include <thread>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPatchMakerDlg 对话框
UINT GetFileInfo(LPVOID pParam);
TCHAR sSelectPath[MAX_PATH]; 
cTextParsing TextPars;

CPatchMakerDlg::CPatchMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchMakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPatchMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPatchMakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPatchMakerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPatchMakerDlg 消息处理程序

BOOL CPatchMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPatchMakerDlg::OnPaint()
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
HCURSOR CPatchMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPatchMakerDlg::OnBnClickedButton1()
{
	char szPath[MAX_PATH];     //存放选择的目录路径 
	ZeroMemory(szPath, sizeof(szPath));   
	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "请选择需要打包的目录：";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{
		sprintf_s(sSelectPath,szPath);
		GetDlgItem(IDC_EDIT1)->SetWindowText(szPath);
		//CWinThread* pThread = AfxBeginThread(,NULL,THREAD_PRIORITY_NORMAL);
		std::thread t(GetFileInfo);
		t.detach();
	}
	else   
		AfxMessageBox("无效的目录，请重新选择"); 
}
UINT GetFileInfo(LPVOID pParam)
{
	((CListBox*)AfxGetMainWnd()->GetDlgItem(IDC_LIST1))->ResetContent();
	setlocale(LC_ALL,"Chinese-simplified");
	std::vector<std::string> file;
	std::string strMD5;
	std::string strPath;
	std::string MyUpdatapath;
	MyUpdatapath = sSelectPath;
	MyUpdatapath +="\\";
	MyUpdatapath += "MyUpdata.txt";
	DeleteFile(MyUpdatapath.c_str());
	TextPars.getFilesAll(sSelectPath,file);
	std::ofstream ofn(MyUpdatapath.c_str());
	std::vector<std::string> filePathdone;
	CString str;
	int b;
	str.Format("共[%d]个补丁文件",file.size());
	AfxGetMainWnd()->SetDlgItemText(IDC_STATIC,str);
	for (size_t i = 0;i<file.size();i++)
	{
		strMD5=(CMD5Checksum::GetMD5(file[i].c_str())).GetBuffer();
		strPath=(file[i].replace(file[i].begin(),file[i].begin()+lstrlen(sSelectPath),"")).c_str();
		b=((CListBox*)AfxGetMainWnd()->GetDlgItem(IDC_LIST1))->AddString(strPath.c_str());
		((CListBox*)AfxGetMainWnd()->GetDlgItem(IDC_LIST1))->SetCurSel(b);
		strPath += "|";
		strPath +=strMD5;
		strPath	+="#";
		ofn<<strPath<<std::endl;
		str.Format("共[%d]个补丁文件,已处理[%d]个",file.size(),i+1);
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC,str);
	}
	ofn.close();
	str.Format("共[%d]个补丁文件,处理完毕",file.size());
	AfxGetMainWnd()->SetDlgItemText(IDC_STATIC,str);
	return 0;
}

