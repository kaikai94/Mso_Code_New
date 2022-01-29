// PatchMakerDlg.cpp : ʵ���ļ�
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


// CPatchMakerDlg �Ի���
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


// CPatchMakerDlg ��Ϣ�������

BOOL CPatchMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPatchMakerDlg::OnPaint()
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
HCURSOR CPatchMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPatchMakerDlg::OnBnClickedButton1()
{
	char szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	ZeroMemory(szPath, sizeof(szPath));   
	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = "��ѡ����Ҫ�����Ŀ¼��";   
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
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
		AfxMessageBox("��Ч��Ŀ¼��������ѡ��"); 
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
	str.Format("��[%d]�������ļ�",file.size());
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
		str.Format("��[%d]�������ļ�,�Ѵ���[%d]��",file.size(),i+1);
		AfxGetMainWnd()->SetDlgItemText(IDC_STATIC,str);
	}
	ofn.close();
	str.Format("��[%d]�������ļ�,�������",file.size());
	AfxGetMainWnd()->SetDlgItemText(IDC_STATIC,str);
	return 0;
}

