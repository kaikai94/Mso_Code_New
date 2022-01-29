// UpDataLuchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UpDataLuch.h"
#include "UpDataLuchDlg.h"
#include <afxinet.h>
#include "MHFile.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUpDataLuchDlg �Ի���
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


// CUpDataLuchDlg ��Ϣ�������

BOOL CUpDataLuchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int nPos = GetCurrentDirectory( MAX_PATH, CurPath);
	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetTimer(1,5000,NULL);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUpDataLuchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
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
		outfs = filesize / 1024;		// �����ļ���С��ǧ�ֽڣ�

		sprintf_s(buf,"%d",outfs);
		FileSize+=buf;	// ��KBΪ��λ��ʽ�ļ���С

		CFile fDestFile(info->Path.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

		int byteswrite;		// д���ļ����ֽ���
		int pos = 0;		// ��ǰ��������λ��
		int nperc,kbrecv;	// �������İٷֱ�,��ȡ�������ݴ�С��KbsΪ��λ��
		double secs,kbsec;	// ��¼����, �ٶȣ�KB/�룩

		//m_Prog->SetRange32(0,filesize);

		while (byteswrite = fTargFile->Read(filebuf, 512))	// ��ȡ�ļ�
		{
			// ���ݿ�ʼʱ���뵱ǰʱ��Ƚϣ���ȡ����
			COleDateTimeSpan dlElapsed = COleDateTime::GetCurrentTime() - dlStart;
			secs = dlElapsed.GetTotalSeconds();
			pos = pos + byteswrite;					// �����µĽ�����λ��
			fDestFile.Write(filebuf, byteswrite);	// ��ʵ������д���ļ�
			//m_Prog->SetPos(pos);

			nperc = pos * 100 / filesize;			// ���Ȱٷֱ�
			kbrecv = pos / 1024;					// ��ȡ�յ�������
			kbsec = kbrecv / secs;					// ��ȡÿ�����ض��٣�KB��
		}
		// ������ɣ��ر��ļ�
		fDestFile.Close();
		IsDone = TRUE;
	}

	catch(CInternetException *IE)
	{
		CString strerror;
		TCHAR error[255];
		IE->GetErrorMessage(error,255); // ��ȡ������Ϣ
		pwnd->SetDlgItemText(IDC_STATIC_TIP,error);
		IsNotDone = TRUE;
	}
	return 0;
}
std::string  MakeUrl(std::string& str)
{
	char buf[32];
	time_t nRand = time(NULL);
	sprintf_s(buf,"?abc=%d",nRand);//���������������ֹ��ȡcookie
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
				MessageBox("��¼������ʧ�ܣ��뵽�����ֶ����ظ���","��ʾ",MB_OK);
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
		AfxMessageBox("����ɸ��£����������е�¼����¼");
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