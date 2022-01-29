// UpDataLuchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpDataLuch.h"
#include "UpDataLuchDlg.h"
#include <afxinet.h>
#include "MHFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpDataLuchDlg 对话框
UINT DownloadFile(LPVOID pParam);
std::string  MakeUrl(std::string& str);
void RunLunch();
void GetUrl(std::string str);
BOOL IsDone;
BOOL IsNotDone;
char CurPath[MAX_PATH];
std::string DownloadUrl;
struct  Info
{
	std::string Path;
	std::string Url;
};
Info WorkInfo;
Info BmpInfo;
std::vector<std::string> split(std::string& str,const char* c);
std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value) ;    

CUpDataLuchDlg::CUpDataLuchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpDataLuchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUpDataLuchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUpDataLuchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CUpDataLuchDlg 消息处理程序

BOOL CUpDataLuchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nPos = GetCurrentDirectory( MAX_PATH, CurPath);
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1,5000,NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpDataLuchDlg::OnPaint()
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
HCURSOR CUpDataLuchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT DownloadFile(LPVOID pParam)
{
	Info* info = (Info*)pParam;
	CWnd*			pwnd = AfxGetMainWnd();
	//CProgressCtrl*	m_Prog = (CProgressCtrl*)pwnd->GetDlgItem(IDC_PROGRESS);

	char				filebuf[512];
	CInternetSession	netSession;
	CStdioFile			*fTargFile;
	int					outfs;
	std::string			FileSize,KBin,KBsec,NewName,Perc;
	char buf[32];
	try
	{
		fTargFile = netSession.OpenURL(info->Url.c_str(),1,INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_RELOAD);

		COleDateTime dlStart = COleDateTime::GetCurrentTime();
		int filesize = fTargFile->SeekToEnd();
		fTargFile->SeekToBegin();
		outfs = filesize / 1024;		// 计算文件大小（千字节）

		sprintf_s(buf,"%d",outfs);
		FileSize+=buf;	// 以KB为单位格式文件大小

		CFile fDestFile(info->Path.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

		int byteswrite;		// 写入文件的字节数
		int pos = 0;		// 当前进度条的位置
		int nperc,kbrecv;	// 进度条的百分比,获取到的数据大小（Kbs为单位）
		double secs,kbsec;	// 记录秒数, 速度（KB/秒）

		//m_Prog->SetRange32(0,filesize);

		while (byteswrite = fTargFile->Read(filebuf, 512))	// 读取文件
		{
			// 根据开始时间与当前时间比较，获取秒数
			COleDateTimeSpan dlElapsed = COleDateTime::GetCurrentTime() - dlStart;
			secs = dlElapsed.GetTotalSeconds();
			pos = pos + byteswrite;					// 设置新的进度条位置
			fDestFile.Write(filebuf, byteswrite);	// 将实际数据写入文件
			//m_Prog->SetPos(pos);

			nperc = pos * 100 / filesize;			// 进度百分比
			kbrecv = pos / 1024;					// 获取收到的数据
			kbsec = kbrecv / secs;					// 获取每秒下载多少（KB）
		}
		// 下载完成，关闭文件
		fDestFile.Close();
		IsDone = TRUE;
	}

	catch(CInternetException *IE)
	{
		CString strerror;
		TCHAR error[255];
		IE->GetErrorMessage(error,255); // 获取错误消息
		pwnd->SetDlgItemText(IDC_STATIC_TIP,error);
		IsNotDone = TRUE;
	}
	return 0;
}
std::string  MakeUrl(std::string& str)
{
	char buf[32];
	time_t nRand = time(NULL);
	sprintf_s(buf,"?abc=%d",nRand);//生成随机参数，防止读取cookie
	str +=buf;
	replace_all(str,"\\","/");
	return str;
}
void CUpDataLuchDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case 1:
		{
			KillTimer(1);
			std::string tmp = GetCommandLine();
			//AfxMessageBox(tmp.c_str());
			std::vector<std::string> filePathdone1=split(tmp,"|");
			WorkInfo.Path = filePathdone1[1];
			WorkInfo.Url = MakeUrl(filePathdone1[0]);
			GetUrl(WorkInfo.Url);
			IsDone = FALSE;
			IsNotDone = FALSE;
			AfxBeginThread(DownloadFile,&WorkInfo,THREAD_PRIORITY_NORMAL);

			SetTimer(2,3000,NULL);
		}
		break;
	case 2:
		{
			KillTimer(2);
			BmpInfo.Path = CurPath;
			BmpInfo.Path += "\\Image\\Autopatch\\BackGround.bmp";
			BmpInfo.Url = DownloadUrl;
			BmpInfo.Url += "/BackGround.bmp";
			AfxBeginThread(DownloadFile,&BmpInfo,THREAD_PRIORITY_NORMAL);
			SetTimer(3,5000,NULL);

		}
		break;
	case 3:
		{
			KillTimer(3);
			if(IsDone)
			{
				RunLunch();
			}
			else if (IsNotDone)
			{
				MessageBox("登录器更新失败，请到官网手动下载更新","提示",MB_OK);
				AfxGetMainWnd()->SendMessage(WM_CLOSE);
			}
		}
		break;
	}
	
}
void RunLunch()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	if(CreateProcess(NULL,(LPSTR)WorkInfo.Path.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle( pi.hProcess );
		CloseHandle( pi.hThread );
	}
	else 
	{
		AfxMessageBox("已完成更新，请重新运行登录器登录");
		HANDLE hProcess = GetCurrentProcess();//get current process
		TerminateProcess(hProcess,0);         //close process
	}
	AfxGetMainWnd()->SendMessage(WM_CLOSE);
}
void GetUrl(std::string str)
{
	int i = str.find_last_of("/");
	str.replace(i,str.length(),"");
	DownloadUrl = str;
}
std::vector<std::string> split(std::string& str,const char* c)
{
	char *cstr, *p;
	std::vector<std::string> res;
	cstr = new char[str.size()+1];
	strcpy(cstr,str.c_str());
	p = strtok(cstr,c);
	while(p!=NULL)
	{
		res.push_back(p);
		p = strtok(NULL,c);
	}
	return res;
}
std::string&   replace_all(std::string&   str,const   std::string&   old_value,const   std::string&   new_value)     
{     
	while(true)   {     
		std::string::size_type   pos(0);     
		if(   (pos=str.find(old_value))!=std::string::npos   )     
			str.replace(pos,old_value.length(),new_value);     
		else   break;     
	}     
	return   str;     
}