// 4DyuchiFileCreateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "4DyuchiFileCreate.h"
#include "4DyuchiFileCreateDlg.h"
#include ".\4dyuchifilecreatedlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")


#pragma comment(lib, "..\\4DyuchiGXGFunc\\SS3DGFuncN.lib")

// CDyuchiFileCreateDlg �Ի���


DWORD __stdcall UnlocProgresskProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);

CDyuchiFileCreateDlg*	g_pDlg;

CDyuchiFileCreateDlg::CDyuchiFileCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDyuchiFileCreateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDyuchiFileCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDyuchiFileCreateDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BROWN, OnBnClickedBrown)
	ON_BN_CLICKED(IDC_LOOK, OnBnClickedLook)
	ON_BN_CLICKED(IDC_EXTRA, OnBnClickedExtra)
	ON_BN_CLICKED(IDC_ZIP, OnBnClickedZip)
END_MESSAGE_MAP()


// CDyuchiFileCreateDlg ��Ϣ�������

BOOL CDyuchiFileCreateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_EditPath=NULL;

	m_ListFile=NULL;

	m_Msg=NULL;

	m_Progress=NULL;

	m_pFileInfoList=NULL;

	m_pStorage=NULL;

	m_pPackFileHandle=NULL;

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��

	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_EditPath = (CEdit*)GetDlgItem(IDC_PATH);
	
	m_ListFile = (CListBox*)GetDlgItem(IDC_LIST1);

	m_Msg=(CStatic *)GetDlgItem(IDC_MSG);

	m_Progress= (CProgressCtrl*)GetDlgItem(IDC_PROGRESS);

	m_Progress->SetRange(0,100);

	m_Progress->SetStep(1);

	g_pDlg = this;

	CoInitialize(NULL);

	HRESULT hr;

	hr = CoCreateInstance(CLSID_4DyuchiFileStorage,NULL,CLSCTX_INPROC_SERVER,IID_4DyuchiFileStorage,(void**)&m_pStorage);

	if (hr != S_OK)

		__asm int 3
		

	m_pStorage->Initialize(MAX_FILE_ATOM_NUM,1024,64,FILE_ACCESS_METHOD_ONLY_FILE);

	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDyuchiFileCreateDlg::OnPaint() 
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
HCURSOR CDyuchiFileCreateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDyuchiFileCreateDlg::OnBnClickedBrown()
{
	CFileDialog dlg( TRUE, _T( "pak" ), _T( "*.pak" ), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T( "pak�ļ�|*.pak|�����ļ�|*||" ) );

    if ( dlg.DoModal() == IDOK ) 
	{
		m_EditPath->SetWindowText(dlg.GetPathName());
    }
}

void CDyuchiFileCreateDlg::OnBnClickedLook()
{

	UpdateData(TRUE);

	m_ListFile->ResetContent();
	
	m_EditPath->GetWindowText(pPakFileName,256);
	
	DWORD dwLen = lstrlen(pPakFileName);

	if (!dwLen)
	{
        AfxMessageBox("����ѡ��Pak�ļ�",MB_OK);
		return;
	}

	void*	pPackFileHandle = NULL;

	pPackFileHandle = m_pStorage->MapPackFile(pPakFileName);

	if (!pPackFileHandle)
		return;

	m_pPackFileHandle=pPackFileHandle;

	FSPACK_FILE_INFO	fileInfo;

	m_pStorage->GetPackFileInfo(pPackFileHandle,&fileInfo);

    m_pStorage->CreateFileInfoList(pPackFileHandle,&m_pFileInfoList,MAX_FILE_ATOM_NUM);

	for(int i=0;i<fileInfo.dwFileItemNum;i++)
	{
		m_ListFile->InsertString(i,m_pFileInfoList[i].pszFileName);
	}

	m_pStorage->UnmapPackFile(m_pPackFileHandle);


}


void CDyuchiFileCreateDlg::OnBnClickedExtra()
{
	CButton  * m_BtnExtra= (CButton*) GetDlgItem(IDC_EXTRA);

	m_BtnExtra->EnableWindow(FALSE);

	if(m_pPackFileHandle)
	{
		void*	pPackFileHandle = NULL;

		pPackFileHandle = m_pStorage->MapPackFile(pPakFileName);

		if (!pPackFileHandle)
			return;

		m_pPackFileHandle=pPackFileHandle;

		m_pStorage->ExtractAllFilesFromPackFile(m_pPackFileHandle);

		m_pStorage->UnlockPackFile(m_pPackFileHandle,UnlocProgresskProc);

		m_pStorage->UnmapPackFile(m_pPackFileHandle);
	}
	else
	{
       AfxMessageBox("�����г�Pak�ļ���ִ�д˲���",MB_OK);
	}

	m_BtnExtra->EnableWindow(TRUE);
}


void CDyuchiFileCreateDlg::OnBnClickedZip()
{
	m_ListFile->ResetContent();

	m_EditPath->GetWindowText(pPakFileName,256);
	
	DWORD dwLen = lstrlen(pPakFileName);

	if (!dwLen)
	{
        AfxMessageBox("��ѡ��Ҫ�����Pak�ļ�");

		return;
	}

	CString    strFolderPath;  
    BROWSEINFO broInfo = {0};  
    TCHAR       szDisName[MAX_PATH] = {0};  
  
    broInfo.hwndOwner = this->m_hWnd;  
    broInfo.pidlRoot  = NULL;  
    broInfo.pszDisplayName = szDisName;  
    broInfo.lpszTitle = _T("ѡ��·��");  
    broInfo.ulFlags   = BIF_NEWDIALOGSTYLE | BIF_DONTGOBELOWDOMAIN  
        | BIF_BROWSEFORCOMPUTER | BIF_RETURNONLYFSDIRS | BIF_RETURNFSANCESTORS;  
    broInfo.lpfn      = NULL;  
    broInfo.lParam    = NULL;  
    broInfo.iImage    = IDR_MAINFRAME;  
  
    LPITEMIDLIST pIDList = SHBrowseForFolder(&broInfo);  

    if (pIDList != NULL)  
    {  
        memset(szDisName, 0, sizeof(szDisName));  
        SHGetPathFromIDList(pIDList, szDisName);  
        strFolderPath = szDisName; 
		DeleteFile(pPakFileName);  //ɾ��ԭ�е�Pak�ļ�
		BrowseCurrentAllFile(strFolderPath);
		CreatePakFile(pPakFileName);
    
	}

}

void CDyuchiFileCreateDlg::BrowseCurrentAllFile(CString strDir)
{
	if(strDir == _T(""))
     {
         return;
     }
     else
     {
         if(strDir.Right(1) != _T("//"))
              strDir += L"//";
         strDir =strDir+_T("*.*");
     }
     CFileFind finder;
     CString strPath;
     BOOL bWorking = finder.FindFile(strDir);
     while(bWorking)
     {
         bWorking = finder.FindNextFile();
         strPath = finder.GetFilePath();
         if(finder.IsDirectory() && !finder.IsDots())
		 {
             BrowseCurrentAllFile(strPath); //�ݹ����
		 }
         else if(!finder.IsDirectory() && !finder.IsDots())
         {
			int dotPos=strPath.ReverseFind('.');

            CString fileExt=strPath.Right(strPath.GetLength()-dotPos);

			if(StrStrI(fileExt,".pak")==NULL && StrStrI(fileExt,".tmp")==NULL) //pak �� tmp �ļ�Ĭ���ų����������pak�ļ�����
		    {
				m_ListFile->AddString(strPath);
			}

         }
 
     }
}


DWORD __stdcall UnlocProgresskProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg)
{
	return 	g_pDlg->UnlockProc(dwCurCount,dwTotalCount,pArg);
}

DWORD CDyuchiFileCreateDlg::UnlockProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg)
{
	DWORD	dwPercentage = (DWORD)( (float)(dwCurCount + 1) / (float)dwTotalCount * 100.0f );

	m_Progress->SetPos(dwPercentage);

	RedrawWindow(NULL,NULL,RDW_UPDATENOW);

	return 0;

}

HANDLE CDyuchiFileCreateDlg::CreatePakFile(char * szPackFileName)
{

	I4DyuchiFileStorage  *	p_pStorage;

	HRESULT hr;

	hr = CoCreateInstance(CLSID_4DyuchiFileStorage,NULL,CLSCTX_INPROC_SERVER,IID_4DyuchiFileStorage,(void**)&p_pStorage);

	if (hr != S_OK)

		__asm int 3
		

	p_pStorage->Initialize(MAX_FILE_ATOM_NUM,1024,64,FILE_ACCESS_METHOD_ONLY_FILE);

	HANDLE m_PakHandle=NULL;

	void*	pPackFileHandle = NULL;

	pPackFileHandle = m_pStorage->MapPackFile(pPakFileName);

	if (!pPackFileHandle)
	{
		return NULL;
	}

	char szFileName[_MAX_PATH];

	CFileStatus fileStatus;

	char m_MsgText[300];

	UpdateData(TRUE);

	p_pStorage->LockPackFile(pPackFileHandle,0);

	for(int i=0; i<m_ListFile->GetCount() ; i++)
	{
		CString dwFilePath;

		m_ListFile->GetText(i,dwFilePath);

		memcpy(szFileName,dwFilePath,_MAX_PATH);

		CFile::GetStatus(szFileName, fileStatus);

	    DWORD	dwSize= fileStatus.m_size;

		if (dwSize)
		{
			p_pStorage->InsertFileToPackFile(pPackFileHandle,szFileName);

			sprintf(m_MsgText,"���ڴ��:%s",szFileName);

			m_Msg->SetWindowText(m_MsgText);

			p_pStorage->UnlockPackFile(pPackFileHandle,UnlocProgresskProc);
		}

	}

	sprintf(m_MsgText,"�ļ�������");

	m_Msg->SetWindowText(m_MsgText);

	p_pStorage->UnmapPackFile(pPackFileHandle);

    return pPackFileHandle;

}
